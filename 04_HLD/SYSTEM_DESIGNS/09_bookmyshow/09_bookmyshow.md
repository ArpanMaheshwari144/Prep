# BookMyShow: do counter, ek seat ki kahani

Arpan BookMyShow pe Avengers ke pehle show ki seat D5 book kar raha hai. Socho cinema ke do counter hain, dono ek hi
seat bech rahe, aur ek ko doosre ka pata nahi. Kaam: ek seat kabhi do logon ko na bike, aur advance booking wale din
10 lakh ki bheed me bhi jo pay kar raha uska kaam na atke.

Har kadam = **kahani** (dikkat aur hal, bolne wali bhasha) -> **Asli naam** (interview me yahi shabd) -> tasveer
(kahani ka shabd, bracket me asli naam).

---

## FR / NFR / Numbers (interview me pehle 2 minute)

```
FR      show aur seat map dekho · seat chuno -> pay karo -> ticket pakka
        · seat chuni to 5 minute tere naam rukti, pay nahi kiya to chhoot jaati
NFR     ek seat kabhi do logon ko na bike (yahi dil hai) · seat map dekhna tez · badi movie ke din ki bheed jhele
Numbers advance booking 10 baje: 10 lakh log ek saath, seat sirf 50 hazaar · 100 log map dekhte to ~1-2 book karte
        -> dekhna bahut, likhna kam · data chhota (aaj + agle hafte ke show), purana archive
```

---

## 1. Do counter, ek seat (dekhna aur parchi lagana ek saath)

Maine D5 chuni aur "book" dabaya, usi second ek aur ladke ne bhi D5 pe book dabaya. Dono ke app ne seat khaali dekhi,
aur dono ko "seat mil gayi" aa gaya. Ye isliye hua kyunki seat dekhna aur seat book karna alag-alag rakha tha, beech
me doosra ghus gaya. To dono ko ek saath rakho. Maan lo do counter hain, main is counter pe, doosra us counter pe, aur
andar ek hi seat bachi. Dono counter ko nahi pata ki doosra bhi wahi seat bech raha. Maine kaha "ek seat chahiye, lo
mere paise". Mere counter wale ne jaake seat pe ek parchi rakh di: "ye Arpan ki seat hai, Arpan iska payment karega,
ye Arpan ko milegi, kisi aur ko nahi." Doosra counter wala dekhta hai "achha, yahan Arpan ki parchi lagi hai, ise main
bech nahi sakta", aur ladke ko bol deta hai "tu ja bhai, seat book ho gayi".

**Asli naam:** dekhna aur parchi ek saath = ek hi query `UPDATE seats SET status = HELD ... WHERE seat = D5 AND
status = FREE`, parchi = hold, doosre counter ko mana = uski query me 0 row badli, "seat chali gayi". Ye banking wali
kahani se aaya (wahan `WHERE balance >= 500`, yahan `WHERE status = FREE`). Register = SQL DB (Postgres): data chhota,
aur pay hua to seat + booking + payment teeno ek saath badlein ya koi nahi, ye transaction SQL ka seedha kaam hai.

**Maan lo Arpan ko family ke saath D5, D6, D7 teeno chahiye; counter wale ne D5, D6 pe parchi lagayi, D7 tak pahuncha to usi second doosre counter wale ne D7 pe Priya ki parchi laga di:** Ye almost same hai. D5 D6 D7 ka kaam ek saath hi hoga, alag-alag nahi: dekhna aur rokna teeno ka ek hi baar, beech me koi nahi aa sakta. Agar teeno available hain tabhi book karo. Maan lo mujhe D1 se D10 poora block chahiye, to main beech me khada hoke bolunga "sab sun lo, D1 se D10 meri", ek parchi pe likh dunga aur sab counter wale usi parchi se padhenge. Jaise ek chhotu board leke khada ho gaya: "D1 se D10 book hai, ise koi book nahi karega".
**Asli naam:** "teeno ek saath, beech me koi nahi" = ek transaction (all-or-nothing, UPDATE ... WHERE seat IN (...) AND FREE; ek bhi na mili to rollback, kisi pe parchi nahi), "ek parchi poore block ki" = ek booking id ke neeche saari seat.

