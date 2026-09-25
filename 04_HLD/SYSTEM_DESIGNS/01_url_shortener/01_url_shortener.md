# URL Shortener — POORA ROUND (4 MOVE, jaise asli me hota hai)

> **NAV** — ARCHETYPE F (infra/component) · DIL: chhota unique code + tez redirect. UP: [MASTER](../../00_MASTER_SHEET.md) · CONCEPTS: [ID-gen](../../FOUNDATIONS/13_distributed_id_snowflake.md) · [caching](../../FOUNDATIONS/04_caching.md) · [sharding](../../FOUNDATIONS/06_database_sharding.md) · trade-off: [301 vs 302](../../TRADEOFFS.md)

> Arpan-derived 21-Jun · 15-Sep: asli mock-video dekh ke dobara likha.
> **Asli round me koi 7-step rail nahi chali** — sirf ye 4 move hue:
> POOCHA -> do chhote block LIKHE -> BOXES banaye -> phir bolte-bolte JODTA gaya.
> Isliye ye file bhi 4 move me hai. Har jagah: **tu kya BOLTA hai · BOARD pe kya banta · FAISLA + KYUN**.
>
> Problem (1 line): long URL -> short 6-7 char code; short pe click -> original pe redirect (302).

```
★★ TEEN NIYAM (poori file par lagte — [APPROACH_DELIVERY](../../HLD_APPROACH_DELIVERY.md) 5b)
   1. PERFECT design ek saath mat banao — chhote se shuru, dikkat pe badhao
   2. NUMBER ke peeche mat bhaago — bolo, ek faisla nikaalo, aage badho
   3. BOTTLENECK ratto mat — KHUD USER banke raasta chalao, khud dikh jaayega
   ★ har cheez interviewer se CONFIRM karte chalo. Monologue nahi, BAAT-CHEET hai.
```

---

# MOVE 1 — POOCHO (board pe abhi kuch nahi)

```
   TU: "URL shortener me kai cheezein aati hain — link banana, redirect, analytics,
        custom alias, expiry. Aap kis pe focus karwana chahenge?"
   WO: "Bas link banana aur redirect dekh lete hain."
   TU: "Theek hai — to analytics dashboard aur custom alias scope se bahar,
        aur redirect-path pe zyada waqt. Sahi hai?"

   TU: "Main ~100 million links per day maan raha hoon, read:write 100:1.
        Theek hai ya aap alag scale dekhna chahenge?"
```

```
   ★ scope TU mat kaato — USSE poochho. Phir scope uska chuna hua hai, aur tu bhar-poor
     usi hisse pe ja sakta hai.
   ★ pata na ho -> "ye maine use nahi kiya" bolna THEEK hai, wo khud bhar dega.
     (feedback-slide: "be open about the limits of your expertise")
```

---

# MOVE 2 — DO CHHOTE BLOCK LIKHO

```
   TU: "Pehle likh leta hoon system me cheezein kya hain aur humein karna kya hai."

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

   TU (board pe ungli rakh ke): "Ye chaar cheezein main poore design me wapas laata rahunga —
                                 har faisla inhi me se kisi se justify karunga."
```

```
   Numbers:
     - writes : 100M / day = 10^8 / 10^5 = ~1,000 / sec
     - reads  : 100x       = ~100,000 / sec
     - row    : ~500 bytes
     - 5 saal : 100M x 365 x 5 = ~180 billion rows -> ~90 TB
     - code   : 62^7 = ~3.5 trillion -> 7 char kaafi (100+ saal)

     TRICK: 1 din ~ 100,000 sec (10^5) -> division aasan
```

```
   HAR NUMBER SE EK FAISLA (yahi bolna — number akela bekaar hai):
     100 : 1 read-heavy  ──►  CACHE hi asli cheez hai
     ~90 TB              ──►  ek machine me nahi -> aage SHARD
     7 char kaafi        ──►  lambai ka jhagda khatam, aage badho

   ⚠ SLIP-YAAD (galti ho chuki): "5yr x 12mo x 100M x 500B = 3TB" GALAT tha
     (wo 100M/MONTH maan ke tha). PER-DAY consistent raho -> ~90 TB.

   ★★ DELIVERY-GEM (over-engineering se bachata):
      modest maano: 1 billion URLs -> ~500 GB -> SINGLE DB me fit -> shard NAHI
      aggressive  : 100M/day -> ~90 TB -> shard by shortCode
      dono sahi — farak sirf ASSUMPTION ka. "Asli Bitly ~500GB hai, single DB me aa jaata —
      bina zaroorat shard mat karo."
   ★ scaling ka poora ganit ABHI nahi. (slide: "you don't necessarily have to go into
     the details of scaling at this point")

   KYUN CHAHIYE ye system (ek line): Twitter/SMS limit . marketing tracking . QR/print . saaf dikhna
```

