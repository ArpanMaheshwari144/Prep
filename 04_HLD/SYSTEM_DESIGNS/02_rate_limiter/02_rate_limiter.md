# Rate limiter: shuru se, ek kahani

**Mock se pehle hand notes:** [RATE_LIMITER_HAND_NOTES.md](RATE_LIMITER_HAND_NOTES.md)

Kram wahi jo interview me hota hai: **FR, phir NFR, phir numbers, phir design**. Design sabse seedhe dabbe se
shuru hota hai, aur har dikkat aane pe ek naya dabba judta hai. Diagram me peela dabba = abhi juda. Poori file
me ek hi kahani hai: Arpan ka app, aur uske login pe hamla karta ek bot.

**CUE (yahi shabd yaad rakhne hain, kram se):**
server me ginti -> Redis (ek ginti) -> token bucket -> Lua (ek saath 20) -> gateway pe
-> copy + fail-open / closed -> Kafka + ban -> userId (login pe IP)
-> global cap 503 -> home region

---

## 1. FR: kya karna hai

```
1. har user ki had: login 5 baar / minute, baaki API 100 / minute
2. had paar ho to 429 (Too Many Requests), saath me "kitni der baad aao" (Retry-After)
scope ke bahar (poochh ke hata do): plan ke hisaab se alag had (free / pro), ye baad me
```

## 2. NFR: kaisa chalna chahiye

```
- TEZ: limiter har request ke aage baitha hai, ~1 ms me haan ya na. Ye dheema to poori site dheemi
- ginti EK: 10 server hon tab bhi har user ki ginti ek hi jagah
- limiter gire to site na gire
```

## 3. Numbers

```
- ~1 crore user, sabse bheed wale waqt ~10,000 request har second
- har user ke liye sirf 2 chhote number (kitne bache, aakhri baar kab) -> 1 crore x ~50 byte = ~500 MB
```

Matlab dikkat jagah ki nahi, SPEED ki hai. 500 MB ek machine ki RAM me aa jaata hai. Isliye aage RAM wala
Redis aayega, DB nahi.

---

## 4. Design

### Kadam 1: sabse seedha (server ke andar ginti)

App server ki memory me ek map rakho: `user -> is minute kitni request aayi`.

```
request aayi -> map[arpan] me +1 -> 5 se zyada? -> 429
                                    nahi        -> aage jaane do
minute badla -> map saaf
```

```mermaid
flowchart LR
    U["Arpan"] --> S["App server<br/>map: arpan -> 3"]
    S --> K["kaam"]
    style S fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Jab tak server ek hai, ye chalta hai: tez hai, aur sab ek hi jagah gina ja raha hai.

Par bheed badhi to 10 app server lag gaye, aur aage LB. Ab Arpan ki 5 login App 1 pe gayi aur 5 App 2 pe. Dono
ne alag-alag gina, dono ko laga "5 hi to hain", aur 10 andar chali gayi. Had toot gayi.

### Kadam 2: kai server, ginti ek jagah (Redis)

Ilaaj: sabki ginti **ek jagah** rakho, yaani Redis me. Har app server har request pe Redis se poochhta hai.

```
key   = rl:arpan:login          (kaun user + kaunsa kaam)
INCR  -> 1, 2, 3 ...            (Redis me ginti +1 karo, nayi ginti wapas aati hai)
umar  = 60 sec (TTL)            -> pehli request pe lagti; 60 sec baad key apne aap gayab = ginti reset
6vi aayi -> 429
```

```mermaid
flowchart LR
    U["Arpan"] --> LB["LB"]
    LB --> S1["App 1"] & S2["App 2"]
    S1 & S2 -- "INCR rl:arpan:login" --> R[("Redis<br/>arpan:login = 6")]
    style R fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Redis kyun, DB kyun nahi? NFR kehta hai ~1 ms, aur har second 10,000 request. DB disk pe likhta hai, har baar
kai ms lagte, poori site dheemi ho jaati. Redis RAM me hai, ~1 ms. Aur numbers me nikla tha ki data sirf 500
MB hai, RAM me aa jaata hai.

