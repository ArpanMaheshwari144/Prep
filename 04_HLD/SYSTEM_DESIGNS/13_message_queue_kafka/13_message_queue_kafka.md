# Design a Message Queue (Kafka-jaisa) — CONCEPT BLOCKS (padhne ke liye)

> Ye file = 15-Sep ko jo 6 block padhe, wahi likhe hue. Pehle YE padh -> phir 7-step RAIL pe BOLKE
> design assemble kar -> phir neeche wala RAIL section tere apne shabdon se bharenge.
> Concept-detail (queue kya/kyun, general) = FOUNDATIONS/07_message_queues.md. Ye file = "design Kafka" ke liye.
> ★ Ye tera NAMED gap tha ("Kafka aa gaya to?") — is file ke baad wo gap band.

```
★ POST-OFFICE ANALOGY: tu chitthi seedha banda-ke-haath nahi deta (wo ghar pe na ho to tu ATKA).
   Tu DAKGHAR (Kafka) me daal deta -> turant free. Banda apni marzi se le jaata.
   Ek hi chitthi ki copy 3 log bhi le sakte (3 consumer-group). 7 din tak rakhi rehti (retention).
```

---

## BLOCK 1 — KAFKA HAI HI KYUN (problem pehle, solution baad me)

```
   TERA APNA usercrud SIGNUP FLOW (sync = seedha call):

      POST /signup
         -> DB me user save            (100ms)
         -> email-service call         (2s)      <- SMTP slow
         -> analytics-service call     (1s)
         -> notification-service call  (500ms)
      ------------------------------------------
         user 3.5 SECOND tak WAIT kar raha ek signup ke liye

   AUR BADI DIKKAT — COUPLING:
      email-service DOWN  ->  signup API FAIL  ->  user register hi nahi hua
      (user ka DB-save to ho gaya tha... par response error gaya)
      matlab ek CHHOTI service ne MAIN kaam gira diya.
```

```
   FIX = beech me ek DABBA (Kafka) rakh do:

      POST /signup -> DB save -> Kafka me EVENT daal do -> RESPONSE turant (~150ms)
                                       │
                                       ├──> email-service        apni raftaar se uthata
                                       ├──> analytics-service    apni raftaar se
                                       └──> notification-service apni raftaar se

   FAYDE (ye 3 line bolni hai):
     1. producer FIRE-AND-FORGET  -> user ko 3.5s wait nahi (150ms)
     2. DECOUPLED                 -> email-service down ho to bhi SIGNUP chalta rehta
     3. event INTAZAAR karta hai  -> service wapas aayi -> wahi se uthake kaam kar legi (kuch khoya nahi)
```

```
   ★ TERE CODE SE CONNECT (usercrud, live chal chuka hai):
        KafkaProducerController  -> event daalta (produce)
        KafkaConsumer            -> event uthata (consume)
        DLQ                      -> baar-baar fail hua event alag topic me (dead-letter)
```

---

## BLOCK 2 — TOPIC + PARTITION (scale + ordering)

```
   TOPIC = event ki CATEGORY (alag-alag kaam alag topic)
        user-signup  |  payment-done  |  order-placed
        # sab kuch ek topic me mat thoko -> consumer ko filter karna padega
```

```
   PARTITION = topic ko TUKDON me kaat ke alag-alag broker (machine) pe rakhna
        TOPIC: user-signup
           ├─ Partition 0  -> Broker A
           ├─ Partition 1  -> Broker B
           └─ Partition 2  -> Broker C

        KYUN: (a) ek machine me poora topic na aaye -> baant do (SCALE)
              (b) load 3 machine pe bat gaya        -> zyada throughput
```

```
   har PARTITION = APPEND-ONLY LOG + OFFSET (line-number)

        P0: [0][1][2][3][4][5] <- naya hamesha END me lagta (append), beech me nahi
                  ▲
              consumer yahan tak padh chuka (offset = 2)

        ★ padhne ke baad event DELETE NAHI hota -> isliye REPLAY possible
          (offset peeche kar do -> purane event dobara padh lo)
```

