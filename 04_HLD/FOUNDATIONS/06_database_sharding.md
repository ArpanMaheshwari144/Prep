# Database Sharding

> **NAV** — KYA: sharding (shard-key, consistent hashing, hot-shard). UP: [MASTER](../00_MASTER_SHEET.md) · farak: [replication vs sharding](../TRADEOFFS.md) · lagta hai: [twitter-feed](../SYSTEM_DESIGNS/03_twitter_feed/03_twitter_feed.md) · [message-queue](../SYSTEM_DESIGNS/13_message_queue_kafka/13_message_queue_kafka.md)

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

> ★★ **GEO dono me aata hai — aur MATLAB ALAG hai (yahan log phisalte hain):**
> ```
> EK HI cheez ki NAKAL user ke paas    ->  REPLICATION (geo)
>      cricket score India me bhi, US me bhi — WAHI score dono jagah
>      dikkat thi: user DOOR hai
>
> ALAG-ALAG cheez alag jagah            ->  SHARDING (geo)
>      EU walon ka data EU me, US walon ka US me — ek banda EK hi jagah
>      dikkat thi: data BADA hai (ya KANOON keh raha hai)
> ```
> ★ geo-sharding ki wajah aksar technical hoti hi nahi — **KANOONI** hoti hai
> (EU ka data EU se bahar rakhna hi mana hai). Ye bolne layak point hai.

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
100 TB → 10 × 10 TB
1M w/s → distributed (100K each)
Backup → parallel
Queries → smaller tables, fast
```

### ★★ Sharding se PEHLE teen aur raaste hain — aur wo kahan RUKTE hain

> Interviewer ye poochhta hai. "Data bada tha isliye shard kiya" adhoora jawab hai —
> asli jawab = "baaki teen raaste yahan tak chalte hain, uske aage sirf sharding bachti."

```
A. MACHINE BADI KAR DO (vertical)
     chalta hai — ek had tak. Har machine ki CHHAT hoti hai.
     aur 64 -> 512 GB me agla size 2x mehnga, 1.3x tez. Paisa doobta hai.
     aur wo phir bhi EK machine hai.

B. REPLICAS LAGA DO
     ★ ye sabse badi galatfehmi hai — isse dikkat CHHUTI BHI NAHI:
        leader  20 TB
        copy-1  20 TB     <- wahi 20 TB, teen baar
        copy-2  20 TB        data kahin BANTA nahi, sirf NAKAL badhi
        copy-3  20 TB
     aur saare WRITE ab bhi ek hi leader pe ja rahe hain.
     -> REPLICATION padhne ka load baanta hai. DATA nahi baanta.

C. PURANA DATA ARCHIVE KAR DO
     ye sach me kaam karta hai, aur log ise kam aankte hain.
     ★ "data kaunsa hai" se tay hota hai:
          survey / log / analytics   ->  aaram se archive
          paisa / order / audit      ->  DELETE to kabhi nahi (7-10 saal KANOONAN rakhna hai)
                                         par THANDI storage me sarkana phir bhi hota hai
                                         ("hatana" nahi — "garam se thanda")
     ★ RUKTA KAHAN HAI: archive tab tak chalta jab tak PURANA data badh raha ho.
        Jab ZINDA data khud bada ho jaye — 10 lakh user se 5 crore, sabke ACTIVE
        order/balance — tab hatane ko kuch hai hi nahi. Archive SAMAY khareedta hai,
        dikkat khatam nahi karta.
```

> **NICHOD:** A ki chhat aa jaati · B dikkat ko chhuti hi nahi · C samay deti par rukti hai.
> Jis din LIVE data hi ek machine se bada ho jaye — sharding ke alawa kuch bachta hi nahi.

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

**"matches query pattern" = SHARD ON YOUR QUERY KEY (drill se seekha):**
```
   Jis field se tu LOOKUP karta hai, USI pe shard karo.
   URL shortener: redirect mein tere paas SIRF shortCode hota (GET /abc123) — user-id/geo nahi.
     -> user-id pe shard kiya  -> redirect pe pata hi nahi kaunsa shard (user-id available nahi) -> sab dhoondho. SLOW.
     -> shortCode pe shard      -> hash(shortCode) -> seedha ek shard. SAHI.
   Principle: shard us cheez pe jisse query aati hai (warna har query = saare shards scan).
