# Stock trading (Groww / Zerodha): shuru se, ek kahani

Kram wahi jo interview me hota hai: **FR, phir NFR, phir numbers, phir design**. Design sabse seedhe dabbe
se shuru hota hai, aur har dikkat aane pe ek naya dabba judta hai. Diagram me peela dabba = abhi juda. Poori
file me ek hi kahani hai: Arpan TCS ke 10 share 3500 pe kharidna chahta hai, Suresh apne 10 TCS 3500 pe
bechna chahta hai.

Ye paise wali tikdi ka chautha design hai: jo [banking](../13_banking_system/13_banking_system.md),
[payment](../06_payment_system/06_payment_system.md) ya [BookMyShow](../09_bookmyshow/09_bookmyshow.md) me pehle
se hai, wahan bas ek line + link. Poora sirf wahi jo stock ka apna hai (order book, matching, 9:15 ki bheed).

---

## 1. FR: kya karna hai

```
1. buy / sell order dalna (LIMIT: "3500 pe hi", MARKET: "jo bhaav abhi hai")
2. order milana (Arpan ka buy + Suresh ka sell = sauda)
3. sauda hua to paisa Arpan se Suresh, share Suresh se Arpan
4. live price dikhana, order ka haal (cancel bhi)
scope ke bahar: margin / risk, IPO, mutual fund
```

## 2. NFR: kaisa chalna chahiye

```
- paisa aur share hamesha sahi ("thodi der baad sahi" nahi)
- imaandar: jo pehle aaya, wo pehle mile
- 9:15 ki bheed jhele, har cheez ka record (audit)
```

## 3. Numbers

```
- 50 lakh order / din, market ~6 ghante (~20,000 sec) -> ~250 order / sec aam
- 9:15 pe jhatka: 10,000+ order / sec
- live price: ~10 lakh logon ka app khula, har second naya bhaav -> poochhne ki jagah PUSH
```

---

## 4. Design

### Kadam 1: sabse seedha (Order Svc + matching engine + DB)

Stock ka dil hai **order book**: har share ki do list.

```
TCS order book
  KHARIDNE WALE (buy)          BECHNE WALE (sell)
  Arpan  10 @ 3500             Suresh 10 @ 3500   <- bhaav mila -> SAUDA
  Mohan   5 @ 3490             Ravi    8 @ 3510
```

Arpan ne order dala. Order Svc use **matching engine** ko deta hai. Engine dekhta hai: koi bechne wala 3500 ya
usse kam pe hai? Suresh hai, sauda ho gaya. Phir DB me likha jaata hai: Arpan ka paisa ghata, share badhe;
Suresh ka ulta. Kram ka niyam: ek hi bhaav pe do bechne wale hon, to jo pehle aaya usse pehle (price-time
priority).

