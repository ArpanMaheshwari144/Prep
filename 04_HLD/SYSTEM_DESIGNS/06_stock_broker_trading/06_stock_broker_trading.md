# Stock Broker / Trading Platform — POORA ROUND (4 MOVE)

> **NAV** — ARCHETYPE C (transactional) · DIL: order match + paisa/share consistent. UP: [MASTER](../../00_MASTER_SHEET.md) · CONCEPTS: [db-what-when](../../FOUNDATIONS/09_databases_what_when.md) · [CAP](../../FOUNDATIONS/08_cap_theorem.md) · saath: [payment](../07_payment_system/07_payment_system.md)

> Finance interview GOLD (JP/GS): consistency, ACID, idempotency, ledger, audit — FAANG-hyperscale NAHI.
> Konovo fraud-domain se bridge. FLAVOR = CONSISTENCY + LATENCY dono bhaari (paisa + speed).
> 15-Sep: asli mock-video ke hisaab se dobara likha — koi 7-step rail nahi, sirf 4 move.
> Har jagah: **tu kya BOLTA hai · BOARD pe kya banta · FAISLA + KYUN**.
>
> Problem (1 line): user buy/sell order de -> system MATCH kare -> paisa + share consistent move ho -> aur live price dikhe.

```
★★ TEEN NIYAM (poori file par lagte — [APPROACH_DELIVERY](../../01_DELIVERY.md) 5b)
   1. PERFECT design ek saath mat banao — chhote se shuru, dikkat pe badhao
   2. NUMBER ke peeche mat bhaago — bolo, ek faisla nikaalo, aage badho
   3. BOTTLENECK ratto mat — KHUD USER banke raasta chalao, khud dikh jaayega
```

---

# MOVE 1 — POOCHO (board pe abhi kuch nahi)

```
   TU: "Trading platform bada hai — order matching, portfolio, live price feed, risk checks,
        settlement. Aap kis pe focus karwana chahenge?
        Main ORDER MATCHING aur paise ke movement pe ja sakta hoon — wahin asli dikkat hai."

   TU: "Kuch cheezein confirm kar lun —
          - LIMIT aur MARKET dono order chahiye?
          - PARTIAL match allowed hai (10 me se 6 mile)?
          - hum exchange hain, ya broker jo exchange ko order bhejta hai?
          - live price feed bhi scope me hai?"

   ★ KEY BAAT jo shuru me hi bolni hai:
     "Yahan paisa bhi hai aur speed bhi — isliye consistency NON-NEGOTIABLE hai
      (strong, kabhi eventual nahi), aur latency bhi critical hai."
```

---

# MOVE 2 — DO CHHOTE BLOCK LIKHO

```
   ┌──────────────────────┐    ┌────────────────────────────────────┐
   │ Trading Platform     │    │ Use cases:                         │
   │   - User / Wallet    │    │   - BUY / SELL order (stock, qty,  │
   │   - Stock (symbol)   │    │     price, type)                   │
   │   - Order            │    │   - order MATCH ho                 │
   │   - Trade            │    │   - paisa + share move ho          │
   │   - Portfolio        │    │   - live price dikhe               │
   │   - Ledger           │    │   - order cancel / status          │
   └──────────────────────┘    │                                    │
                               │ NOT in scope: risk engine, margin, │
   ┌──────────────────────────┐│   IPO, mutual funds                │
   │ Kya chahiye (NFR):       │└────────────────────────────────────┘
   │  - FAST (microseconds)   │
   │  - CONSISTENT: ek share  │ <- DIL
   │    do logon ko na bike   │
   │  - FAIR: pehle aaya,     │
   │    pehle match           │
   │  - reliable + auditable  │
   └──────────────────────────┘
```

