# Twitter Feed

> App kholo to home timeline: jinhe tum follow karte ho unke tweet, sabse naye upar. Tweet daalna (POST) bhi. Misaal: Arpan ne app khola, Virat "Match great", Sachin "Watching IPL", Dhoni "Practice".
> Dil do cheezein: feed padhna sasta ho (log 50 baar padhte, 1 baar likhte, isliye feed pehle se bana ke rakho), aur celebrity ka tweet (10 crore follower).
>
> Har card: kya toota -> kya kiya -> yahi kyun -> doosra kyun nahi. Kram me chalo. "Pooche" wali line = interviewer aage poochhe to.

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c10_2-9c25b583.svg)

---

## Shuru

Sabse simple: do table. tweets (kisne kya likha) aur follows (kaun kisko follow karta). Feed = jinko follow karta unke
tweet nikaalo, time se sort karo, upar ke 50 do.

Dabbon ke naam poori file me yahi: Redis inbox = har user ki taiyaar feed. Fanout service = naya tweet followers ki feed
me daale. Tweet store (Cassandra) = saare tweet. Tweet service = tweet likhe. Timeline service = feed padhe. Follow
store = kaun kise follow karta. Kafka = tweet service se fanout tak khabar.

Pehle poocho: timeline, post, search, trending, DM, notification, kis pe dhyaan? Main home timeline + post pe chalta
hoon. ~50 crore user maan lun? Celebrity (10 crore follower) sambhalna hai? Design yahin mudta hai. Feed bilkul usi
second ki chahiye, ya 2 second purani chalegi? Chalegi to pehle se banana possible. Photo / video bhi, ya sirf text?
Bahar: search, trending, DM, notification.

Numbers: 50 crore tweet / din, din me ~1 lakh second, to ~5,000 tweet har second. Ek aadmi ~200 ko follow karta.
Celeb ke 10 crore follower. Padhna : likhna ~50 : 1. Feed 200 ms se tez chahiye.

Number se faisla: 50 : 1 hai, to har baar feed khulne pe jodna mat, PEHLE se bana ke rakho. Celeb ka ek tweet 10 crore
jagah likhna, yahi asli dikkat. 2 sec purani feed chalegi, to pehle se banana + cache chal jaayega.

**Pooche: consistency ya availability, kya chunoge?** Feed ke liye: chalta rahe, chahe thoda purana ho. Arpan ko 2
second purani feed dikhi to koi farak nahi. Par feed khuli hi nahi to wo app band kar dega. Isliye network toote to
bhi jo hai wo dikhao, sahi wala thodi der me aa jaayega.

---

*ab tootega kahan? sabse pehle padhne pe, kyunki padhna 50 guna*

## 1. Subah app khola, feed ghoomti rahi (har baar 200 logon ka jod)

Kholte waqt 200 logon ke tweet laao, jodo, sort karo. Har baar dheema.

Isliye tweet hote hi follower ke inbox me uski id daal dete hain (fanout). Ravi ne tweet t9 kiya, turant mere inbox me.
App khola to bas apna inbox padho (tweet_id list), bana hua, jodna nahi. Ek read.

Ye kyun chalta? Padhna 50 guna zyada hai, to mehnat likhte waqt karo. App khole to sirf inbox padho. Jo inbox me nahi
(poora tweet, unfollow), wo padhte waqt jodo / chhaanto.

DB pe index ya read replica laga do? Thoda tez, par har khulne pe 200 ka jod wahi ka wahi.

**Pooche: id se poora tweet kaise?** 50 id ek saath cache / store se (batch). Ek-ek laaye to 50 chakkar.

**Pooche: kisi ko unfollow kiya?** Padhte waqt follow list se chhaanto. Poora inbox badalna mehnga, safai peeche.

**Pooche: maine abhi kisi ko follow kiya?** Uske aakhri 20 tweet id mere inbox me daal do, ya padhte waqt uske
tweet milao. Warna naya follow feed me tab dikhe jab wo agla tweet kare.

