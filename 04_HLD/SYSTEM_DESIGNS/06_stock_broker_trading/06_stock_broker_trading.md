# Stock Broker / Trading Platform — 7-STEP RAIL (single spine, revise top→bottom)

> Finance interview GOLD (JP/GS): consistency, ACID, idempotency, ledger, audit — NOT FAANG-hyperscale.
> Bridge from Konovo fraud-domain. FLAVOR = CONSISTENCY + LATENCY heavy (paisa + speed).
> RAIL (04_HLD/HLD_APPROACH_DELIVERY.md). Merged into clean 7-step 7-Sep.
> Problem (1 line): user buy/sell order -> system MATCH kare -> paisa + shares CONSISTENT move -> + live price.

---

## STEP 1 — REQUIREMENTS

```
FUNCTIONAL:  BUY/SELL order (stock, qty, price, type) -> MATCH -> paisa+shares move -> live price -> cancel/status.
NON-FUNCTIONAL:  FAST (microseconds) | CONSISTENT (ek share do ko na bike) | reliable | FAIR (pehle-aaya-pehle-match).
CLARIFY:  orders/sec? limit ya market? partial match allowed?
★ KEY: PAISA + SPEED -> consistency NON-NEGOTIABLE (STRONG, never eventual) + latency critical.
```

---

## STEP 2 — ESTIMATE (scale / numbers)

```
   50 lakh users, ~50 lakh orders/din (market-open storm). per-sec ~250 normal, PEAK 10k+ burst.
   price feed = CRORE reads/sec (broadcast).
   -> matching IN-MEMORY+fast | feed WebSocket-push | money SQL/ACID.
```

---

## STEP 3 — API DESIGN

```
   POST   /order {stock, side, qty, price, type, idempotencyKey} -> orderId + OPEN
   DELETE /order/{id}   cancel  |  PUT /order/{id} modify  |  GET /order/{id} status
   GET    /portfolio    |  WS /prices?symbol=   (live price PUSH, poll nahi)
   ★ senior: idempotencyKey (double-click rok) + price = WebSocket push (not polling).
```

---

## STEP 4 — DATA MODEL + DB (KYUN)

```
   TABLES: ORDER / WALLET / LEDGER / PORTFOLIO / TRADE.   ORDER BOOK = RAM (DB nahi!).
   money/orders -> SQL + ACID (strong, all-or-nothing, audit; NoSQL eventual NAHI).
   order book   -> IN-MEMORY per-symbol (microseconds).   ledger -> append-only immutable (audit).
   ★ CONTRAST: speed-temp (book) = RAM  |  paisa-permanent = SQL/ACID.
```

---

## STEP 5 — HL BOXES (order ka safar)

```
   [User: "Buy 10 TCS @3000"]
        ▼
   1. ORDER SERVICE     receive, validate, + IDEMPOTENCY (dup na lage)
        ▼
   2. WALLET/LEDGER     paisa BLOCK (double-spend rok)
        ▼
   3. MATCHING ENGINE   buy<->sell match (order book, price-time, SINGLE-THREAD per symbol)
        ▼
   4. SETTLEMENT        paisa+shares ACTUAL move (double-entry, ATOMIC)
        ▼
   5. PRICE FEED        live price sab ko (WebSocket + pub/sub)
   + EVENT LOG (sequencer): crash recovery + audit trail (append PEHLE, book baad me)

   box KYUN: block-before-match (double-spend rok) | per-symbol matching (race-free) | atomic settlement (paisa na vanish).
```

---

## STEP 6 — DEEP DIVE: matching engine (the HEART — race kaise roko?)

```
ORDER BOOK (2 sorted lines):
   SELLERS (asks): 3001<-best(sasta) 3003 3005   |   BUYERS (bids): 2998<-best(mehnga) 2995 2990
   - Sellers SASTA upar, Buyers MEHNGA upar.  MATCH jab: best-bid >= best-ask.
PRICE-TIME PRIORITY: (1) best PRICE pehle (2) same-price -> TIME/FIFO (pehle-aao-pehle-pao). Fair+efficient.
```

