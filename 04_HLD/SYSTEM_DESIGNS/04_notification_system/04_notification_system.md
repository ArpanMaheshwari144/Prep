# Notification System

> Koi ghatna hui (order, OTP, offer), to sahi user ko, sahi raaste (push / email / SMS) se, sahi waqt pe khabar.
> Dil teen cheezein: ek ghatna, kai raaste, kai log (fanout). Khabar kabhi khoye nahi, do baar jaaye nahi. OTP turant.
>
> Mock me sirf heading + Cue + poster. Poster me sab: upar "Pehle aisa tha", tasveer (ilaaj), hara "Kyun chalta", laal number = kya toot sakta + kaise bacha, peela = doosra tareeka kyun nahi, grey = baaki.
> Cross-Q: hal ke har bade shabd pe socho -> gira? · dheema / bheed? · do ek saath? · galat hua? · kaise / kitna?

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c11_2-ee668a17.svg)

---

## Shuru

```
FR      EK ghatna, KAI raaste: push / email / SMS / app ke andar (Amazon order = sab)
        user ki pasand + message ke saanche (template) bhi
MISAAL  order = push + email, OTP = SMS turant, sale offer = push (ruk sakta) · bahar: analytics dashboard
NFR     ek ghatna kai log / raaste · bhejne wala ruke nahi (async) · khoye nahi (dobara koshish + DLQ)
        bheed jhele (~1 lakh / sec) · OTP ki alag tez line · user ki pasand maano
NUMBERS 10 crore user x ~5 / din = 50 crore / din, din me ~1 lakh sec -> ~5,800 / sec aam din
        sale ke din 10 guna ~58,000 / sec, har ek 3 raaste se -> ~1.75 lakh / sec Kafka me
        machines sale ke hisaab se: ek worker ~1000 / sec -> sale pe ~175 worker (raaste ke hisaab se bante)
        hisaab (kya gaya kya nahi) ~90 TB / saal · Kafka me 7 din ki khabar ~3 TB
PEHLE   sabse simple tha: Order Svc khud seedha email company (Email API) ko call kare.
        Order email ke jawab ka intezaar karta tha, company band to order hi fail
```

> [!IMPORTANT]
> **RAIL: har design isi kram me chalta (8 dabbe, sab file me same)**
> kaam chala → bheed aayi → machine giri → do log ek saath → kuch dheema → data bada → door ke log → bahar ka hamla
>
> | rail | is design me | card |
> |---|---|---|
> | 1 kaam chala | email company band to order hi fail · user ki pasand / raat ki shaanti (**is design ka khaas: ek ghatna, kai raaste, peeche se**) | 1, 11 |
> | 2 bheed aayi | sale me OTP offer ke peeche · laakhon message, company ka 429 · 1 crore ka campaign | 7, 8, 10 |
> | 3 machine giri | worker beech me mara: do email · ya OTP gaya hi nahi | 3, 4 |
> | 4 do log ek saath | wahi khabar do worker ko (dohraav), key se ek hi baar | 3 |
> | 5 kuch dheema | SMS dheemi, push / email atke · har second dobara maara · 30 sec me worker phanse | 2, 5, 6 |
> | 6 data bada | hisaab ~90 TB / saal, "bheja" vs "mila" Cassandra me | 9 |
> | 7 door ke log | is file me nahi aata | - |
> | 8 bahar ka hamla | nakli webhook "pahuncha" (hastakshar jaancho) | 9 |


> [!NOTE]
> **AWS pe kaunsa DB (interview me naam bolna ho to)** · SQL wala = RDS, Cassandra RDS pe nahi chalta
>
> - Tracking (Cassandra) → **Amazon Keyspaces** (ya EC2 pe khud): ~90 TB / saal, likhna bahut, seedhi row
> - Dedup / had (Redis) → **ElastiCache (Redis)**: tez check, key ki umar

---

*ab tootega kahan? order seedha company ke bharose hai*

## 1. Maine order kiya, email company band thi, order hi fail

**Rail 1 KAAM CHALA (khaas)** · Cue: KAFKA (async), order turant jawab

<img src="img/c1_poster.png?v=3" width="1000" alt="card 1: kya toota, kaise bacha">


