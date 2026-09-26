# HLD — DRILL (need -> block reflex + kyun/kaise Q&A)

> **NAV** — KYA: design ke bina concept drill. PART 1 = rapid-fire need -> block. PART 2 = "kyun / kaise" Q&A
> with ready ENGLISH line. Interview-din ki compact list = [MASTER](../00_MASTER_SHEET.md) section 4 ·
> A ya B = [02_TRADEOFFS](../02_TRADEOFFS.md) · detail = [FOUNDATIONS](../FOUNDATIONS)
> (26-Sep: 00_REFLEX_SHEET + 02_WHY_HOW_QA is ek file me mile. Kai topic dono part me hain —
>  PART 1 = yaad ka tez version, PART 2 = wahi topic "kyun" ke saath + English line.
>  Purana 8-step framework hataya — ab 7-step RAIL = MASTER section 3.)

---

# PART 1 — REFLEX (need -> block, rapid-fire)

> PRACTICE MODE (30-Jun): koi design nahi. SEEDHA tradeoff/concept ka kab/kyun/kaise reflex.
> ye answers HAR design me SAME aate -> isliye design ke bina drill. (detail = FOUNDATIONS/01..18.)
> use: Claude sawaal pheke -> tu recall karke bole. ya khud cover karke recall kar.

---

## ★ MASTER REFLEX TABLE (need -> block -> kyun) ★

```
   #   NEED / SCENARIO               ->  BLOCK                ->  KYUN (1 line)
   ───────────────────────────────────────────────────────────────────────────────────────────
    1. read-heavy (baar-baar padho)  ->  CACHE (Redis)        ->  RAM me data, disk se 100x tez
    2. reads phir bhi DB pe zyada    ->  READ REPLICA         ->  DB ki copies, reads baant do (read-scale)
    3. "abhi likha turant padha"stale->  READ-YOUR-OWN-WRITES ->  replication lag -> woh read master se
    4. WRITES bahut / big data       ->  SHARDING (+key)      ->  data tukdon me alag DB, write/storage-scale
    5. instant SPIKE/burst absorb    ->  QUEUE (Kafka)        ->  requests HOLD/buffer, crash nahi (LB nahi!)
    6. traffic distribute            ->  LOAD BALANCER        ->  requests servers me BAANTO (hold nahi)
    7. availability / failover       ->  REPLICATION          ->  copy gira to dusri le le
    8. consistency-critical (paisa)  ->  SQL + CP             ->  ACID, galat data NEVER
    9. slow/async kaam, decouple     ->  QUEUE                ->  background me process
   10. concurrency (2 user, no dbl-X)->  LOCK / ATOMIC        ->  check+mark ek atomic step (race khatam)
   11. duplicate/retry (SAME user)   ->  IDEMPOTENCY KEY      ->  key se ek hi baar effect (paisa ek baar)
   12. abuse / too many req per user ->  RATE LIMITER         ->  Redis count + token-bucket -> 429
   13. global static files slow      ->  CDN                  ->  edge server user ke paas (kam latency)
   14. server -> user real-time PUSH ->  WEBSOCKET            ->  permanent 2-way pipe (live price/chat)
   15. slow query (full scan)        ->  DB INDEX (B-tree)    ->  sorted -> O(log n) search
   16. data update -> cache stale    ->  CACHE-ASIDE          ->  DB update + cache DELETE (not update) -> repopulate
   17. request kis server pe bheje   ->  LB ALGO              ->  round-robin/least-conn/IP-hash(session-sticky)
   18. bada app, scale + isolation   ->  MICROSERVICES        ->  per-service scale+deploy, ek down baaki chalu
   19. microservices ka single dwaza ->  API GATEWAY          ->  routing+auth+rate-limit+logging ek jagah
   20. login state (multi-server)    ->  JWT (stateless)      ->  signed token self-proving, no store (vs Session=Redis)
   21. cache FULL -> kya hataao      ->  EVICTION(LRU/LFU/TTL)->  LRU=time(last-use) · LFU=count · TTL=auto-expire
   22. badi list -> page-by-page     ->  PAGINATION (cursor)  ->  offset=deep-slow(skip) | cursor="last id ke baad"=fast
   23. server down -> LB ko pata     ->  HEALTH-CHECK/HEARTBT ->  LB /health ping+threshold(pull) | server "alive"(push)
   24. dead service hammer / cascade ->  CIRCUIT BREAKER      ->  OPEN=fail-fast / HALF-OPEN=test / CLOSED=normal
   25. bade files (img/video/pdf)    ->  BLOB / S3            ->  file S3 me, DB me sirf URL. pre-signed URL+CDN
   26. har req nayi DB conn mehngi   ->  CONNECTION POOL      ->  ready conn reuse (borrow/use/return). size-fix->exhaust
   27. multi-service transaction     ->  SAGA                 ->  local commits chain -> fail -> COMPENSATING (ulta)
   28. message kitni baar pahunche   ->  DELIVERY GUARANTEE   ->  at-least-once + IDEMPOTENCY = exactly-once
   29. "1M user -> server/storage?"  ->  CAPACITY ESTIMATION  ->  QPS=req/day÷10^5 · peak×2-3 · storage=req×size×(yr×400)
   30. read-heavy, JOIN slow         ->  DENORMALIZATION      ->  data jod ke ek jagah (duplicate) -> no JOIN, read fast
   31. frontend->API alag origin     ->  CORS                 ->  server Access-Control-Allow-Origin header (specific origin)
   ───────────────────────────────────────────────────────────────────────────────────────────
   PRINCIPLE: need dekho -> block match. mismatch mat karo (spike pe replica NAHI -> queue).
```

