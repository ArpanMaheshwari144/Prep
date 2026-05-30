# Notification System — Visual System Design

---

## 1 Problem (1 line)

```
Amazon order placed:
   Push  + Email + SMS + In-app
   = ONE event → MANY channels + MANY users
```

```
Notification System ka kaam:
   Event aaye → right channel → right user → right time
   Reliably, at scale
```

---

## 2 Wedding Card Analogy

```
1 shaadi (event) hai
     │
     ▼
500 logon ko invite karna alag tarike se:
   • Family — WhatsApp     (in-app)
   • Office — Email
   • Padosi — physical card (SMS)
   • VIP   — phone call    (priority)

Notification system = wedding planner
   Ek invitation, multi-channel delivery
```

---

## 3 Core Requirements

```
1. FANOUT     — 1 event → many users + many channels
2. ASYNC       — don't block producer (background delivery)
3. RELIABLE   — retry + queue + DLQ
4. SCALABLE   — 100K notifications/sec at peak
5. PRIORITY   — OTP fast lane, marketing slow lane
6. PREFERENCES — user-honored channel choice
```

---

## 4 Channels

```
┌─────────────┬─────────────────────────────┐
│  Channel    │  Providers                   │
├─────────────┼─────────────────────────────┤
│ Push     │ FCM (Android), APNS (iOS)   │
│ Email    │ SES, SendGrid, Mailgun      │
│ SMS      │ Twilio, AWS SNS, MSG91      │
│ In-app   │ DB store + WebSocket push   │
│ Voice    │ Twilio Voice                │
│ Slack    │ Webhook / API               │
└─────────────┴─────────────────────────────┘
```

---

## 5 Bird's Eye View

```
   EVENT SOURCES         NOTIFICATION             CHANNELS
   (publishers)             SERVICE             (delivery)
        │                       │                    │
   Order placed         ┌────────────────┐      Push (FCM)
   Payment done    ──►  │   Notification │ ──►  Email (SES)
   User signup          │     System     │      SMS (Twilio)
   Cart abandoned       │                │      In-app (WS)
   Promo                └────────────────┘      Slack
```

---

## 6 Naive Design (Won't Work)

```
   Order Service ──► Email Server ──► User

Problems:
   Order Service WAIT karta (email slow)
   Email server down? Order fails
   Tight coupling, no retry, single channel
```

---

## 7 Add Queue (Decouple)

```
   Order Service
        │ publish event
        ▼
   ┌──────────────────┐
   │  KAFKA QUEUE     │
   └────────┬─────────┘
            │
            ▼
   ┌──────────────────┐
   │  Notification    │
   │  Service         │
   └──────────────────┘
        │
        ▼
   Email send

Benefits:
   Order Service FAST
   Email down? Queue mein wait
   Decoupled
```

---

## 8 Fanout to Channels

```
   Notification Service
        │
        ├──► Push worker  → FCM
        ├──► Email worker → SES
        └──► SMS worker   → Twilio

= 1 event → 3 channels PARALLEL
```

---

## 9 Full Architecture

```
                    EVENT PRODUCERS
       ┌─────────────┬─────────────┬─────────────┐
   Order Svc     Payment Svc   User Svc      Cart Svc
       │             │             │             │
       └─────────────┼─────────────┘             │
                     ▼                            ▼
            ┌────────────────────┐
            │   KAFKA TOPIC      │
            │   "notifications"  │
            └─────────┬──────────┘
                      ▼
            ┌─────────────────────┐
            │  Notification       │
            │  Service (consumer) │
            │                      │
            │  1. Receive event    │
            │  2. Check user pref  │
            │  3. Load template    │
            │  4. Decide channels  │
            │  5. Fanout           │
            └─────────┬───────────┘
                      │
       ┌──────────────┼──────────────┐
       ▼              ▼              ▼
   ┌──────┐      ┌──────┐      ┌──────┐
   │PUSH  │      │EMAIL │      │SMS   │
   │queue │      │queue │      │queue │
   └──┬───┘      └──┬───┘      └──┬───┘
      ▼             ▼             ▼
   PUSH worker   EMAIL worker   SMS worker
      │             │             │
      ▼             ▼             ▼
   FCM/APNS      SES/SendGrid  Twilio
      │             │             │
      └─────────────┼─────────────┘
                    ▼
                  USER
                    │
                    ▼
            ┌─────────────────┐
            │  Tracking DB    │
            │  (sent/failed)  │
            └─────────────────┘
```

---

## Supporting Stores

```
User Preferences DB:
   user_id → channels enabled
   user_id → quiet hours
   user_id → unsubscribed topics

Template DB:
   template_id → message format
   I18n (Hindi/Eng)
   Variables: {{name}}, {{order_id}}

Tracking/Analytics:
   Sent, delivered, opened, clicked
   Failed (DLQ)
```

---

## 11 Full Flow Walkthrough

```
1. User places order
2. Order Service publishes Kafka event:
      { event: "ORDER_PLACED", userId: 123, orderId: 999 }
3. Notification Service consumes
4. Lookup User Preferences:
      user 123 → push:ON, email:ON, SMS:OFF
5. Lookup Template:
      "Order #{{orderId}} confirmed"
6. Render with values
7. FANOUT — push-queue + email-queue
   (SMS skip — pref OFF)
8. Workers consume → call provider APIs
9. Provider sends to user
10. Track result; fail? → retry/DLQ
```

---

## 12 Retry + Exponential Backoff

