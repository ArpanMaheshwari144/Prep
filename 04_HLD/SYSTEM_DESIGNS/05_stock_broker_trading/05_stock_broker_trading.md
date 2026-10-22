# Stock Broker / Trading Platform

> User BUY / SELL order deta hai, system order match karta hai, paisa aur share sahi jagah pahunchte hain, sabko live price dikhta hai.
> Dil do cheezein: paisa kabhi galat nahi (strong consistency), aur matching bahut tez (RAM me, microseconds). Finance interview me (JP / GS) yahi poochte: consistency, ACID, idempotency, ledger, audit.
>
> Mock me sirf heading + Cue + kahani ki tasveer. Neeche har card: HAL · KYUN · NAHI, phir YAAD RAKH dabba + CROSS-Q dabba (sawaal → cue, poora jawab).
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

<img src="img/c1_poster.png?v=1" width="1000" alt="card 1: kya toota, kaise bacha">


```
HAL     screen khulte hi app ek id banata hai. Network gaya aur app ne dobara bheja, to wahi id jaati hai.
        Server id pe UNIQUE: wahi id aayi to purana jawab, order ek hi (idempotency key)
KYUN    ek id = ek kaam. Id app banata, server UNIQUE se pakadta. User ko ek hi order dikhe,
        app kitni baar bhi bheje
NAHI    button pehli tap pe band -> ADHOORA: kaafi nahi, retry app khud karta hai, button dabta hi nahi
```

> [!IMPORTANT]
> **Yaad rakh:** Screen khulte hi app ek id banata, retry pe wahi id. Server UNIQUE se pakadta: ek id = ek order.


---

*ab order ek hai, par paisa do order me baant diya to?*

## 2. Wallet me 50k the, phir bhi 30k-30k ke dono order lag gaye

**Rail 4 DO LOG EK SAATH (wallet)** · Cue: check + block EK atomic query

<img src="img/c2_poster.png?v=1" width="1000" alt="card 2: kya toota, kaise bacha">


```
HAL     check aur block ek hi query me (atomic UPDATE):
        SET blocked = blocked + 30k WHERE total - blocked >= 30k
        Ek jeeta = 1 row, block ho gaya. Doosre ko 20k bacha dikha, 0 row, turant reject
KYUN    "hai?" aur "block" ek hi UPDATE me, DB ke andar. Kitni bhi machine ho, DB ek-ek karke chalata hai.
        Wallet kabhi minus nahi hota
BLOCKED blocked badhta hai naye order se, ghatta hai sauda / cancel / umar khatam hone se
NAHI    pehle padho, phir block -> RACE: laptop (TCS 30k) + phone (INFY 30k), dono server ne 50k dekha,
           dono pass, wallet minus
        Java synchronized -> CHHOTA: taala ek machine ka hai, phone doosre server pe gaya
```

> [!IMPORTANT]
> **Yaad rakh:** Check + block ek hi atomic UPDATE: blocked + 30k WHERE total - blocked >= 30k. 1 row = jeeta.


---

*ab paisa theek, par ek hi share do khareedaron ko?*

## 3. Suresh ke paas 10 the, mujhe aur Mohan dono ko 10 mil gaye

**Rail 4 DO LOG EK SAATH (khaas)** · Cue: ek symbol ek thread, book RAM me

<img src="img/c3_poster.png?v=1" width="1000" alt="card 3: kya toota, kaise bacha">


```
HAL     ek share ke saare order ek line me, us line ko ek hi thread (ek symbol ek thread).
        Main pehle aaya to 10 mujhe mile, Mohan ko 0, wo line me
KYUN    ek stock ki book ko ek waqt ek hi haath. Doosra line me rukta, race hoti hi nahi.
        Thread atka (GC pause) to sirf us stock ki line rukti, INFY apne thread pe, standby taiyaar
RAM/SQL jo tez aur thodi der ka hai (order book), wo RAM me. Jo paisa aur hamesha ka hai (wallet / ledger),
        wo SQL DB + ACID me
BOOK    order book ki do taraf: bechne wale sasta upar (3000, 3001, 3005), khareedne wale mehnga upar
        (2999, 2998)
SAUDA   sauda tabhi jab top khareedar >= top bechne wala. Daam book wale ka: naya BUY 3001,
        top SELL 3000, to sauda 3000 pe. Same daam pe jo pehle aaya wo pehle bikta
BROKER  is design me hum broker hain, par apna matching (exchange jaisa). Asli India me Groww / Zerodha
        order NSE ko bhejte, matching NSE karta; wallet block broker ka kaam
NAHI    do thread bina line -> RACE: dono ko "10 hai" dikha, dono ne le liye, 20 bik gaye
        har order pe lock -> GALAT: race share pe hai, order pe nahi. Dono apna lock lete, dono ko 10 dikhe
        DB me hi match (sabse simple: Order service DB me likhe, wahin SELL dhoondh ke match, DB ka ACID)
           -> DHEEMA: DB har order pe dheema, matching RAM me
```

> [!IMPORTANT]
> **Yaad rakh:** Ek stock ke saare order ek line, us line ka ek hi thread. Book RAM me, paisa SQL + ACID me.


---

*ab ek thread hai, par 9:15 pe bheed aayi to?*

