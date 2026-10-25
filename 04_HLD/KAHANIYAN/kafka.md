# Kafka kaise bana (LinkedIn ki kahani)

[<- saari kahaniyan](README.md) · farak / kab kaunsa: [Kafka ya RabbitMQ](../SYSTEM_DESIGNS/12_message_queue_kafka/kafka_ya_rabbitmq_kab_kaunsa.md)

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
