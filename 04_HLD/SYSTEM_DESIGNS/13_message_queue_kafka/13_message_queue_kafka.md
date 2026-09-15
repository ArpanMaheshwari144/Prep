# Design a Message Queue (Kafka-jaisa) — 7-STEP RAIL (single spine, revise top→bottom)

> **NAV** — ARCHETYPE B/F · DIL: kisi ko rokna nahi, kho na jaye. UP: [MASTER](../../00_MASTER_SHEET.md) · CONCEPTS: [message-queues](../../FOUNDATIONS/07_message_queues.md) · [replication](../../FOUNDATIONS/05_database_replication.md) · [sharding/partition](../../FOUNDATIONS/06_database_sharding.md) · saath: [notification](../04_notification_system/04_notification_system.md)

> HLD design. Concept-detail (queue kya/kyun, general) = FOUNDATIONS/07_message_queues.md.
> RAIL = 04_HLD/HLD_APPROACH_DELIVERY.md.
> Problem (1 line): ek aisa dabba jo events le le aur kai services ko apni-apni raftaar se de de —
> bina kisi ko rokey, bina kuch khoye.

---

## STEP 1 — REQUIREMENTS

```
   ★ OPENER (problem se shuru, solution se nahi):
     "Ek service hai jo user save karti hai, phir email bhejti hai -- wo bhi 10 jagah --
      aur dashboard update karti hai. Ye sab SEEDHA karenge to user SCREEN PE RUKA rahega
      jab tak background ka saara kaam na ho jaaye. Ye galat hai. User ko TURANT jawab
      milna chahiye, baaki services apna kaam peeche karti rahein. Yahi kaam message queue ka hai."
```

```
   FUNCTIONAL:
     · producer message DAAL sake                  -> publish(topic, key, message)
     · consumer message UTHA sake                  -> subscribe(topic, group) + poll
     · EK message KAI consumer tak                 -> email + dashboard + analytics (broadcast)
     · message KHO na jaye + dobara padh sakein    -> offset + replay
     · ek user ke event ORDER me rahein            -> key-based ordering

   NON-FUNCTIONAL (sirf "KYA chahiye" -- "kaise" step 6 me):
     · DURABILITY        -> message kho na jaye
     · HIGH AVAILABILITY -> broker mare to system chalta rahe
     · HIGH THROUGHPUT   -> lakhs msg/sec        <- MQ ka DIL yahi hai, bolna mat bhoolna
     · LOW LATENCY       -> producer ka daalna ms me
     · ORDERING          -> per-key order (global nahi)
     · RETENTION/REPLAY  -> kuch din pada rahe
     · CONSISTENCY       -> EVENTUAL. yahan iska matlab "sab turant sync" nahi;
                            matlab = message KHOYE nahi aur ORDER na toote.
                            consumer thoda peeche chal sakta (lag) -- aur ye THEEK hai.

   ★ NFR me "offset rakho / replica rakho" MAT bolna -- wo SOLUTION hai, requirement nahi.
     NFR = kya chahiye. Solution step 5/6 me.
```

```
   ★★ "DUPLICATE NA HO" MAT BOLNA (15-Sep mock me ye phasa tha)
       requirement me "duplicate na ho" bola -> step 6 me "at-least-once, duplicate aayega,
       consumer idempotent banega" bola -> DONO EK DOOSRE KO KAATTE HAIN
       -> interviewer turant: "aapne no-duplicate bola tha, ab allow kar rahe ho?"

       SAHI LINE (yahi bolna):
         "No message loss. Duplicates ARE possible, and we handle them with idempotent
          consumers -- so the EFFECT is exactly-once."
       -> contradiction khatam, aur step-6 ka jawab requirement se hi nikal aata hai.
```

## STEP 2 — ESTIMATE (numbers se decision nikaalo)

