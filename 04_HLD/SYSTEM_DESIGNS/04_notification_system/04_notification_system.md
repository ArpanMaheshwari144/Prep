# Notification System

> Koi ghatna hui (order, OTP, offer), to sahi user ko, sahi raaste (push / email / SMS) se, sahi waqt pe khabar.
> Dil teen cheezein: ek ghatna, kai raaste, kai log (fanout). Khabar kabhi khoye nahi, do baar jaaye nahi. OTP turant.
>
> Mock me sirf heading + Cue + GIF. Neeche har card: **hal** · **kyun** · **nahi** (doosra kyun nahi) · **Pooche**.

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c11_2-ee668a17.svg)

---

## Shuru

- **FR:** EK ghatna, KAI raaste: push / email / SMS / app ke andar (Amazon order = sab) · user ki pasand + message ke saanche (template) bhi · misaal: order = push + email, OTP = SMS turant, sale offer = push, ruk sakta · bahar: analytics dashboard
- **NFR:** ek ghatna kai log / raaste · bhejne wala ruke nahi (async) · khoye nahi (dobara koshish + DLQ) · bheed jhele (~1 lakh / sec) · OTP ki alag tez line · user ki pasand maano
- **numbers:** 10 crore user x ~5 / din = 50 crore / din, din me ~1 lakh sec -> ~5,800 / sec aam din · sale ke din 10 guna ~58,000 / sec, har ek 3 raaste se -> ~1.75 lakh / sec Kafka me · machines sale ke hisaab se: ek worker ~1000 / sec -> sale pe ~175 worker (raaste ke hisaab se bante) · hisaab (kya gaya kya nahi) ~90 TB / saal · Kafka me 7 din ki khabar ~3 TB

---

*ab tootega kahan? order seedha company ke bharose hai*

## 1. Maine order kiya, email company band thi, order hi fail

**Cue:** KAFKA (async), order turant jawab · dabba: Kafka + Notification Svc

- **pehle:** sabse simple: Order service khud seedha email bhejne wali company (Email API) ko call kare. Order email ke jawab ka intezaar karta, company band to order fail.
- **hal:** order + khabar ek hi DB txn me (outbox), relay use Kafka me daalta. Order ko turant jawab. Notification Svc peeche Kafka se padh ke email bhejti.
- **kyun:** order kabhi khabar ka intezaar nahi karta. Khabar disk ke log (Kafka) me, padhne wala yaad rakhta kahan tak. Company band to bhi khabar padi, wapas aate hi wahin se.
- **kyun:** bheed jhatke me aati, isliye beech me line (Kafka)
- **nahi:** Order service me hi retry -> request utni der atki rehti. Aur Order service giri to retry bhi gaya.
- **Pooche:** Kafka tak pahunchne se pehle service giri? -> khabar outbox row me padi. Relay baad me bhej deta, khoti nahi.
- **Pooche:** email company band? -> khabar Kafka log me ~7 din padi. Padhne wala kahan tak padha, yaad rakhta (offset).
- **Pooche:** company 2 din baad aayi? -> wahin se aage. Order kabhi email se fail nahi.
- **Pooche:** Notification Svc dheemi? -> machine badhao (consumer group). Har line ek machine, lines bat jaati.
- **Pooche:** Kafka ki machine giri? -> log ki 3 copy. Baaki copy se chalta, khabar nahi khoti.
- **Pooche:** traffic achanak 10 guna? -> saari khabar pehle Kafka line me, jhatka line sambhaalti, worker apni speed se. Sale ka time pata (12 baje), to 11:50 tak worker badha do; machine apne aap badhne me minute, jhatka second me. Par worker utne hi jitna DB aur SMS / email company jhel sake, warna wahan atkega.

---

*ab sab raaste ek hi line me, ek dheema to?*

## 2. SMS company dheemi, mera push aur email bhi atke

**Cue:** har raaste ki ALAG LINE + worker · dabba: Push / Email / SMS queue

