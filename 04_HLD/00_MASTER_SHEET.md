# HLD MASTER SHEET — koi bhi design ASSEMBLE karne ka tareeka

> **NAV** — KYA: interview-din ki EK file (archetype -> assemble -> bolo). · KAISE-BOLNA + SHABD: [01_DELIVERY](01_DELIVERY.md) · A YA B: [02_TRADEOFFS](02_TRADEOFFS.md) · CONCEPT-detail: [FOUNDATIONS](FOUNDATIONS) · 15 DESIGN: [SYSTEM_DESIGNS](SYSTEM_DESIGNS) · DRILL: [RAW_DRILL/00_DRILL](RAW_DRILL/00_DRILL.md)

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
     00_MASTER_SHEET.md    <- YE. KYA bolna: 6 sawaal · archetype · rail · blocks · 15 design ka DIL
     01_DELIVERY.md        <- KAISE bolna (meta-moves, na-pata-ho-to, follow-ups) + SHABD (word atke to)
     02_TRADEOFFS.md       <- A ya B: 15 jode + chhote jode + ready English line

   PADHNE / DEPTH
     FOUNDATIONS/01..18    <- ek concept = ek file
        01 hld · 02 capacity · 03 LB · 04 cache · 05 replication · 06 sharding · 07 queue
        08 CAP · 09 db kaunsa · 10 microservice baat-cheet · 11 SPOF · 12 search · 13 id
        14 HUB (bachane wala hi girane wala) · 15 CDN · 16 DNS · 17 WAF · 18 monitoring
     SYSTEM_DESIGNS/01..15 <- 15 poore design (05 = concept walkthrough)

   DRILL / ABHYAS
     RAW_DRILL/00_DRILL.md                <- PART 1 need -> block reflex · PART 2 kyun/kaise Q&A
     RAW_DRILL/01_DESIGN_PRACTICE_LOG.md  <- bolke kiye design ka log (naya yahin)

   BAHAR (zaroorat pe)
     ../06_COMPARES/       <- gehre "farak batao" explainer (TCP/UDP, HTTP versions, JWT/OAuth...)
     ../05_INFRA_DEEP/     <- sirf DEVOPS commands + hands-on demo folder (LB / monitoring / load-test)
```
```
   ★ CONFUSION-PROOF NIYAM:
       "kya bolna"         -> ye file
       "kaise bolna"       -> 01_DELIVERY
       "A ya B?"           -> 02_TRADEOFFS
       "ye cheez hai kya?" -> FOUNDATIONS
       "poora design"      -> SYSTEM_DESIGNS
       "khud ko test"      -> RAW_DRILL/00_DRILL
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
> Detail + misaal = [01_DELIVERY](01_DELIVERY.md) section 5b.

---

## ★★ HAR DESIGN PE 6 SAWAAL — dikkatein yaad nahi karni, ye nikaal dete hain (26-Sep, Arpan ka sawaal)

> "Itni dikkatein yaad kaise rahengi?" -> yaad sirf YE 6. Har dabbe / har kadam pe inme se ek poochho,
> dikkat khud nikal aati. SAWAAL har design me SAME, JAWAB design ke hisaab se badalta.

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

> Detail + 31-row table = RAW_DRILL/00_DRILL.md. Ye compact version interview-din ke liye.

```
   read baar-baar          -> CACHE (Redis, cache-aside + TTL)   RAM, disk se 100x tez
   read phir bhi DB pe     -> READ REPLICA                       reads baant do
   write/storage nahi sama -> SHARDING (+ shard key)             data tukdon me
   spike / burst           -> QUEUE                              requests HOLD karo (LB nahi)
   slow kaam, decouple     -> QUEUE + WORKER                     user block na ho
   traffic baantna         -> LOAD BALANCER (L7)                 content-based routing + TLS
   kai service, ek darwaza -> API GATEWAY                        auth+routing+rate-limit ek jagah
   bhaari file stream/DL   -> SERVER SIRF LINK DE (presigned)    bytes client<->S3 seedha,
                                                                 warna server bandwidth marta
   machine mare            -> REPLICATION (leader/follower)      copy se kaam chale
   paisa / seat            -> SQL + LOCK/unique + CP             galat data NEVER
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

TRADE-OFF bolna ho -> `04_HLD/02_TRADEOFFS.md` (15 jode + ready English lines).

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
