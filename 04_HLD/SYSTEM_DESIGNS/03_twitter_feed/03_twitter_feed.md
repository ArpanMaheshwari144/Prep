# Twitter Feed — Visual System Design

> **Status:** FULL TOPIC DONE — Day 1 + Day 2 + Day 3 covered.

---

## 1 Problem (1 line)

```
   Tu app KHOLA → Tera home timeline dikhna chahiye
   ┌─────────────────────────────────────┐
   │ Virat:  "Match was great"           │
   │ Sachin: "Watching IPL"               │
   │ Dhoni:  "Practice today"             │
   └─────────────────────────────────────┘

   = Tweets from people TU follow karta
   = Latest first
```

---

## 2 Scale Numbers

```
   500 million users
   500 million tweets/day = ~5800/sec
   Avg user follows 200, has 200 followers
   Some celebs = 100M+ followers (Bieber problem)

   Read vs Write = ~50:1
   = READ-HEAVY
```

---

## 3 Master Analogy — Royal Kingdom

```
Kingdom mein news circulate karna problem.
Same problem Twitter ke saath.

   Notice Board     = Redis inbox per user
   Town Crier       = Fanout Service
   Palace Board     = Tweet store (Cassandra)
   Royal Scribe     = Tweet Service
```

---

## 4 Notice Board (Redis Inbox)

```
Har user ke ghar mein notice board.
Yeh notice board pe wo tweets aate jo us user ke
follow kiye hue logon ne kiye.

   Arpan ka ghar:
   ┌─────────────────────────────┐
   │  NOTICE BOARD            │
   │                              │
   │  • Suresh: "Lunch good"      │
   │  • Mukesh: "Movie"           │
   │  • Naresh: "Office"          │
   └─────────────────────────────┘

   = redis:inbox:arpan_123

Jab Arpan app khole:
   Bus apne notice board ko padh le
   = INSTANT
```

---

## 5 Town Crier (Fanout)

```
Notice board pe tweets KAISE pohchti?

Virat tweets: "Match was great!"
        │
        ▼
   ┌──────────────────────────────────┐
   │    TOWN CRIER                │
   │   (Fanout Service)                │
   │                                    │
   │   "Sab followers ke notice"       │
   │   "board pe lagao!"                │
   └──────────────────────────────────┘
        │
        ├─→ Arpan's notice board    ← updated
        ├─→ Suresh's notice board   ← updated
        └─→ ... (all followers)

= PUSH MODEL (fanout-on-write)
```

---

## 6 Bieber Problem (Hot User)

```
Bieber: 10 CRORE followers
He tweets: "Naya gaana!"
        │
        ▼
   ┌──────────────────────────────────┐
   │    TOWN CRIER                │
   │                                    │
   │   "10 CRORE notice boards mein    │
   │    likhne hain??"                  │
   │                                    │
   │   SYSTEM CHOKED                  │
   └──────────────────────────────────┘
```

```
SOLUTION: Bieber's tweets DON'T fanout.
         Stored ONLY at palace board (DB).
         Followers fetch on demand.

   ┌────────────────────────────┐
   │  PALACE NOTICEBOARD      │
   │  (Bieber's tweets here)     │
   │  = DB (Cassandra)           │
   └────────────────────────────┘
```

---

## 7 Hybrid Model (Industry Reality)

```
┌────────────────────────┬────────────────────┐
│  User type             │  Strategy           │
├────────────────────────┼────────────────────┤
│ Normal (< 10K follow)  │ PUSH (fanout)       │
│ Celebs (> 10K follow)  │ PULL (fetch on read)│
└────────────────────────┴────────────────────┘
```

---

## 8 Read Flow — App Open

