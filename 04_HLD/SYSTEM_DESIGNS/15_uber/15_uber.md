# Uber: Connaught Place se ghar ki kahani

Arpan shaam 7 baje Connaught Place pe khada hai aur ghar ke liye Uber book karna hai. Sheher me 5 lakh driver
gaadi chala rahe hain, sab har second jagah badal rahe. Kaam: "Book" dabate hi paas ka khaali driver, ek driver ek
hi sawari, aur bheed me bhi app na atke.

Har kadam = **kahani** (dikkat aur hal, bolne wali bhasha) -> **Asli naam** (interview me yahi shabd) -> tasveer
(kahani ka shabd, bracket me asli naam).

---

## FR / NFR / Numbers (interview me pehle 2 minute)

```
FR      driver ki jagah lo · paas ka khaali driver do · ek driver ek sawari
        · Arpan ko Raju live dikhe · kiraya pehle dikhe · trip khatam pe pay
        · bheed me (concert) kiraya badh sakta hai, aur wo pehle hi dikh jaaye
        · ek saath kai booking aayein to aisi gaadi do ki sabki milake doori kam · jo der se ruka wo baar-baar peeche na rahe
NFR     driver turant mile (kuch second) · ek driver do jagah kabhi nahi · bheed (concert) me bhi chale
        · ride ke beech driver gaya to sambhalo
Numbers 5 lakh driver, har 4 sec jagah -> ~1.25 lakh likhai / sec · booking shaam ko hazaaron / sec
        -> likhai bahut zyada (jagah), booking pe paas wale jaldi chhaantne hain
```

---

## 1. Driver kahan hai, server ko pata hi nahi (beech ka dabba)

"Book" dabaya, par server ko pata hi nahi kaunsa driver abhi kahan hai. Location to phone me khud hai, GPS ka
latitude-longitude. To phone khud bheje, har hilne pe nahi, thodi-thodi der me. Beech me ek dabba rakh lo jiska kaam
sirf ek hai: har driver ki sabse taaza jagah batana.

**Asli naam:** beech ka dabba = Location Service; driver ka phone har ~4 sec (maan lo) lat/long bhejta.

```mermaid
flowchart TB
    A["Arpan: Book"]:::card
    X["Server: kaunsa driver kahan?<br/>pata hi nahi"]:::bad
    P["Driver ka phone: GPS lat/long,<br/>thodi-thodi der me (~4 sec)"]:::card
    subgraph BD["BEECH KA DABBA"]
        L(["Kaam sirf ek: har driver ki<br/>sabse taaza jagah (Location Service)"]):::pill
    end
    A -.-> X
    P --> L
    A --> L
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style BD fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 2. 1.25 lakh likhai / sec, DB ghut gaya (bada bhai)

Har 4 sec, 5 lakh driver = 1.25 lakh update / sec. Normal DB ghut gaya, aur jagah 30 sec peeche chal rahi. Do raaste
the: ya to chhoti parchi, kaat ke nayi jagah likho; ya bada dabba jo likhne me tez ho. Crore-crore likhai pe parchi
dheemi ho sakti, gir sakti, aur kab tak replica lagaunga, kal driver aur badhenge. To bada bhai bulao, jo isi kaam ke
liye bana hai, apni copies khud rakhta.

**Asli naam:** bada bhai = Cassandra (likhai tez, apni copies, machine jodo to badhta; andar ring = consistent hashing, nayi machine jude to sirf paas ka hissa hile). Driver ki row driver_id pe,
nayi jagah aate hi upar likh do (sirf taaza chahiye).

```mermaid
flowchart TB
    X["Har 4 sec, 5 lakh driver:<br/>1.25 lakh / sec, normal DB ghut gaya"]:::bad
    P["Chhoti parchi: dheemi ho sakti,<br/>gir sakti, kab tak replica"]:::card
    subgraph BB["BADA BHAI (isi kaam ke liye bana)"]
        C[("Likhai tez, copies khud<br/>(Cassandra)")]:::pill
    end
    X -.-> P
    X --> C
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style BB fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 3. 5 lakh ka fasla naapna, 40 sec ghoomna (naksha dabbon me)