```mermaid
flowchart TB
    A["Arpan: ek seat chahiye,<br/>lo mere paise"]:::card
    L["Doosra ladka: D5"]:::card
    C1["Mera counter"]:::card
    C2["Doosra counter"]:::card
    subgraph EK["DEKHNA + BOOK KARNA EK SAATH"]
        D[("D5 pe parchi: ye Arpan ki,<br/>kisi aur ko nahi (hold)")]:::pill
    end
    X["Doosra counter: parchi lagi hai,<br/>tu ja bhai, seat book ho gayi"]:::bad
    A --> C1
    L --> C2
    C1 --> D
    C2 --> D
    D -.-> X
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style EK fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 2. Parchi laga ke chai peene chala gaya (5 minute ki parchi)

Maine D5 pe parchi laga di, phir pay kiye bina phone rakh ke chai peene chala gaya, aur kabhi wapas nahi aaya. Ab D5 pe
hamesha ke liye meri parchi, koi aur le hi nahi sakta. To parchi ko bas thodi der ke liye wahin rehne do. Counter wala
dekh raha: "ye payment kar raha, chal main 5 minute tak dekhta hoon. Aa gaya to theek, nahi to hata dunga yahan se."
Usne 5 minute intezaar kiya, main nahi aaya, to parchi phaad ke phenk di. Seat agle ke liye.

**Asli naam:** 5 minute tak dekhta hoon = hold expiry (`hold_until = now + 5 min`), parchi phaad ke phenk di = hold
hata ke seat wapas FREE, pay aa gaya to HELD -> BOOKED.

```mermaid
flowchart TB
    A["Arpan parchi laga ke<br/>chai peene gaya"]:::bad
    subgraph PM["PARCHI THODI DER KE LIYE"]
        D[("Counter wala 5 minute tak<br/>dekhta (hold_until)")]:::pill
    end
    F["Nahi aaya: parchi phaad di,<br/>seat agle ke liye (FREE)"]:::good
    A --> D
    D -- "5 minute, pay nahi" --> F
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style PM fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 3. Parchi phaadne wala chhutti pe (chhota bhai, ya parchi pe hi umar)

Parchi phaadne wala counter wala har minute ghoom ke purani parchiyan dekhta hai. Aaj wo chhutti pe hai. Meri parchi
ko 20 minute ho gaye, abhi bhi D5 pe lagi, aur naya aadmi D5 lena chahta hai. Sabse pehli baat, parchi uthane wala
chhutti pe hai to uska ek chhota bhai rakh lo, wo kaam kar lega, ya us din ke liye uska replacement lao. Ya phir hum
us parchi pe hi umar laga dein ki wo apne aap wahan se gayab ho jaaye: parchi 5 minute tak yahan padi hai, 5 minute ke
baad tu hat ja, parchi khatam. Tab naya aadmi aaye to counter khud dekh leta hai ki parchi ki umar nikal chuki,
phaadne wale ka intezaar hi nahi.

**Asli naam:** chhota bhai / replacement = backup safai job (purane HELD ko FREE karta, taaki map pe seat jaldi khaali dikhe), parchi pe hi umar = shart
query me hi: `WHERE status = FREE OR (status = HELD AND hold_until < now)`, phaadne wale ka intezaar nahi.