```
   100M messages/day  ->  ~1,200 msg/sec  ->  peak 3-5x  ->  ~5,000 msg/sec

   ★ MQ ka ASLI number = STORAGE (Kafka message DELETE nahi karta, RAKHTA hai):
        1,200/sec x ~1 KB avg   = ~1.2 MB/sec
        per day                 = ~100 GB/day
        retention 7 din         = ~700 GB
        replication-factor 3    = ~2 TB total disk

   ISI SE AGLA STEP NIKALTA HAI (number ko decision se baandho):
        ek broker ~1-2 TB aaram se rakh leta   -> 3-4 BROKER
        parallelism ki ikai = partition        -> har topic 3-6 PARTITION

   ★★ YE AAKHRI DO LINE BOLNA MAT BHOOLNA (15-Sep mock me chhoot gayi thi):
       700GB/2TB tak pahunch ke ruk gaya to estimate ek alag-thalag ganit lagta hai.
       Ye do line jodte hi step-5 ke BOXES isi number se nikalte dikhte hain --
       "2 TB hai, isliye 3-4 broker; parallelism partition se aati, isliye 3-6 partition."
```

## STEP 3 — API

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
        (push hota to broker use overload kar deta.) Ye TRADE-OFF hai, bolna.
     2. send() BATCH karta -> ek-ek message network pe nahi jaata, jama kar ke ek saath
        -> isliye throughput itna high.

   ★ CLOSING LINE:
     "The queue's own API is deliberately tiny -- send, poll, commit, seek.
      All the business logic lives in the consumer, not in the broker.
      That's why the broker can be so fast."
```

## STEP 4 — DATA MODEL

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
   ★ DATA DO KISM KA HAI -- ye distinction bolna (yahin "consistency" ki asli jagah hai):
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
```

## STEP 5 — HL BOXES (+ har box KYUN)

```
   ── (a) CONTEXT (pehle 10 second -- "MQ baithta kahan hai") ─────
      USER -> API SERVICE -> [ MQ ] -> email-svc / analytics-svc / dashboard-svc -> apni DB
                   |
                   +-> apni DB (user save) + TURANT response

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

   ★ CLOSING MOVE (bahut asar karta): neeche wale teen group pe ungli rakh ke bolna --
     "this is exactly the requirement I started with -- one event, many consumers."
```

```
   ★★ TOPIC vs CONSUMER-GROUP -- yahan GALTI hui thi (15-Sep mock)
       GALAT bola: "har service ke liye alag TOPIC hoga (user / email / notification)"
       SAHI      : EK topic, aur uspe TEEN consumer-GROUP

            TOPIC user-signup
               -> GROUP email-svc          poora topic milta
               -> GROUP notification-svc   poora topic milta
               -> GROUP analytics-svc      poora topic milta

       KYUN ye galti mehngi hai: alag-alag topic banaoge to PRODUCER ko ek hi event
       TEEN baar bhejna padega -> step-1 ki "ek message kai consumer tak" wali
       requirement hi toot gayi. Yahi MQ ka dil hai, interviewer isi pe ungli rakhta.

   ★ CDN yahan NAHI aata (wo static content ka kaam hai, ye backend ka dabba hai).
```

## STEP 6 — DEEP-DIVE (2-3 chuno, poora nahi)