- **hal:** har raaste ki alag line + alag worker (alag queue). Ghatna aayi to teen chhote message (ghatna id + raasta), har raasta apni line me.
- **kyun:** pehle push, email, SMS ek hi line me, worker SMS pe atka to sab atke. Ab har raasta apni line, apni speed. SMS dheema to sirf SMS ruka, push + email aa gaye.
- **nahi:** SMS worker badha do -> company khud dheemi hai. Line phir bhi ek, push / email phir bhi peeche.
- **Pooche:** SMS line lambi ho gayi? -> sirf SMS ke worker badhao. Push / email pe asar nahi.
- **Pooche:** push kis phone pe jaaye? -> device token, install pe user ke saath save. Bekaar nikla to hatao.

---

*ab worker beech me mare to?*

## 3. Worker mara, mujhe do email aaye

**Cue:** KEY (SET NX, khabar ki apni id) · dabba: Redis

- **hal:** bhejne se pehle khabar ki apni id ki key (idempotency key): ye khabar_id + raasta bheja? Redis me key mili to skip, ek email.
- **kyun:** pehle Worker-A ne email bheja, "padh liya" bolne se pehle mara, khabar dobara aayi, Worker-B ne phir bheja. Ab pakki id se dobara aaye to pehchaan ke skip.
- **kyun:** Redis tez raasta, aur DB me (khabar_id, raasta) ek baar hi lage, wo aakhri deewar
- **nahi:** har baar naya random id -> dohraav kabhi pakda nahi jaata
- **Pooche:** Redis gira? -> DB me (khabar_id, raasta) ek baar hi lage (UNIQUE). Pakka wahi rokta.
- **Pooche:** key 24 ghante me mit gayi? -> DB row nahi bhoolti, phir rokega
- **Pooche:** DB me ek baar kaise? -> Cassandra INSERT IF NOT EXISTS (khabar_id, raasta) (LWT)

---

*ab key lagi, par galat waqt pe*

## 4. Key lag gayi, par mera OTP gaya hi nahi

**Cue:** SENDING / SENT (do haal) · dabba: (Redis key)

- **hal:** do haal: bhejte waqt key SENDING (chhoti umar, ~60 sec), company ne OK bola tab SENT. Chhodna sirf SENT pe.
- **kyun:** pehle koshish se pehle hi SENT laga di, call fail, Worker-B ne SENT dekh ke chhod diya, OTP kho gaya. SENDING = adhoora, chhoti umar se khud mit jaata, to khabar khoti nahi.
- **kyun:** kabhi do baar chali jaaye to chalega: OTP do baar > na aana
- **nahi:** key hi hata do -> worker mara to Kafka dobara deta, OTP / email do baar. Card 3 wapas.
- **Pooche:** bheja, jawab aane se pehle worker mara? -> maana hua risk: do baar ja sakta. OTP do baar > na aana.
- **Pooche:** call fail, worker zinda? -> khud key hatao. Koshish adhoori, agli chal jaaye.
- **Pooche:** worker hi mara? -> hatane wala koi nahi. SENDING ki umar (60 sec) khatam, key khud mit jaati.
- **Pooche:** Worker-B ko SENDING mili? -> thoda ruko, phir dekho. Chhodna sirf SENT pe.
- **Pooche:** SENDING ki umar kitni? -> call (~2-3 sec) se lambi, 60 sec. Pehle khatam hui to do log bhejenge.

---

*ab fail hua to dobara kaise maarein?*

## 5. SMS company pehle se pareshaan, hum har second dobara maar rahe

**Cue:** BACKOFF + JITTER, had ke baad DLQ · dabba: DLQ

