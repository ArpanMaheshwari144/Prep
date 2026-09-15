# URL Shortener — BOLNE KA POORA KRAM (jaise asli round me hota hai)

> **NAV** — ARCHETYPE F (infra/component) · DIL: chhota unique code + tez redirect. UP: [MASTER](../../00_MASTER_SHEET.md) · CONCEPTS: [ID-gen](../../FOUNDATIONS/13_distributed_id_snowflake.md) · [caching](../../FOUNDATIONS/04_caching.md) · [sharding](../../FOUNDATIONS/06_database_sharding.md) · trade-off: [301 vs 302](../../TRADEOFFS.md)

> Arpan-derived 21-Jun · 7-step me merge 7-Sep · **15-Sep: asli mock-video ke kram pe dobara likha**
> (poochna -> board ka pehla block -> numbers -> chhote boxes -> dikkat pe badhao -> deep dive).
> Problem (1 line): long URL -> short 6-7 char code; short pe click -> original pe redirect (302).

```
   https://amazon.in/dp/B0BLKJSDKFJ934KJSDF/ref=...      (long)
                          │
                          ▼
                    bit.ly/abc123                         (short)
```

```
★★ TEEN NIYAM (ye poori file par lagte -- [APPROACH_DELIVERY](../../HLD_APPROACH_DELIVERY.md) 5b):
   1. PERFECT design ek saath mat banao -- chhote se shuru, dikkat pe badhao
   2. NUMBER ke peeche mat bhaago -- bolo, ek faisla nikaalo, aage badho
   3. BOTTLENECK ratto mat -- KHUD USER banke raasta chalao, khud dikh jaayega
```

---

## STEP 0 — KYA POOCHNA (board pe kuch likhne se PEHLE)

```
   1. SCOPE (sabse pehla sawaal -- scope tum mat kaato, USSE poochho):
        "URL shortener me kai cheezein aati hain -- link banana, redirect, analytics,
         custom alias, expiry. Aap kis pe focus karwana chahenge?"
        -> jo wo bole, poore 40 minute usi pe.

   2. USE-CASE confirm:
        "To main do cheez maan raha hoon: long URL se short banana, aur short pe
         click karne pe original pe bhejna. Analytics scope me hai ya chhod du?"

   3. SCALE (number TUM do, confirm WO kare):
        "Main ~100 million links per day aur read:write 100:1 maan raha hoon --
         theek hai, ya aap alag scale dekhna chahenge?"

   4. Product-sawaal (agar samay ho):
        "Custom alias chahiye? Links expire hone chahiye? Analytics kitni real-time?"
```

```
   ★ agar kisi cheez ka pata na ho -> "ye maine use nahi kiya" bolna THEEK hai --
     interviewer khud bhar dega. (asli mock ki feedback-slide: "if you're not familiar
     with the system, they can fill you in" + "be open about the limits of your expertise")
```

---

## STEP 1 — BOARD KA PEHLA BLOCK (jawab milte hi ye likho, 4-5 bullet)

```
   URL Shortener
     - Links
     - Users
     - Clicks

   Use cases:
     - create a short link from a long URL
     - redirect from short link to the original

   Not in scope (poochh ke chhoda):
     - analytics dashboard . custom alias . expiry
```

```
   NON-FUNCTIONAL (kya chahiye -- "kaise" abhi nahi):
     - LOW LATENCY      -> redirect p99 < 200ms   (ye DIL hai)
     - HIGH AVAILABILITY-> redirect kabhi down na ho
     - READ-HEAVY       -> click >> create
     - unique short code (do link ko ek code na mile)
```

```
   KYUN CHAHIYE (asli duniya ka use -- ek line me bol dena, achha lagta):
   ┌────────────────────┐        WHY 302 (yahin flag kar do):
   │ Twitter/SMS limits │           har hit SERVER pe aata -> click count milta
   │ Marketing tracking │           (301 permanent -> browser cache -> analytics MISS)
   │ QR codes / print   │
   │ Aesthetics         │
   └────────────────────┘
```

