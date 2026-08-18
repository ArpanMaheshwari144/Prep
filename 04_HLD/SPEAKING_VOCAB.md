# HLD SPEAKING VOCAB — Arpan ka word-bank

> **Kyun ye file:** concept 100% aata (Hindi me round faad de). Sirf interview me English
> **word tongue pe** nahi aata -> us line pe freeze. Ye gap CHHOTA + FINITE hai — system-design ke
> technical word gine-chune. Ye file = wo word + EXACT line jaise interview me bolega.
>
> **KAISE use kare:** padho mat — **LOUD BOLO**. Har topic ka "one-breath line" 2-3 baar zabaan se
> nikaalo. Word tongue pe chadhega. (jaise DSA sheet revise — bas yaha bolke.)
>
> **Yaad rakh:** soch tere paas HAI. Ye sirf word-swap hai (bucket khatam -> "bucket is empty").
> "sunne me native" banna zaroori nahi — clear soch + sahi word kaafi. JP-Bangalore Hinglish-ok.

---

## 0. UNIVERSAL HLD VERBS (har design me kaam aate — connective tissue)

| bolna hai (Hindi) | English word | line |
|---|---|---|
| load sambhalna | **handle** | "the system should *handle* millions of requests per second" |
| bada karna | **scale** | "we *scale horizontally* by adding more servers" |
| baant dena | **distribute** | "requests are *distributed* across nodes" |
| pakka karna | **ensure** | "to *ensure* consistency, we ..." |
| kam karna (load/delay) | **reduce / offload** | "caching *reduces* load on the database" |
| fail hone pe bhi chale | **fault-tolerant / failover** | "if a node fails, we *failover* to a replica" |
| bottleneck / adchan | **bottleneck** | "the database becomes the *bottleneck* at scale" |
| trade-off | **trade-off** | "there's a *trade-off* between consistency and latency" |

---

## 1. RATE LIMITER (token bucket)

| tera Hindi | English word | line |
|---|---|---|
| bucket se token liya | **borrow / consume** | "each request *consumes* a token from the bucket" |
| bucket bharta rehta | **refill / replenish** | "the bucket *refills* at a steady rate, say 10 tokens/sec" |
| bucket ki size | **capacity** | "the bucket has a fixed *capacity*" |
| ek saath thode zyada allow | **burst** | "this allows short *bursts* up to the bucket size" |
| bucket khatam / saare token use | **empty / exhausted** | "once the bucket is *empty*..." |
| request rok do | **throttle** | "extra requests are *throttled*" |
| mana kar do | **reject** | "the request is *rejected* with a 429 Too Many Requests" |
| ek jaisi speed | **steady rate** | "it smooths traffic to a *steady rate*" |

**ONE BREATH:** *"A token bucket has a fixed capacity and refills at a steady rate. Each request consumes a token. If tokens are available it's allowed, otherwise it's throttled and rejected with a 429 — this handles short bursts while keeping a steady average rate."*

---

## 2. CACHING

| tera Hindi | English word | line |
|---|---|---|
| data mila cache me | **cache hit** | "if it's a *cache hit*, we return immediately" |
| cache me nahi mila | **cache miss** | "on a *cache miss*, we go to the database" |
| purana data hata do | **evict** | "the least recently used entry is *evicted*" |
| expire time | **TTL (time to live)** | "each entry has a *TTL* after which it expires" |
| purana/basi data | **stale** | "the cache may serve *stale* data for a short time" |
| cache saaf karna | **invalidate** | "when data changes, we *invalidate* the cache entry" |
| load ghatana | **offload** | "caching *offloads* read traffic from the DB" |

**ONE BREATH:** *"We add a cache in front of the database. On a cache hit we return fast; on a miss we read from the DB and populate the cache. Entries have a TTL and are evicted when full. On writes we invalidate the entry to avoid stale data."*

---

## 3. LOAD BALANCING

