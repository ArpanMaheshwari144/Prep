# URL Shortener — POORA ROUND, JAISE ASLI ME HOTA HAI

> **NAV** — ARCHETYPE F (infra/component) · DIL: chhota unique code + tez redirect. UP: [MASTER](../../00_MASTER_SHEET.md) · CONCEPTS: [ID-gen](../../FOUNDATIONS/13_distributed_id_snowflake.md) · [caching](../../FOUNDATIONS/04_caching.md) · [sharding](../../FOUNDATIONS/06_database_sharding.md) · trade-off: [301 vs 302](../../TRADEOFFS.md)

> Arpan-derived 21-Jun · 15-Sep: asli mock-video ke hisaab se dobara likha —
> **ye notes nahi, BAAT-CHEET ka script hai.** Har step me: tu kya BOLTA hai · wo kya bolta hai ·
> BOARD pe kya banta hai · aur FAISLA + KYUN.
> Problem (1 line): long URL -> short 6-7 char code; short pe click -> original pe redirect (302).

```
★★ TEEN NIYAM (poori file par lagte — [APPROACH_DELIVERY](../../HLD_APPROACH_DELIVERY.md) 5b)
   1. PERFECT design ek saath mat banao — chhote se shuru, dikkat pe badhao
   2. NUMBER ke peeche mat bhaago — bolo, ek faisla nikaalo, aage badho
   3. BOTTLENECK ratto mat — KHUD USER banke raasta chalao, khud dikh jaayega
   ★ har cheez INTERVIEWER SE CONFIRM karte chalo. Ye monologue nahi, BAAT-CHEET hai.
```

---

## STEP 1 — SCOPE POOCHO (board pe abhi kuch nahi likhna)

```
   TU BOLTA HAI:
     "URL shortener me kai cheezein aati hain — link banana, redirect, analytics,
      custom alias, expiry. Aap kis pe focus karwana chahenge?"

   WO BOLTA HAI (aksar):
     "Bas link banana aur redirect dekh lete hain."

   TU CONFIRM KARTA HAI:
     "Theek hai, to main analytics dashboard aur custom alias ko scope se bahar rakh raha hoon,
      aur redirect-path pe zyada waqt dunga. Sahi hai?"
```

```
   ★ scope TU mat kaato — USSE poochho. Phir wo scope uska chuna hua hai, aur tu bhar-poor
     usi hisse pe ja sakta hai bina dar ke.
   ★ kisi cheez ka pata na ho -> "ye maine use nahi kiya" bolna THEEK hai, wo khud bhar dega.
     (asli mock feedback-slide: "be open about the limits of your expertise")
```

---

## STEP 2 — BOARD PE PEHLA BLOCK (do chhote box, bas)

```
   TU BOLTA HAI:
     "Main pehle likh leta hoon ki system me cheezein kya hain aur humein kya karna hai."

   BOARD PE:

   ┌──────────────────────┐     ┌───────────────────────────────┐
   │ URL Shortener        │     │ Use cases:                    │
   │   - Links            │     │   - long URL -> short banao   │
   │   - Users            │     │   - short -> original redirect│
   │   - Clicks           │     │                               │
   └──────────────────────┘     │ NOT in scope (poochh ke chhoda│
                                │   - analytics dashboard       │
   ┌──────────────────────┐     │   - custom alias / expiry     │
   │ Kya chahiye (NFR):   │     └───────────────────────────────┘
   │  - redirect p99<200ms│ <- ye DIL hai
   │  - hamesha up        │
   │  - read >> write     │
   │  - code UNIQUE       │
   └──────────────────────┘
```

```
   TU BOLTA HAI (board pe ungli rakh ke):
     "Ye chaar hi cheezein main poore design me wapas laata rahunga —
      har faisla inhi me se kisi ek se justify karunga."

   KYUN CHAHIYE ye system (ek line, achha lagta hai):
     Twitter/SMS ki lambai limit . marketing tracking . QR code/print . dikhne me saaf
```

---

## STEP 3 — NUMBERS (har number se EK FAISLA, warna number bekaar)

```
   TU BOLTA HAI (number TU deta hai, confirm WO karta hai):
     "Main lagbhag 100 million links per day maan raha hoon, aur read:write 100:1.
      Theek hai, ya aap alag scale dekhna chahenge?"
```

