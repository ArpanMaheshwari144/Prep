# BookMyShow (Ticket Booking)

> Movie / show dekho, seat chuno, pay karo, ticket. JP ke liye kaam ka: paisa aur seat ek saath sahi ("paisa do baar kharch na ho" jaisa).
> Dil do cheezein: ek seat do logon ko na bike (ek saath booking), aur badi movie ke din ki bheed.
>
> Mock me sirf heading + Cue + poster. Poster me sab: upar "Pehle aisa tha", tasveer (ilaaj), hara "Kyun chalta", laal number = kya toot sakta + kaise bacha, peela = doosra tareeka kyun nahi, grey = baaki.
> Cross-Q: hal ke har bade shabd pe socho -> gira? · dheema / bheed? · do ek saath? · galat hua? · kaise / kitna?

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c7_2-64f79499.svg)

---


## Shuru

```
FR      movies aur shows dekhna, seat map dekhna, seat book karna (payment ke saath) aur ticket pakka hona.
        Refund, recommendation aur review scope se bahar hain
NFR     ek seat kabhi do logon ko na bike (yahi dil hai), dekhna tez ho, badi movie ki bheed jhel le,
        aur system bharosemand ho
NUMBERS 1 crore user hain, par bojh do alag hain, isliye ek number mat bolo.
        Dekhna (shows, seat map) sab karte hain, lakhon padhai hoti hai, isliye cache aur read replica.
        Booking kam log karte hain, par ek hi seat pe ek saath aur jhatke me, isliye queue.
        Active data chhota hai (aaj aur agle hafte ke show), purana archive me jaata hai, isliye sharding nahi
PEHLE   seats table me seat_id, show_id, status. "Book" dabaya to status = 'booked'. Pehle padha (khaali),
        phir likha, to 10:00:01 pe dono ne A1 khaali dekhi aur dono ne book kar di
```

> [!IMPORTANT]
> **RAIL: har design isi kram me chalta (8 dabbe, sab file me same)**
> kaam chala → bheed aayi → machine giri → do log ek saath → kuch dheema → data bada → door ke log → bahar ka hamla
>
> | rail | is design me | card |
> |---|---|---|
> | 1 kaam chala | seat chuno, pay karo, ticket · payment me 3 min, seat koi le gaya | 2 |
> | 2 bheed aayi | 11 baje 1 lakh log seat map dekh rahe · 12 baje concert, 2 lakh ek show pe | 4, 5 |
> | 3 machine giri | Redis restart, saara map replica pe · App machine giri | 6, 7 |
> | 4 do log ek saath | Arpan aur Ravi ek second me A1 (**is design ka khaas: ek seat ek ticket**) · pay dobara gaya | 1, 3 |
> | 5 kuch dheema | alag nahi: payment ka intezaar = hold (card 2) | 2 |
> | 6 data bada | nahi aata | - |
> | 7 door ke log | nahi aata | - |
> | 8 bahar ka hamla | koi 10 seat hold karke baith gaya (card 2 ka cross-Q) | 2 |


> [!NOTE]
> **AWS pe kaunsa DB (interview me naam bolna ho to)** · SQL wala = RDS, Cassandra RDS pe nahi chalta
>
> - Seat / booking (SQL) → **AWS RDS (Postgres / MySQL)**: ek seat ek hi baar bike: ACID + row lock
> - Seat map (Redis) → **ElastiCache (Redis)**: dekhne ki bheed, ~5 sec purana chalta

---

*ab tootega kahan? sabse pehle wahin jahan dil hai, ek seat pe do log*

## 1. Maine aur Ravi ne ek hi second me A1 book ki, dono ko mil gayi

**Rail 4 DO LOG EK SAATH (khaas)** · Cue: ATOMIC UPDATE ... WHERE available

<img src="img/c1_poster.png?v=2" width="1000" alt="card 1: kya toota, kaise bacha">


---

*ab seat ek ko mili, par payment me waqt lagta*

## 2. Maine A1 chuni, payment me 3 minute lag rahe

**Rail 1 KAAM CHALA (hold)** · Cue: HOLD + held_until (umar)

<img src="img/c2_poster.png?v=2" width="1000" alt="card 2: kya toota, kaise bacha">


---

*ab seat pakad li, par pay ka network gaya*

## 3. Pay dabaya, network gaya, app ne dobara bheja

**Rail 4 DO LOG EK SAATH (pay)** · Cue: IDEMPOTENCY KEY (bookingId)

<img src="img/c3_poster.png?v=2" width="1000" alt="card 3: kya toota, kaise bacha">


---

*ab booking sahi, par dekhne wale bahut*

## 4. 11 baje Arpan seat map dekh raha tha, saath me 1 lakh log, book koi-koi

**Rail 2 BHEED AAYI (map)** · Cue: CACHE + READ REPLICA

<img src="img/c4_poster.png?v=2" width="1000" alt="card 4: kya toota, kaise bacha">


---

*ab dekhna sambhal liya, par badi release pe likhne wale bhi lakhon*

## 5. 12 baje Arijit ka concert khula, 2 lakh log ek show pe

**Rail 2 BHEED AAYI (sale)** · Cue: WAITING ROOM + Kafka (sirf badi sale)

