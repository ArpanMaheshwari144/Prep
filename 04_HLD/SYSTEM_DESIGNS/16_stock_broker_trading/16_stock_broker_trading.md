# Stock Broker / Trading Platform — Visual System Design

Finance interview GOLD (JP / GS). Hits the finance-flavor depth they test: consistency, ACID, idempotency, ledger, audit trail — NOT FAANG-hyperscale. Natural bridge from Konovo fraud-detection domain.

---

## 1. Problem (1 line)

```
   User buy/sell order de → system MATCH kare → paisa + shares move
   ho → sab CONSISTENT rahe (paisa idhar-udhar na ho) → + live price
```

---

## 2. Components Overview (order ka safar)

```
   [User: "Buy 10 TCS @ 3000"]
        │
        ▼
   ┌──────────────────┐
   │ 1. ORDER SERVICE │  receive, validate, + IDEMPOTENCY (dup na lage)
   └────────┬─────────┘
        ▼
   ┌──────────────────┐
   │ 2. WALLET/LEDGER │  paisa BLOCK (double-spend rok)
   └────────┬─────────┘
        ▼
   ┌──────────────────┐
   │ 3. MATCHING ENG. │  buy ↔ sell match (order book, price-time)
   └────────┬─────────┘
        ▼
   ┌──────────────────┐
   │ 4. SETTLEMENT    │  paisa+shares ACTUAL move (double-entry, ATOMIC)
   └────────┬─────────┘
        ▼
   ┌──────────────────┐
   │ 5. PRICE FEED    │  live price sab ko (WebSocket + pub/sub)
   └──────────────────┘
```

---

## 3. Matching Engine (the heart)

### Order Book — do sorted line

```
   SELLERS (asks)        BUYERS (bids)
   3001  ← best (sasta)  2998  ← best (mehnga)
   3003                  2995
   3005                  2990

   - Sellers: SASTA upar (buyer ko sasta chahiye)
   - Buyers:  MEHNGA upar (seller ko zyada chahiye)
   - MATCH jab: best bid >= best ask (buyer-max >= seller-min)
```

Core rule (1-on-1): DEAL hota jab Buyer-MAX price >= Seller-MIN price.
Real-life: seller sabse zyada dene wale ke paas jaata, buyer sabse saste ke paas — market automate karta.

### Price-Time Priority

```
   1. PRICE priority → best price pehle
   2. Same price?    → TIME priority → jo PEHLE aaya (FIFO)
   = "pehle aao pehle pao" (same price pe). Fair + efficient.
```

### Single-Threaded PER SYMBOL (deep-dive — RULE, not if-condition)

```
   Soch: ek dukaan, ek BILLING COUNTER. Sab EK line mein → ek-ek bill → koi gadbad nahi.

   Ab 5 counters (5 threads) par SAME stock ka SAME order book:
      Counter-A: "Suresh ke 10 share Ramesh ko de diye"
      Counter-B: (usi waqt) "Suresh ke 10 share Mohan ko de diye"
      = Suresh ke paas the sirf 10 → DONO ko de diye = 20 bik gaye
      = DOUBLE-MATCH / race condition = paisa-share disaster
```

```
   FIX: har STOCK ka EK hi matching thread.
      TCS → Thread-1   INFY → Thread-2   RELIANCE → Thread-3
      har symbol → apni EK queue → ek-ek process (exact order)
      = naturally serialized → koi race, koi lock nahi → in-memory, microseconds
```

```
   Lock kyun nahi? → exchange-speed pe lock = slow + deadlock risk.
                     Single-thread = race ho hi nahi sakti (line hi ek hai).
   Scale kaise?    → alag symbol = alag thread (symbol ke hisaab se baant do).
```

```
   YEH RULE HAI, CONDITION NAHI:
      if-condition  = "kabhi haan kabhi naa" (situation pe depend)
      RULE/invariant = "hamesha, bina exception" (jaise aasmaan = neela)
   Single-thread per symbol = har trading system mein HONA HI HOGA — warna toot jayega.

   Interview line:
   "Matching engine is single-threaded PER SYMBOL — orders serialized in one queue,
    no locks, deterministic + replayable. Scale horizontally BY symbol."
```

### Order Types — LIMIT vs MARKET (deep-dive)

```
   Soch: sabzi mandi.
   LIMIT  = "TCS sirf 3000 ya BEHTAR pe loonga. Mehnga? Rukunga." → PRICE pakka, time flexible
   MARKET = "Bhav chhodo, ABHI do jo current price hai."          → TIME pakka, price flexible
```

```
   BUY limit  → "3000 ya usse NEECHE" (buyer ko sasta chahiye)
   SELL limit → "3000 ya usse UPAR"   (seller ko mehnga chahiye)
   = dono "ya usse BEHTAR" — behtar = buyer ke liye sasta, seller ke liye mehnga.

   Kab: sahi daam chahiye, jaldi nahi → LIMIT | turant ghuso/niklo → MARKET.
```

