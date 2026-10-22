# Kafka: shuru se, dabbe aur teer se

Ek kahani: Arpan ne order kiya. Order Svc ko khabar email wale ko deni hai ("order #101 bana, email bhejo").
Shuru me sab ek dabbe me. Har kadam pe ek dikkat aati hai, ek dabba ya ek shabd judta hai.
Jo shabd tu bolega, sawaal wahi se uthega. Isliye har shabd ke neeche uske sawaal aur jawab:

```
DABBA -> gir gaya?  bheed aa gayi?  dheema ho gaya?
TEER  -> pahuncha hi nahi?  do baar pahuncha?  der se pahuncha?
SHABD -> ye kyun, doosra kyun nahi?
```

Ginti (interviewer pooche to):
```
roz 10 crore khabar / ~86,400 sec = ~1,200 har second, sale me ~5,000 har second
ek khabar ~1 KB -> roz ~100 GB, 7 din ~700 GB, 3 copy -> ~2 TB disk
```

---

## Kadam 0: sabse seedha

```
[ Arpan ] --order--> [ Order Svc ] --"email bhejo"--> [ Email Svc ]
```

Order Svc khud Email Svc ko bulata hai aur jawab aane tak rukta hai.

Dikkat:
```
Email Svc dheema (5 sec)  -> Arpan ka order bhi 5 sec ruka
Email Svc band            -> order hi fail, jabki order to sahi tha
```

---

## Kadam 1: beech me ek line (queue)

```
[ Order Svc ] --khabar--> [ LINE ] --uthata--> [ Email Svc ]
                           #101 #102 ...
```

Order Svc khabar line me daal ke aage badh jaata hai. Email Svc apni speed se uthata hai.
Email band ho, tab bhi order chalta hai, khabar line me padi rehti hai.

Shabd bola: **queue (line)**. Sawaal:
```
line kahan rakhi?        -> ek machine ki RAM me
wo machine restart hui?  -> RAM khaali, saari khabar gayi        <- AGLA KADAM
```

---

## Kadam 2: line disk pe, aur "kahan tak padha" ka bookmark

```
[ Order Svc ] --khabar--> [ KAFKA: disk pe log ] -----> [ Email Svc ]
                           0  1  2  3 ... 500             bookmark: 120 tak padha
                           (sirf aakhir me judta)          (offset)
```

Khabar disk pe likhi jaati hai, padhne se mitti nahi. Email Svc yaad rakhta hai kahan tak padha (offset).
Restart hua to bookmark dekh ke 121 se aage padhta hai.

Shabd bola: **log, offset**. Sawaal:
```
padhne se khabar mitti kyun nahi?  -> taaki dobara padh sakein (replay), aur doosri service bhi padhe
bookmark kahan rakha?              -> Kafka khud rakhta hai
kal ki khabar dobara padhni hai?   -> bookmark peeche kar do, khabar disk pe padi hai
disk bharti gayi?                  -> <- KADAM 10
121 number wali khabar disk pe kahan hai?  -> Kafka ek chhoti index file rakhta: number -> jagah
```

---

## Kadam 3: bheed aayi, ek worker kam pada

```
Sale: har second 5,000 order. Ek Email worker sirf 500 utha paata.

[ LINE ] --> [ Worker ]      line lambi hoti ja rahi, Arpan ka order fasa
```

Ek line ko ek waqt me ek hi worker padh sakta hai. Do worker ek line se uthayenge to ek hi order
do baar utha sakte hain, ya kram bigad jaayega. Isliye line ke tukde karo:

```
             [ Tukda 0 ] ---> [ Worker 0 ]
 khabar -->  [ Tukda 1 ] ---> [ Worker 1 ]
             ...               ...
             [ Tukda 9 ] ---> [ Worker 9 ]

                              [ Worker 10 ]  <- koi tukda nahi bacha, khaali
```

10 tukde, 10 worker, 10 x 500 = 5,000 har second.

Shabd bola: **partition (tukda)**. Sawaal:
```
11va worker lagaya?          -> khaali baithega. Pehle tukde badhao
10 tukde, 5 worker?          -> ek worker 2 tukde padhega, chalega, bas dheema
kitne tukde rakhein?         -> jitne worker ek saath chahiye. 5,000 / 500 = 10. Thoda zyada rakho
tukde baad me badhaye?       -> chalta hai, par kram toot sakta hai      <- KADAM 4
tukde alag machine pe?       -> haan, 3 machine (broker) me barabar baante, ek pe bojh nahi
kaunsa tukda kis machine pe?  -> Controller ka naksha rakhta hai
```

---

## Kadam 4: kram toota

