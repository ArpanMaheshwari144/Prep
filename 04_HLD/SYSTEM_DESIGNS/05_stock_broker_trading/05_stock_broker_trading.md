# Stock Broker / Trading Platform

> User BUY / SELL order deta hai, system order match karta hai, paisa aur share sahi jagah pahunchte hain, sabko live price dikhta hai.
> Dil do cheezein: paisa kabhi galat nahi (strong consistency), aur matching bahut tez (RAM me, microseconds). Finance interview me (JP / GS) yahi poochte: consistency, ACID, idempotency, ledger, audit.
>
> Mock me sirf heading + Cue + poster. Poster me sab: upar "Pehle aisa tha", tasveer (ilaaj), hara "Kyun chalta", laal number = kya toot sakta + kaise bacha, peela = doosra tareeka kyun nahi, grey = baaki.
> Cross-Q: hal ke har bade shabd pe socho -> gira? · dheema / bheed? · do ek saath? · galat hua? · kaise / kitna?

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c8_2-41340ea6.svg)

---


## Shuru

```
FR      scope pehle poochho (matching, price feed, risk, settlement). Hum karenge: buy / sell (LIMIT + MARKET,
        aadha bhara / partial fill bhi), order match, paisa + share move (settlement), live price,
        cancel / status. Scope ke bahar: risk / margin / IPO / mutual fund
NFR     paisa hamesha sahi (strong, "thodi der baad sahi" nahi). Tez. Imaandar (jo pehle aaya wo pehle).
        Har cheez ka record (audit)
NUMBERS ~50 lakh user, ~50 lakh order roz, market ~6 ghante (~20,000 sec) -> ~250 order / sec.
        9:15 pe jhatka, 10,000+ / sec. Price: ~10 lakh logon ka app khula, har second naya price
        -> ~10 lakh / sec. Isliye matching RAM me, aur price PUSH (har user ka baar-baar poochhna nahi)
```

> [!IMPORTANT]
> **RAIL: har design isi kram me chalta (8 dabbe, sab file me same)**
> kaam chala → bheed aayi → machine giri → do log ek saath → kuch dheema → data bada → door ke log → bahar ka hamla
>
> | rail | is design me | card |
> |---|---|---|
> | 1 kaam chala | buy / sell order, book me match, paisa + share move, live price | Shuru |
> | 2 bheed aayi | 9:15 sab TCS pe · 10 lakh log price poochh rahe | 4, 8 |
> | 3 machine giri | matching server gira, book gayab · settlement beech me gira · Portfolio DB band | 5, 6, 7 |
> | 4 do log ek saath | ek tap do order · 50k me 30k + 30k · 10 share, do khareedar (**is design ka khaas: ek stock ek thread**) | 1, 2, 3 |
> | 5 kuch dheema | alag nahi: har order pe DB lock = dheema, isliye matching RAM me | 4 |
> | 6 data bada | nahi aata: ~250 order / sec, chhota | - |
> | 7 door ke log | nahi aata | - |
> | 8 bahar ka hamla | is file me nahi: rate limit = 02_rate_limiter | - |


> [!NOTE]
> **AWS pe kaunsa DB (interview me naam bolna ho to)** · SQL wala = RDS, Cassandra RDS pe nahi chalta
>
> - Paisa / order (SQL) → **AWS RDS (Postgres / MySQL)**: ACID: paisa aur order kabhi aadha nahi
> - Order book → RAM me: DB nahi, log se wapas banti

---

*ab tootega kahan? sabse pehle darwaze pe, jab ek tap do baar pahunche*

## 1. Maine ek baar dabaya, order do lag gaye

**Rail 4 DO LOG EK SAATH (retry)** · Cue: IDEMPOTENCY KEY (wahi id dobara)

<img src="img/c1_poster.png?v=3" width="1000" alt="card 1: kya toota, kaise bacha">


---

*ab order ek hai, par paisa do order me baant diya to?*

## 2. Wallet me 50k the, phir bhi 30k-30k ke dono order lag gaye

**Rail 4 DO LOG EK SAATH (wallet)** · Cue: check + block EK atomic query

<img src="img/c2_poster.png?v=3" width="1000" alt="card 2: kya toota, kaise bacha">


---

*ab paisa theek, par ek hi share do khareedaron ko?*

## 3. Suresh ke paas 10 the, mujhe aur Mohan dono ko 10 mil gaye

**Rail 4 DO LOG EK SAATH (khaas)** · Cue: ek symbol ek thread, book RAM me

<img src="img/c3_poster.png?v=3" width="1000" alt="card 3: kya toota, kaise bacha">


---

*ab ek thread hai, par 9:15 pe bheed aayi to?*

## 4. 9:15 baje sab TCS pe toot pade