Umar (TTL) isliye ki 60 sec baad key khud mit jaaye, safai ke liye koi job na chalana pade.

Ye "minute" ghadi ka nahi, Arpan ki pehli request se shuru hota hai (10:00:20 pe pehli aayi to 10:01:20 tak).
Par kinare pe dikkat wahi: bot ne 10:01:19 pe 5 bheji (window ka aakhri second) aur 10:01:20 pe phir 5 (nayi window
ka pehla second). Ek second ke andar 10 andar chali gayi, kyunki umar khatam hote hi ginti 0 ho gayi.

### Kadam 3: minute ke kinare pe dugni (kaunsa tareeka gine)

Ab tak ek band khidki (60 sec) me gin rahe the, jo khatam hote hi 0 se shuru hoti hai (isse **fixed window** kehte hain). Gin-ne ke chaar tareeke hain:

```
SLIDING LOG      har request ka time likh lo, nayi aaye to pichhle 60 sec wale gino
                 -> sabse sahi, par har request ka time rakhna = 1 crore user pe bahut memory
SLIDING COUNTER  is minute ki ginti + pichhle minute ka bacha hissa (2 number)
                 -> lagbhag sahi, sasta
LEAKY BUCKET     request ek line me lagti hain aur fix raftaar se nikalti hain (nal se boond jaisi)
                 -> smooth hai, par jhatke me aayi request (burst) ko nahi nikalne deta
TOKEN BUCKET     ek baalti me max 5 token. Har 12 sec me 1 token bharta. Har request 1 token leti.
                 Baalti khaali -> 429
```

**Token bucket isliye:** isme sirf 2 number rakhne padte hain (kitne token bache, aakhri baar kab dekha), jo
wahi 500 MB hai. Jhatka bhi chalta hai: Arpan 1 ghanta baad aaya to baalti bhari mili, aur wo 5 ek saath bhej
sakta hai, asli user aise hi bhejta hai. Aur kinare wali dikkat nahi aati: baalti khaali hui to agla token 12
sec baad hi aata hai, minute badalne se kuch reset nahi hota. AWS aur Stripe bhi yahi lagate hain.

Token bharta kaun hai? Koi background job nahi. Request aane pe hi hisaab lagta hai (lazy refill):

```
tokens = min(5, tokens + (abhi - aakhri baar) x raftaar)
          ^ baalti se zyada nahi   ^ beete time me jitne bane
Arpan 1 ghanta baad aaya: beete time me 300 bante, par baalti 5 ki hai, to 5 hi
```

```mermaid
flowchart LR
    U["Arpan"] --> G["App x 10"]
    G -- "token lo" --> R[("Redis<br/>arpan:login<br/>tokens = 4 · aakhri = 10:00:05")]
    R -- "khaali" --> X["429"]
    style R fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Par bot ne ek saath 20 login bheje. 10 app server pe 20 request ne ek saath Redis se padha "tokens = 5", sabne socha "bache
hain", aur sabne 1 ghata ke likh diya. 20 andar chale gaye, jabki 5 hi jaane chahiye the.

### Kadam 4: 20 ek saath aaye, sab andar (Lua script, ek jhatke me)

Har request 3 kaam karti hai: padho, jaancho, likho. Ye teen alag kadam the, aur beech me doosri request ghus
gayi:

```
Request 1:  padha 5 -> "bache hain" -> likha 4
Request 2:  padha 5 -> "bache hain" -> likha 4      (Request 1 ka likha hua abhi dikha hi nahi)
...
Request 20: padha 5 -> "bache hain" -> likha 4     -> 20 andar, jabki had 5 thi
```

Ilaaj: teeno kaam **ek hi kadam** me karo. Redis me ek chhoti **Lua script** bhejo, jo token bharna, jaanchna
aur ghatana, teeno ek jhatke me karti hai. Redis ek baar me ek hi command chalata hai (single thread), to jab
script chal rahi hai, koi doosri request beech me ghus hi nahi sakti. 20 request line me lagti hain: pehli 5
ko token mile, baaki 15 ko 429.

```mermaid
flowchart LR
    B["bot: 20 ek saath"] --> S["App x 10"]
    S <-->|"jaata: Lua (padho + jaancho + likho) ek jhatke me<br/>wapas: 5 haan, 15 na"| R[("Redis<br/>ek-ek karke chalata")]
    style R fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Par limiter app ke andar hai. Bot ki 15 mana wali request bhi LB se hoti hui app tak aayi, app ne Redis se
