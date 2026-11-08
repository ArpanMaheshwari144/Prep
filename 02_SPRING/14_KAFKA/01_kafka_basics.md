# Kafka (Spring, usercrud me chalaya)

HLD wali kahani (stadium, admin ki line, gate matlab partition, kamra matlab consumer group) 04_HLD/SYSTEM_DESIGNS/12_message_queue_kafka me hai.
Yahan sirf Spring wala hissa hai: tere usercrud me Kafka code me kaise chalta hai.

## Kahani: Order Service aur SMS

Order hote hi SMS, Email aur Fraud, sabko batana hai. Agar Order Service khud ek-ek ko call kare, to teen dikkat aati hain. SMS 3 sec le to order bhi 3
sec ka ho jaata hai. SMS service gir gayi to order bhi fail ho jaata hai. Aur kal Fraud jodna ho to Order Service ka code badalna padta hai. Isliye beech me Kafka rakhte hain: Order Service event
daal ke aage badh jaati hai, aur har service apni speed se khud padh leti hai.

## Tasveer (Spring me kaise)

```
PRODUCER:  kafkaTemplate.send("user-events", key, message)   -> daala aur bhool gaya
CONSUMER:  @KafkaListener(topics="user-events", groupId="usercrud-group")
           -> Spring andar loop chala ke khud poochta (pull), message aaya to method chalta
FAIL:      DefaultErrorHandler(recoverer, FixedBackOff(1000, 2))
           -> 3 try, phir "user-events-dlt" me park, main line aage
DUPLICATE: processedIds set -> pehle dekha? skip : kaam karo, phir set me daalo
```

Producer bas `kafkaTemplate.send` karta hai aur bhool jaata hai. Consumer pe `@KafkaListener` laga hai, aur Spring andar ek loop chala ke khud Kafka se poochta rehta hai (pull), message aaya to method chal jaata hai. Fail hone pe `DefaultErrorHandler` 3 baar try karta hai, phir message ko "user-events-dlt" me park kar deta hai taaki main line aage badhe. Duplicate ke liye `processedIds` set hai: message pehle dekha hai to skip, warna kaam karo aur phir id set me daal do.

## Dikkat aur ye kyun

- **Seedhi call:** isme kaam dheema hota hai, ek service giri to sab gir jaate hain, aur sab ek-doosre se jakde rehte hain. Kafka beech me ho to bhejne wala aur sunne wala ek-doosre pe tike nahi rehte.
- **Poison message:** ek kharab message baar-baar fail hota hai, aur uske peeche ki poori line atak jaati hai. DLQ iska hal hai: kuch baar try karo, phir use alag topic me park kar do.
- **Ek consumer kam pada:** `spring.kafka.listener.concurrency=3` lagao aur topic me 3 partition rakho, to 3 thread parallel chalte hain.
  Ek group me ek partition ko ek hi consumer padhta hai, isliye duplicate nahi hota.

## Trade-off

Kafka at-least-once hai. Spring offset ko kaam khatam hone ke BAAD commit karta hai. Agar kaam ke beech me crash ho gaya, to wahi message dobara aayega. Message khoya nahi, par
duplicate ho sakta hai. Isliye consumer ko idempotent banate hain: message id dekh ke dobara aaya message chhod do. Ye wahi payment wali
idempotency key hai, bas jagah alag hai. Agar offset pehle commit karte, to crash pe message kho jaata, jise at-most-once kehte hain, aur wo bura hai.

## Asli kaam wala trap (live pakda)

```java
kafkaTemplate.send(TOPIC, message);            // bina key: 6 message sab partition 0, ek hi thread
kafkaTemplate.send(TOPIC, message, message);   // key ke saath: P0, P1, P2 me bante, 3 thread
```

Bina key ke Kafka ek partition pakad leta hai aur poora burst usi me daal deta hai (sticky partitioner), isliye sirf ek thread kaam karta hai. Key do to message hash(key) % 3 se teeno partition me bant jaate hain,
aur ek key ke messages ka kram bhi bana rehta hai.

Doosra asli kissa: Boot 4 me raw `spring-kafka` lagayi to `KafkaTemplate` bana hi nahi, aur 60 line manual config likhni
padi. Jad ye thi ki autoconfig `spring-boot-starter-kafka` ke saath aata hai. Starter lagaya to sirf 2 bean bache jo khud likhne pade: error handler aur DLT topic.

## Tera project (usercrud)

`KafkaProducerController` hai (`/kafka/send`), `KafkaConsumer` hai (jisme `@KafkaListener`, DLT listener aur processedIds hain), aur
`KafkaConfig` hai (jisme sirf `DefaultErrorHandler` aur `NewTopic` hain). DLT topic ke partition utne hi rakho jitne main topic ke hain,
kyunki recoverer message ko usi partition number pe bhejta hai.

## Andar se: message disk pe kaise rakha jaata hai (padhai, design me nahi poochte)

Ye wahi kahani hai jo bank aur Twitter me pehle aa chuki hai. Stadium ke gate pe admin ek register me har aane wale ka
naam likhta hai. Wo beech ke panne pe nahi likhta, aur purana naam kaat ke dobara nahi likhta. Bas aakhri line ke neeche
agla naam jodta jaata hai. Kafka bhi disk pe yahi karta hai: har partition ek register hai, aur naya message hamesha
uske aakhir me judta hai. Beech me jagah dhoond ke likhna dheema hota hai, aakhir me jodte jaana sabse tez, isliye
Kafka itna tez likh paata hai.

Register bhar jaaye to admin purana band karke almari me rakh deta hai aur naya register khol leta hai. Har register ke
cover pe likha hota hai ki wo kis number se shuru hua: "0 se", "500 se", "1000 se". 7 din purana maal hatana ho to Kafka
panne kaat-kaat ke nahi hatata, jo poora register 7 din se purana ho use seedha almari se phenk deta hai.

Offset 501 maanga to do kadam: pehle covers dekho, 501 "500 se" wale register me hai (covers kram me lage hain, to
binary search). Phir us register ke saath lagi chhoti parchi dekho, jaise kitaab ki vishay-suchi: "500 -> panna 1,
600 -> panna 4". Parchi har number ke liye nahi likhti, beech-beech ke, taaki chhoti rahe. Panna 1 pe jao, do-chaar
line aage padho, 501 mil gaya.

**Asli naam:** aakhir me jodo = append-only log, ek register = segment (`.log` file), poora register phenkna =
retention, chhoti parchi = `.index` (offset se jagah, beech-beech ki entry = sparse index).

**Pehle kahan bola:** kaato mat neeche likho = Banking 5 (ledger) · mahine ke mahine tod do = Banking 12 · purana
head office = Banking 13 · ID se seedha page, 1 lakh panne nahi palatne = Twitter 16.
