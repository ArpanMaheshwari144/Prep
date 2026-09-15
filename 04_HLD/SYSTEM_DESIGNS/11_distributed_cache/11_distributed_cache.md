# Distributed Cache (Redis-jaisa) — POORA ROUND (4 MOVE)

> **NAV** — ARCHETYPE F · DIL: SPEED + node mare to chale. UP: [MASTER](../../00_MASTER_SHEET.md) · CONCEPTS: [caching](../../FOUNDATIONS/04_caching.md) · [sharding](../../FOUNDATIONS/06_database_sharding.md) · [replication](../../FOUNDATIONS/05_database_replication.md)

> 15-Sep: asli mock-video ke hisaab se dobara likha — koi 7-step rail nahi, sirf 4 move.
> Har jagah: **tu kya BOLTA hai · BOARD pe kya banta · FAISLA + KYUN**.
> Concept detail = FOUNDATIONS/04_caching.md
>
> Problem (1 line): fast in-memory key-value store jo ek machine se bada ho, aur node mare to bhi chale.

```
★ LIBRARY-DESK ANALOGY (visual anchor):
   10,000 kitaabein godaam me (DB = slow par poora). Top-100 popular front-desk pe (cache = fast, RAM).
   Ek desk (1 machine) me saari popular nahi aatin + banda beemar pad gaya (node down)
   -> kai desk (nodes) + kaunsi kitaab kis desk pe (sharding) + backup banda (replication)
   = DISTRIBUTED CACHE
```

```
★★ TEEN NIYAM (poori file par lagte — [APPROACH_DELIVERY](../../HLD_APPROACH_DELIVERY.md) 5b)
   1. PERFECT design ek saath mat banao — chhote se shuru, dikkat pe badhao
   2. NUMBER ke peeche mat bhaago — bolo, ek faisla nikaalo, aage badho
   3. BOTTLENECK ratto mat — KHUD USER banke raasta chalao, khud dikh jaayega
```

---

# MOVE 1 — POOCHO (board pe abhi kuch nahi)

```
   TU: "Cache ka design karte waqt do raste hain — ek node ka cache (eviction, TTL),
        ya distributed cache (sharding, replication). Aap kis pe focus karwana chahenge?"

   TU: "Kuch cheezein confirm kar lun —
          - kis tarah ka data? read-heavy hai?
          - STALENESS kitni chalegi — 5 minute purana data chalega, ya real-time chahiye?
          - eviction policy pe koi pasand hai (LRU / LFU)?
          - cache DB ke aage hai, ya khud primary store hai?"

   ★ STALENESS wala sawaal zaroori hai — isi se invalidation ka poora hissa tay hota hai.
```

---

# MOVE 2 — DO CHHOTE BLOCK LIKHO

```
   ┌──────────────────────┐    ┌────────────────────────────────┐
   │ Distributed Cache    │    │ Use cases:                     │
   │   - Key / Value      │    │   - get(key) -> value          │
   │   - Node             │    │   - put(key, value, ttl)       │
   │   - TTL              │    │   - delete(key)                │
   │   - Eviction policy  │    │                                │
   └──────────────────────┘    │ NOT in scope: persistence,     │
                               │   pub/sub, transactions        │
   ┌───────────────────────────┐└────────────────────────────────┘
   │ Kya chahiye (NFR):        │
   │  - <1ms LATENCY   <- DIL  │
   │  - HA: node mare to chale │
   │  - SCALABLE: TBs data     │
   │  - CONSISTENCY = EVENTUAL │
   │    (stale chalega)        │
   └───────────────────────────┘

   TU: "Cache ka poora point SPEED hai — isliye RAM me rakhunga, disk pe nahi.
        Aur yahan consistency pe samjhauta kar sakta hoon: asli sach DB me hai,
        cache uski tez copy hai. Thoda purana data chal jaayega."

   ★ BOOKING KA ULTA: wahan consistency = dil (seat double na ho). Yahan SPEED = dil.
     Ye farak bolna — dikhata hai ki tu requirement se design nikaal raha hai.
```

```
   Numbers:
     - ~1 TB data
     - ~1 M QPS
     - READ-HEAVY

   HAR NUMBER SE EK FAISLA (yahi bolna):
     1 TB       ──►  ek node ki RAM me nahi aayega        -> data TODNA padega (sharding)
     1 M QPS    ──►  ek node itna throughput nahi degi    -> nodes ke beech baanto
     node marta ──►  cache gayab = saara load DB pe       -> REPLICATION

   => teeno milke "single node" ko khaarij kar dete hain -> DISTRIBUTED justify ho gaya
```

---

# MOVE 3 — BOXES BANAO (chhota banao, phir dikkat pe badhao)

```
   TU: "Sabse simple cheez se shuru — ek machine, ek HashMap."

        CLIENT ──► [ CACHE (HashMap, RAM) ] ──miss──► [ DB ]
                         key -> value                  source of truth
                         O(1) get/put

   TU: "Hit pe DB gaya hi nahi. Ab chala ke dekhte hain."
```

