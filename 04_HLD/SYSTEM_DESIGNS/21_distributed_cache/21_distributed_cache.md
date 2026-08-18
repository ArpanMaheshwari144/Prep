# Design a Distributed Cache — Visual System Design

> **HLD Design 21 — component design (Redis-jaisa distributed cache).**
> Concept detail = FOUNDATIONS/04_caching.md. Yahan wahi cheezein DESIGN-format me (interview-delivery).

---

## 1 Problem (Analogy)

```
   LIBRARY FRONT DESK
        │
   10,000 books warehouse me (DB = slow, complete)
   Top-100 popular books front desk pe (cache = fast, RAM)
        │
        ▼
   Ek front desk (1 machine) me sab popular books nahi aate + banda beemar (node down)
        │
        ▼
   Kai front desk (nodes) + har book kis desk pe (sharding) + backup banda (replication)
        │
        ▼
        = DISTRIBUTED CACHE
```

---

## 2 Requirements (SCOPE clarify — interviewer se poochho)

```
   FUNCTIONAL:
      get(key)            -> value
      put(key, value, ttl)
      delete(key)

   NON-FUNCTIONAL (yahi weight rakhte):
      • Low latency    -> < 1ms (in-memory RAM)
      • High Availability -> node mare to bhi chale
      • Scalable       -> horizontal, TBs data (1 machine me nahi)
      • Consistency    -> EVENTUAL ok (cache; source-of-truth = DB)

   SCALE bol do: ~1 TB data, ~1M QPS -> 1 node me na fit na handle -> DISTRIBUTED justify
```

---

## 3 API

```
   GET    key
   PUT    key value ttl
   DELETE key
```

---

## 4 Single Node pehle (foundation)

```
   In-memory HASHMAP:   key -> value        (O(1) get/put)
   EVICTION = LRU     = HashMap + Doubly Linked List  (O(1) get/put -- LeetCode 146)
        access -> node FRONT (most recent) ;  evict -> list END (least recent)
   TTL      = har entry ke saath expiry timestamp
        lazy   -> access pe check "expired?" -> hata do
        active -> background thread periodically purge
```

Problem: **1 TB ek machine ki RAM me nahi aata** -> data todna padega.

---

## 5 Distributed — SHARDING (data split across nodes)

```
   Key kis node pe? 
   Naive:  hash(key) % N     (N = node count)
   ✗ node add/remove -> N badla -> LGBHAG SAARE keys remap
        -> massive cache MISS -> DB pe bheed -> crash
```

### ★ Fix = CONSISTENT HASHING  (interview me BOLNA — bada point)

```
   • ek HASH RING (0 .. 2^32)
   • node + key dono ring pe hash -> key apne CLOCKWISE-next node pe
   • node add/remove -> sirf K/N keys move (poora nahi remap)
   • VIRTUAL NODES -> ek node = ring pe kai points -> load even bikhre
```

---

## 6 Availability — node down?

```
   REPLICATION: har shard ka 1-2 REPLICA (primary + replica)
      • primary down -> replica PROMOTE -> cache zinda
      • trade-off: replication LAG -> thodi der inconsistency (eventual, ok)
```

---

## 7 DEEP DIVE — Read path (DB na hit ho) = CACHE-ASIDE

```
   get(key):
      cache HIT?   -> return (DB gaya hi nahi)     <- ~90% yahi
      cache MISS?  -> app DB se padho -> cache me PUT -> return
   = 90% hit ratio -> DB load 10x kam.  ("DB na hit ho" ka jawab)
```

---

## 8 DEEP DIVE — Stale data (cache purana -> update kaise)

```
   WRITE strategy:
      Cache-Aside   : DB update -> cache DELETE (invalidate) -> next read fresh
      Write-Through : cache + DB dono sync (always fresh, slow write)

   INVALIDATION (staleness control):
      • TTL       -> auto-expire (staleness ki max limit)
      • explicit  -> write pe cache delete
      • PRODUCTION = TTL + explicit COMBO
```

> Power-line: *"Cache invalidation hardest problem — TTL + explicit invalidation combo, staleness bounded."*

---

## 9 DEEP DIVE — Hot key EXPIRE (stampede / thundering herd)

