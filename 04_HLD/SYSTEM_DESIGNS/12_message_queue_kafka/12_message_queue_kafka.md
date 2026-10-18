# Design a Message Queue (Kafka-jaisa)

> Ek dabba jo khabar (event) le le aur kai services ko unki apni speed se de de — bina kisi ko roke, bina kuch khoye.
> Is design ka dil: **kisi ko roko mat (bahut saari khabar / second)** + **kuch khoye nahi** + **ek user ki khabar kram me**.

---

## KAHANI (ek din, main = order team ka engineer, aaj SALE; har dikkat se design me ek dabba judta)

```
    KYA HUA                                   ILAAJ                                 DABBA JUDA
1   subah queue restart, 500 signup gayab  -> LOG disk pe + OFFSET                  Log
2   10 baje sale, 5,000 / sec, worker 500  -> PARTITION (tukde, alag machine)       Brokers A / B / C
3   Ravi ne naam do baar badla, ulta laga  -> KEY = userId (ek partition, kram)     Producer (key)
4   email ki 3 machine, Ravi ko 3 email    -> CONSUMER GROUP (ek team)              Group email
5   analytics + dashboard ko bhi chahiye   -> HAR SERVICE KA APNA GROUP             Group analytics, dashboard
6   dopahar Broker A ki disk mari          -> LEADER + ISR, acks=all                Brokers (ISR) + Controller
7   email bheja, commit se pehle crash     -> PEHLE KAAM PHIR COMMIT + IDEMPOTENT   Redis (eventId)
8   order DB me bana, Kafka se pehle gira  -> OUTBOX + relay                        Producer + Outbox
9   shaam ko C2 mara, lag badhta gaya      -> REBALANCE + LAG alert + DLQ           DLQ
10  raat ko broker ki disk bhar rahi       -> RETENTION 7 din + COMPACTION          Brokers (retention)

POORA DESIGN (upar ke dabbe kram se):
   Producer (key + acks, outbox) -> key se partition -> LEADER broker (offset) -> ISR copy
   -> har service ka apna group, apne offset se -> kaam -> commit · Redis = eventId dedup · DLQ
   Controller (KRaft) = naksha · khabar 7 din padi (replay)
```

---

## TASVEER (ByteByteGo / Alex Xu · CC BY-NC-ND 4.0)

