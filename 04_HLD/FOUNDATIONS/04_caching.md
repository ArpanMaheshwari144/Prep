# Caching

> **HLD Topic 4 — Speed weapon (Day 1)**

---

## STORY — Library Front Desk

> Library mein 10,000 books. Tu daily 5-7 popular books padhta — same hi.
>
> **Without caching:**
> Har baar warehouse jao → 5 min walk × 7 books = 35 min waste daily.
>
> **With caching (front desk):**
> Top-100 popular books **front desk pe** rakho. 10 sec each access.
>
> **Cache = front desk** (frequently accessed data, fast storage)
> **DB = warehouse** (slow but complete)

---

## Why Cache?

```
Without cache:
   Every request → DB hit (~50-100 ms)
   1000 RPS → DB overload → crash

With cache:
   90% requests → Cache (RAM ~1 ms)
   10% requests → DB (cache miss)
   DB load: 10x reduced
   Latency: 50-100x faster
```

**Trade memory for speed** — RAM expensive but FAST.

---

## Multi-level Cache Architecture

```
   USER (browser)
        │
        ▼
   1. BROWSER CACHE       ← static files (your laptop)
        │ miss
        ▼
   2. CDN CACHE           ← Cloudflare edge servers
        │ miss
        ▼
   3. LB / API GATEWAY    ← can cache responses
        │ miss
        ▼
   4. APP CACHE           ← in-app (Caffeine/Guava)
        │ miss
        ▼
   5. DISTRIBUTED CACHE   ← Redis, Memcached
        │ miss
        ▼
   6. DATABASE            ← source of truth (slowest)
```

**Each layer reduces load on next.**

### Local (app) vs Distributed (Redis) cache — DEPTH

```
   LOCAL / IN-PROCESS (Caffeine, JVM ke andar):
      ✓ FASTEST (same process, no network hop)
      ✗ Har server ka APNA cache → 5 servers = 5 alag (inconsistent)
      ✗ Server restart → cache gaya
   DISTRIBUTED (Redis, alag service):
      ✓ SHARED (sab servers ek hi Redis) → consistent
      ✓ Server restart → cache safe (Redis alag)
      ✗ Thoda slow (network hop)
   = aksar DONO (L1 local + L2 Redis) = "near cache" pattern
     (local fastest hits, miss pe Redis, phir DB)
```

---

## 4 Cache Strategies

### 1. **Cache-Aside (Lazy Loading)** — most common (90%)

```
Read:
   1. App checks cache
   2. Cache MISS → query DB
   3. Store result in cache
   4. Return to user

Write:
   1. Update DB
   2. Invalidate cache (delete entry)
   3. Next read fetches fresh
```

**Pros:** Simple, no cache update on every write
**Cons:** First read slow, stale risk
**Use:** Read-heavy systems

### 2. **Write-Through**
```
Write → update CACHE + DB synchronously (both)
```
**Pros:** Cache always fresh
**Cons:** Slow writes
**Use:** Critical fresh data

### 3. **Write-Behind (Write-Back)**
```
Write → update cache → return immediately
Cache → DB (async, batched in background)
```
**Pros:** Very fast writes
**Cons:** Data loss risk on cache crash
**Use:** Logs, analytics, high write throughput

### 4. **Refresh-Ahead**
```
Cache PROACTIVELY refresh before expiry
```
**Use:** Predictable hot data (trending items)

### 5. **Write-Around**
```
Write → SEEDHA DB mein (cache BYPASS, cache update NAHI)
Cache sirf READ pe bharta (cache-aside jaisa)
```
**Use:** Jab likha data TURANT nahi padha jaata (warna cache mein
"kabhi na padha jaane wala" data bharta = waste). **Pros:** cache pollution kam.
**Cons:** likhne ke turant baad read = cache miss (DB se).

---

## Strategy Comparison

| | Cache-Aside | Write-Through | Write-Behind | Refresh-Ahead |
|---|---|---|---|---|
| Read | Fast (warm) | Fast | Fast | Fastest |
| Write | Fast | Slow | Very fast | Slow |
| Consistency | Eventual | Strong | Weak | Eventual |
| Complexity | Easy | Medium | Hard | Hard |
| Use case | Read-heavy (90%) | Critical data | High write | Predictable hot |

---

## Eviction Policies

Cache full → kya hatao?

### **LRU (Least Recently Used)** — most popular
```
Cache: [A, B, C, D, E]   E most recent, A oldest
Add F → Evict A (oldest accessed)
```
**Used by:** Redis, browsers, OS page cache

**LRU IMPLEMENT kaise (O(1)) — DSA connect:**
```
   HashMap (key → node, O(1) lookup) + Doubly Linked List (recency order)
   → access: node ko FRONT laao (most recent)
   → evict: list ke END se hatao (least recent)
   = "HashMap + DLL" = classic LRU Cache design (LeetCode 146 — DSA Phase 2)
```

