# Twitter Feed — 7-STEP RAIL (single spine, revise top→bottom)

> RAIL: Requirements → Estimate → API → Data-model → HL-boxes → Deep-dive → Bottleneck. READ-HEAVY.
> (framework: 04_HLD/HLD_APPROACH_DELIVERY.md). Merged into clean 7-step 7-Sep. FULL TOPIC (Day1+2+3).
> Problem (1 line): app khole -> HOME TIMELINE (jinko follow karta, LATEST-first). tweet POST bhi.

```
   Tu app KHOLA -> home timeline: Virat "Match great" · Sachin "Watching IPL" · Dhoni "Practice"
   = tweets from people TU follow karta, latest-first.
```

```
★ MASTER ANALOGY — ROYAL KINGDOM (visual anchor):
   Notice Board  = Redis inbox per user   |  Town Crier = Fanout Service
   Palace Board  = Tweet store (Cassandra)|  Royal Scribe = Tweet Service  |  News reader = Timeline Svc
```

---

## STEP 1 — REQUIREMENTS

```
FUNCTIONAL:  user app khole -> HOME TIMELINE (follow-kiye logon ke tweets, LATEST-first). tweet POST karo.
NON-FUNCTIONAL:  feed FAST khule (low latency) . READ-HEAVY . scale (500M user) . reliable.
CLARIFY:  kitne users/tweets? celeb (crore followers) handle karna? real-time ya thodi purani? media?
```

---

## STEP 2 — ESTIMATE (scale / numbers)

```
   500M users . 500M tweets/day = ~5,800/sec . avg follow 200, 200 followers.
   celeb 100M+ followers (★ BIEBER problem).
   READ : WRITE ~ 50 : 1  -> READ-HEAVY
   -> KEY: inbox PRECOMPUTE + cache (har read pe compute mat karo — read-heavy me read sasta banao).
```

---

## STEP 3 — API DESIGN

```
   POST /tweet   {content}          -> tweetId          (BANANA -> POST)
   GET  /feed?limit=50              -> home timeline    (LAANA  -> GET)
   GET  /user/{id}/tweets           -> profile timeline
```

---

## STEP 4 — DATA MODEL + DB (KYUN)

```
   TWEET store:  Cassandra  (wide-column, write-heavy, LSM-tree fast write, easy shard by user_id)
   FEED inbox:   Redis list per user -> tweet_IDs only (LPUSH write / LRANGE read, LTRIM 800)
   USER graph:   Graph DB (Neo4j) ya Cassandra (followers / following)
   KYUN: tweets massive + simple + read-heavy -> NoSQL Cassandra . inbox turant chahiye -> Redis (in-memory).

COMPONENTS MAP:  notice-board=Redis inbox . town-crier=Fanout Svc . palace-board=Cassandra . royal-scribe=Tweet Svc .
                 news-reader=Timeline Svc . kingdom-register=Graph DB . newspaper-truck=Kafka.
```

---

## STEP 5 — HL BOXES (arch + write/read flow)

```
                    USER opens app
                         ▼
                  ┌──────────────┐
                  │  Route 53    │  DNS
                  └──────┬───────┘
                         ▼
                  ┌──────────────┐
                  │  CloudFront  │  CDN (media)
                  └──────┬───────┘
                         ▼
                  ┌──────────────┐
                  │     ALB      │  Load Balancer
                  └──────┬───────┘
            ┌────────────┼────────────┐
            ▼            ▼            ▼
     ┌─────────┐  ┌─────────┐  ┌─────────┐
     │ Tweet   │  │Timeline │  │  User   │
     │ Service │  │ Service │  │ Service │
     │ (write) │  │ (read)  │  │ (graph) │
     └────┬────┘  └────┬────┘  └────┬────┘
          ▼            │            ▼
     ┌─────────┐       │       ┌─────────┐
     │  KAFKA  │       │       │ Graph DB│ (follows)
     └────┬────┘       │       └─────────┘
          ▼            │
     ┌─────────────┐   │
     │  Fanout Svc │   │
     │  (workers)  │   │
     └──────┬──────┘   │
            └──────────┼──────────────┐
                       ▼              ▼
                 ┌──────────┐  ┌─────────────┐
                 │  REDIS   │  │  CASSANDRA  │
                 │ (inbox/  │  │ (tweets DB, │
                 │  notice) │  │  full text) │
                 └──────────┘  └─────────────┘
```

```
WRITE FLOW (tweet post):
   Virat tweets -> Tweet Service
      ├──► SAVE -> Cassandra (tweet_id, user_id, content, time)
      └──► event -> KAFKA -> Fanout Service (worker):
                get followers -> celebs FILTER OUT (unke liye fanout nahi) ->
                har NORMAL follower: LPUSH redis:inbox:userX  tweet_id

READ FLOW (app open):
   1. PUSH inbox read (Redis LRANGE)  -> normal users ke tweets
   2. PULL celebs (Cassandra fetch)   -> Virat/Bieber ke tweets (fanout nahi hua tha)
   3. MERGE + SORT by timestamp
   4. HYDRATE tweet_ids -> full content -> return top 50
   READ-FLOW LINE: "Timeline Svc -> LRANGE Redis inbox(push) + celeb-following Cassandra(pull) -> merge+sort -> hydrate -> top 50".
```