```
   BOARD PE:

     Numbers:
       - writes : 100M / day  =  10^8 / 10^5  =  ~1,000 / sec
       - reads  : 100x writes =  ~100,000 / sec
       - row    : ~500 bytes
       - 5 saal : 100M x 365 x 5 = ~180 billion rows  ->  ~90 TB
       - code   : 62^7 = ~3.5 trillion  ->  7 char kaafi (100+ saal)

     TRICK: 1 din ~ 100,000 sec (10^5) -> division aasan
```

```
   HAR NUMBER SE FAISLA (ye BOLNA -- sirf number mat bolna):

     100 : 1 read-heavy   ──►  CACHE hi asli cheez hai
     ~90 TB               ──►  ek machine me nahi -> aage SHARD karna padega
     7 char kaafi         ──►  lambai ka jhagda khatam, aage badho

   ⚠ SLIP-YAAD (galti ho chuki): "5yr x 12mo x 100M x 500B = 3TB" GALAT tha
     (wo 100M/MONTH maan ke tha). PER-DAY consistent raho -> ~90 TB.
```

```
   ★★ DELIVERY-GEM (over-engineering se bachata):
      Modest maano   : 1 billion total URLs -> ~500 GB -> SINGLE DB me FIT -> shard NAHI
      Aggressive     : 100M/day -> ~90 TB -> shard by shortCode
      DONO sahi -- farak sirf ASSUMPTION ka. Assumption BOLO, phir usi se reason karo.
      "Asli Bitly ~500GB hai, single DB me aa jaata -- bina zaroorat shard mat karo."

   ★ scaling ka POORA ganit ABHI nahi -- wo deep-dive me.
     (feedback-slide: "you don't necessarily have to go into the details of scaling at this point")
```

---

## STEP 4 — PEHLA (CHHOTA) DESIGN + phir dikkat pe badhao

```
   TU BOLTA HAI:
     "Main pehle sabse simple cheez banata hoon jo kaam kar de, phir dekhte hain kahan tootti hai."

   BOARD PE — V1:

        USER ──► [ LB ] ──► [ App x2 ] ──► [ DB ]

   TU BOLTA HAI:
     "Ye dono kaam kar deta hai. Ab main USER ban ke ise chala ke dekhta hoon."
```

### dikkat 1 — "har click DB pe jaa raha hai, aur redirect 200ms se tez chahiye"

```
        USER ──► [ LB ] ──► [ App ] ──► [ REDIS ] ──miss──► [ DB ]
                                           ▲                   │
                                           └───── populate ────┘

   FAISLA + KYUN (bina poochhe bolo):
     "Read : write 100:1 hai — isliye cache sabse pehle. Cache-aside rakhunga,
      aur TTL link ki expiry ke barabar. ~95% read yahin nipat jaayenge."
     + DB me shortCode pe PRIMARY KEY / B-tree index -> O(log n), disk pe bhi tez
```

### dikkat 2 — "do server ek hi short code bana denge"

```
        [ App-1 ] ──┐
                    ├──► [ COUNTER service ]
        [ App-2 ] ──┘            │
                                 ▼
                    App-1 ko range  1 .. 1000
                    App-2 ko range  1001 .. 2000

   FAISLA + KYUN:
     "Har server ko ek RANGE de deta hoon. Ranges alag hain, isliye takraav ka sawaal hi nahi,
      aur har request pe coordinator se poochna bhi nahi padta."
     phir number ko base62 me badlo -> chhota code
```

### dikkat 3 — "har click pe analytics likhna hai"

```
        [ App ] ══302 redirect══► USER          (turant, kuch ruka nahi)
            │
            └──event──► [ KAFKA ] ──► [ Analytics svc ] ──► [ Analytics DB ]
                              │
                              └── baar-baar fail ──► [ DLQ ]

   FAISLA + KYUN:
     "Analytics sync likhunga to redirect slow ho jaayega — aur latency hi mera dil hai.
      Isliye event queue me daal kar turant redirect kar deta hoon."
```

### dikkat 4 — "5 saal ka ~90 TB ek machine me nahi + LB khud gir gaya to?"

```
        USER
          │
        [ Route 53 ]      DNS + HEALTH-CHECK
          │
        [ CloudFront ]    CDN
          │
        [ LB ] ──► [ App x3 ] ──► [ Redis ] ──miss──► [ CASSANDRA ]
                                                       shard by shortCode
                                                       + 3x replica

   FAISLA + KYUN (teeno alag-alag wajah se aaye, ek saath mat bolo):
     Route 53   -> "health-check mara hua LB traffic se hata deta hai — LB ka SPOF khatam"
     CloudFront -> "static cheezein user ke paas se milti hain, origin pe load kam"
     Cassandra  -> "90 TB ek machine me nahi -> shard by shortCode;
                    aur machine mar sakti hai -> 3 copy (replica)"
     ★ SHARDING aur REPLICA alag cheezein hain:
          shard   = data ke TUKDE (jagah + write scale)
          replica = wahi data ki COPY  (bachav + read scale)
```

