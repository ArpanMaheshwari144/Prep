# News Aggregator

> Alag-alag source (NDTV, TOI, BBC ...) se khabar kheencho, store karo, user ko ek feed do (Google News / Inshorts jaisa).
> Dil teen cheezein: padhne wale bahut zyada, likhne wale kam. Likhne ka raasta aur padhne ka raasta alag. Feed pehle se bana ke cache me.
>
> Har card: kya toota -> kya kiya -> yahi kyun -> doosra kyun nahi. Kram me chalo. "Pooche" wali line = interviewer aage poochhe to.

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c9_2-edd65c62.svg)

---

## Shuru

Sabse simple: Fetcher sources se khabar laata aur DB me daal deta. Feed service DB se "taazi 20" nikaal ke user ko
de deti.

Pehle poochho: do hisse hain, khabar andar laana (ingestion) aur feed dikhana, kis pe focus? Kitne source, 10 ya
10,000? Khabar kitni taazi chahiye, turant ya 5 min purani chalegi? Chalegi, to feed pehle se bana lo aur cache me
rakho. Feed sabki same hai ya har user ki alag? Kitne user, kitni khabar roz?

Kaam (FR): sources se kheencho, store karo, feed do (taazi list), aur chaho to category / search. Comments aur ML
ranking scope se bahar. Kaisa chale (NFR): feed tez (yahi dil), khabar taazi, lakhon user, aur ek source band ho to
system na gire. Har user feed kholta, khabar gine-chune source se aati, to padhna likhne se bahut zyada.

Numbers: 10 lakh user, har ek din me 5 baar, yaani 50 lakh padhai roz. Din me ~1 lakh second, to ~50 har second
(subah 10 guna, ~500 har second). 1000 source, har 5 min, to ~3 lakh fetch roz. Nayi khabar ~1 lakh roz, yaani ~1
likhai har second. Padhna likhne se bahut zyada, isliye cache + read replica. Likhna peeche chale, isliye queue. Data
badhta jaata, isliye purana archive (bahut bade scale pe shard).

---

*ab tootega kahan? sabse pehle padhne pe, kyunki sab subah ek saath kholte hain*

## 1. Subah maine app khola, sabko wahi 20 khabar, har baar DB se

Pehle har baar app khulne pe DB sort karta tha. Laakhon user, DB 500 har second sort kar raha, feed dheemi.

Isliye sabki ek hi feed ek baar banate hain aur Redis me time se rakhte hain (sorted set). Nayi khabar aati hai to
`ZADD feed:latest`, aur feed kholne pe `ZREVRANGE 0 19`, 20 id mil jaati. DB tak koi nahi jaata.

Ye kyun chalta? Sabko ek jaisi feed hai, to ek baar bana ke Redis me rakh do, har user DB nahi jaata. Nayi khabar
aate hi list me apni jagah le leti, user ko turant dikhti.

Har user pe DB se `ORDER BY ... LIMIT 20` kyun nahi? Yahi pehle tha. Har user pe DB, aur DB sort karte-karte dheema.
Ab ~99% DB tak jaate hi nahi.

**Pooche: breaking news kitni der me dikhegi?** Aate hi ZADD, to turant. "5 min" sirf sabse bura haal hai.

**Pooche: neeche scroll kiya (21-40)?** `ZREVRANGE 20 39`, ya cursor: aakhri dekhi khabar ke time se pehle ki 20.

**Pooche: Redis restart hua, list khaali?** DB se ek baar bana lo, lock ke saath: ek hi banaye, baaki ruk ke Redis se padhein.

**Pooche: Redis me list kitni lambi?** Top 500. RAM chhoti rehti. Usse purani chahiye to DB.

---

*ab padhna tez ho gaya, par khabar hi der se aa rahi*

## 2. Meri feed me khabar 20 minute purani

Pehle ek hi fetcher khabar laata bhi tha aur DB me likhta bhi tha, 1000 source ek-ek karke. Source A dheema hokar
atka, to B se Z tak line me khade. Khabar 20 min purani.

Isliye fetcher sirf laata hai aur Kafka me daal deta (key = source_id). Worker apni speed se Kafka se padh ke DB +
Redis me likhta. Kai fetcher, Kafka me 10 partition, phir worker.

Ye kyun chalta? Laana aur padhna alag ho gaye. Laana peeche apni speed se chalta, user sirf Redis ki list padhta.
Laane me der ya bheed ho tab bhi feed khulti, bas thodi purani. Ek source atke, baaki chalte rehte.

Fetcher hi DB me likh de to? Wahi pehle wala haal. Ek atka to sab peeche khade.

SQS kyun nahi, Kafka kyun? SQS bhi chal jaata. Par Kafka me search / analytics apne group se wahi khabar padh lete,
aur dobara bhi padh sakte.

**Pooche: source pe kuch naya nahi hai to?** ETag / Last-Modified dekho. "Naya nahi" bola to download hi nahi.

