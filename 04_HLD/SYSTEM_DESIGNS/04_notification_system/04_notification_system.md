# Notification System

> Event aaye -> sahi CHANNEL -> sahi USER -> sahi TIME, bharose ke saath, scale pe.
> Is design ka dil: **ek event -> kai channel / kai user (fanout)** + **kabhi khoye nahi, do baar jaaye nahi** + **OTP turant**.

```
Amazon order placed -> Push + Email + SMS + In-app = EK event, KAI channel
SHAADI-CARD: 1 shaadi -> 500 log, sabko alag raaste se nyota:
   family = WhatsApp (in-app) · office = email · padosi = card (SMS) · VIP = phone (priority)
   notification system = shaadi ka planner
```

---

## TASVEER (ByteByteGo / Alex Xu · CC BY-NC-ND 4.0)

![How Does a Typical Push Notification System Work?](https://assets.bytebytego.com/diagrams/0042-design-a-notification-push-system.png)
Source: [How Does a Typical Push Notification System Work?](https://bytebytego.com/guides/how-does-a-typical-push-notification-system-work/)
(poora naksha: event -> notification service -> queue -> worker -> provider FCM / APNs / SMS)

---

## SHURU — poocho + numbers

```
POOCHO:  "Sirf delivery pipeline, ya preferences + template bhi?"
         CHANNEL kaunse? push / email / SMS / in-app · user ko din me kitni? (~5)
         PRIORITY tiers? (OTP turant, marketing ruk sakti)  <- zaroori: ek queue = OTP 10 min late = asli fail
         real-time ya thodi der chalegi?

USE CASE: order placed -> push + email · OTP -> SMS turant · sale offer -> push, ruk sakta
          scope bahar: analytics dashboard

NFR (core 6): FANOUT (1 event -> kai user + channel) · ASYNC (producer ruke na) · RELIABLE (retry + DLQ)
              SCALE (~100K / sec) · PRIORITY (OTP fast lane) · PREFERENCE (user ki marzi)

NUMBERS: 100M user x 5 / din = 500M / din = ~5,800 / sec avg · SALE PEAK 10x = ~58,000 / sec
         fanout se aur: 58K x 3 channel = ~1.75 lakh / sec Kafka me -> partition + worker PEAK pe, avg pe nahi
         1 worker ~1000 / sec -> ~60-100 worker
         tracking ~90 TB / saal (Cassandra) · Kafka 7 din retention ~3 TB
         spiky -> QUEUE (Kafka) · write-heavy tracking -> NoSQL
```
```
POOCHEGA: "What if traffic suddenly spikes 10x?"
BOL:      "Kafka absorbs the burst and workers drain at their own pace. Sale time is known, so I scale
           workers before 12 — autoscaling takes minutes, the spike takes seconds — and cap them at what
           the DB and providers can take."
```

---

## DABBA 0 — sabse simple

```
SOLUTION: Order service seedha email API call kare
```
```
  [ Order Svc ]
    │
    ▼
  [ Email API ]
    │
    ▼
  USER
```

---

## DIKKAT 1 — Order email ke intezaar me ruka, email down = ORDER fail

```
DIKKAT:   tight coupling · order ka response email bhejne tak ruka · koi retry nahi

SOLUTION: beech me QUEUE — Order event Kafka pe daal ke turant laut-ta
          email down -> event queue me pada, wapas aane pe nikal jaayega · order kabhi fail nahi

NAYA:     Kafka · Notification Svc
```
```
  [ Order Svc ]
    │
    ▼
  [ Kafka ]
    │
    ▼
  [ Notification Svc ]
    │
    ▼
  [ Email API ]
    │
    ▼
  USER
```

---

## DIKKAT 2 — teen channel, SMS slow to push / email bhi atke

```
DIKKAT:   har channel ki speed + rate limit alag, ek line me sab

SOLUTION: Notification Svc FANOUT kare -> HAR CHANNEL KI APNI QUEUE + APNE WORKER
          Push -> FCM / APNs · Email -> SES / SendGrid · SMS -> Twilio / AWS SNS
          ek slow channel baaki ko nahi rokta

NAYA:     Push / Email / SMS queue · Push / Email / SMS worker
BADLA:    Email API -> FCM / SES / Twilio (har channel ka provider)
```
```
  [ Order Svc ]
    │
    ▼
  [ Kafka ]
    │
    ▼
  [ Notification Svc ]
    │
    ├──► [ Push queue ]  ──► [ Push worker ]  ──► [ FCM / APNs ]
    ├──► [ Email queue ] ──► [ Email worker ] ──► [ SES ]
    └──► [ SMS queue ]   ──► [ SMS worker ]   ──► [ Twilio ]
```

---

## DIKKAT 3 — user ko SMS chahiye hi nahi tha, aur raat 2 baje bhej diya

```
DIKKAT:   user ki marzi nahi dekhi

SOLUTION: Notification Svc ke andar 3 kaam:
          USER-PREF dekho:   123 = push ON · email ON · SMS OFF · quiet 10pm-8am · unsubscribed topics
          TEMPLATE lo:       "Order #{{orderId}} confirmed" + i18n (Hindi / English)
          CHANNEL tay karo:  push + email (SMS skip)
          pref / template har event pe -> CACHE kar lo

NAYA:     User-pref DB · Template DB
```
```
  [ Order Svc ]
    │
    ▼
  [ Kafka ]
    │
    ▼
  [ Notification Svc ] ──► [ User-pref DB ]
    │         │
    │         └──► [ Template DB ]
    │
    ├──► [ Push queue ]  ──► [ Push worker ]  ──► [ FCM / APNs ]
    ├──► [ Email queue ] ──► [ Email worker ] ──► [ SES ]
    └──► [ SMS queue ]   ──► [ SMS worker ]   ──► [ Twilio ]
```

---

## DIKKAT 4 — ack kho gaya, retry hua, user ko DO email

```
DIKKAT:   bheja -> provider ne liya -> ack raaste me kho gaya -> queue ko laga fail -> RETRY -> 2 email

SOLUTION: IDEMPOTENT WORKER — "at-least-once delivery + idempotent worker"
          SET notification:abc123 sent NX EX 86400
             "OK" -> naya -> BHEJO · nil -> pehle ho chuka -> SKIP   (SET ... NX "OK" deta; "1" purane SETNX ka jawab tha)
          NX = check + set EK atomic step (EXISTS phir SET = beech me doosra ghus jaata)
          key = event ka APNA id (eventId), har retry pe wahi. har baar naya UUID = dedup kabhi nahi pakdega
          (wahi race + wahi ilaaj jo payment idempotency me)

NAYA:     Redis (idempotency key)
```
```
  [ Order Svc ]
    │
    ▼
  [ Kafka ]
    │
    ▼
  [ Notification Svc ] ──► [ User-pref DB ]
    │         │
    │         └──► [ Template DB ]
    │
    ├──► [ Push queue ]  ──► [ Push worker ]  ──► [ FCM / APNs ]
    ├──► [ Email queue ] ──► [ Email worker ] ──► [ SES ]
    └──► [ SMS queue ]   ──► [ SMS worker ]   ──► [ Twilio ]
                                   │
                                   ▼
                              [ Redis ]
```
```
POOCHEGA: "What if the same event comes twice / the worker retries?"
BOL:      "Delivery is at-least-once, so the worker is idempotent: it does SET NX on the event id in Redis
           and skips if the key already exists."
```

---

## DIKKAT 5 — key laga di, par provider call FAIL (29-Sep mock me poocha)

```
DIKKAT:   SET NX -> "OK" · provider call FAIL · retry: SET NX -> nil -> SKIP
          email kabhi gaya hi nahi, system maan raha "bhej diya" = MESSAGE KHO GAYA

SOLUTION: A) fail pe key HATAO (DEL) -> retry dobara bhejega
             kam pakka: worker crash -> DEL chala hi nahi -> key atki
          B) DO HAALAT  <- zyada pakka
             pehle:      SET abc-123 "sending" NX EX 60     (chhoti expiry)
             success pe: SET abc-123 "sent" EX 86400        (lamba)
             worker beech me mara -> 60 sec me "sending" khud mita -> retry chal gaya

NAYA:     koi dabba nahi
```
```
POOCHEGA: "You set the idempotency key, but then the send failed. Now what?"
DHYAAN:   payment me bhi yahi: key lagi, PSP fail -> IN_PROGRESS -> DONE
BOL:      "I don't want the key to block the retry. I set it as 'sending' with a short TTL, and mark it
           'sent' only after the provider accepts. If the send fails or the worker dies, the key expires
           and the retry goes through."
```

---

## DIKKAT 6 — provider fail ho raha, hum turant retry maar rahe

```
DIKKAT:   marte hue provider pe aur hathoda

SOLUTION: BACKOFF + JITTER: 1s -> 2s -> 4s -> 8s · wait = base x 2^n + random(0..1000ms)
          jitter = saare worker ek saath wapas na aayein (thundering herd)
          main queue -> fail -> retry queue (delayed) -> max retry paar -> DLQ -> manual review + ops alert
          DLQ me (poison): galat email (SES reject) · invalid phone · provider ka permanent outage
          Kafka offset commit KAAM (provider call) ke BAAD -> crash = event dobara aayega, khoyega nahi
          producer: acks=all + replication -> event Kafka me hi na khoye

NAYA:     DLQ
```
```
  [ Order Svc ]
    │
    ▼
  [ Kafka ]
    │
    ▼
  [ Notification Svc ] ──► [ User-pref DB ]
    │         │
    │         └──► [ Template DB ]
    │
    ├──► [ Push queue ]  ──► [ Push worker ]  ──► [ FCM / APNs ]
    ├──► [ Email queue ] ──► [ Email worker ] ──► [ SES ]
    └──► [ SMS queue ]   ──► [ SMS worker ]   ──► [ Twilio ]
                                   │
                                   ├──► [ Redis ]
                                   └──► [ DLQ ]
```
```
POOCHEGA: "How do you make sure no message is lost?"
BOL:      "Producers write with acks=all to replicated Kafka. Workers commit the offset only after the
           provider call, so a crash means a redelivery, not a loss — and that's why they're idempotent.
           Retries back off with jitter, and after max retries the message goes to a DLQ, never dropped."
```

---

## DIKKAT 7 — provider slow, saare worker uske 30 sec timeout me phase

```
DIKKAT:   har call 30 sec atka -> poora worker pool kha liya -> system thapp
          slow = down se BURA

SOLUTION: CHHOTA timeout har provider call pe
          MULTI-PROVIDER: SMS = Twilio + AWS SNS · email = SES + SendGrid -> ek down, doosre pe
          CIRCUIT BREAKER: CLOSED --N fail--> OPEN (call band, backup pe) --thodi der--> HALF-OPEN (ek test call)
                           theek -> CLOSED · fail -> wapas OPEN
          circuit khula -> worker us provider ko call hi nahi karta -> phasta nahi

BADLA:    har channel ka ek provider -> do (FCM / APNs · SES + SendGrid · Twilio + SNS)
```
```
  [ Order Svc ]
    │
    ▼
  [ Kafka ]
    │
    ▼
  [ Notification Svc ] ──► [ User-pref DB ]
    │         │
    │         └──► [ Template DB ]
    │
    ├──► [ Push queue ]  ──► [ Push worker ]  ──► [ FCM / APNs ]
    ├──► [ Email queue ] ──► [ Email worker ] ──► [ SES + SendGrid ]
    └──► [ SMS queue ]   ──► [ SMS worker ]   ──► [ Twilio + SNS ]
                                   │
                                   ├──► [ Redis ]
                                   └──► [ DLQ ]
```
```
POOCHEGA: "What if the provider is slow?"
BOL:      "Short timeouts, a circuit breaker per provider, and a second provider per channel. When the
           circuit opens, workers stop calling it and route to the backup instead of hanging."
```

---

## DIKKAT 8 — OTP marketing ke 50,000 message ke peeche

```
DIKKAT:   ek topic me OTP bhi line me

SOLUTION: PRIORITY LANES — har lane ka ALAG Kafka topic + apna worker pool
          HIGH   OTP / 2FA       -> millisecond · notif-high
          MEDIUM order update    -> kuch second  · notif-medium
          LOW    marketing       -> minute-ghanta · notif-low
          Kafka me priority hoti hi nahi, isliye alag topic
          (PriorityBlockingQueue sirf EK process ke andar, distributed me nahi)

BADLA:    Kafka -> 3 topic (high / medium / low)
```
```
  [ Order Svc ]
    │
    ▼
  [ Kafka high / medium / low ]
    │
    ▼
  [ Notification Svc ] ──► [ User-pref DB ]
    │         │
    │         └──► [ Template DB ]
    │
    ├──► [ Push queue ]  ──► [ Push worker ]  ──► [ FCM / APNs ]
    ├──► [ Email queue ] ──► [ Email worker ] ──► [ SES + SendGrid ]
    └──► [ SMS queue ]   ──► [ SMS worker ]   ──► [ Twilio + SNS ]
                                   │
                                   ├──► [ Redis ]
                                   └──► [ DLQ ]
```
```
POOCHEGA: "How do you prioritize urgent work, like OTPs?"
BOL:      "Separate topics per priority with their own worker pools, so an OTP never waits behind a
           marketing blast."
```

---

## DIKKAT 9 — burst gaya, provider ne 429 diya, sab fail

```
DIKKAT:   provider limit: FCM ~6 lakh / min per project (~10K / sec) · SES ~14 / sec (naye account
          ka shuruaati default, badhwa sakte) · Twilio short code ~100 / sec, long code ~1 / sec

SOLUTION: worker KHUD throttle (token bucket) -> provider ki raftaar se bhejo
          429 pe turant retry NAHI -> backoff + jitter · Retry-After maano · message QUEUE me ruke, drop NAHI
          (FAIL-OPEN yahan galat — wo HUMARE limiter ka faisla, provider ka darwaza hum nahi khol sakte)
          chala ke dekha -> HANDS-ON neeche (naive me 1000 me se 700 phenke)

NAYA:     koi dabba nahi — worker me throttle
```
```
POOCHEGA: "The provider returns 429 — you're sending too fast. What now?"
BOL:      "Workers throttle themselves with a token bucket at the provider's rate. On a 429 they back off
           with jitter and respect Retry-After; messages wait in the queue and go to a DLQ after max
           retries, never dropped."
```

---

## DIKKAT 10 — "bhej diya" ka matlab "mil gaya" nahi

```
DIKKAT:   worker ne bheja = ACCEPTED, user tak pahuncha? pata nahi

SOLUTION: provider ka WEBHOOK -> "delivered" / "failed" / "bounced" -> TRACKING DB
          Tracking (Cassandra): sent · delivered · opened · clicked · failed
          failed (galat number / bounce) -> retry · doosra channel · ya mark failed

NAYA:     Tracking DB
```
```
  [ Order Svc ]
    │
    ▼
  [ Kafka high / medium / low ]
    │
    ▼
  [ Notification Svc ] ──► [ User-pref DB ]
    │         │
    │         └──► [ Template DB ]
    │
    ├──► [ Push queue ]  ──► [ Push worker ]  ──► [ FCM / APNs ]
    ├──► [ Email queue ] ──► [ Email worker ] ──► [ SES + SendGrid ]
    └──► [ SMS queue ]   ──► [ SMS worker ]   ──► [ Twilio + SNS ]
                                   │                     │
                                   ├──► [ Redis ]        └──► [ Tracking DB ]
                                   └──► [ DLQ ]
```

---

## 10x SCALE — har dabba alag

```
Kafka            -> peak 10x -> partitions pehle se extra · partition by hash(user_id)
Notification Svc -> pref / template CACHE · box badhao
channel queue    -> alag queue (ho gaya) + priority lane
worker           -> peak pe badhao (1 worker ~1000 / sec) · circuit breaker + multi-provider · throttle
Tracking DB      -> 90 TB / saal -> Cassandra + row pe TTL / purana cold storage
                    (retention size ghatata · shard load baant-ta — dono alag)

POOCHEGA: "Data keeps growing — what happens in 3 years?"
BOL:      "Tracking rows get a TTL and old data moves to cold storage; the live table stays small."

POOCHEGA: "How would you scale this to 10x?"      -> event ka raasta chalo, pehle jo toote
POOCHEGA: "What's the single point of failure?"   -> provider (multi-provider), Kafka (replication)
POOCHEGA: "How do you know it's working?"         -> "bheja" nahi — webhook se delivered / failed ·
                                                     Kafka consumer lag · DLQ size · alert
```

---

## POOCHE TO (deep-dive)

```
EVENT:    producer Kafka pe { event: "ORDER_PLACED", userId: 123, orderId: 999 }
          (andar ke liye REST bhi: POST /notify { userId, type, channel?, payload })

CHANNEL:  Push FCM (Android) / APNs (iOS) · Email SES / SendGrid / Mailgun · SMS Twilio / SNS / MSG91
          In-app DB + WebSocket · Voice Twilio · Slack webhook

DB:       USER-PREF: user_id -> channels · quiet hours · unsubscribed topics
          TEMPLATE:  template_id -> format · i18n · variables {{name}} {{orderId}}
          TRACKING:  sent / delivered / opened / clicked / failed -> Cassandra
                     (bahut + simple + write-heavy, ACID nahi chahiye, horizontal scale)

100K / sec:  ~100 worker · topic "notifications" P0 -> W1 ... P99 -> W100
             partition by hash(user_id) -> ek user ke event EK partition -> ORDER bana rahe
             (warna "shipped" pehle, "placed" baad me)

WALKTHROUGH:  order -> Order Svc Kafka pe { ORDER_PLACED, 123, 999 } -> Notif Svc uthaye -> pref
              (push ON, email ON, SMS OFF) -> template bhare -> push + email queue -> worker -> provider
              -> user -> webhook -> Tracking DB · fail -> retry -> DLQ
```

---

## AAKHRI DABBA + WRAP

```
Kafka = decouple + spike + priority topic · Notification Svc = pref + template + fanout
channel queue = apni speed · worker = idempotent (SET NX) + backoff / jitter + circuit breaker + throttle
DLQ = poison baaki ko na roke · Tracking DB = "bheja" vs "mila"
```
```
  [ Order Svc ]
    │
    ▼
  [ Kafka high / medium / low ]
    │
    ▼
  [ Notification Svc ] ──► [ User-pref DB ]
    │         │
    │         └──► [ Template DB ]
    │
    ├──► [ Push queue ]  ──► [ Push worker ]  ──► [ FCM / APNs ]
    ├──► [ Email queue ] ──► [ Email worker ] ──► [ SES + SendGrid ]
    └──► [ SMS queue ]   ──► [ SMS worker ]   ──► [ Twilio + SNS ]
                                   │                     │
                                   ├──► [ Redis ]        └──► [ Tracking DB ]
                                   └──► [ DLQ ]
```
```
BOL: "Services publish events to Kafka. The notification service checks preferences, fills the template
      and fans out to per-channel queues. Workers are idempotent with Redis SET NX, retry with backoff and
      jitter, throttle to the provider's limit, use a circuit breaker with a backup provider, and send to
      a DLQ after max retries. OTPs have their own topic. Partitioning by user id keeps per-user order,
      and provider webhooks update the tracking DB. Next: quiet hours, i18n templates, open / click analytics."
     (asli duniya: Uber ride notification · Amazon order update · Slack · WhatsApp · bank alert)
```

---

## HANDS-ON — provider ne 429 diya: chala ke dekha (30-Sep)

> Grill me galti: 429 pe "fail-open" bola. Fail-open HUMARE limiter ka faisla (Redis gira to); yahan limiter
> PROVIDER ka, uska darwaza hum nahi khol sakte -> dheema HUMEIN hona padega.
> CODE: `04_HLD/HANDS_ON/03_provider_429/Sms429Demo.java` (Docker nahi, time nakli) · line 21 `SMART = false / true` -> `java Sms429Demo.java`

```
SETUP:  1000 SMS ek saath · provider 100 / sec · t=3,4 pe 50 / sec
NAIVE:  sab ek saath, 429 pe agle second turant retry, 3 fail -> PHENK DO
SMART:  apni taraf 100 / sec (token bucket) · 429 pe 1s, 2s, 4s · kabhi phenko mat
```
```
NAIVE      t | provider | queue | bheje | 200 | 429 | chhode | pahunche
           0 |  100/sec |  1000 |  1000 | 100 | 900 |      0 | 100
           1 |  100/sec |   900 |   900 | 100 | 800 |      0 | 200
           2 |  100/sec |   800 |   800 | 100 | 700 |    700 | 300
           PAHUNCHE 300 / 1000 · CHHODE 700 · call 2700 (429 = 2400)

SMART      t | provider | queue | bheje | 200 | 429 | chhode | pahunche
           0 |  100/sec |  1000 |   100 | 100 |   0 |      0 | 100
           1 |  100/sec |   900 |   100 | 100 |   0 |      0 | 200
           2 |  100/sec |   800 |   100 | 100 |   0 |      0 | 300
           3 |   50/sec |   700 |   100 |  50 |  50 |      0 | 350
           4 |   50/sec |   650 |   100 |  50 |  50 |      0 | 400
           5 |  100/sec |   600 |   100 | 100 |   0 |      0 | 500
           6 |  100/sec |   500 |   100 | 100 |   0 |      0 | 600
           7 |  100/sec |   400 |   100 | 100 |   0 |      0 | 700
           8 |  100/sec |   300 |   100 | 100 |   0 |      0 | 800
           9 |  100/sec |   200 |   100 | 100 |   0 |      0 | 900
          10 |  100/sec |   100 |   100 | 100 |   0 |      0 | 1000
           PAHUNCHE 1000 / 1000 · CHHODE 0 · call 1100 (429 = 100)

           NAIVE 3 sec, jaldi par 700 kabhi nahi milenge · SMART 11 sec, dheere par sab pahunche
DARWAZA:   1 sec me 100 nikal sakte · 1000 dhakka maarein -> 100 nikle, baaki 3 dhakke ke baad ghar
           SMART = line lagwao, 100-100
```
```
THROTTLE  raftaar provider ki limit pe baandho
BACKOFF   429 pe 1s, 2s, 4s · Retry-After aaye to wahi
JITTER    thoda random farak (simulation me nahi, time poore second me)
QUEUE     jo abhi nahi ja sakta wo queue me ruke · max try ke baad DLQ, drop nahi

BOL: "I simulated it: naive retry dropped 700 of 1000, throttle plus backoff delivered all 1000."
```

ARCHETYPE B (ingest) · CONCEPTS: [message-queues](../../FOUNDATIONS/07_message_queues.md) · [ms-communication](../../FOUNDATIONS/10_ms_communication.md) · saath: [13 message-queue](../13_message_queue_kafka/13_message_queue_kafka.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
