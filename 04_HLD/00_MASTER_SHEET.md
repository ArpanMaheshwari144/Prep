# HLD MASTER SHEET — koi bhi design ASSEMBLE karne ka tareeka

> **NAV** — KYA: interview-din ki EK file (archetype -> assemble -> bolo). · CROSS-QUESTION BANK · A YA B · KAISE BOLNA · SHABD = isi file me (neeche) · CONCEPT-detail: [FOUNDATIONS](FOUNDATIONS) · 15 DESIGN: [SYSTEM_DESIGNS](SYSTEM_DESIGNS) · LOG: [HLD_PRACTICE_LOG](HLD_PRACTICE_LOG.md)

> EK file. Interview se pehle sirf YE. (detail chahiye to hi SYSTEM_DESIGNS/* kholo.)
>
> IDEA: har design naya nahi hota. Har design **6 archetype** me se kisi ek (ya do ke mel) me girta hai.
> Archetype pehchano -> uska DIL pata -> default blocks lagao -> RAIL pe bol do.
>
> ★ HONEST HAD: ye sheet kisi bhi design ka HIGH-LEVEL khada kar degi (boxes + kyun + trade-off).
>   Jo cheez tune PADHI hi nahi uska DEEP-DIVE ye sheet nahi degi -- wahan section 7 wali
>   honest line kaam aati hai. Ye kami nahi, ye tareeka hai.

---

## 0. KAUNSI FILE KAB — poore 04_HLD ka naksha (26-Sep saaf kiya: har kaam ki EK file)

```
   INTERVIEW-DIN / REVISE
     00_MASTER_SHEET.md    <- YE. KYA bolna: 6 sawaal · CROSS-QUESTION BANK · archetype · rail ·
                              blocks · A ya B jode · 15 design ka DIL · KAISE bolna · SHABD
                              (29-Sep: 01_DELIVERY + 02_TRADEOFFS isi me mile)

   PADHNE / DEPTH
     FOUNDATIONS/01..18    <- ek concept = ek file
        01 hld · 02 capacity · 03 LB · 04 cache · 05 replication · 06 sharding · 07 queue
        08 CAP · 09 db kaunsa · 10 microservice baat-cheet · 11 SPOF · 12 search · 13 id
        14 HUB (bachane wala hi girane wala) · 15 CDN · 16 DNS · 17 WAF · 18 monitoring
     SYSTEM_DESIGNS/01..15 <- 15 poore design (05 = concept walkthrough)

   DRILL / ABHYAS
     HLD_PRACTICE_LOG.md                  <- bolke kiye design ka log (naya yahin)

   BAHAR (zaroorat pe)
     ../06_COMPARES/       <- gehre "farak batao" explainer (TCP/UDP, HTTP versions, JWT/OAuth...)
     ../05_INFRA_DEEP/     <- sirf DEVOPS commands + hands-on demo folder (LB / monitoring / load-test)
```
```
   ★ CONFUSION-PROOF NIYAM:
       "kya bolna"         -> ye file
       "what if X fails?"  -> ye file, CROSS-QUESTION BANK
       "kaise bolna"       -> ye file, KAISE BOLNA + SHABD (aakhir me)
       "A ya B?"           -> ye file, A ya B — TRADE-OFF JODE (section 4 ke baad)
       "ye cheez hai kya?" -> FOUNDATIONS
       "poora design"      -> SYSTEM_DESIGNS
       "khud ko test"      -> HLD_PRACTICE_LOG (bolke kiye round)
     Har file ke UPAR ek NAV line hai -> wahan se seedha jump.
```

---

## ★★ TEEN BAATEIN JO SAB PE BHAARI (Arpan ka apna nichod, 15-Sep — asli mock videos se)

```
1. PERFECT DESIGN HOTA HI NAHI -- chhote se shuru, phir scale ke baare me socho.
   sab ek saath kaise soch sakta koi? IMPOSSIBLE hai. pehle chhota, phir bada.
   -> STEP 5 = sirf kaam-chalau boxes. cache/CDN/queue/shard = STEP 6-7, dikkat ke jawab me.

2. NUMBER KE PEECHE MAT BHAAG -- wo bekaar hai. bolo aur aage badho.
   number bol ke tu kuch SPECIAL nahi kar deta.

3. BOTTLENECK RATTO MAT -- bolte waqt KHUD USER BANKE dekho:
   wo kya soch raha, kahan cheez phat rahi. Bottleneck khud dikh jaayega.
```
> Misaal (Spotify / Bitly) = neeche "KAISE BOLNA" section.

---

## ★★ HAR DESIGN PE 6 SAWAAL — sirf YAAD DILAANE ki list, rail nahi (28-Sep sudhaar)

> 28-Sep (rate limiter ke baad, Arpan): sawaal bhi khud poochna, jawab bhi khud dena — aur JAWAB
> padhne se aata. Ye 6 koi naya design nahi banwate; sirf PADHE hue dabbe pe dikkat yaad dilaate.
> Har design ki APNI khaas dikkatein YAAD rakhni padti hain = section 5 (DIL + KHAAS hissa).

```
                              PAYMENT                              GOOGLE DOCS
1. ye GIRA to?                beech me crash -> transaction,       server gira -> ops log/buffer,
                              PENDING pehle                        snapshot + ops
2. DOBARA aaya to?            retry -> idempotency key             same op do baar -> op-id, dobara chhodo
3. do EK SAATH aaye to?       race -> UNIQUE, WHERE balance >= x   ek jagah do log type -> OT / CRDT
4. bahar wala SLOW / band?    PSP -> status + timeout + recon      user offline -> likhne do, baad me merge
5. BAHUT zyada ho gaya?       LB, replica, shard by account_id     crore WebSocket -> conn tier, shard docId
6. kisi ko PURANA dikha?      cache / replica lag -> balance       edit der se -> WebSocket + pub/sub push
                              PRIMARY se
```

```
★ har design ka ek sawaal sabse BHAARI = uska DIL:
     payment 2 + 3 (retry, race) · google docs 3 (saath edit) · chat 1 + 6 (offline, der)
     bookmyshow 3 (ek seat do log) · rate limiter 3 + 5 · feed 5 + 6
★ jawab kahan se: common dabbe (LB/replica/shard/cache) aate hain · KHAAS wala = neeche section 5
★ round me bhool gaya -> ye 6 mann me ghumao. "ek second, sochta hoon" bolna normal.
   45 min me 2-3 dikkat GEHRAI se = kaafi. sab ek saath koi nahi bolta.
```

---

## ★★★ CROSS-QUESTION BANK — jo har design me ghoom ke aate hain (roz 5, ulte kram me)

> KYUN: design notes KAHANI ke kram me likhe hain (dikkat 1 -> 2 -> 3). Interviewer kram se nahi
> poochta — beech me koi bhi "what if X fails?" phenk deta. Ye bank wahi sawaal DESIGN se ALAG karke
> rakhta hai. Har sawaal: Q (jaise wo poochega) · jawab (kya toota + ilaaj) · BOL (ek English line) ·
> kahan aaya (SYSTEM_DESIGNS number).
> ROZ: 5 sawaal, NEECHE se upar (ulte kram) — taaki kram yaad na ho, sawaal yaad ho. Pehle khud bolo,
> phir jawab dekho. Design ka naam badal ke poochho ("payment me ye gira to?", "chat me?").

### A. GIRA / SLOW

```
Q1  "What happens if this server / node / DB goes down?"
    app server: stateless + LB health check (2-3 fail = pool se bahar) -> baaki chalte rahe.
    DB primary: replica promote (failover); sync/semi-sync ack warna last writes kho sakti.
    cache/broker node: replica (Redis replica, Kafka ISR). ek copy = SPOF -> copies ALAG AZ me.
    BOL: "Every stateful piece has a replica in a different AZ; the LB health-checks the stateless
          servers, so a dead node is removed from the pool and we fail over to a replica."
    kahan aaya: 01 · 02 · 11 · 13 · 14 · 15   (depth: FOUNDATIONS/11 SPOF)

Q2  "What if the downstream service / provider is slow?"
    bina timeout -> har worker 30 sec atka -> poora system thapp (slow = down se BURA).
    ilaaj: chhota TIMEOUT · CIRCUIT BREAKER: N fail -> OPEN = call BAND (fail-fast, timeout me
    phaste hi nahi) -> thodi der baad HALF-OPEN = ek test call -> theek to CLOSED ·
    FALLBACK provider (SMS: Twilio -> SNS) · paisa ho to PENDING rakho + reconcile, andha retry nahi.
    BOL: "I'd put a timeout on the call and a circuit breaker around it. Once it opens we stop
          calling the provider at all, fail fast, and route to a backup provider."
    kahan aaya: 04 (dikkat 6) · 07 (PSP) · 09 (har source alag timeout/skip)

Q3  "What if the cache goes down?"
    saara read DB pe -> DB bhi gira (cache ne jo load chhupaya tha wo ek saath aaya).
    ilaaj: cache ka replica/cluster · fallback DB pe PAR load shedding / rate limit ke saath ·
    STAMPEDE roko (hot key expire = 1000 miss): mutex (ek hi rebuild kare) / soft-TTL.
    rate limiter ka Redis gira -> default FAIL-OPEN (allow), payment/auth me FAIL-CLOSED.
    BOL: "The cache is an optimization, so we fall back to the DB, but with load shedding and a
          mutex on rebuild so a cold cache doesn't stampede the database."
    kahan aaya: 11 (dikkat 5) · 02 (Redis down) · 01 · 03

Q4  "What if a whole region / data center goes down?"
    multi-AZ = sasta, lagbhag hamesha. multi-region = MEHNGA (data sync, latency, double infra).
    ilaaj: DNS (Route 53) health check -> mara region hatao, paas wala region do · data async
    doosre region me copy (thoda data jaa sakta) · money-path pe soch ke.
    BOL: "Within a region we run multi-AZ. For a full region outage, DNS health checks route users
          to another region; replication across regions is async, so we accept a small loss window."
    kahan aaya: 01 · 03 · 02 (region-sticky)   (depth: FOUNDATIONS/11, 16)
```

### B. DOBARA / SAATH

```
Q5  "What if the same request comes twice / the client retries?"
    network timeout -> user dobara tap -> do baar paisa / do message.
    ilaaj: IDEMPOTENCY KEY (client UUID, retry pe same) -> server claim: UNIQUE constraint ya
    Redis SET key NX EX -> dobara aaye to STORED RESULT lautao (error nahi). queue consumer = eventId.
    BOL: "The client sends an idempotency key; the server claims it atomically, and a retry with
          the same key gets the stored result instead of being processed again."
    kahan aaya: 07 · 04 · 10 (double click) · 13 (eventId) · 15 (clientMsgId)

Q6  "You set the idempotency key, but then the send failed. Now what?"
    SET NX lag gaya -> provider FAIL -> retry pe key mili -> SKIP -> message KHO gaya.
    ilaaj: do haalat: SET key "sending" NX EX 60 -> success pe SET key "sent" EX 86400.
    fail ya worker mara -> 60 sec me "sending" khud mit jaata -> retry chal jaata.
    (fail pe DEL karna bhi chalta, par worker crash pe DEL chalta hi nahi -> kam pakka)
    BOL: "I set it as 'sending' with a short TTL and only mark it 'sent' after the provider
          accepts it, so if the send fails or the worker dies, the key expires and the retry goes through."
    kahan aaya: 04 (dikkat 4b) · 07 (IN_PROGRESS -> DONE wahi soch)

Q7  "Two users do this at the same time — what happens?"
    read -> check -> write = do log dono "available" padhte, dono likh dete.
    ilaaj: check WRITE ke ANDAR: UPDATE seats SET status='booked' WHERE seat_id=? AND
    status='available' (1 row = jeeta, 0 = gaya) · balance: WHERE balance >= x ·
    version column (optimistic: WHERE version=?) · counter: Redis INCR/DECR · kai step: Lua script.
    BOL: "I make the check part of the write -- a conditional update -- so only one of them
          succeeds; the other gets zero rows updated."
    kahan aaya: 10 · 14 · 02 (INCR/Lua) · 07 (UNIQUE claim) · 06 (har symbol ek thread)
    ★ farak: 2 user ek resource = atomic/lock · 1 user ka retry = idempotency (Q5).
```

### C. BHEED / BADA

```
Q8  "What if traffic suddenly spikes 10x?"
    ilaaj: QUEUE spike ko HOLD karti (LB baantta hai, hold nahi; replica sirf READ) ·
    AUTOSCALE stateless servers (dhyan: consumer autoscale ne DB maara -> DB ki had pe cap) ·
    RATE LIMIT per user (429 + Retry-After) · ADMISSION CONTROL / LOAD SHEDDING: darwaze pe ginti
    (seat 3000 -> 3000 andar, baaki "full"/waiting room), system ki sehat dekh ke mana.
    BOL: "A queue absorbs the burst, we autoscale the stateless tier, and admission control at the
          edge rejects what we can't serve instead of letting everything slow down."
    kahan aaya: 10 · 02 (load shedding) · 04 (burst) · 03   (FOUNDATIONS/14: ilaaj hi bimari)

Q9  "How would you scale this to 10x users?"
    pehle bolo KYA pehle tootega (DB write? hot key? ek queue?) -> phir wahi ka ilaaj:
    read zyada -> cache + read replica · write/storage nahi sama -> shard · slow kaam -> async queue.
    BOL: "First I'd find what breaks first -- usually DB writes or a hot key -- then cache and
          replicas for reads, sharding for writes, and push slow work to a queue."
    kahan aaya: sab (section 3 STEP 7)

Q10 "The database is too big / takes too many writes. What do you do?"
    replica sirf READ baant-ta; write/storage ke liye SHARD.
    SHARD KEY = high cardinality + barabar baat + query pattern se mel (user_id / account_id /
    docId / chat_id). country/date = bura (skew -> ek shard pe sab). cross-shard join mushkil.
    naya node pe hash % N = lagbhag sab key hilti -> consistent hashing = sirf ~K/N.
    BOL: "Replicas only scale reads, so for writes I'd shard by user id -- high cardinality,
          even spread, and it matches how we query, so most requests hit a single shard."
    kahan aaya: 01 · 03 · 12 (docId) · 13 · 14 (account_id) · 15 (chat_id)   (FOUNDATIONS/06)

Q11 "What about a hot key / celebrity / hot partition?"
    shard barabar baata, par EK key pe hi saara traffic -> wo ek node mara.
    ★ consistent hashing ye NAHI bachata (wo key phir bhi ek hi node pe).
    ilaaj: hot key KAI node pe replicate + L1 local cache (read) · key me bucket (chat_id + 0..9),
    padhte waqt jodo (write) · feed me celeb = fanout on READ (hybrid).
    BOL: "Consistent hashing doesn't help a single hot key, so I'd replicate that key and add a
          local cache for reads, or split its writes across buckets like chatId plus 0 to 9."
    kahan aaya: 11 · 03 (celeb) · 15 (viral group) · 13 (bada customer)
```

### D. PURANA / KRAM

```
Q12 "The user updated something but still sees the old value. Why?"
    (a) CACHE stale: DB update hua, cache me purana. ilaaj: write pe cache key DELETE (update nahi --
        do write ulte kram me = galat value), agla read DB se bharega + TTL safety-net.
    (b) REPLICA LAG: write primary pe, turant read replica se. ilaaj: READ-YOUR-OWN-WRITES --
        likhne wale ko thodi der / critical read PRIMARY se.
    BOL: "On a write I delete the cache key rather than update it, with a TTL as a backstop; and
          for replica lag, the user who just wrote reads from the primary for a short window."
    kahan aaya: 11 · 01 · 03 · 14 (balance primary se) · 07   (FOUNDATIONS/04, 05)

Q13 "How do you keep messages / events in order?"
    Kafka order sirf EK PARTITION ke andar; global order = ek partition = throughput khatam.
    ilaaj: key = userId/chatId/accountId -> hash(key) % partitions -> us key ke sab event ek
    partition me · order SERVER sequence id se, client time se nahi · partition badhaye to order toot sakta.
    BOL: "I only need per-key ordering, so I key by chat id -- all its messages land in one
          partition and stay in order, while different chats run in parallel."
    kahan aaya: 13 · 15 · 06 (har symbol ek sequencer) · 12 (doc ke ops ek jagah serialize)

Q14 "Consistency or availability — which do you pick?"
    network partition me dono nahi milte. PAISA / SEAT / INVENTORY = CP (reject kar do, galat mat do).
    feed / like-count / search / cache = AP (purana chalega). ek hi system me dono ho sakte.
    BOL: "For the booking itself I'd pick consistency -- I'd rather reject a write than double-book
          a seat. The browse and search path can stay available and eventually consistent."
    kahan aaya: 10 · 07 · 14 · 12 (edits AP, permissions CP) · 03 (AP)
```

### E. KHO GAYA / BEECH ME

```
Q15 "How do you make sure no message is lost?"
    producer: acks=all + replication (ISR) · consumer: AT-LEAST-ONCE = kaam PEHLE, offset commit
    BAAD me (crash = dobara aayega, khoyega nahi) -> isliye consumer IDEMPOTENT ·
    fail -> backoff retry -> max ke baad DLQ (poison message baaki ko na roke) + alert.
    DB + event dono chahiye -> OUTBOX (event usi DB txn me outbox table me, alag process bheje).
    BOL: "At-least-once delivery with an idempotent consumer, retries with backoff, and a dead
          letter queue for poison messages. If a DB write and an event must both happen, an outbox."
    kahan aaya: 13 · 04 · 14 (outbox) · 15 (pehle DB, phir bhejo)

Q16 "What if the server crashes in the middle of the operation?"
    DB me likha, event nahi gaya / PSP ko bheja, jawab nahi aaya -> aadha kaam.
    ilaaj: pehle durable STATE likho (PENDING / UPLOADING) -> phir bahar ka call -> webhook (push) +
    RECONCILIATION job (pull: pending dhoondho, poocho, resolve) · DB+event = OUTBOX · ek DB = ek
    @Transactional; kai service = SAGA + compensate · event log pehle, phir state (crash = replay).
    BOL: "I write a PENDING state before the external call, so after a crash a reconciliation job
          finds it and asks the provider what actually happened. Nothing is ever just lost."
    kahan aaya: 07 · 14 · 06 (event log replay) · 08 (status tracking)

Q17 "The provider returns 429 — you're sending too fast. What now?"
    bina throttle -> burst -> 429 -> sab fail -> turant retry = aur hathoda.
    ilaaj: apni taraf THROTTLE (token bucket, provider ki raftaar se) · exponential BACKOFF + JITTER
    (sab worker ek saath wapas na aayein) · Retry-After maano · queue me rakho, drop nahi.
    BOL: "Workers throttle themselves with a token bucket at the provider's rate, and on a 429
          they back off exponentially with jitter instead of hammering it."
    kahan aaya: 04 (dikkat 8) · 02 (hum khud 429 + Retry-After dete)

Q18 "How do you prioritize urgent work, like OTPs?"
    ek queue = OTP marketing ke 50,000 ke peeche. ★ Kafka me message priority NAHI hoti.
    ilaaj: har lane ka ALAG topic + apna worker pool (high/medium/low) -> OTP ka pool kabhi khaali.
    (PriorityBlockingQueue sirf EK process ke andar, distributed me nahi)
    BOL: "Kafka has no message priority, so I'd use separate topics with their own worker pools --
          OTPs never wait behind a marketing blast."
    kahan aaya: 04 (dikkat 7)
```

### F. BAAKI

```
Q19 "How do you know the system is working?"
    METRICS: p99 latency · error rate · queue lag · cache hit rate · DB connections -> ALERT on had.
    "bheja" != "pahuncha": provider WEBHOOK (delivered/failed/bounced) -> tracking DB.
    paisa: reconciliation (ledger vs bank). logs + trace id se ek request follow.
    BOL: "I'd track p99 latency, error rate, queue lag and cache hit rate with alerts, and use
          delivery webhooks and reconciliation to confirm the outcome, not just the send."
    kahan aaya: 04 (dikkat 9) · 07 · 14 · sab   (FOUNDATIONS/18)

Q20 "How do you secure it / stop abuse?"
    authN (JWT/OAuth) + authZ (ye cheez ISI user ki? ownerId check) · per-user/IP RATE LIMIT ·
    WAF edge pe (SQLi/XSS/bad IP/bot) · TLS · presigned URL chhoti expiry · file ka MAGIC BYTES
    check (naam pe bharosa nahi) · secrets vault me, code me nahi.
    BOL: "Authentication at the gateway, an ownership check on every resource, rate limiting per
          user, and a WAF at the edge for common attacks."
    kahan aaya: 08 · 02 · 01 (abuse) · 07   (FOUNDATIONS/17 WAF)

Q21 "Data keeps growing — what happens in 3 years?"
    ilaaj: RETENTION / TTL (jo nahi chahiye wo mita do) · time se PARTITION (mahina) -> purana
    partition DETACH -> COLD storage (S3/Glacier, sasta) · ledger/audit KABHI delete nahi, sirf khiskao.
    ★ retention != sharding (retention size ghatata, shard load baant-ta).
    BOL: "Hot data stays in the main store; older data is partitioned by month and moved to cheap
          cold storage. Ledger data is never deleted, only archived."
    kahan aaya: 09 · 14 · 15 · 04 · 07

Q22 "What's the single point of failure here?"
    request ka raasta kheencho, har dabbe pe "ye gira to?" -- aur jo dabba nahi: DNS, TLS cert,
    ek AZ, NAT gateway, third-party, config store. 2 copy kaafi nahi: alag failure domain? TESTED?
    ek hi galat config sab pe? bache hue bojh jhelenge (N+1)?
    BOL: "I walk the request path and ask 'what if this fails' at every hop, including DNS,
          certificates and third parties. Every stateful piece has a tested replica in another AZ."
    kahan aaya: sab   (FOUNDATIONS/11)
```

---

## 1. TEEN LINE KA METHOD (poora khel isi me hai)

```
   1. Ye design kis ARCHETYPE ka hai?        -> section 2
   2. Us archetype ka DIL kya hai?           -> wahi Step-1 me bolna, wahi deep-dive banega
   3. Dil ke hisaab se BLOCKS uthao          -> section 4 ka menu
   -> RAIL pe rakh ke bol do                 -> section 3
```

---

## 2. ★★ 6 ARCHETYPE — koi bhi design in me se hai (ye sabse zaroori table)

```
┌───────────────────────────────────────────────────────────────────────────────────────┐
│ A. READ-HEAVY / FEED          DIL = read SASTA ho                                     │
│    misaal: twitter-feed, news-aggregator, youtube-home, LinkedIn-feed, instagram      │
│    default blocks: CACHE (aggressive) . read-REPLICA . DENORMALIZE . CDN              │
│                    precompute/FANOUT-on-write . pagination (cursor)                   │
│    signature Q   : celebrity/hot-key? -> hybrid fanout (normal=write, celeb=read)     │
│    consistency   : EVENTUAL (2 sec purana feed chalega)                                │
├───────────────────────────────────────────────────────────────────────────────────────┤
│ B. WRITE-HEAVY / INGEST       DIL = write GIRE nahi + peeche kaam chalta rahe          │
│    misaal: logging, analytics, clickstream, IoT, message-queue, notification-ingest    │
│    default blocks: QUEUE (Kafka) . partition/shard by key . batch-write . async worker │
│                    append-only log . retention                                         │
│    signature Q   : ordering? -> per-key partition . duplicate? -> idempotent consumer  │
│    consistency   : EVENTUAL + at-least-once                                            │
├───────────────────────────────────────────────────────────────────────────────────────┤
│ C. TRANSACTIONAL / MONEY      DIL = galat data KABHI nahi (ek hi baar, ek hi seat)      │
│    misaal: payment, booking (bookmyshow), trading, wallet, inventory, ticket           │
│    default blocks: SQL + ACID . LOCK ya unique-constraint . IDEMPOTENCY-key            │
│                    LEDGER (append-only, audit) . SAGA (multi-service) . CP side of CAP │
│    signature Q   : do user ek saath? -> atomic check+mark . retry pe double? -> idem-key│
│    consistency   : STRONG (yahan koi compromise nahi)                                  │
├───────────────────────────────────────────────────────────────────────────────────────┤
│ D. REAL-TIME / PUSH           DIL = turant pahunche + connection zinda rahe             │
│    misaal: chat/WhatsApp, google-docs-collab, live-price, live-score, presence          │
│    default blocks: WEBSOCKET . connection-registry (kaun kis server pe)                 │
│                    PUB/SUB (Redis/Kafka) server-to-server . offline -> push (FCM/APNS)  │
│                    message-store (Cassandra) . ordering per-conversation                │
│    signature Q   : user offline? -> store + push . 2 log ek saath likhein? -> OT/CRDT   │
│    consistency   : per-conversation order + eventual convergence                        │
├───────────────────────────────────────────────────────────────────────────────────────┤
│ E. SEARCH / LOOKUP            DIL = sahi cheez turant dhoondh ke do                     │
│    misaal: typeahead, search, news-search, product-search, nearby/geo                   │
│    default blocks: INVERTED INDEX (Elasticsearch) . TRIE (prefix) . top-K cache         │
│                    async indexer (DB -> index) . ranking . geohash (nearby ke liye)     │
│    signature Q   : index kab update? -> async, thoda stale chalega                      │
│    consistency   : EVENTUAL (naya item 1 min baad dikhe to chalega)                     │
├───────────────────────────────────────────────────────────────────────────────────────┤
│ F. INFRA / COMPONENT          DIL = ek machine se bada + machine mare to chale           │
│    misaal: distributed-cache, rate-limiter, URL-shortener(ID-gen), unique-ID, LB, MQ    │
│    default blocks: CONSISTENT HASHING . SHARD + REPLICA . leader/follower                │
│                    in-memory DS . TTL/eviction . coordinator (ZK/KRaft/etcd)             │
│    signature Q   : node add/remove? -> consistent hashing . node mare? -> replica/failover│
│    consistency   : mostly eventual, metadata strong                                      │
└───────────────────────────────────────────────────────────────────────────────────────┘

★ BADE PRODUCT = 2-3 ARCHETYPE KA MEL. Yahi "design LinkedIn/Instagram/Uber" ka raaz hai:
     LinkedIn  = A (feed) + E (search/people) + D (messaging) + B (activity-ingest)
     Instagram = A (feed) + F (media/blob+CDN) + E (explore/search)
     Uber      = D (live location) + E (geo-match) + C (payment)
     WhatsApp  = D (real-time) + B (ingest) + F (storage)
     Zomato    = E (search) + C (order/payment) + D (live tracking)
  -> interview me BOL do: "this really breaks into three sub-systems" -> phir ek-ek karke chalo.
     Ye move akela hi tujhe structured dikha deta hai.
```

---

## 3. UNIVERSAL RAIL — har design pe yahi bolna (design ka naam bas badalta hai)

```
STEP 1  REQUIREMENTS (2 min)
   ★★ PEHLA MOVE = SCOPE KAATO (15-Sep, asli mock video se -- ex-Google EM ka pehla kaam):
      "This is a big system -- I'll scope it to X and Y and go deep there. Sound good?"
        Spotify wale round me usne turant bola: "sirf finding aur playing."
        -> baaki 40 minute sirf 2 cheez pe gaye, design bikhra nahi.
      Bada product (LinkedIn/Uber/Zomato) mile -> 2-3 sub-system bolo, phir EK chuno.

   . 2-3 clarifying Q poochho: kitne user? read-heavy ya write-heavy? real-time chahiye?
     kya scope me NAHI hai?
   . FR = 4-5 line, user ki bhasha me ("user post kare", "user feed dekhe")
   . NFR = scale . latency . availability . consistency (STRONG ya EVENTUAL -- ye bolna ZAROORI)
   . ★ AAKHIR ME DIL BOLO: "the heart of this problem is ___"   <- yahi poore interview ka anchor

STEP 1b CORE ENTITIES (20 second -- ek line, bas)
   3-5 NAAM bol do, poora schema nahi:
        Kafka  -> "Topic, Partition, Message, Consumer Group, Offset"
        Spotify-> "User, Song, Artist, Playlist"
        Bitly  -> "User, Link, Click"
   KYUN: iske baad API aur data-model apne aap nikal aate hain (dono me yahi naam aayenge).
   (Hello-Interview framework isko alag step kehta hai; asli round me ye API ke saath hi
    ek line me bol diya jaata -- bada step mat banao.)

STEP 2  ESTIMATE (2 min) -- number se DECISION nikaalo, warna number bekaar
   QPS      = daily requests / 100,000      (approx: 86,400 ~ 10^5)
   peak     = 2-3x
   storage  = per-record size x records x days
   ★ phir turant bolo: "iska matlab ek machine kaafi nahi -> shard/replica chahiye"

STEP 3  API (1-2 min) -- 3-5 endpoint, bas
   POST /resource  . GET /resource/{id}  . GET /feed?cursor=...
   . auth ek line . pagination = CURSOR (offset nahi, deep-page slow)

STEP 4  DATA MODEL (2-3 min)
   . main tables/collections + KEY fields
   . SQL ya NoSQL -> KYUN (rishte+transaction = SQL . flat+huge+fixed-access = NoSQL)
   . shard key kya hoga
   . bade file/media -> S3/blob, DB me sirf URL

STEP 5  HL BOXES (5 min) -- yahan sabse zyada number milte
   client -> CDN(static) -> LB -> API gateway -> service(s) -> cache -> DB
                                                      |
                                                      +-> QUEUE -> workers -> (email/index/analytics)
   ★ har box ke saath ek "kyun" -- box bina kyun ke = ratta

STEP 6  DEEP-DIVE (8-10 min) -- 2-3 cheez, POORA nahi
   ★ kaunsi chuno? -> jo DIL hai wahi (Step 1 me jo bola). Archetype ka "signature Q" = yahi.

STEP 7  BOTTLENECK / SCALE-10x (3 min)
   . kya PEHLE tootega? -> DB write . single hot key . ek queue . ek service
   . fix: shard . cache . replica . partition badhao . async karo . rate-limit
   . monitoring: p99 latency, error-rate, queue-lag, cache hit-rate
```

---

## 4. BLOCK MENU (need -> block -> ek line kyun)

> Ye compact version interview-din ke liye.

```
   read baar-baar          -> CACHE (Redis, cache-aside + TTL)   RAM, disk se 100x tez
   read phir bhi DB pe     -> READ REPLICA                       reads baant do
   write/storage nahi sama -> SHARDING (+ shard key)             data tukdon me
   spike / burst           -> QUEUE                              requests HOLD karo (LB nahi, REPLICA bhi
                                                                 nahi — replica read-scale hai, write-spike nahi)
   slow kaam, decouple     -> QUEUE + WORKER                     user block na ho
   traffic baantna         -> LOAD BALANCER (L7)                 content-based routing + TLS
   kai service, ek darwaza -> API GATEWAY                        auth+routing+rate-limit ek jagah
   bhaari file stream/DL   -> SERVER SIRF LINK DE (presigned)    bytes client<->S3 seedha,
                                                                 warna server bandwidth marta
   machine mare            -> REPLICATION (leader/follower)      copy se kaam chale
   paisa / seat            -> SQL + LOCK/unique + CP             galat data NEVER
   consistency vs availab. -> CAP choice                         paisa/booking = CP, social feed = AP
   retry pe double-effect  -> IDEMPOTENCY KEY                    ek hi baar asar
   2 user ek resource      -> ATOMIC check+mark (ya lock)        race khatam
   abuse / flood           -> RATE LIMITER (token bucket, 429)   Redis counter
   static global slow      -> CDN                                edge user ke paas
   server->user turant     -> WEBSOCKET (ya SSE agar ek-tarfa)   zinda connection
   slow query              -> INDEX (B-tree)                     O(log n)
   join slow, read-heavy   -> DENORMALIZE                        jodke rakh do
   node add/remove         -> CONSISTENT HASHING                 sirf thodi key ghoome
   cache full              -> EVICTION (LRU/LFU/TTL)             jagah banao
   badi list               -> CURSOR PAGINATION                  "last id ke baad"
   text/prefix dhoondhna   -> ELASTICSEARCH / TRIE               inverted index / prefix tree
   bade file               -> S3 + pre-signed URL                DB me sirf link
   multi-service txn       -> SAGA + compensating                distributed rollback
   dead service hammer     -> CIRCUIT BREAKER                    fail-fast, cascade roko
   unique id at scale      -> SNOWFLAKE / range-allocation       DB sequence bottleneck
   message kitni baar      -> at-least-once + IDEMPOTENT         exactly-once ka effect
```

NIYAM: pehle NEED dekho, phir block. Har block ka apna kaam — galat jagah mat lagao.

TRADE-OFF bolna ho -> neeche "A ya B — TRADE-OFF JODE" section (jode + ready English lines).

### ★ (drill se) — menu me jo rows nahi thi

```
   abhi likha, turant padha,  -> READ-YOUR-OWN-WRITES               likhne wale ko thodi der
   purana mila (replica lag)                                        PRIMARY se padhao
   update pe cache stale      -> cache key DELETE (update nahi)     do write ulte kram me = galat value;
                                                                    delete -> agla read DB se bharega
   request kis server pe      -> LB ALGO: round-robin (barabar) · weighted (badi machine) ·
                                 least-conn (request lambi-chhoti) · IP-hash (sticky session)
   bada app, alag scale       -> MICROSERVICES                      alag scale + deploy, ek gira baaki chalu
                                                                    (chhota app = monolith, MS over-engineer)
   login state, kai server    -> JWT (stateless) / session (Redis)  JWT = signature verify, koi store nahi
   server mara, LB ko pata?   -> HEALTH CHECK (LB pull, /health)    2-3 lagataar fail = hatao, 2-3 pass = wapas
                                 HEARTBEAT (server push "zinda")    signal band = mara
   har request naya DB conn   -> CONNECTION POOL (HikariCP)         borrow -> use -> return; size fix -> exhaust
   "1M user -> kitna server?" -> ESTIMATE                           QPS = req/day / 10^5 · peak x2-3 ·
                                                                    storage = req/day x size x (saal x 400)
   frontend alag origin       -> CORS header                        server Access-Control-Allow-Origin
                                                                    (specific origin, prod me "*" nahi)
```

```
   ★ jode, ek line (A ya B):
     LRU vs LFU          -> LRU = kab aakhri baar use (TIME) · LFU = kitni baar use (COUNT)
                            A: 100 baar use, aakhri 1 ghanta pehle · B: 2 baar, aakhri 1 sec pehle
                            -> LRU A hataata, LFU B hataata
     cache-aside vs W-through -> aside = DB update + cache DELETE · through = cache + DB saath likho
     session vs JWT      -> session = har request Redis lookup (stateful) · JWT = token khud saboot
     monolith vs MS      -> chhota / ek team = monolith · alag scale + isolation = MS
     WebSocket vs Kafka  -> WebSocket = server se USER ke browser tak live · Kafka = backend services ke beech
```

```
   ★ design ke waqt 3 reflex (rate-limiter drill se, har shared component pe lagte):
     A. component HAR request pe baitha (middleware)  -> LATENCY sabse zaroori -> in-memory (Redis)
     B. shared state, kai server                      -> EK central store; per-server count = limit toot-ti
     C. wo component mara                             -> default FAIL-OPEN (allow);
                                                         FAIL-CLOSED sirf payment / auth jaise
   ★ TRAP: "load baantna" = SHARDING, CDN nahi. CDN sirf static file (img/video/css), counter nahi.
```

---

## ★ A ya B — TRADE-OFF JODE (02_TRADEOFFS yahan mila, 29-Sep)

> Trade-off koi alag hoshiyari nahi: ek slot pe DO option pata ho + ek wajah. Sirf ek option pata =
> trade-off aa hi nahi sakta (knowledge ka sawaal, dimaag ka nahi). List FINITE hai — har design inhi
> me se 3-4 use karta. Teesra hissa (KEEMAT) sabse zaroori — wahi dikhata ki samajh ke bol raha hai.
> (section 4 wale jode yahan dobara nahi: LRU/LFU · cache-aside/write-through · session/JWT ·
>  monolith/MS · WebSocket/Kafka. Queue vs LB, 301/302, rate-limit algo, modulo vs consistent hashing,
>  offset vs cursor, lock vs idempotency bhi upar section 4/5 me hain.)

```
★ SAANCHA (ratna sirf ye):
   "There are two options here -- A and B.
    I'd go with A, because <requirement jo Step-1 me bola tha>.
    The trade-off is that we give up <B ka faayda>."
   Hinglish soch: "Do raste hain -- A aur B. Main A lunga kyunki requirement ___ hai. Keemat ye ki ___ chhodna padega."
   misaal: "SQL vs Cassandra -- SQL simple par 180B rows pe scale nahi; Cassandra lunga kyunki
            key-value + horizontal scale + HA."
```

```
 1 SQL vs NoSQL       SQL = RISHTE + ACID + flexible query (join/report) · NoSQL = bahut data, flat,
                      access FIX (key se uthana: timeline/logs/chat). wajah: POOCHNA hai ya sirf UTHANA?
                      keemat: SQL -> sharding ka dard · NoSQL -> join/txn khud sambhalo
   BOL: "The access pattern is a simple key lookup at very high volume, so NoSQL. If we needed
         multi-table transactions, I'd flip to a relational DB."            (01 · 03 · 07/10 = SQL)

 2 SYNC vs ASYNC      sync = user ko jawab ABHI chahiye (login, payment authorize, seat check) ·
                      async = intezaar nahi (email, analytics, report). wajah: user screen pe rukega?
                      keemat: async -> eventually consistent, duplicate sambhalo, debug mushkil
   BOL: "The user doesn't need to wait for the email, so I'd push it to a queue and return
         immediately. The cost is that the system becomes eventually consistent."  (13 · 04 · 07 · 08)

 3 PUSH vs PULL       push = server bheje (WebSocket/SSE), real-time · pull = client apni raftaar se
                      maange (poll), slow client dabta nahi. keemat: push -> slow consumer overload +
                      connection sambhalo · pull -> agle poll tak thodi der
   BOL: "Kafka is pull-based -- consumers poll at their own pace, so a slow consumer never gets
         overwhelmed. The trade-off is a little extra latency."             (13 · 04 · 03)

 4 FANOUT WRITE/READ  on-write = post pe sab inbox me daalo, read tez · on-read = maangne pe jodo.
                      keemat: on-write -> celeb pe write-storm · on-read -> har read mehnga. JAWAB = HYBRID
   BOL: "I'd fan out on write for regular users, but for celebrities that means millions of writes
         per post, so for them I'd merge at read time -- a hybrid."          (03 · 09)

 5 WRITE-BACK         (aside/through section 4 me) write-back = pehle cache, DB baad me (async) ->
                      write sabse tez, crash pe DATA LOSS. keemat: aside -> pehla read slow (miss) ·
                      through -> har write slow · back -> sabse risky
   BOL: "Cache-aside as the default. If reads must never be stale, write-through. I'd avoid
         write-back here because a crash would lose data."                   (11 · 01 · 03)

 6 STRONG vs EVENTUAL = CAP CP vs AP   (Q14 bank me bhi)
                      strong/CP = paisa, seat, inventory, DB-leader -> partition me REJECT ·
                      eventual/AP = like-count, feed, cache, search-index, DNS, cart -> jawab do, baad me sudhaaro
                      wajah: "2 sec purana dikhe to kya bigdega?" keemat: strong -> slow + availability girti
   BOL: "For the seat booking itself I need strong consistency -- two users can't get the same
         seat. But the 'seats available' count on the listing can be eventually consistent."
   BOL: "During a network partition I'd rather reject the write than double-book a seat -- so
         this component is CP. The browse/search path can stay AP."          (10 · 07 · 06 · 03)

 7 L4 vs L7 LB        L4 = sirf IP/port, bahut TEZ, andar nahi dekhta · L7 = URL/header/cookie ->
                      path routing, sticky session, TLS terminate; thoda slow par samajhdar
   BOL: "I'd use an L7 load balancer because I want to route /api and /static differently and
         terminate TLS there. L4 would be faster but can't see the request."  (har design)

 8 VERTICAL vs HORIZ. vertical = badi machine, simple, par CEILING + SPOF · horizontal = zyada machine,
                      HA, par state/shard/coordination ka dard
   BOL: "Vertical scaling is simpler and I'd use it early, but it has a ceiling and it's a single
         point of failure -- past that I'd scale horizontally behind a load balancer."

 9 REPLICATION vs SHARDING   replica = ek data KAI copy -> READ scale + HA · shard = TUKDE -> WRITE
                      scale + storage. keemat: replica -> lag (stale read) · shard -> cross-shard join,
                      re-shard dard, hot shard
   BOL: "Reads are the bottleneck, so read replicas first. If the write volume itself outgrows one
         machine, then I'd shard by userId."                                 (01 · 03 · 07 · 13)

10 NORMALIZE vs DENORM.  normalize = ek jagah, update aasan, JOIN chahiye · denormalize = copy saath
                      rakho, read tez, update kai jagah. wajah: read-heavy ya update-heavy?
   BOL: "This is read-heavy and the join was the bottleneck, so I'd denormalize and store the
         author's name with the post. The cost is updating it in two places." (03 · 09 · 10)

11 LONG-POLL vs WS vs SSE   long-poll = server request rok ke rakhe, simple par mehnga · WebSocket =
                      dono taraf (chat, trading, collab) · SSE = sirf server->client, halka, HTTP pe.
                      keemat: WS -> har connection zinda rakho (memory + sticky LB)
   BOL: "Chat is bidirectional, so WebSocket. If it were only server-to-client updates, SSE
         would be lighter."                                                  (15 · 12 · 06 · 04)

12 AT-LEAST vs EXACTLY-ONCE  at-least = kaam pehle, commit baad, duplicate aa sakta (default) ·
                      exactly = mehnga, har jagah possible nahi. JAWAB = at-least-once + idempotent consumer
   BOL: "I'd take at-least-once delivery and make the consumer idempotent using an event id,
         rather than paying for true exactly-once."                          (13 · 07 · 04)

13 BATCH vs STREAM    batch = raat ko ek saath (report, billing, reconciliation), sasta, DER se ·
                      stream = event aate hi (fraud, live dashboard, alert), turant par mehnga+complex
   BOL: "Daily reconciliation can be a batch job. Fraud detection can't wait, so that one has
         to be a stream."                                                    (07 · 09)

14 SINGLE / MULTI-LEADER / LEADERLESS   single = ek jagah write, conflict nahi, par leader bottleneck
                      + SPOF · multi = kai region me local write tez, par CONFLICT · leaderless = kisi
                      ko bhi likho, quorum se padho (Dynamo/Cassandra), HA par version conflict.
                      keemat: conflict resolution likhna (last-write-wins / CRDT)
   BOL: "I'd keep a single leader for writes so there are no conflicts. If we needed low write
         latency in multiple regions, we'd go multi-leader and pay for conflict resolution."
                                                                             (07 · 11 · 12)
```

```
★ CHHOTE JODE (ek line):
   CDN pull vs push -> pull = pehli request pe origin se le aata (simple) · push = pehle se bhar do
                       (bada file / launch)
   Bloom filter     -> "pakka NAHI hai" ka sasta check, DB hit bachata (false-positive ok,
                       false-negative kabhi nahi)
   gehra "farak batao" (SQL/NoSQL, WS/SSE, Kafka/RabbitMQ...) = ../06_COMPARES

★ KAISE USE: roz 3 jode (hafte me poori list) · design revise karte rok ke poochho "is slot pe
  DOOSRA option kya tha?" -- jodne se yaad rehta, alag ratne se nahi · mock me 2 trade-off bolo
  (section 8) · kuch na yaad aaye -> section 7 ki SAFE TRADE-OFF LINE.
```

---

## 5. 15 PADHE HUE DESIGN — DIL + KHAAS HISSA (jo PADHNA padta hai)

> Common dabbe (LB · replica · cache · shard · queue · CDN · S3) har design me wahi — wo derive ho jaate.
> Neeche sirf wo jo us design ka APNA hai, jise bina padhe bol nahi sakte (Arpan ka nichod, 25-Sep).
> Detail = SYSTEM_DESIGNS/<naam>. Revise karte waqt bas ye section.

```
01 URL-SHORTENER   DIL: chhota unique code + tez redirect
   . COUNTER + BASE62: counter kabhi repeat nahi -> collision nahi; /62 ke remainder ulte = code; 7 char = 3.5 trillion
   . RANGE ALLOCATION: coordinator har server ko block (1..1000) de, server local chalaye; crash = baaki range waste (manzoor)
   . 302 (301 nahi, browser cache kar leta -> click gine nahi jaate) · custom alias = UNIQUE constraint -> 409

02 RATE-LIMITER    DIL: over-limit reject, legit allow
   . 4 ALGO: TOKEN bucket (refill, burst OK) · LEAKY bucket (fixed rate nikle) ·
             FIXED window (edge pe 2x ka bug) · SLIDING window (sahi, par memory zyada)
   . Redis INCR atomic; kai step = Lua; EXPIRE sirf PEHLI baar (count==1), warna key kabhi reset nahi
   . 429 + Retry-After · fail-OPEN vs fail-CLOSED · per-user limit bheed se nahi bachata (load shedding alag)

03 TWITTER-FEED    DIL: read sasta ho
   . FANOUT ON WRITE: post pe follower ke inbox me sirf tweet_id (LPUSH), LTRIM 800 se capped
   . HYBRID: ~10K se kam follower = push, celeb = pull; read = inbox + celeb tweets -> merge -> sort -> top 50
   . fanout async (Kafka) -> "tweet bana" turant, "sab tak pahuncha" baad me

04 NOTIFICATION    DIL: ek event -> sahi channel / user / time
   . brain: preference (channel, quiet hours) + template -> per-CHANNEL queue + worker
   . idempotent worker (SET id NX EX) + backoff x 2^n + jitter -> retry queue -> DLQ + circuit breaker
   . PRIORITY lane (OTP alag, promo ke peeche nahi) · "accepted" != "delivered" (webhook)

05 BROWSER-JOURNEY (design nahi, concept)   URL -> DNS -> TCP -> TLS -> HTTP -> render

06 STOCK-BROKER    DIL: order MATCH + paisa/share sahi
   . MATCHING ENGINE: har symbol ka EK thread, lock nahi; asks sasta-pehle, bids mehnga-pehle;
     best-bid >= best-ask -> match; price-time priority; scale SYMBOL se, ek symbol ke andar kabhi nahi
   . order pe paisa BLOCK, match pe debit · settlement ACID + double-entry
   . EVENT LOG / SEQUENCER: pehle log, phir book; crash = replay (WAL wali soch) · live price = pub/sub

07 PAYMENT         DIL: paisa DO BAAR na kate
   . IDEMPOTENCY KEY: client har tap pe UUID, retry pe same; server STORED RESULT lautaye (reject nahi)
     claim = UNIQUE constraint / SETNX, IN_PROGRESS -> DONE, key ~24h
   . PENDING pehle likho, phir PSP call -> webhook (push) + reconciliation (pull) dono
   . LEDGER double-entry, immutable (galti = nayi correction entry) · SAGA (compensate) vs 2PC (lock, coordinator atke)

08 FILE-UPLOAD     DIL: upload -> validate -> track
   . PRESIGNED URL: bytes client <-> S3 seedha, server sirf link (GET bhi, 5-15 min)
   . MULTIPART: 5 MB tukde, sirf fail tukda dobara · tmp/ + lifecycle (DINO me) + abort-incomplete
   . trackingId + status UPLOADING -> VALIDATING -> DONE/FAILED · ownerId authz · MAGIC BYTES (naam pe bharosa nahi)

09 NEWS-AGGREGATOR DIL: kai source -> ek feed
   . WRITE path (crawl) aur READ path (feed) ALAG, ek doosre ko dheema na karein
   . worker me CLEAN + DEDUPE (ek khabar 5 source) + CATEGORY · har source alag timeout/retry/skip
   . category-wise cache merge (10 lakh fanout se bache) · RETENTION != sharding

10 BOOKMYSHOW      DIL: do log EK seat na lein
   . ATOMIC: UPDATE seats SET status='booked' WHERE seat_id=? AND status='available' -> 1 row jeeta, 0 = gayi
   . HOLD: status='held' + held_until (5 min); pay -> booked, time gaya -> available
   . 2 user ek seat = atomic mark · 1 user double click = idempotency · spike = queue + per-show worker

11 DISTRIBUTED-CACHE DIL: speed + node mare to chale
   . CONSISTENT HASHING: ring, key clockwise agle node pe; node add/remove pe sirf ~K/N keys hilti; VIRTUAL nodes
     (hash % N pe lagbhag sab keys shift)
   . LRU = HashMap + doubly linked list, dono O(1) · stampede: mutex / soft-TTL · hot key: replicate + L1 local

12 GOOGLE-DOCS     DIL: saath edit, kuch na khoye, sab same
   . TEXT nahi, OPERATION bhejo ({insert "X", pos 0})
   . OT: winner mat chuno, TRANSFORM karo (baad wale ki position shift), tie-break deterministic
     CRDT: har char ki unique id, merge apne aap, central server nahi chahiye
   . snapshot + baad ke ops · shard by docId (OT ek jagah serialize) · edits AP, permissions CP

13 MESSAGE-QUEUE   DIL: kisi ko roko mat, kuch kho na jaaye
   . APPEND-ONLY LOG + OFFSET (kram-number): padh ke delete nahi, consumer apna offset rakhe -> replay
   . key -> partition = ek key ka ORDER · CONSUMER GROUP: group me baanto, alag group = sabko poora
     parallelism = partition count
   . ISR + acks (0/1/all) · at-least-once + idempotent consumer (eventId) · partition badhane pe order toot sakta

14 BANKING         DIL: paisa na bane na mare
   . ek DB = @Transactional (saga NAHI); cross-bank = saga
   . DOUBLE-ENTRY LEDGER (jod hamesha 0) · BALANCE = derived, USI txn me update · raat ko reconciliation (ledger jeete)
   . overdraft: UPDATE ... WHERE balance >= x (0 rows = mana) · deadlock: account-id ke kram me lock · outbox

15 CHAT            DIL: turant pahunche, offline pe na khoye
   . connection + register: kaun kis server pe (Redis me sirf PATA) · pub-sub server-to-server
   . PEHLE DB me likho, PHIR bhejo · catch-up "id X ke baad ka do"
   . CURSOR (read_upto / delivered_upto) = unread + ticks ek hi idea · clientMsgId (retry dedup)
   . order SERVER id se (client time nahi) · presence = TTL + heartbeat · group = fan-out on READ

★ USE: design ka naam suno -> yahan se DIL + KHAAS hissa -> baaki common dabbe dikkat pe lagao.
```

---

## 6. ★ ANJAAN DESIGN AA JAAYE TO — 5 MINUTE RECIPE

```
   1. ARCHETYPE pehchano (section 2). Bade product ho to 2-3 sub-system me TODO -- aur ye BOLO.
   2. DIL bolo: "the heart of this is ___"
   3. FR 4 line + NFR (read/write-heavy, consistency strong ya eventual)
   4. Estimate -> ek line: "iska matlab shard/cache/queue chahiye"
   5. Archetype ke DEFAULT BLOCKS utha ke boxes bana do
   6. Deep-dive = us archetype ka SIGNATURE Q
   7. Bottleneck = hot-key / DB-write / ek queue
```

### WORKED EXAMPLE — "Design LinkedIn" (bilkul aise bolna)

```
   "LinkedIn is really three systems, so let me split it and then go deep on one:
      (a) the FEED -- read-heavy
      (b) SEARCH -- people and jobs
      (c) MESSAGING -- real-time
    Which one would you like me to focus on? I'll assume the feed."

   FEED = archetype A -> DIL = read sasta ho
      FR   : post banao . connections ki feed dekho . like/comment
      NFR  : read:write ~ 100:1 . feed <200ms . eventual consistency theek
      EST  : 1M DAU x 10 feed-open/day = 10M reads/day (~120/sec, peak 400/sec); posts 100k/day
      DATA : posts (postId, authorId, text, mediaUrl, ts)  [NoSQL, flat + huge]
             connections (userId, connId)                  [graph-ish; SQL ya graph-DB]
             timeline-cache (userId -> [postId...])        [Redis list]
      BOXES: client -> CDN(media) -> LB -> feed-service -> Redis timeline -> posts-DB
                                          |
                                          +-> Kafka -> fanout-worker -> followers ki timeline me daalo
      DEEP : fanout-on-write default; ek banda jiske 5 lakh follower hain ->
             uske liye fanout-on-read (hybrid). Media S3+CDN pe.
      BOTTLE: hot post / big account -> write-storm -> hybrid + cache
              10x -> shard posts by postId, timeline-cache shard by userId

   SEARCH  (agar wo poochein) = archetype E -> Elasticsearch + async indexer + top-K cache
   MESSAGING (agar wo poochein) = archetype D -> WebSocket + connection-registry + pub/sub +
              offline push + Cassandra message-store
```

---

## 7. JAB KUCH NA PATA HO — 4 LINE (ratni hain)

```
   1. "I haven't worked with that specific piece, but here's how I'd reason about it..."
   2. Fundamentals se todo, LOUD: "it's read-heavy, so cache first; writes are the risk, so a queue..."
   3. Clarifying question poochho -> time milta hai aur interviewer khud nudge deta hai
   4. Jaani-hui cheez se jodo: "this looks like X, where I did ___"

   ★ MAT karna: bluff. Galat-confident jawab follow-up me turant pakda jaata --
     aur wahi asli nuksaan karta hai, "pata nahi" se zyada.

   ★ SAFE TRADE-OFF LINE (jab jodi yaad na aaye):
     "There's a trade-off here -- the other option would be X, but given the requirement
      I stated earlier, I'd stay with this one."
```

---

## 8. INTERVIEW-DIN CHECKLIST (10 line, bas itna)

```
   [ ] shuru me RAIL announce karo ("I'll go requirements -> estimate -> API -> data model ->
       high-level -> deep-dive -> bottlenecks")
   [ ] 2-3 clarifying question (scope bhi poochho: "kya ye scope me nahi hai?")
   [ ] DIL ek line me bolo
   [ ] numbers bolo (QPS + storage) aur turant unse ek DECISION nikaalo
   [ ] har box ke saath "kyun"
   [ ] kam-se-kam 2 TRADE-OFF (dono taraf -- "A chuna, B ki keemat ye")
   [ ] deep-dive 2-3 cheez, poora nahi
   [ ] "scale 10x" ka jawab ready (shard/cache/replica/async)
   [ ] chup mat raho -- soch LOUD bolo
   [ ] na aaye to honest + reason (section 7)
```

---

## ★ KAISE BOLNA (01_DELIVERY PART 1 yahan mila, 29-Sep)

> Rail (section 3), scope kaato, core entities, har box naam + ek "kyun", teen niyam (upar),
> na-pata-ho 4 line (section 7), checklist (section 8) pehle se is file me hain. Neeche sirf jo baaki tha.
> Common follow-ups ("scale 10x", "gira to", CAP, race, hot key, data lost, monitoring) = CROSS-QUESTION BANK.

```
HLD ALAG KYUN LAGTA: DSA = binary (pattern hai/nahi) · Java = fixed sawaal-jawab · HLD = OPEN game,
   answer-key nahi, interviewer SAATH chalata. Grade = soch + communication + trade-off + ambiguity me
   aage badhna. "Perfect complete answer" grade hi nahi hota. Khula != blank: rail hamesha hai,
   aur rail TU lead karta hai, interviewer wait nahi karta.

ESTIMATE: 30 second, scale justify karo ("read-heavy, billions -> cache + shard"), aage badho.
   1500 vs 2000 RPS design nahi badalta. PAR poora SKIP bhi nahi -- ek banda Zomato me reject hua,
   BOTE chhod ke seedha distributed pe kood gaya tha.

TEEN NIYAM ki misaal (15-Sep, asli mock videos):
   chhote se shuru: Spotify / Bitly dono me pehle 4 box (app / LB / server / DB), phir jahan toota
      wahan CDN / cache / S3 / shard joda.
   USER banke chalo: app kholi -> search -> har search DB pe?       -> cache
                     play dabaya -> 5 MB stream -> gaana viral?      -> CDN
   number: 1000 ya 1200, farak nahi. mota andaaza, ek decision, aage.
```

```
INTERVIEWER KYA DEKHTA HAI (16-Sep, ex-Google EM "10 signals"):
   ★ "Domain knowledge is NOT a quiz -- it's the art of APPLYING components."
     component ka lecture nahi; SAHI JAGAH lagao + wajah bolo.
   1 BOILERPLATE me mat ulajho: LB / gateway / CDN ek line me, waqt DIL pe (matching, fanout,
     dedupe, seat-lock). box ginne se score nahi milta.
   2 PARKING LOT: "There's an optimization here -- caching. Let me park it and come back once
     the core flow is right."  (dikh bhi gaya, design saaf bhi raha)
   3 LISTENING: unki baat apne shabdon me dohrao ("so you want me to focus only on playback --
     right?") + beech me chhota pause.
   4 CONCISE: board pe chhote label, poora vaakya muh se.
   5 SHARING: soch bolo ("I'm thinking this because..."), sirf nateeja nahi.
   6 FLEXIBLE: requirement badli / feedback aaya -> approach badlo, zidd nahi.
   7 TEST your design: apna flow chala ke edge case pakdo (= user banke chalo).
   8 CHOICES: har faisle ka "kyun", ittefaq se kuch nahi.
   9 SCALING: BOTE dikhao, usi me atko mat.
   ⚠ RED FLAGS: defensive hona / feedback pe bahas · requirements me 5-8 min se zyada ·
     har optimization turant thokna (parking lot use karo).

★ JP / FINANCE: deep-dive me ye teen shabd DROP karo: IDEMPOTENCY · AUDIT-TRAIL / LEDGER · ACID jahan paisa.
```

```
NA PATA HO -- section 7 ke upar jo baaki tha:
   COOL-DOWN: panic ki jad = "mujhe pata HONA chahiye". Replace: "main ise REASON karunga."
   5th escape: ASSUME + MOVE -> "I'll assume X and move on."   (kabhi FREEZE / chup nahi)
   RATTI LINE: "I haven't worked with X directly, but I'd approach it by ___. Let me note it and continue."
   per subject: DSA  -> brute force se shuru, loud ("naive O(n^2) is this, now optimize")
                JAVA -> "I don't remember the exact API, but my understanding is..." + reason
                HLD  -> block menu se reason; single right answer hota hi nahi, REASONING hi jawab

   ANJAAN DESIGN: design yaad nahi karte -- wahi ~15 BLOCK ki nayi jodi. Rail pe DERIVE karo:
      Google Docs -> "concurrent edits clash -> I'd sequence operations per doc" (OT/CRDT naam na
      aaye tab bhi reason). JP backend me Google-scale kam; rate-limiter / payment / notification /
      url type tere paas hain.

   "KUCH NAHI AAYA" ka darr:
      1-3 cheez na aana har candidate ke saath GUARANTEED. grade = na-aane pe kaise sambhala.
      ek unknown se round bekaar nahi; poora round grade hota, JP = 2/3 round tera zone.
      asli khatra = unknown nahi, SPIRAL (fail lag raha -> panic -> baaki bikhra). unknown ko spiral se alag rakho.
      "nahi aata par aise nikaalunga" = POSITIVE signal, bluff se zyada izzat.
      ANCHOR: 4 saal + 700 prod ticket, "2am prod down, pata nahi kya toota" -- ye usse aasaan.

META-SACH: perfect answer koi nahi deta · nerves + kuch chhootna normal · HLD = DIALOGUE, interviewer
   nudge karta · bolna TRAINABLE hai (mock se aata) · pehla interview = pehla rep, final nahi.
MOCK me coach kya dekhta: rail pe chala? · har box kyun? · trade-off saanche me? · atakne pe navigate?
   · pichhli baar se behtar? (poora-hai-ki-nahi NAHI)

1-LINE RECALL: RAIL pakdo -> har box naam + why -> trade-off saanche me -> atko to escape (kabhi chup
   nahi). Perfect nahi, NAVIGATE. Anjaan = blocks se derive. "Nahi aata" = ratti line + reason, spiral se alag.
```

---

## ★ SHABD — word atke to (01_DELIVERY PART 2 yahan mila, 29-Sep — poora rakha)

> **Kyun:** concept 100% aata (Hindi me round faad de). Sirf interview me English **word tongue pe**
> nahi aata -> us line pe freeze. Ye gap CHHOTA + FINITE hai. Yahan wo word + EXACT line.
> **KAISE:** padho mat — **LOUD BOLO**. Har topic ki "one breath" line 2-3 baar zabaan se nikaalo.
> **Yaad rakh:** soch tere paas HAI, ye sirf word-swap hai (bucket khatam -> "bucket is empty").
> Native banna zaroori nahi — clear soch + sahi word kaafi. JP-Bangalore Hinglish-ok.

### 0. UNIVERSAL HLD VERBS (har design me — connective tissue)

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

### 1. RATE LIMITER (token bucket)

| tera Hindi | English word | line |
|---|---|---|
| bucket se token liya | **consume** (borrow nahi — token wapas nahi jaata) | "each request *consumes* a token from the bucket" |
| bucket bharta rehta | **refill / replenish** | "the bucket *refills* at a steady rate, say 10 tokens/sec" |
| bucket ki size | **capacity** | "the bucket has a fixed *capacity*" |
| ek saath thode zyada allow | **burst** | "this allows short *bursts* up to the bucket size" |
| bucket khatam / saare token use | **empty / exhausted** | "once the bucket is *empty*..." |
| request rok do | **throttle** | "extra requests are *throttled*" |
| mana kar do | **reject** | "the request is *rejected* with a 429 Too Many Requests" |
| ek jaisi speed | **steady rate** | "it smooths traffic to a *steady rate*" |

**ONE BREATH:** *"A token bucket has a fixed capacity and refills at a steady rate. Each request consumes a token. If tokens are available it's allowed, otherwise it's throttled and rejected with a 429 — this handles short bursts while keeping a steady average rate."*

### 2. CACHING

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

### 3. LOAD BALANCING

| tera Hindi | English word | line |
|---|---|---|
| load baant do | **distribute** | "the load balancer *distributes* requests across servers" |
| baari-baari | **round robin** | "a simple strategy is *round robin*" |
| server zinda hai? check | **health check** | "it does periodic *health checks* on each server" |
| kharab server hata do | **remove from pool** | "an unhealthy server is *removed from the pool*" |
| ek user ek server pe | **sticky session** | "*sticky sessions* pin a user to one server" |

**ONE BREATH:** *"A load balancer sits in front and distributes incoming requests across servers, using strategies like round robin. It runs health checks and removes unhealthy servers from the pool, so traffic only goes to healthy nodes."*

### 4. DATABASE — REPLICATION & SHARDING

| tera Hindi | English word | line |
|---|---|---|
| data ki copy | **replica** | "we keep read *replicas* of the database" |
| likhne wala main DB | **primary / leader** | "writes go to the *primary*, reads to the replicas" |
| copy update hone me delay | **replication lag** | "there can be some *replication lag*" |
| data tukdo me baant do | **shard / partition** | "we *shard* the data across multiple databases" |
| kis shard me jaye | **shard key** | "we pick a *shard key*, like user id" |
| ek shard pe zyada load | **hot partition** | "a bad shard key can cause a *hot partition*" |

**ONE BREATH:** *"We use a primary for writes and read replicas for reads to scale reads, accepting some replication lag. For write scale we shard the data across databases using a shard key like user id, being careful to avoid hot partitions."*

### 5. ASYNC / MESSAGE QUEUES

| tera Hindi | English word | line |
|---|---|---|
| kaam baad me karo | **asynchronous** | "we process it *asynchronously*" |
| beech me queue | **queue / broker** | "requests go into a *message queue* like Kafka" |
| daalne wala | **producer** | "the *producer* publishes the event" |
| uthane wala | **consumer** | "a *consumer* picks it up and processes it" |
| load ka jhatka jhelo | **buffer / absorb spikes** | "the queue *buffers* traffic and absorbs spikes" |
| ek hi baar effect ho | **idempotent** | "processing is *idempotent*, so retries are safe" |

**ONE BREATH:** *"Instead of doing it inline, we push the work to a message queue. The producer publishes an event and a consumer processes it asynchronously. This decouples the services and lets the queue absorb traffic spikes. We make processing idempotent so retries are safe."*

### 6. CONSISTENCY / CAP

| tera Hindi | English word | line |
|---|---|---|
| har jagah same data | **strong consistency** | "banking needs *strong consistency*" |
| thodi der me sab same | **eventual consistency** | "for feeds, *eventual consistency* is fine" |
| network toot gaya | **network partition** | "during a *network partition*, we must choose" |
| response time | **latency** | "this reduces *latency* for the user" |
| ek saath kitne handle | **throughput** | "it increases the system's *throughput*" |
| do log ek seat, ek hi jeete | **conditional update** (check WHERE me) | "only one succeeds, because the check is *inside the WHERE*: `UPDATE seats SET status='booked' WHERE seat_id='A1' AND status='available'`" |

★ AADAT (27-Sep, BookMyShow): "atomic" / "race nahi hogi" bole to SAATH me query bhi bolo — haath roz WHERE lagata hai, muh se bhi nikle. `save()` sirf id pe UPDATE karta, status check nahi -> isliye ye line zaroori.

**ONE BREATH:** *"By CAP, during a network partition we choose between consistency and availability. Payments need strong consistency, but for something like a news feed, eventual consistency is acceptable to keep latency low and availability high."*

### 7. RELIABILITY / FAILURE

| tera Hindi | English word | line |
|---|---|---|
| ek point fail = sab fail | **single point of failure (SPOF)** | "we remove any *single point of failure*" |
| backup pe switch | **failover** | "on failure we *failover* to a standby" |
| dobara koshish | **retry with backoff** | "the client *retries with exponential backoff*" |
| girta hua system bacha lo | **circuit breaker** | "a *circuit breaker* stops calling a failing service" |
| thoda-thoda kaam karta rahe | **graceful degradation** | "the system *degrades gracefully* instead of crashing" |

**ONE BREATH:** *"We avoid single points of failure by replicating components and using failover to standbys. Clients retry with backoff, and a circuit breaker protects against a failing downstream service, so the system degrades gracefully instead of going fully down."*

### HOW TO DRILL (roz 5 min)
1. Ek topic uthao -> **one-breath line LOUD bolo** 2-3 baar.
2. Phir file band karke wahi concept **apne words me English me bolo** (Hindi soch -> English word).
3. Atka? -> word table dekh lo -> dobara bolo. (jaise DSA nudge.)
4. Roz 1-2 topic. Ghis-ghis ke tongue pe chadhega. **Bolna hai, padhna nahi.**
