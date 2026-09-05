# Rate Limiter — Visual System Design

---

## 1 Problem (Analogy)

```
   PUBLIC WATER TAP
        │
   Normal user: 1 bottle bhar ke gaya
   Pagal user: 10 trucks le aaya, hours tak
                    │
                    ▼
              Saara paani khatam
              Real users vanchit
                    │
                    ▼
        Park Rule: "5 bottles/day per person"
                    │
                    ▼
              = RATE LIMITING
```

```
SAME for APIs:

   Normal user → 2 login attempts → done
   Hacker bot → 10K attempts/sec → brute force
                  │
                  ▼
            Rate limit:
            "5 attempts/min per IP"
```

---

## 2 Real Use Cases

```
┌─────────────────────────┬──────────────────────────┐
│  Use Case               │  Limit                    │
├─────────────────────────┼──────────────────────────┤
│  Login attempts         │  5/min per IP            │
│  Password reset         │  3/hour per email        │
│  Public API endpoint    │  100/min per API key     │
│  Signup creation        │  10/day per IP           │
│  Search queries         │  60/min per user         │
└─────────────────────────┴──────────────────────────┘
```

---

## 3 Core Idea — Visual

```
   USER REQUEST
        │
        ▼
   ┌─────────────────────┐
   │   RATE LIMITER      │  ← gatekeeper
   │   (counter check)   │
   └──────────┬──────────┘
              │
       ┌──────┴──────┐
       ▼             ▼
   limit OK?      limit hit?
       │             │
       ▼             ▼
   forward      REJECT
   to API       (429 Too Many Requests)
```

---

## 4 Where It Sits — 3 Options

```
┌────────────────────┬───────────────────┬─────────────┐
│  Placement         │  Pros              │  Cons       │
├────────────────────┼───────────────────┼─────────────┤
│ API Gateway        │ Built-in, easy     │ Vendor lock │
│ Separate service   │ Custom logic       │ Extra hop   │
│ App Server library │ No extra service   │ Inconsistent│
└────────────────────┴───────────────────┴─────────────┘

         WINNER ──────► API Gateway (most common)
```

```
   ★ WHY front/gateway (reject-EARLY reasoning — 8-Jul discussion):
   - rate limiter itna AAGE (edge/gateway) isliye -> over-limit request ko BACKEND tak jaane hi mat do.
   - reject EARLY -> backend ka compute/resources BACHTE. jise reject hi karna hai, uspe kaam kyun?
   - agar deep (har service ke andar) limit karo -> request pehle hi poore system me ghoom ke resource kha chuki.
   - (optional layering: coarse GLOBAL limit gateway pe + finer PER-SERVICE limit -> but primary = gateway.)
```

```
WHY REDIS (centralized counter):

   Server 1: count = 3
   Server 2: count = 4     ← each thinks alone
   Server 3: count = 2
                              Total real = 9
                              Limit = 5
                              break ho raha

   FIX:
   ┌──────┐  ┌──────┐  ┌──────┐
   │ S1   │  │ S2   │  │ S3   │
   └──┬───┘  └──┬───┘  └──┬───┘
      └─────────┼─────────┘
                ▼
         ┌────────────┐
         │   REDIS    │  ← single source of truth
         │  (counter) │     atomic INCR
         └────────────┘
```

---

## 4.5 ★ NFR + FAIL-OPEN + RELIABILITY (design drill 3-Jul — was missing)

```
   NFR (kaunsa critical):
   - ★ LOW LATENCY = SABSE critical. rate limiter HAR request ke saamne baithta ->
     thoda bhi slow -> POORA API slow. isliye Redis (in-memory, ~microsec).
   - FAIL-OPEN = agar rate limiter / Redis DOWN ho jaaye -> requests ALLOW kar do (block NAHI).
     kyun: saare LEGIT users block karna >> thodi der abuse chalne dena.
     (fail-CLOSED = block all -> sirf jab security critical, e.g. payment.)

   RELIABILITY (Redis reflexes):
   - Redis DOWN     -> REPLICA le le (HA/failover). replica bhi gaya -> FAIL-OPEN (last resort).
   - Redis OVERLOAD -> SHARDING: user_id/region se alag Redis nodes (A-M->R1, N-Z->R2) -> load divide.
                       (yeh CDN NAHI -> CDN static files ke liye. counters ke liye SHARDING.)

   crisp: latency critical -> Redis. down -> replica -> fail-open. overload -> shard.
          (shard = scale + isolation | replica = recovery — dono milke reliable.)
```

