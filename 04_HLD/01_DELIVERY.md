# HLD — DELIVERY ("KAISE bolna" + SHABD)

> **NAV** — KYA: interview me KAISE bolna (rail · meta-moves · na-pata-ho-to · follow-ups) + atke to SHABD.
> KYA bolna = [MASTER](00_MASTER_SHEET.md) · A ya B = [TRADEOFFS](02_TRADEOFFS.md) · drill = [RAW_DRILL/00_DRILL](RAW_DRILL/00_DRILL.md)
> (26-Sep: HLD_APPROACH_DELIVERY + SPEAKING_VOCAB is ek file me mile. Purani files git history me.)

---

# PART 1 — KAISE BOLNA (approach / delivery playbook)

> Ye note DESIGN ka content nahi — ye HLD interview me KAISE conduct karna hai, wahi.
> Banaya 3-Sep: Arpan ne pehli baar HLD-mock kiya + bola "padh liya par bolna kaise nahi aata".
> HLD = khula game (DSA binary + Java deterministic se ALAG). Par khula ≠ blank — hamesha RAIL hoti hai.

---

## 0 — 3 rounds ka farak (kyun HLD alag lagta)
```
   DSA     = BINARY      (library me pattern hai -> ho gaya; warna nahi)
   Java    = DETERMINISTIC (fixed sawaal -> fixed jawab)
   HLD     = OPEN GAME   (koi answer-key nahi; interviewer SAATH drive karta)
```
> HLD grade karta = SOCH + COMMUNICATION + TRADE-OFF navigate + ambiguity me aage badhna.
> "Perfect complete answer" grade karta hi NAHI. Open isliye = wo dekhna chahte tu ANJAAN me kaise chalega.

---

## 1 — THE RAIL (khula game me kabhi blank se shuru nahi; isi 7-step pe chal)
```
   Requirements  →  Estimate  →  API  →  Data model  →  HL boxes  →  Deep-dive  →  Bottleneck
```
- Ye rail HI "approach" hai. Kisi bhi design me isi kram pe bolo.
- Har box pe: KYA (component) + KYUN (1 line reason). Bas. "Redis cache — read-heavy, DB bachana."
- Tu YE LEAD karta — interviewer wait nahi karta.

> ★ ESTIMATE pe MAT ATKO (Hello-Interview + cracked-round comments): ek QUICK estimate
>   karo (scale justify — "read-heavy, billions -> cache+shard") phir aage. Exact number
>   (1500 vs 2000 RPS, storage 3TB vs 90TB) design nahi badalta -> us math pe waqt/energy mat do.
>   PAR bilkul SKIP bhi mat karo -> ek banda Zomato me reject hua kyunki BOTE skip karke
>   seedha distributed chala gaya. Balance = 30-sec estimate, phir move on.

---

## 2 — 4 META-MOVES (open-game ke asli sawaalon ke jawab)

### (a) "Kahan se start?"
```
   ★★ PEHLA MOVE = SCOPE KAATO (asli mock video, ex-Google EM -- 15-Sep)
      "This is a big system -- I'll scope it to X and Y and go deep there. Sound good?"
      Spotify ke round me usne turant bola: "sirf finding aur playing"
      -> poora 40-minute sirf 2 cheez pe gaya, design bikhra nahi.
      Bada product mile (LinkedIn/Uber/Zomato) -> 2-3 sub-system bolo, phir EK chuno.

   Phir: requirements clarify + 2 clarifying Q
   ("custom URL chahiye? links expire hote? scale kitna?")
   -> chup mat baitho, tu lead karta hai.

   CORE ENTITIES (20 second): requirements ke turant baad 3-5 NAAM bol do
   ("Topic, Partition, Message, Consumer Group, Offset") -- iske baad API aur
   data-model apne aap nikal aate hain. Alag bada step mat banao, ek line hai.
```

### (b) "Kya-kaise-kyun bolna?"
```
   Rail ke har box pe -> naam + EK why.
   Zyada nahi. Ek line why har component pe = seniority-signal.
```

