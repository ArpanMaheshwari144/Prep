# Notification System

> Koi ghatna hui (order, OTP, offer), to sahi user ko, sahi raaste (push / email / SMS) se, sahi waqt pe khabar.
> Dil teen cheezein: ek ghatna, kai raaste, kai log (fanout). Khabar kabhi khoye nahi, do baar jaaye nahi. OTP turant.
>
> Mock me sirf heading + Cue + kahani ki tasveer. Neeche har card: HAL · KYUN · NAHI, phir YAAD RAKH dabba + CROSS-Q dabba (sawaal → cue, poora jawab).
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

---

*ab tootega kahan? order seedha company ke bharose hai*

## 1. Maine order kiya, email company band thi, order hi fail

**Rail 1 KAAM CHALA (khaas)** · Cue: KAFKA (async), order turant jawab

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c1_1.png?v=8" width="900">

<img src="img/c1_2.png?v=8" width="900">

<img src="img/c1_3.png?v=8" width="900">

</details>

```
HAL     order aur khabar ek hi DB txn me likhte hain (outbox), relay use Kafka me daalta hai.
        Order ko turant OK milta hai, Notification Svc peeche Kafka se padh ke email bhejti hai
KYUN    order kabhi khabar ka intezaar nahi karta. Khabar disk ke log (Kafka) me padi rehti hai,
        padhne wala yaad rakhta hai kahan tak padha. Company band ho to bhi khabar padi hai,
        wapas aate hi wahin se
        bheed jhatke me aati hai, isliye beech me ek line (Kafka) chahiye
NAHI    Order Svc me hi retry -> ATAKTA: request utni der atki rehti hai, aur Order Svc gira to retry bhi gaya
```

> [!IMPORTANT]
> **Yaad rakh:** Order + khabar ek DB txn (outbox), relay Kafka me. Order turant OK, email peeche, company band ho to bhi khabar padi.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - Kafka tak pahunchne se pehle service giri? → **khabar outbox me padi**: khabar outbox ki row me padi hai, relay baad me bhej deta hai, khabar khoti nahi
> - Kafka ki ek machine gir gayi? → **log ki 3 copy hain**: baaki copy se chalta hai, khabar nahi khoti
> - traffic achanak 10 guna? → **jhatka Kafka ki line sambhaalti**: saari khabar pehle Kafka ki line me jaati hai, worker apni speed se. Sale ka time pata hai (12 baje), to 11:50 tak worker badha do; machine apne aap badhne me minute leti hai, jhatka second me aata hai. Par worker utne hi jitna DB aur SMS / email company jhel sake, warna wahan atkega
> - email company ghanton band? → **khabar Kafka me ~7 din padi, offset se aage**: padhne wala offset se yaad rakhta hai kahan tak padha
> - Notification Svc dheemi? → **machine badhao (consumer group)**: Har line ek machine ko milti hai, lines bat jaati hain
> - email company 2 din baad wapas aayi? → **wahin se aage chalta hai**: Order kabhi email ki wajah se fail nahi hota


---

*ab sab raaste ek hi line me, ek dheema to?*

## 2. SMS company dheemi, mera push aur email bhi atke

**Rail 5 KUCH DHEEMA** · Cue: har raaste ki ALAG LINE + worker

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c2_1.png?v=8" width="900">

<img src="img/c2_2.png?v=8" width="900">

<img src="img/c2_3.png?v=8" width="900">

</details>

```
HAL     har raaste ki alag line aur alag worker hota hai. SMS dheema ho to sirf SMS rukta hai.
        Ghatna aayi to teen chhote message bante hain (ghatna id + raasta), har raasta apni line me jaata hai
KYUN    pehle push, email, SMS ek hi line me the, worker SMS pe atka to sab atak gaye.
        Ab har raasta apni line me, apni speed se chalta hai. SMS ruka, par push + email aa gaye
NAHI    SMS ke worker badha do -> BEKAAR: company khud dheemi hai, line phir bhi ek, push / email peeche
```

