# URL Shortener — Visual System Design

---

## 1 Problem (1 line)

```
   https://amazon.in/dp/B0BLKJSDKFJ934KJSDF/ref=...      (long)
                          │
                          ▼
                    bit.ly/abc123                         (short)
```

```
USE CASES:
   ┌────────────────────┐
   │ Twitter/SMS limits │
   │ Marketing tracking │
   │ QR codes / print   │
   │ Aesthetics         │
   │ Future redirect    │
   └────────────────────┘
```

---

## 2 Core Idea — HashMap Pattern

```
   ┌──────────────┬──────────────────────────┐
   │  short_code  │  long_url                │
   ├──────────────┼──────────────────────────┤
   │  abc123      │  https://amazon.in/...   │
   │  xyz789      │  https://flipkart.com/.. │
   │  3xK9pQ2     │  https://google.com/...  │
   └──────────────┴──────────────────────────┘

         KEY              VALUE
```

```
2 OPERATIONS:

   POST /shorten              GET /abc123
        │                          │
        ▼                          ▼
   generate code              lookup code
   save (k,v)                 return long URL
        │                          │
        ▼                          ▼
   return short URL          302 redirect
```

---

## 3 Short Code Generation — 3 Methods

```
┌─────────────────┬──────────────┬──────────────┬─────────────┐
│   Method        │  Speed       │  Collision   │  Length     │
├─────────────────┼──────────────┼──────────────┼─────────────┤
│ Random          │  Fast        │  YES (check) │  6-7 chars  │
│ Counter         │  Fast        │  NO          │  Variable   │
│ Counter+Base62  │  Fast        │  NO          │  Compact    │
└─────────────────┴──────────────┴──────────────┴─────────────┘

         WINNER ──────────────────────────────► Counter+Base62
```

---

## 4 Base62 — Visual

```
INDEX → CHARACTER MAPPING

   0─9     →   '0'─'9'        (10 chars)
   10─35   →   'a'─'z'        (26 chars)
   36─61   →   'A'─'Z'        (26 chars)
                              ━━━━━━━━━━━
                              Total: 62
```

```
CONVERSION: 1,000,000,000 → ?

   1B ÷ 62 = 16129032   r 16   → "g"
   16M ÷ 62 = 260145    r 42   → "G"
   260K ÷ 62 = 4195     r 55   → "T"
   4195 ÷ 62 = 67       r 41   → "F"
   67 ÷ 62 = 1          r 5    → "5"
   1 ÷ 62 = 0           r 1    → "1"

   Read remainders REVERSE:    "15FTGg"

   1 BILLION  =  6 chars only
```

```
COMPRESSION POWER:

   62^7 = 3.5 TRILLION combinations in 7 chars
   = 100+ years of users at scale
```

---

## 5 Storage Choice

```
ACCESS PATTERN:
   INSERT once + SELECT WHERE short_code = ?
   = Pure key-value lookup
```

```
┌──────────────┬─────────────┬──────────────┬─────────────┐
│  Database    │  Pattern    │  Scale       │  Verdict    │
├──────────────┼─────────────┼──────────────┼─────────────┤
│  MySQL       │  Relational │  Up to ~1B   │  Works      │
│  Mongo       │  Document   │  Up to ~10B  │  Works      │
│  Cassandra   │  Wide-col   │  Trillions   │  Optimal    │
│  DynamoDB    │  K-V (AWS)  │  Trillions   │  Optimal    │
│  Redis       │  In-memory  │  Cache layer │  Always +   │
└──────────────┴─────────────┴──────────────┴─────────────┘
```

---

## 6 Capacity Estimation

```
ASSUMPTIONS (ek hi maano — 100M writes/DAY; poore note me consistent):
   100M new URLs / DAY
   Each URL clicked 100×   = read 100× write  (READ-HEAVY)
   Storage retention       = 5 years
```

```
WRITES (shorten):                READS (redirect):
   100M / day                       100× writes
   ÷ ~100k sec/day                  ≈ 100k / sec
   ≈ 1000 / sec

         ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
                    READ : WRITE
                       100 : 1
         ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

         CACHING IS CRITICAL
```

```
STORAGE (100M PER DAY -> ×365 din, NA ×12 months):
   100M/day × 365 × 5 yr  = ~180 BILLION URLs
   × 500 bytes            ≈ ~90 TB

   ⚠ PEHLE GALAT tha: "5yr × 12mo × 100M × 500B = 3TB"
     -> wo 100M/MONTH maan ke tha; upar 100M/DAY bola -> inconsistent.
     Ab per-DAY consistent (×365) -> ~90 TB. Unit (day vs month) hi asli slip.
```

