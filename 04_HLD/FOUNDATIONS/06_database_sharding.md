# Database Sharding

> **HLD Topic 6 — Write scaling + Storage scaling**

---

## Replication vs Sharding (clarify FIRST)

```
REPLICATION:                  SHARDING:
DB1: [ALL DATA]               DB1: [users 1-1M]
DB2: [ALL DATA]               DB2: [users 1M-2M]
DB3: [ALL DATA]               DB3: [users 2M-3M]
    ↑                              ↑
copies of SAME              PIECES of WHOLE

Goal: read scale + HA       Goal: write scale + storage scale
```

**Production reality:** Dono saath — har shard ka apna replica set. Sharding (write) + Replication (read) = full coverage.

---

## STORY — Phone Directory of India

> India 1.4 billion entries ek hi phone book mein? 50 ton ki book, ghanton ki lookup.
>
> **Without sharding:** Single book → unreadable, unmanageable
>
> **With sharding:** 28 state directories alag-alag
> - Maharashtra → Mumbai office
> - Delhi → Delhi office
> - Pehle state decide → us state ki directory query
>
> **State = shard key. Office = shard.**

---

## Why Sharding?

### Without
```
1 TB       → ok
100 TB     → single disk capacity limit
1M w/s     → single disk IOPS limit
Backup     → days
Queries    → table scan slow
```

### With (10 shards)
```
100 TB     → 10 × 10 TB 
1M w/s     → distributed (100K each) 
Backup     → parallel 
Queries    → smaller tables, fast 
```

---

## Architecture

```
                APP SERVERS
                     │
                     ▼
              ┌──────────────┐
              │ SHARD ROUTER │ ← decides shard
              └──────┬───────┘
                     │
        ┌────────────┼────────────┐
        ▼            ▼            ▼
   ┌─────────┐  ┌─────────┐  ┌─────────┐
   │ Shard 1 │  │ Shard 2 │  │ Shard 3 │
   │ users   │  │ users   │  │ users   │
   │ 1-1M    │  │ 1M-2M   │  │ 2M-3M   │
   └─────────┘  └─────────┘  └─────────┘
   (each has its own replicas — for read scale)
```

---

## 4 Sharding Strategies

### 1. **Range-based**
```
user_id 1 - 1M     → Shard 1
user_id 1M - 2M    → Shard 2
user_id 2M - 3M    → Shard 3
```
**Pros:** Range queries fast (`WHERE id BETWEEN x AND y`)
**Cons:** **Hot spot** — naye users last shard → overload
**Use:** Time-series, ordered data

### 2. **Hash-based** (most common)
```
shard_id = hash(user_id) % N

user 5    → hash → 73 % 4 = 1  → Shard 1
user 100  → hash → 91 % 4 = 3  → Shard 3
```
**Pros:** Even distribution, no hot spots
**Cons:** Range queries painful, **resharding nightmare**
**Use:** Most apps (default)

### 3. **Geo-based**
```
US users     → US shard
EU users     → EU shard
India users  → India shard
```
**Pros:** Low latency (data near user), compliance
**Cons:** Uneven load, GDPR complexity
**Use:** Global apps (Netflix, Uber)

### 4. **Directory-based**
```
Lookup service maintains:
   user_id → shard_id

Every query:
   1. Ask lookup service
   2. Route to shard
```
**Pros:** Flexible, easy rebalancing
**Cons:** Lookup overhead, lookup = SPOF
**Use:** When sharding logic must change often

---

## Strategy Comparison

| | Range | Hash | Geo | Directory |
|---|---|---|---|---|
| Distribution | Uneven | Even | Uneven | Custom |
| Range query | Fast | Slow | Slow | Slow |
| Hot spots | High | Low | Medium | Low |
| Resharding | Easy | Hard | Medium | Easy |
| Complexity | Low | Low | Medium | High |
| Common use | Time-series | Most apps | Global | Flexible |

---

## 4 Hard Problems

### 1. **Shard Key choice** — sabse important
**Bad keys:**
- Country (some countries 100x bigger)
- Date (only latest shard hot)
- Status (skewed distribution)

**Good keys:**
- user_id (uniform, high cardinality)
- uuid (random, even)
- Composite key (e.g., user_id + region)

> **Rule:** High cardinality + even distribution + matches query pattern

---

### 2. **Resharding** — real nightmare

```
Initial: 4 shards
   shard = hash(x) % 4

Add 5th shard:
   shard = hash(x) % 5

   hash(x) % 4  ≠  hash(x) % 5
   ALMOST ALL DATA migrates 
```

**Solution: Consistent Hashing**
```
Hash ring (0 to 2^32):
   Servers placed at hash positions
   Keys placed at hash positions
   Each key → next clockwise server
   
   Add server → only K/N keys move (not all)
```