```
   Numbers:
     - ~50 lakh users
     - ~50 lakh orders / din      (market-open pe storm)
     - normal ~250 / sec, PEAK 10,000+ ka burst
     - price feed = CRORE reads / sec  (broadcast — sab dekh rahe hain)

   HAR NUMBER SE EK FAISLA (yahi bolna):
     matching microseconds me   ──►  order book RAM me (IN-MEMORY), DB me NAHI
     price feed crore reads     ──►  poll nahi — WebSocket PUSH + pub/sub broadcast
     paisa                      ──►  SQL + ACID (strong), NoSQL eventual nahi

   ★ CONTRAST jo bolna hai:
        speed wala + temporary   (order book)  ->  RAM
        paisa wala + permanent   (wallet/ledger) -> SQL / ACID
```

---

# MOVE 3 — BOXES BANAO (chhota banao, phir dikkat pe badhao)

```
   TU: "Sabse simple se shuru."

        USER ──► [ Order Service ] ──► [ DB ]  order pada hai
                                        koi milta-julta sell order dhoondho -> match

   TU: "Ab isme do log, do thread aur ek crash daal ke dekhte hain."
```

### dikkat 1 — "paanch thread ek hi stock ki book pe kaam kar rahe hain"

```
        Counter-A: "Suresh ke 10 share Ramesh ko de do"
        Counter-B: "wahi 10 share Mohan ko de do"
                    -> 20 bik gaye, jabki Suresh ke paas the 10
                    = DOUBLE-MATCH race = disaster

   FAISLA: ★ SINGLE-THREADED PER SYMBOL

        TCS      ──► queue ──► Thread T1   (sirf TCS ki book)
        INFY     ──► queue ──► Thread T2
        RELIANCE ──► queue ──► Thread T3

        har symbol ki apni EK queue -> ek-ek karke process -> bilkul serialized
        -> race ho hi nahi sakti -> koi LOCK ki zaroorat nahi -> in-memory, microseconds

   ★ LOCK kyun nahi? Exchange ki speed pe lock = slow + deadlock ka risk.
     Single thread me race possible hi nahi (line ek hi hai).
   ★ SCALE: alag symbol = alag thread (shard BY symbol)
   ★ ye RULE hai, if-condition nahi — har trading system me HONA HI HOGA, warna toot jaayega.
   ★ LINE: "Matching engine is single-threaded PER SYMBOL — orders serialized in one queue,
            no locks, deterministic and replayable. Scale horizontally BY symbol."
```

### dikkat 2 — "wallet me 50k hai, banda 30k-30k ke DO order daal deta hai"

```
        dono match ho gaye -> 60k chahiye, hai 50k  -> DOUBLE SPEND

   FAISLA: order pe paisa BLOCK karo (kaato mat)

        wallet total      50,000
        blocked (order)   30,000        <- hotel / petrol pump ke deposit jaisa
        available         20,000

        match hua   -> ab paisa KATA + share mile
        cancel hua  -> unblock
        pending hai -> blocked pada rahega

   TU: "Match se PEHLE paisa kaatunga nahi, BLOCK karunga — warna ek hi paise se
        do order match ho jaayenge."
```

### dikkat 3 — "settlement ke beech me crash ho gaya"

```
        steps: buyer ka debit  ->  seller ka credit  ->  shares move

        beech me crash -> 30k GAYAB

   FAISLA: SAARE step EK transaction me — ACID

        BEGIN
            buyer  -30,000     +10 shares
            seller +30,000     -10 shares
        COMMIT                  (sab ya kuch nahi -> ROLLBACK)

        (Spring me @Transactional yahi karta hai)

   ★ LEDGER double-entry: jitna ek se gaya, utna doosre ko mila
     -> paisa na BANTA hai na GAYAB hota, sirf MOVE karta -> total constant = AUDIT TRAIL
   ★ STRONG vs EVENTUAL: like-count eventual chal jaata, PAISA hamesha STRONG.
     LINE: "Money = strong consistency, never eventual."
```

### dikkat 4 — "paisa Wallet-DB me hai, share Portfolio-DB me — ek transaction kaise?"

