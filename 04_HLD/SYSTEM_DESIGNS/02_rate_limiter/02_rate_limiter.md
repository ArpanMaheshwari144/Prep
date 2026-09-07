# Rate Limiter — 7-STEP RAIL (single spine, revise top→bottom)

> RAIL: Requirements → Estimate → API → Data-model → HL-boxes → Deep-dive → Bottleneck.
> (framework: 04_HLD/HLD_APPROACH_DELIVERY.md). Merged into clean 7-step 7-Sep. HANDS-ON demos = end me (preserve).
> Problem (1 line): over-limit requests REJECT (429), legit ALLOW. e.g. "5 login/min per IP".

```
   PUBLIC WATER TAP analogy:
   Normal user: 1 bottle | Pagal user: 10 trucks, hours tak -> saara paani khatam, real users vanchit
   -> Park Rule "5 bottles/day per person" = RATE LIMITING
   SAME for APIs: hacker bot 10K login/sec (brute force) -> limit "5/min per IP".
```

---

## STEP 1 — REQUIREMENTS

```
FUNCTIONAL:  over-limit request REJECT (429 Too Many Requests), legit ALLOW. limit e.g. "5/min per IP".
NON-FUNCTIONAL:
   - ★ LOW LATENCY = SABSE critical -> rate limiter HAR request ke saamne baithta; thoda slow -> POORA API slow.
       isliye Redis (in-memory, ~microsec).
   - ★ FAIL-OPEN: rate limiter / Redis DOWN -> requests ALLOW karo (block NAHI).
       soch: saare LEGIT users block >> thodi der abuse. (FAIL-CLOSED = block-all -> sirf payment/security-critical.)
CLARIFY:  limit kis pe? (IP / user_id / API-key) . per-endpoint? . tiered (free/pro)?

USE CASES:  login 5/min-IP . password-reset 3/hr-email . public-API 100/min-key . signup 10/day-IP . search 60/min-user.
```

---

## STEP 2 — ESTIMATE (★ rate-limiter ka asli insight)

```
   Rate limiter HAR request ke saamne baithta -> system ka SABSE HIGH-QPS component (gateway pe millions/sec).
   ★ KEY LINE: "Ye STORAGE-problem nahi, LATENCY problem hai — har request pe check <1ms hona chahiye,
                warna poora API slow. Isliye in-memory Redis (counters), na DB."
   -> per-key state chhota (counter + TTL), par ops-rate BAHUT high -> Redis atomic INCR.
```

---

## STEP 3 — API / RESPONSE

```
   limit OK   -> forward to API
   limit HIT  -> HTTP 429 (Too Many Requests) + Retry-After: 30 (kitna wait)

SUCCESS (200):                          REJECTED (429):
   X-RateLimit-Limit:     100              Retry-After:           30   (wait seconds)
   X-RateLimit-Remaining: 47               X-RateLimit-Limit:     100
   X-RateLimit-Reset:     1715180400       X-RateLimit-Remaining: 0
                          (unix time)      X-RateLimit-Reset:     1715180400
```

---

## STEP 4 — DATA MODEL (Redis key + atomic)

```
   KEY:  rate:{endpoint}:{user} -> count
   EXAMPLES:  rate:login:192.168.1.5 -> 4 | rate:signup:user_456 -> 2 | rate:search:apikey_xyz -> 47
   TTL = window time (60 sec) -> key AUTO-EXPIRE (window reset ho jaata)

   ATOMIC (no race):   MULTI -> INCR rate:login:userX -> EXPIRE rate:login:userX 60 -> EXEC   (ek atomic unit)
```

```
★ WHY ATOMIC — REDIS SINGLE-THREADED (23-Jul mock):
   Redis ek time pe SIRF EK command -> beech me koi doosri request ghus NAHI sakti -> INCR/MULTI-EXEC apne-aap ATOMIC.
   RACE jo rokta: 2 request ek-saath, bucket me 1 token -> read-then-write hota to DONO ko token (limit toot);
     INCR atomic -> ek poora hoke hi doosra -> safe.
   ★ multi-STEP logic (token-bucket: refill+check+decrement) -> LUA SCRIPT (Redis poore script ko ek atomic unit me).
   INTERVIEW LINE: "Since Redis is single-threaded, check-and-decrement is atomic — INCR for a counter,
     or a Lua script for token-bucket, to avoid the read-modify-write race."
   ★ CONNECT (2-Sep): ye WAHI race hai jo IDEMPOTENCY me thi (HDFC duplicate-payment) — naive containsKey+put ka gap
     -> 2 request ghus -> double charge. Wahan ilaaj=putIfAbsent (atomic), yahan=Lua (atomic).
     Rate-limiter <-> idempotency = SAME race, same lock-cure ("read+modify+write ko EK atomic unit").
```

