# Twitter Feed — POORA ROUND (4 MOVE, jaise asli me hota hai)

> **NAV** — ARCHETYPE A (read-heavy/feed) · DIL: read SASTA ho. UP: [MASTER](../../00_MASTER_SHEET.md) · CONCEPTS: [caching](../../FOUNDATIONS/04_caching.md) · [sharding](../../FOUNDATIONS/06_database_sharding.md) · [replication](../../FOUNDATIONS/05_database_replication.md) · trade-off: [fanout write vs read](../../02_TRADEOFFS.md)

> 15-Sep: asli mock-video ke hisaab se dobara likha — koi 7-step rail nahi, sirf 4 move:
> POOCHA -> do chhote block LIKHE -> BOXES banaye -> phir bolte-bolte JODTA gaya.
> Har jagah: **tu kya BOLTA hai · BOARD pe kya banta · FAISLA + KYUN**.
>
> Problem (1 line): app khole -> HOME TIMELINE (jinko follow karta, latest-first). Tweet POST bhi.

```
   Tu app KHOLA -> home timeline: Virat "Match great" · Sachin "Watching IPL" · Dhoni "Practice"
   = un logon ke tweets jinhe TU follow karta hai, latest pehle.
```

```
★ MASTER ANALOGY — ROYAL KINGDOM (visual anchor, poori file me isi se jodna):
   Notice Board = Redis inbox per user   |  Town Crier   = Fanout Service
   Palace Board = Tweet store (Cassandra)|  Royal Scribe = Tweet Service
   News reader  = Timeline Service       |  Register     = Graph DB (kaun kisko follow karta)
   Newspaper truck = Kafka
```

```
★★ TEEN NIYAM (poori file par lagte — [01_DELIVERY](../../01_DELIVERY.md) 5b)
   1. PERFECT design ek saath mat banao — chhote se shuru, dikkat pe badhao
   2. NUMBER ke peeche mat bhaago — bolo, ek faisla nikaalo, aage badho
   3. BOTTLENECK ratto mat — KHUD USER banke raasta chalao, khud dikh jaayega
```

---

# MOVE 1 — POOCHO (board pe abhi kuch nahi)

```
   TU: "Twitter bada hai — timeline, tweet post, search, trending, DM, notifications.
        Aap kis pe focus karwana chahenge? Main home timeline aur tweet post pe ja sakta hoon."

   TU: "Kuch cheezein confirm kar lun —
          - kitne users? main ~500 million maan raha hoon
          - CELEBRITY handle karna hai? (jiske 10 crore follower hain)
          - timeline bilkul real-time chahiye, ya kuch second purana chalega?
          - media (photo/video) scope me hai ya sirf text?"

   ★ CELEBRITY wala sawaal sabse zaroori hai — is poore design ka asli mod wahi hai.
     (aur "kuch second purana chalega?" ka jawab 'haan' mile to precompute ka raasta khulta hai)
```

---

# MOVE 2 — DO CHHOTE BLOCK LIKHO

```
   ┌──────────────────────┐     ┌──────────────────────────────────┐
   │ Twitter              │     │ Use cases:                       │
   │   - Users            │     │   - tweet POST karo              │
   │   - Tweets           │     │   - HOME TIMELINE kholo          │
   │   - Follows (graph)  │     │     (follow-kiye logon ke tweets,│
   │   - Timeline         │     │      latest pehle)               │
   └──────────────────────┘     │                                  │
                                │ NOT in scope: search . trending  │
   ┌──────────────────────────┐ │                . DM . notifications│
   │ Kya chahiye (NFR):       │ └──────────────────────────────────┘
   │  - feed FAST khule <200ms│ <- DIL
   │  - READ-HEAVY            │
   │  - 500M user pe chale    │
   │  - eventual OK (2 sec    │
   │    purana chalega)       │
   └──────────────────────────┘
```

