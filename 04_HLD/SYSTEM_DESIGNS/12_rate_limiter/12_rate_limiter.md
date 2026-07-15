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

[← HLD README](../README.md)
