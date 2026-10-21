# News Aggregator

> Alag-alag source (NDTV, TOI, BBC ...) se khabar kheencho, store karo, user ko ek feed do (Google News / Inshorts jaisa).
> Dil teen cheezein: padhne wale bahut zyada, likhne wale kam. Likhne ka raasta aur padhne ka raasta alag. Feed pehle se bana ke cache me.
>
> Mock me sirf heading + Cue + kahani ki tasveer. Neeche har card: HAL · KYUN · NAHI, phir YAAD RAKH dabba + CROSS-Q dabba (sawaal → cue, poora jawab).
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

---

*ab tootega kahan? sabse pehle padhne pe, kyunki sab subah ek saath kholte hain*

## 1. Subah maine app khola, sabko wahi 20 khabar, har baar DB se

**Rail 2 BHEED AAYI** · Cue: FEED CACHE (Redis SORTED SET)

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c1_1.png?v=3" width="900">

<img src="img/c1_2.png?v=3" width="900">

<img src="img/c1_3.png?v=3" width="900">

</details>

```
HAL     sabki ek feed ek baar banti hai, Redis me time se (sorted set). Nayi khabar ZADD feed:latest,
        kholne pe ZREVRANGE 0 19 = 20 id. DB tak koi nahi jaata
KYUN    sabko ek jaisi feed hai, to ek baar bana ke Redis me rakho. Nayi khabar aate hi apni jagah baith
        jaati hai, turant dikhti hai
NAHI    har user pe DB se ORDER BY ... LIMIT 20
        -> DHEEMA: yahi pehle tha, DB 500 / sec sort kar raha, feed dheemi. Ab ~99% DB tak nahi jaata
```

> [!IMPORTANT]
> **Yaad rakh:** Feed ek baar banti, Redis sorted set me time se (ZADD). Kholne pe ZREVRANGE 0 19, DB tak nahi.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - feed cache me, to khabar kitni taazi chahiye, 5 min purani chalegi? → **chalegi**: chalegi, isliye feed pehle se bana ke cache me rakhte hain
> - breaking news kitni der me feed me? → **aate hi ZADD, turant**: "5 min" sirf sabse bura haal hai
> - user neeche scroll kare (21-40)? → **cursor (aakhri khabar ka time)**: ZREVRANGE 20 39, ya cursor: aakhri dekhi khabar ke time se pehle ki 20
> - Redis restart, list khaali? → **DB se ek baar banao, lock ke saath**: ek banaye, baaki ruk ke Redis se padhein
> - Redis me list kitni lambi rakhoge? → **top 500, RAM chhoti rehti hai**: Usse purani chahiye to DB se


---

*ab padhna tez ho gaya, par khabar hi der se aa rahi*

## 2. Meri feed me khabar 20 minute purani

**Rail 5 KUCH DHEEMA (fetch)** · Cue: KAFKA: laana aur likhna alag

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c2_1.png?v=3" width="900">

<img src="img/c2_2.png?v=3" width="900">

<img src="img/c2_3.png?v=3" width="900">

</details>

```
HAL     fetcher sirf laata hai aur Kafka me daalta hai (key = source_id). Worker apni speed se Kafka se padh
        ke DB + Redis me likhta hai. Kai fetcher, Kafka me 10 partition, phir worker
KYUN    laana aur padhna alag hai. User sirf Redis list padhta hai, to laane me der ya bheed ho tab bhi feed
        khulti hai, bas thodi purani
        ek source atke to baaki chalte rehte hain
NAHI    fetcher hi DB me likhe
        -> LINE: pehle yahi tha, 1000 source ek-ek karke, A atka to B se Z line me khade
        SQS
        -> CHALTA: chal jaata, par Kafka me search / analytics apne group se wahi khabar padh lete hain,
           dobara bhi
        worker ke dobara likhne ko Bloom se rokna
        -> GALAT: Bloom DB se alag hai, Bloom update se pehle crash ho sakta hai, khabar feed me do baar
```

