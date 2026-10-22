# Twitter Feed

> App kholo to home timeline: jinhe tum follow karte ho unke tweet, sabse naye upar. Tweet daalna (POST) bhi. Misaal: Arpan ne app khola, Virat "Match great", Sachin "Watching IPL", Dhoni "Practice".
> Dil do cheezein: feed padhna sasta ho (log 50 baar padhte, 1 baar likhte, isliye feed pehle se bana ke rakho), aur celebrity ka tweet (10 crore follower).
>
> Mock me sirf heading + Cue + kahani ki tasveer. Neeche har card: HAL · KYUN · NAHI, phir YAAD RAKH dabba + CROSS-Q dabba (sawaal → cue, poora jawab).
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

<img src="img/c1_poster.png?v=1" width="1000" alt="card 1: kya toota, kaise bacha">


```
HAL     tweet hote hi follower ke inbox me uski id chali jaati hai (fanout).
        App kholo to bas apna inbox padho, ek hi read
        Ravi ne t9 tweet kiya, wo turant mere inbox me aa gaya. Inbox = tweet_id ki list, pehle se bani hui
KYUN    padhna 50 guna hai, to mehnat likhte waqt karo, har baar khulne pe jodna mat.
        2 sec purani feed chalegi, isliye pehle se banana aur cache chal jaata hai.
        Jo inbox me nahi hai (poora tweet, unfollow), wo padhte waqt jodo ya chhaanto
NAHI    DB pe index  -> WAHI: thoda tez hota, par har khulne pe 200 ka jod wahi ka wahi
        read replica -> WAHI: thoda tez hota, par har khulne pe 200 ka jod wahi ka wahi
```

> [!IMPORTANT]
> **Yaad rakh:** Tweet hote hi follower ke inbox me sirf id (fanout). Feed = apna inbox, ek read, likhte waqt mehnat.


---

*ab padhna tez, par inbox bharne ka kaam likhne pe aa gaya*

## 2. Maine tweet kiya, button 5 sec ghoomta raha

**Rail 5 KUCH DHEEMA** · Cue: KAFKA, fanout peeche

<img src="img/c2_poster.png?v=1" width="1000" alt="card 2: kya toota, kaise bacha">


```
HAL     tweet DB me save hota hai aur khabar Kafka me jaati hai, mujhe turant "ho gaya" dikhta hai.
        Workers peeche Kafka se padh ke 5,000 inbox bharte hain
NAAM    Kafka = tweet service se fanout tak khabar le jaata hai
KYUN    pehle usi request me 5,000 inbox ek-ek karke bharte the: 5+ sec, timeout, "fail" dikha,
        par tweet ban chuka tha. Ab "ho gaya" tab, jab tweet DB me save. Inbox bharna Kafka se peeche.
        Dobara chale to bhi tweet ek hi baar dikhta hai (sorted set). Apna tweet apni feed me turant
NAHI    App me peeche ek thread -> KHOYA: App crash hua to thread ka kaam gaya.
           Kafka disk pe hai, wahan se dobara padh sakte
```

> [!IMPORTANT]
> **Yaad rakh:** Tweet DB me save hote hi "ho gaya". Khabar Kafka me, workers peeche inbox bharte. Sorted set me dohra nahi.


---

*ab aam tweet theek, par celeb ka ek tweet?*

## 3. Bieber ka ek tweet = 10 crore inbox, line ghanton atki

**Rail 5 KUCH DHEEMA (khaas)** · Cue: HYBRID: aam push, celeb pull

<img src="img/c3_poster.png?v=1" width="1000" alt="card 3: kya toota, kaise bacha">


```
HAL     aam user ka tweet push hota hai (inbox me), celeb ka pull. Bieber ka tweet celeb cache me rehta hai,
        feed khule tab dono milate hain
KYUN    likhai ka bojh follower ki ginti se aata hai. Celeb ka ek tweet 10 crore jagah likhna, yahi dikkat.
        Push kiya to Kafka line me 10 crore kaam, mera tweet peeche, ghanton der.
        Feed hamesha dono ka mel hai: mera inbox + follow kiye celebs ke naye tweet, time se milao.
        User ko farak nahi dikhta, aur line khaali rehti
NAHI    sabke liye pull -> MEHNGA: har feed khulne pe phir 200 ka jod
```

> [!IMPORTANT]
> **Yaad rakh:** Aam user push, celeb pull (celeb cache). Feed = mera inbox + celebs ke naye tweet, time se milao.


---

*ab ek tweet pe crore log ek saath*

## 4. Match khatam, Virat ka tweet, crore log ek saath, DB gira

