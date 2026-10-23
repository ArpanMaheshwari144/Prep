# BookMyShow: shuru se, ek kahani

Kram wahi jo interview me hota hai: **FR, phir NFR, phir numbers, phir design**. Design sabse seedhe dabbe
se shuru hota hai, aur har dikkat aane pe ek naya dabba judta hai. Diagram me peela dabba = abhi juda. Poori
file me ek hi kahani hai: Arpan ko Ravi ke saath PVR ka 9 baje wala show dekhna hai, seat A1.

**CUE (yahi shabd yaad rakhne hain, kram se):**
padho phir likho (do log ek seat) -> ek query UPDATE ... WHERE FREE -> HELD 5 min (hold_until)
-> seat map Redis cache -> waiting room (token, signed parchi) -> pay: idempotency key + PSP webhook -> BOOKED

**Teeno ka jod (banking, payment, BMS):** andar ka dhaancha ek hi hai. Jo kadam banking / payment me pehle
se hai, wahan bas ek line "ye wahi hai"; poora sirf wahi jo BMS ka apna hai.

```
dikkat                         banking            payment             BMS
do log ek saath, ek cheez      kadam 4: balance   (banking wala)      seat A1: UPDATE ... WHERE status = FREE
                               check + kaato
network gaya, dobara bheja     kadam 3: idem key  (banking wala)      pay dobara -> wahi idempotency key
bahar ka paisa (bank / PSP)    -                  kadam 2-4: PSP,     ticket ka paisa = payment file poori
                                                  PENDING, webhook
BMS ka APNA                    -                  -                   seat HOLD 5 min (umar ke saath),
                                                                      seat map ki bheed (cache),
                                                                      concert ki line (waiting room)
```

