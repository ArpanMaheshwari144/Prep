# Spring Kafka (code me kaise chalta, usercrud me chalaya)

Kafka andar se kaise chalta (topic, partition, group, commit, copy, outbox, disk pe register) = [kafka_concepts.md](../../04_HLD/SYSTEM_DESIGNS/12_message_queue_kafka/kafka_concepts.md). HLD wali kahani (stadium) = [12_message_queue_kafka](../../04_HLD/SYSTEM_DESIGNS/12_message_queue_kafka/12_message_queue_kafka.md).
Yahan sirf Spring wala hissa hai: tere usercrud me Kafka code me kaise chalta hai.

## Scene: order hua, teeno ko batana hai

Order hote hi SMS, Email aur Fraud, teeno ko batana hai. Agar Order Service khud ek-ek ko call kare, to teen dikkat aati hain. SMS 3 sec le
to order bhi 3 sec ka ho jaata hai. SMS service giri to order bhi fail ho jaata hai. Aur kal Fraud jodna ho to Order Service ka code badalna
padta hai. Isliye beech me Kafka rakhte hain: Order Service `kafkaTemplate.send(...)` karke aage badh jaati hai, aur `@KafkaListener` wale
apni speed se khud padh lete hain.

```
PRODUCER:  kafkaTemplate.send("user-events", key, message)   -> daala aur bhool gaya
CONSUMER:  @KafkaListener(topics="user-events", groupId="usercrud-group")
           -> Spring andar loop chala ke khud poochta (pull), message aaya to method chalta
FAIL:      DefaultErrorHandler(recoverer, FixedBackOff(1000, 2))
           -> 3 try, phir "user-events-dlt" me park, main line aage
DUPLICATE: processedIds -> pehle dekha? skip : kaam karo, phir id rakh lo
```

Ek kharab message baar-baar fail ho to uske peeche ki poori line atak jaati hai (poison message). Isliye `DefaultErrorHandler` 3 baar try
karta hai, phir use "user-events-dlt" me park kar deta hai taaki main line aage badhe. Ek consumer kam pade to
`spring.kafka.listener.concurrency=3` lagao aur topic me 3 partition rakho, to 3 thread parallel chalte hain. Ek group me ek partition ko
ek hi consumer padhta hai, isliye duplicate nahi hota.

## Chala ke dekha: key se partition (Kafka ka asli murmur2 hash)

```
key user-7  -> P2
key user-7  -> P2
key user-7  -> P2      <- ek user ke saare message ek hi partition, kram bana rehta
key user-12 -> P0
key user-31 -> P0
```

Key do to message `hash(key) % 3` se partition me jaata hai, aur ek key ke saare message ek hi partition me, kram me. Bina key ke ye nahi
hota: sticky partitioner poora burst ek hi partition me daal deta hai, isliye 3 thread lagaye ho tab bhi ek hi chalta hai. Ye usercrud me
live pakda tha:

```java
kafkaTemplate.send(TOPIC, message);            // bina key: 6 message sab partition 0, ek hi thread
kafkaTemplate.send(TOPIC, message, message);   // key ke saath: P0, P1, P2 me bante, 3 thread
```

## Chhoti dikkat: kaam ho gaya, commit se pehle crash

Spring offset ko kaam khatam hone ke BAAD commit karta hai. Kaam aur commit ke beech crash hua to wahi message dobara aayega:

```
run1 off1 SMS bheja SMS-order102
run1 CRASH commit se pehle (offset 1 pe ruka)
run2 off1 m1 pehle dekha -> skip            <- processedIds ne bacha liya
```

Message khoya nahi, par duplicate ho sakta hai (at-least-once). Hal: message id dekh ke dobara aaya message chhod do (idempotent consumer).
Ye wahi payment wali idempotency key hai, bas jagah alag hai. Agar offset pehle commit karte to crash pe message kho jaata (at-most-once),
jo aur bura hai.

## Tera project (usercrud)

`KafkaProducerController` hai (`/kafka/send`), `KafkaConsumer` hai (jisme `@KafkaListener`, DLT listener aur processedIds hain), aur
`KafkaConfig` hai (jisme sirf `DefaultErrorHandler` aur `NewTopic` hain). DLT topic ke partition utne hi rakho jitne main topic ke hain,
kyunki recoverer message ko usi partition number pe bhejta hai.

Boot 4 me raw `spring-kafka` lagayi to `KafkaTemplate` bana hi nahi, aur 60 line manual config likhni padi. Jad ye thi ki autoconfig
`spring-boot-starter-kafka` ke saath aata hai. Starter lagaya to sirf 2 bean bache jo khud likhne pade: error handler aur DLT topic.

## Prod wala trap: processedIds memory me

```
--- process restart, memory wala set bhi gaya ---
run3 off1 SMS bheja SMS-order102            <- customer ko SMS dobara
```

usercrud me `processedIds` ek `HashSet` hai, jo memory me rehta hai. App restart hua to set bhi chala gaya, aur commit se pehle wala message
phir aaya to SMS dobara chala gaya. Demo ke liye set theek hai; asli me ye id DB ya Redis me jaati hai, unique key ke saath. Pod restart aur
deploy roz hote hain, isliye ye roz hit karega.
