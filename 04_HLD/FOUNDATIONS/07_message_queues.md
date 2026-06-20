# Message Queues

> **HLD Topic 7 — Async + Decoupling + Spike absorption**

---

## Why MQ? (Sync vs Async)

### Without (Sync REST)
```
User → /signup → API
                  ├─ DB save (200ms)
                  ├─ Welcome email (1500ms) ← slow!
                  ├─ SMS OTP (800ms)
                  └─ Analytics (300ms)

Total: ~2.8 sec → user waiting
Email server down → ENTIRE signup fails
```

### With (Async Queue)
```
User → /signup → API
                  ├─ DB save (200ms)
                  └─ Push to queue → return 200

                  Background workers (parallel):
                     Email worker  → consume + send
                     SMS worker    → consume + send
                     Analytics     → consume + update

Total: ~250ms → user happy
Email down → message waits in queue, retry later
```

**Core benefit: DECOUPLE producer from consumer.**

---

## STORY — Restaurant Kitchen Order Slips

> Restaurant: waiter (producer) + chef (consumer).
>
> **Without queue:** Waiter chef ke pass jaaye "ye banao" → chef busy → waiter wait → service slow → lunch rush mein chaos.
>
> **With queue (slip board):** Waiter slip lagao → wapas customer ke pass → chef jab free pick kare → process → next.
>
> **Lunch spike?** Slips pile, chef apne pace pe handle. Customer wait nahi karta.
>
> **Chef sick?** Slips queue mein safe — backup chef takeover.
>
> **Slip = Message. Board = Queue. Waiter = Producer. Chef = Consumer.**

---

## 4 Big Wins

```
1. DECOUPLING
   Producer/consumer ek doosre ko nahi jaante
   Email service down → signup chalu rahe

2. ASYNC PROCESSING
   Heavy work background → user wait nahi

3. BUFFERING / SPIKE ABSORB
   1M signups/min → queue absorb
   Workers steady pace pe process

4. RELIABILITY
   Consumer crash → message wait + retry
   No data loss
```

---

## Architecture

```
   PRODUCERS              BROKER                CONSUMERS

   Web Server     ──┐                       ┌── Email Worker
   Mobile App     ──┼──► [m][m][m][m]  ─────┼── SMS Worker
   Admin Panel    ──┘   queue/topic         └── Analytics Worker
                       (Kafka/RabbitMQ)
```

---

## 2 Core Patterns

### **Point-to-Point (Queue)**
```
Producer → Queue → ONE consumer picks
   • Each message processed by ONE worker
   • Multiple workers compete for messages
   • Use: task processing (email, image resize)
```

### **Pub-Sub (Topic)**
```
Producer → Topic → ALL subscribers receive
   • Each message broadcast to many
   • Each subscriber gets own copy
   • Use: events ("order placed" → email + SMS + analytics + warehouse)
```

---

## Delivery Semantics

| Semantic | Guarantee | Speed | Use Case |
|---|---|---|---|
| **At-most-once** | May lose | Fast | Metrics, logs |
| **At-least-once** | May duplicate | Medium | Most cases (with idempotency) |
| **Exactly-once** | No loss, no dup | Slow | Banking, payments |

**Key insight:** At-least-once + idempotent consumer ≈ exactly-once (cheaper).

---

## Kafka vs RabbitMQ

| | **Kafka** | **RabbitMQ** |
|---|---|---|
| Model | Distributed log (append-only) | Traditional broker |
| Throughput | Millions/sec | ~50K/sec |
| Storage | Persistent (days/weeks) | Until consumed |
| Replay | (re-read history) | |
| Consumer | Pull-based | Push-based |
| Routing | Simple (topics) | Complex (exchanges) |
| Use case | Event streaming, analytics | Task queues, RPC |
| Mental model | "Distributed commit log" | "Smart post office" |

**Production rule:**
- Big scale + replay → **Kafka**
- Complex routing + small/medium scale → **RabbitMQ**
- AWS managed simple → **SQS**

