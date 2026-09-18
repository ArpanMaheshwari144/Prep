# Caching

> **NAV** — KYA: caching (aside/through, TTL, eviction). UP: [MASTER](../00_MASTER_SHEET.md) · trade-off: [cache strategies](../TRADEOFFS.md) · lagta hai: [distributed-cache](../SYSTEM_DESIGNS/11_distributed_cache/11_distributed_cache.md) · [url-shortener](../SYSTEM_DESIGNS/01_url_shortener/01_url_shortener.md) · [twitter-feed](../SYSTEM_DESIGNS/03_twitter_feed/03_twitter_feed.md)

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

## ★★ PEHLE LOAD KO ALAG KARO, PHIR ILAAJ CHUNO (cache har read pe nahi lagti)

> Ek hi page pe do bilkul alag tarah ka load ho sakta hai. Ek ilaaj dono pe nahi chalta.

```
RESULT-DAY wala scenario (10 lakh student ek saath):

   TOPPERS LIST   -> EK hi cheez, 10 lakh baar maangi gayi
                     DB wahi query 10 lakh baar chala raha = poori bewakoofi
                     -> CACHE  (10 lakh me se DB pe sirf 1 request jaayegi)

   APNA RESULT    -> har student ki ALAG cheez -> 10 lakh alag key
                     cache lagane pe hit-rate lagbhag ZERO (har banda naya key maangta)
                     -> cache yahan BEKAAR
                     -> ilaaj: roll_number pe INDEX (data 2 GB hai, index se milliseconds)
```

```
★ SHARED (sabko wahi)   -> CACHE
★ PER-USER (sabko alag) -> INDEX / seedha DB
★ pehle poocho: "kitni UNIQUE cheezein maangi ja rahi hain?"
     thodi si, bahut baar -> cache ka sona
     har request alag     -> cache bekaar
```

```
★ AUR: "machine badi kar do" is dikkat ka ilaaj nahi hai —
  badi machine wahi FALTU kaam tez karti hai, kaam KAM nahi karti.
  Cache kaam HATA deta hai. (aur saal me ek din ke liye 4x bill dena alag nuksan)
```

---

## ★★ CACHE KI KEEMAT — aur kab ise NAHI lagana

> Cache lagana muft nahi hai. Ye chaar cheezein saath me aati hain:

```
1. STALE DATA      DB me update ho gaya, cache me purana pada hai
                   ILAAJ : update pe entry DELETE (invalidate) + TTL safety-net
                   KEEMAT: system ab eventually-consistent hai — kuch der purana dikh sakta hai

2. MEMORY LIMIT    RAM mehngi hai, sab kuch nahi samayega
                   ILAAJ : LRU eviction
                   KEEMAT: kabhi jo chahiye tha wo abhi-abhi nikal gaya hoga

3. STAMPEDE        ek HOT key expire -> 1000 request ek saath miss -> sab DB pe -> DB down
                   ILAAJ : mutex (ek hi rebuild kare) . soft-TTL (expire se pehle background refresh)

4. ★ CACHE KHUD GIR GAYA  (sabse bada, aur sabse zyada bhoola jaata hai)
                   ab 100% traffic seedha DB pe — jo DB is load ke liye bana hi nahi tha
                   (cache lagate hi tumne DB ko us load ke liye CHHOTA kar diya tha)
                   ILAAJ : replica + failover; kahin-kahin fail-open
```

```
★ CACHE KAB NAHI LAGANA — ek line ka niyam:
     "purana data dikhna kitna mehnga hai?"
        bahut mehnga  -> bank balance . seat availability . payment status  -> cache nahi (ya bahut chhota TTL)
        sasta         -> feed . profile . product page . URL mapping        -> bilkul lagao
```

```
★ RAM vs DISK (thos number, bolne layak):
     DISK ~1-10 milli-second   |   RAM ~100 nano-second   -> hazaron guna farak
   + cache me QUERY hoti hi nahi — seedha key lookup (O(1)).
     DB ko parse -> plan -> index -> disk page -> row banana padta hai.
   => jagah bhi badli (RAM) aur KAAM bhi chhota hua (query -> lookup).
```

---

## ★★ REDIS = EK CHEEZ, TEEN ALAG KAAM (ye ghaalmel sabse zyada hota hai)

> Designs me "Redis" baar-baar aata hai — par har jagah uska KAAM alag hota hai.
> Ye farak na pakdo to sab mix ho jaata hai.

