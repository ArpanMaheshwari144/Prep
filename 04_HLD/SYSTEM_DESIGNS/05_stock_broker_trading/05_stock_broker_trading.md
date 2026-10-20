# Stock Broker / Trading Platform

> User BUY / SELL order deta hai, system order match karta hai, paisa aur share sahi jagah pahunchte hain, sabko live price dikhta hai.
> Dil do cheezein: paisa kabhi galat nahi (strong consistency), aur matching bahut tez (RAM me, microseconds). Finance interview me (JP / GS) yahi poochte: consistency, ACID, idempotency, ledger, audit.
>
> Mock me sirf heading + Cue + GIF. Neeche har card ek block: HAL (GIF ka ILAAJ) · KYUN · NAHI (doosra kyun nahi) · AGAR (cross-Q).
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

---

*ab tootega kahan? sabse pehle darwaze pe, jab ek tap do baar pahunche*

## 1. Maine ek baar dabaya, order do lag gaye

**Cue:** IDEMPOTENCY KEY (wahi id dobara) · dabba: Order Service

```
HAL     screen khulte hi app ek id banata hai. Network gaya aur app ne dobara bheja, to wahi id jaati hai.
        Server id pe UNIQUE: wahi id aayi to purana jawab, order ek hi (idempotency key)
KYUN    ek id = ek kaam. Id app banata, server UNIQUE se pakadta. User ko ek hi order dikhe,
        app kitni baar bhi bheje
NAHI    button pehli tap pe band -> ADHOORA: kaafi nahi, retry app khud karta hai, button dabta hi nahi
AGAR    wahi id dobara aayi, aur pehli wali abhi chal rahi hai
        -> "thoda ruko" bolo (IN_PROGRESS, 409). Ho gayi ho to saved jawab do
        id kab tak yaad rakhoge?
        -> ~24 ghante, phir saaf. Retry minute me hi aata hai
```

---

*ab order ek hai, par paisa do order me baant diya to?*

## 2. Wallet me 50k the, phir bhi 30k-30k ke dono order lag gaye

**Cue:** check + block EK atomic query · dabba: Wallet

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
AGAR    block kiya, phir order cancel ho gaya
        -> block hata do
        block kiya, aur sauda aadha hua
        -> jitna bika utna kata, baaki blocked rehta. Order abhi khula hai
        MARKET order me bhav pata nahi, block kitna karoge?
        -> aakhri bhav + thoda upar block karo. Sauda ke baad bacha paisa wapas
        SELL pe bhi aisa hi block?
        -> haan, wahi line portfolio pe: qty - blocked_qty >= 10
        order ki umar khatam ho gayi (day order)
        -> shaam ko block hatao. Blocked sirf sauda / cancel / umar se ghatta hai
        cancel aur match usi pal aaye
        -> book ek haath me hai. Jo nahi bika, sirf uska block hatao
```

---

*ab paisa theek, par ek hi share do khareedaron ko?*

## 3. Suresh ke paas 10 the, mujhe aur Mohan dono ko 10 mil gaye

**Cue:** ek symbol ek thread, book RAM me · dabba: Matching Engine

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
AGAR    BUY 2998 aaya
        -> top SELL 3000 se kam hai, sauda nahi. Book me khada rehta (mehnga upar)
        same daam pe do log aaye
        -> jo pehle aaya (chhota number) wo pehle
        mujhe 10 chahiye, uske paas 6 hain
        -> 6 ka sauda, baaki 4 book me, unka paisa ruka. Naya seller @3000 aaya to 4 bhi bhare.
           Status: FILLED / PARTIALLY FILLED / OPEN (Zerodha / Groww / NSE me yahi)
        book DB me rakhi to?
        -> nahi, RAM me rakho. DB = har order pe milliseconds, RAM = microseconds
        5000 stock = 5000 thread?
        -> nahi. 8 core = 8 thread. stock % 8, par ek stock hamesha ek hi thread pe
```

---

*ab ek thread hai, par 9:15 pe bheed aayi to?*

## 4. 9:15 baje sab TCS pe toot pade

**Cue:** baanto mat, ek thread kaafi, line · dabba: Queue

```
HAL     hazaaron order / sec aaye, to bhi TCS ki ek line, ek thread RAM me tez nipta deta.
        Line me lagte hi user ko ACCEPTED turant, FILLED tab jab sauda ho
KYUN    RAM me ek thread lakhon order / sec nipta deta. Line lambi = der, par order khota nahi, line me hai
NAHI    har order pe lock + DB -> DHEEMA: ek-ek karke dheema, jam
        TCS ko do thread me baanto -> TOOTA: ek stock = ek line. Baantna stocks ke BEECH,
           ek stock ke andar kabhi nahi
AGAR    baantoge nahi, to badhaoge kaise?
        -> stocks ke beech: TCS ek thread, INFY doosra. TCS ke andar nahi
```

---

*ab sab RAM me hai, server gira to?*

## 5. Server gira, poori order book gayab

**Cue:** pehle log, phir book; gira = replay · dabba: Event Log (= wahi line)

