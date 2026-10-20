# BookMyShow (Ticket Booking)

> Movie / show dekho, seat chuno, pay karo, ticket. JP ke liye kaam ka: paisa aur seat ek saath sahi ("paisa do baar kharch na ho" jaisa).
> Dil do cheezein: ek seat do logon ko na bike (ek saath booking), aur badi movie ke din ki bheed.
>
> Mock me sirf heading + Cue + GIF. Neeche har card: **hal** · **kyun** · **nahi** (doosra kyun nahi) · **Pooche**.

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c7_2-64f79499.svg)

---


## Shuru

- **FR:** movies / shows dekho · seat map · seat book (payment) · ticket pakka · refund, recommendation, review scope se bahar
- **NFR:** ek seat do ko na bike (yahi dil) · dekhna tez · badi movie ki bheed jhele · bharosemand
- **numbers:** 1 crore user, par do alag bojh, ek number mat bolo. Dekhna (shows / seat map) sab karte, lakhon padhai -> cache + read replica · booking kam log, par ek hi seat pe ek saath, jhatke me -> queue · active data chhota (aaj + agle hafte ke show), purane archive -> sharding nahi

---

*ab tootega kahan? sabse pehle wahin jahan dil hai, ek seat pe do log*

## 1. Maine aur Ravi ne ek hi second me A1 book ki, dono ko mil gayi

**Cue:** ATOMIC UPDATE ... WHERE available · dabba: SQL DB

- **pehle (sabse simple):** seats table (seat_id, show_id, status). "Book" dabao, status = 'booked'. Pehle padha (khaali), phir likha -> 10:00:01 pe dono ne A1 khaali dekhi, dono ne book ki.
- **hal:** dekhna aur book karna ek hi query me (atomic UPDATE):

`UPDATE seats SET status = 'booked' WHERE show_id = 42 AND seat_id = 'A1' AND status = 'available'`.

- **jeeta / haara:** jeeta = 1 row, A1 meri. Haara = 0 row, "seat ja chuki, doosri chuno".
- **kyun:** faisla DB ki ek query, WHERE khaali. DB row lock ek-ek ko chalaata: main pehle, Ravi ruka, phir uska WHERE 0 row.
- **kyun:** lock App me nahi, DB me
- **nahi:** `SELECT ... FOR UPDATE` -> chalta bhi, par do baar DB + lock lamba. Ek statement = lock sabse chhota.
- **nahi:** version column (optimistic) -> haarne wala dobara try karta. Conditional UPDATE me haara seedha "seat gayi".
- **Pooche:** consistency ya availability (network toota, aakhri seat pe do log)? -> ek hi system me dono
- **Pooche:** booking kaunsa? -> CP: sahi hona pehle. Network toota, seat khaali pakka nahi pata = mana. Ek seat do ko bechna usse bura.
- **Pooche:** search kaunsa? -> AP: movie list / seat ginti 2 sec purani bhi chalegi, wo chalta rahe
- **Pooche:** Ravi ne phir dabaya? -> WHERE phir 0 row. Dobara dabane se kuch nahi milta.
- **Pooche:** seat rows pehle se hoti? -> haan. Show banate hi saari seat rows, booking = UPDATE hi.
- **Pooche:** 4 seat li, A4 gayi? -> ek transaction. Badli rows 4 nahi to ROLLBACK. Aadhi booking bekaar.
- **Pooche:** 4 seat, do log ulta kram me? -> DB deadlock pakad ke ek ko abort, wo retry kare. Alag UPDATE ho to seat_id ke kram me bhejo.

---

*ab seat ek ko mili, par payment me waqt lagta*

## 2. Maine A1 chuni, payment me 3 minute lag rahe

**Cue:** HOLD + held_until (umar) · dabba: Sweeper job