---

##  ONE-LINER DECISION SHORTCUTS (X vs Y -> kab-kaunsa · flash-reflex)

```
    1. SQL vs NoSQL          :  ACID/consistency/relations -> SQL   |  flexible/scale -> NoSQL
    2. CP vs AP              :  money/banking -> CP                 |  social/feed -> AP
    3. LRU vs LFU            :  time (last-use) -> LRU              |  count (frequency) -> LFU
    4. OFFSET vs CURSOR      :  skip-N / deep-slow -> OFFSET        |  last-id / fast (badi list) -> CURSOR
    5. Cache-aside vs W-through: DB update + cache DELETE -> ASIDE  |  cache+DB saath update -> WRITE-THROUGH
    6. Session vs JWT        :  server-state / Redis-lookup -> SESSION | stateless self-proving token -> JWT
    7. Monolith vs MS        :  ek deploy / chhota -> MONOLITH      |  alag services / scale+isolation -> MS
    8. Normalize vs Denorm   :  alag-tables + JOIN / write-friendly -> NORMALIZE | merge+duplicate / read-fast -> DENORM
    9. Replica vs Shard      :  read-heavy -> REPLICA               |  write-heavy -> SHARD
   10. Queue vs LB           :  spike-absorb -> QUEUE               |  traffic-distribute -> LB
   11. WebSocket vs Kafka    :  server->USER live-push -> WEBSOCKET |  backend-services (pub-sub) -> KAFKA
   12. Lock/Atomic vs Idempotency: 2-user same-resource race -> LOCK/ATOMIC | same-user duplicate/retry -> IDEMPOTENCY
```

---

## ★ DESIGN-TIME DECISION REFLEXES (assemble ke waqt — gaps se bane, 3-Jul rate-limiter drill)

```
   in decisions pe atka tha -> ab reflex:

   A. NFR kaunsa critical?      -> component HAR request pe baithta (middleware) -> LATENCY sabse critical -> in-memory (Redis).
   B. component DOWN -> allow ya block?  -> FAIL-OPEN (allow) default -> saare legit block karna >> thodi der abuse.
                                           FAIL-CLOSED (block) sirf jab security critical (payment/auth).
   C. shared state, MULTIPLE servers?    -> CENTRALIZED store (ek shared Redis) -> per-server count = limit TOOTE (S1+S3 alag).
   D. central store DOWN?                -> REPLICA (HA/failover) -> woh bhi gaya -> FAIL-OPEN.
   E. central store OVERLOAD (scale)?    -> SHARDING (user_id/region se baant) -> NOT CDN! (CDN=static files only, counters nahi.)
   F. counter / rate / session?          -> REDIS (in-memory fast + atomic INCR + shared + TTL auto-expire).

   ★ 2 BADE TRAP (aaj galti hui):
   - "load divide karna" = SHARDING, na ki CDN. (CDN sirf static img/video/css.)
   - down-case default = FAIL-OPEN (allow), na ki "sab block -> abuse rokega kaun".
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

## 13. LOAD BALANCER ALGORITHMS (request kis server pe)
```
   Q: LB ke peeche S1/S2/S3 -> request kis pe bheje, kaise decide?
   ROUND-ROBIN      : baari-baari S1,S2,S3,S1... -> sabko barabar. (servers EQUAL capacity tab.)
   WEIGHTED RR      : powerful server ko ZYADA (weight). (bada server -> 2x requests.)
   LEAST-CONNECTIONS: jis pe abhi sabse KAM active conn -> usko. (requests uneven-length ho tab best.)
   IP-HASH          : same user(IP) -> HAMESHA same server. ★ SESSION STICKINESS (session data local us server pe).
   YAAD: round-robin(equal) · weighted(capacity) · least-conn(uneven) · IP-hash(session sticky).
```

## 14. MONOLITH vs MICROSERVICES
```
   MONOLITH: saare features ek codebase+deploy. develop/debug EASY, par scale HARD + ek part crash -> POORA down.
             -> chhota app / kam users.
   MICROSERVICES: har feature alag service+deploy. debug MUSHKIL (distributed), par:
             - scale EASY: sirf JIS service pe load usko alag scale (payment-svc 10 instance, baaki 2) -> resource bache.
             - FAULT ISOLATION: ek service down -> baaki chalu.
             - independent DEPLOY: alag team alag deploy, poora app redeploy nahi.
             -> bada app / millions.
   ★ CAUTION: chhote app pe MS = OVER-ENGINEER (network calls + distributed-debug + cross-service consistency).
              "default MONOLITH se start -> scale aaye to MS me todo."
   YAAD: monolith=simple/chhota | MS=scale+fault-isolation+independent-deploy/bada. galti = chhote pe MS.
```

## 15. API GATEWAY (microservices ka single darwaza)
```
   Q: 10 microservices -> client har service ka address + auth/rate-limit har jagah? -> ?  A: API GATEWAY.
   = microservices ke saamne SINGLE entry-point. client sirf gateway se baat kare, internal services ka pata nahi.
   KAAM:
     1. ROUTING        -> request sahi service pe bheje. (core)
     2. AUTH           -> token/login check EK jagah (har service me dobara nahi). ★ sabse bada fayda.
     3. RATE-LIMIT     -> central throttling.
     4. LOGGING/MONITOR-> saare requests ek jagah track.
     5. (bonus) AGGREGATION -> 2-3 services ka data jod ke ek response.
   YAAD: API Gateway = MS ka single entry-point -> cross-cutting (routing+auth+rate-limit+logging) ek jagah centralize.