**Pooche: 1000 khabar ek saath aa gayi?** Line lambi hoti, gira kuch nahi. Kafka disk pe rakhta, worker apni speed se padhta.

**Pooche: worker ne likha, Kafka commit se pehle crash, naya worker ne dobara likha to?** Bloom se rokna kaam nahi
aata, kyunki Bloom DB se alag cheez hai aur Bloom update se pehle hi crash ho sakta. Khabar feed me do baar aa jaati.
Isliye DB khud roke: url pe UNIQUE, `ON CONFLICT DO NOTHING`. Dobara likha to bhi ek hi.

**Pooche: har source kitni baar dekhoge?** Apna-apna samay. NDTV har 1 min, chhota blog har 15 min.

---

*ab khabar taazi, par 8 baje sab ek saath aa gaye*

## 3. 8 baje election result, 40 guna bheed

500 se seedha 20,000 har second. Ek Feed machine ka CPU bhar gaya, aur Redis miss seedha main DB pe gaya, jahan
fetcher bhi likh raha tha. Feed aur DB dono dheeme.

Isliye Feed Service kai machine pe, aage LB. Redis miss hua to DB ki copy (read replica) se padho. Main DB sirf likhai.

Ye kyun chalta? Padhai copy pe, likhai main pe. Feed Service kuch yaad nahi rakhti, to uski machine badhao, koi bhi
request le. Copy thodi peeche ho to user ko kuch second purani khabar, nuksaan nahi.

Miss main DB pe hi kyun nahi? Wahan fetcher bhi likh raha hai. Bheed ki padhai likhai ko bhi dheema kar deti.

**Pooche: 8 baje ka pata tha to?** 7:45 pe pehle se machine badhao (scheduled scaling) aur cache pehle bharo.

**Pooche: 20,000 har second aur badhe?** Aur machine. Feed Service kuch yaad nahi rakhti, koi bhi le.

**Pooche: copy peeche reh gayi?** Kuch second. 5 sec purani khabar se nuksaan nahi.

**Pooche: ek hi key pe 20,000 har second?** Redis ki replica, aur App me 1-2 sec ki copy. Ek machine pe sab nahi.

---

*ab bheed sambhal li, par feed me wahi khabar baar-baar*

## 4. Dopahar "India ne match jeeta", meri feed me 5 baar

Pehle har naya URL nayi khabar maana. 5 source, alag URL, thoda alag title, to meri feed me 5 baar.

Isliye dekha hua URL Bloom filter se pakadte hain, aur wahi khabar title ke hash se (Bloom + SimHash). Title chhote
akshar me, chinh hatao, phir hash. Same hash = same khabar, URL chahe alag ho.

Ye kyun chalta? Ek khabar ek baar. Pehchaan (hash) same to khabar same. Bloom me URL ki jagah pe koi bit 0 hai to
pakka naya, sab 1 hain to shayad dekha. Kabhi ek nayi khabar chhoot jaaye to news me chalta, par user ko ek khabar
5 baar nahi.

Redis SET kyun nahi, Bloom kyun? RAM bachao. Crore URL ka set = bahut RAM. Bloom ka galat "haan" kabhi ek nayi khabar
chhuda sakta, news me chalta.

Har worker apna Bloom kyun nahi? Ek ne dekha, doosre ko pata hi nahi. Isliye ek saanjha Bloom (Redis me).

**Pooche: "wins" vs "won", hash hi alag pad gaya?** SimHash: 90% same = duplicate.

**Pooche: 5 me se kaunsa dikhaoge?** Jo pehle aaya ya bada source. Baaki "aur sources" me link.

**Pooche: Bloom se URL hatana ho?** Normal Bloom me nahi hota. Roz naya filter, purana phenko (ya counting Bloom).

---

*ab khabar saaf, par ek source hi atak gaya*

## 5. Dopahar baad nayi khabar aani ruki: ek chhota source 30 sec atka, baaki 999 bhi ruke

Fetcher source X ke jawab ka intezaar karta raha, 30 sec chup. Baaki 999 source line me atke, feed ruk gayi.

Isliye thodi der me chhodo (timeout, 2-3 sec, thodi retry), aur baar-baar fail ho to call hi mat karo (circuit
breaker). Breaker OPEN = us source ko call hi nahi, baaki apne raaste.

Ye kyun chalta? Ek beemar source baaki ko nahi rokta. Had tak intezaar, phir breaker. Us source ki khabar kuch der
nahi aati, baaki feed chalti rehti.

Poora jawab aane tak ruke rehte to? Wahi 30 sec. Ek atka, 999 peeche.

**Pooche: breaker kab wapas chalu?** Thodi der baad HALF-OPEN: ek test call. Theek mila to CLOSED, wapas chalu.

**Pooche: source ne 429 diya?** Dheere chalo. Uska samay badhao aur Retry-After maano.

**Pooche: kuch din se OPEN hi hai?** Alert, aur list se bahar.

---

*ab sab khabar aa rahi, par mujhe sab nahi chahiye*