---

## STEP 5 — POORA NAKSHA (jahan pahunche) + har box ka KYUN

```
                            USER
                             │
                    ┌────────▼────────┐
                    │    Route 53     │  DNS + health-check + nearest region
                    └────────┬────────┘
                    ┌────────▼────────┐
                    │   CloudFront    │  CDN (static)
                    └────────┬────────┘
                    ┌────────▼────────┐
                    │  API GATEWAY    │  auth + rate-limit + routing, ek jagah
                    └───┬─────────┬───┘
              WRITE ┌───▼───┐ ┌───▼────┐ READ
                    │ App   │ │  App   │   read:write = 100:1
                    │ write │ │  read  │   -> alag-alag scale (read 20, write 2)
                    └───┬───┘ └───┬────┘
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
   HAR BOX PE EK LINE (saath-saath, bina poochhe):
     Route 53   : mara hua LB hata deta -> SPOF khatam . nearest region
     CloudFront : static user ke paas se -> origin pe load kam
     API Gateway: auth + rate-limit + routing ek jagah, har service me dobara nahi
     App        : stateless -> jitne chahiye utne instance
     read/write alag : load 100:1 -> alag scale + ek gire to doosra chalta rahe
     COUNTER    : range -> takraav bina unique code, har request pe coordination nahi
     REDIS      : read-heavy -> 95% yahin . cache-aside . TTL = link expiry
     KAFKA      : redirect block na ho -> analytics peeche
     CASSANDRA  : billions rows, sirf key se uthana -> shard by shortCode + replica
```

```
   DO RAASTE alag-alag chalao (interviewer ko yahi dekhna hai):

   READ (click)                             WRITE (naya link)
   ────────────                             ─────────────────
   USER click                               USER POST
      │                                        │
   LB -> App(read)                          LB -> App(write)
      │                                        │
   Redis HIT? ──yes──► 302 redirect         Counter (range + base62)
      │                                        │
      no                                    123456 -> "8m3"
      ▼                                        │
   Cassandra ──► Redis me daal do              ▼
      │                                     Save: Redis + Cassandra
      ▼                                        │
   302 redirect                                ▼
      │                                     short URL wapas
      └── async ──► Kafka
```

---

## STEP 6 — API (do hi endpoint)

```
     BANANA                                    LAANA
     ──────                                    ─────
     USER ──POST /api/shorten──► [App]         USER ──GET /abc123──► [App]
             { long_url,            │                                   │
               custom_code? }       │                                   ▼
                                    ▼                    302 Found + Location: https://amazon...
             { short_url,  ◄────────┘
               expires_at }
                                                YAAD: banana = POST . laana = GET
```

```
   TU BOLTA HAI (302 kyun):
     "Main 302 rakhunga, 301 nahi —
        1. 301 permanent hai, browser cache kar leta hai -> phir hit server tak aata hi nahi
           -> click-count/analytics gaya
        2. link expire ya update hua to purana cache toot jaata hai; 302 har baar fresh hai."

   ┌──────┬─────────────┬───────────────┐
   │ 301  │ Cache OK    │ NO tracking   │
   │ 302  │ No cache    │ Tracks clicks │  <- bit.ly 302 use karta hai
   └──────┴─────────────┴───────────────┘
```

---

## STEP 7 — DATA MODEL (ek row + ek faisla)

```
   BOARD PE — ek row kaisi dikhti hai:

   ┌────────────┬──────────────────────┬────────────┬────────────┐
   │ short_code │ long_url             │ created_at │ expires_at │
   │  "abc123"  │ https://amazon.in/.. │    ...     │    ...     │
   └─────┬──────┴──────────────────────┴────────────┴────────────┘
         │
         └── ye KEY hai (partition key)
                  │
                  ▼
         GET /abc123 ──► seedha usi ek partition pe ──► O(1)
                          (poora cluster scan nahi)

   TU BOLTA HAI:
     "Redirect hamesha short_code se aata hai, isliye wahi partition key rakhunga —
      single-partition point read."
```