> [!IMPORTANT]
> **Yaad rakh:** Har raaste (push / email / SMS) ki alag line + alag worker. Ek dheema to sirf wahi ruka.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - SMS ki line lambi ho gayi? → **sirf SMS ke worker badhao, baaki ko mat chhuo**: push / email pe koi asar nahi
> - push ki line se bhejna hai, par kis phone pe jaaye? → **device token**: device token pe, jo install pe user ke saath save hota hai. Token bekaar nikla to hata do


---

*ab worker beech me mare to?*

## 3. Worker mara, mujhe do email aaye

**Rail 4 DO LOG EK SAATH** · Cue: KEY (SET NX, khabar ki apni id)

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c3_1.png?v=8" width="900">

<img src="img/c3_2.png?v=8" width="900">

<img src="img/c3_3.png?v=8" width="900">

</details>

```
HAL     bhejne se pehle khabar ki apni key (khabar_id + raasta) Redis me dekhte hain.
        Key mili to skip, ek hi email jaata hai
KYUN    pehle Worker-A ne email bheja aur "padh liya" bolne se pehle mar gaya, khabar dobara aayi, Worker-B ne
        phir bheja. Ab pakki id hai, to dobara aayi khabar pehchaan ke skip ho jaati hai
        Redis tez raasta hai, aur DB me (khabar_id, raasta) ek hi baar lag sakta hai, wo aakhri deewar hai
NAHI    har baar naya random id -> BEKAAR: dohraav kabhi pakda hi nahi jaata
```

> [!IMPORTANT]
> **Yaad rakh:** Bhejne se pehle key (khabar_id + raasta) Redis me dekho, mili to skip. Aakhri deewar DB ka UNIQUE.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - key wala Redis hi gira? → **DB me (khabar_id, raasta) UNIQUE, wahi rokta**: ek hi baar lagta hai, pakka wahi rokta hai
> - Redis ki key 24 ghante me mit gayi? → **DB ki row nahi bhoolti**: wo phir bhi rokegi
> - DB me ek hi baar kaise lagta hai? → **LWT (IF NOT EXISTS)**: Cassandra me INSERT IF NOT EXISTS (khabar_id, raasta), yaani LWT


---

*ab key lagi, par galat waqt pe*

## 4. Key lag gayi, par mera OTP gaya hi nahi

**Rail 3 MACHINE GIRI** · Cue: SENDING / SENT (do haal)

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c4_1.png?v=8" width="900">

<img src="img/c4_2.png?v=8" width="900">

<img src="img/c4_3.png?v=8" width="900">

</details>

```
HAL     bhejte waqt key SENDING rakhte hain (chhoti umar, ~60 sec), company OK bole tab SENT.
        Chhodna sirf SENT pe
KYUN    pehle koshish se pehle hi SENT laga di, call fail hui, Worker-B ne SENT dekh ke chhod diya,
        OTP kho gaya.
        SENDING matlab adhoora, chhoti umar se khud mit jaata hai, to khabar khoti nahi
        kabhi do baar chali jaaye to chalega: OTP do baar aana, na aane se behtar hai
NAHI    key hi hata do -> DOHRAAV: worker mara to Kafka dobara deta hai, OTP / email do baar, card 3 wapas
```

> [!IMPORTANT]
> **Yaad rakh:** Bhejte waqt SENDING (umar ~60 sec), company OK bole tab SENT. Chhodna sirf SENT pe.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - bhej diya, jawab se pehle worker mara? → **do baar ja sakta**: ye maana hua risk hai. OTP do baar aana, na aane se behtar
> - call fail, worker zinda? → **worker khud key hata deta**: Koshish adhoori thi, agli chal jaaye
> - SENDING laga ke worker hi mar gaya? → **60 sec me key khud mit-ti, agla bhejta**: hatane wala koi nahi, par SENDING ki umar (60 sec) khatam hote hi key khud mit jaati hai
> - SENDING ki umar kitni rakhein? → **call se lambi, 60 sec**: call (~2-3 sec) se lambi, 60 sec. Umar pehle khatam ho gayi to do log bhej denge
> - Worker-B ko SENDING mili? → **thoda ruko**: phir dekho. Chhodna sirf SENT pe


