# HLD MASTER SHEET — koi bhi design ASSEMBLE karne ka tareeka

> **NAV** — KYA: interview-din ki EK file (archetype -> assemble -> bolo). · KAISE-BOLNA: [APPROACH_DELIVERY](HLD_APPROACH_DELIVERY.md) · TRADE-OFF: [TRADEOFFS](TRADEOFFS.md) · SHABD: [SPEAKING_VOCAB](SPEAKING_VOCAB.md) · CONCEPT-detail: [FOUNDATIONS](FOUNDATIONS) · 13 DESIGN: [SYSTEM_DESIGNS](SYSTEM_DESIGNS) · DRILL: [RAW_DRILL](RAW_DRILL)

> EK file. Interview se pehle sirf YE. (detail chahiye to hi SYSTEM_DESIGNS/* kholo.)
>
> IDEA: har design naya nahi hota. Har design **6 archetype** me se kisi ek (ya do ke mel) me girta hai.
> Archetype pehchano -> uska DIL pata -> default blocks lagao -> RAIL pe bol do.
>
> ★ HONEST HAD: ye sheet kisi bhi design ka HIGH-LEVEL khada kar degi (boxes + kyun + trade-off).
>   Jo cheez tune PADHI hi nahi uska DEEP-DIVE ye sheet nahi degi -- wahan section 7 wali
>   honest line kaam aati hai. Ye kami nahi, ye tareeka hai.

---

## 0. KAUNSI FILE KAB — poore 04_HLD ka naksha (har file ka ek hi kaam)

```
   INTERVIEW-DIN / REVISE
     00_MASTER_SHEET.md          <- YE. kya bolna: archetype -> blocks -> rail. (tu abhi yahin hai)
     HLD_APPROACH_DELIVERY.md    <- KAISE bolna: meta-moves, trade-off saancha, na-pata-ho-to, follow-ups
     TRADEOFFS.md                <- 15 A-vs-B jode + ready English line
     SPEAKING_VOCAB.md           <- shabd atak jaaye to

   PADHNE / DEPTH
     FOUNDATIONS/01..13          <- ek-ek concept ka detail (cache, shard, CAP, queue...)
     SYSTEM_DESIGNS/01..13       <- 13 poore design, har ek 7-step rail pe

   DRILL / ABHYAS
     RAW_DRILL/00_REFLEX_SHEET   <- rapid-fire: need -> block (design ke bina)
     RAW_DRILL/02_WHY_HOW_QA     <- kyun/kaise Q&A, khud se poochho
     RAW_DRILL/01_PRACTICE_LOG   <- purane mock ka log (archive) + naya mock yahin likho
```
```
   ★ CONFUSION-PROOF NIYAM:
       "kya bolna"   -> ye file
       "kaise bolna" -> APPROACH_DELIVERY
       "A ya B?"     -> TRADEOFFS
       "ye cheez hai kya?" -> FOUNDATIONS
       "poora design"      -> SYSTEM_DESIGNS
     Har file ke UPAR ek NAV line hai -> wahan se seedha jump. README pe wapas jaane ki zaroorat nahi.
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
   . 2-3 clarifying Q poochho: kitne user? read-heavy ya write-heavy? real-time chahiye?
     kya scope me NAHI hai?
   . FR = 4-5 line, user ki bhasha me ("user post kare", "user feed dekhe")
   . NFR = scale . latency . availability . consistency (STRONG ya EVENTUAL -- ye bolna ZAROORI)
   . ★ AAKHIR ME DIL BOLO: "the heart of this problem is ___"   <- yahi poore interview ka anchor

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

> Detail + 31-row table = RAW_DRILL/00_REFLEX_SHEET.md. Ye compact version interview-din ke liye.

```
   read baar-baar          -> CACHE (Redis, cache-aside + TTL)   RAM, disk se 100x tez
   read phir bhi DB pe     -> READ REPLICA                       reads baant do
   write/storage nahi sama -> SHARDING (+ shard key)             data tukdon me
   spike / burst           -> QUEUE                              requests HOLD karo (LB nahi)
   slow kaam, decouple     -> QUEUE + WORKER                     user block na ho
   traffic baantna         -> LOAD BALANCER (L7)                 content-based routing + TLS
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

TRADE-OFF bolna ho -> `04_HLD/TRADEOFFS.md` (15 jode + ready English lines).

---

## 5. 13 PADHE HUE DESIGN — sirf DIL (mix na ho isliye, ek line each)

```
   url-shortener     -> chhota unique code + tez redirect      (ID-gen + cache + 302)
   rate-limiter      -> over-limit reject, legit allow         (Redis atomic counter + token-bucket)
   twitter-feed      -> read SASTA ho                          (fanout-on-write + celeb hybrid)
   notification      -> ek event -> sahi channel/user/time     (queue + per-channel worker + retry/DLQ)
   browser-journey   -> Enter dabane ke baad kya-kya hota      (DNS->TCP->TLS->HTTP->render)
   stock-broker      -> order MATCH + paisa/share consistent   (matching engine + ledger + low latency)
   payment           -> paisa DO BAAR na kate                  (idempotency-key + ledger + saga)
   file-upload       -> upload -> validate -> track            (S3 presigned + async validate + status)
   news-aggregator   -> kai source -> ek feed                  (crawl/ingest + dedup + rank + cache)
   bookmyshow        -> do log EK seat na lein                 (atomic hold/lock + TTL + payment)
   distributed-cache -> SPEED, + node mare to chale            (consistent hashing + LRU + replica)
   google-docs       -> saath edit, kuch na khoye, sab same    (WebSocket + OT/CRDT + convergence)
   message-queue     -> kisi ko rokna nahi, kho na jaye        (partition+offset + group + replication)

   ★ USE: kisi bhi design me confusion ho -> "iska DIL kya hai?" -> upar se uthao.
     Dil sahi to blocks kahin se bhi lagao, jawab sahi rahega. Dil galat = jawab galat.
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