```
Naive retry:
   Fail → retry immediately → fail → retry
   = Hammer the dying service

Smart retry:
   Fail → wait 1s → retry
   Fail → wait 2s → retry
   Fail → wait 4s → retry
   Fail → wait 8s → retry
   = Service ko recover karne ka time

Add JITTER:
   wait = base * 2^n + random(0, 1000ms)
   = Prevents thundering herd
```

---

## 13 DLQ (Dead Letter Queue)

```
   ┌───────────────────────┐
   │  Main Queue           │
   └──────────┬────────────┘
              │ worker tries
              ▼
         Process fails
              │
              ▼
         Retry queue (delayed)
              │ max retries reached
              ▼
   ┌───────────────────────┐
   │  DLQ (Dead Letter)    │
   │  Manual review        │
   └───────────────────────┘
              │
              ▼
   Alert ops → investigate

Use cases:
   Bad email format (SES rejects)
   Invalid phone (Twilio fails)
   Permanent provider outage
```

---

## 14 Idempotency

```
PROBLEM:
   Notification sent → ack lost → retry
   = Duplicate notification (2 emails)

SOLUTION: Idempotency key
   Each notification = unique ID

   Worker logic:
      Check Redis: notification:abc123 already processed?
         Yes → skip
         No  → send + mark
```

```
Implementation:
   SET notification:abc123 sent EX 86400
   On retry: EXISTS check → skip if yes
```

---

## 15 Priority Queue

```
Not all notifications equal:
   OTP / 2FA    — ms matter
   Order confirm — seconds OK
   Marketing    — minutes/hours OK
```

```
SOLUTION:
   ┌────────────────────────┐
   │  HIGH (OTP)            │  ← worker pool 1 (fast)
   ├────────────────────────┤
   │  MEDIUM (orders)        │  ← worker pool 2
   ├────────────────────────┤
   │  LOW (marketing)        │  ← worker pool 3
   └────────────────────────┘

Or: PriorityBlockingQueue with priority field
```

---

## 16 Rate Limiting (Throttling)

```
Provider limits:
   FCM:    1000 req/sec
   SES:    14 req/sec (default)
   Twilio: 100 req/sec

Without throttling:
   Burst → provider 429 → all fail

With throttling:
   Workers respect rate limits
   Token bucket / leaky bucket
```

---

## 17 Horizontal Scaling

```
1 worker handles ~1000 msg/sec
Need 100K/sec? → 100 workers

Kafka partitioning:
   ┌─────────────────────────────────┐
   │  Topic: notifications           │
   │                                  │
   │  Partition 0  →  Worker 1       │
   │  Partition 1  →  Worker 2       │
   │  Partition 2  →  Worker 3       │
   │  ...                             │
   │  Partition 99 →  Worker 100     │
   └─────────────────────────────────┘
```

```
Partition by user_id:
   hash(user_id) % numPartitions

   Same user → SAME partition
   = Ordering preserved per user
```

---

## 18 Throughput Visual

```
100 orders/sec at peak
        ↓
3x fanout (email+push+SMS)
        ↓
300 msg/sec into Kafka
        ↓
100 partitions × 100 workers
        ↓
30,000 msg/sec processing capacity
        ↓
Smooth delivery
```

---

## 19 Capacity Estimation

```
ASSUMPTIONS:
   100M users
   Avg 5 notifications/user/day
   = 500M/day = ~5,800/sec average
   Peak (sale day): 10x = 58,000/sec

STORAGE:
   Tracking 1 year × 500M × 365 × 500 bytes
   = ~90 TB (Cassandra)

KAFKA:
   100 partitions
   Retention 7 days
   = ~3 TB
```

---

## 20 Components Summary

```
┌─────────────────┬─────────────────────────────────┐
│  Component      │  Role                            │
├─────────────────┼─────────────────────────────────┤
│ Event Producers │ Order/Payment/User services     │
│ Kafka           │ Event queue (decouples)         │
│ Notification Svc│ Consume + route + fanout        │
│ User Pref DB    │ Channel preferences             │
│ Template DB     │ Message formats                 │
│ Channel queues  │ Per-channel buffer              │
│ Workers         │ Channel-specific senders        │
│ Providers       │ FCM, SES, Twilio, etc.          │
│ DLQ             │ Failed messages, manual review  │
│ Tracking DB     │ Sent/delivered/failed metrics   │
│ Redis           │ Idempotency keys, rate limiters │
└─────────────────┴─────────────────────────────────┘
```

---

## 21 Read Flow Line (Memorize)

```
"Event in Kafka → Notification Service consumes →
 lookup user preferences + template →
 fanout to per-channel queues →
 channel workers call providers (FCM/SES/Twilio) →
 success tracked, failures retry with exponential backoff →
 max retries exhausted → DLQ for manual review.

 Idempotency via Redis keys to prevent duplicates.
 Kafka partitioning by user_id for ordering + scaling."
```

---

## 22 Production Examples

```
Real systems using this pattern:
   • Uber: ride notifications
   • Amazon: order updates
   • Slack: message notifications
   • WhatsApp: server-to-device
   • Banking: transaction alerts
```

---

## Memory Hooks

```
Notification System = Wedding card distribution

CORE:
   Fanout → 1 event → many channels
   Async  → don't block producer
   Queue  → Kafka decouples

RESILIENCE:
   Retry + exponential backoff + jitter
   DLQ for poison messages
   Idempotency (Redis keys)

PRIORITY:
   OTP fast lane, marketing slow lane

SCALING:
   Kafka partitions × workers
   Partition by user_id (ordering)

CHANNELS:
   FCM (push) | SES (email) | Twilio (SMS)
```

[← HLD README](../README.md)