Book pe server saare 5 lakh driver ka Arpan se fasla naap raha, aur Arpan 40 sec se "driver dhoondh rahe hain" dekh
raha. Connaught Place pe khade ko ISBT ka driver kyun? Sirf 8-9 km ke andar wale chahiye. Aur 5 lakh me se chhaanto
mat: naksha pehle se dabbon me kata ho, server sirf usi dabbe me dekhe jahan Arpan khada. Usme 3 driver, unme jo
khaali wo Arpan ko. Baaki 5 lakh kahin bhi ghoomein, lena-dena nahi.

**Asli naam:** naksha dabbon me = geohash (grid), sirf apna dabba = 5 lakh ki jagah gine-chune driver.

![Naksha pehle se dabbon me kata: server sirf Arpan ke dabbe me dekhe](../00_IMG/15_uber_map_kadam3.svg)

## 4. Arpan dabbe ke kinare pe, 200 meter door khaali driver (gola khincho)

Arpan dabbe ke kinare pe khada hai, aur 200 meter door bagal wale dabbe me khaali driver, par app bola "koi nahi".
Dabbe ki seema pe mat chalo. Arpan jahan khada wahan se 8 km ka gola khincho; jin dabbon ko gola chhoota, unke driver
bhi dekho, phir asli fasla naap ke sabse paas wala.

**Asli naam:** gola + chhoote dabbe = apna geohash + aas-paas ke dabbe (neighbours), phir asli distance.

![Dabbe ki seema pe mat chalo: Arpan se 8 km ka gola, chhoote dabbon ke driver bhi](../00_IMG/15_uber_map_kadam4.svg)

**Maan lo (reel se aaya) seedhi lakeer pe sabse paas Raju 500 meter, par Gomti nadi ke us paar, pul 4 km door; isi kinare Shyam 1.5 km:** Sirf map pe kaun paas
dikh raha ye nahi dekhna. Gole ke driver ki asli doori nikaalo: sadak se kitni door, kitni der lagegi. Nadi wala sadak se 8 km,
peeche wala 2 km, to Arpan ko peeche wala. Achanak jaam alag baat, wo pehle se nahi pata.
**Asli naam:** map pe paas = seedhi lakeer (straight-line) doori, sadak se kitni door = road distance / ETA (routing se, sirf gole ke kuch paas wale driver pe).

## 5. Raju ek, Arpan aur Priya do (BMS wali parchi)

Gole me ek khaali driver Raju. Arpan aur Priya ne ek hi second me "Book" dabaya, aur Raju ke phone pe do ride aa gayi.
BMS wali parchi lagao: "Raju khaali hai to Arpan ka", ek kadam me. Jiski pehle pahunchi uski parchi, doosre ko "Raju
chala gaya" aur uske liye agla driver. Parchi ki umar rakho: Raju 15 sec me accept na kare to parchi hatao, agla
driver.

**Asli naam:** parchi = driver pe hold / lock (atomic: sirf FREE ho to lagao), umar = TTL. Taala har driver ka alag,
to bheed me bhi sab ek taale pe nahi toot-te.

**Maan lo net atka, Arpan ne Book do baar daba diya:** Payment wali idempotency key: phone Book dabate hi booking id banaye, dono request me wahi. Server pehle id dekhe, pehle aa chuki to nayi ride nahi, wahi purani ride lauta do.
**Asli naam:** idempotency key (wahi ID dobara aaye to chhodo).

