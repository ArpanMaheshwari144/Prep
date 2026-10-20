# File Upload + Validate + Track

> User file / folder deta hai, bahar ki company (third-party) use jaanchti hai (2-3 sec), file store hoti, user ko tracking link milta. JP ne ye sach me poocha tha (asli SDE-3 writeup). Aam product design, finance nahi.
> Dil teen cheezein: user ruke nahi (async + trackingId), file ke bytes hamare server se na guzrein (presigned URL), aur haal hamesha pata.
>
> Mock me sirf heading + Cue + kahani ki tasveer. Neeche har card ek block: HAL (↳ agar = cross-Q) · KYUN · NAHI (doosra kyun nahi).
> Cross-Q: hal ke har bade shabd pe socho -> gira? · dheema / bheed? · do ek saath? · galat hua? · kaise / kitna?

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c9_2-40c6937c.svg)

---


## Shuru

```
FR      user file upload kare, bahar wali company use jaanche, file store ho, user ko tracking link
        aur haal mile. Editing, sharing aur versions scope se bahar hain
NFR     user ko rukna na pade (yahi dil hai), file kabhi khoye nahi, haal hamesha pata ho ki file kahan tak
        pahunchi, aur bheed me bhi chale aur safe rahe
NUMBERS roz 1 lakh file aati hai, din me ~1 lakh second, to ~1-2 upload / sec, jo kam hai
        deadline wale din (31 March) aakhri ghante me ~100 guna, yaani ~150-200 upload / sec
        log haal baar-baar dekhte hain, to padhna bahut zyada hai aur likhna kam
PEHLE   pehle poochho ki jaanch kitna time leti hai (2-3 sec), aur nateeja turant chahiye ya user baad me
        haal dekhega. Turant nahi chahiye to async + tracking
```

---

*ab tootega kahan? sabse pehle jaanch pe, kyunki wo 3 sec leti*

## 1. Maine report.pdf daali, 3 second ghoomta screen dekhta raha

Cue: ASYNC + TRACKING ID (queue, worker)

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c1_1.png?v=5" width="900">

<img src="img/c1_2.png?v=5" width="900">

<img src="img/c1_3.png?v=5" width="900">

</details>

```
HAL     user ko turant tracking id T-101 milti hai aur haal VALIDATING. Khabar Kafka me jaati hai,
        worker baad me Validator chalata hai aur DB me DONE likhta hai. User T-101 se haal dekhta hai
        ↳ agar worker beech me mar gaya? -> khabar dobara milti hai
          kyunki offset commit DB me likhne ke baad hi hota hai
        ↳ agar wahi khabar dobara aa gayi? -> pehle haal dekho, DONE to skip
          T-101 DONE hai to skip kar do (idempotent)
        ↳ agar Validator do baar chal gaya?
          -> jaanch T-101 pe idempotent hai, dobara chalna theek hai, DONE ek hi baar likha jaata hai
        ↳ agar Validator 1 ghanta band? -> 4,000 khabar line me padi rehti
          DONE likhe bina offset aage nahi badhta
        ↳ agar band hai, call karte rahoge? -> nahi, circuit breaker lagao
          3-5 fail pe call band, thodi der baad ek test call.
          Aur doosri company rakho, kyunki unki copy hum khud nahi chala sakte
        ↳ agar Validator band hai, to upload bhi rok dein?
          -> nahi, error diya to 31 March ki deadline gayi aur file gayi hi nahi. Upload le lo,
             khabar Kafka me padi rahegi, Validator aate hi jaanch lega
        ↳ agar is line ke liye Kafka hi kyun?
          -> zaroori nahi hai. ~200 / sec pe SQS ya RabbitMQ kaafi hai. Kafka tab jab bahut bheed ho
             ya khabar dobara padhni ho
        ↳ agar Validator ko 5 GB ki file kaise doge?
          -> file nahi bhejte, 10 min ka signed GET link dete hain. Validator seedha S3 se padhta hai,
             bytes tab bhi hamare server se nahi guzarte
KYUN    jaanch dheemi hai, isliye beech me queue aur worker, dheema kaam line me peeche chala jaata hai.
        User ko turant T-101 mil gaya, to wo file dobara nahi bhejta
        khabar tabhi "padh li" maani jaati hai (offset commit) jab DB me haal likh diya.
        Worker ya Validator gire to khabar line me bachi rehti hai
NAHI    Upload Svc file S3 me rakhe aur Validator call karke jawab ka intezaar kare
        -> ATKA: 31 March ko 200 upload / sec pe har thread 3 sec atka rehta hai, threads khatam ho jaate hain
```

