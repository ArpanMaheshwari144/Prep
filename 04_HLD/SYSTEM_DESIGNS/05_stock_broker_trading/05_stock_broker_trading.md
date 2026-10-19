# Stock Broker / Trading Platform

> User BUY / SELL order deta hai, system order match karta hai, paisa aur share sahi jagah pahunchte hain, sabko live price dikhta hai.
> Dil do cheezein: paisa kabhi galat nahi (strong consistency), aur matching bahut tez (RAM me, microseconds). Finance interview me (JP / GS) yahi poochte: consistency, ACID, idempotency, ledger, audit.
>
> Har card: kya toota -> kya kiya -> yahi kyun -> doosra kyun nahi. Kram me chalo. "Pooche" wali line = interviewer aage poochhe to.

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c8_2-41340ea6.svg)

---

## Mock cue (mock me sirf ye dekho: kya hua -> ilaaj -> kaunsa dabba juda)

```
   KYA HUA                                     ILAAJ                                DABBA JUDA
1  ek tap, network gaya, do order lag gaye  -> IDEMPOTENCY KEY (wahi id dobara)     Order Service
2  wallet 50k, 30k-30k dono order pass      -> check + block EK atomic query        Wallet
3  Suresh ke 10, mujhe aur Mohan dono ko    -> ek symbol ek thread, book RAM me     Matching Engine
4  9:15 sab TCS pe toot pade                -> baanto mat, ek thread kaafi, line    Queue
5  10:15 server gira, book gayab            -> pehle log, phir book; gira = replay  Event Log (= wahi line)
6  mera 30k kata, Suresh ko mila nahi       -> ek transaction: chaaron ya koi nahi  Settlement
7  dopahar INFY 20k kata, share nahi aaye   -> har step ka haal, aage badho         Wallet DB + Portfolio DB
8  10 lakh log price poochh rahe            -> push (TV jaisa), WebSocket           Pub/Sub + WebSocket

POORA DESIGN (upar ke dabbe kram se):
   User -> Order Service (id) -> Wallet (block) -> Event Log -> Matching (ek stock ek thread)
        -> Settlement -> Wallet DB + Portfolio DB
   price: Matching -> Pub/Sub -> WebSocket -> users
```

---

## Shuru

Sabse simple: Order service har order DB me likhti, aur DB me milta-julta SELL dhoondh ke match karti.

Scope pehle poochho. Trading bada hai: matching, price feed, risk, settlement. Hum order matching aur paise / share
ka settlement (len-den poora karna) lenge. Risk / margin / IPO / mutual fund scope me nahi. LIMIT aur MARKET dono
order chalenge, aadha bhara (partial fill) bhi. Is design me hum broker hain par apna matching karte (exchange jaisa).
Asli India me Groww / Zerodha order NSE ko bhejte, matching NSE karta; wallet block broker ka kaam.

Kya chahiye: buy / sell, order match, paisa + share move, live price, cancel / status. Kaisa chale: paisa hamesha
sahi (strong, kabhi "thodi der baad sahi" nahi), tez, imaandar (jo pehle aaya wo pehle), har cheez ka record (audit).

Numbers: ~50 lakh user, ~50 lakh order roz. Market ~6 ghante (~20,000 second) khulta, to ~250 order har second.
9:15 baje market khulte hi jhatka, 10,000+ order har second. Price dekhne wale bahut zyada: ~10 lakh log app khula
rakhte, har second naya price, to ~10 lakh har second. Isliye matching RAM me, aur price PUSH karo (har user ka
baar-baar poochhna nahi). Jo tez chahiye aur thodi der ka hai (order book) wo RAM me. Jo paisa hai aur hamesha ka
hai (wallet / ledger) wo SQL DB + ACID me.

Order book kaise match karta? TCS ka share: kuch bechna chahte (SELL), kuch khareedna (BUY). Book ki do taraf hain:
bechne wale sasta upar (3000, 3001, 3005), khareedne wale mehnga upar (2999, 2998). Sauda tabhi jab top khareedar
>= top bechne wala. Daam pehle se book wale ka. Naya BUY 3001 aaya, top SELL 3000, to sauda 3000 pe. Same daam pe jo
pehle aaya wo pehle bikta.

**Pooche: BUY 2998 aaya to?** Top SELL 3000 se kam hai, sauda nahi. Order book me khada rehta (mehnga upar).

**Pooche: same daam, do log?** Jo pehle aaya (chhota number) wo pehle.

**Pooche: mujhe 10 chahiye, uske paas 6?** 6 ka sauda. Baaki 4 book me khade, un 4 ka paisa ruka rehta. Naya seller
@3000 aaya to 4 bhi bhar gaye. Status: FILLED / PARTIALLY FILLED / OPEN (Zerodha / Groww / NSE me yahi).

**Pooche: book DB me rakhi to?** RAM me rakho. DB = har order pe milliseconds, RAM = microseconds.