---

## STEP 2 — NUMBERS (usi block ke neeche -- mote-mote, har number se ek FAISLA)

```
   ASSUMPTION (ek hi maano, poore round me wahi): 100M writes/DAY
   TRICK: 1 din ~ 100,000 sec (10^5) -> division aasan

   WRITES (shorten)                 READS (redirect)
      100M / day                       100x writes
      = 10^8 / 10^5                    ~ 100,000 / sec
      ~ 1,000 / sec

            ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
                 READ : WRITE  =  100 : 1
            ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
      FAISLA -> READ-HEAVY -> CACHE hi asli cheez hai
```

```
   STORAGE (100M PER DAY -> x365 din, NA x12 mahine):
      100M/day x 365 x 5 saal = ~180 BILLION URLs
      x 500 bytes             = ~90 TB
      FAISLA -> ek machine me nahi aayega -> aage shard karna padega

   ⚠ SLIP-YAAD (ye galti hui thi): "5yr x 12mo x 100M x 500B = 3TB" GALAT tha
     (wo 100M/MONTH maan ke tha). Per-DAY consistent raho -> ~90 TB.
```

```
   SHORT CODE ki lambai:
      62^7 = ~3.5 TRILLION combos -> 7 character kaafi hai (100+ saal chalega)
```

```
   ★★ DELIVERY-GEM (ye bolega to over-engineering se bachega):
      - Modest assumption : 1 BILLION total URLs -> ~500 GB -> SINGLE DB me FIT -> shard NAHI
      - Aggressive        : 100M/day -> ~90 TB -> shard by shortCode
      DONO sahi -- farak sirf ASSUMPTION ka. Assumption BOLO, phir usse reason karo.
      "Real Bitly ~500GB hai, single DB me aa jaata -- bina zaroorat shard mat karo."

   ★ scaling ka POORA ganit ABHI mat kholo -- wo deep-dive me. (asli round ki feedback-slide:
     "you don't necessarily have to go into the details of scaling at this point")
```

---

## STEP 3 — BOXES: PEHLE CHHOTA, PHIR DIKKAT PE BADHAO

### V1 — kaam-chalau (pehle sirf itna, 1 minute)

```
        USER ──► [ LB ] ──► [ App x2 ] ──► [ DB ]

        "Ye dono kaam kar deta hai jo humne tay ki.
         Ab main ise CHALA ke dekhta hoon kahan tootega."
```

### dikkat 1 — "har click DB pe jaa raha, aur redirect 200ms se tez chahiye"

```
        USER ──► [ LB ] ──► [ App ] ──► [ REDIS ] ──miss──► [ DB ]
                                           ▲                   │
                                           └───── populate ────┘

        cache-aside . TTL = link ki expiry
        100:1 read -> ~95% yahin nipat jaayenge, DB bach gaya
        + DB me shortCode pe PRIMARY-KEY / B-tree index -> O(log n), disk pe bhi tez
```

### dikkat 2 — "do server ek hi short code bana denge"

```
        [ App-1 ] ──┐
                    ├──► [ COUNTER service ]
        [ App-2 ] ──┘            │
                                 ▼
                    App-1 ko range  1 .. 1000
                    App-2 ko range  1001 .. 2000

        har server apni RANGE me se uthata -> ranges alag-alag -> takraav ka sawaal hi nahi
        phir number ko base62 me badal do -> chhota code
```

### dikkat 3 — "har click pe analytics likhenge to redirect slow ho jaayega"

```
        [ App ] ══302 redirect══► USER          (turant, kuch ruka nahi)
            │
            └──event──► [ KAFKA ] ──► [ Analytics svc ] ──► [ Analytics DB ]
                              │
                              └── baar-baar fail ──► [ DLQ ]

        redirect ka raasta aur analytics ka raasta ALAG -> ek doosre ko slow nahi karte
```

