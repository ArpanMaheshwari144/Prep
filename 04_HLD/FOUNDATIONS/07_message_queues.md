# Message Queues

> **NAV** — KYA: queue concept (kyun/kab). POORA DESIGN: [13 message-queue](../SYSTEM_DESIGNS/13_message_queue_kafka/13_message_queue_kafka.md) · UP: [MASTER](../00_MASTER_SHEET.md) · lagta hai: [notification](../SYSTEM_DESIGNS/04_notification_system/04_notification_system.md) · [payment](../SYSTEM_DESIGNS/07_payment_system/07_payment_system.md)

> **HLD Topic 7 — Async + Decoupling + Spike absorption**

---

## ★★ QUEUE = EK CHEEZ, TEEN ALAG KAAM (designs me ye ghaalmel hota hai)

> Har design me queue dikhti hai, par uska KAAM har jagah ek nahi hota.
> Teesra wala (serialize) sabse kam pehchana jaata hai — aur wahi sabse interesting hai.

```
1. ASYNC / DECOUPLE  — "abhi mat karo, baad me kar lena"          (sabse common, ~80%)
     notification  : signup -> email/push/SMS peeche chalein
     payment       : ledger ke baad ka kaam
     file-upload   : validation 2-3 sec leti -> user ko rokna nahi
     url-shortener : analytics likhna -> redirect slow na ho
     -> FAAYDA: user ka wait khatam + email-service gir jaaye to bhi SIGNUP chalta rahe

2. BUFFER / SPIKE ABSORB — "sab ek saath mat aao, line me lago"
     bookmyshow : popular release -> lakhs log ek saath, wahi seats
                  queue spike pee jaati hai, warna DB hi baith jaata
     -> yahan kaam "peeche karwana" nahi, BHEED ko line me lagana hai
     -> queue me message ka thread nahi banta — wo bas JAMA hota hai (isliye 50k aa jaayein to bhi kuch nahi tootta)

3. ★ SERIALIZE / ORDER — "ek baar me ek hi andar aayega"   (= LOCK ka sasta badal)
     stock-broker : ek symbol ki EK queue -> orders ek-ek karke
                    -> do thread ek hi order-book pe ghus hi nahi sakte -> DOUBLE-MATCH assambhav
     bookmyshow   : per-show worker -> us show ki seat-requests ek-ek karke
     -> yahan queue SPEED ke liye nahi, SAHI-PAN (correctness) ke liye hai
     -> lock lagane ki zaroorat hi nahi padi, kyunki line ek hi hai
```

```
★ EK LINE ME FARAK:
     async     -> "abhi mat karo, baad me kar lena"
     buffer    -> "sab ek saath mat aao, line me lago"
     serialize -> "ek baar me ek hi andar aayega"
```

---

## ★★ QUEUE KI JAGAH AUR KYA LAG SAKTA THA (aur wo kyun nahi)

```
BACKGROUND THREAD (async in-process)
   + sabse sasta, koi naya box nahi
   - server crash/restart -> memory me pade saare pending kaam GAYAB
     (user ko "ho gaya" bol chuke ho, aur kisi ko pata bhi nahi chalega)
   - spike -> 50,000 thread -> server khud mar gaya
   - retry / fail hua kaam kahan jaaye -> khud likhna padega

DB KO HI QUEUE BANA LO (jobs table + worker)
   + durable hai
   - polling DB pe padti rehti + lock/contention
   - ek event KAI service ko chahiye -> ganda ho jaata

CRON / BATCH (har 5 min)
   + simple
   - der lagti hai (OTP 5 minute baad? nahi chalega)

QUEUE
   + DURABLE (disk pe, machine ke BAHAR) + spike absorb + retry/DLQ built-in + ek event kai consumer
   - ek aur system paalna + eventual consistency + duplicate khud handle karo
```

```
★ NICHOD (bolne wali line):
  "Background thread se bhi ho jaata — par wo tab tak theek hai jab tak kaam KHO jaane se
   farak na pade. Jaise hi 'ye kaam pakka hona chahiye' banta hai, mujhe DURABILITY chahiye —
   aur wahi queue deti hai."

★ FARAK EK LINE ME:
     background thread : kaam MEMORY me, usi machine pe  -> machine gayi, kaam gaya
     queue             : kaam DISK pe, machine ke BAHAR  -> machine jaaye, kaam bacha rehta
```

---

## ★★ QUEUE KAB GALAT HAI (keemat)

```
1. TURANT JAWAB CHAHIYE   -> caller intezaar kar raha hai -> yahan SYNC (REST/gRPC)
                             (OTP BHEJNA queue se ho sakta; OTP VERIFY karna nahi)
2. GLOBAL ORDER CHAHIYE   -> queue partition-level pe hi order deti hai, poore system ka nahi
3. DUPLICATE AAYEGA       -> at-least-once default -> consumer ko IDEMPOTENT banana PADEGA (ab ye kaam tera hai)
4. EVENTUAL + DEBUG MUSHKIL -> "ho gaya" bolne ke baad kaam peeche chal raha hai;
                             flow ab ek jagah nahi, kai service me bikhra
```