> [!IMPORTANT]
> **Yaad rakh:** Fetcher sirf laata, Kafka me daalta (key = source). Worker apni speed se DB + Redis. url UNIQUE.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - fetcher gaya, par source pe kuch naya nahi hai? → **ETag / Last-Modified**: ETag / Last-Modified dekho. "Naya nahi" to download hi nahi karte
> - 1000 khabar ek saath Kafka me? → **line lambi, gira kuch nahi**: Kafka disk pe rakhta hai, worker apni speed se padhta hai
> - likha, Kafka commit se pehle crash? → **DB khud rokta: url pe UNIQUE**: naya worker dobara likhe to bhi ON CONFLICT DO NOTHING
> - fetcher har source ko kitni baar dekhega? → **apna-apna samay**: apna-apna samay: NDTV har 1 min, chhota blog har 15 min


---

*ab khabar taazi, par 8 baje sab ek saath aa gaye*

## 3. 8 baje election result, 40 guna bheed

**Rail 2 BHEED AAYI (result)** · Cue: LB + READ REPLICA

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c3_1.png?v=3" width="900">

<img src="img/c3_2.png?v=3" width="900">

<img src="img/c3_3.png?v=3" width="900">

</details>

```
HAL     Feed Service kai machine pe chalti hai, aage LB. Redis miss hua to DB ki copy (read replica) se padho.
        Main DB sirf likhai ke liye
KYUN    padhai copy pe, likhai main pe. Feed Svc kuch yaad nahi rakhti, to machine badhao, koi bhi request le
        copy thodi peeche hai, matlab kuch second purani khabar, koi nuksaan nahi
NAHI    ek hi Feed machine
        -> BHARA: 500 se seedha 20,000 / sec aaye, CPU bhar gaya
        miss pe main DB se padho
        -> DHEEMA: wahan fetcher bhi likh raha hai, bheed ki padhai likhai ko bhi dheema karti hai
```

> [!IMPORTANT]
> **Yaad rakh:** Feed Svc kai machine + LB. Redis miss pe DB ki copy (read replica). Garam key pe App me 2 sec copy.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - 8 baje ki bheed ka pehle se pata tha? → **scheduled scaling + cache pehle**: 7:45 pe machine badhao (scheduled scaling) + cache pehle se bharo
> - 20,000 / sec aur badh gaya? → **aur machine lagao, koi bhi request le**: Feed Svc kuch yaad nahi rakhti, koi bhi request le leti hai
> - read replica peeche reh gayi? → **kuch second, koi nuksaan nahi**: 5 sec purani khabar se koi nuksaan nahi
> - Redis ki ek key pe 20,000 / sec? → **Redis replica + App me copy**: App me 1-2 sec ki copy. Sab ek machine pe nahi aata


---

*ab bheed sambhal li, par feed me wahi khabar baar-baar*

## 4. Dopahar "India ne match jeeta", meri feed me 5 baar

**Rail 4 DO LOG EK SAATH (khaas)** · Cue: DEDUPE (title hash + SimHash)

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c4_1.png?v=3" width="900">

<img src="img/c4_2.png?v=3" width="900">

<img src="img/c4_3.png?v=3" width="900">

</details>

```
HAL     dekha hua URL Bloom filter se pakdo, wahi khabar title ke hash se (Bloom + SimHash).
        Title ke chhote akshar karo, chinh hatao, phir hash
KYUN    same hash matlab same khabar, URL chahe alag ho (5 source, alag URL, thoda alag title).
        Ek khabar feed me ek hi baar
BLOOM   URL ki jagah koi bit 0 hai to pakka naya, sab 1 hain to shayad dekha hai. Kabhi nayi khabar chhoot
        jaaye to news me chalta hai
NAHI    har naya URL = nayi khabar
        -> DOHRAAV: pehle yahi tha, 5 source to feed me 5 baar
        Redis SET me URL rakho
        -> MEHNGA: crore URL = bahut RAM. Bloom ka galat "haan" kabhi ek nayi chhudaye, news me chalta hai
        har worker apna Bloom
        -> ANDHA: ek ne dekha, doosre ko pata nahi. Isliye ek saanjha Bloom (Redis me)
```

