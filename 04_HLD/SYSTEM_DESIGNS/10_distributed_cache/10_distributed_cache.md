# Distributed Cache (Redis-jaisa)

> Tez in-memory key-value store jo ek machine se bada ho, aur node mare to bhi chale. Concept detail = FOUNDATIONS/04_caching.md
> Is design ka dil: **SPEED (<1ms)** + **node mare to chale**. Booking ka ULTA: wahan consistency dil, yahan speed.

```
LIBRARY DESK: 10,000 kitaab godaam me (DB = slow, poora) · top-100 front desk pe (cache = RAM, tez)
   ek desk me saari nahi aati + banda beemar (node down)
   -> kai desk (nodes) + kaunsi kitaab kis desk (sharding) + backup banda (replication) = DISTRIBUTED CACHE
```

---

## TASVEER (ByteByteGo / Alex Xu · CC BY-NC-ND 4.0)

![How Can Cache Systems Go Wrong?](https://assets.bytebytego.com/diagrams/0038-how-caches-can-go-wrong.png)
Source: [How Can Cache Systems Go Wrong?](https://bytebytego.com/guides/how-can-cache-systems-go-wrong/)
(thundering herd / penetration / breakdown / crash)

![Consistent Hashing Explained](https://assets.bytebytego.com/diagrams/0151-consistent-hashing.png)
Source: [Consistent Hashing Explained](https://bytebytego.com/guides/consistent-hashing/)
(node jude ya gire to sirf thodi keys khiskein — ring)

---

## SHURU — poocho + numbers

```
POOCHO:  "Ek node ka cache (eviction, TTL) ya distributed (sharding, replication) — kis pe?"
         kaisa data, read-heavy? · STALENESS kitni chalegi? (5 min purana?)  <- invalidation isi se tay
         eviction pasand (LRU / LFU)? · cache DB ke aage ya khud primary store?

FR:      get(key) · put(key, value, ttl) · delete(key)
         scope bahar: persistence · pub/sub · transactions
NFR:     <1ms (dil) · HA (node mare to chale) · TBs data · consistency EVENTUAL (asli sach DB me, cache tez copy)

NUMBERS: ~1 TB · ~1M QPS · read-heavy
         1 TB     -> ek node ki RAM me nahi -> SHARDING
         1M QPS   -> ek node itna nahi deti -> nodes me baanto
         node mara -> saara load DB pe -> REPLICATION
         teeno milke single node khaarij -> DISTRIBUTED justify
```

---

## DABBA 0 — sabse simple

```
SOLUTION: ek machine, ek HashMap (RAM) · O(1) get / put · CACHE-ASIDE: hit -> return · miss -> DB -> cache me PUT -> return
```
```mermaid
flowchart TD
    n_App["App"]
    n_DB["DB"]
    n_Cache["Cache"]
    n_App --> n_DB
    n_App --> n_Cache
```

---

## DIKKAT 1 — RAM bhar gayi, kya hatayein?

```
DIKKAT:   jagah khatam

SOLUTION: EVICTION = LRU (jo SABSE LAMBE SAMAY se nahi chhua — "kab") · LFU = SABSE KAM BAAR ("kitni baar")
          LRU = HashMap + Doubly Linked List (LC-146)
            get(x) -> FRONT laao · jagah nahi -> TAIL hatao · dono O(1)
            [FRONT] x <-> b <-> a <-> z <-> ... <-> q [TAIL, ye nikalega]
          TTL: entry ke saath expiry · LAZY (access pe check) + ACTIVE (background purge)
          LRU kyunki wahi rahe jo abhi kaam aa raha; frequency zyada maayne rakhe to LFU

NAYA:     koi dabba nahi — node ke andar
```
```mermaid
flowchart TD
    n_App["App"]
    n_DB["DB"]
    n_Cache["Cache<br/>LRU + TTL"]
    n_App --> n_DB
    n_App --> n_Cache
```
```
AGLA SAWAAL (tere jawab se):
  "Redis sach me poora LRU rakhta (har key ki list)?"
   -> nahi, memory bachane ko 'approximate LRU': kuch random keys uthata, unme sabse purani hatata
  "LFU me purana famous key hamesha rahega?"
   -> isliye LFU ginti ko waqt ke saath ghatata (decay) -> kal ka hero aaj nikal sake
```

---

## DIKKAT 2 — 1 TB ek node me nahi: key kis node pe?

```
DIKKAT:   kai node chahiye, key ka ghar tay karna

SOLUTION: hash(key) % N -> node juda / gaya -> N badla -> LAGBHAG SAARI key ka node badla
                        -> massive MISS -> sab DB pe -> DB CRASH -> NAHI
          CONSISTENT HASHING: RING (0 .. 2^32), node + key dono ring pe, key CLOCKWISE agle node pe
                        node juda / gaya -> sirf us arc ki ~K/N key hilti   <- YAHI
                        VIRTUAL NODES: ek machine = ring pe kai point -> load even
                        (wahi sharded DB aur LB me bhi — ek tool, kai jagah)
          range (A-M / N-Z) -> simple, par hot range -> NAHI
          CACHE CLIENT (routing) key -> node, app ko nodes ki ginti se azaad rakhta

NAYA:     Cache client (app ke andar library, key dekh ke sahi node chunti)
BADLA:    Cache -> Node A / B / C
```
```mermaid
flowchart TD
    n_App["App"]
    n_DB["DB"]
    n_Cache_client["Cache client"]
    n_Node_A["Node A"]
    n_Node_B["Node B"]
    n_Node_C["Node C"]
    n_App --> n_DB
    n_App --> n_Cache_client
    n_Cache_client --> n_Node_A
    n_Cache_client --> n_Node_B
    n_Cache_client --> n_Node_C
```
```
AGLA SAWAAL (tere jawab se):
  "Cache client ko kaise pata kaun-se node zinda hain?"
   -> config service / cluster ka topology (Redis Cluster khud batata: 'ye key us node pe hai, MOVED')
  "Naya node juda, uski key khaali -> miss?"
   -> haan, sirf us arc ki ~K/N key ka ek baar miss -> DB pe thoda bojh, poora nahi
```

---

## DIKKAT 3 — ek node mara, uski saari key gayab, load DB pe

```
DIKKAT:   node = ek shard ka akela ghar

SOLUTION: REPLICATION — har shard ka 1-2 replica · primary mara -> replica PROMOTE
          replica ALAG AZ me (ek AZ me = saath marenge)
          trade-off: replication LAG -> thodi der purana · cache me eventual chalta

BADLA:    Node A / B / C -> Node + replica

KAISE (kaun pakadta, kaun promote):
          Redis Sentinel (alag 3 process) ya Cluster ke baaki masters primary ko ping karte (gossip)
          zyada (quorum) bolein "mara" -> vote se ek replica ko primary banaya, clients ko naya pata
KYUN ASYNC replication (sync nahi):
          sync = har write pe replica ke "haan" ka intezaar -> cache ka <1ms toot jaata
          keemat: primary mara to aakhri kuch write replica tak nahi pahunche -> cache me chalta (DB me sach hai)
```
```mermaid
flowchart TD
    n_App["App"]
    n_DB["DB"]
    n_Cache_client["Cache client"]
    n_Node_A_replica["Node A + replica"]
    n_Node_B_replica["Node B + replica"]
    n_Node_C_replica["Node C + replica"]
    n_App --> n_DB
    n_App --> n_Cache_client
    n_Cache_client --> n_Node_A_replica
    n_Cache_client --> n_Node_B_replica
    n_Cache_client --> n_Node_C_replica
```
```
POOCHEGA: "What happens if a cache node goes down?"
BOL:      "Each shard has a replica in another zone that gets promoted, and with consistent hashing only that
           node's keys are affected."

AGLA SAWAAL (tere jawab se):
  "Network toota, purana primary bhi zinda samjha raha (do primary)?"
   -> split brain. Quorum + jo minority me hai wo write lena band kare (min-replicas)
  "Failover me kitni der?"
   -> ~10-30 sec; us beech us shard ki key DB se (thoda dheema)
```

---

## DIKKAT 4 — DB me update, cache purana (STALE)

```
DIKKAT:   cache me purani value

SOLUTION: CACHE-ASIDE: DB update -> cache key DELETE (invalidate) -> agli read fresh   <- sabse common
          WRITE-THROUGH: cache + DB ek saath -> hamesha fresh, har write slow
          TTL: max staleness ki hadd
          PRODUCTION = TTL + explicit invalidation ("invalidation is one of the hardest problems")
          key DELETE karo, UPDATE nahi (do write ulte kram me pahunche -> galat value baith-ti)
          doosra kaaran REPLICA LAG -> jisne abhi likha wo thodi der PRIMARY se (read-your-own-writes)

NAYA:     koi dabba nahi
```
```mermaid
flowchart TD
    n_App["App"]
    n_DB["DB"]
    n_Cache_client["Cache client"]
    n_Node_A_replica["Node A + replica"]
    n_Node_B_replica["Node B + replica"]
    n_Node_C_replica["Node C + replica"]
    n_App --> n_DB
    n_App --> n_Cache_client
    n_Cache_client --> n_Node_A_replica
    n_Cache_client --> n_Node_B_replica
    n_Cache_client --> n_Node_C_replica
```
```
POOCHEGA: "The user updated something but still sees the old value. Why?"
BOL:      "On update I delete the cache key rather than overwrite it, and keep a TTL as a safety net. If it's
           replica lag, the writer reads from the primary for a short while."

AGLA SAWAAL (tere jawab se):
  "DB update hua, cache DELETE fail ho gaya?"
   -> TTL bachata (max utni der purana). Pakka chahiye -> DB change event (CDC) se delete ka retry
  "DELETE ke baad koi purani value phir se cache me daal de (race)?"
   -> chhota delay ke baad dobara delete (double delete) ya version / TTL se
```

---

## DIKKAT 5 — super-hot key expire, 1000 request ek saath MISS

```
DIKKAT:   STAMPEDE (thundering herd): 1000 miss -> 1000 DB pe -> DB CRASH

SOLUTION: MUTEX / lock -> sirf 1 DB se rebuild, baaki WAIT, phir cache se
          SOFT-TTL -> expiry se PEHLE background refresh
          NEVER-EXPIRE -> bahut hot key, background update
          (2-Oct) PATA HO (sale, WC final, iPhone launch) -> PRE-WARM + servers pehle badhao
                  PATA NA HO (viral tweet) -> upar ke teen
                  twitter hot-tweet TTL 1 hr khatam + lakhon padh rahe = wahi stampede
                  asli me kam kyunki badi site ilaaj PEHLE lagaati

NAYA:     koi dabba nahi

KAISE (mutex):
          miss hua -> SET lock:key 1 NX EX 5
          jeeta (OK) -> DB se laao, cache bharo, lock DEL
          haara (nil) -> 50-100 ms ruko, cache dobara padho (tab tak jeetne wale ne bhar diya)
          lock pe EX kyun: jeetne wala beech me mara to lock 5 sec me khud chhoote, warna sab hamesha atke
```
```mermaid
flowchart TD
    n_App["App"]
    n_DB["DB"]
    n_Cache_client["Cache client"]
    n_Node_A_replica["Node A + replica"]
    n_Node_B_replica["Node B + replica"]
    n_Node_C_replica["Node C + replica"]
    n_App --> n_DB
    n_App --> n_Cache_client
    n_Cache_client --> n_Node_A_replica
    n_Cache_client --> n_Node_B_replica
    n_Cache_client --> n_Node_C_replica
```
```
POOCHEGA: "What if the cache goes down / a hot key expires?"
DHYAAN:   poora cache gaya -> DB fallback, par load shedding / rate limit ke saath — warna chhupa load ek saath DB pe
BOL:      "For known events I'd pre-warm the cache and scale up in advance. For unpredictable spikes I still
           need stampede protection: a lock so only one request rebuilds the key, and early background refresh
           before the TTL expires."

AGLA SAWAAL (tere jawab se):
  "Haarne wale kitni der rukenge?"
   -> chhota wait + kuch retry; phir bhi nahi to purani value (stale) do, DB pe mat bhejo
  "Soft-TTL kaise?"
   -> value ke saath 'refresh_after' time; padhne wala dekhe time nikal gaya -> background me ek refresh,
      tab tak purani value hi do
```

---

## DIKKAT 6 — ek key itni popular, uska shard akela mar raha

```
DIKKAT:   HOT KEY: consistent hashing ne ek node pe daali, saara traffic us key pe
          ★ consistent hashing ek hot key ko nahi bachata

SOLUTION: (1) hot key KAI node pe copy -> read bat jaayein (score#1..#10, random copy se padho)
          (2) L1 LOCAL CACHE — app ke andar mini cache -> request Redis tak jaati hi nahi
              L1 app (nano-sec) -> L2 distributed (micro-sec) -> DB (milli-sec)
          WRITE hot -> key me bucket (key#0..#9), kisi ek me likho, padhte waqt jodo
          PEHCHAAN: key KHAALI, sab DB bhage = STAMPEDE (dikkat 5, mutex)
                    key BHARI, ek node READ se mar raha = HOT KEY (ye, L1 + copies)
          MISAAL: IPL live score / flash sale page, 5 crore log ek key
                  L1 1-2 sec TTL har app server pe (score thoda purana chalega) = sabse bada ilaaj · page / image CDN

NAYA:     L1 local cache (har App server ki apni memory me chhota cache)
BADLA:    App -> App + L1 local cache
```
```mermaid
flowchart TD
    n_App_L1_local_cache["App + L1 local cache"]
    n_DB["DB"]
    n_Cache_client["Cache client"]
    n_Node_A_replica["Node A + replica"]
    n_Node_B_replica["Node B + replica"]
    n_Node_C_replica["Node C + replica"]
    n_App_L1_local_cache --> n_DB
    n_App_L1_local_cache --> n_Cache_client
    n_Cache_client --> n_Node_A_replica
    n_Cache_client --> n_Node_B_replica
    n_Cache_client --> n_Node_C_replica
```
```
POOCHEGA: "What about a hot key / celebrity / hot partition?"
BOL:      "The key is there, one node just can't take the reads. I'd put a 1-2 second local cache on each app
           server so most reads never reach Redis, and copy the key across replicas so the rest are spread
           out. A slightly stale score is fine here. If the key were missing instead, that's a stampede — one
           request rebuilds, the rest wait."

AGLA SAWAAL (tere jawab se):
  "Hot key pehchanoge kaise?"
   -> node ke metrics (ek key pe ops/sec), Redis --hotkeys, client side ginti -> had paar = copy / L1 me
  "L1 me purana score kitni der?"
   -> L1 TTL 1-2 sec -> utna purana chalta (score ke liye theek, paisa ke liye nahi)
```

---

## 10x SCALE — har dabba alag

```
Cache client  -> node juda / gaya -> consistent hashing + virtual nodes
Node          -> RAM bhari -> LRU + TTL · mara -> replica promote · data badha -> node jodo
hot key       -> copies + L1 local cache
expiry        -> mutex · soft-TTL · never-expire · pre-warm
DB            -> stale -> invalidate + TTL
AAGE:         write-back (write-heavy ho to) · multi-region cache

POOCHEGA: "How would you scale this to 10x?"      -> request ka raasta chalo
POOCHEGA: "What's the single point of failure?"   -> node (replica), cache client config (sab app me copy)
POOCHEGA: "How do you know it's working?"         -> HIT RATIO sabse zaroori (gira = kuch toota) · p99 · evictions · alert
```

---

## POOCHE TO (deep-dive)

```
API:      GET key · PUT key value ttl · DELETE key
          jaan-boojh ke chhoti — query nahi, sirf key se -> index / planner nahi -> isiliye tez

NODE KE ANDAR:  HashMap key -> value (O(1)) · LRU = HashMap + DLL · TTL lazy + active
                in-memory KV (Redis-jaisa), DB peeche source of truth
                <1ms -> RAM (disk nahi) · join nahi -> KV kaafi

KEY BAANTNA:    hash % N -> sab shift -> NAHI · CONSISTENT HASHING -> ring + clockwise + virtual nodes <- YAHI
                range -> hot range -> NAHI

TRADE-OFF:      speed vs consistency — cache AP ki taraf, eventual chalta (CAP ka spirit)
```

---

## AAKHRI DABBA + WRAP

```
App + L1 = nano-sec, hot key ka pehla ilaaj · Cache client = consistent hashing, key -> node
Node = HashMap + DLL (LRU) + TTL, primary + replica (alag AZ) · DB = source of truth, miss pe
```
```mermaid
flowchart TD
    n_App_L1_local_cache["App + L1 local cache"]
    n_DB["DB"]
    n_Cache_client["Cache client"]
    n_Node_A_replica["Node A + replica"]
    n_Node_B_replica["Node B + replica"]
    n_Node_C_replica["Node C + replica"]
    n_App_L1_local_cache --> n_DB
    n_App_L1_local_cache --> n_Cache_client
    n_Cache_client --> n_Node_A_replica
    n_Cache_client --> n_Node_B_replica
    n_Cache_client --> n_Node_C_replica
```
```
SINGLE node : HashMap + DLL (LRU) + TTL
DISTRIBUTE  : consistent hashing (shard) + replication (HA)
READ        : cache-aside (~90% hit, DB load 10x kam)
STALE       : TTL + explicit invalidation
STAMPEDE    : mutex / soft-TTL
HOT KEY     : copies / L1 local
```
```
BOL: "A distributed cache is an in-memory key-value store, sharded with consistent hashing and replicated for
      availability. Cache-aside with LRU and TTL is the production combo. The two hard parts are invalidation —
      TTL plus explicit deletes — and stampedes — a lock or early refresh. Hot keys get a local L1 cache and
      copies. It's a speed versus consistency trade-off, decided by the use case."
```

ARCHETYPE F · CONCEPTS: [caching](../../FOUNDATIONS/04_caching.md) · [sharding](../../FOUNDATIONS/06_database_sharding.md) · [replication](../../FOUNDATIONS/05_database_replication.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