---

# MOVE 3 — BOXES BANAO (chhota banao, phir dikkat pe badhao)

```
   TU: "Sabse simple cheez banata hoon jo kaam kar de, phir dekhte hain kahan tootti hai."

        USER ──► [ LB ] ──► [ App x2 ] ──► [ DB ]

   TU: "Ye dono kaam kar deta hai. Ab main USER ban ke ise chala ke dekhta hoon."
```

### dikkat 1 — "har click DB pe jaa raha, aur redirect 200ms se tez chahiye"

```
        USER ──► [ LB ] ──► [ App ] ──► [ REDIS ] ──miss──► [ DB ]
                                           ▲                   │
                                           └───── populate ────┘

   TU: "Read:write 100:1 hai — isliye cache sabse pehle. Cache-aside, aur TTL link ki
        expiry ke barabar. ~95% read yahin nipat jaayenge."
      + DB me shortCode pe PRIMARY KEY / B-tree index -> O(log n), disk pe bhi tez
```

### dikkat 2 — "do server ek hi short code bana denge"

```
        [ App-1 ] ──┐
                    ├──► [ COUNTER service ]
        [ App-2 ] ──┘            │
                                 ▼
                    App-1 ko range 1..1000 . App-2 ko 1001..2000

   TU: "Har server ko ek RANGE de deta hoon. Ranges alag hain to takraav ka sawaal hi nahi,
        aur har request pe coordinator se poochna bhi nahi padta."
      phir number ko base62 -> chhota code   (detail MOVE 4 me)
```

### dikkat 3 — "har click pe analytics likhna hai"

```
        [ App ] ══302 redirect══► USER          (turant, kuch ruka nahi)
            │
            └──event──► [ KAFKA ] ──► [ Analytics svc ] ──► [ Analytics DB ]
                              │
                              └── baar-baar fail ──► [ DLQ ]

   TU: "Sync likhunga to redirect slow ho jaayega — aur latency hi mera dil hai.
        Isliye event queue me daal kar turant redirect kar deta hoon."
```

### dikkat 4 — "5 saal ka ~90 TB ek machine me nahi aayega"

```
        USER ──► [ LB ] ──► [ App x3 ]
                                │
                    [ Redis ] ──┘
                        │ miss
                        ▼
                 [ CASSANDRA ]   shard by shortCode + 3x replica

   TU: "90 TB ek machine me aayega hi nahi -> data ke TUKDE karne padenge (shard).
        Aur wo machine bhi mar sakti hai -> har tukde ki 3 COPY (replica)."

   ★ SHARD aur REPLICA alag cheezein hain:
        shard   = data ke TUKDE   (jagah + write scale)
        replica = wahi data ki COPY (bachav + read scale)
```

### dikkat 5 — "LB khud gir gaya — saare App zinda hain, par koi unhe traffic de hi nahi raha"

```
        ab tak sab kuch EK LB ke peeche tha
        wo gira -> App chal rahe hain, DB chal raha hai, Redis chal raha hai
                -> aur site DOWN hai

   FAISLA: ROUTE 53 (DNS) + health-check
           mara hua LB traffic se HATA diya jaata hai, dusre pe chala jaata hai

   ★ redundancy AKELI kaafi nahi hoti -- do LB rakh bhi diye to
     koi cheez chahiye jo DEKHE ki ek mar gaya aur traffic MOD de
     (Redis me yahi kaam Sentinel karta hai -- wahi shakal)
```

### dikkat 6 — "ek bande ne script chala di — ek raat me 10 lakh short link bana diye"

```
        counter ki range tez khatam hone lagi
        DB me kachra bhar gaya
        aur asli user ka create request line me lag gaya

   FAISLA: RATE LIMIT (per user / per IP / per API-key)

   ★ ye limit har App server me alag-alag likhoge to har server apna-apna ginega
     -> isi liye use EK jagah rakhna aasan hai: API GATEWAY
     (auth aur routing bhi wahi ek jagah baith jaate hain)
   ★ poora rate-limiter apne aap me ek design hai -> 02_rate_limiter
```