---

*ab fail hua to dobara kaise maarein?*

## 5. SMS company pehle se pareshaan, hum har second dobara maar rahe

**Rail 5 KUCH DHEEMA (retry)** · Cue: BACKOFF + JITTER, had ke baad DLQ

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c5_1.png?v=8" width="900">

<img src="img/c5_2.png?v=8" width="900">

<img src="img/c5_3.png?v=8" width="900">

</details>

```
HAL     har baar zyada der rukte hain (1, 2, 4, 8 sec) aur thoda random jodte hain (backoff + jitter).
        Had paar ho gayi to khabar alag line (DLQ) me jaati hai + alert
KYUN    pehle fail pe turant dobara, 50 worker har second ek saath, company pe aur hathoda,
        wo kabhi theek nahi hoti. Ab ruk-ruk ke maarte hain,
        aur random thoda sa jodne se sab worker ek hi pal me wapas nahi aate
        khabar phenkna kabhi nahi. Had ke baad DLQ + alert, theek karke wapas
NAHI    retry band, ek fail = chhodo -> KHOYA: company 2 sec hili to OTP hamesha ke liye gaya
```

> [!IMPORTANT]
> **Yaad rakh:** Har baar zyada ruko (1, 2, 4, 8 sec) + thoda random (jitter). Had paar to DLQ + alert, phenko nahi.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - backoff me lamba rukna ho (10 min)? → **alag retry lines**: alag lines banao (retry-1m, retry-10m), worker dekhta hai "kab chalana hai"
> - khabar DLQ me pahunch gayi, ab uska kya? → **dashboard**: theek karo, main line me wapas


---

*ab company band nahi, bas dheemi*

## 6. SMS company band nahi, dheemi: saare worker 30 sec me phanse

**Rail 5 KUCH DHEEMA (company)** · Cue: TIMEOUT + CIRCUIT BREAKER

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c6_1.png?v=8" width="900">

<img src="img/c6_2.png?v=8" width="900">

<img src="img/c6_3.png?v=8" width="900">

</details>

```
HAL     2-3 sec me chhod do (timeout). Baar-baar fail hua to circuit breaker khul jaata hai:
        Twilio ko call band, OTP SNS se jaata hai. Thodi der baad Twilio ko ek test call, theek hai to wapas
KYUN    pehle har call 30 sec tak intezaar karti thi, saare SMS worker Twilio pe phanse, OTP bhi ruka.
        Dheema matlab band maano
        har company ka apna circuit breaker hai, to uske band hone se baaki raaste chalte rehte hain.
        Khabar line me rukti hai, phenki nahi jaati
NAHI    worker badha do -> PHANSE: naye worker bhi 30 sec me phans jaate, dheemi company pe aur bojh
```

> [!IMPORTANT]
> **Yaad rakh:** 2-3 sec me chhodo (timeout). Baar-baar fail to breaker khula, doosri company (SNS) se.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - circuit breaker har process ka apna hai ya saanjha? → **apna ya Redis me**: har process ka apna (Resilience4j), ya Redis me saanjha
> - Twilio ka breaker khula, push / email pe asar? → **nahi, har company ka apna breaker**: Har company ka apna circuit breaker hai, aur worker alag line pe
> - SNS bhi band ho gaya? → **khabar line me rukti, phenko nahi**: backoff se koshish. OTP ka alert
> - push bhi doosri company se bhej do? → **nahi: FCM / APNs**: ye hota nahi. Android = FCM, iPhone = APNs, phone ka OS tay karta hai


---

*ab sale aayi, bheed me OTP kahan?*

## 7. 12 baje sale shuru, mera OTP 50,000 offer ke peeche

**Rail 2 BHEED AAYI (OTP)** · Cue: OTP ki ALAG TEZ LINE

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c7_1.png?v=8" width="900">

