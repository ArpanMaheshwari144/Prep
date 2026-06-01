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