```
★ SINGLE-THREADED PER SYMBOL (RULE, not condition):
   PROBLEM: 5 threads SAME stock ka SAME book -> Counter-A "Suresh ke 10 Ramesh ko" + Counter-B "wahi 10 Mohan ko"
            = 20 bik gaye (Suresh ke the 10) = DOUBLE-MATCH race = disaster.
   FIX: har STOCK ka EK matching thread. TCS->T1, INFY->T2, RELIANCE->T3. Har symbol -> apni EK queue -> ek-ek process (exact order).
        = naturally serialized -> koi race, koi LOCK nahi -> in-memory, microseconds.
   LOCK kyun nahi? exchange-speed pe lock = slow + deadlock. Single-thread = race ho hi nahi sakti (line ek hai).
   SCALE: alag symbol = alag thread (shard BY symbol).
   ★ RULE not if-condition: har trading system me HONA HI HOGA (jaise aasmaan neela). warna toot jaaye.
   LINE: "Matching engine is single-threaded PER SYMBOL — orders serialized in one queue, no locks,
          deterministic + replayable. Scale horizontally BY symbol."
```

```
★ ONE HOT SYMBOL (scale WITHIN a symbol — tricky): market-open pe AKELE TCS pe lakhon orders/sec.
   "shard by symbol" yahan NAHI chalega (TCS already ek symbol/ek thread).
   FIX (2): 1. thread MAXIMIZE karo, split MAT karo — matching pure in-memory + no-lock -> ek thread lakhon/sec nigal leta
               (NASDAQ literally aise). "slow" ka dar zyada.
            2. burst ko durable QUEUE/EVENT-LOG absorb kare (backpressure): orders pehle queue me append -> thread FIFO
               apni pace pe consume -> spike me kuch DROP nahi, bas thodi latency (arrival-rate != process-rate DECOUPLE).
   Book ko 2 thread me kyun NAHI: ek shared book -> DOUBLE-MATCH race wapas + price-time toot + lock (slow). Correctness = ek book = ek thread.
   LINE: "You don't parallelize a single order book — keep it single-threaded for correctness, optimize in-memory,
          put a durable queue in front to absorb bursts. Scale BY symbol across threads, NEVER within a symbol."
```

```
ORDER TYPES (sabzi-mandi):
   LIMIT  = "TCS sirf 3000 ya BEHTAR. Mehnga? rukunga." -> PRICE pakka, time flexible. (BUY: 3000-ya-neeche | SELL: 3000-ya-upar)
   MARKET = "bhav chhodo, ABHI do current price." -> TIME pakka, price flexible.
   kab: sahi-daam-jaldi-nahi -> LIMIT | turant-ghuso/niklo -> MARKET.

PARTIAL FILL: "BUY 10 TCS @3000", us price pe abhi sirf 6 -> 6 turant MATCH (partial) + baaki 4 book me PENDING ->
   naya seller @3000 -> 4 bhare -> FULLY filled.
   3 states (app me bhi): FILLED (poore 10) | PARTIALLY filled (6 mile, 4 pending) | OPEN/unfilled (kuch nahi). (Zerodha/Groww/NSE/BSE).
```

---

## STEP 7 — MONEY / RELIABILITY / SCALE (JP ka asli interest)