## 4. 9:15 baje sab TCS pe toot pade

**Rail 2 BHEED AAYI** · Cue: baanto mat, ek thread kaafi, line

<img src="img/c4_poster.png?v=1" width="1000" alt="card 4: kya toota, kaise bacha">


```
HAL     hazaaron order / sec aaye, to bhi TCS ki ek line, ek thread RAM me tez nipta deta.
        Line me lagte hi user ko ACCEPTED turant, FILLED tab jab sauda ho
KYUN    RAM me ek thread lakhon order / sec nipta deta. Line lambi = der, par order khota nahi, line me hai
NAHI    har order pe lock + DB -> DHEEMA: ek-ek karke dheema, jam
        TCS ko do thread me baanto -> TOOTA: ek stock = ek line. Baantna stocks ke BEECH,
           ek stock ke andar kabhi nahi
```

> [!IMPORTANT]
> **Yaad rakh:** TCS ki ek line, ek thread RAM me lakhon / sec. Line me lagte hi ACCEPTED, sauda pe FILLED. Baanto stocks ke beech.


---

*ab sab RAM me hai, server gira to?*

## 5. Server gira, poori order book gayab

**Rail 3 MACHINE GIRI (book)** · Cue: pehle log, phir book; gira = replay

<img src="img/c5_poster.png?v=1" width="1000" alt="card 5: kya toota, kaise bacha">


```
HAL     har order pehle log me (disk), phir book me. Log me har order ko number (#501).
        10:15 pe gira, 4,000 order gayab, to log dobara chalao, wahi 4,000 wapas (replay)
KYUN    log hi sach hai, book uski photo. Gira = aakhri photo + uske baad ke panne.
        Dobara chalaya to kuch nahi badalta (trade_id)
NAHI    sirf book ki photo -> ADHOORA: photo ke BAAD wale order kahin likhe hi nahi
        time se kram -> GALAT: do machine ka time (clock) alag, aur ek second me do order aa jaate
```

> [!IMPORTANT]
> **Yaad rakh:** Har order pehle log (disk, number ke saath), phir book. Gira = log replay. Standby log padh ke taiyaar.


---

*ab sauda hua, par paisa aadha pahuncha to?*

## 6. Mera paisa kat gaya, Suresh ko mila hi nahi

**Rail 3 MACHINE GIRI (settlement)** · Cue: ek transaction: chaaron ya koi nahi

<img src="img/c6_poster.png?v=1" width="1000" alt="card 6: kya toota, kaise bacha">


```
HAL     chaaron badlaav ek transaction me: Arpan -30k +10 share, Suresh +30k -10 share, phir COMMIT.
        Ya sab, ya koi nahi
KYUN    paisa ek DB me hai, to chaaron likhai ek transaction. COMMIT se pehle crash = rollback, kuch nahi
        likha, user ko PENDING, galat number kabhi nahi. Ledger ka jod hamesha 0
NAHI    aakhir me "done" flag -> ANDHA: flag se pehle gira to pata hi nahi kitna likha tha
```

> [!IMPORTANT]
> **Yaad rakh:** Chaaron badlaav ek transaction: ya sab, ya koi nahi. trade_id UNIQUE, ledger ka jod hamesha 0.


---

*ab paisa ek DB me, share doosre me, ek transaction nahi ban sakti to?*

## 7. Dopahar: paisa Wallet DB me, share Portfolio DB me — INFY ka 20k kata, share nahi aaye

**Rail 3 MACHINE GIRI (do DB)** · Cue: har step ka haal, aage badho

<img src="img/c7_poster.png?v=1" width="1000" alt="card 7: kya toota, kaise bacha">


```
HAL     do DB = ek transaction nahi ban sakti. Har kadam ka haal likho (status log: wallet done,
        portfolio done). Fail hua to likhe haal se retry, aage hi, jab tak portfolio done (SETTLED)
KYUN    match ke baad sauda PAKKA hai. System hi aage badhaata (retry), user ko kuch dobara nahi karna.
        Beech ka haal user ko PENDING dikhta
NAHI    paisa ulta kar do -> GALAT: match ke baad sauda pakka, bechne wale ko 20k mil chuke
```

> [!IMPORTANT]
> **Yaad rakh:** Do DB = ek txn nahi. Har kadam ka haal likho, fail to wahi se retry, aage hi. User ko PENDING.


---

*ab andar sab theek, par bahar 10 lakh log price maang rahe*

## 8. 10 lakh log ek saath price poochh rahe, server baith gaya

**Rail 2 BHEED AAYI (price)** · Cue: push (TV jaisa), WebSocket

<img src="img/c8_poster.png?v=1" width="1000" alt="card 8: kya toota, kaise bacha">


```
HAL     connection khula rakho, price badla tabhi server khud bheje (WebSocket push).
        Matching -> pub/sub -> har WebSocket server -> users
KYUN    price = sirf abhi ka. Badla tab server khud bheje, purana store nahi. Users kuch nahi poochte
NAHI    har user baar-baar poochhe (polling) -> BAITHA: 10 lakh har second = server baitha.
           Poochhne ki line hi nahi chahiye
```

> [!IMPORTANT]
> **Yaad rakh:** Connection khula rakho, price badla tab server khud bheje (WebSocket push), har ~100 ms aakhri wala.


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