- **hal:** har baar zyada der + thoda random (backoff + jitter): 1, 2, 4, 8 sec. Had paar to alag line (DLQ) + alert.
- **kyun:** pehle fail pe turant dobara, 50 worker har second ek saath, company pe aur hathoda, kabhi theek nahi hoti. Ab ruk-ruk ke, random thoda sa jodo to sab worker ek hi pal me wapas nahi aate.
- **kyun:** khabar phenkna kabhi nahi. Had ke baad DLQ + alert, theek karke wapas.
- **nahi:** retry band, ek fail = chhodo -> company 2 sec hili to OTP hamesha ke liye gaya
- **Pooche:** lamba rukna ho (10 min)? -> alag lines (retry-1m, retry-10m), worker "kab chalana" dekhe
- **Pooche:** DLQ wali khabar ka kya? -> dashboard pe dikhe, theek karke main line me wapas
- **Pooche:** khabar khoye hi nahi, kaise? -> saari copy ne li tab OK (acks=all). Offset kaam ke BAAD.

---

*ab company band nahi, bas dheemi*

## 6. SMS company band nahi, dheemi: saare worker 30 sec me phanse

**Cue:** TIMEOUT + CIRCUIT BREAKER · dabba: Twilio + SNS, SendGrid

- **hal:** thodi der me chhodo (2-3 sec timeout). Baar-baar fail to circuit breaker khula: Twilio ko call band, OTP SNS se. Thodi der baad Twilio ko ek test call, theek to wapas.
- **kyun:** pehle har call 30 sec tak intezaar, saare SMS worker Twilio pe phanse, OTP bhi ruka. Dheema = band maano.
- **kyun:** har company ka apna circuit breaker, to uske band hone se baaki raaste chalte. Khabar line me rukti, phenki nahi jaati.
- **nahi:** worker badha do -> naye worker bhi 30 sec me phanse. Dheemi company pe aur bojh.
- **Pooche:** push / email pe asar? -> nahi. Har company ka apna circuit breaker, worker alag line pe.
- **Pooche:** SNS bhi band? -> khabar line me rukti, backoff se koshish, phenko nahi. OTP ka alert.
- **Pooche:** circuit breaker har process apna ya saanjha? -> har process apna (Resilience4j), ya Redis me saanjha
- **Pooche:** push doosri company se bhej do? -> hota nahi. Android = FCM, iPhone = APNs, phone ka OS tay karta.

---

*ab sale aayi, bheed me OTP kahan?*

## 7. 12 baje sale shuru, mera OTP 50,000 offer ke peeche

**Cue:** OTP ki ALAG TEZ LINE · dabba: Kafka high / med / low

- **hal:** OTP ki apni tez line + apne worker (priority). High line OTP, beech wali order update, low line offer.
- **kyun:** sab barabar zaroori nahi: OTP turant, offer ruk sakta. Ek hi line me sab daala to OTP 10 min late, time khatam, login fail, yahi asli fail.
- **kyun:** zaroori (OTP) kabhi kisi ke peeche nahi: apni line, apne worker, company me apna account
- **nahi:** ek line me "zaroori" tag laga do -> Kafka me "ye pehle bhejo" hota hi nahi
- **Pooche:** order update (medium)? -> beech wali line. OTP se peeche, offer se aage.
- **Pooche:** OTP line khaali? -> uske worker beech wali padh lein. Ulta kabhi nahi.
- **Pooche:** offer OTP ki company had kha jaaye? -> OTP ka alag account / number. Had wala card 8.

---

*ab bheed itni ki company hi mana kare*

## 8. Sale me laakhon message ek saath, company ne 429 diya

**Cue:** SAANJHA TOKEN BUCKET (Redis) · dabba: (Redis bucket)

- **hal:** sab worker ka ek saanjha token bucket (Redis me), har sec 100 token. Pehle token lo, mila to bhejo.
- **kyun:** had company ki hai (100 / sec, baaki 429), worker ki nahi. Sab worker ek bucket se token lein. Token nahi = ruko, khabar phenko nahi.
- **nahi:** har worker apni speed 100 / sec -> 100 worker = 100 guna, phir 429
- **Pooche:** token nahi mila? -> line me intezaar. Ye fail nahi, retry ginti nahi badhti.
- **Pooche:** worker badh gaye? -> speed wahi. Bucket saanjha, kul 100 / sec.
- **Pooche:** 1 lakh email, had kam? -> had badhwao + kai account. Tab tak line dheere khaali, warna 429.
- **Pooche:** bucket wala Redis gira? -> har worker chhoti local had (fail-closed). Company ki had paar nahi.
- **Pooche:** had kitni hoti? -> company ki: SES shuru ~14 / sec (sandbox 1 / sec), Twilio short code ~100 / sec