### (c) "Trade-off kaise bolna?"  (ek fixed SAANCHA)
```
   "X vs Y — X deta [fayda] par costs [nuksan]; main X lunga kyunki [requirement]."

   e.g. "SQL vs Cassandra — SQL simple par 180B rows pe scale nahi;
         Cassandra lunga kyunki key-value + horizontal scale + HA."
   -> har trade-off isi saanche me. Rat lo ye pattern.
```

### (d) "Answer NAHI pata to kaise nikaalun?"  (SABSE IMPORTANT — noob-fear yahi)
```
   RULE: kabhi FREEZE / chup NAHI. 4 escape:

   1. REASON ALOUD   -> "Main isse aise sochunga..." (first-principles derive)
   2. ASSUME + MOVE  -> "Main maan leta X, aage badhta hoon."
   3. CLARIFYING Q   -> "Scale kitna maanu?" (waqt bhi milta)
   4. HONEST+APPROACH-> "Ye use nahi kiya, par aise approach karunga..."

   Interviewer YAHI test karta -> atakne pe kaise NAVIGATE karta.
   Wo "sab pata" nahi dekhta; wo "ambiguity handle" dekhta.
```

---

## 3 — META-SACH (darr todne ke liye)
```
   - Perfect answer koi nahi deta -> tu bhi kabhi nahi dega, na chahiye.
   - Nervousness + kuch point chhoot-na = NORMAL, fail nahi.
   - HLD = DIALOGUE (monologue nahi) -> interviewer nudge karta, tu respond.
   - Ye TRAINABLE performance-skill hai (English-delivery jaisa) -> mock-reps se aata,
     na ki "aata/nahi-aata" knowledge.
   - Pehla HLD kisi ka smooth nahi. Pehla interview = "pehla rep", "final" nahi.
```

---

## 4 — MOCK me main (Claude) kya coach karunga
```
   Answer "poora hai ki nahi" NAHI dekhunga.
   Dekhunga: rail pe chala? · har box why bola? · trade-off saanche me bola?
             · atakne pe 4-move se navigate kiya? · pichhli-baar-se behtar?
   -> laundry-list/quiz/gatekeeper nahi. Coaching, na test.
```

---

## 5 — ANJAAN DESIGN + "kuch nahi aata" ka darr (3-Sep, Arpan ka #1 fear)

### (A) Design jo padha NAHI (Google Docs / Search jaisa)
```
   Designs YAAD nahi karte -> wo same ~15 BLOCKS ke naye COMBINATION hain.
   Blocks: LB · cache · DB · queue · shard · replica · CDN · index · consistency-choice.
   15+ design padhe = ye blocks bahut baar milte dekhe -> naya = wahi blocks nayi jodni.

   Anjaan design -> RAIL pe chal ke DERIVE kar (recall nahi):
     e.g. Google Docs -> "real-time collab edit -> concurrent edits clash ->
          I'd order/sequence operations per doc." (naam OT/CRDT na aaye tab bhi REASON)
   Interviewer JAAN-BOOJH ke anjaan deta -> framework-apply dekhna, ratta nahi.
   JP-backend me Google-scale nahi -> rate-limiter/payment/notification/url type = tere paas.
```

### (B) "Kuch aata nahi -> nerves -> poora interview bekar" — us case me:
```
   1. Kuch na aana GUARANTEED + EXPECTED. Har candidate ko 1-3 cheez nahi aati.
      Grade = "na-aane pe kaise handle kiya", na ki "sab aata tha".
   2. Ek unknown se interview bekar NAHI. Poora round grade hota; JP = 2/3 round tera zone.
   3. Asli khatra = unknown nahi, SPIRAL hai ("fail ho raha -> panic -> baaki bikhra").
      Skill = unknown ko spiral se ALAG karna.
   4. FIX = pehle se ratti-hui line (rehearse -> automatic -> trigger hi na bane):
      > "I haven't worked with X directly, but I'd approach it by [jo aata usse reason].
         Let me note it and continue."
   5. "Nahi aata par aise nikaalunga" = POSITIVE signal -> bluff se zyada respect.
   6. ANCHOR: 4 saal + 700 prod-tickets -> "2am prod down, pata nahi kya toota" tune
      baar-baar dabaav me navigate kiya. Interview-unknown usse AASAAN. Tu mushkil kar chuka.
```
> NICHOD: sab nahi jaanega = normal, fail nahi. Interview banata/bigaadta = tera RESPONSE, na unknown.
> Pehla interview = warm-up rep, "final" nahi.

