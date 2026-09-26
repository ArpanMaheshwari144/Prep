# Notification System — POORA ROUND (4 MOVE, jaise asli me hota hai)

> **NAV** — ARCHETYPE B (ingest) · DIL: ek event -> sahi channel/user/time. UP: [MASTER](../../00_MASTER_SHEET.md) · CONCEPTS: [message-queues](../../FOUNDATIONS/07_message_queues.md) · [ms-communication](../../FOUNDATIONS/10_ms_communication.md) · saath: [13 message-queue](../13_message_queue_kafka/13_message_queue_kafka.md)

> 15-Sep: asli mock-video ke hisaab se dobara likha — koi 7-step rail nahi, sirf 4 move:
> POOCHA -> do chhote block LIKHE -> BOXES banaye -> phir bolte-bolte JODTA gaya.
> Har jagah: **tu kya BOLTA hai · BOARD pe kya banta · FAISLA + KYUN**.
>
> Problem (1 line): event aaye -> sahi CHANNEL -> sahi USER -> sahi TIME, bharose ke saath, scale pe.

```
   Amazon order placed -> Push + Email + SMS + In-app = EK event -> KAI channel + KAI user.

★ SHAADI-CARD ANALOGY (visual anchor):
   1 shaadi (event) -> 500 log, sabko alag tarike se nyota:
      Family = WhatsApp (in-app) . Office = Email . Padosi = card (SMS) . VIP = phone call (priority)
   Notification system = shaadi ka planner — ek nyota, kai raaston se delivery.
```

```
★★ TEEN NIYAM (poori file par lagte — [APPROACH_DELIVERY](../../01_DELIVERY.md) 5b)
   1. PERFECT design ek saath mat banao — chhote se shuru, dikkat pe badhao
   2. NUMBER ke peeche mat bhaago — bolo, ek faisla nikaalo, aage badho
   3. BOTTLENECK ratto mat — KHUD USER banke raasta chalao, khud dikh jaayega
```

---

# MOVE 1 — POOCHO (board pe abhi kuch nahi)

```
   TU: "Notification system me kai taraf ja sakte hain. Aap kis pe focus karwana chahenge —
        delivery pipeline, ya user preferences aur templating bhi?"

   TU: "Kuch cheezein confirm kar lun —
          - kaunse CHANNEL chahiye? push, email, SMS, in-app?
          - ek user ko din me kitni notification? main ~5 maan raha hoon
          - PRIORITY tiers hain? (OTP turant chahiye, marketing ruk sakti hai)
          - real-time chahiye ya thodi der chalegi?"

   ★ PRIORITY wala sawaal zaroori hai — OTP aur marketing ek hi queue me daal diye
     to OTP 10 minute late pahunchega, aur wahi asli fail hai.
```

---

# MOVE 2 — DO CHHOTE BLOCK LIKHO

```
   ┌──────────────────────────┐   ┌────────────────────────────────────┐
   │ Notification System      │   │ Use cases:                         │
   │   - Event                │   │   - order placed -> push+email     │
   │   - User (+ preferences) │   │   - OTP -> SMS, turant             │
   │   - Channel              │   │   - sale offer -> push, ruk sakta  │
   │   - Template             │   │                                    │
   │   - Delivery status      │   │ NOT in scope: analytics dashboard  │
   └──────────────────────────┘   └────────────────────────────────────┘

   ┌───────────────────────────────┐
   │ Kya chahiye (NFR) — core 6:   │
   │  1. FANOUT   1 event -> kai   │
   │              user + kai channel│
   │  2. ASYNC    producer ruke na │
   │  3. RELIABLE retry + DLQ      │
   │  4. SCALE    ~100K notif/sec  │
   │  5. PRIORITY OTP fast-lane    │
   │  6. PREFERENCE user ki marzi  │
   └───────────────────────────────┘
```

```
   Numbers:
     - 100 M users x 5 notif/day  =  500 M / day  =  ~5,800 / sec  (average)
     - PEAK (sale day) 10x        =  ~58,000 / sec
     - fanout se aur badhega (ek event -> email + push + SMS)
     - tracking storage           =  ~90 TB / saal (Cassandra)
     - Kafka (7 din retention)    =  ~3 TB

   HAR NUMBER SE EK FAISLA:
     spiky peak (10x)   ──►  QUEUE chahiye jo jhatka absorb kare (Kafka)
     write-heavy        ──►  NoSQL (Cassandra) tracking ke liye, ACID ki zaroorat nahi
     58K/sec            ──►  ek worker ~1000/sec -> ~60-100 worker + utni partitions

   ★ SANITY CHECK (bolne layak): "peak 58K event/sec x 3 channel = ~1.75 lakh/sec Kafka me;
     isliye partitions + workers PEAK ke hisaab se, average (5,800/sec) ke nahi."
```