```
★ DO SAWAAL SE FAISLA HO JAATA HAI:
     "caller ko JAWAB chahiye?"        haan -> sync (REST)         nahi -> queue
     "kaam KHO jaaye to chalega?"      haan -> background thread   nahi -> queue (durable)
```

---

## Why MQ? (Sync vs Async)

### Without (Sync REST)
```
User → /signup → API
                  ├─ DB save (200ms)
                  ├─ Welcome email (1500ms) ← slow!
                  ├─ SMS OTP (800ms)
                  └─ Analytics (300ms)

Total: ~2.8 sec → user waiting
Email server down → ENTIRE signup fails
```

### With (Async Queue)
```
User → /signup → API
                  ├─ DB save (200ms)
                  └─ Push to queue → return 200

                  Background workers (parallel):
                     Email worker  → consume + send
                     SMS worker    → consume + send
                     Analytics     → consume + update

Total: ~250ms → user happy
Email down → message waits in queue, retry later
```

**Core benefit: DECOUPLE producer from consumer.**

---

## STORY — Restaurant Kitchen Order Slips

> Restaurant: waiter (producer) + chef (consumer).
>
> **Without queue:** Waiter chef ke pass jaaye "ye banao" → chef busy → waiter wait → service slow → lunch rush mein chaos.
>
> **With queue (slip board):** Waiter slip lagao → wapas customer ke pass → chef jab free pick kare → process → next.
>
> **Lunch spike?** Slips pile, chef apne pace pe handle. Customer wait nahi karta.
>
> **Chef sick?** Slips queue mein safe — backup chef takeover.
>
> **Slip = Message. Board = Queue. Waiter = Producer. Chef = Consumer.**

---

## 4 Big Wins

```
1. DECOUPLING
   Producer/consumer ek doosre ko nahi jaante
   Email service down → signup chalu rahe

2. ASYNC PROCESSING
   Heavy work background → user wait nahi

3. BUFFERING / SPIKE ABSORB
   1M signups/min → queue absorb
   Workers steady pace pe process

4. RELIABILITY
   Consumer crash → message wait + retry
   No data loss
```

---

## Architecture

```
   PRODUCERS              BROKER                CONSUMERS

   Web Server     ──┐                       ┌── Email Worker
   Mobile App     ──┼──► [m][m][m][m]  ─────┼── SMS Worker
   Admin Panel    ──┘   queue/topic         └── Analytics Worker
                       (Kafka/RabbitMQ)
```

---

## 2 Core Patterns

### **Point-to-Point (Queue)**
```
Producer → Queue → ONE consumer picks
   • Each message processed by ONE worker
   • Multiple workers compete for messages
   • Use: task processing (email, image resize)
```

### **Pub-Sub (Topic)**
```
Producer → Topic → ALL subscribers receive
   • Each message broadcast to many
   • Each subscriber gets own copy
   • Use: events ("order placed" → email + SMS + analytics + warehouse)
```

---

## Delivery Semantics

| Semantic | Guarantee | Speed | Use Case |
|---|---|---|---|
| **At-most-once** | May lose | Fast | Metrics, logs |
| **At-least-once** | May duplicate | Medium | Most cases (with idempotency) |
| **Exactly-once** | No loss, no dup | Slow | Banking, payments |

**Key insight:** At-least-once + idempotent consumer ≈ exactly-once (cheaper).

---

## Kafka vs RabbitMQ

| | **Kafka** | **RabbitMQ** |
|---|---|---|
| Model | Distributed log (append-only) | Traditional broker |
| Throughput | Millions/sec | ~50K/sec |
| Storage | Persistent (days/weeks) | Until consumed |
| Replay | (re-read history) | |
| Consumer | Pull-based | Push-based |
| Routing | Simple (topics) | Complex (exchanges) |
| Use case | Event streaming, analytics | Task queues, RPC |
| Mental model | "Distributed commit log" | "Smart post office" |

**Production rule:**
- Big scale + replay → **Kafka**
- Complex routing + small/medium scale → **RabbitMQ**
- AWS managed simple → **SQS**

---

## WHY Kafka Bana — LinkedIn Origin (event-driven architecture)

> Apni connection (revision 20 Jun): LinkedIn ne Kafka banaya, unka pura system pub-sub hai.

```
PROBLEM (LinkedIn ~2010): bahut services -- profile, feed, search, messaging,
   analytics, ML recommendations. Har koi ko har kisi ka data chahiye.
   Agar har service SEEDHA har service se baat kare:
        N services -> N×N connections -> SPAGHETTI
        naya consumer add -> sab producers ko change (nightmare)

SOLUTION (Kafka = central event LOG):
   koi bhi event ho (profile update, post like, job view)
        -> ek central LOG (topic) mein likho, EK BAAR
        -> jisko chahiye SUBSCRIBE kar le (feed, search, analytics, ML...)
   Producer ko pata NAHI kaun sun raha. Naya consumer add -> kisi ko change nahi.
   -> LinkedIn ka pura architecture EVENT-DRIVEN / pub-sub ban gaya.
```

