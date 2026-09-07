# Notification System — 7-STEP RAIL (single spine, revise top→bottom)

> RAIL: Requirements → Estimate → API → Data-model → HL-boxes → Deep-dive → Bottleneck.
> (framework: 04_HLD/HLD_APPROACH_DELIVERY.md). Merged into clean 7-step 7-Sep.
> Problem (1 line): event aaye -> right CHANNEL -> right USER -> right TIME, reliably at scale.

```
   Amazon order placed -> Push + Email + SMS + In-app = ONE event -> MANY channels + MANY users.
★ WEDDING-CARD ANALOGY: 1 shaadi (event) -> 500 log alag tarike se invite:
   Family=WhatsApp(in-app) . Office=Email . Padosi=card(SMS) . VIP=phone-call(priority).
   Notification system = wedding planner (ek invitation, multi-channel delivery).
```

---

## STEP 1 — REQUIREMENTS

```
FUNCTIONAL:  event aaye -> right CHANNEL -> right USER -> right TIME. reliably deliver.
NON-FUNCTIONAL (core 6):
   1. FANOUT      — 1 event -> many users + many channels
   2. ASYNC       — producer block na ho (background delivery)
   3. RELIABLE    — retry + queue + DLQ
   4. SCALABLE    — ~100K notif/sec peak
   5. PRIORITY    — OTP fast-lane, marketing slow-lane
   6. PREFERENCES — user ka channel-choice honor
CLARIFY:  kaunse channels? notifications/user/day? priority tiers? real-time ya delay-ok?
```

---

## STEP 2 — ESTIMATE (scale / numbers)

```
   100M users × 5 notif/user/day = 500M/day = ~5,800/sec AVERAGE.
   PEAK (sale day) 10x = ~58,000/sec.  fanout multiply (email+push+sms) -> aur zyada msg.
   STORAGE:  tracking ~90 TB/yr (Cassandra) . Kafka ~3 TB (7-day retention).
   -> WRITE-HEAVY + SPIKY -> QUEUE (Kafka) absorb ; scale -> partitions × workers.

THROUGHPUT sanity: 100 events/sec -> 3x fanout -> 300/sec into Kafka; 100 partitions × 100 workers = ~100K/sec capacity
   -> demand 300 << 100K = bahut headroom -> smooth.
```

---

## STEP 3 — API / CHANNELS + EVENT

```
   EVENT-DRIVEN: producers Kafka pe publish -> { event:"ORDER_PLACED", userId:123, orderId:999 }
   (ya internal: POST /notify { userId, type, channel?, payload })
   CHANNELS:
      Push (FCM Android / APNS iOS) . Email (SES / SendGrid / Mailgun) . SMS (Twilio / AWS-SNS / MSG91) .
      In-app (DB + WebSocket) . Voice (Twilio) . Slack (webhook).
```

---

## STEP 4 — DATA MODEL + DB (KYUN)

```
   USER-PREF DB:  user_id -> channels-enabled, quiet-hours, unsubscribed-topics
   TEMPLATE DB:   template_id -> format, i18n (Hindi/Eng), variables {{name}} {{orderId}}
   TRACKING DB:   sent / delivered / opened / clicked / failed   (Cassandra)
   KYUN: massive + simple + write-heavy + ACID na chahiye -> NoSQL (horizontal scale).
```

---

## STEP 5 — HL BOXES (arch + walkthrough)

```
                    EVENT PRODUCERS
       ┌─────────────┬─────────────┬─────────────┐
   Order Svc     Payment Svc   User Svc      Cart Svc
       └─────────────┼─────────────┴─────────────┘
                     ▼
            ┌────────────────────┐
            │  KAFKA TOPIC       │  "notifications"  (decouple + spike absorb)
            └─────────┬──────────┘
                      ▼
            ┌─────────────────────┐
            │  Notification Svc   │  1.receive 2.check user-pref 3.load template
            │  (consumer)         │  4.decide channels 5.FANOUT
            └─────────┬───────────┘
       ┌──────────────┼──────────────┐
       ▼              ▼              ▼
   ┌──────┐      ┌──────┐      ┌──────┐
   │PUSH  │      │EMAIL │      │SMS   │   per-channel QUEUES
   │queue │      │queue │      │queue │
   └──┬───┘      └──┬───┘      └──┬───┘
      ▼             ▼             ▼
   PUSH worker   EMAIL worker   SMS worker
      ▼             ▼             ▼
   FCM/APNS      SES/SendGrid  Twilio  ───► USER ───► Tracking DB (sent/failed)
```