![Can Kafka Lose Messages?](https://assets.bytebytego.com/diagrams/0130-can-kafka-lose-messages.png)
Source: [Can Kafka Lose Messages?](https://bytebytego.com/guides/can-kafka-lose-messages/)
(HANDS-ON — khabar kahan khoti: producer / broker / consumer, wahi 3 jagah)

![Delivery Semantics](https://assets.bytebytego.com/diagrams/0165-delivery-semantics.png)
Source: [Delivery Semantics](https://bytebytego.com/guides/delivery-semantics/)
(at-most-once / at-least-once / exactly-once — offset commit PEHLE ya BAAD)

---

## SHURU: pehle poocho, phir numbers

```
POOCHO:  "Kram, khona-nahi, delivery guarantee, dobara padhna — kis pe focus?"
         message KHO sakta hai ya bilkul nahi? · KRAM poore topic ka chahiye ya ek user ka kaafi?
         ek message KAI service ko (sabko) ya kisi ek ko? · purana dobara padhna (replay)?
         (pehla aur teesra sawaal poora design badalte)

KYA KARE (FR):    producer DAALE · consumer UTHAYE · EK message
                  KAI service tak (email + dashboard + analytics)
                  purana dobara padh sakein · ek user ki khabar KRAM me
KAISA CHALE (NFR): khoye nahi · ek machine gire to chale · lakhon / second (dil) · daalna ms me
                   ek key ka kram · 7 din rakho (replay) · consumer thoda peeche chale (lag) to THEEK

★ "DUPLICATE NAHI AAYEGA" MAT BOLNA: baad me "at-least-once, duplicate aayega" bologe to dono baatein kat-ti
  -> "aapne to no-duplicate bola tha?"
  SAHI: "No message loss. Duplicates ARE possible, and we handle them with idempotent consumers —
         so the EFFECT is exactly-once."

NUMBERS (andaaza):
   10 crore message / din, ~86,400 second -> ~1,200 / second -> bheed me 3-5 guna -> ~5,000 / second
   ~1 KB ek message -> ~100 GB / din · 7 din rakho ~700 GB · 3 copy ~2 TB
   3 copy = kam se kam 3 BROKER (har copy alag machine) · order topic ~10 PARTITION (5,000 / sec,
     ek worker 500); shuru me thoda zyada rakho (partition = kitne ek saath padh sakein)
   MQ ka asli number JAGAH hai (Kafka padh ke delete nahi karta, RAKHTA) — log ise sirf "pipe" samajh lete
```

---

## ════ MOCK SE PEHLE: SIRF YE CARD PADHO ════

> Har card: pehle PEELA DABBA dekho (scene + ilaaj). Yaad aa gaya -> aage. Nahi aaya -> neeche
> DIKKAT / HAL / NAHI padho. AAGE POOCHEGA sirf jab interviewer aage kuch poochhe.

#### DABBA 0: SABSE SIMPLE (yahan se shuru, phir "kahan tootega")
```
Ek machine, uski memory me ek queue. Producer daale, consumer uthaye.
```
```mermaid
%%{init: {"flowchart": {"wrappingWidth": 450}}}%%
flowchart LR
    n_P["Producer"]
    n_Q["Queue"]
    n_C["Consumer"]
    n_P --> n_Q
    n_Q --> n_C
```

#### 1 Subah queue machine restart hui, 500 signup gayab
```mermaid
%%{init: {"flowchart": {"wrappingWidth": 450}}}%%
flowchart LR
    n_P["Producer"]
    n_L["Log<br/>restart, 500 message gayab<br/>-> disk pe log, kahan tak padha yaad<br/>(log + offset)<br/>consumer restart -> Kafka yaad rakhe kahan tak padha (offset)<br/>dobara padhna -> offset peeche karo (replay)"]
    n_C["Consumer"]
    n_P --> n_L
    n_L --> n_C
    style n_L fill:#f9a825,color:#000
```
```
DIKKAT     Subah, sale se pehle signup khule. Queue machine ki memory me 500 signup message, machine restart
           -> memory saaf -> 500 gayab. Email service ko pata hi nahi chala ki 500 log aaye.
HAL        disk pe log, padha kahan tak (LOG + OFFSET): sirf aakhir me jodo: P0: [0][1][2][3][4][5].
           Har message ka number = OFFSET, padhne se mit-ta nahi. Email ne 2 tak padha -> restart -> "3 se".
NAHI       DB me queue: insert = index + lock = idhar-udhar likhna; queue me WHERE nahi, bas "X ke baad do".
```
```
AAGE POOCHEGA:
  NIYAM: message padhne se mit-ta nahi (disk pe log); har padhne wala sirf apna number (offset) yaad rakhta.
         consumer gira -> wapas aake apne offset se aage; producer ko pata bhi nahi chalta.
  SAFAR (ek signup khabar, har mod pe kya):
  sab theek         -> producer file ke aakhir me jode (log) -> consumer page cache se padhe; khabar mit-ti
                       nahi
  consumer restart  -> Kafka khud yaad rakhta (__consumer_offsets): kahan tak padha (offset)
  dobara padhna hai -> offset peeche karo (replay): khabar padi hai
```

#### 2 10 baje sale shuru, 5,000 / sec aaye, ek worker 500 hi bhej paata
```mermaid
%%{init: {"flowchart": {"wrappingWidth": 450}}}%%
flowchart LR
    n_P["Producer"]
    n_B["Brokers A / B / C<br/>5,000 / sec, worker 500<br/>-> topic ke tukde, har tukda alag worker<br/>(partition)<br/>worker 12, tukde 10 -> 2 khaali baithe<br/>lag phir bhi badha -> pehle tukde badhao, worker baad me"]
    n_C["Consumer"]
    n_P --> n_B
    n_B --> n_C
    style n_B fill:#f9a825,color:#000
```
```
DIKKAT     10 baje sale: "order-events" pe 5,000 khabar / sec. Email worker ~500 / sec hi bhej paata
           (har email ek API call). Topic ek line -> ek hi worker kram se padhe -> ghanton ki line (lag).
HAL        topic ke tukde (PARTITION): topic ke 10 tukde (P0-P9), teen broker A / B / C pe bate; har tukda
           alag worker -> 10 tukde = 10 worker = ~5,000 / sec. Likhai aur jagah bhi bat gayi.
NAHI       copy (replica) badhao: copy sirf bachav ke liye, padhna bhi leader se hi; padhne wale = partition.
```
```
AAGE POOCHEGA:
  NIYAM: partition = kitne ek saath padh sakein (ek partition ek worker); bheed badhe -> aur partition.
         worker partition se zyada = baaki khaali baithe; lag badhe to pehle partition ginti dekho.
  SAFAR (sale ki bheed, har mod pe kya):
  sab theek           -> 5,000 / sec -> topic ke 10 tukde (partition), har tukda ek worker: 10 x 500
  worker 12, tukde 10 -> 2 khaali baithe: ek tukda ek hi worker
  lag phir bhi badha  -> pehle tukdon ki ginti dekho, worker baad me
```

#### 3 Ravi ne naam do baar badla, purana naam reh gaya
```mermaid
%%{init: {"flowchart": {"wrappingWidth": 450}}}%%
flowchart LR
    n_P["Producer<br/>do badlaav do partition me<br/>-> ek user ki khabar ek hi tukde me<br/>(key = userId)<br/>producer retry ulta -> har message ka number (idempotent producer)<br/>10 se 20 tukde -> key ka tukda badla, kram thoda toota<br/>key nahi di -> koi kram nahi (round-robin)<br/>ek tukda garam -> key me bucket jodo (userId + bucket)"]
    n_B["Brokers A / B / C"]
    n_C["Consumer"]
    n_P --> n_B
    n_B --> n_C
    style n_P fill:#f9a825,color:#000
```
```
DIKKAT     Ravi ne naam "Ravi K" kiya, 1 min baad "Ravi Kumar". Pehla P0 me, doosra P1 me. P0 ka consumer
           10 min peeche (lag) -> pehle "Ravi Kumar" laga, phir "Ravi K" -> galat naam reh gaya.
HAL        ek user ek tukda (KEY = userId): Kafka kram sirf EK partition ke andar deta. hash(userId) % 10 ->
           hamesha P7 -> dono badlaav P7 me, kram se.
NAHI       poore topic ka kram: matlab sirf ek partition = speed khatam. Kram sirf key ke andar.
```
```
AAGE POOCHEGA:
  NIYAM: kram sirf EK partition ke andar: jiska kram chahiye wahi key (userId); same key = same partition.
         alag key ka aapas me kram nahi milta, aur chahiye bhi nahi.
  SAFAR (Ravi ke do naam-badlaav, har mod pe kya):
  sab theek           -> key = userId: hash % 10 -> P7, same tukda = kram pakka (alag user ka kram chahiye
                         nahi)
  producer retry ulta -> har message ka number: 1 fail, 2 gaya, 1 phir -> theek (idempotent producer)
  10 se 20 tukde      -> % 10 se % 20: key ka tukda badla, thodi der kram toota -> shuru me zyada rakho
  key di hi nahi      -> round-robin / sticky: koi kram nahi
  ek tukda garam      -> key todo (userId#bucket): sirf jab us key pe kram zaroori nahi
```

#### 4 Email service ki 3 machine, Ravi ko 3 order email
```mermaid
%%{init: {"flowchart": {"wrappingWidth": 450}}}%%
flowchart LR
    n_P["Producer (key = userId)"]
    n_B["Brokers A / B / C"]
    n_GE["Group email<br/>3 machine, 3 email<br/>-> teeno ek team, har tukda ek ko<br/>(consumer group)<br/>group.id = machine naam -> teen alag team, 3 email<br/>machine badhani -> tukde bhi badhao, warna khaali"]
    n_P --> n_B
    n_B --> n_GE
    style n_GE fill:#f9a825,color:#000
```
```
TASVEER: ek team, tukde baante
topic order-events:  P0 P1 P2 P3 | P4 P5 P6 | P7 P8 P9
group email:         C1          | C2       | C3
Ravi: hash(userId) % 10 = P7  -> sirf C3 padhta -> ek hi email
C2 gaya -> P4-6 baaki machines me (rebalance) · 12 machine = 2 khaali (10 hi tukde)
```
```
AAGE POOCHEGA:
  NIYAM: ek group ke andar ek partition = ek hi machine -> ek khabar team me ek hi baar.
         machine gayi -> uska partition baaki ko (rebalance); machine partition se zyada = khaali baithi.
  SAFAR (Ravi ka order email, har mod pe kya):
  sab theek               -> ek group.id: P0-3 -> C1 · P4-6 -> C2 · P7-9 -> C3; Ravi P7 -> sirf C3, ek email
  group.id = machine naam -> teen alag team, teeno ne bheja: 3 email
  machine badhani         -> tukde bhi badhao: 10 tukde + 12 machine = 2 khaali
```

#### 5 Ravi ka order analytics aur dashboard ko bhi chahiye
```mermaid
%%{init: {"flowchart": {"wrappingWidth": 450}}}%%
flowchart LR
    n_P["Producer (key = userId)"]
    n_B["Brokers A / B / C"]
    n_GE["Group email"]
    n_GA["Group analytics<br/>teeno service ko chahiye<br/>-> har service ki apni team<br/>(alag group)<br/>nayi service judi -> naya group, producer same<br/>usko purana chahiye -> shuru se ya ab se (auto.offset.reset)<br/>ek group peeche -> doosra nahi rukta, offset alag"]
    n_GD["Group dashboard"]
    n_P --> n_B
    n_B --> n_GE
    n_B --> n_GA
    n_B --> n_GD
    style n_GA fill:#f9a825,color:#000
    style n_GD fill:#f9a825,color:#000
```
```
DIKKAT     Ravi ka order email ko bhi chahiye, analytics ko bhi, dashboard ko bhi. Ek group = sirf ek ko.
HAL        har service ki apni team (APNA GROUP): topic ek; email, analytics, dashboard group alag.
           Team ke andar kaam bat-ta, alag team = poori khabar. Har group ka apna offset.
NAHI       har service ka alag topic: producer ek event teen baar bheje -> "ek message kai service" hi toota.
```
```
AAGE POOCHEGA:
  NIYAM: group ke andar baanto, alag group = sabko (har group ka apna offset); ye MQ ka dil hai.
         nayi service = naya group, producer kuch nahi badalta; ek group peeche ho to doosra nahi rukta.
  SAFAR (Ravi ka order, teen service, har mod pe kya):
  sab theek             -> email / analytics / dashboard = teen group: har group ko poori khabar, apna
                           offset
  nayi service aaj judi -> naya group, producer kuch nahi badalta
  usko 7 din purana?    -> naye group ka offset nahi: tum batao, shuru se ya ab se (auto.offset.reset)
  ek group peeche       -> doosra nahi rukta: offset alag-alag
```

#### 6 Dopahar Broker A ki disk mar gayi, data gaya
```mermaid
%%{init: {"flowchart": {"wrappingWidth": 450}}}%%
flowchart LR
    n_P["Producer (key = userId)"]
    n_B["Brokers A / B / C"]
    n_K["Controller (KRaft)<br/>Broker A ki disk gayi<br/>-> har tukde ki copy, sab me likha tab OK<br/>(ISR, acks=all)<br/>copy kam -> likhna band, khone se rukna behtar (min.insync=2)<br/>A mara -> Controller ISR se B ko leader<br/>peeche wala follower -> leader nahi ban sakta"]
    n_GE["Group email"]
    n_GA["Group analytics"]
    n_GD["Group dashboard"]
    n_P --> n_B
    n_B --> n_K
    n_B --> n_GE
    n_B --> n_GA
    n_B --> n_GD
    style n_K fill:#f9a825,color:#000
    style n_B fill:#f9a825,color:#000
```
```
DIKKAT     Dopahar Broker A ki disk mari. Har partition ki ek hi copy thi -> P0 ka data hamesha ke liye gaya.
HAL        har tukde ki copy (LEADER + ISR): LEADER + 2 follower alag machine pe; saath chalti copy = ISR.
           Leader gira -> Controller (KRaft) ISR se naya chunta. Zaroori: acks=all (saari ISR ne liya tab OK).
NAHI       acks=1 sab jagah: leader ne "OK" bola, copy se pehle gira -> khabar gayi.
```
```
TASVEER: leader + copy, acks=all
P0:  A = LEADER   ·   B = follower   ·   C = follower        saath chalti copy = ISR {A, B, C}
producer -> A ne likha -> B, C ne bhi liya -> tab "OK" (acks=all)
A ki disk mari -> Controller (KRaft) ISR se B ko naya leader -> data bacha
jo follower peeche reh gaya wo ISR se bahar -> leader nahi ban sakta
```
```
AAGE POOCHEGA:
  NIYAM: "done" tabhi jab ISR copy pe bhi likha (acks=all); naya leader sirf ISR se.
         producer ko OK = khabar bachi; copy kam pad jaaye to likhna band, khona nahi.
  SAFAR (ek khabar likhna, har mod pe kya):
  sab theek              -> leader A likhe, B + C copy lein (ISR); saari ISR ne li tab OK (acks=all)
  copy kam pad gayi      -> min.insync=2 -> likhna band: khone se rukna behtar
  A mara                 -> Controller (KRaft) ISR se B ko leader: ek faisla, do leader nahi
  peeche wala follower   -> leader nahi ban sakta: aakhri khabar uske paas nahi
```

#### 7 Email bheja, "padh liya" bolne se pehle crash
```mermaid
%%{init: {"flowchart": {"wrappingWidth": 450}}}%%
flowchart LR
    n_P["Producer (key = userId)"]
    n_B["Brokers A / B / C"]
    n_K["Controller (KRaft)"]
    n_GE["Group email"]
    n_R["Redis<br/>commit se pehle crash<br/>-> pehle kaam phir padh liya, dobara to skip<br/>(eventId)<br/>commit se pehle crash -> dobara aayi, eventId se skip<br/>dedup userId se -> agla sahi event bhi skip, eventId lo<br/>eventId jaldi mitaya -> phir bhejega, umar retry se lambi (TTL)"]
    n_GA["Group analytics"]
    n_GD["Group dashboard"]
    n_P --> n_B
    n_B --> n_K
    n_B --> n_GE
    n_B --> n_GA
    n_B --> n_GD
    n_GE --> n_R
    style n_R fill:#f9a825,color:#000
```
```
DIKKAT     Email service ne offset 5 padha (Ravi ka order). Email bheja -> "padh liya" (commit) se pehle CRASH
           -> restart pe phir 5 -> Ravi ko 2 email.
HAL        pehle kaam, phir 'padh liya' (COMMIT) (at-least-once: khoyega nahi, kabhi do baar) + consumer
           IDEMPOTENT: eventId Redis me hai? hai = skip.
NAHI       pehle commit, phir kaam: email se pehle CRASH -> restart 6 se -> event 5 KHO gaya.
```
```
AAGE POOCHEGA:
  NIYAM: pehle kaam, phir commit -> khona kabhi nahi, par dobara aa sakta; dobara aaye to eventId se pehchano.
         pehchaan consumer karta, Kafka nahi; isliye Ravi ko do email nahi jaata.
  SAFAR (ek email khabar, har mod pe kya):
  sab theek                    -> khabar li -> pehle email bhejo, PHIR commit: khona kabhi nahi
  bheja, commit se pehle crash -> khabar dobara aayegi -> eventId dekho, bhej chuke to skip
  dedup userId se kiya         -> Ravi ka agla sahi event bhi skip -> pehchaan eventId se, kram userId se
  eventId jaldi mita diya      -> dobara aayi to phir bhejega: umar (TTL) retry window se lambi
```

#### 8 Order #101 DB me bana, Kafka bhejne se pehle server gira
```mermaid
%%{init: {"flowchart": {"wrappingWidth": 450}}}%%
flowchart LR
    n_P["Producer + Outbox<br/>DB me bana, event nahi gaya<br/>-> order ke saath khabar bhi DB me, baad me bhejo<br/>(outbox)<br/>COMMIT se pehle crash -> kuch nahi hua, Ravi dobara kare<br/>COMMIT ke baad crash -> relay baad me bheje, khoya nahi<br/>bheja, sent=true se pehle -> dobara jaaye, consumer E-101 skip<br/>outbox badi -> sent wali roz saaf"]
    n_B["Brokers A / B / C"]
    n_K["Controller (KRaft)"]
    n_GE["Group email"]
    n_R["Redis (eventId)"]
    n_GA["Group analytics"]
    n_GD["Group dashboard"]
    n_P --> n_B
    n_B --> n_K
    n_B --> n_GE
    n_B --> n_GA
    n_B --> n_GD
    n_GE --> n_R
    style n_P fill:#f9a825,color:#000
```
```
DIKKAT     Ravi ka order #101 DB me bana, Kafka bhejne se pehle order server gira -> event gaya hi nahi.
HAL        khabar bhi order ke saath DB me (OUTBOX, usi transaction me): BEGIN · INSERT orders(101) · INSERT
           outbox(E-101, sent = false) · COMMIT. Relay (alag thread, har 1 sec) sent = false wali row Kafka
           bheje, ACK aaye to sent = true.
NAHI       "offset bachayega": nahi, offset consumer ki taraf hai; ye producer ki taraf ki dikkat.
```
```
TASVEER: order + khabar ek saath
BEGIN -> orders(101) + outbox(E-101, sent = false) -> COMMIT      (dono ya koi nahi)
relay (har 1 sec): sent = false wali row -> Kafka -> ACK -> sent = true
server COMMIT se pehle gira -> na order, na khabar · baad me gira -> row padi, relay bhej dega
relay ACK se pehle gira -> khabar dobara jaati -> consumer eventId se skip
```
```
AAGE POOCHEGA:
  NIYAM: server kuch yaad nahi rakhta, DB ka sent column yaad rakhta (order + khabar ek transaction).
         relay gira -> sent = false wali dobara jaati -> consumer ko dobara mil sakti, eventId se skip.
  SAFAR (order #101 se Kafka tak, har mod pe kya):
  sab theek                 -> order + outbox row ek transaction (sent = false) -> relay bheje -> sent =
                               true
  COMMIT se pehle crash     -> kuch nahi hua: na order na outbox, Ravi ko error, wo dobara karega
  COMMIT ke baad crash      -> relay uthte hi sent = false wali bhejta: late, khoya nahi
  bheja, sent=true se pehle -> dobara jaati; consumer E-101 dekh ke skip -> Ravi ko EK email
  outbox badi hui           -> sent = true wali roz saaf (delete / partition drop)
```

#### 9 Shaam ko C2 mar gaya, lag badhta ja raha
```mermaid
%%{init: {"flowchart": {"wrappingWidth": 450}}}%%
flowchart LR
    n_P["Producer (key + outbox)"]
    n_B["Brokers A / B / C"]
    n_K["Controller (KRaft)"]
    n_GE["Group email"]
    n_R["Redis (eventId)"]
    n_DLQ["DLQ<br/>C2 mara, lag badha<br/>-> mare ka kaam baaki me, kharab khabar alag line<br/>(rebalance + DLQ)<br/>C2 gaya -> uske tukde C1 + C3 me (rebalance)<br/>lag phir badha -> alert, consumer badhao<br/>khabar baar-baar fail -> kuch koshish, phir alag line (DLQ)"]
    n_GA["Group analytics"]
    n_GD["Group dashboard"]
    n_P --> n_B
    n_B --> n_K
    n_B --> n_GE
    n_B --> n_GA
    n_B --> n_GD
    n_GE --> n_R
    n_GE --> n_DLQ
    style n_DLQ fill:#f9a825,color:#000
```
```
AAGE POOCHEGA:
  NIYAM: koi partition bina padhne wale ke nahi (rebalance), koi kharab khabar poori line na roke (DLQ).
         lag badhe -> alert; kharab khabar alag line me, theek karke baad me dobara.
  SAFAR (C2 mara, har mod pe kya):
  sab theek                -> har tukde ka ek padhne wala; lag = aakhri offset - group kahan tak, ~0
  C2 gaya                  -> uske P4-6 C1 + C3 me bat gaye (rebalance)
  lag phir bhi badh raha   -> alert -> consumer badhao (tukdon tak); dheema kaam alag
  ek khabar baar-baar fail -> kuch koshish, phir alag line (DLQ): warna poora tukda atka
```

#### 10 Raat ko broker ki disk bharti ja rahi
```mermaid
%%{init: {"flowchart": {"wrappingWidth": 450}}}%%
flowchart LR
    n_P["Producer (key + outbox)"]
    n_B["Brokers A / B / C<br/>disk bharti ja rahi<br/>-> 7 din rakho phir udao<br/>(retention + compaction)<br/>dobara padhna -> offset peeche ya naya group (replay)<br/>consumer 7 din band -> data gaya, retention lambi + lag alert<br/>har key ki aakhri -> sirf aakhri rakho (compaction)"]
    n_K["Controller (KRaft)"]
    n_GE["Group email"]
    n_R["Redis (eventId)"]
    n_DLQ["DLQ"]
    n_GA["Group analytics"]
    n_GD["Group dashboard"]
    n_P --> n_B
    n_B --> n_K
    n_B --> n_GE
    n_B --> n_GA
    n_B --> n_GD
    n_GE --> n_R
    n_GE --> n_DLQ
    style n_B fill:#f9a825,color:#000
```
```
AAGE POOCHEGA:
  NIYAM: jo rakha hai usi se REPLAY; rakhna consumer ki sabse lambi chhutti se lamba.
         purani khabar retention se jaati (poora segment); har key ki aakhri hi chahiye to compaction.
  SAFAR (disk bharti, har mod pe kya):
  sab theek                -> padh liya -> delete nahi; 7 din rakho, phir purana segment (~1 GB) poora udao
                              = sasta
  bug theek, dobara padhna -> offset peeche karo, ya naya group shuru se (replay): data pada hai
  consumer 7 din band      -> data gaya: retention consumer ki sabse lambi chhutti se lambi + lag pe alert
  har key ki aakhri hi     -> compaction: sirf aakhri khabar rakho (profile / config)
```

---

## AAKHRI DABBA + WRAP (ek hi baar)

```
Producer (key + acks, outbox) -> key se partition -> LEADER broker pe jodo (offset) -> follower copy (ISR)
-> har consumer group apne offset se padhe -> kaam -> commit · khabar retention tak padi (replay)
Controller (KRaft) = naksha, do leader na banein · Redis = eventId dedup · DLQ = kharab message alag

DB:  message = append-only file (segment ~1 GB + index), DB nahi; bachav = copy + acks
     naksha (kaun leader) = pakka sahi (KRaft / Zookeeper) · offset = __consumer_offsets
```
```
JAWAB:    Bhejne wala khabar key ke saath bhejta, key se tukda tay hota, tukde ke andar kram.
          Har tukde ki copy, leader gira to saath chal rahi copy leader.
          Zaroori khabar 'saath chal rahi saari copy (ISR) me likh gaya tabhi done'.
          Har service ki apni team, to ek khabar sab tak. Worker kaam ke baad 'padh liya' bolta, id se
          dobara skip, kharab khabar DLQ me. 7 din rakhte, dobara padh sakte.
```
```mermaid
%%{init: {"flowchart": {"wrappingWidth": 450}}}%%
flowchart LR
    n_P["Producer (key + outbox)"]
    n_B["Brokers A / B / C"]
    n_K["Controller (KRaft)"]
    n_GE["Group email"]
    n_R["Redis (eventId)"]
    n_DLQ["DLQ"]
    n_GA["Group analytics"]
    n_GD["Group dashboard"]
    n_P --> n_B
    n_B --> n_K
    n_B --> n_GE
    n_B --> n_GA
    n_B --> n_GD
    n_GE --> n_R
    n_GE --> n_DLQ
```

---

## 10 GUNA TRAFFIC — har dabba alag se

```
partition      -> consumer badhaye par partition 10 hi -> extra KHAALI baithe, lag badhta -> PARTITION BADHAO
                  ★ "Adding partitions changes hash(key) % n, so an existing key can move to a different
                    partition and its ordering can break — that's why we start with a few extra partitions."
dheema consumer -> aage wala (email API / DB) dheema -> consumer badhao (partition tak) · batch
                  · bhaari kaam alag topic
GARAM PARTITION -> ek bade customer ki key pe saara bojh
                  -> key todo (userId#1..4), us key ka pakka kram chhodo
                  (ya us customer ka alag topic) · consistent hashing ek garam key ko nahi bachata
broker disk    -> 2 TB tha, 10 guna = 20 TB -> retention 7 se 2-3 din · compaction · broker jodo
                  · tiered storage / S3
rebalance ki aandhi -> baar-baar deploy -> session-timeout theek karo · static membership · ek-ek karke deploy
producer       -> har jagah acks=all = dheema -> khabar ke hisaab se (payment = all, log = 1)

POOCHEGA: "What about a hot key / hot partition?"   -> upar GARAM PARTITION
          TERA JAWAB: "pehle machine badi karo, phir usi dabbe ko
                      kai hisson me todo" -> TODNA = sahi direction
          BADI MACHINE KAAFI KYUN NAHI: group me ek partition ko EK
          hi consumer padhta -> machine badi, padhne wala phir bhi ek
          MISAAL: user-9 ke saare event P2 pe -> key = "user-9#" +
                  (eventId % 4) -> 4 partition, 4 consumer saath padhein
          KEEMAT: user-9 ka POORA pakka kram gaya (har tukde ke andar bacha)
                  kram sach me chahiye -> key chhoti cheez pe rakho
                  (orderId / accountId), jahan kram chahiye bas wahan
POOCHEGA: "How would you scale this to 10x?"        -> message ka raasta chalo
POOCHEGA: "What's the single point of failure?"     -> leader (ISR promote), controller (KRaft quorum)
POOCHEGA: "How do you know it's working?"
          -> CONSUMER LAG · under-replicated partitions · DLQ size · alert
```

---

## POOCHE TO (gehra sawaal aaye tab)

```
API:      PRODUCER  send(topic, key, value) -> (partition, offset) rasid · acks = 0 / 1 / all
          CONSUMER  subscribe(topic, groupId) · poll(timeout) ek saath
          kai · commit(offset) · seek(partition, offset) = REPLAY
          ADMIN     createTopic(name, partitions, replicationFactor) / deleteTopic / describeTopic
          PULL hai, PUSH nahi: consumer apni speed se maangta, dheema consumer dab ke nahi marta
                               keemat: agle poll tak thodi der (push hota to
                               broker dhakelta -> dheema consumer doob jaata)
          send() ek saath kai bhejta (batch) -> isliye bahut tez
          "The queue's own API is deliberately tiny — send, poll, commit, seek. All the business logic lives
           in the consumer, not in the broker. That's why the broker can be so fast."

MESSAGE:  offset (BROKER deta) · timestamp · key (hash(key) % n) · value
          (JSON / Avro) · headers (traceId, eventType, retryCount)
DISK:     /topic-user-signup/partition-0/
             00000000000000000000.log     message, sirf jodte (segment ~1 GB)
             00000000000000000000.index   offset -> file me jagah
             00000000000512340000.log     segment bhara -> naya

DATA 3 KISM:  MESSAGE (bahut, tez) -> append-only file, bachav = copy + acks (DB nahi)
              NAKSHA (kam, 100% sahi: topic / partition / leader / ISR) -> pakka sahi (Zookeeper / KRaft-Raft)
                        kyun: do broker khud ko ek partition ka leader samjhein = SPLIT-BRAIN = data kharab
              CONSUMER OFFSET -> __consumer_offsets

KAB KAFKA:    bhejne wale ko jawab ka intezaar nahi (signup ke baad email / analytics) · EK khabar KAI service
              bahut saari khabar + replay (log, clickstream, audit)
              · bhejne / lene wale ki speed alag (bheed ka buffer)
KAB NAHI:     TURANT jawab chahiye -> REST / gRPC · poore system ka
              pakka kram · simple kaam ki line + har message ka retry
              -> RabbitMQ / SQS (Kafka bahut bhaari) · chhota
                 system, 2 service -> ek aur cheez sambhaalna mehnga
KEEMAT:       "thodi der baad sahi" (LAG) · duplicate sambhaalna padta · debug mushkil (flow bikhra)
              · ek aur cluster paalna (partition / retention / lag dekhna)

SHABD ATKE TO (bina shabd ke bolo):
   partition      -> "the topic is split into parts, and each part sits on a different machine"
   offset         -> "each part is like a numbered list, and every consumer remembers which number
                      it has read up to"
   consumer group -> "a set of consumers that act as one team and divide the parts among themselves"
   ISR            -> "the copies that are fully caught up with the main one"
   idempotent     -> "even if the same event comes twice, the work happens only once,
                      because we store the event id"
   compaction     -> "for each key we keep only the latest value and throw away the older ones"

CHHE BAAT (poora design ek-ek line me):
   1 KYUN        ek doosre ka intezaar todna -> producer bhej ke aage, consumer apni speed
   2 TOPIC/PART  partition = scale + append-only log + offset · key -> kram EK PARTITION me (poore topic nahi)
   3 GROUP       group ke andar baanto · alag group = sabko · kitne ek saath = partition ginti tak
   4 COPY        leader / follower · naya leader sirf ISR se · acks 0 / 1 / all = tezi vs bachav
   5 DELIVERY    at-least-once + idempotent consumer = asar exactly-once · retention -> replay
   6 TRADE-OFF   async + sabko + replay -> Kafka · turant jawab / poora kram -> Kafka NAHI

AUR SAWAAL (safar ke mod nahi):
  ek khabar 5 min ki?     -> poora tukda atka (kram se padhta): dheema kaam alag topic / pool
  offset matlab byte?     -> nahi, kram number 0, 1, 2; chhoti .index file batati file me kahan
  disk pe dheema nahi?    -> nahi: sirf aakhir me jodna = sequential, disk ka sabse tez kaam
  disk kab khaali?        -> padhne se nahi; sirf retention se (purana segment poora udao, card 10)
  10 tukde 3 broker pe?   -> barabar baante, kaunsa kahan = naksha Controller ka
  kitne partition?        -> jitne worker ek saath; shuru me thoda zyada (baad me badle to key ka tukda
                             hilta)
  acks ke option?         -> 0 = bhej ke bhaag (kho sakta) · 1 = sirf leader (click / log) · all = saari ISR
                             (paisa)
  Kafka me exactly-once?  -> sirf Kafka se Kafka (transactions); email / DB ke liye consumer khud pehchaane
  kuch na khoye, poora?   -> outbox + acks=all + kaam ke baad commit + eventId dedup + kharab ki alag line
                             (DLQ)
  rebalance me sab rukte? -> purane me haan (eager); cooperative (Kafka 2.4+) me sirf jinke badle
  RabbitMQ / SQS me kram? -> guarantee nahi: kai consumer ek line se; SQS FIFO ya ek consumer = kram, speed
                             kam
```

---

## HANDS-ON — EVENT KAHAN KHOTA HAI: 3 jagah crash karwa ke dekha

> Grill sawaal (bank): "paisa kata -> event SMS / fraud / statement tak. Ek bhi event kho na jaaye, kaise?"
> SAGA nahi: saga = faile kaam ko ULTA karna. Yahan ulta nahi, event RASTE me kho raha.
> Sahi soch jo thi: "pehle pakka likho" = OUTBOX.
> CODE: `04_HLD/HANDS_ON/04_event_loss/EventLossDemo.java` (nakli, Docker nahi) · line 24 `FIX = false / true` -> `java EventLossDemo.java`

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

ARCHETYPE B/F · CONCEPTS: [message-queues](../../FOUNDATIONS/07_message_queues.md) · [replication](../../FOUNDATIONS/05_database_replication.md) · [sharding/partition](../../FOUNDATIONS/06_database_sharding.md) · saath: [notification](../04_notification_system/04_notification_system.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