**Kafka ka khaas (normal queue se farak — yehi insight):**
```
   Normal MQ (RabbitMQ/SQS): message uthaya -> DELETE (transient, ek baar)
   Kafka: DURABLE append-only LOG -> retention (days/weeks)
        -> naya consumer aaye -> PURANE events bhi padh sakta (REPLAY)
        -> isliye analytics/ML ko bhi feed kar paaya (woh history chahte)
```

> **1-line:** N×N spaghetti se bachne ko sab kuch ek central durable event-log mein —
> producer blind, consumer subscribe, replay possible. LinkedIn = pub-sub at its core.

**★ AUR GEHRA — unhone MQ banaya hi NAHI tha (18-Sep):**
```
Us waqt ke MQ (ActiveMQ/JMS) is kaam ke liye theek nahi baithe, 3 wajah:
   1. HAR MESSAGE ka hisab rakhte the (kisko diya, kisne ack kiya, dobara kisko bhejna)
      -> mehnga; volume badhne pe BROKER khud ghutne tek deta
   2. padhne pe message MITT jaata -> ek team ne le liya, dusri ko nahi
   3. isi wajah se NAYA consumer purana data kabhi nahi paa sakta tha

Jay Kreps + team ne ULTA sawaal pucha: "DB apne andar kya karta hai?"
   -> WRITE-AHEAD LOG. Ek file jisme sirf AAGE likha jaata hai.
   -> aur disk ki sabse TEZ cheez yahi hai: seedhi lambi likhai (sequential write),
      random likhai se kai guna tez.

To unhone MQ banana chhoda hi nahi -- unhone ek BAANTA HUA COMMIT LOG banaya,
aur DO faisle liye jinse baaki sab khud nikla:
   1. padhne pe kuch MAT mitao   -> message rehne do; umar (retention) poori ho tabhi hatao
   2. hisab BROKER se HATAO      -> broker ka kaam bas "aage likho + byte range do"
                                    kaun kahan tak padha = sirf EK NUMBER (offset)

   -> isi se: replay · kai team ek hi data pe · naye consumer ko poora itihaas
   -> aur throughput bhi: broker ko per-message kuch sochna hi nahi,
      OS ka page cache + zero-copy seedha network pe bhej deta hai
```

★ **Isliye ULTA kehna zyada sach hai:**
> *Kafka MQ banane ki koshish me log nahi bana. Wo LOG banaya gaya tha — aur MQ uska ek ISTEMAAL nikal aaya.*

**Jo MQ me normal hai par Kafka me hai hi NAHI** (kami nahi — us faisle ki KEEMAT hai):
```
per-message ack  ·  ek message ko haTA dena  ·  priority queue  ·  ek message ko dobara kataar me daalna
```

---

## Kafka Deep (key concepts)

```
TOPIC = logical stream of messages
   "user-events", "order-events"

PARTITION = topic split into pieces (parallelism)
   Topic "user-events" → partitions 0,1,2,3
   Each partition = ordered append-only log

PRODUCER → writes with optional KEY
   Same key → same partition (ordering preserved)

CONSUMER GROUP = set of consumers sharing work
   Each partition → ONE consumer in group
   Add consumers → parallelism (max = #partitions)

OFFSET = position in partition
   Consumer tracks: "I've read up to offset 1247"
   Replay = reset offset
```

---

## ═══ HANDS-ON — KAFKA REPLAY (asli Kafka, docker, 18-Sep) ═══

> Setup: `07_PROJECTS/usercrud/docker-compose.yml` ka `kafka` service (apache/kafka:3.8.0, KRaft).
> Har command + uska ASLI output.

### TASVEER — pehle ye, baaki sab isi ka naam hai

```
TOPIC = teen ALMARI (partition 0,1,2). Har almari me DABBE ek line me, number 0 se.
Naya dabba hamesha line ke AAKHIR me judta. Beech me ghusana / purana hatana = hota hi nahi.

PARCHI (offset) = har almari pe chipki, likha hai "agla dabba main yahan se uthaunga".
   - parchi CONSUMER ke paas nahi, KAFKA ke paas rehti hai
   - parchi par ek hi number, naya likha to purana MIT gaya
   - parchi hilane se DABBON ko kuch nahi hota

★ parchi ALMARI ki nahi, GROUP ki hoti hai -> ek hi almari pe kai groups ki alag parchi
```

### 1. Topic banaya

```
kafka-topics.sh --create --topic orders --partitions 3 --replication-factor 1
   Created topic orders.

kafka-topics.sh --describe --topic orders
   Partition: 0   Leader: 1   Replicas: 1   Isr: 1
   Partition: 1   Leader: 1   Replicas: 1   Isr: 1
   Partition: 2   Leader: 1   Replicas: 1   Isr: 1
```
`Replicas: 1` = ek hi copy (ek hi broker hai). `Isr` = jo copies leader ke saath chal rahi hain —
sirf yahi leader ban sakti hain.

