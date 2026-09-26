# Design a Message Queue (Kafka-jaisa) — POORA ROUND (4 MOVE)

> **NAV** — ARCHETYPE B/F · DIL: kisi ko rokna nahi, kho na jaye. UP: [MASTER](../../00_MASTER_SHEET.md) · CONCEPTS: [message-queues](../../FOUNDATIONS/07_message_queues.md) · [replication](../../FOUNDATIONS/05_database_replication.md) · [sharding/partition](../../FOUNDATIONS/06_database_sharding.md) · saath: [notification](../04_notification_system/04_notification_system.md)

> 15-Sep: pehle padha, phir mock bola, phir asli mock-video ke hisaab se dobara likha — 4 move, koi rail nahi.
> Har jagah: **tu kya BOLTA hai · BOARD pe kya banta · FAISLA + KYUN**.
>
> Problem (1 line): ek aisa dabba jo events le le aur kai services ko apni-apni raftaar se de de —
> bina kisi ko rokey, bina kuch khoye.

```
★★ TEEN NIYAM (poori file par lagte — [01_DELIVERY](../../01_DELIVERY.md) 5b)
   1. PERFECT design ek saath mat banao — chhote se shuru, dikkat pe badhao
   2. NUMBER ke peeche mat bhaago — bolo, ek faisla nikaalo, aage badho
   3. BOTTLENECK ratto mat — KHUD USER banke raasta chalao, khud dikh jaayega
```

---

# MOVE 1 — POOCHO (board pe abhi kuch nahi)

```
   TU: "Message queue me kai pehlu hain — ordering, durability, delivery guarantee, replay.
        Aap kis pe focus karwana chahenge?"

   TU: "Kuch cheezein confirm kar lun —
          - message KHO sakta hai, ya bilkul nahi?
          - ORDER maayne rakhta hai? poore topic ka, ya per-user kaafi?
          - ek message KAI consumer ko jaana chahiye (broadcast), ya ek hi ko?
          - purane message dobara padhne honge (replay)?"

   ★ pehla aur teesra sawaal poora design badal dete hain.
```

---

# MOVE 2 — DO CHHOTE BLOCK LIKHO

```
   ┌──────────────────────┐   ┌────────────────────────────────────┐
   │ Message Queue        │   │ Use cases:                         │
   │   - Topic            │   │   - producer message DAALE         │
   │   - Partition        │   │   - consumer message UTHAYE        │
   │   - Message (offset) │   │   - EK message KAI consumer tak    │
   │   - Consumer Group   │   │     (email + dashboard + analytics)│
   │   - Offset           │   │   - purane message dobara padhein  │
   └──────────────────────┘   │   - ek user ke event ORDER me      │
                              └────────────────────────────────────┘
   ┌──────────────────────────────┐
   │ Kya chahiye (NFR):           │
   │  - DURABILITY  kho na jaye   │
   │  - HIGH AVAILABILITY         │
   │  - HIGH THROUGHPUT  <- DIL   │
   │    (lakhs msg/sec)           │
   │  - LOW LATENCY (daalna ms me)│
   │  - ORDERING (per key)        │
   │  - RETENTION / REPLAY        │
   │  - CONSISTENCY = EVENTUAL    │
   └──────────────────────────────┘
```

```
   ★★ "DUPLICATE NA HO" MAT BOLNA — yahin phasa tha (15-Sep mock)
      requirement me "duplicate na ho" bola, aur baad me "at-least-once, duplicate aayega,
      consumer idempotent banega" bola -> DONO EK DOOSRE KO KAATTE HAIN
      -> interviewer turant: "aapne no-duplicate bola tha, ab allow kar rahe ho?"

      SAHI LINE (yahi bolna):
        "No message loss. Duplicates ARE possible, and we handle them with idempotent
         consumers — so the EFFECT is exactly-once."
      -> virodhabhas khatam, aur baad ka jawab requirement se hi nikal aata hai.

   ★ CONSISTENCY ka matlab yahan: "sab turant sync" nahi — matlab message KHOYE nahi
     aur ORDER na toote. Consumer thoda peeche chal sakta hai (lag), aur wo THEEK hai.
```

