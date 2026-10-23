# Rate limiter: shuru se, ek kahani

Kram: **FR -> NFR -> numbers -> design**. Design sabse seedhe dabbe se shuru, har dikkat pe ek dabba judta.
Peela = abhi juda. Kahani ek hi: Arpan ka app, login pe ek bot.

**CUE (yahi kahani, yahi kram):** server me ginti -> Redis (ek ginti) -> token bucket -> Lua (atomic) ->
gateway pe -> copy + fail-open / closed -> local had / N -> Kafka + ban -> userId (login pe IP)
-> global cap 503 -> home region

---

## 1. FR: kya karna hai

```
1. har user ki had: login 5 / min, baaki API 100 / min
2. had paar -> 429 (Too Many Requests) + "kitni der baad aao" (Retry-After)
scope ke bahar (poochh ke hatao): plan ke hisaab se alag had (free / pro)
```

## 2. NFR: kaisa chalna chahiye

```
- TEZ           : har request ke aage baitha, ~1 ms me haan / na. Dheema = poori site dheemi
- ginti EK      : 10 server hon, ginti sabki ek hi jagah
- limiter gire to site na gire
```

## 3. Numbers

```
- ~1 crore user, peak ~10,000 request / sec
- har user ke 2 chhote number (kitne bache, aakhri baar kab) -> 1 crore x ~50 byte = ~500 MB
```

Dikkat jagah ki nahi, SPEED ki. 500 MB ek machine ki RAM me aa jaata, isliye aage RAM wala Redis, DB nahi.

---

## 4. Design

### Kadam 1: sabse seedha (server ke andar ginti)

App server ki memory me ek map: `user -> is minute kitni request`.

```
request aayi -> map[arpan]++ -> 5 se zyada? -> 429
                                nahi        -> aage jaane do
minute badla -> map saaf
```

```mermaid
flowchart LR
    U["Arpan"] --> S["App server<br/>map: arpan -> 3"]
    S --> K["kaam"]
    style S fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Ek server tak chalta: tez, aur sab ek hi jagah gina ja raha.

Par bheed badhi, 10 App server lage, aage LB. Arpan ki 5 login App 1 pe, 5 App 2 pe. Dono ne alag gina
("5 hi to hain"), 10 andar. Had toot gayi.

### Kadam 2: kai server, ginti ek jagah (Redis)

Ilaaj: ginti sabki EK jagah, **Redis**. Har App har request pe Redis se poochhe.

```
key   = rl:arpan:login          (kaun user + kaunsa kaam)
INCR  -> 1, 2, 3 ...            (Redis me +1, nayi ginti wapas)
umar  = 60 sec (TTL)            -> minute khatam, key khud gayab = ginti reset, safai ka job nahi
6 aayi -> 429
```

```mermaid
flowchart LR
    U["Arpan"] --> LB["LB"]
    LB --> S1["App 1"] & S2["App 2"]
    S1 & S2 -- "INCR rl:arpan:login" --> R[("Redis<br/>arpan:login = 6")]
    style R fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Redis kyun, DB kyun nahi? NFR: ~1 ms, 10,000 / sec. DB disk pe, har baar kai ms, site dheemi.
Redis RAM me ~1 ms, aur data sirf 500 MB, RAM me aa jaata.

Par "har minute 5" = ghadi ka minute (10:00 - 10:01). Bot ne 10:00:59 pe 5 bheji, 10:01:00 pe phir 5.
Ek second me 10 andar, kyunki minute badalte hi ginti 0.

### Kadam 3: minute ke kinare pe dugni (kaunsa tareeka gine)

Ab tak ghadi ka minute (**fixed window**): 10:00:59 pe 5 + 10:01:00 pe 5 = ek second me 10.

```
SLIDING LOG     har request ka time likho, nayi aaye to pichhle 60 sec wale gino
                -> sabse sahi, par har request ka time = memory bahut (1 crore user)
SLIDING COUNTER is minute ki ginti + pichhle minute ka bacha hissa (2 number)
                -> lagbhag sahi, sasta
LEAKY BUCKET    request line me, fix raftaar se nikalti (nal se boond)
                -> smooth, par jhatka (burst) nahi nikalne deta
TOKEN BUCKET    baalti me max 5 token, har 12 sec 1 bharta, har request 1 leti, khaali -> 429
                -> ISLIYE YE
```

