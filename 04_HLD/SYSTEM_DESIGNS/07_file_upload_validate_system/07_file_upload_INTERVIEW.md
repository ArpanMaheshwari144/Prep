# File Upload + Validate + Track

> User file / folder deta hai, bahar ki company (third-party) use jaanchti hai (2-3 sec), file store hoti, user ko tracking link milta. JP ne ye sach me poocha tha (asli SDE-3 writeup). Aam product design, finance nahi.
> Dil teen cheezein: user ruke nahi (async + trackingId), file ke bytes hamare server se na guzrein (presigned URL), aur haal hamesha pata.
>
> Mock me sirf heading + Cue + kahani ki tasveer. Neeche har card: HAL · KYUN · NAHI, phir YAAD RAKH dabba + CROSS-Q dabba (sawaal → cue, poora jawab).
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

> [!IMPORTANT]
> **RAIL: har design isi kram me chalta (8 dabbe, sab file me same)**
> kaam chala → bheed aayi → machine giri → do log ek saath → kuch dheema → data bada → door ke log → bahar ka hamla
>
> | rail | is design me | card |
> |---|---|---|
> | 1 kaam chala | report.pdf daali, jaanch 3 sec, screen ghoomti (**is design ka khaas: async jaanch + haal ka tracking id**) | 1 |
> | 2 bheed aayi | deadline pe sab har 2 sec haal poochh rahe | 5 |
> | 3 machine giri | app crash, "complete" kabhi aaya hi nahi · Validator ghanta band | 4, 1 |
> | 4 do log ek saath | do worker ek saath done_count +1 · wahi khabar dobara | 6, 1 |
> | 5 kuch dheema | 5 GB server ke raaste · 4.5 GB pe wifi gaya | 2, 3 |
> | 6 data bada | poora folder (1000 file), haal kaise pata | 6 |
> | 7 door ke log | nahi aata (download CDN se ho sakta) | - |
> | 8 bahar ka hamla | Ravi ne mera trackingId daala · S3 link WhatsApp pe · naam pdf andar EXE | 7, 8, 9 |


> [!NOTE]
> **AWS pe kaunsa DB (interview me naam bolna ho to)** · SQL wala = RDS, Cassandra RDS pe nahi chalta
>
> - File ka haal (SQL) → **AWS RDS (Postgres / MySQL)**: trackingId se dhoondhna + haal ko ACID
> - File khud → **S3**: bytes DB me nahi
> - Haal ka cache (Redis) → **ElastiCache (Redis)**: baar-baar GET /status tez

---

*ab tootega kahan? sabse pehle jaanch pe, kyunki wo 3 sec leti*

## 1. Maine report.pdf daali, 3 second ghoomta screen dekhta raha

**Rail 1 KAAM CHALA (khaas)** · Cue: ASYNC + TRACKING ID (queue, worker)

<img src="img/c1_poster.png?v=1" width="1000" alt="card 1: kya toota, kaise bacha">


```
HAL     user ko turant tracking id T-101 milti hai aur haal VALIDATING. Khabar Kafka me jaati hai,
        worker baad me Validator chalata hai aur DB me DONE likhta hai. User T-101 se haal dekhta hai
KYUN    jaanch dheemi hai, isliye beech me queue aur worker, dheema kaam line me peeche chala jaata hai.
        User ko turant T-101 mil gaya, to wo file dobara nahi bhejta
        khabar tabhi "padh li" maani jaati hai (offset commit) jab DB me haal likh diya.
        Worker ya Validator gire to khabar line me bachi rehti hai
NAHI    Upload Svc file S3 me rakhe aur Validator call karke jawab ka intezaar kare
        -> ATKA: 31 March ko 200 upload / sec pe har thread 3 sec atka rehta hai, threads khatam ho jaate hain
```

> [!IMPORTANT]
> **Yaad rakh:** Turant T-101 + VALIDATING. Khabar Kafka me, worker peeche jaanch ke DONE likhta.


---

*ab jaanch peeche gayi, par 5 GB ki file hamare server se hi guzar rahi*

## 2. Meri 5 GB video hamare server ke raaste S3 jaa rahi

**Rail 5 KUCH DHEEMA (bytes)** · Cue: PRESIGNED URL, bytes seedha S3

<img src="img/c2_poster.png?v=1" width="1000" alt="card 2: kya toota, kaise bacha">