```
   Numbers:
     - users            : 500 M
     - tweets           : 500 M / day  =  ~5,800 / sec
     - avg follow       : ~200 . avg followers ~200
     - CELEB            : 10 crore (100M+) followers      <- ★ BIEBER problem
     - read : write     : ~50 : 1
```

```
   HAR NUMBER SE EK FAISLA (yahi bolna):
     50 : 1 read-heavy   ──►  READ ko sasta banao — har read pe compute MAT karo,
                              pehle se bana ke rakho (PRECOMPUTE)
     celeb 10 crore      ──►  ek tweet pe 10 crore kaam? -> yahi design ka asli mod hai
     eventual OK         ──►  precompute + cache ka raasta khula hai
```

---

# MOVE 3 — BOXES BANAO (chhota banao, phir dikkat pe badhao)

```
   TU: "Sabse simple cheez se shuru karta hoon."

        USER ──► [ App ] ──► [ DB ]
                              tweets table
                              follows table

        feed kaise banega? -> "jinko main follow karta hoon unke tweets nikaalo, time se sort karo, top 50"

   TU: "Ye chal jaata hai. Ab main USER ban ke app kholta hoon aur dekhta hoon kya hota hai."
```

### dikkat 1 — "har baar app kholne pe 200 logon ka data joda ja raha hai"

```
        app khuli ──► 200 follow nikaalo ──► sabke tweets ──► sort ──► top 50
                          (har baar, har user ke liye, 50:1 read pe)   -> FEED SLOW

   FAISLA: read ke waqt mat jodo — POST ke waqt hi sabke INBOX me daal do (precompute)

        Virat tweet ──► [ Fanout ] ──► har follower ka inbox me tweet_id daal do
                                          redis:inbox:arpan  ->  [t9, t7, t3, ...]

        ab app khuli ──► sirf apna inbox padho ──► INSTANT

   TU: "Read-heavy system me kaam READ se hata kar WRITE pe daal dete hain.
        Tweet ek baar bana, par padha 50 baar jaayega — to mehnat likhte waqt kar lo."
   ★ royal kingdom: Town Crier (fanout) har ghar ke Notice Board (inbox) pe parcha chipka deta hai
```

### dikkat 2 — "tweet ka button 3 second tak ghoomta raha — 200 inbox likhe ja rahe the"

```
        dikkat 1 me humne kaam READ se hata ke WRITE pe daal diya tha.
        ab wo poora kaam POST karne wale ke SAR pe hai:

        tweet bheja ──► 200 follower ──► 200 inbox write ──► TAB jaake "ho gaya"
                                                              (user itni der RUKA hua hai)

        aur Redis zara slow hua to tweet POST hi FAIL ho gaya --
        jabki tweet to ban chuka tha, sirf baantna baaki tha

   FAISLA: tweet DB me LIKHO -> ek EVENT queue (Kafka) pe daalo -> user ko TURANT "ho gaya"
           FANOUT WORKER peeche se inbox bharta rahe

        tweet ──► [ DB ] ──► [ KAFKA ] ──► [ Fanout worker ] ──► inbox x200
                     │
                     └──► user ko turant 200 OK

   ★ post ab DO hisson me bat gaya:
        "tweet ban gaya"    -> turant, user ka kaam khatam
        "sab tak pahuncha"  -> peeche, dheere bhi chale to chalega
   ★ aur ek bachav muft me: worker gir bhi gaya to tweet NAHI khoya --
     event Kafka me pada hai, worker wapas aa ke wahin se uthayega
```

### dikkat 3 — "Bieber ne tweet kiya — 10 CRORE inbox likhne padenge"

