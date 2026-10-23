# Message queue (Kafka): shuru se, ek kahani

Kram wahi jo interview me hota hai: **FR, phir NFR, phir numbers, phir design**. Design sabse seedhe dabbe
se shuru hota hai, aur har dikkat aane pe ek naya dabba judta hai. Diagram me peela dabba = abhi juda. Poori
file me ek hi kahani hai: Arpan ne Swiggy pe order kiya. Concepts kahani se padhne hon to
[kafka_concepts.md](kafka_concepts.md).

---

## 1. FR: kya karna hai

```
1. Order Svc ki khabar ("Arpan ne order kiya") kai service tak pahunche: SMS, Fraud, Analytics
2. Order Svc kisi ka intezaar na kare
3. ek user ki khabar kram me pahunche (order, phir pay, phir cancel)
scope ke bahar (poochh ke hata do): duniya bhar ke region, exactly-once (Kafka se bahar)
```

## 2. NFR: kaisa chalna chahiye

```
- khabar kabhi na khoye
- bheed me bhi chale (sale me 5,000 / sec)
- ek service dheemi ho to baaki na rukein
```

## 3. Numbers

```
- roz 10 crore khabar -> ~1,200 / sec, sale me ~5,000 / sec
- ek worker ~500 khabar / sec sambhal paata hai -> 5,000 / 500 = 10 tukde chahiye
- ek khabar ~1 KB -> roz ~100 GB -> 7 din rakho to ~700 GB -> 3 copy = ~2 TB disk
```

"10 tukde" kadam 3 me kaam aayega, aur "7 din, 3 copy" baad me.

---

## 4. Design

### Kadam 1: sabse seedha (Order Svc seedha sabko call kare)

Arpan ne order kiya. Order Svc order banata hai, phir khud SMS Svc, Fraud Svc aur Analytics ko ek-ek karke
call karta hai, har ek ke jawab ka intezaar karta hai, aur tab jaake Arpan ko "order ho gaya" bolta hai.

```mermaid
flowchart LR
    U["Arpan"] --> O["Order Svc"]
    O -- "call, intezaar" --> S["SMS Svc"]
    O -- "call, intezaar" --> F["Fraud Svc"]
    O -- "call, intezaar" --> A["Analytics"]
    style O fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Dikkat: SMS Svc dheema hai to Arpan ka order bhi dheema ho jaata hai. SMS Svc gira to order hi fail, jabki
order ka SMS se koi lena-dena nahi. Aur kal koi naya Coupon Svc aaya, to Order Svc ka code badalna padega.

Order Svc ko kisi ka intezaar nahi karna. Wo khabar kahin daal de aur aage badh jaaye, aur jise chahiye wo
wahan se le le. Par "kahin" ka matlab kahan?

### Kadam 2: khabar kahan daalein (Kafka topic, disk pe)

Order Svc khabar ek line (queue) me daal deta hai aur aage badh jaata hai. SMS, Fraud aur Analytics usi line
se padhte hain. Par line kaisi ho? Ye asli choice hai:

```
SERVER KI MEMORY ME LIST  -> server restart = saari khabar gayab. NFR "kabhi na khoye" toota
RABBITMQ (aam queue)      -> khabar ek ne padhi to line se nikal gayi (mit gayi).
                             SMS ne padhi to Fraud ko mili hi nahi. Har service ke liye alag line banao
KAFKA (topic, disk pe)    -> khabar disk pe likhi jaati hai, padhne se mit-ti NAHI.
                             SMS, Fraud, Analytics teeno wahi khabar padhte hain, apni-apni jagah se
```

**Isliye Kafka.** Line ko Kafka me **topic** kehte hain (`order-events`). Padhne se khabar mit-ti nahi,
isliye ek hi khabar teen service padh leti hain, aur kal Coupon Svc aaya to wo bas padhna shuru kar degi,
Order Svc me ek line nahi badalti. Har service apna bookmark rakhti hai ki kahan tak padha (**offset**), to
Fraud dheema hua to bas wo peeche rahega, SMS nahi rukega. Disk pe hai, to kuch din (jaise 7 din) rakhi
rehti hai, aur kisi service me bug tha to bookmark peeche karke dobara padh sakte hain.

```mermaid
flowchart LR
    U["Arpan"] --> O["Order Svc"]
    O -- "khabar daali, aage badha" --> T[("KAFKA<br/>topic order-events")]
    T --> S["SMS Svc"]
    T --> F["Fraud Svc"]
    T --> A["Analytics"]
    style T fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Par Diwali sale me har second 5,000 order. Saari khabar ek topic me, aur topic ek hi machine pe. Upar se SMS