---

## STEP 6 — DEEP DIVE: feed FAST kaise? PUSH vs PULL (asli khel)

```
OPTION 1 — PUSH (fanout-on-write): tweet -> sab followers ke Redis inbox me LPUSH.
   app khole -> apna inbox padho = INSTANT.
   PAR celeb (Bieber 10 crore) -> 10 crore inbox likho -> TOWN CRIER choked -> SYSTEM CHOKED.
   FIX: celeb ke tweets fanout NAHI -> sirf palace-board (Cassandra) me -> followers on-demand fetch.

OPTION 2 — PULL (fanout-on-read): celeb tweet sirf DB me; follower app-khole pe FETCH. har read pe query (celeb ke liye theek).

★ WINNER = HYBRID (industry reality):
   ┌────────────────────────┬────────────────────┐
   │ Normal (< 10K followers)  │ PUSH (fanout)       │
   │ Celeb  (> 10K followers)  │ PULL (fetch on read)│
   └────────────────────────┴────────────────────┘
   READ = inbox(push, normal) + celeb-tweets(pull) -> MERGE + SORT (time) + HYDRATE -> top 50.
   concrete: Arpan follows Virat(celeb)+Suresh(normal) -> Suresh Redis-inbox se (push), Virat Cassandra se (pull) -> merge.
   -> trade-off BOL ke choose = asli marks yahin.

★ PUSH<->PULL CROSSOVER: threshold ~10K followers. Tu 8K (PUSH) -> 2.5K aur kamaye = 10.5K -> threshold cross ->
   future tweets PULL mode. System auto-evaluate karta.
```

---

## STEP 7 — BOTTLENECK / SCALE / OPTIMIZATION

```
HOT-TWEET CACHE (library-bestseller): Virat tweet -> 10 crore read -> sab Cassandra hit -> DB crash.
   Bestseller front-counter pe (cache), normal book shelf-peeche (DB).
   -> Redis HOT-TWEET cache (recent celeb, TTL 1hr): 95% HIT | miss -> Cassandra -> save-in-Redis -> return.
   TTL pattern: recent (<1hr)=HOT->cache (SETEX ... 3600) | old=COLD->direct DB.

2 CACHES (bounded ALAG):
   CACHE-1 INBOX (per-user, PUSH side): redis:inbox:arpan -> [tweet_ids only], LTRIM 800, ~6.4KB/inbox.
       500M users × 6.4KB = ~3.2 TB -> Redis cluster (manageable).
   CACHE-2 HOT-TWEET (shared, PULL side): cache:tweet:virat -> content, TTL 1hr. ~500K recent × 500B = ~250MB (single node).
   Cassandra = source of truth (all tweets, petabytes, sharded).

INACTIVE users TTL: 30-din no-open -> inbox Redis se DELETE (memory bacha) -> reactivate -> Cassandra se REBUILD (one-time cost).

SHARDING strategies:
   1. by tweet_id (random): even, PAR user-tweets scatter -> profile = scatter-gather (slow).
   2. by user_id (★ recommended): user-tweets ek shard -> profile fast. PAR hot-user (Bieber shard hammered).
   3. by user_id + time: hot-user time pe distribute, recent together, old -> cold-storage.

GEO SHARDING: India/EU/US regions.
   LATENCY (India user -> India shard 5ms, na US 200ms) . COMPLIANCE (GDPR EU-data EU me) .
   LOAD (India-peak != US-peak) . FAILURE-isolation (India outage -> EU/US safe).
   cross-region: Indian follows Bieber(US) -> hot tweets India-Redis me REPLICATE (prod = replicate hot-data).

HOT-USER REPLICATION: Bieber shard hammered, baaki idle -> Bieber tweets MULTIPLE shards pe replicate -> reads distribute -> load balanced.

REAL TWITTER = MULTI-DIMENSIONAL: user_id shard (primary) + time sub-shard + geo replication + hot-data global cache. No single strategy enough.

WRAP: WRITE Tweet-Svc->Cassandra+Kafka->Fanout->Redis inbox (normal followers only).
      READ Timeline-Svc->Redis inbox(push)+Cassandra(celeb pull)->merge+sort+hydrate->top 50.
      HYBRID push/pull (Bieber) . hot-tweet cache (95% hit) . shard user_id+time+geo . hot-user replicate.
      IMPROVE: ML ranking, media CDN, trending/search.
```

---

[← HLD README](../README.md)