```mermaid
flowchart TB
    A["Arpan: Book"]:::card
    Pr["Priya: Book (usi second)"]:::card
    subgraph BP["BMS WALI PARCHI"]
        R[("Raju khaali hai to Arpan ka,<br/>ek kadam me (lock)")]:::pill
    end
    OK["Arpan: Raju mila"]:::good
    N["Priya: Raju chala gaya,<br/>agla driver"]:::card
    T["15 sec me accept nahi:<br/>parchi hatao (TTL)"]:::bad
    A --> R
    Pr --> R
    R --> OK
    R -.-> N
    R -.-> T
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style BP fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 6. Concert khatam, 50 hazaar log, 500 cab (line + surge)

Diljit ka concert khatam hua, 50 hazaar log ek saath book kar rahe, aas-paas cab sirf 500. BMS jaisa: 500 cab hain
to 500 log andar, baaki line me, agli baari. 50 hazaar ko server pe bitha ke kya karunga. Pehle 500 sambhalo, phir
kiraya badha ke door ki cab bulao.

**Asli naam:** 500 andar, baaki line = virtual queue (waiting room), kiraya badhao, door ki cab aaye = surge pricing.

```mermaid
flowchart TB
    U["Concert khatam: 50 hazaar log,<br/>cab 500"]:::bad
    subgraph BL["BMS JAISA: 500 ANDAR, BAAKI LINE ME"]
        Q(["Agli baari (virtual queue)"]):::pill
    end
    S2(["Kiraya badha ke door ki cab bulao<br/>(surge)"]):::pill
    OK["Pehle 500 sambhalo"]:::good
    U --> Q
    Q --> OK
    Q --> S2
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style BL fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

**Maan lo (reel se aaya) Ekana stadium me match khatam, 2 minute me 15,000 booking ek hi dabbe se, us dabbe wale server ka CPU 100%, baaki
shehar ke server khaali:** Driver kam pade to gola bada karo, Gomti Nagar, Chinhat se gaadi aaye. Server ka bojh: us ilaake ke liye 2-3
dabbe extra, load ek machine pe na rahe. Stadium, Connaught Place jaise hotspot pata hain, wahan pehle se 2-3 extra dabbe; 40-50 hazaar
booking ki had paar ho to system apne aap dabbe badha de.
**Asli naam:** ek dabbe pe saara bojh = hot cell (hot shard), 2-3 dabbe extra = hot cell ko chhote dabbon me tod ke kai shard pe, pehle se = pre-scaling, had paar to apne aap = auto-scaling.

## 7. Raju ne 14.9 sec pe accept dabaya, server pe 16 sec pe pahuncha (ghadi server ki)

Raju ne 14.9 sec pe accept dabaya, par server pe 16 sec pe pahuncha, tab tak Priya ko de diya. Ghadi server ki hai. 15
sec tak ruka, phir chala gaya; der se aaya to uski parchi phaad do, accept radd. Uber me bhi aise booking cancel hoti
dikhti hai.

**Asli naam:** ghadi server ki = TTL ka faisla server pe, der se aaya accept reject.

**Maan lo ek-ek driver ki jagah aas-paas ke saare driver ke phone ek saath bajao:** jo pehle accept kare uski ride.
Asli naam: broadcast dispatch; do ne ek saath accept kiya to BMS wali parchi, jo server pe pehle pahuncha wo jeeta.
Window 20-25 sec bhi kar sakte, par faisla phir bhi server ki ghadi.

```mermaid
flowchart TB
    R["Raju: accept 14.9 sec pe dabaya"]:::card
    subgraph GS["GHADI SERVER KI"]
        S{"Server pe kab pahuncha?"}:::pill
    end
    X["16 sec pe: der se, parchi phaad do,<br/>accept radd"]:::bad
    B(["Ya: aas-paas ke saare phone bajao,<br/>jo pehle accept kare (broadcast)"]):::card
    R --> S
    S --> X
    B -.-> S
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style GS fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 8. Raju Janpath gaya, naam CP ke dabbe me pada (taaza jagah dekho)

Raju Connaught Place ke dabbe se Janpath ke dabbe me gaya, par purane dabbe me naam pada hai; Arpan aur Priya dono ko
dikh raha. Sach wahi jo GPS ne abhi bheja: Raju Janpath me. Book pe Raju ki taaza jagah dekho; Arpan ke gole me nahi
to use mat do, Arpan ke liye CP me koi aur. Dono ek saath dabaayein to wahi taala.

**Asli naam:** taaza jagah dekho = assign se pehle driver ki latest location check (ek driver ki ek sach jagah), lock
double se bachata.

```mermaid
flowchart TB
    X["Raju Janpath gaya, CP ke dabbe me<br/>naam pada: Arpan, Priya dono ko dikhe"]:::bad
    subgraph TJ["SACH WAHI JO GPS NE ABHI BHEJA"]
        L(["Book pe Raju ki taaza jagah dekho"]):::pill
    end
    N["Gole me nahi: Raju mat do,<br/>Arpan ko CP me koi aur"]:::good
    T["Dono ek saath: wahi taala"]:::card
    X --> L
    L --> N
    L -.-> T
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style TJ fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 9. Register driver ke naam se, chahiye jagah ke naam se (jagah ka panna)