```
★ SCALE = ASSUMPTION pe DEPEND karta (Hello-Interview + kal ka day/month sabak):
   - Modest maano: 1 BILLION total URLs (100M daily-active = zyadatar READERS, writers kam)
       -> 1B × 500B ≈ ~500 GB -> SINGLE DB me FIT -> sharding ki zaroorat NAHI.
   - Aggressive maano: 100M writes/DAY -> billions -> ~90 TB -> shard by shortCode chahiye.
   - DONO sahi -> farak sirf ASSUMPTION ka. INTERVIEW me: assumption BOLO + usse reason karo;
     exact number pe mat atko (wahi kal wala "estimate obsess mat" sabak).
   ★ DELIVERY-GEM: real-Bitly ~500GB single-DB me aata -> BINA-zaroorat SHARD mat karo
     (over-engineering = common galti). Shard SIRF jab throughput/storage genuinely demand kare.
```

```
★ LATENCY (concrete NFR bolo): redirect p99 < 200ms.
★ FAST LOOKUP: shortCode pe PRIMARY-KEY (B-tree) index -> O(log n) lookup (disk pe bhi tez).
   + Redis-LRU cache hot-URLs -> zyadatar disk hit hi nahi.
```

---

## 7 API Design — Just 2 Endpoints

```
┌─────────────────────────────────────────────────────────┐
│  POST /api/shorten                                      │
├─────────────────────────────────────────────────────────┤
│  Request:                                               │
│  {                                                      │
│    "long_url": "https://amazon...",                     │
│    "custom_code": "arpan-resume"   (optional)           │
│  }                                                      │
│                                                         │
│  Response:                                              │
│  {                                                      │
│    "short_url": "bit.ly/abc123",                        │
│    "expires_at": "2031-05-08"                           │
│  }                                                      │
└─────────────────────────────────────────────────────────┘
```

```
┌─────────────────────────────────────────────────────────┐
│  GET /abc123                                            │
├─────────────────────────────────────────────────────────┤
│  Response:                                              │
│      HTTP 302 Found                                     │
│      Location: https://amazon...                        │
│                                                         │
│  301 (permanent) vs 302 (temporary)                     │
│      ┌──────┬─────────────┬───────────────┐             │
│      │ 301  │ Cache OK    │ NO tracking   │             │
│      │ 302  │ No cache    │ Tracks clicks │ ← bit.ly    │
│      └──────┴─────────────┴───────────────┘             │
└─────────────────────────────────────────────────────────┘
```

```
★ 302 ke DO reason bolo (Hello-Interview se):
   1. CLICK-ANALYTICS  -> har hit server pe aata, count milta (301 cache -> nahi milta)
   2. EXPIRY / UPDATABLE longUrl -> 301 browser CACHE kar leta, to jab URL
        expire ho ya client longUrl badle -> 301 purana dikhaata (toot jaata).
      302 = har baar fresh -> safe.
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

## 8 Architecture — Full Picture

```
                       USER
                        │
                        ▼
                 ┌─────────────┐
                 │  Route 53   │ DNS
                 └──────┬──────┘
                        │
                        ▼
                 ┌─────────────┐
                 │ CloudFront  │ CDN (static assets)
                 └──────┬──────┘
                        │
                        ▼
                 ┌─────────────┐
                 │     ALB     │ Load Balancer
                 └──┬───┬───┬──┘
                    │   │   │
            ┌───────┘   │   └───────┐
            ▼           ▼           ▼
       ┌────────┐  ┌────────┐  ┌────────┐
       │App S1  │  │App S2  │  │App S3  │  Spring Boot
       └───┬────┘  └───┬────┘  └───┬────┘  (auto-scaling)
           │           │           │
           └─────┬─────┴─────┬─────┘
                 │           │
       ┌─────────┼───────────┼─────────────┐
       ▼         ▼           ▼             ▼
   ┌──────┐  ┌────────┐  ┌─────────┐  ┌─────────┐
   │REDIS │  │COUNTER │  │ KAFKA   │  │   DLQ   │
   │cache │  │service │  │ (async) │  └─────────┘
   └──┬───┘  └────────┘  └────┬────┘
      │ miss                  │
      ▼                       ▼
   ┌────────────┐      ┌──────────────┐
   │ CASSANDRA  │      │ Analytics    │
   │ (urls DB)  │      │ Service + DB │
   └────────────┘      └──────────────┘
