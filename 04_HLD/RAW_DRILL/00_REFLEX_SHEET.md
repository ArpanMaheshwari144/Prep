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
   data update -> cache stale       ->  CACHE-ASIDE          ->  DB update + cache DELETE (not update) -> read repopulate
   request kis server pe bheje      ->  LB ALGO              ->  round-robin/least-conn/IP-hash(session-sticky)
   bada app, alag scale + isolation ->  MICROSERVICES        ->  per-service scale+deploy, ek down baaki chalu (chhote pe nahi!)
   microservices ka single darwaza  ->  API GATEWAY          ->  routing+auth+rate-limit+logging ek jagah
   login state (multi-server)       ->  JWT (stateless)      ->  signed token self-proving, no store (vs Session=Redis lookup)
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

---

## ⏳ PENDING DRILL (abhi bache hue, JP-relevant)
```
   1. 8-STEP FRAMEWORK       -> answer assemble: clarify->scale->API->boxes->data->deep-dive->bottleneck->wrap. (INTERVIEW_FRAMEWORK.md)
   2. MSG DELIVERY GUARANTEE  -> at-least-once / exactly-once (idempotency se juda).
   3. SAGA / DISTRIBUTED TXN   -> MS me paisa-transaction rollback (JP finance-relevant). (note: 02_transactional)
   4. CONNECTION POOLING     -> har request pe nayi DB connection mehngi -> pool me reuse.
   5. DENORMALIZATION        -> read-heavy me joins mehnge -> data pehle se jod ke rakho (NoSQL me common).
   6. CORS                   -> frontend(domain A) -> API(domain B): browser BLOCK karta (same-origin) -> server "Access-Control-Allow-Origin" header de -> allow. (frontend+backend alag domain = common.)
   (SKIP — hyperscale/niche, JP-moderate me nahi: consistent-hashing, bloom-filter, leader-election, WAL.)
```

```
   ★ in 11 ke baad HLD ka JP-level set POORA -> phir sirf revise+drill, naya concept nahi.
```

---
> drill hote jaayein -> pending se upar (sections me) move karte jaana.