### ab poora naksha (jahan pahunche) + har box ka KYUN

```
                            USER
                             │
                    ┌────────▼────────┐
                    │    Route 53     │  DNS + health-check + nearest region
                    └────────┬────────┘
                    ┌────────▼────────┐
                    │  API GATEWAY    │  rate-limit (dikkat 6) + auth + routing
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

     Route 53   : mara hua LB hata deta -> SPOF khatam . nearest region
     API Gateway: rate-limit EK jagah (10 lakh link wali dikkat) + auth + routing
     App        : stateless -> jitne chahiye utne
     read/write alag : load 100:1 -> alag scale + ek gire to doosra chalta rahe
     COUNTER    : range -> takraav bina unique code
     REDIS      : read-heavy -> 95% yahin . cache-aside . TTL = link expiry
     KAFKA      : redirect block na ho -> analytics peeche
     CASSANDRA  : billions rows, key se uthana -> shard by shortCode + replica
```

```
   DO RAASTE alag chalao (interviewer ko yahi dekhna hai):

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

# MOVE 4 — BOLTE-BOLTE JODO (API · data · code-generation · bottleneck — sab yahin)

> Ye alag "step" nahi hain. Interviewer poochta jaata hai, tu jodta jaata hai.
> Jo wo poochhe wahi kholo — sab ek saath mat bol dena.

## ► "API kya hogi?"

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

   TU (302 kyun): "302 rakhunga, 301 nahi —
     1. 301 permanent hai, browser cache kar leta -> hit server tak aata hi nahi -> click count gaya
     2. link expire/update hua to purana cache toot jaata; 302 har baar fresh hai."

   ┌──────┬─────────────┬───────────────┐
   │ 301  │ Cache OK    │ NO tracking   │
   │ 302  │ No cache    │ Tracks clicks │  <- bit.ly 302 use karta
   └──────┴─────────────┴───────────────┘
```

## ► "DB me kya rakhoge, aur kaunsa DB?"

```
   ek row:
   ┌────────────┬──────────────────────┬────────────┬────────────┐
   │ short_code │ long_url             │ created_at │ expires_at │
   │  "abc123"  │ https://amazon.in/.. │    ...     │    ...     │
   └─────┬──────┴──────────────────────┴────────────┴────────────┘
         │
         └── ye KEY hai (partition key)
                  │
                  ▼
         GET /abc123 ──► seedha usi ek partition pe ──► O(1)  (poora scan nahi)

   TU: "Redirect hamesha short_code se aata hai, isliye wahi partition key —
        single-partition point read."
```

```
   kaunsa DB — faisla:

        ┌──────────────────┐
        │ join chahiye?    │── nahi ──┐
        │ transaction?     │          │
        └──────────────────┘          ▼
                             pure KEY-VALUE lookup
                                      │
                                      ▼
                        ┌────────────────────────────┐
                        │ Cassandra / DynamoDB       │
                        │ billions + key lookup      │
                        └────────────────────────────┘

   ┌──────────────┬─────────────┬──────────────┬─────────────┐
   │  MySQL       │  Relational │  ~1B tak     │  Chal jaata │
   │  Mongo       │  Document   │  ~10B tak    │  Chal jaata │
   │  Cassandra   │  Wide-col   │  Trillions   │  Best       │
   │  DynamoDB    │  K-V (AWS)  │  Trillions   │  Best       │
   │  Redis       │  In-memory  │  Cache layer │  Hamesha +  │
   └──────────────┴─────────────┴──────────────┴─────────────┘

   ★ "NoSQL powerful hai" MAT bolna -> ACCESS PATTERN justify karta hai:
     INSERT ek baar + SELECT WHERE short_code = ? . koi join nahi . HUGE (90 TB) + ek-key lookup
     (write-heavy NAHI — ye 100:1 READ-heavy hai; NoSQL ki wajah SCALE + simple key-lookup hai)
```

## ► "short code banate kaise ho?" (design ka DIL — yahin sabse zyada waqt)