Register driver ke naam se laga hai ("Raju -> Connaught Place"). Booking pe chahiye "Connaught Place me kaun-kaun", to
phir 5 lakh line. Aur Raju dabba badle to purane panne me naam pada rehta, shaam tak 2,000 purane naam. To jagah ke
hisaab se baant do: Connaught Place me itne driver, Janpath me itne, AIIMS ke paas itne. 5 lakh ki jagah utni hi line
jitne us jagah me. Raju us jagah se nikla to wahan se hata do, naye panne me daal do. Raju booked hai to list se
nikaal do, ab wo kisi ko nahi milna. Booked nahi aur ghoom raha to kaat ke nayi list me, U-turn le ke wapas aaya to
phir purani list me. Booked Raju ne Connaught Place pe sawari utaari to pata hai thodi der me khaali hoga, phir list me
daal do.

**Asli naam:** jagah ke hisaab se panna = geohash -> drivers (Redis set har dabbe ka), nikla to hata do, naye me daal
do = har location update pe purane set se remove + naye me add (driver ki row me pichhla dabba yaad), booked to list
se bahar = sirf khaali driver set me, sawari khatam pe wapas.

```mermaid
flowchart TB
    X["Register driver ke naam se:<br/>CP me kaun? phir 5 lakh line"]:::bad
    subgraph JP["JAGAH KE HISAAB SE PANNA (Redis set)"]
        CP[("CP: itne driver")]:::pill
        JN[("Janpath: Raju")]:::pill
    end
    R["Raju CP se nikla: wahan se hata,<br/>naye panne me daal"]:::card
    B["Booked: list se bahar,<br/>sawari khatam pe wapas"]:::good
    X --> CP
    R --> JN
    B -.-> CP
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style JP fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 10. 10 lakh phone "Raju kahan?" poochh rahe (poochne ka mauka mat do)

Raju aa raha, Arpan ko gaadi naksha pe khisakti dikhni chahiye. 10 lakh phone har second "Raju kahan?" poochh rahe,
99% jawab "wahin", server dab gaya. Server poochh kyun raha, poochne ka mauka hi mat do. Ek aisa dabba bitha do ki Raju
hila to turant Arpan ko dikha de, hila to dikha de. Asal me hilta hai, jhooth nahi dikhate.

**Asli naam:** poochne ka mauka mat do, khud dikha do = push (WebSocket, khula taar), polling band.

![Raju hila to turant Arpan ko dikha do: khula taar, poochne ka mauka nahi](../00_IMG/15_uber_map_kadam10.svg)

## 11. Arpan server 12 pe, Raju ki jagah server 57 pe (store karwa lo, seedha raasta)

Arpan ka taar server 12 pe hai, aur Raju ki jagah server 57 pe aati. 57 ko pata hi nahi Arpan kahan. 100 server me
dhoondhna nahi. Store karwa lo: Arpan server 12 pe, Raju ki ride Arpan ke saath. Raju aur Arpan ke taar jodo, Raju ko
sirf Arpan se lena-dena. Jaise ghar ka raasta pata ho to seedha usi raaste.

**Asli naam:** store karwa lo = session map (user -> server, Redis) + ride map (driver -> rider), seedha usi raaste =
server 57 seedha server 12 ko (pub/sub channel), wahan se Arpan ke taar pe. Chat wala DSA map + chhota ladka hi.

```mermaid
flowchart TB
    S57["Server 57: Raju ki jagah aayi,<br/>Arpan kahan?"]:::card
    subgraph SK["STORE KARWA LO (Redis)"]
        M[("Arpan -> server 12<br/>Raju ki ride -> Arpan")]:::pill
    end
    S12["Server 12: Arpan ke taar pe<br/>(seedha usi raaste, pub/sub)"]:::good
    S57 --> M
    M --> S12
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style SK fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 12. 250 dikha, 500 kate (jo dikhaya wahi + tip)