<img src="img/c7_2.png?v=8" width="900">

<img src="img/c7_3.png?v=8" width="900">

</details>

```
HAL     OTP ki apni tez line aur apne worker (priority).
        High line me OTP, beech wali me order update, low line me offer
KYUN    sab barabar zaroori nahi: OTP turant chahiye, offer ruk sakta hai.
        Ek hi line me sab daala to OTP 10 min late, time khatam, login fail, yahi asli fail hai
        zaroori (OTP) kabhi kisi ke peeche nahi: apni line, apne worker, company me apna account
NAHI    ek line me "zaroori" tag laga do -> BEKAAR: Kafka me "ye pehle bhejo" jaisa kuch hota hi nahi
```

> [!IMPORTANT]
> **Yaad rakh:** OTP ki apni tez line + apne worker. High = OTP, beech = order, low = offer. Ulta kabhi nahi.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - OTP line khaali, OTP worker khaali baithe? → **beech wali padh lein. Ulta kabhi nahi**: Ulta kabhi nahi
> - offer wale OTP ki company had kha jaayein? → **OTP ka alag account**: ya alag number. Had wala card 8
> - order update kis line me jaaye (medium)? → **beech wali line me**: OTP se peeche, offer se aage


---

*ab bheed itni ki company hi mana kare*

## 8. Sale me laakhon message ek saath, company ne 429 diya

**Rail 2 BHEED AAYI (company ki had)** · Cue: SAANJHA TOKEN BUCKET (Redis)

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c8_1.png?v=8" width="900">

<img src="img/c8_2.png?v=8" width="900">

<img src="img/c8_3.png?v=8" width="900">

</details>

```
HAL     sab worker ka ek saanjha token bucket (Redis me), har second 100 token. Pehle token lo, mila to bhejo
KYUN    had company ki hai (100 / sec, baaki 429), worker ki nahi. Sab worker ek hi bucket se token lete hain.
        Token nahi mila to ruko, khabar phenko nahi
NAHI    har worker apni speed 100 / sec -> 429: 100 worker matlab 100 guna, company phir 429 degi
```

> [!IMPORTANT]
> **Yaad rakh:** Sab worker ka ek saanjha token bucket (Redis). Token nahi mila to ruko, fail nahi.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - bucket se token nahi mila? → **line me intezaar, ye fail nahi**: retry ki ginti nahi badhti
> - worker badh gaye? → **speed wahi**: Bucket saanjha, kul 100 / sec
> - 1 lakh email, had kam? → **had badhwao + kai account lo**: Tab tak line dheere khaali hogi, warna 429
> - bucket wala Redis hi gira? → **har worker apni surakshit had (100 / 4 = 25)**: Bhejna band nahi, bas dheema. Company ki had paar nahi hoti
> - bucket ki had kitni hoti hai? → **company ki had**: company ki had: SES shuru me ~14 / sec (sandbox 1 / sec), Twilio short code ~100 / sec


---

*ab bhej diya, par pahuncha?*

## 9. Company ne "bhej diya" bola, par mere phone tak pahuncha?

**Rail 6 DATA BADA / 8 HAMLA** · Cue: WEBHOOK + company ka message id

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c9_1.png?v=8" width="900">

<img src="img/c9_2.png?v=8" width="900">

<img src="img/c9_3.png?v=8" width="900">

</details>

```
HAL     worker company ka message no. (SID-9) save karta hai. Company baad me khud batati hai (webhook):
        "SID-9 pahuncha" aaya, to Webhook Svc ne Tracking DB me SID-9 DELIVERED likha
KYUN    company ne sirf "le liya" bola, aage andhera. "Bheja" aur "mila" alag haal hain.
        "Mila" tab likho jab company (asli, signed webhook) bataye
NAHI    baar-baar khud poochte raho -> MEHENGA: laakhon message x har kuch second = had + kharcha
```

