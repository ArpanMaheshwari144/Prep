# Twitter Feed

> App kholo to home timeline: jinhe tum follow karte ho unke tweet, sabse naye upar. Tweet daalna (POST) bhi. Misaal: Arpan ne app khola, Virat "Match great", Sachin "Watching IPL", Dhoni "Practice".
> Dil do cheezein: feed padhna sasta ho (log 50 baar padhte, 1 baar likhte, isliye feed pehle se bana ke rakho), aur celebrity ka tweet (10 crore follower).
>
> Mock me sirf heading + Cue + poster. Poster me sab: upar "Pehle aisa tha", tasveer (ilaaj), hara "Kyun chalta", laal number = kya toot sakta + kaise bacha, peela = doosra tareeka kyun nahi, grey = baaki.
> Cross-Q: hal ke har bade shabd pe socho -> gira? · dheema / bheed? · do ek saath? · galat hua? · kaise / kitna?

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c10_2-9c25b583.svg)

---

## Shuru

```
FR      home timeline aur tweet post karna, dhyaan inhi do pe. Photo / video bhi aate hain.
        Bahar rakha: search, trending, DM, notification
NFR     feed 200 ms se tez khulni chahiye. Feed 2 sec purani bhi chalegi, isliye pehle se bana sakte hain.
        Celebrity (10 crore follower) ko sambhalna hai, design yahin mudta hai
NUMBERS ~50 crore user. 50 crore tweet / din, din me ~1 lakh sec, to ~5,000 tweet / sec.
        Ek aadmi ~200 ko follow karta hai, celeb ke 10 crore follower. Padhna : likhna ~50 : 1
PEHLE   sabse simple: do table, tweets (kisne kya likha) aur follows (kaun kisko follow karta hai).
        Feed = follow walon ke tweet nikaalo, time se sort, upar ke 50. Har baar 200 logon ka jod, dheema
NAAM    Redis inbox = har user ki taiyaar feed. Fanout service = naya tweet followers ki feed me daalti hai
```

> [!IMPORTANT]
> **RAIL: har design isi kram me chalta (8 dabbe, sab file me same)**
> kaam chala → bheed aayi → machine giri → do log ek saath → kuch dheema → data bada → door ke log → bahar ka hamla
>
> | rail | is design me | card |
> |---|---|---|
> | 1 kaam chala | feed khulne pe har baar 200 logon ka jod (**is design ka khaas: push inbox / celeb pull**) | 1 |
> | 2 bheed aayi | Virat ka tweet, crore feed ek saath · highlights ki bheed me tweet dheema | 4, 9 |
> | 3 machine giri | India region me ek hi App, wo giri | 8 |
> | 4 do log ek saath | alag nahi aata: worker dobara chala to bhi sorted set me tweet ek hi baar | 2 |
> | 5 kuch dheema | tweet button 5 sec ghooma · Bieber ka tweet, line ghanton atki (**khaas**) | 2, 3 |
> | 6 data bada | 50 crore inbox se RAM bhari · 5 saal me 0.5 PB tweet | 5, 6 |
> | 7 door ke log | Bangalore se US 300 ms · video door ke server se | 7, 10 |
> | 8 bahar ka hamla | is file me nahi: rate limit + WAF = 02_rate_limiter | - |


> [!NOTE]
> **AWS pe kaunsa DB (interview me naam bolna ho to)** · SQL wala = RDS, Cassandra RDS pe nahi chalta
>
> - Tweets (Cassandra) → **Amazon Keyspaces** (ya EC2 pe khud): likhai bahut, user_id se baanto, join / transaction chahiye nahi
> - Inbox / feed (Redis sorted set) → **ElastiCache (Redis)**: feed pehle se bani, padhna tez

---

*ab tootega kahan? sabse pehle padhne pe, kyunki padhna 50 guna*

## 1. Subah app khola, feed ghoomti rahi (har baar 200 logon ka jod)

**Rail 1 KAAM CHALA (khaas)** · Cue: PUSH: tweet hote hi INBOX me id

<img src="img/c1_poster.png?v=3" width="1000" alt="card 1: kya toota, kaise bacha">


---

*ab padhna tez, par inbox bharne ka kaam likhne pe aa gaya*

## 2. Maine tweet kiya, button 5 sec ghoomta raha

**Rail 5 KUCH DHEEMA** · Cue: KAFKA, fanout peeche

<img src="img/c2_poster.png?v=3" width="1000" alt="card 2: kya toota, kaise bacha">


---

*ab aam tweet theek, par celeb ka ek tweet?*

## 3. Bieber ka ek tweet = 10 crore inbox, line ghanton atki

**Rail 5 KUCH DHEEMA (khaas)** · Cue: HYBRID: aam push, celeb pull

<img src="img/c3_poster.png?v=3" width="1000" alt="card 3: kya toota, kaise bacha">


---

*ab ek tweet pe crore log ek saath*

## 4. Match khatam, Virat ka tweet, crore log ek saath, DB gira

**Rail 2 BHEED AAYI** · Cue: HOT-TWEET CACHE (RAM)

<img src="img/c4_poster.png?v=3" width="1000" alt="card 4: kya toota, kaise bacha">


---

*ab Redis hi bhar raha*