---

## STEP 5 — HL BOXES (arch + placement)

```
                  USER
                   ▼
            ┌──────────────┐
            │  Route 53    │  DNS
            └──────┬───────┘
                   ▼
            ┌──────────────┐
            │  CloudFront  │  CDN
            └──────┬───────┘
                   ▼
            ┌──────────────┐
            │     ALB      │  Load Balancer
            └──────┬───────┘
                   ▼
            ┌──────────────────────┐
            │  API GATEWAY         │
            │  ┌────────────────┐  │
            │  │ Rate Limiter   │  │
            │  │ Middleware     │  │
            │  └────────────────┘  │
            └──────────┬───────────┘
               ALLOWED │ REJECTED(429)
                       ▼
                ┌──────────────┐        ┌──────────────┐
                │  App Servers │        │ REDIS CLUSTER│ (counters, atomic INCR)
                └──────────────┘        └──────┬───────┘
                                               ▼
                                        ┌──────────────┐
                                        │ Kafka ->     │  pattern-detection (abuse)
                                        │ Pattern Svc  │
                                        └──────────────┘
```

```
REQUEST FLOW (inside limiter):
   1. Identify user (IP / user_id / API-key)
   2. Redis key check "rate:login:userX" -> INCR atomic
   3. count > limit ?  YES -> 429 + Retry-After  |  NO -> forward to API

PLACEMENT (3 options): API Gateway | separate service | app-library -> ★ WINNER = API GATEWAY (most common).
   ★ WHY front/gateway (reject EARLY, 8-Jul): over-limit request ko BACKEND tak jaane hi mat do -> backend compute BACHTA.
     jise reject hi karna hai uspe kaam kyun? deep-me limit karo to request pehle poore system me ghoom ke resource kha chuki.
     (optional: coarse GLOBAL limit gateway + finer PER-SERVICE -> par primary = gateway.)
   ★ WHY REDIS (CENTRALIZED): multi-server -> har server apna count (s1=3,s2=4,s3=2) -> total 9, par kisi ek ko
     5-limit cross dikha hi nahi -> limit TOOT -> SINGLE SOURCE OF TRUTH = Redis + atomic INCR.
   ★ Route 53 = AWS smart DNS: naam->IP + HEALTH-CHECK/failover (dead LB se traffic hata) + nearest-region routing + domain-reg.
```

```
READ-FLOW LINE (memorize):
"User -> Route53 -> CloudFront -> ALB -> API Gateway rate-limiter -> Redis atomic INCR+EXPIRE ->
 limit-andar? App Server | limit-cross? 429+Retry-After | abuse-pattern? Kafka event | repeat-offender? WAF permanent ban"
```

---

## STEP 6 — DEEP DIVE: Algorithms (4 methods)

```
1. TOKEN BUCKET (★ MOST COMMON, AWS/Stripe): bucket me tokens auto-add (1/sec, max N). req -> token lo -> allow; empty -> reject.
      -> real traffic SPIKY (user 10 req ek-saath phir shaant); jama-tokens se BURST allow = user-friendly.
2. LEAKY BUCKET: req bucket me, fixed-rate se leak/process (hole 1/sec). full -> overflow reject. SMOOTH, burst BLOCK (laggy lagta).
3. FIXED WINDOW: per-minute counter (0->5, reset).
      ★ EDGE-SPIKE BUG: 10:00:59 pe 5 + 10:01:00 pe 5 = 10 req in 2 sec (window-boundary pe 2x limit). sasta par ye bug.
4. SLIDING WINDOW: "last 60 sec" me count. request-driven (naya req -> ABHI se 60 sec peeche gino).
      SMOOTH + accurate PAR har request ka timestamp -> MEMORY-heavy.
```