```
   ★★ ORDERING — sabse zyada poocha jaane wala point:
        ORDER guarantee = SIRF EK PARTITION ke andar.
        POORE TOPIC ka global order Kafka DETA HI NAHI.

        dikkat: user-123 ke 3 event 3 alag partition me chale gaye -> ulta-seedha process ho sakte
           (signup -> profile-update -> delete   ...  delete pehle process ho gaya to?)

   FIX = KEY:
        event ke saath ek KEY do (jaise userId)
            partition = hash(key) % numPartitions
        -> EK key ke SAARE event HAMESHA EK HI partition me
        -> us user ka order PAKKA
```

---

## BLOCK 3 — CONSUMER GROUP + OFFSET

```
   DIKKAT: email-service ke 3 instance chala diye (scale ke liye).
           agar teeno poora topic padhenge -> EK user ko 3 EMAIL. galat.
```

```
   CONSUMER GROUP = "hum ek hi team hain, kaam BAANT lo"

        TOPIC user-signup (3 partition)        GROUP: email-service
              P0 ─────────────────────────────► Consumer-1
              P1 ─────────────────────────────► Consumer-2
              P2 ─────────────────────────────► Consumer-3

        RULE: ek partition -> group me SIRF EK consumer (do ko nahi milta)
        -> kaam BAT gaya, duplicate nahi
```

```
   ★ PARALLELISM KI LIMIT = PARTITION COUNT
        3 partition, 5 consumer -> 2 consumer KHAALI baithe (kuch nahi milega)
        zyada parallel chahiye -> partition BADHAO
```

```
   ★★ ALAG GROUP = BROADCAST (Block-1 wala teen-service wala jaadu YAHI hai)

        TOPIC: user-signup
           ├──► GROUP email-service     -> POORA topic milta
           ├──► GROUP analytics-service -> POORA topic milta (dobara)
           └──► GROUP notification-svc  -> POORA topic milta (dobara)

        SAME GROUP  = kaam BAANTO (load-share)
        ALAG  GROUP = sabko POORA (broadcast)
        har group apna ALAG offset rakhta -> ek doosre ko affect nahi karte
```

```
   OFFSET kahan rakha jaata:
        Kafka ke andar hi ek internal topic: __consumer_offsets
        per (group, topic, partition) -> "kahan tak padha"
        -> consumer RESTART ho -> wahi se uthata (shuru se nahi)
        -> offset peeche kar do -> REPLAY
        COMMIT consumer karta hai (kab commit kare = Block 5 ka pura khel)
```

---

## BLOCK 4 — REPLICATION (machine mar jaaye to?)

```
   DIKKAT: Partition 0 sirf Broker-A pe hai. Broker-A ki disk gayi
           -> us partition ka SAARA data GAYA. na likh sakte na padh sakte.
```

```
   FIX = har partition ki COPIES doosre brokers pe (replication-factor)

        Partition 0 -> Broker A  [LEADER]     <- saara read/write YAHIN
                       Broker B  [FOLLOWER]   <- leader se copy karta rehta
                       Broker C  [FOLLOWER]   <- copy karta rehta

        replication-factor = 3  (3 copies)
```

```
   KAAM KAISE:
     1. producer HAMESHA leader ko likhta (follower ko nahi)
     2. follower leader se data KHEENCHTE rehte (sync)
     3. leader mara -> ek FOLLOWER ko naya LEADER bana diya -> kaam chalu
        (faisla controller karta: purana Kafka = Zookeeper, naya = KRaft)
```

```
   ★ ISR = In-Sync Replicas  (interview me ye poochte hain)
        ISR = wo follower jo leader ke SAATH-SAATH chal rahe (peeche nahi rah gaye)
        naya leader SIRF ISR me se banega
        KYUN? jo peeche rah gaya wo leader bana -> uske paas kuch event HONGE HI NAHI -> DATA LOSS
```