### dikkat 4 — "5 saal ka ~90 TB ek machine me nahi + LB khud gir gaya to sab band"

```
        USER
          │
        [ Route 53 ]      DNS + HEALTH-CHECK -> mara hua LB traffic se hata deta
          │                                     (LB ka SPOF isi se khatam)
        [ CloudFront ]    CDN -> static cheezein user ke paas se
          │
        [ LB ] ──► [ App x3 ] ──► [ Redis ] ──miss──► [ CASSANDRA ]
                                                       shard by shortCode
                                                       + 3x replica
```

---

## STEP 4 — POORA NAKSHA (yahan tak pahunche hain) + har box pe KYUN

```
                            USER
                             │
                    ┌────────▼────────┐
                    │    Route 53     │  DNS + health-check + geo-routing
                    └────────┬────────┘
                    ┌────────▼────────┐
                    │   CloudFront    │  CDN (static assets)
                    └────────┬────────┘
                    ┌────────▼────────┐
                    │  API GATEWAY    │  auth + rate-limit + routing, ek jagah
                    └───┬─────────┬───┘
              WRITE ┌───▼───┐ ┌───▼────┐ READ
                    │ App   │ │  App   │   read : write = 100 : 1
                    │ write │ │  read  │   -> dono ko ALAG-ALAG scale kar sakte
                    └───┬───┘ └───┬────┘      (read ke 20 instance, write ke 2)
             ┌──────────┼─────────┼──────────┬──────────┐
             ▼          ▼         ▼          ▼          ▼
       ┌─────────┐ ┌────────┐ ┌───────┐ ┌───────┐
       │ COUNTER │ │ REDIS  │ │ KAFKA │ │  DLQ  │
       │ (range) │ │ cache  │ │(async)│ └───────┘
       └─────────┘ └───┬────┘ └───┬───┘
                       │ miss     │
                       ▼          ▼
                 ┌──────────┐  ┌───────────────┐
                 │CASSANDRA │  │ Analytics svc │
                 │shard+repl│  │   + apni DB   │
                 └──────────┘  └───────────────┘
```

```
   HAR BOX KA KYUN (saath-saath bolna, bina poochhe):
     Route 53   : mara hua LB hata deta -> LB ka SPOF khatam . nearest region
     CloudFront : static user ke paas se -> origin pe load kam
     API Gateway: auth + rate-limit + routing ek jagah, har service me dobara nahi
     App        : stateless -> jitne chahiye utne instance
     read/write alag : load 100:1 hai -> alag-alag scale + ek gire to doosra chalta rahe
     COUNTER    : range de do -> takraav bina unique code, har baar coordination nahi
     REDIS      : read-heavy -> 95% yahin . cache-aside . TTL = link expiry
     KAFKA      : redirect ko block nahi karna -> analytics peeche chalta rahe
     CASSANDRA  : billions rows, sirf key se uthana -> shard by shortCode + replica
```

```
   READ FLOW (cache-aside)                 WRITE FLOW
   ─────────────────────                   ──────────
   USER click                              USER POST
      │                                       │
   LB -> App(read)                         LB -> App(write)
      │                                       │
   Redis HIT? ──yes──► 302 redirect        Counter service (range + base62)
      │                                       │
      no                                   ID 123456 -> "8m3"
      ▼                                       │
   Cassandra ──► Redis me daal do             ▼
      │                                    Save: Redis + Cassandra
      ▼                                       │
   302 redirect                               ▼
      │                                    short URL wapas
      └── async ──► Kafka (analytics)
```

---

## STEP 5 — API (do hi endpoint)