```
BUS-STAND ANALOGY (sliding window ACTUALLY kaise shift hoti):
   Watchman register: "last 1 hour me 5 passenger MAX". entries: Ramesh 10:05, Suresh 10:15, ... Dinesh 10:55.
   11:00 pe naya -> "60 min peeche = 10:00" -> count(10:00 ke baad)=5 -> REJECT.
   11:10 pe naya -> "60 min peeche = 10:10" -> 10:05 wala AB OUT -> count=4 -> ALLOW + add 11:10.
   ★ WINDOW SHIFT = ON-DEMAND (request-driven), NOT timer/background-job. Trigger = request arrival, not clock.

COMPARISON:
   ┌──────────────────┬───────────┬─────────┬──────────┬─────────┐
   │  Algorithm       │ Bursts    │ Smooth  │ Memory   │ Common  │
   ├──────────────────┼───────────┼─────────┼──────────┼─────────┤
   │ Token Bucket     │ YES       │ Variable│ Low      │ AWS,Stripe│
   │ Leaky Bucket     │ NO        │ YES     │ Low      │ Throttle │
   │ Fixed Window     │ Edge fail │ NO      │ Lowest   │ GitHub  │
   │ Sliding Window   │ Smooth    │ YES     │ High     │ Cloudflare│
   └──────────────────┴───────────┴─────────┴──────────┴─────────┘
   => DEFAULT-PICK = TOKEN BUCKET (burst-friendly + low-memory).
```

---

## STEP 7 — BOTTLENECK / SCALE / RELIABILITY

```
TIERED LIMITS:  anonymous 60/hr . free 5k/hr . pro 10k/hr . enterprise custom (user-tier DB se fetch -> Redis counter compare).

DISTRIBUTED (user multi-region): arpan_123 Bangalore+Berlin+US-VPN -> har region 50 -> total 150 > 100 -> TOOT.
   FIX LADDER:
     1. CENTRALIZED Redis (accurate PAR SPOF + high-latency)
     2. LOCAL + async-sync (fast PAR slight over-allow)
     3. ★ REGION-STICKY (BEST): hash(user_id) % regions = home_region -> saare paths ek region -> local Redis full-picture.
          hash("arpan_123")%3=0->INDIA . hash("john_456")%3=1->EU . hash("alex_789")%3=2->US.

★ ACCURACY vs LATENCY (local+async-sync ka asli trade-off — yaad rakh):
   local+async = har instance LOCAL count se allow, sync BAAD me -> OVER-ALLOW (limit APPROXIMATE/soft ho jaati).
   KAB CHALEGA (over-allow OK): limit sirf "server bachane" (general API throttle/abuse) -> thoda upar-neeche se aafat nahi.
   KAB NAHI (exact chahiye): limit = PAISA/SECURITY/correctness -> "3 OTP attempts" . "10 free calls phir charge" . withdrawal-limit
        -> CENTRAL ATOMIC (Redis+Lua), latency ki keemat bhugto.
   1-LINE: protective/soft limit -> local+async (fast) | money/security limit -> central atomic (exact).

RELIABILITY (Redis reflexes + SPOF-chain):
   - Redis DOWN -> REPLICA auto-promote (Sentinel/cluster failover) -> (poori Redis layer gayi -> TAB FAIL-OPEN, last resort).
   - Redis OVERLOAD -> SHARDING (user_id/region se alag nodes A-M->R1, N-Z->R2). (shard=scale+isolation | replica=recovery.)
   - ★ SPOF-CHAIN: koi bhi critical ka SINGLE instance mat rakho -> har layer >=2 + AUTO-FAILOVER (LB, Redis, app, DB).
        LB down -> multiple LB (active-active/passive) + health-check/VIP failover. (cloud ALB khud multi-AZ redundant.)
        ★ MECHANISM zaroori: replicate + koi failure DETECT karke REDIRECT kare = health-check+failover (Redis=Sentinel, LB=Route53/VIP); warna replica bekaar.
        TOP=DNS(Route53) globally-managed -> khud single-box nahi -> top-level SPOF nahi.

LAYERED DEFENSE (rate-limit AKELA kyun nahi):
   Layer1 rate-limit (soft/temporary 429 "wait 60s") -> Kafka event -> Layer2 pattern-detection (repeat abuser?) -> Layer3 WAF/IP-blocklist (PERMANENT).
   WHY not block immediately: false-positives (real-user fast clicks) . shared NAT IP (1 IP=100 user) . legit bursts (marketing).
     rate-limit = forgiving (retry ok) | WAF = permanent (verified abuse only).

WRAP: User->Route53->CDN->ALB->API-Gateway[rate limiter]->Redis atomic-counter->App.
      ALGO=token-bucket . KEY=rate:{endpoint}:{user}+TTL . 429+Retry-After.
      distributed->region-sticky . reliable->replica+fail-open+shard . layered->rate-limit+Kafka+WAF.
```