```
   Arpan opens app
        │
        ▼
   ┌──────────────────────────┐
   │  Step 1: PUSH inbox read │
   │     (Redis)              │
   │     normal users tweets  │
   └────────────┬─────────────┘
                │
                ▼
   ┌──────────────────────────┐
   │  Step 2: PULL celebs    │
   │     (Cassandra)          │
   │     Virat, Bieber tweets │
   └────────────┬─────────────┘
                │
                ▼
   ┌──────────────────────────┐
   │  Step 3: MERGE + SORT    │
   │     by timestamp         │
   └────────────┬─────────────┘
                │
                ▼
   ┌──────────────────────────┐
   │  Step 4: HYDRATE         │
   │     tweet_ids → content  │
   └────────────┬─────────────┘
                │
                ▼
   ┌──────────────────────────┐
   │  Return top 50           │
   └──────────────────────────┘
```

---

## 9 Read Flow — Concrete Example

```
Arpan follows: Virat (celeb) + Suresh (normal)
   App opens
        │
        ├── Suresh ki tweet:
        │     Redis inbox se NIKLI
        │     (PUSH se pehle se aayi thi)
        │
        ├── Virat ki tweet:
        │     Cassandra DB se DIRECT FETCH
        │     (PULL — celeb hai, fanout nahi)
        │
        └── Merge → feed
```

---

## Write Flow — Tweet Post

```
   Virat tweets
        │
        ▼
   Tweet Service receives
        │
        ├──► Save → CASSANDRA (palace board)
        │    tweet_id, user_id, content, time
        │
        └──► Push event → KAFKA
                          │
                          ▼
              Fanout Service (worker)
                          │
                          ├── Get followers list
                          ├── Filter celebs OUT (no fanout for them)
                          └── For each normal follower:
                                  LPUSH redis:inbox:userX tweet_id
```

---

## 11 Full Architecture

```
                    USER opens app
                         │
                         ▼
                  ┌──────────────┐
                  │  Route 53    │   DNS
                  └──────┬───────┘
                         │
                         ▼
                  ┌──────────────┐
                  │  CloudFront  │   CDN (media)
                  └──────┬───────┘
                         │
                         ▼
                  ┌──────────────┐
                  │     ALB      │   Load Balancer
                  └──────┬───────┘
                         │
            ┌────────────┼────────────┐
            ▼            ▼            ▼
     ┌─────────┐  ┌─────────┐  ┌─────────┐
     │ Tweet   │  │Timeline │  │  User   │
     │ Service │  │ Service │  │ Service │
     │ (write) │  │ (read)  │  │ (graph) │
     └────┬────┘  └────┬────┘  └────┬────┘
          │            │            │
          ▼            │            ▼
     ┌─────────┐       │       ┌─────────┐
     │  KAFKA  │       │       │ Graph DB│
     │ (events)│       │       │(follows)│
     └────┬────┘       │       └─────────┘
          │            │
          ▼            │
     ┌─────────────┐   │
     │  Fanout     │   │
     │  Service    │   │
     │  (workers)  │   │
     └──────┬──────┘   │
            │          │
            └──────────┼──────────────┐
                       │              │
                       ▼              ▼
                 ┌──────────┐  ┌─────────────┐
                 │  REDIS   │  │  CASSANDRA  │
                 │ (inbox - │  │ (tweets DB) │
                 │  notice  │  │  full       │
                 │  boards) │  │  content)   │
                 └──────────┘  └─────────────┘
```

---

## 12 Components Mapping

```
┌─────────────────────┬──────────────────────────────┐
│  Analogy            │  Real Component              │
├─────────────────────┼──────────────────────────────┤
│ Notice board        │ Redis inbox                  │
│ Town Crier          │ Fanout Service               │
│ Palace noticeboard  │ Cassandra (tweet store)      │
│ Royal scribe        │ Tweet Service                │
│ News reader         │ Timeline Service             │
│ Kingdom register    │ Graph DB (follows)           │
│ Newspaper truck     │ Kafka (async events)         │
└─────────────────────┴──────────────────────────────┘
```

---

## 13 DB Choices