> [!IMPORTANT]
> **Yaad rakh:** Company ka message no. (SID) save. "Mila" sirf jab company signed webhook se bataye.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - webhook aaya hi nahi? → **ghanton se atke ko ek baar poochho**: Twilio SID se. SES me poll nahi, sirf event (SNS / Firehose)
> - koi nakli webhook bhej de "pahuncha"? → **hastakshar jaancho, nakli phenko**: Twilio HMAC, SES/SNS certificate, SendGrid public key
> - Tracking DB me hisaab kahan rakhte hain? → **Cassandra**: Cassandra me. Likhna bahut hai, isliye NoSQL: seedhi row, jodna kuch nahi


---

*ab ek ke liye nahi, 1 crore ke liye*

## 10. Shaam ko "Diwali offer sabko": 1 crore log

**Rail 2 BHEED AAYI (campaign)** · Cue: 1000 ke TUKDE + campaign row

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c10_1.png?v=8" width="900">

<img src="img/c10_2.png?v=8" width="900">

<img src="img/c10_3.png?v=8" width="900">

</details>

```
HAL     1000-1000 ke tukde banao, low priority line me. Har tukde ke baad campaign row me likho kahan tak
        pahunche (aakhri_id = 3000)
KYUN    pehle admin ne "sabko bhejo" dabaya, ek request me 1 crore ek-ek, ghanton atki, OTP bhi peeche.
        Ab bada kaam chhote tukdon me: gira to wahin se, galat chala to beech me roko. Ek user ko ek hi baar
NAHI    1 crore ko ek bada "sabko bhejo" message -> ATKA: ek worker pe atka, gira to shuru se
        1 crore alag-alag message -> BOJH: 1000 ka tukda = saath bhi, aur dobara karna pade to chhota
```

> [!IMPORTANT]
> **Yaad rakh:** 1000-1000 ke tukde, low line, campaign row me aakhri_id. Gira to wahin se, galat to CANCELLED.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - tukde me 600 bhej ke worker gira? → **tukda dobara, par pehle wale 600 chhoot jaate**: par (campaign_id, user_id) se pehle wale 600 chhoot jaate hain
> - galat offer chal gaya? → **campaign row pe CANCELLED likho**: Worker har tukde se pehle ye dekhta hai
> - 1 crore ke tukde bhejne me kitni der? → **~17 min**: ~17 min (10,000 / sec). Jaan-boojh ke phailaya, had + OTP line bachi
> - sabko ek hi push bhejna hai? → **FCM topic: ek call**: Topic me sirf pasand wale (pasand se sync)


---

*ab bheja to sahi, par user chahta tha?*

## 11. Raat 2 baje offer ka SMS, jabki maine band kiya tha

**Rail 1 KAAM CHALA (pasand)** · Cue: PASAND + SHAANTI + SAANCHA (cache)

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c11_1.png?v=8" width="900">

<img src="img/c11_2.png?v=8" width="900">

<img src="img/c11_3.png?v=8" width="900">

</details>

```
HAL     bhejne se pehle user ki pasand + shaanti ka waqt dekho (preference + quiet hours): offer SMS band,
        raat 10 se subah 8 ghanti nahi. Shaanti me offer aaya to roko,
        subah 8 baje bhejo (scheduled_at + job), phenko nahi
KYUN    pehle pasand dekhi hi nahi, ghanti baji, user gussa. Ab bhejne se pehle pasand dekhte hain.
        Offer pasand / shaanti ke neeche, zaroori alert (OTP) upar
NAHI    pasand sirf Campaign Svc me check -> ADHOORA: order / OTP wali khabar bina check nikal jaati
```

> [!IMPORTANT]
> **Yaad rakh:** Bhejne se pehle pasand + shaanti ka waqt. Offer roko, subah bhejo. OTP pasand se upar.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - shaanti ke waqt OTP / login alert aaya? → **bhejo. Zaroori alert pasand se upar**: Zaroori alert pasand se upar hai
> - pasand badli, par cache purana? → **badalte hi cache delete**: aur chhoti umar. Bhejne se pehle sahi chahiye


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
