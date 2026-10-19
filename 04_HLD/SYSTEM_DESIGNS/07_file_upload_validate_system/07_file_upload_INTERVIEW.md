# File Upload + Validate + Track

> User file / folder deta hai, bahar ki company (third-party) use jaanchti hai (2-3 sec), file store hoti, user ko tracking link milta. JP ne ye sach me poocha tha (asli SDE-3 writeup). Aam product design, finance nahi.
> Dil teen cheezein: user ruke nahi (async + trackingId), file ke bytes hamare server se na guzrein (presigned URL), aur haal hamesha pata.
>
> Har card: kya toota -> kya kiya -> yahi kyun -> doosra kyun nahi. Kram me chalo. "Pooche" wali line = interviewer aage poochhe to.

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c9_2-40c6937c.svg)

---

## Shuru

Sabse simple: Client file Upload service ko de. Service file S3 me rakhe, jaanch wale (Validator) ko call kare (2-3
sec), jawab wapas.

Pehle poochho. File kitni badi? 10 MB ya 5 GB, design badal jaata; badi hai to presigned + multipart. Kaunse type
allowed? Jaanch kitna time leti (2-3 sec)? Sirf file ya folder bhi? Nateeja turant chahiye ya baad me haal dekhega?
Turant nahi chahiye to async + tracking.

Kya chahiye: upload, bahar wale se jaanch, store, tracking link / haal. Scope bahar: editing, sharing, versions. Kaisa
chale: user ruke nahi (dil), file khoye nahi, haal hamesha pata (kahan tak pahunchi), bheed + safe.

Numbers: 1 lakh file roz, din me ~1 lakh second, to ~1-2 upload har second (kam). Par deadline wale din (31 March)
aakhri ghante me ~100 guna, ~150-200 upload har second. Aur log haal baar-baar dekhte, padhna bahut zyada, likhna kam.
Isliye haal ke liye cache + read replica. Badi file ke bytes DB me nahi, S3 me (blob storage). Jaanch dheemi hai,
isliye beech me queue + worker.

---

*ab tootega kahan? sabse pehle jaanch pe, kyunki wo 3 sec leti*

## 1. Maine report.pdf daali, 3 second ghoomta screen dekhta raha

31 March, 200 upload har second, har server thread Validator ke jawab tak 3 sec atka, threads khatam. Isliye user ko
turant tracking id (T-101) aur haal VALIDATING de dete hain. Khabar Kafka me jaati, worker baad me Validator chalata,
DB me DONE likhta (async). User T-101 se haal dekhta.

Ye kyun chalta? Dheema kaam line me peeche. User ko turant T-101, wo kuch dobara nahi bhejta. Khabar tabhi "padh li"
(offset commit) jab DB me haal likh diya. Worker ya Validator gire to khabar line me bachi rehti.

Server Validator ka intezaar kare? Har thread 3 sec atka, 200 upload har second pe threads khatam.

**Pooche: worker beech me mar gaya?** Khabar dobara milti, kyunki offset commit DB likhne ke baad tha.

**Pooche: wahi khabar dobara aayi?** Pehle haal dekho. T-101 DONE hai to skip (idempotent).

**Pooche: Validator do baar chal gaya?** Jaanch T-101 pe idempotent hai. Dobara chalna theek, DONE ek hi baar.

**Pooche: Validator 1 ghanta band?** 4,000 khabar line me padi rehti. DONE bina offset aage nahi.

**Pooche: us ghante bhi call karte rahoge?** Nahi, circuit breaker: 3-5 fail pe band, thodi der baad ek test call.
Aur doosri company (unki copy hum nahi chala sakte).

**Pooche: Validator band hai, upload bhi rok dein?** Nahi. Upload pe error diya to 31 March ki deadline nikal gayi,
file gayi hi nahi. Isliye upload le lo, khabar Kafka me padi, Validator aate hi jaanche. User ka kaam hua, jaanch baad me.

**Pooche: is line ke liye Kafka hi kyun?** Zaroori nahi. ~200 har second pe SQS / RabbitMQ kaafi. Kafka tab jab bahut
bheed ho ya dobara padhna ho.

---

*ab jaanch peeche gayi, par 5 GB ki file hamare server se hi guzar rahi*

## 2. Meri 5 GB video hamare server ke raaste S3 jaa rahi

Phone se 5 GB hamare server pe, phir wahi 5 GB S3 pe. Bytes do baar, thread minuton atka, mehnga + dheema. Isliye
server sirf ek signed URL deta (10 min ka) + T-102. Phone 5 GB seedha S3 pe bhejta, phir "/complete T-102" (presigned URL).