```
   (1) TOPIC / PARTITION / ORDERING
       TOPIC = event ki category (user-signup, payment-done)
       PARTITION = topic ko tukdon me kaat ke alag broker pe -> SCALE + parallelism
       har partition = append-only log + OFFSET (line-number), naya hamesha END me

       ★ ORDER guarantee = SIRF EK PARTITION ke andar. POORE TOPIC ka global order Kafka DETA HI NAHI.
         dikkat: user-123 ke 3 event 3 partition me -> ulta-seedha process ho sakte
         FIX = KEY: partition = hash(key) % n, key = userId
               -> us user ke saare event EK partition -> order pakka
         trade-off: global order chahiye? -> ek hi partition -> throughput khatam.

   (2) CONSUMER GROUP + OFFSET
       group ke ANDAR partitions baant jaate (load-share); ek partition -> group me EK hi consumer
       ALAG group = POORA topic dobara (BROADCAST) -> yahi "ek event, 10 jagah" wala kaam
       har group ka apna offset -> ek doosre ko affect nahi karte
       ★ PARALLELISM KI LIMIT = PARTITION COUNT (3 partition, 5 consumer -> 2 khaali baithe)

   (3) REPLICATION (broker mare to?)
       har partition ki N copies: 1 LEADER (saara read/write) + followers (copy karte rehte)
       leader mara -> follower ko naya leader banaya (faisla controller karta: ZK / KRaft)
       ★ ISR = In-Sync Replicas = wo follower jo saath-saath chal rahe
         naya leader SIRF ISR se -> warna jo peeche tha wo leader bana = DATA LOSS
       ★ acks = producer kab maane "likh gaya":
            acks=0   -> daal ke bhaaga        -> tez, kho sakta
            acks=1   -> leader ne likha       -> beech ka
            acks=all -> leader + saare ISR    -> safe, thoda slow
         SEEDHA TRADE-OFF (speed vs safety): payment/order -> acks=all . click/log -> acks=1

   (4) DELIVERY GUARANTEE + IDEMPOTENCY
       kaam kar ke commit se pehle crash -> event DOBARA . commit kar ke kaam se pehle crash -> event GAYA
         AT-MOST-ONCE   pehle commit phir kaam  -> duplicate nahi, LOSS ho sakta
         AT-LEAST-ONCE  pehle kaam phir commit  -> loss nahi, DUPLICATE ho sakta   <- DEFAULT
         EXACTLY-ONCE   mehnga + slow, har jagah possible nahi
       ★★ ASLI JAWAB: "Kafka at-least-once deta hai; exactly-once EFFECT hum CONSUMER-SIDE
          IDEMPOTENCY se laate hain."
            consumer: Redis me eventId dekho -> hai to SKIP -> nahi to kaam + id daalo (TTL ~24h)
       ★★ DO ALAG KEY -- ye farak bolna (bahut kam log bolte):
            partition-key = userId    (ordering ke liye)
            dedup-key     = eventId   (duplicate pakadne ke liye)
          userId ko dedup-key banaya to us user ka DOOSRA legit event bhi skip = BUG
       ★ ye dedup-cache QUEUE ke ANDAR nahi, CONSUMER-side hai. Kafka khud cache nahi karta --
         wo disk pe append karta aur OS ka page-cache use karta (isliye fast).

   (5) RETENTION + REPLAY
       read hone ke baad DELETE nahi hota (ye QUEUE nahi, LOG hai)
         time-based   -> "7 din rakho" (default ~7 days)
         size-based   -> "partition 100GB se bada -> purana kaato"
         compaction   -> per KEY sirf LATEST value (baaki kaat do) -- state-type data ke liye
       ★ isi wajah se REPLAY milta: naya group -> offset 0 se poora itihaas
                                     bug fix kiya -> offset peeche -> dobara process

   (6) CONSUMER MAR JAAYE TO -- REBALANCE
       C1->P0, C2->P1, C3->P2 ... C2 MARA -> coordinator (ek broker) ko heartbeat band
         -> REBALANCE -> partitions dobara bante -> C1->P0,P1 aur C3->P2 (kaam ruka nahi)
       naya consumer juda -> phir rebalance -> load phir se bat gaya
       ★ COST: rebalance ke dauran poora group thodi der RUKTA hai (stop-the-world)
               -> consumer baar-baar restart mat karo

   (7) CONSUMER LAG (production ka sabse zaroori metric)
       lag = (partition ka latest offset) - (group ka committed offset)  = "kitna peeche"
       lag badh raha -> consumer slow ya kam -> consumer badhao (partition se zyada nahi)
       -> alert lagao. ye bolna, monitoring ki samajh dikhti hai.

   (8) RETRY + DLQ
       consumer fail -> backoff ke saath retry -> N baar fail -> DEAD-LETTER TOPIC me daalo
       -> baaki messages atke nahi. DLQ alag se dekho / replay karo.
```

