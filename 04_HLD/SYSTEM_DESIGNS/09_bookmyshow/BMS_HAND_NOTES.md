# BookMyShow: hand notes (mock se pehle ye padho)

### Shuru

**Kaam:** movie / show dekho, seat map dekho, seat chuno -> pay -> ticket pakka. Bahar: user ka maanga refund (cancel karke), review, recommendation. Andar: booking fail hui to kata paisa apne aap wapas.

**Sabse upar:** ek seat kabhi do logon ko na bike (yahi dil). Seat map dekhna tez. Concert ki bheed jhele.

**Numbers:** 100 log map dekhte, ~1-2 book karte -> dekhne ke liye cache, likhne ke liye ek pakka SQL. Data chhota (aaj + agle hafte ke show) -> shard nahi.

### Banking / payment se aaya (naya nahi)

Ek query (check + likho) = banking kadam 4 · idempotency key = banking kadam 3 · PSP + webhook = payment file.

&nbsp;

**1. SERVICE + SQL**

BMS Service + SQL DB, `seats` table: `show 9pm | A1 | FREE`. SQL kyunki seat BOOKED + booking + payment, teeno ek saath ya koi nahi (transaction), aur data chhota hai.

**Booking ka DB:** RDS Multi-AZ, copy SYNC kyunki booking = paisa (sync hamara faisla, failover AWS ka).

Par Ravi ne bhi usi second A1 dabaya. Dono ne padha FREE, dono ne likha BOOKED, ek seat pe do log.

&nbsp;

**2. EK QUERY (UPDATE ... WHERE FREE)**

Padhna aur likhna ek query me: `UPDATE seats SET BOOKED WHERE seat = A1 AND status = FREE`. Row pe lock, ek ke baad ek: Arpan ko "1 row badli" = seat mili, Ravi ko "0 row badli" = seat chali gayi.

**Maan lo 2 seat (A1 + A2), ek mili ek nahi:** dono UPDATE ek transaction me, dono milein ya koi nahi.

Par pay karne me 3 min lagte. Pehle BOOKED kiya aur Arpan bhaag gaya to seat hamesha phasi; pehle pay kiya to beech me Ravi le gaya aur paisa kat gaya.

&nbsp;

**3. HELD 5 MIN (hold_until)**

Beech ka haal **HELD**: "5 min Arpan ki". Pay kiya -> BOOKED (sirf apna aur umar baaki ho). Nahi kiya -> query me hi shart `hold_until < now()`, mara hua hold apne aap gina nahi jaata. Chhota job har minute purane HELD saaf karta, bas map pe jaldi khaali dikhe.

**Maan lo 5:30 min pe pay hua, hold mar chuka, seat Ravi le gaya:** paisa wapas (payment file wala).

Par 9 baje ke show ka map lakhon log dekh rahe, har "dekho" pe SQL se 200 seat, booking dheemi.

&nbsp;

**4. SEAT MAP REDIS CACHE**

`map:9pm` Redis me (umar 5 sec). HOLD hua -> SQL me likho -> Redis key DELETE. Map 2-5 sec purana chalega: map sirf aankh ke liye, **FAISLA SQL** karta ("0 row badli"). Banking ke read replica jaisa: dekhna copy se, lena primary se.

Par 12 baje concert khula: 2 lakh log ek second me, seat 5000, sab ek table ki chand rows pe, SQL lock me phasa.

&nbsp;

**5. WAITING ROOM (line + token)**

Pehle Redis me **line** ("aap 45,000 number pe"), har minute sirf 2,000 ko **TOKEN** (10 min umar). SQL pe sirf 2,000.

**Aur SQL machine kyun nahi:** jhagda chand rows pe hai, ek row ka lock ek hi machine pe. Bheed DB se pehle roko.

**Ek aadmi 50 tab:** token login se juda, ek user ek token. Bot = rate limiter.

**Maan lo Redis aur copy dono gaye:** dil SQL me hai, to dheema, galat nahi. Map -> har service apni memory me 5 sec. Andar wale -> token SIGNED parchi, Redis ke bina chalta. Line wale -> darwaza BAND (fail-closed), warna 2 lakh SQL pe. (Rate limiter me fail-open tha, yahan ulta.)

Par Arpan ne Pay dabaya, network gaya, app ne dobara bheja, paisa do baar kata.

&nbsp;

**6. PAY: KEY + PSP + WEBHOOK**

Key = booking id (B-77), payments me `UNIQUE(booking_id)` -> dobara aaya to wahi purana jawab. Paisa PSP se (INITIATED -> PENDING -> webhook SUCCESS). Webhook pe HELD -> BOOKED: 1 row badli = ticket + SMS · 0 row = hold mara -> paisa wapas.

&nbsp;

### Wrap

seat ka faisla = ek query (WHERE FREE) · pay ka waqt = HELD 5 min · map = Redis cache (faisla SQL) · concert = waiting room + signed token · pay dobara = booking id key · Redis gaya = dheema, galat nahi

**Chal raha kaise pata:** booking p99, "0 row badli" ginti, kitne hold umar se mare, line kitni lambi.


Poora design (har kadam ka diagram aur "kyun") = [09_bookmyshow.md](09_bookmyshow.md)