---

# MOVE 3 — BOXES BANAO (chhota banao, phir dikkat pe badhao)

```
   TU: "Sabse simple cheez se shuru karta hoon."

        [ Order Service ] ──► [ Email API (SES) ] ──► USER

   TU: "Chalta hai. Ab dekhte hain kahan tootega."
```

### dikkat 1 — "order API email ke intezaar me ruki hai, aur email down ho to ORDER hi fail"

```
        NAIVE (yahi galti hoti hai):
            Order ──► Email server ──► User
              │
              └─ Order ka response email ke bheje jaane tak RUKA hua
                 email server down -> ORDER hi fail ho gaya
                 tight coupling, koi retry nahi

        FAISLA — beech me QUEUE:

            [ Order Svc ] ──event──► [ KAFKA ] ──► [ Notification Svc ] ──► email

   TU: "Order ko email ka intezaar karne ki zaroorat hi nahi. Event queue me daal ke
        turant laut jaata hoon. Email service down ho to event queue me pada rehta hai —
        wapas aane pe nikal jaayega. Order kabhi fail nahi hoga."
```

### dikkat 2 — "ek event ke teen channel hain, SMS slow hai to push/email bhi atak gaye"

```
        FAISLA — HAR CHANNEL KI APNI QUEUE + APNE WORKER

            [ Notification Svc ]
                   │  fanout
         ┌─────────┼─────────┐
         ▼         ▼         ▼
      ┌──────┐ ┌──────┐ ┌──────┐
      │PUSH  │ │EMAIL │ │ SMS  │    per-channel QUEUE
      │queue │ │queue │ │queue │
      └──┬───┘ └──┬───┘ └──┬───┘
         ▼        ▼        ▼
      PUSH-w   EMAIL-w   SMS-w      per-channel WORKER
         ▼        ▼        ▼
      FCM/APNS  SES/    Twilio/
                SendGrid AWS-SNS

   TU: "Har channel ki speed aur rate-limit alag hai. Alag queue aur alag worker rakhunga
        taaki ek channel ka slow hona baaki ko na rokey."
```

### dikkat 3 — "user ko SMS chahiye hi nahi tha, aur raat 2 baje bhej diya"

```
        FAISLA — Notification Svc ke andar teen kaam:

            event aaya
                │
                ├─► USER-PREF dekho    user 123: push ON . email ON . SMS OFF . quiet 10pm-8am
                ├─► TEMPLATE lo        "Order #{{orderId}} confirmed"  (+ i18n Hindi/Eng)
                └─► CHANNEL tay karo   -> push + email  (SMS skip)

   USER-PREF DB : user_id -> channels-enabled . quiet-hours . unsubscribed-topics
   TEMPLATE DB  : template_id -> format . i18n . variables {{name}} {{orderId}}
```

### dikkat 4 — "ack kho gaya, retry hua, user ko DO email chale gaye"

```
        worker ne bheja ──► provider ne le liya ──► ack RAASTE ME KHO GAYA
                                                       │
                                            queue ko laga "fail hua" -> RETRY -> 2 email

        FAISLA — IDEMPOTENT WORKER

            worker: "notification:abc123 pehle bhej chuke?"
                        │
                SET notification:abc123 sent NX EX 86400
                        │
                   ├─ "OK" mila -> naya hai -> BHEJO   (SET ... NX "OK" deta; "1" purane SETNX ka jawab tha)
                   └─ nil mila-> pehle ho chuka -> SKIP

   ★ NX = "set only if absent" = check aur set EK atomic step me.
     Alag-alag EXISTS phir SET karoge to beech me doosri request ghus sakti hai (race).
   ★ ek line: "at-least-once delivery + idempotent worker"
   ★ ye WAHI cheez hai jo payment idempotency me hai — same race, same ilaaj.
```

### dikkat 5 — "provider fail ho raha hai aur hum turant retry maar rahe hain"