```
   POST /api/shorten                        GET /{code}      (e.g. /abc123)
   ─────────────────                        ────────────
   Request:                                 Response:
   {                                           HTTP 302 Found
     "long_url": "https://amazon...",          Location: https://amazon...
     "custom_code": "arpan-resume"  (optional)
   }
   Response:                                YAAD: banana = POST, laana = GET
   {                                              (swap mat karna)
     "short_url": "bit.ly/abc123",
     "expires_at": "2031-05-08"
   }
```

```
   301 (permanent) vs 302 (temporary)
   ┌──────┬─────────────┬───────────────┐
   │ 301  │ Cache OK    │ NO tracking   │
   │ 302  │ No cache    │ Tracks clicks │  <- bit.ly 302 use karta
   └──────┴─────────────┴───────────────┘

   ★ 302 ke DO reason bolo:
     1. CLICK-ANALYTICS -> har hit server pe aata (301 cache ho gaya to hit milta hi nahi)
     2. EXPIRY / URL badalna -> 301 browser cache kar leta -> expire/change pe toot jaata;
        302 = har baar fresh -> safe
```

---

## STEP 6 — DATA MODEL (KYUN bolo)

```
   CORE = HashMap pattern (KEY -> VALUE)
   ┌──────────────┬──────────────────────────┐
   │  short_code  │  long_url                │
   ├──────────────┼──────────────────────────┤
   │  abc123      │  https://amazon.in/...   │
   │  xyz789      │  https://flipkart.com/.. │
   └──────────────┴──────────────────────────┘
      KEY (partition)        VALUE

   SCHEMA: short_code (PARTITION KEY) -> long_url  (+ created_at, expires_at, user_id?)

   KEY = short_code KYUN:
      redirect hamesha short_code se aata (GET /{code})
      -> short_code pe partition -> SINGLE-partition point-read -> O(1)
      -> poora cluster scan nahi karna padta
```

```
   ACCESS PATTERN: INSERT ek baar + SELECT WHERE short_code = ?   = pure key-value (koi join nahi)

   DB CHOICE
   ┌──────────────┬─────────────┬──────────────┬─────────────┐
   │  Database    │  Pattern    │  Scale       │  Verdict    │
   ├──────────────┼─────────────┼──────────────┼─────────────┤
   │  MySQL       │  Relational │  ~1B tak     │  Chal jaata │
   │  Mongo       │  Document   │  ~10B tak    │  Chal jaata │
   │  Cassandra   │  Wide-col   │  Trillions   │  Best       │
   │  DynamoDB    │  K-V (AWS)  │  Trillions   │  Best       │
   │  Redis       │  In-memory  │  Cache layer │  Hamesha +  │
   └──────────────┴─────────────┴──────────────┴─────────────┘

   WHY NoSQL: simple key-based point-lookup, koi join nahi, huge + write-heavy
              -> horizontal scale + high write throughput + tunable consistency
   ("NoSQL powerful hai" MAT bolna -> ACCESS-PATTERN justify karta hai.)
```

---

## STEP 7 — DEEP DIVE (design ka DIL: short code kaise bane)

### (a) teen tareeke — ek chuno, kyun bolo

```
   long URL ──► ??? ──► short code

   ┌─────────────────┬──────────┬─────────────────────────┬──────────┐
   │   Method        │  Speed   │  Collision              │  Length  │
   ├─────────────────┼──────────┼─────────────────────────┼──────────┤
   │ MD5 / Random    │  Fast    │  YES (har baar DB check)│  6-7     │
   │ Counter         │  Fast    │  NO                     │  Variable│
   │ Counter+Base62  │  Fast    │  NO                     │  Compact │ ★ WINNER
   └─────────────────┴──────────┴─────────────────────────┴──────────┘

   WINNER = Counter + Base62:
      counter kabhi repeat nahi -> zero collision -> DB check ki zaroorat HI nahi + code chhota
      (Random/MD5 ko har baar "already exists?" DB-check chahiye -> extra read -> scale pe slow)
```

