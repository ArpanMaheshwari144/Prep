# Design a Message Queue (Kafka-jaisa)

> Ek dabba jo khabar (event) le le aur kai services ko unki apni speed se de de, bina kisi ko roke, bina kuch khoye.
> Dil: kisi ko roko mat (bahut saari khabar har second), kuch khoye nahi, aur ek user ki khabar kram me.
>
> Mock me sirf heading + Cue + GIF. Neeche har card ek block: HAL (GIF ka ILAAJ) · KYUN · NAHI (doosra kyun nahi) · AGAR (cross-Q).
> Cross-Q: hal ke har bade shabd pe socho -> gira? · dheema / bheed? · do ek saath? · galat hua? · kaise / kitna?

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c10_2-56b5dd98.svg)

---


## Shuru

```
FR      producer daale, consumer uthaye. Ek message kai service tak jaaye (email + dashboard + analytics).
        Purana dobara padh sakein. Ek user ki khabar kram me aaye
NFR     kuch khoye nahi. Ek machine gire to bhi chale. Lakhon message har second (dil). Daalna ms me.
        Ek key ka kram. 7 din rakho (replay). Consumer thoda peeche chale (lag) to theek hai
NUMBERS 10 crore message roz / ~86,400 sec = ~1,200 / sec, bheed me 3-5 guna, ~5,000 / sec.
        ~1 KB ka message -> ~100 GB roz, 7 din ~700 GB, 3 copy ~2 TB
```

---

*ab tootega kahan? sabse pehle machine restart pe, kyunki sab RAM me hai*

## 1. Subah queue machine restart hui, 500 signup gayab

**Cue:** LOG disk pe + OFFSET · dabba: Log

![card 1 chalti tasveer: pehle dikkat, phir ilaaj](img/c1_anim.gif)

```
HAL     disk pe file hai (log), message padhne se mitta nahi.
        Email service apna number yaad rakhti hai (offset)
PEHLE   sabse simple = ek machine, uski memory me ek queue. Producer daale, consumer uthaye
MISAAL  message file ke aakhir me judta hai. Email ne 120 tak padha tha, to restart ke baad 121 se padhega
KYUN    message disk pe pada hai. Consumer gira to wapas aake apne offset se aage padhta,
        producer ko pata bhi nahi chalta.
        Consumer page cache se padhta hai
NAHI    RAM wali queue -> GAYA: restart hua aur sab gaya
AGAR    consumer restart hua, wo kahan tak padha tha, ye kaun yaad rakhta hai?
        -> Kafka khud rakhta hai (__consumer_offsets)
        log dobara padhna hai, kaise padhoge?
        -> offset peeche kar do (replay). Khabar wahin padi hai
        offset ka number byte hai kya?
        -> nahi, kram number hai: 0, 1, 2. Chhoti .index file batati hai ki file me wo kahan hai
        disk pe likhna dheema nahi hoga?
        -> nahi. Sirf aakhir me jodna = sequential, ye disk ka sabse tez kaam hai
```

---

*ab sale aayi, ek worker kitna jhelega?*

## 2. 10 baje sale shuru, 5,000 / sec aaye, ek worker 500 hi bhej paata

**Cue:** PARTITION (tukde, alag machine) · dabba: Brokers A / B / C

![card 2 chalti tasveer: pehle dikkat, phir ilaaj](img/c2_anim.gif)

```
HAL     topic ke 10 tukde (partition), har tukda alag machine pe, har tukde ka apna worker. 10 x 500 = 5,000
KYUN    partition = kitne worker ek saath padh sakte. Ek partition ek worker. Bheed badhe to aur partition
NAHI    ek hi line me aur worker -> KHAALI: ek tukda ek hi worker padhta hai,
           partition se zyada worker khaali baithe rehte
AGAR    worker 12 hain aur tukde 10, kya hoga?
        -> 2 worker khaali baithe rahenge. Ek tukda ek hi worker padhta hai
        worker badhaye, phir bhi lag badh raha hai, kya dekhoge?
        -> pehle tukdon ki ginti dekho, worker baad me
        partition kitne rakhne hain, kaise tay karoge?
        -> jitne worker ek saath chahiye (order topic ~10: 5,000 / sec, ek worker 500).
           Shuru me thoda zyada rakho, baad me badle to key ka tukda hil jaata hai
        10 tukde aur 3 broker hain, kaise baantoge?
        -> barabar baanto. Kaunsa tukda kahan hai, ye naksha Controller rakhta hai
```