**Rail 2 BHEED AAYI** · Cue: HOT-TWEET CACHE (RAM)

<img src="img/c4_poster.png?v=1" width="1000" alt="card 4: kya toota, kaise bacha">


```
HAL     garam tweet RAM me rakho (hot-tweet cache). Miss hua to sirf ek request DB jaati hai (lock),
        baaki sab usi ka jawab lete hain
KYUN    pehle har feed Virat ka tweet DB se la rahi thi. Ab garam cheez RAM me hai, DB tak sirf ek request,
        DB aaram se rehta hai
NAHI    DB ki copies badha do -> DHEEMA: wo bhi disk se padhti hain (ms).
           Ek garam hisse ki copies crore read nahi jhel paati
```

> [!IMPORTANT]
> **Yaad rakh:** Garam tweet RAM me (hot-tweet cache). Miss pe sirf ek request DB (lock), baaki uska jawab.


---

*ab Redis hi bhar raha*

## 5. Shaam ko Redis alert: 50 crore inbox, RAM bhar rahi

**Rail 6 DATA BADA (RAM)** · Cue: AAKHRI 800 + soye hue hatao

<img src="img/c5_poster.png?v=1" width="1000" alt="card 5: kya toota, kaise bacha">


```
HAL     naya tweet upar judta hai, 800 se neeche wale kaat do (trim). Soye hue user ka inbox 30 din me
        mit jaata hai (TTL)
KYUN    pehle har inbox me saare id the, soye users ke bhi. Ab RAM me sirf id, aur sirf zinda logon ki.
        Jo 1 mahine se nahi aaya, uska inbox hi nahi. Jo RAM me nahi, wo DB se bana lo:
        ek baar dheema hoga, par galat nahi
NAHI    Redis me machine jodte jao -> MEHNGA: soye 40 crore ka inbox bhi RAM khaata.
           Kharcha badhta hai, dikkat wahi rehti
```

> [!IMPORTANT]
> **Yaad rakh:** Inbox me sirf aakhri 800 id (trim), soye user ka inbox 30 din me mitao (TTL). Wapas aaye to pull se banao.


---

*ab tweet itne ho gaye*

## 6. DB alert: har second 5,000 tweet, 5 saal me 0.5 PB

**Rail 6 DATA BADA (tweet)** · Cue: CASSANDRA, user_id se baanto

<img src="img/c6_poster.png?v=1" width="1000" alt="card 6: kya toota, kaise bacha">


```
HAL     Cassandra lagao, user_id se baanto. Mere saare tweet ek machine pe, naya sabse upar. Ek hi read
NAAM    Tweet store (Cassandra) = saare tweet yahan rehte hain
KYUN    ek DB na itna rakh sakta, na itna likh sakta. Jis cheez se padhte ho (user_id), usi se baanto.
        Likhna sirf aakhir me jodna hai, isliye tez. 3 copy, 2 me likho 2 se padho, naya mil hi jaata hai
NAHI    MySQL -> DHEEMA: har tweet pe har index disk pe dhoondh ke badalna. Join / transaction chahiye hi nahi
        tweet_id se baanto -> BIKHRA: bojh barabar, par profile ke liye saari machine poochni padti
```

> [!IMPORTANT]
> **Yaad rakh:** Cassandra, user_id se baanto: mere saare tweet ek machine pe, ek read. 3 copy, 2 likho 2 padho.


---

*ab data ek desh me, user doosre me*

## 7. Main Bangalore me, data US me, har feed 300 ms

**Rail 7 DOOR KE LOG** · Cue: REGION-WISE + Route 53

<img src="img/c7_poster.png?v=1" width="1000" alt="card 7: kya toota, kaise bacha">


```
HAL     har region me data ki copy rakho (multi-region). Route 53 paas wale region (India) ka pata deta hai
KYUN    data user ke paas wale region me hai, to feed India region se aati. Copy US se aati rehti hai.
        Doosre region ki copy thodi peeche, to region gira to feed thodi purani, par chalti rehti
NAHI    sab US se hi do -> DHEEMA: har request samundar paar, 300 ms aana-jaana, har feed atak ke aati
```

> [!IMPORTANT]
> **Yaad rakh:** Har region me data ki copy, Route 53 paas wala region deta. Region gira to doosra, feed thodi purani.


---

*ab har region me App hi ek hai*

## 8. Raat ko har region me ek hi App machine; India wali giri, India band

**Rail 3 MACHINE GIRI** · Cue: STATELESS App x N + ALB

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c8_1.png?v=5" width="900">

