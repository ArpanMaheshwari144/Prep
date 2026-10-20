# Stock Broker / Trading Platform

> User BUY / SELL order deta hai, system order match karta hai, paisa aur share sahi jagah pahunchte hain, sabko live price dikhta hai.
> Dil do cheezein: paisa kabhi galat nahi (strong consistency), aur matching bahut tez (RAM me, microseconds). Finance interview me (JP / GS) yahi poochte: consistency, ACID, idempotency, ledger, audit.
>
> Mock me sirf heading + Cue + GIF. Neeche har card: **hal** · **kyun** · **nahi** (doosra kyun nahi) · **Pooche**.

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c8_2-41340ea6.svg)

---


## Shuru

- **FR:** scope pehle poochho (matching, price feed, risk, settlement) · hum: buy / sell (LIMIT + MARKET, aadha bhara / partial fill bhi), order match, paisa + share move (settlement), live price, cancel / status · scope bahar: risk / margin / IPO / mutual fund
- **NFR:** paisa hamesha sahi (strong, "thodi der baad sahi" nahi) · tez · imaandar (jo pehle aaya wo pehle) · har cheez ka record (audit)
- **numbers:** ~50 lakh user, ~50 lakh order roz, market ~6 ghante (~20,000 sec) -> ~250 order / sec. 9:15 pe jhatka 10,000+ / sec. Price: ~10 lakh log app khula, har second naya price -> ~10 lakh / sec. Isliye matching RAM me, price PUSH (har user ka baar-baar poochhna nahi)

---

*ab tootega kahan? sabse pehle darwaze pe, jab ek tap do baar pahunche*

## 1. Maine ek baar dabaya, order do lag gaye

**Cue:** IDEMPOTENCY KEY (wahi id dobara) · dabba: Order Service

- **hal:** screen khulte hi app ek id banata. Network gaya, app dobara bheje, to wahi id. Server id pe UNIQUE: wahi id = purana jawab, order ek (idempotency key)
- **kyun:** ek id = ek kaam. Id app banata, server UNIQUE se pakadta. User ko ek hi order dikhe, app kitni baar bhi bheje.
- **nahi:** button pehli tap pe band -> kaafi nahi, retry app khud karta, button dabta hi nahi
- **Pooche:** wahi id aayi, pehli abhi chal rahi? -> "thoda ruko" (IN_PROGRESS, 409). Ho gaya to saved jawab.
- **Pooche:** id kab tak yaad? -> ~24 ghante, phir saaf. Retry minute me hi aata.

---

*ab order ek hai, par paisa do order me baant diya to?*

## 2. Wallet me 50k the, phir bhi 30k-30k ke dono order lag gaye

**Cue:** check + block EK atomic query · dabba: Wallet

- **hal:** check aur block ek hi query: `SET blocked = blocked + 30k WHERE total - blocked >= 30k`. Ek jeeta = 1 row, block. Doosre ko 20k bacha, 0 row, turant reject (atomic UPDATE)
- **kyun:** "hai?" aur "block" ek hi UPDATE, DB ke andar. Kitni bhi machine, DB ek-ek karke chalata. Wallet kabhi minus nahi.
- **blocked:** badhta naye order se, ghatta sauda / cancel / umar khatam se
- **nahi:** pehle padho, phir block -> laptop (TCS 30k) + phone (INFY 30k), dono server ne 50k dekha, dono pass, wallet minus
- **nahi:** Java synchronized -> taala ek machine ka, phone doosre server pe gaya
- **Pooche:** block kiya, phir order cancel? -> block hatao
- **Pooche:** block kiya, sauda aadha hua? -> jitna bika utna kata, baaki blocked. Order abhi khula.
- **Pooche:** MARKET order me bhav pata nahi, block kitna? -> aakhri bhav + thoda upar, sauda ke baad bacha wapas
- **Pooche:** SELL pe bhi aisa? -> wahi line portfolio pe: qty - blocked_qty >= 10
- **Pooche:** order ki umar khatam (day order)? -> shaam ko block hatao. Blocked sirf sauda / cancel / umar se ghatta.
- **Pooche:** cancel aur match usi pal? -> book ek haath me hai. Jo nahi bika sirf uska block hatao.

---

*ab paisa theek, par ek hi share do khareedaron ko?*