---

*ab tukde ho gaye, par ek user ki khabar kram me?*

## 3. Ravi ne naam do baar badla, purana naam reh gaya

**Cue:** KEY = userId (ek partition, kram) · dabba: Producer (key)

![card 3 chalti tasveer: pehle bug, phir key = userId](img/c3_anim.gif)

```
HAL     key = userId rakho. Ravi ki har khabar ek hi partition (P1) me jaati hai, kram se
KYUN    kram sirf EK partition ke andar milta hai. Jiska kram chahiye, wahi key. Same key = same partition.
        Tukda key ke hash se tay hota hai (hash % tukde)
NAHI    poore topic ka kram -> FALTU: alag key ka aapas me kram nahi milta, aur chahiye bhi nahi.
           Ek user ka kram kaafi hai
POOCHHO pehle poochho: kram poore topic ka chahiye, ya ek user ka kaafi hai?
AGAR    producer ne retry kiya aur message ulta pahuncha, kram ka kya?
        -> har message ka number hota hai. 1 fail, 2 gaya, 1 phir aaya, to bhi theek (idempotent producer)
        hash % 10 tha, ab 20 tukde kar diye, kram ka kya?
        -> % 10 se % 20 hua to key ka tukda badal gaya, thodi der kram toota. Isliye shuru me zyada rakho
        key di hi nahi, to message kahan jaayega?
        -> round-robin / sticky se kahin bhi. Koi kram nahi
        ek key ka tukda garam ho gaya (bheed), kya karoge?
        -> key tod do (userId#bucket). Par sirf tab, jab us key pe kram zaroori nahi
        RabbitMQ / SQS lagate to kram milta?
        -> guarantee nahi, kai consumer ek line se padhte hain. SQS FIFO ya ek consumer = kram,
           par speed kam
```

---

*ab ek service ki kai machine padh rahi*

## 4. Email service ki 3 machine, Ravi ko 3 order email

**Cue:** CONSUMER GROUP (ek team) · dabba: Group email

![card 4 chalti tasveer: pehle dikkat, phir ilaaj](img/c4_anim.gif)

```
HAL     teeno machine ek team hain (consumer group, ek group.id).
        Har tukda team ke ek hi member ko jaata hai: P7 sirf C3 ko
BAANT   P0-3 C1 ko, P4-6 C2 ko, P7-9 C3 ko. Ravi P7 pe hai, to sirf C3 bhejta, ek hi email
KYUN    ek group ke andar ek partition = ek hi machine. To ek khabar team me ek hi baar jaati hai
AGAR    group.id galat de diya (machine ka naam), kya hoga?
        -> teen alag team ban gayi, teeno ne bheja. Ravi ko 3 email
        team me machine badhani hai, kaise?
        -> tukde bhi badhao. 10 tukde + 12 machine = 2 khaali
        team ki ek machine gir gayi, uske tukde ka kya?
        -> baaki machines ko mil jaate hain (rebalance)
```

---

*ab group me baant diya, par doosri service ko bhi wahi khabar chahiye*

## 5. Ravi ka order analytics aur dashboard ko bhi chahiye

**Cue:** HAR SERVICE KA APNA GROUP · dabba: Group analytics, dashboard

![card 5 chalti tasveer: pehle dikkat, phir ilaaj](img/c5_anim.gif)

```
HAL     har service ka apna group. Group ke andar baanto, alag group = sabko poori khabar
MISAAL  email, analytics, dashboard: teen group, har ek ka apna offset
KYUN    har group apni jagah (offset) khud yaad rakhta hai. Ye MQ ka dil hai
NAHI    teeno service ek hi group me -> ADHOORA: group me message ek hi ko jaata, sirf email ko mila,
           analytics / dashboard ko kuch nahi
POOCHHO ek message kai service ko (sabko) jaaye, ya kisi ek ko? Ye sawaal poora design badalta hai
AGAR    nayi service aaj judi, kya badalna padega?
        -> bas naya group. Producer kuch nahi badalta
        naye group ko 7 din purana data chahiye, uska offset kahan se aayega?
        -> naye group ka offset hota hi nahi. Tum batao: shuru se ya ab se (auto.offset.reset)
        ek group dheema hai (peeche), to doosra group rukta hai?
        -> nahi, offset alag-alag hain. Dashboard 95 pe hai to bhi email nahi rukta
```