```
   EK super-hot key expire -> 1000s request EK SAATH miss -> sab DB pe -> crash
   FIX:
      • MUTEX / lock    -> 1 thread DB se rebuild, baaki WAIT (phir cache se)
      • soft/logical TTL -> background me expiry se PEHLE refresh
      • never-expire    -> hot keys expire hi mat karo, async update
```

---

## 10 DEEP DIVE — Hot key OVERLOAD (ek shard pe bheed)

```
   Ek key itni popular ki uska SHARD akela overwhelmed
   FIX: hot key ko KAI nodes pe replicate / L1 LOCAL cache (app ke andar)
        -> load bikhar jaata
```

---

## 11 Full Picture

```
        CLIENT (app servers)
           │  get/put
           ▼
   ┌─────────────────────────────┐
   │   CACHE CLIENT (routing)    │  <- consistent hashing: key -> node
   └───────────┬─────────────────┘
       ┌───────┼───────┐
       ▼       ▼       ▼
   ┌──────┐ ┌──────┐ ┌──────┐
   │Node A│ │Node B│ │Node C│   each: HashMap+DLL(LRU) + TTL
   │ +rep │ │ +rep │ │ +rep │   each: primary + replica (HA)
   └──┬───┘ └──┬───┘ └──┬───┘
      │  miss  │        │
      ▼        ▼        ▼
   ┌─────────────────────────────┐
   │        DATABASE             │  source of truth (slow)
   └─────────────────────────────┘
```

---

## 12 Trade-off (interview closer)

> *"Distributed cache = in-memory KV, consistent-hashing se sharded, replicated for HA.
> Cache-aside + LRU + TTL = production combo. Hardest = invalidation (TTL+explicit) aur
> stampede (mutex/soft-TTL). Koi system perfect nahi — speed vs consistency trade-off,
> use-case ke hisaab se chunta." (= CAP ka spirit)*

---

## Memory Hook

```
   Single node:  HashMap + DLL(LRU) + TTL
   Distribute:   Consistent Hashing (shard) + Replication (HA)
   Read:         Cache-Aside (miss -> DB -> populate)
   Stale:        TTL + explicit invalidation
   Stampede:     hot-key expire -> mutex / soft-TTL
   Hotspot:      replicate hot key / L1 local
```

---

# 8-STEP INTERVIEW FRAMEWORK DRIVE

> (Framework: 04_HLD/INTERVIEW_FRAMEWORK.md) — file content se, more-explanation. SPEED = dil. Interview me isi flow me bolo.

## STEP 1 — REQUIREMENTS (SPEED = dil)
```
   FUNCTIONAL:  get(key) -> value . put(key, value, ttl) . delete(key)
   NON-FUNCTIONAL:
     - <1ms LATENCY  -> cache ka poora point = SPEED. disk (ms) nahi, RAM (microsec) me rakhte.
     - HA (High Availability) -> ek node mare to bhi cache CHALE (mara = miss = DB pe load). backup/replica se.
     - SCALABLE      -> TBs data, ek machine me nahi -> horizontal (kai node).
     - CONSISTENCY = EVENTUAL ok -> cache stale ho to chalega; asli sach DB me (cache = fast copy).
   CLARIFY:  kis type data? read-heavy? staleness (5-min ya real-time)? evict policy?
   # booking ka ULT: wahaan consistency=dil (seat double na ho), yahaan SPEED=dil.
```

## STEP 2 — SCALE / numbers (numbers drive karte)
```
   1 TB data, ~1M QPS, READ-HEAVY.
   - 1 node me 1TB RAM NAHI aata (na fit)         -> data todo = SHARDING
   - 1 node 1M QPS handle nahi karta (throughput) -> nodes ke beech baanto
   - node mar sakta                                -> REPLICATION
   => single-node se DISTRIBUTED justify.
```

## STEP 3 — API design
```
   GET    key
   PUT    key value ttl
   DELETE key
```