```
Arpan ne naam badla: pehle "A", phir "B".

 "A" --> [ Tukda 1 ] --> [ Worker 1 ]  (dheema)   --> baad me chhapa
 "B" --> [ Tukda 5 ] --> [ Worker 5 ]  (tez)      --> pehle chhapa

 aakhir me purana naam "A" bacha
```

Kram sirf ek tukde ke andar milta hai. To Arpan ki saari khabar ek hi tukde me bhejo.
Har khabar ke saath ek key do: key = Arpan ka userId. Same key hamesha same tukde me jaati hai.

```
 "A" (key Arpan) --> [ Tukda 1 ]  --> pehle A
 "B" (key Arpan) --> [ Tukda 1 ]  --> phir B
```

Shabd bola: **key = userId**. Sawaal:
```
key nahi di?                     -> khabar kisi bhi tukde me, kram nahi bachta
poore topic ka ek kram chahiye?  -> ek hi tukda, phir wahi bheed. Aksar ek user ka kram kaafi
ek celeb ki itni khabar ki uska tukda bhar gaya?  -> key me number jodo (userId#1, #2), kram chhodna padega
tukde 10 se 20 kiye?             -> key ka tukda badal gaya, kuch der kram toota. Isliye shuru me zyada rakho
Order Svc ne A bheja, wo network me atka, retry kiya, tab tak B pahunch gaya?
                                 -> Order Svc har khabar pe number lagata (1, 2). Kafka number dekh ke kram
                                    theek rakhta aur dobara aayi khabar nahi likhta (idempotent producer)
RabbitMQ / SQS me kram?          -> pakka nahi milta. SQS FIFO ya ek hi padhne wala ho tab, par speed kam
```

---

## Kadam 5: Email Svc ki 3 machine, Arpan ko 3 email

```
             [ Tukda 7 ] ---> [ Email machine 1 ]
                         ---> [ Email machine 2 ]   teeno ne padha, teen email
                         ---> [ Email machine 3 ]
```

Teeno machine ko ek hi team bana do (consumer group, ek hi group.id).
Team ke andar har tukda sirf ek machine ko milta hai.

```
 TEAM "email"
 [ Tukda 0-3 ] ---> [ machine 1 ]
 [ Tukda 4-6 ] ---> [ machine 2 ]
 [ Tukda 7-9 ] ---> [ machine 3 ]    Tukda 7 sirf machine 3 ke paas -> ek email
```

Shabd bola: **consumer group**. Sawaal:
```
team ki ek machine gir gayi?      -> uske tukde baaki machine ko mil jaate (rebalance)   <- KADAM 9
har machine ka group.id alag?     -> Kafka teeno ko alag team samjhega, phir 3 email
team me machine badhani hai?      -> tukde bhi badhao, warna nayi machine khaali
```

---

## Kadam 6: wahi order Analytics aur Dashboard ko bhi chahiye

```
                        +--> TEAM email      bookmark 100
 [ order ki khabar ] ---+--> TEAM analytics  bookmark 98
                        +--> TEAM dashboard  bookmark 95 (dheema)
```

Har service ki apni team, apna bookmark. Har team ko poori khabar milti hai.
Dashboard dheema hai to bhi Email nahi rukta.

Shabd bola: **har service ka apna group**. Sawaal:
```
teeno ko ek hi team me daal dete?  -> team me ek khabar ek ko, baaki do ko kuch nahi milta
nayi service judni hai?            -> bas naya group. Order Svc me kuch nahi badalta
nayi service shuru se padhe ya ab se?  -> tay karna padta hai (auto.offset.reset)
```

---

## Kadam 7: Kafka ki machine hi mar gayi

```
 [ Order Svc ] --khabar--> [ Kafka machine A ]   disk mari -> khabar gayi
```

Har tukde ki 3 copy, 3 alag machine pe. Ek leader (likhna yahin), do copy.

```
 [ Order Svc ] --khabar--> [ Machine A: LEADER ]
                                 |-- copy --> [ Machine B ]
                                 |-- copy --> [ Machine C ]

 A mari -> B naya leader, khabar bachi
```

Shabd bola: **leader, copy (replica), acks**. Sawaal:
```
A ne "ho gaya" bola, copy banne se pehle mar gaya?  -> khabar gayi. Isliye "ho gaya" tab bolo jab copy
                                                       bhi likh jaaye (acks=all)
ek copy peeche chal rahi?           -> sirf poori taaza copy "pakki" (ISR). Leader pakki me se hi banta
dono copy bhi gayi, sirf A bacha?   -> likhna rok do (min.insync = 2). Khone se behtar rukna
naya leader kaun chunta?            -> Controller (KRaft). Ek hi faisla, do leader kabhi nahi
acks ke teen tareeke?               -> 0 = bhej ke bhool jao, 1 = sirf leader (click, log ke liye theek),
                                       all = saari pakki copy (paisa, order)
3 copy ke liye kitni machine?       -> kam se kam 3 alag machine
pehle kya poochho?                  -> khabar kho sakti hai ya bilkul nahi? Isi se acks tay hota
```