```
TWEET STORE:    Cassandra
                • Wide-column, write-heavy
                • LSM tree (write fast)
                • Easy sharding by user_id

USER GRAPH:     Graph DB (Neo4j) OR Cassandra
                followers, following tables

FEED INBOX:     Redis cluster
                In-memory list per user
                LPUSH (write) / LRANGE (read)
```

---

## 14 Read Flow Line (Memorize)

```
"User opens app → Timeline Service →
 Step 1: LRANGE Redis inbox (PUSH side, normal users) →
 Step 2: Get celeb following → Cassandra fetch (PULL side) →
 Step 3: Merge + sort by time →
 Step 4: Hydrate tweet_id → full content (Cassandra) →
 Return top 50."
```

---

---

## 15 Hot Tweet Caching (Library Bestseller)

### Problem
```
Virat tweets → 10 CRORE followers read
If everyone hits Cassandra = 10 CRORE DB hits
= DB crash
```

### Library Analogy
```
Normal book:
   Shelf ke peeche rakhi
   Maange to staff jaake la de

BESTSELLER (high demand):
   Front counter pe rakhi (cache)
   Direct grab — no shelf trip
```

### Solution — Hot Tweet Cache
```
   ┌─────────────────────────┐
   │  HOT TWEET CACHE         │  ← Redis
   │  (Recent celeb tweets)   │
   │                           │
   │  Virat:  "Match great"   │
   │  Bieber: "New song"      │
   │  Modi:   "Speech today"  │
   └────────────┬────────────┘
                │ if miss
                ▼
   ┌─────────────────────────┐
   │  CASSANDRA (DB)          │
   └─────────────────────────┘
```

```
Read flow:
   Want Virat's tweet
        │
        ▼
   Check HOT CACHE first
        │
        ├── HIT (95% time) → return (FAST)
        │
        └── MISS → Cassandra → save in Redis → return
```

```
TTL pattern:
   Recent tweet (< 1 hour) = HOT → cache
   Old tweet                = COLD → direct DB

   SETEX virat_tweet_123 3600 "content"
                          ▲ 1 hour TTL
```

---

## 16 2 Caches — Bounded Separately

```
   ┌─────────────────────────────────┐
   │  CACHE 1: Inbox Cache            │
   │  (per user, PUSH side)           │
   │                                   │
   │  redis:inbox:arpan_123 →         │
   │     [tweet_id_5, id_4, id_3,...]│
   │                                   │
   │  Storage: tweet IDs only!        │
   │  Limit: LTRIM to 800 per user    │
   │  ~6.4 KB per inbox                │
   └─────────────────────────────────┘

   ┌─────────────────────────────────┐
   │  CACHE 2: Hot Tweet Cache        │
   │  (shared, PULL side)             │
   │                                   │
   │  cache:tweet:virat_123 →         │
   │     "Match was great!"           │
   │                                   │
   │  Storage: tweet content           │
   │  Limit: TTL 1 hour                │
   │  Only recent + popular            │
   └─────────────────────────────────┘
```

### Storage Math
```
Inbox cache (500M users):
   500M × 6.4 KB = 3.2 TB
   = Redis cluster, multiple nodes
   = Manageable

Hot tweet cache:
   ~500K recent celeb tweets × 500 bytes
   = 250 MB
   = Single Redis node easily

Cassandra (source of truth):
   All tweets ever
   Petabytes
   Sharded
```

---

## 17 Inactive Users — TTL Cleanup

```
Active users:
   App open frequently
   Inbox cache kept hot

Inactive users (no app open 30 days):
   TTL → inbox deleted from Redis
   Save memory

Reactivate:
   User opens app
   Inbox rebuilt from Cassandra
   = One-time cost, not ongoing
```

---

## 18 PUSH → PULL Crossover

```
Question: Normal vs celeb — line kahan?

Threshold (simplified):
   < 10,000 followers      → PUSH (normal)
   > 10,000 followers      → PULL (celeb)
```

