# HLD — RAW DRILL REFLEX SHEET (design-less, rapid-fire)

> PRACTICE MODE (30-Jun): koi design nahi. SEEDHA tradeoff/concept ka kab/kyun/kaise reflex.
> ye answers HAR design me SAME aate -> isliye design ke bina drill. (detail = 04-09 foundation notes.)
> use: Claude sawaal pheke -> tu cold bole. ya khud cover karke recall kar.

---

## ★ MASTER REFLEX TABLE (need -> block -> kyun) ★

```
   NEED / SCENARIO               ->  BLOCK            ->  KYUN (1 line)
   ──────────────────────────────────────────────────────────────────────────────
   read-heavy (baar-baar padho)  ->  CACHE (Redis)    ->  RAM me data, disk se 100x tez
   reads phir bhi DB pe zyada    ->  READ REPLICA     ->  DB ki copies, reads baant do (read-scale)
   WRITES bahut zyada / big data ->  SHARDING         ->  data tukdon me alag DB, write/storage-scale
   instant SPIKE/burst absorb    ->  QUEUE (Kafka)    ->  requests HOLD/buffer, crash nahi (LB nahi!)
   traffic distribute            ->  LOAD BALANCER    ->  requests servers me BAANTO (hold nahi)
   availability / failover       ->  REPLICATION      ->  copy gira to dusri le le
   consistency-critical (paisa)  ->  SQL + CP         ->  ACID, galat data NEVER
   slow/async kaam, decouple     ->  QUEUE            ->  background me process
   concurrency (no double-X)     ->  LOCK / ATOMIC    ->  check+mark ek atomic step (race khatam)
   ──────────────────────────────────────────────────────────────────────────────
   PRINCIPLE: need dekho -> block match. mismatch mat karo (spike pe replica NAHI -> queue).
```

---

## 1. CACHE (read-heavy)
```
   Q: reads bahut, DB slow -> ?            A: CACHE (Redis).
   KYUN: Redis = RAM (memory) -> read ~microsec. DB = disk -> ~millisec. 100x+ tez.
         baar-baar same data -> ek baar DB se, cache me rakho -> aage saare reads cache se.
```

## 2. READ REPLICA (read still high)
```
   Q: cache ke baad bhi reads single DB jhel nahi raha -> ?   A: READ REPLICAS.
   MASTER -> saare WRITES.  REPLICAS (copies) -> saare READS baant do.
   master apna data replicas ko bhejta (replication). read-scale + HA bonus.
```

## 3. SHARDING (write-heavy / storage)
```
   Q: WRITES bahut badh gaye, ek master jhel nahi raha -> ?   A: SHARDING.
   data ko tukdon me baant, har tukda ALAG DB (shard). e.g. A-M->DB1, N-Z->DB2. write-load divide.
   replica = read-scale | shard = write/storage-scale. (DONO ALAG!)

   ★ SHARD-KEY (interviewer yahi pakadta): 2 cheezein —
     1. QUERY-key (jis pe filter hota -> user_id/tweet_id)
     2. EVENLY-distributed (high variety) -> load saare shards me baante
     galat key (e.g. country=India) -> 90% ek shard pe -> HOT SHARD -> faayda khatam.
```

## 4. QUEUE vs LOAD BALANCER (★ spike confusion ★)
```
   Q: achanak SPIKE (IPL/flash-sale) 1 sec me lakhs req -> ?   A: QUEUE (Kafka).
   QUEUE -> requests HOLD/buffer (line/token). backend apni speed se uthaye. spike ABSORB.
   LB    -> requests servers me BAANTTA, buffer NAHI. load>capacity -> baant ke bhi crash.
   ★ YAAD: spike absorb -> QUEUE (rokna) | traffic distribute -> LB (baantna).
```

## 5. SQL vs NoSQL
```
   SQL  jab: ACID/transactions (paisa, booking) · RELATIONS/JOINS · fixed schema · consistency-critical.
   NoSQL jab: ACID critical nahi · flexible/key-value/document (posts, logs) · MASSIVE SCALE (horizontal easy).
   YAAD: SQL = consistency + relations | NoSQL = scale + flexible.
   JP-flavor: payment/trade/balance -> SQL | feed/logs/session/notification -> NoSQL.
```

## 2b. REPLICATION LAG (replica ka chhupa catch)
```
   Q: write master pe gaya, user ne TURANT refresh (read replica se) -> PURANA data dikha. kyun? fix?
   KYUN: master->replica copy INSTANT nahi -> thoda DELAY = REPLICATION LAG (millisec-sec).
         us gap me replica pe abhi purana data -> user ko purana mila.
   FIX: "READ YOUR OWN WRITES" -> jisne abhi likha, use thodi der MASTER se padhao (naya wahan hai),
        baaki sab replica se. (ya sync-replication, par slow.)
   YAAD: replica thoda purana ho sakta. "abhi likha turant padha" -> master se. (eventual-consistency catch.)
```

## 6. CAP THEOREM
```
   Partition (network toot) ke time C ya A — ek hi mil sakta:
   CP (Consistency)  -> paisa/bank/trade/booking. "sahi ya kuch nahi" (stale NEVER). band kar dena better.
   AP (Availability) -> social (Insta likes/feed). "chalu rehna zaroori, thoda purana data chalega".
   YAAD: money = CP | social = AP.
```

## 7. RATE LIMITER (abuse/DDoS rokna)
```
   Q: ek user/bot 1000 req/sec maar raha, baaki suffer -> ?  A: RATE LIMITER.
   per user+endpoint COUNT rakho -> threshold cross -> block (429). algos: token-bucket / fixed-window / sliding-window.
   TOKEN BUCKET (common): bucket me tokens, har req 1 token kharch, khaali->reject, fixed rate refill.
   ★ COUNT KAHAN: REDIS me -> fast (in-memory) + SHARED across servers (user kisi bhi server pe -> count ek hi jagah).
                  (local memory me rakha to har server alag count -> limit toot jaaye.)
   YAAD: rate-limit = per-user count in REDIS + token-bucket -> cross -> 429.
```

## 8. CDN (global static content)
```
   Q: global users, static files (img/video/css/js) door server se aane me slow -> ?  A: CDN.
   EDGE servers (duniya bhar me, user ke kareeb) pe static files ki copy. user -> nearest edge se mile.
   miss (edge pe nahi) -> ORIGIN server se laao -> edge pe cache -> user ko serve.
   KYUN fast: data kam distance travel -> kam latency.
   YAAD: CDN = STATIC files edge se (img/video/css/js). dynamic data nahi.
```

---
> aage aur reflexes add karte jaana (idempotency-vs-atomic, websocket, indexing) jaise drill hote.