poochha, aur tab jaake 429 diya. Mana karne me bhi hamare app ki machine aur time kharch hua.

### Kadam 5: mana wali request bhi andar tak aayi (limiter darwaze pe)

Ilaaj: limiter ko **sabse aage, API Gateway pe** lagao. Mana wali request app tak pahunchti hi nahi.

```
pehle:  bot -> LB -> App (Redis se poochha) -> 429      app ka time gaya
ab:     bot -> API Gateway (Redis se poochha) -> 429    app tak pahunchi hi nahi
```

```mermaid
flowchart LR
    U["Arpan / bot"] --> G["API Gateway<br/>limiter yahan"]
    G -- "had me" --> LB["LB"] --> S["App x 10"]
    G -- "had paar" --> X["429"]
    G -- "Lua" --> R[("Redis")]
    style G fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Har app ke andar library kyun nahi? Tab har service (login, payment, search) ko apna limiter code likhna
padta, aur bot ka bojh fir bhi app pe padta. Gateway pe ek jagah lagao, sab pe chalega. AWS pe API Gateway me
throttling pehle se hoti hai. Par har user ki alag had chahiye to apna limiter Redis (ElastiCache) ke saath
lagate hain.

Par ab sab kuch Redis pe tika hai. Dopahar ko Redis gir gaya. Ab limiter kisi request ko haan bole ya na?
Sabko 429 diya to poori site band.

### Kadam 6: Redis gira (copy + fail-open / fail-closed)

Ilaaj 1: **Redis ki copy**. Redis gira to copy turant uski jagah le leti hai. AWS ElastiCache ye apne aap kar
deta hai, kuch second me.

Phir bhi un kuch second me kya karein? Do raaste hain. **Fail-open** matlab Redis nahi mila to sabko jaane do:
site chalti rahi, bas kuch second bina had ke. **Fail-closed** matlab Redis nahi mila to sabko 429: koi bot
nahi ghusa, par poori site band.

Aam API (search, feed) pe fail-open, kyunki kuch second bina had ke chal jaana chhota nuksaan hai, site band
hona bada. NFR bhi kehta hai "limiter gire to site na gire". Login aur OTP pe fail-closed, kyunki yahin bot
password ka andaza lagata hai, aur OTP pe SMS ka paisa lagta hai. Inhe kuch second band rakhna sasta padta
hai.

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

Par wahi bot har 60 second baad wapas aa jaata hai. 429 khaata hai, minute khatam hote hi phir aata hai, aur
din bhar yahi chalta hai.

### Kadam 7: wahi bot baar-baar (pattern pakdo, ban karo)

Ilaaj: har 429 ki khabar **Kafka** me daalo. Peeche ek Pattern Service dekhta hai: "ye 1 ghante me 50 baar had
paar kar chuka hai", to use **ban** kar deta hai. Ban list gateway ya WAF (Web Application Firewall, darwaze ka chowkidar) pe hoti hai, to ban wala darwaze pe
hi ruk jaata hai, Redis tak bhi nahi pahunchta.

Kafka isliye ki 429 dena tez rahe (1 ms). Pattern dhoondhna peeche chalta hai, gateway ko roke bina.

Ban hamesha ke liye nahi, kuch der ke liye hota hai (TTL, jaise 1 ghanta, aur baar-baar aaya to badhao).
Khaaskar IP ka ban, kyunki IP badalte rehte hain, aur ek IP pe kai log hote hain (office, mobile network).
Hamesha ka ban lagaya to kal wahi IP kisi asli user ko mila to wo atak jaayega.

```mermaid
flowchart LR
    U["bot"] --> W["WAF / gateway<br/>ban list (TTL)"]
    W --> G["API Gateway<br/>limiter"]
    G -- "429 ki khabar" --> K[("Kafka")]
    K --> P["Pattern Service<br/>baar-baar? -> ban"]
    P -- "ban (1 ghanta)" --> W
    style K fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style P fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style W fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Par ek naya bot aaya. Uske paas leak hui password list thi. Usne 1,000 alag account pe 4-4 password try kiye