```
   ★ acks — producer kab maane ki "likh gaya":
        acks=0    -> daal ke bhaag gaya, confirm nahi        -> sabse TEZ, data KHO sakta
        acks=1    -> LEADER ne likh liya, bas                -> beech ka; leader turant mara aur
                                                                follower tak nahi pahuncha -> loss possible
        acks=all  -> LEADER + saare ISR ne likh liya         -> sabse SAFE, thoda SLOW

        SEEDHA TRADE-OFF: speed <--> safety
            payment / order event -> acks=all  (paisa kho nahi sakta)
            click / log event     -> acks=1    (ek log gaya to gaya, speed zaroori)
```

---

## BLOCK 5 — DELIVERY GUARANTEE + RETENTION

```
   DIKKAT (dono taraf):
     A) padha(offset 5) -> email bhej diya -> CRASH (commit nahi hua)
        restart -> offset abhi bhi 5 -> WAHI event dobara -> EMAIL DO BAAR
     B) padha(offset 5) -> commit kar diya -> kaam karne se PEHLE crash
        restart -> offset 6 se -> event 5 ka kaam KABHI nahi hua -> EVENT KHO GAYA
```

```
   TEEN GUARANTEE (isi order se yaad rakh):
     AT-MOST-ONCE   -> pehle COMMIT phir kaam  -> duplicate NAHI, par LOSS ho sakta
     AT-LEAST-ONCE  -> pehle KAAM phir COMMIT  -> loss NAHI, par DUPLICATE ho sakta   <- DEFAULT
     EXACTLY-ONCE   -> na loss na duplicate    -> mehnga + slow, har jagah possible nahi
```

```
   ★★ ASLI INDUSTRY JAWAB (ye line SD round me sona hai):
        "Kafka at-least-once deta hai; exactly-once EFFECT hum CONSUMER-SIDE
         IDEMPOTENCY se laate hain."

        matlab: exactly-once ka peecha mat karo -> at-least-once rakho (loss nahi hoga)
                + consumer ko IDEMPOTENT banao (duplicate aaye to dobara asar na kare)
```

```
   IDEMPOTENT = ye tu PEHLE SE jaanta (usercrud me KIYA hai):
        event ke saath eventId / requestId aata
        consumer:  "ye id pehle process ki?"  -> DB/Redis me dekho
                        haan -> SKIP
                        nahi -> kaam karo + id save karo
        -> event 10 baar aaye, paisa EK BAAR katega
```

```
   RETENTION — event kab tak pada rehta:
        Kafka read hone ke baad DELETE nahi karta (ye QUEUE nahi, LOG hai)
          time-based   -> "7 din rakho" (default ~7 days)          <- sabse common
          size-based   -> "partition 100GB se bada ho to purana kaato"
          compaction   -> per KEY sirf LATEST value rakho (baaki kaat do)
                          = "user-123 ka current address" type STATE ke liye

        ★ RETENTION ki wajah se hi REPLAY possible:
             naya consumer-group -> offset 0 se -> poora itihaas dobara padh lo
             bug fix kiya? -> offset peeche karo -> dobara process
```

---

## BLOCK 6 — POORA FLOW + KAB KAFKA / KAB NAHI

```
 PRODUCER                    KAFKA CLUSTER                         CONSUMERS

                      +-------------------------------------+
  event               |  TOPIC: user-signup                 |      GROUP: email-svc
  key=userId  ------> |                                     | ---> (P0,P1 -> C1 / P2 -> C2)
  acks=all            |  P0 [0][1][2][3]...  Broker A (L)   |
                      |  P1 [0][1][2]...     Broker B (L)   |      GROUP: analytics-svc
  hash(key)%3         |  P2 [0][1][2][3]...  Broker C (L)   | ---> (poora topic dobara)
  -> partition        |                                     |
                      |  har P ki 3 copies (L + 2 follower) |      GROUP: notification-svc
                      |  retention: 7 din                   | ---> (poora topic dobara)
                      +-------------------------------------+
                                                             har GROUP apna OFFSET rakhta
```