```

```
READ FLOW:                   WRITE FLOW:
   USER click                    USER POST
       │                             │
       ▼                             ▼
   LB → App                      LB → App
       │                             │
   Redis HIT? ─yes─► return       Counter Service
       │                             │
       no                         ID 123456 → "8m3"
       ▼                             │
   Cassandra ───► Redis cache       ▼
       │                          Save to:
       ▼                          • Redis
   302 redirect                   • Cassandra
       │                             │
   ASYNC ► Kafka                     ▼
   (analytics)                    Return short URL
```

---

## 9 Distributed Counter Problem

```
PROBLEM (multi-server):

   Server 1      Server 2      Server 3
   counter=5     counter=5     counter=5
       │             │             │
       ▼             ▼             ▼
   counter++     counter++     counter++
   = 6           = 6           = 6
       │             │             │
       ▼             ▼             ▼
   amazon        flipkart      google
       │             │             │
       └─────────────┼─────────────┘
                     ▼
              CODE "6" → 3 URLs!
              COLLISION
```

```
SOLUTIONS:

┌────────────────────┬────────────┬─────────────┬─────────┐
│  Approach          │ Coord      │ Bottleneck  │ Verdict │
├────────────────────┼────────────┼─────────────┼─────────┤
│ DB atomic counter  │ Per-write  │ Yes (DB)    │ Slow    │
│ Redis INCR         │ Per-write  │ Yes (Redis) │ Better  │
│ Range allocation   │ Per-batch  │ 1000× less  │ Good    │
│ Snowflake IDs      │ NONE       │ NONE        │ Best    │
└────────────────────┴────────────┴─────────────┴─────────┘
```

```
SNOWFLAKE ID — 64 bit:

   ┌──────────────────┬──────────────┬─────────────┐
   │ 41 bit timestamp │ 10 bit machine│ 12 bit seq  │
   └──────────────────┴──────────────┴─────────────┘

   Each server independently generates IDs
   timestamp + machine_id ≠ same possible
   = NO coordination needed
   = 4M IDs/sec across 1024 servers
```

---

## Custom Short Codes

```
USER REQUEST FLOW:

   POST /api/shorten { custom_code?: "arpan-resume" }
                          │
                ┌─────────┴─────────┐
                ▼                   ▼
           CUSTOM PROVIDED      AUTO-GENERATE
                │                   │
                ▼                   ▼
        ┌───────────────┐    Counter Service
        │ Validations:  │    (Snowflake)
        │ Length OK   │           │
        │ Not reserved│           │
        │ No profanity│           │
        │ Unique      │           │
        └───────┬───────┘           │
                │                   │
        Conflict? ──yes──► 409      │
                │                   │
                no                  │
                │                   │
                └────────┬──────────┘
                         ▼
                Save: Redis + Cassandra
                         │
                         ▼
                Return short URL
```

```
RESERVED WORDS LIST:
   ┌──────┬──────┬─────────┬────────┐
   │ admin│  api │  login  │settings│
   │ help │ docs │ pricing │  blog  │
   └──────┴──────┴─────────┴────────┘
   = system routes — block these
```

```
RACE CONDITION (concurrent custom requests):

   T=0   User A: check "arpan" → available
   T=0   User B: check "arpan" → available
   T=1   User A: save → success
   T=1   User B: save → DUPLICATE

   FIX: DB UNIQUE constraint
        INSERT IF NOT EXISTS (atomic)
```

---

## ★ INTERVIEW GOTCHAS + NUGGETS (Hello Interview video + comments, 3-Sep)

```
1. CACHE-TTL GOTCHA (deep-dive, interviewer poochta):
   Redis me expiry store nahi -> expired URL bhi cache se serve ho sakta
   = TTL/expiry FUNCTIONAL-REQ violate.
   FIX: cache-entry ka TTL = URL-expiry (Redis SET ... EX <expiry>) -> expire pe apne-aap nikal jaaye.

2. COUNTER BATCHING (counter bottleneck fix):
   Write-service Redis se EK BAAR me next 1000 counts le le (range) -> har request pe Redis-hit nahi.
   = mera "range allocation" wahi hai. (Redis crash pe kuch range waste = OK, collision nahi.)

3. COLLISION = DB UNIQUE-CONSTRAINT + retry (pre-read check NAHI):
   shortCode pe unique-constraint; insert fail ho to naya code + retry.
   Counter/Snowflake se waise bhi unique-by-design -> constraint = safety net.