### 2. 9 dabbe — BINA key

```
seq 1 9 | sed 's/^/order-/' | kafka-console-producer.sh --topic orders
kafka-get-offsets.sh --topic orders
   orders:0:0
   orders:1:0
   orders:2:9        <- SAB ek hi almari me
```
★ key na ho to Kafka round-robin NAHI karta — **sticky** hai: ek partition chun ke batch bharne tak
wahin daalta rehta (network ke liye sasta). Purane Kafka me round-robin tha, 2.4 se ye.

### 3. 9 dabbe — key ke SAATH

```
printf 'u1:order-1\nu2:order-2\n...' > /tmp/o.txt
kafka-console-producer.sh --topic orders --property parse.key=true --property key.separator=: < /tmp/o.txt
kafka-get-offsets.sh --topic orders
   orders:0:3
   orders:1:3
   orders:2:12       <- 9 purane + 3 naye
```
Naye 9 gaye **3-3-3**. Niyam: `hash(key) % partitions`.
★ Isliye `u1` ke saare message HAMESHA usi almari me -> **ek customer ka KRAM bana rehta hai.**

```
almari 0 :  [x][x][x]
almari 1 :  [x][x][x]
almari 2 :  [o1][o2]...[o9][x][x][x]        <- naye dabbe 9,10,11 number pe
```

### 4. Pehli baar padha — group `billing`

```
kafka-console-consumer.sh --topic orders --from-beginning --group billing --timeout-ms 8000
   order-2, order-4, order-9, order-1, order-3, order-6, order-1, order-2, ...
   Processed a total of 18 messages
```
Kram BIKHRA hua aaya — consumer teeno almari se saath-saath kheenchta hai.
★ **Poore topic me kram hota hi nahi. Kram sirf EK PARTITION ke andar pakka hai.**
(wo purane 9 ek saath sahi kram me dikhe — kyunki wo ek hi almari me the)

`TimeoutException` = error nahi; 8 sec naya message na aane pe consumer band ho gaya.

### 5. Parchi dekhi

```
kafka-consumer-groups.sh --describe --group billing
   Consumer group 'billing' has no active members.

   PARTITION   CURRENT-OFFSET   LOG-END-OFFSET   LAG
       0             3                3            0
       1             3                3            0
       2            12               12            0
```
```
CURRENT-OFFSET = PARCHI kahan hai        ("main yahan tak padh chuka")
LOG-END-OFFSET = DABBE kahan tak hain    ("itna maal maujood hai")
LAG            = dono ka antar           (production me ISI pe alert lagta hai)
```
`no active members` = consumer band, par **parchi Kafka ke paas rehti hai**
(ek chhupe topic `__consumer_offsets` me). Padhne se dabba MITTA NAHI — Kafka queue nahi, **LOG** hai.
Dabbe tab hatte hain jab retention poori ho, padhe jaane par nahi.

### 6. Wahi command DOBARA — 0 message

```
kafka-console-consumer.sh --topic orders --from-beginning --group billing --timeout-ms 8000
   Processed a total of 0 messages
```
★ `--from-beginning` ka asli matlab: *"agar is group ki PARCHI hai hi nahi, to shuru se."*
```
parchi NAHI  ->  --from-beginning chalta hai  ->  shuru se
parchi HAI   ->  flag CHUP  ->  parchi jeetti hai
```

### 7. Naye group `audit` se padha — wahi 18 wapas

```
kafka-console-consumer.sh --topic orders --from-beginning --group audit --timeout-ms 8000
   ... wahi 18 message, wahi kram
```
★★ **Parchi GROUP ki hoti hai, almari ki nahi:**
```
almari 2 :  [o1][o2]...[o9][x][x][x]
                  ^                ^
            audit ki parchi   billing ki parchi
```
Dabbe ek hi baar disk pe. billing apne hisaab se padhta, audit apne hisaab se.
Purani queue (RabbitMQ/JMS): ek ne uthaya to KHATAM, dusre ko nahi milta.
Kafka: koi uthata hi nahi, sab sirf DEKHTE hain.

→ Isi se ek topic pe kai team baith jaati hain (billing · audit · analytics · fraud-check).
Kal paanchvi team aaye — naya group naam le, poora itihaas mil jaata. Producer me ek line nahi badalti.

### 8. REPLAY — parchi peeche khiskayi

**Pehle DRY-RUN** (kuch badla nahi, sirf pucha "kya karoge"):
```
kafka-consumer-groups.sh --group billing --topic orders --reset-offsets --to-earliest --dry-run
   GROUP     TOPIC    PARTITION   NEW-OFFSET
   billing   orders       0           0
   billing   orders       1           0
   billing   orders       2           0
```

