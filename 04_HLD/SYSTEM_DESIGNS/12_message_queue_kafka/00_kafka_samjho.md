# Kafka samjho (tasveer se)

> Ek line: main kaam ho gaya, ab khabar jahan-jahan chahiye wahan peeche se pahunchti rahe, aur main kaam kisi ka intezaar na kare.
> Kafka khud SMS nahi bhejta: wo khabar rakhta aur pahunchata hai, kaam padhne wale karte hain.

---

## 1. Topic = naam wali diary

<img src="img/k1_topic.png?v=1" width="1000" alt="topic">

```
"A topic is a named stream of events. The order service writes to order-events and moves on.
 Any service that cares subscribes to it, so adding a new consumer needs no change on the producer.
 Unlike a queue, reading doesn't delete the message: it's retained, so many services can read the same event."
```

---

## 2. Partition = topic ke hisse

<img src="img/k2_partition.png?v=1" width="1000" alt="partition">

```
"A topic is split into partitions so the load spreads across brokers and many consumers can read in parallel.
 Ordering is guaranteed only within a partition, so I use a key like userId: same key, same partition,
 so one user's events stay in order. The number of partitions caps how many consumers in a group can work in parallel."
```

---

## 3. Consumer group = ek service ki team

<img src="img/k3_group.png?v=1" width="1000" alt="consumer group">

```
"Each service is a consumer group. Inside a group, each partition goes to exactly one consumer,
 so the work is split and nothing is processed twice. Different groups each get the full stream
 with their own offsets, so SMS and Fraud read the same events independently. If a consumer dies,
 Kafka rebalances its partitions to the others."
```

---

Agla (abhi baaki): 4 offset + commit (khoti ya dobara?) · 5 broker gira (copy, leader, acks) · 6 Kafka se pehle app gira (outbox)