(credential stuffing). Har account pe sirf 4, to account wali had (5 / min) kabhi lagi hi nahi. Isliye IP pe
bhi had jodi. Par office ki night shift me 1,000 log ek hi IP se aate hain (office ka internet), sab milke had
paar kar gaye, aur poora office atak gaya.

### Kadam 8: had kis naam pe (IP ya userId)

IP pe had lagane me do dikkat hain. Office ke 1,000 log ek IP pe hain, to sab atak gaye. Aur ulta, bot har
request alag IP se bheje to IP wali had kabhi lagti hi nahi.

Ilaaj: had **userId** pe lagao, jo login ke baad JWT token se milta hai. Har insaan ki apni had, chahe IP ek
ho ya hazaar.

Par login se pehle to userId hota hi nahi. Isliye login pe do had: **IP** pe, aur jis account pe koshish ho
rahi hai us **account** pe. IP wali dheeli (jaise 500 / min), taaki poora office aaram se login kare, bas ek IP
se hazaaron ki baadh ruke. Account wali kadi (5 / min), taaki ek account pe password guess karne wala ruke.
Office ke 1,000 log alag-alag account hain, har ek ki apni 5, to koi nahi atakta. Ban me bhi yahi: userId pe
lamba ban, IP pe chhota.

```
login se pehle   -> IP (dheeli, 500 / min) + account (kadi, 5 / min)   (rl:ip:1.2.3.4:login, rl:acct:arpan:login)
login ke baad    -> userId                                            (rl:arpan:search)
```