```
HAL     server sirf ek signed URL deta hai (10 min ka) aur T-102. Phone 5 GB seedha S3 pe bhejta hai,
        phir server ko "/complete T-102" bolta hai
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

> [!IMPORTANT]
> **Yaad rakh:** Server sirf 10 min ka signed URL + T-102 deta, phone bytes seedha S3 pe. Had (5 GB) paas me sign.


---

*ab bytes seedha S3 jaate, par beech me net gaya to?*

## 3. 4.5 GB chadh chuki thi, wifi chala gaya

**Rail 5 KUCH DHEEMA (wifi)** · Cue: MULTIPART, sirf bache tukde

<img src="img/c3_poster.png?v=1" width="1000" alt="card 3: kya toota, kaise bacha">


```
HAL     file 100 tukdon me jaati hai, kai tukde saath, har tukde ki rasid (ETag), aakhir me COMPLETE.
        Wifi gaya to 90 ki rasid S3 pe hai, sirf bache 91 - 100 bhejo (multipart)
KYUN    jis tukde ki rasid S3 pe hai, wo pakka pahunch gaya. Rasid S3 pe hai, app pe nahi.
        Net gaya ya app band hua, S3 se poochho aur sirf bache tukde dobara bhejo. User ko shuru se kuch nahi
NAHI    ek hi lamba upload
        -> BEKAAR: 90% pe wifi gaya, to sab bekaar, phir 0 se 5 GB bhejna padta
```

> [!IMPORTANT]
> **Yaad rakh:** Multipart: 100 tukde, har tukde ki rasid (ETag) S3 pe. Wifi gaya to sirf bache tukde.


---

*ab upload hua, par app hi crash ho gaya aur kisi ne bataya nahi*

## 4. App crash, "complete" kabhi aaya hi nahi

**Rail 3 MACHINE GIRI** · Cue: SWEEPER job (atki file)

<img src="img/c4_poster.png?v=1" width="1000" alt="card 4: kya toota, kaise bacha">


```
HAL     har kuch min (jaise 5) ek sweeper job atki file dhoondhta hai. 10 min se haal nahi badla, jabki
        jaanch 3 sec ki hai, to file atki hai. S3 me bytes hain to khud dobara queue me daalo.
        Bytes nahi hain to FAILED, user dobara daale
KYUN    har file ka haal DB me hai: UPLOADING -> VALIDATING -> DONE / FAILED. Der tak nahi badla matlab atka
        dobara line me daalna safe hai, worker haal dekh ke skip kar deta hai
NAHI    sweeper hi na ho
        -> ATKA: T-103 DB me UPLOADING pe hamesha atka rehta, bytes S3 me pade, paisa lag raha,
           koi dekhta hi nahi
```

> [!IMPORTANT]
> **Yaad rakh:** Har kuch min sweeper: 10 min se haal nahi badla = atka. Bytes hain to dobara line me, nahi to FAILED.


---

*ab file chal padi, par deadline ke din sab haal poochh rahe*

## 5. Deadline ke din sab har 2 sec haal poochh rahe

**Rail 2 BHEED AAYI** · Cue: CACHE Redis + READ REPLICA

<img src="img/c5_poster.png?v=1" width="1000" alt="card 5: kya toota, kaise bacha">


```
HAL     GET /status/T-101 pehle Redis se aata hai (1 ms). Redis me nahi mila to read replica se laao
        aur Redis me rakh do (cache + replica)
KYUN    padhna bahut zyada hai, likhna kam, isliye haal ke liye cache + read replica
        sach DB me hai, Redis sirf tez copy hai. Padhna Redis se, likhna dono me.
        Purana dikhe to chhoti umar (TTL) se khud theek ho jaata hai
        Redis gire to replica se padho, dheema chalega par band nahi hoga
NAHI    har poochh seedha DB pe
        -> GHIRA: 10,000 log har 2 sec poochh rahe, primary DB ghir gaya, sab dheema
```

> [!IMPORTANT]
> **Yaad rakh:** GET /status pehle Redis (1 ms), miss pe replica, Redis me rakho. DONE pe Redis key hatao.


---

*ab ek file theek, par poora folder aaya to?*

## 6. Maine file nahi, poora folder daala (1000 file)

**Rail 6 DATA BADA (folder)** · Cue: PARENT id + CHILD id

<img src="img/c6_poster.png?v=1" width="1000" alt="card 6: kya toota, kaise bacha">


```
HAL     folder ka ek id hai (P-1), har file uska child. Batch init me 1000 URL milte hain, har file ka
        child id. Har child DONE pe folder pe ginti +1 hoti hai (done_count). Sab DONE to P-1 DONE