---

## ★★ 5b — ARPAN KA APNA NICHOD (15-Sep, asli mock videos dekh ke — sabse zaroori)

```
1. PERFECT DESIGN BANANE KI KOSHISH MAT KAR -- WO HOTA HI NAHI
   "chhote se shuru karo, phir scale ke baare me socho.
    sab kuch ek saath mat banao. sab ek saath kaise soch sakta koi -- wo IMPOSSIBLE hai.
    pehle chhota, phir usko bada."
   -> Spotify/Bitly dono asli round me yahi hua: pehle 4 box (app/LB/server/DB),
      phir jahan toota wahan CDN/cache/S3/shard joda.
   -> STEP 5 me POORA design mat banao. cache/CDN/queue/shard STEP 6-7 me,
      DIKKAT ke jawab me aayein.

2. NUMBER KE PEECHE MAT BHAAG
   "number ke peeche mat bhaago, wo bekaar hai. bas bolna hai aur aage badhna hai.
    number bol ke tu kuch SPECIAL nahi kar deta."
   -> 1000 ya 1200 -- farak nahi padta. mota andaaza bolo, ek decision nikaalo, aage badho.

3. BOTTLENECK RATTO MAT -- KHUD USER BANKE DEKHO
   "sab kuch aise hi aata rehta hai jab design bolte waqt KHUD USER banke dekho --
    ki wo kya soch raha hai, kahan cheez phat rahi hai. cheezein ratto mat."
   -> app kholi -> search -> har search DB pe?        -> cache chahiye
   -> play dabaya -> 5MB stream -> gaana viral hua?   -> CDN chahiye
   -> ye list yaad nahi karni; user ka raasta chalao, bottleneck khud dikh jaayega.
```

---

## ★★ 5c — INTERVIEWER KYA DEKHTA HAI (16-Sep, ex-Google EM ka "10 signals" video)

```
★ SABSE BADI LINE (aur ye hamare component-plan ko confirm karti hai):
    "Domain knowledge is NOT a quiz — it's the art of APPLYING components."
  Matlab: pooche jaane wale component ka lecture nahi chahiye — ye chahiye ki tu use
  SAHI JAGAH laga sake aur wajah bata sake. Isliye padhai ki ikai = COMPONENT, design nahi.
```

```
1. BOILERPLATE ME MAT ULAJHO — core problem pe raho
     load balancer, API gateway, CDN — ye boilerplate hain. Ek line me bol ke aage badho.
     Waqt CORE logic pe do (matching engine, fanout, dedupe, seat-lock — jo bhi us design ka DIL hai).
     ⚠ mera galat kadam (15-Sep): url-shortener me API-Gateway ka box "chhoot gaya tha" bol ke joda —
       wo asal me boilerplate hai, core nahi. Box ginne se score nahi milta.

2. ★ PARKING LOT — optimization dikhe to MENTION karo, turant THOKO mat
     "There's an optimization here — caching. Let me park it and come back
      once the core flow is right."
     -> dono kaam ho gaye: dikha diya ki dikhta hai tujhe, aur design saaf bhi raha.
     (yahi "chhote se shuru karo" ka bolne wala roop hai)

3. LISTENING — jo wo bole, apne shabdon me DOHRA ke confirm karo (do second ka kaam)
     "To aap chaahte hain ki main sirf play-music wale hisse pe rahun — sahi samjha?"
     + beech-beech me chhota pause; sirf bolte mat raho.

4. CONCISENESS — board pe poori kahani mat likho. Chhote label, bas.
     (poora vaakya MUH se, board sirf tera notes hai)

5. SHARING MINDSET — apni soch bolo, sirf nateeja mat bolo
     "Main ye isliye soch raha hoon ki..." -> interviewer ko tera mental model dikhe

6. FLEXIBILITY — wo feedback de ya requirement badle to approach BADAL do. ZIDD nahi.

7. TESTING YOUR DESIGN — apne hi flow ko chala ke dekho, edge case pakdo
     (= hamara "USER banke raasta chalao" — bottleneck isi se nikalta hai)

8. DESIGN CHOICES — har faisle ka "kyun" bolo. Intentional raho, ittefaq se mat lagao.

9. SCALING — back-of-the-envelope (QPS, storage) karke dikhao — par usi me atko mat.
```