---

## 4.6 ★ SPOF-CHAIN — har layer redundant (deep-dive, Arpan-derived 15-Jul)

```
   PRINCIPLE: koi bhi critical cheez ka SINGLE instance mat rakho. har SPOF -> >=2 + AUTO-FAILOVER.
              chain apne WEAKEST-single-point pe tootti -> har layer replicate: LB, Redis, app, DB.

   - REDIS down  -> replica AUTO-PROMOTE (Redis Sentinel / cluster failover). seva chalti rahti.
                    poori Redis layer gayi (primary + saare replica) -> TAB FAIL-OPEN (last resort).
                    order: replica failover -> phir fail-open.

   - LB down     -> kabhi 1 LB nahi chalate. multiple LB (active-active / active-passive).
                    health-check + failover: dead LB detect -> traffic doosre pe.
                       (Route 53 health-check dead LB se traffic hata deta, YA floating/Virtual-IP standby LB pe shift.)
                    cloud ALB khud MULTI-AZ + internally redundant -> ek box nahi -> isse SPOF nahi maante.

   - ★ MECHANISM (zaroori): replicate kar diya, par koi failure DETECT karke REDIRECT kare =
                    health-check + failover (Redis=Sentinel, LB=Route53/VIP). warna sirf replica bekaar.

   - TOP = DNS (Route 53) globally distributed/managed -> khud single-box nahi -> top-level SPOF nahi banta.

   crisp: Redis down -> replica -> (last) fail-open | LB down -> dusra LB (health-check/VIP) | har layer redundant + auto-failover.
```

---

## 5 Algorithms — 4 Methods

### Token Bucket

```
Tokens auto-add: 1/sec
       │
       ▼
   ┌──────────┐
   │  BUCKET  │
   │  │   max 10 tokens
   │ (max 10) │
   └────┬─────┘
        │
   Req aayi → token le → allow
                │
                ▼
            (token used)

   Bucket empty? → REJECT
```

### Leaky Bucket

```
   Reqs aaye ──→ ┌──────────┐
                 │  BUCKET  │
                 │    │
                 └────┬─────┘
                      │ hole (1/sec)
                      ▼
                Process at FIXED rate

   Bucket FULL → overflow → REJECT
```

### Fixed Window Counter

```
   Time:  10:00───────10:01───────10:02
   Count:    [0→5]      [0→5]      [0→5]
              │          │          │
              reset      reset      reset
```

**Edge spike problem:**
```
   10:00:59 → 5 reqs
   10:01:00 → 5 reqs (new window)
   = 10 reqs in 2 seconds!
```

### Sliding Window

```
   Time:    │←──── 60 seconds ────→│ NOW
            │                       │
   Reqs:    ▓ ▓ ▓ ▓     ▓     ▓ ▓ ▓

   Count last 60 sec = 8
   Limit = 5? → REJECT
```

#### Bus Stand Analogy (How Window ACTUALLY Shifts)

```
Watchman bus stand pe register rakhta:
   Rule: "Last 1 hour mein 5 passenger MAX"

   ┌─────────┬───────────┐
   │ Time    │  Name     │
   ├─────────┼───────────┤
   │ 10:05   │  Ramesh   │
   │ 10:15   │  Suresh   │
   │ 10:30   │  Mukesh   │
   │ 10:45   │  Naresh   │
   │ 10:55   │  Dinesh   │
   └─────────┴───────────┘
```