- **hal:** seat HELD + umar: usi row me `held_until = ab + 5 min`. Pay ho gaya to BOOKED. Umar khatam = khaali maano (hold).
- **kyun:** hold DB ki usi row ka haal (held + umar). Mujhe 5 min seat pakki, baaki ko 'held' dikhti.
- **kyun:** umar khatam ya app gira to sweeper khud chhodta. Sweeper sirf safai, asli faisla UPDATE ke WHERE me.
- **nahi:** seat available rakho -> Ravi le jaata, chhin gayi
- **nahi:** seedha booked kar do -> payment fail to seat hamesha band
- **Pooche:** umar khatam, phir pay aaya? -> confirm pe bhi WHERE (held + main + umar). 0 row = seat gayi, refund.
- **Pooche:** koi 10 seat pakad ke baith gaya? -> had lagao (hold ki ginti + rate limit). Hold = sabke liye band.

---

*ab seat pakad li, par pay ka network gaya*

## 3. Pay dabaya, network gaya, app ne dobara bheja

**Cue:** IDEMPOTENCY KEY (bookingId) · dabba: Payment Svc

- **hal:** har booking ki ek id, pay hamesha usi id se (idempotency key). Wahi B-42 dobara = "ho chuka", purana jawab, paisa ek baar.
- **kyun:** ek booking = ek payment. User ko kuch dobara nahi karna, beech ka haal PENDING dikhta.
- **kyun:** paisa kata par seat nahi mili = refund
- **nahi:** har request naya payment -> pehle yahi tha: paisa kata, network gaya, dobara bheja, 500 do baar kata
- **Pooche:** user ne Pay double tap kiya? -> wahi bookingId, paisa ek baar
- **Pooche:** paisa kata, seat fail? -> refund + user ko batao. Payment aur seat saga jaisa: ek fail to doosra ulta.
- **Pooche:** PSP ka webhook der se? -> PAYMENT_PENDING rakho, hold thoda badhao, roz milao (reconciliation)

---

*ab booking sahi, par dekhne wale bahut*

## 4. 11 baje main seat map dekh raha tha, saath me 1 lakh log, book koi-koi

**Cue:** CACHE + READ REPLICA · dabba: Redis + Read replica

- **hal:** map Redis se: `seatmap:show42`, ~5 sec umar, DB tak nahi. Book dabaya to faisla sirf main DB ke UPDATE pe.
- **kyun:** dekhna copy se (Redis / replica), faisla sirf main DB ka UPDATE. Map thoda purana chalta, galat dikhi seat bhi DB pakad leta (0 row).
- **nahi:** map bhi main DB se -> pehle yahi tha, 1 lakh dekhne wale booking ko bhi dheema kar dete
- **Pooche:** Redis me nahi mila? -> replica se banao, Redis me rakho
- **Pooche:** kisi ne book kiya to map? -> booking main DB pe, phir map ki key DEL
- **Pooche:** A1 bik chuki, map me khaali? -> chalta. Faisla UPDATE ka, wo 0 row bolega. Turant chahiye to WebSocket / SSE.
- **Pooche:** lakh log ek hi key pe? -> kai copy + App me 1-2 sec ki copy. Ek key = ek machine (hot key).

---

*ab dekhna sambhal liya, par badi release pe likhne wale bhi lakhon*

## 5. 12 baje Arijit ka concert khula, 2 lakh log ek show pe

**Cue:** WAITING ROOM + Kafka (sirf badi sale) Kafka + Booking worker

- **hal:** darwaze pe ginti: ~3000 andar, baaki line me (waiting room). Andar walon ki booking Kafka me, ek show ek line (key = show_id). Booking worker ek-ek UPDATE.
- **kab:** sirf badi sale pe. Aam din App seedha SQL primary pe UPDATE, turant jawab. Badi sale pe user ko "booking ho rahi hai".
- **kyun:** likhne ki bheed ko line me khada karo. User ko apni jagah dikhti. Ek show = ek line, sach hamesha DB ki seats.
- **nahi:** sab seedha DB pe -> 3000 seat, 2 lakh log ek hi rows ke lock pe ruke, sab dheema
- **Pooche:** UPDATE jeeta, user ko kaise pata? -> bookingId se poll ya WebSocket push. Line me tha, jawab baad me.
- **Pooche:** seat haar gaya? -> "seat gayi, doosri lo". Faisla DB ka.
- **Pooche:** andar wala bina liye chala gaya? -> line se agla andar. Line sirf darwaza hai.
- **Pooche:** ek worker dheema nahi? -> ~ms ka UPDATE, hazaar / sec. Aur chahiye to show ke hisse (balcony / stall).
- **Pooche:** line ki ginti Redis me thi, Redis restart? -> ginti sirf darwaza. Sach DB ki seats, DB se dobara gino.
- **Pooche:** release ka time to pata hai? -> haan, pehle se machine badhao + har user ki had (rate limit). Waqt pata = tayyari pehle.