```
token bucket kyun?  -> sirf 2 number (tokens, aakhri baar) = wahi 500 MB
                    -> jhatka chalta: 1 ghanta baad aaya to 5 ek saath (asli user aise hi)
                    -> kinara nahi: khaali hui to agla token 12 sec baad, minute se reset nahi
                    -> AWS / Stripe yahi
bharta kaun?        -> koi job nahi. Request aane pe hisaab (lazy refill):
                       tokens = min(5, tokens + (abhi - aakhri baar) x raftaar)
                       1 ghanta baad: 300 bante, par baalti 5 ki -> 5
```

```mermaid
flowchart LR
    U["Arpan"] --> G["App x 10"]
    G -- "token lo" --> R[("Redis<br/>arpan:login<br/>tokens = 4 · aakhri = 10:00:05")]
    R -- "khaali" --> X["429"]
    style R fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Par bot ne ek saath 20 login bheje. 20 App ne ek saath padha "tokens = 5",
sabne socha "bache hain", sabne 1 ghata ke likha.
20 andar, jabki 5 jaane the.

### Kadam 4: 20 ek saath aaye, sab andar (atomic, Lua script)

Har request 3 kaam: padho -> jaancho -> likho. 20 ek saath:

```
App 1:  padha 5 -> "bache hain" -> likha 4
App 2:  padha 5 -> "bache hain" -> likha 4      (App 1 ka likha abhi dikha hi nahi)
...
App 20: padha 5 -> "bache hain" -> likha 4      -> 20 andar, had 5
```

Ilaaj: teeno kaam EK kadam me (**atomic**). Redis me **Lua script**: refill + jaanch + ghatao, ek jhatke me.
Redis ek baar me ek hi command chalata (single thread), script ke beech koi ghus nahi sakta.
20 line me: pehli 5 ko token, baaki 15 ko 429.

```mermaid
flowchart LR
    B["bot: 20 ek saath"] --> S["App x 20"]
    S -- "Lua: padho + jaancho + likho<br/>ek jhatke me" --> R[("Redis<br/>ek-ek karke chalata")]
    R -- "5 haan, 15 na" --> S
    style R fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

```
lock lagao?   -> har request pe lock lo + chhodo = 2 extra chakkar, lock wala atka to sab ruke. 1 ms toota
sirf INCR?    -> atomic hai par sirf ginti. Token bucket me refill phir ghatana = do kaam, isliye Lua
```

Par limiter App ke andar. Bot ki 15 mana wali bhi LB se App tak aayi, App ne Redis poochha, tab 429.
Mana karne me bhi hamare App ka time aur machine kharch.

### Kadam 5: mana wali bhi andar tak aayi (limiter darwaze pe)

Ilaaj: limiter **sabse aage, API Gateway** pe. Mana wali App tak pahunchti hi nahi.

```
pehle:  bot -> LB -> App (Redis poochha) -> 429      App ka time gaya
ab:     bot -> API Gateway (Redis poochha) -> 429    App tak pahunchi hi nahi
```

```mermaid
flowchart LR
    U["Arpan / bot"] --> G["API Gateway<br/>limiter yahan"]
    G -- "had me" --> LB["LB"] --> S["App x 10"]
    G -- "had paar" --> X["429"]
    G -- "Lua" --> R[("Redis")]
    style G fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

```
har App me library kyun nahi?  -> har service (login, payment, search) apna code likhe.
                                  Gateway = ek jagah, sab pe chale, aur bot ka bojh App pe nahi
AWS pe                         -> API Gateway me throttling pehle se. Har user ki alag had = apna limiter +
                                  Redis (ElastiCache)
```

Par sab Redis pe tika. Dopahar Redis gira. Limiter haan bole ya na? Sabko 429 = poori site band.

### Kadam 6: Redis gira (copy + fail-open / fail-closed)

Ilaaj 1: **Redis ki copy**. Redis gira to copy jagah leti (ElastiCache khud, kuch second me).

Un kuch second me?

```
FAIL-OPEN    Redis nahi -> sabko jaane do    -> site chalti, bas kuch second bina had
FAIL-CLOSED  Redis nahi -> sabko 429         -> bot nahi ghusa, par site band