```
CASE 1: Naya passenger 11:00 pe aaya
   Watchman: "abhi 11:00, 60 min peeche = 10:00"
   Count entries 10:00 ke baad = 5
   Limit hit → REJECT

CASE 2: Naya passenger 11:10 pe aaya
   Watchman: "abhi 11:10, 60 min peeche = 10:10"
   10:05 wala AB OUT (60 min se purana)
   Count = 4
   Limit OK → ALLOW + add 11:10
```

```
WINDOW SHIFT KAISE HOTI:

   NOT timer-based:
        "Har second window 1 step slide"
        (background job NAHI hoti)

   ON-DEMAND (request-driven):
        "Naya request aata = watchman ABHI se 60 min peeche dekhta"
        Calculation fresh = window naturally shift

   = Same end result
   = Trigger = request arrival, not clock
```

```
TIME:  10:00 ──────── 10:30 ──────── 11:00 ──────── 11:30
                                         │
                                  passenger aaya 11:00 pe
                                         │
                                         ▼
                            Window CALCULATE hua:
                        ←──── 60 min back ────┤
                       10:00              11:00


TIME:  10:00 ──────── 10:30 ──────── 11:00 ──────── 11:30
                                                 │
                                          passenger aaya 11:10 pe
                                                 │
                                                 ▼
                                  Window CALCULATE hua:
                              ←──── 60 min back ────┤
                             10:10              11:10
```

### Comparison Table

```
┌──────────────────┬───────────┬─────────┬──────────┬─────────┐
│  Algorithm       │ Bursts    │ Smooth  │ Memory   │ Common  │
├──────────────────┼───────────┼─────────┼──────────┼─────────┤
│ Token Bucket     │ YES    │ Variable│ Low      │ AWS,Stripe│
│ Leaky Bucket     │ NO     │ YES     │ Low      │ Throttle │
│ Fixed Window     │ Edge fail │ NO      │ Lowest   │ GitHub  │
│ Sliding Window   │ Smooth    │ YES     │ High     │ Cloudflare│
└──────────────────┴───────────┴─────────┴──────────┴─────────┘

         MOST COMMON ─────► Token Bucket

   ★ WHY TOKEN BUCKET (interview default-pick):
     - real traffic SPIKY hota (user 10 req ek-saath, phir shaant) -> token bucket JAMA-tokens se
       ye burst allow karta = user-friendly. (bucket bhara -> ek-saath nikal jaao.)
     - Leaky Bucket -> sabko CONSTANT-rate pe smooth karta -> burst BLOCK (real-user ko laggy lagta).
     - Sliding Window -> accurate PAR har request ka timestamp -> MEMORY-heavy.
     - Fixed Window -> sasta par WINDOW-EDGE pe double-burst (10:00:59 + 10:01:00 = 2x limit ek-saath).
     => default = TOKEN BUCKET: burst-friendly + low-memory (AWS/Stripe yehi use karte).
```

---

## ★ Route 53 kya hai (AWS DNS — sabse upar wala hop)

```
   - AWS ki DNS service (naam "53" = DNS port 53). Amazon ne banaya.
   - jo bhi user aaye, ye use SAHI IP tak ROUTE karta -> isiliye har HLD me sabse UPAR.
   - DNS = PHONEBOOK: naam (amazon.in) -> IP address. computer IP samajhta hai, naam nahi -> koi translate kare = DNS.
   - Route 53 ke kaam:
       1. naam -> IP resolve (base DNS).
       2. ★ HEALTH-CHECK + FAILOVER: dead server/LB se traffic HATA deta, sirf zinda pe bheje. (LB-SPOF fix isi se.)
       3. latency/geo routing: user ko NEAREST + fastest region pe le jaaye.
       4. domain registration (domain khareedna) bhi.
   - flow: user domain type -> Route 53 (resolve + health-check) -> nearest HEALTHY LB/region -> CDN -> LB -> app.
   crisp: Route 53 = AWS ka smart DNS -> naam->IP + health-check (zinda pe) + nearest-region routing.
```