Click karke wahi kadam: [banking kadam 3 (key)](../13_banking_system/13_banking_system.md#kadam-3-network-gaya-app-ne-dobara-bheja-idempotency-key) ·
[banking kadam 4 (ek query)](../13_banking_system/13_banking_system.md#kadam-4-do-kataai-ek-saath-balance--400-check--kaato-ek-query-me) ·
[payment kadam 2 (PSP)](../06_payment_system/06_payment_system.md#kadam-2-bank-se-wallet-me-2000-psp) ·
[payment kadam 4 (webhook)](../06_payment_system/06_payment_system.md#kadam-4-pakka-kaise-pata-chale--hamara-server-gira-pending-pehle-webhook-poochne-wala-job)

---

## 1. FR: kya karna hai

```
1. movie / show dekhna, seat map dekhna
2. seat chuno -> pay karo -> ticket pakka
scope ke bahar: refund, review, recommendation
```

## 2. NFR: kaisa chalna chahiye

```
- ek seat kabhi do logon ko na bike (yahi dil hai)
- seat map dekhna tez (bheed yahin hoti)
- badi movie / concert ke din ki bheed jhele
```

## 3. Numbers

```
- dekhne wale bahut, book karne wale kam: 100 log map dekhte to ~1-2 book karte
  -> dekhne ke liye cache, likhne ke liye ek pakka SQL DB
- active data chhota (aaj + agle hafte ke show) -> sharding nahi chahiye, purana archive
```

---

## 4. Design

### Kadam 1: sabse seedha (BMS Svc + SQL DB, pehle padho phir likho)

BMS Svc aur ek SQL DB. `seats` table me har seat ki row: `show 9pm | A1 | FREE`. Arpan "Book A1" dabata hai.
BMS Svc pehle padhta hai "A1 FREE hai?", haan. Phir likhta hai `status = BOOKED`.

```mermaid
flowchart LR
    U["Arpan"] -- "Book A1" --> S["BMS Svc"]
    S -- "1. padho: A1 FREE?<br/>2. likho: A1 = BOOKED" --> D[("SQL DB: seats")]
    style S fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style D fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Dikkat: Ravi ne bhi usi second me A1 dabaya. Dono ne padha "FREE", dono ne likha "BOOKED". Dono ko ticket,
ek seat pe do log. Ye banking ke ["do kataai ek saath, balance -400"](../13_banking_system/13_banking_system.md#kadam-4-do-kataai-ek-saath-balance--400-check--kaato-ek-query-me) jaisi hi dikkat hai: pehle padho, phir
likho, aur beech ke pal me koi aur ghus jaata hai.

### Kadam 2: padhna aur likhna ek hi query me (banking kadam 4 wala hi)

"Pehle padho, phir likho" do alag kaam hain, aur dono ke beech Ravi ghus gaya. Ilaaj: dono ko ek hi query me.

```sql
UPDATE seats SET status = 'BOOKED', user = 'Arpan'
WHERE show = '9pm' AND seat = 'A1' AND status = 'FREE';
```

DB is row pe lock lagata hai, to Arpan aur Ravi ki query ek ke baad ek chalti hai, saath nahi:

```
Arpan ki query pehle: A1 FREE hai -> BOOKED, user = Arpan  -> "1 row badli" -> Arpan ko seat mili
Ravi ki query baad me: A1 ab FREE nahi, WHERE match nahi  -> "0 row badli" -> Ravi: "seat chali gayi"
```

[Banking me yahi tha](../13_banking_system/13_banking_system.md#kadam-4-do-kataai-ek-saath-balance--400-check--kaato-ek-query-me): `UPDATE ... SET balance = balance - 500 WHERE balance >= 500`. Wahan shart "balance
hai?", yahan shart "seat FREE hai?". Baaki sab same.

Par seat book karte hi paisa nahi aata, Arpan ko pay karne me 3 minute lagte hain. Do raaste, dono kharab:
- pehle seat BOOKED, phir pay: Arpan ne pay hi nahi kiya, app band kar di, to A1 hamesha ke liye BOOKED padi
  rahegi, kisi ko nahi milegi.
- pehle pay, phir seat: 3 minute me Ravi le gaya, to Arpan ka paisa kat gaya aur seat nahi mili.

### Kadam 3: seat HOLD 5 minute, umar ke saath (BMS ka apna)

Na pehle BOOKED, na pehle pay. Beech ka ek haal: **HELD** = "Arpan ke liye 5 min ruki hui".
Seat 5 min Arpan ki; pay kiya to pakki, nahi kiya to apne aap wapas FREE.

```sql
-- seat chuni: HOLD (Kadam 2 wali ek query, bas haal HELD + umar)
UPDATE seats SET status = 'HELD', user = 'Arpan', hold_until = now() + 5 min
WHERE show = '9pm' AND seat = 'A1'
  AND (status = 'FREE' OR (status = 'HELD' AND hold_until < now()));   -- purana hold mar chuka to bhi le lo

-- pay ho gaya: pakka (sirf apna, aur umar abhi baaki ho)
UPDATE seats SET status = 'BOOKED'
WHERE seat = 'A1' AND user = 'Arpan' AND status = 'HELD' AND hold_until > now();
```

```
9:00:00  Arpan ne A1 chuni   -> HELD, Arpan, 9:05 tak
9:01:00  Ravi ne A1 chuni    -> HELD hai, umar baaki -> "seat abhi kisi ke paas hai"
9:02:30  Arpan ne pay kiya   -> BOOKED. pakka

dusri kahani: Arpan ne pay kiya hi nahi
9:05:01  Ravi ne A1 chuni    -> hold_until < now -> purana hold mara -> Ravi ka HELD
```

```mermaid
flowchart LR
    U["Arpan"] -- "A1 chuni" --> S["BMS Svc"]
    S -- "HELD, Arpan, 9:05 tak" --> D[("SQL DB: seats<br/>status + hold_until")]
    U -- "pay" --> P["Payment<br/>(payment file)"]
    P -- "pay ho gaya" --> S
    S -- "HELD -> BOOKED" --> D
    J["safai job (har minute)"] -. "purane HELD -> FREE" .-> D
    style D fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style J fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Koi job chahiye jo 5 min baad seat FREE kare? Zaroori nahi. Shart query me hi likhi hai
(`hold_until < now()`), to mara hua hold apne aap gina hi nahi jaata.
Ek chhota job har minute purane HELD saaf kar deta hai, bas taaki seat map pe A1 jaldi khaali dikhe.

Arpan ne 5:30 min pe pay kiya, hold mar chuka tha, aur seat Ravi le gaya? Paisa kat chuka, seat nahi mili ->
paisa wapas (refund). Ye [payment file](../06_payment_system/06_payment_system.md#kadam-4-pakka-kaise-pata-chale--hamara-server-gira-pending-pehle-webhook-poochne-wala-job) wala hi hissa hai (PSP, PENDING, webhook).

Par 9 baje ke show ka seat map lakhon log dekh rahe hain, book koi-koi kar raha hai. Har "map dekho" pe SQL DB
se 200 seat padhna. DB padhne me hi ghut raha hai, aur Arpan ki booking query bhi dheemi ho gayi.

### Kadam 4: seat map ka cache (Redis)

Har "map dekho" pe SQL tak jaana band. Redis me har show ka seat map ek key me:
`map:9pm = { A1: HELD, A2: FREE, ... 200 seat }`.

```
lakhon log "map dekho" --> [Redis: map:9pm] --mila--> turant (SQL tak gaye hi nahi)
                                 | nahi mila / umar khatam
                                 +--> SQL se 200 seat padho -> Redis me rakho (umar 5 sec)

Arpan ne A1 HOLD ki --> SQL me likha (Kadam 3) --> Redis ki map:9pm DELETE --> agla dekhne wala naya map
```

```mermaid
flowchart LR
    V["lakhon log<br/>map dekh rahe"] --> R[("Redis: map:9pm<br/>umar 5 sec")]
    R -. "nahi mila" .-> D[("SQL DB: seats")]
    U["Arpan: A1 HOLD"] --> S["BMS Svc"]
    S -- "HOLD query" --> D
    S -- "map:9pm DELETE" --> R
    style R fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Map 2-5 sec purana dikhe to chalega? Haan. Map sirf dikhane ke liye hai, faisla nahi karta. Ravi ko cache me
A1 FREE dikha, jabki asal me Arpan le chuka tha. Ravi ne A1 dabaya, Kadam 3 wali query SQL pe chali: "0 row
badli, seat chali gayi". Sach hamesha SQL me, cache sirf aankh ke liye. Yahi banking ke [read replica](../13_banking_system/13_banking_system.md#kadam-8-sab-balance-dekh-rahe-transfer-dheeme-read-replica) me tha:
balance dekhna copy se, paisa kaatna hamesha primary se. Yahan map dekhna cache se, seat lena hamesha SQL se.

Ab kya theek hua: lakhon "dekho" Redis jhel leta hai, SQL sirf booking ki query dekhta hai, Arpan ki booking
tez.

Par 12 baje Arijit Singh ke concert ki booking khuli. 2 lakh log ek hi second me, ek hi show pe "Book" daba
rahe hain, seat sirf 5000. Saari HOLD query ek hi table ki chand rows pe toot padti hain. SQL lock ki line me
phans jaata hai, sab ka app ghoomta rehta hai.

### Kadam 5: waiting room (line me lagao, thode-thode andar bhejo)

2 lakh log seedhe booking pe nahi jaate. Pehle ek **line** me lagte hain
(Redis me line, jo pehle aaya wo aage).

```
12:00:00  2 lakh log "Book" --> [WAITING ROOM: Redis line]
                                  Arpan: "aap 45,000 number pe ho, ~8 min"
                                    |
                                    | har minute sirf 2,000 ko token (andar aane ki parchi)
                                    v
                        token wale --> seat map --> HOLD query (Kadam 3) --> pay
                        token ki umar 10 min, fir dobara line
```

```mermaid
flowchart LR
    B["2 lakh log<br/>Book dabaya"] --> W[("WAITING ROOM<br/>Redis line")]
    W -- "har minute 2,000 ko token" --> S["BMS Svc"]
    S -- "HOLD query" --> D[("SQL DB: seats")]
    style W fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

SQL pe ek waqt me sirf 2,000 log, 2 lakh nahi. Lock ki line chhoti, query tez. Aur jo pehle aaya wo pehle
andar, to sabke saath barabari.

Doosra kyun nahi, "SQL ki aur machine laga do"? Machine badhane se nahi chalega. Jhagda 5000 seat ki chand
rows pe hai, aur ek row ka lock ek hi machine pe hota hai. Machine 10 kar do, A1 ki row phir bhi ek hi hai.
Bheed ko DB tak pahunchne se pehle hi rokna padta hai.

Ek aadmi 50 tab khol ke line me 50 baar lage? Token user ke login se juda hai, ek user ek token. Bot ki bheed
ke liye wahi rate limiter jo [rate limiter file](../02_rate_limiter/02_rate_limiter.md) me hai.

**Redis primary + copy dono gaye, to?** Pehle ek baat pakki: dil (ek seat ek hi ticket) SQL me hai, Redis me
nahi. HOLD, BOOKED, payment sab SQL me. Redis poora gaya to system dheema ya ruka ho sakta, galat nahi.

```
1. SEAT MAP (dikhane wala cache) gaya
   -> har BMS Svc apni memory me aakhri map 5 sec rakhta (chhota local cache)
   -> lakhon "dekho" usi se, SQL tak sirf 5 sec me ek baar
   -> map thoda purana dikhe to chalega (Kadam 4: faisla SQL karta)

2. WAITING ROOM (line) gaya, concert ke beech
   -> jo andar hain: unka token ek SIGNED parchi hai (umar ke saath)
      BMS Svc parchi ka signature khud check karta, Redis nahi chahiye -> wo booking poori karte
   -> jo line me the: line ka kram kho gaya
      -> darwaza BAND (fail-closed): "booking thodi der me, ruko" page
      -> seedha 2 lakh ko andar nahi chhodte, warna SQL ghut jaayega (Kadam 5 ki dikkat wapas)
   -> Redis wapas -> line naye sire se
```

Darwaza band kyun, khula kyun nahi: rate limiter me fail-open = "Redis gira to sabko jaane do". Yahan khula
chhoda to 2 lakh seedhe SQL pe, sabki booking atak jaayegi. Band rakha to kuch der koi book nahi karega, par
jo andar hain unki booking poori, aur galat kuch nahi.

Ek line: Redis gaya to raftaar aur line ki barabari gayi, sahi-galat nahi. Map local memory se, andar walon ki
signed parchi chalti rahe, line ke liye darwaza band jab tak Redis wapas na aaye.

Par Arpan andar gaya, A1 HOLD ki, Pay dabaya. Network gaya, app ne apne aap dobara bheja. Paisa do baar kata.

### Kadam 6: pay dobara + paisa (banking kadam 3 + payment file)

Naya kuch nahi, teen padhe hue tukde BMS me judte hain.

**1. Idempotency key ([banking kadam 3](../13_banking_system/13_banking_system.md#kadam-3-network-gaya-app-ne-dobara-bheja-idempotency-key) wala hi):** app har "Pay" ke saath ek key bhejta hai,
yahan wo key booking ki id hi hai: `booking B-77`. Payments table me `UNIQUE(booking_id)`.

```
pehla Pay  : B-77 -> table me nahi -> paisa kaato, row likho
dobara Pay : B-77 -> row pehle se  -> "ho chuka", wahi purana jawab. Paisa dobara nahi kata
```

**2. Paisa bank se ([payment file](../06_payment_system/06_payment_system.md#kadam-2-bank-se-wallet-me-2000-psp) wala hi):** BMS khud bank se baat nahi karta. Payment file ka raasta: PSP
(Razorpay), pehle PENDING, phir PSP ka webhook "SUCCESS".

**3. Webhook aaya, tab seat pakki (BMS ka jod):**

```
PSP webhook "B-77 SUCCESS" --> Kadam 3 wali query: A1 HELD -> BOOKED (sirf agar umar baaki)
                                   |-- 1 row badli -> ticket pakka, Arpan ko SMS / email (notification file)
                                   |-- 0 row badli -> hold mar chuka tha, seat gayi -> paisa wapas (refund)
```

```mermaid
flowchart LR
    U["Arpan"] -- "Pay (key B-77)" --> S["BMS Svc"]
    S --> P["Payment Svc<br/>(PENDING, UNIQUE booking_id)"]
    P --> G["PSP -> bank"]
    G -- "webhook: B-77 SUCCESS" --> S
    S -- "A1 HELD -> BOOKED" --> D[("SQL DB: seats")]
    S -- "ticket ka SMS" --> N["Notification"]
    style P fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

Yahan teeno files ek saath judti hain: banking se key, payment se PSP + webhook, [notification](../04_notification_system/04_notification_system.md) se ticket
ka SMS.

---

## Wrap: 30 sec me poora design

```
Seat ka faisla SQL me ek query se: UPDATE ... WHERE status = FREE (banking kadam 4 wala), to ek seat kabhi do
ko nahi. Seat chuni = HELD 5 min (hold_until), pay pe BOOKED, umar khatam to apne aap wapas. Seat map lakhon
dekhte, isliye Redis cache (purana chalega, faisla SQL karta). Concert ki bheed: waiting room, har minute
thode token, signed parchi. Pay pe idempotency key (booking id) + payment file ka PSP / webhook; webhook pe
seat pakki, hold mar chuka to refund. Redis poora gaya to dheema, galat nahi.
```

---

## Pooche to (design nahi badalta, bas ek baar dekh lo)

```
API?
  -> GET /shows/{id}/seats (FREE / HELD / BOOKED)
     · POST /bookings { showId, seats } -> HOLD -> bookingId + umar
     · POST /bookings/{id}/pay. User JWT se lo, body se nahi (warna kisi aur ke naam se book).

Tables?
  -> seats = show_id, seat_id, status, user_id, hold_until
     bookings = booking_id, user_id, show_id, seats, status · payments = booking_id (UNIQUE), status

SQL kyun, NoSQL kyun nahi?
  -> sahi hona hi dil hai. Row lock + ek query se ek seat do ko bikna namumkin. NoSQL ka "thodi der baad sahi"
     = do machine alag-alag "FREE" bol sakti, khatra. Data chhota (aaj + agla hafta), to shard bhi nahi.

App machine giri to?
  -> App kuch yaad nahi rakhta (stateless), hold SQL me. LB doosri machine pe bhej deta, kuch nahi gaya.

SQL primary gira to?
  -> sync copy (Multi-AZ) ~30-120 sec me primary banti. Booking = paisa, isliye copy sync. App wahi key se
     dobara bheje (idempotent).

2 seat ek saath (A1 + A2), ek mili ek nahi?
  -> dono UPDATE ek hi transaction me. Dono ki "1 row badli" tabhi commit, warna rollback: dono milein ya
     koi nahi.

Chal raha hai, kaise pata?
  -> booking p99, "0 row badli" (seat ja chuki) ginti, kitne hold umar se khatam,
     waiting room me line kitni lambi.

"BookMyShow sach me aise karta?"
  -> mat bolo "BMS aise karta". Bolo "a common pattern for flash sales", unka andar ka pata nahi.
```

CONCEPTS: [db-what-when](../../FOUNDATIONS/09_databases_what_when.md) ·
[CAP](../../FOUNDATIONS/08_cap_theorem.md) · saath: [payment](../06_payment_system/06_payment_system.md) ·
[banking](../13_banking_system/13_banking_system.md) ·
[<- MASTER SHEET](../../00_MASTER_SHEET.md)