## 5. Shaam ko Redis alert: 50 crore inbox, RAM bhar rahi

**Rail 6 DATA BADA (RAM)** · Cue: AAKHRI 800 + soye hue hatao

<img src="img/c5_poster.png?v=3" width="1000" alt="card 5: kya toota, kaise bacha">


---

*ab tweet itne ho gaye*

## 6. DB alert: har second 5,000 tweet, 5 saal me 0.5 PB

**Rail 6 DATA BADA (tweet)** · Cue: CASSANDRA, user_id se baanto

<img src="img/c6_poster.png?v=3" width="1000" alt="card 6: kya toota, kaise bacha">


---

*ab data ek desh me, user doosre me*

## 7. Arpan Bangalore me, data US me, har feed 300 ms

**Rail 7 DOOR KE LOG** · Cue: REGION-WISE + Route 53

<img src="img/c7_poster.png?v=3" width="1000" alt="card 7: kya toota, kaise bacha">


---

*ab har region me App hi ek hai*

## 8. Raat ko har region me ek hi App machine; India wali giri, India band

**Rail 3 MACHINE GIRI** · Cue: STATELESS App x N + ALB

<img src="img/c8_poster.png?v=3" width="1000" alt="card 8: kya toota, kaise bacha">


---

*ab padhne ki bheed likhne ko daba rahi*

## 9. Highlights ki bheed ne mera tweet daalna dheema kar diya

**Rail 2 BHEED AAYI (padhna)** · Cue: ALAG SERVICE (likhna/padhna)

<img src="img/c9_poster.png?v=3" width="1000" alt="card 9: kya toota, kaise bacha">


---

*ab bhaari maal: photo, video*

## 10. Maine match ki video daali, sabko dheemi aur mehngi

**Rail 7 DOOR KE LOG (media)** · Cue: S3 + CDN

<img src="img/c10_poster.png?v=3" width="1000" alt="card 10: kya toota, kaise bacha">


---

## Poora design ek saans me

```
LIKHNA: Route 53 -> ALB -> Tweet Svc -> Cassandra me save -> Kafka -> Fanout workers
        -> aam followers ke Redis inbox me sirf tweet number (aakhri 800 rakho), celeb skip
PADHNA: Timeline Svc -> Redis inbox (push) + celebs ke tweet celeb cache se (pull)
        -> milao + sort -> number se poora tweet laao -> upar ke 50
MEDIA:  photo / video S3 me, user tak CDN se
FOLLOW: User Svc + Follow store
```

Bolne ke liye: likhna, tweet Cassandra me save, aur "naya tweet" Kafka me. Worker aam followers ke Redis inbox me tweet
id daalte. Padhna, Arpan ka inbox lo, famous logon ke naye tweet alag cache se lo, dono milao, time se sort, top 50
bhejo. Famous ke liye "padhte waqt laao", baaki ke liye "pehle se daal do". Cassandra user id + time se baanta, doosre
desh me copy. Aage: ML se ranking, trending, search.

---

## 10 guna traffic

- **Timeline Svc**: har baar 200 logon ka jodna nahi, inbox pehle se bana (ho gaya). Machine badhao.
- **Redis inbox**: 50 crore inbox, to aakhri 800 + soye hue hatao. Cluster + copy.
- **Celeb ke read**: celeb cache 95%. Garam key ki kai copy + App ka chhota cache.
- **Cassandra**: user_id (+ time) se baanta. Bieber ki machine garam, to uska data aur copy.
- **Doosre desh**: region + garam data har region me copy.
- **Kafka**: partition badhao, fanout workers badhao.
- **IPL final**: 10 guna, likhai Kafka line me rukti. Pata ho to machine + cache pehle garam.
- **Asli Twitter**: sab ek saath. user_id se baanta + time ke tukde + region copy + garam data sab jagah cache.

**Pooche: kahan se gir sakta?** Har dabbe pe "ye gira to?"

**Pooche: chal raha hai, kaise pata?** Feed ka time (p99), galtiyan, Kafka me kitna pending (fanout peeche?), alert.

---

## Pooche to (kisi card me nahi baitha)

API: POST /tweet { content } -> tweetId. GET /feed?limit=50 -> home timeline. GET /user/{id}/tweets.

Tweet DB: Cassandra (bahut zyada, seedha kaam, likhna bahut, user_id + time se baanta).

Inbox: har user ka Redis sorted set (score = time). Naya jodo (ZADD, same id dobara = ek hi). Padho (ZREVRANGE 0 49).
800 ke baad kaato (ZREMRANGEBYRANK). Sirf number. Poora tweet daala to ek tweet 200 jagah copy, RAM phategi.

Follow store: ek table / Cassandra (followers / following). Neo4j zaroori nahi.

Follow store ke liye Graph DB (Neo4j) chahiye? Nahi. Follow list ek table / Cassandra me. Neo4j tab, jab "dost ke dost" chahiye.

Tweet row: tweet_id, user_id, content, time.

---

ARCHETYPE A (read-heavy/feed) · CONCEPTS: [caching](../../FOUNDATIONS/04_caching.md) · [sharding](../../FOUNDATIONS/06_database_sharding.md) · [replication](../../FOUNDATIONS/05_database_replication.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