**Pooche: RAM me hai, crash hua to book gayi?** Match se PEHLE log me likhte hain. Crash pe log shuru se chalao, wahi
book wapas (card 5).

---

*ab tootega kahan? sabse pehle darwaze pe, jab ek tap do baar pahunche*

## 1. Maine ek baar dabaya, order do lag gaye

Isliye screen khulte hi app ek id banata hai. Network gaya aur app dobara bheje, to wahi id jaati hai. Server us id
pe UNIQUE rakhta, wahi id dobara aayi to purana jawab, order ek hi (idempotency key).

Ye kyun chalta? Ek id = ek kaam. Id app banata, server UNIQUE se pakadta. Wahi id dobara = purana jawab, kaam dobara
nahi. User ko ek hi order dikhe, app chahe kitni baar bheje.

Button pehli tap pe band kar do to? Kaafi nahi. Retry app khud karta hai, button dabta hi nahi.

**Pooche: wahi id aayi, par pehli abhi chal rahi?** "Thoda ruko" (IN_PROGRESS, 409). Ho gaya to saved jawab.

**Pooche: id kab tak yaad rakhoge?** ~24 ghante, phir saaf. Retry minute me hi aata.

---

*ab order ek hai, par paisa do order me baant diya to?*

## 2. Wallet me 50k the, phir bhi 30k-30k ke dono order lag gaye

Laptop se TCS 30k, phone se INFY 30k. Dono server ne pehle 50k dekha, dono pass, wallet minus me. Isliye check aur
block ek hi query me: `SET blocked = blocked + 30k WHERE total - blocked >= 30k`. Ek jeeta to 1 row, block. Doosre
ko 20k hi bacha, 0 row, turant reject (atomic UPDATE).

Ye kyun chalta? "Hai?" aur "block" ek hi UPDATE, DB ke andar. Kitni bhi machine ho, DB ek-ek karke chalata. Wallet
kabhi minus nahi. 'blocked' badhta naye order se, ghatta sauda / cancel / umar khatam se.

Java synchronized laga do? Nahi. Taala ek machine ka hai, phone doosre server pe gaya.

**Pooche: block kiya, phir order cancel?** Block hatao.

**Pooche: block kiya, sauda aadha hua?** Jitna bika utna kata, baaki blocked. Order abhi khula hai.

**Pooche: MARKET order me bhav pata nahi, block kitna?** Aakhri bhav + thoda upar block, sauda ke baad bacha wapas.

**Pooche: SELL pe bhi aisa?** Wahi line portfolio pe: qty - blocked_qty >= 10.

**Pooche: order ki umar khatam (day order)?** Shaam ko block hatao. Blocked sirf sauda / cancel / umar se ghatta.

**Pooche: cancel aur match usi pal?** Book ek haath me hai. Jo nahi bika sirf uska block hatao.

---

*ab paisa theek, par ek hi share do khareedaron ko?*

## 3. Suresh ke paas 10 the, mujhe aur Mohan dono ko 10 mil gaye

Do thread, dono ko "10 hai" dikha, dono ne le liye, 20 bik gaye. Isliye ek share ke saare order ek line me, aur us
line ko ek hi thread chalata (ek symbol ek thread). Main pehle aaya to 10 mile, Mohan ko 0, wo line me.

Ye kyun chalta? Ek stock ki book ko ek waqt ek hi haath. Doosra line me rukta, race hi nahi. Thread atka (GC pause)
to sirf us stock ki line rukti, INFY apne thread pe chalta, standby taiyaar.

Har order pe lock laga do? Nahi. Race share pe hai, order pe nahi. Dono apna-apna lock lete, dono ko 10 dikhe.

Matching DB ke ACID se kyun nahi? DB har order pe dheema. Matching RAM me.

**Pooche: 5000 stock = 5000 thread?** Nahi. 8 core = 8 thread. stock % 8, par ek stock hamesha ek hi thread pe.

---

*ab ek thread hai, par 9:15 pe bheed aayi to?*

## 4. 9:15 baje sab TCS pe toot pade

Hazaaron order har second. Isliye TCS ki ek line, aur ek thread RAM me tez nipta deta, lakhon har second. Line me
lagte hi user ko ACCEPTED turant, FILLED tab jab sauda ho.

Ek thread hi kyun kaafi? RAM me ek thread lakhon order har second nipta deta. Line lambi hui to der, par order khota
nahi, line me hai.

Har order pe lock + DB? Ek-ek karke dheema, jam.

TCS ko do thread me baant do? Nahi. Ek stock = ek line. Baantna stocks ke BEECH, ek stock ke andar kabhi nahi.

**Pooche: baanto nahi, to badhaoge kaise?** Stocks ke beech: TCS ek thread, INFY doosra. TCS ke andar nahi.

---

*ab sab RAM me hai, server gira to?*

## 5. Server gira, poori order book gayab