```
        NAIVE : fail -> turant retry -> marte hue provider pe aur hathoda

        FAISLA — BACKOFF + JITTER + DLQ

            fail ──► ruko 1s ──► 2s ──► 4s ──► 8s ──► ...     (provider ko saans lene do)
                     wait = base x 2^n + random(0..1000ms)
                                          └── JITTER: saare worker ek saath wapas na aayein
                                              (thundering herd)

            main queue ──► worker fail ──► retry queue (delayed)
                                              │
                                     max retry paar ──► [ DLQ ] ──► manual review + ops alert

        DLQ me kya girta hai (poison messages):
            galat email (SES reject) . invalid phone (Twilio fail) . provider ka permanent outage
```

### dikkat 6 — "provider slow ho gaya, saare worker uske timeout me phas gaye"

```
        provider SLOW/DOWN ──► har call 30 sec timeout ──► saare worker atke ──► system thapp

        FIX 1 — MULTI-PROVIDER: ek channel ke 2+ provider (SMS: Twilio + AWS-SNS)
                                ek down -> doosre pe route -> koi SPOF nahi

        FIX 2 — CIRCUIT BREAKER:

                 CLOSED  ──N baar fail──►  OPEN  ──thodi der baad──►  HALF-OPEN
                   ▲                        │                            │
                   │                   call BAND                    ek test call
                   │                   backup pe route                   │
                   └──────── theek nikla ◄─────────────────────── fail -> wapas OPEN

   TU: "Circuit khul jaane pe worker us provider ko call hi nahi karta — isliye wo
        timeout me phasta nahi, aur backup provider chalta rehta hai."
```

### dikkat 7 — "OTP marketing ke 50,000 message ke peeche queue me laga hai"

```
        PRIORITY LANES:

            HIGH    OTP / 2FA        -> millisecond maayne rakhte  -> apna fast worker pool
            MEDIUM  order updates    -> kuch second chalega
            LOW     marketing/offers -> minute-ghanta chalega

        implement: alag worker-pool per lane, ya ek PriorityBlockingQueue (priority field)
```

### dikkat 8 — "burst gaya, provider ne 429 de diya, sab fail ho gaye"

```
        PROVIDER RATE LIMITS (asli numbers):
            FCM    default ~6 lakh / minute per project (~10K / sec)
            SES    ~14 / sec  (naye account ka SHURUAATI default, badhwaya ja sakta)
            Twilio short code ~100 / sec · long code ~1 / sec

        bina throttle -> burst -> 429 -> saare message fail
        FAISLA: worker khud limit maane (token bucket / leaky bucket) -> provider ki raftaar se bhejo
```

### dikkat 9 — "'bhej diya' ka matlab 'mil gaya' nahi hota"

```
        worker ne bheja = "ACCEPTED" (provider ne le liya)
        user tak pahuncha? -> abhi pata nahi

        FAISLA — provider ka WEBHOOK / callback:
            provider ──► "delivered" / "failed" / "bounced" ──► Tracking DB update

        TRACKING DB (Cassandra): sent . delivered . opened . clicked . failed
        FAILED (galat number / bounce) -> retry . doosra channel . ya mark-failed
```

### ab poora naksha (jahan pahunche) + har box ka KYUN

```
                    EVENT PRODUCERS
       ┌─────────────┬─────────────┬─────────────┐
   Order Svc     Payment Svc   User Svc      Cart Svc
       └─────────────┼─────────────┴─────────────┘
                     ▼
            ┌────────────────────┐
            │  KAFKA TOPIC       │  "notifications"   decouple + spike absorb
            └─────────┬──────────┘
                      ▼
            ┌─────────────────────┐
            │  Notification Svc   │  1. event lo  2. user-pref dekho  3. template lo
            │  (consumer)         │  4. channel tay karo  5. FANOUT
            └─────────┬───────────┘
       ┌──────────────┼──────────────┐
       ▼              ▼              ▼
   ┌──────┐      ┌──────┐      ┌──────┐
   │PUSH  │      │EMAIL │      │SMS   │   per-channel QUEUE (priority lanes andar)
   │queue │      │queue │      │queue │
   └──┬───┘      └──┬───┘      └──┬───┘
      ▼             ▼             ▼
   PUSH worker   EMAIL worker   SMS worker      <- idempotent + backoff + circuit breaker
      ▼             ▼             ▼
   FCM/APNS      SES/SendGrid  Twilio/SNS ───► USER
                                     │
                                  webhook ───► [ Tracking DB ]  sent/delivered/failed
                                     │
                            fail x N ───► [ DLQ ] ──► ops alert

     KAFKA         : decouple + spike absorb (order kabhi na ruke)
     Notif Svc     : pref + template + fanout — ek jagah faisla
     per-channel Q : har channel apni speed/rate pe chale
     worker        : idempotent (SET NX) + backoff/jitter + circuit breaker
     Tracking DB   : "bheja" vs "mila" ka farak rakhta
     DLQ           : poison message baaki ko na rokey
```

