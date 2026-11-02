# Stock trading: Arpan aur Ravi ki kahani

Arpan TCS ke 10 share 3500 pe kharidna chahta hai, Ravi apne 10 TCS 3500 pe bechna chahta hai. Dono ek-doosre ko
jaante nahi, aur unke jaise lakhon log har second order daal rahe. Kaam: sahi log aapas me milein, paisa aur share
hamesha sahi, aur sauda ho gaya to sabko pata chale. (Chaabi wali baat banking jaisi, live bhaav chat jaisa.)

Har kadam = **kahani** (dikkat aur hal, bolne wali bhasha) -> **Asli naam** (interview me yahi shabd) -> tasveer
(kahani ka shabd, bracket me asli naam).

---

## FR / NFR / Numbers (interview me pehle 2 minute)

```
FR      buy / sell order dalo (LIMIT / MARKET) · order milao = sauda · paisa aur share badlo · live bhaav, cancel
NFR     paisa aur share hamesha sahi · jo pehle aaya wo pehle mile · 9:15 ki bheed jhele · har cheez ka record
Numbers 50 lakh order roz, market ~6 ghante -> ~250 / sec aam · 9:15 pe jhatka ~10,000 / sec, saare share me bata
        live bhaav: ~10 lakh app khule, har second naya bhaav -> poochhne ki jagah push
```

---

## 1. Arpan ka order Ravi se kaun milayega? (beech ka aadmi)

Arpan aur Ravi ek-doosre ko jaante hi nahi. Koi 3490 pe kharidna chahta, koi 3510 pe bechna. To beech me ek aadmi
bitha do. Kharidne wale ko behtar bhaav chahiye, bechne wale ko bhi. Main beech me baitha hoon: Arpan ko is bhaav me
chahiye, Ravi ko is bhaav me, dono ka bhaav theek hai to dono ka sauda kar do. Khatam kahani. Na Arpan ne Ravi ko
jaana na Ravi ne Arpan ko, bas beech me ek aadmi baitha tha.

**Asli naam:** beech ka aadmi = matching engine, har share ki do list (kharidne wale, bechne wale) = order book,
bhaav mila = match (ek bhaav pe do log to jo pehle aaya wo pehle: price-time priority). Paisa aur hold = SQL DB
(Postgres; ek user ka paisa kaatna / hold ek transaction me = ACID). Portfolio (share dikhana) alag service ka DB, sauda
wahan Kafka se pahunchta (kadam 6-7).

```mermaid
flowchart LR
    A["Arpan: buy 10 @ 3500"] --> O["Order Service"]
    R["Ravi: sell 10 @ 3500"] --> O
    O --> M["Beech ka aadmi<br/>(Matching engine)"]
    M --> D[("Paisa + share<br/>(Postgres)")]
    style M fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

## 2. Net gaya, 20 share kharid liye (chaabi)

Arpan ne "Buy 10 TCS" dabaya. Order engine tak pahuncha, par jawab aate waqt net gaya. App ne socha order gaya hi
nahi, dobara bhej diya, 20 share aa gaye. To order ke saath ek chaabi lagao. Order server pe aaya, saath me chaabi
laaya. Main server hoon, maine us chaabi ka taala khareeda aur chaabi apne paas rakh li. 5 minute baad wahi chaabi
leke "10 TCS" dobara aaya. Ye to wahi hai jo mere paas hai, ghapla hai. Bhai tu nikal, ye chaabi is building me
nahi chalegi.

**Asli naam:** chaabi = idempotency key (client_order_id), taala khareeda = pehli baar wali key DB me UNIQUE(user_id,
client_order_id), wahi chaabi dobara = duplicate order reject, purana order hi lautao.

## 3. 35,000 se do order (paisa hold)

Arpan ke account me 35,000. TCS ka 35,000 ka order book me pada, abhi match nahi hua. Isi beech usi 35,000 se INFY
ka 35,000 ka order bhi daal diya, dono match ho gaye, chahiye 70,000. To Arpan ko ye paise kharch hi na karne do.
Jaise EMI me hota: log bolte "bank ne mera paisa hold kar rakha, na kharch kar sakta na kahin bhej sakta". Same yahan:
order lagte hi paisa hold, ab wo kahin aur kharch nahi kar paayega, INFY wala order lagta hi nahi.

**Asli naam:** paisa hold = funds block (available se ghata ke blocked me, check aur kaatna ek query me), kharch na
kar paaye = doosra order reject. Ravi ke share pe bhi yahi, ek share do baar nahi bikta.

## 4. 4 thread, 10 share 20 me bik gaye (ek line, ek jagah)

9:15 baje bheed, engine tez karne ko 4 thread. Do thread ne ek saath TCS ki book dekhi, dono ko Ravi ke wahi 10 share
dikhe. Ek ne Arpan ko bech diye, doosre ne Mohan ko. Ye aisa hai: mere paas 10 share hain, maine apni ek copy bana di
aur thodi door khada kar diya. Ek ladka mere paas aaya, ek copy ke paas, dono ne 10 khareed liye, 20 bik gaye. To copy
ko apne paas kheench lo, mujhme mil ja. Arpan aaya, Mohan aaya, pehle main Arpan ko bechunga, mere paas zero, ab Mohan
ko mana. Matlab sabko ek line me, ek hi jagah rakho, alag-alag mat baanto.

**Asli naam:** copy door khadi, dono ne bech diya = do thread ek book pe (race condition), ek line ek jagah = ek share
ki book ek hi thread (single-threaded per symbol). Lock nahi chahiye, aur line ka kram hi "pehle aaya pehle mila".
Cancel bhi isi line me: sauda pehle aaya to "bik chuka", cancel pehle to hold wapas.

## 5. Book RAM me thi, machine mari (permanent dabba + parchi)

Tez chalne ko TCS ki book RAM me rakhi. 10:30 baje machine mar gayi, saare order, Arpan ka adhoora order bhi, RAM ke
saath ud gaye. RAM me kyun rakh raha? Ye paise ki cheez hai, kaata-jodi main DB me karunga, primary DB me. Wo dheema
pade to uski 3-4 copy bana ke rakho, gira to doosri ya teesri copy hai. Zyada likhai ho to DB ko alag-alag tukdon me
baanto. Aur pehle numbers dekho: aam din 250 / sec, 9:15 pe bhi bheed saare share me bati hui, itni nahi ki ek tukda
ya ek thread jhel na paaye.

Aur adhoora order: bag pack kar raha, saara saamaan aaya nahi, to poora bag khaali karo aur dobara jamao. Subah ek
parchi banayi, Bangalore ja raha, 10 kapde, 5 ye, 3 ye, jo rakha us pe tick. Bag khaali kiya to bhi parchi utni ki
utni hai, wo dekh ke dobara jama lo.

**Asli naam:** permanent dabba = DB (primary + replica / standby), tukde = shard (share ke hisaab se), pehle numbers =
numbers se RAM ya DB ka faisla (bahut kam traffic pe DB pe hi chal jaata; RAM + micro-second wala jhanjhat tab jab
NSE jaisi bheed ho), subah se parchi = order log (append-only, sirf jodna): har order pehle parchi pe, phir book (DB) me; DB ki copy bhi
hil jaaye to parchi dekh ke bag = log replay se book wapas.

```mermaid
flowchart LR
    O["Order Service"] --> M["Ek share ek thread<br/>(Matching engine)"]
    M --> L[("Parchi<br/>(Order log)")]
    M --> D[("Permanent dabba<br/>(Primary DB)")]
    D --> C[("3-4 copy<br/>(Replica)")]
    style L fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style D fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style C fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