```
   EK LINE ME FLOW:
     producer -> key se partition chuna -> LEADER pe append (offset mila) -> follower copy (ISR)
     -> har consumer-GROUP apne offset se padhta -> kaam -> offset commit
     -> event 7 din pada rehta (replay possible)
```

```
   ★★ KAB KAFKA LO:
     - caller ko JAWAB ka intezaar nahi (fire-and-forget)      -> signup ke baad email/analytics
     - EK event, KAI consumer (broadcast)                      -> 3 service same event chahte
     - bahut high throughput + replay/history chahiye          -> logs, clickstream, audit
     - producer aur consumer ki SPEED alag (buffer chahiye)    -> traffic spike absorb

   ★★ KAB KAFKA MAT LO:
     - caller ko TURANT jawab chahiye        -> REST / gRPC ka kaam (sync)
     - strict GLOBAL ordering chahiye        -> Kafka sirf PER-PARTITION order deta
     - simple task-queue + per-message retry -> RabbitMQ / SQS simpler (Kafka overkill)
     - chhota system, 2 hi service           -> ek aur moving part paalna mehnga
```

```
   ★ KAFKA KI COST (ye bolega to MATURITY dikhegi -- interviewer isi ka intezaar karta):
     - system EVENTUALLY-CONSISTENT ban jaata (consumer peeche chal sakta = LAG)
     - duplicate handle karna PADTA hi hai (idempotency likhni padti)
     - DEBUG mushkil: flow ab ek jagah nahi, kai service me bikhra
     - ek aur cluster maintain karna (partition / retention / consumer-lag monitoring)
```

---

## ★ SAB KUCH EK JAGAH (revise ke liye — 6 line)

```
1. KYUN        sync coupling todna -> producer fire-and-forget, consumer apni raftaar
2. TOPIC/PART  topic = category . partition = scale + append-only log + offset
               key -> hash(key)%n -> ordering PER-PARTITION (global NAHI)
3. GROUP       group ke ANDAR partition baantte (load-share) . ALAG group = broadcast
               parallelism ki LIMIT = partition count
4. REPLICATION leader/follower . naya leader sirf ISR se . acks 0/1/all = speed vs safety
5. DELIVERY    at-least-once (default) + IDEMPOTENT consumer = exactly-once EFFECT
               retention: time / size / compaction -> isliye REPLAY milta
6. TRADE-OFF   async + broadcast + replay chahiye -> Kafka
               sync jawab / global order chahiye  -> Kafka NAHI
```

---

## ★ SHABD ATAK JAAYE TO (word-freeze fallback — bina term ke bolo)

```
   "partition"        -> "the topic is split into parts, and each part sits on a different machine"
   "offset"           -> "each part is like a numbered list, and every consumer remembers
                          which number it has read up to"
   "consumer group"   -> "a set of consumers that act as one team and divide the parts among themselves"
   "ISR"              -> "the copies that are fully caught up with the main one"
   "idempotent"       -> "even if the same event comes twice, the work happens only once,
                          because we store the event id and skip it the second time"
   "compaction"       -> "for each key we keep only the latest value and throw away the older ones"
```

---

## ★★ 7-STEP RAIL — POORA JAWAB (ye BOLNA hai — top se bottom, isi kram me)

> Upar ke 6 block = "Kafka andar se kaise kaam karta". Ye section = "interview me kaise BOLNA".
> Dono alag cheezein hain — isliye ye alag likha hai.
> [TERA] tag = jo tune 15-Sep ke mock me KHUD bola (wahi shabd rakhe hain, revise pe wahi yaad aayenge).

---

### STEP 1 — REQUIREMENTS