---

*ab jaanch peeche gayi, par 5 GB ki file hamare server se hi guzar rahi*

## 2. Meri 5 GB video hamare server ke raaste S3 jaa rahi

Cue: PRESIGNED URL, bytes seedha S3

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c2_1.png?v=5" width="900">

<img src="img/c2_2.png?v=5" width="900">

<img src="img/c2_3.png?v=5" width="900">

</details>

```
HAL     server sirf ek signed URL deta hai (10 min ka) aur T-102. Phone 5 GB seedha S3 pe bhejta hai,
        phir server ko "/complete T-102" bolta hai
        ↳ agar had 5 GB, koi 50 GB bhej de? -> had paas me, S3 khud reject
          POST me content-length-range, PUT me size sign hota hai (ek PUT max 5 GB).
          S3 event pe size phir se dekh lo
        ↳ agar "/complete" aaya hi nahi? -> S3 event se queue me daalo
          S3 event matlab "file aa gayi". Client pe bharosa nahi karte
        ↳ agar S3 paas ko sahi kaise maanta hai?
          -> S3 wahi hash khud banata hai. Server ne AWS secret se sign kiya tha, to dono mil jaate hain
KYUN    server sirf paas deta hai: ek file ke liye, 10 min ke liye. Bytes seedha app se S3 jaate hain,
        server pe koi bojh nahi
        paas leak ho bhi jaaye to sirf us ek file pe chalta hai, 10 min tak. Paas khatam to naya maango
S3      badi file ke bytes DB me nahi rakhte, S3 me rakhte hain (blob storage)
PEHLE   pehle poochho file kitni badi hai. 10 MB ya 5 GB, design badal jaata hai.
        Badi hai to presigned + multipart
NAHI    bytes server ke raaste bhejo
        -> DOGUNA: phone se 5 GB server pe aate, phir wahi S3 pe jaate. Bytes do baar, thread minuton atka,
           mehnga bhi aur dheema bhi
        AWS ki chaabi hi app ko de do
        -> KABHI: chaabi matlab poora bucket. Paas matlab sirf ek file, 10 min ke liye
```

---

*ab bytes seedha S3 jaate, par beech me net gaya to?*

## 3. 4.5 GB chadh chuki thi, wifi chala gaya

Cue: MULTIPART, sirf bache tukde

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c3_1.png?v=5" width="900">

<img src="img/c3_2.png?v=5" width="900">

<img src="img/c3_3.png?v=5" width="900">

</details>

```
HAL     file 100 tukdon me jaati hai, kai tukde saath, har tukde ki rasid (ETag), aakhir me COMPLETE.
        Wifi gaya to 90 ki rasid S3 pe hai, sirf bache 91 - 100 bhejo (multipart)
        ↳ agar app band, kal wapas aaya? -> S3 se poochho (ListParts U-9)
          rasid wahan hai, app pe nahi
        ↳ agar COMPLETE aaya hi nahi? -> S3 lifecycle adhoore tukde hataye
          kuch din (jaise 7) baad khud hata deta hai (abort)
        ↳ agar tukda kitna bada rakhoge?
          -> S3 ki had hai: min 5 MB (aakhri tukda chhod ke), max 10,000 tukde. Badi file to bada tukda
KYUN    jis tukde ki rasid S3 pe hai, wo pakka pahunch gaya. Rasid S3 pe hai, app pe nahi.
        Net gaya ya app band hua, S3 se poochho aur sirf bache tukde dobara bhejo. User ko shuru se kuch nahi
NAHI    ek hi lamba upload
        -> BEKAAR: 90% pe wifi gaya, to sab bekaar, phir 0 se 5 GB bhejna padta
```

---

*ab upload hua, par app hi crash ho gaya aur kisi ne bataya nahi*

## 4. App crash, "complete" kabhi aaya hi nahi