```
⚠ RED / YELLOW FLAGS (ye MAT karna):
   · DEFENSIVE ho jaana — feedback pe bahas karna
   · REQUIREMENTS me itna waqt lagana ki design ka waqt hi na bache
     -> clarify + scope = 5-8 MINUTE, usse zyada nahi
   · har optimization ko turant design me thok dena (-> parking lot use karo)
```

---

## 6 — COMMON FOLLOW-UPS (jo ~har design me poochte — jawab ready rakh)

```
   "Scale 10x?"              -> shard + read-replica + cache + async (queue)
   "Ye component gir jaaye?" -> replica / failover / no single point (+ circuit breaker)
   "Consistency vs availability?" -> CAP: kaunsa chuna + KYUN (paisa/seat = CP, feed/cache = AP)
   "Race condition?"         -> atomic check+mark . lock . unique-constraint . idempotency-key
   "Hot / celebrity key?"    -> cache + consistent hashing + hybrid fanout
   "Data lost na ho?"        -> replication + durable queue + ACID jahan zaroori
   "Monitoring kya?"         -> p99 latency . error-rate . queue-lag . cache hit-rate
```

```
   ★ JP / FINANCE FLAVOR: deep-dive me ye teen shabd DROP karo -> turant asar
        IDEMPOTENCY  ·  AUDIT-TRAIL / LEDGER  ·  ACID jahan paisa hai
```

## 7 — "NA PATA HO" ka jawab — per subject (5-step)

```
   COOL-DOWN: panic ki jad = "mujhe pata HONA chahiye." Us soch ko DROP ->
              replace: "main ise REASON karunga." Ek line poora pressure utaar deti.

   1. BLUFF mat kar (galat-confident turant pakda jaata; honest > fake)
   2. Saaf bol: "ye maine directly use nahi kiya, par main aise approach karunga..."
   3. Fundamentals se REASON kar -- jo PATA hai usme tod ke socho, LOUD
   4. CLARIFYING question poochho -> waqt + engagement + interviewer khud nudge deta
   5. Jaani-hui cheez se JODO: "ye X jaisa lagta, wahan maine ___ kiya tha"

   DSA  : trick na pata -> BRUTE-FORCE se shuru ("naive O(n^2) ye, ab optimize"), loud
   JAVA : concept na pata -> "exact API yaad nahi, par mera samajh ye..." + reason
   HLD  : component na pata -> block-menu se REASON (cache/queue/shard/LB).
          single-right-answer hota hi nahi -> REASONING hi answer hai
```

---

> 1-line recall: **RAIL pakdo (7-step) → har box naam+why → trade-off saanche me → atko to 4-move (kabhi chup nahi). Perfect nahi, NAVIGATE karna hai. Anjaan = blocks se DERIVE. "Nahi aata" = ratti-line + reason, spiral se alag.**


---

# PART 2 — SHABD (speaking vocab: word atke to)