```
        Bieber tweet ──► [ Fanout ] ──► 100,000,000 inbox writes
                                          │
                                          └─► Town Crier CHOKED -> poora system atka

   FAISLA — HYBRID (yahi asli jawab hai):

        ┌───────────────────────────┬────────────────────────────┐
        │ NORMAL (< 10K followers)  │ PUSH  — fanout on write    │
        │ CELEB  (> 10K followers)  │ PULL  — read ke waqt fetch │
        └───────────────────────────┴────────────────────────────┘

        celeb ka tweet sirf tweets DB me jaata (fanout NAHI)
        (tweets DB = Cassandra — kyun, wo dikkat 6 me: petabytes + bahut writes)
        read ke waqt: apna inbox (push)  +  celeb ke tweets (pull)  ──► merge + sort

   CONCRETE: Arpan follows Virat (celeb) + Suresh (normal)
             Suresh ka tweet -> Redis inbox se (push)
             Virat ka tweet  -> Cassandra se (pull)
             -> merge + sort by time -> top 50

   ★ CROSSOVER: threshold ~10K followers. Tu 8K pe hai (PUSH mode) -> 2.5K aur follower aa gaye
     = 10.5K -> threshold cross -> aage ke tweets PULL mode me. System khud re-evaluate karta hai.

   TU: "Dono ka trade-off bol ke chun raha hoon — push read ko instant banata par celeb pe write-storm
        laata; pull write bachata par har read mehnga. Isliye hybrid."
```

### dikkat 4 — "Virat ka tweet 10 crore log ek saath padh rahe hain"

```
        10 crore read ──► sab Cassandra pe ──► DB CRASH

   FAISLA: HOT-TWEET CACHE

        ┌─ library analogy: bestseller front-counter pe rakho (cache),
        │                   normal kitaab peeche shelf me (DB)
        ▼
        [ Redis hot-tweet cache ]   recent celeb tweets, TTL 1 hr
             95% HIT
             miss -> Cassandra -> Redis me daal do -> return

        recent (<1hr) = HOT -> cache (SETEX ... 3600)
        purana        = COLD -> seedha DB
```

### dikkat 5 — "500 million inbox Redis me? memory phat jaayegi"

```
        DO ALAG CACHE hain (inhe ghaalmel mat karo):

        CACHE-1  INBOX (per-user, PUSH side)
            redis:inbox:arpan -> [tweet_ids only]   <- sirf ID, poora tweet nahi
            LTRIM 800 (itna hi rakho)  ->  ~6.4 KB per inbox
            500M user x 6.4 KB = ~3.2 TB -> Redis cluster me manageable

        CACHE-2  HOT-TWEET (shared, PULL side)
            cache:tweet:virat -> content, TTL 1 hr
            ~500K recent tweets x 500 B = ~250 MB -> ek node me aa jaata

        CASSANDRA = source of truth (saare tweets, petabytes, sharded)

        INACTIVE USER: 30 din se app nahi khola -> uska inbox Redis se DELETE
                       wapas aaya -> Cassandra se REBUILD (ek baar ka kharcha, memory bach gayi)
```

### dikkat 6 — "saare tweets ek DB me nahi aayenge"

```
   SHARDING ke teen tareeke (aur unka nuksan):

     1. by tweet_id (random)   -> load bilkul even, PAR ek user ke tweets bikhar jaate
                                  -> profile page = scatter-gather (slow)
     2. by user_id  ★ pehla    -> ek user ke saare tweets ek shard pe -> profile fast
        chunaav                   PAR hot-user (Bieber ka shard hammer hota rahega)
     3. by user_id + time      -> hot user bhi time ke hisaab se bant gaya,
                                  recent saath, purana cold-storage me

   HOT-USER REPLICATION: Bieber ka shard hammer, baaki idle
        -> Bieber ke tweets KAI shard pe replicate -> read bat gaye
```

### dikkat 7 — "India ka user US ke shard se padh raha hai (200ms)"

```
        GEO SHARDING: India / EU / US

        KYUN (chaar wajah, chaaron bolna):
          LATENCY     : India user -> India shard 5ms, US shard 200ms
          COMPLIANCE  : GDPR — EU ka data EU me rahe
          LOAD        : India ka peak aur US ka peak alag samay pe
          FAILURE     : India region gira -> EU/US chalte rahenge

        CROSS-REGION: Indian banda Bieber (US) ko follow karta
                      -> Bieber ke HOT tweets India ke Redis me REPLICATE
                      (production me yahi hota hai — hot data ko paas laao)
```