### Partial Fill (deep-dive — order ek baar mein poora na bhare to?)

```
   Soch: dukaan pe "10 packet @12" maange, us daam pe sirf 6 the.
   → 6 LE LIYE (jitne mile) + baaki 4 ka parcha chhoda ("aur aaye to rakhna, wait karta hoon").

   Trading: "BUY 10 TCS @3000", us price pe abhi sirf 6 share.
      → 6 turant MATCH (partial fill)
      → baaki 4 order book mein PENDING
      → naye seller @3000 aaye → 4 bhare → ab FULLY filled
```

```
   Order ki 3 haalat (app mein bhi yahi dikhta):
   - FILLED            → poore 10 mil gaye        (Executed)
   - PARTIALLY filled  → kuch (6) mile, baaki (4) pending  (Partially executed)
   - OPEN / unfilled   → abhi tak kuch nahi       (Open)
   = real hai: Zerodha/Groww/NSE/BSE sab mein yahi.
```

---

## 4. Money Side (JP ka asli interest)

### 4a. Block on order (double-spend rok)

```
   Wallet ₹50,000. Order "10 TCS @ 3000" = ₹30,000.
   Match nahi hua abhi → paisa KATA nahi, BLOCK hua:
     Total 50,000 | Blocked 30,000 | Available 20,000
   Analogy: hotel/petrol deposit (HOLD, kata nahi).

   Kyun: warna usi 50k se 2 order → dono match → 60k chahiye = double-spend.
   Order ke baad: match→kata+shares | cancel→unblock | pending→blocked rehta
```

### 4b. Settlement + Ledger (double-entry)

```
   Match ke baad actual move:
   ┌──────────────────────────────────────────┐
   │  Buyer khaata:   -30,000   +10 shares     │
   │  Seller khaata:  +30,000   -10 shares     │
   └──────────────────────────────────────────┘
   = jitna ek se gaya utna doosre ko aaya
   = paisa/share na BANTA na GAYAB — sirf MOVE
   Kyun double-entry: total constant + har entry traceable = AUDIT TRAIL
```

### 4c. Atomicity (ACID — THE point)

```
   Settlement steps: 1.buyer debit  2.seller credit  3.shares move
   Beech mein crash? → step1 hua, step2 nahi → ₹30k GAYAB = disaster

   FIX: saare steps EK transaction → sab COMMIT ya sab ROLLBACK
   = all-or-nothing (tera Spring @Transactional = exactly yehi)
```

### 4d. Strong vs Eventual consistency

```
   Like-count → eventual OK (2 sec baad sahi, chalega)
   PAISA      → STRONG zaroori (har waqt exact, "eventually" nahi)
   = interview line: "money = strong consistency, no eventual"
```

---

## 5. Idempotency (duplicate/retry safe)

```
   PROBLEM: "Pay" click → paisa cut → response timeout → tu RE-click
            → risk: do baar cut (₹60k)

   FIX: har request ek UNIQUE idempotency-key (e.g. ABC123)
        server yaad rakhta "ABC123 done" → dobara aaye → naya process
        NAHI, same result wapas → chahe 10 baar bhejo, EK baar kata

   Analogy: GPay double-click par paisa ek hi baar; BookMyShow ek
            ticket = ek show (used → reuse nahi)
   Definition: same op kitni baar bhi chale → result SAME (ek-baar jaisa)
```

---

## 6. Price Feed (real-time market data)

```
   POLLING (galat): har user har sec poochta → lakhon req/sec = server dead + laggy

   WEBSOCKET (sahi): connection ek baar khula → server PRICE CHANGE pe PUSH kare
                     = real-time, baar-baar request nahi

   PUB/SUB FAN-OUT: ek price change → broadcast → lakhon subscribers ek saath
                    (RADIO analogy: ek broadcast, sab radios sunte)
```

```
   WhatsApp se FARAK (design decision):
   - WhatsApp: har message keemti → STORE + guaranteed delivery (offline bhi milta)
   - Price feed: sirf LATEST matters → ephemeral broadcast (missed ticks gaye,
                 reconnect pe bas CURRENT price) → per-user history nahi chahiye
   = push mechanism same; delivery-guarantee alag
```

---

## 6b. Settlement across services — SAGA (deep-dive)

```
   Settlement ATOMIC tha (@Transactional, ACID) — par woh sirf jab sab EK database mein ho.

   Microservices mein:
      paisa  → Wallet Service (apna DB) | shares → Portfolio Service (apna DB) | order → Order Service (apna DB)
   = 3 alag service, 3 alag DB. Ek @Transactional 3 alag DB pe NAHI chal sakti.

   PROBLEM: Wallet ne paisa kaata ✓ → Portfolio share-add pe crash ✗
            = paisa gaya, share nahi mila = inconsistent. All-or-nothing toot gaya.
```

