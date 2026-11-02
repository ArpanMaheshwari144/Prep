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
NFR     ek seat kabhi do logon ko na bike (yahi dil hai) · seat map dekhna tez · badi movie ke din ki bheed jhele
Numbers advance booking 10 baje: 10 lakh log ek saath, seat sirf 50 hazaar · 100 log map dekhte to ~1-2 book karte
        -> dekhna bahut, likhna kam · data chhota (aaj + agle hafte ke show), purana archive
```

---

## 1. Do counter, ek seat (dekhna aur parchi lagana ek saath)

Maine D5 chuni aur "book" dabaya, usi second ek aur ladke ne bhi D5 pe book dabaya. Dono ke app ne seat khaali dekhi,
aur dono ko "seat mil gayi" aa gaya. Ye isliye hua kyunki seat dekhna aur seat book karna alag-alag rakha tha, beech
me doosra ghus gaya. To dono ko ek saath rakho. Maan lo do counter hain, main is counter pe, doosra us counter pe, aur
andar ek hi seat bachi. Maine kaha "ek seat chahiye, lo mere paise". Mere counter wale ne jaake seat pe ek parchi rakh
di: "ye Arpan ki seat hai, Arpan iska payment karega, ye Arpan ko milegi, kisi aur ko nahi." Doosra counter wala aata
hai, dekhta hai "achha, yahan Arpan ki parchi lagi hai, ise main bech nahi sakta", aur ladke ko bol deta hai "tu ja
bhai, seat book ho gayi".

**Asli naam:** dekhna aur parchi ek saath = ek hi query `UPDATE seats SET status = HELD ... WHERE seat = D5 AND
status = FREE`, parchi = hold, doosre counter ko mana = uski query me 0 row badli, "seat chali gayi". Ye banking wali
kahani se aaya (wahan `WHERE balance >= 500`, yahan `WHERE status = FREE`). Register = SQL DB (Postgres): data chhota,
aur pay hua to seat + booking + payment teeno ek saath badlein ya koi nahi, ye transaction SQL ka seedha kaam hai.

```mermaid
flowchart LR
    A["Arpan"] --> S["Counter<br/>(BMS Service)"]
    L["Doosra ladka"] --> S
    S --> D[("Seat register<br/>(SQL DB: seats)")]
    style D fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

## 2. Parchi laga ke chai peene chala gaya (5 minute ki parchi)

Maine D5 pe parchi laga di, phir pay kiye bina phone rakh ke chai peene chala gaya, aur kabhi wapas nahi aaya. Ab D5 pe
hamesha ke liye meri parchi, koi aur le hi nahi sakta. To parchi ko bas thodi der ke liye wahin rehne do. Counter wala
dekh raha: "ye payment kar raha, chal main 5 minute tak dekhta hoon. Aa gaya to theek, nahi to hata dunga yahan se."
Usne 5 minute intezaar kiya, main nahi aaya, to parchi phaad ke phenk di. Seat agle ke liye.

**Asli naam:** 5 minute tak dekhta hoon = hold expiry (`hold_until = now + 5 min`), parchi phaad ke phenk di = hold
hata ke seat wapas FREE, pay aa gaya to HELD -> BOOKED.

**Maan lo parchi phaadne wala counter wala aaj chhutti pe hai:** wo har minute ghoom ke purani parchiyan dekhta tha.
Aaj meri parchi ko 20 minute ho gaye, abhi bhi D5 pe lagi, aur naya aadmi D5 lena chahta hai. Pehli baat, uska ek
chhota bhai rakh lo jo wo kaam kar le, ya us din ke liye replacement lao. Aur sabse achha, parchi pe hi umar likh do:
parchi 5 minute tak yahan padi hai, 5 minute ke baad tu hat ja, parchi khatam. Tab naya aadmi aaye to counter khud
dekh leta hai ki parchi ki umar nikal chuki, phaadne wale ka intezaar hi nahi.

**Asli naam:** chhota bhai / replacement = backup safai job (purane HELD ko FREE karta, taaki map pe seat jaldi khaali
dikhe), parchi pe hi umar = shart query me hi: `WHERE status = FREE OR (status = HELD AND hold_until < now)`.

## 3. 10 baje 10 lakh log (counter badhao + dekhne walon ki alag diary)

Avengers ki advance booking subah 10 baje khuli. 10 lakh log ek saath app pe toot pade, seat sirf 50 hazaar. Servers
doob gaye, aur jo asli me pay kar rahe the unke page bhi atke. To pehle counter badhao, server badhao. Aur zyadatar
log to sirf dekhne aa rahe hain ki kaunsi seat khaali hai, book koi-koi kar raha. To jo sirf dekhne aa raha uske liye
ek alag diary, aur jo kuch hilane aa raha (parchi lagana, pay) uske liye alag diary. Dekhne wale apni diary padhein,
asli register sirf bechne walon ke liye khaali rahe.

