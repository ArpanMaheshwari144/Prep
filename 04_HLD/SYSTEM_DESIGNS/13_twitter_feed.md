# Twitter Feed — Visual System Design

> **Status:** FULL TOPIC DONE — Day 1 + Day 2 + Day 3 covered.

---

## 1️⃣ Problem (1 line)

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

## 2️⃣ Scale Numbers

```
   500 million users
   500 million tweets/day = ~5800/sec
   Avg user follows 200, has 200 followers
   Some celebs = 100M+ followers (Bieber problem)

   Read vs Write = ~50:1
   = READ-HEAVY
```

---

## 3️⃣ Master Analogy — Royal Kingdom

```
Kingdom mein news circulate karna problem.
Same problem Twitter ke saath.

   Notice Board     = Redis inbox per user
   Town Crier       = Fanout Service
   Palace Board     = Tweet store (Cassandra)
   Royal Scribe     = Tweet Service
```

---

## 4️⃣ Notice Board (Redis Inbox)

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

## 5️⃣ Town Crier (Fanout)

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

## 6️⃣ Bieber Problem (Hot User)

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

## 7️⃣ Hybrid Model (Industry Reality)

```
┌────────────────────────┬────────────────────┐
│  User type             │  Strategy           │
├────────────────────────┼────────────────────┤
│ Normal (< 10K follow)  │ PUSH (fanout)       │
│ Celebs (> 10K follow)  │ PULL (fetch on read)│
└────────────────────────┴────────────────────┘
```

---

## 8️⃣ Read Flow — App Open

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

## 9️⃣ Read Flow — Concrete Example

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

## 1️⃣1️⃣ Full Architecture

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

## 1️⃣2️⃣ Components Mapping

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

## 1️⃣3️⃣ DB Choices

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

## 1️⃣4️⃣ Read Flow Line (Memorize)

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

## 1️⃣5️⃣ Hot Tweet Caching (Library Bestseller)

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

## 1️⃣6️⃣ 2 Caches — Bounded Separately

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

## 1️⃣7️⃣ Inactive Users — TTL Cleanup

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

## 1️⃣8️⃣ PUSH → PULL Crossover

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

## 1️⃣9️⃣ Sharding Strategies

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

## 2️⃣0️⃣ GEO Sharding (Geographic)

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

## 2️⃣1️⃣ Hot User Replication

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

## 2️⃣2️⃣ Real Twitter — Hybrid

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

[← HLD README](../README.md)