### dikkat 8 — "ab tak sab EK App box me chal raha hai — wo bojh aur SPOF dono hai"

```
   (a) ek box lakhs user nahi jhel raha, aur gira to sab band
         -> App ke kai box + aage LB (ALB). App stateless, state Redis / DB me.

   (b) likhna (tweet) aur padhna (feed) ka bojh bilkul alag — 1:50
         -> ek hi box me rakha to feed ka rush tweet-post ko bhi dheema kare
         -> Tweet Service (write) aur Timeline Service (read) ALAG, apne-apne hisaab se scale
         -> follow-graph (kaun kisko) har fanout pe chahiye -> User Service + graph store

   (c) photo / video duniya bhar se, har baar humare server se
         -> CDN (CloudFront), media user ke paas wali edge se

   (d) ek region / ek LB gira to site gayi
         -> Route 53: DNS + health-check, mara hua hataye, paas wala region de
```

### ab poora naksha (jahan pahunche) + har box ka KYUN

```
                    USER app kholta hai
                         ▼
                  ┌──────────────┐
                  │  Route 53    │  DNS + health-check
                  └──────┬───────┘
                  ┌──────▼───────┐
                  │  CloudFront  │  CDN (media: photo/video)
                  └──────┬───────┘
                  ┌──────▼───────┐
                  │     ALB      │  Load Balancer
                  └──────┬───────┘
            ┌────────────┼────────────┐
            ▼            ▼            ▼
     ┌─────────┐  ┌─────────┐  ┌─────────┐
     │ Tweet   │  │Timeline │  │  User   │
     │ Service │  │ Service │  │ Service │
     │ (write) │  │ (read)  │  │ (graph) │
     └────┬────┘  └────┬────┘  └────┬────┘
          ▼            │            ▼
     ┌─────────┐       │       ┌─────────┐
     │  KAFKA  │       │       │ Graph DB│  kaun kisko follow karta
     └────┬────┘       │       └─────────┘
          ▼            │
     ┌─────────────┐   │
     │  Fanout Svc │   │   celeb ko SKIP karta hai
     │  (workers)  │   │
     └──────┬──────┘   │
            └──────────┼──────────────┐
                       ▼              ▼
                 ┌──────────┐  ┌─────────────┐
                 │  REDIS   │  │  CASSANDRA  │
                 │ inbox +  │  │ saare tweet │
                 │ hot-tweet│  │ (sharded)   │
                 └──────────┘  └─────────────┘

     Tweet Svc  : likhne ka darwaza (royal scribe)
     Kafka      : fanout ko async kar diya -> tweet post turant lautta hai
     Fanout Svc : town crier — normal followers ke inbox bhar deta, celeb skip
     Redis      : inbox (instant read) + hot-tweet (celeb read ka bojh)
     Cassandra  : source of truth — write-heavy, LSM-tree fast write, user_id se shard
     Graph DB   : follow-graph (1-hop follow list ke liye simple adjacency table / Cassandra kaafi;
                  Neo4j tabhi jab "dost ke dost" jaise kai-hop sawaal ho)
     Timeline   : merge karne wala (push + pull)
     CDN        : media user ke paas se
```

```
   DO RAASTE alag-alag (ye bolna):

   WRITE (tweet post)                        READ (app khuli)
   ──────────────────                        ─────────────────
   Virat tweets -> Tweet Service             1. Redis inbox padho (LRANGE)   <- normal wale
        │                                    2. celeb ke tweets Cassandra se <- pull wale
        ├─► Cassandra me SAVE                3. MERGE + SORT (time se)
        │   (tweet_id, user_id, content, ts) 4. HYDRATE: tweet_id -> poora content
        │                                    5. top 50 wapas
        └─► event -> KAFKA -> Fanout worker
                 followers nikaalo
                 celeb FILTER OUT
                 har normal follower:
                    LPUSH redis:inbox:userX tweet_id

   YAAD-RAKHNE WALI LINE (read):
     "Timeline Svc -> LRANGE Redis inbox (push) + celeb-following Cassandra (pull)
      -> merge + sort -> hydrate -> top 50"
```