**Phir EXECUTE:**
```
kafka-consumer-groups.sh --group billing --topic orders --reset-offsets --to-earliest --execute
kafka-consumer-groups.sh --describe --group billing

   PARTITION   CURRENT-OFFSET   LOG-END-OFFSET   LAG
       0             0                3            3
       1             0                3            3
       2             0               12           12
```
★ **DABBON ko haath nahi laga** — `LOG-END-OFFSET` waise ka waisa (3/3/12).
Sirf parchi hili. LAG 0 se 18 ho gaya.

★ **Purana 3/3/12 MIT gaya** — parchi par ek hi number hota hai, upar likh diya gaya. Koi undo nahi.

### 9. Dobara padha — 18 wapas, BINA `--from-beginning`

```
kafka-console-consumer.sh --topic orders --group billing --timeout-ms 8000
   ... 18 message
   Processed a total of 18 messages
```
Flag tha hi nahi. Parchi 0 pe thi, isliye shuru se mila. **Faisla hamesha PARCHI ka, flag ka nahi.**

### ★ RESET karte waqt — do aadat

```
1. PEHLE --describe chala ke purane number LIKH LO   -> wahi teri "undo" hai
                                                        (--to-offset 12 se wapas ja sakte ho)
2. --dry-run pehle, --execute baad me
   (Kafka sakht hai: dono me se ek diye bina command chalta hi nahi)
```
★ Reset tabhi chalta hai jab group ka **koi consumer chal na raha ho** — isi liye har baar
`has no active members` dikh raha tha. Chalta hua consumer apna purana offset wapas likh deta.

### KAB REPLAY ZAROORAT PADTA — asli wajah

```
consumer me BUG tha (tax galat jud raha) -> 18 invoice galat ban gaye
   bug theek kar diya, par wo 18 message "padhe hue" hain -> parchi unse AAGE hai
   bina Kafka  : upstream se "2 din ka data dobara bhejo" (wo aksar kar hi nahi sakte)
                 ya ek-baar ka backfill script (nayi jagah, nayi galti)
   Kafka ke saath : parchi peeche karo -> wahi 18 dobara, theek code se
```
Aur jagah: nayi service ko poora itihaas chahiye · downstream DB kharab hua ·
kharab message skip karna (`--shift-by`).

★ **KEEMAT:** replay = wahi kaam **DOBARA**. Code paisa kaat raha ho to paisa do baar katega.
→ consumer ka **IDEMPOTENT** hona zaroori hai (`usercrud` me `IdempotencyController` yahi kaam karta).
Replay usko "achha rehne dete hain" se "zaroori hai" bana deta hai.

### Ek line me (interview me bolne layak)

> *"Kafka queue nahi, LOG hai — aur replay usi ka natija hai. Message padhne se mitta nahi, offset
> sirf ek bookmark hai jo har consumer-group ka alag hota hai. Bookmark peeche karo, poora itihaas
> dobara chal jaata hai. Keemat ye hai ki consumer idempotent hona chahiye."*

---

## ═══ HANDS-ON — KAFKA REBALANCE (usi setup pe, 18-Sep) ═══

> Wahi teen almari (`orders`, 3 partition). Ab kai AADMI padhne aayenge — ek hi group `billing` ke.
> Har consumer background me chalaya: `docker exec -d kafka sh -c "kafka-console-consumer.sh ... &> /tmp/aN.log"`
> Dekhne ka command: `kafka-consumer-groups.sh --describe --group billing` (+ `--members`)

### NIYAM — ek almari, ek hi waqt me, sirf EK aadmi ke paas

Do log ek almari nahi padh sakte — warna dono wahi dabba uthate, kaam DO BAAR hota.

```
1 aadmi  ->  teeno almari usi ke paas
2 aadmi  ->  2 + 1
3 aadmi  ->  1 + 1 + 1
4 aadmi  ->  1 + 1 + 1 + KHAALI BAITHA     <- almari hi teen hain
```
Jab bhi koi AAYE ya JAAYE, almariyan dobara baanti jaati hain = **REBALANCE**.

### Kadam 1 — ek consumer

```
PARTITION   CONSUMER-ID
    0       console-consumer-322c9b18-...
    1       console-consumer-322c9b18-...      <- ek hi ID, teeno jagah
    2       console-consumer-322c9b18-...
```

### Kadam 2 — dusra consumer aaya

```
PARTITION   CONSUMER-ID
    0       322c9b18...      <- pehla
    1       322c9b18...      <- pehla
    2       ec45df46...      <- DUSRA
```
2 + 1. Kuch nahi karna pada — dusre ke ghuste hi Kafka ne pehle se **ek almari cheen li**.

★ Partition 2 ka `CURRENT-OFFSET` abhi bhi 12 tha. **Parchi aadmi ke saath nahi jaati, almari pe lagi
rehti hai.** Naya aadmi wahin se uthata hai jahan purana chhod gaya — isi liye kaam dobara nahi hota.

### Kadam 3 — teesra consumer

```
PARTITION   CONSUMER-ID
    0       322c9b18...
    1       ae0f70a0...      <- teesra
    2       ec45df46...
```
1-1-1. Poora group barabar bant gaya, teeno lane saath chal rahi hain = **horizontal scaling**.