> **Kyun ye file:** concept 100% aata (Hindi me round faad de). Sirf interview me English
> **word tongue pe** nahi aata -> us line pe freeze. Ye gap CHHOTA + FINITE hai — system-design ke
> technical word gine-chune. Ye file = wo word + EXACT line jaise interview me bolega.
>
> **KAISE use kare:** padho mat — **LOUD BOLO**. Har topic ka "one-breath line" 2-3 baar zabaan se
> nikaalo. Word tongue pe chadhega. (jaise DSA sheet revise — bas yaha bolke.)
>
> **Yaad rakh:** soch tere paas HAI. Ye sirf word-swap hai (bucket khatam -> "bucket is empty").
> "sunne me native" banna zaroori nahi — clear soch + sahi word kaafi. JP-Bangalore Hinglish-ok.

---

## 0. UNIVERSAL HLD VERBS (har design me kaam aate — connective tissue)

| bolna hai (Hindi) | English word | line |
|---|---|---|
| load sambhalna | **handle** | "the system should *handle* millions of requests per second" |
| bada karna | **scale** | "we *scale horizontally* by adding more servers" |
| baant dena | **distribute** | "requests are *distributed* across nodes" |
| pakka karna | **ensure** | "to *ensure* consistency, we ..." |
| kam karna (load/delay) | **reduce / offload** | "caching *reduces* load on the database" |
| fail hone pe bhi chale | **fault-tolerant / failover** | "if a node fails, we *failover* to a replica" |
| bottleneck / adchan | **bottleneck** | "the database becomes the *bottleneck* at scale" |
| trade-off | **trade-off** | "there's a *trade-off* between consistency and latency" |

---

## 1. RATE LIMITER (token bucket)

| tera Hindi | English word | line |
|---|---|---|
| bucket se token liya | **borrow / consume** | "each request *consumes* a token from the bucket" |
| bucket bharta rehta | **refill / replenish** | "the bucket *refills* at a steady rate, say 10 tokens/sec" |
| bucket ki size | **capacity** | "the bucket has a fixed *capacity*" |
| ek saath thode zyada allow | **burst** | "this allows short *bursts* up to the bucket size" |
| bucket khatam / saare token use | **empty / exhausted** | "once the bucket is *empty*..." |
| request rok do | **throttle** | "extra requests are *throttled*" |
| mana kar do | **reject** | "the request is *rejected* with a 429 Too Many Requests" |
| ek jaisi speed | **steady rate** | "it smooths traffic to a *steady rate*" |

**ONE BREATH:** *"A token bucket has a fixed capacity and refills at a steady rate. Each request consumes a token. If tokens are available it's allowed, otherwise it's throttled and rejected with a 429 — this handles short bursts while keeping a steady average rate."*

---

## 2. CACHING

| tera Hindi | English word | line |
|---|---|---|
| data mila cache me | **cache hit** | "if it's a *cache hit*, we return immediately" |
| cache me nahi mila | **cache miss** | "on a *cache miss*, we go to the database" |
| purana data hata do | **evict** | "the least recently used entry is *evicted*" |
| expire time | **TTL (time to live)** | "each entry has a *TTL* after which it expires" |
| purana/basi data | **stale** | "the cache may serve *stale* data for a short time" |
| cache saaf karna | **invalidate** | "when data changes, we *invalidate* the cache entry" |
| load ghatana | **offload** | "caching *offloads* read traffic from the DB" |

**ONE BREATH:** *"We add a cache in front of the database. On a cache hit we return fast; on a miss we read from the DB and populate the cache. Entries have a TTL and are evicted when full. On writes we invalidate the entry to avoid stale data."*

---

## 3. LOAD BALANCING

| tera Hindi | English word | line |
|---|---|---|
| load baant do | **distribute** | "the load balancer *distributes* requests across servers" |
| baari-baari | **round robin** | "a simple strategy is *round robin*" |
| server zinda hai? check | **health check** | "it does periodic *health checks* on each server" |
| kharab server hata do | **remove from pool** | "an unhealthy server is *removed from the pool*" |
| ek user ek server pe | **sticky session** | "*sticky sessions* pin a user to one server" |