```
   ★ OPENER (ye pehle bolna -- problem se shuru, solution se nahi):   [TERA]
     "Maan lo ek service hai jo user save karti hai, phir email bhejti hai -- wo bhi 10 jagah --
      aur dashboard update karti hai. Agar ye sab SEEDHA karenge to user SCREEN PE RUKA rahega
      jab tak background ka saara kaam na ho jaaye. Ye galat hai. User ko TURANT jawab milna
      chahiye, baaki services apna kaam peeche karti rahein. Yahi kaam ek message queue ka hai."
```

```
   FUNCTIONAL (system ke apne kaam -- 5 line):
     · producer message DAAL sake                  -> publish(topic, key, message)
     · consumer message UTHA sake                  -> subscribe(topic, group) + poll
     · EK message KAI consumer tak                 -> email + dashboard + analytics (broadcast)
     · message KHO na jaye + dobara padh sakein    -> offset + replay
     · ek user ke event ORDER me rahein            -> key-based ordering

   NON-FUNCTIONAL (sirf "KYA chahiye" -- "kaise" step 6 me):
     · DURABILITY        -> message kho na jaye                     (kaise? replication -- baad me)
     · HIGH AVAILABILITY -> broker mare to system chalta rahe        [TERA: "service fail na ho"]
     · HIGH THROUGHPUT   -> lakhs msg/sec           <- MQ ka DIL yahi hai, ye bolna mat bhoolna
     · LOW LATENCY       -> producer ka daalna ms me                 [TERA]
     · ORDERING          -> per-key order (global nahi)
     · RETENTION/REPLAY  -> kuch din pada rahe
     · CONSISTENCY       -> EVENTUAL. yahan consistency ka matlab "sab turant sync" nahi;
                            matlab = message KHOYE nahi aur ORDER na toote. consumer thoda
                            peeche chal sakta (lag) -- aur ye THEEK hai.

   ★ GALTI JO MOCK ME HUI (mat dohrana):  "offset rakho, replica rakho" NFR me mat bolna --
     ye SOLUTION hai, requirement nahi. NFR = kya chahiye. Solution step 5/6 me.
```

### STEP 2 — ESTIMATE

```
   100M messages/day  ->  ~1,200 msg/sec   ->  peak 3-5x  ->  ~5,000 msg/sec     [TERA]

   ★ MQ me ASLI number = STORAGE (kyunki Kafka message DELETE nahi karta, rakhta hai):
        1,200/sec x ~1 KB avg   = ~1.2 MB/sec
        per day                 = ~100 GB/day
        retention 7 din         = ~700 GB
        replication-factor 3    = ~2 TB total disk

   ISI SE AGLA STEP NIKALTA HAI (ye jodna -- number ko decision se baandho):
        ek broker ~1-2 TB aaram se rakh leta   -> 3-4 BROKER
        parallelism ki ikai = partition        -> har topic 3-6 PARTITION
```

### STEP 3 — API

```
   ── PRODUCER side ───────────────────────────────────────
   send(topic, key, value)               -> "ye event daal do"
        returns (partition, offset)      <- RECEIPT: kahan pada, kis number pe
        config  acks = 0 / 1 / all       <- safety knob

   ── CONSUMER side ───────────────────────────────────────
   subscribe(topic, groupId)             -> "main is team ka hissa hoon"
   poll(timeout)                         -> batch me messages kheencho
   commit(offset)                        -> "yahan tak kaam ho gaya"
   seek(partition, offset)               -> REPLAY (offset aage/peeche)

   ── ADMIN side (ek line kaafi) ──────────────────────────
   createTopic(name, partitions, replicationFactor) / deleteTopic / describeTopic
```

```
   ★ DO BAAT JO YAHAN BOLNE SE TU ALAG DIKHEGA:
     1. PULL hai, PUSH nahi -> consumer poll() karta, broker dhakelta nahi.
        KYUN? consumer apni raftaar se le -> slow consumer dab ke marta nahi.
        (push hota to broker use overload kar deta.) Ye TRADE-OFF hai, bolna.
     2. send() BATCH karta -> ek-ek message network pe nahi jaata, jama kar ke ek saath
        -> isliye throughput itna high.

   ★ CLOSING LINE:
     "The queue's own API is deliberately tiny -- send, poll, commit, seek.
      All the business logic lives in the consumer, not in the broker.
      That's why the broker can be so fast."
```