### Kadam 4 — chautha consumer: KHAALI BAITHA

```
kafka-consumer-groups.sh --describe --group billing --members

CONSUMER-ID       #PARTITIONS
0a968ad8...            1
318e8cf2...            0        <- ★ group me hai, par KAAM KUCH NAHI
2077cef2...            1
06088e7a...            1
```

★★ **EK GROUP ME KAAM KARNE WAALON KI HADD = PARTITION KI GINTI.**

3 partition hain to 3 se zyada consumer kabhi kaam nahi karenge. Chautha, paanchva, dasva — sab khaali
baithenge: group me rahenge, memory lenge, heartbeat bhejenge, kaam zero.

→ "Load badh gaya, aur pods scale kar do" — pehle dekho **partition kitne hain**. Partition khatam ho
chuke to pod badhane se kuch nahi hoga.

→ Aur partition badhana aasan nahi: badhane pe `hash(key) % partitions` ka jawab badal jaata hai,
yaani `u1` ka maal kal se DUSRI almari me jayega — **purana kram toot jaata hai**.
Isi liye topic banate waqt partition aage ka soch ke rakhte hain.

### ★ KADAM 5 — ek chalta hua consumer MAARA

```
docker exec kafka sh -c "pkill -o -f ConsoleConsumer"      # -o = sabse purana

kafka-consumer-groups.sh --describe --group billing --members
   0a968ad8...   1
   318e8cf2...   1        <- ★ JO KHAALI BAITHA THA, ab uske paas 1 almari
   2077cef2...   1
   (06088e7a mar gaya, list se gayab)
```

Mare hue consumer ki almari **khaali baithe aadmi ko mil gayi**. Koi alert nahi, koi manual kaam nahi.

**Production me yahi sabse kaam ka hissa hai:**
```
ek pod mara  ->  uske partition apne aap bant gaye  ->  KAAM RUKA NAHI
naya pod aaya ->  rebalance dobara
```

Do cheezein jo ismein chhupi hain:
```
1. PARCHI BACH GAYI — mare hue consumer ke partition ka bookmark wahin tha; naye ne WAHIN SE uthaya,
   shuru se nahi. (isi liye parchi KAFKA me rakhi jaati hai, consumer me nahi)

2. REBALANCE ke waqt thodi der SAB RUK jaata hai — purane assignment chhootte, naye lagte, us beech
   consume nahi hota. Isliye baar-baar rebalance (flapping pods) KHUD ek problem hai.
```

### ★ EK GALTI JO IS DEMO ME HUI (yaad rakhne layak)

Pehli koshish me `--timeout-ms 300000` diya tha = *"5 min naya message na aaye to band ho jao"*.
Pehla consumer 5-6 min purana ho chuka tha, to **wo apne aap mar gaya** jab tak chautha aaya —
aur members me 4 ki jagah 3 dikhe, koi khaali nahi.

→ Number dekh ke seedha natija mat nikalo. Pehle `ps -ef | grep -c ConsoleConsumer` se ginti karke
dekho ki **waqai kitne zinda hain**. Saaf demo ke liye sabko `pkill` karke ek saath naye chalaye,
`--timeout-ms 900000` ke saath — tab sahi tasveer bani.

### Ek line me (interview me bolne layak)

> *"Ek consumer-group me partition ek waqt me ek hi consumer ko milta hai, isliye parallelism ki
> chhat partition-count hai — usse zyada pod sirf khaali baithenge. Consumer aaye ya jaaye,
> Kafka partitions dobara baant deta hai, aur offset partition pe rehta hai (consumer me nahi)
> isliye naya consumer wahin se uthata hai. Keemat ye ki rebalance ke dauraan consume ruk jaata hai."*

---

## RabbitMQ Deep (key concepts)

```
EXCHANGE = receives messages from producer
   Types:
     Direct   → routing key match
     Topic    → wildcard pattern
     Fanout   → broadcast all queues
     Headers  → header attribute match

QUEUE = stores messages until consumer reads

BINDING = exchange → queue routing rule
```

```java
// Spring example
@RabbitListener(queues = "email-queue")
public void process(EmailEvent event) {
    emailService.send(event);
}
```

---

## 4 Hard Problems

### 1. **Message Ordering**
```
Kafka: Order guaranteed only WITHIN a partition
       Cross-partition = no order
       Fix: Use same KEY for related messages → same partition
            (e.g., key=user_id keeps user's events ordered)
```

### 2. **Idempotency** (mandatory!)
```
At-least-once → duplicate possible
Consumer must handle:
   "Process payment for order_123" — comes twice
   Charge twice
   Check if already processed → skip

Pattern:
   Unique message_id + processed_messages table
   Or: idempotent operations (upsert, set status)
```

### 3. **Dead Letter Queue (DLQ)**
```
Message fails repeatedly (e.g., 5 retries) → DLQ
   → Doesn't block main queue
   → Manual investigation
   → Fix root cause, replay

Production essential — bina DLQ = poison messages stuck
```