aam API (search, feed) -> fail-open. Bina had chhota nuksaan, site band bada
                          (NFR: limiter gire to site na gire)
login / OTP            -> fail-closed. Yahin bot password ka andaza lagata, OTP pe SMS ka paisa
```

```mermaid
flowchart LR
    G["API Gateway"] -- "Lua" --> R[("Redis (gira)")]
    R -.-> RC[("Redis copy<br/>jagah leti")]
    G -- "Redis nahi: search / feed" --> O["jaane do (fail-open)"]
    G -- "Redis nahi: login / OTP" --> C["429 (fail-closed)"]
    style RC fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style O fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style C fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Par raat ko attack, aur usi waqt Redis + copy dono gaye. Fail-open se aam API bina had, bot andar.

### Kadam 7: Redis + copy dono gaye (har gateway apni ginti)

Ilaaj: Redis wapas aane tak har gateway machine **apni memory me ginti**, had baant ke:

```
asli had = 100 / min, gateway = 10 machine  ->  har machine 100 / 10 = 10 / min   (had / N)
LB barabar baantta, to kul milaake lagbhag 100. Poori sahi nahi, lagbhag sahi
fail-open    -> site chalti, had hi nahi (bot ghusa)
local had/N  -> site chalti, had lagbhag sahi    <- ISLIYE YE
Redis wapas  -> sab phir Redis se
```

```mermaid
flowchart LR
    U["Arpan / bot"] --> LB["LB"]
    LB --> G1["Gateway 1<br/>apni ginti: 10 / min"] & G2["Gateway 2<br/>apni ginti: 10 / min"]
    G1 -. "Redis + copy gaye" .-> R[("Redis (gira)")]
    G2 -.-> R
    style G1 fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style G2 fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style R fill:#ffe3e3,stroke:#e03131,stroke-width:2px,color:#000
```

Par wahi bot har 60 sec baad wapas: 429 khaata, minute khatam, phir aata. Din bhar.

### Kadam 8: wahi bot baar-baar (pattern -> ban)

Ilaaj: har 429 ki khabar **Kafka** me. Peeche ek Pattern Svc dekhta:
"ye 1 ghante me 50 baar had paar kar chuka" -> **ban**.
Ban list gateway / WAF pe, ban wala darwaze pe hi ruk jaata (Redis tak bhi nahi).

```
Kafka kyun?      -> 429 dena tez rahe (1 ms). Pattern dhoondhna peeche, gateway ruke bina
ban kitni der?   -> hamesha nahi, kuch der (TTL, jaise 1 ghanta, baar-baar aaya to badhao)
IP pe kyun der?  -> IP badalte rehte / ek IP pe kai log (office, mobile network). Hamesha ka ban
                    = kal wahi IP kisi asli user ko mila to wo atka
```

```mermaid
flowchart LR
    U["bot"] --> W["WAF / gateway<br/>ban list (TTL)"]
    W --> G["API Gateway<br/>limiter"]
    G -- "429 ki khabar" --> K[("Kafka")]
    K --> P["Pattern Svc<br/>baar-baar? -> ban"]
    P -- "ban (1 ghanta)" --> W
    style K fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style P fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style W fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Par had IP pe lagayi thi. Office ki night shift: 1,000 log ek hi IP se (office ka internet).
Sab milke 100 / min paar, poora office atka.

### Kadam 9: had kis naam pe (IP ya userId)

IP pe had: office ke 1,000 log ek IP, sab atke. Ulta, bot har request alag IP se, had kabhi nahi lagti.

Ilaaj: had **userId** pe (login ke baad JWT se). Har insaan ki apni had, IP ek ho ya hazaar.

```
login se pehle   -> IP + jis account pe koshish   (rl:ip:1.2.3.4:login, rl:acct:arpan:login)
                    login ki had chhoti (5 / min), office ke log alag-alag login karte nikal jaate