```mermaid
flowchart LR
    U["Arpan: buy TCS 10 @ 3500"] --> O["Order Svc"]
    O --> M["Matching engine<br/>(TCS order book)"]
    M -- "sauda" --> D[("SQL DB<br/>paisa + share")]
    style O fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style M fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Dikkat: Arpan ne "Buy" dabaya, network atka, app ne dobara bheja (ya do baar tap). Order book me Arpan ke do
order lag gaye, 20 share kharid liye gaye, jabki use 10 chahiye the.

### Kadam 2: ek tap, ek order ([banking kadam 3](../13_banking_system/13_banking_system.md#kadam-3-network-gaya-app-ne-dobara-bheja-idempotency-key) wali key)

App har order ke saath apni ek id bhejta hai: `client_order_id = A-501`. Orders table me
`UNIQUE(client_order_id)`.

```
pehla Buy  : A-501 -> table me nahi -> order likho -> order book me
dobara Buy : A-501 -> pehle se hai  -> "ho chuka", wahi purana order lautao. Book me dusra nahi
```

```mermaid
flowchart LR
    U["Arpan: Buy (A-501)"] --> O["Order Svc"]
    O -- "UNIQUE client_order_id" --> T[("Orders table")]
    O -- "sirf pehli baar" --> M["Matching engine"]
    style T fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Ye bilkul [banking kadam 3](../13_banking_system/13_banking_system.md#kadam-3-network-gaya-app-ne-dobara-bheja-idempotency-key) hai, bas paise ki jagah order. Order book me jaane se pehle ye check, to dohra
order kabhi book tak pahunchta hi nahi.

Par Arpan ke wallet me 50,000 hain. Usne ek saath do order daale: TCS 30,000 ka aur INFY 30,000 ka. Dono
Order Svc pe saath pahunche, dono ne dekha "50k hai, chalega", dono lag gaye. 60,000 ke order, paisa 50,000.

### Kadam 3: paisa BLOCK (BMS ka seat HOLD, bas balance pe)

Wallet me do khaane: **available** (khula paisa) aur **blocked** (order ke liye ruka paisa). Engine tak
jaane se pehle us order ka paisa block, ek hi query me:

```sql
UPDATE wallet SET available = available - 30000, blocked = blocked + 30000
WHERE user = 'Arpan' AND available >= 30000;
```

```
shuru:  available 50,000 | blocked 0
TCS  :  50k >= 30k -> 1 row badli -> available 20,000 | blocked 30,000 -> order book me
INFY :  20k >= 30k? nahi -> 0 row -> "paisa kam hai", order lagta hi nahi
```

```mermaid
flowchart LR
    O["Order Svc"] -- "paisa BLOCK (ek query)" --> W[("Wallet<br/>available | blocked")]
    O -- "1 row badli to hi" --> M["Matching engine"]
    style W fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Ye [BMS kadam 3](../09_bookmyshow/09_bookmyshow.md#kadam-3-seat-hold-5-minute-umar-ke-saath-bms-ka-apna) jaisa: wahan seat HELD, yahan paisa BLOCKED. Query wahi
[banking kadam 4](../13_banking_system/13_banking_system.md#kadam-4-do-kataai-ek-saath-balance--400-check--kaato-ek-query-me) wali: "check + kaato ek query me".

Aage kya:
- sauda hua: blocked se paisa Suresh ko (Kadam 6).
- order cancel / din khatam, bika nahi: blocked wapas available (BMS ka "hold mara to seat FREE").

Bechne wale pe bhi yahi: Suresh ke paas 10 TCS. Usne do jagah "sell 10" daala. Pehle pe 10 share block, doosre
pe "share kam hain". Ek share do baar nahi bik sakta.

Par 9:15 baje market khula, 10,000 order / sec, aadhe sirf TCS pe. Ek hi TCS order book pe kai thread ek saath
haath daal rahe hain. Do thread ne ek hi Suresh ko do kharidne walon se mila diya, book bigad gayi.

### Kadam 4: ek share ki book, ek hi thread (stock ka apna)

Har share ki apni order book, aur har book ko **sirf ek thread** chalata hai. Us share ke saare order ek line
(queue) me lagte hain (abhi engine ki RAM me; Kadam 5 me yahi line log banegi: disk + standby), thread
unhe ek-ek karke uthata hai.

```
TCS ke order  --> [TCS line] --> TCS thread (akela) --> TCS order book
INFY ke order --> [INFY line] --> INFY thread (akela) --> INFY order book
RELIANCE ...  --> [RELIANCE line] --> RELIANCE thread --> ...

line me kram = jo pehle aaya wo pehle milta (imaandari, NFR wala)
```

```mermaid
flowchart LR
    O["Order Svc"] --> LT[("TCS line")]
    O --> LI[("INFY line")]
    LT --> TT["TCS thread (akela)<br/>TCS order book"]
    LI --> TI["INFY thread (akela)<br/>INFY order book"]
    style LT fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style LI fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Ab kya theek hua:
- ek book pe ek hi haath, to Suresh do logon se kabhi nahi milta. Lock bhi nahi chahiye.
- kram line se tay: jo pehle line me laga, wo pehle milega.
- alag share alag thread pe (ya alag machine pe), to TCS aur INFY saath-saath chalte hain.

Lock kyun nahi (asli choice): book pe lock bhi chal jaata, par 9:15 pe saare thread usi ek lock ki line me
khade honge, aur lock pehle kise mile ye tay nahi, to "pehle aaya pehle mila" toot jaata. Akela thread RAM me
bahut tez, aur kram apne aap sahi.

RAM matlab? Matching engine wali machine ki apni memory (Java program ka heap). Order book wahin ek data
structure: buy aur sell ki do sorted list (bhaav + time ke kram se). Redis ya DB nahi, warna har milaan pe
network ka chakkar, aur 9:15 pe wahi dheema. Isi wajah se machine giri to book bhi gayi (agla kadam).

Ek thread TCS ki bheed jhel lega? Haan. Book RAM me, ek order milana microseconds ka kaam. Ek thread ek second
me lakhon order nipta sakta (mera andaza, asli number exchange pe nirbhar). TCS ke 5,000 / sec usse bahut kam.

Par order book RAM me hai. Dopahar 12 baje matching machine gir gayi, poori TCS order book gayab. Kisne kis
bhaav pe kharidne ka order dala tha, kuch nahi bacha.

### Kadam 5: engine giri (log + standby, ack ke baad milaan)

Dikkat: order book engine ki RAM me hai. Machine giri to book gayi, aur restart tak TCS ka trading ruka.

Ilaaj: engine har aane wala **order** (book nahi) pehle pakka karta hai, phir milata hai, isi kram me:

```
order 501 "Arpan buy 10 @ 3500"
  1. apni disk pe LOG me likho
  2. STANDBY machine ko bhejo
  3. standby bole "mil gaya" (ack)  <- iske BAAD hi milao
  4. milao
12:00 primary gira -> standby ke paas 1..501 sab, usi kram me -> book wahi -> standby primary ban gaya
```

```mermaid
flowchart LR
    O["Order Svc"] --> P["Primary engine<br/>(RAM book)"]
    P -- "1. likho" --> L[("apni disk: LOG")]
    P -- "2. order bhejo" --> S["Standby engine<br/>(wahi book banti)"]
    S -- "3. ack" --> P
    style L fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style S fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Ack ka intezaar kyun: engine ne bheja aur turant gira, standby tak pahuncha hi nahi. Agar engine pehle hi
milaan kar chuka hota, to wo sauda standby ko pata hi nahi. Ack = standby ke paas order pakka.

Standby ko order kyun, book kyun nahi: standby wahi program chala raha, wahi order usi kram me milata hai, to
uski RAM me apne aap wahi book banti hai. Ye [banking kadam 7](../13_banking_system/13_banking_system.md#kadam-7-salary-ki-bheed--ek-hi-db-kai-machine--db-ki-sync-copy) ki sync copy jaisa hi.

Ek waqt me ek hi engine: heartbeat se tay hota hai ki primary mar gaya, tabhi standby kaam shuru karta hai.
Purana primary wapas aaye to wo standby ban ke baithta hai. Do engine ek book pe chal gaye to ek Suresh do
baar bik jaayega (**split brain**, sabse bada khatra).

Snapshot: naya standby subah 9:15 se poora log padhe to minute lagenge. Har minute book ki photo (snapshot);
naya standby aakhri photo + uske baad ka chhota log padhta hai.

Kafka kyun nahi yahan: Kafka me log rakhna bhi chalta, par har order pe uska chakkar milli-second ka hai, apna
log + seedha copy usse kahin tez (andaza: ~100 micro-second vs 2-5 ms). Exchange ke liye ye farak bada hai.

### Kadam 6: sauda hua, kai logon ko batana (yahan Kafka asli wajah se)

Dikkat: sauda S-88 hua (Arpan ne Suresh se 10 TCS 3500 pe liye). Ab ek saath kai kaam baaki:

```
sauda S-88 --> 1. Arpan ka paisa kaato, Suresh ko do     (settlement)
           --> 2. Arpan ke portfolio me 10 TCS           (settlement)
           --> 3. Arpan ko "10 TCS mil gaye" SMS / push  (notification)
           --> 4. audit / record                         (NFR "har cheez ka record")
           --> 5. TCS ka naya bhaav 3500                 (aage)
```

TCS thread 9:15 pe in paanchon ka intezaar nahi kar sakta, warna milaan dheema. Par sauda khona bhi nahi.

Galat raaste:
- engine khud har kaam kare: har sauda pe 5 jagah call, thread wahin atka.
- engine seedha Kafka ko bheje: ms-ms ka intezaar, aur milaan ke baad bhejne se pehle gira to sauda kahin likha
  hi nahi.

Sahi: engine sauda bhi usi Kadam 5 wale log me likhta (jaise order likhta tha). Ek chhota **relay** log padh ke
sauda **Kafka** ke `trades` topic me daalta. Paanchon kaam karne wale wahin se padhte.

```mermaid
flowchart LR
    E["Engine"] -- "sauda S-88" --> L[("apna LOG")]
    L --> R["Relay"]
    R --> K[("Kafka: trades")]
    K --> ST["Settlement"]
    K --> N["Notification"]
    K --> A["Audit"]
    K --> PS["Price Svc"]
    style R fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style K fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Kafka hi kyun: dikkat thi "ek baat, kai sunne wale, aur bolne wala ruke nahi". Kafka yahi karta: ek baar likha,
sab apni raftaar se padhte, padhne se khabar mitti nahi. Ye [banking kadam 6](../13_banking_system/13_banking_system.md#kadam-6-sms-service-giri-outbox--kafka) / Kafka file ka **outbox**
hi hai, bas outbox table ki jagah engine ka apna log.

Par settlement worker ne S-88 uthaya. Arpan ka paisa wallet DB me, share portfolio DB me. Paisa kat gaya, aur
share likhne se pehle portfolio DB band ho gaya. Arpan ke 35,000 gaye, share aaye nahi.

### Kadam 7: sauda ke baad paisa + share (settlement)

Pakka niyam: **sauda ho gaya to ulta nahi hota.** Exchange me Suresh ka share bik chuka. Humari taraf koi DB
band ho, tab bhi sauda cancel nahi karte, bas use poora karte hain.

Sabse seedha: wallet aur portfolio ek hi DB me. Tab paisa + share ek hi transaction me
([banking kadam 2](../13_banking_system/13_banking_system.md#kadam-2-beech-me-server-gira-paisa-gayab-ek-transaction)):

```sql
BEGIN
  wallet:    Arpan blocked -35,000      Suresh available +35,000
  portfolio: Arpan TCS +10              Suresh TCS (blocked) -10
  settlement: S-88 = DONE               -- sauda id UNIQUE, do baar nahi
COMMIT          -- ya sab hua, ya kuch nahi
```

Agar dono alag DB me (badi company me aksar):

```
Settlement worker: Kafka trades se "SAUDA S-88" uthaya
  1. settlement table: S-88 = PENDING
  2. wallet DB:    paisa move      -> S-88 paisa = DONE
  3. portfolio DB: share move      -> BAND hai!
       -> ruk-ruk ke dobara (1s, 2s, 4s) -- sauda ulta NAHI
       -> 10 min tak band -> ALERT, ops dekhe
       -> Arpan ko app me: "share aa rahe hain (PENDING)"
  4. portfolio wapas -> share move -> S-88 = DONE
raat ka milaan: saare sauda vs wallet vs portfolio -> farak = alert
```

```mermaid
flowchart LR
    L[("Kafka: trades<br/>SAUDA S-88")] --> W["Settlement worker"]
    W --> ST[("settlement: S-88<br/>PENDING / DONE")]
    W -- "paisa" --> WD[("Wallet DB")]
    W -- "share (band to dobara)" --> PD[("Portfolio DB")]
    style W fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style ST fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Har step `S-88` se pehchana jaata: worker beech me mara aur dobara chala, to "S-88 paisa = DONE" dekh ke paisa
dobara nahi kaatta. Wahi idempotency, bas key sauda id. Raat ka milaan [banking kadam 9](../13_banking_system/13_banking_system.md#kadam-9-balance-aur-khaata-alag-ho-gaye-raat-ka-milaan--reconciliation) wala hi.

Arpan ko "retry karo" kyun nahi: Arpan dobara Buy dabayega to wo naya order, 20 share aa jaayenge. Galti
humari taraf ki, to theek bhi hum karte: PENDING dikhao, khud poora karo.

Par 10 lakh logon ka app khula, sab ko har second TCS ka naya bhaav dekhna. Har app har second server se
poochh raha "TCS kitna?". 10 lakh sawaal / sec, server baith gaya.