---

## ═══ HANDS-ON — Nginx se rate-limiter LIVE chalaya (khud kiya, 21-Aug) ═══

> Upar sab THEORY padhi. Ye section = wahi cheez REAL TOOL me chala ke apni aankhon se dekhi.
> Koi program NAHI likha — sirf ek ready tool (Nginx) on kiya + config di + hammer maar ke 503 nikaala.
> (files: isi folder me `nginx.conf`)

### 0. Maqsad + tareeka
```
Rate-limiter ko PADHA to tha -> ab dekhna tha "practice me kaam kaise karta".
Tool = Nginx (real web-server, jisme rate-limiter PEHLE se built-in hai).
Nginx ko Docker container me chalaya, config di, curl se tez requests maari -> 503 aaya.
```

### 1. DOCKER kyun + kaise
```
Nginx laptop pe install karne ki zaroorat nahi -> Docker se ek command me container khada.
    docker run -d --name rl -p 8080:80 -v "<path>\nginx.conf:/etc/nginx/nginx.conf:ro" nginx

    -d            = background me chalao (detached)
    --name rl     = container ka naam "rl"
    -p 8080:80    = laptop ka 8080 -> container ke 80 se joda (localhost:8080 pe milega)
    -v "...:...:ro" = apni nginx.conf ko container ke andar wali jagah pe MOUNT karo (ro=read-only)
    nginx         = image (pehli baar auto-download hui: "Pulling from library/nginx")

    -> ek lambi container-ID print hui = chalu.
```

### 2. CONFIG file (nginx.conf) — ismein kya likha (2 line hi asli rate-limiter)
```
limit_req_zone $binary_remote_addr zone=mylimit:10m rate=1r/m;   <- (1) BUCKET banao
    $binary_remote_addr = client ka IP (har IP ka apna bucket)
    zone=mylimit:10m    = bucket ka naam + memory
    rate=1r/m           = REFILL speed (1 token per minute)

location / {
    limit_req zone=mylimit burst=5 nodelay;                       <- (2) LIMIT lagao
        burst=5   = bucket ka SIZE (ek saath 5 jhel lega)
        nodelay   = burst ko turant serve karo (queue me lataka mat)
    root /usr/share/nginx/html;  index index.html;               <- content serve
}

Do knob (dono token-bucket theory se):  rate = REFILL speed  |  burst = bucket SIZE
```

### 3. TEST kaise kiya (curl)
```
Normal (ek hit):
    curl http://localhost:8080                 -> "OK" / welcome page (bucket me token hai)

Hammer (30 request ek jhatke me, sirf status-code dikhao):
    for /L %i in (1,1,30) do @curl -s -o nul -w "%{http_code} " http://localhost:8080
        for /L (START,STEP,END) = (1,1,30) -> loop 30 baar -> 30 requests
        -s -o nul  = chup raho, body phenk do
        -w "%{http_code}" = sirf status code chhapo
```

### 4. Kya DEKHA (live)
```
200 200 200 200 200 200 503 503 503 503 ...
└──── bucket ke token ────┘ └──── khatam = BLOCKED (503) ────┘

Docker Desktop -> Containers -> rl -> Logs me nginx khud likhta:
    "GET / HTTP/1.1" 503 197
    [error] limiting requests, excess: 5.774 by zone "mylimit", client: 172.17.0.1
        503 197      = status 503, response sirf 197 byte
        excess: 5.77 = ye request bucket se kitne token UPAR thi
        zone mylimit = kis bucket ne roka
        client 172.17.0.1 = Docker gateway IP (saari requests ek hi IP se dikhi -> ek hi bucket share)
```

