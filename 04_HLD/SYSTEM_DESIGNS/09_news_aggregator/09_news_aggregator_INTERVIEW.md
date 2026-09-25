# News Aggregator — POORA ROUND (4 MOVE)

> **NAV** — ARCHETYPE A+E · DIL: kai source -> ek feed. UP: [MASTER](../../00_MASTER_SHEET.md) · CONCEPTS: [elasticsearch](../../FOUNDATIONS/12_elasticsearch_search.md) · [caching](../../FOUNDATIONS/04_caching.md) · saath: [twitter-feed](../03_twitter_feed/03_twitter_feed.md)

> JP general-product design (Google News / Inshorts jaisa): alag-alag source se news kheencho
> -> store karo -> user ko ek feed do. READ-HEAVY system.
> 15-Sep: asli mock-video ke hisaab se dobara likha — koi 7-step rail nahi, sirf 4 move.
> Har jagah: **tu kya BOLTA hai · BOARD pe kya banta · FAISLA + KYUN**.

```
★★ TEEN NIYAM (poori file par lagte — [APPROACH_DELIVERY](../../HLD_APPROACH_DELIVERY.md) 5b)
   1. PERFECT design ek saath mat banao — chhote se shuru, dikkat pe badhao
   2. NUMBER ke peeche mat bhaago — bolo, ek faisla nikaalo, aage badho
   3. BOTTLENECK ratto mat — KHUD USER banke raasta chalao, khud dikh jaayega
```

---

# MOVE 1 — POOCHO (board pe abhi kuch nahi)

```
   TU: "News aggregator me do bade hisse hain — news andar laana (ingestion) aur
        user ko feed dikhana. Aap kis pe focus karwana chahenge?"

   TU: "Kuch cheezein confirm kar lun —
          - kitne SOURCE? 10 ya 10,000?
          - news kitni FRESH chahiye — real-time, ya 5 minute purani chalegi?
          - feed sabko SAME hoga ya PERSONALIZED?
          - kitne user, kitni news per day?"

   ★ "5 minute purani chalegi?" — is sawaal ka jawab poore design ka raasta khol deta hai.
     Haan mila -> precompute + cache ka pura raasta khul gaya.
```

---

# MOVE 2 — DO CHHOTE BLOCK LIKHO

```
   ┌──────────────────────┐    ┌────────────────────────────────────┐
   │ News Aggregator      │    │ Use cases:                         │
   │   - Source (RSS/site)│    │   - sources se articles kheencho   │
   │   - Article          │    │   - store karo                     │
   │   - Category         │    │   - user ko FEED do (latest list)  │
   │   - User (+ prefs)   │    │   - (optional) category / search   │
   └──────────────────────┘    │                                    │
                               │ NOT in scope: comments, ML ranking │
   ┌──────────────────────────┐└────────────────────────────────────┘
   │ Kya chahiye (NFR):       │
   │  - feed FAST khule <- DIL│
   │  - news FRESH rahe       │
   │  - lakhs user pe chale   │
   │  - ek SOURCE down ho to  │
   │    system na gire        │
   └──────────────────────────┘

   ★ KEY SOCH jo shuru me hi bolni hai:
     "Har user feed kholta hai, par news to gine-chune source se aati hai —
      matlab READS >> WRITES. Poora design isi ek baat pe khada hoga."
```

```
   Numbers:
     - 10 lakh user, har user 5 baar/din feed kholta  ->  50 lakh reads/din
     - 1000 source, har 5 min me nayi news            ->  ~3 lakh writes/din

     TRICK (1 din ~ 1,00,000 sec):
        reads  = 50,00,000 / 1,00,000 = ~50 / sec     (spike 5-10x = ~500 / sec)
        writes = 3,00,000  / 1,00,000 = ~3 / sec

   HAR NUMBER SE EK FAISLA:
     reads >> writes   ──►  CACHE + READ REPLICA (ye design ka dil hai)
     writes background ──►  QUEUE (user ko intezaar nahi karna)
     data bada hota ja ──►  SHARD + purana ARCHIVE
```

---

# MOVE 3 — BOXES BANAO (chhota banao, phir dikkat pe badhao)

