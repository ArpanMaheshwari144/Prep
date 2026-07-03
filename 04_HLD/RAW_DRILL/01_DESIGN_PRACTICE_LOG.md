# HLD DESIGN PRACTICE LOG (assembled 8-step designs — revise ke liye)

> Full-design practice (sheet ko design me "pour" karna). Har design 8-step me assembled.
> Revise: yeh dekho -> phir khud blank se try -> gaps sheet me daalo. (delivery/assemble skill.)

---

## DESIGN 1 — RATE LIMITER  (3-Jul, first full design)

```
   1. CLARIFY   : per-user limit (100/min), cross -> 429. NFR: LOW-LATENCY (critical), FAIL-OPEN.
   2. SCALE     : high-QPS (saari API traffic). har req = count read + increment (write).
   3. API       : existing GET/POST endpoints ke aage rate-limiter middleware.
   4. BOXES     : Client -> API GATEWAY (rate limiter) -> [Redis: count] -> allow? -> App -> DB
                                                          reject -> 429
   5. DATA      : Redis, per user -> {token-count, last-refill-time}. (in-memory: fast + atomic INCR + shared)
   6. DEEP-DIVE : TOKEN BUCKET -> bucket me tokens, req aaye token lo, khaali -> reject.
                  REFILL fixed-rate (1/sec) -> burst (idle-jama) + sustained dono handle.
   7. BOTTLENECK: multi-server -> CENTRALIZED Redis (warna limit toote). down -> REPLICA -> fail-open.
                  overload -> SHARDING (user/region se, NOT CDN).
   8. WRAP      : token-bucket + central Redis (replica'd + sharded) + gateway-placement + fail-open.

   GAPS jo mile (-> sheet me daale): NFR=latency(availability nahi), fail-open, "load-divide=sharding NOT CDN".
```

---

## DESIGN 2 — NOTIFICATION SYSTEM  (3-Jul, kaafi behtar)

```
   1. CLARIFY  : event -> notification (push/email/SMS). ★ PRIORITY tiers (OTP=critical/reliable/fast, promo=delay-OK).
   2. SCALE    : millions of events. write-heavy. spike (flash-sale) possible.
   3. API      : POST /notify {userId, type, channel, payload}. (internal — services call karte.)
   4. BOXES    : Service -> NOTIFICATION SERVICE -> FAN-OUT (recipients nikaalo) ->
                 KAFKA (partitioned queue) -> WORKERS (parallel) -> providers (FCM/SMS/email) -> user
   5. DATA     : notifications(id,user,type,status) + user-preferences(channel) + templates.
   6. DEEP-DIVE: ASYNC (user ko turant response, kaam queue me). FAN-OUT (1 event -> N messages).
                 DUPLICATE -> IDEMPOTENCY-key (at-least-once + idempotent worker). fail -> RETRY -> DLQ.
   7. BOTTLENECK: spike -> QUEUE absorb. scale -> PARTITIONS + more workers. provider-down -> retry/DLQ + circuit-breaker.
   8. WRAP     : async Kafka-queue + fan-out + parallel-workers + idempotency + priority-tiers + retry/DLQ.

   NAYA seekha: DLQ (dead-letter queue) = baar-baar fail message -> main queue se hata ke park -> investigate later.
                (fan-out = 1 event -> N messages | workers = N parallel bhejo.)
```

---

```
   ★ PATTERN dono me common (yaad rakh):
   - user ko TURANT response, bhaari kaam QUEUE + WORKER (async).
   - shared count/state -> CENTRALIZED store.
   - down -> replica/fail-open · overload -> shard/partition · duplicate -> idempotency · fail -> retry/DLQ.
   -> 8-step skeleton + ye reflexes = koi bhi design assemble ho jaata.
```