### **LFU (Least Frequently Used)**
```
Track access count → Evict lowest count
```

### **FIFO**
```
Evict oldest insertion (regardless of access)
```
**Suboptimal** — popular old entries lost.

### **TTL (Time To Live)**
```
Each entry has expiry → auto-evicted
```
**Use:** Sessions, OTPs, time-sensitive data

---

## Real-World Cache Tools

### **Redis** (modern choice — 80% market)
- In-memory key-value
- Persistence: snapshot + AOF
- Data structures: String, List, Set, Hash, Sorted Set, Streams
- Single-threaded (atomic ops)
- Pub-Sub support
- **Use:** Distributed cache, sessions, leaderboards

### **Memcached** (legacy)
- Pure key-value, simpler
- Multi-threaded
- No persistence
- **Use:** Pure cache, no advanced needs

### **Caffeine** (Java in-process)
- Library — runs inside JVM
- Spring `@Cacheable` integrates
- **Use:** Single-app local cache

### Redis vs Memcached

| | Redis | Memcached |
|---|---|---|
| Data structures | Many | Key-value only |
| Persistence | | |
| Pub-Sub | | |
| Modern choice | | Legacy |

---

## The Hard Problem — Cache Invalidation

> *"Two hard things in CS: cache invalidation and naming things."*

**Problem:** DB updated, cache stale → users see old data.

### Solutions:

#### 1. **TTL (simplest)**
```
SET cache:user:123 → John (TTL: 5 min)
After 5 min → expires automatically
Next read fetches fresh
```
**Trade-off:** 5 min staleness OK.

#### 2. **Explicit invalidation**
```
On write:
   1. Update DB
   2. DELETE cache:user:123
   3. Next read → fresh fetch
```
**Risk:** Step 2 fails → stale forever.

#### 3. **Pub-Sub invalidation**
```
DB write → publish event → caches subscribe → delete entry
```
**Use:** Multi-region distributed.

**Production combo:** TTL + explicit invalidation.

---

## Advanced — 3 Cache Problems + Bloom Filter (DEPTH)

Senior-level interview depth. (Penetration + Avalanche naye; Breakdown = stampede, upar interview-points mein bhi.)

```
   ┌──────────────┬──────────────────────────┬───────────────────────────┐
   │ Problem      │ Kya                       │ Fix                        │
   ├──────────────┼──────────────────────────┼───────────────────────────┤
   │ PENETRATION  │ non-existent keys (jo hai │ negative caching (null bhi │
   │              │ HI NAHI) → cache miss     │ cache, short TTL) /        │
   │              │ → har baar DB → overload  │ BLOOM FILTER (DB se pehle  │
   │              │                           │ "exist?" check)            │
   │ AVALANCHE    │ BAHUT keys EK SAATH expire│ JITTER / random TTL (sab   │
   │              │ (same TTL) → bheed DB pe  │ ek saath nahi, bikhre)     │
   │              │ → DB crash                │ + cache HA (cluster)       │
   │ BREAKDOWN /  │ EK super-HOT key expire   │ MUTEX (1 thread rebuild,   │
   │ STAMPEDE     │ → massive concurrent      │ baaki wait) / soft-logical │
   │              │ traffic ek saath DB pe    │ TTL (background refresh) / │
   │              │                           │ never-expire hot keys      │
   └──────────────┴──────────────────────────┴───────────────────────────┘
```

### Bloom Filter (penetration fix — kaise kaam karta)

```
   = "yeh key set mein hai ya nahi" — space-efficient, probabilistic
   2 jawab: "DEFINITELY nahi" (pakka) | "SHAYAD hai" (maybe, false-positive ok)

   BANTA: ek BIT ARRAY (0/1) + k HASH functions
   ADD:   key ko k hash → un k positions ke bits 1 karo (key store nahi hoti)
   CHECK: k hash → koi bit 0? → DEFINITELY nahi (reject, DB hit nahi)
                   saare 1? → shayad hai → DB check (confirm)

   INSIGHT: bit 0 = key kabhi add hi nahi hui (no false negative)
            saare 1 = shayad (doosri key ke hash collide hoke 1 kar sakte = false positive)
   = "nahi" 100% sahi; "haan" ~99% (kabhi false positive)
   USE: pre-load valid keys → fake/non-existent keys DB se pehle reject
   Origin: Burton Bloom (1970, academic). Google/Cassandra/Redis use karte.
```

### Penetration vs Avalanche vs Breakdown (confuse mat)
```
   PENETRATION = fake keys (hai HI NAHI)        → negative cache / bloom
   AVALANCHE   = bahut REAL keys ek saath expire → jitter TTL
   BREAKDOWN   = EK hot key expire (massive traffic) → mutex / soft-TTL
```

