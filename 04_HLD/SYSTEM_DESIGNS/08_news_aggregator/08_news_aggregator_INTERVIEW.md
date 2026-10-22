# News Aggregator

> Alag-alag source (NDTV, TOI, BBC ...) se khabar kheencho, store karo, user ko ek feed do (Google News / Inshorts jaisa).
> Dil teen cheezein: padhne wale bahut zyada, likhne wale kam. Likhne ka raasta aur padhne ka raasta alag. Feed pehle se bana ke cache me.
>
> Mock me sirf heading + Cue + poster. Poster me sab: upar "Pehle aisa tha", tasveer (ilaaj), hara "Kyun chalta", laal number = kya toot sakta + kaise bacha, peela = doosra tareeka kyun nahi, grey = baaki.
> Cross-Q: hal ke har bade shabd pe socho -> gira? · dheema / bheed? · do ek saath? · galat hua? · kaise / kitna?

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c9_2-edd65c62.svg)

---


## Shuru

```
FR      sources se khabar kheencho, store karo, user ko feed do (taazi list), chaho to category / search.
        Comments aur ML ranking scope se bahar hain
NFR     feed tez khule (yahi dil hai), khabar taazi ho, lakhon user jhele, ek source band ho to
        system na gire. Har user feed kholta hai aur khabar gine-chune source se aati hai,
        to padhna likhne se bahut zyada
NUMBERS 10 lakh user x 5 baar = 50 lakh padhai roz, din ~1 lakh sec, to ~50 / sec
        (subah 10 guna, ~500 / sec)
        1000 source har 5 min = ~3 lakh fetch roz. Nayi khabar ~1 lakh roz = ~1 likhai / sec
        padhna likhne se bahut zyada, to cache + read replica. Likhna peeche, to queue.
        Data badhta hai, to purana archive (bahut bade scale pe shard)
PEHLE   sabse simple: Fetcher sources se laata hai, DB me daalta hai. Feed Svc har baar DB se "taazi 20"
        sort karke deta hai
```

> [!IMPORTANT]
> **RAIL: har design isi kram me chalta (8 dabbe, sab file me same)**
> kaam chala → bheed aayi → machine giri → do log ek saath → kuch dheema → data bada → door ke log → bahar ka hamla
>
> | rail | is design me | card |
> |---|---|---|
> | 1 kaam chala | sab source se khabar laao, feed me dikhao · tech / sports ki feed · "cricket" search | 6, 7 |
> | 2 bheed aayi | subah sabko wahi 20 khabar, har baar DB · 8 baje result, 40 guna | 1, 3 |
> | 3 machine giri | Redis restart, list khaali (card 1 ka cross-Q) · worker commit se pehle gira | 1, 2 |
> | 4 do log ek saath | wahi khabar 5 source se (**is design ka khaas: dedupe**) | 4 |
> | 5 kuch dheema | fetcher ek source pe atka, khabar 20 min purani · chhota source 30 sec, 999 ruke | 2, 5 |
> | 6 data bada | 6 mahine me 2 crore row · source 100 guna, ek DB nahi | 8, 9 |
> | 7 door ke log | nahi aata | - |
> | 8 bahar ka hamla | source ka 429 (card 5 ka cross-Q) | 5 |


> [!NOTE]
> **AWS pe kaunsa DB (interview me naam bolna ho to)** · SQL wala = RDS, Cassandra RDS pe nahi chalta
>
> - Khabar (SQL) → **AWS RDS (Postgres / MySQL)**: url UNIQUE, mahine ke tukde, index se tez
> - Feed (Redis sorted set) → **ElastiCache (Redis)**: taazi 20 turant
> - Search (Elasticsearch) → **Amazon OpenSearch**: shabd se dhoondhna

---

*ab tootega kahan? sabse pehle padhne pe, kyunki sab subah ek saath kholte hain*

## 1. Subah maine app khola, sabko wahi 20 khabar, har baar DB se

**Rail 2 BHEED AAYI** · Cue: FEED CACHE (Redis SORTED SET)

<img src="img/c1_poster.png?v=2" width="1000" alt="card 1: kya toota, kaise bacha">


---

*ab padhna tez ho gaya, par khabar hi der se aa rahi*

## 2. Meri feed me khabar 20 minute purani

**Rail 5 KUCH DHEEMA (fetch)** · Cue: KAFKA: laana aur likhna alag

<img src="img/c2_poster.png?v=2" width="1000" alt="card 2: kya toota, kaise bacha">


---

*ab khabar taazi, par 8 baje sab ek saath aa gaye*

## 3. 8 baje election result, 40 guna bheed

**Rail 2 BHEED AAYI (result)** · Cue: LB + READ REPLICA

<img src="img/c3_poster.png?v=2" width="1000" alt="card 3: kya toota, kaise bacha">


---

*ab bheed sambhal li, par feed me wahi khabar baar-baar*

## 4. Dopahar "India ne match jeeta", meri feed me 5 baar

**Rail 4 DO LOG EK SAATH (khaas)** · Cue: DEDUPE (title hash + SimHash)

<img src="img/c4_poster.png?v=2" width="1000" alt="card 4: kya toota, kaise bacha">


---

*ab khabar saaf, par ek source hi atak gaya*