**Use:** Cassandra, DynamoDB, Memcached cluster all use this.

---

### 3. **Cross-shard queries**

```
"Get all users active last 1 hour"
   → Query EVERY shard
   → Merge results
   → Slow + complex

Cross-shard JOIN
   → Very expensive
   → Often denormalize to avoid

Cross-shard TRANSACTION
   → 2-Phase Commit (2PC)
   → Slow + brittle
   → Often avoid via eventual consistency
```

**Production pattern:** Design schema to keep related data on same shard (co-location).

---

### 4. **Hot Spots**

```
Celebrity tweet → 1M writes/sec to one user_id
   → That shard tank 
   
Fix:
   1. Sub-sharding (split celebrity data further)
   2. Hash-based key (avoid sequential)
   3. Caching layer (read hot spot fix)
   4. Write buffering (queue + batch)
```

---

## Real-World Tools

### **MongoDB**
- Built-in auto-sharding
- Auto-balancing across shards
- Hash + range strategies

### **Cassandra**
- Distributed by design (consistent hashing)
- No master — peer-to-peer
- Tunable consistency

### **Vitess** (YouTube, Slack)
- MySQL sharding layer
- Transparent — app sees one DB
- Used at massive scale

### **Citus** (PostgreSQL extension)
- PG with horizontal scaling
- Co-located joins for performance

### **DynamoDB** (AWS)
- Partition key = shard key
- Auto-managed (you don't see shards)
- Hot partition = throttling

---

## Interview Talking Points

**Q: "Replication vs Sharding?"**

> *"Replication = same data multiple copies, read scaling + HA. Sharding = different data different DBs, write+storage scaling. Production both — har shard ka apna replica set. Replication horizontal copy, sharding horizontal split."*

**Q: "Shard key kaise pick?"**

> *"High cardinality + even distribution + matches query pattern. user_id common — uniform. Country bad — skewed. Galat key = hot spot = back to single DB problem. Composite key bhi consider — user_id + region."*

**Q: "Resharding kaise handle?"**

> *"Naive hash-based nightmare — `hash % 4` se `hash % 5` mein almost full data migrate. Solution: **Consistent Hashing** — hash ring pe servers + keys, only K/N data move when shard added. Cassandra, DynamoDB yahi use karte."*

**Q: "Cross-shard query problem?"**

> *"Query har shard pe → merge — slow. JOINs cross-shard expensive. Transactions cross-shard 2PC complex. Fix: schema design — related data same shard pe (co-location). Eventual consistency accept where ok."*

**Q: "Hot spots kaise fix?"**

> *"Celebrity user / trending key one shard overload. Fixes: sub-sharding (further split), hash key (avoid sequential), caching layer, write buffering. Twitter ka actual problem yahi hai — celebrity tweets."*

---

## Power Phrase

> **"Sharding = different data on different DBs. Write+storage scaling. 4 strategies — Range/Hash/Geo/Directory. Hash most common. Hard problems: shard key choice, resharding (Consistent Hashing fix), cross-shard queries, hot spots. Production: sharding + replication together — each shard has its own replicas."**

---

## Memory Hook

```
Sharding = "Phone directory split by state"
   • Different data → different DBs
   • Replication = copies, Sharding = pieces

Strategies:
   Range       → time-series, hot spot risk
   Hash        → most common, resharding hard
   Geo         → global apps, GDPR
   Directory   → flexible, lookup SPOF

Hard problems:
   1. Shard key — high cardinality + even
   2. Resharding — Consistent Hashing fix
   3. Cross-shard — co-locate related data
   4. Hot spots — sub-shard + cache + buffer

Tools:
   MongoDB, Cassandra, Vitess (MySQL),
   Citus (PG), DynamoDB

Production:
   Sharding (write scale) + Replication (read scale)
```

---

## Trap Box

```
Trap 1: "Sharding = Replication"
         Replication = same data, Sharding = different data
         Different goals — both used together

Trap 2: "Shard by country"
         Some countries 100x bigger → uneven
         user_id or uuid (uniform distribution)

Trap 3: "Naive hash sharding"
         Add shard → almost full migrate
         Consistent Hashing

Trap 4: "Cross-shard JOIN no problem"
         Very expensive, slow
         Co-locate related data, denormalize

Trap 5: "Sharding solves everything"
         Adds complexity — distributed txn, queries hard
         Vertical scale first (bigger DB), shard when forced

Trap 6: "Range = good for analytics"
         Hot spot — naye records last shard pe
         Hash for writes, separate analytics DB

Trap 7: "Resharding rare so ignore"
         Growth forces resharding eventually
         Pick Consistent Hashing from day 1
```