### Cache-Aside RACE CONDITION (concurrent read + write)

```
   Setup: cache-aside, ek READ (miss) + ek WRITE ek saath. DB = "John" (purana).

   t1: A (READ)  → cache miss → DB padha → "John" mila (cache mein DAALA NAHI, slow)
   t2: B (WRITE) → DB update "John" → "Johnny"
   t3: B (WRITE) → cache delete (khaali tha)
   t4: A (continue) → cache mein "John" DAAL diya (jo t1 pe padha — PURANA)

   RESULT: cache = "John" (purana), DB = "Johnny" (naya) → STALE
   = slow read purana padh ke, write ke BAAD cache mein daal de → stale (threads)
```

**Fixes:**
```
   1. SHORT TTL — stale ho bhi gaya to jaldi expire (safety net)
   2. DELAYED DOUBLE-DELETE — write: DB update → cache delete → thodi der baad
      DOBARA delete (beech ka stale catch ho jaaye)
   3. VERSIONING/CAS — cache mein version; purana version reject (complex)
```

**Lock se rok sakte? Haan, PAR:**
```
   - Java `synchronized` = SINGLE JVM tak. Distributed (multi-server + Redis)
     mein → DISTRIBUTED LOCK chahiye (Redis SETNX/Redlock, Zookeeper)
   - Lock = CONTENTION + LATENCY → caching ka speed maara → SELECTIVELY use
     (hot keys / strong-consistency), har operation pe nahi
   = general race → cheaper (TTL/double-delete); lock → critical/hot-key (mutex)
```

> **Meta:** Koi system PERFECT nahi — har fix naye trade-off laata (cache→invalidation,
> lock→contention, TTL→staleness). Senior = RIGHT trade-off chunna, perfect dhoondna nahi
> (= CAP theorem ka spirit).

---

## Cache Hit Ratio

```
Hit Ratio = (Cache hits) / (Total requests)

Good:   >80%
Great:  >95%

Low ratio fixes:
   • Increase cache size
   • Switch eviction (LRU ↔ LFU)
   • Reconsider what's cacheable
```

Monitor in production — Spring Boot Actuator + Micrometer.

---

## Interview Talking Points

**Q: "Caching kya aur kab use?"**

> *"Frequently accessed data ko fast storage (RAM) mein. DB load + latency reduce. Multi-level — browser/CDN/app/distributed (Redis)/DB. **Cache-Aside** most common — DB miss pe populate. **LRU** eviction. **TTL mandatory** — staleness control."*

**Q: "Cache invalidation strategies?"**

> *"3 main: **TTL** (simple, eventual), **explicit** (DB write + cache delete), **pub-sub** (multi-region). Production mostly TTL + explicit combo. Trade-off: consistency vs simplicity."*

**Q: "Redis vs Memcached?"**

> *"Redis modern — persistence, data structures (List/Set/Hash), Pub-Sub. Memcached legacy — pure key-value. Production 80% Redis."*

**Q: "Cache stampede?"**

> *"Cache expire pe 1000 requests simultaneously → DB crash. Fixes: **mutex** (1 thread refresh, baaki wait), **refresh-ahead** (proactive), **soft TTL** (small jitter prevents simultaneous expiry)."*

---

## Power Phrase

> **"Cache = frequently accessed data fast storage mein. Multi-level (browser/CDN/app/Redis/DB). Cache-Aside + LRU + TTL = production combo. Invalidation hardest — TTL + explicit invalidation combo. Redis 80% market."**

---

## Memory Hook

```
Cache = "Library front desk"
   • Hot data → front (fast)
   • Cold data → warehouse (DB)

Multi-level:
   Browser → CDN → App → Redis → DB

Strategies:
   Cache-Aside    → READ-heavy (90%)
   Write-Through  → critical fresh data
   Write-Behind   → high write volume
   Refresh-Ahead  → predictable hot

Eviction:
   LRU  → least recently used (most common)
   LFU  → least frequently used
   FIFO → oldest insertion
   TTL  → time-based expiry

Tools:
   Redis      → modern, full-featured (80%)
   Memcached  → simple key-value (legacy)
   Caffeine   → Java in-process

Invalidation:
   TTL + explicit invalidation = production combo
```

---

## Trap Box

```
Trap 1: "Cache everything"
         Dynamic data (balance, real-time) — fresh chahiye
         Read-heavy + stable data only

Trap 2: "No TTL"
         Cache infinite grow → memory leak
         Always TTL — force refresh

Trap 3: "Cache stampede"
         Expire ke baad sab DB hit
         Mutex / refresh-ahead / soft TTL

Trap 4: "Stale data not noticed"
         DB updated, cache stale, user sees old
         TTL + explicit invalidation

Trap 5: "Cache as DB replacement"
         No persistence — restart = data loss
         Cache supplements DB, not replaces
```
