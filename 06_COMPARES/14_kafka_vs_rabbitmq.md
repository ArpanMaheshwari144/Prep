# Kafka vs RabbitMQ — register ya daakghar

---

## Koi poochhe to aise bolo

**Kafka** = register: har event likhta jaata, padhne se mitta nahi, kuch din rehta. Har padhne wala apni jagah (offset) yaad
rakhta, isliye ek hi event kai jagah (search, notification, analytics) apni-apni raftaar se; galti ho to shuru se dobara padh lo;
bahut badi bheed bhi jhel leta. **RabbitMQ** = daakghar: chitthi aayi, sahi dabbe (queue) me gayi, ek worker ne uthayi, "mil
gaya" (ack) bola, chitthi khatam; ek kaam ek baar ek worker se, aur message ke hisaab se raasta badalna (routing) ho to achha.
Niyam: ek event kai jagah / dobara padhna / bahut data = Kafka; ek kaam ek baar + routing ke niyam = RabbitMQ.

---

## Kahani

```
Kafka    = stadium ka register / line: sab likha rehta, har gate (consumer group) apni jagah se padhe, dobara bhi
RabbitMQ = daakghar: chitthi pahunchi, "mil gaya" bola, chitthi khatam
```

---

## Tere design aur project se

- **Twitter, notification, news:** ek post / order ka event kai jagah (feed, search, SMS), isliye Kafka.
- **mini_payment_ms:** payment event Kafka me, notification-service ne padha, event log me pada raha.
- **Email / report banana:** ek kaam ek baar, yahan RabbitMQ bhi chal jaata.
- Poora farak = `04_HLD/SYSTEM_DESIGNS/12_message_queue_kafka/kafka_ya_rabbitmq_kab_kaunsa.md`.

---

## Dikkat se hal

Notification bug se 2 ghante galat SMS gaye. Kafka me event pade hain: bug theek karo, offset peeche le jao, dobara padh lo.
RabbitMQ me ack ke baad chitthi mit chuki, dobara padhne ko kuch nahi.

---

## Trap

Kafka me 10 consumer laga diye, par partition 3 hi the: 7 consumer khaali baithe. Ek consumer group me jitne partition, utne hi
consumer kaam karte; pehle partition badhao.