---

*ab padhna tez, par inbox bharne ka kaam likhne pe aa gaya*

## 2. Maine tweet kiya, button 5 sec ghoomta raha

Mere 5,000 follower. Usi request me 5,000 inbox ek-ek karke bharo. 5+ sec, timeout, mujhe "fail" dikha, par tweet ban
chuka tha.

Isliye tweet DB me save aur khabar Kafka me, user ko turant "ho gaya". Workers peeche Kafka se padh ke 5,000 inbox
bharte.

Ye kyun chalta? User ko "ho gaya" tab jab tweet DB me save. Inbox bharna Kafka se peeche. Dobara chale to bhi tweet ek
hi baar dikhe (sorted set). Apna tweet apni feed me turant.

App me peeche ek thread chala do? App crash to thread ka kaam gaya. Kafka disk pe hai, dobara padh sakte.

**Pooche: DB me save hua, Kafka fail?** Dono ek saath nahi hote. Outbox / retry. Dobara aaya to sorted set me ek hi.

**Pooche: apni hi feed me apna tweet?** Seedha jodo. Fanout peeche, apna turant.

**Pooche: worker 60/100 pe gira?** Kafka ko "ho gaya" bola hi nahi (commit nahi). Khabar phir aayegi.

**Pooche: wahi 60 dobara likhe?** Tweet ek hi baar. Sorted set me same tweet_id ek hi rehta.

**Pooche: ek worker kam pade?** Worker badhao, Kafka ki lines (partition) aapas me bat jaati.

**Pooche: 5,000 tweet/sec x 200 follower = 10 lakh inbox likhai/sec, Redis jhelega?** Ek Redis ~1 lakh/sec,
isliye Redis Cluster 10-20 tukde, inbox user_id se bate.

---

*ab aam tweet theek, par celeb ka ek tweet?*

## 3. Bieber ka ek tweet = 10 crore inbox, line ghanton atki

Celeb ka bhi push kiya. Kafka line me 10 crore kaam, mera tweet line me peeche, ghanton der.

Isliye aam user = push, celeb = pull (hybrid). Celeb ka tweet inbox me nahi jaata, celeb cache me rehta. Feed khuli to
mera inbox + follow kiye celebs ke naye tweet, time se milao.

Ye kyun chalta? Likhai ka bojh follower ginti se hai. Aam = push (inbox me), celeb = pull (padhte waqt laao + milao).
Feed hamesha dono ka mel, user ko farak nahi dikhta. Line khaali.

Sabke liye pull kar do? Har feed khulna phir 200 ka jod, mehnga.

**Pooche: 10,000 follower ke aas-paas wala?** 12,000 upar to pull, 8,000 neeche aaye tabhi push. Line pe baar-baar na
palte.

**Pooche: kisi ne 50 celeb follow kiye?** Celeb ke tweet alag chhote cache me rakhe, 50 pull bhi sasta.

**Pooche: celeb cache kitna bada?** Chhota: ~5 lakh tweet x 500 byte = ~250 MB, sirf garam.