login ke baad    -> userId                       (rl:arpan:search)
ban (kadam 8)    -> userId pe lamba, IP pe chhota (TTL)
```

```mermaid
flowchart LR
    U["Arpan / bot"] --> W["WAF<br/>ban list (TTL)"]
    W --> G["API Gateway<br/>key = userId (login pe IP)<br/>Lua token bucket"]
    G -- "Lua" --> R[("Redis + copy")]
    G -- "had me" --> S["App x 10"]
    G -- "429 ki khabar" --> K[("Kafka")]
    K --> P["Pattern Svc"]
    P -- "ban" --> W
    style G fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Par shaam ki sale: har user apni had ke andar, par 1 crore log ek saath.
Sab "had me", phir bhi server bhar ke gira.

### Kadam 10: sab had me, phir bhi server gira (global cap + 503)

Ilaaj: **poore system ki ek had (global cap)**. Servers maan lo 50,000 / sec jhelte.
Gateway pe ek aur ginti: is second KUL kitni aayi. Upar wali ko **503** (abhi bhare hain, baad me aao).

```
user ki had   (429)  -> "TU zyada bhej raha"     ek insaan ko rokta
global cap    (503)  -> "HUM abhi bhare hain"    poore system ko bachata

kaun pehle kate (load shedding):
  pehle kato    -> search suggestion, recommendation, analytics
  aakhri tak    -> payment, order, login
aur             -> sale ka time pata, to pehle se server badhao (pre-scale)
                   phir bhi zyada -> line / waiting room ("aap line me 2,000ve")
```

```mermaid
flowchart LR
    U["1 crore log"] --> W["WAF"]
    W --> G["API Gateway<br/>1. user had (429)<br/>2. global cap (503)"]
    G -- "zaroori: payment" --> S["App x 10"]
    G -- "bhara: suggestion" --> X["503 / baad me"]
    G -- "Lua" --> R[("Redis + copy")]
    style G fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style X fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Par Arpan ki script 3 region se chali (Mumbai, Singapore, US). Har region ka apna Redis, har ek ne 100 gine.
Had 100, nikal gayi 300.

### Kadam 11: 3 region se 300 nikal gayi (home region)

Kadam 2 wali dikkat, bas server ki jagah region alag-alag gin rahe.

```
sab ek Redis se poochhein    -> sahi, par US se Mumbai = ~200 ms har request. 1 ms toota
had baant do (100 / 3 = 33)  -> tez, par sirf Mumbai se aaye to bhi 33 hi (kadam 7 jaisa had / N)
HOME REGION                  -> har user ka ek ghar region (jahan account bana), ginti sirf wahin
                                <- ISLIYE YE
```

```
Arpan (ghar Mumbai) Mumbai se  -> Mumbai Redis, 1 ms
Arpan ki script US se          -> US gateway Mumbai Redis se poochhe (~200 ms)
asli user zyadatar apne region se (1 ms). Bahar se kabhi-kabhi, aur wahi aksar bot, uska dheema chalta
```

```mermaid
flowchart LR
    M["Mumbai gateway"] --> RM[("Mumbai Redis<br/>Arpan ki ginti")]
    S["Singapore gateway"] -- "Arpan? ghar Mumbai" --> RM
    US["US gateway"] -- "Arpan? ghar Mumbai" --> RM
    style RM fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Bolo: "Har user ka home region. Ginti wahin. Bahar se aaye to home se poochho, thoda dheema, par sahi."

---

## 5. Wrap (30 sec, NFR se milao)

```
User -> WAF (ban list, TTL) -> API Gateway: user had (429) + global cap (503)
     -> Lua token bucket in Redis (key = userId, login pe IP · home region · copy)
     -> had me? App : 429 + Retry-After
429 ki khabar -> Kafka -> Pattern Svc -> ban -> WAF

TEZ (1 ms)          -> Redis RAM me, gateway pe, ek Lua jhatka
ginti EK            -> Redis (kai server) + home region (kai region) + Lua (ek saath 20)
gire to site chale  -> copy, fail-open (aam) / fail-closed (login), local had / N
```

---

## Pooche to (ek line)

