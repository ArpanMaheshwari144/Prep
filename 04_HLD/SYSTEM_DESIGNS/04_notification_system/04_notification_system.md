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
★★ TEEN NIYAM (poori file par lagte — [MASTER](../../00_MASTER_SHEET.md) "Kaise bolna")
   1. PERFECT design ek saath mat banao — chhote se shuru, dikkat pe badhao
   2. NUMBER ke peeche mat bhaago — bolo, ek faisla nikaalo, aage badho
   3. BOTTLENECK ratto mat — KHUD USER banke raasta chalao, khud dikh jaayega
```

---

## ═══ DIAGRAM — tasveer se samjho (ByteByteGo / Alex Xu) ═══

> Tasveer unki site se seedha dikhti hai (copy nahi ki). Credit: ByteByteGo, Alex Xu · License CC BY-NC-ND 4.0.
> Tareeka: design revise karte waqt tasveer dekho, phir neeche ka apna section padho ("Is file me kahan juda" wahi batata hai).

### How Does a Typical Push Notification System Work?

![How Does a Typical Push Notification System Work?](https://assets.bytebytego.com/diagrams/0042-design-a-notification-push-system.png)

- **Is file me kahan juda:** poora naksha: event -> notification service -> queue -> worker -> provider (FCM / APNs / SMS).
- Source: [How Does a Typical Push Notification System Work?](https://bytebytego.com/guides/how-does-a-typical-push-notification-system-work/)

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

   ► INTERVIEWER AISE POOCHEGA:
       "What if traffic suddenly spikes 10x?"

   ► YAHI SAWAAL DOOSRE DESIGN ME BHI (wahan bhi yahi soch):
       bookmyshow    -> popular release        -> queue + darwaze pe counter / waiting room
       rate limiter  -> bheed                  -> load shedding, 429 + Retry-After
       twitter feed  -> viral tweet / event    -> cache + queue, pehle se scale (pre-warm)

   ► MASTER SHEET SE JODA: sale ka time pata hai (12 baje) -> worker PEHLE se badhao;
       autoscale minute leta, spike seconds me aata. worker badhao to DB/provider ki had pe cap.
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
                SET notification:abc123 sent NX EX 86400      (isme ek chhed hai -> dikkat 4b)
                        │
                   ├─ "OK" mila -> naya hai -> BHEJO   (SET ... NX "OK" deta; "1" purane SETNX ka jawab tha)
                   └─ nil mila-> pehle ho chuka -> SKIP

   ★ NX = "set only if absent" = check aur set EK atomic step me.
     Alag-alag EXISTS phir SET karoge to beech me doosri request ghus sakti hai (race).
   ★ ek line: "at-least-once delivery + idempotent worker"
   ★ ye WAHI cheez hai jo payment idempotency me hai — same race, same ilaaj.

   ► INTERVIEWER AISE POOCHEGA:
       "What if the same request comes twice / the client retries?"

   ► YAHI SAWAAL DOOSRE DESIGN ME BHI (wahan bhi yahi soch):
       payment     -> Pay timeout, user dobara   -> idempotency key, dobara pe STORED result
       bookmyshow  -> Pay do baar daba           -> idempotency key (bookingId)
       kafka       -> consumer ko event dobara   -> eventId "processed" table, skip
       chat        -> message retry              -> clientMsgId se dedup

   ► MASTER SHEET SE JODA: key = event ka apna id (eventId), har retry pe WAHI.
       naya UUID har baar banaya to dedup kabhi pakdega hi nahi.
```

#### ★ dikkat 4b — "key laga di, par provider call FAIL ho gaya" (29-Sep mock me poocha gaya)

