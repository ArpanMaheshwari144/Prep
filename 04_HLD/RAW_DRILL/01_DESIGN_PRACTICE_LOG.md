# HLD DESIGN PRACTICE LOG (assembled 8-step designs — revise ke liye)

> **NAV** — KYA: purane mock ka LOG (archive — Jul me 8-step numbering thi; aaj ka rail 7-step hai, [MASTER](../00_MASTER_SHEET.md) dekho). Naya mock yahin likhna.

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

---

## SOCIAL MEDIA APP — dikkat-driven growth, BOLKE (25-Sep, Arpan ne khud chalaya, video wala tareeka)

```
   User -> Server -> DB se shuru, har dikkat pe ek dabba:
   1. DB bojh          -> SQL rakha ("dikkat aaye tab badlenge") -> CACHE
   2. aur bojh         -> 1 master + 2 read replica
   3. server bojh      -> 4 server + LB (horizontal)
   4. video / image    -> S3, DB me sirf URL + CDN + browser cache
   5. bada upload      -> QUEUE + WORKER, user ko turant status
   6. data aur badha   -> SHARD (region se, GDPR bhi)
   7. India-EU follow  -> HYBRID FAN-OUT (kam follower = push, celebrity = pull)
   8. alag data        -> polyglot (graph / Mongo / SQL), jahan dikkat wahan

   SUDHAAR (Claude ne point kiye):
   - S3 ki wajah "fast" nahi — DB bade binary ke liye bana nahi (size / backup / kharcha)
   - cookie me file nahi rehti — static = browser HTTP cache, cookie = session id / token
     (Arpan: pata tha, jaldi me bola — samajh ki galti nahi)
   - queue upload nahi karti — client pre-signed URL se seedha S3 (multipart);
     queue upload ke BAAD ka kaam (transcode / thumbnail / check)
     (Arpan: pata tha, typing ki jaldi me bola — samajh ki galti nahi)
   - region ke andar bhi shard (hash user_id) — warna India ka shard akela bada
   - celebrity post "direct DB" nahi — read time pe khincho, post khud cache me
   - Neo4j tabhi jab kai-hop (suggestions); follow list = simple table
   - (jodne ko) servers stateless, session Redis / token me · fan-out queue se async · polyglot = outbox

   AAJ NAHI HUE: requirements + estimation (seedha growth pe gaye) · API · schema ·
                 like counter · comments · notification · search
```

---

## PAYMENT SYSTEM — BOLKE (26-Sep, Arpan ne khud chalaya)

```
   FR: A se kate, B me jude, paisa na khoye, retry pe do baar na kate · NFR: strong consistency, low latency, 100M users
   1. beech me crash        -> DB TRANSACTION (debit + credit, dono ya koi nahi)
   2. retry pe do baar      -> IDEMPOTENCY key + race ke liye DB UNIQUE constraint (KHUD pakda)
   3. asli paisa bahar      -> PSP (Razorpay) + status state machine + RECONCILIATION
   4. alag bank / service   -> SAGA (ulta kaam)
   5. hisaab                -> LEDGER
   6. data bada             -> shard by account_id (alag shard = phir saga)
   7. padhai / server bojh  -> read replica · LB + kai box · CACHE balance pe NAHI (stale — KHUD pakda)
   8. DB choice             -> SQL (har jagah consistency) · abuse -> rate limit · PSP slow -> status

   SUDHAAR / JODNA:
   - ledger ki wajah = AUDIT (kab/kahan se/kahan gaya); "DB fail ho to ledger" nahi (ledger usi txn me)
   - PENDING pehle likho, PHIR PSP call · status wapas = webhook (push) + reconciliation (pull)
   - duplicate key pe error nahi, PEHLE wala result wapas
   - replica bhi peeche chalti: balance PRIMARY se, history replica se

   AAJ NAHI HUE: ek hi account se do payment ek saath (balance race -> UPDATE ... WHERE balance >= x)
                 PSP slow pe timeout + circuit breaker · notification / outbox · API · schema · monitoring
```

---

## BOOKMYSHOW — BOLKE (27-Sep, Arpan ne khud chalaya)

