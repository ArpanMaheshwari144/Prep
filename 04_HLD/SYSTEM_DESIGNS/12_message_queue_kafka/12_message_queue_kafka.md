# Design a Message Queue (Kafka-jaisa)

> Ek dabba jo khabar (event) le le aur kai services ko unki apni speed se de de, bina kisi ko roke, bina kuch khoye.
> Dil: kisi ko roko mat (bahut saari khabar har second), kuch khoye nahi, aur ek user ki khabar kram me.
>
> Mock me sirf heading + Cue padho. Har card: kya toota -> kya kiya -> yahi kyun -> doosra kyun nahi. "Pooche" wali line = interviewer aage poochhe to.

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c10_2-56b5dd98.svg)

---


## Shuru

Sabse simple: ek machine, uski memory me ek queue. Producer daale, consumer uthaye.

Pehle poochho: message kho sakta hai ya bilkul nahi? Kram poore topic ka chahiye ya ek user ka kaafi? Ek message kai
service ko (sabko) jaaye ya kisi ek ko? Purana dobara padhna (replay) chahiye? Pehla aur teesra sawaal poora design
badalte hain.

Kya chahiye: producer daale, consumer uthaye. Ek message kai service tak (email + dashboard + analytics). Purana
dobara padh sakein. Ek user ki khabar kram me. Kaisa chale: khoye nahi, ek machine gire to bhi chale, lakhon har
second (dil), daalna ms me, ek key ka kram, 7 din rakho (replay). Consumer thoda peeche chale (lag) to theek hai.

Ek baat dhyaan: "duplicate nahi aayega" mat bolna. Baad me "at-least-once, duplicate aayega" bologe to dono baatein
katengi. Sahi: khabar khoti nahi, duplicate aa sakta hai, aur consumer use pehchaan ke sambhalta (idempotent). To
asar exactly-once.

Numbers: 10 crore message roz, ~86,400 second, to ~1,200 har second. Bheed me 3-5 guna, ~5,000 har second. Ek message
~1 KB, to ~100 GB roz. 7 din rakho ~700 GB, 3 copy ~2 TB. 3 copy = kam se kam 3 broker (har copy alag machine). Order
topic ~10 partition (5,000 har second, ek worker 500). Shuru me thoda zyada rakho.

MQ ka asli number JAGAH hai. Kafka padh ke delete nahi karta, rakhta hai. Log ise sirf "pipe" samajh lete.

---

*ab tootega kahan? sabse pehle machine restart pe, kyunki sab RAM me hai*

## 1. Subah queue machine restart hui, 500 signup gayab

**Cue:** LOG disk pe + OFFSET · dabba: Log

500 signup machine ki RAM me the. Restart hua, RAM saaf. Email service ko laga koi signup aaya hi nahi.

Isliye disk pe file. Message file ke aakhir me judta, padhne se mitta nahi (log). Har padhne wala sirf apna number
yaad rakhta, kahan tak padha (offset). Email ne 120 tak padha tha, restart ke baad 121 se aage.

Ye kyun chalta? Message padhne se mitta nahi, disk pe pada hai. Consumer gira to wapas aake apne offset se aage.
Producer ko pata bhi nahi chalta. Consumer page cache se padhta.

RAM wali queue kyun nahi? Restart hua aur sab gaya.

**Pooche: consumer restart hua, kahan tak padha kaun yaad rakhta?** Kafka khud (__consumer_offsets).

**Pooche: dobara padhna hai?** Offset peeche karo (replay). Khabar padi hai.

**Pooche: offset matlab byte?** Nahi, kram number 0, 1, 2. Chhoti .index file batati file me kahan hai.

**Pooche: disk pe likhna dheema nahi?** Nahi. Sirf aakhir me jodna = sequential, disk ka sabse tez kaam.

---

*ab sale aayi, ek worker kitna jhelega?*

## 2. 10 baje sale shuru, 5,000 / sec aaye, ek worker 500 hi bhej paata

**Cue:** PARTITION (tukde, alag machine) · dabba: Brokers A / B / C

Topic ek line, ek worker. Line badhti ja rahi, ghanton der (lag).

Isliye topic ke 10 tukde (partition), har tukda alag machine pe, har tukda apna worker. 10 x 500 = 5,000.

Ye kyun chalta? Partition = kitne ek saath padh sakein. Ek partition ek worker. Bheed badhe to aur partition.