```

## 16. SESSION vs JWT (login state, multi-server)
```
   Q: login ke baad user ko "yaad" kaise rakhe? LB ke peeche 10 server -> agla request dusre server pe gaya to?
   MODEL A — SESSION + REDIS (stateful):
     login -> session Redis me, client ko SESSION-ID. har request -> server Redis lookup "id valid?" -> haan.
     10 server sab same Redis check. COST: har request pe Redis lookup.
   MODEL B — JWT (stateless): ★
     login -> JWT token jo KHUD SIGNED (secret-key signature). token me user-info + signature -> self-proving.
     har request -> server sirf SIGNATURE verify (secret-key se) -> NO store, NO Redis. sab server same secret -> koi bhi verify kare.
   ANALOGY (Arpan's): JWT = ID-card with office STAMP -> guard sirf stamp dekhe -> kahi bhi ghoomo (no reception call).
                      Session = visitor pass -> guard har baar reception ko CALL kare (Redis lookup).
   YAAD: Session = Redis lookup per req (stateful) | JWT = signature verify, no store (STATELESS, scale-friendly).
```

## 17. CAPACITY ESTIMATION (back-of-envelope — calculator NAHI)
```
   4 cheez estimate: QPS · Storage · Bandwidth · Cache.

   ★★ MENTAL-MATH SHORTCUT (powers of 10 -> exponents ADD karo, no calculator) ★★
   YAAD karo:
     1 din ≈ 10^5 sec   (86,400 ≈ 1,00,000)         <- sabse zaroori
     1 saal ≈ 400 din   (365 round-up, easy multiply)
     K=10^3 · M=10^6 · B=10^9 · T=10^12
     1 char=1 byte · KB=10^3 B · MB=10^6 · GB=10^9 · TB=10^12

   FORMULAS:
     total req/day = DAU × req-per-user
     AVG QPS  = (req/day) ÷ 10^5          [10 ke power minus 5 -> exponent se seedha]
     PEAK QPS = avg × 2 se 3
     STORAGE  = (req/day) × record-size × (saal × 400)
     CACHE    = hot 20% (80-20 rule)      BANDWIDTH = QPS × response-size

   TRICK (multiply bina calculator): sab ko a×10^n likho -> numbers multiply, exponent ADD.
     e.g. 20M × 2KB = (2×10^7) × (2×10^3) = 4×10^10 bytes = 40 GB.   (10^9 B=1GB -> 10^10=10GB -> ×4=40GB)
     e.g. 40GB/day × 3yr = 40 × (3×400) = 40 × 1200 = 48,000 GB ≈ ~48 TB.  (round -> fast)

   WORKED (5M DAU, 4 req/user, 2KB, 3yr): total=20M/day · avgQPS=200 · peak=400-600 · storage≈44-48TB.
   ★ interview me EXACT nahi -> rough number + method dikhao. round aggressively.
```

## 18. CACHE EVICTION (cache full -> kya hataao)
```
   Q: Redis full -> naya data ke liye purana kaunsa hataao?  A: eviction algo.
   LRU (Least Recently Used) -> TIME based: jo LONG-TIME se use nahi hua (last-access purana) -> evict. ★ common.
   LFU (Least Frequently Used)-> COUNT based: jo sabse KAM BAAR use hua -> evict.
   TTL (Time To Live)         -> har entry pe expiry -> time khatam pe APNE AAP delete (full ho ya na ho). fresh bhi rakhe.
   example: A=100 baar use par last 1hr pehle · B=2 baar use par last 1sec pehle -> LRU hataye A (time), LFU hataye B (count).
   YAAD: LRU=TIME(last kab) · LFU=COUNT(kitni baar) · TTL=auto-expire. aksar TTL + LRU saath.
```

## 19. PAGINATION (badi list -> page-by-page)
```
   Q: feed me 1 crore items -> ek saath nahi bhej sakte -> ?  A: pagination (page-by-page, e.g. 20/page).
   OFFSET: "LIMIT 20 OFFSET N" -> simple. DIKKAT: deep page (OFFSET 10 lakh) -> DB pehle N rows SKIP kare -> SLOW.
           + beech me naya item -> items shift/duplicate.
   CURSOR (better): "last dekhe item ke BAAD ke 20" -> WHERE id > last_seen_id LIMIT 20.
           -> DB INDEX se SEEDHA jump (koi skip nahi) -> deep page bhi FAST + stable. infinite-scroll isi se.
   YAAD: OFFSET=skip N (deep slow) | CURSOR="last id ke baad" (index jump, fast). badi list/infinite-scroll -> CURSOR.
```

## 20. HEALTH-CHECK / HEARTBEAT (server down -> LB ko kaise pata)
```
   Q: LB ke peeche server crash -> LB ko kaise pata (taaki us pe request na bheje)?
   HEALTH CHECK (LB PULL): LB har few-sec server ko "GET /health" ping kare. 200 OK -> healthy. fail count++.
     ★ THRESHOLD: 1 fail pe nahi (blip) -> lagataar 2-3 fail -> UNHEALTHY -> rotation se HATAO.
     recover: LB check karta rahe -> 2-3 success wapas -> rotation me ADD. (down ko mauka.)
   HEARTBEAT (server PUSH): server khud "main alive" signal bheje LB/registry ko. signal band -> dead.
   YAAD: HEALTH-CHECK = LB PULL (/health ping, threshold, recover) | HEARTBEAT = server PUSH (alive signal).
```

## 21. CIRCUIT BREAKER (cascading failure roke — resilience)
```
   Q: A -> B call, B down. A baar-baar B call kare -> har call timeout tak ruke -> A ke threads bhare -> A bhi choke
      = CASCADING FAILURE. -> A ko dead B hammer karne se kaise roke?  A: CIRCUIT BREAKER (3 states).
   CLOSED    = normal, sab call ja rahi (server theek).
   OPEN      = server fail -> call BLOCK -> turant FAIL/fallback (fail FAST, no timeout-wait -> threads bache).
   HALF-OPEN = thodi (1-2) test call bhej -> theek? -> CLOSED | phir fail? -> OPEN. (recover ka mauka.)
   trip OPEN kab: fails threshold cross (e.g. last 10 me 50% fail).
   naam-trick (bijli): CLOSED=juda=current behe=normal · OPEN=toota=current band=block.
   YAAD: CB = CLOSED(normal)/OPEN(block+fail-fast)/HALF-OPEN(test). dead service hammer nahi -> cascading failure roke.
```

## 22. BLOB / OBJECT STORAGE (bade files kahan)
```
   Q: users bade files (img/video/pdf, MBs-GBs) upload karte -> DB me rakhu?  A: NAHI -> OBJECT STORAGE (S3).
   files -> S3 (object storage) me. DB me sirf URL/reference.
   KYUN NOT DB: file GB tak -> DB load/space badhe + server bottleneck (poori video server se laana feasible nahi).
   UPLOAD/DOWNLOAD = PRE-SIGNED URL: client SEEDHA S3 se (server ke through nahi) -> server bandwidth/threads bache.
   + CDN: S3 files edge se serve -> global fast.
   YAAD: bade files -> S3 + DB me URL. flow = pre-signed URL (direct S3, server bypass) + CDN.
```

## 23. CONNECTION POOLING (DB connection reuse)
```
   Q: har request pe naya DB connection (open->query->close) -> mehnga kyun? fix?  A: CONNECTION POOL (HikariCP).
   NAYA connection mehnga: TCP handshake + auth + setup (~10-100ms) har baar -> overhead + DB churn se choke.
   POOL: N connections pehle se OPEN+ready. request BORROW -> use -> RETURN (band nahi, reuse). no re-handshake -> fast.
   ★ EXHAUSTION (Arpan's real bug): pool size FIXED (e.g.10). sab borrow + return nahi (slow query pakde rahe) ->
     pool khali nahi -> naye request WAIT/fail. (uska "HikariCP exhaustion, 95% email fail, blocking batch query" bug.)
   YAAD: pool = ready connections reuse (borrow/use/return) -> handshake bacha. size fixed -> hold-too-long -> EXHAUSTION.
```

## 24. SAGA (distributed transaction — multi-service)
```
   Q: order+payment+inventory teeno alag DB. @Transactional ek DB pe -> multi-DB me kaise atomic? payment fail to?
   A: SAGA = local transactions ki CHAIN. har service apne DB me LOCAL commit.
   fail -> pichhle committed steps DB-rollback NAHI ho sakte (already commit) -> COMPENSATING (ULTA) action chalao:
     payment fail -> "cancel order" (create ka ulta) + "restore inventory" (ghatane ka ulta) -> undo.
   2 types: CHOREOGRAPHY (har service event sun ke khud next/compensate) · ORCHESTRATION (central orchestrator direct kare).
   YAAD: SAGA = multi-service distributed txn. local commits chain -> fail -> COMPENSATING ulta-actions (rollback nahi). (@Transactional=ek DB.)
```

## 25. MESSAGE DELIVERY GUARANTEE (queue/Kafka — kitni baar pahunche)   CLEARED 2-Jul
```
   AT-MOST-once : bhej diya, bas -> LOST ho sakta (fire-forget). fast, unreliable. (kam use)
   AT-LEAST-once: PAKKA pahunche, par DUPLICATE ho sakta (ack na mila -> dobara bhej). ★ COMMON.
   EXACTLY-once : na lost na duplicate, theek 1 baar. HARD + mehnga.
   ★ practical: AT-LEAST-once + IDEMPOTENCY-key (duplicate ignore) = effectively EXACTLY-once. (industry standard.)
   YAAD: at-most(lost) · at-least(duplicate, common) · exactly(hard). real exactly = at-least + idempotent consumer.
```

---

## 26. DENORMALIZATION (read-heavy -> duplicate, no JOIN)
```
   Q: normalized me alag tables -> read pe JOIN mehnga (read-heavy me slow) -> reads fast kaise?  A: DENORMALIZE.
   NORMALIZE = alag dabbe, no-repeat, JOIN pe padho (write-friendly). = ingredients alag, har baar jodo.
   DENORMALIZE = sab jod ke EK table/row (duplicate chalega) -> read fast, no JOIN. = ready-made THALI.
   TRADE-OFF: read FAST | par data REPEAT (user_name 10 order me 10 baar) -> jagah zyada + naam badle to 10 jagah update (write mushkil + inconsistency risk).
   YAAD: read-heavy + JOIN slow -> denormalize (duplication accept for read-speed). NoSQL me common.
```

## 27. CORS (cross-origin browser block)
```
   Q: frontend(myapp.com) -> API(api.myapp.com, alag origin) call -> browser BLOCK, CORS error. kyun? fix?
   KYUN: browser ki SAME-ORIGIN POLICY -> alag origin ki request default block (security).
   FIX: backend response me header -> Access-Control-Allow-Origin: https://myapp.com -> browser permission dekh ke allow.
   ★ REFINEMENT: "*" (koi bhi origin) PROD me RISKY -> SPECIFIC origin whitelist karo.
                 (credentials/cookies ke saath "*" chalta hi nahi -> specific dena padta.)
   precision: ye BROWSER enforce karta (server-to-server/Postman me CORS nahi). header server response me, browser check.
   YAAD: cross-origin -> browser block (same-origin policy) -> server Access-Control-Allow-Origin header (specific, "*" nahi prod me).
```

---

# PART 2 — KYUN / KAISE Q&A (reasoning + English interview line)

> JP follow-up style: "kyu? kaise? kab?" reasoning questions. Jaise-jaise karenge, pattern jama hoga.
> Format: Q → crisp answer (Arpan-language) + interview-line (English). Arpan ne mostly khud diye (3-Jul).

---

## 3-Jul batch

**Q1. CDN STATIC files ke liye — DYNAMIC data (live balance) CDN pe kyu nahi?**
```
   dynamic data HAR USER ka ALAG + constantly BADALTA. CDN edge pe EK copy cache karke SABKO deta →
   dynamic cache karo to user ko STALE/GALAT/kisi-aur-ka data mile. static = sabke liye same + badalta nahi → safe.
   interview: "CDN caches one edge copy served to everyone — fine for static (same for all, rarely changes);
   dynamic/personalized would serve stale/wrong data, so it stays on origin."
```

**Q2. Rate-limiter API-GATEWAY pe kyu (app-server pe nahi)?**
```
   gateway = FRONT gate → invalid request EARLY reject → app-server ka resource waste na ho.
   (Arpan analogy: chor andar aa gaya, alarm baad me baja = bekaar. rok DARWAZE pe.)
   interview: "Rate limiter sits at the entry so bad requests are rejected early, before consuming app-server resources."
```

**Q3. Cache-aside: update pe cache UPDATE nahi, DELETE kyu?**
```
   failed/out-of-order cache-write galat data chhod sakta. DELETE → next read DB (source of truth) se repopulate → safe.
   interview: "On write I invalidate (delete) the cache — a failed or racy cache-write could leave wrong data; delete lets
   the next read repopulate from the DB."
```

**Q4. Spike pe QUEUE, LB kyu nahi?**
```
   LB load DISTRIBUTE karta, BUFFER nahi → load > capacity → phir bhi crash. QUEUE spike ABSORB karta (hold → one-by-one process).
   interview: "LB distributes but doesn't buffer; if load exceeds capacity it still fails. A queue absorbs the spike and the
   backend consumes at its own pace."
```

**Q5. Read-replica READ scale deta, WRITE scale kyu nahi?**
```
   write har replica pe apply + propagate karna padta → write-volume ghatata nahi. write scale = SHARD (data baant, har shard apne writes).
   interview: "Replicas scale reads, not writes — every write still hits master + all replicas. To scale writes you shard."
```

**Q6. SQL ACID deta, NoSQL scale ke liye kya TRADE karta?**
```
   SQL = ACID + consistency + joins, par scale mushkil. NoSQL TRADES strong-consistency (→ eventual) + joins FOR horizontal-scale + flexible-schema.
   interview: "NoSQL trades strong consistency and joins for easy horizontal scale and flexible schema. Pick by need."
```

**Q7. Idempotency SAME-user duplicate rokta — 2 ALAG user same-seat race pe kyu FAIL, kya chahiye?**
```
   2 alag user = 2 alag KEY → idempotency dono ko "alag legit" samajh ke PASS → double-book. wahan ATOMIC (conditional-update/lock) → sirf EK jeete.
   interview: "Idempotency dedupes one user's retries via a key; two different users have different keys → both pass → double-book.
   A race between different users needs an atomic check-and-set/lock so only one succeeds."
```

**Q8. CAP: paisa/booking me CP kyu (availability chhoda)?**
```
   PARTITION pe: stale/galat balance DIKHAO (available) ya REJECT karo. paisa me galat-balance/double-spend = catastrophe → REJECT > wrong. "sahi ya kuch nahi."
   interview: "During a partition, money chooses consistency — better to reject than show a stale balance or allow a double-spend.
   Brief unavailability beats being wrong about money." (note: OTP/message-timing = reliability, CAP-core NAHI.)
```

**Q9. Health-check: 1 fail pe down kyu nahi, 2-3 threshold kyu?**
```
   1 fail = transient blip ho sakta (network glitch / dropped response), real outage nahi. N CONSECUTIVE fail → dead, false-positive se bachne.
   (Arpan analogy: ek scratch pe shoe nahi fenkte.)
   interview: "One failed check could be a transient blip, not a real outage — so we need N consecutive failures before marking dead."
```

**Q10. Circuit-breaker OPEN me FAIL-FAST kyu (wait karne se behtar)?**
```
   OPEN me dead/slow service ko call → har call TIMEOUT tak HANG → threads bhar jaati → tera service bhi choke (cascade).
   fail-FAST = turant error/fallback → threads free → no cascade.
   interview: "When open, calling the failing service hangs every call until timeout, tying up threads until your service chokes.
   Failing fast returns an instant error/fallback, frees resources, stops the cascade."
```

**Q11. Shard-KEY galat (country, 90% India) → dikkat? achhi key kaise?**
```
   skewed key (country) → 90% users EK shard → HOT SHARD (overload), baaki khaali → benefit khatam.
   good key = (1) QUERY-key (queries me condition, e.g. userId) (2) EVENLY-distributed/high-cardinality. userId dono; country #2 fail.
   interview: "Shard on the query key so lookups hit one shard — but it must be high-cardinality and evenly distributed.
   A skewed key like country puts 90% on one shard (hot shard), killing the benefit."
```

**Q12. at-least-once me DUPLICATE → kaise handle?**
```
   at-least-once = pakka pahunche, par ack-lost pe resend → duplicate. IDEMPOTENCY-key → consumer check kare "processed?" → skip → effect once.
   interview: "The consumer uses an idempotency key — checks if the message-id was already processed and skips it — so the effect happens once."
```

**Q13. SAGA me rollback nahi COMPENSATING kyu?**
```
   normal rollback (@Transactional) sirf EK DB ka UNCOMMITTED work undo karta. SAGA me har service ALREADY COMMIT kar chuki apne DB me →
   committed cheez rollback nahi hoti → COMPENSATING action (naya reverse op, e.g. "cancel order") se undo.
   interview: "A rollback only undoes uncommitted work in one DB. In a saga each service already committed locally, so you run a
   compensating action — an explicit reverse operation — to undo the committed effect."
```

**Q14. DB index O(log n) fast — har column pe kyu nahi?**
```
   index = B-tree (sorted, O(log n) read). PAR har WRITE pe tree REORDER karna padta → writes SLOW + extra storage.
   isliye sirf query-condition wale columns pe index. (EXPLAIN se dekho query index use kar rahi ya nahi.)
   interview: "An index is a B-tree giving O(log n) reads, but every write must update the tree — so indexes slow writes and
   add storage. Index only columns used in query filters, not everything."
```

**Q15. Read-replica: abhi likha, turant padha → PURANA mila. kyu? fix?**
```
   kyu: write MASTER pe gaya, replica tak abhi propagate nahi (replication lag) → replica se padha → stale.
   fix = READ-YOUR-OWN-WRITES: jisne abhi likha, use thodi der REPLICA ki jagah MASTER se padhao (fresh wahi hai),
   replica catch-up ho jaye phir normal (replica se).
   interview: "That's replication lag — the write hasn't propagated to the replica yet. Fix: for a user who just wrote,
   read from the master briefly (read-your-own-writes) until the replica catches up."
```

**Q16. Har request pe naya DB connection mehnga kyu? behtar?**
```
   naya connection = har baar TCP handshake + auth + setup (~10-100ms) → overhead. CONNECTION POOL (HikariCP):
   set of ready connections → borrow → use → RETURN (reuse), naya nahi banate.
   (Arpan analogy: hotel me set-of-waiters sab guests handle karte, har guest pe naya waiter nahi.)
   interview: "Creating a connection each time needs a TCP handshake + auth + setup. A pool keeps ready connections you
   borrow, use, and return — avoiding repeated setup."
```

**Q17. Bade files (video/image) DB me store karein? nahi to kahan?**
```
   NAHI — S3 (object storage) me, DB me sirf URL. big files DB me = load/space + server-bottleneck (video server se serve = heavy load).
   client SEEDHA S3 se (server bypass) = PRE-SIGNED URL (server temporary signed-URL deta, client us se direct S3 up/download). + CDN for speed.
   interview: "Store large files in object storage (S3) with only the URL in the DB. Uploads/downloads use a pre-signed URL so
   the client talks directly to S3, keeping the file off the server. Serve via CDN for speed."
```

**Q18. Live price → server user ko real-time PUSH. WebSocket ya Kafka?**
```
   WEBSOCKET — server↔user ka permanent 2-way pipe, server jab chahe turant PUSH kare (client maange ya na maange).
   Kafka NAHI kyu: Kafka = backend SERVICES ke beech (internal pub-sub), user ke browser tak nahi jaata.
   interview: "Use a WebSocket — it's a persistent connection so the server can push live updates instantly. Kafka is for
   backend service-to-service messaging, not pushing to a user's browser."
```

**Q19. Cache FULL → kaunsa purana hataao? kaise decide?**
```
   EVICTION algo: LRU = TIME (jo LONG-time se use nahi hua, last-access purana) · LFU = COUNT (jo sabse KAM baar use hua) ·
   TTL = auto-expire (time khatam pe apne aap delete). LRU sabse common.
   interview: "Use an eviction policy — LRU evicts the least recently used (time), LFU the least frequently used (count),
   TTL auto-expires by time. LRU is most common."
```

**Q20. Feed crore items → page-by-page. OFFSET deep-page pe dikkat?**
```
   OFFSET (LIMIT 20 OFFSET 100000) → DB pehle 1 lakh rows SKIP karta → deep page jitna gehra, utna SLOW.
   CURSOR pagination = "last-id ke BAAD ke 20" (WHERE id > last_id) → index se seedha JUMP, no skip → fast + stable. infinite-scroll isi se.
   interview: "Offset scans and skips all preceding rows, so deep pages get slow. Cursor pagination fetches rows after the last
   seen id using the index — a direct jump, fast and stable."
```

**Q21. Denormalization trade-off (nuksaan)?**
```
   read FAST (no JOIN, data pehle se jod ke ek jagah). par data DUPLICATE (user_name har order me) →
   naam badle to SAARI copies update → WRITE hard + inconsistency risk + storage zyada. "read-fast, write-hard."
   interview: "Denormalization speeds reads by avoiding joins, but the duplicated data means an update must touch many rows —
   harder writes, more storage, and consistency risk."
```

**Q22. Kab monolith, kab MS? chhote app pe MS kyu galat?**
```
   chhota app → MONOLITH (simple). bada/growing → MICROSERVICES (fault-isolation: ek service down → baaki chalu, no SPOF).
   (Arpan analogy: Gmail down par Google chalta.) chhote app pe MS GALAT = network-calls + distributed-debug +
   cross-service-consistency complexity = OVER-ENGINEERING. "monolith se start, scale aaye to MS me todo."
   interview: "Small app → monolith for simplicity; large/growing → microservices for independent scale and fault isolation.
   On a small app, MS adds network calls, distributed debugging, and cross-service consistency overhead — over-engineering."
```

**Q23. Single entry-point (routing + auth + rate-limit) — kya cheez?**
```
   API GATEWAY. ROUTING: request ko sahi service pe bheje (client ko internal addresses yaad nahi rakhne).
   ★ + AUTH + RATE-LIMIT + LOGGING gateway pe EK jagah (har service me dohrana nahi) = cross-cutting concerns centralize.
   interview: "An API Gateway is a single entry point — it routes to the right service and centralizes cross-cutting concerns
   like auth, rate-limiting, and logging, so each service doesn't repeat them."
```

**Q24. LB request kis server pe bheje? algorithms? IP-hash kab?**
```
   ROUND-ROBIN (baari-baari s1,s2,s3) · WEIGHTED-RR (bade/strong server ko zyada) · LEAST-CONNECTIONS (jis pe kam active conn) ·
   IP-HASH (same user → HAMESHA same server). IP-hash kab: SESSION-STICKINESS — session us server pe local hai, warna doosre server
   pe jaaye → session lost → re-login.
   interview: "Round-robin, weighted round-robin, least-connections, or IP-hash. IP-hash keeps a user on the same server —
   used for session stickiness so their session isn't lost."
```

**Q25. Rate-limiter algorithm + token-bucket refill kyu?**
```
   algos: TOKEN-BUCKET (common) · FIXED-WINDOW · SLIDING-WINDOW. token-bucket: bucket me tokens, har req 1 token, khaali → reject.
   REFILL kyu: fixed rate pe tokens WAPAS aate (100/min) → user ko quota har period wapas → warna ek baar khatam = hamesha blocked.
   + idle-jama tokens (bucket-capacity tak) → BURST allow.
   interview: "Token bucket: each request consumes a token, empty → reject. Tokens refill at a fixed rate so the user regains
   quota each period (else they'd be blocked forever), and accumulated tokens allow bursts."
```

**Q26. CORS: frontend(domain A) → API(domain B) → browser BLOCK. kyu? fix?**
```
   browser ki SAME-ORIGIN POLICY → alag origin ki request default block. fix = server response me
   Access-Control-Allow-Origin header → permission → browser allow. ★ prod me SPECIFIC origin whitelist (NOT "*", security;
   credentials ke saath "*" chalta bhi nahi). browser-enforced (server-to-server/Postman me CORS nahi).
   interview: "The browser's same-origin policy blocks cross-origin requests unless the server returns an
   Access-Control-Allow-Origin header naming that origin. In prod I whitelist the specific origin, not '*'."
```

**Q27. LB ke peeche ek server CRASH → LB ko kaise pata chalta + kya karta?**
```
   HEALTH-CHECK: LB har server ko /health ping bhejta. response aaya = zinda, koi dikkat nahi.
   response NAHI aaya → turant dead NAHI mark karta (network delay/blip ho sakta) → 3-4 baar retry (kuch second).
   phir bhi no response → LB us server ko DEAD mark → us pe traffic bhejnа BAND → baaki HEALTHY servers pe route.
   server recover + health-check dobara pass → LB wapas pool me ADD kar deta.
   (1 fail = blip ho sakta, isliye N-consecutive threshold — [links Q9])
   interview: "The LB health-checks each server (pings /health). A missed response isn't marked dead immediately — it could
   be a transient blip — so after N consecutive failed checks the LB marks it dead, stops routing to it, and sends traffic to
   the healthy servers. When it recovers and passes checks again, the LB adds it back."
```

**Q28. Popular item ka TTL expire + 10,000 request ek saath → kya hoga? kaise roko?**
```
   problem = CACHE STAMPEDE (thundering herd): hot key expire → saari 10k request ek saath MISS → seedhe DB pe → DB overload/crash.
   fix = LOCK / SINGLE-FLIGHT: sirf PEHLI request DB jaaye, data laaye, cache repopulate kare;
         baaki requests WAIT karein → phir fresh cache se padhein. DB ko ek hit, hazaar nahi.
   (bonus: refresh-before-expiry / stale-while-revalidate bhi option — proactive refresh.)
   interview: "A hot key expiring makes all concurrent requests miss and hit the DB at once — a cache stampede that can
   overload it. Fix with a lock / single-flight: only the first request goes to the DB, recomputes, and repopulates the cache;
   the rest wait and then read the fresh value. The DB takes one hit, not thousands."
```

**Q29. S3 ko "shard" kar sakte? (DB shard karte, S3 kyu nahi?)**
```
   NAHI — S3 ko DB jaisa manually shard nahi karte.
   shard KYUN karte? -> ek DB NODE ki limit (storage/throughput) -> data baant ke multiple nodes.
   S3 -> already MASSIVELY-DISTRIBUTED managed service -> AWS khud internally partition + auto-scale karta
         (near-INFINITE storage + requests). "single-node limit" wali problem hi nahi -> manual shard bekaar.
   -> jo sharding tu DB me karta, wo S3 me AWS PEHLE SE kar raha (chhupa ke).

   S3 ka analogous concept (confuse hota):
     1. KEY-PREFIX distribution: S3 objects ko key-prefix se internally partition karta. bahut high throughput chahiye
        -> keys alag prefixes me spread (/users/a/, /users/b/) -> parallel. (ab AWS per-prefix auto-scale, mostly zaroori nahi.)
     2. Multiple BUCKETS: region/tenant/type se alag -> ISOLATION/region-locality ke liye, SCALE ke liye nahi (1 bucket already infinite).
   interview: "You don't shard S3 — it's a managed, massively-distributed store that AWS auto-partitions and scales to
   near-infinite. The DB reason to shard (single-node limits) doesn't apply. The S3 analogue is key-prefix distribution for
   throughput, or multiple buckets for isolation/region — not for scale, since one bucket already scales."
```

**Q30. S3 itna scalable+sasta hai to sab wahi store karo, DB banao hi kyu?**
```
   S3 aur DB ALAG kaam ke liye -> S3 DB ko REPLACE nahi karta.

   S3 (object storage) DETA: file/blob ko KEY se store/get. bas.
      QUERY nahi ("SELECT WHERE age>25") ·  no ACID/transaction (atomic update) ·  no joins/relationships
      no partial-update (object pura overwrite) ·  many-small-fast-reads pe slow.
   DB DETA (jo S3 nahi): QUERY (where/sort/filter) · INDEX · ACID/transaction (no-double-spend) · JOINS · in-place UPDATE.

   analogy: S3 = giant WAREHOUSE (boxes label/key se fetch, par "sab red-shirt wale box do" nahi pooch sakte -> har box kholna padega).
            DB = smart INDEXED CATALOG (turant query/filter/relate/atomic-update, par bhaari boxes khud store nahi karta).

   -> DONO saath: bada FILE -> S3 | uska metadata + queryable-data + S3-URL -> DB. (video-app: video->S3, title/user/views->DB) [=Q17 blob-S3]
   interview: "S3 is object storage — great for large blobs fetched by key, but it can't query, index, join, transact, or
   partially update. A DB gives structured, queryable, consistent data. So you store the big file in S3 and its metadata +
   queryable fields + the S3 URL in the DB — they're complementary, not substitutes."
```

**Q31. CDN = cache hai, wo browser ke andar hota? (kahan baithta?)**
```
   CDN = distributed CACHE -> HAAN (essence). PAR browser me NAHI.
   BROWSER CACHE = device/browser ke ANDAR (local) -> ALAG cheez.
   CDN           = duniya bhar me faile EDGE SERVERS ka network (geo-distributed, e.g. CloudFront: Mumbai/Singapore/US...).
                   static content (image/video/css) edge pe cache -> user ke PAAS wale server se serve.
   levels: browser-cache (device) -> CDN edge (user ke shehar) -> ORIGIN (door, main). CDN beech me, EXTERNAL servers.
   point: content user ke NAZDEEK se -> low latency. (India user -> Mumbai edge, US origin tak nahi.)
   interview: "A CDN is a distributed cache, but on geographically-spread edge servers near users — not in the browser.
   Browser cache is local to the device; the CDN sits between browser and origin, serving static content from the nearest edge."
```

**Q32. DB ko region-wise replica (EU->EU, Asia->Asia) — ye kya? sharding se farak? geo-shard bhi hota?**
```
   ye = GEO-REPLICATION (multi-region replication): har region me DB ki COPY -> user paas wale replica se READ -> low latency.

   REPLICATION vs SHARDING (core):
     replication = SAME data ki COPIES (har replica pe POORA data) -> availability + read-scale + geo-locality.
     sharding    = data ko TUKDO me BAANT (har shard pe ALAG/different data) -> SCALE (ek node pe fit nahi).

   GEO-SHARDING bhi hota (haan): data ko REGION se BAANT -> EU users ka data EU shard, Asia ka Asia shard (DIFFERENT data, copy nahi).
     kyun geo-SHARD: ★ DATA-RESIDENCY/COMPLIANCE (GDPR -> EU data EU me hi rehna chahiye, legal) [★ JP=finance, HUGE] + write-locality + scale.
   real me DONO: region se SHARD (compliance) + har region me REPLICATE (availability).
   interview: "Region-wise copies = geo-replication (same data replicated per region for read-locality/availability).
   Sharding splits data (different data per node) for scale. Geo-sharding partitions data BY region — mainly for data
   residency/compliance (e.g. GDPR keeping EU data in EU) plus write-locality. Big apps do both: geo-shard + replicate within region."
```
