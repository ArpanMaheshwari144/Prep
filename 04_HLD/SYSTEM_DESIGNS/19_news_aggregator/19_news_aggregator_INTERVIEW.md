# News Aggregator — INTERVIEW (8-step framework)

> JP general-product design (Google News / Inshorts jaisा): alag sources se news kheencho
> -> store -> user ko ek feed do. READ-HEAVY system.
> Framework: 04_HLD/INTERVIEW_FRAMEWORK.md

---

## STEP 1 — REQUIREMENTS clarify (solution pe mat kudo)
```
   FUNCTIONAL:  sources (websites/RSS) se articles kheencho -> store -> user ko feed (latest list)
                -> category/search optional
   NON-FUNCTIONAL:  feed FAST khule, news FRESH, scale (lakhs user), source down -> system na gire
   clarifying Qs:  kitne source (10 ya 10k)? real-time ya 5-min purani chalegi?
                   personalized ya sabko same? kitne user / news per day?
   KEY soch: reads (har user feed kholta) >> writes (sources se) -> READ-HEAVY (poora design isi pe)
```

## STEP 2 — SCALE / numbers
```
   users 10 lakh, har user 5 baar/din feed -> 50 lakh reads/din
   sources 1000, 5 min mein nayi news -> ~3 lakh writes/din
   TRICK (1 din ~ 1,00,000 sec):
     reads  = 50,00,000 / 1,00,000 = ~50/sec  (spike 5-10x = ~500)
     writes = 3,00,000  / 1,00,000 = ~3/sec
   -> reads >> writes = READ-HEAVY -> CACHE + READ-REPLICA
      writes background -> QUEUE | bada storage -> SHARD/archive
```

## STEP 3 — API design
```
   USER (LAANA = GET):
     GET /feed?page=1&category=tech   -> latest news list (PAGED)
     GET /article/{id}                -> ek article ka content
     GET /search?q=cricket            -> search
   ANDAR (BANANA = POST):
     POST /ingest {source data}       -> nayi news daalo (mostly background)
   senior signal: PAGINATION (10k news ek saath mat bhejo -> page/limit -> infinite scroll)
                  GET = read | POST = state-change
```

## STEP 4 — HIGH-LEVEL boxes (write-path + read-path ALAG)
```
   ── WRITE (news andar) ──
   Sources(1000) -> Fetcher/Crawler -> Queue(Kafka) -> Worker(clean+DEDUPE+category) -> DB + Cache

   ── READ (user padhe) ──
   User -> LB -> Feed Service -> Cache (pehle yahan) --miss--> DB (read replica)

   har box KYUN:
     Fetcher  -> sources se kheenchta (poll/RSS), alag taaki feed pe asar na ho
     Queue    -> 1000 source ek saath (spike) -> absorb, worker apni speed se khaye
     Worker   -> raw ganda -> clean + DEDUPE (ek khabar 5 source pe) + category
     DB       -> permanent store
     Cache    -> read-heavy -> ready feed RAM mein -> DB har baar na maaro
     Feed Svc -> feed banata, pehle cache, miss pe DB-replica
     LB       -> kai feed-svc instance pe load baanto
   CORE decision: WRITE-path (slow, background) aur READ-path (fast) ALAG -> ek doosre ko slow na karein
```

## STEP 5 — DATA MODEL + DB (KYUN bolo)
```
   ARTICLE:  id(KEY) | title | content | sourceId | category | publishedAt | url
   SOURCE:   id(KEY) | name | rssUrl | lastFetchedAt
   (opt) USER_PREFS: userId | categories[] | savedArticles[]

   DB choice -> NoSQL (Mongo/Cassandra):
     news = massive (crore, badhta) + simple structure + read-heavy + ACID na chahiye (paisа nahi)
     NoSQL = horizontal scale easy + flexible schema + eventual-consistency chalega
   CONTRAST (JP/finance flavor): News -> NoSQL | PAISA/ledger -> HAMESHA SQL/ACID
   -> "data ka nature dekho phir DB choose karo"
```

## STEP 6 — DEEP DIVE (asli khel): feed FAST kaise rahe?
```
   OPTION 1 — ON-THE-FLY (har request DB query "latest 20")
       simple/fresh PAR har request DB maare -> spike pe DB marr jaaye. NAHI.
   OPTION 2 — CACHE / PRECOMPUTE (BEST)
       ready feed (latest 20) Redis mein -> user seedha cache se -> turant, DB ko haath nahi
       nayi news -> cache refresh. 5-min purani chalegi (news mein OK, paisа nahi)
   OPTION 3 — FANOUT (har user ki personalized feed pehle se)
       personalized+fast PAR 10 lakh alag feed banao+store -> mehnga. sirf personalized chahiye to.
   WINNER: OPTION 2 -> sabko lagभग SAME latest feed -> ek cache sab use -> read-heavy perfect
       flow: nayi news -> worker -> DB+cache refresh | user -> cache(99%) --miss--> DB-replica -> wapas cache
   -> trade-off bol ke choose = asli marks yahin
```

## STEP 7 — BOTTLENECK / scale (har box: 10x load / failure?)
```
   Feed Service   -> kai instance + LB
   DB read load   -> READ REPLICAS + cache (99% cache se)
   DB storage     -> SHARD (category/date) + purani news ARCHIVE (cold storage)
   Fetcher        -> kai parallel + queue backlog absorb; source down -> retry+skip (system na gire)
   Cache (Redis)  -> cluster (replicas) + miss pe DB-replica fallback
   SPOF: "ek box gira to pura system gire?" -> wahi replicate karo
```

## STEP 8 — WRAP (ek saans + improve)
```
   WRITE: sources -> fetcher -> queue -> worker(clean+dedupe+category) -> DB(NoSQL)+cache
   READ:  user -> LB -> feed svc -> cache(99%) --miss--> DB read-replica
   DB: NoSQL (massive+simple, ACID na chahiye) | DEEP: feed precompute+cache
   SCALE: svc-LB, DB replica+shard+archive, fetcher parallel+retry, cache cluster
   IMPROVE: personalized (category-wise cache), CDN images, ML ranking, real-time push (breaking)
```

---
> CORE pattern (har design same): clarify -> scale -> API -> boxes -> data+DB(kyun) -> DEEP DIVE(options->choose)
> -> bottleneck -> wrap. META: think out loud, trade-off har choice, chup mat baitho.
> News twist: write-path/read-path ALAG + feed precompute+cache (read-heavy). Paisа hota to SQL/ACID.