## 5. Dopahar baad nayi khabar aani ruki: ek chhota source 30 sec atka, baaki 999 bhi ruke

**Rail 5 KUCH DHEEMA (source)** · Cue: TIMEOUT + CIRCUIT BREAKER

<img src="img/c5_poster.png?v=2" width="1000" alt="card 5: kya toota, kaise bacha">


---

*ab sab khabar aa rahi, par mujhe sab nahi chahiye*

## 6. Shaam ko mujhe sirf tech aur sports chahiye

**Rail 1 KAAM CHALA (category)** · Cue: CATEGORY FEED, padhte waqt milao

<img src="img/c6_poster.png?v=2" width="1000" alt="card 6: kya toota, kaise bacha">


---

*ab feed ban gayi, par dhoondhna dheema*

## 7. Raat ko maine "cricket" dhoondha, poori table padhi gayi

**Rail 1 KAAM CHALA (search)** · Cue: INVERTED INDEX

<img src="img/c7_poster.png?v=2" width="1000" alt="card 7: kya toota, kaise bacha">


---

*ab sab chal raha, par data bhar raha*

## 8. Raat ko DBA ka mail: 6 mahine me ~2 crore row, kharcha badhta ja raha

**Rail 6 DATA BADA (purana)** · Cue: mahine ka PARTITION, purana ARCHIVE

<img src="img/c8_poster.png?v=2" width="1000" alt="card 8: kya toota, kaise bacha">


---

*ab jagah theek, par source hi 100 guna ho gaye*

## 9. Mail ka doosra hissa: source 100 guna hue to ek DB machine nahi jhelegi

**Rail 6 DATA BADA (likhai)** · Cue: SHARD hash(article_id)

<img src="img/c9_poster.png?v=2" width="1000" alt="card 9: kya toota, kaise bacha">


---

## Poora design ek saans me

```
LIKHNA: Sources -> Fetcher (timeout / dobara / skip + breaker) -> Kafka (bheed sambhaale)
        -> Worker (saaf + dedupe + category) -> DB + Redis + Elasticsearch
PADHNA: LB -> Feed Svc (kai machine) -> Redis (99%, taazi feed pehle se bani) -> nahi mila to read replica
        search = Elasticsearch · purani khabar = Archive
DB:     Postgres (primary + read replica + mahine ke tukde), likhai sirf ~1 / second
```

Bolne ke liye: khabar laana aur khabar dikhana alag raaste. Peeche fetcher sites se laata, Kafka se worker tak, worker
saaf karke, ek khabar ek baar, category laga ke DB me rakhta. Sabko lagbhag wahi feed, to Redis me pehle se bani, 99%
wahin se. Feed service kai machine pe, purani khabar sasti jagah, search Elasticsearch pe.

---

## 10 guna traffic

- **Feed Svc**: kai machine + LB.
- **Redis**: gira to cluster + replica. Nahi mila to read replica.
- **DB**: read replica. Jagah ke liye archive (bahut bade scale pe shard).
- **Fetcher**: source band ya dheema to timeout + dobara + skip + circuit breaker.
- **Kafka**: 1000 ek saath aaye to line sambhaale, partition badhao.
- **Worker**: kai worker ek saath.
- **Aage**: pasand wali feed (category cache), photo CDN, ML ranking, badi khabar turant push.

**Pooche: 10 guna kaise?** Dono raaste alag chalao, pehle jo toote use badhao.

**Pooche: kahan se sab gir sakta?** Ek machine giri to sab? Uski copy. Source band to circuit breaker + skip.

**Pooche: chal raha hai, kaise pata?** Feed ka p99, cache hit rate, Kafka lag, source error rate, alert.

---

## Pooche to (kisi card me nahi baitha)

API: `GET /feed?page=1&category=tech` -> taazi list, page me (neeche scroll, 10,000 ek saath nahi).
`GET /article/{id}`, `GET /search?q=cricket`, aur andar peeche `POST /ingest { source data }`. GET = padhna, POST =
kuch badalna, ulta mat karna.

Row: ARTICLE = id, title, content, sourceId, category, publishedAt, url. SOURCE = id, name, rssUrl, lastFetchedAt.
USER_PREFS (optional) = userId, categories[], savedArticles[].

DB kyun Postgres: likhai ~1 har second (chhoti), `ORDER BY published_at DESC LIMIT 20` index se seedha, primary +
read replica, mahine ke tukde, purana archive. Padhai Redis aage se sambhaal leta.

Mongo bhi chal jaata (replica set + TTL). Cassandra tab jab likhai bahut badi ho jaaye (100 guna). Par Cassandra me
primary-replica nahi (sab node barabar, leaderless), aur poori table pe "taazi 20" nahi chalta, isliye yahan nahi.

- **Pehle poochho:** do hisse, khabar andar laana (ingestion) aur feed dikhana, kis pe focus?
- **Pehle poochho:** kitne source, 10 ya 10,000?
- **Pehle poochho:** feed sabki same hai ya har user ki alag?
- **Pehle poochho:** kitne user, kitni khabar roz?

---

ARCHETYPE A+E · CONCEPTS: [elasticsearch](../../FOUNDATIONS/12_elasticsearch_search.md) · [caching](../../FOUNDATIONS/04_caching.md) · saath: [twitter-feed](../03_twitter_feed/03_twitter_feed.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