```
   Numbers:
     - 100 M messages / day  ->  ~1,200 / sec  ->  peak 3-5x  ->  ~5,000 / sec
     - avg message ~1 KB     ->  ~1.2 MB / sec  ->  ~100 GB / day
     - retention 7 din       ->  ~700 GB
     - replication x3        ->  ~2 TB total disk

   HAR NUMBER SE FAISLA (★ ye aakhri do line bolna MAT bhoolna — 15-Sep me yahi chhoot gayi thi):
     ~2 TB        ──►  ek broker ~1-2 TB rakh leta -> 3-4 BROKER
     parallelism  ──►  ikai partition hai -> har topic 3-6 PARTITION

   ★ MQ ka asli number STORAGE hai (Kafka message delete nahi karta, RAKHTA hai) —
     ye baat bolna, kyunki log ise sirf "pipe" samajh ke storage bhool jaate hain.
```

---

# MOVE 3 — BOXES BANAO (chhota banao, phir dikkat pe badhao)

```
   TU: "Sabse simple cheez se shuru."

        [ PRODUCER ] ──► [ QUEUE (ek machine, memory me) ] ──► [ CONSUMER ]

   TU: "Chal gaya. Ab chala ke dekhte hain."
```

### dikkat 1 — "consumer band tha, message chala gaya"

```
        producer ──► queue ──► consumer (DOWN)
                       │
                       └─ memory me tha -> restart -> SAB GAYAB

   FAISLA: queue ko LOG bana do — disk pe APPEND-ONLY file + har message ko OFFSET

        P0:  [0][1][2][3][4][5]  <- naya hamesha END me lagta (beech me kabhi nahi)
                       ▲
                consumer yahan tak padha (offset = 2)

   TU: "Padhne ke baad message DELETE nahi karunga. Har message ka ek number (offset) hoga,
        aur consumer yaad rakhega kahan tak padha. Isse do faayde — consumer restart ho
        to wahin se uthayega, aur offset peeche karke REPLAY bhi kar sakte hain."

   ★★ APPEND-ONLY FILE, DB NAHI (ye sabse bada trap hai):
      1. SQL insert -> har row pe index update (B-tree) + transaction + lock
                    -> RANDOM writes = disk ka sabse SLOW mode
         append-only -> hamesha END me -> SEQUENTIAL writes = disk ka sabse TEZ (100s MB/sec)
      2. DB isliye hota hai ki tum POOCH sako ("WHERE email = x").
         Queue me tum kuch POOCHTE HI NAHI — bas "offset 500 ke baad wala do".
         -> offset = message ka KRAM-number (0, 1, 2 ...), byte-position NAHI
         -> ek chhota "sparse index" file offset -> byte-position bata deta (neeche .index dekho)
         -> DB jaisa B-tree / query index chahiye HI nahi
```

### dikkat 2 — "ek machine me 2 TB aur 5,000/sec nahi aayega"

```
   FAISLA: topic ko TUKDON me kaato — PARTITION, alag-alag broker pe

        TOPIC: user-signup
           ├─ Partition 0  ──► Broker A
           ├─ Partition 1  ──► Broker B
           └─ Partition 2  ──► Broker C

        har partition apna alag append-only log
        -> jagah bhi bat gayi, aur likhne ka load bhi
```

### dikkat 3 — "user-123 ke teen event teen alag partition me chale gaye"