## 3. Suresh ke paas 10 the, mujhe aur Mohan dono ko 10 mil gaye

**Cue:** ek symbol ek thread, book RAM me · dabba: Matching Engine

- **hal:** ek share ke saare order ek line, us line ko ek hi thread (ek symbol ek thread). Main pehle aaya = 10 mile, Mohan ko 0, wo line me.
- **kyun:** ek stock ki book ko ek waqt ek hi haath. Doosra line me rukta, race hi nahi.
- **kyun:** thread atka (GC pause) to sirf us stock ki line rukti, INFY apne thread pe, standby taiyaar
- **RAM / SQL:** jo tez + thodi der ka (order book) = RAM. Jo paisa + hamesha ka (wallet / ledger) = SQL DB + ACID.
- **order book:** do taraf: bechne wale sasta upar (3000, 3001, 3005), khareedne wale mehnga upar (2999, 2998)
- **sauda:** tabhi jab top khareedar >= top bechne wala. Daam book wale ka: naya BUY 3001, top SELL 3000 -> sauda 3000 pe. Same daam pe pehle aaya pehle bikta.
- **broker:** is design me hum broker, par apna matching (exchange jaisa). Asli India me Groww / Zerodha order NSE ko bhejte, matching NSE; wallet block broker ka kaam.
- **nahi:** do thread bina line -> dono ko "10 hai" dikha, dono ne le liye, 20 bik gaye
- **nahi:** har order pe lock -> race share pe hai, order pe nahi. Dono apna lock lete, dono ko 10 dikhe.
- **nahi:** sabse simple: Order service DB me likhe, DB me hi milta SELL dhoondh ke match (DB ka ACID) -> DB har order pe dheema, matching RAM me
- **Pooche:** BUY 2998 aaya? -> top SELL 3000 se kam, sauda nahi. Book me khada (mehnga upar).
- **Pooche:** same daam, do log? -> jo pehle aaya (chhota number) wo pehle
- **Pooche:** mujhe 10 chahiye, uske paas 6? -> 6 ka sauda, baaki 4 book me, unka paisa ruka. Naya seller @3000 = 4 bhi bhare. Status: FILLED / PARTIALLY FILLED / OPEN (Zerodha / Groww / NSE me yahi)
- **Pooche:** book DB me rakhi to? -> RAM me rakho. DB = har order pe milliseconds, RAM = microseconds.
- **Pooche:** 5000 stock = 5000 thread? -> nahi. 8 core = 8 thread. stock % 8, par ek stock hamesha ek hi thread pe.

---

*ab ek thread hai, par 9:15 pe bheed aayi to?*

## 4. 9:15 baje sab TCS pe toot pade

**Cue:** baanto mat, ek thread kaafi, line · dabba: Queue

- **hal:** hazaaron order / sec -> TCS ki ek line, ek thread RAM me tez nipta deta. Line me lagte hi user ko ACCEPTED turant, FILLED tab jab sauda ho.
- **kyun:** RAM me ek thread lakhon order / sec nipta deta. Line lambi = der, par order khota nahi, line me hai.
- **nahi:** har order pe lock + DB -> ek-ek karke dheema, jam
- **nahi:** TCS ko do thread me baanto -> ek stock = ek line. Baantna stocks ke BEECH, ek stock ke andar kabhi nahi.
- **Pooche:** baanto nahi, to badhaoge kaise? -> stocks ke beech: TCS ek thread, INFY doosra. TCS ke andar nahi.

---

*ab sab RAM me hai, server gira to?*

## 5. Server gira, poori order book gayab

**Cue:** pehle log, phir book; gira = replay · dabba: Event Log (= wahi line)