### STEP 4 — DATA MODEL

```
   ★★ SABSE BADA TRAP: "messages DB me rakhenge" -- NAHI. Plain FILE me jaate hain.

   KYUN (do wajah, dono bolni):
     1. SQL insert -> har row pe index update (B-tree) + transaction + lock
                   -> RANDOM writes = disk ka sabse SLOW mode
        append-only file -> hamesha END me likho, kuch dhoondhna nahi
                   -> SEQUENTIAL writes = disk ka sabse TEZ mode (100s MB/sec)
     2. DB isliye hota hai ki tu POOCH sake ("WHERE email=x", "ORDER BY date").
        Queue me tu kuch POOCHTA HI NAHI -- bas "offset 500 ke baad wala do".
        -> offset = file me byte-position -> seedha jump -> index chahiye HI nahi
        -> DB ka poora machinery yahan BOJH hai, faayda nahi.
```

```
   MESSAGE (ek record):
     offset      long     <- partition me position (BROKER deta hai, producer nahi)
     timestamp   long
     key         bytes    <- isi se partition chunta: hash(key) % n
     value       bytes    <- asli payload (JSON / Avro)
     headers     map      <- traceId, eventType, retryCount

   DISK PE SHAKAL (per partition ek folder):
     /topic-user-signup/partition-0/
          00000000000000000000.log     <- messages, append-only (segment ~1GB)
          00000000000000000000.index   <- offset -> byte-position (jump ke liye)
          00000000000512340000.log     <- segment bhar gaya -> naya banao
     retention ke baad PURA SEGMENT FILE delete (row-by-row delete nahi -- isliye sasta)
```

```
   ★ TERA SQL WALA INSTINCT -- uski ASLI JAGAH ye hai (ye bolega to strong lagega):   [TERA]
     data DO KISM ka hai:
       1. MESSAGES (bahut zyada, tez chahiye)
            -> append-only FILE. safety? -> replication + acks (DB nahi)
       2. METADATA (bahut kam, par 100% sahi hona chahiye):
            kaun-sa topic / kitne partition / kis partition ka LEADER kaun / ISR me kaun
            -> STRONGLY CONSISTENT store: Zookeeper (purana) ya KRaft-Raft (naya)
            -> KYUN? do broker khud ko ek hi partition ka leader samajh le
               = SPLIT-BRAIN = data corrupt
       3. CONSUMER OFFSETS -> Kafka ka apna internal topic __consumer_offsets
            key = group+topic+partition, value = offset
            (compaction se sirf LATEST bacha rehta)

     ★ matlab: "consistency chahiye" tera instinct SAHI tha -- bas wo METADATA pe lagta hai,
       messages pe nahi. Ye distinction bolna.
```

### STEP 5 — HL BOXES