```
   KAUNSA DB — faisla:

        ┌──────────────────┐
        │ join chahiye?    │── nahi ──┐
        │ transaction?     │          │
        └──────────────────┘          ▼
                             pure KEY-VALUE lookup hai
                                      │
                                      ▼
                        ┌────────────────────────────┐
                        │ Cassandra / DynamoDB       │
                        │ billions + key lookup      │
                        └────────────────────────────┘

   ┌──────────────┬─────────────┬──────────────┬─────────────┐
   │  Database    │  Pattern    │  Scale       │  Verdict    │
   ├──────────────┼─────────────┼──────────────┼─────────────┤
   │  MySQL       │  Relational │  ~1B tak     │  Chal jaata │
   │  Mongo       │  Document   │  ~10B tak    │  Chal jaata │
   │  Cassandra   │  Wide-col   │  Trillions   │  Best       │
   │  DynamoDB    │  K-V (AWS)  │  Trillions   │  Best       │
   │  Redis       │  In-memory  │  Cache layer │  Hamesha +  │
   └──────────────┴─────────────┴──────────────┴─────────────┘

   ★ "NoSQL powerful hai" MAT bolna -> ACCESS PATTERN justify karta hai:
     INSERT ek baar + SELECT WHERE short_code = ? . koi join nahi . huge + write-heavy
```

---

## STEP 8 — DEEP DIVE: short code kaise bane (design ka DIL)

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

   TU BOLTA HAI:
     "Counter + base62 lunga. Counter kabhi repeat nahi hota, isliye collision ka sawaal hi nahi
      aur DB check bhi nahi karna padta. Random/MD5 me har baar 'already exists?' padhna padta —
      scale pe wo extra read mehnga hai."
```

```
★★ WORD-FREEZE FALLBACK (term bhool jaao -> CONCEPT bol do, atko mat):
   "MD5/hash" bhoola -> "long URL ka ek HASH lo, uske first 7 character"
   "Base62"  bhoola  -> "mere paas 62 character hain (a-z, A-Z, 0-9) — ID ko un 62 me
                         ENCODE kar do -> chhoti string ban jaati hai"
   "Counter" bhoola  -> "ek global auto-increment ID"
   ★ Interviewer ko WORD nahi, SAMAJH chahiye. Concept bolo -> naam wo khud bol dega.
```

### (b) base62 — number se code

```
      counter number             base62 karo             short code
        1,000,000,000    ──►   baar-baar / 62    ──►      "15FTGg"
                               remainders ULTA padho

      index -> char:   0-9 -> '0'-'9' (10)
                      10-35 -> 'a'-'z' (26)
                      36-61 -> 'A'-'Z' (26)
                                       ────
                                       62

      1 BILLION = sirf 6 character . 62^7 = 3.5 trillion -> 7 char me 100+ saal
```

### (c) distributed counter ki dikkat + RANGE ALLOCATION

```
   DIKKAT:
        [S1] counter=5     [S2] counter=5     [S3] counter=5
              │                  │                  │
              └──── sab ++ ──────┴──────────────────┘
                     sabne "6" banaya -> 3 URL ka EK code -> COLLISION

   ┌────────────────────┬────────────┬──────────────┬───────────┐
   │  Approach          │ Coord kab  │ Bottleneck   │ Verdict   │
   ├────────────────────┼────────────┼──────────────┼───────────┤
   │ DB atomic counter  │ har write  │ haan (DB)    │ Slow      │
   │ Redis INCR         │ har write  │ haan (Redis) │ Better    │
   │ Range allocation   │ per BATCH  │ 1000x kam    │ ★ WINNER  │
   └────────────────────┴────────────┴──────────────┴───────────┘

   ★ RANGE / BLOCK ALLOCATION:

        ┌──────────────────────────┐
        │   COORDINATOR            │   aksar ZOOKEEPER
        │  "agli range kiski?"     │   (ya ek DB counter-table)
        └───┬───────┬───────┬──────┘
            │       │       │
        1..1000  1001..  2001..
            │     2000    3000
            ▼       ▼       ▼
          [S1]    [S2]    [S3]

   TU BOLTA HAI:
     "Har server apni range locally use karta hai — coordinator se sirf ek baar per block
      baat hoti hai, har request pe nahi. Ranges alag hain to collision ho hi nahi sakta.
      Server restart pe kuch number waste ho jaate hain — wo chalega, collision se behtar hai."
   ★ vocab: "range / block allocation" + coordinator "Zookeeper"
