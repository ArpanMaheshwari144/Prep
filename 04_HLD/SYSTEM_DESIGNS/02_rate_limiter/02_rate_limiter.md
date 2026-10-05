# Rate Limiter

> Limit se upar wali request REJECT (429), legit ALLOW. Misaal: "5 login / min per IP".
> Is design ka dil: **har request ke saamne baitha hai -> <1ms** + **ginti sab server pe EK** + **khud mare to site na mare**.

```
PUBLIC NAL: normal banda 1 bottle · pagal banda 10 truck -> paani khatam, asli user khaali haath
            -> park ka niyam "5 bottle / din per banda" = RATE LIMITING
            API pe: bot 10K login / sec (brute force) -> "5 / min per IP"
```

---

## TASVEER (ByteByteGo / Alex Xu · CC BY-NC-ND 4.0)

![How Does Redis Persist Data?](https://assets.bytebytego.com/diagrams/0214-how-redis-presists-data.png)
Source: [How Does Redis Persist Data?](https://bytebytego.com/guides/how-does-redis-persist-data/)
(HANDS-ON #3 CASE C — persistence band thi, restart pe ginti gayab. RDB / AOF yahi hain.)

---

## SHURU — poocho + numbers

```
POOCHO:  limit KIS PE? IP / user_id / API key · har endpoint alag (login 5/min, search 60/min)?
         tiered plan (free / pro / enterprise)?
         ★ maqsad SERVER BACHANA hai ya PAISA / SECURITY?  <- sabse zaroori, yahi aage ka trade-off tay karta
           soft -> thoda over-allow chalega · paisa / security -> bilkul exact

USE CASE: login 5/min per IP · password-reset 3/hr per email · public API 100/min per key ·
          signup 10/day per IP · search 60/min per user

FR:      rule (kya limit) · counter (kitni hui) · window (kis samay me) · over -> 429
NFR:     LOW LATENCY <1ms (har request ke saamne, slow = poora API slow) · FAIL-OPEN (limiter mare -> allow;
         payment / auth / OTP pe FAIL-CLOSED) · kai server pe bhi sahi ginti

NUMBERS: storage ka hisaab bekaar — per key ek counter + TTL, bas
         asli = OPS RATE: gateway pe baitha = system ka sabse high-QPS dabba (millions / sec)
         -> ye STORAGE nahi LATENCY problem -> in-memory (Redis), DB nahi · check ATOMIC
```

---

## DABBA 0 — sabse simple

```
SOLUTION: counter App ki memory me · count++ · limit paar -> 429
```
```
  USER
    │
    ▼
  [ App ]
```

---

## DIKKAT 1 — teen server, ginti bat gayi

```
DIKKAT:   LB ne baanta: App-1 = 3, App-2 = 4, App-3 = 2 -> total 9, limit 5
          par kisi EK ko 5 paar dikha hi nahi -> LIMIT TOOT GAYI

SOLUTION: counter EK central jagah — in-memory, kyunki har request pe hit -> REDIS (single source of truth)
          counter + TTL

NAYA:     LB · Redis
BADLA:    App -> App x N (counter ab App ke andar nahi)
```
```
  USER
    │
    ▼
  [ LB ]
    │
    ▼
  [ App x N ]
    │
    ▼
  [ Redis ]
```

---

## DIKKAT 2 — do request ek saath, dono ko jagah mil gayi

```
DIKKAT:   A padha 4 -> +1 -> likha 5 · B padha 4 -> +1 -> likha 5 -> limit 5, 6 ghus gayi

SOLUTION: padho-badhao-likho = EK ATOMIC kaam
          Redis single-threaded -> INCR apne aap atomic
          token bucket jaisa kai step (refill + check + ghatao) -> LUA SCRIPT (poora ek unit)
            count = INCR rate:login:userX
            if count == 1 then EXPIRE rate:login:userX 60 end      <- EXPIRE sirf PEHLI baar
          ★ JAAL: har request pe EXPIRE 60 -> TTL har baar 60 pe dhakelta -> key kabhi expire nahi
                  -> user hamesha block. (Redis 7+: EXPIRE key 60 NX bhi yahi)
          CONNECT: wahi race jo idempotency me (containsKey + put ka gap -> double charge)
                  wahan putIfAbsent, yahan INCR / Lua — ilaaj same: teen step EK unit

NAYA:     koi dabba nahi — Redis me INCR / Lua
```
```
POOCHEGA: "Two requests come at the same time — what happens?"
DHYAAN:   2 user ek cheez = atomic / lock · 1 user ka retry = idempotency (dono alag)
BOL:      "Redis is single-threaded, so INCR is atomic. For token bucket I use a Lua script, so the
           check and the decrement run as one unit and there's no read-modify-write race."
```

---

## DIKKAT 3 — limiter App ke andar: reject hone wali request bhi poore system me ghoom aayi

```
DIKKAT:   jise reject karna hai uspe backend ka compute kyun jale

SOLUTION: limiter SABSE AAGE — API GATEWAY pe, 429 wahin, backend tak jaane hi nahi
          3 jagah ho sakti: gateway · alag service · app library -> GATEWAY sabse common
          (chaaho to gateway pe mota global limit + service ke andar baarik limit)

NAYA:     API Gateway (limiter iske andar)
```
```
  USER
    │
    ▼
  [ LB ]
    │
    ▼
  [ API Gateway ] ──► [ Redis ]
    │
    ▼
  [ App x N ]
```

---

## DIKKAT 4 — Redis hi gir gaya

```
DIKKAT:   har request ka faisla Redis pe tha

SOLUTION: (1) REPLICA + auto failover (Sentinel / cluster) -> replica ALAG AZ me
          (2) poori Redis layer gayi -> FAIL-OPEN (sab allow) · payment / auth / OTP -> FAIL-CLOSED
          (3) Redis pe bojh -> SHARD (user_id / region: A-M -> R1, N-Z -> R2)
              shard = scale + alag-alag · replica = bachav
          SPOF chain: har layer >= 2 + AUTO failover. sirf copy rakhna kaafi nahi — koi DEKHE aur MODE
              Redis = Sentinel · LB = Route 53 / VIP · cloud ALB khud multi-AZ · Route 53 khud global

NAYA:     Route 53
BADLA:    Redis -> Redis Cluster (replica + shard) · LB -> ALB (multi-AZ)
```
```
  USER
    │
    ▼
  [ Route 53 ]
    │
    ▼
  [ ALB ]
    │
    ▼
  [ API Gateway ] ──► [ Redis Cluster ]
    │
    ▼
  [ App x N ]
```
```
POOCHEGA: "What if Redis goes down?"
DHYAAN:   faisla PEHLE se code me likha ho — fail-open ya fail-closed, endpoint ke hisaab se
BOL:      "Redis has a replica with automatic failover in another zone. If the whole layer is gone I
           fail open for normal APIs so the site stays up, and fail closed for login and OTP."
```

---

## DIKKAT 5 — ek user Bangalore + Berlin + US-VPN se maar raha

```
DIKKAT:   INDIA 50 + EU 50 + US 50 = 150, limit 100 -> har region ko apna hi dikha

SOLUTION: teen raaste:
          1. CENTRAL Redis        -> exact, par ek jagah + door wale region ko latency
          2. LOCAL + async sync   -> tez, par thoda OVER-ALLOW
          3. REGION-STICKY (best) -> hash(user_id) % regions = HOME region, uske saare raaste wahin
                                     -> local Redis ke paas poori ginti
          ACCURACY vs LATENCY:
             soft / server bachana          -> local + async chalega
             paisa / security ("3 OTP", "10 free call phir charge", withdrawal) -> CENTRAL ATOMIC (Redis + Lua)

NAYA:     koi dabba nahi — routing ka niyam
```
```
POOCHEGA: "What if a whole region goes down?"
BOL:      "Route 53 sends the user to another region and the count starts from zero there, so the limit
           is loose for one window. For a rate limiter that's fine — it's only a minute of counting."
```

---

## DIKKAT 6 — ek hi banda baar-baar maar raha, 429 se ruk hi nahi raha

```
DIKKAT:   rate limit = sirf "60 sec baad aao", wo phir aa jaata

SOLUTION: LAYERED: (1) rate limit = soft, 429
                   (2) event KAFKA -> Pattern service ("ye baar-baar?")
                   (3) WAF / IP blocklist = PERMANENT ban
          turant permanent kyun nahi: asli user tez click · ek NAT IP ke peeche 100 log · sale ka legit burst
          -> limit maafi wali, ban sirf pakke abuse pe

NAYA:     Kafka · Pattern Svc · WAF
```
```
  USER
    │
    ▼
  [ Route 53 ]
    │
    ▼
  [ ALB ]
    │
    ▼
  [ API Gateway ] ──► [ Redis Cluster ] ──► [ Kafka ] ──► [ Pattern Svc ] ──► [ WAF ]
    │
    ▼
  [ App x N ]
```

---

## DIKKAT 7 — limiter sahi chal raha, phir bhi server gira (BHEED)

```
DIKKAT:   limit 100 / min per user · server ki had 5,000 / sec
          ek abuser -> 100 pe ruka ✓
          10,000 ASLI user, har ek limit ke ANDAR -> 12,000 / sec -> SERVER GIRA ✗
          limiter ne kuch galat nahi kiya — har user niyam me tha

SOLUTION: RATE LIMIT = "kis USER ne kitni"  (abuse / fairness)
          LOAD SHEDDING = "SYSTEM abhi kitna jhel sakta" (bachna)  <- ye alag cheez hai
          poore system ka GLOBAL CAP / concurrency limit · sehat dekh ke shedding ("CPU 90% -> nayi mat lo")
          QUEUE burst pakad leti · pata ho kab aayega (12 baje sale) -> PEHLE scale out
          (autoscale ko minute lagte, spike second me aata)

NAYA:     koi dabba nahi — Gateway me global cap + shedding
```
```
POOCHEGA: "What if traffic suddenly spikes 10x?"
DHYAAN:   "rate limiter laga hai" kaafi NAHI — per-user limit bheed nahi rokti
BOL:      "A per-user limit doesn't stop a crowd where everyone is under their limit. For that I need a
           global cap and load shedding based on system health, plus a queue for the burst and
           pre-scaling when I know the spike is coming."
```

---

## DIKKAT 8 — attack aaya, aur limiter USI WAQT gayab

```
DIKKAT:   attack -> traffic 50x -> Redis pe bhi 50x -> Redis down -> FAIL-OPEN -> sab allow
          limiter tabhi gaya jab sabse zyada zaroorat thi

SOLUTION: fail-open ke SAATH local fallback:
          Redis zinda -> poori global ginti · Redis gaya -> har node APNI memory se motamoti rok
          exact nahi (har node apna ginega), par attack me "kuch nahi" se bahut behtar

NAYA:     koi dabba nahi — App / Gateway me local counter fallback
```

---

## DIKKAT 9 — IP pe gin rahe the

```
DIKKAT:   NAT: ek office / mobile network ke HAZAAR log ek IP -> ek ne limit khaayi, sab block
          BOTNET: hazaar IP, har IP se 5 -> kisi ki limit nahi tooti -> ruka hi nahi

SOLUTION: logged-in -> user_id / API key pe gino, IP pe NAHI
          login / signup se pehle (pehchaan nahi) -> IP majboori -> limit DHEELI + asli faisla WAF / bot detection

NAYA:     koi dabba nahi
```

---

## 10x SCALE — har dabba alag

```
API Gateway    -> har request pe limiter -> in-memory + atomic, gateway box badhao
Redis          -> SHARD (user / region) · REPLICA + failover · phir fail-open + local fallback
multi-region   -> REGION-STICKY hashing
abuser         -> Kafka pattern -> WAF ban
bheed          -> global cap + load shedding + queue + pre-scale

POOCHEGA: "How would you scale this to 10x?"       -> request ka raasta chalo, har dabbe pe "kya toota"
POOCHEGA: "What's the single point of failure?"    -> Redis (cluster + failover), LB (Route 53)
POOCHEGA: "How do you know it's working?"          -> p99 limiter latency · 429 rate · Redis errors · alert
```

---

## POOCHE TO (deep-dive)

```
RESPONSE:  OK  -> aage bhejo + X-RateLimit-Limit: 100 · X-RateLimit-Remaining: 47 · X-RateLimit-Reset: <unix time>
           HIT -> 429 Too Many Requests + Retry-After: 30 + Remaining: 0
           sirf "na" mat bolo — kitna bacha + kab aao batao, warna client turant retry maarta
           ★ Retry-After me JITTER (28, 33...) — warna sab theek 30 sec baad EK SAATH -> nayi laher

POOCHEGA:  "The provider returns 429 — what now?"  (notification wala ulta sawaal)
BOL:       "I respect Retry-After, back off exponentially with jitter, and throttle my own sends."

REDIS KEY: rate:{endpoint}:{user} -> count · TTL = window (60 sec) -> key gayab = window reset, cleanup job nahi
           rate:login:192.168.1.5 -> 4 · rate:signup:user_456 -> 2 · rate:search:apikey_xyz -> 47

FLOW:      user pehchano (IP / user / key) -> INCR (atomic) + EXPIRE -> count > limit ? 429 + Retry-After : App

ALGORITHM:
  TOKEN BUCKET  (AWS / Stripe)  bucket me token bharte (1/sec, max N) · request = 1 token · khaali -> reject
                                asli traffic SPIKY -> jama token se burst nikal jaata  <- YAHI LUNGA
  LEAKY BUCKET                  andar girti, neeche se FIXED rate · bhara -> reject · smooth par burst BLOCK
  FIXED WINDOW  (GitHub)        per-minute counter · EDGE BUG: 10:00:59 pe 5 + 10:01:00 pe 5 = 2 sec me 10
  SLIDING WINDOW (Cloudflare)   "last 60 sec" ke timestamp · sahi + smooth, par memory bhaari
     BUS-STAND: register "last 1 ghanta me 5 max" · 11:00 -> 10:00 ke baad 5 -> REJECT
                11:10 -> 10:10 ke baad gino, Ramesh (10:05) bahar -> 4 -> ALLOW
                window SHIFT request aane pe hoti, koi timer / job nahi

  Bursts / Smooth / Memory:  token YES / variable / low · leaky NO / YES / low ·
                             fixed edge-fail / NO / lowest · sliding smooth / YES / high

GINTI vs LOAD:  100 chhoti request (5ms) = 0.5 sec kaam · 100 report query (30 sec) = 50 MINUTE kaam
                limiter ke liye barabar -> bhaari endpoint pe alag kadi limit / request ko WAZAN
                (report = 50 token, login = 1) / CONCURRENCY limit ("ek user ki 2 bhaari query ek waqt")

TIERED:    anonymous 60/hr · free 5,000/hr · pro 10,000/hr · enterprise custom -> tier DB se, Redis me usi limit se compare
```

---

## AAKHRI DABBA + WRAP

```
Route 53 = DNS + health-check · ALB = multi-AZ · API Gateway = limiter sabse aage, reject early
Redis Cluster = single source of truth, <1ms, atomic INCR / Lua, replica + shard
Kafka -> Pattern Svc -> WAF = baar-baar wale ka permanent ban
```
```
  USER
    │
    ▼
  [ Route 53 ]
    │
    ▼
  [ ALB ]
    │
    ▼
  [ API Gateway ] ──► [ Redis Cluster ] ──► [ Kafka ] ──► [ Pattern Svc ] ──► [ WAF ]
    │
    ▼
  [ App x N ]
```
```
BOL: "The limiter sits in the API gateway, in front of everything, so rejected requests never reach the
      backend. Counts live in Redis with an atomic INCR or a Lua script, keyed by endpoint and user,
      with a TTL as the window. I use token bucket because real traffic is bursty. Over the limit it's a
      429 with Retry-After. Redis is replicated and sharded, fails open for normal APIs and closed for
      login and OTP. Users stick to a home region, repeat abusers go through Kafka to a WAF ban, and a
      global cap with load shedding protects against a legit crowd."
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
   SACH: nginx docs isko LEAKY BUCKET kehte; nodelay se bartaav token bucket JAISA. 503 = limit_req_status default.

HAMMER:  for /L %i in (1,1,30) do @curl -s -o nul -w "%{http_code} " http://localhost:8080
DIKHA:   200 200 200 200 200 200 503 503 503 ...
LOG:     [error] limiting requests, excess: 5.774 by zone "mylimit", client: 172.17.0.1
         (excess = bucket se kitna upar · client = Docker gateway IP, sab ek IP = ek bucket)

GOTCHA:  1. 503 tabhi jab ARRIVAL > rate (rate=2r/s pe dheere loop -> sab 200, refill pakad leta)
         2. burst=N -> spike me N+1 pass (burst=5 -> 6, 100 -> 101); textbook N, nginx N+1, timing se 21 / 22 bhi
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
PROD:  single node memory -> restart pe reset + har server apna count -> Redis INCR + EXPIRE
       per user / IP -> map<key, counter> · lib: Bucket4j (token bucket) / Redis + Lua
BOL:   "I built a fixed-window counter in usercrud — five OK, then 429. In production I'd use token
        bucket (Bucket4j) with a shared Redis counter; Nginx limit_req does it at the edge."
```

---

## HANDS-ON #3 — Redis KHUD maara: kaise tootta (30-Sep)

CODE: `04_HLD/HANDS_ON/01_rate_limiter_redis/RateLimiterDemo.java` · `04_HLD/HANDS_ON/02_incr_expire_crash/IncrExpireCrash.java`
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

CASE C  docker start  GET rl:user:1 -> khaali · user (pehle count 7, BLOCK) -> count 1 ALLOW, 2 ALLOW
                      persistence band -> ginti gayab -> blocked ko 5 nayi mil gayi

CASE D  INCR ke baad, EXPIRE se pehle crash (USE_LUA = false)
                      RUN 1: count 1, CRASH · TTL -> -1
                      RUN 2: count 2..5 ALLOW, 6..7 429, har baar TTL -1 -> key KABHI nahi mitegi, HAMESHA block
FIX D   USE_LUA = true   req 10..14: count 10 TTL 58 · 11/57 · 12/56 · 13/56 · 14/55 -> 1 min me free

TTL:  -1 = key hai, timer nahi · -2 = key hi nahi · 58 = 58 sec baaki
```
```
FIX:  B -> faisla pehle se: fail-open (aam API) / fail-closed (login, OTP) + replica
      C -> replica / AOF on. limiter me aksar chalne do (1 min ki ginti, paisa nahi). paisa hota to nahi.
      D -> Lua: INCR + EXPIRE ek command, Redis single-thread -> script beech me nahi rukti (ya EXPIRE 60 NX)
           pehle:  app --INCR--> Redis  [crash]  app --EXPIRE--> Redis
           Lua:    app --EVAL(INCR + EXPIRE)--> Redis
```
```lua
local c = redis.call('INCR', KEYS[1])
if c == 1 then redis.call('EXPIRE', KEYS[1], ARGV[1]) end
return c
```
```
BOL: "If Redis is down the limiter fails open so the site stays up, and fails closed on login and OTP.
      A Redis restart resets counters — acceptable for a limiter, a replica reduces it. INCR and EXPIRE
      sent separately can leave a key with no TTL after a crash and block a user forever, so I send both
      in one Lua script. I killed a Docker Redis myself to see all three."
```

[← MASTER SHEET](../../00_MASTER_SHEET.md)