Ek line me aur worker kyun nahi? Ek tukda ek hi worker padhta hai. Worker partition se zyada hon to baaki khaali
baithe.

**Pooche: worker 12, tukde 10?** 2 khaali baithe. Ek tukda ek hi worker.

**Pooche: lag phir bhi badha?** Pehle tukdon ki ginti dekho, worker baad me.

**Pooche: kitne partition?** Jitne worker ek saath chahiye. Shuru me thoda zyada, kyunki baad me badle to key ka tukda
hilta.

**Pooche: 10 tukde 3 broker pe kaise?** Barabar baante. Kaunsa kahan, ye naksha Controller ka.

---

*ab tukde ho gaye, par ek user ki khabar kram me?*

## 3. Ravi ne naam do baar badla, purana naam reh gaya

**Cue:** KEY = userId (ek partition, kram) · dabba: Producer (key)

![card 3 chalti tasveer: pehle bug, phir key = userId](img/c3_anim.gif)

Isliye ek user hamesha ek hi tukde me, kram wahin (key = userId). hash % 10 se P7, to Ravi ki har khabar P7 me, kram
pakka.

Ye kyun chalta? Kram sirf EK partition ke andar milta. Jiska kram chahiye wahi key (userId). Same key = same partition.

Poore topic ka kram kyun nahi? Alag key ka aapas me kram nahi milta, aur chahiye bhi nahi. Ek user ka kaafi.

**Pooche: producer ne retry kiya, ulta pahuncha?** Har message ka number. 1 fail, 2 gaya, 1 phir aaya, to bhi theek
(idempotent producer).

**Pooche: 10 se 20 tukde kiye?** % 10 se % 20. Key ka tukda badla, thodi der kram toota. Isliye shuru me zyada rakho.

**Pooche: key di hi nahi?** Round-robin / sticky. Koi kram nahi.