<img src="img/c5_poster.png?v=2" width="1000" alt="card 5: kya toota, kaise bacha">


---

*ab line bani, par cache hi khaali ho gaya*

## 6. 2 baje Redis restart, saara seat map ek saath replica pe

**Rail 3 MACHINE GIRI (Redis)** · Cue: SET NX lock, pehla hi banaye

<img src="img/c6_poster.png?v=2" width="1000" alt="card 6: kya toota, kaise bacha">


---

*ab data surakshit, par App machine hi giri*

## 7. Shaam ko Arpan seat chun raha tha, App machine gir gayi

**Rail 3 MACHINE GIRI (App)** · Cue: STATELESS App + LB health check

<img src="img/c7_poster.png?v=2" width="1000" alt="card 7: kya toota, kaise bacha">


---

## Poora design ek saans me

```
DEKHNA:  LB -> App (stateless) -> Redis Cluster (99%) -> read replica
BOOKING: aam din: App -> SQL primary seedha: atomic conditional UPDATE (HOLD + umar), turant jawab
         badi sale: App -> Kafka (har show ek line) -> Booking worker -> wahi UPDATE ("booking ho rahi hai")
         -> Payment Svc (idempotency key) -> booked
Sweeper = purane hold saaf (sirf safai; asli faisla UPDATE ke WHERE me)
DB:      SQL (ACID + row lock), active data chhota, shard nahi
```

Bolne ke liye: seat map Redis aur copy se, booking primary DB pe ek query se ("khaali ho tabhi book"), to ek seat kabhi
do ko nahi. Seat chuni = 5 minute hold, query purana hold khaali maanti, job safai karti. Release ke din ki bheed:
darwaze pe ginti + har show ki ek line. Payment pe id, do baar nahi katega. SQL kyunki sahi hona sabse zaroori, data
chhota to baantna nahi.

---

## 10 guna traffic

- **App**: stateless, machine badhao.
- **Dekhna**: Redis cluster (99%) + read replica.
- **Booking**: atomic conditional UPDATE / row lock, HOLD + umar.
- **Bada show**: Kafka + har show ek line + darwaze pe ginti + waiting room. Garam row pe ek hi jeete.
- **Payment**: idempotency key.
- **SQL**: replica + apne aap failover. Data chhota, shard nahi.
- **Aage**: virtual waiting room. Kai DB / shard ho to seat ka faisla usi shard ke atomic UPDATE se (Redlock = Redis
  ka lock, DB ka nahi). Hold ka time theek karna. Bade show ka analytics.

**Pooche: 10 guna kaise?** User ka raasta chalo, pehle jo toote use badhao.

**Pooche: kahan se sab gir sakta?** SQL primary (failover) aur Redis (cluster). Redis gira to har master ki copy
(Cluster). Master gira to copy master ban jaati, khud (Sentinel nahi).

**Pooche: chal raha hai, kaise pata?** Booking p99, 0-row ("ja chuki") rate, Kafka lag, kitne hold khatam, alert.

---

## Pooche to (kisi card me nahi baitha)

API: `GET /movies?city=BLR`, `GET /shows?movieId=X`, `GET /shows/{id}/seats` (available / held / booked).
`POST /bookings { showId, seats }` -> seat HOLD (atomic) -> bookingId + umar. User JWT se lo, body se nahi, warna koi
bhi kisi aur ke naam se book kare. `POST /bookings/{id}/pay` -> success = 'booked', fail = seat chhodo.

Row: seats = seat_id, show_id, status ('available' / 'held' / 'booked'), user_id, held_until.
bookings = booking_id, user_id, show_id, seat_ids, status, created_at.

SQL kyun: sahi hona hi dil hai. ACID + row lock se ek seat do ko bikna namumkin. Seat + booking + payment ka haal kai
row ek saath badalta, to ek transaction. NoSQL ka "thodi der baad sahi" matlab do machine alag-alag "khaali" bol
sakti, khatra. Data chhota, isliye sharding nahi (bina zaroorat shard = faltu bhaari design).

SQL primary hi gir gaya to? ~30-120 sec me failover (Multi-AZ / Aurora). App wahi id se dobara bheje (idempotent).
Aakhri booking copy tak pahunchi thi? Sync copy: likhai dono jagah pakki hoti hai (Multi-AZ). Booking matlab paisa.

"BookMyShow sach me aise karta hai?" Mat bolo "BMS aise karta". Bolo "a common pattern in flash sales", unka andar ka
pata nahi.

- **Pehle poochho:** browse, booking, payment, refund, recommendation, kis pe focus? -> sirf seat booking, asli dikkat wahin
- **Pehle poochho:** badi movie ke din ek saath kitne log ek hi show pe? -> bheed doosra bada dushman, pehla ek seat do ko
- **Pehle poochho:** payment khud ya bahar ka gateway?
- **Pehle poochho:** seat chunne ke baad kitni der rokni hai (hold)?
- **Pehle poochho:** refund scope me?

---

ARCHETYPE C · CONCEPTS: [db-what-when](../../FOUNDATIONS/09_databases_what_when.md) · [CAP](../../FOUNDATIONS/08_cap_theorem.md) · saath: [payment](../06_payment_system/06_payment_system.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