KYUN    folder ek bada id hai, har file ka chhota id. Folder ka haal bachchon ka jod hai. Sab DONE to DONE,
        koi FAILED to PARTIAL. Ek file fail hui to sirf wahi dobara
PEHLE   pehle poochho ki sirf file aayegi ya folder bhi
NAHI    har file ka alag id hi rakho
        -> ANDHA: folder ka koi haal nahi, "folder ho gaya?" ke liye 1000 id khud dekhne padte
```

> [!IMPORTANT]
> **Yaad rakh:** Folder ka id P-1, har file child. Child DONE pe ginti +1 (ek UPDATE), sab DONE to P-1 DONE.


---

*ab sab chal raha, par koi doosre ki file maange to?*

## 7. Ravi ne mera trackingId daal ke meri file le li

**Rail 8 BAHAR KA HAMLA (id)** · Cue: OWNER CHECK (AUTHZ), 403

<img src="img/c7_poster.png?v=1" width="1000" alt="card 7: kya toota, kaise bacha">


```
HAL     har request pe check hota hai ki file ka maalik hi maang raha hai. JWT bolta hai ravi, T-101 ka
        maalik arpan hai, to 403 (owner check)
KYUN    kaun ho, ye gateway batata hai (JWT). Is file pe haq hai ya nahi, ye service khud dekhti hai, har
        request pe (owner_id == jwt.user_id). Id leak ho tab bhi file nahi milti
NAHI    id hai to file de do
        -> CHORI: Ravi ne GET /download/T-101 kiya, service ne id dekhi aur file de di
```

> [!IMPORTANT]
> **Yaad rakh:** Har request pe owner check: JWT ka user = file ka maalik? Nahi to 403 (admin role chhoot).


---

*ab andar se safe, par link hi bahar chala gaya to?*

## 8. Maine S3 ka seedha link WhatsApp pe bhej diya

**Rail 8 BAHAR KA HAMLA (link)** · Cue: SIGNED GET, 5 min ka naya paas

<img src="img/c8_poster.png?v=1" width="1000" alt="card 8: kya toota, kaise bacha">


```
HAL     DB me sirf S3 key (pata) rakhte hain. Download maanga to pehle maalik check, phir naya 5 min ka
        link (signed GET)
KYUN    pakka link kabhi bahar nahi jaata. Link aage gaya to bas 5 min tak chalta hai. Group me kisi ne
        1 ghante baad khola to expire, nahi khula
NAHI    pakka link DB me rakh ke de do
        -> HAMESHA: wo hamesha chalta hai, WhatsApp group me koi bhi kabhi bhi khole. File sabke haath,
           hamesha ke liye
```

> [!IMPORTANT]
> **Yaad rakh:** DB me sirf S3 key. Download pe pehle maalik check, phir naya 5 min ka signed link.


---

*ab file ka raasta safe, par file khud hi jhooth bol rahi ho to?*

## 9. Naam report.pdf, andar EXE nikla

**Rail 8 BAHAR KA HAMLA (file)** · Cue: MAGIC BYTES (pehle kuch byte)

<img src="img/c9_poster.png?v=1" width="1000" alt="card 9: kya toota, kaise bacha">


```
HAL     worker file ke pehle 4 byte padhta hai. Shuru me "MZ" hai to EXE, "%PDF" nahi, to reject (magic bytes)
KYUN    client ne jo bheja (naam, type, size) wo kuch sach nahi. Sach file ke bytes me hai: pehle kuch byte
        type batate hain, scan virus batata hai. Jaanch pass nahi hui to DONE nahi, reject
PEHLE   pehle poochho ki kaunse type allowed hain
NAHI    naam / type pe bharosa karo (report.pdf, application/pdf)
        -> JHOOTH: wo client ka bola hua hai, andar virus.exe aa gaya
```

> [!IMPORTANT]
> **Yaad rakh:** Pehle 4 byte padho (magic bytes): "%PDF" = PDF, "MZ" = EXE reject. Virus ke liye scan alag.


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

Dedup: file ka hash (SHA-256). Pehle se hai to dobara store nahi, jagah bachi. Dedup sirf user ke andar, warna
"ye file pehle se hai" se doosre ki file ka pata chal jaata (leak).

Koi bheed / hamla kare? Rate limit har user pe, sabse aage WAF, HTTPS, secrets vault me.

---

ARCHETYPE B+F · CONCEPTS: [message-queues](../../FOUNDATIONS/07_message_queues.md) · [reliability/SPOF](../../FOUNDATIONS/11_reliability_spof_cloud.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