## STEP 7 — BOTTLENECK (traffic 10x -> kya PEHLE tootega)

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
             . DB bottleneck ho to bulk-insert

   3. HOT PARTITION (key skew)
        ek hi key pe bahut traffic (ek bada customer) -> uski partition garam, baaki khaali
        FIX: key todo (userId#1..4 composite) -- par tab us key ka strict order chhodna padega
             (ya us tenant ke liye alag topic)

   4. BROKER DISK / RETENTION
        2 TB ka andaza tha, 10x pe 20 TB -> disk bhar jaayegi
        FIX: retention 7 din -> 2-3 din . compaction . broker add karo
             . purana data S3/object-store me (tiered storage)

   5. REBALANCE STORM
        bahut consumer, baar-baar restart/deploy -> har baar stop-the-world
        FIX: session-timeout tuning . static membership . rolling deploy

   6. PRODUCER SIDE
        acks=all har jagah -> latency badhi
        FIX: event ke hisaab se knob -> payment/order = acks=all . click/log = acks=1
```

```
   ★★ 15-Sep MOCK ME YE DO LINE REH GAYI THI -- agli baar ZAROOR:
     (a) DISK: "10x pe 2TB -> 20TB. retention 7->2 din, compaction, broker add,
                purana data S3/tiered-storage me."
     (b) PARTITION BADHANE KA CATCH (senior-level line, sabse zyada asar):
         "Adding partitions changes hash(key) % n, so an existing key can move to a
          different partition and its ordering can break -- that's why we start with
          a few extra partitions instead of adding them later."
```

```
   ★★ TRADE-OFF BOLNA MAT BHOOLNA (15-Sep mock me ek bhi nahi aaya)
       Is design me DO ready pade hain, kam se kam EK bolna:
         1. acks 0/1/all   -> "speed vs safety: payment ke liye acks=all, logs ke liye acks=1;
                               keemat ye ki acks=all thoda slow karta hai"
         2. pull vs push   -> "pull liya taaki slow consumer dab ke na mare;
                               keemat ye ki agle poll tak thodi latency aati hai"
       SAANCHA: "A chuna kyunki ___ . Keemat ye hai ki ___ ."
```

---

## ★ KAB KAFKA, KAB NAHI (trade-off -- ye poocha jaata hai)

```
   KAFKA LO jab:
     · caller ko JAWAB ka intezaar nahi (fire-and-forget)      -> signup ke baad email/analytics
     · EK event, KAI consumer (broadcast)                      -> 3 service same event chahte
     · high throughput + replay/history chahiye                -> logs, clickstream, audit
     · producer aur consumer ki SPEED alag (buffer chahiye)    -> spike absorb

   KAFKA MAT LO jab:
     · caller ko TURANT jawab chahiye        -> REST / gRPC ka kaam (sync)
     · strict GLOBAL ordering chahiye        -> Kafka sirf PER-PARTITION order deta
     · simple task-queue + per-message retry -> RabbitMQ / SQS simpler (Kafka overkill)
     · chhota system, 2 hi service           -> ek aur moving part paalna mehnga

   ★ KAFKA KI COST (ye bolne se maturity dikhti):
     · system EVENTUALLY-CONSISTENT ban jaata (consumer peeche chal sakta = LAG)
     · duplicate handle karna PADTA hi hai (idempotency likhni padti)
     · DEBUG mushkil: flow ab ek jagah nahi, kai service me bikhra
     · ek aur cluster maintain karna (partition / retention / consumer-lag monitoring)
```

---

## ★ SAB EK JAGAH (6 line -- jaldi revise ke liye)

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