**Pooche: tweet pe laakhon like?** Ginti 10 tukde (tweet123#0..#9), padhte waqt jodo. Ek key ek machine pe hoti.

---

*ab ek tweet pe crore log ek saath*

## 4. Match khatam, Virat ka tweet, crore log ek saath, DB gira

Har feed Virat ka tweet DB se laa rahi thi. Crore read, DB gira, feed band.

Isliye garam tweet RAM me (hot-tweet cache). Miss hua to sirf ek request DB jaaye (lock), baaki uska jawab lein.

Ye kyun chalta? Garam cheez RAM me, DB tak sirf ek request. DB aaram se.

DB ki copies badha do? Wo bhi disk se (ms). Ek garam hisse ki copies crore read nahi jhelti.

**Pooche: ek key pe bheed, wo machine garam?** Key ki kai copy (tweet:123#1..#5), kisi se bhi padho. Saath me App memory
me 1-2 sec.

**Pooche: Virat ne edit / delete kiya?** Cache se key hatao. Agla read DB se naya.

---

*ab Redis hi bhar raha*

## 5. Shaam ko Redis alert: 50 crore inbox, RAM bhar rahi

Har inbox me saare id, soye users ke bhi. RAM khatam.

Isliye sirf aakhri kuch sau (800), aur sirf zinda logon ka inbox (trim). Purane hatao. Jo 1 mahine se nahi aaya, uska
inbox hi nahi rakhte.

Ye kyun chalta? RAM me sirf id aur sirf zinda log. Jo RAM me nahi, wo DB se bana lo: ek baar dheema, galat nahi.

Redis me machine jodte jao? Soye 40 crore ka inbox bhi RAM khaata. Kharcha badhta, dikkat wahi.

**Pooche: inbox me kya rakhte?** Sirf number + score (~60-100 byte), poora tweet nahi.

**Pooche: soya user lauta?** Inbox nahi hai. Pehli baar pull se banao, phir bharo. RAM sirf zinda ki.

**Pooche: 800 ke aage scroll kiya?** Wahan se store (DB) se. Bahut kam log itna neeche jaate.

**Pooche: Redis gira?** Copy (replica). Warna har feed DB se, mehnga.

**Pooche: copy bhi gayi?** Kuch request mana, har inbox ek request, dheere bharo.

**Pooche: feed me neeche scroll, agla page?** Page number nahi, aakhri dikhe tweet ka time (cursor): "isse purane
50". Beech me naya tweet aaya to page nahi khiskta.

---

*ab tweet itne ho gaye*

## 6. DB alert: har second 5,000 tweet, 5 saal me 0.5 PB

Ek DB tha. Na rakh sake, na likh sake.

Isliye Cassandra, user_id se baanta (shard by user_id). Mere saare tweet ek machine pe, naya sabse upar. Ek read.

Ye kyun chalta? Jis cheez se padhte ho (user_id), usi se baanto. Likhna sirf aakhir me jodo, tez. 3 copy, 2 me likho
2 se padho, naya mil hi jaata.

MySQL? Har tweet pe har index disk pe dhoondh ke badalna, dheema. Join / transaction chahiye nahi.

tweet_id se kyun nahi baanta? Bojh barabar ho jaata, par profile ke liye saari machine poochni padti.

**Pooche: likhai tez kaise?** Commit log ke aakhir jodo, badlo nahi. RAM bhari to RAM ki nayi file disk pe.

**Pooche: files bahut ho gayi?** Padhna dheema. Peeche milao (compaction) + bloom filter.

**Pooche: meri machine giri?** 3 copy, 2 me likho, 2 se padho, naya mil hi jaata (quorum).

**Pooche: famous banda, uski machine garam?** user_id se baantna phir bhi theek. Garam tweet alag RAM cache me.

**Pooche: famous ke bahut tweet?** Key me mahina jodo (user_id, mahina). Ek machine na bhare.

---

*ab data ek desh me, user doosre me*

## 7. Main Bangalore me, data US me, har feed 300 ms

Har request samundar paar, 300 ms aana-jaana. Feed atak ke aati.

Isliye har region me data ki copy (multi-region). Route 53 paas wale region ka pata deta, India region se feed.
Copy US se aati rehti.

Ye kyun chalta? Data user ke paas wale region me. Doosre region ki copy thodi peeche, to region gira to feed thodi
purani, par chalti.

Sab US se hi do? Wahi 300 ms, har feed atak ke.

**Pooche: US celeb ka tweet?** Garam tweet India cache me bhi.

**Pooche: main US ghooma?** Request India bhejo, ya paas ki copy se (thoda purana).

**Pooche: poora region gira?** Route 53 zinda region pe palte. Copy peeche thi, to feed thodi purani.

**Pooche: dono region me likha?** Baad wala jeete (last write wins). Tweet aksar naya hota, takraav kam.

**Pooche: region ka fayda?** Tez. Europe ka data Europe me (GDPR). Har region ki bheed ka time alag. Ek gira to baaki
chalein.

---

*ab har region me App hi ek hai*

## 8. Raat ko har region me ek hi App machine; India wali giri, India band

Isliye kai App, aage ALB baante. App kuch yaad nahi rakhta (stateless x N). App 2 giri to bahar, baaki chalte rahe.

Ye kyun chalta? Machine kuch yaad nahi rakhti, sab Redis / DB me. Koi bhi gire, jude, badle, user ko farak nahi.

Ek hi App rakhte? Wo giri to poora region band.

**Pooche: machine giri, pata kaise?** ALB har ~10 sec poochta "zinda?". Chup = bahar, theek = wapas (Route 53 bhi).

**Pooche: naya code daalna, App band karni?** Ek-ek karke nikaalo, daalo, jodo (rolling). Baaki bheed sambhaalti.

**Pooche: nayi machine judi?** Pehli request dheemi, apna data nahi. ALB dheere traffic de.

---

*ab padhne ki bheed likhne ko daba rahi*

## 9. Highlights ki bheed ne mera tweet daalna dheema kar diya

Padhna aur likhna ek hi service me. Crore log scroll kar rahe, mera tweet usi bheed me, dheema.

Isliye likhne aur padhne ki alag service. ALB path se: POST /tweet to Tweet Svc (x 2), GET /feed to Timeline Svc
(x 20). Jahan bheed, wahi badhao.

Ye kyun chalta? Har kaam alag service, to jiski bheed badhi sirf wahi badhao. Ek ka dheema doosre ko nahi rokta.

Ek hi service rakhte? Padhne ki bheed me likhna bhi dheema. Wahi hua.

**Pooche: feed ki bheed badhi?** Sirf Timeline badhao. Tweet Svc pe asar nahi.

**Pooche: feed ko follow list kahan se?** User / Follow store se, par Redis me rakhi, har feed pe chahiye.

**Pooche: Suresh ke 5,000 follower, inbox kaise likhe?** Follow store se 1000-1000 padh ke inbox likho. Ek baar me
bhaari.

**Pooche: Graph DB (Neo4j) chahiye?** Nahi. Follow list ek table / Cassandra me. Neo4j tab jab "dost ke dost" chahiye.

---

*ab bhaari maal: photo, video*

## 10. Maine match ki video daali, sabko dheemi aur mehngi

50 MB ki video. Delhi, London, Sydney, har dekhne wala hamare US server se 50 MB khinche. Buffering + bandwidth bill.

Isliye video S3 me, user tak paas wale CDN se. Upload seedha S3 pe (presigned link), App beech me nahi. Tweet me sirf
CDN link.

Ye kyun chalta? Bhaari maal S3 me, user tak CDN se: paas, sasta. Tweet me sirf link, link badlo = nayi file. Link pe
hastakshar = sirf jisko mila wahi dekhe.

API bhi CDN se? Nahi. CDN sirf media ke raaste.

**Pooche: koi dekhe, CDN ke paas nahi?** S3 se laake kuch der rakhe.

**Pooche: photo hatayi, CDN pe abhi bhi dikhe?** CDN ko "hatao" bolo (invalidation), ya link me version, nayi file.

**Pooche: private photo?** Link pe hastakshar + expiry (signed URL). Sirf jisko mila, thodi der.

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

Tweet row: tweet_id, user_id, content, time.

---

ARCHETYPE A (read-heavy/feed) · CONCEPTS: [caching](../../FOUNDATIONS/04_caching.md) · [sharding](../../FOUNDATIONS/06_database_sharding.md) · [replication](../../FOUNDATIONS/05_database_replication.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