<img src="img/c8_2.png?v=5" width="900">

<img src="img/c8_3.png?v=5" width="900">

</details>

```
HAL     kai App hain, aage ALB baant-ta hai. App kuch yaad nahi rakhta (stateless x N).
        App 2 giri to wo bahar, baaki chalu rehte hain
KYUN    machine kuch yaad nahi rakhti, sab Redis / DB me. Koi bhi gire, jude, badle, user ko farak nahi
NAHI    ek hi App -> GIRA: wo giri to poora region band
```

> [!IMPORTANT]
> **Yaad rakh:** Kai App (stateless), aage ALB. Giri App bahar, naya code ek-ek karke (rolling).

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - App 2 giri, pata kaise? → **ALB har ~10 sec poochta "zinda ho?"**: Chup raha to bahar, theek hua to wapas (Route 53 bhi)
> - naya code daalna hai, App band karein? → **nahi, ek-ek karke badlo (rolling)**: (rolling). Baaki App bheed sambhaal leti hain
> - nayi App judi, turant poori bheed? → **nahi, ALB dheere-dheere deta**: pehli request dheemi hoti hai, uske paas apna data nahi


---

*ab padhne ki bheed likhne ko daba rahi*

## 9. Highlights ki bheed ne mera tweet daalna dheema kar diya

**Rail 2 BHEED AAYI (padhna)** · Cue: ALAG SERVICE (likhna/padhna)

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c9_1.png?v=5" width="900">

<img src="img/c9_2.png?v=5" width="900">

<img src="img/c9_3.png?v=5" width="900">

</details>

```
HAL     likhne aur padhne ki alag service. ALB path se baant-ta hai. Jahan bheed hai, wahi badhao
PATH    POST /tweet -> Tweet Svc (x 2), GET /feed -> Timeline Svc (x 20)
NAAM    Tweet service = tweet likhti. Timeline service = feed padhti. Follow store = kaun kise follow karta
KYUN    pehle padhna aur likhna ek service me tha. Crore log scroll kar rahe the,
        mera tweet usi bheed me dheema ho gaya.
        Ab jiski bheed badhi sirf wahi badhao, ek ka dheemapan doosre ko nahi rokta
NAHI    ek hi service -> DHEEMA: padhne ki bheed me likhna bhi dheema hota hai, wahi hua
```

> [!IMPORTANT]
> **Yaad rakh:** Likhne aur padhne ki alag service, ALB path se baante. Jiski bheed, wahi badhao.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - feed ki bheed aur badh gayi? → **sirf Timeline Svc badhao, Tweet Svc ko chhuo mat**: Tweet Svc pe koi asar nahi
> - Suresh ke 5,000 follower? → **Follow store se 1000-1000 padh ke likho**: inbox likho. Ek baar me sab padhna bhaari hai
> - Timeline ko follow list chahiye, wo kahan se laayega? → **Redis me rakhi follow list**: User / Follow store se, par wo Redis me rakhi hoti hai, kyunki har feed pe chahiye


---

*ab bhaari maal: photo, video*

## 10. Maine match ki video daali, sabko dheemi aur mehngi

**Rail 7 DOOR KE LOG (media)** · Cue: S3 + CDN

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c10_1.png?v=5" width="900">

<img src="img/c10_2.png?v=5" width="900">

<img src="img/c10_3.png?v=5" width="900">

</details>

```
HAL     video S3 me rakho (seedha upload, presigned link), dekhne wale ko paas wale CDN se do
KYUN    pehle 50 MB ki video Delhi, London, Sydney, har dekhne wala hamare US server se khinchta:
        buffering aur bandwidth ka bill. Ab bhaari maal S3 me, user tak CDN se: paas bhi, sasta bhi.
        Upload me App beech me nahi aata. Tweet me sirf CDN ka link, link badla = nayi file.
        Link pe hastakshar = sirf jisko mila wahi dekhe
NAHI    API bhi CDN se -> GALAT: CDN sirf media ke raaste pe hai
```

> [!IMPORTANT]
> **Yaad rakh:** Video S3 me (seedha upload, presigned), dekhne wale ko paas ke CDN se. Private = signed URL.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - CDN ke paas video nahi, koi dekhne aaya? → **CDN S3 se laake rakh leta**: kuch der apne paas rakh leta hai
> - photo hata di, CDN pe abhi dikh rahi? → **CDN ko "hatao" bolo**: (invalidation), ya link me version daalo, nayi file
> - private photo hai, link kisi aur ko mil gaya? → **link pe hastakshar + expiry**: (signed URL). Sirf jisko mila, thodi der ke liye


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