```
        signup -> P0 . profile-update -> P2 . delete -> P1
        -> consumer ko ULTA-SEEDHA mil sakta (delete pehle process ho gaya!)

   ★ ORDER guarantee sirf EK PARTITION ke andar hai. POORE TOPIC ka global order Kafka DETA HI NAHI.

   FAISLA: KEY do

        partition = hash(key) % numPartitions      key = userId

        -> us user ke SAARE event EK HI partition me -> uska order pakka

   TU: "Global ordering chahiye hoti to sirf ek partition rakhni padti — aur tab poora
        throughput khatam ho jaata. Isliye per-key ordering leta hoon, ye kaafi hai."
```

### dikkat 4 — "email-service ke 3 instance chala diye — teeno ne padh liya, user ko 3 email"

```
   FAISLA: CONSUMER GROUP — "hum ek hi team hain, kaam BAANT lo"

        TOPIC user-signup (3 partition)        GROUP: email-service
              P0 ────────────────────────────► Consumer-1
              P1 ────────────────────────────► Consumer-2
              P2 ────────────────────────────► Consumer-3

        RULE: ek partition -> group me SIRF EK consumer

   ★ PARALLELISM KI LIMIT = PARTITION COUNT
     3 partition, 5 consumer -> 2 consumer KHAALI baithe rahenge
```

### dikkat 5 — "ek hi event email, analytics aur dashboard TEENO ko chahiye"

```
   ★★ YAHAN GALTI HUI THI (15-Sep mock):
      GALAT bola: "har service ke liye alag TOPIC hoga"
      SAHI      : EK topic, uspe TEEN consumer-GROUP

        TOPIC: user-signup
           ├──► GROUP email-service      poora topic milta
           ├──► GROUP analytics-service  poora topic milta (dobara)
           └──► GROUP dashboard-service  poora topic milta (dobara)

      KYUN ye galti mehngi hai: alag topic banaoge to PRODUCER ko ek hi event TEEN baar
      bhejna padega -> MOVE-2 ki "ek message kai consumer tak" wali requirement hi toot gayi.
      Yahi MQ ka dil hai, interviewer isi pe ungli rakhta hai.

   SAME GROUP  = kaam BAANTO (load-share)
   ALAG GROUP  = sabko POORA (broadcast)
   har group ka apna alag offset -> ek doosre ko affect nahi karte

   OFFSET rehta kahan hai: Kafka ke apne internal topic me -> __consumer_offsets
        key = (group, topic, partition) -> value = offset
        (compaction se sirf LATEST bacha rehta)
```

### dikkat 6 — "Broker-A ki disk gayi — us partition ka SAARA data gaya"

```
   FAISLA: har partition ki COPIES doosre brokers pe (replication-factor)

        Partition 0 ──► Broker A  [LEADER]     saara read/write YAHIN
                        Broker B  [FOLLOWER]   leader se copy karta rehta
                        Broker C  [FOLLOWER]   copy karta rehta

        1. producer HAMESHA leader ko likhta
        2. follower leader se data KHEENCHTE rehte
        3. leader mara -> ek FOLLOWER naya leader ban jaata -> kaam chalu
           (faisla controller karta: purana Kafka = Zookeeper, naya = KRaft)

   ★ ISR = In-Sync Replicas (ye interview me poochte hain)
     ISR = wo follower jo leader ke SAATH-SAATH chal rahe (peeche nahi rah gaye)
     naya leader SIRF ISR me se banega
     KYUN? jo peeche rah gaya wo leader bana -> uske paas kuch event HONGE HI NAHI -> DATA LOSS

   ★ acks — producer kab maane ki "likh gaya" (ye ek TRADE-OFF hai, bolna):
        acks=0    daal ke bhaag gaya        -> sabse TEZ, data kho sakta
        acks=1    LEADER ne likh liya       -> beech ka
        acks=all  LEADER + saare ISR ne     -> sabse SAFE, thoda SLOW

        "payment/order event ke liye acks=all lunga, click/log ke liye acks=1 —
         keemat ye hai ki acks=all thoda slow karta hai."
```

### dikkat 7 — "kaam ho gaya par commit se pehle consumer crash"