---

## WHY Kafka Bana — LinkedIn Origin (event-driven architecture)

> Apni connection (revision 20 Jun): LinkedIn ne Kafka banaya, unka pura system pub-sub hai.

```
PROBLEM (LinkedIn ~2010): bahut services -- profile, feed, search, messaging,
   analytics, ML recommendations. Har koi ko har kisi ka data chahiye.
   Agar har service SEEDHA har service se baat kare:
        N services -> N×N connections -> SPAGHETTI
        naya consumer add -> sab producers ko change (nightmare)

SOLUTION (Kafka = central event LOG):
   koi bhi event ho (profile update, post like, job view)
        -> ek central LOG (topic) mein likho, EK BAAR
        -> jisko chahiye SUBSCRIBE kar le (feed, search, analytics, ML...)
   Producer ko pata NAHI kaun sun raha. Naya consumer add -> kisi ko change nahi.
   -> LinkedIn ka pura architecture EVENT-DRIVEN / pub-sub ban gaya.
```

**Kafka ka khaas (normal queue se farak — yehi insight):**
```
   Normal MQ (RabbitMQ/SQS): message uthaya -> DELETE (transient, ek baar)
   Kafka: DURABLE append-only LOG -> retention (days/weeks)
        -> naya consumer aaye -> PURANE events bhi padh sakta (REPLAY)
        -> isliye analytics/ML ko bhi feed kar paaya (woh history chahte)
```

> **1-line:** N×N spaghetti se bachne ko sab kuch ek central durable event-log mein —
> producer blind, consumer subscribe, replay possible. LinkedIn = pub-sub at its core.

---

## Kafka Deep (key concepts)

```
TOPIC = logical stream of messages
   "user-events", "order-events"

PARTITION = topic split into pieces (parallelism)
   Topic "user-events" → partitions 0,1,2,3
   Each partition = ordered append-only log

PRODUCER → writes with optional KEY
   Same key → same partition (ordering preserved)

CONSUMER GROUP = set of consumers sharing work
   Each partition → ONE consumer in group
   Add consumers → parallelism (max = #partitions)

OFFSET = position in partition
   Consumer tracks: "I've read up to offset 1247"
   Replay = reset offset
```

---

## RabbitMQ Deep (key concepts)

```
EXCHANGE = receives messages from producer
   Types:
     Direct   → routing key match
     Topic    → wildcard pattern
     Fanout   → broadcast all queues
     Headers  → header attribute match

QUEUE = stores messages until consumer reads

BINDING = exchange → queue routing rule
```

```java
// Spring example
@RabbitListener(queues = "email-queue")
public void process(EmailEvent event) {
    emailService.send(event);
}
```

---

## 4 Hard Problems

### 1. **Message Ordering**
```
Kafka: Order guaranteed only WITHIN a partition
       Cross-partition = no order
       Fix: Use same KEY for related messages → same partition
            (e.g., key=user_id keeps user's events ordered)
```

### 2. **Idempotency** (mandatory!)
```
At-least-once → duplicate possible
Consumer must handle:
   "Process payment for order_123" — comes twice
   Charge twice
   Check if already processed → skip

Pattern:
   Unique message_id + processed_messages table
   Or: idempotent operations (upsert, set status)
```

### 3. **Dead Letter Queue (DLQ)**
```
Message fails repeatedly (e.g., 5 retries) → DLQ
   → Doesn't block main queue
   → Manual investigation
   → Fix root cause, replay

Production essential — bina DLQ = poison messages stuck
```

### 4. **Backpressure**
```
Producers >> Consumers → queue grows ∞
Fixes:
   • Auto-scale consumers (k8s HPA on queue depth)
   • Rate-limit producers
   • Drop messages (priority-based)
   • Bigger broker storage (Kafka holds days)
```

---

## Real-World Tools