---

*ab machine hi mar gayi to?*

## 6. Dopahar Broker A ki disk mar gayi, data gaya

**Cue:** LEADER + ISR, acks=all · dabba: Brokers (ISR) + Controller

![card 6 chalti tasveer: pehle dikkat, phir ilaaj](img/c6_anim.gif)

```
HAL     har tukde ka leader A aur 2 copy (ISR). Saari ISR copy le lein, tab producer ko OK (acks=all)
KYUN    leader A likhta hai, B aur C copy lete hain. "done" tabhi, jab ISR copy pe bhi likh gaya.
        Producer ko OK = khabar bach gayi. Naya leader sirf ISR me se banta hai.
        3 copy = kam se kam 3 broker chahiye (har copy alag machine pe)
NAHI    acks=1 -> KHOYA: leader ki memory pe hi "OK", leader copy se pehle gira to khabar gayi
POOCHHO message kho sakta hai, ya bilkul nahi? Ye sawaal poora design badalta hai
AGAR    ISR copy kam pad gayi, phir bhi likhte rahein?
        -> min.insync=2 hai, to likhna band. Khone se rukna behtar hai
        leader A gir gaya, naya leader kaun banega?
        -> Controller (KRaft) ISR me se B ko leader banata hai. Ek hi faisla, do leader nahi bante
        ek follower peeche chal raha hai, wo leader ban sakta hai?
        -> nahi. Aakhri khabar uske paas nahi hai
        acks badalna ho, to kaunse option hain?
        -> 0 = bhej ke bhaag jao (kho sakta). 1 = sirf leader (click / log). all = saari ISR (paisa)
```

---

*ab broker pe to bach gaya, consumer pe kho sakta?*

## 7. Email bheja, "padh liya" bolne se pehle crash

**Cue:** PEHLE KAAM PHIR COMMIT + IDEMPOTENT · dabba: Redis (eventId)

![card 7 chalti tasveer: pehle dikkat, phir ilaaj](img/c7_anim.gif)

```
HAL     pehle kaam karo, phir commit. Har khabar ka eventId Redis me dekho: dobara aaye to skip
        (idempotent consumer)
MISAAL  5 phir aaya, Redis me E-5 pehle se pada hai, to skip. Ravi ko ek hi email
KYUN    pehle kaam phir commit, to khabar kabhi nahi khoti, bas dobara aa sakti hai. Dobara aaye to eventId
        se pehchano. Pehchaan consumer karta hai, Kafka nahi
NAHI    pehle commit, phir kaam -> CHHOOTA: commit hua, kaam se pehle crash, restart pe aage se padha.
           Wo khabar chhoot gayi
DHYAAN  "duplicate nahi aayega" mat bolna. Baad me "at-least-once, duplicate aayega" bologe to dono katengi.
        Sahi: khabar khoti nahi, duplicate aa sakta hai, consumer pehchaan ke sambhalta hai (idempotent).
        Asar exactly-once jaisa
AGAR    eventId ki jagah userId se dedup kiya, kya hoga?
        -> Ravi ka agla sahi event bhi skip ho jaayega. Pehchaan eventId se, kram userId se
        Redis se eventId jaldi mit gaya, kya hoga?
        -> khabar dobara aayi to phir bhej dega. eventId ki umar (TTL) retry window se lambi rakho
        exactly-once poocha, Kafka me nahi hota?
        -> sirf Kafka se Kafka (transactions). Email / DB ke liye consumer khud pehchaane
```

---

*ab consumer sambhal gaya, par producer khud bhejne se pehle gira to?*

## 8. Order #101 DB me bana, Kafka bhejne se pehle server gira

**Cue:** OUTBOX + relay · dabba: Producer + Outbox

![card 8 chalti tasveer: pehle dikkat, phir ilaaj](img/c8_anim.gif)