---

*ab sab raaste ek hi line me, ek dheema to?*

## 2. SMS company dheemi, mera push aur email bhi atke

**Rail 5 KUCH DHEEMA** · Cue: har raaste ki ALAG LINE + worker

<img src="img/c2_poster.png?v=3" width="1000" alt="card 2: kya toota, kaise bacha">


---

*ab worker beech me mare to?*

## 3. Worker mara, mujhe do email aaye

**Rail 4 DO LOG EK SAATH** · Cue: KEY (SET NX, khabar ki apni id)

<img src="img/c3_poster.png?v=3" width="1000" alt="card 3: kya toota, kaise bacha">


---

*ab key lagi, par galat waqt pe*

## 4. Key lag gayi, par mera OTP gaya hi nahi

**Rail 3 MACHINE GIRI** · Cue: SENDING / SENT (do haal)

<img src="img/c4_poster.png?v=3" width="1000" alt="card 4: kya toota, kaise bacha">


---

*ab fail hua to dobara kaise maarein?*

## 5. SMS company pehle se pareshaan, hum har second dobara maar rahe

**Rail 5 KUCH DHEEMA (retry)** · Cue: BACKOFF + JITTER, had ke baad DLQ

<img src="img/c5_poster.png?v=3" width="1000" alt="card 5: kya toota, kaise bacha">


---

*ab company band nahi, bas dheemi*

## 6. SMS company band nahi, dheemi: saare worker 30 sec me phanse

**Rail 5 KUCH DHEEMA (company)** · Cue: TIMEOUT + CIRCUIT BREAKER

<img src="img/c6_poster.png?v=3" width="1000" alt="card 6: kya toota, kaise bacha">


---

*ab sale aayi, bheed me OTP kahan?*

## 7. 12 baje sale shuru, mera OTP 50,000 offer ke peeche

**Rail 2 BHEED AAYI (OTP)** · Cue: OTP ki ALAG TEZ LINE

<img src="img/c7_poster.png?v=3" width="1000" alt="card 7: kya toota, kaise bacha">


---

*ab bheed itni ki company hi mana kare*

## 8. Sale me laakhon message ek saath, company ne 429 diya

**Rail 2 BHEED AAYI (company ki had)** · Cue: SAANJHA TOKEN BUCKET (Redis)

<img src="img/c8_poster.png?v=3" width="1000" alt="card 8: kya toota, kaise bacha">


---

*ab bhej diya, par pahuncha?*

## 9. Company ne "bhej diya" bola, par mere phone tak pahuncha?

**Rail 6 DATA BADA / 8 HAMLA** · Cue: WEBHOOK + company ka message id

<img src="img/c9_poster.png?v=3" width="1000" alt="card 9: kya toota, kaise bacha">


---

*ab ek ke liye nahi, 1 crore ke liye*

## 10. Shaam ko "Diwali offer sabko": 1 crore log

**Rail 2 BHEED AAYI (campaign)** · Cue: 1000 ke TUKDE + campaign row

<img src="img/c10_poster.png?v=3" width="1000" alt="card 10: kya toota, kaise bacha">


---

*ab bheja to sahi, par user chahta tha?*

## 11. Raat 2 baje offer ka SMS, jabki maine band kiya tha

**Rail 1 KAAM CHALA (pasand)** · Cue: PASAND + SHAANTI + SAANCHA (cache)

<img src="img/c11_poster.png?v=3" width="1000" alt="card 11: kya toota, kaise bacha">


---

## Poora design ek saans me

```
Order / Campaign Svc -> Kafka (tez / beech / dheemi) -> Notification Svc (pasand + saancha, cache)
-> push / email / SMS ki line
-> worker (key BHEJ RAHA / BHEJ DIYA · ruk ke dobara · circuit breaker · speed limit)
-> 2 company -> user · webhook -> Tracking DB · fail -> DLQ
DB:  Tracking = Cassandra (likhna bahut, seedhi row, pakki consistency nahi chahiye, aaram se badhta)
     Pasand / saancha = chhota, har ghatna pe padha jaata -> cache
```