```
   Soch: TRAVEL booking — Flight + Hotel + Cab (3 alag company, koi ek transaction nahi).
   Flight book ✓ → Hotel full ✗ → Flight CANCEL + refund (taaki paisa na phase).
   = step-by-step; aage fail to peeche wale ko ULTA karke undo.
```

```
   SAGA pattern:
   - Bade transaction ko chhote LOCAL steps mein todo (har service apna step, apne DB pe)
   - Koi step fail → pichle steps ka ULTA (COMPENSATING action):
       Step1: Wallet debit 30k     ✓
       Step2: Portfolio add shares  ✗ FAIL
       Compensate: Wallet REFUND 30k (step1 ka ulta) → wapas consistent
   - Rollback DB nahi karta — HAMARA CODE compensating step likhta hai.
```

```
   ACID vs SAGA:
   EK database     → @Transactional → INSTANT all-or-nothing (DB khud rollback, koi aadha-state nahi dikhta)
   KAI services/DB → SAGA           → code-driven undo, "EVENTUALLY" all-or-nothing
                                      (chhota window jahan state aadhi → phir compensate → consistent)

   Line: "Saga = same all-or-nothing GOAL, code-driven compensating undo, eventually consistent.
          Use jab transaction services/DBs ke beech faili ho (ek @Transactional kaafi nahi)."
```

---

## 7. Event Log / Sequencer (deep-dive — crash recovery + audit)

```
   PROBLEM: order book RAM mein hai (single-thread, fast). Server crash/restart →
            RAM saaf → poora order book + pending orders GAYAB. Disaster.
```

```
   Soch: CRICKET match.
   - Live scoreboard (screen) = RAM. Bijli gayi → score gayab.
   - Scorer ka REGISTER (ball-by-ball) = log. Bijli aayi → register se poora score WAPAS.
```

```
   FIX = EVENT LOG (sequencer):
   - Har event PEHLE append-only log mein likho (disk/Kafka), PHIR order book mein lagao:
       "Order#1 aaya", "Order#2 aaya", "Match hua", "Order#3 cancel"...  (sequence mein)
   - Crash? → log shuru se REPLAY → har event dobara apply → order book bilkul waisa wapas.
   = yeh wahi "write-ahead log (WAL)" soch hai jo databases mein hoti.

   2 muft faayde:
   1. AUDIT TRAIL (who-what-when, immutable) = regulator/JP ke liye sona
   2. Single-thread DETERMINISTIC → replay se HAR baar SAME result.
```

### Logging vs Audit (alag cheez — confuse mat karna)

```
                  LOGGING                      AUDIT (audit trail)
   Kiske liye:    engineer (debug/monitor)     regulator/business (proof)
   Kya:           technical (error/latency)    business action (who-what-when)
   Kitne din:     thode (rotate/delete)        saal-saal (legal majboori)
   Badal sakte:   haan (freely)                NAHI (immutable/tamper-proof)

   logging = "kya chal raha bataata (debug)"   |  audit = "kisne kya kiya (proof, permanent)"
   JP/BlackRock: DONO rakhte; par audit NON-NEGOTIABLE (regulator — "woh gaya to company band").
   Trading ka event log itna pakka ki woh AUDIT ka kaam bhi de deta (ek cheez, dono role).
```

---

## POWER PHRASES (interview)

- "Order book = bids (highest up) + asks (lowest up); match when best-bid >= best-ask; price-time priority."
- "Order pe paisa BLOCK (reserve), match pe debit — double-spend rok."
- "Ledger = double-entry: ek deta, ek leta, total constant = audit trail."
- "Settlement ATOMIC (ACID / @Transactional) — all-or-nothing, warna paisa vanish."
- "Money = STRONG consistency, never eventual."
- "Idempotency key → duplicate/retry pe ek hi baar process (GPay double-click → ek charge)."
- "Price feed = WebSocket push + pub/sub fan-out; ephemeral (latest-only), not stored like WhatsApp."

---

## TRAP BOX

```
┌─────────────────────────────────────────────────────────┐
│ TRAP 1: Money pe eventual consistency                    │
│   → NO. Paisa = strong/ACID. Eventual = like-counts only.│
│                                                          │
│ TRAP 2: Order pe turant debit                            │
│   → Match se pehle BLOCK karo (kata nahi). Match pe debit.│
│                                                          │
│ TRAP 3: Settlement steps alag-alag (non-atomic)          │
│   → Ek transaction: all-or-nothing, warna paisa vanish.  │
│                                                          │
│ TRAP 4: Idempotency bhulna                               │
│   → Network retry = duplicate charge. Idempotency-key must.│
│                                                          │
│ TRAP 5: Price feed ko WhatsApp jaisa store karna          │
│   → Latest-only ephemeral; per-user tick history bekaar. │
│                                                          │
│ TRAP 6: Polling for live price                           │
│   → Server dead + laggy. WebSocket push + pub/sub.       │
└─────────────────────────────────────────────────────────┘
```