```
had kahan se aati (free / pro)?  -> DB me user ka plan (bina login 60 / ghanta, free 5,000, pro 10,000),
                                    gateway cache kare. Redis me usi had se milao
had badalni ho?                  -> niyam config me (DB / AppConfig), gateway ~30 sec me padhe. Naya code nahi
429 pe kya bhejein?              -> Retry-After (kitne sec baad, thoda aage-peeche 28 / 33 taaki sab ek saath
                                    na lautein) + X-RateLimit-Remaining (kitne bache)
tumhe kisi ne 429 diya to?       -> Retry-After maano, ruk-ruk ke dobara (backoff + jitter),
                                    apni raftaar ghatao
100 halki vs 100 bhaari?         -> limiter ko dono "100". Bhaari (report) pe kadi had, ya wazan (report =
                                    50 token, login = 1)
10 guna traffic?                 -> gateway machine badhao · Redis shard (user ke hisaab se) + copy
kaise pata chal raha?            -> limiter ka time (p99), kitne 429 / 503, Redis galtiyan, alert
AWS naam                         -> Redis = ElastiCache · gateway = API Gateway (throttling) · WAF = AWS WAF
```

---

## HANDS-ON #1 — Nginx limiter LIVE (21-Aug, isi folder ki `nginx.conf`)

```
docker run -d --name rl -p 8080:80 -v "<path>\nginx.conf:/etc/nginx/nginx.conf:ro" nginx
   -d background · --name rl · -p laptop 8080 -> container 80 · -v apni conf andar MOUNT (ro) · nginx = image

nginx.conf ki 2 asli line:
   limit_req_zone $binary_remote_addr zone=mylimit:10m rate=1r/m;    <- har IP ka bucket, refill 1 / min
   limit_req zone=mylimit burst=5 nodelay;                           <- bucket SIZE 5, burst turant serve
   rate = kis speed se nikalti · burst = kitni ruk sakti
   + location / { ... root /usr/share/nginx/html; index index.html; }  <- content serve
   koi program NAHI likha — sirf ready tool (Nginx, rate limiter built-in) on kiya + config + hammer -> 503
   SACH: nginx docs isko LEAKY BUCKET kehte; nodelay se bartaav
         token bucket JAISA. 503 = limit_req_status default.

HAMMER:  for /L %i in (1,1,30) do @curl -s -o nul -w "%{http_code} " http://localhost:8080
         (1,1,30) = 30 baar · -s -o nul = chup, body phenk do · -w = sirf status code
DIKHA:   200 200 200 200 200 200 503 503 503 ...
LOG:     (Docker Desktop -> Containers -> rl -> Logs) "GET / HTTP/1.1" 503 197
         [error] limiting requests, excess: 5.774 by zone "mylimit", client: 172.17.0.1
         (excess = bucket se kitna upar · client = Docker gateway IP, sab ek IP = ek bucket)

GOTCHA:  1. 503 tabhi jab ARRIVAL > rate (rate=2r/s pe dheere loop -> sab 200, refill pakad leta)
         2. burst=N -> spike me N+1 pass (burst=5 -> 6, 100 ->
            101); textbook N, nginx N+1, timing se 21 / 22 bhi
         3. 1 IP = 1 bucket
         4. config badla -> docker restart rl
CHALAO:  docker start rl · curl http://localhost:8080 · docker logs rl · docker restart rl · docker stop rl
```

---

## HANDS-ON #2 — App ke andar fixed window (usercrud, 26-Aug)

```java
@RestController
public class RateLimitController {
    private int LIMIT = 5;
    private long windowStart = 0;
    private int count = 0;

    @GetMapping("/rate-demo")
    public synchronized ResponseEntity<String> hit() {          // synchronized: shared counter
        long now = System.currentTimeMillis();
        if (now - windowStart > 1000) { windowStart = now; count = 0; }   // window reset
        count++;
        if (count > LIMIT)
            return ResponseEntity.status(HttpStatus.TOO_MANY_REQUESTS).body("429 - limit paar");
        return ResponseEntity.ok("OK - request #" + count);
    }
}
// SecurityConfig: .requestMatchers("/rate-demo").permitAll()
// ResponseEntity KYUN: status-code (200/429) khud set karne ko
```
```
LIVE:  1..10 | % { curl.exe -s http://localhost:8080/rate-demo; "" }
       OK #1..#5 · 429 (6..10) · 1 sec baad window reset -> phir OK
EDGE:  fixed window boundary: 999ms pe 5 + 1001ms pe 5 = ~2ms me 10
PROD:  single node memory -> restart pe reset + har server apna count -> Redis INCR + EXPIRE
       per user / IP -> map<key, counter> · lib: Bucket4j (token bucket) / Redis + Lua
JAWAB:    usercrud me khud banaya: 5 request theek, chhathi pe 429.
          Asli system me token bucket (Bucket4j library) + sab servers ka saanjha Redis counter. Nginx
          bhi ye kaam sabse aage kar sakta (limit_req).
```