Ye kyun chalta? Server sirf paas deta: ek file, 10 min. Bytes seedha app se S3. Server pe bojh nahi, aur paas leak ho
bhi jaaye to sirf us ek file pe, 10 min tak. Paas khatam to naya maango.

AWS chaabi hi app ko de do? Kabhi nahi. Chaabi = poora bucket. Paas = sirf ek file, 10 min.

**Pooche: had 5 GB, koi 50 GB bheje?** Had paas me hi: POST me content-length-range, PUT me size sign (ek PUT max
5 GB). S3 khud reject karta. S3 event pe size phir se dekho.

**Pooche: "/complete" aaya hi nahi?** S3 event ("file aa gayi") se queue me daalo. Client pe bharosa nahi.

**Pooche: S3 paas kaise maanta?** Wahi hash khud banata. Server ne AWS secret se sign kiya tha.

---

*ab bytes seedha S3 jaate, par beech me net gaya to?*

## 3. 4.5 GB chadh chuki thi, wifi chala gaya

Ek hi lamba upload tha, 90% pe wifi gaya, phir 0 se 5 GB. Isliye file tukdon me: 100 tukde, kai saath jaate, har
tukde ki rasid (ETag), aakhir me COMPLETE. Wifi gaya to 90 ki rasid S3 pe hai, sirf bache 91 - 100 bhejo (multipart).

Ye kyun chalta? Jis tukde ki rasid S3 pe hai, wo pakka. Rasid S3 pe hai, app pe nahi. Net gaya ya app band, S3 se
poochho, sirf bache tukde dobara. User ko shuru se kuch nahi.

Ek hi lamba upload? Ek baar toota to sab bekaar, 0 se shuru.

**Pooche: app band, kal wapas aaya?** S3 se poochho (ListParts U-9). Rasid wahan hai, app pe nahi.

**Pooche: COMPLETE kabhi aaya hi nahi?** Adhoore tukde S3 lifecycle kuch din (jaise 7) baad hata deta (abort).

**Pooche: tukda kitna bada rakhoge?** S3 ki had: min 5 MB (aakhri chhod ke), max 10,000 tukde. Badi file = bada tukda.

---

*ab upload hua, par app hi crash ho gaya aur kisi ne bataya nahi*

## 4. App crash, "complete" kabhi aaya hi nahi

T-103 DB me UPLOADING pe atka hamesha, bytes S3 me pade (paisa lag raha), koi nahi dekhta. Isliye har kuch min (jaise
5) ek job atki file dhoondhti (sweeper). 10 min se haal nahi badla, jabki jaanch 3 sec ki hai, to atka. S3 me bytes
hain to khud dobara queue me. Nahi hain to FAILED, user dobara daale.

Ye kyun chalta? Har file ka haal DB me hai: UPLOADING -> VALIDATING -> DONE / FAILED. Der se nahi badla = atka.
Dobara line me daalna safe hai, worker haal dekh ke skip kar deta.

Sweeper na ho? File atki hi rehti, koi nahi dekhta.

**Pooche: /complete aur S3 event dono chhoot gaye?** Isi liye sweeper hai, har kuch min atki file dhoondhe.

**Pooche: sweeper do machine pe chal gaya?** Ek hi chale (lock / leader). Ya chalne do, dobara daalna safe hai.

**Pooche: to S3 lifecycle kis kaam ka?** Bytes ki safai: adhoore uploads ke bytes khud hataata. DB ka haal sweeper badle.

---

*ab file chal padi, par deadline ke din sab haal poochh rahe*

## 5. Deadline ke din sab har 2 sec haal poochh rahe

10,000 log har 2 sec, har poochh seedha primary DB pe, DB ghira, sab dheema. Isliye GET /status/T-101 pehle Redis se
(1 ms). Redis me nahi mila to read replica se laao, Redis me rakho (cache + replica).

Ye kyun chalta? Sach DB me, Redis sirf tez copy. Padhna Redis se, likhna dono me. Purana dikhe to chhoti umar (TTL)
se khud theek. Redis gire to replica se, dheema par band nahi.

Har poochh seedha DB pe? 10,000 log har 2 sec, primary ghir gaya.

**Pooche: worker ne DB me DONE likha, app abhi bhi VALIDATING kyun?** Sirf DB badla tha, Redis me purana VALIDATING
pada tha. Isliye worker DONE likhe to usi waqt Redis bhi: DB likho, Redis key hatao, agla poochh DB se naya DONE laaye.
Redis chhoota bhi to umar (TTL) baad naya.