Book pe ₹250 dikha, Arpan ne haan kiya. Ride ke beech surge 2 guna, aakhir me ₹500 kate. Arpan: "maine 250 dekh ke
haan kiya tha." Jo dikhaya wahi lena meri zimmedari: 250 hi lo. Traffic me phase aur driver ki mehnat zyada lagi to
rider ki marzi pe tip, 50-100 de de, na de to jitna dikhaya utna. (Uber / Rapido asal me badha deti hain, par main
banata to aise.)

**Asli naam:** jo dikhaya wahi = upfront fare: quote ride ke saath save (lock), aakhir me wahi kaato; zyada mehnat =
tip alag, rider ki marzi.

```mermaid
flowchart TB
    A["Book pe 250 dikha, Arpan ne haan kiya"]:::card
    X["Beech me surge 2 guna:<br/>500 kate"]:::bad
    subgraph JD["JO DIKHAYA WAHI"]
        F(["250 hi lo<br/>(upfront fare, ride ke saath save)"]):::pill
    end
    T["Zyada mehnat lagi: rider ki marzi pe tip"]:::good
    A --> F
    X -.-> F
    F --> T
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style JD fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 13. Trip khatam, wallet me ₹40 (poora ya kuch nahi, ride tab tak khuli)

Trip khatam, ₹250 kaatne gaye, Arpan ke wallet me sirf ₹40. Payment fail, Arpan gaadi me hi baitha, Raju saamne, aur Raju ka petrol aur
40 minute gaye. Kaatna hai to poora kaato, aadha mat kaato: 40 kaat ke chhod diya to log jaan-boojh ke 1 rupaya
rakhenge. Wallet me 250 nahi to app me saaf dikhao "payment nahi hua", aur Raju ke phone pe bhi dikhe. Arpan GPay se
de, warna cash, kaise bhi. Jab tak paisa nahi aaya, ride khatam hi nahi hogi.

**Asli naam:** poora ya kuch nahi = atomic charge (partial nahi), ride tab tak khatam nahi = ride status
PAYMENT_PENDING jab tak SUCCESS, GPay / cash = fallback payment method, dono app pe dikhe.

**Maan lo 250 kaatne gaye, gateway chup, pata nahi kata ya nahi:** Dobara kaatne pe ride id hi key: gateway pehchaan
lega ki ye wahi charge hai, do baar nahi katega. Dobara bhejne se pehle gateway se poochh lo kya hua (Payment wala
status job).

**Asli naam:** idempotency key (wahi ID dobara aaye to chhodo).

**Maan lo poochhte raho aur gateway ka jawab aaye hi na, Arpan aur Raju dono gaadi me intezaar kar rahe. Kab tak
rokoge? (BookMyShow 6c wala hi jawab):** Gateway kitni der lega, hamare haath me nahi, to hum apni ghadi
rakhte: 30 se 55 second, phir dono app pe "payment nahi hua, paisa kata to wapas aa jaayega", Arpan GPay / cash se de.
Paisa katte hi gateway chhota sa event bhejta, jaise S3 bhejta; aaya to "mil gaya". Baad me kata nikla aur cash de chuka,
to gateway ko bolo "lautao". Onboard pe SLA lo: uptime aur down hone pe khabar; bank ki der nahi.

**Asli naam:** 30-55 sec = hamara client timeout, event = webhook (PSP callback), "mil gaya" = ack (2xx), baad me kata to
lautao = REFUND_PENDING + raat ki refund job (BMS 6e) + refund API (Razorpay ko bolo lautao), REFUNDED sirf pakke jawab pe, onboard pe SLA = uptime SLA + incident response.

```mermaid
flowchart TB
    T["Trip khatam: 250 kaatne,<br/>wallet me 40"]:::bad
    subgraph PK["POORA KAATO YA KUCH NAHI"]
        A(["40 kaat ke mat chhodo<br/>(atomic charge)"]):::pill
    end
    S["Dono app pe: payment nahi hua,<br/>GPay / cash"]:::card
    OK["Paisa aaya tabhi ride khatam<br/>(PAYMENT_PENDING tak khuli)"]:::good
    T --> A
    A --> S
    S --> OK
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style PK fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