> [!IMPORTANT]
> **Yaad rakh:** URL Bloom filter se, wahi khabar title hash se (saaf karke). Shabd badla to SimHash 90%.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - "wins" aur "won", hash alag? → **SimHash: 90% same to duplicate**
> - 5 me se kaunsi khabar dikhaoge? → **pehli ya bade source ki**: jo pehle aayi ya bade source ki. Baaki "aur sources" me link
> - Bloom se URL hatana ho to? → **roz naya filter**: normal Bloom me nahi hota. Roz naya filter, purana phenko (ya counting Bloom)


---

*ab khabar saaf, par ek source hi atak gaya*

## 5. Dopahar baad nayi khabar aani ruki: ek chhota source 30 sec atka, baaki 999 bhi ruke

**Rail 5 KUCH DHEEMA (source)** · Cue: TIMEOUT + CIRCUIT BREAKER

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c5_1.png?v=3" width="900">

<img src="img/c5_2.png?v=3" width="900">

<img src="img/c5_3.png?v=3" width="900">

</details>

```
HAL     thodi der me chhod do (timeout 2-3 sec, thodi retry). Baar-baar fail ho to call hi mat karo
        (circuit breaker). OPEN matlab us source ko call nahi
KYUN    ek beemar source baaki ko nahi rokta. Had tak intezaar, phir breaker. Us source ki khabar kuch der
        nahi aayegi, baaki feed chalti rahegi
NAHI    poora jawab aane tak ruko
        -> ATKA: wahi 30 sec, ek atka to 999 peeche khade
```

> [!IMPORTANT]
> **Yaad rakh:** 2-3 sec timeout. Baar-baar fail to breaker OPEN, call band. Thodi der baad HALF-OPEN: ek test call.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - source ne 429 diya? → **dheere chalo**: samay badhao, Retry-After maano
> - breaker kuch din se OPEN? → **alert**: source ko list se bahar karo
> - OPEN hai, wapas kab chalu? → **thodi der baad HALF-OPEN: ek test call**: Theek nikla to CLOSED


---

*ab sab khabar aa rahi, par mujhe sab nahi chahiye*

## 6. Shaam ko mujhe sirf tech aur sports chahiye

**Rail 1 KAAM CHALA (category)** · Cue: CATEGORY FEED, padhte waqt milao

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c6_1.png?v=3" width="900">

<img src="img/c6_2.png?v=3" width="900">

<img src="img/c6_3.png?v=3" width="900">

</details>

```
HAL     har category ki list pehle se bani hai (feed:tech, feed:sports). Khabar aayi to apni list me.
        User ne khola to tech + sports ke top 20 time se milao (category feed)
KYUN    list pehle se bani hai, milana padhte waqt hota hai. User ki pasand bas ye batati hai ki kaunsi list
        milaani hai. Sirf ~20 list, 10 lakh nahi
NAHI    har user ki alag feed
        -> MEHNGA: 10 lakh feed, har nayi khabar sab me daalni padti
```

> [!IMPORTANT]
> **Yaad rakh:** Har category ki list pehle se (feed:tech). Kholne pe pasand wali lists milao, id se dedupe.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - khabar 2 category list me? → **dono me ZADD, milate waqt id se dedupe**: taaki do baar na dikhe
> - mujhe tech zyada pasand hai, to? → **personalization, scope bahar**: wo asli personalization hai, har user ki ranking. Scope se bahar


---

*ab feed ban gayi, par dhoondhna dheema*

## 7. Raat ko maine "cricket" dhoondha, poori table padhi gayi

**Rail 1 KAAM CHALA (search)** · Cue: INVERTED INDEX

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c7_1.png?v=3" width="900">

<img src="img/c7_2.png?v=3" width="900">

<img src="img/c7_3.png?v=3" width="900">

</details>

```
HAL     har shabd ki list ki wo kis-kis khabar me hai (inverted index, Elasticsearch).
        "cricket" -> [a12, a45, a90]. ES sirf id deta hai, text DB / cache se aata hai
KYUN    ES sirf dhoondhta hai, sach DB me hai. ES gira ya galat hua to DB se dobara bana lo.
        Khabar nahi khoti, bas search dheemi ya band
NAHI    LIKE '%cricket%'
        -> POORI: % aage hai to index nahi lagta, poori table (2 crore row ek-ek) padhi jaati hai
        Postgres search
        -> CHHOTA: chhote scale pe haan (full-text, GIN). ES chahiye ranking, galat spelling, bahut data
           ke liye, aur DB pe bojh nahi padta
```

