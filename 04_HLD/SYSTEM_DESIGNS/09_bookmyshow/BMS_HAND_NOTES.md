# BookMyShow: hand notes (mock se pehle ye padho)

### Shuru

**Kaam:** movie / show dekho, seat map dekho, seat chuno -> pay -> ticket pakka. Concert ki booking bhi.

**Bahar:** user khud cancel karke refund maange, review, recommendation. **Andar:** booking fail hui to kata paisa apne aap wapas.

**Sabse upar:** ek seat kabhi do logon ko na bike (yahi dil). Seat map dekhna tez. Concert ki bheed jhele.

**Numbers:** 100 log map dekhte, ~1-2 book karte -> dekhne ke liye cache, likhne ke liye ek pakka SQL. Data chhota (aaj + agle hafte ke show) -> shard nahi. Concert khulte hi ~2 lakh log ek second me, seat 5000.

### Banking / payment se aaya (naya nahi)

Ek query (check + likho) = banking kadam 4 · idempotency key = banking kadam 3 · PSP + webhook = payment file.

&nbsp;

**1. SERVICE + SQL**

BMS Service + SQL DB. `seats` table me har seat ki row: `show 9pm | A1 | FREE`.

**SQL kyun:** pay hua to seat BOOKED + booking row + payment row, teeno ek saath ya koi nahi (transaction). Data chhota hai, NoSQL ki taakat chahiye hi nahi.

**AWS pe:** RDS Multi-AZ. Copy SYNC rakhi kyunki booking = paisa (sync hamara faisla, failover AWS ka kaam).

**App server kuch yaad nahi rakhta:** sach sirf SQL me (A1 = HELD, Arpan). Isliye app server ki koi "replica" nahi hoti. Koi bhi server gira to LB doosre pe bhej deta, wo SQL dekh ke wahi jawab deta.

Par Ravi ne bhi usi second A1 dabaya. Dono ne padha FREE, dono ne likha BOOKED, ek seat pe do log.

&nbsp;

**2. EK QUERY (UPDATE ... WHERE FREE)**

Padhna aur likhna ek hi query me:

`UPDATE seats SET status = BOOKED WHERE seat = A1 AND status = FREE`

DB row pe lock lagata hai, to dono query ek ke baad ek chalti hain. Arpan ki pehle: "1 row badli" = seat mili. Ravi ki baad me: "0 row badli" = "seat chali gayi".

**Maan lo 2 seat (A1 + A2), ek mili ek nahi:** dono UPDATE ek transaction me. Dono milein ya koi nahi.

Par pay karne me 3 min lagte hain. Pehle BOOKED kiya aur Arpan bhaag gaya, to seat hamesha phasi. Pehle pay kiya, to beech me Ravi le gaya aur Arpan ka paisa kat gaya.

&nbsp;

**3. HELD 5 MIN (hold_until)**

Beech ka haal **HELD** = "5 min Arpan ki". Pay kiya -> BOOKED (sirf apna, aur umar baaki ho). Nahi kiya -> seat apne aap kisi aur ki ho sakti hai.

**Apne aap kaise:** shart query me hi hai. HOLD wali query `status = FREE` **ya** `HELD aur hold_until < now()` dono pe chalti hai. Mara hua hold gina hi nahi jaata, kisi job ka intezaar nahi.

**Job kis liye:** ek chhota job har minute purane HELD saaf karta hai, bas taaki seat map pe A1 jaldi khaali dikhe. **Maan lo job band pada hai:** seat phir bhi milti hai, kyunki shart query me hai.

**Maan lo 5:30 min pe pay hua, hold mar chuka, seat Ravi le gaya:** paisa wapas (payment file wala raasta).

Par 9 baje ke show ka map lakhon log dekh rahe hain. Har "map dekho" pe SQL se 200 seat, aur Arpan ki booking dheemi.

&nbsp;

**4. SEAT MAP REDIS CACHE**

Har "map dekho" pe SQL tak jaana band. Redis me har show ka map ek key me: `map:9pm` (umar 5 sec).

**Badla kab:** Arpan ne A1 HOLD ki -> SQL me likha -> Redis ki `map:9pm` DELETE -> agla dekhne wala naya map.

**Purana map chalega:** map sirf aankh ke liye hai, FAISLA SQL karta hai. SQL baap, Redis bachcha.

**Maan lo Ravi ko purane map me A1 FREE dikhi, jabki Arpan HOLD kar chuka:** Ravi ne dabaya -> kadam 3 wali query SQL pe -> "0 row badli" -> "seat chali gayi". Kuch galat nahi hua.

**Banking jaisa:** wahan balance dekhna read replica se, paisa kaatna primary se. Yahan map dekhna Redis se, seat lena SQL se.

