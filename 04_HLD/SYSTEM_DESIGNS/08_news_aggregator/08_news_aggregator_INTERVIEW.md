# News Aggregator

> Alag-alag source (NDTV, TOI, BBC ...) se khabar kheencho -> store karo -> user ko EK feed (Google News / Inshorts jaisa). JP ka aam product design.
> Is design ka dil: **padhne wale bahut zyada, likhne wale kam** + **likhne ka raasta aur padhne ka raasta ALAG** + **feed pehle se bana ke cache me**.

---

## KAHANI (ek din, election result wala; har dikkat se design me ek dabba judta)

```
   KYA HUA                                       ILAAJ                                 DABBA JUDA
1  subah 7, sabko wahi 20, har baar DB pe     -> FEED CACHE (Redis SORTED SET)         Redis
2  meri feed me khabar 20 min purani          -> KAFKA: laana aur likhna alag          Kafka + Worker
3  8 baje election result, 40 guna bheed      -> LB + READ REPLICA                     LB + Read replica
4  dopahar "India ne match jeeta" 5 baar      -> DEDUPE (title hash + SimHash)         (Worker me dedupe)
5  dopahar baad ek source 30 sec atka         -> TIMEOUT + CIRCUIT BREAKER (fuse)      (Fetcher me fuse)
6  shaam ko mujhe sirf tech + sports          -> CATEGORY FEED, padhte waqt milao      (Redis me feed:tech)
7  raat ko maine "cricket" dhoondha           -> INVERTED INDEX                        Elasticsearch
8  raat DBA mail: 2 crore row, kharcha        -> mahine ka PARTITION, purana ARCHIVE   Archive (S3)
9  mail aage: 100 guna, ek DB nahi jhelegi    -> SHARD hash(article_id)                (DB ke tukde)

POORA DESIGN (upar ke dabbe kram se):
   LIKHNA: Sources -> Fetcher (timeout + fuse) -> Kafka -> Worker (saaf + dedupe + category)
           -> DB + Redis + ES
   PADHNA: User -> LB -> Feed Svc x N -> Redis (99%) -> nahi mila to Read replica
           search = ES · purana = Archive
```

---

## TASVEER (ByteByteGo / Alex Xu · CC BY-NC-ND 4.0)