---

# 8-STEP INTERVIEW FRAMEWORK DRIVE

> Upar ka detail/visual = depth. Yeh = 8-step framework piro ke (jaise interview mein bolega).
> Framework: 04_HLD/INTERVIEW_FRAMEWORK.md. FLAVOR: CONSISTENCY + LATENCY heavy (paisа + speed).

## STEP 1 — REQUIREMENTS clarify
```
   FUNCTIONAL:  BUY/SELL order (stock,qty,price) -> MATCH -> paisа+shares move -> live price -> cancel/status
   NON-FUNCTIONAL:  FAST (microseconds) | CONSISTENT (ek share do ko na bike) | reliable | FAIR (pehle aaya pehle match)
   Qs: orders/sec? limit ya market? partial match?
   KEY: PAISА+SPEED -> consistency NON-NEGOTIABLE (strong, never eventual) + latency critical
```

## STEP 2 — SCALE / numbers
```
   50 lakh users, orders 50 lakh/din (market-open storm). per-sec ~250 normal, PEAK 10k+ burst.
   price feed = CRORE reads/sec (broadcast).
   -> matching in-memory+fast | feed websocket-push | money SQL/ACID
```

## STEP 3 — API design
```
   POST /order {stock,side,qty,price,type, idempotencyKey} -> orderId + OPEN
   DELETE /order/{id} cancel | PUT modify | GET /order/{id} status | GET /portfolio | WS /prices?symbol=
   senior: idempotencyKey (double-click rok) + price=WebSocket push (poll nahi)
```

## STEP 4 — HIGH-LEVEL boxes (order ka safar)
```
   Order Svc(validate+IDEMPOTENCY) -> Wallet(paisа BLOCK) -> Matching(order book, SINGLE-THREAD per symbol)
   -> Settlement(ATOMIC double-entry) -> Price Feed(WebSocket+pubsub) | + EVENT LOG (crash recovery + audit)
   KYUN: block-before-match (double-spend rok) | per-symbol matching (race-free) | atomic settlement (paisа na vanish)
```

## STEP 5 — DATA MODEL + DB (KYUN)
```
   ORDER/WALLET/LEDGER/PORTFOLIO/TRADE tables. ORDER BOOK = RAM (DB nahi).
   money/orders -> SQL+ACID (strong, all-or-nothing, audit; NoSQL eventual NAHI)
   order book -> IN-MEMORY per-symbol (microseconds) | ledger -> append-only immutable (audit)
   CONTRAST: speed-temp(book)=RAM | paisа-permanent=SQL/ACID
```

## STEP 6 — DEEP DIVE: matching race kaise roko?
```
   order book: best-bid>=best-ask match; price-time priority (same price->FIFO)
   PROBLEM: 2 thread same book -> Suresh ke 10 share DONO ko -> 20 bik gaye = DOUBLE-MATCH
   OPTIONS: 1.LOCK (slow+deadlock, NAHI)  2.SINGLE-THREAD PER SYMBOL (BEST: ek symbol=ek queue=serialized->race-free->no lock->microseconds)
   WINNER: single-thread per symbol. scale = shard by symbol. RULE hai (warna toot jaaye).
   line: "single-threaded PER SYMBOL — one queue, no locks, deterministic+replayable, scale BY symbol"
```

## STEP 7 — BOTTLENECK / scale
```
   order book RAM crash -> EVENT LOG replay (=WAL) + audit | settlement crash -> EK transaction (ACID/@Transactional)
   cross-service settlement -> SAGA (local steps + fail pe compensate/ulta) | feed crore reads -> WebSocket+pubsub
   one symbol load -> shard by symbol
   CORE: ACID(ek DB)=instant all-or-nothing | SAGA(kai service)=code-driven compensating undo, eventually
```

## STEP 8 — WRAP
```
   Order(validate+idempotency)->Wallet(BLOCK)->Matching(single-thread/symbol)->Settlement(ATOMIC)->Feed(ws+pubsub) | EventLog(replay+audit)
   DATA: money=SQL/ACID | book=in-memory. DEEP: single-thread per symbol. SCALE: shard by symbol, event-log, ACID/SAGA, pubsub.
   IMPROVE: stop-loss, circuit breakers, real-time risk checks, regulatory reporting.
```

> Trading twist: money=ACID/strong + matching=single-thread-per-symbol(race-free) + event-log(replay+audit) + feed=push.