**Asli naam:** counter / server badhao = kai BMS Service machine + load balancer, kuch hilane wali diary = primary
(saari HOLD / BOOKED query yahin), dekhne wale ki alag diary = read replica (seat map yahan se). Replica thoda peeche
ho to chalega: map sirf dikhata hai, faisla primary ki query karti hai, wahan 0 row aayi to "seat chali gayi".

```mermaid
flowchart LR
    U["10 lakh log"] --> LB["Banda<br/>(Load balancer)"]
    LB --> S["Bahut counter<br/>(BMS Service)"]
    S --> P[("Hilane wali diary<br/>(SQL primary)")]
    S --> R[("Dekhne wali diary<br/>(Read replica)")]
    P --> R
    style LB fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style R fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

## 4. Hall jitni jagah, utne andar (baaki line me, parchi pe number)

Server badha diye, phir bhi 10 lakh log ek saath andar aaye to saari HOLD query chand seaton pe toot padti hain. To
thode-thode log andar jaao, zyada nahi. Sabse pehli baat, cinema ki capacity kitni hai, utne log andar. Maan lo mere
hall me 1000 ki jagah aur saamne 5000 khade, to 1000 andar jaao, baaki 4000 bahar ruko. Aur wo line me lago: "le bhai,
ye teri parchi, tu jaayega, phir agla number tera." Jo pehle aaya wo pehle andar, sabke saath barabari.

**Asli naam:** capacity jitne andar, baaki line me = virtual queue (waiting room), ye teri parchi, agla number tera =
token (andar aane ka pass, umar 10 minute). Server badhane se ye kaam nahi hota, kyunki jhagda seat ki chand rows pe
hai, ek row ek hi jagah lock hoti hai; bheed ko register tak pahunchne se pehle hi rokna padta hai.

```mermaid
flowchart LR
    U["10 lakh log"] --> W[("Line + number ki parchi<br/>(Waiting room + token)")]
    W --> S["Counter<br/>(BMS Service)"]
    S --> P[("Hilane wali diary<br/>(SQL primary)")]
    S --> R[("Dekhne wali diary<br/>(Read replica)")]
    style W fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

## 5. Token khatam, parchi baaki (do alag ghadi)

Line me mera number aaya, andar jaane ka token mila jo 10 minute chalega. Maine 8ve minute pe D5 chuni, to D5 pe 5
minute ki parchi lagi. 10ve minute pe token khatam, par parchi ke 3 minute baaki. Pay kar paunga? Haan. Humne token
diya, wo andar aa gaya, iska matlab wo authenticate ho chuka, ab koi dikkat nahi. Andar aa gaya aur seat ruk gayi
iske liye, to use utne time me payment karna hai jo seat ke liye allot hua. Kar diya to theek, nahi kiya to parchi
hata do, seat agle ki.

**Asli naam:** token = sirf gate ka pass (andar aao, HOLD karo), hold = seat ki ghadi (`hold_until`, SQL me). Do alag
ghadiyan: token gate ki, hold seat ki; pay ka faisla sirf hold ki ghadi karti. Token khatam = bas nayi seat HOLD nahi
kar sakta, dobara line. Pay ka raasta banking / payment wali kahani se aaya: dobara bheja to idempotency key (booking
id), paisa PSP se, webhook aaya tab D5 HELD -> BOOKED.

**Maan lo Arpan ne 4:30 pe Pay dabaya, PSP dheema, 5:00 pe parchi ki umar nikli aur 5:10 pe Priya D5 maang rahi:** time
ke andar pay shuru hua to seat Arpan ki, vaada ho chuka, warna consumer court ka case. PSP dheema hai to galti hamari
taraf ki. To Pay dabate hi parchi pe status likh do "pay chal raha, PSP ka jawab baaki", aur jab tak ye likha hai umar
badhi rahe, seat kisi ko na dikhe.
**Asli naam:** HELD -> PAYMENT_PENDING (Pay dabate hi), is status pe na query na safai job hold hataye; PSP ka SUCCESS
-> BOOKED, FAILED / timeout -> FREE.

```mermaid
flowchart LR
    A["Arpan"] --> W["Number ki parchi<br/>(Token, 10 min)"]
    W --> S["Counter<br/>(BMS Service)"]
    S --> D[("Seat ki parchi<br/>(SQL: hold_until, 5 min)")]
    J["Chhota bhai<br/>(safai job)"] -.-> D
    S --> Pay["Pay<br/>(Payment Service + PSP)"]
    style Pay fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

---

Notes (Arpan ke shabd) = [BMS_HAND_NOTES.md](BMS_HAND_NOTES.md) · [<- MASTER SHEET](../../00_MASTER_SHEET.md)