```
1. CACHE          -> DB ka jawab paas rakh liya
                     gir gaya to? -> sirf SLOW hoga, jawab wahi milega
                     misaal: url-shortener (wahi URL baar-baar) . hot tweet

2. SHARED STATE   -> sach YAHIN rehta hai (DB me hai hi nahi)
                     gir gaya to? -> GALAT behaviour, niyam TOOT jaayega
                     misaal: rate-limiter ka counter (har server ka apna count ho jaata -> limit toot)

3. MEMORY / DEDUP -> "ye kaam pehle ho chuka" ka nishaan
                     gir gaya to? -> dobara ho jaayega (double charge / double email)
                     misaal: idempotency key (payment) . eventId dedup (Kafka consumer)
```

```
★ PEHCHAN KA EK SAWAAL:
     "ye Redis abhi gir jaaye to kya hoga?"
        sirf SLOW hoga, jawab wahi         -> ye CACHE tha
        GALAT jawab / niyam toot jaayega   -> ye cache NAHI tha, ye SOURCE OF TRUTH tha
```

```
★ AUR EK CHEEZ JO CACHE JAISI DIKHTI HAI PAR NAHI HAI — PRECOMPUTE:
     twitter ka inbox (Redis list) = cache nahi, PEHLE SE BANAYA HUA jawab.
     cache  = "DB ne jo diya wo paas rakh liya"
     precompute = "jawab pehle hi bana ke rakh diya, DB se aisa jawab milta hi nahi"
```

```
   DESIGN              REDIS KAHAN               ASLI WAJAH
   ────────────────────────────────────────────────────────────────────────────
   url-shortener       cache-aside               wahi URL baar-baar padha ja raha
   twitter (hot tweet) cache-aside               ek tweet 10 crore log padh rahe
   rate-limiter        counter store             count sab servers me bikhar raha -> ek saanjhi jagah
   twitter (inbox)     precomputed feed          har read pe 200 logon se jodna mehnga
   payment / Kafka     dedup store (eventId)     "ye pehle ho chuka?" yaad rakhna
   dist-cache          cache khud product hai    —
```

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

## ═══ HANDS-ON — EVICTION LIVE (asli Redis, docker, 18-Sep) ═══

> Upar ki theory ko CHAL KE dekha. Har command + uska ASLI output neeche likha hai.
> Setup: `07_PROJECTS/usercrud/docker-compose.yml` me `redis-master` service (redis:7-alpine).
> Poora sawaal jise saabit karna tha: **"cache bhar jaaye to kya hota hai, aur kaun sa maal marta hai?"**

### Setup

```yaml
  redis-master:
    image: redis:7-alpine
    container_name: redis-master
    ports:
      - "6379:6379"
    command: redis-server --maxmemory 2mb --maxmemory-policy allkeys-lru
    restart: unless-stopped
```

```
docker compose up -d redis-master
docker exec redis-master redis-cli ping        ->  PONG
```

### ★ GALTI 1 — maxmemory BASELINE se neeche rakh diya (asli production gotcha)

```
docker exec redis-master redis-cli INFO memory
   used_memory_human : 1.08M       <- KHAALI Redis, 0 key, phir bhi 1 MB kha chuka
   maxmemory_human   : 2.00M
   DBSIZE            : 0

docker exec redis-master redis-benchmark -t set -n 100000 -r 100000 -d 200 -q
   Error from server: OOM command not allowed when used memory > 'maxmemory'.
```

**KYUN:** `maxmemory` poore Redis process ki memory ginta hai, sirf teri keys ki nahi.
Khaali Redis hi 1.08 MB le chuka tha (andaruni structures + client buffers) — bachi sirf ~0.9 MB.
Benchmark ke 50 client khulte hi limit paar. Ab Redis ne evict karna chaha, **par keys thi hi nahi** —
jo memory bhari thi wo keys ki thi hi nahi. Isliye haath khade: `OOM`.

**SABAK:** `maxmemory` hamesha baseline se KAAFI upar. Warna Redis ke paas phenkne ko kuch bachta hi nahi.

```
docker exec redis-master redis-cli CONFIG SET maxmemory 10mb
```

### ★ GALTI 2 — `DEBUG POPULATE` Redis 7 me band hai

```
DEBUG POPULATE 100000
   (error) ERR DEBUG command not allowed. If the enable-debug-command option is set to "local"...
```
Startup flag chahiye, chalte-chalte on nahi hota (koi galti se `DEBUG SEGFAULT` na maar de).
Iski jagah **`redis-benchmark`** — Redis ke saath hi aata hai, aur asli load jaisa hai.

```
redis-benchmark -t set -n 5000 -r 5000 -d 500 -q
   -t set    sirf SET chalao
   -n 5000   itne command bhejo
   -r 5000   itne alag key-naam ke daayre me se chuno
   -d 500    har value 500 byte
   -q        chup-chaap, bas aakhri line
```