```

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

Soch — number line (0 se max) ko mod ke **gol** bana do (ghadi). Yehi **ring**.

```
STEP 1 — Shards ko ring pe baithao (hash(shard) = position):

                 0 / max
                    │
          S3 ●      │      ● S1
         (80)       │     (10)
                    │
               ●────┴
              S2 (40)

STEP 2 — User ko bhi ring pe (hash(user) = position). Maan le 30 pe.

STEP 3 — RULE: user ki jagah se aage (clockwise, number badhta) chalo,
         jo PEHLA shard mile — user ushi ka. Pehle pe RUK jao.

   user 30 → aage → 40 pe S2 (pehla) → user S2 ka.
```

**Har shard apne se pehle waala arc own karta** (pichle node se khud tak) —
kyunki us range ke saare users aage chal ke ushi pe pohochte:

```
   0  – 10   → S1
   10 – 40   → S2
   40 – 80   → S3
   80 – max  → wapas ghum ke S1   (ring gol hai)
```

**Jaadu — naya shard add (S4 ko 60 pe daala):**

```
          S3 ●            ● S1 (10)
         (80)
            ●
          S4 (60)
            ●────
          S2 (40)

   40 – 60  → ab pehle S4 milta → S3 se S4 pe SHIFT (sirf yeh slice)
   60 – 80  → abhi bhi S3 (koi change nahi)
   0  – 40  → S1/S2, chhua tak nahi
```

```
   % N waala:  4→5 pe SAARI keys move.
   Ring waala: sirf EK arc/slice (~K/N keys) move, poora data nahi.
   => add/remove sasta + smooth. Yehi consistent hashing ka point.
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

## ★★ KIS DESIGN ME, KAUNSI KEY — (list ratne ki nahi, NIYAM lagane ki)

> NIYAM: **"data itna bada ki ek machine me na samaye"** (ya kanoon keh raha ho).
> Niyam lagao, jawab khud nikal aata hai — table sirf usko check karne ko hai.

```
DESIGN              shard KEY                    kyun
------------------------------------------------------------------------
url shortener       short-code ka HASH           arab rows, har row chhoti
                                                 "range" kabhi chahiye hi nahi -> hash fit

twitter feed        tweets  -> userId            "mere tweets"  = ek machine
                    inbox   -> userId            "mera feed"    = ek machine

payment             userId / accountId           + GEO/KANOON yahin aata
                                                 (EU ka data EU me = majboori, choice nahi)

bookmyshow          cityId / showId              ek show ki saari seat EK jagah
                                                 (warna seat-lock cross-machine = aafat)

★ message queue     PARTITION hi SHARD hai      Kafka ka partition-key = shard-key.
  (Kafka)           partition-key = shard-key     "same key -> same partition"
                                                  = "same key -> same machine".
                                                  Aur wahan partition REHASH hone pe ORDER
                                                  toot-ta tha — yahan shard-key badalne pe
                                                  POORA DATA hilta hai. Ek hi baat, do naam.
```

### ★ Jahan sharding lagti HI NAHI (ye bhi bolna aata chahiye)

```
rate limiter     counter hai, kuch KB ka — baantne ko kuch hai hi nahi
cricket score    50 byte ka score. Dikkat thi "BAHUT JAGAH bhejna",
                 "data bada" nahi -> ilaaj FANOUT + delta, sharding NAHI
seat booking     500 log EK hi row pe -> ye WRITE ki ladai hai
                 -> lock / atomic UPDATE. Shard/replica dono bekaar.
```

> **★ SABSE BADA FAISLA — shard-key:** aisi chuno ki ROZ-MARRA ki query EK hi shard
> me nipat jaaye. `userId` pe shard kiya -> "mere saare order" = ek machine (sasta).
> `orderId` pe kiya -> wahi sawaal chaaron machine se (4 guna mehnga).
> **Galat key = har request 4 guna. Aur baad me badalna = poora data dobara sarkana.**

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