### Edge Case
```
Tu ne 8,000 followers banaye:
   PUSH user — tweets go to inboxes

Tu ne 2,500 aur kamaye = 10,500 total:
   Threshold crossed
   Switch to PULL mode for future tweets

   System auto-evaluate karta
```

---

## 19 Sharding Strategies

### Approach 1: By tweet_id (random)
```
Even distribution
User's tweets scattered
Profile = scatter-gather (slow)
```

### Approach 2: By user_id (recommended)
```
User's tweets together = one shard
Profile load fast
Hot user problem (Bieber shard hammered)
```

### Approach 3: By user_id + time
```
Distribute hot users across time
Recent tweets together
Old tweets archived to cold storage
```

---

## 20 GEO Sharding (Geographic)

### Pattern
```
   ┌──────────────────────────────────┐
   │  INDIA REGION                     │
   │  Cassandra (India shard)          │
   │  • Virat, Dhoni, Rohit            │
   │  • Indian users tweets            │
   └──────────────────────────────────┘

   ┌──────────────────────────────────┐
   │  EU REGION                        │
   │  Cassandra (EU shard)             │
   │  • Messi, Ronaldo                 │
   │  • EU users tweets                │
   └──────────────────────────────────┘

   ┌──────────────────────────────────┐
   │  US REGION                        │
   │  Cassandra (US shard)             │
   │  • Bieber, Taylor Swift           │
   │  • US users tweets                │
   └──────────────────────────────────┘
```

### Benefits
```
LATENCY:
   India user → India shard (5ms)
   NOT US shard (200ms)

COMPLIANCE:
   GDPR — EU data stays in EU
   Geo shard fits naturally

LOAD distribution:
   India peak hours ≠ US peak hours
   Each region handles own load

FAILURE isolation:
   India outage = EU/US unaffected
```

### Cross-region Reads
```
Indian user follows Bieber (US celeb):
   Option 1: Cross-region fetch (slower)
   Option 2: Bieber's hot tweets replicated to India Redis
   = Production uses option 2 (replicate hot data)
```

---

## 21 Hot User Replication

```
Problem:
   Bieber's shard hammered
   Other shards idle
   = uneven load

Solution:
   Bieber's tweets → replicated to MULTIPLE shards
   Reads distributed across replicas
   = Load balanced
```

---

## 22 Real Twitter — Hybrid

```
Production uses ALL strategies:
   • User_id sharding (primary)
   • Time-based sub-sharding
   • Geo replication (regional)
   • Hot data globally cached

= Multi-dimensional sharding
= No single strategy enough
```

---

## FULL TOPIC SUMMARY

```
Day 1: Concept
   PUSH vs PULL
   Bieber problem
   Hybrid model

Day 2: Architecture
   Notice Board (Redis inbox)
   Town Crier (Fanout)
   Palace Board (Cassandra)
   Read flow + Write flow
   Full architecture diagram

Day 3: Optimization
   Hot tweet caching (Library bestseller)
   Cache storage strategy (2 caches bounded)
   Inactive user TTL cleanup
   PUSH→PULL crossover (10K threshold)
   Sharding (user_id + time + geo)
   Hot user replication
```

---

# 7-STEP RAIL DRIVE

> (RAIL: 04_HLD/HLD_APPROACH_DELIVERY.md) — Requirements → Estimate → API → Data model → HL boxes → Deep-dive → Bottleneck. READ-HEAVY. Interview me isi flow me bolo.

## STEP 1 — REQUIREMENTS
```
   FUNCTIONAL:  user app khole -> HOME TIMELINE (jinko follow karta, LATEST-first). tweet POST karo.
   NON-FUNCTIONAL:  feed FAST khule (low latency) . READ-HEAVY . scale (500M user) . reliable.
   CLARIFY:  kitne users/tweets? celeb (crore followers) handle karna? real-time ya thodi purani chalegi? media?
```