---

## 6 Architecture — Full Picture

```
                  USER
                   │
                   ▼
            ┌──────────────┐
            │  Route 53    │
            └──────┬───────┘
                   │
                   ▼
            ┌──────────────┐
            │  CloudFront  │
            └──────┬───────┘
                   │
                   ▼
            ┌──────────────┐
            │     ALB      │
            └──────┬───────┘
                   │
                   ▼
            ┌──────────────────────┐
            │  API GATEWAY         │
            │  ┌────────────────┐  │
            │  │ Rate Limiter   │──┼──┐
            │  │ Middleware     │  │  │
            │  └────────────────┘  │  │
            └──────────┬───────────┘  │
                       │              │
            ALLOWED ◄──┴──► REJECTED  │
                       │              │
                       ▼              │
                ┌──────────────┐      │
                │  App Servers │      │
                └──────────────┘      │
                                      ▼
                              ┌──────────────┐
                              │ REDIS CLUSTER│
                              │ (counters)   │
                              └──────────────┘
                                      │
                                      ▼
                              ┌──────────────┐
                              │  Kafka →     │
                              │  Pattern     │
                              │  Detection   │
                              └──────────────┘
```

---

## 7 Request Flow Inside Rate Limiter

```
Request: "User X wants /api/login"
              │
              ▼
   ┌──────────────────────────┐
   │ 1. Identify user         │
   │    (IP / user_id /       │
   │     API key)             │
   └────────────┬─────────────┘
                │
                ▼
   ┌──────────────────────────┐
   │ 2. Redis pe key check:   │
   │    "rate:login:userX"    │
   │    INCR atomic           │
   └────────────┬─────────────┘
                │
                ▼
   ┌──────────────────────────┐
   │ 3. Count > limit?        │
   └────────────┬─────────────┘
                │
       ┌────────┴────────┐
       ▼                 ▼
     YES               NO
       │                 │
       ▼                 ▼
   REJECT 429         FORWARD
   + Retry-After      to API
```

---

## 8 Redis Keys + Atomic Ops

```
KEY FORMAT:
   rate:{endpoint}:{user_identifier}   →  count

EXAMPLES:
   rate:login:192.168.1.5         →  4
   rate:signup:user_456           →  2
   rate:search:apikey_xyz789      →  47

   TTL = window time (60 sec)
   = key auto-expires
```

```
ATOMIC OPERATION (no race):

   MULTI
     INCR rate:login:userX
     EXPIRE rate:login:userX 60
   EXEC

   = atomic, thread-safe
```

```
★ WHY atomic? — REDIS SINGLE-THREADED (23-Jul, mock me seekha):

   Redis ek time pe SIRF EK command chalata (single-threaded) -> beech me koi
   doosri request ghus NAHI sakti -> isiliye INCR / MULTI-EXEC apne-aap ATOMIC.

   RACE jo ise rokta: 2 request EK saath, bucket me 1 token bacha ->
     read-then-write hota to DONO ko token milta (limit toot) ->
     par INCR atomic -> ek poora hoke hi doosra chalta -> safe.

   ★ multi-STEP logic (token-bucket: refill + check + decrement) -> LUA SCRIPT:
     Redis poore Lua script ko ek ATOMIC unit me chalata (production standard).

   INTERVIEW LINE: "Since Redis is single-threaded, the check-and-decrement is
     atomic -- INCR for a counter, or a Lua script for token-bucket logic, to
     avoid the read-modify-write race."

   ★ CONNECT (2-Sep mock): ye WAHI race hai jo IDEMPOTENCY me thi (HDFC
     duplicate-payment) -- naive containsKey+put ke beech ka gap -> 2 request
     ghus jaati -> double charge. Wahan ilaaj = putIfAbsent (atomic). Yahan
     ilaaj = Lua script (atomic). DONO = "read+modify+write ko EK atomic
     unit banao, beech ka gap band". Rate-limiter <-> idempotency = SAME
     race, same lock-cure.
```