### 5. ★★ GEMS / gotchas (interview me bhi)
```
1. LIMIT tabhi kaatti jab ARRIVAL-rate > limit: pehle rate=2r/s pe slow curl-loop se sab 200 aaye
   (loop dheere tha, refill keep-up kar gaya). Tez maaro tabhi 503.
2. burst = N  ->  spike me N+1 pass (nginx apni taraf se +1 karta):
      burst=5 -> 6,   burst=100 -> 101.
      Wo +1 bucket me se NAHI hoti -> rate ki "live" 1 request bucket ke bahar se nikalti (alag slot).
      TEXTBOOK token-bucket = N (exact) | NGINX = N+1 (quirk).
      Aur EXACT bhi nahi -> timing/refill se thoda wobble (21, kabhi 22). Isiliye chala ke dekho, theory pe aankh-band bharosa nahi.
3. 1 IP = 1 bucket ($binary_remote_addr). Real world me har user ka apna IP -> apna bucket.
4. config badla -> "docker restart rl" karna PADTA (warna purana config chalta rehta).
```

### 6. Dobara kaise chalaye (quick)
```
docker start rl                                      (band ho to)
curl http://localhost:8080                           (normal check)
for /L %i in (1,1,30) do @curl -s -o nul -w "%{http_code} " http://localhost:8080   (hammer)
docker logs rl                                       (limiting-requests lines dekho)
docker restart rl                                    (config badla ho to reload)
docker stop rl                                        (band karna ho to)
```

---

## ═══ HANDS-ON #2 — APP-LEVEL fixed-window (usercrud, Java code) ═══
> Nginx wala (upar) = EDGE/INFRA pe limit (server ke bahar). Ye = APP ke ANDAR code me limit.
> Interview me "rate limiting kaise implement karoge" ka CODE-level jawab. Kiya 26-Aug usercrud pe.

### PROBLEM (kaunsa)
Ek endpoint pe 1 second me max N request allow; usse zyada -> 429 Too Many Requests.
Client abuse / accidental flood se server bachana.

### KYA BANAYA (fixed-window counter — 3 algorithm me se sabse simple)
```
STATE (3 field): LIMIT=5 (max/window) · windowStart (window shuru ka time, ms) · count (ab tak kitni)
LOGIC (har request pe):
   1. now - windowStart > 1000ms ?  -> nayi window: windowStart=now, count=0   (RESET)
   2. count++
   3. count > LIMIT ? -> return 429   :   return 200 "OK"
```
= "har 1 second ek nayi window, count zero se; limit paar -> 429." (window reset hote hi phir allow.)

### CODE (RateLimitController)
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
// ResponseEntity KYUN: status-code (200/429) khud set karne ko (plain String me control nahi).
```

### LIVE (jo dikha)
```
PS> 1..10 | % { curl.exe -s http://localhost:8080/rate-demo; "" }
   OK - request #1 ... #5          (pehli 5 allowed)
   429 - limit paar (count=6..10)  (6th se block)
   1 sec baad -> window reset -> phir OK
```

### 3 ALGORITHM (design-level, is code = pehla)
```
1. FIXED WINDOW   (banaya): per-second counter. SIMPLE. par window-BOUNDARY pe burst (999ms pe 5 + 1001ms pe 5 = 10 in ~2ms).
2. SLIDING WINDOW LOG: har request ka timestamp rakho, last-1sec wale gino. accurate, par memory zyada.
3. TOKEN BUCKET   (real prod): bucket me token bharte raho (rate), request = 1 token; token khatam -> reject. burst allow + smooth.
```

### ★ SCALE / PROD NOTES
```
- ye in-memory single-node -> restart pe reset + multi-server pe har server ka apna counter (galat total).
  MULTI-NODE -> REDIS me counter (INCR + EXPIRE) -> saare servers ek shared limit.
- per-USER/per-IP limit -> map<key, counter> (yahan global tha).
- prod-lib: Bucket4j (Java, token-bucket) / Redis + Lua (atomic).
```

### INTERVIEW LINE
```
"App-level rate limit fixed-window counter se kiya - per-second count, LIMIT paar -> 429 (ResponseEntity).
 Trade-off: fixed-window me boundary-burst; production me token-bucket (Bucket4j) ya sliding-window better.
 Multi-node pe Redis INCR+EXPIRE se shared counter. Nginx pe edge-level bhi kar sakte (limit_req).
 Maine usercrud pe live kiya - 10 rapid requests, pehli 5 OK phir 429."
```

---

[← HLD README](../README.md)