```
        A) padha(offset 5) -> email bhej diya -> CRASH (commit nahi hua)
           restart -> offset abhi bhi 5 -> WAHI event dobara -> EMAIL DO BAAR

        B) padha(offset 5) -> commit kar diya -> kaam karne se PEHLE crash
           restart -> offset 6 se -> event 5 ka kaam KABHI nahi hua -> EVENT KHO GAYA

   TEEN GUARANTEE:
        AT-MOST-ONCE   pehle COMMIT phir kaam  -> duplicate NAHI, par LOSS ho sakta
        AT-LEAST-ONCE  pehle KAAM phir COMMIT  -> loss NAHI, par DUPLICATE ho sakta   <- DEFAULT
        EXACTLY-ONCE   na loss na duplicate    -> mehnga + slow, har jagah possible nahi

   ★★ ASLI JAWAB (ye line SD round me sona hai):
      "Kafka at-least-once deta hai; exactly-once EFFECT hum CONSUMER-SIDE IDEMPOTENCY se laate hain."

        consumer: Redis me eventId dekho -> hai to SKIP -> nahi to kaam karo + id daalo (TTL ~24h)

   ★★ DO ALAG KEY — ye farak bolna (bahut kam log bolte hain):
        partition-key = userId    (ordering ke liye)
        dedup-key     = eventId   (duplicate pakadne ke liye)
      userId ko dedup-key banaya to us user ka DOOSRA legit event bhi skip ho jaayega = BUG.

   ★ ye dedup cache QUEUE ke ANDAR nahi, CONSUMER-side hai. Kafka khud cache nahi karta —
     wo disk pe append karta hai aur OS ka page-cache use karta hai (isliye fast).
```

### dikkat 8 — "consumer mar gaya / consumer peeche chal raha hai"

```
   REBALANCE:
        group me C1->P0, C2->P1, C3->P2 ... C2 MARA
            -> coordinator (ek broker) ko heartbeat aana band
            -> REBALANCE -> partitions dobara bante -> C1->P0,P1 aur C3->P2  (kaam ruka nahi)
        naya consumer juda -> phir rebalance -> load phir se bat gaya

        ★ COST: purane (eager) rebalance me poora group thodi der RUKTA hai (stop-the-world)
                (Kafka 2.4+ cooperative rebalance: sirf badli hui partitions rukti hain)
                -> consumer baar-baar restart mat karo

   CONSUMER LAG (production ka sabse zaroori metric — tera 700-ticket zone):
        lag = (partition ka latest offset) - (group ka committed offset)   = "kitna peeche"
        lag badh raha -> consumer slow ya kam hain -> consumer badhao (partition se zyada nahi)
        -> alert lagao

   RETRY + DLQ (ye tu usercrud me KAR chuka hai — confidence se bolna):
        consumer fail -> backoff ke saath retry -> N baar fail -> DEAD-LETTER TOPIC
        -> baaki message atke nahi. DLQ alag se dekho / replay karo.
```

### dikkat 9 — "7 din ka data rakhoge to disk bharegi hi"

```
   RETENTION — ye QUEUE nahi, LOG hai; padhne ke baad delete nahi hota
        time-based   "7 din rakho" (default ~7 days)          <- sabse common
        size-based   "partition 100GB se bada -> purana kaato"
        compaction   per KEY sirf LATEST value rakho (baaki kaat do)
                     = "user-123 ka current address" type STATE ke liye

   ★ isi ki wajah se REPLAY milta hai:
        naya consumer-group -> offset 0 se -> poora itihaas dobara
        bug fix kiya -> offset peeche karo -> dobara process

   ★ purana SEGMENT poora file delete hota hai (row-by-row nahi) — isliye sasta
```

### ab poora naksha (jahan pahunche) + har box ka KYUN