```mermaid
flowchart LR
    U["Arpan / bot"] --> W["WAF<br/>ban list (TTL)"]
    W --> G["API Gateway<br/>key = userId (login pe IP + account)<br/>Lua token bucket"]
    G -- "Lua" --> R[("Redis + copy")]
    G -- "had me" --> S["App x 10"]
    G -- "429 ki khabar" --> K[("Kafka")]
    K --> P["Pattern Service"]
    P -- "ban" --> W
    style G fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Humne kya socha aur kya tay kiya (login pe): pehle login pe IP ki had lagayi thi, par dikkat ye nikli ki
office me kitne log hain ye pata nahi (100 bhi ho sakte, TCS jaise lakh bhi). Had kadi rakho to asli log
atakte, dheeli rakho to bot nikal jaata. Isliye tay kiya: IP pe had nahi (ya bahut dheeli). Asli deewar MFA /
passkey, kyunki password mil bhi gaya to bina phone ke bekaar. Galat password pe block nahi, CAPTCHA, taaki
asli user na atke. Aur cloud / badnaam IP ko WAF pe band kiya. Koi ek perfect hal nahi hai, isliye parat-dar-
parat.

Par shaam ko sale hai. Har user apni had ke andar hai, par 1 crore log ek saath aa gaye. Sab "had me" hain,
phir bhi server bhar ke gir gaya.

### Kadam 9: sab had me, phir bhi server gira (global cap + 503)

User ki had har insaan ko alag rokti hai. Par 1 crore log ek saath aaye, to kul bojh server ki taakat se zyada
ho gaya.

Ilaaj: **poore system ki ek had (global cap)**. Maan lo servers 50,000 request / second jhel sakte hain.
Gateway pe ek aur ginti lagao: is second kul kitni request aayi. Uske upar wali request ko **503** do (abhi
bhar gaye hain, thodi der baad aao).

```
user ki had   (429)  -> "TU zyada bhej raha hai"     ek insaan ko rokta hai
global cap    (503)  -> "HUM abhi bhare hain"         poore system ko bachata hai
```

Sab ek barabar nahi hain, to bhar gaye to pehle kam zaroori kaam chhodo (load shedding): search suggestion,
recommendation aur analytics pehle kato, payment, order aur login aakhri tak chalao. Matlab sale me "aapke
liye suggestion" band ho gaya, par Arpan ka payment chalta raha. Sale ka time pehle se pata hota hai, to
servers pehle se badha do. Phir bhi zyada aaye to line (waiting room), jaise ticket sale me: "aap line me
2,000ve hain".

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

Par Arpan ki script 3 region se chali: Mumbai, Singapore aur US. Har region ka apna Redis tha, har ek ne 100
gine. Had 100 thi, par 300 nikal gayi.

### Kadam 10: 3 region se 300 nikal gayi (home region)

Ye kadam 2 wali hi dikkat hai, bas ab server ki jagah region alag-alag gin rahe hain. Teen raaste hain:

```
sab region ek hi Redis se poochhein  -> sahi ginti, par US se Mumbai Redis = ~200 ms har request. 1 ms toota
had baant do (100 / 3 = 33)          -> tez, par Arpan sirf Mumbai se aaye to bhi use 33 hi milenge
HOME REGION                          -> har user ka ek ghar region (jahan account bana), ginti sirf wahin
```

**Home region isliye:** Arpan ka ghar Mumbai hai. Wo Mumbai se aaya to Mumbai Redis, 1 ms. Uski script US se
aayi to US gateway Mumbai Redis se poochhta hai, ~200 ms. Asli user zyadatar apne hi region se aata hai, to
uske liye 1 ms hi rehta hai. Bahar se aana kabhi-kabhi hota hai, aur aksar wahi bot hota hai, to uska dheema
hona chalta hai.

```mermaid
flowchart TB
    M["Mumbai gateway"] --> RM[("Mumbai Redis<br/>Arpan ki ginti")]
    S["Singapore gateway"] -- "Arpan? ghar Mumbai" --> RM
    US["US gateway"] -- "Arpan? ghar Mumbai" --> RM
    style RM fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Interview me bolo: "Har user ka home region. Ginti wahin. Bahar se aaye to home se poochho, thoda dheema, par
sahi."

---

## 5. Wrap (30 second me poora design, NFR se milao)

Request pehle WAF pe aati hai, jahan ban list hai. Phir API Gateway pe limiter: user ki had (429) aur poore
system ki had (503). Ginti Redis me hai, token bucket ki Lua script se ek jhatke me, key = userId (login pe
IP), user ke home region me, aur Redis ki copy ke saath. Had ke andar hai to app pe, warna 429 aur
Retry-After. Har 429 ki khabar Kafka se Pattern Service tak, jo baar-baar wale ko ban karta hai.

```
TEZ (1 ms)            -> Redis RAM me, gateway pe, ek Lua jhatka
ginti EK              -> Redis (kai server ke liye) + home region (kai region ke liye)
                         + Lua (ek saath 20 ke liye)
gire to site chale    -> Redis ki copy, fail-open (aam API) / fail-closed (login)
```

---

## Pooche to (design nahi badalta, bas ek baar dekh lo)