```
   ── (a) CONTEXT (pehle 10 second -- "Kafka baithta kahan hai") ─────
      USER -> API SERVICE -> [ MQ ] -> email-svc / analytics-svc / dashboard-svc -> apni DB
                   |
                   +-> apni DB (user save) + TURANT response                      [TERA]

   ── (b) AB MQ KA DABBA KHOLO -- asli jawab ye hai ────────────────

   PRODUCER (API service)
        |  send(topic, key, value), acks=all
        |  . key se partition: hash(key) % n
        |  . batching: thode msg jama kar ke ek saath
        v
   +------------------- KAFKA CLUSTER -----------------------+
   |                                                         |
   |   BROKER A           BROKER B           BROKER C        |
   |   P0 (leader)        P1 (leader)        P2 (leader)     |
   |   P1 (follower)      P2 (follower)      P0 (follower)   |  <- har P ki 3 copy,
   |   P2 (follower)      P0 (follower)      P1 (follower)   |     leader alag machine pe
   |                                                         |     (load bhi bat gaya)
   |   har partition = append-only .log + .index             |
   |   internal topic: __consumer_offsets                    |
   +---------------------------+-----------------------------+
                               |
                 +-------------+--------------+
                 |  CONTROLLER / METADATA      |  <- Zookeeper ya KRaft
                 |  kaun leader, ISR me kaun,  |     (strongly consistent, chhota data)
                 |  broker mara -> naya leader |
                 +-----------------------------+
                               | poll()
           +-------------------+--------------------+
           v                   v                    v
      GROUP: email-svc    GROUP: analytics    GROUP: dashboard
      (P0->C1, P1->C2,    (poora topic        (poora topic
       P2->C3)             dobara)             dobara)
           |                   |                    |
           v                   v                    v
        apni DB             apni DB              apni DB
```

```
   HAR DABBE KA KYUN (saath-saath bolna -- interviewer yahi poochta):
     PRODUCER-side key      -> per-user ORDER bachane ko
     BROKER x 3             -> ek machine me 2TB + 5000/sec nahi aata -> baanto
     PARTITION              -> parallelism ki IKAI (utne hi consumer chal sakte)
     LEADER / FOLLOWER      -> machine mare to data + service bacha rahe
     CONTROLLER (ZK/KRaft)  -> split-brain roko; metadata chhota par 100% sahi chahiye
     __consumer_offsets     -> consumer restart -> wahi se uthaye (+ replay)
     ALAG GROUPS            -> ek event, teen alag kaam

   ★ CLOSING MOVE (bahut acha lagta hai): neeche wale teen group pe ungli rakh ke bolna --
     "this is exactly the requirement I started with -- one event, many consumers."

   ★ CDN yahan NAHI aata (static content ka kaam hai, ye backend ka dabba hai).
```

### STEP 6 — DEEP-DIVE (2-3 cheez chuno, poora nahi)

```
   (1) ORDERING / PARTITION-KEY                                              [TERA]
       Kafka topic-level order deta hi nahi -- PARTITION ke andar deta hai.
       userId ko KEY banao -> hash(userId) % n -> us user ke saare event EK partition me
       -> us user ka order safe. (global order chahiye? -> ek hi partition = throughput khatam.
          ye trade-off bolna.)

   (2) DUPLICATE / IDEMPOTENCY                                               [TERA]
       at-least-once default hai -> duplicate aayega hi.
       consumer: Redis me eventId dekho -> hai to SKIP -> nahi to kaam karo + id daalo (TTL ~24h)
       => "Kafka at-least-once deta hai; exactly-once EFFECT hum consumer-side idempotency se laate hain."

       ★★ DO ALAG KEY -- ye farak bolna (bahut kam log bolte hain):
            partition-key = userId    (ordering ke liye)
            dedup-key     = eventId   (duplicate pakadne ke liye)
          userId ko dedup-key banaya to us user ka DOOSRA legit event bhi skip ho jaayega = BUG.

       ★ ye cache QUEUE ke ANDAR nahi, CONSUMER-side hai. Kafka khud cache nahi karta --
         wo disk pe append karta hai aur OS ka page-cache use karta hai (isliye fast).

   (3) CONSUMER MAR JAAYE TO -- REBALANCE
       group me C1->P0, C2->P1, C3->P2. C2 MARA.
         -> coordinator (ek broker) ko heartbeat aana band
         -> REBALANCE -> partitions dobara baante -> C1->P0,P1 aur C3->P2  (kaam ruka nahi)
       naya consumer juda -> phir rebalance -> load phir se bat gaya
       ★ COST: rebalance ke dauran poora group thodi der RUKTA hai (stop-the-world)
               -> consumer baar-baar restart mat karo

   (4) CONSUMER LAG (production ka sabse zaroori metric -- tera 700-ticket zone)
       lag = (partition ka latest offset) - (group ka committed offset)   = "kitna peeche"
       lag badh raha -> consumer slow ya kam hain -> consumer badhao
                        (par partition se zyada nahi -- extra khaali baithenge)
       -> alert lagao. ye bolna, monitoring ki samajh dikhti hai.

   (5) RETRY + DLQ (tu ye usercrud me KAR chuka -- confidence se bolna)
       consumer fail -> backoff ke saath retry -> N baar fail -> DEAD-LETTER TOPIC me daalo
       -> baaki messages atke nahi. DLQ alag se dekho / replay karo.
```