Cue: SWEEPER job (atki file)

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c4_1.png?v=5" width="900">

<img src="img/c4_2.png?v=5" width="900">

<img src="img/c4_3.png?v=5" width="900">

</details>

```
HAL     har kuch min (jaise 5) ek sweeper job atki file dhoondhta hai. 10 min se haal nahi badla, jabki
        jaanch 3 sec ki hai, to file atki hai. S3 me bytes hain to khud dobara queue me daalo.
        Bytes nahi hain to FAILED, user dobara daale
        ↳ agar /complete aur S3 event dono chhoot gaye? -> isi liye sweeper
          wo har kuch min atki file dhoondhta hai
        ↳ agar sweeper do machine pe chal gaya? -> ek hi chale (lock / leader)
          Ya chalne do, kyunki dobara daalna safe hai
        ↳ agar sweeper hai, to S3 lifecycle kis kaam ka?
          -> wo bytes ki safai karta hai: adhoore uploads ke bytes khud hataata hai.
             DB ka haal sweeper badalta hai
KYUN    har file ka haal DB me hai: UPLOADING -> VALIDATING -> DONE / FAILED. Der tak nahi badla matlab atka
        dobara line me daalna safe hai, worker haal dekh ke skip kar deta hai
NAHI    sweeper hi na ho
        -> ATKA: T-103 DB me UPLOADING pe hamesha atka rehta, bytes S3 me pade, paisa lag raha,
           koi dekhta hi nahi
```

---

*ab file chal padi, par deadline ke din sab haal poochh rahe*

## 5. Deadline ke din sab har 2 sec haal poochh rahe

Cue: CACHE Redis + READ REPLICA

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c5_1.png?v=5" width="900">

<img src="img/c5_2.png?v=5" width="900">

<img src="img/c5_3.png?v=5" width="900">

</details>

```
HAL     GET /status/T-101 pehle Redis se aata hai (1 ms). Redis me nahi mila to read replica se laao
        aur Redis me rakh do (cache + replica)
        ↳ agar DB me DONE, app me VALIDATING? -> DONE pe Redis key hatao
          sirf DB badla, Redis me purana haal pada tha. Worker DONE likhe to usi waqt DB likho
          aur Redis key hatao, agla poochh DB se naya DONE laayega.
          Redis chhoot bhi gaya to umar (TTL) baad naya
        ↳ agar replica peeche, purana haal Redis me baitha? -> SET NX + chhoti umar
          DONE ke baad bhi purana haal Redis me baith sakta hai. Miss pe sirf SET NX + chhoti umar,
          ya primary se padho
        ↳ agar Redis gira? -> seedha replica pe jao, dheema par chalta hai
          Poochne pe rate limit lagao
        ↳ agar har 2 sec poochhna zaroori hai?
          -> gap badhao (2s, 4s, 8s). Haal itni jaldi nahi badalta
        ↳ agar poochhna hi band karwa do to?
          -> server khud bheje (SSE / WebSocket), sirf jab haal badle
KYUN    padhna bahut zyada hai, likhna kam, isliye haal ke liye cache + read replica
        sach DB me hai, Redis sirf tez copy hai. Padhna Redis se, likhna dono me.
        Purana dikhe to chhoti umar (TTL) se khud theek ho jaata hai
        Redis gire to replica se padho, dheema chalega par band nahi hoga
NAHI    har poochh seedha DB pe
        -> GHIRA: 10,000 log har 2 sec poochh rahe, primary DB ghir gaya, sab dheema
```

---

*ab ek file theek, par poora folder aaya to?*

## 6. Maine file nahi, poora folder daala (1000 file)

Cue: PARENT id + CHILD id

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c6_1.png?v=5" width="900">

<img src="img/c6_2.png?v=5" width="900">

<img src="img/c6_3.png?v=5" width="900">

</details>