![How to Avoid Crawling Duplicate URLs at Google Scale?](https://assets.bytebytego.com/diagrams/0089-bloomfilter.png)
Source: [How to Avoid Crawling Duplicate URLs at Google Scale?](https://bytebytego.com/guides/how-to-avoid-crawling-duplicate-urls-at-google-scale/)
(crawler + DEDUPE — BLOOM FILTER: "ye URL pehle dekha?" kam memory me; kabhi-kabhi galat "haan", galat "na" kabhi nahi)

---

## SHURU: pehle poocho, phir numbers

```
POOCHO:  "Do hisse: khabar andar laana (ingestion) aur feed dikhana — kis pe focus?"
         kitne SOURCE? 10 ya 10,000? · kitni TAAZI? turant chahiye ya
         5 min purani chalegi?  <- chalegi = pehle se bana lo + cache
         feed sabki SAME ya har user ki ALAG? · kitne user, kitni khabar / din?

KYA KARE (FR):    sources se kheencho · store · FEED (taazi list) · (optional) category / search
                  scope bahar: comments · ML ranking
KAISA CHALE (NFR): feed TEZ (dil) · khabar TAAZI · lakhon user · ek source band ho to system na gire
                   SOCH: "har user feed kholta, khabar gine-chune source se aati -> padhna >> likhna"

NUMBERS (andaaza):
   10 lakh user x 5 baar / din = 50 lakh padhai / din
      din me ~1 lakh second -> ~50 / second (subah 10 guna ~500 / second)
   1000 source, har 5 min -> ~3 lakh fetch / din, nayi khabar ~1 lakh / din -> ~1 likhai / second
   -> padhna >> likhna -> CACHE + READ REPLICA (dil)
   -> likhna peeche chale -> QUEUE
   -> data badhta jaata -> purana ARCHIVE (bahut bade scale pe SHARD)
```

---

## ════ MOCK SE PEHLE: SIRF YE CARD PADHO ════

> Har card: pehle PEELA DABBA dekho (scene + ilaaj). Yaad aa gaya -> aage. Nahi aaya -> neeche
> DIKKAT / HAL / NAHI padho. AAGE POOCHEGA sirf jab interviewer aage kuch poochhe.

#### DABBA 0: SABSE SIMPLE (yahan se shuru, phir "kahan tootega")
```
Fetcher sources se khabar laaye, DB me daale. Feed service DB se "taazi 20" nikaal ke de.
```
```mermaid
flowchart LR
    n_USER["USER"]
    n_Feed_Svc["Feed Svc"]
    n_DB["DB"]
    n_Fetcher["Fetcher"]
    n_USER --> n_Feed_Svc
    n_Feed_Svc --> n_DB
    n_Fetcher --> n_DB
```

#### 1 Subah maine app khola, sabko wahi 20 khabar, har baar DB se
```mermaid
flowchart LR
    n_USER["USER"]
    n_Feed_Svc["Feed Svc"]
    n_Redis["Redis<br/>sabko wahi 20, DB pe 500 / sec<br/>-> sorted set feed cache"]
    n_DB["DB"]
    n_Fetcher["Fetcher"]
    n_USER --> n_Feed_Svc
    n_Feed_Svc --> n_Redis
    n_Feed_Svc --> n_DB
    n_Fetcher --> n_DB
    n_Fetcher --> n_Redis
    style n_Redis fill:#f9a825,color:#000
```
```
AAGE POOCHEGA:
  NIYAM: sabko ek jaisa = ek baar bana ke Redis me; nayi khabar aate hi list me apni jagah.
  "Kaise kaam karta?"              -> pehle har user pe DB "ORDER BY published_at DESC LIMIT 20". Ab Redis
                                      SORTED SET feed:latest (score = kab chhapi): nayi khabar ZADD, top 500
                                      rakho; khola -> ZREVRANGE feed:latest 0 19 -> 20 id. ~99% DB tak nahi.
  "Breaking turant chahiye?"       -> aate hi ZADD, asal me turant dikhti; "5 min" sirf sabse bura haal.
  "Neeche scroll (21-40)?"         -> ZREVRANGE 20 39, ya cursor (aakhri dekhi ke time se pehle ki 20).
```

#### 2 Meri feed me khabar 20 minute purani
```mermaid
flowchart LR
    n_USER["USER"]
    n_Feed_Svc["Feed Svc"]
    n_Redis["Redis (taazi feed)"]
    n_DB["DB"]
    n_Worker["Worker"]
    n_Kafka["Kafka<br/>khabar 20 min purani<br/>-> laana aur likhna alag"]
    n_Fetcher["Fetcher"]
    n_Sources["Sources"]
    n_USER --> n_Feed_Svc
    n_Feed_Svc --> n_Redis
    n_Feed_Svc --> n_DB
    n_Worker --> n_DB
    n_Worker --> n_Redis
    n_Kafka --> n_Worker
    n_Fetcher --> n_Kafka
    n_Sources --> n_Fetcher
    style n_Kafka fill:#f9a825,color:#000
    style n_Worker fill:#f9a825,color:#000
```
```
AAGE POOCHEGA:
  NIYAM: khabar laana peeche apni speed se; main sirf Redis ki taiyaar list padhta.
  "Kaise kaam karta?"              -> pehle ek fetcher 1000 source ek-ek laata + DB me likhta = ~20 min. Ab
                                      kai fetcher -> KAFKA (disk pe diary); workers apni speed se, OFFSET
                                      (kahan tak padha) yaad. 10 PARTITION (tukde) = 10 worker.
  "Likhna, padhna alag kyun?"      -> khabar laana peeche, user ko hamesha cache se taiyaar feed.
  "1000 ki khabar ek saath aayi?"  -> Kafka ki line lambi hui, kuch gira nahi.
  "Ek source kitni baar dekhe?"    -> har source ka apna samay (NDTV har 1 min, chhota blog har 15 min).
  "Kuch naya hi nahi aaya to?"     -> ETag / Last-Modified: source bole "kuch naya nahi" to download hi nahi.
  "Worker ne do baar likhi?"       -> URL / text hash pe UNIQUE -> INSERT ... ON CONFLICT DO NOTHING -> skip.
  "Kafka kyun, SQS kyun nahi?"     -> SQS bhi chalta; Kafka me search / analytics apne group se, dobara bhi.
```

#### 3 8 baje election result, 40 guna bheed
```mermaid
flowchart LR
    n_USER["USER"]
    n_LB["LB"]
    n_Feed_Svc_xN["Feed Svc x N"]
    n_Redis["Redis (taazi feed)"]
    n_Read_replica["Read replica<br/>result ki subah 40 guna<br/>-> LB + read replica"]
    n_DB["DB"]
    n_KW["Kafka -> Worker"]
    n_SF["Sources -> Fetcher"]
    n_USER --> n_LB
    n_LB --> n_Feed_Svc_xN
    n_Feed_Svc_xN --> n_Redis
    n_Feed_Svc_xN --> n_Read_replica
    n_DB --> n_Read_replica
    n_KW --> n_DB
    n_KW --> n_Redis
    n_SF --> n_KW
    style n_LB fill:#f9a825,color:#000
    style n_Read_replica fill:#f9a825,color:#000
```
```
DIKKAT     8 baje election result: aam 500 / sec ki jagah 20,000 / sec. Ek Feed Svc machine ka CPU bhara,
           cache miss seedha primary DB pe, jahan fetcher bhi likh raha -> dono dheeme.
HAL        LB + READ REPLICA: Feed Svc kuch yaad nahi rakhti -> kai machine + LB. Redis me na mile to DB ki
           copy (replica) se padho; main DB sirf likhne ke liye.
NAHI       Kafka se bheed sambhalo: Kafka likhne ke raaste ke liye hai, padhai ki bheed nahi jhelta.
```
```
AAGE POOCHEGA:
  NIYAM: padhai copy pe, likhai main pe; jo kuch yaad nahi rakhta, uski machine badhao.
  "Pata hai 8 baje bheed aati?"    -> 7:45 pe machine badhao (scheduled scaling) + cache pehle se bharo.
  "Replica kitni peeche?"          -> kuch second, news me chalta.
```

#### 4 Dopahar "India ne match jeeta", meri feed me 5 baar
```mermaid
flowchart LR
    n_USER["USER"]
    n_LB["LB"]
    n_Feed_Svc_xN["Feed Svc x N"]
    n_Redis["Redis (taazi feed)"]
    n_DBR["DB + Read replica"]
    n_Worker["Worker<br/>ek khabar 5 baar<br/>-> hash + SimHash dedupe"]
    n_Kafka["Kafka"]
    n_SF["Sources -> Fetcher"]
    n_USER --> n_LB
    n_LB --> n_Feed_Svc_xN
    n_Feed_Svc_xN --> n_Redis
    n_Feed_Svc_xN --> n_DBR
    n_Worker --> n_DBR
    n_Worker --> n_Redis
    n_Kafka --> n_Worker
    n_SF --> n_Kafka
    style n_Worker fill:#f9a825,color:#000
```
```
AAGE POOCHEGA:
  NIYAM: ek khabar ek baar: pehchaan (hash) same = khabar same, URL chahe alag.
  "Kaise kaam karta?"              -> title chhote akshar, chinh hatao -> hash; same hash = same khabar. Thoda
                                      alag ("India wins" / "won") -> SimHash, "90% same" = duplicate.
  "Wahi URL dobara le aaya?"       -> BLOOM FILTER: lambi bit-line + kuch hash; URL ke hash wali jagah dekho:
                                      koi 0 = pakka naya · saare 1 = SHAYAD dekha (galat "na" kabhi nahi).
  "Bloom kyun, Redis SET nahi?"    -> crore URL = bahut RAM; Bloom thodi memory. Galat "haan" se kabhi ek
                                      nayi khabar chhoot sakti, news me chalta.
  "Bloom se URL hatana?"           -> normal Bloom me nahi; roz naya filter, purana phenko (ya counting Bloom)
  "5 me se kaunsa source dikhao?"  -> jo pehle aaya / bada source; baaki "aur sources" me link.
```

#### 5 Dopahar baad meri feed ruki: ek chhota source 30 sec atka, baaki 999 bhi ruke
```mermaid
flowchart LR
    n_USER["USER"]
    n_LB["LB"]
    n_Feed_Svc_xN["Feed Svc x N"]
    n_Redis["Redis (taazi feed)"]
    n_Read_replica["Read replica"]
    n_DB["DB"]
    n_KW["Kafka -> Worker"]
    n_Fetcher["Fetcher<br/>ek source 30 sec atka<br/>-> timeout + circuit breaker"]
    n_USER --> n_LB
    n_LB --> n_Feed_Svc_xN
    n_Feed_Svc_xN --> n_Redis
    n_Feed_Svc_xN --> n_Read_replica
    n_DB --> n_Read_replica
    n_KW --> n_DB
    n_KW --> n_Redis
    n_Fetcher --> n_KW
    style n_Fetcher fill:#f9a825,color:#000
```
```
AAGE POOCHEGA:
  NIYAM: ek beemar source baaki ko na roke: had tak intezaar, phir fuse.
  "Kaise kaam karta?"              -> har site 2-3 sec timeout, thodi retry; baar-baar fail -> fuse OPEN, us
                                      source ko call hi nahi. Baaki 999 apne raaste.
  "Fuse wapas kab judta?"          -> CLOSED -> (kai fail) OPEN -> (thodi der) HALF-OPEN test call -> CLOSED.
  "Source ne 429 diya?"            -> us source ka samay badhao + uska Retry-After maano.
  "Source hamesha ke liye band?"   -> kuch din OPEN raha -> alert, list se hatao.
```

#### 6 Shaam ko mujhe sirf tech aur sports chahiye
```mermaid
flowchart LR
    n_USER["USER"]
    n_LB["LB"]
    n_Feed_Svc_xN["Feed Svc x N"]
    n_Redis["Redis<br/>sirf tech + sports<br/>-> category feed"]
    n_Read_replica["Read replica"]
    n_DB["DB"]
    n_KW["Kafka -> Worker"]
    n_SF["Sources -> Fetcher"]
    n_USER --> n_LB
    n_LB --> n_Feed_Svc_xN
    n_Feed_Svc_xN --> n_Redis
    n_Feed_Svc_xN --> n_Read_replica
    n_DB --> n_Read_replica
    n_KW --> n_DB
    n_KW --> n_Redis
    n_SF --> n_KW
    style n_Redis fill:#f9a825,color:#000
```
```
DIKKAT     Shaam ko mujhe sirf tech aur sports pasand. Har user ki alag feed (10 lakh feed) bahut mehngi.
HAL        CATEGORY FEED: har category ki ek list (feed:tech, feed:sports). Mere liye dono ke top 20, time se
           mila ke (ya Redis ZUNIONSTORE) top 20. 10 lakh feed ki jagah sirf ~20 category feed.
NAHI       har baar DB se WHERE category IN (tech, sports): har khulne pe DB, cache ka fayda gaya.
```
```
AAGE POOCHEGA:
  NIYAM: list category ki pehle se, milana padhte waqt.
  "Mujhe tech zyada, alag wazan?"  -> tab asli personalization: har user ki ranking service (scope se bahar).
```

#### 7 Raat ko maine "cricket" dhoondha, poori table padhi gayi
```mermaid
flowchart LR
    n_USER["USER"]
    n_LB["LB"]
    n_Feed_Svc_xN["Feed Svc x N"]
    n_Elasticsearch["Elasticsearch<br/>cricket = full scan<br/>-> inverted index"]
    n_Redis["Redis (feed + category)"]
    n_DBR["DB + Read replica"]
    n_KW["Kafka -> Worker"]
    n_SF["Sources -> Fetcher"]
    n_USER --> n_LB
    n_LB --> n_Feed_Svc_xN
    n_Feed_Svc_xN --> n_Elasticsearch
    n_Feed_Svc_xN --> n_Redis
    n_Feed_Svc_xN --> n_DBR
    n_KW --> n_DBR
    n_KW --> n_Redis
    n_KW --> n_Elasticsearch
    n_SF --> n_KW
    style n_Elasticsearch fill:#f9a825,color:#000
```
```
AAGE POOCHEGA:
  NIYAM: ES sirf dhoondhta (id deta), sach DB me; ES hamesha dobara ban sakta.
  "Kaise kaam karta?"              -> pehle LIKE '%cricket%' = full scan. INVERTED INDEX = kitaab ke peeche ki
                                      index (HashMap): shabd -> kin khabaron me. ES se id, text DB / cache se.
  "'cricket kohli' kaise?"         -> "cricket" -> [a12, a45, a90] · "kohli" -> [a45, a77]
                                      OR = dono jodo, AND = jo dono me (a45) -> score se sort.
  "ES aur DB me alag data?"        -> DB = sach; ES dobara bana lo (reindex); likhna fail -> retry ki line.
  "Galat likha (crikcet)?"         -> ES fuzzy search (1-2 akshar ka farak chalta).
  "Postgres se nahi chalega?"      -> chhote scale pe Postgres full-text (GIN); ES = ranking, galat spelling,
                                      bahut data, DB pe bojh nahi.
```

#### 8 Raat ko DBA ka mail: 6 mahine me ~2 crore row, kharcha badhta ja raha
```mermaid
flowchart LR
    n_USER["USER"]
    n_LF["LB -> Feed Svc x N"]
    n_Elasticsearch["Elasticsearch"]
    n_Redis["Redis (feed + category)"]
    n_DBR["DB + Read replica"]
    n_Archive["Archive<br/>2 crore row, kharcha<br/>-> mahine ka partition"]
    n_KW["Kafka -> Worker"]
    n_SF["Sources -> Fetcher"]
    n_USER --> n_LF
    n_LF --> n_Elasticsearch
    n_LF --> n_Redis
    n_LF --> n_DBR
    n_DBR --> n_Archive
    n_KW --> n_DBR
    n_KW --> n_Redis
    n_KW --> n_Elasticsearch
    n_SF --> n_KW
    style n_Archive fill:#f9a825,color:#000
```
```
DIKKAT     Usi raat DBA ka mail: 6 mahine me ~2 crore khabar ki row, disk / backup ka kharcha badh raha.
           Taazi 20 ke alawa purana koi padhta hi nahi. (Query dheemi NAHI hui: published_at pe index hai.)
HAL        PARTITION + ARCHIVE: Postgres me har mahine ka alag tukda. Purana -> DETACH -> S3 archive.
           Jo kabhi nahi chahiye -> tukda DROP (ek pal ka kaam).
NAHI       "sharding karo": ye purana hatana (retention) hai, data baantna (sharding) nahi.
```
```
AAGE POOCHEGA:
  NIYAM: hafta garam, baaki thanda; purana hatana = retention, sharding nahi.
  "1 saal purani khabar khuli?"    -> Archive / S3 se aayegi (dheemi chalegi).
  "DETACH pe table lock hoga?"     -> normal DETACH = poore table pe ACCESS EXCLUSIVE (pal bhar, feed padhai
                                      bhi rukti); PG 14+ DETACH PARTITION ... CONCURRENTLY -> kuch nahi rukta.
  "3 saal baad?"                   -> wahi: hafta garam, baaki thanda.
```

#### 9 Mail ka doosra hissa: source 100 guna hue to ek DB machine nahi jhelegi
```mermaid
flowchart LR
    n_USER["USER"]
    n_LF["LB -> Feed Svc x N"]
    n_RES["Redis + Elasticsearch"]
    n_Read_replica["Read replica"]
    n_DB["DB<br/>ek machine nahi jhelti<br/>-> article_id hash se shard"]
    n_Archive["Archive"]
    n_KW["Kafka -> Worker"]
    n_SF["Sources -> Fetcher"]
    n_USER --> n_LF
    n_LF --> n_RES
    n_LF --> n_Read_replica
    n_DB --> n_Read_replica
    n_DB --> n_Archive
    n_KW --> n_DB
    n_KW --> n_RES
    n_SF --> n_KW
    style n_DB fill:#f9a825,color:#000
```
```
DIKKAT     Mail aage: "source 100 guna ya log khud content daalne lage to ek DB likhai aur data nahi jhelegi."
           (Hamare number pe ek DB kaafi hai, ye sirf bahut bade scale pe.)
HAL        SHARD (data tukdon me, alag machine) hash(article_id) se -> likhai sab tukdon pe barabar.
           Keemat: "taazi 20" ke liye har tukde se thoda (scatter-gather), par wo to Redis se aata hi hai.
NAHI       date se baanto: aaj ki saari likhai aaj wale shard pe -> wo akela garam.
```
```
AAGE POOCHEGA:
  NIYAM: baantne ki key wo jo bojh barabar baante.
  "Category se baanto?"            -> sports / politics bade, baaki chhote -> bojh barabar nahi (skew).
  "Shard badhaye, data kitna?"     -> consistent hashing -> thoda sa data hilta.
```

---

## AAKHRI DABBA + WRAP (ek hi baar)

```
LIKHNA: Sources -> Fetcher (timeout / dobara / skip + fuse) -> Kafka (bheed sambhaale)
        -> Worker (saaf + dedupe + category) -> DB + Redis + Elasticsearch
PADHNA: LB -> Feed Svc (kai machine) -> Redis (99%, taazi feed pehle se bani) -> nahi mila to read replica
        search = Elasticsearch · purani khabar = Archive

DB:  Postgres (primary + read replica + mahine ke tukde) -> likhai sirf ~1 / second, ek machine kaafi;
     "taazi 20" index se seedha; padhai Redis aage se sambhaalta
     (likhai 100 guna ho jaaye tab hi Cassandra jaisa / shard socho)
```
```
JAWAB:    Khabar laana aur khabar dikhana alag raaste. Peeche fetcher sites se laata, Kafka se worker
          tak, worker saaf karke, ek khabar ek baar, category laga ke DB me rakhta.
          Sabko lagbhag wahi feed, to Redis me pehle se bani, 99% wahin se. Feed service kai machine pe,
          purani khabar sasti jagah, search Elasticsearch pe.
```
```mermaid
flowchart LR
    n_USER["USER"]
    n_LB["LB"]
    n_Feed_Svc_x_N_1["Feed Svc 1"]
    n_Feed_Svc_x_N_2["Feed Svc 2"]
    n_Elasticsearch["Elasticsearch"]
    n_Redis["Redis (feed + category)"]
    n_Read_replica["Read replica"]
    n_DB["DB"]
    n_Archive["Archive"]
    n_Worker["Worker (saaf + dedupe)"]
    n_Kafka["Kafka"]
    n_Fetcher["Fetcher (timeout + fuse)"]
    n_Sources["Sources"]
    n_USER --> n_LB
    n_LB --> n_Feed_Svc_x_N_1
    n_LB --> n_Feed_Svc_x_N_2
    n_Feed_Svc_x_N_1 --> n_Elasticsearch
    n_Feed_Svc_x_N_2 --> n_Elasticsearch
    n_Feed_Svc_x_N_1 --> n_Redis
    n_Feed_Svc_x_N_2 --> n_Redis
    n_Feed_Svc_x_N_1 --> n_Read_replica
    n_Feed_Svc_x_N_2 --> n_Read_replica
    n_DB --> n_Read_replica
    n_DB --> n_Archive
    n_Worker --> n_DB
    n_Worker --> n_Redis
    n_Worker --> n_Elasticsearch
    n_Kafka --> n_Worker
    n_Fetcher --> n_Kafka
    n_Sources --> n_Fetcher
```

---

## 10 GUNA TRAFFIC — har dabba alag se

```
PADHNA: Feed Svc  -> kai machine + LB
        Redis     -> gira? cluster + replica, nahi mila to read replica
        DB        -> READ REPLICA · jagah -> ARCHIVE (+ bahut bade scale pe shard)
LIKHNA: Fetcher   -> source band / dheema -> timeout + dobara + skip + fuse
        Kafka     -> 1000 ek saath -> line sambhaale, partition badhao
        Worker    -> kai worker ek saath
AAGE:   pasand wali feed (category cache) · photo CDN · ML ranking · badi khabar turant push

POOCHEGA: "How would you scale this to 10x?"      -> dono raaste alag chalao, pehle jo toote
POOCHEGA: "What's the single point of failure?"   -> "ek machine giri to sab?" -> uski copy
                                                     · source band -> fuse + skip
POOCHEGA: "How do you know it's working?"         -> feed p99 ·
          cache hit rate · Kafka lag · source error rate · alert
```

---

## POOCHE TO (gehra sawaal aaye tab)

```
API:      GET /feed?page=1&category=tech -> taazi list (page me, neeche scroll; 10,000 ek saath nahi)
          GET /article/{id} · GET /search?q=cricket · POST /ingest { source data } (andar, peeche)
          GET = padhna · POST = kuch badalna (ulta mat karna)

DB:       ARTICLE: id | title | content | sourceId | category | publishedAt | url
          SOURCE:  id | name | rssUrl | lastFetchedAt
          USER_PREFS (optional): userId | categories[] | savedArticles[]
          Postgres: likhai ~1 / second (chhoti), "ORDER BY published_at DESC LIMIT 20" index se,
                    primary + read replica, mahine ke tukde (partition) -> purana archive
          Mongo bhi chal jaata (replica set + TTL). Cassandra tab jab likhai bahut badi ho jaaye;
          par Cassandra me primary-replica nahi (sab node barabar, leaderless)
          aur poori table pe "taazi 20" nahi chalta -> isliye yahan nahi.
```

---

ARCHETYPE A+E · CONCEPTS: [elasticsearch](../../FOUNDATIONS/12_elasticsearch_search.md) · [caching](../../FOUNDATIONS/04_caching.md) · saath: [twitter-feed](../03_twitter_feed/03_twitter_feed.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
