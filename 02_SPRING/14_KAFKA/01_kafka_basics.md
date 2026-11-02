# Kafka (Spring, usercrud me chalaya)

HLD wali kahani (stadium, admin ki line, gate = partition, kamra = consumer group) = 04_HLD/SYSTEM_DESIGNS/12_message_queue_kafka.
Yahan sirf Spring wala hissa: tere usercrud me code me kaise chalta.

## Kahani: Order Service aur SMS

Order hote hi SMS, Email, Fraud sabko batana. Order Service khud ek-ek ko call kare to: SMS 3 sec le to order bhi 3
sec ka · SMS gira to order fail · kal Fraud jodna = Order Service ka code badlo. Beech me Kafka: Order Service event
daal ke aage, har service apni speed se khud padhti.

## Tasveer (Spring me kaise)

```
PRODUCER:  kafkaTemplate.send("user-events", key, message)   -> daala aur bhool gaya
CONSUMER:  @KafkaListener(topics="user-events", groupId="usercrud-group")
           -> Spring andar loop chala ke khud poochta (pull), message aaya to method chalta
FAIL:      DefaultErrorHandler(recoverer, FixedBackOff(1000, 2))
           -> 3 try, phir "user-events-dlt" me park, main line aage
DUPLICATE: processedIds set -> pehle dekha? skip : kaam karo, phir set me daalo
```

## Dikkat -> kyun

- **Seedhi call:** dheema + ek gira sab gira + jakda hua. Kafka beech me = bhejne wala aur sunne wala ek-doosre pe tike nahi.
- **Poison message:** ek kharab message baar-baar fail, peeche ki line atki. DLQ: kuch try, phir alag topic me park.
- **Ek consumer kam pada:** `spring.kafka.listener.concurrency=3` + topic ke 3 partition = 3 thread parallel.
  Ek partition = group me ek hi consumer, isliye duplicate nahi.

## Trade-off

Kafka = at-least-once. Spring offset kaam ke BAAD commit karta: kaam ke beech crash -> wahi message dobara (khoya
nahi, duplicate ho sakta). Isliye consumer idempotent: message id dekh ke dobara wala chhodo. Payment wali
idempotency key hi, bas jagah alag. (Pehle commit karte to crash pe message kho jaata = at-most-once, bura.)

## Asli kaam wala trap (live pakda)

```java
kafkaTemplate.send(TOPIC, message);            // bina key: 6 message sab partition 0, ek hi thread
kafkaTemplate.send(TOPIC, message, message);   // key ke saath: P0, P1, P2 me bante, 3 thread
```

Bina key Kafka ek partition pakad ke poora burst wahi daalta (sticky partitioner). Key do to hash(key) % 3 se bante,
aur ek key ka kram bhi bana rehta.

Doosra asli kissa: Boot 4 me raw `spring-kafka` lagayi to `KafkaTemplate` bana hi nahi, 60 line manual config likhni
padi. Jad: autoconfig `spring-boot-starter-kafka` me aata hai. Starter lagaya -> sirf 2 bean bache (error handler + DLT topic).

## Tera project (usercrud)

`KafkaProducerController` (`/kafka/send`), `KafkaConsumer` (`@KafkaListener` + DLT listener + processedIds),
`KafkaConfig` (sirf `DefaultErrorHandler` + `NewTopic`). DLT ke partition utne hi rakho jitne main topic ke
(recoverer usi partition number pe bhejta).

## Interview line

```
In usercrud the producer just calls kafkaTemplate.send with a key, and a @KafkaListener consumes it.
Kafka is at-least-once because the offset is committed after processing, so the consumer is idempotent:
it skips message IDs it has already handled. Failures retry three times with a backoff and then go to a
dead-letter topic, so one bad message never blocks the partition.
```