```
   long URL ──► ??? ──► short code

   ┌─────────────────┬──────────┬─────────────────────────┬──────────┐
   │   Method        │  Speed   │  Collision              │  Length  │
   ├─────────────────┼──────────┼─────────────────────────┼──────────┤
   │ MD5 / Random    │  Fast    │  YES (har baar DB check)│  6-7     │
   │ Counter         │  Fast    │  NO                     │  Variable│
   │ Counter+Base62  │  Fast    │  NO                     │  Compact │ ★ WINNER
   └─────────────────┴──────────┴─────────────────────────┴──────────┘

   TU: "Counter + base62 lunga. Counter kabhi repeat nahi hota -> collision ka sawaal hi nahi
        aur DB check bhi nahi. Random/MD5 me har baar 'already exists?' padhna padta —
        scale pe wo extra read mehnga hai."
```

```
★★ WORD-FREEZE FALLBACK (term bhool jaao -> CONCEPT bol do, atko mat):
   "MD5/hash" bhoola -> "long URL ka ek HASH lo, uske first 7 character"
   "Base62"  bhoola  -> "mere paas 62 character hain (a-z, A-Z, 0-9) — ID ko un 62 me
                         ENCODE kar do -> chhoti string ban jaati"
   "Counter" bhoola  -> "ek global auto-increment ID"
   ★ Interviewer ko WORD nahi, SAMAJH chahiye. Concept bolo -> naam wo khud bol dega.
```

```
   base62 — number se code:

      1,000,000,000  ──►  baar-baar / 62, remainders ULTA padho  ──►  "15FTGg"

      0-9 -> '0'-'9' (10) . 10-35 -> 'a'-'z' (26) . 36-61 -> 'A'-'Z' (26)  = 62
      1 BILLION = sirf 6 char . 62^7 = 3.5 trillion -> 7 char me 100+ saal
```

```
   distributed counter ki DIKKAT:

        [S1] counter=5     [S2] counter=5     [S3] counter=5
              └──── sab ++ ──── sabne "6" banaya ──► 3 URL ka EK code = COLLISION

   ┌────────────────────┬────────────┬──────────────┬───────────┐
   │ DB atomic counter  │ har write  │ haan (DB)    │ Slow      │
   │ Redis INCR         │ har write  │ haan (Redis) │ Better    │
   │ Range allocation   │ per BATCH  │ 1000x kam    │ ★ WINNER  │
   └────────────────────┴────────────┴──────────────┴───────────┘

   ★ RANGE / BLOCK ALLOCATION:

        ┌──────────────────────────┐
        │   COORDINATOR            │   aksar ZOOKEEPER
        │  "agli range kiski?"     │   (ya ek DB counter-table)
        └───┬───────┬───────┬──────┘
        1..1000  1001..  2001..
            │     2000    3000
            ▼       ▼       ▼
          [S1]    [S2]    [S3]

   TU: "Har server apni range locally use karta — coordinator se sirf ek baar per block baat
        hoti hai, har request pe nahi. Ranges alag hain to collision ho hi nahi sakta."
   ★ vocab: "range / block allocation" + coordinator "Zookeeper"

   ── FOLLOW-UP: "wo server 400 pe CRASH ho gaya — baaki numbers ka kya?" ──

      restart hua server purani range YAAD NAHI rakhta -> coordinator se NAYI range maangta
      -> 401 se 1000 tak ke numbers BEKAAR chale gaye

      TU: "Kuch numbers waste ho jaayenge, aur main jaan-boojh ke usse theek maan raha hoon —
           62^7 yaani ~3.5 trillion combination hain, kuch hazaar waste hona kuch nahi bigaadta.
           Agar main har number ko crash-proof banata to har request pe coordination karni padti,
           aur range allocation ka poora faayda hi khatam ho jaata."

      => waste karna SOCHA-SAMJHA FAISLA hai, bug nahi — doosra option mehnga hai.

   ★ SUNO KI SAWAAL KIS BOX KA HAI: yahan APP SERVER ka crash pooch sakta hai, ya REDIS ka —
     dono ke jawab alag hain. Jawab dene se PEHLE dohra do:
        "Aap us app server ki baat kar rahe hain jiske paas 1-1000 ki range thi, sahi?"
     do second lagte hain, aur galat box pe jawab jaana ruk jaata hai.
```

## ► "custom short code allow karoge?"

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

   RESERVED WORDS (block — system ke apne route): admin . api . login . settings . help . docs . pricing . blog

   RACE (do log wahi custom code maangein):
      dono "available?" dekhte -> dono ko haan -> dono save -> DUPLICATE
      FIX: DB me UNIQUE constraint / INSERT IF NOT EXISTS -> ek ko 409