## STEP 4 — HIGH-LEVEL boxes (+ har box KYUN)
```
   CLIENT -> CACHE CLIENT (routing) -> Node A/B/C (+replica) --miss--> DATABASE
   box KYUN:
     Cache Client -> key ko node pe route (consistent-hash); app ko nodes ki ginti se azaad rakhe
     Node         -> in-mem store: HashMap + DLL(LRU) + TTL ; primary + replica (HA)
     DB           -> permanent source of truth (slow); miss pe yahin se
   CORE: cache HIT pe DB gaya hi nahi (~90%) -> DB load 10x kam.
```

## STEP 5 — DATA MODEL (per node)
```
   HashMap key -> value (O(1) get/put)
   EVICTION = LRU = HashMap + Doubly-Linked-List (O(1) -- LC146):
        get(x)      -> node ko list ke FRONT la do (most-recent)
        jagah bhari -> TAIL (least-recently-used) hatao -> dono O(1)
   TTL = entry ke saath expiry-ts:
        lazy   -> access pe check "expired?" -> hata
        active -> background thread periodic purge
   choice: in-mem KV (Redis-jaisa), DB peeche = source of truth.
   # <1ms -> RAM (disk nahi) . join nahi chahiye -> KV kaafi.
```

## STEP 6 — DEEP DIVE: keys ko nodes pe kaise baanto? (options -> choose)
```
   OPTION 1 — hash(key) % N  (naive): key ko N se mod -> node.
        DIKKAT: node add/remove -> N badla -> hash%N badla -> LGBHAG SAARE keys ka node badla
                -> massive MISS -> sab request DB pe -> DB CRASH. NAHI.
   OPTION 2 — CONSISTENT HASHING (BEST):
        ek RING (0..2^32); node aur key DONO ko ring pe hash.
        key -> apne CLOCKWISE-agle node pe baithti.
        node add/remove -> sirf us arc ke K/N keys move (poora nahi remap).
        VIRTUAL NODES -> ek physical node = ring pe kai points -> load even (warna ek node pe bheed).
        # wahi consistent-hashing sharded-DB + load-balancer me bhi = ek tool, kai jagah.
   OPTION 3 — range-shard (A-M / N-Z): simple PAR hot-range imbalance (ek range pe bheed). NAHI.
   WINNER: OPTION 2 -> minimal rebalance + even load.
```

## STEP 7 — BOTTLENECK / SCALE
```
   Node DOWN -> REPLICATION: har shard ka 1-2 replica (primary+replica). primary mare -> replica PROMOTE -> cache zinda.
                trade-off: replication LAG -> thodi der inconsistency (eventual, ok).
   Read path -> CACHE-ASIDE: HIT -> return (~90%, DB gaya hi nahi) | MISS -> DB se padho -> cache PUT -> return. DB load 10x kam.
   STALE data (DB update hua par cache purana):
        Cache-Aside: DB update -> cache DELETE -> next read fresh
        TTL: auto max-staleness limit
        PROD = TTL + explicit COMBO. ("cache invalidation = hardest problem")
   Hot-key STAMPEDE: ek super-hot key expire -> 1000s request EK SAATH miss -> sab DB pe -> crash.
        FIX: MUTEX (1 thread DB se rebuild, baaki WAIT phir cache se) | soft-TTL (expiry se pehle bg-refresh) | never-expire.
   Hot-key OVERLOAD: ek key itni popular ki uska SHARD akela overwhelmed.
        FIX: hot key kai nodes replicate | L1 LOCAL cache (app ke andar, in-process -> request Redis tak jaati hi nahi).
```

## STEP 8 — WRAP
```
   SINGLE:     HashMap + DLL(LRU) + TTL
   DISTRIBUTE: Consistent-Hashing (shard) + Replication (HA = mare-to-chale)
   READ:       Cache-Aside (miss -> DB -> populate; 90% hit)
   STALE:      TTL + explicit invalidation  |  STAMPEDE: mutex / soft-TTL  |  HOTSPOT: replicate / L1-local
   # L1 = app ke andar local mini-cache (nano-sec) ; L2 = distributed Redis (network, micro-sec) -> DB.
   IMPROVE:    metrics/monitoring (hit-ratio), write-back for write-heavy, multi-region cache.
   TRADE-OFF:  speed vs consistency -> use-case pe chunta (= CAP spirit; cache = AP-leaning, eventual ok).
```