```
★★ WORD-FREEZE FALLBACK (term bhool jaao -> CONCEPT bol do, atko mat):
   "MD5/hash" bhoola  -> "long URL ka ek HASH lo, uske first 7 character"
   "Base62"  bhoola   -> "mere paas 62 character hain (a-z, A-Z, 0-9) -- ID ko un 62 me
                          ENCODE kar do -> chhoti string ban jaati"
   "Counter" bhoola   -> "ek global auto-increment ID"
   ★ Interviewer ko WORD nahi, SAMAJH chahiye. Concept bolo -> naam wo khud bol dega.
     Soch teri, sirf shabd badla -> word-freeze pe kabhi atke nahi.
```

### (b) base62 — number se code

```
      counter number              base62 karo              short code
        1,000,000,000     ──►   baar-baar / 62      ──►     "15FTGg"
                                remainders ULTA padho

      index -> char:
         0 - 9   -> '0'-'9'   (10)
        10 - 35  -> 'a'-'z'   (26)
        36 - 61  -> 'A'-'Z'   (26)
                              ────
                              62 total

      1 BILLION = sirf 6 character
      62^7 = 3.5 trillion combos -> 7 char me 100+ saal
```

### (c) distributed counter ki dikkat + RANGE ALLOCATION

```
   DIKKAT (multi-server, ek hi counter):

        [S1] counter=5     [S2] counter=5     [S3] counter=5
              │                  │                  │
              └──── sab ++ ──────┴──────────────────┘
                         sabne "6" banaya  ->  3 URL ka EK code  ->  COLLISION

   ┌────────────────────┬────────────┬──────────────┬───────────┐
   │  Approach          │ Coord kab  │ Bottleneck   │ Verdict   │
   ├────────────────────┼────────────┼──────────────┼───────────┤
   │ DB atomic counter  │ har write  │ haan (DB)    │ Slow      │
   │ Redis INCR         │ har write  │ haan (Redis) │ Better    │
   │ Range allocation   │ per BATCH  │ 1000x kam    │ ★ WINNER  │
   └────────────────────┴────────────┴──────────────┴───────────┘

   ★ RANGE / BLOCK ALLOCATION ka drawing:

        ┌──────────────────────────┐
        │   COORDINATOR            │   aksar ZOOKEEPER
        │  "agli range kiski?"     │   (ya ek DB counter-table)
        └───┬───────┬───────┬──────┘
            │       │       │
        1..1000  1001..  2001..
            │     2000    3000
            ▼       ▼       ▼
          [S1]    [S2]    [S3]      har server apni range LOCALLY use karta
                                     -> har request pe coordination NAHI,
                                        sirf ek baar per BLOCK -> tez
                                     -> ranges alag -> collision ho hi nahi sakta
                                     -> server restart, kuch number waste = chalega
                                        (collision se accha)

   ★ vocab jo bolna: "range/block allocation" + coordinator "Zookeeper"
   ★ ye counter+base62 ka SCALE-version hai.
```

### (d) custom short code

```
   POST { custom_code? }
        │
        ├─ custom diya hai ──► validate (lambai . reserved nahi . profanity nahi . unique)
        │                          │
        │                     conflict? ──► 409 wapas
        │
        └─ nahi diya      ──► AUTO: counter(range) + base62
                                   │
                                   ▼
                          Save: Redis + Cassandra ──► short URL wapas

   RESERVED WORDS (block karo -- ye system ke apne route hain):
      admin . api . login . settings . help . docs . pricing . blog

   RACE (2 log ek saath same custom code maangein):
      dono "available?" check karte -> dono ko haan -> dono save -> DUPLICATE
      FIX: DB me UNIQUE constraint / INSERT IF NOT EXISTS (atomic) -> ek ko 409 mil jaayega
```

### (e) teen gotcha (interviewer kuredega)