```
 PRODUCER                    KAFKA CLUSTER                         CONSUMERS

                      +-------------------------------------+
  event               |  TOPIC: user-signup                 |      GROUP: email-svc
  key=userId  ------> |                                     | ---> (P0,P1 -> C1 / P2 -> C2)
  acks=all            |  P0 [0][1][2][3]...  Broker A (L)   |
                      |  P1 [0][1][2]...     Broker B (L)   |      GROUP: analytics-svc
  hash(key)%3         |  P2 [0][1][2][3]...  Broker C (L)   | ---> (poora topic dobara)
  -> partition        |                                     |
                      |  har P ki 3 copies (L + 2 follower) |      GROUP: dashboard-svc
                      |  retention: 7 din                   | ---> (poora topic dobara)
                      |  __consumer_offsets (internal)      |
                      +------------------+------------------+
                                         |
                            +------------+-------------+
                            |  CONTROLLER / METADATA    |  Zookeeper ya KRaft
                            |  kaun leader, ISR me kaun |  (chhota data, par 100% sahi)
                            +---------------------------+

     PARTITION   : scale + parallelism ki ikai
     KEY         : per-user ordering
     LEADER/FOLL : machine mare to data + service bache
     ISR         : naya leader sirf yahan se, warna data loss
     CONTROLLER  : split-brain roko (do broker khud ko ek hi partition ka leader na samjhein)
     __offsets   : consumer restart -> wahi se + replay
     ALAG GROUPS : ek event, teen alag kaam
```

```
   EK LINE ME FLOW:
     producer -> key se partition chuna -> LEADER pe append (offset mila) -> follower copy (ISR)
     -> har consumer-GROUP apne offset se padhta -> kaam -> offset commit
     -> event 7 din pada rehta (replay possible)
```

---

# MOVE 4 — BOLTE-BOLTE JODO (jo poocha jaaye, wahi kholo)

## ► "API kya hogi?"

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
   ★ DO BAAT JO YAHAN ALAG DIKHATI HAIN:
     1. PULL hai, PUSH nahi -> consumer poll() karta, broker dhakelta nahi.
        KYUN? consumer apni raftaar se le -> slow consumer dab ke marta nahi.
        (push hota to broker use overload kar deta.) Ye TRADE-OFF hai —
        "pull liya taaki slow consumer na mare; keemat ye ki agle poll tak thodi latency aati hai."
     2. send() BATCH karta -> ek-ek message network pe nahi jaata, jama kar ke ek saath
        -> isliye throughput itna high.

   ★ CLOSING LINE:
     "The queue's own API is deliberately tiny — send, poll, commit, seek.
      All the business logic lives in the consumer, not in the broker.
      That's why the broker can be so fast."
```

## ► "Ek message me kya-kya hoga? Disk pe kaisa dikhta hai?"

```
   MESSAGE (ek record):
     offset      long     <- partition me position (BROKER deta hai, producer nahi)
     timestamp   long
     key         bytes    <- isi se partition chunta: hash(key) % n
     value       bytes    <- asli payload (JSON / Avro)
     headers     map      <- traceId, eventType, retryCount

   DISK PE (per partition ek folder):
     /topic-user-signup/partition-0/
          00000000000000000000.log     <- messages, append-only (segment ~1GB)
          00000000000000000000.index   <- offset -> byte-position (jump ke liye)
          00000000000512340000.log     <- segment bhar gaya -> naya banao
```

```
   ★ DATA DO KISM KA HAI — ye distinction bolna (yahin "consistency" ki asli jagah hai):
     1. MESSAGES (bahut zyada, tez chahiye)
          -> append-only FILE. safety? -> replication + acks (DB nahi)
     2. METADATA (bahut kam, par 100% sahi hona chahiye)
          kaun-sa topic / kitne partition / kis partition ka LEADER kaun / ISR me kaun
          -> STRONGLY CONSISTENT store: Zookeeper (purana) ya KRaft-Raft (naya)
          -> KYUN? do broker khud ko ek hi partition ka leader samajh lein = SPLIT-BRAIN = data corrupt
     3. CONSUMER OFFSETS -> Kafka ka apna internal topic __consumer_offsets