## 6. Sauda hua, sabko batana (10 ladkon ki fauj)

Arpan aur Ravi ka sauda ho gaya. Khabar kai jagah jaani: dono ke portfolio, paise ka hisaab, SMS, bhaav ki screen.
Engine khud ek-ek ko call kare to SMS dheema hua to engine bhi ruka, 9:15 ki line atki. To main apne paas 10 ladke
ikatthe karunga: tu aa, tu aa, le ye information. Main khud kuch nahi karunga, bas baith ke instruction di. Tu ye
kar, tu ye kar, jab ho jaaye to bata dena.

**Asli naam:** 10 ladkon ki fauj = Kafka (trades topic), har ladka apna kaam = har service ka alag consumer group
(settlement, notification, audit, Price Service), main khud kuch nahi = engine sauda ek baar daal ke aage, line nahi
rukti.

```mermaid
flowchart LR
    M["Matching engine"] --> K[("10 ladkon ki fauj<br/>(Kafka: trades)")]
    K --> S["Settlement"]
    K --> N["SMS<br/>(Notification)"]
    K --> A["Audit"]
    K --> P["Price Service"]
    style K fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

## 7. Paisa kata, share dikh nahi rahe (sauda ulta nahi, parchi pakdo)

Sauda pakka. Arpan ke portfolio me "10 TCS" likhna tha, us waqt portfolio wala DB band. Paisa kat chuka, share dikh
nahi rahe. Yahan rollback nahi kar sakte, matching engine ka poora system hi ye hai ki tumhara match ho gaya. Ab
kahein "match nahi hua tha" to wo case kar dega, baat consumer court jaayegi. To use bata do: abhi service thodi
dheeli hai, ghabrao nahi, app me dikha denge jab ho jaayega. Parchi apne paas rakh lo. 10-15 minute baad DB wapas
aaya: le ye meri parchi, ye kaam karna hai, jaldi kar, mere grahak intezaar kar rahe.

**Asli naam:** rollback nahi = sauda ulta nahi hota, thodi der baad dikha denge = settlement PENDING, parchi apne paas
= message Kafka me pada, DB aate hi retry. Sauda id (trade_id) se pehchana, to dobara chale tab bhi paisa do baar nahi
katta.

## 8. TCS ka bhaav 10 lakh screen pe (chat wala khula taar)

TCS ka bhaav har second badal raha (3500, 3501, 3499), 10 lakh logon ki screen pe live chahiye. Har app har second
poochhe to 10 lakh request har second. Iska design chat me kar chuke: app ka taar khula rakho aur bhaav khud bhejo,
beech me ek chhota ladka jo bhaav sab machine tak chilla de.

**Asli naam:** khula taar = WebSocket (push), chhota ladka = Redis pub/sub (channel "TCS"), Price Service = trades
se har share ka ek bhaav / sec. Bhaav kho bhi jaaye to agle second naya, isliye yahan Kafka nahi.

---

Notes (Arpan ke shabd) = [STOCK_HAND_NOTES.md](STOCK_HAND_NOTES.md) · [<- MASTER SHEET](../../00_MASTER_SHEET.md)
