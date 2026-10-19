# Notification System

> Koi ghatna hui (order, OTP, offer), to sahi user ko, sahi raaste (push / email / SMS) se, sahi waqt pe khabar.
> Dil teen cheezein: ek ghatna, kai raaste, kai log (fanout). Khabar kabhi khoye nahi, do baar jaaye nahi. OTP turant.
>
> Har card: kya toota -> kya kiya -> yahi kyun -> doosra kyun nahi. Kram me chalo. "Pooche" wali line = interviewer aage poochhe to.

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c11_2-ee668a17.svg)

---

## Shuru

Amazon pe order kiya, to phone pe push + email + SMS + app ke andar. EK ghatna, KAI raaste.

Sabse simple: Order service khud seedha email bhejne wali company (Email API) ko call kare.

Pehle poocho: sirf bhejne ki line banani hai, ya user ki pasand + message ke saanche (template) bhi? Kaunse raaste,
push / email / SMS / app ke andar? Ek user ko din me kitne (~5)? Kya sab barabar zaroori? OTP turant chahiye, offer ruk
sakta. Ek hi line me sab daala to OTP 10 minute late, yahi asli fail. Turant chahiye ya thodi der chalegi?

Misaal: order hua to push + email. OTP to SMS turant. Sale offer to push, ruk sakta hai. Bahar: analytics dashboard.

Kaisa chale, 6 cheez: ek ghatna kai log / raaste. Bhejne wala ruke nahi (async). Khoye nahi (dobara koshish + DLQ).
Bheed jhele (~1 lakh / second). OTP ki alag tez line. User ki pasand maano.

Numbers: 10 crore user x 5 / din = 50 crore / din. Din me ~1 lakh second, to ~5,800 har second (aam din). Sale ke din
10 guna, ~58,000 har second. Har ek 3 raaste se, to ~1.75 lakh / second Kafka me. Machines aam din ke hisaab se nahi,
sale ke hisaab se. Ek worker ~1000 / second bhejta, to sale pe ~175 worker (raaste ke hisaab se bante). Kya gaya kya
nahi ka hisaab ~90 TB / saal (Cassandra). Kafka me 7 din ki khabar ~3 TB. Bheed jhatke me aati, isliye beech me line
(Kafka). Hisaab me likhna bahut, isliye NoSQL.

**Pooche: traffic achanak 10 guna ho gaya to?** Sale ke time saari khabar pehle Kafka ki line me jaati, to jhatka line
sambhaal leti, worker apni speed se bhejte. Sale ka time pata hai (12 baje), to 11:50 tak worker badha do. Machine apne
aap badhne me minute lagte, jhatka second me. Par worker utne hi badhao jitna DB aur SMS / email company jhel sake,
warna wahan atak jaayega.

---

*ab tootega kahan? order seedha company ke bharose hai*

## 1. Maine order kiya, email company band thi, order hi fail

Diwali subah order kiya. Order email ke jawab ka intezaar kar raha tha, email company band, order fail.

Isliye order + khabar ek hi DB txn me (outbox), relay use Kafka me daalta. Order ko turant jawab. Notification Svc
peeche Kafka se padh ke email bhejti.

Kafka hi kyun? Order kabhi khabar ka intezaar nahi karta. Khabar disk ke log (Kafka) me, padhne wala yaad rakhta kahan
tak. Company band ho to bhi khabar padi rehti, wapas aate hi wahin se.

Order service me hi retry kar lo? Request utni der atki rehti. Aur Order service giri to retry bhi gaya.

**Pooche: Kafka tak pahunchne se pehle service giri?** Khabar outbox row me padi hai. Relay baad me bhej deta, khoti nahi.

**Pooche: email company band?** Khabar Kafka log me ~7 din padi. Padhne wala kahan tak padha, yaad rakhta (offset).

**Pooche: company 2 din baad aayi?** Wahin se aage. Order kabhi email se fail nahi.

**Pooche: Notification Svc dheemi?** Machine badhao (consumer group). Har line ek machine, lines bat jaati.

**Pooche: Kafka ki machine giri?** Log ki 3 copy. Baaki copy se chalta, khabar nahi khoti.

---

*ab sab raaste ek hi line me, ek dheema to?*

## 2. SMS company dheemi, mera push aur email bhi atke

Push, email, SMS ek hi line me. Worker SMS pe atka, sab atke.

Isliye har raaste ki alag line + alag worker (alag queue). Ghatna aayi to teen chhote message (ghatna id + raasta), har
raasta apni line me.

Ye kyun chalta? Har raasta apni line, apni speed. Ek ka dheema ya band hona doosre pe nahi aata. SMS dheema to sirf SMS
ruka, push + email aa gaye.

SMS worker badha do? Company khud dheemi hai. Line phir bhi ek, push / email phir bhi peeche.

**Pooche: SMS line lambi ho gayi?** Sirf SMS ke worker badhao. Push / email pe asar nahi.

**Pooche: push kis phone pe jaaye?** Device token, install pe user ke saath save. Bekaar nikla to hatao.

---

*ab worker beech me mare to?*

## 3. Worker mara, mujhe do email aaye