```mermaid
flowchart TB
    X["Parchi phaadne wala chhutti pe,<br/>parchi 20 minute se lagi"]:::bad
    N["Naya aadmi: D5 chahiye"]:::card
    subgraph DR["DO RAASTE"]
        B(["Chhota bhai / replacement<br/>(backup safai job)"]):::pill
        U(["Parchi pe hi umar: 5 minute baad<br/>apne aap gayab"]):::pill
    end
    OK["Umar nikal chuki: D5 naye ki"]:::good
    X --> B
    N --> U
    U --> OK
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style DR fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 4. 10 baje 10 lakh log (counter badhao, alag diary, hall jitne andar)

Avengers ki advance booking subah 10 baje khuli. 10 lakh log ek saath app pe toot pade, seat sirf 50 hazaar. Servers
doob gaye, aur jo asli me pay kar rahe the unke page bhi atke. To pehle counter badhao, server badhao. Phir jo sirf
dekhne aa raha uske liye ek alag diary, aur jo kuch hilane aa raha (parchi lagana, pay) uske liye alag diary, primary
aur replica wali cheez. Aur thode-thode log andar jaao, zyada nahi. Sabse pehli baat, cinema ki capacity kitni hai,
utne log andar. Maan lo mere hall me 1000 ki jagah aur saamne 5000 khade, to 1000 andar jaao, baaki 4000 ghar jaao. Aur
wo line me lago: "le bhai, ye teri parchi, tu jaayega, phir agla number tera."

**Asli naam:** counter / server badhao = kai BMS Service machine (load balancer ke peeche), kuch hilane wali diary =
SQL primary (saari HOLD / BOOKED query yahin), dekhne wale ki alag diary = read replica (seat map yahan se), capacity
jitne andar baaki line me = virtual queue (waiting room), ye teri parchi agla number tera = token (andar aane ka pass, umar 10 minute). Replica thoda peeche ho to chalega: map
sirf dikhata hai, faisla primary ki query karti hai. Server badhane se line wala kaam nahi hota, kyunki jhagda seat ki
chand rows pe hai, ek row ek hi jagah lock hoti hai; bheed ko register tak pahunchne se pehle hi rokna padta hai.

```mermaid
flowchart TB
    U["10 baje 10 lakh log,<br/>seat 50 hazaar"]:::bad
    subgraph LN["CAPACITY JITNE ANDAR, BAAKI LINE ME"]
        W(["Le bhai, ye teri parchi,<br/>agla number tera (token)"]):::pill
    end
    S["Counter badha de, server badha de"]:::card
    subgraph DY["ALAG DIARY"]
        P[("Hilane wale ki diary<br/>(primary)")]:::card
        R[("Dekhne wale ki diary<br/>(read replica)")]:::pill
    end
    U --> W
    W --> S
    S --> P
    S --> R
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style LN fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
    style DY fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 5. Token khatam, parchi baaki (do alag ghadi)

Line me mera number aaya, andar jaane ka token mila jo 10 minute chalega. Maine 8ve minute pe D5 chuni, to D5 pe 5
minute ki parchi lagi. 10ve minute pe token khatam, par parchi ke 3 minute baaki. Pay kar paunga? Haan. Humne token
diya, wo andar aa gaya, iska matlab wo authenticate ho chuka, ab koi dikkat nahi. Andar aa gaya, to haan bhai, seat
ruk gayi iske liye. Use utne time me payment karna hai jo uske liye allot hua. Kar diya to theek, nahi to parchi hata
do, seat agle ki.

**Asli naam:** token = sirf gate ka pass, hold = seat ki ghadi (`hold_until`). Do alag ghadiyan: token gate ki, hold
seat ki; pay ka faisla sirf hold ki ghadi karti.

