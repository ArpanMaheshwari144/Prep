# HLD — WHY / HOW Q&A DRILL (reasoning + follow-ups)

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
   NAHI — S3 ko DB jaisा manually shard nahi karte.
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
     ❌ QUERY nahi ("SELECT WHERE age>25") · ❌ no ACID/transaction (atomic update) · ❌ no joins/relationships
     ❌ no partial-update (object pura overwrite) · ❌ many-small-fast-reads pe slow.
   DB DETA (jo S3 nahi): QUERY (where/sort/filter) · INDEX · ACID/transaction (no-double-spend) · JOINS · in-place UPDATE.

   analogy: S3 = giant WAREHOUSE (boxes label/key se fetch, par "sab red-shirt wale box do" nahi pooch sakte -> har box kholna padega).
            DB = smart INDEXED CATALOG (turant query/filter/relate/atomic-update, par bhaari boxes khud store nahi karta).

   -> DONO saath: bada FILE -> S3 | uska metadata + queryable-data + S3-URL -> DB. (video-app: video->S3, title/user/views->DB) [=Q17 blob-S3]
   interview: "S3 is object storage — great for large blobs fetched by key, but it can't query, index, join, transact, or
   partially update. A DB gives structured, queryable, consistent data. So you store the big file in S3 and its metadata +
   queryable fields + the S3 URL in the DB — they're complementary, not substitutes."
```
