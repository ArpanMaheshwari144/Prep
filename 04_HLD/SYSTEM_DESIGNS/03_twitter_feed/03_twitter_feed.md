# Twitter Feed

> App khole -> HOME TIMELINE (jinko follow karta unke tweet, latest pehle). Tweet POST bhi.
> Misaal: app khola -> Virat "Match great" · Sachin "Watching IPL" · Dhoni "Practice" (jinhe TU follow karta).
> Is design ka dil: **read SASTA** (50:1, pehle se bana ke rakho) + **CELEB ka tweet** (10 crore follower).

```
ROYAL KINGDOM (poori file isi se jodo):
   Notice Board = Redis inbox per user   · Town Crier   = Fanout service
   Palace Board = tweet store (Cassandra)· Royal Scribe = Tweet service
   News reader  = Timeline service       · Register     = Graph DB (kaun kisko follow)
   Newspaper truck = Kafka
```

---

## TASVEER (ByteByteGo / Alex Xu · CC BY-NC-ND 4.0)

![Twitter Architecture 2022 vs. 2012](https://assets.bytebytego.com/diagrams/0392-twitter-architecture-2022-vs-2012.jpeg)
Source: [Twitter Architecture 2022 vs. 2012](https://bytebytego.com/guides/twitter-architecture-2022-vs-2012/)
(asli Twitter ka naksha — timeline service, fan-out, cache. Ye design uska chhota roop.)

---

## SHURU — poocho + numbers

```
POOCHO:  "Timeline, post, search, trending, DM, notification — kis pe focus? Main home timeline + post pe."
         ~500M user maan lun? · CELEBRITY handle karna hai? (10 crore follower)  <- design ka asli mod
         bilkul real-time ya kuch second purana chalega? (haan -> precompute ka raasta khulta)
         media scope me ya sirf text?

FR:      tweet POST · HOME TIMELINE (follow kiye logon ke, latest pehle) · profile tweets
         scope bahar: search · trending · DM · notification
NFR:     feed < 200ms · READ-HEAVY · 500M user · EVENTUAL OK (2 sec purana chalega)

NUMBERS: 500M user · 500M tweet / din = ~5,800 / sec · avg follow ~200 · celeb 10 crore · read:write ~50:1
         50:1     -> har read pe compute MAT karo, PEHLE se bana ke rakho (precompute)
         celeb    -> ek tweet pe 10 crore kaam? -> asli mod
         eventual -> precompute + cache chalega
```
```
POOCHEGA: "Consistency or availability — which do you pick?"
BOL:      "For the feed and like counts, availability. A feed a couple of seconds old is fine; a feed
           that doesn't load is not."
```

---

## DABBA 0 — sabse simple

```
SOLUTION: tweets table + follows table · feed = jinko follow karta unke tweet nikaalo, time se sort, top 50
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_App["App"]
    n_DB["DB"]
    n_USER --> n_App
    n_App --> n_DB
```

---

## DIKKAT 1 — har baar app kholne pe 200 logon ka data joda ja raha

```
DIKKAT:   app kholte hi jitne logon ko follow karta, sabke tweet laao, jodo, sort karo -> har user, har baar.
          Read >> write -> FEED SLOW.

SOLUTION: (1) FAN-OUT ON WRITE: tweet POST hote hi har follower ke INBOX me daal do (precompute).
          (2) App khuli -> sirf apna ready inbox padho -> turant.
          Kyun: tweet ek baar banta, kai baar padha jaata -> mehnat likhte waqt sasti.

NAYA:     Fanout (tweet ko har follower ke inbox me daalne wala) · Redis inbox (har user ki ready feed, sirf tweet_id)
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_App["App"]
    n_Fanout["Fanout"]
    n_Redis_inbox["Redis inbox"]
    n_DB["DB"]
    n_USER --> n_App
    n_App --> n_Fanout
    n_App --> n_DB
    n_Fanout --> n_Redis_inbox
    n_App --> n_Redis_inbox
```
```
BOARD PE: app khuli -> 200 follow -> sabke tweet -> sort -> top 50 · read : write = 50 : 1
          Virat tweet -> Fanout -> redis:inbox:arpan = [t9, t7, t3, ...]

AGLA SAWAAL (tere jawab se):
  "Inbox me sirf tweet_id, to feed dikhate waqt poora tweet kahan se?"
   -> inbox se 50 id -> ek saath batch me tweet store / hot cache se laao (MGET), ek-ek nahi
  "Kisi ko unfollow kiya, uske tweet inbox me pade hain?"
   -> padhte waqt follow list se filter, ya peeche se saaf. Turant poora inbox nahi badalte
```

---

## DIKKAT 2 — tweet ka button 3 sec ghoomta raha (200 inbox likhe ja rahe the)

```
DIKKAT:   saare followers ke inbox likhna POST ki request me hi -> button ghoomta raha; Redis slow hua to
          POST hi fail, jabki tweet ban chuka tha.

SOLUTION: (1) Tweet DB me likho + EVENT Kafka pe -> user ko turant "ho gaya".
          (2) FANOUT WORKERS peeche se inbox bharte.
          Worker gira -> tweet nahi khoya, event Kafka me hai, wahin se wapas.

NAYA:     Kafka
BADLA:    Fanout -> Fanout workers (Kafka se padhte)
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_App["App"]
    n_Kafka["Kafka"]
    n_Fanout_workers["Fanout workers"]
    n_Redis_inbox["Redis inbox"]
    n_DB["DB"]
    n_USER --> n_App
    n_App --> n_Kafka
    n_App --> n_DB
    n_Kafka --> n_Fanout_workers
    n_Fanout_workers --> n_Redis_inbox
    n_App --> n_Redis_inbox
```
```
POOCHEGA: "I just tweeted but don't see it in my own feed. Why?"
DHYAAN:   fanout PEECHE chalta hai — isliye. "bug hai" nahi
BOL:      "Read-your-own-writes: the author's new tweet is added to their own feed directly, without
           waiting for fan-out."

AGLA SAWAAL (tere jawab se):
  "Fanout worker beech me gira (100 me se 60 inbox likhe)?"
   -> Kafka offset commit nahi hua -> event dobara aata, worker phir se likhta. Inbox me id dobara na aaye
      -> inbox Redis LIST hai (LPUSH dobara = id do baar) -> LPUSH se pehle idempotency check (SET eventId NX)
      ya inbox ko sorted set banao (ZADD same id = ek hi, score = time)
  "Kafka me partition kaise baante?"
   -> key = author_id -> ek author ke tweet ek partition, kram me; workers partitions baant lete
```

---

## DIKKAT 3 — Bieber ne tweet kiya: 10 CRORE inbox

```
DIKKAT:   celebrity ka ek tweet = 10 crore inbox me likhna -> fanout atak gaya, poora system atka.

SOLUTION: HYBRID:
          (1) Normal user (kam followers) -> PUSH: inbox me daalo.
          (2) Celebrity (bahut followers) -> PULL: tweet sirf apne store me, fanout nahi.
          (3) Feed padhte waqt: apna inbox + followed celebs ke naye tweet -> milao, time se sort.
          Kaun celeb: followers ki ek had (threshold) se tay.

NAYA:     koi dabba nahi — Fanout celeb ko SKIP karta, App read pe merge karta
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_App["App<br/>push + pull merge"]
    n_Kafka["Kafka"]
    n_Fanout_workers["Fanout workers<br/>celeb SKIP"]
    n_Redis_inbox["Redis inbox"]
    n_DB["DB"]
    n_USER --> n_App
    n_App --> n_Kafka
    n_App --> n_DB
    n_Kafka --> n_Fanout_workers
    n_Fanout_workers --> n_Redis_inbox
    n_App --> n_Redis_inbox
```
```
BOARD PE: ek tweet -> 100,000,000 inbox write
          < 10K follower = PUSH · > 10K = PULL · 8K pe push, 10.5K hua -> aage ke tweet pull
          Arpan follows Virat (celeb) + Suresh -> Suresh = inbox se · Virat = tweet store se

POOCHEGA: "What about a celebrity / hot key / hot partition?"
DHYAAN:   GALAT RAASTE: "user_id pe shard hi galat" (baaki crore ke liye theek, dikkat EK key ki) ·
          "celeb ko alag server" (wo bhi EK server). consistent hashing ek hot key ko nahi bachata
BOL:      "Sharding by user id is fine for everyone else; the problem is one hot key, and consistent
           hashing doesn't fix that. For reads I'd replicate that key in the cache across many nodes
           and add a local cache on each app server, with media on a CDN. For the feed, celebrities
           use fan-out on read. If writes are hot, like likes, I'd split the key into buckets
           (tweet123#0..#9) and sum on read."

AGLA SAWAAL (tere jawab se):
  "Merge kahan hota aur kitna mehnga?"
   -> Timeline svc me: inbox (50 id) + celeb list (aksar 10-20 celeb, har ek ke latest kuch) -> time se sort
      -> chhota kaam, celeb ke tweet hot cache se
  "Koi 9,999 se 10,001 follower pe aaya-gaya baar-baar?"
   -> beech me gap rakho (12K se upar gaya to pull · 8K se neeche aaya tabhi wapas push) taaki baar-baar palti na ho
```

---

## DIKKAT 4 — Virat ka tweet 10 crore log ek saath padh rahe

```
DIKKAT:   celeb ka tweet crore log ek saath padh rahe -> pull wale saare read seedha tweet DB pe -> DB crash.

SOLUTION: HOT-TWEET CACHE: celeb ke naye tweet RAM me (TTL ke saath), sab wahin se padhein.
          Purane (cold) seedha DB se. Miss -> DB se laake cache me.

NAYA:     Hot-tweet cache (celeb ke naye tweet RAM me, sab wahin se padhein)

KYUN YE:  Cassandra ke read replica kyun nahi -> wo bhi disk se (ms), cache RAM se (<1ms), aur replica 10 crore read nahi jhelta
          cache miss pe 1000 request ek saath DB pe na jaayein -> sirf EK DB jaaye, baaki cache bharne ka intezaar (lock)
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_App["App"]
    n_Kafka["Kafka"]
    n_Fanout_workers["Fanout workers"]
    n_Redis_inbox["Redis inbox"]
    n_Hot_tweet_cache["Hot-tweet cache"]
    n_DB["DB"]
    n_USER --> n_App
    n_App --> n_Kafka
    n_App --> n_Hot_tweet_cache
    n_App --> n_DB
    n_Kafka --> n_Fanout_workers
    n_Fanout_workers --> n_Redis_inbox
    n_App --> n_Redis_inbox
```
```
BOARD PE: SETEX ... 3600 (1 hr) · ~95% hit · < 1 hr = HOT -> cache · purana = COLD -> DB

POOCHEGA: "What if traffic suddenly spikes 10x?"
BOL:      "Kafka holds the fan-out burst. If I know when it's coming, like an IPL final, I scale out and
           pre-warm the hot-tweet cache beforehand — autoscaling takes minutes, the spike takes seconds."

AGLA SAWAAL (tere jawab se):
  "Virat ne tweet edit / delete kiya, cache purana?"
   -> delete / edit pe us tweet ki cache key bhi DEL, agla read DB se naya
  "Ek hi tweet ki key ek Redis node pe -> wahi node garam?"
   -> us key ki kai copy (tweet:123#1..#5), read random copy se + App me chhota local cache
```

---

## DIKKAT 5 — 500M inbox Redis me? memory phat jaayegi

```
DIKKAT:   har user ka poora feed memory me -> crore users, memory phat jaayegi.

SOLUTION: (1) INBOX (har user ka): sirf tweet_id, poora tweet nahi, aur aakhri kuch sau hi.
          (2) HOT-TWEET cache (shared): sirf naye celeb tweet, chhota.
          (3) Mahine bhar se nahi aaya -> inbox hatao; wapas aaya to DB se ek baar bana do.

NAYA:     koi dabba nahi
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_App["App"]
    n_Kafka["Kafka"]
    n_Fanout_workers["Fanout workers"]
    n_Redis_inbox["Redis inbox"]
    n_Hot_tweet_cache["Hot-tweet cache"]
    n_DB["DB"]
    n_USER --> n_App
    n_App --> n_Kafka
    n_App --> n_Hot_tweet_cache
    n_App --> n_DB
    n_Kafka --> n_Fanout_workers
    n_Fanout_workers --> n_Redis_inbox
    n_App --> n_Redis_inbox
```
```
BOARD PE: inbox: LTRIM 800 -> ~6.4 KB · 500M x 6.4 KB = ~3.2 TB -> Redis cluster
          hot-tweet: ~500K x 500 B = ~250 MB -> ek node · inactive = 30 din

POOCHEGA: "What if the cache goes down?"
DHYAAN:   Redis gira -> har feed DB se banana -> mehnga -> DB bhi gir sakta
BOL:      "Redis is a replicated cluster. If it fails, I shed load, let one request rebuild each hot
           key instead of thousands, and warm inboxes back up gradually."

AGLA SAWAAL (tere jawab se):
  "Inactive user wapas aaya, rebuild me kitna time?"
   -> pehli baar feed pull se bana do (follows ke latest tweet), phir inbox bhar do -> ek baar dheema, phir tez
  "LTRIM 800 ke baad purana scroll?"
   -> 800 ke aage scroll = DB se pull (bahut kam log itna neeche jaate)
```

---

## DIKKAT 6 — saare tweets ek DB me nahi aayenge

```
DIKKAT:   saare tweets ek DB me nahi aayenge: petabytes data, bahut zyada writes.

SOLUTION: (1) CASSANDRA: likhna bahut tez (LSM), access simple key se.
          (2) SHARD by user_id + time: user ke tweet ek jagah (profile tez), celeb bhi time se bat jaata.
              (tweet_id pe = load barabar par profile mehnga · country / date = bura key, ek tukda bhari)
          (3) Celeb ke tweet kai shard pe copy -> read bat jaate.

BADLA:    DB -> Cassandra (shard by user_id + time)

KAISE (LSM = tez write):
          write -> pehle commit log (disk pe sirf append) + RAM ki memtable -> user ko "done"
          memtable bhari -> disk pe SSTable file seedha likh deta. Purana data badalta nahi, bas nayi file
          peeche se chhoti files mila di jaati (compaction)
KYUN YE:  MySQL me har write = B-tree index me jagah dhoondh ke badlo (random disk write) -> itne writes pe dheema
          join / transaction yahan chahiye hi nahi, bas "user ke tweet time ke kram me" 
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_App["App"]
    n_Kafka["Kafka"]
    n_Fanout_workers["Fanout workers"]
    n_Redis_inbox["Redis inbox"]
    n_Hot_tweet_cache["Hot-tweet cache"]
    n_Cassandra["Cassandra"]
    n_USER --> n_App
    n_App --> n_Kafka
    n_App --> n_Hot_tweet_cache
    n_App --> n_Cassandra
    n_Kafka --> n_Fanout_workers
    n_Fanout_workers --> n_Redis_inbox
    n_App --> n_Redis_inbox
```
```
POOCHEGA: "The database is too big / takes too many writes. What do you do?"
BOL:      "Shard by user id so a user's tweets sit together and the profile page is one shard, add time
           as a sub-key so hot users spread out, and replicate the hottest users' tweets for reads."

AGLA SAWAAL (tere jawab se):
  "LSM me read dheema kyun ho sakta?"
   -> ek key kai SSTable me bikhri ho sakti -> sab dekhna padta. Bloom filter + compaction se kam
  "Partition key aur clustering key kya?"
   -> partition = (user_id, month) -> kis node pe · clustering = tweet time DESC -> andar kram
```

---

## DIKKAT 7 — India ka user US ke shard se padh raha (200ms)

```
DIKKAT:   India ka user US ke shard se padh raha -> har request door, dheema.

SOLUTION: GEO SHARDING (India / EU / US). Kyun: latency (paas = tez) · compliance (GDPR, EU data EU me) ·
          load (har region ka peak alag) · failure (ek region gira, baaki chale).
          US celeb ko India se follow -> uske hot tweet India ke cache me copy.

BADLA:    Cassandra ab region-wise (India / EU / US)

KAISE:    user region tak kaise -> Route 53 latency / geo routing: DNS jawab me paas wale region ka pata
          Bieber ke tweet India tak kaise -> Cassandra multi-DC replication: har region ek DC, likha hua
          doosre DC me async copy (us DC ke liye alag replication factor)
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_App["App"]
    n_Kafka["Kafka"]
    n_Fanout_workers["Fanout workers"]
    n_Redis_inbox["Redis inbox"]
    n_Hot_tweet_cache["Hot-tweet cache"]
    n_Cassandra["Cassandra"]
    n_USER --> n_App
    n_App --> n_Kafka
    n_App --> n_Hot_tweet_cache
    n_App --> n_Cassandra
    n_Kafka --> n_Fanout_workers
    n_Fanout_workers --> n_Redis_inbox
    n_App --> n_Redis_inbox
```
```
BOARD PE: latency 5 ms (paas) vs 200 ms (door)

POOCHEGA: "What if a whole region goes down?"
BOL:      "Route 53 health checks move users to the nearest healthy region. Data is copied there
           asynchronously, so the feed may be a little stale after failover — we already accepted
           eventual consistency."

AGLA SAWAAL (tere jawab se):
  "India ka user US travel kar raha, data kahan?"
   -> uska data home region me hi, request wahan forward ya paas wali copy se read (thoda purana chalega)
  "Do region me ek saath likha (conflict)?"
   -> tweet naya hi banta (update kam) -> conflict kam. Ho to last-write-wins timestamp se
```

---

## DIKKAT 8 — sab EK App box me: bojh bhi, SPOF bhi

```
DIKKAT:   sab ek App box me: lakhon user nahi jhelta, aur gira to sab band.

SOLUTION: (1) App ke kai box (stateless) + aage ALB.
          (2) Uske aage ROUTE 53 (DNS + health check): mara hatao, user ko paas wala region do.

NAYA:     Route 53 · ALB
BADLA:    App ek se DO — bojh bat gaya, ek gire to doosra chale (asal me zaroorat jitne, diagram me 2)

KAISE:    ALB har box pe health check (GET /health har ~10 sec) -> fail = box list se bahar, theek = wapas
          Route 53 bhi isi tarah poore region / ALB ko dekhta
KYUN YE:  ek bada server (vertical) kyun nahi -> ek had ke baad mehnga aur wahi SPOF
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_Route_53["Route 53"]
    n_ALB["ALB"]
    n_App_x_N_1["App 1"]
    n_App_x_N_2["App 2"]
    n_Kafka["Kafka"]
    n_Fanout_workers["Fanout workers"]
    n_Redis_inbox["Redis inbox"]
    n_Hot_tweet_cache["Hot-tweet cache"]
    n_Cassandra["Cassandra"]
    n_USER --> n_Route_53
    n_Route_53 --> n_ALB
    n_ALB --> n_App_x_N_1
    n_ALB --> n_App_x_N_2
    n_App_x_N_1 --> n_Kafka
    n_App_x_N_2 --> n_Kafka
    n_App_x_N_1 --> n_Hot_tweet_cache
    n_App_x_N_2 --> n_Hot_tweet_cache
    n_App_x_N_1 --> n_Cassandra
    n_App_x_N_2 --> n_Cassandra
    n_Kafka --> n_Fanout_workers
    n_Fanout_workers --> n_Redis_inbox
    n_App_x_N_1 --> n_Redis_inbox
    n_App_x_N_2 --> n_Redis_inbox
```
```
AGLA SAWAAL (tere jawab se):
  "Naya box juda, uski cache khaali -> pehli requests dheemi?"
   -> App box me state nahi (Redis alag), isliye farak kam; LB naye box ko dheere-dheere traffic de (slow start)
  "Deploy karte waqt sab box restart?"
   -> rolling deploy: ek-ek box nikaalo, update, wapas. Site kabhi band nahi
```

---

## DIKKAT 9 — likhna aur padhna ek hi box me (1:50)

```
DIKKAT:   likhna aur padhna ek service me -> feed padhne ki bheed post karna bhi dheema kar deti.

SOLUTION: (1) TWEET SERVICE (likhna) + TIMELINE SERVICE (padhna, push + pull milana) -> alag scale.
          (2) USER SERVICE + GRAPH (kaun kisko follow karta). Seedha follow-list ke liye table / Cassandra
              kaafi; Graph DB tabhi jab "dost ke dost" jaise sawaal hon.

BADLA:    App -> teen ALAG service: Tweet Svc + Timeline Svc + User Svc (har ek ke kai box, diagram me ek-ek)
NAYA:     Graph DB (kaun kisko follow karta)
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_Route_53["Route 53"]
    n_ALB["ALB"]
    n_Tweet_Svc["Tweet Svc"]
    n_Cassandra["Cassandra"]
    n_Kafka["Kafka"]
    n_Fanout_workers["Fanout workers"]
    n_Redis_inbox["Redis inbox"]
    n_Timeline_Svc["Timeline Svc"]
    n_Hot_tweet_cache["Hot-tweet cache"]
    n_User_Svc["User Svc"]
    n_Graph_DB["Graph DB"]
    n_USER --> n_Route_53
    n_Route_53 --> n_ALB
    n_ALB --> n_Tweet_Svc
    n_ALB --> n_Timeline_Svc
    n_ALB --> n_User_Svc
    n_Tweet_Svc --> n_Cassandra
    n_Tweet_Svc --> n_Kafka
    n_Kafka --> n_Fanout_workers
    n_Fanout_workers --> n_Redis_inbox
    n_Timeline_Svc --> n_Redis_inbox
    n_Timeline_Svc --> n_Hot_tweet_cache
    n_Timeline_Svc --> n_Cassandra
    n_User_Svc --> n_Graph_DB
    n_Fanout_workers --> n_Graph_DB
```
```
AGLA SAWAAL (tere jawab se):
  "Timeline svc ko follow list kahan se?"
   -> User/Graph svc se, par har read pe call nahi -> follow list Redis me cache
  "Fanout ko har baar 10K followers ki list?"
   -> Graph store se pages me (1000-1000) padh ke inbox likhta
```

---

## DIKKAT 10 — photo / video duniya bhar se, har baar humare server se

```
DIKKAT:   photo / video bhaari, duniya bhar se -> har baar apne server se dena dheema aur mehnga.

SOLUTION: CDN (CloudFront): media user ke paas wali edge se.

NAYA:     CDN

KAISE:    tweet me media ka URL CDN ka hota (cdn.twitter.com/img/abc.jpg)
          user ki request paas wali EDGE pe -> wahan hai (hit) to wahin se · nahi (miss) -> edge ORIGIN (S3) se
          laata, apne paas TTL tak rakhta, phir aage sab ko wahin se
KYUN YE:  seedha S3 se -> door ke user ko dheema + S3 / bandwidth ka kharcha har baar
          (dhyaan: CDN media ke raaste pe hai, origin S3; API ke raaste pe nahi)
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_Route_53["Route 53"]
    n_CDN["CDN"]
    n_ALB["ALB"]
    n_Tweet_Svc["Tweet Svc"]
    n_Cassandra["Cassandra"]
    n_Kafka["Kafka"]
    n_Fanout_workers["Fanout workers"]
    n_Redis_inbox["Redis inbox"]
    n_Timeline_Svc["Timeline Svc"]
    n_Hot_tweet_cache["Hot-tweet cache"]
    n_User_Svc["User Svc"]
    n_Graph_DB["Graph DB"]
    n_S3["S3 media"]
    n_USER --> n_Route_53
    n_ALB --> n_Tweet_Svc
    n_ALB --> n_Timeline_Svc
    n_ALB --> n_User_Svc
    n_Tweet_Svc --> n_Cassandra
    n_Tweet_Svc --> n_Kafka
    n_Kafka --> n_Fanout_workers
    n_Fanout_workers --> n_Redis_inbox
    n_Timeline_Svc --> n_Redis_inbox
    n_Timeline_Svc --> n_Hot_tweet_cache
    n_Timeline_Svc --> n_Cassandra
    n_User_Svc --> n_Graph_DB
    n_Fanout_workers --> n_Graph_DB
    n_Route_53 --> n_ALB
    n_USER --> n_CDN
    n_CDN --> n_S3
```
```
AGLA SAWAAL (tere jawab se):
  "Photo delete ki, CDN pe abhi bhi dikh rahi?"
   -> CDN invalidation call ya chhota TTL; ya URL me version (abc_v2.jpg) -> naya URL = naya file
  "Private photo CDN pe sabko mil jaayegi?"
   -> signed URL (expiry ke saath) -> sirf jisko link mila, thodi der ke liye
```

---

## 10x SCALE — har dabba alag

```
Timeline Svc   -> har read pe 200 ka join? -> INBOX precompute (ho gaya) · box badhao
Redis inbox    -> 500M inbox -> LTRIM 800 + inactive delete · cluster + replica
celeb reads    -> HOT-TWEET cache 95% hit · hot key kai node pe copy + App me L1 local cache
Cassandra      -> shard by user_id (+ time) · Bieber shard hammer -> hot user replicate
doosre desh    -> GEO shard + hot data region me copy
Kafka          -> partition badhao · fanout workers badhao
REAL TWITTER = multi-dimensional: user_id shard + time sub-shard + geo replication + hot data global cache

POOCHEGA: "How would you scale this to 10x?"      -> user ka raasta chalo, pehle jo toote wahi ilaaj
POOCHEGA: "What's the single point of failure?"   -> har box pe "ye gira to?"
POOCHEGA: "How do you know it's working?"         -> feed p99 · error rate · Kafka lag (fanout peeche?) · alert
```

---

## POOCHE TO (deep-dive)

```
API:      POST /tweet { content } -> tweetId · GET /feed?limit=50 -> home timeline · GET /user/{id}/tweets

DB:       TWEET  -> Cassandra (massive, simple, write-heavy, LSM, user_id shard)
          INBOX  -> Redis list per user: LPUSH likhte · LRANGE padhte · LTRIM 800
                    sirf ID — warna ek tweet 200 jagah copy, memory phat-ti
          GRAPH  -> Graph DB (Neo4j) ya Cassandra (followers / following)

PUSH vs PULL:
          PUSH (fan-out on write) -> read instant, celeb pe choke
          PULL (fan-out on read)  -> write bacha, har read query
          HYBRID <- industry me yahi: normal push, celeb pull, read pe merge

WRITE:    Virat -> Tweet Svc -> Cassandra SAVE (tweet_id, user_id, content, ts)
                             -> event Kafka -> Fanout: followers nikaalo, celeb FILTER, har normal:
                                LPUSH redis:inbox:userX tweet_id
READ:     Timeline Svc -> LRANGE Redis inbox (push) + celeb ke tweet (hot cache / Cassandra, pull)
                       -> MERGE + SORT -> HYDRATE (tweet_id -> content) -> top 50
```

---

## AAKHRI DABBA + WRAP

```
Route 53 = DNS + health · CDN = media · ALB · Tweet Svc = likhna · Kafka = fanout async
Fanout workers = normal ke inbox, celeb skip · Redis inbox = sirf ID, LTRIM 800 · Hot-tweet cache = celeb read
Timeline Svc = push + pull merge · Cassandra = sab tweet, user_id + time + geo · User Svc + Graph DB = follow
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_Route_53["Route 53"]
    n_CDN["CDN"]
    n_ALB["ALB"]
    n_Tweet_Svc["Tweet Svc"]
    n_Cassandra["Cassandra"]
    n_Kafka["Kafka"]
    n_Fanout_workers["Fanout workers"]
    n_Redis_inbox["Redis inbox"]
    n_Timeline_Svc["Timeline Svc"]
    n_Hot_tweet_cache["Hot-tweet cache"]
    n_User_Svc["User Svc"]
    n_Graph_DB["Graph DB"]
    n_S3["S3 media"]
    n_USER --> n_Route_53
    n_ALB --> n_Tweet_Svc
    n_ALB --> n_Timeline_Svc
    n_ALB --> n_User_Svc
    n_Tweet_Svc --> n_Cassandra
    n_Tweet_Svc --> n_Kafka
    n_Kafka --> n_Fanout_workers
    n_Fanout_workers --> n_Redis_inbox
    n_Timeline_Svc --> n_Redis_inbox
    n_Timeline_Svc --> n_Hot_tweet_cache
    n_Timeline_Svc --> n_Cassandra
    n_User_Svc --> n_Graph_DB
    n_Fanout_workers --> n_Graph_DB
    n_Route_53 --> n_ALB
    n_USER --> n_CDN
    n_CDN --> n_S3
```
```
BOL: "On write, the Tweet service saves to Cassandra and puts an event on Kafka; fan-out workers push the
      tweet id into each normal follower's Redis inbox. On read, the Timeline service takes the inbox,
      pulls celebrity tweets from a hot-tweet cache, merges, sorts, hydrates and returns the top 50.
      Hybrid push / pull handles celebrities, Cassandra is sharded by user id and time with geo copies.
      Next I'd add ML ranking, trending and search."
```

ARCHETYPE A (read-heavy/feed) · CONCEPTS: [caching](../../FOUNDATIONS/04_caching.md) · [sharding](../../FOUNDATIONS/06_database_sharding.md) · [replication](../../FOUNDATIONS/05_database_replication.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
