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

## 7-STEP RAIL — (ABHI KHAALI — Arpan bolke bharega, phir yahan likhenge)

```
   STEP 1  REQUIREMENTS   ->
   STEP 2  ESTIMATE       ->
   STEP 3  API            ->
   STEP 4  DATA MODEL     ->
   STEP 5  HL BOXES       ->
   STEP 6  DEEP-DIVE      ->
   STEP 7  BOTTLENECK     ->
```

> Jo tu BOLEGA wahi yahan likha jayega (tere apne shabd + tere apne example) —
> taaki revise pe wahi line yaad aaye jo tune khud boli thi.