```
   TU: "Sabse simple se shuru."

        [ Fetcher ] ──► [ DB ]  articles
                          ▲
        USER ──► [ App ] ─┘   "latest 20 nikaal ke de do"

   TU: "Ye kaam kar raha hai. Ab 10 lakh user aur 1000 source daal ke dekhte hain."
```

### dikkat 1 — "har request pe DB se 'latest 20' query ja rahi hai"

```
        50 / sec normal, spike pe 500 / sec — sab DB pe
        -> DB marr jaayega, aur sabko LAGBHAG WAHI feed chahiye thi

   TEEN OPTION (ye teeno bolna, phir chunna):

     OPTION 1 — ON-THE-FLY (har request pe DB query)
          simple + hamesha fresh, PAR har request DB pe -> spike pe DB down  => NAHI

     OPTION 2 — ★ PRECOMPUTE + CACHE (WINNER)
          ready feed (latest 20) Redis me pada ho -> user seedha cache se le
          nayi news aaye -> cache refresh
          5 minute purani feed chal jaayegi (news me theek hai; paisa hota to NAHI)

     OPTION 3 — FANOUT (har user ki personalized feed pehle se bana ke rakho)
          fast + personalized, PAR 10 lakh alag feed banana aur rakhna mehnga
          => sirf tab jab personalization asli requirement ho

   FAISLA: OPTION 2 — kyunki sabko lagbhag SAME latest feed chahiye
           -> ek cache sab use karenge -> read-heavy ke liye ekdum fit

        nayi news ──► worker ──► DB + cache refresh
        user      ──► cache (99%) ──miss──► DB ──► wapas cache me

   ★ trade-off bol ke chunna — asli number yahin milte hain
```

### dikkat 2 — "1000 source ek saath, aur fetch karna slow hai"

```
        agar user ki request ke waqt fetch karenge -> user ruka rahega
        aur 1000 source ek saath aa jaayein -> spike

   FAISLA: WRITE-PATH aur READ-PATH bilkul ALAG kar do

     ── WRITE (news andar aati hai) — slow, background ──
        Sources(1000) ──► [ Fetcher / Crawler ] ──► [ QUEUE (Kafka) ] ──► [ Worker ] ──► DB + Cache

     ── READ (user padhta hai) — fast ──
        User ──► [ Feed Service ] ──► [ Cache ] ──miss──► [ DB ]
                 (LB aur read-replica abhi NAHI — wo dikkat 8 me, jab sach me zaroorat aaye)

   TU: "Ye mera core decision hai — dono raaste alag rakhunga taaki ek doosre ko slow na karein.
        Crawling background ka kaam hai, feed dikhana foreground ka."

   ★ QUEUE hi kyun: 1000 source ek saath aa gaye -> queue absorb kar legi,
     worker apni raftaar se khaayega.
```

### dikkat 3 — "ek hi khabar paanch alag source se aa gayi"

```
        feed me wahi news 5 baar dikhegi -> bekaar

   FAISLA: WORKER ke andar teen kaam

        raw article ──► [ WORKER ] ──► DB + cache
                            │
                            ├─ CLEAN     (ads/HTML hatao, title/content nikaalo)
                            ├─ DEDUPE    (ek khabar 5 source pe -> ek hi rakho)
                            └─ CATEGORY  (tech / sports / politics tag karo)
```

### dikkat 4 — "ek source down hai ya bahut slow"

```
        fetcher us source pe atak gaya -> baaki 999 source ki news bhi ruk gayi

   FAISLA:
        - har source ka fetch alag (parallel), ek doosre se azaad
        - TIMEOUT rakho
        - fail hua -> RETRY, phir bhi na chale -> SKIP karo aur aage badho
        - queue ka backlog spike absorb karta rahega

   TU: "Ek source ka down hona poore system ko nahi gira sakta —
        ye maine requirement me bhi likha tha."
```

### dikkat 5 — "6 mahine me 5 crore row — disk, backup aur kharcha badhta ja raha hai"