```
MONEY SIDE:
   a. BLOCK on order (double-spend rok): wallet 50k, order 30k -> match-se-pehle paisa KATA nahi, BLOCK (hotel/petrol-deposit).
        Total 50k | Blocked 30k | Available 20k. warna 50k se 2 order -> dono match -> 60k chahiye = double-spend.
        match->kata+shares | cancel->unblock | pending->blocked.
   b. SETTLEMENT + LEDGER (double-entry): buyer -30k +10sh | seller +30k -10sh. jitna ek se gaya utna doosre ko -> na BANTA na GAYAB, sirf MOVE.
        double-entry -> total constant + har entry traceable = AUDIT TRAIL.
   c. ATOMICITY (ACID — THE point): steps [buyer-debit, seller-credit, shares-move]. beech crash -> 30k GAYAB.
        FIX: saare steps EK transaction -> sab COMMIT ya sab ROLLBACK (Spring @Transactional = yehi).
   d. STRONG vs EVENTUAL: like-count -> eventual OK | PAISA -> STRONG (har waqt exact). LINE: "money = strong consistency, no eventual".

IDEMPOTENCY (duplicate/retry safe): "Pay" -> cut -> timeout -> re-click -> risk 60k.
   FIX: har request UNIQUE idempotency-key; server "ABC123 done" yaad -> dobara -> same result wapas (chahe 10 baar, EK baar kata).
   (GPay double-click -> ek charge; BookMyShow ek-ticket-ek-show.)

PRICE FEED (real-time): POLLING galat (lakhon req/sec -> server dead). WEBSOCKET sahi (conn ek baar -> server PRICE-CHANGE pe PUSH).
   PUB/SUB FAN-OUT: ek change -> broadcast -> lakhon subscribers (RADIO). 
   WhatsApp se FARAK: WhatsApp msg keemti -> STORE+guaranteed. price-feed sirf LATEST -> EPHEMERAL broadcast (missed ticks gaye,
        reconnect pe bas current). push same, delivery-guarantee alag.

CROSS-SERVICE SETTLEMENT -> SAGA: @Transactional sirf EK DB pe. microservices me paisa=Wallet-DB, shares=Portfolio-DB, order=Order-DB
   -> 3 DB pe ek @Transactional NAHI chalti. Wallet-debit ✓ -> Portfolio-crash ✗ = paisa-gaya-share-nahi.
   SAGA: bade txn ko chhote LOCAL steps me todo; koi step fail -> pichle ka ULTA (COMPENSATING):
        step1 Wallet-debit 30k ✓ | step2 Portfolio-add ✗ FAIL -> compensate: Wallet REFUND 30k -> consistent.
        (rollback DB nahi karta -> HAMARA code compensating step likhta.) TRAVEL-analogy: Flight✓ Hotel✗ -> Flight-cancel+refund.
   ACID(ek DB)=INSTANT all-or-nothing | SAGA(kai service)=code-driven undo, EVENTUALLY all-or-nothing.

EVENT LOG / SEQUENCER (crash recovery + audit): order book RAM me -> crash -> book+pending GAYAB.
   FIX = append-only log (disk/Kafka): har event PEHLE log me likho, PHIR book me lagao. crash -> log REPLAY -> book waisa wapas.
   (= write-ahead-log WAL. cricket: scoreboard=RAM gayab | scorer-register=log -> wapas.)
   2 muft faayde: (1) AUDIT TRAIL (who-what-when, immutable) = regulator/JP sona (2) single-thread DETERMINISTIC -> replay = same result.
   LOGGING vs AUDIT: logging=engineer-debug (technical, thode-din, badal-sakte) | AUDIT=regulator-proof (business who-what-when, saal-saal, IMMUTABLE).
        JP/BlackRock dono; audit NON-NEGOTIABLE. Trading event-log itna pakka ki AUDIT ka kaam bhi de (ek cheez, dono role).

SCALE: many symbols -> shard BY symbol (alag thread) | one hot symbol -> can't split book -> optimize in-memory + durable queue absorb.

WRAP: Order(validate+idempotency)->Wallet(BLOCK)->Matching(single-thread/symbol)->Settlement(ATOMIC)->Feed(ws+pubsub) | EventLog(replay+audit).
      DATA: money=SQL/ACID | book=in-memory. DEEP: single-thread per symbol. SCALE: shard-by-symbol, event-log, ACID/SAGA, pubsub.
      IMPROVE: stop-loss, circuit-breakers, real-time risk-checks, regulatory reporting.
```

---

## ★ POWER PHRASES (interview)
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

[← HLD README](../README.md)
