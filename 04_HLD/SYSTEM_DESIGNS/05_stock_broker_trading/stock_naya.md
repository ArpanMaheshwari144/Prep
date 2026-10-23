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

Wallet me do khaane: **available** (khula paisa) aur **blocked** (order ke liye ruka paisa). Order book me
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
(queue) me lagte hain, thread unhe ek-ek karke uthata hai.

```
TCS ke order  --> [TCS line] --> TCS thread (akela) --> TCS order book
INFY ke order --> [INFY line] --> INFY thread (akela) --> INFY order book
RELIANCE ...  --> [RELIANCE line] --> RELIANCE thread --> ...

line me kram = jo pehle aaya wo pehle milta (imaandari, NFR wala)
```

```mermaid
flowchart LR
    O["Order Svc"] -- "key = share" --> LT[("TCS line")]
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

Ye [Kafka](../12_message_queue_kafka/12_message_queue_kafka.md) kadam 3 jaisa: wahan `key = userId` taaki ek user ki khabar ek line me, yahan `key = share`
taaki ek share ke order ek line me.

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

### Kadam 5: pehle log likho, phir milao + standby machine

Matching engine order milane se **pehle** har order ek **log** me likhta hai: disk pe, sirf aage judta, kabhi
mitta nahi (Kafka ka topic ya engine ki apni log file). Milaan ke baad jo sauda bana, wo bhi log me.

```
order aaya --> 1. LOG me likho (disk, pakka)  #1041 Arpan buy TCS 10 @ 3500
           --> 2. RAM book me milao            -> sauda S-88: Arpan <- Suresh 10 @ 3500
           --> 3. sauda bhi LOG me              #1042 SAUDA S-88
```

Machine giri to? Ek **standby** machine pehle se wahi log live padh rahi hai, aur apni RAM me wahi book banati
ja rahi hai. **Primary** = jo abhi milaan kar rahi hai, **standby** = jo taiyaar baithi hai.

```
12:00:00  primary machine giri
12:00:02  standby ko 2 sec se primary ki aawaz nahi aayi -> standby khud primary ban gayi
          book pehle se bani hui thi (log #1042 tak padh chuki thi)
          naye order ab ye nayi primary legi. kuch khoya nahi
```

```mermaid
flowchart LR
    O["Order Svc"] --> L[("LOG (disk)<br/>#1041 order, #1042 sauda")]
    L --> P["Primary<br/>(milaan karti)"]
    L --> S["Standby<br/>(log padh ke book taiyaar)"]
    S -. "primary giri: khud primary" .-> P
    style L fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style S fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Ye [banking kadam 2](../13_banking_system/13_banking_system.md#kadam-2-beech-me-server-gira-paisa-gayab-ek-transaction) jaisa: wahan DB pehle WAL (apna log) likhta, phir kaam, aur crash ke baad log padh ke
wapas khada. Yahan bhi wahi soch: RAM bhool sakti hai, log nahi bhoolta.

Dono machine ek saath milaan karein to tez nahi? Nahi. Ek waqt me milaan sirf primary karti hai, standby bas log
padh ke book taiyaar rakhti hai. Dono milayein to ek hi Suresh do baar bik jaayega (Kadam 4 wali dikkat wapas).

Standby ne log ka koi order dobara padh liya to? Har order aur sauda ka apna number (#1041, S-88). Same log se
same book banti hai, aur sauda S-88 aage do baar nahi chalta, kyunki Kadam 6 me sauda id se hi paisa move hota.

Par sauda S-88 ho gaya. Ab paisa Arpan se Suresh, share Suresh se Arpan. Arpan ka paisa wallet DB me, share
portfolio DB me. Paisa kat gaya, aur share likhne se pehle portfolio DB band ho gaya. Arpan ke 35,000 gaye,
share aaye nahi.
