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
