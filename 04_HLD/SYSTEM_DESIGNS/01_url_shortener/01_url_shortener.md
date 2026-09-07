# URL Shortener — 7-STEP RAIL (single spine, revise top→bottom)

> RAIL: Requirements → Estimate → API → Data-model → HL-boxes → Deep-dive → Bottleneck.
> (framework: 04_HLD/HLD_APPROACH_DELIVERY.md). Arpan-derived 21-Jun; merged into clean 7-step 7-Sep.
> Problem (1 line): long URL -> short 6-7 char code; short pe click -> original pe redirect (302).

```
   https://amazon.in/dp/B0BLKJSDKFJ934KJSDF/ref=...      (long)
                          │
                          ▼
                    bit.ly/abc123                         (short)
```

---

## STEP 1 — REQUIREMENTS (chup mat baitho, clarify karo)

```
FUNCTIONAL:
   • long URL -> short URL banao (POST)
   • short URL pe click -> original URL pe REDIRECT (GET -> 302)
   • analytics / click-tracking dashboard
   • (optional) custom alias, expiry

NON-FUNCTIONAL:
   • low latency (fast redirect — p99 < 200ms)
   • high availability (redirect kabhi down na ho)
   • READ-HEAVY (click >> create)

CLARIFYING Qs (interviewer se poochho):
   • custom short-URL allow karna hai?
   • links expire hote ya hamesha rehte?
   • analytics kitni real-time chahiye?
```

```
USE CASES:                         WHY 302 (yahin flag kar dena):
   ┌────────────────────┐             har hit SERVER pe aata -> click count milta
   │ Twitter/SMS limits │             (301 permanent -> browser cache -> analytics MISS)
   │ Marketing tracking │
   │ QR codes / print   │
   │ Aesthetics         │
   └────────────────────┘
```

---

## STEP 2 — ESTIMATE (scale / numbers — rough, exact pe mat atko)

```
ASSUMPTION (ek hi maano, poore note me consistent): 100M writes/DAY.
TRICK: 1 din ~ 100,000 sec (10^5) -> easy division.

WRITES (shorten):                READS (redirect):
   100M / day                       100× writes
   = 10^8 / 10^5                     ≈ 100,000 / sec
   ≈ 1000 writes/sec

         ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
                    READ : WRITE  =  100 : 1
         ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
                 => READ-HEAVY => CACHING IS CRITICAL
```

```
STORAGE (100M PER DAY -> ×365 din, NA ×12 months):
   100M/day × 365 × 5 yr  = ~180 BILLION URLs
   × 500 bytes            ≈ ~90 TB
   ⚠ slip-yaad: "5yr×12mo×100M×500B=3TB" GALAT tha (wo 100M/MONTH maan ke) -> per-DAY consistent = ~90 TB.
```

```
★ SCALE = ASSUMPTION pe depend (Hello-Interview + day/month sabak):
   - Modest: 1 BILLION total URLs -> 1B × 500B ≈ ~500 GB -> SINGLE DB me FIT -> sharding NAHI.
   - Aggressive: 100M writes/DAY -> billions -> ~90 TB -> shard by shortCode.
   - DONO sahi -> farak sirf ASSUMPTION ka. INTERVIEW: assumption BOLO + usse reason karo; exact pe mat atko.
   ★ DELIVERY-GEM: real-Bitly ~500GB single-DB me aata -> BINA-zaroorat SHARD mat karo (over-engineering = common galti).

★ LATENCY (concrete NFR bolo): redirect p99 < 200ms.
★ FAST LOOKUP: shortCode PRIMARY-KEY (B-tree) index -> O(log n) (disk pe bhi tez) + Redis-LRU hot-URLs.
```

---

## STEP 3 — API DESIGN (just 2 endpoints)

```
   POST /api/shorten                        GET /{code}   (e.g. /abc123)
   ─────────────────                        ─────────────
   Request:                                 Response:
   {                                            HTTP 302 Found
     "long_url": "https://amazon...",           Location: https://amazon...
     "custom_code": "arpan-resume" (optional)
   }                                        YAAD: banana=POST, laana=GET (swap mat karna)
   Response:
   {
     "short_url": "bit.ly/abc123",
     "expires_at": "2031-05-08"
   }
```

```
301 (permanent) vs 302 (temporary):
   ┌──────┬─────────────┬───────────────┐
   │ 301  │ Cache OK    │ NO tracking   │
   │ 302  │ No cache    │ Tracks clicks │ ← bit.ly uses 302
   └──────┴─────────────┴───────────────┘

★ 302 ke DO reason bolo:
   1. CLICK-ANALYTICS  -> har hit server pe aata, count milta (301 cache -> nahi milta)
   2. EXPIRY / UPDATABLE longUrl -> 301 browser cache kar leta -> expire/change pe purana toot jaata;
      302 = har baar fresh -> safe.
```