---

*ab line bani, par cache hi khaali ho gaya*

## 6. 2 baje Redis restart, saara seat map ek saath replica pe

**Cue:** SET NX lock, pehla hi banaye · dabba: Redis (SET NX + copy)

- **hal:** sirf ek request DB se banaye, baaki ruk ke Redis se (lock). Pehli lock leti (`SET NX EX 5`), replica se ek query, map Redis me. Baaki 50-100 ms ruk ke Redis se.
- **kyun:** khaali cache pe ek hi DB jaata, baaki rukte. Jo gir sakta uski copy pehle se taiyaar.
- **kyun:** user ko bas thoda dheema, galat seat kabhi nahi, faisla DB me
- **nahi:** sabko DB jaane do -> hazaaron miss ek saath replica pe, replica baith gayi, map nahi khulta
- **Pooche:** lock wala hi mar gaya? -> EX 5 = lock khud hat jaata. NX = koi aur pehle se na laga ho.
- **Pooche:** 100 ms baad bhi khaali? -> 2-3 retry, phir khud replica se. User ko bas dheema.
- **Pooche:** SQL primary hi gira? -> ~30-120 sec failover (Multi-AZ / Aurora). App wahi id se dobara bheje (idempotent).
- **Pooche:** aakhri booking copy tak pahunchi thi? -> sync copy: likhai dono jagah pakki (Multi-AZ). Booking = paisa.

---

*ab data surakshit, par App machine hi giri*

## 7. Shaam ko main seat chun raha tha, App machine gir gayi

**Cue:** STATELESS App + LB health check · dabba: LB + App x N

- **hal:** App kuch yaad nahi rakhta (stateless), aage LB zinda machine ko deta. LB kisi bhi machine pe bheje, hold DB me.
- **kyun:** sab DB me. Machine giri to agli machine wahi hold dekhti, kyunki hold DB me tha, memory me nahi. Seat wahin.
- **nahi:** ek hi App machine -> wo giri to booking band
- **Pooche:** giri machine LB se bahar kaise? -> LB har kuch sec poochta (GET /health). 2-3 baar fail = bahar. Ek fail = jhatka.
- **Pooche:** payment beech me tha? -> same bookingId se dobara, doosri machine pe. Idempotent, do baar nahi katega.

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

Bolne ke liye: seat map Redis aur copy se, booking main DB pe ek query se ("khaali ho tabhi book"), to ek seat kabhi
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

"BookMyShow sach me aise karta hai?" Mat bolo "BMS aise karta". Bolo "a common pattern in flash sales", unka andar ka
pata nahi.

- **Pehle poochho:** browse, booking, payment, refund, recommendation, kis pe focus? -> main seat booking, asli dikkat wahin
- **Pehle poochho:** badi movie ke din ek saath kitne log ek hi show pe? -> bheed doosra bada dushman, pehla ek seat do ko
- **Pehle poochho:** payment khud ya bahar ka gateway?
- **Pehle poochho:** seat chunne ke baad kitni der rokni hai (hold)?
- **Pehle poochho:** refund scope me?

---

ARCHETYPE C · CONCEPTS: [db-what-when](../../FOUNDATIONS/09_databases_what_when.md) · [CAP](../../FOUNDATIONS/08_cap_theorem.md) · saath: [payment](../06_payment_system/06_payment_system.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