```
        microservices: paisa = Wallet DB . shares = Portfolio DB . order = Order DB
        -> teen DB pe ek @Transactional chal hi nahi sakti

        Wallet debit ✓  ->  Portfolio crash ✗   = paisa gaya, share nahi mila

   FAISLA: SAGA (compensating transaction)

        bade transaction ko chhote LOCAL steps me todo;
        koi step fail -> pichhle ka ULTA chalao

            step 1  Wallet debit 30k       ✓
            step 2  Portfolio me share add ✗ FAIL
            -> COMPENSATE: Wallet me 30k REFUND -> system phir consistent

        ★ rollback DB nahi karta — HAMARA code compensating step likhta hai
        ★ TRAVEL ANALOGY: Flight ✓ Hotel ✗ -> flight cancel + refund

   ★ FARAK bolna:
        ACID (ek DB)      = INSTANT all-or-nothing
        SAGA (kai service)= code-driven undo, EVENTUALLY all-or-nothing
```

### dikkat 5 — "user ne 'Buy' do baar daba diya / network ne retry maar diya"

```
        risk: 60k ka order lag gaya, jabki banda ek hi chahta tha

   FAISLA: IDEMPOTENCY KEY

        har request ke saath ek UNIQUE key
        server yaad rakhta hai "ABC123 ho chuka" -> dobara aaye -> wahi result wapas
        (chahe 10 baar aaye, EK hi baar kata)

   (GPay pe double-click -> ek hi charge. BookMyShow me ek ticket, ek show.)
```

### dikkat 6 — "order book to RAM me hai — server crash hua to sab gayab"

```
        crash -> book + saare pending order GAYAB

   FAISLA: EVENT LOG / SEQUENCER (append-only, disk ya Kafka)

        har event PEHLE log me likho  ──►  PHIR book me lagao
                     │
                crash ho gaya
                     │
                log REPLAY karo ──► book bilkul waisa hi wapas

        (ye write-ahead log (WAL) ka hi tareeka hai)
        ★ CRICKET ANALOGY: scoreboard (RAM) gayab ho sakta hai,
          par scorer ka register (log) se sab wapas ban jaata hai

   ★ DO MUFT FAAYDE (ye bolna — interview me sona hai):
        1. AUDIT TRAIL — kaun, kya, kab; immutable -> regulator/JP ko yahi chahiye
        2. single-thread DETERMINISTIC hai -> replay karo to bilkul WAHI result

   ★ LOGGING vs AUDIT (farak bolna):
        logging = engineer ke debug ke liye (technical, kuch din, badal sakte)
        AUDIT   = regulator ke liye (business: kaun-kya-kab, saalon tak, IMMUTABLE)
        JP / BlackRock dono maangte; audit non-negotiable hai.
        Trading ka event-log itna pakka hota hai ki AUDIT ka kaam bhi de deta hai (ek cheez, do role).
```

### dikkat 7 — "lakhs log live price dekh rahe hain"

```
        POLLING (galat): har client baar-baar poochhe -> lakhon request/sec -> server dead

   FAISLA: WEBSOCKET PUSH + PUB/SUB fanout

        connection ek baar bane  ──►  price BADLE tab server khud PUSH kare

        ek price change ──► [ PUB/SUB ] ──► lakhon subscriber   (RADIO broadcast jaisa)

   ★ WhatsApp se FARAK (ye achha point hai):
        WhatsApp ka message KEEMTI hai -> store + guaranteed delivery
        price feed sirf LATEST maayne rakhti -> EPHEMERAL broadcast
        (missed ticks gaye to gaye; reconnect pe bas current price chahiye)
        push dono me hai, delivery-guarantee alag hai.
```

### dikkat 8 — "market-open pe AKELE TCS pe lakhon order aa rahe hain"