## 6. Shaam ko mujhe sirf tech aur sports chahiye

Isliye har category ki ek list pehle se (`feed:tech`, `feed:sports`). Khabar aayi to apni category list me. Maine
khola to tech + sports ke top 20 time se mila diye (category feed).

Ye kyun chalta? List category ki pehle se bani, milana padhte waqt. User ki pasand = kaunsi list milaani. Sirf ~20
list, 10 lakh nahi.

Har user ki alag feed kyun nahi? 10 lakh user = 10 lakh feed, har nayi khabar sab me daalni. Bahut mehnga.

**Pooche: ek khabar 2 category me?** Dono list me ZADD. Milate waqt id se dedupe, do baar na dikhe.

**Pooche: mujhe tech zyada pasand hai?** Wo asli personalization hai, har user ki ranking. Scope se bahar.

---

*ab feed ban gayi, par dhoondhna dheema*

## 7. Raat ko maine "cricket" dhoondha, poori table padhi gayi

DB 2 crore row ek-ek padh ke match kar raha tha. Bahut dheema.

Isliye har shabd ki list: wo kis-kis khabar me hai (inverted index, Elasticsearch). "cricket" -> [a12, a45, a90].
ES sirf 3 id deta, khabar ka text DB / cache se.

Ye kyun chalta? ES sirf dhoondhta (id deta), sach DB me. ES gira ya galat nikla to DB se dobara bana lo. Khabar kabhi
nahi khoti, bas us beech search dheemi ya band.

`LIKE '%cricket%'` kyun nahi? % aage laga to index kaam nahi karta, poori table padhi jaati.

Postgres ka search nahi chalega? Chhote scale pe haan (full-text, GIN). ES chahiye ranking, galat spelling, bahut
data ke liye, aur DB pe bojh nahi padta.

**Pooche: "cricket kohli" dhoondha?** Dono list milao: [a12, a45, a90] aur [a45, a77]. AND = a45, OR = sab. Phir score se sort.

**Pooche: "crikcet" likh diya?** Fuzzy (ES): 1-2 akshar ka farak chal jaata.

**Pooche: ES aur DB alag ho gaye?** DB sach hai, ES dobara bana lo (reindex). Likhna fail hua to retry ki line.

---

*ab sab chal raha, par data bhar raha*

## 8. Raat ko DBA ka mail: 6 mahine me ~2 crore row, kharcha badhta ja raha

Sab ek table me tha, May se Oct tak. Purana koi padhta hi nahi, par disk + backup ka kharcha badhta ja raha.

Isliye har mahine ka alag tukda (partition), aur purana S3 pe (archive). Postgres me sirf haal ke mahine.

Ye kyun chalta? Naya mahina garam (Postgres, index se tez), purane thande (S3, sasta). Purani khabar khulti hai, bas
dheemi. Purana hatana = retention hai, sharding nahi.

Sab ek hi table me rehne do? Wahi faltu kharcha. Jo koi padhta nahi uski disk aur backup.

**Pooche: mahina beet gaya to?** DETACH karke S3. Bilkul nahi chahiye to DROP. Pal ka kaam.

**Pooche: DETACH pe lock lagta?** Normal DETACH pe poore table pe pal bhar ka lock, padhai bhi rukti. PG 14+ me
`DETACH ... CONCURRENTLY`, kuch nahi rukta.

**Pooche: 1 saal purani khabar khuli?** S3 archive se. Dheemi, par koi kabhi-kabhi hi kholta.

**Pooche: 3 saal baad DB kitna?** Utna hi. Naya mahina garam, purana thanda (S3).

---

*ab jagah theek, par source hi 100 guna ho gaye*

## 9. Mail ka doosra hissa: source 100 guna hue to ek DB machine nahi jhelegi

Saari likhai ek machine pe, disk + likhai full.

Isliye article_id se tukde, har tukda alag machine (shard). `hash(a91)` -> shard 2. Likhai sab tukdon pe barabar.

Ye kyun chalta? Baantne ki key wo jo bojh barabar baante, yaani article_id ka hash. Ek shard garam ya mara to sirf
uska hissa dheema, baaki chalta.

Category se shard kyun nahi? Bojh tirchha (skew). Sports / politics bade, baaki chhote.

**Pooche: taazi 20 kahan se?** Redis se. Miss hua to har tukde se thoda (scatter-gather).

**Pooche: shard badhaya to?** Thoda hilta (consistent hashing). Sirf naye shard ke hisse ka data.

**Pooche: do shard pe wahi khabar?** Dedupe pehle saanjhe Bloom / Redis se. DB ka UNIQUE sirf shard ke andar chalta.

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

---

ARCHETYPE A+E · CONCEPTS: [elasticsearch](../../FOUNDATIONS/12_elasticsearch_search.md) · [caching](../../FOUNDATIONS/04_caching.md) · saath: [twitter-feed](../03_twitter_feed/03_twitter_feed.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