**Pooche: replica peeche thi, purana haal Redis me baith gaya?** DONE ke baad purana haal Redis me baith sakta. Miss
pe sirf SET NX + chhoti umar, ya primary se padho.

**Pooche: har 2 sec poochna zaroori?** Gap badhao (2s, 4s, 8s). Haal itni jaldi nahi badalta.

**Pooche: poochhna hi band karwa do?** Server khud bheje (SSE / WebSocket), haal badla tab hi.

**Pooche: Redis gira?** Replica pe seedha. Dheema par chalta. Poochne pe rate limit.

---

*ab ek file theek, par poora folder aaya to?*

## 6. Maine file nahi, poora folder daala (1000 file)

1000 file, har ek ka alag tracking id, folder ka koi haal nahi. "Folder ho gaya?" ke liye 1000 id dekhun? Isliye folder
ka ek id (P-1), har file uska child. Batch init me 1000 URL, har file ka child id. Har child DONE pe folder pe ginti +1
(done_count). Sab DONE = P-1 DONE (parent + child).

Ye kyun chalta? Folder = bada id, har file ka chhota id. Folder ka haal = bachchon ka jod. Sab DONE = DONE, koi FAILED
= PARTIAL. Ek file fail to sirf wahi dobara, poora folder nahi.

Har file ka alag id hi rakhte? Folder ka haal pata hi nahi, 1000 id khud dekho.

**Pooche: koi file FAILED?** P-1 PARTIAL. Sirf fail wali dobara, poora folder nahi.

**Pooche: do worker saath done_count +1 karein?** Ek UPDATE: done_count = done_count + 1, DB ke andar. Ginti nahi khoti.

**Pooche: lakh chhoti files ho to?** ZIP. Client ek file bana ke bheje, server khol le.

---

*ab sab chal raha, par koi doosre ki file maange to?*

## 7. Ravi ne mera trackingId daal ke meri file le li

Ravi ne GET /download/T-101 kiya, service ne id dekhi aur de di, file chori. Isliye har request pe check: file ka
maalik = maangne wala? JWT bole ravi, T-101 ka maalik arpan, to 403 (owner check).

Ye kyun chalta? Kaun ho, ye gateway batata (JWT). Is file pe haq hai ya nahi, ye service khud dekhti, har request pe
(owner_id == jwt.user_id). Id leak ho jaaye tab bhi file nahi milti.

Id hai to file de do? Wahi chori.

**Pooche: id to random tha, phir check kyun?** Id leak hoti hai: log, screenshot, link.

**Pooche: admin maange?** Owner ya admin role, warna 403.

**Pooche: PR me owner check kahan dhoondhoge?** Har handler ki pehli line: file.owner_id == jwt.user_id ya admin,
warna 403. userId JWT se, body se kabhi nahi.

---

*ab andar se safe, par link hi bahar chala gaya to?*

## 8. Maine S3 ka seedha link WhatsApp pe bhej diya

Pakka link hamesha chalta, WhatsApp group me gaya, koi bhi kabhi bhi khole. Isliye DB me sirf S3 key rakhte. Download
maanga to maalik check, phir naya 5 min ka link (signed GET).

Ye kyun chalta? DB me sirf pata (S3 key), pakka link kabhi bahar nahi. Har download pe maalik check, naya 5 min link.
Link aage gaya to bas 5 min tak chalta. Group me 1 ghante baad khola, expire, nahi khula.

Pakka link DB me rakh ke de do? File sabke haath, hamesha ke liye.

**Pooche: 5 min me download adhoora reh gaya?** Umar sirf shuru ki hai. Chalu download beech me nahi katta.

**Pooche: 5 min ke andar hi aage bhej diya?** Risk maana. Zyada sensitive ho to CloudFront signed URL + IP had.

---

*ab file ka raasta safe, par file khud hi jhooth bol rahi ho to?*

## 9. Naam report.pdf, andar EXE nikla

Upload pe naam report.pdf, type application/pdf, humne client ka bola maana, andar virus.exe. Isliye worker file ke
pehle 4 byte padhta. Shuru "MZ" = EXE, "%PDF" nahi, reject (magic bytes).

Ye kyun chalta? Client ka bheja (naam, type, size) kuch sach nahi. Sach file ke bytes me: pehle kuch byte = type, scan
= virus. Jaanch pass nahi to file DONE nahi, reject.

Naam / type pe bharosa? Wo client ka bola hai, virus andar aa gaya.

**Pooche: shuru PDF hai, par andar virus?** Magic bytes sirf type batate. Scan bhi chahiye (ClamAV / S3 malware).

**Pooche: jaanch me fail hui file ka kya?** Quarantine bucket (download band). Baad me delete, ya security dekhe.

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