```
   POORA WALKTHROUGH (ek line me bolne layak):
     1. user ne order kiya
     2. Order Svc ne Kafka pe daala { ORDER_PLACED, userId:123, orderId:999 }
     3. Notif Svc ne uthaya
     4. user-pref dekha (123: push ON, email ON, SMS OFF)
     5. template liya ("Order #{{orderId}} confirmed") -> values bhari
     6. fanout: push-queue + email-queue (SMS skip)
     7. worker ne provider ko call kiya
     8. provider -> user
     9. webhook se status aaya -> Tracking DB
    10. fail hua to retry -> aakhir me DLQ
```

---

# MOVE 4 — BOLTE-BOLTE JODO (jo poocha jaaye, wahi kholo)

## ► "Event kaisa dikhta hai / API kya?"

```
   EVENT-DRIVEN (yahi asli tareeka):
     producer Kafka pe publish karta ->  { event: "ORDER_PLACED", userId: 123, orderId: 999 }

   (ya andar ke liye ek REST bhi rakh sakte)
     POST /notify  { userId, type, channel?, payload }

   CHANNELS:
     Push   : FCM (Android) / APNS (iOS)
     Email  : SES / SendGrid / Mailgun
     SMS    : Twilio / AWS SNS / MSG91
     In-app : DB + WebSocket
     Voice  : Twilio          Slack : webhook
```

## ► "DB me kya rakhoge?"

```
   USER-PREF DB : user_id -> channels-enabled . quiet-hours . unsubscribed-topics
   TEMPLATE DB  : template_id -> format . i18n (Hindi/English) . variables
   TRACKING DB  : sent / delivered / opened / clicked / failed      (Cassandra)

   KYUN NoSQL: bahut zyada + simple + write-heavy, aur ACID ki zaroorat nahi
               -> horizontal scale aasan
```

## ► "100K per second pe kaise chalega?"

```
   1 worker ~1000 msg/sec  ->  100K/sec ke liye ~100 worker

   KAFKA PARTITIONING:
        topic "notifications"
            P0  ──► W1
            P1  ──► W2
            ...
            P99 ──► W100

   ★ partition by hash(user_id)  -> ek user ke saare event EK partition me
                                  -> us user ke liye ORDER bana rehta
                                  (warna "order shipped" pehle aur "order placed" baad me aa sakta)
```

## ► "Kahan tootega / 10x pe?"

```
   ★ RATTO MAT — event ka raasta chalao:

      event aaya
          │
          ├─► Kafka        -> peak 10x?           -> partitions badhao (pehle se extra rakho)
          ├─► Notif Svc    -> pref/template lookup har event pe -> CACHE kar lo
          ├─► channel queue-> ek channel slow     -> alag queue (kar diya) + priority lane
          ├─► worker       -> provider slow/down  -> circuit breaker + multi-provider
          │                   duplicate           -> idempotency key
          │                   provider 429        -> throttle (token bucket)
          └─► tracking     -> 90 TB/saal          -> Cassandra + purana data cold storage me
```

## ► WRAP (aakhir me 3-4 line)

```
   "Event -> Kafka -> Notification Service (preferences + template) -> per-channel queues
    -> workers -> providers -> user -> tracking.
    Reliability: retry with exponential backoff + jitter, DLQ, aur idempotent worker (Redis SET NX).
    Priority: OTP ke liye alag fast lane.
    Scale: partitions x workers, partition by user_id taaki per-user order bana rahe.
    Resilience: multi-provider + circuit breaker + delivery webhook.
    Aage badhata to: quiet hours, i18n templates, aur open/click analytics."

   (asli duniya me yahi hai: Uber ride notifications, Amazon order updates, Slack, WhatsApp, bank alerts)
```

---

[← MASTER SHEET](../../00_MASTER_SHEET.md)