```
        feed ki query hai:   ORDER BY published_at DESC LIMIT 20

        ★ SACH: published_at pe index hai to ye query 5 crore pe bhi MILLISECONDS me
          (DB index ke aakhri sire se 20 entry padh leta). Query slow NAHI hai.
        asli dikkat: table, index, backup, restore sab 5 crore ka -> disk + kharcha + dheema restore

        aur dekho: ye 20 row NAYI hain.
        baaki 4.99 crore row sirf jagah ghere baithi hain, koi padhta hi nahi.

   FAISLA: PURANA data alag karo
        latest ~7 din   ->  garam table (chhoti, tez)
        usse purana     ->  cold storage / archive table
        cache me sirf latest  ->  DB pe load waise hi kam

   ★ ye SHARDING nahi hai -- ye RETENTION hai. Dono alag cheezein hain,
     aur aksar ek hi saans me bol di jaati hain.
```

### dikkat 6 — (sirf BADE scale pe) "ek DB box likhai + data nahi jhel raha"

```
        ★ IMAANDARI SE: humare number pe (writes ~3/sec, 7 din garam data) ek DB box aaram se
          chal jaata hai -> SHARD ki ZAROORAT NAHI. Interview me yahi bolo:
          "is scale pe shard nahi karunga; source 100x ho jaayein ya user-generated content aaye,
           tab shard."

   TAB (bade scale pe) FAISLA: SHARD  ->  date (ya category) ke hisaab se

   ★ date se shard karne ka ek ASAR hai:
     saari NAYI likhai EK hi shard pe girti hai (aaj wala) -> wahi shard garam rahega
     -> wahi HOT-PARTITION wali baat jo caching aur chat design me bhi aati hai
```

### dikkat 7 — "user ko apni pasand ki feed chahiye"

```
        poora per-user fanout mehnga hai (10 lakh alag feed)

   FAISLA (beech ka raasta): CATEGORY-WISE CACHE
        feed:tech . feed:sports . feed:politics  — har category ki ready feed
        user ki prefs dekho -> 2-3 category ki cached feed merge kar do
        -> personalization bhi mil gaya, aur 10 lakh feed banane se bach bhi gaye
```

### dikkat 8 — "subah 8 baje sab ek saath app kholte hain — ek Feed Service box ka CPU khatam"

```
        news ka traffic SPIKE wala hota hai (subah, lunch, raat)
        ek box bhara  ->  request line me  ->  aur wahi box gira to feed POORI band

        aur cache-MISS wali request seedha PRIMARY DB pe ja rahi hai --
        usi primary pe fetcher 1000 source ki LIKHAI kar raha hai
        ->  padhne wale ne likhne wale ko dheema kar diya

   FAISLA (do alag cheezein, do alag wajah se):
        kai FEED SERVICE instance + LB   ->  spike jhelne ke liye, aur ek gire to baaki chalein
                                             (Feed Service stateless hai -- feed cache me hai,
                                              box ki memory me nahi)
        cache-miss ka read  ->  READ REPLICA se, primary se NAHI
                                ->  padhna aur likhna alag raaston pe chale jaayein

   ★ replica CACHE ki jagah nahi leti -- cache 99% rok leti hai,
     replica sirf bache hue 1% ko primary se door rakhti hai
```

### ab poora naksha (jahan pahunche) + har box ka KYUN

```
   ── WRITE PATH (background, slow chalega) ──

   Sources (1000)
        │  RSS / poll
        ▼
   [ FETCHER / CRAWLER ]  ──► [ QUEUE (Kafka) ] ──► [ WORKER ] ──► [ DB (NoSQL) ]
        timeout + retry + skip      spike absorb      clean            +
                                                      dedupe       [ CACHE refresh ]
                                                      category

   ── READ PATH (fast) ──

   USER ──► [ LB ] ──► [ FEED SERVICE ] ──► [ CACHE (Redis) ] ──miss──► [ DB read-replica ]
                                                 ~99% hit                      │
                                                                     wapas cache me daal do

     Fetcher   : source se kheenchta (poll/RSS) — alag rakha taaki feed pe asar na ho
     Queue     : 1000 source ek saath aa jaayein to spike absorb kare
     Worker    : raw ganda data -> clean + DEDUPE + category
     DB        : permanent store
     Cache     : read-heavy hai -> ready feed RAM me -> DB har baar mat maaro
     Feed Svc  : feed banata — pehle cache, miss pe REPLICA (primary ko chhua bhi nahi)
     LB        : subah wale spike pe kai feed-service instance (dikkat 8)

   ★ CORE DECISION (ye line bolni hai): WRITE path aur READ path ALAG hain —
     ek doosre ko slow nahi karte.
```

