# Kafka concepts (tasveer se)

> Ek line: main kaam ho gaya, ab khabar jahan-jahan chahiye wahan peeche se pahunchti rahe, aur main kaam kisi ka intezaar na kare.
> Kafka khud SMS nahi bhejta: wo khabar rakhta aur pahunchata hai, kaam padhne wale karte hain.

---

## 1. Topic = naam wali diary

<img src="img/k1_topic.png?v=2" width="1000" alt="topic">

```
"A topic is a named stream of events. The order service writes to order-events and moves on.
 Any service that cares subscribes to it, so adding a new consumer needs no change on the producer.
 Unlike a queue, reading doesn't delete the message: it's retained, so many services can read the same event."
```

---

## 2. Partition = topic ke hisse

<img src="img/k2_partition.png?v=2" width="1000" alt="partition">

```
"A topic is split into partitions so the load spreads across brokers and many consumers can read in parallel.
 Ordering is guaranteed only within a partition, so I use a key like userId: same key, same partition,
 so one user's events stay in order. The number of partitions caps how many consumers in a group can work in parallel."
```

---

## 3. Consumer group = ek service ki team

<img src="img/k3_group.png?v=2" width="1000" alt="consumer group">

```
"Each service is a consumer group. Inside a group, each partition goes to exactly one consumer,
 so the work is split and nothing is processed twice. Different groups each get the full stream
 with their own offsets, so SMS and Fraud read the same events independently. If a consumer dies,
 Kafka rebalances its partitions to the others."
```

---

## 4. Offset + commit = banda kahan tak padh chuka

**Pichhli:** tasveer 3 me Arpan ki khabar (P1) sirf group "sms" ke banda-B ke paas jaati hai, ek hi SMS.
**Ab:** banda-B ne SMS bheja, aur "kahan tak padha" aage likhne se pehle gir gaya. Khabar khoti ya dobara aayi?

```
 P1 (Arpan):   [0 order] [1 pay] [2 cancel]
                            ^
                 group "sms" ka bookmark (offset): 1 tak padha

 banda-B:  1. agli khabar uthai (2 cancel)
           2. Arpan ko SMS bheja
           3. Kafka ko bola "2 tak padh liya"   <- isko COMMIT kehte hain, bookmark 2 pe
```

Kya-kya ho sakta hai:
```
gaadi kharab = SMS bheja, COMMIT se pehle banda-B gira (bookmark abhi 1 pe)
  -> nateeja = P1 banda-A ko mila (rebalance), wo 1 ke baad se padhta: "2 cancel" DOBARA
               -> Arpan ko 2 SMS
  -> bus     = har khabar ki apni id (eventId). SMS se pehle dekho "ye id pehle ho chuki?"
               (Redis / DB me likhi). Haan to chhod do

ulta kram    = pehle COMMIT, phir SMS. COMMIT ke baad, SMS se pehle gira
  -> nateeja = banda-A 3 se padhta. "2 cancel" ka SMS KABHI nahi gaya. Ye 2 SMS se bhi bura
```

Teen baatein:
- Kram: pehle kaam (SMS), phir COMMIT. Isse khabar kabhi khoti nahi.
- Khabar dobara aa sakti hai (at-least-once). Use eventId se pakdo.
- Ye pehchaanna padhne wale (SMS Svc) ka kaam hai, Kafka ka nahi.

```
"I commit the offset only after the work is done, so a crash never loses a message, it can only
 redeliver it. That makes it at-least-once, so the consumer must be idempotent: every event has
 an eventId, and before sending I check whether that id was already processed."
```

---

## 5. Broker gira = copy, leader, acks

**Pichhli:** upar 4 me padhne wali taraf pakki: pehle kaam, phir commit, eventId se pehchaan.
**Ab:** P1 (Arpan ki teeno khabar) machine B pe hai. Machine B ki disk hi mar gayi to?