### 4. **Backpressure**
```
Producers >> Consumers → queue grows ∞
Fixes:
   • Auto-scale consumers (k8s HPA on queue depth)
   • Rate-limit producers
   • Drop messages (priority-based)
   • Bigger broker storage (Kafka holds days)
```

---

## ★★ "QUEUE NE SPIKE ABSORB NAHI KIYA — ULTA BIGAAD DIYA" (20-Sep deep-dive)

> "QUEUE KAB GALAT HAI" wala section upar hai — wo batata hai kab queue LAGANI hi nahi chahiye.
> Ye section alag sawaal hai: **queue sahi jagah lagi hai, chal rahi hai — aur usi ne system maara.**

### 1. Jad ki baat — queue BURST absorb karti hai, LAGATAAR overload nahi

```
aa rahi hain     1,000 msg/s
nikal rahi hain    800 msg/s

har second 200 jama  ->  1 ghante me 7,20,000 pending
```

Queue ne kaam GAAYAB nahi kiya. Usne **"abhi error"** ko **"hamesha late"** me badal diya.
Agar aane ki raftaar nikalne ki raftaar se LAGATAAR zyada hai, to queue sirf ek dheere-dheere
doobta hua system hai.

```
queue TAB kaam karti hai jab spike AATA-JAATA ho
   10 second ka jhatka, jise agle 2 minute me nipta diya jaaye    <- ye absorb hai
   lagataar 25% zyada load                                        <- ye absorb NAHI, ye DOOBNA hai
```

### 2. ★★ BACKLOG NIKALTE WAQT KA ULTA SPIKE (asli jawab)

```
consumer 10 minute down raha   ->  6,00,000 message jama
consumer WAPAS aaya            ->  wo POORI RAFTAAR se kheenchta hai
                               ->  DB pe normal se 10x load
                               ->  DB GIR GAYA
```

**Queue ne spike absorb nahi kiya — usne spike ko JAMA karke rakha aur baad me EK SAATH chhod
diya.** Aur wo laher asli spike se BADI thi.

```
ILAAJ consumer me hai:
   bahar nikalne ki RAFTAAR pe apni limit (consumer-side rate limit)
   backlog ke baad DHEERE-DHEERE raftaar badhao (ramp-up)

   ★ consumer ka kaam "jitna TEZ ho sake" nahi hai — "jitna PEECHE WALA jhel sake" hai
```

### 3. Jo kaam ab BEKAAR ho chuka, wo bhi ho raha hai (MURDA KAAM)

```
user ne request bheji                  ->  queue me padi
30 sec baad user ka sabar khatam       ->  page band / retry maar diya
2 min baad consumer ne purana message  ->  poora kaam kiya
                                       ->  us kaam ka ab koi INTEZAAR hi nahi kar raha
                                       ->  aur retry wala NAYA message bhi queue me hai
```

Aadha kaam murda kaam hai, aur load dugna.

```
ILAAJ:
   message ke saath uski DEADLINE rakho; consumer uthate waqt dekhe —
      "ye 30 sec purana hai -> chhod do"
   queue ko BOUNDED rakho: bhar gayi -> seedha MANA karo
   ★ turant "na" kehna, late jawab dene se BEHTAR hai
```

### 4. Queue ne BACKPRESSURE CHHUPA di

```
BINA queue      server bhara -> client ko TURANT 503 -> wo ruk jaata
                (system ne "na" bola, aur wahi sahi tha)

QUEUE ke saath  sab accept ho raha -> kisi ko mana nahi mil raha
                -> producer POORI raftaar se bhejta rehta
                -> dashboard pe sab HARA (koi error nahi)
                -> sirf ek number chupchaap badh raha hai: QUEUE DEPTH
```

Jab tak koi wo number dekhta hai, ghante ka backlog ban chuka hota hai.

```
★ isi liye asli ALERT  `error rate` pe NAHI —
     QUEUE DEPTH  aur  MESSAGE KI UMAR (oldest message age) pe lagta hai
```

### 5. Consumer AUTOSCALE ne DB ko maar diya

```
queue depth badh rahi   ->  autoscaling ne consumer 5 se 50 kar diye
50 consumer x 20 conn   =   1,000 connection DB pe
DB ka pool 200 ka tha   ->  DB khatam  ->  ab KUCH BHI nahi chal raha
```

Bottleneck queue me tha hi nahi — **DB me tha**. Consumer badhane se sirf DB pe zor badha.

```
ILAAJ: consumer badhane se PEHLE dekho RUKA KAUN hai
       peeche wale ki had ke hisaab se consumer ki MAX ginti baandho
```

### ★ Aur ek — jo seedha TRAFFIC GUNA kar deti hai (fan-out)

```
1 event publish hua
   -> 5 service subscribe hain
      -> har ek apne aage 3 call karti hai
         = 1 request andar, 15 call bahar
```

Producer ko lagta hai usne "ek chhota event" bheja hai.
**Pub-sub me traffic ginti nahi badhata — GUNA karta hai.**

