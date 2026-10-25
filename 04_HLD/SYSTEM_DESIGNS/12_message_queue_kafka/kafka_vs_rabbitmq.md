# Kafka kyun bana, aur Kafka vs RabbitMQ

Saath: [Kafka design](12_message_queue_kafka.md) · [kafka_concepts](kafka_concepts.md)

---

## 1. Kafka kyun bana (LinkedIn ki kahani)

Kafka LinkedIn me 2010-11 ke aas-paas bana tha. Jay Kreps, Neha Narkhede aur Jun Rao ne banaya.

**Dikkat kya thi:**
LinkedIn pe har pal bahut kuch ho raha tha: kisi ne profile dekhi, kisi ne click kiya, kisi ne search kiya, koi
page khula. Is data ki zaroorat kai jagah thi:
- search ka index update karna
- "People You May Know" ki sifarish banana
- Hadoop me raat ka hisaab-kitaab
- monitoring aur alert

Har bhejne wale system ko har lene wale system se alag-alag pipe se jodna pad raha tha. 5 bhejne wale aur 5 lene
wale ho gaye, to 25 pipe. Har naya system judta, to aur pipe. Ye ek uljha hua jaal ban gaya tha.

**Purane tool kyun nahi chale:**

```
purani message queue (ActiveMQ jaisi) -> itna bada data (roz arbon events) nahi jhel paati thi,
                                         aur padhte hi message mita deti thi, dobara padh nahi sakte the
log jodne wale tool                   -> sirf Hadoop me batch me daalte the, turant (real-time) nahi
```

**Kafka ka idea:**
Beech me ek badi line rakh do (log), jisme har event bas aage judta jaata hai, aur kuch din disk pe pada rehta hai.

```
bhejne wale  --> ek baar likho --> [ Kafka: ek lambi line, disk pe, kuch din tak ]
                                        |--> search apni raftaar se padhe
                                        |--> recommendations apni raftaar se
                                        |--> Hadoop raat ko ek saath padhe
                                        |--> naya system aaye, shuru se dobara padh le
```

25 pipe ki jagah ab sab bas ek line se jud gaye. Har padhne wala apna bookmark (offset) khud rakhta hai, aur kisi
ke padhne se message mit-ta nahi.

Ye wahi cheez hai jo hamare designs me hai: notification aur twitter me ek khabar daalo aur kai worker apni
raftaar se uthayein, aur news me Fetcher aur Worker ke beech Kafka. LinkedIn ki dikkat bhi bilkul yahi thi, bas
bahut bade paimane pe.

2011 me LinkedIn ne ise open-source kar diya, aur ye Apache Kafka ban gaya. Naam lekhak Franz Kafka ke naam pe
hai. Jay Kreps ne kaha tha ki ye system "likhne" ke liye bana hai, isliye ek lekhak ka naam rakha.

Interview me koi poochhe "Kafka kyun, seedha DB kyun nahi?", to yahi LinkedIn wali dikkat ek line me: ek baar
likho, aur kai log apni-apni raftaar se padhein.

---

## 2. RabbitMQ kab aaya, aur farak kya

RabbitMQ Kafka se pehle aaya tha.

```
2007  RabbitMQ   (Erlang me likha, AMQP naam ke niyam pe bana)
2011  Kafka      (LinkedIn, upar wali kahani)
```

RabbitMQ jis AMQP niyam pe bana hai, wo **JP Morgan** me shuru hua tha (2003, John O'Hara). Bank ko alag-alag
system ke beech bharose se message bhejne ka ek khula tareeka chahiye tha. Usi niyam pe baad me RabbitMQ bana.

**Dono ka farak, ek misaal se:**

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

## 3. LinkedIn ne RabbitMQ kyun nahi liya, seedha Kafka kyun banaya

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