**Rail 2 BHEED AAYI** · Cue: baanto mat, ek thread kaafi, line

<img src="img/c4_poster.png?v=3" width="1000" alt="card 4: kya toota, kaise bacha">


---

*ab sab RAM me hai, server gira to?*

## 5. Server gira, poori order book gayab

**Rail 3 MACHINE GIRI (book)** · Cue: pehle log, phir book; gira = replay

<img src="img/c5_poster.png?v=3" width="1000" alt="card 5: kya toota, kaise bacha">


---

*ab sauda hua, par paisa aadha pahuncha to?*

## 6. Mera paisa kat gaya, Suresh ko mila hi nahi

**Rail 3 MACHINE GIRI (settlement)** · Cue: ek transaction: chaaron ya koi nahi

<img src="img/c6_poster.png?v=3" width="1000" alt="card 6: kya toota, kaise bacha">


---

*ab paisa ek DB me, share doosre me, ek transaction nahi ban sakti to?*

## 7. Dopahar: paisa Wallet DB me, share Portfolio DB me — INFY ka 20k kata, share nahi aaye

**Rail 3 MACHINE GIRI (do DB)** · Cue: har step ka haal, aage badho

<img src="img/c7_poster.png?v=3" width="1000" alt="card 7: kya toota, kaise bacha">


---

*ab andar sab theek, par bahar 10 lakh log price maang rahe*

## 8. 10 lakh log ek saath price poochh rahe, server baith gaya

**Rail 2 BHEED AAYI (price)** · Cue: push (TV jaisa), WebSocket

<img src="img/c8_poster.png?v=3" width="1000" alt="card 8: kya toota, kaise bacha">


---

## Poora design ek saans me

```
User -> Order Svc (check + idempotency key) -> Wallet (paisa BLOCK)
-> Event Log / Kafka (number lagta, key = stock) -> Matching engine (ek stock ek thread, book RAM me)
-> Settlement (ek transaction, ya alag DB ho to step-step, aage hi) -> Wallet DB + Portfolio DB
price: Matching -> Pub/Sub -> WebSocket -> users
DB: paisa / order = SQL + ACID · order book = RAM · ledger = append-only (sirf jodo)
    portfolio / history = read replica
```

Bolne ke liye: order service check karti aur dobara order rokti (id se). Wallet paisa block karta. Order number ke
saath log me jaata, phir har stock ka ek-thread matching engine RAM wali book me match karta. Sauda ek transaction,
alag DB ho to step-step, match ke baad sirf aage. Price WebSocket se. Log se crash ke baad book wapas, aur hisaab ka
record bhi. Aage: stop-loss, risk check, regulator ki report.

---

## 10 guna traffic

- **Order Service**: stateless, kuch yaad nahi rakhti. Machine badhao + load balancer.
- **Portfolio / history**: read replica (padhne ke liye DB ki copy).
- **Matching**: book RAM me, yaad rakhta. Do chalti copy nahi (do book = ek share do baar bike). Standby log padhta
  khada rehta, sirf pehla gire tab chalta. Stock ke hisaab se baanto: TCS ek engine pe, INFY doosre pe.
- **Kafka**: partition pehle se zyada rakho (jaise 64). Beech me badhaya to key ki jagah badalti, ek stock ke order do
  partition me, kram tootta.
- **Price**: WebSocket server badhao.

**Pooche: 10 guna users ho gaye to kaise badhaoge?** Jo service kuch yaad nahi rakhti (order service), uski machine
badhao + load balancer. Padhne wale data (portfolio, history) ki copy. Matching engine book yaad rakhta, do chalti
copy nahi bana sakte (do book = gadbad), standby bas khada rehta. Engine ko stock ke hisaab se baanto: TCS ek machine,
INFY doosri.

---

## Pooche to (kisi card me nahi baitha)

LIMIT = "3000 ya isse behtar, warna rukunga", daam pakka. MARKET = "jo bhav hai, abhi do", waqt pakka. Sahi daam
chahiye, jaldi nahi, to LIMIT. Turant ghusna / nikalna, to MARKET.

API: POST /order {stock, side, qty, price, type, idempotencyKey} · DELETE /order/{id} · GET /order/{id} ·
WS /prices?symbol=

Book me BUY 2998 aaya, top SELL 3000 hai? Sauda nahi, wo book me khada rehta (khareedne wale mehnga upar).

Order book DB me kyun nahi? DB = har order pe milliseconds, RAM = microseconds. Isliye book RAM me.

---

ARCHETYPE C (transactional) · CONCEPTS: [db-what-when](../../FOUNDATIONS/09_databases_what_when.md) · [CAP](../../FOUNDATIONS/08_cap_theorem.md) · saath: [payment](../06_payment_system/06_payment_system.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