---

## 9 Tiered Limits

```
┌──────────────┬─────────────────────┐
│  Tier        │  Limit               │
├──────────────┼─────────────────────┤
│  Anonymous   │  60/hour            │
│  Free user   │  5,000/hour         │
│  Pro user    │  10,000/hour        │
│  Enterprise  │  Custom             │
└──────────────┴─────────────────────┘

   Request → check user tier in DB
              │
              ▼
        limit fetched
              │
              ▼
        Redis counter compared
```

---

## Distributed Rate Limiting

### PROBLEM
```
   USER (arpan_123)
       │
       ├── Bangalore from   ──► India region: 50 reqs
       ├── Berlin from      ──► EU region:    50 reqs
       └── US VPN from      ──► US region:    50 reqs

   Each region thinks 50/100 OK
   Total = 150 reqs > limit (100)
   Limit broken
```

### SOLUTION 1: Centralized Redis
```
   ┌──────────┐  ┌──────────┐  ┌──────────┐
   │  US      │  │  EU      │  │  Asia    │
   └────┬─────┘  └────┬─────┘  └────┬─────┘
        │             │             │
        └─────────────┼─────────────┘
                      ▼
              ┌──────────────┐
              │ GLOBAL REDIS │
              └──────────────┘

   Accurate    High latency, SPOF
```

### SOLUTION 2: Local + Async Sync
```
   ┌──────┐  ┌──────┐  ┌──────┐
   │ US   │  │ EU   │  │ Asia │
   │Redis │  │Redis │  │Redis │
   └──┬───┘  └──┬───┘  └──┬───┘
      └─────────┼─────────┘
                │ async (1 sec)
                ▼
        ┌──────────────┐
        │  Aggregator  │
        └──────────────┘

   Fast    Slight over-limit possible
```

### SOLUTION 3: Region-Sticky (BEST)
```
   USER (arpan_123, home = INDIA)
       │
       ├── Bangalore   ──► India Edge ──► INDIA region
       ├── Berlin      ──► EU Edge    ──► INDIA region
       └── US VPN      ──► US Edge    ──► INDIA region

   ALL paths end at INDIA region
   = Local Redis sees full picture
```

```
ROUTING LOGIC:
   hash(user_id) % regions = home_region

   hash("arpan_123") % 3 = 0 → INDIA
   hash("john_456") % 3  = 1 → EU
   hash("alex_789") % 3  = 2 → US
```

---

## 11 Layered Defense (Production Reality)

```
   USER REQUEST
        │
        ▼
   ┌───────────────────────┐
   │  LAYER 1: Rate Limit  │  ← soft block (temporary)
   │  (429 reject)         │     "wait 60 sec"
   └─────────┬─────────────┘
             │ rejected events
             ▼
   ┌───────────────────────┐
   │  KAFKA event stream   │  ← async analytics
   └─────────┬─────────────┘
             │
             ▼
   ┌───────────────────────┐
   │  LAYER 2: Pattern     │  ← detect repeat abusers
   │  Detection            │     daily abuse pattern?
   └─────────┬─────────────┘
             │
             ▼
   ┌───────────────────────┐
   │  LAYER 3: WAF /       │  ← PERMANENT block
   │  IP Blocklist         │     blocked at edge
   └───────────────────────┘
```

```
Why not block immediately?
   ┌────────────────────┬──────────────────────────┐
   │  Risk              │  Reason                   │
   ├────────────────────┼──────────────────────────┤
   │ False positives    │ Real user 10 fast clicks │
   │ Shared IPs (NAT)   │ 1 IP = 100 users         │
   │ Legitimate bursts  │ Marketing campaign       │
   └────────────────────┴──────────────────────────┘

   Rate limit = forgiving (retry possible)
   WAF block = permanent (verified abuse only)
```

---

## 12 Response Headers