---

## STEP 4 — DATA MODEL + DB CHOICE (KYUN bolo)

```
CORE = HashMap pattern (KEY -> VALUE):
   ┌──────────────┬──────────────────────────┐
   │  short_code  │  long_url                │
   ├──────────────┼──────────────────────────┤
   │  abc123      │  https://amazon.in/...   │
   │  xyz789      │  https://flipkart.com/.. │
   └──────────────┴──────────────────────────┘
         KEY (partition)      VALUE

SCHEMA:  short_code (PARTITION KEY) -> long_url  (+ created_at, expires_at, user_id?)
   KEY = short_code KYUN: redirect hamesha short_code se aata (GET /{code}) ->
         short_code pe partition -> SINGLE-partition point-read -> O(1), poora cluster scan nahi.
```

```
ACCESS PATTERN:  INSERT once + SELECT WHERE short_code = ?  = pure key-value lookup (no joins).

DB CHOICE:
   ┌──────────────┬─────────────┬──────────────┬─────────────┐
   │  Database    │  Pattern    │  Scale       │  Verdict    │
   ├──────────────┼─────────────┼──────────────┼─────────────┤
   │  MySQL       │  Relational │  Up to ~1B   │  Works      │
   │  Mongo       │  Document   │  Up to ~10B  │  Works      │
   │  Cassandra   │  Wide-col   │  Trillions   │  Optimal    │
   │  DynamoDB    │  K-V (AWS)  │  Trillions   │  Optimal    │
   │  Redis       │  In-memory  │  Cache layer │  Always +   │
   └──────────────┴─────────────┴──────────────┴─────────────┘
   WHY NoSQL (Cassandra/DynamoDB): simple key-based point-lookup, no joins, huge+write-heavy scale ->
   horizontal scaling + high write throughput + tunable consistency. ("power" nahi -> ACCESS-PATTERN justify karta.)
```

---

## STEP 5 — HIGH-LEVEL BOXES (architecture + flow)

```
                       USER
                        │
                        ▼
                 ┌─────────────┐
                 │  Route 53   │ DNS (naam->IP + health-check + geo-routing)
                 └──────┬──────┘
                        ▼
                 ┌─────────────┐
                 │ CloudFront  │ CDN (static assets)
                 └──────┬──────┘
                        ▼
                 ┌─────────────┐
                 │     ALB     │ Load Balancer
                 └──┬───┬───┬──┘
            ┌───────┘   │   └───────┐
            ▼           ▼           ▼
       ┌────────┐  ┌────────┐  ┌────────┐
       │App S1  │  │App S2  │  │App S3  │  Spring Boot (auto-scaling)
       └───┬────┘  └───┬────┘  └───┬────┘
           └─────┬─────┴─────┬─────┘
       ┌─────────┼───────────┼─────────────┐
       ▼         ▼           ▼             ▼
   ┌──────┐  ┌────────┐  ┌─────────┐  ┌─────────┐
   │REDIS │  │COUNTER │  │ KAFKA   │  │   DLQ   │
   │cache │  │service │  │ (async) │  └─────────┘
   └──┬───┘  └────────┘  └────┬────┘
      │ miss                  ▼
      ▼                 ┌──────────────┐
   ┌────────────┐       │ Analytics    │
   │ CASSANDRA  │       │ Service + DB │
   │ (urls DB)  │       └──────────────┘
   └────────────┘
```

```
READ FLOW (cache-aside):              WRITE FLOW:
   USER click                            USER POST
       │                                     │
   LB → App                              LB → App
       │                                     │
   Redis HIT? ─yes─► return              Counter Service (range+Base62)
       │                                     │
       no                                ID 123456 -> "8m3"
       ▼                                     │
   Cassandra ───► Redis populate            ▼
       │                                  Save to: • Redis  • Cassandra
       ▼                                     │
   302 redirect                              ▼
       │                                  Return short URL
   ASYNC ► Kafka (analytics)
```

```
★ Route 53 = AWS ka smart DNS: naam->IP + HEALTH-CHECK/failover (dead LB se traffic hata) + nearest-region routing + domain-registration. (LB-SPOF fix isi se.)

COMPONENTS SUMMARY:
   Route 53=DNS · CloudFront=CDN · ALB=LB · App=Spring Boot logic · Redis=cache(95% hit) ·
   Cassandra=permanent store · Counter Svc=range+Base62(7-char) · Kafka=async analytics · Analytics DB=separate query store.
```

---

## STEP 6 — DEEP DIVE: short code kaise GENERATE? (design ka DIL)