```
   SHURU: scope poocha ("kahan focus karun?") · FR: city -> movies/shows -> seat map -> book + pay
          NFR: HA, low latency, EK SEAT DO LOGON KO NAHI
   1. load do hisse          -> browse (bahut, baar-baar) = CACHE + REPLICA · booking kam (KHUD baanta)
   2. simple se shuru        -> User -> App -> DB
   3. do log ek seat         -> SQL (consistency) + atomic UPDATE
   4. pay me 3-5 min         -> HOLD + TTL (held_until column)
   5. double Pay             -> idempotency key
   6. bada release / spike   -> QUEUE + WORKER, worker ek-ek karke atomic UPDATE
   7. async pe user ko kya   -> DARWAZE pe GINTI (admission counter) — KHUD NIKAALA, file me nahi tha

   SUDHAAR:
   - "DB ek hi update karega" ka MATLAB sahi, par ye WHERE status='available' se hota hai,
     DB apne aap nahi (bina WHERE dono update chalte, dono ko ticket)
   - status 'held' bolo, 'booked' sirf payment ke baad
   - SQL me TTL nahi, row apne aap nahi badalti -> UPDATE me "OR held_until < now()" ya sweeper
     (FILE me adhoora tha — ab dikkat 2 me poora likha)
   - ginti "total seat" batati, "teri seat" nahi -> "booked" sirf atomic UPDATE jeetne pe;
     tab tak "in progress" (dikkat 5)

   Arpan ka nichod (27-Sep): HLD bhi DSA jaisa GATE hai — 45 min job ka andaza nahi deta.
```

---

## RATE LIMITER — BOLKE, 6 SAWAAL SE (28-Sep, Arpan ne khud chalaya)

```
   SHURU: scope poocha ("kis pe limit, kahan?") · FR: logged-in = user id / API key, anonymous = IP,
          endpoint-wise limit (login sakht), paar -> 429 + Retry-After
          NFR: low latency, sab server pe ek ginti, fail-open, HA
   BASIC: User -> LB -> API Gateway (limiter) -> Server · ginti Redis me

   SAWAAL        -> DIKKAT                                  -> DABBA
   ek saath?     -> do request ek ginti padh ke dono pass    -> Redis INCR (atomic), check lauti value pe
   gira?         -> Redis gira to limiter atke               -> replica + Sentinel auto promote, sab gira -> FAIL-OPEN
   bahar slow?   -> Berlin ki request India wale Redis tak   -> region-wise Redis + user apne region pe chipka
   dobara?       -> ek banda 429 kha ke bhi maarta rahe      -> event QUEUE (async) -> worker pattern -> WAF block
   bahut zyada?  -> ek Redis saara load na jhele             -> SHARDING (region-wise baanta); region ke andar
                                                              aur chahiye to Redis Cluster, key = user id
   purana dikha? -> failover pe replica ki ginti peeche      -> chalta hai, thode extra request
   (algo)        -> kaunsa?                                  -> TOKEN BUCKET (burst ok); tokens + last_refill,
                                                                refill request pe hisaab se, Lua script me (race)

   SUDHAAR:
   - IP har user pe nahi, sirf anonymous pe (office/WiFi me bahut log ek IP)
   - GET phir SET = race wapas; INCR ki lauti value pe check
   - token bucket = padho-hisaab-likho, ek step nahi -> Lua script

   Arpan ka nichod (28-Sep): topic pata, notes kholte hi yaad aata; FR = kaun/kya/phir,
   NFR = 5 shabd, baaki 6 sawaal round me khud nikaal dete hain.
```

---

## NOTIFICATION — BOLKE (29-Sep, Arpan ne khud chalaya, notes bina padhe)

```
   SHURU: scope poocha ("kis pe focus?") · requirements apne shabdon me dohraye
   1. basic                 -> Order Svc -> Email Svc -> User
   2. email band = order fail -> beech me KAFKA (order publish karke laut jaata)
   3. SMS slow = sab slow   -> har channel ki apni QUEUE + WORKER + rate limit
   4. jise SMS nahi chahiye -> USER PREF + TEMPLATE + time, fanout se PEHLE (worker pe faltu kaam nahi)
   5. ack kho gaya          -> idempotency key, Redis SET NX
   6. provider fail         -> backoff + jitter, retry queue
   7. bheja != mila         -> status rakho
   8. OTP fast              -> "Kafka me priority nahi hoti" (sahi)
   9. DB                    -> Cassandra (tracking, likhna zyada)

   ROUND ME SIKHA / THEEK HUA:
   - key laga di par provider call fail -> message kho gaya -> "sending" (chhota TTL) -> "sent"
     (naya point, notes me dikkat 4b)
   - provider down pe workers timeout me atke -> CIRCUIT BREAKER + doosra provider (notes dikkat 6)
     fail pe circuit OPEN hota hai (CLOSED nahi): taar toota = call band
   - status kahan se aata -> provider ka WEBHOOK -> Tracking DB
   - priority -> alag TOPIC + alag worker pool (PriorityBlockingQueue ek process tak; FILE me galat tha)
```