- **hal:** har order pehle log me (disk), phir book me. Log me har order ko number (#501). 10:15 gira, 4,000 order gayab -> log dobara chalao, wahi 4,000 wapas (replay).
- **kyun:** log hi sach, book uski photo. Gira = aakhri photo + uske baad ke panne. Dobara chala to kuch nahi badalta (trade_id).
- **nahi:** sirf book ki photo -> photo ke BAAD wale order kahin likhe hi nahi
- **nahi:** time se kram -> do machine ka time (clock) alag, aur ek second me do order aa jaate
- **Pooche:** book RAM me, crash hua to gayi? -> match se PEHLE log me likhte. Crash pe log shuru se chalao, wahi book wapas.
- **Pooche:** replay me der lagi to? -> standby pehle se log padhta, book taiyaar. Pehla gire tab wo chalu.
- **Pooche:** dono board ek saath chala do? -> nahi. Do board = do sach = galat.
- **Pooche:** crash me paisa ka kya? -> paisa DB me, wahan rollback. Book log se wapas.

---

*ab sauda hua, par paisa aadha pahuncha to?*

## 6. Mera paisa kat gaya, Suresh ko mila hi nahi

**Cue:** ek transaction: chaaron ya koi nahi · dabba: Settlement

- **hal:** chaaron badlaav ek transaction: Arpan -30k +10 share, Suresh +30k -10 share, phir COMMIT. Ya sab, ya koi nahi.
- **kyun:** paisa ek DB me, to chaaron likhai ek transaction. COMMIT se pehle crash = rollback, kuch nahi likha, user ko PENDING, galat number kabhi nahi. Ledger ka jod hamesha 0.
- **nahi:** aakhir me "done" flag -> flag se pehle gira to pata hi nahi kitna likha tha
- **Pooche:** rollback ke baad retry, Suresh ko do baar? -> nahi, trade_id UNIQUE, dobara = kuch nahi
- **Pooche:** balance ledger jod ke nikaalo? -> dheema, lakhon line. Balance alag column, usi transaction me.
- **Pooche:** asli paisa usi pal Suresh ko gaya? -> nahi, T+1 NSE Clearing. Ye broker ka apna khaata.
- **Pooche:** galti pakdoge kaise? -> raat ko ledger jodo, hamesha 0. Galti turant dikhe.

---

*ab paisa ek DB me, share doosre me, ek transaction nahi ban sakti to?*

## 7. Dopahar: paisa Wallet DB me, share Portfolio DB me — INFY ka 20k kata, share nahi aaye

**Cue:** har step ka haal, aage badho · dabba: Wallet DB + Portfolio DB

- **hal:** do DB = ek transaction nahi. Har kadam ka haal likho (status log: wallet done, portfolio done). Fail = likhe haal se retry, aage hi, jab tak portfolio done (SETTLED).
- **kyun:** match ke baad sauda PAKKA. System hi aage badhaata (retry), user ko kuch dobara nahi. Beech ka haal user ko PENDING.
- **nahi:** paisa ulta kar do -> match ke baad sauda pakka, bechne wale ko 20k mil chuke
- **Pooche:** step-step ki keemat? -> beech me PENDING dikhta, par 2PC jaisa lamba lock nahi
- **Pooche:** retry dobara chal gaya? -> har kadam trade_id se. Ho chuka = no-op, share do baar nahi.
- **Pooche:** match se pehle atka to? -> sirf tab ulta (SAGA): block hatao
- **Pooche:** ghanton fail? -> trade PENDING + alert, ops share daalta
- **Pooche:** Portfolio DB ghanta band, user dobara order kare? -> nahi: naya + purana dono zinda, DB wapas = 5 ki jagah 10 INFY, paisa do baar. Trade PENDING + alert, system wahi trade_id se retry, user kuch nahi karta.
- **Pooche:** orchestration ya choreography? -> orchestration. Paise me saaf dikhe kaunsa step atka.

---

*ab andar sab theek, par bahar 10 lakh log price maang rahe*

## 8. 10 lakh log ek saath price poochh rahe, server baith gaya

**Cue:** push (TV jaisa), WebSocket · dabba: Pub/Sub + WebSocket

- **hal:** connection khula rakho, price badla tabhi server khud bheje (WebSocket push). Matching -> pub/sub -> har WebSocket server -> users.
- **kyun:** price = sirf abhi ka. Badla tab server khud bheje, purana store nahi. Users kuch nahi poochte.
- **nahi:** har user baar-baar poochhe (polling) -> 10 lakh har second = server baitha. Poochhne ki line hi nahi chahiye.
- **Pooche:** second me 1000 baar badla? -> har ~100 ms aakhri price. Purana bekaar.
- **Pooche:** connection toota, wapas juda? -> abhi ka price alag se do. Pub/sub kuch rakhta nahi.
- **Pooche:** 10 lakh connection ek server pe? -> ek server ~lakh connection. Aur server + LB.
- **Pooche:** price sab server tak kaise? -> pub/sub se har server ko

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