```
3 METHODS:
   ┌─────────────────┬──────────────┬──────────────┬─────────────┐
   │   Method        │  Speed       │  Collision   │  Length     │
   ├─────────────────┼──────────────┼──────────────┼─────────────┤
   │ MD5 / Random    │  Fast        │  YES (DB check har baar)│ 6-7 │
   │ Counter         │  Fast        │  NO          │  Variable   │
   │ Counter+Base62  │  Fast        │  NO          │  Compact    │ ★ WINNER
   └─────────────────┴──────────────┴──────────────┴─────────────┘
   WINNER = Counter+Base62: counter guaranteed UNIQUE (repeat nahi -> zero collision -> NO DB check) + chhota.
   (Random/MD5 ko har baar "already exists?" DB-check chahiye -> extra read, slow at scale.)
```

```
BASE62 (index -> char):
   0─9   -> '0'─'9'   (10)      CONVERSION 1,000,000,000 -> ?
   10─35 -> 'a'─'z'   (26)         baar-baar ÷62, remainders REVERSE padho -> "15FTGg"
   36─61 -> 'A'─'Z'   (26)         => 1 BILLION = 6 chars only
                      ─────      COMPRESSION: 62^7 = 3.5 TRILLION combos in 7 chars = 100+ yrs at scale
                      Total 62
```

```
DISTRIBUTED COUNTER PROBLEM (multi-server -> same counter -> collision):
   S1 counter=5, S2 counter=5, S3 counter=5 -> sab ++ -> "6" -> 3 URLs = COLLISION!

SOLUTIONS:
   ┌────────────────────┬────────────┬─────────────┬───────────┐
   │  Approach          │ Coord      │ Bottleneck  │ Verdict   │
   ├────────────────────┼────────────┼─────────────┼───────────┤
   │ DB atomic counter  │ Per-write  │ Yes (DB)    │ Slow      │
   │ Redis INCR         │ Per-write  │ Yes (Redis) │ Better    │
   │ Range allocation   │ Per-batch  │ 1000× less  │ ★ WINNER  │
   └────────────────────┴────────────┴─────────────┴───────────┘
   ★ RANGE ALLOCATION: central ticket/ID service har server ko ek BLOCK deta (1-1000, 1001-2000...);
     server locally deta -> har-request coordination NAHI -> block khatam -> agla range maango.
     restart pe kuch numbers waste = OK, collision nahi. (= counter+Base62 ka scale-version.)
```

```
CUSTOM SHORT CODES:
   POST { custom_code? } -> CUSTOM: validate (length OK, not reserved, no profanity, unique) -> conflict? 409
                         -> else AUTO-GENERATE (counter range+Base62) -> Save: Redis + Cassandra -> return.
   RESERVED WORDS (block — system routes): admin, api, login, settings, help, docs, pricing, blog.
   RACE (2 concurrent same custom): both check "available" -> both save -> dup.
     FIX: DB UNIQUE constraint / INSERT IF NOT EXISTS (atomic).
```

```
★ DEEP-DIVE GOTCHAS (interviewer kuredega):
   1. CACHE-TTL: Redis me expiry na ho -> expired URL bhi serve ho jaaye = FR violate.
        FIX: cache-entry TTL = URL-expiry (Redis SET ... EX <expiry>) -> apne-aap nikal jaaye.
   2. COUNTER BATCHING: write-service Redis se EK BAAR 1000 counts (range) -> per-request Redis-hit nahi. (= range allocation)
   3. COLLISION: shortCode pe UNIQUE-CONSTRAINT + retry (pre-read check NAHI). Counter se waise bhi unique-by-design -> constraint = safety net.
```

---

## STEP 7 — BOTTLENECK / SCALE / SPOF

```
   rate limiting (abuse/hot-key rok) | READ -> read REPLICAS + cache | WRITE -> SHARDING (write-replica nahi hota)
   shard by shortCode (billions ek DB nahi) | async analytics (Kafka -> redirect block na ho) | geo-routing (nearest region)
   ★ SPOF: counter-coordinator khud -> 2-node (active-passive) ya range-allocation (already tolerate karta).

WRAP: Client->CDN->LB->App->Redis->NoSQL(sharded by shortCode); counter+Base62 (range); read-replicas+cache; async analytics.
      Aage: custom URLs, expiry/TTL cleanup, geo-distribution.
```

---

## ★ DELIVERY NUGGETS (round-crack karne walon se)
```
   • Har design-decision ko REQUIREMENT se jodo (ek-ek functional-req karke).
   • HLD = DISCUSSION, perfect script nahi — interviewer ke saath decide karo.
   • ESTIMATE: ek QUICK estimate (scale justify) -> aage. Exact (1500 vs 2000 RPS) design nahi badalta.
     [par bilkul skip bhi mat karo -> Zomato-reject case]
   • CORRECTIONS (soch sahi thi): GET/POST swap (banana=POST), "write replicas" -> sharding, KEY=shortCode.
```

---

[← HLD README](README.md)