---

## HANDS-ON #3 — Redis KHUD maara: kaise tootta

CODE: `04_HLD/HANDS_ON/01_rate_limiter_redis/RateLimiterDemo.java`
· `04_HLD/HANDS_ON/02_incr_expire_crash/IncrExpireCrash.java`
(koi library nahi, socket se Redis, `java File.java`)

```
docker run -d --name demo-redis -p 6390:6379 redis:7 redis-server --save "" --appendonly no
   (RDB + AOF band -> sirf memory, restart = sab gaya, jaan-boojh ke)
docker stop / start demo-redis · docker exec demo-redis redis-cli TTL rl:user:1 · ... DEL rl:user:1

CODE KA DIL (user:1 ko 60 sec me 5):
   count = INCR KEY · if count == 1 -> EXPIRE KEY 60 · count <= 5 ALLOW else 429
   catch IOException -> FAIL_OPEN ? ALLOW : BLOCK
```
```
CASE A  chalta kaise  req 1..9:  count 1 TTL 60 ALLOW · 2/57 ALLOW · 3/56 ALLOW · 4/49 ALLOW · 5/48 ALLOW ·
                                 6/47 429 · 7/45 429 · 8/45 429 · 9/44 429     (window PEHLI request se)

CASE B  docker stop   "REDIS SE BAAT NAHI HUI: Connection refused" -> FAIL_OPEN = true -> ALLOW (2 baar)
                      fail-open = site chale, limit nahi · fail-closed = abuse nahi, site band
                      ("server ne haath khade kar diye, sab jao")

CASE C  docker start  GET rl:user:1 -> khaali · user (pehle count 7, BLOCK) -> count 1 ALLOW, 2 ALLOW
                      persistence band -> ginti gayab -> blocked ko 5 nayi mil gayi

CASE D  INCR ke baad, EXPIRE se pehle crash (USE_LUA = false)
                      RUN 1: count 1, CRASH · TTL -> -1
                      RUN 2: count 2..5 ALLOW, 6..7 429, har baar TTL
                             -1 -> key KABHI nahi mitegi, HAMESHA block
FIX D   USE_LUA = true   req 10..14: count 10 TTL 58 · 11/57 · 12/56 · 13/56 · 14/55 -> 1 min me free

TTL:  -1 = key hai, timer nahi · -2 = key hi nahi · 58 = 58 sec baaki
```
```
FIX:  B -> faisla pehle se: fail-open (aam API) / fail-closed (login, OTP) + replica
      C -> replica / AOF on. limiter me aksar chalne do (1 min ki ginti, paisa nahi). paisa hota to nahi.
      D -> Lua: INCR + EXPIRE ek command, Redis single-thread -> script beech me nahi rukti
           (EXPIRE NX se nahi bachta: wo bhi alag call hai, beech me crash wahi)
           pehle:  app --INCR--> Redis  [crash]  app --EXPIRE--> Redis
           Lua:    app --EVAL(INCR + EXPIRE)--> Redis
```
```lua
local c = redis.call('INCR', KEYS[1])
if c == 1 then redis.call('EXPIRE', KEYS[1], ARGV[1]) end
return c
```
```
JAWAB:    Docker Redis khud band karke teen cheez dekhi:
          1) Redis band: aam API chalti rahi, login / OTP ruk gaye.
          2) Redis restart: saari ginti zero ho gayi. Limiter me chalta hai, copy rakho to kam hota.
          3) Ginti +1 aur 'umar lagao' alag-alag bheje aur beech me crash hua: key bina umar ke reh
          gayi, user hamesha ke liye block. Isliye dono ek Lua script me ek saath.
```
