# HLD — RAW DRILL REFLEX SHEET (design-less, rapid-fire)

> PRACTICE MODE (30-Jun): koi design nahi. SEEDHA tradeoff/concept ka kab/kyun/kaise reflex.
> ye answers HAR design me SAME aate -> isliye design ke bina drill. (detail = 04-09 foundation notes.)
> use: Claude sawaal pheke -> tu cold bole. ya khud cover karke recall kar.

---

## ★ MASTER REFLEX TABLE (need -> block -> kyun) ★

```
   NEED / SCENARIO                  ->  BLOCK                ->  KYUN (1 line)
   ───────────────────────────────────────────────────────────────────────────────────────────
   read-heavy (baar-baar padho)     ->  CACHE (Redis)        ->  RAM me data, disk se 100x tez
   reads phir bhi DB pe zyada       ->  READ REPLICA         ->  DB ki copies, reads baant do (read-scale)
   "abhi likha turant padha" stale  ->  READ-YOUR-OWN-WRITES ->  replication lag -> woh read master se
   WRITES bahut zyada / big data    ->  SHARDING (+key)      ->  data tukdon me alag DB, write/storage-scale
   instant SPIKE/burst absorb       ->  QUEUE (Kafka)        ->  requests HOLD/buffer, crash nahi (LB nahi!)
   traffic distribute               ->  LOAD BALANCER        ->  requests servers me BAANTO (hold nahi)
   availability / failover          ->  REPLICATION          ->  copy gira to dusri le le
   consistency-critical (paisa)     ->  SQL + CP             ->  ACID, galat data NEVER
   slow/async kaam, decouple        ->  QUEUE                ->  background me process
   concurrency (2 user, no double-X)->  LOCK / ATOMIC        ->  check+mark ek atomic step (race khatam)
   duplicate/retry (SAME user)      ->  IDEMPOTENCY KEY      ->  key se ek hi baar effect (paisa ek baar)
   abuse / too many req per user    ->  RATE LIMITER         ->  Redis count + token-bucket -> 429
   global static files slow         ->  CDN                  ->  edge server user ke paas (kam latency)
   server -> user real-time PUSH    ->  WEBSOCKET            ->  permanent 2-way pipe (live price/chat)
   slow query (full scan)           ->  DB INDEX (B-tree)    ->  sorted -> O(log n) search
   ───────────────────────────────────────────────────────────────────────────────────────────
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

## 9. WEBSOCKET vs KAFKA (real-time push — ★ confusion clear ★)
```
   Q: server ko user ko real-time data PUSH karna (live price/chat/notification), bina user maange -> ?
   A: WEBSOCKET.
   WEBSOCKET = server <-> USER(browser) ka PERMANENT 2-way pipe (khula rehta) -> server jab chahe PUSH kare.
       (normal HTTP: client maange tab milta, ek baar, band. websocket: connection khula -> server push.)
   KAFKA = backend SERVICES ke beech data move (internal pipe). user-browser tak NAHI jaata.
   ★ YAAD: server->user real-time PUSH = WEBSOCKET | backend services ke beech = KAFKA.

   bonus (WhatsApp 2 cheezein alag):
     - dono online live chat   -> WEBSOCKET (turant push)
     - net OFF, baad me mila    -> OFFLINE store-and-forward (msg DB/queue me save -> online aaye to deliver). alag concern.
```

## 10. DB INDEXING (slow query fast)
```
   Q: 10 crore rows, "WHERE email=x" slow (full scan) -> ?  A: INDEX on email.
   KAISE: DB index ke liye B-TREE banata (SORTED tree). search = root se -> chhota left, bada right traverse.
   SPEED: full-scan O(n) = 10 crore steps -> B-tree O(log n) = ~27 steps! (contrast bolna interview me.)
   TRADEOFF (index free nahi):
     - WRITES slow (har insert/update pe tree bhi update).
     - extra STORAGE (tree alag).
     -> HAR column pe nahi, SELECTIVE (jin pe queries chalti: email/id).
   YAAD: index = B-tree sorted -> O(log n). tradeoff = writes slow + storage. selective lagao.
```

## 11. IDEMPOTENCY KEY (duplicate/retry — same user)
```
   Q: user ne "Pay" 2 baar dabaya (ya retry) -> paisa 2 baar kata -> ?  A: IDEMPOTENCY KEY.
   har request ek UNIQUE key -> key request ke saath jaaye. server key check kare:
     - key "processing" (chal rahi)  -> duplicate -> reject/ignore.
     - key "DONE" (complete)         -> duplicate -> ERROR nahi, PEHLA SAME result laut do (consistent jawab).
   -> 10 click bhi -> effect ek hi baar.
   ★ YAAD: idempotency-key = SAME user duplicate/retry. (vs atomic-mark/lock = 2 ALAG user same resource race. DONO alag!)
```

## 12. CACHE CONSISTENCY (stale cache rokna)
```
   Q: data update hua, cache me purana pada -> read stale dega -> kaise sync rakhu?  A: CACHE-ASIDE.
   CACHE-ASIDE (common): write pe -> DB update + cache key DELETE (invalidate). agla read: miss -> DB se -> cache me daal.
     ★ DELETE karo, UPDATE nahi -> kyun: 2 thread update karein to galat order race. delete = next read latest laata, race-free.
   WRITE-THROUGH (alt): write pe cache AUR DB dono ek saath. hamesha sync, par har write cache ko bhi chhuye.
   YAAD: stale rokna -> cache-aside: DB update + cache DELETE -> next read repopulate. (delete > update.)
```

---
> aage aur reflexes add karte jaana (LB algos, microservices vs monolith, consistent-hashing) jaise drill hote.