```
FULL WALKTHROUGH:
   1. User places order -> 2. Order Svc publish Kafka { ORDER_PLACED, userId:123, orderId:999 }
   3. Notif Svc consume -> 4. User-pref lookup (123: push:ON, email:ON, SMS:OFF)
   5. Template lookup ("Order #{{orderId}} confirmed") -> 6. render values
   7. FANOUT push-queue + email-queue (SMS skip - pref OFF) -> 8. workers call provider APIs
   9. provider -> user -> 10. track; fail? retry/DLQ.

box KYUN: Kafka=decouple+spike | Notif-Svc=pref+template+fanout | per-channel queue+worker=har channel apni speed/rate | Tracking DB=status.
NAIVE (kyun nahi): Order->Email-server->User direct = Order WAIT (email slow), email-down->order-fail, tight-coupling, no-retry. -> isliye QUEUE.
```

---

## STEP 6 — DEEP DIVE: reliable delivery (asli khel)

```
ASYNC + QUEUE -> producer ko turant ack, bhaari kaam background (decouple).
FANOUT        -> 1 event -> N channels PARALLEL (push+email+sms ek saath).

IDEMPOTENCY (duplicate rok):
   notification sent -> ack kho gaya -> retry -> DUPLICATE (2 email).
   FIX: idempotency key. Worker: Redis me "notification:abc123 already processed?" -> yes:skip | no:send+mark.
   IMPL:  SET notification:abc123 sent NX EX 86400   (NX = set-only-if-absent = ATOMIC check+set)
          -> return 1 (naya, ab bhejo) | nil (pehle se, SKIP).  [alag EXISTS-phir-SET = race-prone; NX = ek atomic step.]
   -> "at-least-once delivery + idempotent worker".

RETRY + EXPONENTIAL BACKOFF:
   naive: fail->retry-immediately = dying-service hammer.
   smart: fail -> wait 1s,2s,4s,8s -> retry (service ko recover time).
   + JITTER:  wait = base × 2^n + random(0,1000ms)  -> thundering-herd rok.

DLQ (Dead Letter Queue):
   main-queue -> worker fail -> retry-queue (delayed) -> max-retries paar -> DLQ -> manual review + ops alert.
   poison msgs: bad email (SES reject), invalid phone (Twilio fail), permanent provider outage.
```

---

## STEP 7 — BOTTLENECK / SCALE / RESILIENCE

```
PRIORITY LANES:  OTP/2FA=HIGH (ms matter, fast worker-pool) . orders=MEDIUM (sec ok) . marketing=LOW (min/hr ok).
   -> alag worker-pools ya PriorityBlockingQueue (priority field).

THROTTLING (provider rate-limits): FCM 1000/sec, SES 14/sec, Twilio 100/sec.
   without -> burst -> provider 429 -> all fail. -> workers respect limits (token/leaky bucket).

PROVIDER FAILOVER + CIRCUIT BREAKER (worker 3rd-party call atke):
   provider DOWN/SLOW -> worker har call timeout -> saare workers phas -> system thapp.
   FIX-1 MULTI-PROVIDER: ek channel ke 2+ provider (SMS: Twilio + AWS-SNS) -> ek down -> doosre pe route (no SPOF).
   FIX-2 CIRCUIT-BREAKER: N consecutive fail -> circuit TRIP (OPEN) -> us provider ko call BAND -> backup route ->
        beech "HALF-OPEN" test -> provider wapas? -> CLOSE (use). states: CLOSED->OPEN(fail-threshold)->HALF-OPEN(test)->CLOSED/OPEN.
        -> workers dead-provider timeouts pe phaste nahi.

DELIVERY STATUS (accept != delivered): worker ne bheja = "ACCEPTED" (provider ne liya), user tak pahuncha? pata nahi.
   provider WEBHOOK/callback -> "delivered"/"failed"/"bounced" -> Tracking DB update (SENT vs DELIVERED/FAILED).
   FAILED (galat number/bounce) -> retry / doosra channel / mark-failed.

HORIZONTAL SCALE: 1 worker ~1000 msg/sec; 100K/sec -> 100 workers.
   Kafka partitioning: topic notifications -> P0->W1 ... P99->W100. partition by hash(user_id) -> same-user->same-partition = per-user ORDERING preserved.

WRAP: Event->Kafka->Notif-Svc (pref+template)->fanout per-channel queues->workers->providers->user->Tracking.
      RELIABLE = retry+backoff+jitter + DLQ + idempotency(Redis SET NX).
      PRIORITY = OTP fast-lane.  SCALE = partitions × workers, partition by user_id.
      RESILIENCE = multi-provider + circuit-breaker + delivery-webhook.
      IMPROVE: quiet-hours, i18n templates, analytics (open/click).
   (real: Uber ride-notif, Amazon order-updates, Slack, WhatsApp, banking alerts.)
```

---

[← HLD README](../README.md)