## 14. (Socha hua) Ride ke beech Raju ka phone band (location timeout + support)

Ride ke beech Raju ka phone switch off, GPS bhi band, naksha pe Raju gayab. Ride khatam kaun karega? Phone band hona
Uber ke haath me nahi, ye design ki dikkat nahi. Phone apni location khud bhejta hai, server poochhta nahi. Location
aani band hui to server ek-do baar dekhega, phir apni taraf se bhi band. Jahan tak aaye utne paise Arpan ki app me
dikhenge, wo de do. Phir Uber support ko phone: "driver ka phone band ho gaya". Support wahan se ride band kar dega,
DB me update, Raju kahin nahi dikhega.

**Asli naam:** location aani band, ek-do baar dekha = location timeout (phone push karta, server sirf intezaar karta),
utne paise = jahan tak ka kiraya / jo dikhaya, support ride band kare = manual close, ride status DB me update, driver
available list se bahar.

```mermaid
flowchart TB
    X["Ride ke beech Raju ka phone band,<br/>naksha pe gayab"]:::bad
    subgraph LT["LOCATION AANI BAND"]
        W(["Server ek-do baar dekhe,<br/>phir apni taraf se band (timeout)"]):::pill
    end
    P["Jahan tak aaye utne paise:<br/>Arpan ki app me"]:::card
    S["Support: ride band, DB update,<br/>Raju kahin nahi"]:::good
    X --> W
    W --> P
    P --> S
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style LT fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

---

## 15. Arpan aur Ravi saath, beech wali gaadi galat ko gayi (batch matching + aging)

Arpan aur Ravi ne saath me book kiya. Do gaadi hain: Car B Arpan se 3 km, Car A Arpan se 2 km aur Ravi se 1 km, aur
Car B Ravi se 6 km. Arpan ki booking pehle pahunchi, to purane niyam se paas wali Car A Arpan ko mili aur Ravi ke liye
Car B bachi, 6 km door. Kul 8 km. Ek booking ko akele mat dekho: dono user aur dono gaadi ki jagah rakh ke har jodi ki
doori nikaalo, phir jodi aisi do ki sabki milake doori kam aaye. Ravi ko beech wali Car A (1 km), Arpan ko bahar wali
Car B (3 km), kul 4 km. Doori km me naapo, minute traffic se ghat-badh jaate hain. Aur ek baar driver ne accept kar liya
to gaadi pakki, use todna nahi; driver khud cancel kare tabhi dobara doori nikaalo.

**Asli naam:** dono ko saath milao, kul doori kam = batch matching (kai booking aur kai gaadi ek saath, sabka milake
pickup kam).

**Maan lo Arpan ki booking 7:00:00 pe aayi aur Ravi ki 7:00:02 pe, aur server ne turant Car A Arpan ko de di:** Booking
aate hi gaadi mat do. Maan lo 15-20 second ruko, user itna ruk lega (Uber me bhi driver dhoondhne me itna lagta hai). Us beech
jitni booking aayi, sab ko saath milao, phir gaadi do, taaki jaldbaazi me galat gaadi kisi ko na jaaye.
**Asli naam:** 15-20 second ruko, sab saath milao = batching window.

**Maan lo office chhutne ka waqt, har baar koi naya banda gaadi ke paas mil jaata, aur Arpan 7:05 tak "finding driver" pe:**
Gaadi extra ho to dikkat hi nahi. Gaadi ek hi ho aur Arpan ke saath 1-2 baar aisa ho chuka, to agli gaadi Arpan ki hi,
chahe wo kisi aur ke zyada paas ho. Paas wale bande se bol do "thodi der aur dekh le, tere liye aur gaadi aa rahi". Arpan line
me hi nahi to ye niyam lagta hi nahi.
**Asli naam:** baar-baar peeche reh gaya = starvation, jitna ruka utni baari upar = aging (gaadi nahi badhti, sirf baari
tay hoti).

**Maan lo Arpan ne 5 minute ruk ke gusse me cancel kiya aur turant dobara book kiya, server ne ginti 7:05 se shuru kar di:**
Do raaste. (1) Cancel kiya to line se hata do, dobara aaye to peeche lage, kyunki beech me aur log aa gaye. (2) Redis me
parchi "Arpan 5 minute ruka tha", 30 second ki umar; 30 second ke andar wapas aaya to uske 5 minute chalte rahenge,
warna parchi khatam aur line me peeche. Sabka dekhna hai: 30 second ke baad bhi Arpan aage rahe to beech me aaye logon ka
nuksaan; cancel Arpan ne kiya, to peeche lagna uski galti ka nateeja, baaki koi pareshaan nahi.
**Asli naam:** parchi = Redis me pehle tap ka waqt (intezaar pehle tap se gino), umar = TTL.

```mermaid
flowchart TB
    subgraph MAP["SADAK PE KAUN KAHAN"]
        direction LR
        B["Car B"]:::card ---|3 km| AR(["Arpan"]):::pill
        AR ---|2 km| A["Car A"]:::card
        A ---|1 km| RV(["Ravi"]):::pill
    end
    subgraph OLD["PURANA: jo pehle aaya, use paas wali"]
        O1["Arpan ko Car A: 2 km<br/>Ravi ko Car B: 6 km<br/>kul 8 km"]:::bad
    end
    subgraph NEW["NAYA: dono ko saath milao (batch matching)"]
        N1["Ravi ko Car A: 1 km<br/>Arpan ko Car B: 3 km<br/>kul 4 km"]:::good
    end
    MAP --> OLD
    MAP --> NEW
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style MAP fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
    style OLD fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
    style NEW fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