```
        DIKKAT:
          1. worker:  SET abc-123 NX           -> "OK"  (key lag gayi)
          2. worker:  email provider ko call   -> FAIL  (provider down)
          3. retry:   SET abc-123 NX           -> nil   (key pehle se hai) -> SKIP
          -> email kabhi gaya hi nahi, par system maan raha "bhej diya" = MESSAGE KHO GAYA

        ILAAJ (do me se ek):
          A) fail hua to key HATA do
               call fail -> DEL abc-123 -> retry pe key nahi milegi -> dobara bhejega

          B) do haalat rakho   <- ZYADA PAKKA
               pehle:       SET abc-123 "sending" NX EX 60       (chhoti expiry)
               success pe:  SET abc-123 "sent"    EX 86400       (lamba)
               worker beech me hi MAR gaya (A ka DEL chala hi nahi)
                 -> 60 sec me "sending" khud mit jaata -> retry chal jaata

          A kyun kam pakka: worker crash ho jaaye to DEL chalta hi nahi -> key atki -> message kho gaya.

   TU: "If the provider call fails, I don't want the key to block the retry. So I set it as
        'sending' with a short TTL, and only mark it 'sent' after the provider accepts it.
        If the send fails or the worker dies, the key expires and the retry goes through."

   ► INTERVIEWER AISE POOCHEGA:
       "You set the idempotency key, but then the send failed. Now what?"

   ► YAHI SAWAAL DOOSRE DESIGN ME BHI (wahan bhi yahi soch):
       payment  -> key lagi, PSP fail  -> IN_PROGRESS -> DONE, wahi soch
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

   ► INTERVIEWER AISE POOCHEGA:
       "How do you make sure no message is lost?"

   ► YAHI SAWAAL DOOSRE DESIGN ME BHI (wahan bhi yahi soch):
       kafka    -> consumer crash            -> offset kaam ke BAAD, idempotent, DLQ
       banking  -> DB + event dono chahiye   -> OUTBOX
       chat     -> message                   -> pehle DB me, phir bhejo

   ► MASTER SHEET SE JODA: worker Kafka offset commit KAAM (provider call) ke BAAD kare —
       crash hua to event dobara aayega, khoyega nahi (isiliye worker idempotent, dikkat 4).
       producer side: acks=all + replication, taaki event Kafka me hi na khoye.
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

   ► INTERVIEWER AISE POOCHEGA:
       "What if the downstream service / provider is slow?"

   ► YAHI SAWAAL DOOSRE DESIGN ME BHI (wahan bhi yahi soch):
       payment          -> PSP slow          -> timeout, PENDING rakho + reconcile, andha retry nahi
       news aggregator  -> ek source slow    -> har source ka timeout, skip karo, baaki dikhao

   ► MASTER SHEET SE JODA: har provider call pe CHHOTA timeout (30 sec nahi).
       slow = down se BURA — worker phas ke poora pool kha jaata.
```

### dikkat 7 — "OTP marketing ke 50,000 message ke peeche queue me laga hai"

```
        PRIORITY LANES:

            HIGH    OTP / 2FA        -> millisecond maayne rakhte  -> apna fast worker pool
            MEDIUM  order updates    -> kuch second chalega
            LOW     marketing/offers -> minute-ghanta chalega

        implement: har lane ka ALAG Kafka topic + apna worker pool (notif-high / -medium / -low)
        ★ Kafka me priority nahi hoti — ek topic me OTP peeche hi lagega. Isliye alag topic.
          (PriorityBlockingQueue sirf EK process ke andar chalti, distributed system me nahi.)

   ► INTERVIEWER AISE POOCHEGA:
       "How do you prioritize urgent work, like OTPs?"

   ► YE SIRF IS DESIGN ME AATA HAI
```

### dikkat 8 — "burst gaya, provider ne 429 de diya, sab fail ho gaye"

```
        PROVIDER RATE LIMITS (asli numbers):
            FCM    default ~6 lakh / minute per project (~10K / sec)
            SES    ~14 / sec  (naye account ka SHURUAATI default, badhwaya ja sakta)
            Twilio short code ~100 / sec · long code ~1 / sec

        bina throttle -> burst -> 429 -> saare message fail
        FAISLA: worker khud limit maane (token bucket / leaky bucket) -> provider ki raftaar se bhejo
        (chala ke dekha: neeche HANDS-ON — naive me 1000 me se 700 SMS phenke gaye)

   ► INTERVIEWER AISE POOCHEGA:
       "The provider returns 429 — you're sending too fast. What now?"

   ► YAHI SAWAAL DOOSRE DESIGN ME BHI (wahan bhi yahi soch):
       rate limiter  -> hum khud 429 dete  -> 429 + Retry-After header

   ► MASTER SHEET SE JODA: 429 aaya to turant retry nahi — exponential backoff + jitter,
       Retry-After header maano, message queue me ruke (drop nahi).
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

   ► INTERVIEWER AISE POOCHEGA:
       "Data keeps growing — what happens in 3 years?"

   ► YAHI SAWAAL DOOSRE DESIGN ME BHI (wahan bhi yahi soch):
       news aggregator  -> purani news         -> TTL / archive
       banking          -> ledger              -> KABHI delete nahi, purana cold storage
       chat             -> purane messages     -> month se partition, cold storage
       payment          -> payment records     -> archive, delete nahi

   ► MASTER SHEET SE JODA: tracking row pe TTL lagao -> purana log khud mitega.
       retention != sharding (retention size ghatata, shard load baant-ta).

   ► INTERVIEWER AISE POOCHEGA (har design me aate hain, jawab = yahi section):
       "How would you scale this to 10x users?"        -> pehle kya tootega, wahi ka ilaaj
       "What's the single point of failure here?"      -> raasta chalo, har box pe "ye gira to?"
       "How do you know the system is working?"        -> p99 · error rate · queue lag · alert

   ► MASTER SHEET SE JODA: yahan "working" = sirf "bheja" nahi — provider webhook se
       delivered / failed (dikkat 9) + Kafka consumer lag + DLQ size pe alert.
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

## ═══ HANDS-ON — PROVIDER NE 429 DIYA: chala ke dekha (30-Sep) ═══
> Grill me galti hui thi: 429 pe "fail-open" bola. Fail-open HUMARE limiter ka faisla hota hai (Redis gira to);
> yahan limiter PROVIDER ka hai, uska darwaza hum nahi khol sakte -> dheema HUMEIN hona padega.
> CODE: `04_HLD/HANDS_ON/03_provider_429/Sms429Demo.java`  (koi Docker nahi, time nakli, turant chalta)
> Line 21: `SMART = false` / `true`  ->  `java Sms429Demo.java`

### Setup
```
1000 SMS ek saath (sale ka din) · provider 100/sec leta · t=3,4 pe provider aur dheema: 50/sec
NAIVE : sab ek saath bhejo, 429 pe agle second turant retry, 3 baar fail -> PHENK DO
SMART : apni taraf 100/sec (throttle, token bucket) · 429 pe 1s, 2s, 4s ruko (backoff) · kabhi phenko mat
```

### ASLI OUTPUT (jo screen pe aaya)

ROUND 1 — `SMART = false`
```
MODE = NAIVE (sab bhej do, turant retry)

  t | provider | queue me | bheje | 200 OK | 429 | chhode | pahunche (kul)