```
HAL     folder ka ek id hai (P-1), har file uska child. Batch init me 1000 URL milte hain, har file ka
        child id. Har child DONE pe folder pe ginti +1 hoti hai (done_count). Sab DONE to P-1 DONE
        ↳ agar folder ki koi file FAILED? -> P-1 PARTIAL, sirf wahi dobara
          sirf fail wali file dobara, poora folder nahi
        ↳ agar do worker ek saath done_count +1? -> ek UPDATE, ginti nahi khoti
          done_count = done_count + 1, DB ke andar hota hai
        ↳ agar lakh chhoti files hon to?
          -> ZIP. Client ek file bana ke bheje, server use khol le
KYUN    folder ek bada id hai, har file ka chhota id. Folder ka haal bachchon ka jod hai. Sab DONE to DONE,
        koi FAILED to PARTIAL. Ek file fail hui to sirf wahi dobara
PEHLE   pehle poochho ki sirf file aayegi ya folder bhi
NAHI    har file ka alag id hi rakho
        -> ANDHA: folder ka koi haal nahi, "folder ho gaya?" ke liye 1000 id khud dekhne padte
```

---

*ab sab chal raha, par koi doosre ki file maange to?*

## 7. Ravi ne mera trackingId daal ke meri file le li

Cue: OWNER CHECK (AUTHZ), 403

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c7_1.png?v=5" width="900">

<img src="img/c7_2.png?v=5" width="900">

<img src="img/c7_3.png?v=5" width="900">

</details>

```
HAL     har request pe check hota hai ki file ka maalik hi maang raha hai. JWT bolta hai ravi, T-101 ka
        maalik arpan hai, to 403 (owner check)
        ↳ agar id to random tha, phir check kyun? -> id leak hoti (screenshot)
          id leak hoti hai: log, screenshot, link se
        ↳ agar admin maange to? -> owner ho ya admin role ho, warna 403
        ↳ agar PR me owner check kahan dhoondhoge?
          -> har handler ki pehli line me: file.owner_id == jwt.user_id ya admin, warna 403.
             userId JWT se aata hai, body se kabhi nahi
KYUN    kaun ho, ye gateway batata hai (JWT). Is file pe haq hai ya nahi, ye service khud dekhti hai, har
        request pe (owner_id == jwt.user_id). Id leak ho tab bhi file nahi milti
NAHI    id hai to file de do
        -> CHORI: Ravi ne GET /download/T-101 kiya, service ne id dekhi aur file de di
```

---

*ab andar se safe, par link hi bahar chala gaya to?*

## 8. Maine S3 ka seedha link WhatsApp pe bhej diya

Cue: SIGNED GET, 5 min ka naya paas

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c8_1.png?v=5" width="900">

<img src="img/c8_2.png?v=5" width="900">

<img src="img/c8_3.png?v=5" width="900">

</details>

```
HAL     DB me sirf S3 key (pata) rakhte hain. Download maanga to pehle maalik check, phir naya 5 min ka
        link (signed GET)
        ↳ agar 5 min me download adhoora? -> chalu download beech me nahi katta
          umar sirf shuru karne ki hai
        ↳ agar 5 min ke andar link aage bhej diya? -> ye risk maana hai
          Zyada sensitive ho to CloudFront signed URL + IP ki had
KYUN    pakka link kabhi bahar nahi jaata. Link aage gaya to bas 5 min tak chalta hai. Group me kisi ne
        1 ghante baad khola to expire, nahi khula
NAHI    pakka link DB me rakh ke de do
        -> HAMESHA: wo hamesha chalta hai, WhatsApp group me koi bhi kabhi bhi khole. File sabke haath,
           hamesha ke liye
```

---

*ab file ka raasta safe, par file khud hi jhooth bol rahi ho to?*

## 9. Naam report.pdf, andar EXE nikla

Cue: MAGIC BYTES (pehle kuch byte)

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c9_1.png?v=5" width="900">

<img src="img/c9_2.png?v=5" width="900">

<img src="img/c9_3.png?v=5" width="900">

</details>

```
HAL     worker file ke pehle 4 byte padhta hai. Shuru me "MZ" hai to EXE, "%PDF" nahi, to reject (magic bytes)
        ↳ agar shuru PDF, andar virus? -> magic bytes sirf type, scan bhi chahiye
          magic bytes sirf type batate hain. Scan bhi chahiye (ClamAV / S3 malware)
        ↳ agar jaanch me fail hui file? -> quarantine bucket (download band)
          Baad me delete, ya security wale dekhein
KYUN    client ne jo bheja (naam, type, size) wo kuch sach nahi. Sach file ke bytes me hai: pehle kuch byte
        type batate hain, scan virus batata hai. Jaanch pass nahi hui to DONE nahi, reject
PEHLE   pehle poochho ki kaunse type allowed hain
NAHI    naam / type pe bharosa karo (report.pdf, application/pdf)
        -> JHOOTH: wo client ka bola hua hai, andar virus.exe aa gaya
```