10:15 matching server gira, book sirf RAM me thi, 4,000 order gayab. Isliye har order pehle log me (disk), phir
book me. Gira to log dobara chalao, wahi 4,000 wapas (replay). Log me har order ko number milta (#501).

Ye kyun chalta? Log hi sach hai, book uski photo. Pehle log, phir book. Gira = log se replay: aakhri photo + uske
baad ke panne. Dobara chala to kuch nahi badalta (trade_id).

Sirf book ki photo kaafi? Nahi. Photo ke BAAD wale order kahin likhe hi nahi.

Time se kram kyun nahi, number kyun? Do machine ka time (clock) alag, aur ek second me do order aa jaate.

**Pooche: replay me der lagi to?** Standby pehle se log padhta rehta, book taiyaar. Pehla gire tab wo chalu.

**Pooche: dono board ek saath chala do?** Nahi. Do board = do sach = galat.

**Pooche: crash me paisa ka kya?** Paisa DB me hai, wahan rollback. Book log se wapas.

---

*ab sauda hua, par paisa aadha pahuncha to?*

## 6. Mera paisa kat gaya, Suresh ko mila hi nahi

Mera wallet -30k hua, phir crash, Suresh ko +30k nahi gaya. Isliye chaaron badlaav ek transaction me: Arpan -30k
+10 share, Suresh +30k -10 share, phir COMMIT. Ya sab, ya koi nahi.

Ye kyun chalta? Paisa ek DB me hai, to chaaron likhai ek transaction. COMMIT se pehle crash = rollback, kuch nahi
likha, user ko PENDING dikhta, galat number kabhi nahi. Ledger ka jod hamesha 0.

Aakhir me "done" flag laga do? Flag se pehle gira to pata hi nahi kitna likha tha.

**Pooche: rollback ke baad retry, Suresh ko do baar?** Nahi. trade_id UNIQUE hai, dobara = kuch nahi.

**Pooche: balance ledger jod ke nikaalo?** Dheema, lakhon line. Balance alag column, usi transaction me.

**Pooche: asli paisa usi pal Suresh ko gaya?** Nahi, T+1 NSE Clearing. Ye broker ka apna khaata hai.

**Pooche: galti pakdoge kaise?** Raat ko ledger jodo, hamesha 0. Galti turant dikhe.

---

*ab paisa ek DB me, share doosre me, ek transaction nahi ban sakti to?*

## 7. Dopahar: paisa Wallet DB me, share Portfolio DB me — INFY ka 20k kata, share nahi aaye

Wallet DB me -20k hua, crash, Portfolio DB me 5 INFY nahi aaye. Do DB, ek transaction nahi. Isliye har kadam ka haal
likhte hain (status log: wallet done, portfolio done). Fail hua to likhe haal se retry, aage hi, jab tak portfolio
done na ho (SETTLED).

Ye kyun chalta? Match ke baad sauda PAKKA. System hi aage badhaata (retry), user ko kuch dobara nahi karna. Beech ka
haal user ko PENDING dikhe.

Paisa ulta kyun nahi kar dete? Match ke baad sauda pakka. Bechne wale ko 20k mil chuke.

**Pooche: step-step ki keemat?** Beech me PENDING dikhta, par 2PC jaisa lamba lock nahi.

**Pooche: retry dobara chal gaya?** Har kadam trade_id se. Ho chuka = no-op, share do baar nahi.

**Pooche: match se pehle atka to?** Sirf tab ulta (SAGA): block hatao.

**Pooche: ghanton fail?** Trade PENDING + alert, ops share daalta.

**Pooche: Portfolio DB ghanta bhar band, user ko bolo dobara order kare?** Nahi. Naya order gaya aur purana bhi zinda,
DB wapas aaya to 5 ki jagah 10 INFY, paisa do baar kata. Isliye trade PENDING + alert, system wahi trade_id se retry
kare, user nahi. User kuch nahi karta, trade khud poora.

**Pooche: orchestration ya choreography?** Orchestration. Paise me saaf dikhe kaunsa step atka.

---

*ab andar sab theek, par bahar 10 lakh log price maang rahe*

## 8. 10 lakh log ek saath price poochh rahe, server baith gaya

Har user har second poochh raha tha, server band. Isliye connection khula rakhte, price badla tabhi server khud
bhejta (WebSocket push). Matching se price pub/sub pe jaata, wahan se har WebSocket server tak, wahan se users ko.

Ye kyun chalta? Price = sirf abhi ka. Badla tab server khud bheje, purana store nahi. Users kuch nahi poochte.

Har user baar-baar poochhe (polling)? Poochhne ki line hi nahi chahiye. 10 lakh har second = server baitha.

**Pooche: second me 1000 baar badla?** Har ~100 ms aakhri price. Purana bekaar.

**Pooche: connection toota, wapas juda?** Abhi ka price alag se do. Pub/sub kuch rakhta nahi.

**Pooche: 10 lakh connection ek server pe?** Ek server ~lakh connection. Aur server + LB.

**Pooche: price sab server tak kaise?** Pub/sub se har server ko.

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