4. DELIVERY (jinhone round crack kiya):
   - Har design-decision ko REQUIREMENT se jodo, ek-ek functional-req karke.
   - HLD = DISCUSSION, perfect script nahi. Interviewer ke saath decide karo.
   - ESTIMATE pe mat atko: ek QUICK estimate (scale justify) -> aage. Exact number (1500 vs 2000 RPS) design nahi badalta. [par bilkul skip bhi mat karo -> Zomato-reject case]
```

---

## Components Summary

```
┌─────────────────┬─────────────────────────────┐
│  Component      │  Role                        │
├─────────────────┼─────────────────────────────┤
│  Route 53       │  DNS resolution              │
│  CloudFront     │  CDN — static assets         │
│  ALB            │  Load balancing              │
│  App Servers    │  Spring Boot business logic  │
│  Redis          │  Cache (95% hit rate)        │
│  Cassandra      │  Permanent URL storage       │
│  Counter Svc    │  Snowflake ID generation     │
│  Kafka          │  Async analytics events      │
│  Analytics DB   │  Separate query store        │
└─────────────────┴─────────────────────────────┘
```

---

# 8-STEP INTERVIEW FRAMEWORK DRIVE

> Arpan ne KHUD derive kiya (21 Jun) — pehla full solo HLD drive. Micro-read ke liye.
> (Framework: 04_HLD/INTERVIEW_FRAMEWORK.md)

## STEP 1 — REQUIREMENTS clarify (chup mat baitho)
```
   FUNCTIONAL:  long URL -> short URL banao;  short pe click -> original pe redirect
   NON-FUNCTIONAL:  fast redirect (low latency), high availability, READ-heavy
   clarifying Qs:  custom short-URL allow? links expire hote ya hamesha?
```

## STEP 2 — SCALE / numbers
```
   maano 100M writes/day.  TRICK: 1 din ~ 100,000 sec (10^5) -> easy division
   100M/day = 10^8 / 10^5 = ~1000 writes/sec
   read = 100x = ~100,000 reads/sec  -> READ-HEAVY (click >> create)
   -> ye number drive karta: read-heavy = cache+CDN; billions = sharding
```

## STEP 3 — API design
```
   POST /shorten   {longUrl} -> {shortUrl}      (BANANA -> POST)
   GET  /{code}    -> 302 redirect to longUrl   (LAANA  -> GET)
   YAAD: banana=POST, laana=GET (swap mat karna)
```

## STEP 4 — HIGH-LEVEL boxes
```
   Client -> CDN -> LB -> App Servers -> Redis Cache -> Database
   read (cache-aside): cache HIT -> turant return | MISS -> DB se laao -> cache daalo -> return
   HOT URLs hi cache (billions cache nahi -> popular wale)
```

## STEP 5 — DATA MODEL + DB choice (KYUN bolo)
```
   schema:  shortCode (KEY) -> longUrl   (+ createdAt, expiresAt)
   KEY = shortCode (redirect mein short se long laana -> lookup by shortCode)
   DB = NoSQL (DynamoDB/Cassandra): pure key-value, no joins, read-heavy -> fast key-lookup + horizontal scale
```

## STEP 6 — DEEP DIVE: short code kaise generate? (3 options)
```
   1. MD5 HASH       -> collision ho sakta + compute cost
   2. RANDOM (62 ch) -> collision check ke liye HAR BAAR DB hit (slow)
   3. COUNTER + Base62 (BEST) -> counter(1,2,3) -> Base62 -> code
        guaranteed UNIQUE (counter repeat nahi -> zero collision -> no DB check) + chhota
        counter SPOF na bane -> har server ko RANGE do (1-1000, 1001-2000)
   -> WINNER: counter + Base62
```

## STEP 7 — BOTTLENECK / SCALE
```
   rate limiting (abuse rok) | READ -> read REPLICAS | WRITE -> SHARDING (write-replicas nahi hota)
   shard by shortCode (billions ek DB nahi) | async analytics (click-count queue -> redirect block na ho)
   geo-routing (nearest region, latency kam)
```

## STEP 8 — WRAP
```
   Client->CDN->LB->App->Redis->NoSQL(sharded); counter+Base62; read-replicas+cache; async analytics.
   Aage: custom URLs, expiry/TTL cleanup, geo-distribution.
```

> CORRECTIONS seekhi (soch sahi thi): GET/POST swap (banana=POST), "write replicas" -> sharding,
> KEY=shortCode. Asli reasoning (read-heavy, NoSQL+kyun, hot-cache, counter+Base62, shard) khud sahi derive.

---

[← HLD README](README.md)