---

*ab bhej diya, par pahuncha?*

## 9. Company ne "bhej diya" bola, par mere phone tak pahuncha?

**Cue:** WEBHOOK + company ka message id · dabba: Webhook + Tracking DB

- **hal:** company baad me khud bataye pahuncha / fail (webhook). Worker company ka message no. (SID-9) save karta. Webhook aaya "SID-9 pahuncha", Webhook Svc ne SID-9 DELIVERED likha.
- **kyun:** company ne sirf "le liya" bola, aage andhera. "Bheja" aur "mila" alag haal. "Mila" tab likho jab company (asli, signed webhook) bataye.
- **nahi:** baar-baar khud poochte raho -> laakhon message x har kuch second = had + kharcha
- **Pooche:** webhook aaya hi nahi? -> sirf ghanton atke ko ek baar poochho (Twilio SID se). SES me poll nahi, sirf event (SNS / Firehose).
- **Pooche:** nakli webhook? -> hastakshar jaancho: Twilio HMAC, SES/SNS certificate, SendGrid public key
- **Pooche:** hisaab kahan rakhte? -> Cassandra (Tracking). Likhna bahut, isliye NoSQL: seedhi row, jodna kuch nahi.

---

*ab ek ke liye nahi, 1 crore ke liye*

## 10. Shaam ko "Diwali offer sabko": 1 crore log

**Cue:** 1000 ke TUKDE + campaign row · dabba: Campaign Svc + DB

- **hal:** 1000-1000 ke tukde, low priority line me. Har tukde ke baad "kahan tak" campaign row me likho (aakhri_id = 3000).
- **kyun:** pehle admin ne "sabko bhejo" dabaya, ek request me 1 crore ek-ek, ghanton atki, OTP bhi peeche. Ab bada kaam chhote tukdon me: gira to wahin se, galat chala to beech me roko. Ek user ko ek hi baar.
- **nahi:** 1 crore ko ek bada "sabko bhejo" message -> ek worker pe atka, gira to shuru se
- **nahi:** 1 crore alag-alag message -> bojh. 1000 ka tukda = saath bhi, aur dobara karna pade to chhota.
- **Pooche:** 600 bhej ke worker gira? -> tukda dobara. (campaign_id, user_id) se pehle ke 600 chhoot jaate.
- **Pooche:** galat offer chal gaya? -> campaign row pe CANCELLED. Worker har tukde se pehle dekhe.
- **Pooche:** 1 crore me kitni der? -> ~17 min (10,000 / sec). Jaan-boojh ke phailaya, had + OTP line bachi.
- **Pooche:** sabko ek hi push? -> FCM topic: ek call. Topic me sirf pasand wale (pasand se sync).

---

*ab bheja to sahi, par user chahta tha?*

## 11. Raat 2 baje offer ka SMS, jabki maine band kiya tha

**Cue:** PASAND + SHAANTI + SAANCHA (cache) · dabba: User-pref + Template DB

- **hal:** bhejne se pehle user ki pasand (preference + quiet hours): offer SMS band, raat 10 se subah 8 ghanti nahi. Shaanti me offer aaya to roko, subah 8 baje bhejo (scheduled_at + job), phenko nahi.
- **kyun:** pehle pasand dekhi hi nahi, ghanti baji, gussa. Ab bhejne se pehle pasand. Offer pasand / shaanti ke neeche, zaroori alert (OTP) upar.
- **nahi:** pasand sirf Campaign Svc me check -> order / OTP wali khabar bina check nikal jaati
- **Pooche:** shaanti me OTP / login? -> bhejo. Zaroori alert pasand se upar.
- **Pooche:** pasand badli, cache purana? -> badalte hi cache delete + chhoti umar. Bhejne se pehle sahi chahiye.

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