### PRAYOG A — `allkeys-lru` : teri key BHI mar jaati

```
docker exec redis-master redis-cli FLUSHALL
docker exec redis-master redis-cli CONFIG RESETSTAT
docker exec redis-master redis-cli SET mera:naam arpan
```

| kadam | command | DBSIZE | used_memory | evicted_keys | `GET mera:naam` |
|---|---|---|---|---|---|
| 0 | (sirf apni key) | 1 | 1.11M | 0 | `arpan` |
| 1 | `-n 10 -r 10 -d 500` | **8** | — | 0 | `arpan` |
| 2 | `-n 5000 -r 5000 -d 500` | 3163 | 2.85M | 0 | `arpan` |
| 3 | `-n 30000 -r 30000 -d 500` | 12714 | **8.22M** | **9712** | **(khaali)** |

**Kadam 1 me 10 daale the, 7 kyun bani?** `-r 10` = sirf 10 naam ka daayra. 10 SET ne aankh band
karke naam chune — kuch naam DO baar aa gaye. `SET` ka niyam: key pehle se hai to nayi nahi banti,
purani ke upar likh jaata hai. To 10 me se ~7 alag nikle (+ `mera:naam` = 8). `KEYS *` ne saabit kiya:

```
key:000000000000  key:000000000002  key:000000000003  key:000000000004
key:000000000005  key:000000000006  key:000000000008  mera:naam
    ^ key:1, key:7, key:9 GAYAB — wo teen dabbe kisi ne chune hi nahi
```

**Kadam 3 ka asli natija:** 30,000 SET bheje. DBSIZE sirf +9551 badhi, aur 9712 keys PHENK di gayi.
Memory 10 MB se **kabhi paar nahi gayi** (8.22M pe ruki). Koi error nahi, koi crash nahi — Redis chalta raha.

★ **Aur `mera:naam` MAR GAYI.** Delete nahi ki thi, TTL nahi lagayi thi.
`allkeys-lru` ka matlab hi hai *saari* keys ki bali chadh sakti hai — Redis ko nahi pata
ki koi key tere liye khaas hai. 3 kadam se chhui nahi thi, to LRU ki nazar me wahi sabse purani thi.

### PRAYOG B — `volatile-lru` : sirf TTL wali marti hain

```
docker exec redis-master redis-cli CONFIG SET maxmemory-policy volatile-lru
docker exec redis-master redis-cli FLUSHALL
docker exec redis-master redis-cli CONFIG RESETSTAT
docker exec redis-master redis-cli SET mera:naam arpan            # TTL NAHI = dhaal
```

Bharne wali keys pe ab TTL hona ZAROORI hai — warna `volatile-lru` ke paas phenkne ko kuch nahi
aur wahi `OOM` wapas aa jaayega. Isliye `SET` ki jagah **`SETEX`** (TTL ke saath):

```
docker exec redis-master redis-benchmark -n 200000 -r 200000 -q SETEX junk:__rand_int__ 600 padding_value_1234567890
```

```
DBSIZE            : 56558
used_memory_human : 8.53M
evicted_keys      : 96957
GET mera:naam     : arpan          <- ZINDA
```

### ★★ DONO KA FARAK — yahi poori seekh hai

```
policy            evicted      mera:naam
allkeys-lru        9,712       (khaali)   <- mar gayi
volatile-lru      96,957       arpan      <- bach gayi
```

96,957 junk keys kat gayi kyunki unpe TTL tha. `mera:naam` pe TTL nahi tha — Redis ne use dekha bhi nahi.

**PRODUCTION ME MATLAB:** Redis me do kism ka maal rehta hai —

| kya | TTL | kyun |
|---|---|---|
| session · cache entry · rate-limit counter | haan | bhar jaaye to phenk do, dobara ban jaayega |
| feature flag · config · lookup table | nahi | dobara banana mehnga ya namumkin |

`volatile-lru` laga do, phir **TTL hi tay karta hai ki kaun phenka jaayega.**
TTL daala = "ye disposable hai". TTL nahi daala = "isko mat chhuna".

★ **Ismein chhupa hua BUG:** agar koi galti se config key pe bhi TTL laga de, wo bhi chupchaap
gayab ho sakti hai — aur pata tab chalega jab feature ruk jaayega. Koi error nahi aayega.

### Ek line me (interview me bolne layak)

> *"Cache kabhi sach ka maalik nahi hota. DB bharne pe 'fail' bolta hai, cache bharne pe
> 'purana maal phenk deta hoon, tu chalta reh' bolta hai. Kaun phenka jaayega ye `maxmemory-policy`
> tay karti hai — aur `volatile-lru` me wo faisla TTL ke haath me chala jaata hai."*

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