---

# MOVE 4 — BOLTE-BOLTE JODO (jo poocha jaaye, wahi kholo)

## ► "API kya hogi?"

```
     POST /tweet   { content }        ──►  tweetId            (BANANA  = POST)
     GET  /feed?limit=50              ──►  home timeline      (LAANA   = GET)
     GET  /user/{id}/tweets           ──►  profile timeline
```

## ► "DB me kya rakhoge, aur kaunsa DB?"

```
     TWEET store  ->  CASSANDRA
                      kyun: massive + simple + write-heavy, LSM-tree fast write,
                            user_id se shard karna aasan

     FEED inbox   ->  REDIS list per user
                      kyun: turant chahiye (in-memory)
                      LPUSH likhne me . LRANGE padhne me . LTRIM 800 (bounded)
                      ★ sirf tweet_ID rakho, poora content nahi -> inbox halka rehta

     USER graph   ->  Graph DB (Neo4j) ya Cassandra
                      followers / following

   TU: "Inbox me sirf ID rakhta hoon — content Cassandra me ek hi jagah rehta hai.
        Warna ek tweet 200 jagah copy hota aur memory phat jaati."
```

## ► "PUSH lena ya PULL?" (deep-dive ka dil — upar dikkat-3 me jawab hai)

```
   OPTION 1 — PUSH (fanout on write)
        tweet -> sab followers ke inbox me LPUSH
        app khuli -> apna inbox padha = INSTANT
        PAR celeb (10 crore) -> town crier choked

   OPTION 2 — PULL (fanout on read)
        celeb ka tweet sirf DB me; follower app-khole pe fetch kare
        har read pe query, par write bacha

   ★ WINNER = HYBRID (industry me yahi hota hai)
        normal -> push . celeb -> pull . read pe merge + sort

   ★ "trade-off bol ke chuna" — asli number yahin milte hain
```

## ► "Kahan tootega / aur scale karo"

```
   ★ RATTO MAT — USER ka raasta chalao:

      user app kholta hai
          │
          ├─► Timeline Svc  -> har read pe 200 follow ka join? -> INBOX precompute
          ├─► inbox (Redis) -> 500M inbox ka size?             -> LTRIM 800 + inactive TTL
          ├─► celeb tweets  -> 10 crore read Cassandra pe?     -> HOT-TWEET cache (95% hit)
          ├─► Cassandra     -> ek DB me sab?                   -> shard by user_id (+time)
          │                    Bieber ka shard hammer?         -> hot-user replication
          └─► doosre desh   -> 200ms latency + GDPR            -> GEO shard + hot-data replicate

      ★ REAL TWITTER = MULTI-DIMENSIONAL:
        user_id shard (primary) + time sub-shard + geo replication + hot-data global cache.
        Koi ek strategy akeli kaafi nahi hoti.
```

## ► WRAP (aakhir me 3-4 line)

```
   "WRITE: Tweet Service -> Cassandra + Kafka -> Fanout -> Redis inbox (sirf normal followers).
    READ : Timeline Service -> Redis inbox (push) + Cassandra (celeb pull) -> merge + sort
           -> hydrate -> top 50.
    Celeb ke liye hybrid push/pull, hot-tweet cache 95% hit,
    shard by user_id + time + geo, aur hot user ko multiple shard pe replicate.
    Aage badhata to: ML ranking, media CDN, trending aur search."
```

---

[← MASTER SHEET](../../00_MASTER_SHEET.md)