----+----------+----------+-------+--------+-----+--------+---------------
  0 |  100/sec |     1000 |  1000 |    100 | 900 |      0 | 100
  1 |  100/sec |      900 |   900 |    100 | 800 |      0 | 200
  2 |  100/sec |      800 |   800 |    100 | 700 |    700 | 300

PAHUNCHE      = 300 / 1000
CHHOD DIYE    = 700   <- ye customers ko SMS kabhi nahi mila
provider call = 2700  (inme 429 = 2400)
```

ROUND 2 — `SMART = true`
```
MODE = SMART (throttle + backoff)

  t | provider | queue me | bheje | 200 OK | 429 | chhode | pahunche (kul)
----+----------+----------+-------+--------+-----+--------+---------------
  0 |  100/sec |     1000 |   100 |    100 |   0 |      0 | 100
  1 |  100/sec |      900 |   100 |    100 |   0 |      0 | 200
  2 |  100/sec |      800 |   100 |    100 |   0 |      0 | 300
  3 |   50/sec |      700 |   100 |     50 |  50 |      0 | 350
  4 |   50/sec |      650 |   100 |     50 |  50 |      0 | 400
  5 |  100/sec |      600 |   100 |    100 |   0 |      0 | 500
  6 |  100/sec |      500 |   100 |    100 |   0 |      0 | 600
  7 |  100/sec |      400 |   100 |    100 |   0 |      0 | 700
  8 |  100/sec |      300 |   100 |    100 |   0 |      0 | 800
  9 |  100/sec |      200 |   100 |    100 |   0 |      0 | 900
 10 |  100/sec |      100 |   100 |    100 |   0 |      0 | 1000

PAHUNCHE      = 1000 / 1000
CHHOD DIYE    = 0   <- ye customers ko SMS kabhi nahi mila
provider call = 1100  (inme 429 = 100)
```

### Kya DEKHA
```
NAIVE                                          SMART
t=0  bheje 1000 -> 100 OK, 900 ko 429          t=0..2  har second 100 bheje, 100 OK, 0 ko 429
t=1  bheje  900 -> 100 OK, 800 ko 429          t=3,4   provider 50/sec -> 50 OK, 50 ko 429
t=2  bheje  800 -> 100 OK, 700 ko 429                  -> wo 50 PHENKE NAHI, backoff me ruke, queue me
     3rd fail -> 700 PHENK DIYE                t=5..10 100/sec, sab nikal gaye

PAHUNCHE   300 / 1000                          1000 / 1000
PHENKE     700 (kabhi SMS nahi milega)         0
CALL       2700 (2400 bekaar 429)              1100 (sirf 100 ko 429)
TIME       3 sec (jaldi, par galat)            11 sec (dheere, par sab pahunche)
```
Misaal: darwaze se 1 sec me 100 nikal sakte. 1000 ek saath dhakka maarein -> har second 100 nikle,
baaki dhakke khaayein, aur 3 dhakke ke baad ghar chale gaye. SMART = line lagwa do, 100-100 bhejo.

### Nichod
```
THROTTLE   apni raftaar provider ki limit pe baandho (1000 ek saath nahi, 100/sec)
BACKOFF    429 pe turant retry = aur hathoda. 1s, 2s, 4s ruko. Retry-After header aaye to wahi maano.
JITTER     backoff me thoda random fark, taaki saare worker ek hi pal pe wapas na aayein
           (simulation me nahi hai — time poore second me hai)
QUEUE      jo abhi nahi ja sakta wo QUEUE me surakshit ruke. Phenkna nahi. Max try ke baad bhi -> DLQ, drop nahi.
```

### INTERVIEW LINE
```
"Our SMS workers throttle themselves with a token bucket at the provider's rate. On a 429 they back off
 exponentially with jitter and respect Retry-After. Messages wait in the queue, none are dropped -
 after max retries they go to a DLQ. I simulated it: naive retry dropped 700 of 1000, throttle + backoff delivered all."
```

---

[← MASTER SHEET](../../00_MASTER_SHEET.md)