```
HAL     order aur khabar EK transaction me (outbox). Relay commit ke BAAD Kafka ko bhejta hai
QUERY   BEGIN, orders(101) + outbox(E-101, sent = false), COMMIT. Dono likhe jaayenge, ya koi nahi
RELAY   relay har 1 second sent = false wali row Kafka ko bhejta, ACK aaya to sent = true
KYUN    server kuch yaad nahi rakhta, DB ka sent column yaad rakhta hai. Order hai to khabar bhi hai
NAHI    Saga -> ALAG: wo fail hue kaam ko ulta karta hai (refund). Yahan ulta nahi karna,
           khabar raaste me kho rahi hai. Pehle pakka likho = outbox
AGAR    COMMIT se pehle crash ho gaya, kya bacha?
        -> kuch nahi hua, na order na outbox. Ravi ko error dikha, wo dobara karega
        COMMIT ke baad crash hua, khabar ka kya?
        -> row DB me padi hai. Relay uthte hi sent = false wali bhej deta. Late, par khoya nahi
        relay ne bheja, par sent = true likhne se pehle gir gaya
        -> khabar dobara jaati hai. Consumer E-101 dekh ke skip karta, Ravi ko EK email
        outbox table badi ho gayi, kya karoge?
        -> sent = true wali rows roz saaf karo (delete / partition drop)
        poore raaste kuch na khoye, kaise?
        -> outbox + acks=all + kaam ke baad commit + eventId dedup + kharab ki alag line (DLQ)
```

---

*ab sab bach gaya, par ek consumer mara to?*

## 9. Shaam ko C2 mar gaya, lag badhta ja raha

**Cue:** REBALANCE + LAG alert + DLQ · dabba: DLQ

![card 9 chalti tasveer: pehle dikkat, phir ilaaj](img/c9_anim.gif)

```
HAL     mare hue C2 ke tukde baaki ko milte hain (rebalance): P4, P5 C1 ko, P6 C3 ko.
        Jo khabar baar-baar fail ho, wo alag line me (DLQ)
KYUN    koi partition bina padhne wale ke nahi rehta. Kharab khabar poori line nahi rokti,
        use theek karke baad me dobara chalao
AGAR    lag naapna hai, kaise naapoge?
        -> lag = aakhri offset - group kahan tak padha. Sab theek ho to ~0
        rebalance ke baad bhi lag badh raha hai, kya karoge?
        -> alert. Consumer badhao (tukdon tak), dheema kaam alag karo
        ek khabar baar-baar fail ho rahi, use DLQ me kab bhejoge?
        -> kuch koshish ke baad alag line (DLQ) me. Warna poora tukda atka rahega
        ek khabar dheemi hai (5 min), kya hoga?
        -> poora tukda atak jaata, kyunki kram se padhta hai. Dheema kaam alag topic / pool me
        rebalance chal raha hai, to sab ruk jaate hain?
        -> purane tareeke (eager) me haan. Cooperative (Kafka 2.4+) me sirf wo, jinke tukde badle
```

---

*ab khabar padi rehti hai, disk ka kya?*

## 10. Raat ko broker ki disk bharti ja rahi

**Cue:** RETENTION 7 din + COMPACTION · dabba: Brokers (retention)

![card 10 chalti tasveer: pehle dikkat, phir ilaaj](img/c10_anim.gif)

```
HAL     7 din rakho, phir purana segment (~1 GB) poora udao (retention). Padh liya to delete nahi. Sasta
KYUN    jo rakha hai usi se replay hota hai. Purani khabar retention se jaati, poora segment ek saath.
        MQ ka asli number JAGAH hai. Kafka padh ke delete nahi karta, rakhta hai.
        Log ise sirf "pipe" samajh lete hain
NAHI    padhte hi delete -> GAYA: replay nahi ho paata. Disk sirf retention se khaali hoti, padhne se nahi
POOCHHO purana dobara padhna (replay) chahiye?
AGAR    bug theek kiya, ab replay chahiye, kaise?
        -> offset peeche karo, ya naya group shuru se padhe (replay). Data pada hai
        consumer 7 din se zyada (retention se lamba) band raha, data ka kya?
        -> gaya. Retention consumer ki sabse lambi chhutti se lambi rakho, aur lag pe alert
        har key ki sirf aakhri value chahiye, kaise?
        -> compaction. Sirf aakhri khabar rakho (profile / config)
```

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
