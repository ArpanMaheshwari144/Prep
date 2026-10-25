# Kafka ya RabbitMQ: farak aur kab kaunsa

Saath: [Kafka design](12_message_queue_kafka.md) · [kafka_concepts](kafka_concepts.md) · kahaniyan:
[Kafka kaise bana](../../KAHANIYAN/kafka.md) · [RabbitMQ kaise bana](../../KAHANIYAN/rabbitmq.md)

---

## 1. Farak, ek misaal se

```
RabbitMQ = daakghar
  chitthi aayi -> sahi dabbe (queue) me gayi -> ek worker ne uthayi, "mil gaya" (ack) bola -> chitthi khatam
  achha: kaam baantna (ek kaam ek worker), routing ke niyam (ye wali yahan, wo wahan)

Kafka = register (bahi-khata)
  har entry bas likhti jaati hai, padhne se mit-ti nahi, kuch din rehti hai
  achha: ek event ko kai log padhein, apni raftaar se, chahe to shuru se dobara; bahut bada data
```

**Kab kaunsa:**
- **RabbitMQ:** jab ek kaam ek baar ek worker se karwana hai, jaise email bhejna ya report banana, aur message
  ke hisaab se raasta badalna hai.
- **Kafka:** jab ek hi event kai jagah chahiye (search, notification, analytics), dobara padhna pad sakta hai,
  aur data bahut bada hai.

Isliye hamare designs me Kafka aaya: twitter, notification aur news, teeno me ek khabar kai jagah jaati hai, ya
bheed me line chahiye.

---

## 2. LinkedIn ne RabbitMQ kyun nahi liya, seedha Kafka kyun banaya

Unhone pehle purani queues try ki thi, aur wo is kaam ke liye chhoti padi. Kafka ke banane walon ne 2011 me jo
paper likha tha, usme ActiveMQ aur RabbitMQ dono se tulna ki hai. Wajah teen thi:

**1. Bahut zyada data.** LinkedIn pe roz arbon events the. RabbitMQ har message ka alag hisaab rakhta hai: kise
diya, usne "mil gaya" (ack) bola ya nahi. Itne messages pe ye hisaab hi bhaari pad gaya. Kafka messages ko jhund
me likhta hai, aur disk pe seedha aage-aage, isliye bahut tez hai.

**2. Padhte hi message mit jaata tha.** RabbitMQ me message tab tak rehta hai jab tak koi use utha na le. LinkedIn
ko chahiye tha ki search abhi padhe, aur Hadoop raat ko wahi saara data padhe. Ek naya system aaye to wo bhi
purana data padh sake. Kafka me message kuch din pada rehta hai, aur jo chahe, jab chahe padh leta hai.

**3. Hisaab kiske paas rahe.** RabbitMQ me broker yaad rakhta hai ki har padhne wale ne kahan tak padha. Padhne
wale badhe, to broker ka bojh bhi badha. Kafka me har padhne wala apna bookmark (offset) khud rakhta hai, broker
bas register sambhalta hai. Isliye 10 naye padhne wale aa jaayein, to bhi broker pe farak nahi padta.

```
LinkedIn ko chahiye tha:  bahut data + kai padhne wale + dobara padhna + raat ko Hadoop
RabbitMQ diya:            ek message ek baar, padhte hi khatam, broker sabka hisaab rakhe
=> kaam ka mel nahi baitha, to apna bana liya
```

Iska matlab ye nahi ki RabbitMQ kharab hai. Wo daakghar wale kaam me aaj bhi badhiya hai: kaam baantna, ek
chitthi ek baar. LinkedIn ki dikkat register wali thi, daakghar wali nahi. Interview me "Kafka ya RabbitMQ?" ka
jawab yahi farak hai.