**ONE BREATH:** *"A load balancer sits in front and distributes incoming requests across servers, using strategies like round robin. It runs health checks and removes unhealthy servers from the pool, so traffic only goes to healthy nodes."*

---

## 4. DATABASE — REPLICATION & SHARDING

| tera Hindi | English word | line |
|---|---|---|
| data ki copy | **replica** | "we keep read *replicas* of the database" |
| likhne wala main DB | **primary / leader** | "writes go to the *primary*, reads to the replicas" |
| copy update hone me delay | **replication lag** | "there can be some *replication lag*" |
| data tukdo me baant do | **shard / partition** | "we *shard* the data across multiple databases" |
| kis shard me jaye | **shard key** | "we pick a *shard key*, like user id" |
| ek shard pe zyada load | **hot partition** | "a bad shard key can cause a *hot partition*" |

**ONE BREATH:** *"We use a primary for writes and read replicas for reads to scale reads, accepting some replication lag. For write scale we shard the data across databases using a shard key like user id, being careful to avoid hot partitions."*

---

## 5. ASYNC / MESSAGE QUEUES

| tera Hindi | English word | line |
|---|---|---|
| kaam baad me karo | **asynchronous** | "we process it *asynchronously*" |
| beech me queue | **queue / broker** | "requests go into a *message queue* like Kafka" |
| daalne wala | **producer** | "the *producer* publishes the event" |
| uthane wala | **consumer** | "a *consumer* picks it up and processes it" |
| load ka jhatka jhelo | **buffer / absorb spikes** | "the queue *buffers* traffic and absorbs spikes" |
| ek hi baar effect ho | **idempotent** | "processing is *idempotent*, so retries are safe" |

**ONE BREATH:** *"Instead of doing it inline, we push the work to a message queue. The producer publishes an event and a consumer processes it asynchronously. This decouples the services and lets the queue absorb traffic spikes. We make processing idempotent so retries are safe."*

---

## 6. CONSISTENCY / CAP

| tera Hindi | English word | line |
|---|---|---|
| har jagah same data | **strong consistency** | "banking needs *strong consistency*" |
| thodi der me sab same | **eventual consistency** | "for feeds, *eventual consistency* is fine" |
| network toot gaya | **network partition** | "during a *network partition*, we must choose" |
| response time | **latency** | "this reduces *latency* for the user" |
| ek saath kitne handle | **throughput** | "it increases the system's *throughput*" |

**ONE BREATH:** *"By CAP, during a network partition we choose between consistency and availability. Payments need strong consistency, but for something like a news feed, eventual consistency is acceptable to keep latency low and availability high."*

---

## 7. RELIABILITY / FAILURE

| tera Hindi | English word | line |
|---|---|---|
| ek point fail = sab fail | **single point of failure (SPOF)** | "we remove any *single point of failure*" |
| backup pe switch | **failover** | "on failure we *failover* to a standby" |
| dobara koshish | **retry with backoff** | "the client *retries with exponential backoff*" |
| girta hua system bacha lo | **circuit breaker** | "a *circuit breaker* stops calling a failing service" |
| thoda-thoda kaam karta rahe | **graceful degradation** | "the system *degrades gracefully* instead of crashing" |

**ONE BREATH:** *"We avoid single points of failure by replicating components and using failover to standbys. Clients retry with backoff, and a circuit breaker protects against a failing downstream service, so the system degrades gracefully instead of going fully down."*

---

## HOW TO DRILL (roz 5 min)
1. Ek topic uthao -> **one-breath line LOUD bolo** 2-3 baar.
2. Phir file band karke wahi concept **apne words me English me bolo** (Hindi soch -> English word).
3. Atka? -> word table dekh lo -> dobara bolo. (jaise DSA nudge.)
4. Roz 1-2 topic. Ghis-ghis ke tongue pe chadhega. **Bolna hai, padhna nahi.**