ka ek worker sirf 500 / sec sambhal paata hai. Line lambi hoti ja rahi hai.

### Kadam 3: bheed (topic ke tukde = partition, aur key)

Ilaaj: topic `order-events` ko andar se **10 tukdon** me baanto, har tukda alag machine pe. Ek tukde ko
**partition** kehte hain. 10 isliye ki numbers me nikla tha: sale me 5,000 / sec, ek worker 500 / sec, to
5,000 / 500 = 10. Ek tukda ek worker padhega, to 10 tukde = 10 worker ek saath.

Par tukde karte hi nayi dikkat aati hai: kram. Arpan ki 3 khabar (order, pay, cancel) alag-alag tukdon me
gayi, to padhne wala kis kram me padhe, koi bharosa nahi. "cancel" pehle padh liya to gadbad. Kafka me kram
sirf ek tukde ke andar pakka hota hai, poore topic me nahi.

Ilaaj: **key**. Order Svc har khabar ke saath ek key deta hai, `userId`. Same key hamesha same tukde me
jaati hai:

```
tukda = hash(arpan) % 10 = 1   -> Arpan ki teeno khabar P1 me, kram se: order, pay, cancel
Ravi ki khabar                -> P6 me (alag tukda, Arpan se koi lena-dena nahi)
```

Ye chat wali baat hi hai: wahan kram sirf ek chat ke andar chahiye tha, yahan sirf ek user ke andar.

```mermaid
flowchart LR
    O["Order Svc<br/>key = userId"] --> P0 & P1 & P9
    subgraph T["KAFKA · topic order-events · 10 tukde"]
        P0[("P0")]
        P1[("P1 · Arpan<br/>order, pay, cancel")]
        P9[("P9")]
    end
    style P0 fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style P1 fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style P9 fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Tukde 10 se 20 kiye to? Tukda = hash(key) % ginti hai. Ginti badli to key ka tukda badal jaata hai: Arpan ki
purani khabar P1 me rahegi, nayi kahin aur, thodi der kram toot sakta hai. Isliye shuru me hi zaroorat se
2-3 guna tukde rakhte hain.

Par SMS Svc ki 3 machine lagayi taaki 10 tukde jaldi padhe jaayein. Teeno machine saare tukde padhne lagin,
aur Arpan ko ek order ke 3 SMS chale gaye.

### Kadam 4: 3 machine, 3 SMS (consumer group)

Teeno machine saare tukde padh rahi thi, to Arpan ki khabar teeno ke paas pahunchi aur teeno ne SMS bhej diya.

Ilaaj: teeno machine ko ek **team** bana do, sabko ek hi team ka naam do (`group.id = "sms"`). Isse
**consumer group** kehte hain. Kafka ka niyam hai: ek team ke andar har tukda sirf ek machine ko milta hai.

```
TEAM sms (3 machine), 10 tukde:
  machine A -> P0, P1, P2, P3     (P1 = Arpan, to Arpan ka SMS sirf A bhejegi)
  machine B -> P4, P5, P6
  machine C -> P7, P8, P9
```

Arpan ko ab ek hi SMS jaata hai. Fraud alag team hai (`group.id = "fraud"`), to use apni poori copy milti
hai, aur har team apna bookmark (offset) alag rakhti hai. Machine B gir gayi to Kafka uske tukde (P4, P5,
P6) A aur C me baant deta hai, ise **rebalance** kehte hain.

11vi machine lagayi to? 10 hi tukde hain, aur ek tukda ek hi machine ko milta hai. 11vi machine ke liye koi
tukda bacha hi nahi, wo khaali baithegi. Tez karna hai to pehle tukde badhao.

```mermaid
flowchart LR
    subgraph T["topic order-events"]
        P1[("P1 · Arpan")]
        P5[("P5")]
        P8[("P8")]
    end
    subgraph S["TEAM sms"]
        A["machine A"]
        B["machine B"]
        C["machine C"]
    end
    P1 --> A
    P5 --> B
    P8 --> C
    T -. "poori copy" .-> F["TEAM fraud"]
    style S fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Par machine A ne Arpan ka SMS bheja, aur bookmark aage karne se pehle gir gayi. P1 machine B ke paas gaya, B
ne bookmark dekha (abhi bhi purana), aur wahi khabar dobara padhi. Arpan ko 2 SMS.

### Kadam 5: bookmark se pehle gira, 2 SMS (pehle kaam phir commit + eventId)