### dikkat 1 — "RAM bhar gayi. Ab kya hatayein?"

```
   FAISLA: EVICTION = LRU (Least Recently Used)

        HashMap  +  Doubly-Linked-List          (yahi LC-146 wala dhaancha hai)

             get(x)        -> node ko list ke FRONT laao (most recently used)
             jagah nahi    -> TAIL wala hatao (sabse purana access)
             dono O(1)

        [FRONT] ← most recent                 least recent → [TAIL]
           x  ←→  b  ←→  a  ←→  z  ←→  ...  ←→  q   (ye nikalega)

   + TTL: har entry ke saath expiry-timestamp
        lazy   -> access pe dekho "expired?" -> hata do
        active -> background thread thodi-thodi der me purge kare

   TU: "LRU isliye ki cache me wahi rehna chahiye jo abhi kaam aa raha hai.
        Agar frequency zyada maayne rakhti ho to LFU bhi chun sakte hain."
```

### dikkat 2 — "1 TB ek node me nahi aayega — kai node chahiye. Par key kis node pe jaayegi?"

```
   OPTION 1 — hash(key) % N   (naive)
        DIKKAT: node add/remove kiya -> N badal gaya -> hash%N badal gaya
                -> LAGBHAG SAARI keys ka node badal gaya
                -> massive MISS -> saari request DB pe -> DB CRASH
        => NAHI

   OPTION 2 — ★ CONSISTENT HASHING (WINNER)

        ek RING (0 .. 2^32), node aur key DONO ring pe hash hote

                    ┌──── Node A ────┐
                 k1 │                │ k2
                    │                │
              Node C│      RING      │Node B
                    │                │
                 k4 └────────────────┘ k3

        har key apne CLOCKWISE agle node pe baithti hai
        node add/remove -> sirf us arc ki K/N keys hilti hain, poora remap NAHI

        ★ VIRTUAL NODES: ek physical node = ring pe kai points
          -> load even bat jaata (warna ek node pe bheed ho jaati)

        # wahi consistent hashing sharded-DB aur load-balancer me bhi chalti
          -> ek tool, kai jagah

   OPTION 3 — range shard (A-M / N-Z)
        simple hai, PAR hot-range imbalance (ek range pe bheed) => NAHI

   FAISLA: OPTION 2 — kam se kam rebalance + even load

        CLIENT ──► [ CACHE CLIENT (routing) ] ──► Node A / Node B / Node C
                     consistent hashing            (app ko nodes ki ginti se azaad rakhta)
```

### dikkat 3 — "ek node mar gaya — uske saare key gayab, load seedha DB pe"

```
   FAISLA: REPLICATION — har shard ka 1-2 replica

        Node A (primary)  ──copy──►  Node A' (replica)
             │
          mar gaya  ──►  replica PROMOTE  ──►  cache zinda rehta

   ★ TRADE-OFF bolna: replication me LAG hota hai -> thodi der inconsistency
     par yahan eventual consistency chal jaati hai, to theek hai.
```

### dikkat 4 — "DB me data update ho gaya, cache purana pada hai (STALE)"

```
   TEEN TAREEKE:

     CACHE-ASIDE     : DB update karo -> cache se entry DELETE (invalidate)
                       -> agli read fresh laayegi                      <- sabse common
     WRITE-THROUGH   : cache aur DB dono ek saath likho
                       -> hamesha fresh, par har write slow
     TTL             : max-staleness ki hadd baandh do (auto)

   PRODUCTION = TTL + explicit invalidation ka COMBO

   TU: "'Cache invalidation is one of the hardest problems' — isliye main dono lagata hoon:
        explicit invalidate jahan pata hai, aur TTL safety-net ki tarah jahan kuch chhoot jaaye."
```

### dikkat 5 — "ek super-hot key expire hui aur 1000 request ek saath miss ho gayi"

```
   STAMPEDE (thundering herd):

        hot key expire
             │
        1000 request ek saath MISS  ──►  1000 request seedha DB pe  ──►  DB CRASH

   FIX (teen, koi ek ya mila ke):
        MUTEX / lock   -> sirf 1 thread DB se rebuild kare, baaki WAIT karein
                          phir sab cache se le lein
        SOFT-TTL       -> expiry se PEHLE background me refresh kar do
        NEVER-EXPIRE   -> bahut hi hot key ko expire hi mat karo, background update karo
```

### dikkat 6 — "ek key itni popular hai ki uska SHARD akela mar raha hai"

```
   HOT-KEY OVERLOAD: consistent hashing ne key ek node pe daali, par us key pe hi saara traffic

   FIX:
     1. hot key ko KAI node pe replicate karo -> read bat jaayein
     2. L1 LOCAL CACHE — app ke andar hi ek mini cache (in-process)
        -> request Redis tak jaati hi nahi

        [ App + L1 local cache ]  nano-second
                 │ miss
                 ▼
        [ L2 distributed cache ]  micro-second (network)
                 │ miss
                 ▼
        [ DB ]                    milli-second
```

### ab poora naksha (jahan pahunche) + har box ka KYUN