## STEP 2 — ESTIMATE (scale / numbers)
```
   500M users, 500M tweets/day = ~5,800/sec. avg follow 200, 200 followers.
   celeb 100M+ followers (BIEBER problem).
   Read:Write ~50:1 -> READ-HEAVY -> inbox PRECOMPUTE + cache (har read pe compute nahi).
```

## STEP 3 — API design
```
   POST /tweet   {content}          -> tweetId        (BANANA -> POST)
   GET  /feed?limit=50              -> home timeline  (LAANA  -> GET)
   GET  /user/{id}/tweets           -> profile timeline
```

## STEP 4 — DATA MODEL + DB (KYUN)
```
   TWEET store:  Cassandra (wide-column, write-heavy, LSM-tree fast write, shard by user_id)
   FEED inbox:   Redis list per user -> tweet_IDs only (LPUSH write / LRANGE read, LTRIM 800)
   USER graph:   Graph DB (Neo4j) ya Cassandra (followers/following)
   KYUN: tweets massive+simple+read-heavy -> NoSQL Cassandra . inbox turant chahiye -> Redis (in-memory).
```

## STEP 5 — HL BOXES (arch)
```
   User -> Route53 -> CDN(media) -> ALB -> [Tweet Svc (write) | Timeline Svc (read) | User/Graph Svc]

   WRITE: Tweet Svc -> Cassandra (save) + Kafka event -> Fanout Svc (workers)
                        -> normal followers ke Redis inbox me LPUSH tweet_id
   READ:  Timeline Svc -> Redis inbox (push side) + Cassandra (celeb pull side)
   Graph DB -> follows/followers
```

## STEP 6 — DEEP DIVE: feed FAST kaise? PUSH vs PULL (asli khel — options -> choose)
```
   OPTION 1 — PUSH (fanout-on-write): tweet -> sab followers ke Redis inbox me LPUSH.
        app khole -> apna inbox padho = INSTANT. PAR celeb (10 crore) -> 10 crore inbox likho -> SYSTEM CHOKED (Bieber).
   OPTION 2 — PULL (fanout-on-read): celeb tweet sirf DB me; follower app-khole pe FETCH. har read pe query (celeb ke liye theek).
   WINNER = HYBRID:
        normal (< 10K follow) -> PUSH (inbox)
        celeb  (> 10K follow) -> PULL (on read)
        READ = inbox(push) + celeb-tweets(pull) -> MERGE + SORT (time) + HYDRATE (id->content) -> top 50.
   -> trade-off bol ke choose = asli marks yahin.
```

## STEP 7 — BOTTLENECK / SCALE
```
   HOT-TWEET CACHE:  celeb tweet -> 10 crore read -> Cassandra crash.
        Redis hot-cache (recent celeb, TTL 1hr) -> 95% HIT | miss -> Cassandra -> cache -> return.
   2 CACHES (bounded alag):  inbox (per-user, IDs only, LTRIM 800 ~6.4KB, 500M = 3.2TB)
                             + hot-tweet (shared, content, TTL ~250MB).
   INACTIVE users TTL:  30-din no-open -> inbox delete (memory bacha) -> reactivate -> Cassandra se rebuild (one-time).
   PUSH<->PULL crossover:  10K threshold, follower cross -> auto-switch mode.
   SHARDING:  by user_id (profile fast, hot-user problem) + time sub-shard + GEO (India/EU/US: latency+GDPR+failure-isolation).
              hot-user REPLICATION (Bieber -> multiple shards -> load balanced).

   WRAP: WRITE Tweet-Svc->Cassandra+Kafka->Fanout->Redis inbox (normal followers only).
         READ Timeline-Svc->Redis inbox(push)+Cassandra(celeb pull)->merge+sort+hydrate->top 50.
         HYBRID push/pull (Bieber) . hot-tweet cache (95% hit) . shard user_id+time+geo . hot-user replicate.
         IMPROVE: ML ranking, media CDN, trending/search.
```

---

[← HLD README](../README.md)