```
Redis + copy dono gaye?          -> har gateway apni memory me ginti rakhe, had baant ke: 100 / min aur
                                    10 machine -> har machine 10 / min (had / N). LB barabar baantta, to kul
                                    lagbhag 100. Fail-open me had hi nahi thi, yahan lagbhag sahi. Redis
                                    wapas aaya to phir Redis se ginti. (Naya dabba nahi, isliye kadam nahi)
had kahan se aati (free / pro)?  -> DB me user ka plan (bina login 60 / ghanta, free 5,000, pro 10,000).
                                    Gateway cache kare, Redis me usi had se milaye
had badalni ho?                  -> niyam config me (DB / AppConfig), gateway ~30 sec me padhe. Naya code nahi
429 pe kya bhejein?              -> Retry-After (kitne sec baad aao; thoda aage-peeche jaise 28 / 33,
                                    taaki sab ek saath na lautein) + X-RateLimit-Remaining (kitne bache)
tumhe kisi ne 429 diya to?       -> Retry-After maano, ruk-ruk ke dobara (backoff + jitter),
                                    apni raftaar ghatao
100 halki vs 100 bhaari?         -> limiter ke liye dono "100". Bhaari (report) pe kadi had, ya wazan do
                                    (report = 50 token, login = 1)
10 guna traffic?                 -> gateway machine badhao · Redis ko user ke hisaab se shard karo + copy
chal raha, kaise pata?           -> limiter ka time (p99 = 100 me se 99 request isse tez), kitne 429 / 503, Redis ki galtiyan, alert
AWS naam                         -> Redis = ElastiCache · gateway = API Gateway (throttling) · WAF = AWS WAF
```

---

## HANDS-ON #1 — Nginx limiter LIVE (21-Aug, isi folder ki `nginx.conf`)

```
docker run -d --name rl -p 8080:80 -v "<path>\nginx.conf:/etc/nginx/nginx.conf:ro" nginx
   -d background · --name rl · -p laptop 8080 -> container 80 · -v apni conf andar MOUNT (read-only) · nginx = image

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
            101); textbook N, nginx N+1, timing se kabhi ek-do zyada bhi
         3. 1 IP = 1 bucket
         4. config badla -> docker restart rl
CHALAO:  docker start rl · curl http://localhost:8080 · docker logs rl · docker restart rl · docker stop rl
```

---

## HANDS-ON #2 — App ke andar fixed window

Alag file me: [HANDS_ON/05_fixed_window_spring](../../HANDS_ON/05_fixed_window_spring/README.md)

---

## HANDS-ON #3 — Redis KHUD maara: kaise tootta

CODE: `04_HLD/HANDS_ON/01_rate_limiter_redis/RateLimiterDemo.java` ·
`04_HLD/HANDS_ON/02_incr_expire_crash/IncrExpireCrash.java` (koi library nahi, socket se Redis, `java
File.java`)

```
docker run -d --name demo-redis -p 6390:6379 redis:7 redis-server --save "" --appendonly no
   (RDB + AOF band -> sirf memory, restart = sab gaya, jaan-boojh ke)
docker stop / start demo-redis · docker exec demo-redis redis-cli TTL rl:user:1 · ... DEL rl:user:1

CODE KA DIL (user:1 ko 60 sec me 5):
   count = INCR KEY · if count == 1 -> EXPIRE KEY 60 · count <= 5 ALLOW else 429
   catch IOException -> FAIL_OPEN ? ALLOW : BLOCK
```
```
CASE A  chalta kaise  request 1..9:  count 1 TTL 60 ALLOW · 2/57 ALLOW · 3/56 ALLOW · 4/49 ALLOW ·
                                 5/48 ALLOW ·
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
FIX D   USE_LUA = true   pehle phansi key ek baar DEL (Lua purani phansi key theek nahi karta: umar sirf c == 1 pe),
                         phir count 1 TTL 60 · 2 · 3 ... -> nayi key kabhi bina umar ke nahi banegi

TTL:  -1 = key hai, timer nahi · -2 = key hi nahi · 58 = 58 sec baaki
```
```
FIX:  B -> faisla pehle se: fail-open (aam API) / fail-closed (login, OTP) + replica
      C -> replica / AOF on. limiter me aksar chalne do (1 min ki ginti, paisa nahi). paisa hota to nahi.
      D -> Lua: INCR + EXPIRE ek command, Redis single-thread -> script beech me nahi rukti
           (doosra raasta: HAR request pe INCR ke baad EXPIRE key 60 NX -> umar na ho to lagti, phansi key
            agli request pe theek. Sirf pehli request pe bheja to wahi crash wali dikkat.)
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