```
   ★ ye tricky hai: "shard by symbol" yahan kaam NAHI aayega —
     TCS to already ek hi symbol hai, ek hi thread pe.

   FAISLA (do, dono bolna):

     1. THREAD KO MAXIMIZE KARO, BOOK KO MAT TODO
        matching poori in-memory + no-lock hai -> ek thread lakhon/sec nigal leta hai
        (NASDAQ literally aise hi karta hai). "slow ho jaayega" ka dar zyada hai.

     2. BURST ko DURABLE QUEUE / EVENT-LOG absorb kare (backpressure)
        order pehle queue me append -> thread apni pace pe FIFO consume kare
        -> spike me kuch DROP nahi hota, bas thodi latency
        (arrival-rate aur process-rate DECOUPLE ho gaye)

   ★ BOOK ko do thread me kyun NAHI baant sakte:
        ek shared book -> DOUBLE-MATCH race wapas + price-time priority toot jaayegi + lock (slow)
        correctness = ek book = ek thread

   ★ LINE: "You don't parallelize a single order book — keep it single-threaded for correctness,
            optimize in-memory, and put a durable queue in front to absorb bursts.
            Scale BY symbol across threads, NEVER within a symbol."
```

### ab poora naksha — ek order ka safar

```
   [ User: "Buy 10 TCS @ 3000" ]
        ▼
   1. ORDER SERVICE        order lo, validate karo, + IDEMPOTENCY (duplicate na lage)
        ▼
   2. WALLET / LEDGER      paisa BLOCK karo (double-spend rok)
        ▼
   3. MATCHING ENGINE      buy <-> sell match (order book RAM me, price-time priority,
        ▼                   SINGLE THREAD per symbol)
   4. SETTLEMENT           paisa + shares ACTUAL move (double-entry, ATOMIC / SAGA)
        ▼
   5. PRICE FEED           live price sabko (WebSocket + pub/sub)

   + EVENT LOG (sequencer) : har event PEHLE log me, PHIR book me
                             -> crash recovery + audit trail

   HAR BOX KA KYUN:
     block-before-match  : double-spend rokta
     per-symbol matching : race-free (lock ke bina)
     atomic settlement   : paisa vanish nahi hota
     event log           : crash pe book wapas + regulator ke liye audit
     websocket+pubsub    : crore reads ko poll se bachaya
```

---

# MOVE 4 — BOLTE-BOLTE JODO (jo poocha jaaye, wahi kholo)

## ► "API kya hogi?"

```
   POST   /order  { stock, side, qty, price, type, idempotencyKey }  ->  orderId + OPEN
   DELETE /order/{id}       cancel
   PUT    /order/{id}       modify
   GET    /order/{id}       status
   GET    /portfolio
   WS     /prices?symbol=   live price PUSH (poll nahi)

   ★ senior wali do cheezein: idempotencyKey (double-click rokta) + price WebSocket se (polling nahi)
```

## ► "DB me kya, aur kaunsa DB?"

```
   TABLES : ORDER . WALLET . LEDGER . PORTFOLIO . TRADE
   ORDER BOOK : RAM me (DB me NAHI!)

   money / orders  ->  SQL + ACID     (strong, all-or-nothing, audit; NoSQL eventual NAHI)
   order book      ->  IN-MEMORY per symbol (microseconds)
   ledger          ->  append-only, immutable (audit)

   ★ CONTRAST ek line me: speed wala temporary (book) = RAM | paisa wala permanent = SQL/ACID
```

## ► "Order book kaise kaam karta hai?" (deep-dive ka dil)

```
   ORDER BOOK = do sorted lines:

     SELLERS (asks)          BUYERS (bids)
       3001  <- best (sasta)   2998  <- best (mehnga)
       3003                    2995
       3005                    2990

     sellers me SASTA upar . buyers me MEHNGA upar
     MATCH tab hota hai jab:  best-bid >= best-ask

   PRICE-TIME PRIORITY:
     (1) behtar PRICE pehle
     (2) same price -> TIME / FIFO (pehle aao, pehle pao)
     -> fair bhi, efficient bhi
```

## ► "Order types kaunse?" (sabzi-mandi wali samajh)