```

### (d) custom short code

```
   POST { custom_code? }
        │
        ├─ custom diya ──► validate (lambai . reserved nahi . profanity nahi . unique)
        │                       │
        │                  conflict? ──► 409 wapas
        │
        └─ nahi diya  ──► AUTO: counter(range) + base62
                                │
                                ▼
                       Save: Redis + Cassandra ──► short URL wapas

   RESERVED WORDS (block — ye system ke apne route hain):
      admin . api . login . settings . help . docs . pricing . blog

   RACE (do log ek saath wahi custom code maangein):
      dono "available?" dekhte -> dono ko haan -> dono save -> DUPLICATE
      FIX: DB me UNIQUE constraint / INSERT IF NOT EXISTS (atomic) -> ek ko 409 mil jaata
```

### (e) teen gotcha (interviewer kuredega)

```
   1. CACHE-TTL : Redis me expiry na rakhi -> expired URL bhi serve hota rahega = requirement toot gayi
                  FIX: cache entry ka TTL = URL ki expiry (SET ... EX <expiry>)

   2. COUNTER BATCHING : write-service ek baar me 1000 count uthaye (range), har request pe nahi
                  (= yahi range allocation hai)

   3. COLLISION : shortCode pe UNIQUE constraint + retry (pehle "exists?" padhna NAHI).
                  Counter se waise hi unique hai -> constraint sirf safety-net
```

---

## STEP 9 — BOTTLENECK (10x pe kya tootega)

```
   ★ RATTO MAT — USER ka raasta chalao, bottleneck khud nikal aayega:

      user ne short link click kiya
          │
          ├─► DNS        -> sab ek region me? door wale slow      -> GEO-ROUTING
          ├─► LB         -> ek LB gira to sab band                -> health-check + multi-AZ
          ├─► App        -> stateless hai                         -> instance badha do (aasan)
          ├─► Redis      -> saare URL nahi samaenge               -> HOT rakho, COLD nikaalo (neeche)
          ├─► Cassandra  -> 90 TB ek machine me nahi              -> SHARD by shortCode
          │                 read zyada                            -> READ REPLICA
          └─► analytics  -> sync likha to redirect slow           -> KAFKA async (pehle hi kar diya)

      + abuse / hot-key   -> RATE LIMITING
      ★ SPOF : counter-coordinator khud -> 2-node (active-passive)
               ya range-allocation (ye already tolerate kar leta hai)
      ★ WRITE ko replica se scale NAHI karte -> write scale = SHARDING
```

```
★★ CACHE HOT/COLD -> LRU EVICTION (Arpan ne KHUD bola, 12-Sep mock — ye line recall trigger karegi)

   Redis me SAARE URL nahi aa sakte. To rakho sirf HOT wale.

   TU BOLTA HAI (apne example se):
     "koi YouTube-song viral ho gaya / WhatsApp forward chal pada -> abhi laakhon hit ->
      wo HOT hai, Redis me rehna chahiye. Dheere-dheere log dekhna band -> COLD ->
      Redis se nikal jaaye -> jagah bane naye hot URLs ko."

        [ naya hot URL ] ──► Redis me ghusa
                               │
        Redis bhar gayi ──► sabse PURANA-access wala bahar (LRU)
                               │
                               ▼
                         Cassandra me to hai hi — agli miss pe wapas aa jaayega

   Ye mechanism = LRU eviction. Redis ki LRU/LFU policy + TTL dono milke ye karte hain.
   ★ ek line me: "Redis pe LRU eviction + TTL -> hot URLs cache me, cold apne aap nikal jaate."
     (concept apne example se bola = word-freeze fallback LIVE. LRU = anchor-word.)
```

---

## STEP 10 — WRAP (aakhir me 3 line)

```
   "Client -> CDN -> LB/Gateway -> App -> Redis -> Cassandra (shard by shortCode).
    Short code = counter + base62, distributed ke liye range allocation.
    Read 100:1 hai isliye cache + read-replica; analytics Kafka se async.
    Aage badhata to: custom URLs, expiry cleanup, aur geo-distribution."
```

```
★ DELIVERY NUGGETS
   • Har faisle ko REQUIREMENT se jodo (STEP 2 wale box pe wapas ungli rakho).
   • HLD = DISCUSSION hai, ratta-script nahi — interviewer ke saath faisla karo.
   • ESTIMATE: quick estimate -> scale justify -> aage. Exact number design nahi badalta.
     (par bilkul SKIP bhi mat karo -> ek banda Zomato me isi wajah se reject hua tha)
   • CORRECTIONS jo ho chuki (soch sahi thi, cheez ulti thi):
       GET/POST swap (banana = POST) . "write replicas" -> SHARDING . KEY = shortCode
```

---

[← MASTER SHEET](../../00_MASTER_SHEET.md)
