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
ASSUMPTIONS:
   100M users × 1 URL/month = 100M URLs/month
   Each URL clicked 100×    = 10B clicks/month
   Storage retention        = 5 years
```

```
WRITES (shorten):                READS (redirect):
   100M / 30 days                  10B / 30 days
   = 3.3M / day                    = 333M / day
   = 40 / sec                      = 4000 / sec

         ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
                    READ : WRITE
                       100 : 1
         ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

         CACHING IS CRITICAL
```

```
STORAGE:
   5 yr × 12 mo × 100M URLs × 500 bytes = 3 TB
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

[← HLD README](README.md)