```
SUCCESS (200 OK):
   X-RateLimit-Limit:     100
   X-RateLimit-Remaining: 47
   X-RateLimit-Reset:     1715180400  (unix time)

REJECTED (429 Too Many Requests):
   Retry-After:           30   (wait seconds)
   X-RateLimit-Limit:     100
   X-RateLimit-Remaining: 0
   X-RateLimit-Reset:     1715180400
```

---

## 13 Read Flow Line (Memorize)

```
"User request → Route 53 → CloudFront → ALB →
 API Gateway pe Rate Limiter middleware →
 Redis se atomic counter check (INCR + EXPIRE) →
 limit ke andar? App Server →
 limit cross? 429 with Retry-After header →
 abuse pattern? Kafka pe event for analytics →
 repeat offender? WAF blocklist permanent ban"
```

---

## Components Summary

```
┌─────────────────┬─────────────────────────────────┐
│  Component      │  Role                            │
├─────────────────┼─────────────────────────────────┤
│  Route 53       │  DNS                             │
│  CloudFront     │  CDN                             │
│  ALB            │  Load balancing                  │
│  API Gateway    │  Where rate limiter sits         │
│  Redis          │  Atomic counter (INCR)           │
│  Counter Key    │  rate:{endpoint}:{user}          │
│  TTL            │  Auto-reset window               │
│  429 Response   │  Reject + Retry-After header     │
│  Kafka          │  Async pattern detection events  │
│  Pattern Svc    │  Detect repeat abusers           │
│  WAF            │  Permanent IP block              │
└─────────────────┴─────────────────────────────────┘
```

---

# 7-STEP RAIL DRIVE

> (RAIL: 04_HLD/HLD_APPROACH_DELIVERY.md) — Requirements → Estimate → API → Data model → HL boxes → Deep-dive → Bottleneck. Interview me isi flow me bolo.

## STEP 1 — REQUIREMENTS
```
   FUNCTIONAL:  over-limit request REJECT (429), legit ALLOW.  limit e.g. = "5/min per IP"
   NON-FUNCTIONAL:
     - LOW LATENCY = critical -> rate limiter HAR request ke saamne baithta; ye slow -> POORA API slow.
       isliye Redis (in-memory).
     - FAIL-OPEN: rate limiter / Redis DOWN -> requests ALLOW karo (block NAHI).
       soch: legit-users-block >> thodi abuse. (FAIL-CLOSED = block-all -> sirf payment/security-critical pe.)
   CLARIFY:  limit kis pe? (IP / user-id / API-key) . per-endpoint? . tiered (free/pro)?
```

## STEP 2 — ESTIMATE (★ rate-limiter ka asli insight)
```
   Rate limiter HAR request ke saamne baithta -> system ka SABSE HIGH-QPS component (gateway pe millions/sec).
   ★ KEY LINE: "Ye storage-problem nahi, LATENCY problem hai — har request pe check <1ms hona chahiye,
                warna poora API slow. Isliye in-memory Redis (counters), na DB."
   -> per-key state chhota (counter + TTL), par ops-rate BAHUT high -> Redis atomic INCR.
```

## STEP 3 — API / RESPONSE
```
   limit OK   -> forward to API
   limit HIT  -> HTTP 429 (Too Many Requests) + Retry-After: 30 (kitna wait)
   HEADERS:   X-RateLimit-Limit | X-RateLimit-Remaining | X-RateLimit-Reset (unix time)
```

## STEP 4 — DATA MODEL (Redis key)
```
   KEY:  rate:{endpoint}:{user} -> count     e.g. rate:login:192.168.1.5 -> 4
   TTL = window time (60 sec) -> key AUTO-EXPIRE (window reset ho jaata)
   ATOMIC (no race):  MULTI -> INCR key -> EXPIRE key 60 -> EXEC   (ek atomic unit)
   # WHY atomic: Redis SINGLE-THREADED -> ek time ek command -> INCR khud atomic (read-modify-write race nahi).
     multi-step (token-bucket: refill+check+decrement) -> LUA SCRIPT (poora ek atomic unit).
```