Worker-A ne email bheja, "padh liya" bolne se pehle mara. Khabar dobara aayi, Worker-B ne phir bheja. Do email.

Isliye bhejne se pehle khabar ki apni id ki key (idempotency key). Pehle dekho: ye khabar_id + raasta bheja? Redis me
key mili to skip, ek email.

Ye kyun chalta? Khabar ki apni pakki id hai, dobara aaye to pehchaan ke skip. Redis tez raasta, aur DB me (khabar_id,
raasta) ek baar hi lage, wo aakhri deewar.

Har baar naya random id? Dohraav kabhi pakda nahi jaata.

**Pooche: Redis gira?** DB me (khabar_id, raasta) ek baar hi lage (UNIQUE). Pakka wahi rokta.

**Pooche: key 24 ghante me mit gayi?** DB row nahi bhoolti, phir rokega.

**Pooche: DB me ek baar kaise?** Cassandra INSERT IF NOT EXISTS (khabar_id, raasta) (LWT).

---

*ab key lagi, par galat waqt pe*

## 4. Key lag gayi, par mera OTP gaya hi nahi

Koshish se pehle hi key "bhej diya" (SENT) laga di. Call fail. Worker-B ne "bhej diya" dekha aur chhod diya. OTP kho
gaya.

Isliye do haal: bhejte waqt key SENDING (chhoti umar, ~60 sec), company ne OK bola tab SENT. Chhodna sirf SENT pe.

Ye kyun chalta? SENDING = adhoora, chhoti umar se khud mit jaata, to khabar khoti nahi. Kabhi do baar chali jaaye to
chalega: OTP do baar > na aana.

Key hi hata do? Worker mara to Kafka dobara deta, OTP / email do baar. Card 3 wapas.

**Pooche: bheja, jawab aane se pehle worker mara?** Maana hua risk: do baar ja sakta. OTP do baar > na aana.

**Pooche: call fail, worker zinda?** Khud key hatao. Koshish adhoori, agli chal jaaye.

**Pooche: worker hi mara?** Hatane wala koi nahi. SENDING ki umar (60 sec) khatam, key khud mit jaati.

**Pooche: Worker-B ko SENDING mili?** Thoda ruko, phir dekho. Chhodna sirf SENT pe.

**Pooche: SENDING ki umar kitni?** Call (~2-3 sec) se lambi, 60 sec. Pehle khatam hui to do log bhejenge.

---

*ab fail hua to dobara kaise maarein?*

## 5. SMS company pehle se pareshaan, hum har second dobara maar rahe

Fail pe turant dobara, 50 worker har second ek saath. Company pe aur hathoda, kabhi theek nahi hoti.

Isliye har baar zyada der + thoda random (backoff + jitter): 1, 2, 4, 8 sec. Had paar to alag line (DLQ) + alert.

Ye kyun chalta? Fail pe ruk-ruk ke dobara, random thoda sa jodo to sab worker ek hi pal me wapas nahi aate. Khabar
phenkna kabhi nahi. Had ke baad DLQ + alert, theek karke wapas.

Retry band, ek fail = chhodo? Company 2 sec hili to OTP hamesha ke liye gaya.

**Pooche: lamba rukna ho (10 min)?** Alag lines (retry-1m, retry-10m), worker "kab chalana" dekhe.

**Pooche: DLQ wali khabar ka kya?** Dashboard pe dikhe, theek karke main line me wapas.

**Pooche: khabar khoye hi nahi, kaise?** Saari copy ne li tab OK (acks=all). Offset kaam ke BAAD.

---

*ab company band nahi, bas dheemi*

## 6. SMS company band nahi, dheemi: saare worker 30 sec me phanse

Har call 30 sec tak jawab ka intezaar. Saare SMS worker Twilio pe phanse, OTP bhi ruka.

Isliye thodi der me chhodo (2-3 sec timeout). Baar-baar fail to circuit breaker khula: Twilio ko call band, OTP SNS se.
Thodi der baad Twilio ko ek test call, theek to wapas.

Ye kyun chalta? Dheema = band maano. Har company ka apna circuit breaker, to uske band hone se baaki raaste chalte.
Khabar line me rukti, phenki nahi jaati.

Worker badha do? Naye worker bhi 30 sec me phanse. Dheemi company pe aur bojh.

**Pooche: push / email pe asar?** Nahi. Har company ka apna circuit breaker, worker alag line pe.

**Pooche: SNS bhi band?** Khabar line me rukti, backoff se koshish, phenko nahi. OTP ka alert.

**Pooche: circuit breaker har process apna ya saanjha?** Har process apna (Resilience4j), ya Redis me saanjha.

**Pooche: push doosri company se bhej do?** Hota nahi. Android = FCM, iPhone = APNs, phone ka OS tay karta.

---

*ab sale aayi, bheed me OTP kahan?*

## 7. 12 baje sale shuru, mera OTP 50,000 offer ke peeche

Sab ek line me. 50,000 offer, phir mera OTP. 10 min baad pahuncha, time khatam, login fail.

Isliye OTP ki apni tez line + apne worker (priority). High line OTP, beech wali order update, low line offer.