```mermaid
flowchart TB
    BK["Booking aayi:<br/>Arpan 7:00:00, Ravi 7:00:02"]:::card
    W(["15-20 sec ruko, beech ki<br/>saari booking jama (batching window)"]):::pill
    C["Cancel karke 30 sec me wapas:<br/>Redis parchi, 5 min chalte rahe (TTL)"]:::pill
    AG{"Koi 1-2 baar<br/>peeche reh gaya?"}:::card
    P["Haan: agli gaadi usi ki (aging)"]:::good
    M["Nahi: sab booking + sab gaadi saath,<br/>kul doori kam (batch matching)"]:::good
    BK --> W
    W --> AG
    C -.-> AG
    AG --> P
    AG --> M
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
```

---

## 16. Arpan ke map pe Raju ki gaadi live (adaptive ping, map matching, dead reckoning)

Ye Zomato wali reel se aaya (Aman ki biryani, Raju ki bike), par Uber me bhi wahi: Arpan map pe Raju ki gaadi apni taraf
aati dekhta hai. Raju signal pe 15-20 minute khada hai aur phone phir bhi har 4 second wahi jagah bhej raha, battery 15%.
Driver hil hi nahi raha to baar-baar location ka kya kaam; jaise sote aadmi ke kamre me baar-baar nahi jaate, jagega tab
jaate. Khada dikhe to 4 second ki jagah 10, 15-20 second pe bhejo; location band nahi, bas der se. Gaadi chali to phir
har 4 second.
**Asli naam:** khada hai to der se, chala to jaldi = adaptive ping (speed ke hisaab se location ka waqt, battery bachti).