Machine A ka kaam teen kadam ka tha: khabar padho, SMS bhejo, phir Kafka ko batao "yahan tak padh liya"
(bookmark aage karna = **commit**). SMS bheja aur commit se pehle gir gayi. Bookmark purana hi raha, to B ne
wahi khabar dobara padhi.

Ulta karte (pehle commit, phir SMS) to commit hua aur SMS se pehle gira, bookmark aage chala gaya, aur ye
SMS kabhi jaata hi nahi. Ye usse bhi bura hai. Isliye kram pakka hai: **pehle kaam, phir commit**. Khabar
kabhi khoti nahi, bas kabhi-kabhi dobara aa jaati hai. Iska naam hai **at-least-once**: kam se kam ek baar,
kabhi do baar.

Dobara aayi khabar pakadne ke liye har khabar ki apni id hoti hai (**eventId**, jaise `E-6`). SMS bhejne se
pehle machine dekhti hai ki ye id pehle ho chuki ya nahi:

```
E-6 aayi -> Redis / DB me dekho: E-6 ho chuki?
             nahi -> SMS bhejo, E-6 likh do, commit
             haan -> chhod do (dobara SMS nahi), commit
```

Ye chat wala `clientMsgId` aur banking wali idempotency key hi hai: dobara aaye to id se pehchaano. Ye
pehchaanna padhne wale (SMS Svc) ka kaam hai, Kafka ka nahi.

Exactly-once (theek ek baar) Kafka sirf Kafka se Kafka tak deta hai. SMS bahar ki cheez hai, wahan eventId
hi rasta hai. Interview me bolo: "at-least-once + eventId se dohra kaam nahi."

```mermaid
flowchart LR
    P1[("P1 · Arpan")] -- "E-6 cancel" --> A["TEAM sms<br/>1. SMS 2. commit"]
    A -- "E-6 pehle ho chuki?" --> R[("Redis / DB<br/>eventId")]
    style R fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Par Arpan ki khabar P1 me thi, aur P1 Kafka ki ek machine (broker) pe tha. Us machine ki disk mar gayi. P1
sirf wahin tha, to Arpan ki saari khabar hamesha ke liye gayi.

### Kadam 6: Kafka ki machine mari (3 copy, acks=all, min.insync)

Ilaaj: har tukde ki **3 copy**, teen alag machine pe. Jis machine pe likhna-padhna hota hai wo **leader**
hai, baaki do copy. P1 ka leader machine B pe, copy A aur C pe. B ki disk mari to A ya C naya leader ban
jaata hai, aur Arpan ki khabar bach jaati hai. Naya leader kaun banega, ye Kafka ki ek machine tay karti hai
jo sab pe nazar rakhti hai: **Controller**.

Par ek pench: leader B ne khabar likhi aur turant Order Svc ko "ho gaya" bol diya, aur copy tak khabar
pahunchne se pehle hi B mar gayi. Copy ke paas wo khabar thi hi nahi, to khabar gayi.

Ilaaj: **acks=all**. Leader "ho gaya" tabhi bolta hai jab pakki copies pe bhi likh jaaye.

```
acks=0    bhej ke bhool jao (sabse tez, khabar kho sakti)
acks=1    sirf leader ne likha to "ho gaya" (leader mara to aakhri khabar gayi)
acks=all  saari pakki copies pe likha tab "ho gaya"     <- order / paisa ke liye
```

"Pakki copy" wo hai jo leader ke saath bilkul taaza chal rahi hai. Unki list ko **ISR** kehte hain. Peeche
chal rahi copy ko leader nahi banate, warna aakhri khabar kho jaayegi.

Sab copies gir gayi aur sirf leader bacha, to `acks=all` me "saari pakki copy" sirf leader khud hai, aur wo
"ho gaya" bol dega jabki copy ek bhi nahi. Isliye saath me **min.insync = 2**: kam se kam 2 pakki copy na
hon to likhna hi band. Khabar khone se behtar hai thodi der ruk jaana.

```mermaid
flowchart LR
    O["Order Svc"] -- "acks=all" --> L[("machine B<br/>P1 leader")]
    L -- "copy" --> A[("machine A<br/>P1 copy")]
    L -- "copy" --> C[("machine C<br/>P1 copy")]
    style A fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style C fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Par ab bhejne wala bacha. Arpan ne order cancel kiya. Order Svc ne DB me likha "order = cancel", aur Kafka
ko khabar bhejne se pehle gir gaya. DB me cancel ho gaya, par SMS aur Fraud ko kabhi pata hi nahi chala.

