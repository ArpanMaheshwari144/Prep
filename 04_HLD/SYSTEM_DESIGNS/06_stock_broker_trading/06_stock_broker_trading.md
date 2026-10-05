# Stock Broker / Trading Platform

> User BUY / SELL order de -> system MATCH kare -> paisa + share sahi move ho -> live price dikhe.
> Is design ka dil: **paisa = strong consistency** + **matching = microseconds**.

---

## TASVEER (ByteByteGo / Alex Xu · CC BY-NC-ND 4.0)

![Design Stock Exchange](https://assets.bytebytego.com/diagrams/0344-stock-exchange.png)
Source: [Design Stock Exchange](https://bytebytego.com/guides/design-stock-exchange/)

![Low Latency Stock Exchange](https://assets.bytebytego.com/diagrams/0265-low-latency-stock-exchange.jpg)
Source: [Low Latency Stock Exchange](https://bytebytego.com/guides/low-latency-stock-exchange/)

---

## SHURU — poocho + numbers

```
POOCHO:  "Trading bada hai — matching, price feed, risk, settlement. Kis pe focus karein?"
         -> order MATCHING + paise / share ka SETTLEMENT. Risk / margin scope me nahi.
         LIMIT + MARKET dono? · PARTIAL fill chalega?

FR:      buy / sell · order match · paisa + share move · live price · cancel / status
NFR:     paisa CONSISTENT (strong, kabhi eventual nahi) · FAST · FAIR (pehle aaya pehle) · audit

NUMBERS: ~50 lakh users · ~50 lakh orders / din · market khulte hi BURST · price dekhne wale crore
         -> matching RAM me · price feed PUSH · paisa SQL + ACID
```

---

## DABBA 0 — sabse simple

```
  USER
    │
    ▼
  [ Order Service ]
    │
    ▼
  [ DB ]          order rakho, milta-julta sell dhoondho -> match
```

---

## DIKKAT 1 — do order ek hi stock pe ek saath (RACE)

```
DIKKAT:   Seller ke 10 share. Ramesh aur Mohan dono ek saath BUY 10.
          do thread dono ko de dein -> 20 bik gaye, the 10  = DOUBLE MATCH

SOLUTION: har SYMBOL ki EK queue + EK thread (single-threaded per symbol)
          TCS -> T1 · INFY -> T2 · ek ke baad ek -> race ho hi nahi sakti -> lock nahi chahiye
          order book RAM me (microseconds)
```
```
  USER
    │
    ▼
  [ Order Service ]
    │
    ▼
  [ Queue per symbol ]      ★ NAYA   TCS | INFY | RELIANCE ...
    │
    ▼
  [ Matching Engine ]       ★ NAYA   1 thread / symbol · order book RAM me
    │
    ▼
  [ DB ]
```
```
POOCHEGA: "Two users order the same stock at the same time — what happens?"
DHYAAN:   "DB ACID rok dega" NAHI — matching DB me hai hi nahi, aur ACID akela check-phir-write race nahi rokta
BOL:      "Orders for one symbol go into one queue handled by one thread, so they're matched one after
           the other, never in parallel — no lock needed."
```

---

## DIKKAT 2 — wallet me 50k, banda 30k-30k ke DO order daal de (DOUBLE SPEND)

```
DIKKAT:   dono match -> 60k chahiye, hai 50k

SOLUTION: order lagte hi paisa BLOCK karo (kaato nahi) — hotel deposit jaisa
          total 50k · blocked 30k · available 20k -> doosra 30k ka order REJECT
          match -> ab kato · cancel -> unblock
          DB me: UPDATE wallet SET blocked = blocked + x WHERE available >= x   (0 row = reject)
```
```
  USER
    │
    ▼
  [ Order Service ]
    │
    ▼
  [ Wallet ]                ★ NAYA   paisa BLOCK
    │
    ▼
  [ Queue per symbol ]
    │
    ▼
  [ Matching Engine ]
    │
    ▼
  [ DB ]
```

---

## DIKKAT 3 — settlement ke beech crash

```
DIKKAT:   buyer -30k ho gaya, seller +30k hone se pehle crash -> 30k GAYAB

SOLUTION: saare step EK transaction me (ACID) — sab ya kuch nahi -> crash = ROLLBACK
            BEGIN  buyer -30k +10 share · seller +30k -10 share  COMMIT
          LEDGER double-entry: jitna ek se gaya utna doosre ko mila -> total same = audit
```
```
  USER
    │
    ▼
  [ Order Service ]
    │
    ▼
  [ Wallet ]
    │
    ▼
  [ Queue per symbol ]
    │
    ▼
  [ Matching Engine ]
    │
    ▼
  [ Settlement ]            ★ NAYA   ek transaction · double-entry ledger
    │
    ▼
  [ DB ]
```

---

## DIKKAT 4 — paisa Wallet DB me, share Portfolio DB me (alag DB)

```
DIKKAT:   do DB pe ek transaction chal hi nahi sakti
          wallet debit ✓ -> portfolio crash ✗ = paisa gaya, share nahi

SOLUTION: SAGA — bade kaam ko chhote LOCAL step me todo; koi step fail -> pichhle ka ULTA step chalao
          wallet -30k ✓ -> portfolio +10 ✗ -> COMPENSATE: wallet +30k wapas
          (flight ✓ hotel ✗ -> flight cancel + refund)
          ACID = ek DB, turant · SAGA = kai service, code se undo
```
```
  USER
    │
    ▼
  [ Order Service ]
    │
    ▼
  [ Wallet ]
    │
    ▼
  [ Queue per symbol ]
    │
    ▼
  [ Matching Engine ]
    │
    ▼
  [ Settlement ]            ab SAGA (kai DB)
    │
    ├──► [ Wallet DB ]      ★ NAYA
    └──► [ Portfolio DB ]   ★ NAYA
```

---

## DIKKAT 5 — user ne BUY do baar daba diya / network ne retry maara

```
DIKKAT:   ek order do baar lag gaya -> do baar paisa

SOLUTION: IDEMPOTENCY KEY — har request ke saath ek unique key
          server yaad rakhta "ABC123 ho chuka" -> dobara aaya -> wahi purana result, naya order nahi
```
```
  USER                      ★ request + idempotencyKey
    │
    ▼
  [ Order Service ]         ★ NAYA KAAM: key dekho, duplicate = purana jawab
    │
    ▼
  [ Wallet ]
    │
    ▼
  [ Queue per symbol ]
    │
    ▼
  [ Matching Engine ]
    │
    ▼
  [ Settlement ]
    │
    ├──► [ Wallet DB ]
    └──► [ Portfolio DB ]
```

---

## DIKKAT 6 — order book RAM me hai, server crash = sab gayab

```
DIKKAT:   matching server gira -> book + saare pending order gayab

SOLUTION: EVENT LOG / SEQUENCER (append-only, disk / Kafka)
          har order PEHLE log me (sequence number ke saath) -> PHIR book me
          crash -> naya server log REPLAY kare -> book bilkul waisi (1 thread = same result)
          cricket: scoreboard (RAM) gaya, scorer ka register (log) se sab wapas
          bonus: yahi log = AUDIT TRAIL (kaun, kya, kab — regulator ko chahiye)
```
```
  USER
    │
    ▼
  [ Order Service ]
    │
    ▼
  [ Wallet ]
    │
    ▼
  [ Event Log / Sequencer ] ★ NAYA   (queue ki jagah) seq no. · key = symbol · replay
    │
    ▼
  [ Matching Engine ]
    │
    ▼
  [ Settlement ]
    │
    ├──► [ Wallet DB ]
    └──► [ Portfolio DB ]
```
```
POOCHEGA: "How do you keep orders in the right sequence?"
DHYAAN:   TIMESTAMP nahi — do server ki ghadi alag, do order same millisecond
BOL:      "A sequencer stamps every order with an increasing sequence number and writes it to the log,
           keyed by symbol so one stock's orders stay in one partition, in order."

POOCHEGA: "What if the server crashes in the middle?"
DHYAAN:   DO jagah: (1) matching (RAM) -> log replay   (2) settlement (DB) -> rollback / SAGA
BOL:      "If the matching engine dies, I rebuild the book by replaying the event log. If settlement
           dies midway, a single-DB transaction rolls back; across services a saga compensates."
```

---

## DIKKAT 7 — lakhon log live price dekh rahe

```
DIKKAT:   har client baar-baar poochhe (polling) -> lakhon request / sec -> server dead

SOLUTION: WEBSOCKET PUSH + PUB/SUB — connection ek baar, price badle tab server khud bheje
          price feed = sirf LATEST chahiye (WhatsApp jaisa store nahi; reconnect pe current price)
```
```
  USER
    │
    ▼
  [ Order Service ]
    │
    ▼
  [ Wallet ]
    │
    ▼
  [ Event Log / Sequencer ]
    │
    ▼
  [ Matching Engine ] ──► [ Pub/Sub ] ──► [ WebSocket servers ] ──► lakhon USER    ★ NAYA
    │
    ▼
  [ Settlement ]
    │
    ├──► [ Wallet DB ]
    └──► [ Portfolio DB ]
```

---

## DIKKAT 8 — market khulte hi AKELE TCS pe lakhon order

```
DIKKAT:   "shard by symbol" yahan kaam nahi — TCS ek hi symbol, ek hi thread

SOLUTION: (1) book mat todo — ek book do thread me = double match wapas
              ek thread RAM me, lock ke bina, bahut tez chalta (LMAX ka public design yahi)
          (2) aage ka EVENT LOG burst sambhaal leta — order line me lagte, thread apni speed se uthata
          naya dabba nahi — wahi log kaam aaya
```

---

## 10x SCALE — har dabba alag

```
Order Service (stateless)   -> zyada server + LOAD BALANCER
Portfolio / history         -> READ REPLICA (AWS RDS)
Matching (stateful)         -> LB + copy NAHI (do copy = do book = double match)
                               -> SHARD BY SYMBOL: TCS ek engine pe, INFY doosre pe
Kafka                       -> khud scale nahi hota -> PARTITION badhao (key = symbol)
Price feed                  -> WebSocket server badhao

POOCHEGA: "How would you scale this to 10x users?"
BOL:      "Stateless services scale horizontally behind a load balancer, read-heavy data gets replicas.
           The matching engine is stateful, so I shard it by symbol — each symbol lives on one engine."
```

---

## POOCHE TO (deep-dive)

```
ORDER BOOK:  SELLERS: sasta upar (3001, 3003...)   BUYERS: mehnga upar (2998, 2995...)
             match jab  best-bid >= best-ask
             PRICE-TIME priority: behtar price pehle, same price -> jo pehle aaya

LIMIT  = "3000 ya behtar, warna rukunga"   -> price pakka
MARKET = "jo bhav hai, abhi do"             -> time pakka

PARTIAL FILL: BUY 10, mile 6 -> 6 match, 4 pending -> FILLED / PARTIALLY FILLED / OPEN

DB: money / orders = SQL + ACID · order book = RAM · ledger = append-only

API: POST /order {stock, side, qty, price, type, idempotencyKey} · DELETE /order/{id} · GET /order/{id}
     WS /prices?symbol=
```

---

## AAKHRI DABBA + WRAP

```
  USER  (+ idempotencyKey)
    │
    ▼
  [ Order Service ]          validate + idempotency
    │
    ▼
  [ Wallet ]                 paisa BLOCK
    │
    ▼
  [ Event Log / Sequencer ]  seq no. · key = symbol · replay · audit
    │
    ▼
  [ Matching Engine ] ──► [ Pub/Sub ] ──► [ WebSocket ] ──► USERS
    │   1 thread / symbol · book RAM · shard by symbol
    ▼
  [ Settlement ]             ek txn · kai DB = SAGA
    │
    ├──► [ Wallet DB ]
    └──► [ Portfolio DB ]
```
```
BOL: "Order Service validates and checks the idempotency key, Wallet blocks the money, every order
      goes into an append-only log with a sequence number, and a single-threaded matching engine per
      symbol keeps the book in memory. Settlement is one transaction, or a saga across services.
      Prices go out over WebSocket via pub/sub. The log gives crash recovery and an audit trail."
```

[← MASTER SHEET](../../00_MASTER_SHEET.md)
