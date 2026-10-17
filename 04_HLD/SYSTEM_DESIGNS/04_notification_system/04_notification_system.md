# Notification System

> Koi ghatna hui (order, OTP, offer) -> sahi user ko, sahi raaste (push / email / SMS) se, sahi waqt pe khabar.
> Is design ka dil teen cheezein hain: **ek ghatna -> kai raaste, kai log (fanout)** · **khabar kabhi khoye nahi, do baar jaaye nahi** · **OTP turant**.

```
Amazon pe order -> phone pe push + email + SMS + app ke andar = EK ghatna, KAI raaste.
SHAADI KA CARD: ek shaadi, 500 mehmaan, sabko alag raaste se nyota:
   ghar wale = WhatsApp · office wale = email · padosi = chhapa card · VIP = phone karke
   notification system = shaadi ka planner
```

---

## TASVEER (ByteByteGo / Alex Xu · CC BY-NC-ND 4.0)

![How Does a Typical Push Notification System Work?](https://assets.bytebytego.com/diagrams/0042-design-a-notification-push-system.png)
Source: [How Does a Typical Push Notification System Work?](https://bytebytego.com/guides/how-does-a-typical-push-notification-system-work/)
(poora naksha: ghatna -> notification service -> queue -> worker -> bhejne wali company FCM / APNs / SMS)

---

## SHURU: pehle poocho, phir numbers

```
POOCHO:  "Sirf bhejne ki line banani hai, ya user ki pasand + message ke saanche (template) bhi?"
         kaunse raaste? push / email / SMS / app ke andar · ek user ko din me kitne? (~5)
         kya sab barabar zaroori? (OTP turant, offer ruk sakta)  <- ek hi line me sab = OTP 10 minute late = asli fail
         turant chahiye ya thodi der chalegi?

MISAAL:  order hua -> push + email · OTP -> SMS turant · sale offer -> push, ruk sakta hai
         bahar: analytics dashboard

KAISA CHALE (NFR, 6 cheez):
   ek ghatna kai log / raaste · bhejne wala ruke nahi (async) · khoye nahi (dobara koshish + DLQ)
   bheed jhele (~1 lakh / second) · OTP ki alag tez line · user ki pasand maano

NUMBERS (andaaza):
   10 crore user x 5 / din = 50 crore / din -> din me ~1 lakh second -> ~5,800 har second (aam din)
   sale ke din 10 guna -> ~58,000 har second
   har ek 3 raaste se -> ~1.75 lakh / second Kafka me. Machines aam din ke hisaab se nahi, sale ke hisaab se.
   ek worker ~1000 / second bhejta -> sale pe ~175 worker (raaste ke hisaab se bante)
   kya gaya kya nahi ka hisaab ~90 TB / saal (Cassandra) · Kafka me 7 din ki khabar ~3 TB
   bheed jhatke me aati -> beech me line (Kafka) · hisaab me likhna bahut -> NoSQL
```
```
POOCHEGA: "What if traffic suddenly spikes 10x?"
BOL:      "Kafka absorbs the burst and workers drain at their own pace. Sale time is known, so I scale
           workers before 12 — autoscaling takes minutes, the spike takes seconds — and cap them at what
           the DB and providers can take."
```

---

## ════ MOCK SE PEHLE: SIRF YE CARD PADHO ════

> Har dikkat ek card: KAHANI (kya hua, kaun, kitna) -> ILAAJ -> ANDAR KAISE (ilaaj kyun kaam karta) ->
> ISKE BAAD KYA TOOTEGA (interviewer yahin ghusta hai: dabba lagaya, ab kya?).

#### DABBA 0: SABSE SIMPLE (yahan se shuru, phir "kahan tootega")
```
Order service khud seedha email bhejne wali company (Email API) ko call kare.
```
```mermaid
flowchart TD
    n_Order_Svc["Order Svc"]
    n_Email_API["Email API"]
    n_USER["USER"]
    n_Order_Svc --> n_Email_API
    n_Email_API --> n_USER
```

#### [1](#d1) ORDER EMAIL KE INTEZAAR ME RUKA, EMAIL DOWN = ORDER FAIL
```
KAHANI:  Arpan ne order kiya. Order service ne email bheja aur jawab ka intezaar karti rahi.
         Ek din email company band thi -> order hi fail ho gaya. Aur email dobara bhejne wala koi nahi.

ILAAJ:   Beech me KAFKA. Order service bas "order hua" ki khabar Kafka me daale aur turant laute.
         Notification service peeche se Kafka padh ke email bheje.

ANDAR KAISE:
         Kafka ek diary hai jo disk pe rehti hai. Notification service yaad rakhti hai kahan tak padha (offset).
         Email band hai -> wo aage nahi badhti. Email wapas aaya -> wahin se shuru. Khabar ~7 din diary me padi rehti hai.

ISKE BAAD KYA TOOTEGA:
  - Order DB me ban gaya, par Kafka me likhne se PEHLE Order service gir gayi?
      -> khabar gayi hi nahi. Ilaaj OUTBOX: order aur khabar EK hi DB transaction me (outbox table),
         ek relay baad me outbox se Kafka bheje.
  - Notification service ki kai machine hain, Kafka kaise baante?
      -> sab ek team (consumer group) me, har line (partition) ek machine ko. Machine badhao, lines bat jaati hain.
  - Order service me hi dobara koshish (retry) kyun nahi?
      -> order ki request utni der atki rehti. Aur Order service giri to retry bhi gaya.
```

#### [2](#d2) TEEN RAASTE, SMS DHEEMA TO PUSH / EMAIL BHI ATKE
```
KAHANI:  Push, email, SMS teeno ek hi line me khade the. SMS company dheemi hui ->
         SMS wale atke, aur unke peeche push aur email bhi khade reh gaye.

ILAAJ:   Har raaste ki apni line (Kafka topic) aur apne worker.
         push -> FCM / APNs · email -> SES · SMS -> Twilio.

ANDAR KAISE:
         SMS dheema -> sirf SMS ki line lambi hui. Email worker apni speed se chalte rahe.
         Worker alag ginti me (email ke 10, SMS ke 3) -> jiski zaroorat ho wahi badhao.

ISKE BAAD KYA TOOTEGA:
  - Ek ghatna ke liye teeno raaste chahiye, to teen baar banegi?
      -> Notification service ek ghatna se teen chhote message banati hai, har line me ek.
         Har message ki pehchaan = ghatna ki id + raasta.
  - Push kis phone pe bhejein?
      -> app install hote hi phone ka pata (device token) user ke saath save. Pata bekaar nikla -> hata do.
```

#### [3](#d3) SMS BAND THA, PHIR BHI RAAT 2 BAJE BHEJA
```
KAHANI:  Arpan ne SMS band kiya tha aur raat 10 se subah 8 "mat bhejo" lagaya tha. Phir bhi raat 2 baje SMS aaya.

ILAAJ:   Bhejne se pehle teen cheez dekho: user ki PASAND (kaunsa raasta chalu, kab mat bhejo, kya band kiya)
         -> message ka SAANCHA (template, sahi bhasha me) -> phir raasta.

ANDAR KAISE:
         Arpan = push chalu, email chalu, SMS band, raat 10-8 shaanti -> sirf push + email, SMS chhodo.
         Pasand aur saancha har ghatna pe chahiye -> unhe cache me rakho.

ISKE BAAD KYA TOOTEGA:
  - Shaanti ke waqt me khabar aayi, to phenk doge?
      -> nahi. Offer ko subah tak rok lo. OTP / security wali shaanti me bhi jaati hai.
  - Arpan ne pasand badli, cache purana hai?
      -> pasand badalte hi cache se hatao, aur cache ki umar chhoti rakho.
```

#### [4](#d4) JAWAB KHO GAYA, DOBARA BHEJA, USER KO DO EMAIL
```
KAHANI:  Worker ne email company ko bheja. Company ne email bhej bhi diya, par uska "ho gaya" wala jawab
         raaste me kho gaya. Worker ko laga fail hua -> dobara bheja -> Arpan ko DO email.

ILAAJ:   Bhejne se pehle Redis me ek parchi lagao: "ye khabar le li" (SET khabar_ki_id NX).
         Parchi lag gayi -> bhejo. Pehle se lagi thi -> koi aur bhej chuka, chhodo.

ANDAR KAISE:
         Kafka ek khabar kabhi-kabhi do baar de deta hai. Isliye parchi khabar ki APNI id se lagti hai,
         jo har baar wahi rehti hai. Har baar naya random id banaya to dohraav kabhi pakda hi nahi jaayega.

ISKE BAAD KYA TOOTEGA:
  - Redis hi gira?
      -> DB me bhi niyam: (khabar_ki_id, raasta) ek hi baar (UNIQUE). Redis tez raasta, DB pakka.
  - Parchi 24 ghante me mit gayi, aur wahi khabar phir aa gayi?
      -> dobara chali jaayegi. Pakka rokna DB ka UNIQUE niyam karta hai.
```

#### [5](#d5) PARCHI LAGA DI, PAR BHEJNA FAIL HO GAYA
```
KAHANI:  Arpan ka OTP bhejna hai. Worker-A ne Redis me parchi lagayi "OTP-123: bhej diya", phir SMS company ko call kiya.
         Call FAIL ho gayi (ya Worker-A beech me hi mar gaya). OTP gaya hi nahi.
         Kafka ne khabar dobara di, Worker-B aaya, parchi dekhi "bhej diya" -> chhod diya.
         OTP kabhi nahi gaya, aur system soch raha hai bhej diya. KHABAR KHO GAYI.

DIKKAT KI JAD:  parchi ek hi baat bolti thi, "bhej diya". Par asal me Worker-A ne sirf KOSHISH shuru ki thi.

ILAAJ:   Parchi pe do alag baatein likho:
           bhejne se PEHLE:      "BHEJ RAHA HOON" (SENDING) + chhoti umar, 60 second
           company ne OK bola:   "BHEJ DIYA" (SENT) + lambi umar, 1 din

ANDAR KAISE (har haalat me kya hota):
   Worker-A ka call fail, par Worker-A zinda   -> wo khud parchi hata deta hai -> agli koshish chal jaati hai.
   Worker-A beech me hi MAR gaya               -> "BHEJ RAHA HOON" 60 second me khud mit jaati hai -> agli koshish chal jaati hai.
   Worker-B ko "BHEJ RAHA HOON" mila           -> koi abhi bhej raha hai, thoda ruko, phir dekho.
   Worker-B ko "BHEJ DIYA" mila                -> pakka chala gaya -> chhodo.
   Ek line me: CHHODNA sirf "BHEJ DIYA" pe. "BHEJ RAHA HOON" = adhoora, pata nahi gaya ya nahi.

ISKE BAAD KYA TOOTEGA:
  - "Fail hua to parchi hata do" itna kaafi kyun nahi?
      -> worker hi mar gaya to hatane wala koi nahi bacha. Isliye "BHEJ RAHA HOON" ki umar chhoti, khud mit jaaye.
  - 60 second chhote pad gaye (company 70 second me jawab de)?
      -> umar hamesha company ke timeout se lambi rakho (timeout 10 second -> umar 60 second).
  - Company ne bhej diya tha, bas jawab aane se pehle worker mara?
      -> tab do baar ja sakta hai. SES / Twilio aisi parchi khud nahi rakhte, to ye chhota risk maana hua hai.
         OTP do baar aana chalega, par na aana nahi chalega.
```

#### [6](#d6) COMPANY GIR RAHI, HUM TURANT DOBARA MAAR RAHE
```
KAHANI:  SMS company pehle se pareshaan hai, aur hum har second dobara bhej ke uspe aur hathoda maar rahe hain.

ILAAJ:   Dobara bhejne me har baar zyada ruko (1, 2, 4, 8 second = BACKOFF) + thoda random (JITTER).
         Had paar -> DLQ me daalo + alert.

ANDAR KAISE:
         Rukne ka time = shuru ka time x 2^koshish + thoda random. Random isliye ki sab worker ek saath na lautein.
         Kafka ko "padh liya" (offset) KAAM HONE KE BAAD bolo: worker beech me mara to khabar dobara aayegi, khoyegi nahi.
         Kafka me likhte waqt "saari copies ne le liya tabhi OK" (acks=all).

ISKE BAAD KYA TOOTEGA:
  - Kafka me "10 minute baad chalana" hota hi nahi, to ruk ke dobara kaise?
      -> alag lines banao (retry-1m, retry-10m). Worker message pe likha "kab chalana" dekhe, abhi nahi to ruke.
  - DLQ me padi khabaron ka kya?
      -> alert + dashboard, aur theek karke wapas main line me daalne ka tool.
  - Khabar kabhi khoye nahi, kaise pakka?
      -> saari copies ne liya tabhi OK + kaam ke baad offset + parchi + DLQ. Phenkna kabhi nahi.
```

#### [7](#d7) COMPANY DHEEMI, SAARE WORKER 30 SEC ME PHANSE
```
KAHANI:  SMS company band nahi hui, bas dheemi ho gayi. Har call 30 second atki.
         Saare worker usi me phans gaye, baaki kaam ruk gaya. Dheemi company band company se bhi buri.

ILAAJ:   Chhota TIMEOUT + har raaste ki DO company (SES + SendGrid, Twilio + SNS) + CIRCUIT BREAKER.

ANDAR KAISE (circuit breaker = ghar ka fuse):
         sab theek (CLOSED) -> kai baar fail -> fuse uda (OPEN): us company ko call band, doosri company pe bhejo
         -> ~30 second baad (HALF-OPEN): 1-2 test call -> chali to wapas CLOSED, fail to phir OPEN.

ISKE BAAD KYA TOOTEGA:
  - Dono company band?
      -> khabar line me ruki rahe (phenko nahi), backoff se koshish. OTP ke liye alert.
  - Push ki doosri company?
      -> nahi hoti. Android = FCM, iPhone = APNs, alag-alag.
```

#### [8](#d8) OTP OFFER KE 50,000 MESSAGE KE PEECHE
```
KAHANI:  Ek hi line me sab. Usi waqt 50,000 offer wale message line me the. Arpan ka OTP unke peeche khada raha,
         10 minute baad aaya. Tab tak login ka time khatam.

ILAAJ:   Zaroorat ke hisaab se alag line (Kafka topic) aur alag worker: tez (OTP) · beech (order) · dheemi (offer).

ANDAR KAISE:
         Kafka me "ye zaroori hai, pehle bhejo" hota hi nahi -> isliye alag line.
         Raaste ki line bhi alag: sms-tez / sms-dheemi.

ISKE BAAD KYA TOOTEGA:
  - Tez line khaali, dheemi me bheed -> tez wale worker baithe rahenge?
      -> haan, thoda bekaar. Khaali hon to beech wali bhi padh lein (par ulta kabhi nahi).
  - OTP bhi company ki limit me phans gaya?
      -> OTP ke liye company ka alag account / alag number, offer use kha na sake.
```

#### [9](#d9) EK SAATH BAHUT, COMPANY NE 429 DIYA
```
KAHANI:  Sale shuru, ek saath laakhon message. SMS company bolti "itna nahi" (429) -> sab fail.

ILAAJ:   Worker khud apni speed kam kare (token bucket), company ki limit ke andar.
         429 aaya -> ruko, company jitna bole utna (Retry-After). Message line me ruke, phenko nahi.
         (Yahan "khol do" wala raasta nahi, company ka darwaza hum nahi khol sakte.)

ANDAR KAISE:
         100 worker, har ek ne poori limit li = 100 guna -> phir 429.
         Isliye Redis me EK saanjhi baalti, har worker bhejne se pehle usse token le (Lua, ek jhatke me).
         Limits: FCM ~10,000 / second · SES naya account ~14 / second · Twilio short code ~100 / second.

ISKE BAAD KYA TOOTEGA:
  - Worker apne aap badh gaye?
      -> baalti saanjhi hai: kitne bhi worker ho, kul speed wahi.
  - Limit hi kam hai (SES 14 / second) aur 1 lakh email bhejne hain?
      -> limit badhwao + kai account / company. Tab tak line dheere-dheere khaali.
```

#### [10](#d10) "BHEJ DIYA" KA MATLAB "MIL GAYA" NAHI
```
KAHANI:  SMS company ne message le liya. Par Arpan ke phone tak pahuncha ya nahi, humein pata hi nahi.

ILAAJ:   Company khud batati hai (WEBHOOK): delivered / bounced / failed -> hamare hisaab (Tracking DB) me likho.
         Push me app khud "mil gaya" bhejti hai.

ANDAR KAISE:
         Bhejte waqt company ka message number (SES MessageId / Twilio SID) apne notification ke saath save.
         Webhook aaya -> us number se row dhoondho -> haalat badlo. Hisaab = Cassandra (likhna bahut, seedhi row).

ISKE BAAD KYA TOOTEGA:
  - Webhook aaya hi nahi?
      -> kuch ghante baad jo "bhej diya" pe atke hain, sirf unke liye ek baar company se poochho.
  - Koi nakli webhook bhej de?
      -> company ka hastakshar (HMAC signature) jaancho, warna mana.
  - Webhook ki jagah baar-baar khud poochte raho?
      -> laakhon message x har kuch second = limit + kharcha. Webhook tabhi aata jab kuch badla.
```

#### [11](#d11) 1 CRORE LOGON KO EK SAATH OFFER
```
KAHANI:  "Diwali offer sabko" = 1 crore user. Ek-ek karke bheja -> request ghanton atki, OTP bhi peeche,
         company ki limit tooti.

ILAAJ:   Campaign service turant bolti "le liya" (202). Ek BATCH job users ko 1000-1000 ke tukdon me padh ke
         Kafka me daale (10,000 message). Workers: pasand dekho -> saancha -> DHEEMI line -> company ki speed se.
         Har message ki pehchaan = (campaign_id, user_id).

ANDAR KAISE:
         "id > aakhri_id, 1000 lao" (cursor), har tukde ke baad aakhri_id campaign row me save
         -> job gira to wahin se shuru. 10,000 / second ki speed se 1 crore ~17 minute (jaan-boojh ke phailaya).
         Push me sabko ek hi message -> FCM ka TOPIC: ek call, aage Google sabko pahunchata hai.

ISKE BAAD KYA TOOTEGA:
  - Worker 1000 me se 600 bhej ke gira?
      -> tukda dobara aayega, (campaign_id, user_id) se pehle ke 600 chhoot jaayenge.
  - Galat offer chala gaya, rokna hai?
      -> campaign row pe "CANCELLED". Worker har tukde se pehle dekhe.
  - 1000 ka tukda hi kyun?
      -> 1 crore alag khabar = bojh. 1 bahut bada message = ek worker pe atka. 1000 = saath-saath bhi, dobara bhi chhota.
  - Ek user ke message ka kram ("order hua" ke baad "ship hua")?
      -> line ka chunaav user_id se -> ek user ki saari khabar ek line me, kram se.
```

```
AAKHRI DABBE (sab dikkat ke baad):
  Order / Campaign Svc -> Kafka (tez / beech / dheemi) -> Notification Svc (pasand + saancha, cache)
  -> push / email / SMS ki line -> worker (parchi · ruk ke dobara · fuse · speed limit)
  -> 2 company -> user · webhook -> Tracking DB · fail -> DLQ

DB:  Tracking = Cassandra -> likhna bahut, seedhi row, pakki consistency nahi chahiye, aaram se badhta
     Pasand / saancha = chhota, har ghatna pe padha jaata -> cache (dikkat 3)

END ME POOCHEGA:
  "10 guna?"              -> Kafka ki lines pehle se zyada · worker badhao · company ki limit pe speed
  "Kahan se gir sakta?"   -> company (do company), Kafka (copies), Redis (DB ka UNIQUE peeche)
  "Chal raha, kaise pata?" -> webhook se delivered / failed · Kafka me kitna pending · DLQ kitna bhara · alert
  "Ek user ka kram?"      -> line user_id se -> "order hua" kabhi "ship hua" ke baad nahi
```

AAKHRI DABBE ki tasveer:

```mermaid
flowchart TD
    n_Admin["Admin"]
    n_Campaign_Svc["Campaign Svc"]
    n_Campaign_DB["Campaign DB"]
    n_Order_Svc["Order Svc"]
    n_Kafka_high_medium_low["Kafka high / medium / low"]
    n_Notification_Svc["Notification Svc"]
    n_User_pref_DB["User-pref DB"]
    n_Template_DB["Template DB"]
    n_Push_queue["Push queue"]
    n_Push_worker["Push worker"]
    n_FCM_APNs["FCM / APNs"]
    n_Email_queue["Email queue"]
    n_Email_worker["Email worker"]
    n_SES_SendGrid["SES + SendGrid"]
    n_SMS_queue["SMS queue"]
    n_SMS_worker["SMS worker"]
    n_Twilio_SNS["Twilio + SNS"]
    n_Redis["Redis"]
    n_Tracking_DB["Tracking DB"]
    n_DLQ["DLQ"]
    n_Admin --> n_Campaign_Svc
    n_Campaign_Svc --> n_Campaign_DB
    n_Campaign_Svc --> n_Kafka_high_medium_low
    n_Order_Svc --> n_Kafka_high_medium_low
    n_Kafka_high_medium_low --> n_Notification_Svc
    n_Notification_Svc --> n_User_pref_DB
    n_Notification_Svc --> n_Push_queue
    n_Notification_Svc --> n_Email_queue
    n_Notification_Svc --> n_SMS_queue
    n_Notification_Svc --> n_Template_DB
    n_Push_queue --> n_Push_worker
    n_Push_worker --> n_FCM_APNs
    n_Email_queue --> n_Email_worker
    n_Email_worker --> n_SES_SendGrid
    n_SMS_queue --> n_SMS_worker
    n_SMS_worker --> n_Twilio_SNS
    n_SMS_worker --> n_Redis
    n_SMS_worker --> n_DLQ
    n_Twilio_SNS --> n_Tracking_DB
    n_Push_worker --> n_Redis
    n_Email_worker --> n_Redis
    n_Push_worker --> n_DLQ
    n_Email_worker --> n_DLQ
    n_SES_SendGrid --> n_Tracking_DB
```

---
## ════ DETAIL: HAR DIKKAT KI INTERVIEW LINE + TASVEER ════

> Card me poori kahani hai. Yahan har dikkat ki ek English line (interview me bolne ke liye) aur us waqt tak ka diagram.

---

<a id="d1"></a>

## DIKKAT 1 — order email ke intezaar me ruka

```
BOL: "The order service publishes an event to Kafka and returns; the notification service consumes it in the background, so a slow or down email provider never fails an order. To avoid losing the event if the service dies between the DB write and Kafka, I use the outbox pattern."
```
```mermaid
flowchart TD
    n_Order_Svc["Order Svc"]
    n_Kafka["Kafka"]
    n_Notification_Svc["Notification Svc"]
    n_Email_API["Email API"]
    n_USER["USER"]
    n_Order_Svc --> n_Kafka
    n_Kafka --> n_Notification_Svc
    n_Notification_Svc --> n_Email_API
    n_Email_API --> n_USER
```

---

<a id="d2"></a>

## DIKKAT 2 — teen raaste, SMS dheema to sab atke

```
BOL: "Each channel has its own Kafka topic and its own workers, so a slow SMS provider only grows the SMS backlog while push and email keep flowing, and I scale each channel separately."
```
```mermaid
flowchart TD
    n_Order_Svc["Order Svc"]
    n_Kafka["Kafka"]
    n_Notification_Svc["Notification Svc"]
    n_Push_queue["Push queue"]
    n_Push_worker["Push worker"]
    n_FCM_APNs["FCM / APNs"]
    n_Email_queue["Email queue"]
    n_Email_worker["Email worker"]
    n_SES["SES"]
    n_SMS_queue["SMS queue"]
    n_SMS_worker["SMS worker"]
    n_Twilio["Twilio"]
    n_Order_Svc --> n_Kafka
    n_Kafka --> n_Notification_Svc
    n_Notification_Svc --> n_Push_queue
    n_Notification_Svc --> n_Email_queue
    n_Notification_Svc --> n_SMS_queue
    n_Push_queue --> n_Push_worker
    n_Push_worker --> n_FCM_APNs
    n_Email_queue --> n_Email_worker
    n_Email_worker --> n_SES
    n_SMS_queue --> n_SMS_worker
    n_SMS_worker --> n_Twilio
```

---

<a id="d3"></a>

## DIKKAT 3 — SMS band tha, phir bhi raat 2 baje

```
BOL: "Before sending, the notification service checks the user's preferences, opted-out channels and quiet hours, then fills the template in their language; both are cached. Marketing waits until morning, while OTP and security alerts go through quiet hours."
```
```mermaid
flowchart TD
    n_Order_Svc["Order Svc"]
    n_Kafka["Kafka"]
    n_Notification_Svc["Notification Svc"]
    n_User_pref_DB["User-pref DB"]
    n_Template_DB["Template DB"]
    n_Push_queue["Push queue"]
    n_Push_worker["Push worker"]
    n_FCM_APNs["FCM / APNs"]
    n_Email_queue["Email queue"]
    n_Email_worker["Email worker"]
    n_SES["SES"]
    n_SMS_queue["SMS queue"]
    n_SMS_worker["SMS worker"]
    n_Twilio["Twilio"]
    n_Order_Svc --> n_Kafka
    n_Kafka --> n_Notification_Svc
    n_Notification_Svc --> n_User_pref_DB
    n_Notification_Svc --> n_Push_queue
    n_Notification_Svc --> n_Email_queue
    n_Notification_Svc --> n_SMS_queue
    n_Notification_Svc --> n_Template_DB
    n_Push_queue --> n_Push_worker
    n_Push_worker --> n_FCM_APNs
    n_Email_queue --> n_Email_worker
    n_Email_worker --> n_SES
    n_SMS_queue --> n_SMS_worker
    n_SMS_worker --> n_Twilio
```

---

<a id="d4"></a>

## DIKKAT 4 — jawab kho gaya, user ko do email

```
BOL: "Kafka is at-least-once, so workers are idempotent: before sending they set the event id in Redis with SET NX and skip if it's already there, with a UNIQUE event-and-channel constraint in the DB as the hard guarantee."
```
```mermaid
flowchart TD
    n_Order_Svc["Order Svc"]
    n_Kafka["Kafka"]
    n_Notification_Svc["Notification Svc"]
    n_User_pref_DB["User-pref DB"]
    n_Template_DB["Template DB"]
    n_Push_queue["Push queue"]
    n_Push_worker["Push worker"]
    n_FCM_APNs["FCM / APNs"]
    n_Email_queue["Email queue"]
    n_Email_worker["Email worker"]
    n_SES["SES"]
    n_SMS_queue["SMS queue"]
    n_SMS_worker["SMS worker"]
    n_Twilio["Twilio"]
    n_Redis["Redis"]
    n_Order_Svc --> n_Kafka
    n_Kafka --> n_Notification_Svc
    n_Notification_Svc --> n_User_pref_DB
    n_Notification_Svc --> n_Push_queue
    n_Notification_Svc --> n_Email_queue
    n_Notification_Svc --> n_SMS_queue
    n_Notification_Svc --> n_Template_DB
    n_Push_queue --> n_Push_worker
    n_Push_worker --> n_FCM_APNs
    n_Email_queue --> n_Email_worker
    n_Email_worker --> n_SES
    n_SMS_queue --> n_SMS_worker
    n_SMS_worker --> n_Twilio
    n_SMS_worker --> n_Redis
    n_Push_worker --> n_Redis
    n_Email_worker --> n_Redis
```

---

<a id="d5"></a>

## DIKKAT 5 — parchi laga di, par bhejna fail

```
BOL: "I don't want the key to block the retry. I set it as 'sending' with a short TTL, and mark it 'sent' only after the provider accepts. If the send fails I delete the key; if the worker dies, the short TTL expires it — either way the retry goes through. I only skip when the key says 'sent'."
```
```mermaid
flowchart TD
    n_Order_Svc["Order Svc"]
    n_Kafka["Kafka"]
    n_Notification_Svc["Notification Svc"]
    n_User_pref_DB["User-pref DB"]
    n_Template_DB["Template DB"]
    n_Push_queue["Push queue"]
    n_Push_worker["Push worker"]
    n_FCM_APNs["FCM / APNs"]
    n_Email_queue["Email queue"]
    n_Email_worker["Email worker"]
    n_SES["SES"]
    n_SMS_queue["SMS queue"]
    n_SMS_worker["SMS worker"]
    n_Twilio["Twilio"]
    n_Redis["Redis"]
    n_Order_Svc --> n_Kafka
    n_Kafka --> n_Notification_Svc
    n_Notification_Svc --> n_User_pref_DB
    n_Notification_Svc --> n_Push_queue
    n_Notification_Svc --> n_Email_queue
    n_Notification_Svc --> n_SMS_queue
    n_Notification_Svc --> n_Template_DB
    n_Push_queue --> n_Push_worker
    n_Push_worker --> n_FCM_APNs
    n_Email_queue --> n_Email_worker
    n_Email_worker --> n_SES
    n_SMS_queue --> n_SMS_worker
    n_SMS_worker --> n_Twilio
    n_SMS_worker --> n_Redis
    n_Push_worker --> n_Redis
    n_Email_worker --> n_Redis
```

---

<a id="d6"></a>

## DIKKAT 6 — company gir rahi, hum turant dobara maar rahe

```
BOL: "Retries use exponential backoff with jitter through separate retry topics, offsets are committed only after processing, the producer uses acks=all, and after max retries the message goes to a DLQ with an alert and a replay tool, so nothing is dropped."
```
```mermaid
flowchart TD
    n_Order_Svc["Order Svc"]
    n_Kafka["Kafka"]
    n_Notification_Svc["Notification Svc"]
    n_User_pref_DB["User-pref DB"]
    n_Template_DB["Template DB"]
    n_Push_queue["Push queue"]
    n_Push_worker["Push worker"]
    n_FCM_APNs["FCM / APNs"]
    n_Email_queue["Email queue"]
    n_Email_worker["Email worker"]
    n_SES["SES"]
    n_SMS_queue["SMS queue"]
    n_SMS_worker["SMS worker"]
    n_Twilio["Twilio"]
    n_Redis["Redis"]
    n_DLQ["DLQ"]
    n_Order_Svc --> n_Kafka
    n_Kafka --> n_Notification_Svc
    n_Notification_Svc --> n_User_pref_DB
    n_Notification_Svc --> n_Push_queue
    n_Notification_Svc --> n_Email_queue
    n_Notification_Svc --> n_SMS_queue
    n_Notification_Svc --> n_Template_DB
    n_Push_queue --> n_Push_worker
    n_Push_worker --> n_FCM_APNs
    n_Email_queue --> n_Email_worker
    n_Email_worker --> n_SES
    n_SMS_queue --> n_SMS_worker
    n_SMS_worker --> n_Twilio
    n_SMS_worker --> n_Redis
    n_SMS_worker --> n_DLQ
    n_Push_worker --> n_Redis
    n_Email_worker --> n_Redis
    n_Push_worker --> n_DLQ
    n_Email_worker --> n_DLQ
```

---

<a id="d7"></a>

## DIKKAT 7 — company dheemi, saare worker phanse

```
BOL: "A slow provider is worse than a dead one, so calls have short timeouts, each channel has two providers, and a circuit breaker opens after repeated failures, sends traffic to the backup, and probes the primary again after about thirty seconds."
```
```mermaid
flowchart TD
    n_Order_Svc["Order Svc"]
    n_Kafka["Kafka"]
    n_Notification_Svc["Notification Svc"]
    n_User_pref_DB["User-pref DB"]
    n_Template_DB["Template DB"]
    n_Push_queue["Push queue"]
    n_Push_worker["Push worker"]
    n_FCM_APNs["FCM / APNs"]
    n_Email_queue["Email queue"]
    n_Email_worker["Email worker"]
    n_SES_SendGrid["SES + SendGrid"]
    n_SMS_queue["SMS queue"]
    n_SMS_worker["SMS worker"]
    n_Twilio_SNS["Twilio + SNS"]
    n_Redis["Redis"]
    n_DLQ["DLQ"]
    n_Order_Svc --> n_Kafka
    n_Kafka --> n_Notification_Svc
    n_Notification_Svc --> n_User_pref_DB
    n_Notification_Svc --> n_Push_queue
    n_Notification_Svc --> n_Email_queue
    n_Notification_Svc --> n_SMS_queue
    n_Notification_Svc --> n_Template_DB
    n_Push_queue --> n_Push_worker
    n_Push_worker --> n_FCM_APNs
    n_Email_queue --> n_Email_worker
    n_Email_worker --> n_SES_SendGrid
    n_SMS_queue --> n_SMS_worker
    n_SMS_worker --> n_Twilio_SNS
    n_SMS_worker --> n_Redis
    n_SMS_worker --> n_DLQ
    n_Push_worker --> n_Redis
    n_Email_worker --> n_Redis
    n_Push_worker --> n_DLQ
    n_Email_worker --> n_DLQ
```

---

<a id="d8"></a>

## DIKKAT 8 — OTP offer ke peeche

```
BOL: "Kafka has no priorities, so priority is separate topics with their own workers: high for OTP, medium for orders, low for marketing. OTP also gets its own provider account so marketing can't use up its limit."
```
```mermaid
flowchart TD
    n_Order_Svc["Order Svc"]
    n_Kafka_high_medium_low["Kafka high / medium / low"]
    n_Notification_Svc["Notification Svc"]
    n_User_pref_DB["User-pref DB"]
    n_Template_DB["Template DB"]
    n_Push_queue["Push queue"]
    n_Push_worker["Push worker"]
    n_FCM_APNs["FCM / APNs"]
    n_Email_queue["Email queue"]
    n_Email_worker["Email worker"]
    n_SES_SendGrid["SES + SendGrid"]
    n_SMS_queue["SMS queue"]
    n_SMS_worker["SMS worker"]
    n_Twilio_SNS["Twilio + SNS"]
    n_Redis["Redis"]
    n_DLQ["DLQ"]
    n_Order_Svc --> n_Kafka_high_medium_low
    n_Kafka_high_medium_low --> n_Notification_Svc
    n_Notification_Svc --> n_User_pref_DB
    n_Notification_Svc --> n_Push_queue
    n_Notification_Svc --> n_Email_queue
    n_Notification_Svc --> n_SMS_queue
    n_Notification_Svc --> n_Template_DB
    n_Push_queue --> n_Push_worker
    n_Push_worker --> n_FCM_APNs
    n_Email_queue --> n_Email_worker
    n_Email_worker --> n_SES_SendGrid
    n_SMS_queue --> n_SMS_worker
    n_SMS_worker --> n_Twilio_SNS
    n_SMS_worker --> n_Redis
    n_SMS_worker --> n_DLQ
    n_Push_worker --> n_Redis
    n_Email_worker --> n_Redis
    n_Push_worker --> n_DLQ
    n_Email_worker --> n_DLQ
```

---

<a id="d9"></a>

## DIKKAT 9 — company ne 429 diya

```
BOL: "Workers throttle themselves with a token bucket shared in Redis at the provider's rate. On a 429 they back off with jitter and respect Retry-After; messages wait in the queue and go to a DLQ after max retries, never dropped."
```
```mermaid
flowchart TD
    n_Order_Svc["Order Svc"]
    n_Kafka_high_medium_low["Kafka high / medium / low"]
    n_Notification_Svc["Notification Svc"]
    n_User_pref_DB["User-pref DB"]
    n_Template_DB["Template DB"]
    n_Push_queue["Push queue"]
    n_Push_worker["Push worker"]
    n_FCM_APNs["FCM / APNs"]
    n_Email_queue["Email queue"]
    n_Email_worker["Email worker"]
    n_SES_SendGrid["SES + SendGrid"]
    n_SMS_queue["SMS queue"]
    n_SMS_worker["SMS worker"]
    n_Twilio_SNS["Twilio + SNS"]
    n_Redis["Redis"]
    n_DLQ["DLQ"]
    n_Order_Svc --> n_Kafka_high_medium_low
    n_Kafka_high_medium_low --> n_Notification_Svc
    n_Notification_Svc --> n_User_pref_DB
    n_Notification_Svc --> n_Push_queue
    n_Notification_Svc --> n_Email_queue
    n_Notification_Svc --> n_SMS_queue
    n_Notification_Svc --> n_Template_DB
    n_Push_queue --> n_Push_worker
    n_Push_worker --> n_FCM_APNs
    n_Email_queue --> n_Email_worker
    n_Email_worker --> n_SES_SendGrid
    n_SMS_queue --> n_SMS_worker
    n_SMS_worker --> n_Twilio_SNS
    n_SMS_worker --> n_Redis
    n_SMS_worker --> n_DLQ
    n_Push_worker --> n_Redis
    n_Email_worker --> n_Redis
    n_Push_worker --> n_DLQ
    n_Email_worker --> n_DLQ
```

---

<a id="d10"></a>

## DIKKAT 10 — bhej diya ka matlab mil gaya nahi

```
BOL: "I store the provider's message id with each notification and update the tracking DB from the provider's delivery webhooks, verified by signature. Anything stuck as 'sent' for hours gets a single status poll."
```
```mermaid
flowchart TD
    n_Order_Svc["Order Svc"]
    n_Kafka_high_medium_low["Kafka high / medium / low"]
    n_Notification_Svc["Notification Svc"]
    n_User_pref_DB["User-pref DB"]
    n_Template_DB["Template DB"]
    n_Push_queue["Push queue"]
    n_Push_worker["Push worker"]
    n_FCM_APNs["FCM / APNs"]
    n_Email_queue["Email queue"]
    n_Email_worker["Email worker"]
    n_SES_SendGrid["SES + SendGrid"]
    n_SMS_queue["SMS queue"]
    n_SMS_worker["SMS worker"]
    n_Twilio_SNS["Twilio + SNS"]
    n_Redis["Redis"]
    n_Tracking_DB["Tracking DB"]
    n_DLQ["DLQ"]
    n_Order_Svc --> n_Kafka_high_medium_low
    n_Kafka_high_medium_low --> n_Notification_Svc
    n_Notification_Svc --> n_User_pref_DB
    n_Notification_Svc --> n_Push_queue
    n_Notification_Svc --> n_Email_queue
    n_Notification_Svc --> n_SMS_queue
    n_Notification_Svc --> n_Template_DB
    n_Push_queue --> n_Push_worker
    n_Push_worker --> n_FCM_APNs
    n_Email_queue --> n_Email_worker
    n_Email_worker --> n_SES_SendGrid
    n_SMS_queue --> n_SMS_worker
    n_SMS_worker --> n_Twilio_SNS
    n_SMS_worker --> n_Redis
    n_SMS_worker --> n_DLQ
    n_Twilio_SNS --> n_Tracking_DB
    n_Push_worker --> n_Redis
    n_Email_worker --> n_Redis
    n_Push_worker --> n_DLQ
    n_Email_worker --> n_DLQ
    n_SES_SendGrid --> n_Tracking_DB
```

---

<a id="d11"></a>

## DIKKAT 11 — 1 crore ko ek saath offer

```
BOL: "The campaign service accepts with a 202, a batch job reads users in chunks of a thousand with a cursor saved after each chunk, and pushes them onto the low-priority topic, throttled to provider limits. The campaign id plus user id is the idempotency key, and a cancel flag is checked before each chunk."
```
```mermaid
flowchart TD
    n_Admin["Admin"]
    n_Campaign_Svc["Campaign Svc"]
    n_Campaign_DB["Campaign DB"]
    n_Order_Svc["Order Svc"]
    n_Kafka_high_medium_low["Kafka high / medium / low"]
    n_Notification_Svc["Notification Svc"]
    n_User_pref_DB["User-pref DB"]
    n_Template_DB["Template DB"]
    n_Push_queue["Push queue"]
    n_Push_worker["Push worker"]
    n_FCM_APNs["FCM / APNs"]
    n_Email_queue["Email queue"]
    n_Email_worker["Email worker"]
    n_SES_SendGrid["SES + SendGrid"]
    n_SMS_queue["SMS queue"]
    n_SMS_worker["SMS worker"]
    n_Twilio_SNS["Twilio + SNS"]
    n_Redis["Redis"]
    n_Tracking_DB["Tracking DB"]
    n_DLQ["DLQ"]
    n_Admin --> n_Campaign_Svc
    n_Campaign_Svc --> n_Campaign_DB
    n_Campaign_Svc --> n_Kafka_high_medium_low
    n_Order_Svc --> n_Kafka_high_medium_low
    n_Kafka_high_medium_low --> n_Notification_Svc
    n_Notification_Svc --> n_User_pref_DB
    n_Notification_Svc --> n_Push_queue
    n_Notification_Svc --> n_Email_queue
    n_Notification_Svc --> n_SMS_queue
    n_Notification_Svc --> n_Template_DB
    n_Push_queue --> n_Push_worker
    n_Push_worker --> n_FCM_APNs
    n_Email_queue --> n_Email_worker
    n_Email_worker --> n_SES_SendGrid
    n_SMS_queue --> n_SMS_worker
    n_SMS_worker --> n_Twilio_SNS
    n_SMS_worker --> n_Redis
    n_SMS_worker --> n_DLQ
    n_Twilio_SNS --> n_Tracking_DB
    n_Push_worker --> n_Redis
    n_Email_worker --> n_Redis
    n_Push_worker --> n_DLQ
    n_Email_worker --> n_DLQ
    n_SES_SendGrid --> n_Tracking_DB
```

---
## 10 GUNA TRAFFIC — har dabba alag se

```
Kafka            -> sale pe 10 guna -> lines (partition) pehle se zyada · line user_id se
Notification Svc -> pasand / saancha cache · machine badhao
raaste ki line   -> alag line (ho gaya) + tez / dheemi line
worker           -> sale pe badhao (ek worker ~1000 / second) · fuse + do company · speed limit
Tracking DB      -> 90 TB / saal -> Cassandra + row ki umar, purana sasti jagah (cold storage)
                    (umar se jagah ghatti hai, baantne se bojh — dono alag cheez)

POOCHEGA: "Data keeps growing — what happens in 3 years?"
BOL:      "Tracking rows get a TTL and old data moves to cold storage; the live table stays small."
```

---

## POOCHE TO (gehra sawaal aaye tab)

```
KHABAR:   Kafka pe { event: "ORDER_PLACED", userId: 123, orderId: 999 }
          (andar ke liye REST bhi: POST /notify { userId, type, channel?, payload })

RAASTE:   Push: FCM (Android) / APNs (iPhone) · Email: SES / SendGrid / Mailgun · SMS: Twilio / SNS / MSG91
          App ke andar: DB + WebSocket · Phone call: Twilio · Slack webhook

DB:       PASAND:  user_id -> kaunse raaste · shaanti ka waqt · kya band kiya
          SAANCHA: template_id -> format · bhasha · khaali jagah {{name}} {{orderId}}
          HISAAB:  bheja / mila / khola / click / fail -> Cassandra (bahut likhna, seedhi row, ACID nahi chahiye)

1 LAKH / SECOND:  ~100 worker · line "notifications" P0 -> W1 ... P99 -> W100
          line user_id se -> ek user ki saari khabar ek line me -> kram bana rehta
          (warna "ship hua" pehle aur "order hua" baad me)

POORA RAASTA EK BAAR:
          order -> Order Svc Kafka me { ORDER_PLACED, 123, 999 } -> Notification Svc uthaye -> pasand dekhe
          (push chalu, email chalu, SMS band) -> saancha bhare -> push + email ki line -> worker -> company
          -> user -> webhook -> Tracking DB · fail -> dobara -> DLQ
```

---

## AAKHRI DABBA + WRAP

```
Kafka = order se alag + bheed jhele + zaroorat ki alag line · Notification Svc = pasand + saancha + fanout
raaste ki line = apni speed · worker = parchi (BHEJ RAHA / BHEJ DIYA) + ruk ke dobara + fuse + speed limit
DLQ = kharab khabar baaki ko na roke · Tracking DB = "bheja" vs "mila"
```
```
BOL: "Services publish events to Kafka. The notification service checks preferences, fills the template
      and fans out to per-channel queues. Workers are idempotent with a sending / sent key in Redis, retry with
      backoff and jitter, throttle to the provider's limit, use a circuit breaker with a backup provider, and
      send to a DLQ after max retries. OTPs have their own topic. Partitioning by user id keeps per-user order,
      and provider webhooks update the tracking DB. Next: quiet hours, i18n templates, open / click analytics."
```
```mermaid
flowchart TD
    n_Admin["Admin"]
    n_Campaign_Svc["Campaign Svc"]
    n_Campaign_DB["Campaign DB"]
    n_Order_Svc["Order Svc"]
    n_Kafka_high_medium_low["Kafka high / medium / low"]
    n_Notification_Svc["Notification Svc"]
    n_User_pref_DB["User-pref DB"]
    n_Template_DB["Template DB"]
    n_Push_queue["Push queue"]
    n_Push_worker["Push worker"]
    n_FCM_APNs["FCM / APNs"]
    n_Email_queue["Email queue"]
    n_Email_worker["Email worker"]
    n_SES_SendGrid["SES + SendGrid"]
    n_SMS_queue["SMS queue"]
    n_SMS_worker["SMS worker"]
    n_Twilio_SNS["Twilio + SNS"]
    n_Redis["Redis"]
    n_Tracking_DB["Tracking DB"]
    n_DLQ["DLQ"]
    n_Admin --> n_Campaign_Svc
    n_Campaign_Svc --> n_Campaign_DB
    n_Campaign_Svc --> n_Kafka_high_medium_low
    n_Order_Svc --> n_Kafka_high_medium_low
    n_Kafka_high_medium_low --> n_Notification_Svc
    n_Notification_Svc --> n_User_pref_DB
    n_Notification_Svc --> n_Push_queue
    n_Notification_Svc --> n_Email_queue
    n_Notification_Svc --> n_SMS_queue
    n_Notification_Svc --> n_Template_DB
    n_Push_queue --> n_Push_worker
    n_Push_worker --> n_FCM_APNs
    n_Email_queue --> n_Email_worker
    n_Email_worker --> n_SES_SendGrid
    n_SMS_queue --> n_SMS_worker
    n_SMS_worker --> n_Twilio_SNS
    n_SMS_worker --> n_Redis
    n_SMS_worker --> n_DLQ
    n_Twilio_SNS --> n_Tracking_DB
    n_Push_worker --> n_Redis
    n_Email_worker --> n_Redis
    n_Push_worker --> n_DLQ
    n_Email_worker --> n_DLQ
    n_SES_SendGrid --> n_Tracking_DB
```

---

## HANDS-ON — provider ne 429 diya: chala ke dekha

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

ARCHETYPE B (ingest) · CONCEPTS: [message-queues](../../FOUNDATIONS/07_message_queues.md) · [ms-communication](../../FOUNDATIONS/10_ms_communication.md) · saath: [12 message-queue](../12_message_queue_kafka/12_message_queue_kafka.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