```
HAL     har order pehle log me (disk), phir book me. Log me har order ko number (#501).
        10:15 pe gira, 4,000 order gayab, to log dobara chalao, wahi 4,000 wapas (replay)
KYUN    log hi sach hai, book uski photo. Gira = aakhri photo + uske baad ke panne.
        Dobara chalaya to kuch nahi badalta (trade_id)
NAHI    sirf book ki photo -> ADHOORA: photo ke BAAD wale order kahin likhe hi nahi
        time se kram -> GALAT: do machine ka time (clock) alag, aur ek second me do order aa jaate
AGAR    book RAM me hai, crash hua to gayi?
        -> match se PEHLE log me likhte hain. Crash pe log shuru se chalao, wahi book wapas
        replay me der lagi to?
        -> standby pehle se log padhta rehta, book taiyaar. Pehla gire tab wo chalu
        dono board ek saath chala do?
        -> nahi. Do board = do sach = galat
        crash me paise ka kya?
        -> paisa DB me hai, wahan rollback. Book log se wapas
```

---

*ab sauda hua, par paisa aadha pahuncha to?*

## 6. Mera paisa kat gaya, Suresh ko mila hi nahi

**Cue:** ek transaction: chaaron ya koi nahi · dabba: Settlement

```
HAL     chaaron badlaav ek transaction me: Arpan -30k +10 share, Suresh +30k -10 share, phir COMMIT.
        Ya sab, ya koi nahi
KYUN    paisa ek DB me hai, to chaaron likhai ek transaction. COMMIT se pehle crash = rollback, kuch nahi
        likha, user ko PENDING, galat number kabhi nahi. Ledger ka jod hamesha 0
NAHI    aakhir me "done" flag -> ANDHA: flag se pehle gira to pata hi nahi kitna likha tha
AGAR    rollback ke baad retry hua, to Suresh ko do baar mila?
        -> nahi, trade_id UNIQUE hai, dobara = kuch nahi
        balance ledger jod ke nikaal lo?
        -> dheema hai, lakhon line. Balance alag column me, usi transaction me
        asli paisa usi pal Suresh ko gaya?
        -> nahi, T+1 NSE Clearing. Ye broker ka apna khaata hai
        galti pakdoge kaise?
        -> raat ko ledger jodo, hamesha 0 aana chahiye. Galti turant dikhe
```

---

*ab paisa ek DB me, share doosre me, ek transaction nahi ban sakti to?*

## 7. Dopahar: paisa Wallet DB me, share Portfolio DB me — INFY ka 20k kata, share nahi aaye

**Cue:** har step ka haal, aage badho · dabba: Wallet DB + Portfolio DB

```
HAL     do DB = ek transaction nahi ban sakti. Har kadam ka haal likho (status log: wallet done,
        portfolio done). Fail hua to likhe haal se retry, aage hi, jab tak portfolio done (SETTLED)
KYUN    match ke baad sauda PAKKA hai. System hi aage badhaata (retry), user ko kuch dobara nahi karna.
        Beech ka haal user ko PENDING dikhta
NAHI    paisa ulta kar do -> GALAT: match ke baad sauda pakka, bechne wale ko 20k mil chuke
AGAR    step-step karne ki keemat kya hai?
        -> beech me PENDING dikhta hai, par 2PC jaisa lamba lock nahi
        retry dobara chal gaya
        -> har kadam trade_id se. Ho chuka = no-op, share do baar nahi
        match se pehle hi atak gaya to?
        -> sirf tab ulta (SAGA): block hatao
        ghanton tak fail ho raha hai
        -> trade PENDING + alert, ops share daalta
        Portfolio DB ghanta bhar band, user dobara order kare?
        -> nahi: naya + purana dono zinda, DB wapas = 5 ki jagah 10 INFY, paisa do baar.
           Trade PENDING + alert, system wahi trade_id se retry, user kuch nahi karta
        orchestration ya choreography?
        -> orchestration. Paise me saaf dikhe kaunsa step atka
```

---

*ab andar sab theek, par bahar 10 lakh log price maang rahe*

## 8. 10 lakh log ek saath price poochh rahe, server baith gaya

**Cue:** push (TV jaisa), WebSocket · dabba: Pub/Sub + WebSocket

```
HAL     connection khula rakho, price badla tabhi server khud bheje (WebSocket push).
        Matching -> pub/sub -> har WebSocket server -> users
KYUN    price = sirf abhi ka. Badla tab server khud bheje, purana store nahi. Users kuch nahi poochte
NAHI    har user baar-baar poochhe (polling) -> BAITHA: 10 lakh har second = server baitha.
           Poochhne ki line hi nahi chahiye
AGAR    second me 1000 baar price badla
        -> har ~100 ms aakhri price bhejo. Purana bekaar
        connection toota, wapas juda
        -> abhi ka price alag se do. Pub/sub kuch rakhta nahi
        10 lakh connection ek server pe?
        -> ek server ~lakh connection. Aur server + LB
        price sab server tak kaise pahunchta hai?
        -> pub/sub se har server ko
```

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

---

ARCHETYPE C (transactional) · CONCEPTS: [db-what-when](../../FOUNDATIONS/09_databases_what_when.md) · [CAP](../../FOUNDATIONS/08_cap_theorem.md) · saath: [payment](../06_payment_system/06_payment_system.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