```
 [ Order Svc ] --khabar--> [ machine B: P1 LEADER ]   likhna-padhna yahin
                                 |-- copy --> [ machine A: P1 copy ]
                                 |-- copy --> [ machine C: P1 copy ]

 machine B mari -> A ya C naya leader, Arpan ki teeno khabar bachi
```

Kya-kya ho sakta hai:
```
gaadi kharab = B ne Order Svc ko "ho gaya" bola, copy banne se PEHLE mar gaya
  -> nateeja = khabar sirf B pe thi, gayi
  -> bus     = "ho gaya" tabhi bolo jab copy bhi likh jaaye (acks=all)

copy peeche = machine C ki copy kuch khabar peeche chal rahi
  -> raasta  = sirf poori taaza copy "pakki" maani jaati (ISR). Naya leader pakki me se hi banta

copy bhi gayi = sirf leader bacha, koi pakki copy nahi
  -> raasta   = likhna rok do (min.insync.replicas = 2). Khabar khone se behtar thodi der rukna
```

Teen baatein:
- Har hissa (partition) 3 machine pe: ek leader, do copy.
- acks=all: copy pe likh jaaye tab "ho gaya". acks=1 sirf leader (click, log ke liye theek).
- Leader gira to pakki copy (ISR) me se naya leader. Ye faisla Controller karta hai (Kafka ki ek machine jo baaki pe nazar rakhti).

```
"Each partition is replicated to three brokers: one leader and two followers. With acks=all the
 producer gets a success only after all in-sync replicas have the message, so if the leader dies,
 a follower from the ISR takes over with nothing lost. With min.insync.replicas=2, if too few
 replicas are alive, writes fail instead of silently losing data."
```

---

## 6. Kafka tak pahunchne se pehle app gira = outbox

**Pichhli:** upar 5 me Kafka ki taraf pakki: 3 copy, acks=all.
**Ab:** bacha bhejne wala. Order Svc ne Arpan ka "cancel" DB me likha, Kafka ko bhejne se pehle gir gaya to?

```
 [ Order Svc ] --1. DB me "cancel"--> [ DB ]
               --2. Kafka ko khabar--> [ Kafka ]
                 ^
          yahan gira: order cancel ho gaya, par SMS / Fraud ko kabhi pata nahi chala
```

Ilaaj: khabar bhi usi DB me, cancel ke saath, EK transaction me likho (outbox table).
Ek chhota relay har second outbox padh ke Kafka ko bhejta hai.

```
 [ Order Svc ] --ek transaction--> [ DB: order = cancel + outbox E-6 (sent = false) ]
                                              |
                                   [ Relay ] padhta --> [ Kafka ] --OK--> sent = true
```

Kya-kya ho sakta hai:
```
gaadi kharab = transaction se pehle gira
  -> nateeja = na cancel likha, na khabar. Arpan dobara cancel dabayega. Galat kuch nahi

bus late     = relay ne Kafka ko bheja, "sent = true" likhne se pehle gira
  -> nateeja = relay uthke E-6 dobara bhejega. SMS Svc eventId se pakad lega (upar 4)

jagah bhari  = outbox table badhti ja rahi
  -> raasta  = sent = true wali row roz saaf karo
```

Teen baatein:
- DB aur Kafka ek transaction me nahi aate. Isliye khabar DB me hi, ek transaction me.
- Relay alag se bhejta hai. Dobara bhej sakta hai, eventId bachata hai.
- Saga nahi: saga kiya hua kaam ulta karta hai. Yahan khabar bachani hai.

```
"I can't write to the database and Kafka in one transaction, so I use the outbox pattern: the
 business change and the event row go into the same DB transaction. A relay publishes pending
 outbox rows to Kafka and marks them sent. A crash at any point either loses nothing or causes a
 duplicate, and consumers already dedupe on eventId."
```

---

Poora bachav ek line me: **outbox (bhejne wala) + acks=all (Kafka) + pehle kaam phir commit + eventId (padhne wala)**.
Baaki (lag, DLQ, retention) = [kafka_seedha.md](kafka_seedha.md) Kadam 10.
