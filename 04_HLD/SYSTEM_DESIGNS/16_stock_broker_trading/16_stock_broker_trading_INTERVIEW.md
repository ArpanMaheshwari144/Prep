# Stock Trading / Matching Engine — INTERVIEW (8-step framework)

> Finance GOLD (JP/GS). Detail/visual = 16_stock_broker_trading.md ; yeh = 8-step framework drive.
> Framework: 04_HLD/INTERVIEW_FRAMEWORK.md
> FLAVOR: News Aggregator read-heavy tha; yeh CONSISTENCY + LATENCY heavy (paisа + speed).

---

## STEP 1 — REQUIREMENTS clarify (solution pe mat kudo)
```
   FUNCTIONAL:  user BUY/SELL order de (stock,qty,price) -> system MATCH (buyer<->seller)
                -> paisа+shares move -> live price -> cancel/modify + status(filled/partial/open)
   NON-FUNCTIONAL:  FAST (microseconds, speed=paisа) | CONSISTENT (ek share do ko na bike) |
                    reliable (order/paisа lost na ho) | FAIR (pehle aaya pehle match)
   clarifying Qs:  orders/sec? limit(fixed price) ya market(jo bhi)? partial match (10 chahiye 6 mile)?
   KEY soch: PAISА + SPEED ka game -> consistency NON-NEGOTIABLE (strong, never eventual) + latency critical
```

## STEP 2 — SCALE / numbers
```
   users 50 lakh, peak active 5 lakh, orders 50 lakh/din (market-open 9:15 = storm)
   per-sec (trading ~6hr ~ 20,000 sec):
     normal = 50,00,000/20,000 = ~250 orders/sec
     PEAK (open/news) = 10-50x = ~10,000+ /sec burst
   price feed = har trader har stock ka live price -> CRORE reads/sec (broadcast)
   -> matching in-memory+fast | feed push-based(websocket) | money strong-consistent(ACID)
```

## STEP 3 — API design
```
   ORDER (banana/badalna):
     POST   /order {stock,side,qty,price,type:LIMIT/MARKET, idempotencyKey} -> orderId + status OPEN
     DELETE /order/{id}   -> cancel (blocked paisа unblock)
     PUT    /order/{id}   -> modify
   READ:
     GET /order/{id}      -> status (OPEN/PARTIAL/FILLED)
     GET /portfolio       -> shares + wallet
     WS  /prices?symbol=  -> live price STREAM (WebSocket, GET nahi)
   senior signal: (1) idempotencyKey -> "Buy" double-click/retry pe ek hi order (double-block rok)
                  (2) price = WebSocket push (poll nahi -> server dead)
```

## STEP 4 — HIGH-LEVEL boxes (order ka safar)
```
   [Buy 10 TCS @3000]
     -> 1. ORDER SERVICE   receive, validate, IDEMPOTENCY (dup na lage)
     -> 2. WALLET/LEDGER   paisа BLOCK (30k reserve) -> double-spend rok
     -> 3. MATCHING ENGINE buy<->sell (order book, price-time, SINGLE-THREAD per symbol)
     -> 4. SETTLEMENT      paisа+shares ACTUAL move (double-entry, ATOMIC)
     -> 5. PRICE FEED      live price sabko (WebSocket + pub/sub)
     + EVENT LOG           har event PEHLE append-only log -> crash recovery + audit

   har box KYUN:
     Order Svc  -> entry+validate+idempotency (retry pe ek order)
     Wallet     -> match SE PEHLE block (kata nahi) -> 50k se 2 order na ban jaaye
     Matching   -> dil: order book + price-time + per-symbol single-thread (race-free)
     Settlement -> match ke baad paisа+share ATOMIC move (double-entry ledger)
     Price Feed -> alag, broadcast-heavy -> matching ko slow na kare
     Event Log  -> append-only -> crash replay + audit trail
```