### STEP 7 — BOTTLENECK (traffic 10x -> kya PEHLE tootega)

```
   1. PARTITION COUNT (sabse pehle yahi tootega)
        consumer badha diye par partition 3 hi -> extra consumer KHAALI baithe -> lag badhta jaayega
        FIX: partition badhao.
        ★ CATCH (ye bolna): partition badhane se hash(key)%n BADAL jaata
           -> purani key ab doosri partition me -> us key ka ORDER TOOT sakta
           -> isliye shuru me hi thodi EXTRA partitions rakhte (ghatai nahi ja sakti)

   2. SLOW CONSUMER / LAG
        downstream (email API / DB) slow -> consumer peeche -> lag badhta
        FIX: consumer scale karo (partition tak) . batch processing . bhaari kaam alag topic me
             . agar DB bottleneck hai to wahan bulk-insert

   3. HOT PARTITION (key skew)
        ek hi key pe bahut traffic (ek bada customer) -> uski partition garam, baaki khaali
        FIX: key ko todo (userId#1..4 = composite key) -- par tab us key ka strict order chhodna padega
             (ya us tenant ke liye alag topic)

   4. BROKER DISK / RETENTION
        2 TB ka andaza tha, 10x pe 20 TB -> disk bhar jaayegi
        FIX: retention 7 din -> 2-3 din . compaction (per key sirf latest) . broker add karo
             . purana data S3/object-store me (tiered storage)

   5. REBALANCE STORM
        bahut consumer, baar-baar restart/deploy -> har baar stop-the-world
        FIX: session-timeout tuning . static membership . rolling deploy

   6. PRODUCER SIDE
        acks=all har jagah -> latency badhi
        FIX: event ke hisaab se knob -> payment/order = acks=all . click/log = acks=1
```

---

## ★ MOCK LOG — 15-Sep (pehla attempt)

```
   KYA SAHI GAYA (ye tere apne, dobara wahi bolna):
     . STEP-1 opener: "user screen pe ruka rahega, usko turant jawab do" -> problem-first framing
     . estimate: 100M/day -> ~1000/sec khud nikaala
     . ordering: "Kafka topic-level nahi, partition ke andar order deta; userId se partition"
     . dedup: "key Redis me store -> dobara process nahi hogi"  -> idempotency khud connect kiya
     . consistency ka instinct -> uski sahi jagah METADATA nikli (ZK/KRaft)

   KYA ADD KARNA (ye is file me upar likha hai, padh ke bolna):
     . NFR me "offset/replica rakho" = solution -> NFR me sirf "kya chahiye"
     . estimate me STORAGE (700GB / 2TB) -> isi se broker+partition count nikalta
     . API (send/poll/commit/seek) + PULL-not-PUSH + batching
     . data model = append-only FILE, DB nahi (+ metadata alag)
     . HL boxes me MQ ka dabba KHOLNA (yahi poora design hai)
     . step 7 bottleneck -> partition-count / lag / hot-partition / disk

   ★ NOTE: pehla attempt ADHOORI teaching pe tha -- 6 block Kafka ke MECHANICS the,
     RAIL API/data-model/estimate/bottleneck maangta hai jo padhaya nahi gaya tha.
     Ye file wo poora karti hai. Agla mock IS FILE ke baad = asli attempt.
```