## STEP 5 — HL BOXES (arch + placement)
```
   User -> Route53 (DNS) -> CloudFront (CDN) -> ALB -> API Gateway (rate-limiter middleware) -> App
                                                         |- Redis cluster (counters, atomic INCR)
                                                         \- Kafka -> pattern-detection (abuse)
   PLACEMENT (where it sits): API Gateway | separate service | app-library -> WINNER = API GATEWAY.
     # WHY front/gateway (reject EARLY): over-limit request ko backend tak jaane hi mat do -> backend resources bachte.
   # WHY Redis (CENTRALIZED): multi-server -> har server apna count (s1=3, s2=4, s3=2) -> total 9, par kisi ek ko
     5-limit cross dikha hi nahi -> limit TOOT. -> SINGLE SOURCE OF TRUTH = Redis + atomic INCR.
   # Route53 = AWS smart DNS: naam->IP + health-check (zinda server pe route) + nearest-region.
```

## STEP 6 — DEEP DIVE: Algorithms (4 methods)
```
   1. TOKEN BUCKET  -> bucket me tokens auto-add (1/sec, max N). req -> token lo -> allow; empty -> reject.
        # MOST COMMON (AWS/Stripe). BURSTS allow karta (jama tokens).
   2. LEAKY BUCKET  -> req bucket me fixed-rate se leak/process. full -> overflow reject. SMOOTH, no burst.
   3. FIXED WINDOW  -> per-minute counter (0-5, reset).
        # EDGE SPIKE BUG: 10:00:59 pe 5 + 10:01:00 pe 5 = 10 req in 2 sec (window boundary pe double).
   4. SLIDING WINDOW -> "last 60 sec" me count (request-driven: naya req -> abhi se 60 sec peeche dekho).
        SMOOTH + accurate, par MEMORY zyada.
```

## STEP 7 — BOTTLENECK / SCALE
```
   TIERED LIMITS:  anonymous 60/hr . free 5k/hr . pro 10k/hr . enterprise custom (user-tier DB se).

   DISTRIBUTED (user multi-region): Bangalore+Berlin+US -> VPN se har region 50 -> total 150 > 100 -> TOOT.
      FIX ladder:  centralized-Redis (accurate PAR SPOF/latency)
                   -> local + async-sync (fast PAR slight over-allow)
                   -> # REGION-STICKY (BEST): hash(user-id) % region -> home region -> saare paths ek region -> local Redis.

   ★ ACCURACY vs LATENCY (local+async-sync ka asli trade-off -- ye decision yaad rakh):
      local+async-sync = har instance apne LOCAL count se allow karta, sync BAAD me -> OVER-ALLOW.
         e.g. limit 100, 5 instances -> har ek sync-se-pehle kuch nikaal deta -> user 100 se ZYADA le jaa sakta.
         limit EXACT nahi rehti -> APPROXIMATE/soft ho jaati.
      KAB CHALEGA (over-allow OK)  : limit sirf "server bachane" ke liye (general API throttle/abuse). Thoda upar-neeche se aafat nahi.
      KAB NAHI CHALEGA (exact chahiye): limit = PAISA / SECURITY / correctness ->
         "3 OTP attempts" (security) . "10 free calls phir charge" (billing) . withdrawal-limit
         -> yahaan CENTRAL ATOMIC (Redis+Lua), latency ki keemat bhugto.
      1-LINE:  protective/soft limit -> local+async (fast) | money/security limit -> central atomic (exact).

   RELIABILITY:  Redis DOWN -> Replica (Sentinel auto-promote) -> (last) FAIL-OPEN.
                 overload -> SHARDING (user/region). SPOF-chain: har layer 2-2 + health-check/failover (LB, Redis, app).

   LAYERED DEFENSE (rate-limit AKELA kyun nahi):
     - false positives (real-user fast clicks) . shared NAT IP (1 IP = 100 user) . legit bursts
     -> rate-limit + Kafka(pattern-detect) + WAF milke.

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