| Tool | Type | Use |
|---|---|---|
| **Kafka** | Log-based | LinkedIn, Uber, Netflix — event streaming |
| **RabbitMQ** | Broker | Traditional Java/Spring apps |
| **AWS SQS** | Managed queue | Simple async tasks |
| **AWS Kinesis** | Managed Kafka-like | AWS event streaming |
| **Google Pub/Sub** | Managed | GCP pub-sub |
| **Redis Streams** | Lightweight | Smaller scale, simple |
| **ActiveMQ** | Broker | Older Java enterprise |

---

## Interview Talking Points

**Q: "MQ vs REST kab?"**

> *"REST sync — caller waits, tightly coupled. MQ async — decouple, buffer spikes, retry on failure. Heavy/slow/non-critical work → MQ. Real-time response → REST. Modern microservices: events for state changes, REST for queries."*

**Q: "Kafka vs RabbitMQ?"**

> *"Kafka = distributed append-only log, millions/sec, persistent, replay support — event streaming. RabbitMQ = traditional broker, complex routing (exchanges), push-based, task queues. Big scale + replay → Kafka. Complex routing + small/medium → RabbitMQ."*

**Q: "Exactly-once delivery kaise?"**

> *"Hard. Practical: **at-least-once + idempotent consumer** (unique message_id, dedup table). Effectively exactly-once at lower cost. Strict exactly-once needs distributed transactions (2PC) — slow, brittle, mostly avoid."*

**Q: "DLQ kya?"**

> *"Dead Letter Queue — messages failing repeatedly (5 retries) shifted here. Main queue se stuck poison messages hatate. Manual investigation + fix + replay. Production mandatory."*

**Q: "Kafka mein ordering?"**

> *"Within partition guaranteed (append-only log). Cross-partition no order. Trick: same key → same partition. user_id key → us user ke saare events ordered. Critical for sequence-dependent flows."*

---

## Power Phrase

> **"MQ = async + decouple + buffer + reliability. Producer publishes, consumer processes — independently. Patterns: P2P (queue) vs Pub-Sub (topic). Delivery: at-least-once + idempotent = practical exactly-once. Kafka (log, millions/sec, replay) vs RabbitMQ (broker, routing). Hard problems: ordering, idempotency, DLQ, backpressure."**

---

## Memory Hook

```
MQ = "Restaurant order slips"
   • Waiter (producer) → board (queue) → chef (consumer)
   • Decouple, async, buffer, reliable

Patterns:
   Point-to-Point (Queue) → 1 message, 1 consumer
   Pub-Sub (Topic)        → 1 message, all subscribers

Delivery:
   At-most-once   → fast, lossy (metrics)
   At-least-once  → most common (idempotent consumer)
   Exactly-once   → slowest, banking

Kafka:
   Topic → Partitions → Offset
   Same key → same partition = order
   Consumer group → parallelism
   Replay support

RabbitMQ:
   Exchange → Binding → Queue
   Direct/Topic/Fanout/Headers
   Push-based, complex routing

Hard problems:
   1. Ordering   — partition key trick
   2. Idempotency — unique msg_id + dedup
   3. DLQ        — poison message escape
   4. Backpressure — auto-scale consumers
```

---

## Trap Box

```
Trap 1: "MQ = always better than REST"
         Real-time queries (get user) → REST simpler
         Async/heavy/spike-prone work → MQ

Trap 2: "Exactly-once is achievable easily"
         Distributed exactly-once = hard problem
         At-least-once + idempotent consumer

Trap 3: "Kafka order guaranteed always"
         Only within partition
         Use partition key for related messages

Trap 4: "No DLQ needed"
         Poison messages stuck retry forever
         DLQ mandatory in production

Trap 5: "Consumer faster than producer always"
         Spikes → backpressure → infinite queue
         Auto-scale consumers, monitor queue depth

Trap 6: "Kafka and RabbitMQ same thing"
         Different paradigms (log vs broker)
         Kafka = streaming, RabbitMQ = task queues

Trap 7: "Async = lost reliability"
         MQ persistence + retry = MORE reliable than REST
         MQ improves reliability with right config
```