Ye kyun chalta? Zaroori (OTP) kabhi kisi ke peeche nahi: apni line, apne worker, company me apna account.

Ek line me "zaroori" tag laga do? Kafka me "ye pehle bhejo" hota hi nahi.

**Pooche: order update (medium)?** Beech wali line. OTP se peeche, offer se aage.

**Pooche: OTP line khaali?** Uske worker beech wali padh lein. Ulta kabhi nahi.

**Pooche: offer OTP ki company had kha jaaye?** OTP ka alag account / number. Had wala card 8.

---

*ab bheed itni ki company hi mana kare*

## 8. Sale me laakhon message ek saath, company ne 429 diya

Sab worker jitna chahe bhejein. Company ki had 100 / sec, baaki 429, sab fail.

Isliye sab worker ka ek saanjha token bucket (Redis me), har sec 100 token. Pehle token lo, mila to bhejo.

Ye kyun chalta? Had company ki hai, worker ki nahi. Sab worker ek bucket se token lein. Token nahi = ruko, khabar
phenko nahi.

Har worker apni speed 100 / sec? 100 worker = 100 guna, phir 429.

**Pooche: token nahi mila?** Line me intezaar. Ye fail nahi, retry ginti nahi badhti.

**Pooche: worker badh gaye?** Speed wahi. Bucket saanjha, kul 100 / sec.

**Pooche: 1 lakh email, had kam?** Had badhwao + kai account. Tab tak line dheere khaali, warna 429.

**Pooche: bucket wala Redis gira?** Har worker chhoti local had (fail-closed). Company ki had paar nahi.

**Pooche: had kitni hoti?** Company ki: SES shuru ~14 / sec (sandbox 1 / sec), Twilio short code ~100 / sec.

---

*ab bhej diya, par pahuncha?*

## 9. Company ne "bhej diya" bola, par mere phone tak pahuncha?

Company ne "le liya" bola. Aage pata nahi, andhera.

Isliye company baad me khud bataye pahuncha / fail (webhook). Worker company ka message no. (SID-9) save karta. Webhook
aaya "SID-9 pahuncha", Webhook Svc ne SID-9 DELIVERED likha.

Ye kyun chalta? "Bheja" aur "mila" alag haal hain. "Mila" tab likho jab company (asli, signed webhook) bataye.

Baar-baar khud poochte raho? Laakhon message x har kuch second = had + kharcha.

**Pooche: webhook aaya hi nahi?** Sirf ghanton atke ko ek baar poochho (Twilio SID se). SES me poll nahi, sirf event
(SNS / Firehose).

**Pooche: nakli webhook?** Hastakshar jaancho: Twilio HMAC, SES/SNS certificate, SendGrid public key.

**Pooche: hisaab kahan rakhte?** Cassandra (Tracking). Likhna bahut, seedhi row, jodna kuch nahi.

---

*ab ek ke liye nahi, 1 crore ke liye*

## 10. Shaam ko "Diwali offer sabko": 1 crore log

Admin ne "sabko bhejo" daba diya. Ek request me 1 crore ek-ek karke, ghanton atki, OTP bhi peeche.

Isliye 1000-1000 ke tukde, low priority line me. Har tukde ke baad "kahan tak" campaign row me likho (aakhri_id = 3000).

Ye kyun chalta? Bada kaam chhote tukdon me, har tukde ke baad "kahan tak" likha. Gira to wahin se, galat chala to beech
me roko. Ek user ko ek hi baar.

1 crore ko ek bada "sabko bhejo" message? Ek worker pe atka, gira to shuru se.

1 crore alag-alag message? Bojh. 1000 ka tukda = saath bhi, aur dobara karna pade to chhota.

**Pooche: 600 bhej ke worker gira?** Tukda dobara. (campaign_id, user_id) se pehle ke 600 chhoot jaate.

**Pooche: galat offer chal gaya?** Campaign row pe CANCELLED. Worker har tukde se pehle dekhe.

**Pooche: 1 crore me kitni der?** ~17 min (10,000 / sec). Jaan-boojh ke phailaya, had + OTP line bachi.

**Pooche: sabko ek hi push?** FCM topic: ek call. Topic me sirf pasand wale (pasand se sync).

---

*ab bheja to sahi, par user chahta tha?*

## 11. Raat 2 baje offer ka SMS, jabki maine band kiya tha

Pasand dekhi hi nahi. Raat 2 baje offer, ghanti baji, gussa.

Isliye bhejne se pehle user ki pasand dekho (preference + quiet hours): offer SMS band, raat 10 se subah 8 ghanti nahi.
Shaanti me offer aaya to roko, subah 8 baje bhejo (scheduled_at + job), phenko nahi.

Ye kyun chalta? Bhejne se pehle pasand. Offer pasand / shaanti ke neeche, zaroori alert (OTP) upar.

Pasand sirf Campaign Svc me check? Order / OTP wali khabar bina check nikal jaati.

**Pooche: shaanti me OTP / login?** Bhejo. Zaroori alert pasand se upar.

**Pooche: pasand badli, cache purana?** Badalte hi cache delete + chhoti umar. Bhejne se pehle sahi chahiye.

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