```
        CLIENT (app servers)   [+ L1 local cache andar]
           │  get / put
           ▼
   ┌─────────────────────────────┐
   │   CACHE CLIENT (routing)    │  consistent hashing: key -> node
   └───────────┬─────────────────┘
       ┌───────┼───────┐
       ▼       ▼       ▼
   ┌──────┐ ┌──────┐ ┌──────┐
   │Node A│ │Node B│ │Node C│   har node: HashMap + DLL (LRU) + TTL
   │ +rep │ │ +rep │ │ +rep │   har node: primary + replica (HA)
   └──┬───┘ └──┬───┘ └──┬───┘
      │  miss  │        │
      ▼        ▼        ▼
   ┌─────────────────────────────┐
   │         DATABASE            │  source of truth (slow)
   └─────────────────────────────┘

     Cache Client : key ko sahi node pe bhejta (consistent hash);
                    app ko nodes ki ginti se azaad rakhta
     Node         : in-memory store — HashMap + DLL(LRU) + TTL; primary + replica
     DB           : permanent sach; miss pe yahin se aata

   ★ CORE LINE: cache HIT pe DB gaya hi nahi (~90%) -> DB ka load 10x kam
```

```
   READ PATH — CACHE-ASIDE:
        HIT  -> seedha return (~90% baar, DB gaya hi nahi)
        MISS -> DB se padho -> cache me PUT karo -> return
```

---

# MOVE 4 — BOLTE-BOLTE JODO (jo poocha jaaye, wahi kholo)

## ► "API kya hogi?"

```
   GET     key
   PUT     key  value  ttl
   DELETE  key

   TU: "API jaan-boojh ke itni chhoti hai — cache me query nahi karte, sirf key se uthate hain.
        Isliye koi index ya query-planner ki zaroorat hi nahi, aur yahi isko itna tez banata hai."
```

## ► "Ek node ke ANDAR data kaise rakha hai?"

```
   HashMap key -> value        (O(1) get / put)

   EVICTION = LRU = HashMap + Doubly-Linked-List   (LC-146 wala dhaancha)
        get(x)     -> FRONT laao
        jagah nahi -> TAIL hatao
        dono O(1)

   TTL = entry ke saath expiry-timestamp
        lazy   -> access pe check
        active -> background purge

   ★ chunaav: in-memory KV (Redis-jaisa), aur DB peeche source of truth.
     <1ms chahiye -> RAM (disk nahi) . join nahi chahiye -> KV kaafi hai.
```

## ► "Keys nodes pe kaise baantoge?" (deep-dive ka dil — upar dikkat-2 me poora hai)

```
   hash % N            -> node badla to sab shift -> DB crash        => NAHI
   CONSISTENT HASHING  -> ring + clockwise + virtual nodes           => ★ WINNER
   range shard         -> simple par hot-range imbalance             => NAHI
```

## ► "Kahan tootega / scale?"

```
   ★ RATTO MAT — request ka raasta chalao:

      client ne get(key) maara
          │
          ├─► cache client -> node add/remove pe sab shift?  -> consistent hashing + virtual nodes
          ├─► node         -> RAM bhari?                     -> LRU + TTL
          │                   node mara?                     -> replica promote
          ├─► hot key      -> ek shard akela mar raha?       -> hot-key replicate . L1 local cache
          ├─► expiry       -> hot key expire pe bheed?       -> mutex . soft-TTL . never-expire
          └─► DB           -> stale data?                    -> invalidate + TTL combo

      + metrics: HIT-RATIO sabse zaroori metric hai (girna shuru hua = kuch toota hai)
```

## ► WRAP + MEMORY HOOK (jaldi revise ke liye)

```
   SINGLE node : HashMap + DLL (LRU) + TTL
   DISTRIBUTE  : Consistent Hashing (shard) + Replication (HA — mare to chale)
   READ        : Cache-Aside (miss -> DB -> populate; ~90% hit)
   STALE       : TTL + explicit invalidation
   STAMPEDE    : hot key expire -> mutex / soft-TTL
   HOTSPOT     : hot key replicate / L1 local cache
   LAYERS      : L1 app ke andar (nano-sec) -> L2 distributed Redis (micro-sec) -> DB (milli-sec)
   AAGE        : monitoring (hit-ratio), write-back agar write-heavy ho, multi-region cache
   TRADE-OFF   : speed vs consistency — use-case pe chunta hai
                 (= CAP ka hi spirit; cache AP ki taraf jhukta, eventual chal jaata)
```

```
★ POWER-CLOSER (aakhir me bolne wali line):
  "Distributed cache = in-memory KV store, consistent hashing se sharded, HA ke liye replicated.
   Cache-aside + LRU + TTL = production ka combo.
   Sabse mushkil do cheezein: invalidation (TTL + explicit) aur stampede (mutex / soft-TTL).
   Koi system perfect nahi hota — speed vs consistency ka trade-off use-case se tay hota hai."
```

---

[← MASTER SHEET](../../00_MASTER_SHEET.md)
