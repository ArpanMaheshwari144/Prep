# Distributed Cache (Redis-jaisa) — 7-STEP RAIL (single spine, revise top→bottom)

> HLD component-design. Concept detail = FOUNDATIONS/04_caching.md. SPEED = dil.
> RAIL (04_HLD/HLD_APPROACH_DELIVERY.md). Merged into clean 7-step 7-Sep.
> Problem (1 line): fast in-memory key-value store jo 1 machine se bada + node-mare-to-chale.

```
★ LIBRARY-DESK ANALOGY: 10,000 books warehouse me (DB = slow, complete). Top-100 popular front-desk pe (cache = fast, RAM).
   Ek desk (1 machine) me sab popular nahi aate + banda beemar (node down) ->
   Kai desk (nodes) + har book kis desk (sharding) + backup-banda (replication) = DISTRIBUTED CACHE.
```

---

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

## STEP 2 — ESTIMATE (scale / numbers drive karte)
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

## STEP 4 — DATA MODEL (per node)
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

## STEP 5 — HL BOXES (+ har box KYUN)
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
   box KYUN:
     Cache Client -> key ko node pe route (consistent-hash); app ko nodes ki ginti se azaad rakhe
     Node         -> in-mem store: HashMap + DLL(LRU) + TTL ; primary + replica (HA)
     DB           -> permanent source of truth (slow); miss pe yahin se
   CORE: cache HIT pe DB gaya hi nahi (~90%) -> DB load 10x kam.
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
        Cache-Aside: DB update -> cache DELETE (invalidate) -> next read fresh
        Write-Through: cache + DB dono sync (always fresh, slow write)
        TTL: auto max-staleness limit
        PROD = TTL + explicit COMBO. ("cache invalidation = hardest problem")
   Hot-key STAMPEDE (thundering herd): ek super-hot key expire -> 1000s request EK SAATH miss -> sab DB pe -> crash.
        FIX: MUTEX (1 thread DB se rebuild, baaki WAIT phir cache se) | soft-TTL (expiry se pehle bg-refresh) | never-expire.
   Hot-key OVERLOAD: ek key itni popular ki uska SHARD akela overwhelmed.
        FIX: hot key kai nodes replicate | L1 LOCAL cache (app ke andar, in-process -> request Redis tak jaati hi nahi).

   WRAP:
     SINGLE:     HashMap + DLL(LRU) + TTL
     DISTRIBUTE: Consistent-Hashing (shard) + Replication (HA = mare-to-chale)
     READ:       Cache-Aside (miss -> DB -> populate; 90% hit)
     STALE:      TTL + explicit invalidation  |  STAMPEDE: mutex / soft-TTL  |  HOTSPOT: replicate / L1-local
     # L1 = app ke andar local mini-cache (nano-sec) ; L2 = distributed Redis (network, micro-sec) -> DB.
     IMPROVE:    metrics/monitoring (hit-ratio), write-back for write-heavy, multi-region cache.
     TRADE-OFF:  speed vs consistency -> use-case pe chunta (= CAP spirit; cache = AP-leaning, eventual ok).
```

> POWER-CLOSER: *"Distributed cache = in-memory KV, consistent-hashing se sharded, replicated for HA.
> Cache-aside + LRU + TTL = production combo. Hardest = invalidation (TTL+explicit) aur stampede (mutex/soft-TTL).
> Koi system perfect nahi — speed vs consistency trade-off, use-case ke hisaab se chunta." (= CAP ka spirit)*

---

## ═══ MEMORY HOOK (recall table) ═══
```
   Single node:  HashMap + DLL(LRU) + TTL
   Distribute:   Consistent Hashing (shard) + Replication (HA)
   Read:         Cache-Aside (miss -> DB -> populate)
   Stale:        TTL + explicit invalidation
   Stampede:     hot-key expire -> mutex / soft-TTL
   Hotspot:      replicate hot key / L1 local
```

---

[← HLD README](../README.md)