```mermaid
flowchart TB
    A["Arpan: token mila (10 min),<br/>8ve minute pe D5 chuni"]:::card
    subgraph G1["GHADI 1: GATE"]
        T(["Token: andar aa gaya,<br/>authenticate ho chuka"]):::pill
    end
    subgraph G2["GHADI 2: SEAT"]
        H(["Parchi 5 minute: seat ruk gayi,<br/>utne time me payment"]):::pill
    end
    OK["10ve minute token khatam,<br/>par parchi baaki: pay kar paayega"]:::good
    A --> T
    T --> H
    H --> OK
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style G1 fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
    style G2 fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 6. Pay dabaya, PSP dheema, parchi ki umar nikal gayi (status likh lo)

Maine 4:30 pe Pay dabaya, PSP dheema. 5:00 pe parchi ki umar khatam, 5:10 pe Priya D5 maang rahi, aur PSP ka jawab
6:00 pe aata hai "paisa kat gaya". Time ke andar Pay dabaya to seat meri, Priya ko nahi; humne vaada kar diya, warna
consumer court ka case. PSP dheema hai to galti meri nahi, hamari taraf ki hai. To parchi pe status likh lo: pay chal
raha hai, PSP ka jawab baaki. Jab tak ye likha hai, umar badha do, seat kisi aur ko dikhe hi nahi. Ya query me hi daal
do.

**Asli naam:** status likh lo = HELD -> PAYMENT_PENDING (Pay dabate hi), umar badha do = is status pe hold expire nahi
hota (na query na safai job isko chhoote). PSP ka SUCCESS -> BOOKED.

**Maan lo chhotu (PSP) jawab hi nahi de raha, 1-2 baar jagaya phir bhi chup:** PSP do hi haal me hota: haan bola to
BOOKED. Dheema hai to 1-2 baar ping, phir bhi chup to booking FAILED dikha do aur seat chhod do, kyunki peeche line lagi
hai. Jaise do ladke: ek baar-baar mana kar raha, to "chhod, doosra ladka tu ja": ab jo bhi naya pay karne aaye, doosre
PSP pe. Agar dheeme wale ne peeche se paisa kaat liya tha, to wo hamari id se pata chalega (user seedha PSP se baat nahi
karta, beech me hum hain). User ko bolo "paisa kata ho to wapas aa jaayega, dobara pay kar le", baad me kata nikla to
paisa wapas. IRCTC, GPay, Paytm roz yahi karte. Hum perfect system ke peeche nahi: PSP hamare bas me nahi, perfect
consistency maango to ticket bik hi nahi sakte; bas user se saaf baat aur paisa wapas.
**Asli naam:** 1-2 baar ping = status check retry (timeout), FAILED dikha do = PAYMENT_PENDING -> FAILED + hold release,
naye log doosre PSP pe = circuit breaker -> PSP-2, hamari id = payment id (idempotency key), baad me kata nikla to wapas
= reconciliation job + refund.

**Maan lo ye sab pehle se tay hi nahi, aur 55 second baad, jab Arpan back daba chuka, 56ve second pe paisa kata (Myntra
wala hi jawab, BookMyShow ka bhi apna PSP nahi, Razorpay hai):** Razorpay ko onboard karte hi vaada lo: "30 second me
jawab do, fail ya pass", buffer 50-55 second, 1 minute bahut. Downtime poochh lo, jaise AWS uptime batata. Badi film
(Avengers Doomsday) pe Stripe, Amazon Pay jaisi 3-4 company aur line me. 55 second baad bhi chup to har 3-4 second pe
2-3 ping, 10 nahi. Aur Razorpay ko bolo "paisa katte hi mujhe chhota sa event bhej do", jaise S3 file aane pe event
bhejta. Event aaya = paisa kata, seat book, aur hum haath utha ke "mil gaya" bol dete. Event nahi aaya = paisa nahi
kata. Event raste me kho gaya to wo Razorpay ka HLD hai, wo dobara bhejega; na hum andhe, na wo.
**Asli naam:** onboard pe vaada = SLA / timeout, 3-4 company line me = backup PSP (failover), 3-4 sec pe ping = status
poll, paisa katte hi event = webhook (PSP callback), "mil gaya" = ack (2xx), raste me kho gaya = PSP ki webhook redelivery.

```mermaid
flowchart LR
    R["Razorpay se pehle hi vaada:<br/>30 sec, buffer 55 (SLA)"]:::card --> P{"Paisa kata?"}:::card
    P -- "haan: turant event<br/>(webhook)" --> S["Seat BOOKED,<br/>'mil gaya' (ack)"]:::good
    P -. "55 sec chup" .-> Q["3-4 sec pe 2-3 ping<br/>(status poll)"]:::bad
    Q -.-> X["Badi film: Stripe, Amazon Pay<br/>line me (backup PSP)"]:::pill
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
```

```mermaid
flowchart TB
    A["Arpan: Pay 4:30, PSP dheema"]:::card
    subgraph SL["PARCHI PE STATUS LIKH LO"]
        D[("Pay chal raha, PSP ka jawab baaki:<br/>umar badha do (PAYMENT_PENDING)")]:::pill
    end
    P{"PSP (chhotu)"}:::card
    B["Haan: BOOKED"]:::good
    F["1-2 baar ping, phir bhi chup:<br/>FAILED, seat chhod do"]:::bad
    R["Paisa kata ho to wapas<br/>(payment id, refund)"]:::card
    N["Naye log: doosra ladka<br/>(PSP-2)"]:::good
    A --> D
    D --> P
    P -- "haan" --> B
    P -. "chup" .-> F
    F --> R
    F -.-> N
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style SL fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

---

Notes (Arpan ke shabd) = [BMS_HAND_NOTES.md](BMS_HAND_NOTES.md)