```
   1. CACHE-TTL: Redis me expiry na rakhi -> expired URL bhi serve hota rahega = requirement toot gayi
        FIX: cache-entry ka TTL = URL ki expiry (Redis SET ... EX <expiry>) -> apne aap nikal jaayega

   2. COUNTER BATCHING: write-service Redis se EK BAAR 1000 count uthaye (range), har request pe nahi
        (= yahi range allocation hai)

   3. COLLISION: shortCode pe UNIQUE-CONSTRAINT + retry (pehle "exists?" padhna NAHI).
        Counter se waise hi unique hai -> constraint sirf safety-net
```

---

## STEP 8 — BOTTLENECK (10x pe kya tootega)

```
   ★ RATTO MAT -- USER KA RAASTA CHALAO, bottleneck khud nikal aayega:

      user ne short link click kiya
          │
          ├─► DNS          -> ek region ke server? door wale slow -> GEO-ROUTING
          ├─► LB           -> ek LB gira to sab band -> health-check + multi-AZ
          ├─► App          -> stateless hai -> instance badha do (aasan)
          ├─► Redis        -> sab URL to nahi samaenge -> HOT rakho, COLD nikaalo (neeche)
          ├─► Cassandra    -> 90 TB ek machine me nahi -> SHARD by shortCode
          │                   read zyada -> READ REPLICA
          └─► analytics    -> sync likha to redirect slow -> KAFKA async (pehle hi kiya)

      + abuse/hot-key      -> RATE LIMITING
      ★ SPOF: counter-coordinator khud -> 2-node (active-passive)
              ya range-allocation (ye already tolerate kar leta hai)
      ★ WRITE ko replica se scale nahi karte -> write scale = SHARDING
```

```
★★ CACHE HOT/COLD -> LRU EVICTION (Arpan ne KHUD bola, 12-Sep mock -- ye line recall trigger karegi):

   Redis me SAARE URL nahi aa sakte. To rakho sirf HOT wale.

     "koi YOUTUBE-SONG viral / WHATSAPP-forward -> abhi laakhon hit -> HOT -> Redis me rahe.
      dheere-dheere log dekhna band -> COLD -> Redis se NIKAL jaaye -> jagah bane naye hot URLs ko."

        [ naya hot URL ]  ──► Redis me ghusa
                               │
        Redis bhar gayi  ──►  sabse PURANA-access wala bahar (LRU)
                               │
                               ▼
                          Cassandra me to hai hi -- agli baar miss pe wapas aa jaayega

   Ye mechanism = LRU eviction (access hua -> hot rehta, na hua -> evict).
   Redis ki LRU/LFU policy + TTL (expiry) dono milke ye karte hain.
   ★ Bolne ka tareeka: "Redis pe LRU eviction + TTL -> hot URLs cache me, cold apne aap nikal jaate."
     (concept apne example se bola = word-freeze fallback LIVE. LRU = anchor-word.)
```

---

## STEP 9 — WRAP (aakhir me 3 line)

```
   "Client -> CDN -> LB/Gateway -> App -> Redis -> Cassandra (shard by shortCode).
    Short code = counter + base62, range allocation se distributed.
    Read 100:1 hai isliye cache aur read-replica; analytics Kafka se async.
    Aage badhata to: custom URLs, expiry cleanup, aur geo-distribution."
```

```
★ DELIVERY NUGGETS
   • Har design-decision ko REQUIREMENT se jodo (ek-ek functional-req uthake).
   • HLD = DISCUSSION hai, ratta-script nahi -- interviewer ke saath faisla karo.
   • ESTIMATE: quick estimate -> scale justify -> aage. Exact number design nahi badalta.
     (par bilkul SKIP bhi mat karo -> ek banda Zomato me isi wajah se reject hua tha)
   • CORRECTIONS jo ho chuki (soch sahi thi, sirf cheez ulti thi):
       GET/POST swap (banana = POST) . "write replicas" -> SHARDING . KEY = shortCode
```

---

[← MASTER SHEET](../../00_MASTER_SHEET.md)