> [!IMPORTANT]
> **Yaad rakh:** Har shabd ki list ki wo kis khabar me (inverted index, Elasticsearch). Do shabd = list milao (AND).

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - "cricket kohli" dhoondha? → **dono list milao, AND = a45**: [a12, a45, a90] aur [a45, a77]. AND = a45, OR = sab. Phir score se sort
> - "crikcet" likh diya? → **fuzzy (ES): 1-2 akshar ka farak chal jaata**
> - ES aur DB alag ho gaye? → **DB sach hai, ES dobara bana lo (reindex)**: Likhna fail ho to retry ki line


---

*ab sab chal raha, par data bhar raha*

## 8. Raat ko DBA ka mail: 6 mahine me ~2 crore row, kharcha badhta ja raha

**Rail 6 DATA BADA (purana)** · Cue: mahine ka PARTITION, purana ARCHIVE

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c8_1.png?v=3" width="900">

<img src="img/c8_2.png?v=3" width="900">

<img src="img/c8_3.png?v=3" width="900">

</details>

```
HAL     har mahine ka alag tukda (partition), purana S3 pe (archive). Postgres me sirf haal ke mahine
KYUN    naya mahina garam hai (Postgres, index se tez), purane thande (S3, sasta). Purani khabar khulti hai,
        bas dheemi
        purana hatana retention hai, sharding nahi
NAHI    sab ek table me (May se Oct)
        -> KHARCHA: purana koi padhta nahi, par uski disk + backup ka faltu kharcha
```

> [!IMPORTANT]
> **Yaad rakh:** Har mahine ka partition, purana DETACH karke S3 archive. Postgres me sirf haal ke mahine.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - mahina beet gaya, partition ka kya? → **DETACH -> S3**: DETACH karke S3 pe. Bilkul nahi chahiye to DROP. Pal bhar ka kaam
> - DETACH pe lock lagega? → **normal DETACH: pal bhar ka lock, padhai ruki**: normal DETACH = poore table pe pal bhar ka lock, padhai bhi rukti hai. PG 14+ me DETACH ... CONCURRENTLY, kuch nahi rukta
> - 1 saal purani khabar khuli? → **S3 archive se aati. Dheemi, par khulti**: kabhi-kabhi hi khulti hai
> - 3 saal baad DB kitna bada hoga? → **utna hi**: utna hi. Naya mahina garam, purana thanda (S3)


---

*ab jagah theek, par source hi 100 guna ho gaye*

## 9. Mail ka doosra hissa: source 100 guna hue to ek DB machine nahi jhelegi

**Rail 6 DATA BADA (likhai)** · Cue: SHARD hash(article_id)

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c9_1.png?v=3" width="900">

<img src="img/c9_2.png?v=3" width="900">

<img src="img/c9_3.png?v=3" width="900">

</details>

```
HAL     article_id se tukde, har tukda alag machine pe (shard). hash(a91) -> shard 2.
        Likhai sab tukdon pe barabar
KYUN    baantne ki key wo jo bojh barabar baante, yaani article_id ka hash. Ek shard garam ya mara to sirf
        uska hissa dheema
NAHI    ek hi machine
        -> BHARA: saari likhai wahin, disk aur likhai full
        category se shard
        -> TIRCHHA: bojh tirchha (skew), sports / politics bade, baaki chhote
```

> [!IMPORTANT]
> **Yaad rakh:** article_id ke hash se shard. Taazi 20 Redis se, miss pe har shard se thoda (scatter-gather).

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - shard hain, taazi 20 kahan se? → **Redis se. Miss to har tukde se thoda**: (scatter-gather)
> - shard badhaya? → **thoda hi data hilta (consistent hashing)**: sirf naye shard ke hisse ka
> - do shard pe wahi khabar? → **dedupe pehle saanjhe Bloom / Redis se**: DB ka UNIQUE sirf shard ke andar kaam karta hai


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