---

## Poora design ek saans me

```
User -> Gateway / LB (login) -> Upload Svc (file ki jaankari + signed URL, bytes ko haath nahi)
-> client SEEDHA S3 pe (tmp/, badi file multipart) -> complete -> Kafka -> Worker (jaanch, magic bytes)
-> DB me haal -> Redis + read replica (haal poochhna) · download = owner check + chhota signed GET
Sweeper + S3 lifecycle = atki / adhoori file
DB: PostgreSQL -> trackingId se seedha dhoondhna simple, par haal
    (UPLOADING -> VALIDATING -> DONE / FAILED) ko ACID chahiye
    files: trackingId (KEY) | ownerId | status | s3_key
```

Bolne ke liye: server sirf paas deta, file seedha S3 me (badi ho to tukdon me). Jaanch 3 second ki, to Arpan ko turant
T-101, worker line se jaanchta, file ke pehle byte dekhta, haal likhta. Haal baar-baar poochha jaata, to Redis + copy,
haal badle to Redis bhi. Maalik check aur chhote paas se safe, S3 niyam + atki file wali job se safai. Aage: fail pe
dobara koshish, download ke liye CDN.

---

## 10 guna traffic

- **Upload Svc**: stateless, kai machine + LB.
- **S3**: AWS khud scale karta. Zyada download wali file CDN (CloudFront) pe.
- **DB**: haal poochhna = cache + read replica. Bahut files = shard (trackingId).
- **Redis**: purana haal na dikhe, worker saath me update kare.
- **Kafka**: jaanch ki line lambi, worker badhao (line ki lambai dekh ke).
- **Adhoori file**: S3 lifecycle + sweeper.

**Pooche: 10 guna pe kaise badhaoge?** File ka raasta chalo, pehle jo toote.

**Pooche: single point of failure kya hai?** Validator (bahar wala). Circuit breaker + doosri company.

**Pooche: chal raha hai, kaise pata?** Misaal: raat ko Validator dheema, line me ~5,000 file atki, alert CloudWatch /
PagerDuty se. Health check sirf batata "zinda hai", "dheema hai" nahi batata. Isliye apne teen number dekho: line kitni
lambi, jaanch wali company kitni der me jawab de rahi aur kitni baar fail, aur kitni files 10 minute se "jaanch ho
rahi" pe atki. Had paar = alert, user ke shikayat karne se pehle. Company baar-baar fail ho to us pe call band (circuit
breaker) aur doosri company.

---

## Pooche to (kisi card me nahi baitha)

API: POST /upload/init { fileName, size } -> signed S3 URL + trackingId (server sirf URL deta). Phir client seedha S3
pe (presigned POST, size ki had ke saath; ya PUT). POST /upload/complete { trackingId } -> "ho gaya, jaancho" (queue).
GET /status/{trackingId} -> VALIDATING / DONE / FAILED. GET /download/{trackingId} -> signed GET URL -> seedha S3.

DB: files table: trackingId (KEY) | fileName | ownerId | status | s3_key | createdAt | updatedAt. Haal: UPLOADING ->
VALIDATING -> DONE / FAILED. "Kahan tak pahunchi" hi gadbad sambhalne ki buniyaad hai. PostgreSQL kyunki dhoondhna
simple par haal ko ACID chahiye. Bahut bada + sirf key-value ho to NoSQL bhi chalta. Paisa hota to hamesha SQL.

Dedup: file ka hash (MD5 / SHA). Pehle se hai to dobara store nahi, jagah bachi.

Koi bheed / hamla kare? Rate limit har user pe, sabse aage WAF, HTTPS, secrets vault me.

---

ARCHETYPE B+F · CONCEPTS: [message-queues](../../FOUNDATIONS/07_message_queues.md) · [reliability/SPOF](../../FOUNDATIONS/11_reliability_spof_cloud.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