```

## ► "kahan tootega / 10x traffic pe?"

```
   ★ RATTO MAT — USER ka raasta chalao, bottleneck khud nikal aayega:

      user ne short link click kiya
          │
          ├─► DNS        -> sab ek region me? door wale slow     -> GEO-ROUTING
          ├─► LB         -> ek LB gira to sab band               -> health-check + multi-AZ
          ├─► App        -> stateless hai                        -> instance badha do
          ├─► Redis      -> saare URL nahi samaenge              -> HOT rakho, COLD nikaalo (neeche)
          ├─► Cassandra  -> 90 TB ek machine me nahi             -> SHARD by shortCode
          │                 read zyada                           -> READ REPLICA
          └─► analytics  -> sync likha to redirect slow          -> KAFKA async (kar diya)

      + abuse / hot-key  -> RATE LIMITING
      ★ SPOF : counter-coordinator khud -> 2-node (active-passive)
               ya range-allocation (ye already tolerate karta hai)
      ★ WRITE ko replica se scale NAHI karte -> write scale = SHARDING

      ★★ SHARD KEY = shortCode, GEO nahi:
           read HAMESHA short code se aata hai (GET /abc123)
             shard by shortCode -> seedha ek hi shard pe jaata hai            ✓
             shard by GEO       -> pata hi nahi chalega code kis region me hai
                                   -> saare shard poochne padenge (scatter-gather)  ✗

           GEO ka kaam ALAG hai — jagah baantna nahi, LATENCY kam karna:
             shard by shortCode = data ko TUKDON me baantna (jagah + write scale)
             geo replication    = door wale user ko paas se jawab dena (speed)
           dono saath chal sakte hain, par ek doosre ki jagah nahi lete.
```

```
★★ CACHE HOT/COLD -> LRU EVICTION (ye example Arpan ka apna hai — isliye recall turant hota hai)

   TU (apne example se): "koi YouTube-song viral ho gaya / WhatsApp forward chal pada ->
     abhi laakhon hit -> wo HOT hai, Redis me rehna chahiye. Dheere-dheere log dekhna band ->
     COLD -> Redis se nikal jaaye -> jagah bane naye hot URLs ko."

        [ naya hot URL ] ──► Redis me ghusa
                               │
        Redis bhar gayi ──► sabse PURANA-access wala bahar (LRU)
                               │
                               ▼
                         Cassandra me to hai hi — agli miss pe wapas aa jaayega

   Redis ki LRU/LFU policy + TTL dono milke ye karte hain.
   ★ ek line me: "Redis pe LRU eviction + TTL -> hot URLs cache me, cold apne aap nikal jaate."
```

## ► TEEN GOTCHA (interviewer kuredega)

```
   1. CACHE-TTL : Redis me expiry na rakhi -> expired URL bhi serve hota rahega = requirement toot gayi
                  FIX: cache entry ka TTL = URL ki expiry (SET ... EX <expiry>)
   2. COUNTER BATCHING : ek baar me 1000 count uthao (range), har request pe nahi (= range allocation)
   3. COLLISION : shortCode pe UNIQUE constraint + retry (pehle "exists?" padhna NAHI).
                  Counter se waise hi unique -> constraint sirf safety-net
```

## ► WRAP (aakhir me 3 line)

```
   "Client -> CDN -> LB/Gateway -> App -> Redis -> Cassandra (shard by shortCode).
    Short code = counter + base62, distributed ke liye range allocation.
    Read 100:1 hai isliye cache + read-replica; analytics Kafka se async.
    Aage badhata to: custom URLs, expiry cleanup, aur geo-distribution."
```

```
★ DELIVERY NUGGETS
   • Har faisle ko REQUIREMENT se jodo (MOVE 2 wale box pe wapas ungli rakho).
   • HLD = DISCUSSION hai, ratta-script nahi.
   • ESTIMATE: quick estimate -> scale justify -> aage. Exact number design nahi badalta.
     (par bilkul SKIP bhi mat karo -> ek banda Zomato me isi wajah se reject hua tha)
   • CORRECTIONS jo ho chuki (soch sahi thi, cheez ulti thi):
       GET/POST swap (banana = POST) . "write replicas" -> SHARDING . KEY = shortCode
```

---

[← MASTER SHEET](../../00_MASTER_SHEET.md)