| tera Hindi | English word | line |
|---|---|---|
| load baant do | **distribute** | "the load balancer *distributes* requests across servers" |
| baari-baari | **round robin** | "a simple strategy is *round robin*" |
| server zinda hai? check | **health check** | "it does periodic *health checks* on each server" |
| kharab server hata do | **remove from pool** | "an unhealthy server is *removed from the pool*" |
| ek user ek server pe | **sticky session** | "*sticky sessions* pin a user to one server" |

**ONE BREATH:** *"A load balancer sits in front and distributes incoming requests across servers, using strategies like round robin. It runs health checks and removes unhealthy servers from the pool, so traffic only goes to healthy nodes."*

---

## 4. DATABASE — REPLICATION & SHARDING

| tera Hindi | English word | line |
|---|---|---|
| data ki copy | **replica** | "we keep read *replicas* of the database" |
| likhne wala main DB | **primary / leader** | "writes go to the *primary*, reads to the replicas" |
| copy update hone me delay | **replication lag** | "there can be some *replication lag*" |
| data tukdo me baant do | **shard / partition** | "we *shard* the data across multiple databases" |
| kis shard me jaye | **shard key** | "we pick a *shard key*, like user id" |
| ek shard pe zyada load | **hot partition** | "a bad shard key can cause a *hot partition*" |

**ONE BREATH:** *"We use a primary for writes and read replicas for reads to scale reads, accepting some replication lag. For write scale we shard the data across databases using a shard key like user id, being careful to avoid hot partitions."*

---

## 5. ASYNC / MESSAGE QUEUES

| tera Hindi | English word | line |
|---|---|---|
| kaam baad me karo | **asynchronous** | "we process it *asynchronously*" |
| beech me queue | **queue / broker** | "requests go into a *message queue* like Kafka" |
| daalne wala | **producer** | "the *producer* publishes the event" |
| uthane wala | **consumer** | "a *consumer* picks it up and processes it" |
| load ka jhatka jhelo | **buffer / absorb spikes** | "the queue *buffers* traffic and absorbs spikes" |
| ek hi baar effect ho | **idempotent** | "processing is *idempotent*, so retries are safe" |

**ONE BREATH:** *"Instead of doing it inline, we push the work to a message queue. The producer publishes an event and a consumer processes it asynchronously. This decouples the services and lets the queue absorb traffic spikes. We make processing idempotent so retries are safe."*

---

## 6. CONSISTENCY / CAP

| tera Hindi | English word | line |
|---|---|---|
| har jagah same data | **strong consistency** | "banking needs *strong consistency*" |
| thodi der me sab same | **eventual consistency** | "for feeds, *eventual consistency* is fine" |
| network toot gaya | **network partition** | "during a *network partition*, we must choose" |
| response time | **latency** | "this reduces *latency* for the user" |
| ek saath kitne handle | **throughput** | "it increases the system's *throughput*" |

**ONE BREATH:** *"By CAP, during a network partition we choose between consistency and availability. Payments need strong consistency, but for something like a news feed, eventual consistency is acceptable to keep latency low and availability high."*

---

## 7. RELIABILITY / FAILURE

| tera Hindi | English word | line |
|---|---|---|
| ek point fail = sab fail | **single point of failure (SPOF)** | "we remove any *single point of failure*" |
| backup pe switch | **failover** | "on failure we *failover* to a standby" |
| dobara koshish | **retry with backoff** | "the client *retries with exponential backoff*" |
| girta hua system bacha lo | **circuit breaker** | "a *circuit breaker* stops calling a failing service" |
| thoda-thoda kaam karta rahe | **graceful degradation** | "the system *degrades gracefully* instead of crashing" |

**ONE BREATH:** *"We avoid single points of failure by replicating components and using failover to standbys. Clients retry with backoff, and a circuit breaker protects against a failing downstream service, so the system degrades gracefully instead of going fully down."*

---

## HOW TO DRILL (roz 5 min)
1. Ek topic uthao -> **one-breath line LOUD bolo** 2-3 baar.
2. Phir file band karke wahi concept **apne words me English me bolo** (Hindi soch -> English word).
3. Atka? -> word table dekh lo -> dobara bolo. (jaise DSA nudge.)
4. Roz 1-2 topic. Ghis-ghis ke tongue pe chadhega. **Bolna hai, padhna nahi.**