---

# MOVE 4 — BOLTE-BOLTE JODO (jo poocha jaaye, wahi kholo)

## ► "API kya hogi?"

```
   USER ke liye (LAANA = GET):
     GET /feed?page=1&category=tech    ->  latest news list (PAGED)
     GET /article/{id}                 ->  ek article ka content
     GET /search?q=cricket             ->  search

   ANDAR ke liye (BANANA = POST):
     POST /ingest { source data }      ->  nayi news daalo (zyadatar background)

   ★ SENIOR SIGNAL: PAGINATION — 10,000 news ek saath mat bhejo;
     page/limit do -> infinite scroll chalega.
   ★ GET = padhna | POST = state badalna (swap mat karna)
```

## ► "DB me kya, aur kaunsa DB?"

```
   ARTICLE :  id (KEY) | title | content | sourceId | category | publishedAt | url
   SOURCE  :  id (KEY) | name | rssUrl | lastFetchedAt
   (optional) USER_PREFS : userId | categories[] | savedArticles[]

   DB choice -> NoSQL (Mongo / Cassandra):
        news = bahut zyada (crore, badhta jaayega) + structure simple
               + read-heavy + ACID ki zaroorat nahi (paisa nahi hai)
        NoSQL -> horizontal scale aasan + flexible schema + eventual consistency chal jaayegi

   ★ CONTRAST (JP/finance flavour — ye bolna):
        News  ->  NoSQL
        PAISA / ledger  ->  HAMESHA SQL + ACID
     "Data ka nature dekho, phir DB chuno" — ye line poore HLD me kaam aati hai.
```

## ► "Search kaise karoge?"

```
   DB me LIKE '%cricket%' -> poora scan -> slow

   FAISLA: alag SEARCH INDEX (Elasticsearch — inverted index)
        worker jab article store kare -> saath me search index bhi update kare (async)
        search request -> Elasticsearch -> matching ids -> content DB/cache se

   ★ index thoda peeche ho to chalega (nayi news 1 min baad search me dikhe — theek hai)
   detail: [FOUNDATIONS/12_elasticsearch_search](../../FOUNDATIONS/12_elasticsearch_search.md)
```

## ► "Kahan tootega / 10x pe?"

```
   ★ RATTO MAT — dono raaste alag-alag chalao:

      READ path:
          user ──► LB          -> ek instance kaafi nahi      -> kai instance + LB
                ──► feed svc   -> har request DB pe?          -> CACHE (99%)
                ──► cache      -> Redis gir gaya?             -> cluster + replica,
                                                                 miss pe DB-replica fallback
                ──► DB         -> read load                   -> READ REPLICA
                                  storage badh raha           -> SHARD (category/date) + ARCHIVE

      WRITE path:
          source ──► fetcher   -> ek source down/slow          -> timeout + retry + skip
                 ──► queue     -> 1000 source ek saath         -> backlog absorb
                 ──► worker    -> kaam bahut                   -> kai worker parallel

      SPOF: "ek box gira to poora system gira?" -> wahi cheez replicate karo

   AAGE badhata to: personalized feed (category-wise cache), images CDN pe,
                    ML ranking, breaking news ka real-time push.
```

## ► WRAP (ek saans me)

```
   "WRITE: sources -> fetcher -> queue -> worker (clean + dedupe + category) -> NoSQL DB + cache.
    READ : user -> LB -> feed service -> cache (99%) -> miss pe DB read-replica.
    DB NoSQL rakha kyunki data massive aur simple hai aur ACID ki zaroorat nahi;
    paisa hota to SQL/ACID leta.
    Deep-dive: feed precompute + cache — kyunki sabko lagbhag same latest feed chahiye.
    Scale: service LB, DB replica + shard + archive, fetcher parallel + retry, cache cluster."
```

---

> ★ News ka TWIST ek line me: WRITE-path aur READ-path ALAG + feed precompute + cache (read-heavy).
> Paisa hota to SQL/ACID hota — data ke nature se DB chunte hain.

---

[← MASTER SHEET](../../00_MASTER_SHEET.md)