**AWS pe:** Redis hum khud nahi chalate, AWS ka ElastiCache. Machine, copy, gira to copy aage karna, ye AWS ka kaam.

Par 12 baje concert khula. 2 lakh log ek second me, seat 5000. Saari HOLD query ek table ki chand rows pe toot padti hain, SQL lock ki line me phas gaya.

&nbsp;

**5. WAITING ROOM (line + token)**

2 lakh log seedhe booking pe nahi jaate. Pehle Redis me ek **line**: "aap 45,000 number pe ho, ~22 min".

**Token:** har minute sirf 2,000 logon ko andar aane ki parchi (TOKEN, 10 min umar). Token wale hi seat HOLD kar sakte hain. To SQL pe ek waqt me sirf 2,000, 2 lakh nahi. Jo pehle aaya wo pehle andar.

**Aur SQL machine kyun nahi:** jhagda 5000 seat ki chand rows pe hai, aur ek row ka lock ek hi machine pe hota hai. Machine 10 karo, A1 ki row phir bhi ek. Bheed ko DB se pehle hi roko.

**Maan lo ek aadmi 50 tab khol ke baitha:** token login se juda hai, ek user ek token. Bot ki bheed = rate limiter.

**Maan lo pay ke beech token ki umar (10 min) khatam:** seat pe koi asar nahi. Do alag ghadiyan hain: TOKEN = darwaze ki parchi (andar aao, HOLD karo), HOLD = seat ki ghadi (SQL me, 5 min). HOLD ho gaya to seat ka faisla sirf hold ki ghadi se. Hold 5 min ka, token 10 ka, to hold hamesha pehle khatam. Token khatam = bas nayi seat HOLD nahi kar sakta, uske liye dobara line.

**Maan lo Redis aur uski copy dono gaye (AWS bhi nahi bacha paaya):** dil SQL me hai, to system dheema ho sakta, galat nahi.
- **Map:** har BMS Service apni memory me aakhri map 5 sec rakhe.
- **Andar wale:** unka token SIGNED parchi hai, BMS Service signature khud check karta, Redis nahi chahiye. Unki booking poori.
- **Line wale:** line ka kram kho gaya. Darwaza BAND (fail-closed): "booking thodi der me". Khula chhoda to 2 lakh seedhe SQL pe. (Rate limiter me fail-open tha, yahan ulta, kyunki yahan SQL bachana zaroori.)

**Maan lo pooche "BookMyShow sach me aise karta?":** "a common pattern for flash sales", unka andar ka pata nahi.

Par Arpan andar gaya, A1 HOLD ki, Pay dabaya. Network gaya, app ne apne aap dobara bheja, paisa do baar kata.

&nbsp;

**6. PAY: KEY + PSP + WEBHOOK**

**Key:** app har Pay ke saath key bhejta hai, yahan key = booking id (B-77). Payments table me `UNIQUE(booking_id)`. Dobara aaya to row pehle se hai -> "ho chuka", wahi purana jawab, paisa dobara nahi.

**Paisa:** BMS khud bank se baat nahi karta. PSP (Razorpay jaisa): INITIATED -> PENDING -> PSP ka webhook "SUCCESS".

**Webhook aaya, tab seat pakki:** HELD -> BOOKED wali query (sirf agar umar baaki). 1 row badli = ticket pakka + SMS (notification). 0 row badli = hold mar chuka, seat gayi -> paisa wapas.

**Maan lo server dheema, Arpan ko pata nahi seat bani ya nahi:** app wahi booking id ke saath dobara poochhe / bheje (ruk-ruk ke, backoff + jitter). LB dheeme server ko hata ke kisi aur pe bheje. Wo SQL dekh ke batata: A1 HELD / BOOKED Arpan ki. Dobara bheja to bhi dohra nahi (key + WHERE FREE).

**API:** `GET /shows/{id}/seats` · `POST /bookings { showId, seats }` -> HOLD -> bookingId + umar · `POST /bookings/{id}/pay`. User JWT se lo, body se nahi.

**Tables:** seats = show_id, seat_id, status, user_id, hold_until · bookings = booking_id, user_id, show_id, seats, status · payments = booking_id (UNIQUE), status.

&nbsp;

### Wrap

seat ka faisla = ek query (WHERE FREE) · pay ka waqt = HELD 5 min (shart query me) · map = Redis cache (faisla SQL) · concert = waiting room + signed token · token ghadi != hold ghadi · pay dobara = booking id key · Redis gaya = dheema, galat nahi · app server kuch yaad nahi rakhta

**Chal raha kaise pata:** booking p99, "0 row badli" ki ginti, kitne hold umar se mare, line kitni lambi.


Poora design (har kadam ka diagram aur "kyun") = [09_bookmyshow.md](09_bookmyshow.md)