**Maan lo Raju Connaught Place ki oonchi buildings ke beech hai, GPS takra ke galat jagah bata raha, gaadi map pe ek
building ki chhat pe dikh rahi (ghost bike):** Server pe dikha location building ke upar aa rahi; asli jagah pe neeche
sadak hai, signal buildings me ulajhta. Hamein pata hai Raju is building ke aas-paas hai aur wahan sadak kaunsi hai, to
chhat pe dikhe to use paas wali sadak pe wapas utaar do.
**Asli naam:** paas wali sadak pe chipka do = map matching (GPS ka galat point sabse paas ki sadak pe, raaste ke hisaab se).

**Maan lo location har 4 second aati hai, beech me gaadi khadi, phir achanak aage koodti (teleport):** Har pal ki asli
jagah dene wali technology nahi, ho bhi to bahut mehngi. Bike chalaane wala seedha aage hi jaata, to map pe gudde ko
halka-halka aage chalate raho; nayi location aaye to sahi jagah pe rakh do. Par flyover ke neeche Raju ne U-turn le liya
aur hum gudde ko aage chala rahe the; signal aaya to asli jagah peeche, gudda uth ke wahan teleport. Ye Uber, Rapido,
Zomato, Swiggy sab me dikhta hai.
**Asli naam:** gudde ko andaze se aage = dead reckoning (speed + sadak se andaza), nayi aayi to sahi jagah = correction,
U-turn pe kooda = andaza galat nikla.

**Maan lo Raju Arpan ki society ki basement parking me ghusa, phone 5 minute bilkul offline; andaze se chalate rahe to
gaadi shehar ke bahar, ruki dikhi to lage Raju bhaag gaya:** Aakhri location pata hai, Raju building ke paas pahunch
gaya. Gaadi wahin rok do aur map ki jagah dikha do "Raju tumhari building pahunch gaya"; Arpan ko shaanti, map dekhna
band. Zomato me Raju khana le ke upar bhi aata hai, isliye wahan do status: pehle "building tak pahuncha", phir haath me
diya tab "order mil gaya". Uber me Arpan neeche aata hai, "Raju pahunch gaya" kaafi.
**Asli naam:** aakhri jagah pe roko = last known location (andaze se aage nahi), map ki jagah status = status update
(arrived / delivered), jagah se pata chala pahuncha = geofence.

```mermaid
flowchart TB
    L["Raju ka phone:<br/>GPS location"]:::card
    A{"Raju khada hai?"}:::card
    A1(["10-20 sec me ek baar<br/>(adaptive ping)"]):::pill
    A2(["Har 4 sec<br/>(chal raha)"]):::pill
    S["Server: point chhat pe?<br/>paas wali sadak pe chipka do<br/>(map matching)"]:::good
    M["Arpan ka map: beech me gudda<br/>andaze se aage (dead reckoning),<br/>nayi aayi to sahi jagah (correction)"]:::good
    O{"5 min koi location nahi?"}:::card
    X["Aakhri jagah pe roko +<br/>'Raju pahunch gaya' (status)"]:::good
    L --> A
    A -->|haan| A1
    A -->|nahi| A2
    A1 --> S
    A2 --> S
    S --> M
    M --> O
    O -->|haan| X
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef pill fill:#f5c542,stroke:#f5c542,color:#000
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
```

```mermaid
flowchart LR
    subgraph U["FLYOVER KE NEECHE U-TURN"]
        direction LR
        P1["Aakhri asli jagah"]:::card --> G["Gudda andaze se<br/>aage chalta raha"]:::bad
        P1 -.->|Raju ne U-turn liya| R["Asli Raju:<br/>100 m peeche"]:::good
        G -->|signal aaya,<br/>gudda kooda| R
    end
    linkStyle default stroke:#8fa3c7,stroke-width:2px
    classDef card fill:#1e2638,stroke:#3a4a6b,color:#e8ecf5
    classDef good fill:#2e7d4f,stroke:#3fbf6f,color:#fff
    classDef bad fill:#5c1f24,stroke:#e05561,color:#fff
    style U fill:#141a29,stroke:#3a4a6b,color:#9fb3d9
```

---

Notes (Arpan ke shabd) = [UBER_HAND_NOTES.md](UBER_HAND_NOTES.md)