## STEP 5 — DATA MODEL + DB (KYUN bolo)
```
   ORDER:    orderId|userId|symbol|side|qty|filledQty|price|type|status|ts
   WALLET:   userId|total|blocked|available
   LEDGER:   entryId|userId|amount(+/-)|type|refOrderId|ts   (double-entry, APPEND-ONLY)
   PORTFOLIO:userId|symbol|qty        TRADE: tradeId|buyId|sellId|symbol|qty|price|ts
   ORDER BOOK: DB nahi! -> RAM mein, per-symbol (sorted bids/asks)

   DB choice (alag data, alag jagah):
     money/orders (wallet,ledger,orders) -> SQL + ACID (paisа=strong, all-or-nothing, audit; NoSQL eventual NAHI)
     order book (matching)               -> IN-MEMORY per-symbol (microseconds; disk slow) + event-log safety
     ledger                              -> append-only immutable (audit trail)
   CONTRAST: speed-temp(order book)=RAM | paisа-permanent(wallet/ledger)=SQL/ACID
```

## STEP 6 — DEEP DIVE (asli khel): matching race kaise roko?
```
   ORDER BOOK:  sellers(asks) sasta-upar | buyers(bids) mehnga-upar
                MATCH jab best-bid >= best-ask | price-time priority (best price; same->FIFO)
   PROBLEM (multi-thread): 2 thread same TCS book pe -> Suresh ke 10 share DONO ko de diye = 20 bik gaye = DOUBLE-MATCH
   OPTIONS:
     1. LOCK (har match tala) -> safe PAR slow + deadlock-risk. exchange-speed pe NAHI.
     2. SINGLE-THREAD PER SYMBOL (BEST) -> har stock 1 thread (TCS->T1, INFY->T2),
        ek symbol = ek queue = ek-ek process -> naturally serialized -> race ho hi nahi sakti -> no lock -> microseconds
   WINNER: single-thread per symbol. line hi EK -> do match saath ho hi nahi sakte.
     scale -> alag symbol = alag thread (shard by symbol, horizontal).
     RULE hai condition nahi -> har trading system mein, warna toot jaaye.
   line: "Matching single-threaded PER SYMBOL — serialized in one queue, no locks, deterministic+replayable. Scale BY symbol."
```

## STEP 7 — BOTTLENECK / scale
```
   1. order book RAM -> crash -> pending GAYAB -> EVENT LOG (append-only, pehle log phir apply) -> crash pe REPLAY (=WAL) + audit
   2. settlement beech crash -> 30k gayab -> EK transaction (ACID/@Transactional) all-or-nothing
   3. settlement KAI services (Wallet-DB,Portfolio-DB) -> ek @Transactional nahi -> SAGA (local steps + fail pe ULTA/compensate:
        Wallet debit 30k ✓ -> Portfolio add ✗ -> Wallet REFUND 30k -> consistent)
   4. price feed crore reads -> alag service + WebSocket push + pub/sub fan-out (broadcast)
   5. one symbol bahut load -> shard by symbol (machines pe baant)
   CORE: ACID (ek DB) = instant all-or-nothing | SAGA (kai service) = code-driven compensating undo, eventually
```

## STEP 8 — WRAP (ek saans + improve)
```
   Order Svc(validate+idempotency) -> Wallet(BLOCK) -> Matching(order book, SINGLE-THREAD per symbol) ->
   Settlement(ATOMIC double-entry) -> Price Feed(WebSocket+pubsub). Har event EVENT-LOG (crash replay+audit).
   DATA: money=SQL/ACID(strong) | order book=in-memory per-symbol(speed).
   DEEP: matching single-thread per symbol -> no lock, microseconds.
   SCALE: shard by symbol, event-log replay, ACID(ek DB)/SAGA(cross-service), feed pub/sub.
   IMPROVE: stop-loss orders, circuit breakers, real-time risk checks, regulatory reporting.
```

---
> CORE pattern (har design same): clarify -> scale -> API -> boxes -> data+DB(kyun) -> DEEP DIVE(options->choose)
> -> bottleneck -> wrap. META: think out loud, trade-off har choice, chup mat baitho.
> Trading twist: money=ACID/strong + matching=single-thread-per-symbol(race-free) + event-log(replay+audit) + feed=push.