---

### ★ EK HI SHAKAL — poore HLD me ghoomti hai

```
LB me      "BACHANE wali cheez ne maara"                (health check / retry / sticky)
cache me   "TEZ karne wali cheez ne raasta rok diya"    (slow Redis + no timeout)
limiter me "ROKNE wali cheez ne bheed ko roka hi nahi"  (per-user limit vs aggregate load)
queue me   "ABSORB karne wali cheez ne spike JAMA karke baad me BADA karke chhoda"
```

Poora dhaancha: `03_load_balancing.md` ka "CHHE TARIKE" · `04_caching.md` ka "REDIS LAGATE HI
SERVER DOWN" · `SYSTEM_DESIGNS/02_rate_limiter` ka "LIMITER LAGA THA PHIR BHI NAHI BACHA".

---

## Real-World Tools

| Tool | Type | Use |
|---|---|---|
| **Kafka** | Log-based | LinkedIn, Uber, Netflix — event streaming |
| **RabbitMQ** | Broker | Traditional Java/Spring apps |
| **AWS SQS** | Managed queue | Simple async tasks |
| **AWS Kinesis** | Managed Kafka-like | AWS event streaming |
| **Google Pub/Sub** | Managed | GCP pub-sub |
| **Redis Streams** | Lightweight | Smaller scale, simple |
| **ActiveMQ** | Broker | Older Java enterprise |

---

## Interview Talking Points

**Q: "MQ vs REST kab?"**

> *"REST sync — caller waits, tightly coupled. MQ async — decouple, buffer spikes, retry on failure. Heavy/slow/non-critical work → MQ. Real-time response → REST. Modern microservices: events for state changes, REST for queries."*

**Q: "Kafka vs RabbitMQ?"**

> *"Kafka = distributed append-only log, millions/sec, persistent, replay support — event streaming. RabbitMQ = traditional broker, complex routing (exchanges), push-based, task queues. Big scale + replay → Kafka. Complex routing + small/medium → RabbitMQ."*

**Q: "Exactly-once delivery kaise?"**

> *"Hard. Practical: **at-least-once + idempotent consumer** (unique message_id, dedup table). Effectively exactly-once at lower cost. Strict exactly-once needs distributed transactions (2PC) — slow, brittle, mostly avoid."*

**Q: "DLQ kya?"**

> *"Dead Letter Queue — messages failing repeatedly (5 retries) shifted here. Main queue se stuck poison messages hatate. Manual investigation + fix + replay. Production mandatory."*

**Q: "Kafka mein ordering?"**

> *"Within partition guaranteed (append-only log). Cross-partition no order. Trick: same key → same partition. user_id key → us user ke saare events ordered. Critical for sequence-dependent flows."*

---

## Power Phrase

> **"MQ = async + decouple + buffer + reliability. Producer publishes, consumer processes — independently. Patterns: P2P (queue) vs Pub-Sub (topic). Delivery: at-least-once + idempotent = practical exactly-once. Kafka (log, millions/sec, replay) vs RabbitMQ (broker, routing). Hard problems: ordering, idempotency, DLQ, backpressure."**

---

## Memory Hook

```
MQ = "Restaurant order slips"
   • Waiter (producer) → board (queue) → chef (consumer)
   • Decouple, async, buffer, reliable

Patterns:
   Point-to-Point (Queue) → 1 message, 1 consumer
   Pub-Sub (Topic)        → 1 message, all subscribers

Delivery:
   At-most-once   → fast, lossy (metrics)
   At-least-once  → most common (idempotent consumer)
   Exactly-once   → slowest, banking

Kafka:
   Topic → Partitions → Offset
   Same key → same partition = order
   Consumer group → parallelism
   Replay support

RabbitMQ:
   Exchange → Binding → Queue
   Direct/Topic/Fanout/Headers
   Push-based, complex routing

Hard problems:
   1. Ordering   — partition key trick
   2. Idempotency — unique msg_id + dedup
   3. DLQ        — poison message escape
   4. Backpressure — auto-scale consumers
```

---

## Trap Box

```
Trap 1: "MQ = always better than REST"
         Real-time queries (get user) → REST simpler
         Async/heavy/spike-prone work → MQ

Trap 2: "Exactly-once is achievable easily"
         Distributed exactly-once = hard problem
         At-least-once + idempotent consumer

Trap 3: "Kafka order guaranteed always"
         Only within partition
         Use partition key for related messages

Trap 4: "No DLQ needed"
         Poison messages stuck retry forever
         DLQ mandatory in production

Trap 5: "Consumer faster than producer always"
         Spikes → backpressure → infinite queue
         Auto-scale consumers, monitor queue depth

Trap 6: "Kafka and RabbitMQ same thing"
         Different paradigms (log vs broker)
         Kafka = streaming, RabbitMQ = task queues

Trap 7: "Async = lost reliability"
         MQ persistence + retry = MORE reliable than REST
         MQ improves reliability with right config
```