**Pooche: ek tukda garam?** Key todo (userId#bucket). Sirf tab jab us key pe kram zaroori nahi.

**Pooche: RabbitMQ / SQS me kram?** Guarantee nahi, kai consumer ek line se padhte. SQS FIFO ya ek consumer = kram,
par speed kam.

---

*ab ek service ki kai machine padh rahi*

## 4. Email service ki 3 machine, Ravi ko 3 order email

**Cue:** CONSUMER GROUP (ek team) · dabba: Group email

Teeno machine alag-alag team, har ek ne har message padha. Ravi ka order P7 me tha, teeno ne bheja, 3 email.

Isliye teeno ek team (consumer group). Har tukda team ke ek hi member ko. Ek group.id: P0-3 C1 ko, P4-6 C2 ko, P7-9
C3 ko. Ravi P7 pe, sirf C3, ek email.

Ye kyun chalta? Ek group ke andar ek partition = ek hi machine. To ek khabar team me ek hi baar.

**Pooche: group.id machine ka naam rakh diya?** Teen alag team ban gayi, teeno ne bheja. 3 email.

**Pooche: machine badhani hai?** Tukde bhi badhao. 10 tukde + 12 machine = 2 khaali.

**Pooche: ek machine gayi?** Uska partition baaki ko (rebalance).

---

*ab group me baant diya, par doosri service ko bhi wahi khabar chahiye*

## 5. Ravi ka order analytics aur dashboard ko bhi chahiye

**Cue:** HAR SERVICE KA APNA GROUP · dabba: Group analytics, dashboard

Teeno service ek hi group me thi. Ek group me ek message ek hi ko, to sirf email ko mila. Analytics aur dashboard ko kuch nahi.

Isliye har service ka apna group, har group ko poori khabar (alag consumer group). Email, analytics, dashboard teen
group, har ek ka apna offset.

Ye kyun chalta? Group ke andar baanto, alag group = sabko. Har group apni jagah (offset) yaad rakhta. Ye MQ ka dil hai.

**Pooche: nayi service aaj judi?** Naya group. Producer kuch nahi badalta.

**Pooche: usko 7 din purana chahiye?** Naye group ka offset nahi hota. Tum batao, shuru se ya ab se (auto.offset.reset).

**Pooche: ek group peeche reh gaya?** Doosra nahi rukta, offset alag-alag. Dashboard 95 pe hai to bhi email nahi rukta.

---

*ab machine hi mar gayi to?*

## 6. Dopahar Broker A ki disk mar gayi, data gaya

**Cue:** LEADER + ISR, acks=all · dabba: Brokers (ISR) + Controller

Isliye har tukde ka ek leader + 2 copy. Leader A likhta, B aur C copy lete (ISR). Saari ISR copy ne le liya, tab
producer ko OK (acks=all).

Ye kyun chalta? "Done" tabhi jab ISR copy pe bhi likha. Producer ko OK = khabar bachi. Naya leader sirf ISR se.

acks=1 kyun nahi? Tab leader ki memory pe hi "OK". Leader replica se pehle gira to khabar gayi.

**Pooche: copy kam pad gayi?** min.insync=2, to likhna band. Khone se rukna behtar.

**Pooche: A mar gaya, naya leader kaun?** Controller (KRaft) ISR me se B ko leader banata. Ek faisla, do leader nahi.

**Pooche: peeche chal raha follower leader ban sakta?** Nahi. Aakhri khabar uske paas nahi.

**Pooche: acks ke option?** 0 = bhej ke bhaag (kho sakta). 1 = sirf leader (click / log). all = saari ISR (paisa).

---

*ab broker pe to bach gaya, consumer pe kho sakta?*

## 7. Email bheja, "padh liya" bolne se pehle crash

**Cue:** PEHLE KAAM PHIR COMMIT + IDEMPOTENT · dabba: Redis (eventId)

Offset 5 = Ravi ka order. Email chala gaya, commit se pehle crash. Restart pe phir 5 se, Ravi ko 2 email.

Isliye pehle kaam, phir commit. Aur har khabar ka eventId Redis me dekho (idempotent consumer). 5 phir aaya, Redis me
E-5 pehle se hai, to skip. Ravi ko ek email.

Ye kyun chalta? Pehle kaam phir commit, to khabar kabhi nahi khoti, bas dobara aa sakti. Dobara aaye to eventId se
pehchano. Pehchaan consumer karta, Kafka nahi.

Pehle commit, phir kaam kyun nahi? Commit ho gaya aur kaam se pehle crash, to restart ke baad aage se padhta. Wo khabar chhoot gayi.

**Pooche: dedup userId se kiya?** Ravi ka agla sahi event bhi skip ho jaata. Pehchaan eventId se, kram userId se.

**Pooche: Redis se eventId jaldi mita diya?** Dobara aayi to phir bhejega. Isliye eventId ki umar (TTL) retry window se lambi.

**Pooche: Kafka me exactly-once nahi hota?** Sirf Kafka se Kafka (transactions). Email / DB ke liye consumer khud pehchaane.

---

*ab consumer sambhal gaya, par producer khud bhejne se pehle gira to?*

## 8. Order #101 DB me bana, Kafka bhejne se pehle server gira

**Cue:** OUTBOX + relay · dabba: Producer + Outbox

DB aur Kafka do alag kaam the. #101 DB me likh gaya, Kafka ko bhejne se pehle crash. #101 ki khabar gayab.

Isliye order aur khabar ek hi transaction me (outbox). BEGIN, orders(101) + outbox(E-101, sent = false), COMMIT.
Dono ya koi nahi. Phir relay har 1 second sent = false wali row Kafka ko bhejta, ACK aaya to sent = true. Kafka ko
bhejna DB commit ke BAAD, relay se.

Ye kyun chalta? Server kuch yaad nahi rakhta, DB ka sent column yaad rakhta. Order hai to khabar bhi hai.

Saga kyun nahi? Saga faile kaam ko ulta karta (refund). Yahan ulta nahi karna, khabar raaste me kho rahi thi. Pehle
pakka likho = outbox.

**Pooche: COMMIT se pehle crash?** Kuch nahi hua, na order na outbox. Ravi ko error, wo dobara karega.

**Pooche: COMMIT ke baad crash?** Row padi hai. Relay uthte hi sent = false wali bhejta. Late, par khoya nahi.

**Pooche: relay ne bheja, sent = true se pehle gira?** Khabar dobara jaati. Consumer E-101 dekh ke skip, Ravi ko EK email.

**Pooche: outbox table badi ho gayi?** sent = true wali roz saaf (delete / partition drop).

**Pooche: kuch na khoye, poora kaise?** Outbox + acks=all + kaam ke baad commit + eventId dedup + kharab ki alag line (DLQ).

---

*ab sab bach gaya, par ek consumer mara to?*

## 9. Shaam ko C2 mar gaya, lag badhta ja raha

**Cue:** REBALANCE + LAG alert + DLQ · dabba: DLQ

C2 ke tukde P4, P5, P6 kisi ke paas nahi. Koi nahi padh raha, Ravi ka email ghanton baad.

Isliye mare ke tukde baaki ko (rebalance): P4, P5 C1 ko, P6 C3 ko. Aur jo khabar baar-baar fail ho, wo alag line me
(DLQ).

Ye kyun chalta? Koi partition bina padhne wale ke nahi rehta. Koi kharab khabar poori line nahi rokti. Use theek karke
baad me dobara chalao.

**Pooche: lag kaise naapte?** Lag = aakhri offset - group kahan tak padha. Sab theek ho to ~0.

**Pooche: lag phir bhi badh raha?** Alert. Consumer badhao (tukdon tak), dheema kaam alag.

**Pooche: ek khabar baar-baar fail?** Kuch koshish, phir alag line (DLQ). Warna poora tukda atka.

**Pooche: ek khabar 5 min leti hai?** Poora tukda atka, kyunki kram se padhta. Dheema kaam alag topic / pool.

**Pooche: rebalance me sab ruk jaate?** Purane me haan (eager). Cooperative (Kafka 2.4+) me sirf jinke tukde badle.

---

*ab khabar padi rehti hai, disk ka kya?*

## 10. Raat ko broker ki disk bharti ja rahi

**Cue:** RETENTION 7 din + COMPACTION · dabba: Brokers (retention)

Sab message hamesha pade, koi nahi hatata. Disk full, naya order likhna band, order flow ruka.

Isliye kuch din rakho, phir purana tukda poora udao (retention). Padh liya to delete nahi. 7 din rakho, phir purana
segment (~1 GB) poora udao. Sasta.

Ye kyun chalta? Jo rakha hai usi se replay. Purani khabar retention se jaati, poora segment ek saath.

Padhte hi delete kyun nahi? Tab replay nahi hota. Disk sirf retention se khaali hoti, padhne se nahi.

**Pooche: bug theek kiya, dobara padhna hai?** Offset peeche karo, ya naya group shuru se (replay). Data pada hai.

**Pooche: consumer 7 din band raha?** Data gaya. Retention consumer ki sabse lambi chhutti se lambi rakho, aur lag pe alert.

**Pooche: har key ki sirf aakhri value chahiye?** Compaction. Sirf aakhri khabar rakho (profile / config).

---

## Poora design ek saans me

```
Producer (key + acks, outbox) -> key se partition -> LEADER broker pe jodo (offset) -> follower copy (ISR)
-> har consumer group apne offset se padhe -> kaam -> commit · khabar retention tak padi (replay)
Controller (KRaft) = naksha, do leader na banein · Redis = eventId dedup · DLQ = kharab message alag
message = append-only file (segment ~1 GB + index), DB nahi · offset = __consumer_offsets
```

Bolne ke liye: bhejne wala khabar key ke saath bhejta, key se tukda tay hota, tukde ke andar kram. Har tukde ki
copy, leader gira to saath chal rahi copy leader. Zaroori khabar tabhi done jab saath chal rahi saari copy (ISR) me
likh gayi. Har service ki apni team, to ek khabar sab tak. Worker kaam ke baad "padh liya" bolta, id se dobara skip,
kharab khabar DLQ me. 7 din rakhte, dobara padh sakte.

---

## 10 guna traffic

- **Partition**: consumer badhaye par partition 10 hi, to extra khaali baithe, lag badhta. Partition badhao. Par
  partition badhane se hash(key) % n badalta, purani key doosre tukde pe ja sakti, kram toot sakta. Isliye shuru me
  kuch extra partition.
- **Dheema consumer**: aage wala (email API / DB) dheema. Consumer badhao (partition tak), batch, bhaari kaam alag topic.
- **Garam partition**: ek bade customer ki key pe saara bojh. Key todo (userId#1..4), us key ka pakka kram chhodo. Ya
  us customer ka alag topic. Consistent hashing ek garam key ko nahi bachata.
- **Broker disk**: 2 TB tha, 10 guna = 20 TB. Retention 7 se 2-3 din, compaction, broker jodo, tiered storage / S3.
- **Rebalance ki aandhi**: baar-baar deploy. Session-timeout theek karo, static membership, ek-ek karke deploy.
- **Producer**: har jagah acks=all = dheema. Khabar ke hisaab se (payment = all, log = 1).

**Pooche: garam key / garam partition?** Pehle machine badi karo, phir usi dabbe ko kai hisson me todo. Todna sahi
direction. Badi machine kaafi kyun nahi? Group me ek partition ko ek hi consumer padhta, machine badi, padhne wala
phir bhi ek. Misaal: user-9 ke saare event P2 pe. Key = "user-9#" + (eventId % 4), to 4 partition, 4 consumer saath
padhein. Keemat: user-9 ka poora pakka kram gaya (har tukde ke andar bacha). Kram sach me chahiye to key chhoti cheez
pe rakho (orderId / accountId), jahan kram chahiye bas wahan.

**Pooche: 10 guna kaise?** Message ka raasta chalo.

**Pooche: kahan se sab gir sakta?** Leader (ISR se promote), controller (KRaft quorum).

**Pooche: chal raha hai, kaise pata?** Consumer lag, under-replicated partitions, DLQ size, alert.

---

## Pooche to (kisi card me nahi baitha)

API: producer send(topic, key, value) -> (partition, offset) ki rasid, acks = 0 / 1 / all. Consumer subscribe(topic,
groupId), poll(timeout) ek saath kai, commit(offset), seek(partition, offset) = replay. Admin createTopic(name,
partitions, replicationFactor) / deleteTopic / describeTopic. send() ek saath kai bhejta (batch), isliye bahut tez.
Queue ki apni API jaan-boojh ke chhoti hai: send, poll, commit, seek. Saara business logic consumer me, broker me
nahi. Isliye broker itna tez.

Pull hai, push nahi: consumer apni speed se maangta, dheema consumer dab ke nahi marta. Keemat: agle poll tak thodi
der. Push hota to broker dhakelta, dheema consumer doob jaata.

Message: offset (broker deta), timestamp, key (hash(key) % n), value (JSON / Avro), headers (traceId, eventType,
retryCount).

Disk pe: /topic-user-signup/partition-0/ ke andar 00000000000000000000.log (message, sirf jodte, segment ~1 GB),
00000000000000000000.index (offset se file me jagah). Segment bhara to naya, jaise 00000000000512340000.log.

Data 3 kism ka. Message (bahut, tez): append-only file, DB nahi, bachav = copy + acks. Naksha (kam, 100% sahi: topic /
partition / leader / ISR): pakka sahi jagah (Zookeeper / KRaft-Raft), kyunki do broker khud ko ek partition ka leader
samjhein to split-brain, data kharab. Consumer offset: __consumer_offsets.

Kab Kafka: bhejne wale ko jawab ka intezaar nahi (signup ke baad email / analytics), ek khabar kai service, bahut
saari khabar + replay (log, clickstream, audit), bhejne / lene wale ki speed alag (bheed ka buffer).

Kab nahi: turant jawab chahiye to REST / gRPC. Poore system ka pakka kram, ya simple kaam ki line + har message ka
retry, to RabbitMQ / SQS (Kafka bahut bhaari). Chhota system, 2 service, to ek aur cheez sambhaalna mehnga.

Keemat: "thodi der baad sahi" (lag), duplicate sambhaalna padta, debug mushkil (flow bikhra), ek aur cluster paalna
(partition / retention / lag dekhna).

Shabd atke to seedhi baat bolo. Partition = topic kai hisson me, har hissa alag machine pe. Offset = har hissa ek
numbered list, har consumer yaad rakhta kis number tak padha. Consumer group = consumers ki ek team jo hisse aapas me
baant leti. ISR = wo copy jo main ke saath poori barabar chal rahi. Idempotent = wahi event do baar aaye to bhi kaam
ek hi baar, kyunki event id rakhi hai. Compaction = har key ki sirf aakhri value rakho, purani phenko.

Chhe baat, poora design ek-ek line me:
1. Kyun: ek doosre ka intezaar todna. Producer bhej ke aage, consumer apni speed.
2. Topic / partition: partition = scale, append-only log + offset. Key se kram EK partition me, poore topic me nahi.
3. Group: group ke andar baanto, alag group = sabko. Kitne ek saath = partition ginti tak.
4. Copy: leader / follower, naya leader sirf ISR se. acks 0 / 1 / all = tezi vs bachav.
5. Delivery: at-least-once + idempotent consumer = asar exactly-once. Retention se replay.
6. Trade-off: async + sabko + replay to Kafka. Turant jawab / poora kram to Kafka nahi.

---

## HANDS-ON — EVENT KAHAN KHOTA HAI: 3 jagah crash karwa ke dekha

Grill sawaal (bank): "paisa kata -> event SMS / fraud / statement tak. Ek bhi event kho na jaaye, kaise?"
SAGA nahi: saga = faile kaam ko ULTA karna. Yahan ulta nahi, event RASTE me kho raha.
Sahi soch jo thi: "pehle pakka likho" = OUTBOX.
CODE: `04_HLD/HANDS_ON/04_event_loss/EventLossDemo.java` (nakli, Docker nahi) · line 24 `FIX = false / true` -> `java EventLossDemo.java`

```
[Bank DB] --1--> [Kafka] --2--> (replica) --3--> [SMS service]
1 e3: paisa kata, event bhejne se PEHLE app crash          (producer)
2 e4: leader ne liya, replica se PEHLE leader gira          (broker)
3 e2: SMS service ne offset commit kiya, SMS se PEHLE crash (consumer)
```
```
ROUND 1  FIX = false
  e1 DB + Kafka · e2 DB + Kafka · e3 DB, CRASH bhejne se pehle -> event kabhi nahi gaya
  e4 DB, Kafka acks=1 leader memory me "OK", CRASH replica se pehle -> e4 GAYA (producer ko laga bhej diya)
  e5 DB + Kafka
  consumer: e1 commit(1) SMS · e2 commit(2) CRASH SMS se pehle -> restart 2 ke aage -> e2 CHHOOTA
            · e5 commit(3) SMS
  NATEEJA: DB [e1..e5] · Kafka [e1, e2, e5] · SMS [e1, e5] · KHOYE [e2, e3, e4]

ROUND 2  FIX = true
  e1..e5 DB debit + OUTBOX row, ek transaction · e3 CRASH bhejne se pehle -> outbox me pada
  relay (restart ke baad bhi) outbox -> Kafka: e1, e2, e3 pahunche
  e4 acks=all leader + replica dono pe, TAB "OK" · CRASH leader -> replica naya leader, e4 uske paas
  e5 pahuncha
  consumer: e1 SMS, commit(1) · e2 SMS, CRASH commit se pehle -> e2 PHIR aaya -> eventId dekha
            -> DUPLICATE chhoda
            e3 SMS commit(3) · e4 SMS commit(4) · e5 SMS commit(5)
  NATEEJA: DB [e1..e5] · Kafka [e1..e5] · SMS [e1..e5] · KHOYE []
```
```
               FIX = false                                   FIX = true
e3 producer    debit hua, bhejne se pehle crash -> KHOYA     debit + outbox EK txn, relay ne bheja -> BACHA
e4 Kafka       acks=1 leader "OK", replica se pehle -> KHOYA acks=all replica ke baad "OK" -> BACHA
e2 consumer    pehle commit, SMS se pehle crash -> KHOYA     pehle SMS, baad commit, phir aaya, dedup -> BACHA

EK NIYAM: "ho gaya" tabhi bolo jab sach me PAKKA
OUTBOX      chhota LEDGER: "ye bhejna hai" debit ke SAATH, USI ek txn
                           -> "paisa kata, likha nahi" ho hi nahi sakta
acks=all    replica pe likhne ke BAAD "OK" (acks=1 = leader ki memory pe "OK" -> leader gira = gaya)
OFFSET BAAD kaam PEHLE, commit BAAD -> crash = event phir aayega (at-least-once)
IDEMPOTENT  eventId yaad, dobara aaye to chhodo
DLQ         baar-baar fail -> retry ke baad DLQ, baaki na ruke (simulation me nahi)
SAGA ≠ ye   saga kaam ULTA karta (refund). Yahan sirf raste me khona rokna.

JAWAB:    Teen jagah se khabar khoti thi, teeno band ki:
          1) Paisa kata aur khabar ek saath DB me (outbox), to bhejne se pehle crash me khabar nahi khoti.
          2) Kafka me 'saath chal rahi saari copy (ISR) me likh gaya tabhi done'.
          3) Worker kaam ke baad 'padh liya' bolta, id se dobara skip, kharab khabar DLQ me.
          Chala ke dekha: bina iske 5 me se 3 khabar kho gayi, iske saath ek bhi nahi.
```

---

ARCHETYPE B/F · CONCEPTS: [message-queues](../../FOUNDATIONS/07_message_queues.md) · [replication](../../FOUNDATIONS/05_database_replication.md) · [sharding/partition](../../FOUNDATIONS/06_database_sharding.md) · saath: [notification](../04_notification_system/04_notification_system.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