Bolne ke liye: services "kuch hua" ki khabar Kafka me daalti. Notification service user ki pasand dekhti, message
bharti, aur push / email / SMS ki alag line me daal deti. Worker key (BHEJ RAHA / BHEJ DIYA) se do baar nahi bhejte, fail pe
ruk-ruk ke dobara, company ki had me. Company dheemi to circuit breaker + doosri company, had ke baad DLQ. OTP ki alag
line. Ek user ki khabar ek hi line me, to kram sahi. Company ke webhook se "mila / fail" update. Aage: raat ka shaant
time, kai bhasha, khola / click ginti.

---

## 10 guna traffic

- **Kafka**: sale pe 10 guna, to lines (partition) pehle se zyada. Line user_id se.
- **Notification Svc**: pasand / saancha cache. Machine badhao.
- **Raaste ki line**: alag line (ho gaya) + tez / dheemi line.
- **Worker**: sale pe badhao (ek worker ~1000 / second). Circuit breaker + do company. Speed limit.
- **Tracking DB**: 90 TB / saal, to Cassandra + row ki umar, purana sasti jagah (cold storage). Umar se jagah ghatti
  hai, baantne se bojh, dono alag cheez.

**Pooche: data badhta ja raha, 3 saal baad?** Kis message ka kya hua (bheja / mila / khola), uski row ki umar rakho
(jaise 90 din), purani rows sasti jagah chali jaayein. Roz wali table chhoti rehti hai.

**Pooche: kahan se gir sakta?** Company (do company), Kafka (copies), Redis (Cassandra LWT peeche).

**Pooche: chal raha hai, kaise pata?** Webhook se delivered / failed, Kafka me kitna pending, DLQ kitna bhara, alert.

---

## Pooche to (kisi card me nahi baitha)

Khabar: Kafka pe { event: "ORDER_PLACED", userId: 123, orderId: 999 }. Andar ke liye REST bhi: POST /notify { userId,
type, channel?, payload }.

Raaste: Push = FCM (Android) / APNs (iPhone). Email = SES / SendGrid / Mailgun. SMS = Twilio / SNS / MSG91. App ke
andar = DB + WebSocket. Phone call = Twilio. Slack webhook.

Pasand: user_id -> kaunse raaste, shaanti ka waqt, kya band kiya. Saancha: template_id -> format, bhasha, khaali jagah
{{name}} {{orderId}}. Hisaab: bheja / mila / khola / click / fail, Cassandra me (bahut likhna, seedhi row, ACID nahi
chahiye).

1 lakh / second: ~100 worker. Line "notifications" P0 -> W1 ... P99 -> W100. Line user_id se, to ek user ki saari
khabar ek line me, kram bana rehta. Warna "ship hua" pehle aur "order hua" baad me.

Khabar khoye hi nahi, ye kaise pakka? Saari copy ne li tab hi OK (acks=all), aur offset kaam ke BAAD.

Poora raasta ek baar: order, Order Svc Kafka me { ORDER_PLACED, 123, 999 }, Notification Svc uthaye, pasand dekhe (push
chalu, email chalu, SMS band), saancha bhare, push + email ki line, worker, company, user, webhook, Tracking DB. Fail to
dobara, phir DLQ.

---

## HANDS-ON — provider ne 429 diya: chala ke dekha

Grill me galti: 429 pe "fail-open" bola. Fail-open HUMARE limiter ka faisla (Redis gira to); yahan limiter
PROVIDER ka, uska darwaza hum nahi khol sakte -> dheema HUMEIN hona padega.

CODE: `04_HLD/HANDS_ON/03_provider_429/Sms429Demo.java` (Docker nahi, time nakli) · line 21 `SMART = false / true` -> `java Sms429Demo.java`

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

JAWAB:    Chala ke dekha: company ki had ka dhyaan na rakha aur turant dobara bheja to 1000 me se 700
          gaye hi nahi.
          Had me bheja aur ruk-ruk ke dobara koshish ki to poore 1000 pahunche.
```

ARCHETYPE B (ingest) · CONCEPTS: [message-queues](../../FOUNDATIONS/07_message_queues.md) · [ms-communication](../../FOUNDATIONS/10_ms_communication.md) · saath: [12 message-queue](../12_message_queue_kafka/12_message_queue_kafka.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