---

## Kadam 8: worker beech me gira

```
 [ Tukda ] ---> [ Email worker ] ---> email bheja ---> bookmark aage kiya
                                   ^
                          yahan gira (email gaya, bookmark nahi hila)
```

Bookmark nahi hila, to restart pe wahi khabar dobara aati hai. Arpan ko 2 email.
Ulta karo (pehle bookmark, phir email) to beech me girne pe email kabhi nahi jaata. Ye usse bura hai.

To kram: pehle kaam, phir bookmark. Aur dobara aaye to pehchano:

```
 khabar E-5 aayi --> [ Redis: E-5 pehle dekha? ] --haan--> chhod do
                                                 --nahi--> email bhejo, E-5 likh do, bookmark aage
```

Shabd bola: **pehle kaam phir commit, eventId**. Sawaal:
```
"duplicate kabhi nahi aayega"?  -> galat. Khabar khoti nahi, dobara aa sakti. Padhne wala pehchaanta
userId se pehchano?             -> galat. Arpan ka agla sahi order bhi chhoot jaayega. Pehchaan eventId se
Redis se E-5 jaldi mit gaya?    -> dobara aayi to pehchani nahi. Utni der rakho jitni der dobara aa sakti
exactly-once milta hai?         -> sirf Kafka se Kafka tak. Email / DB ke liye padhne wala khud pehchaane
```

---

## Kadam 9: bhejne wala hi gira

```
 [ Order Svc ] --1. DB me order 101--> [ DB ]
               --2. Kafka ko khabar--> [ Kafka ]
                 ^
          beech me gira: order bana, khabar gayi hi nahi
```

Khabar bhi usi DB me, order ke saath, ek hi transaction me likho (outbox table).
Ek chhota relay har second outbox se padh ke Kafka ko bhejta hai.

```
 [ Order Svc ] --ek transaction--> [ DB: order 101 + outbox E-101 (sent = false) ]
                                              |
                                   [ Relay ] padhta --> [ Kafka ] --> OK --> sent = true
```

Shabd bola: **outbox, relay**. Sawaal:
```
order likhne se pehle gira?        -> kuch nahi likha. Arpan dobara order karega, galat kuch nahi
relay ne bheja, sent = true se pehle gira?  -> khabar dobara jaayegi. Padhne wala E-101 pehchaan lega
outbox table bharti gayi?          -> sent = true wali row roz saaf karo
saga kyun nahi?                    -> saga fail hua kaam ulta karta. Yahan khabar ko bachana hai
poora bachav ek line me?           -> outbox + acks=all + pehle kaam phir bookmark + eventId + DLQ
```

---

## Kadam 10: line lambi, aur disk bhari

```
 [ Tukda 4,5,6 ] ---> [ Email machine 2 ]  mar gayi
                ---> baant diye: 4,5 machine 1 ko, 6 machine 3 ko   (rebalance)

 ek khabar baar-baar fail --> [ alag line: DLQ ]  baaki chalte rahein
 kitna padhna baaki (lag) badh raha --> alert

 disk: [ din 1 ][ din 2 ] ... [ din 7 ][ aaj ]
          ^ 7 din se purana poora tukda ek saath uda do (retention)
```

Shabd bola: **rebalance, lag, DLQ, retention**. Sawaal:
```
rebalance me sab ruk jaate?        -> purane tareeke me haan. Naye (cooperative) me sirf jinke tukde badle
lag kaise naapein?                 -> aakhri khabar ka number - team ne kahan tak padha
lag badh raha?                     -> alert + worker badhao. Ek khabar 5 min le to use alag pool me bhejo
DLQ wali khabar ka kya?            -> theek karke dobara chalao
padhte hi delete kyun nahi?        -> dobara padhna (replay) band ho jaata
Email Svc 7 din se zyada band?     -> wo khabar ud chuki. Retention sabse lambi chhutti se lamba rakho
ek user ka sirf aakhri naam chahiye?  -> compaction: har key ki sirf aakhri value rakho
Kafka ka asli number kya?          -> disk ki jagah, kyunki Kafka khabar rakhta hai, sirf aage nahi bhejta
```

---

## Poora design ek saath

```
 [ Order Svc ] --ek txn--> [ DB + outbox ] --relay--> [ KAFKA ]
                                                        topic "order", 10 tukde, key = userId
                                                        har tukda 3 copy, acks=all, 7 din
                                                           |
                       +-----------------------------------+------------------+
                       |                                   |                  |
                 TEAM email                         TEAM analytics      TEAM dashboard
                 pehle kaam, phir bookmark
                 eventId se pehchaan, kharab -> DLQ, lag alert
```