```

## ► "Kahan tootega / 10x traffic pe?"

```
   ★ RATTO MAT — message ka raasta chalao:

      producer ne event daala
          │
          ├─► partition count -> consumer badha diye par partition 3 hi
          │                      -> extra consumer KHAALI -> lag badhta -> PARTITION BADHAO
          │   ★★ CATCH (senior-level line — 15-Sep me ye chhoot gayi thi):
          │      "Adding partitions changes hash(key) % n, so an existing key can move to a
          │       different partition and its ordering can break — that's why we start with
          │       a few extra partitions instead of adding them later."
          │
          ├─► slow consumer  -> downstream (email API/DB) slow -> lag badhta
          │                     -> consumer scale (partition tak) . batch . bhaari kaam alag topic
          │
          ├─► HOT PARTITION  -> ek bade customer ki key pe saara traffic, baaki khaali
          │                     -> key todo (userId#1..4) — par tab us key ka strict order chhodna padega
          │                        (ya us tenant ke liye alag topic)
          │
          ├─► BROKER DISK    -> 2 TB ka andaaza tha, 10x pe 20 TB
          │                     -> retention 7 -> 2-3 din . compaction . broker add
          │                        . purana data S3/tiered storage me
          │
          ├─► REBALANCE STORM-> bahut consumer, baar-baar deploy -> har baar stop-the-world
          │                     -> session-timeout tuning . static membership . rolling deploy
          │
          └─► PRODUCER side  -> acks=all har jagah -> latency badhi
                                -> event ke hisaab se knob (payment=all, logs=1)
```

## ► "Kab Kafka lena chahiye, kab NAHI?"  (trade-off — ye poocha jaata hai)

```
   KAFKA LO jab:
     - caller ko JAWAB ka intezaar nahi (fire-and-forget)  -> signup ke baad email/analytics
     - EK event, KAI consumer (broadcast)
     - bahut high throughput + replay/history chahiye      -> logs, clickstream, audit
     - producer aur consumer ki SPEED alag (buffer chahiye) -> spike absorb

   KAFKA MAT LO jab:
     - caller ko TURANT jawab chahiye        -> REST / gRPC ka kaam (sync)
     - strict GLOBAL ordering chahiye        -> Kafka sirf PER-PARTITION order deta
     - simple task-queue + per-message retry -> RabbitMQ / SQS simpler (Kafka overkill)
     - chhota system, 2 hi service           -> ek aur moving part paalna mehnga

   ★ KAFKA KI COST (ye bolne se maturity dikhti hai):
     - system EVENTUALLY-CONSISTENT ban jaata (consumer peeche chal sakta = LAG)
     - duplicate handle karna PADTA hi hai (idempotency likhni padti)
     - DEBUG mushkil: flow ab ek jagah nahi, kai service me bikhra
     - ek aur cluster maintain karna (partition / retention / consumer-lag monitoring)
```

## ► WRAP + SAB EK JAGAH (jaldi revise ke liye)

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

## ► SHABD ATAK JAAYE TO (word-freeze fallback — bina term ke bolo)

```
   "partition"      -> "the topic is split into parts, and each part sits on a different machine"
   "offset"         -> "each part is like a numbered list, and every consumer remembers
                        which number it has read up to"
   "consumer group" -> "a set of consumers that act as one team and divide the parts among themselves"
   "ISR"            -> "the copies that are fully caught up with the main one"
   "idempotent"     -> "even if the same event comes twice, the work happens only once,
                        because we store the event id and skip it the second time"
   "compaction"     -> "for each key we keep only the latest value and throw away the older ones"
```

---

[← MASTER SHEET](../../00_MASTER_SHEET.md)