```
   LIMIT  = "TCS sirf 3000 ya usse BEHTAR. Mehnga hai? main rukunga."
              -> PRICE pakka, time flexible
              (BUY: 3000 ya neeche | SELL: 3000 ya upar)

   MARKET = "bhav chhodo, ABHI do, jo chal raha hai us par."
              -> TIME pakka, price flexible

   kab kya: sahi daam chahiye, jaldi nahi -> LIMIT | turant ghusna/nikalna hai -> MARKET
```

## ► "Partial fill hota hai?"

```
   "BUY 10 TCS @3000", par us price pe abhi sirf 6 available
        -> 6 turant MATCH (partial)
        -> baaki 4 book me PENDING pade rahenge
        -> naya seller @3000 aaya -> 4 bhi bhar gaye -> FULLY filled

   TEEN STATE (app me bhi yahi dikhte):
        FILLED (poore 10)  |  PARTIALLY FILLED (6 mile, 4 pending)  |  OPEN (kuch nahi mila)
   (Zerodha / Groww / NSE / BSE me yahi hota hai)
```

## ► "Kahan tootega / scale?"

```
   ★ RATTO MAT — order ka raasta chalao:

      order aaya
          │
          ├─► order svc   -> double click / retry     -> idempotency key
          ├─► wallet      -> double spend             -> BLOCK on order
          ├─► matching    -> race                     -> single thread PER SYMBOL
          │                  bahut symbols            -> shard BY symbol (alag thread)
          │                  EK hot symbol (TCS)      -> book mat todo; in-memory optimize
          │                                              + durable queue se burst absorb
          ├─► settlement  -> beech me crash           -> ATOMIC (ek DB) / SAGA (kai service)
          ├─► book (RAM)  -> server crash             -> EVENT LOG replay
          └─► price feed  -> crore reads              -> WebSocket push + pub/sub (ephemeral)
```

## ► WRAP (ek saans me)

```
   "Order Service (validate + idempotency) -> Wallet (paisa BLOCK) -> Matching Engine
    (single-threaded per symbol, in-memory order book, price-time priority) -> Settlement
    (atomic; kai service ho to SAGA) -> Price Feed (WebSocket + pub/sub).
    Peeche ek append-only EVENT LOG — crash pe replay, aur regulator ke liye audit trail.
    Data: paisa SQL/ACID, order book RAM me.
    Scale: shard BY symbol; ek hot symbol ko queue se absorb karo, book todo mat.
    Aage badhata to: stop-loss, circuit breakers, real-time risk checks, regulatory reporting."
```

---

## ★ POWER PHRASES (interview me seedha bolne layak)
```
- "Order book = bids (highest up) + asks (lowest up); match when best-bid >= best-ask; price-time priority."
- "Order pe paisa BLOCK (reserve), match pe debit — double-spend rok."
- "Ledger = double-entry: ek deta, ek leta, total constant = audit trail."
- "Settlement ATOMIC (ACID / @Transactional) — all-or-nothing, warna paisa vanish."
- "Money = STRONG consistency, never eventual."
- "Idempotency key -> duplicate/retry pe ek hi baar (GPay double-click -> ek charge)."
- "Price feed = WebSocket push + pub/sub fan-out; ephemeral (latest-only), not stored like WhatsApp."
- "Matching single-threaded PER SYMBOL — one queue, no locks, deterministic; scale BY symbol."
```

## TRAP BOX
```
TRAP 1: Money pe eventual consistency -> NO. Paisa = strong/ACID (eventual = like-counts only).
TRAP 2: Order pe turant debit -> match-se-pehle BLOCK karo; match pe debit.
TRAP 3: Settlement steps non-atomic -> ek transaction (all-or-nothing) warna paisa vanish.
TRAP 4: Idempotency bhulna -> network retry = duplicate charge. Idempotency-key must.
TRAP 5: Price feed ko WhatsApp jaisa store -> latest-only ephemeral; per-user tick-history bekaar.
TRAP 6: Polling for live price -> server dead+laggy. WebSocket push + pub/sub.
```

---

[← MASTER SHEET](../../00_MASTER_SHEET.md)
