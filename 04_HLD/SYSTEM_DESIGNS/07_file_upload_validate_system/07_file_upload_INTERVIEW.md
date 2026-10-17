# File Upload + Validate + Track

> User file / folder deta hai -> bahar ki company (third-party) use JAANCHTI hai (2-3 sec) -> file store -> user ko TRACKING link.
> JP ne ye sach me poocha tha (asli SDE-3 writeup). Aam product design, finance nahi.
> Is design ka dil: **user ruke nahi** (async + trackingId) + **file ke bytes hamare server se na guzrein** (presigned URL) + **haal hamesha pata**.

---

## TASVEER (ByteByteGo / Alex Xu · CC BY-NC-ND 4.0)

![How to Upload a Large File to S3](https://assets.bytebytego.com/diagrams/0284-multipart-upload.png)
Source: [How to Upload a Large File to S3](https://bytebytego.com/guides/how-to-upload-a-large-file-to-s3/)
(badi file -> presigned URL + MULTIPART: tukdon me, tukda fail to sirf wahi dobara)

---

## SHURU: pehle poocho, phir numbers

```
POOCHO:  file kitni badi? (10 MB ya 5 GB — design badal jaata)  <- badi = presigned + multipart
         kaunse type allowed? · jaanch kitna time leti? (2-3 sec) · sirf FILE ya FOLDER bhi?
         nateeja turant chahiye ya baad me haal dekhega?  <- turant nahi = async + tracking

KYA KARE (FR):    upload · bahar wale se JAANCH · store · tracking link / haal
                  scope bahar: editing · sharing · versions
KAISA CHALE (NFR): user ruke nahi (dil) · file khoye nahi · haal
                   HAMESHA pata (kahan tak pahunchi) · bheed + safe

NUMBERS (andaaza):
   1 lakh file / din, din me ~1 lakh second -> ~1-2 upload / second (kam)
   par log haal BAAR-BAAR dekhte -> padhna bahut zyada, likhna kam
   -> haal ke liye CACHE + read replica
   -> badi file ke bytes DB me nahi, S3 me (blob storage)
   -> jaanch dheemi -> beech me QUEUE + worker
```

---

## ════ MOCK SE PEHLE: SIRF YE CARD PADHO ════

> Har card: DIKKAT 1 (kya toota) -> HAL (kya kiya) -> diagram -> DIKKAT 2, 3 ... (interviewer ke
> agle sawaal) har ek ke neeche uska HAL. Revise: sirf baayein column padho.

#### DABBA 0: SABSE SIMPLE (yahan se shuru, phir "kahan tootega")
```
Client file Upload service ko de. Service file S3 me rakhe,
jaanch wale (Validator) ko call kare (2-3 sec), jawab wapas.
```
```mermaid
flowchart TD
    n_CLIENT["CLIENT"]
    n_Upload_Svc["Upload Svc"]
    n_Validator["Validator"]
    n_S3["S3"]
    n_CLIENT --> n_Upload_Svc
    n_Upload_Svc --> n_Validator
    n_Upload_Svc --> n_S3
```

#### 1 USER 2-3 SEC RUKA, SERVER KA THREAD BHI ATKA
```
DIKKAT 1   Arpan ne report.pdf daali. Server ne Validator ko call kiya, 3 second jawab ka intezaar.
           Arpan 3 second ghoomta screen dekhta raha, aur server ka ek thread bhi 3 second bekaar atka.
           Hazaar log ek saath -> saare thread atke -> naye upload ke liye thread hi nahi.
HAL        Arpan ko rukwao mat. Upload hote hi tracking id (T-101) + "jaanch ho rahi hai" de do.
           Upload Svc queue me khabar daale {T-101, file kahan hai} aur turant laut aaye.
           Worker queue se uthaye -> Validator ko call -> DB me T-101 = DONE / FAILED.
           Worker "padh liya" (offset commit) DB me likhne ke BAAD bole -> beech me mara to khabar
           dobara milegi, khoyegi nahi. Arpan jab chahe T-101 se haal dekh le.
```
```mermaid
flowchart TD
    n_CLIENT["CLIENT"]
    n_Upload_Svc["Upload Svc"]
    n_DB["DB"]
    n_S3["S3"]
    n_Kafka["Kafka"]
    n_Worker["Worker"]
    n_Validator["Validator"]
    n_CLIENT --> n_Upload_Svc
    n_Upload_Svc --> n_DB
    n_Upload_Svc --> n_S3
    n_Upload_Svc --> n_Kafka
    n_Kafka --> n_Worker
    n_Worker --> n_Validator
    n_Worker --> n_DB
```
```
DIKKAT 2   Khabar dobara mili, file do baar jaanchi gayi?
HAL        Pehle dekho: T-101 pehle se DONE hai? -> skip (idempotent).

DIKKAT 3   Kafka hi kyun?
HAL        1-2 upload / second ke liye SQS / RabbitMQ bhi kaafi (simple, AWS chalaye).
           Kafka tab jab bahut khabar ho ya dobara padhni ho.

DIKKAT 4   Arpan har 2 sec haal poochhe, bahut nahi?
HAL        Dheere-dheere gap badhao (2s, 4s, 8s), ya server khud bataye (SSE / WebSocket).

DIKKAT 5   Raat 2 baje Validator 1 ghanta band, 4,000 file VALIDATING pe atki. Kya hoga?
HAL        Kuch nahi khoyega: khabar Kafka me padi, worker DONE likhne ke baad hi "padh liya" bolta.
           Validator wapas aaya to line nikalti. Beech me: FUSE (3-5 fail pe call band, thodi der baad ek
           test call) + DOOSRI COMPANY (bahar wale ki copy hum nahi chala sakte).

DIKKAT 6   Ye hua, hamein pata kaise chalega?
HAL        Health check nahi pakadta (wo "zinda" bolta). Apne number dekho: queue ki lambai · Validator
           ka time + error rate · 10 min se VALIDATING me atki files -> had paar = alert, user se pehle.
```

#### 2 5 GB FILE KE BYTES HAMARE SERVER SE GUZAR RAHE
```
DIKKAT 1   Arpan ki 5 GB video: client -> hamara server -> S3. 5 GB do baar network pe chala, server ka
           thread minuton tak atka, aur sirf bytes aage badhane ke liye server badhane pade.
HAL        Server sirf ek 10 minute ka paas (signed URL) + T-101 deta. Bytes seedha S3 me.
             1  Arpan -> server: "5 GB upload karni hai"
             2  server -> Arpan: signed URL (10 min ka) + T-101
             3  Arpan -> S3: bytes seedhe (server beech me nahi)
             4  Arpan -> server: "/complete T-101" -> server queue me daale
           Sign kaise: server apni AWS secret se (bucket + file ka naam + 10 min) ka hash banata.
           S3 ke paas bhi wahi secret, wo khud hash bana ke milata. Mila -> andar aane do.
           Arpan ke haath AWS ki chaabi kabhi nahi aati, sirf EK file ka 10 min ka paas.
```
```mermaid
flowchart TD
    n_CLIENT["CLIENT"]
    n_S3["S3"]
    n_Upload_Svc["Upload Svc"]
    n_DB["DB"]
    n_Kafka["Kafka"]
    n_Worker["Worker"]
    n_Validator["Validator"]
    n_CLIENT --> n_S3
    n_CLIENT --> n_Upload_Svc
    n_Upload_Svc --> n_DB
    n_Upload_Svc --> n_Kafka
    n_Kafka --> n_Worker
    n_Worker --> n_Validator
    n_Worker --> n_DB
```
```
DIKKAT 2   Limit 5 GB thi, Arpan ne 50 GB daal di?
HAL        Presigned POST do: usme size ki had (content-length-range) likh sakte, S3 badi file khud reject.
           PUT wale paas me had nahi lagti -> tab S3 event pe size dekho, had se badi ho to delete.

DIKKAT 3   Arpan ne "/complete" bola hi nahi (app band)?
HAL        S3 khud batata "file aa gayi" (S3 event notification), ya sweeper job (adhoori file dhoondhne
           wali) pakde.
```

#### 3 5 GB FILE 90% PE TOOT GAYI
```
DIKKAT 1   Arpan ki 4.5 GB chadh chuki thi, wifi chala gaya. Ab poori 5 GB shuru se? Arpan pagal ho jaayega.
HAL        File ko tukdon me bhejo (multipart):
             1  "shuru" -> S3 ek uploadId deta (U-9)
             2  har tukda (1..N) apne signed URL pe, kai ek saath -> S3 har tukde ki rasid (ETag) deta
             3  "complete(U-9, [tukda + rasid])" -> S3 kram se jod ke ek file bana deta
           4.5 GB pe wifi gaya -> 90 tukde pahunch chuke, sirf bache 10 bhejo.
```
```mermaid
flowchart TD
    n_CLIENT["CLIENT"]
    n_S3["S3"]
    n_Upload_Svc["Upload Svc"]
    n_DB["DB"]
    n_Kafka["Kafka"]
    n_Worker["Worker"]
    n_Validator["Validator"]
    n_CLIENT --> n_S3
    n_CLIENT --> n_Upload_Svc
    n_Upload_Svc --> n_DB
    n_Upload_Svc --> n_Kafka
    n_Kafka --> n_Worker
    n_Worker --> n_Validator
    n_Worker --> n_DB
```
```
DIKKAT 2   Arpan ne app band kar di, kal wapas aaya?
HAL        Server ke paas U-9 hai. S3 se poochho kaunse tukde aa chuke (ListParts) -> sirf bache bhejo.

DIKKAT 3   Tukda kitna bada?
HAL        S3 me kam se kam 5 MB (aakhri chhod ke), zyada se zyada 10,000 tukde -> badi file = bada tukda.
```

#### 4 UPLOAD SHURU HUA, "COMPLETE" KABHI NAHI AAYA (ADHOORI FILE)
```
DIKKAT 1   Arpan ne upload shuru kiya, phir app crash. "complete" kabhi nahi aaya.
           Bytes S3 me pade (paisa lag raha), DB me T-101 "UPLOADING" pe atka, koi use dhoondhta hi nahi.
HAL        Haal likha hai: UPLOADING -> VALIDATING -> DONE / FAILED. Ek SWEEPER job har kuch minute:
             haal UPLOADING / VALIDATING aur 10 min se nahi badla (jaanch to 3 sec ki hai)
             -> S3 me bytes hain? haan = queue me dobara daalo · nahi = FAILED.
           S3 ka lifecycle niyam purani adhoori uploads (bytes) khud hata deta. Par wo din me ek baar
           chalta aur DB nahi dekhta -> isliye sweeper bhi chahiye.
```
```mermaid
flowchart TD
    n_CLIENT["CLIENT"]
    n_S3["S3"]
    n_Upload_Svc["Upload Svc"]
    n_DB["DB"]
    n_Sweeper_job["Sweeper job"]
    n_Kafka["Kafka"]
    n_Worker["Worker"]
    n_Validator["Validator"]
    n_CLIENT --> n_S3
    n_CLIENT --> n_Upload_Svc
    n_Upload_Svc --> n_DB
    n_Upload_Svc --> n_Kafka
    n_Sweeper_job --> n_DB
    n_Kafka --> n_Worker
    n_Worker --> n_Validator
    n_Worker --> n_DB
    n_Sweeper_job --> n_Kafka
```
```
DIKKAT 2   Sweeper do machine pe chala, file do baar queue me?
HAL        Ek hi sweeper chale (lock / leader), ya dobara daalna safe ho (worker haal dekh ke skip).

DIKKAT 3   Jaanch me fail file ka kya?
HAL        Quarantine bucket (koi download nahi kar sakta), kuch din baad delete / security team dekhe.
```

#### 5 SAB HAR 2 SEC HAAL POOCHH RAHE
```
DIKKAT 1   10,000 log upload karke baithe, har ek har 2 sec "T-101 ka haal?" -> 5,000 sawaal / second DB pe.
           Likhai to 1-2 / second thi, padhai ne DB ko ghera.
HAL        Haal Redis se do, DB tak mat jaao:
             GET /status/T-101 -> Redis me hai? -> wahin se
             nahi -> DB ki copy (replica) se padho -> Redis me rakh do (chhoti umar)
           Worker haal badle (DONE) to DB ke saath Redis me bhi usi waqt naya haal likhe.
```
```mermaid
flowchart TD
    n_CLIENT["CLIENT"]
    n_S3["S3"]
    n_Upload_Svc["Upload Svc"]
    n_Redis["Redis"]
    n_DB["DB"]
    n_Read_replica["Read replica"]
    n_Sweeper_job["Sweeper job"]
    n_Kafka["Kafka"]
    n_Worker["Worker"]
    n_Validator["Validator"]
    n_CLIENT --> n_S3
    n_CLIENT --> n_Upload_Svc
    n_Upload_Svc --> n_Redis
    n_Upload_Svc --> n_DB
    n_Upload_Svc --> n_Kafka
    n_DB --> n_Read_replica
    n_Sweeper_job --> n_DB
    n_Kafka --> n_Worker
    n_Worker --> n_Validator
    n_Worker --> n_DB
    n_Worker --> n_Redis
    n_Upload_Svc --> n_Read_replica
    n_Sweeper_job --> n_Kafka
```
```
DIKKAT 2   Worker ne DB me DONE kiya, Redis me abhi bhi VALIDATING -> Arpan ko purana haal?
HAL        Isliye worker DB ke saath Redis bhi update karta. Bhool gaya to bhi chhoti umar ke baad naya.

DIKKAT 3   Replica thodi peeche thi, purana VALIDATING padh ke Redis me baith gaya?
HAL        Worker ka likha naya haal Redis me upar likh deta. Pakka chahiye to miss pe haal primary se
           padho (ek chhoti query, kam baar).

DIKKAT 4   Poochhna hi band karwa do?
HAL        SSE / WebSocket: haal badla to server khud Arpan ko bheje. Baar-baar poochhna khatam.

DIKKAT 5   Redis gira?
HAL        Sawaal seedha replica pe, dheema par chalta. Poochhne pe rate limit.
```

#### 6 FILE NAHI, POORA FOLDER
```
DIKKAT 1   Arpan ne 1000 file ka folder daala. Ek trackingId se 1000 file ka haal kaise?
HAL        Folder ka ek bada id (P-1), andar har file ka apna chhota id (child).
           Saare child DONE = P-1 DONE · koi FAILED = P-1 PARTIAL / FAILED.
           Bahut saari chhoti files -> client zip bana ke bheje, server khol le.
```
```mermaid
flowchart TD
    n_CLIENT["CLIENT"]
    n_S3["S3"]
    n_Upload_Svc["Upload Svc"]
    n_Redis["Redis"]
    n_DB["DB"]
    n_Read_replica["Read replica"]
    n_Sweeper_job["Sweeper job"]
    n_Kafka["Kafka"]
    n_Worker["Worker"]
    n_Validator["Validator"]
    n_CLIENT --> n_S3
    n_CLIENT --> n_Upload_Svc
    n_Upload_Svc --> n_Redis
    n_Upload_Svc --> n_DB
    n_Upload_Svc --> n_Kafka
    n_DB --> n_Read_replica
    n_Sweeper_job --> n_DB
    n_Kafka --> n_Worker
    n_Worker --> n_Validator
    n_Worker --> n_DB
    n_Worker --> n_Redis
    n_Upload_Svc --> n_Read_replica
    n_Sweeper_job --> n_Kafka
```
```
DIKKAT 2   Parent ka haal kab badle?
HAL        Har child DONE / FAILED pe P-1 ka done_count badhao. Sab ho gaye -> P-1 final.

DIKKAT 3   1000 file = 1000 baar "/init"?
HAL        Nahi. Ek hi call me 1000 signed URL (batch init).
```

#### 7 RAVI NE ARPAN KA trackingId DAAL KE FILE MAANG LI
```
DIKKAT 1   Ravi ne kahin se Arpan ka T-101 dekh liya, GET /download/T-101 maara
           -> Arpan ki file Ravi ko mil gayi.
HAL        "Tum kaun ho?" (AUTHN) = gateway / JWT batata: ye Ravi hai.
           "Is file pe haq hai?" (AUTHZ) = SERVICE me check: T-101 ka owner Arpan, maangne wala Ravi -> 403.
           Har haal / download pe ye check. Gateway ye nahi kar sakta, use pata hi nahi T-101 kiski hai.
```
```mermaid
flowchart TD
    n_CLIENT["CLIENT"]
    n_S3["S3"]
    n_API_Gateway_LB["API Gateway / LB"]
    n_Upload_Svc["Upload Svc"]
    n_Redis["Redis"]
    n_DB["DB"]
    n_Read_replica["Read replica"]
    n_Sweeper_job["Sweeper job"]
    n_Kafka["Kafka"]
    n_Worker["Worker"]
    n_Validator["Validator"]
    n_CLIENT --> n_S3
    n_CLIENT --> n_API_Gateway_LB
    n_API_Gateway_LB --> n_Upload_Svc
    n_Upload_Svc --> n_Redis
    n_Upload_Svc --> n_DB
    n_Upload_Svc --> n_Kafka
    n_DB --> n_Read_replica
    n_Sweeper_job --> n_DB
    n_Kafka --> n_Worker
    n_Worker --> n_Validator
    n_Worker --> n_DB
    n_Worker --> n_Redis
    n_Upload_Svc --> n_Read_replica
    n_Sweeper_job --> n_Kafka
```
```
DIKKAT 2   Id random hai, phir bhi owner check kyun?
HAL        Id leak ho sakti (log me, link share). Asli rok owner check hai, randomness nahi.

DIKKAT 3   Admin ko sab dekhna?
HAL        Owner YA admin role, warna 403.

DIKKAT 4   PR review: request me kisi ki id aayi -> owner check kahan hai?
HAL        Har handler ki pehli line: file.owner_id == jwt.user_id ya admin, warna 403.
           userId JWT se lo, request body se kabhi nahi.
```

#### 8 ARPAN NE S3 KA SEEDHA LINK WHATSAPP PE BHEJ DIYA
```
DIKKAT 1   Arpan ko S3 ka pakka link diya. Usne dost ko WhatsApp pe bheja, dost ne group me.
           Link hamesha chalta -> file kisi ke bhi haath.
HAL        DB me file ka naam (S3 key) rakho, pakka link nahi.
           Download maanga -> pehle maalik check -> phir 5 minute ka naya paas (signed GET). Har baar naya.
           Group me 1 ghante baad kisi ne khola -> paas expire -> nahi khula.
```
```mermaid
flowchart TD
    n_CLIENT["CLIENT"]
    n_S3["S3"]
    n_API_Gateway_LB["API Gateway / LB"]
    n_Upload_Svc["Upload Svc"]
    n_Redis["Redis"]
    n_DB["DB"]
    n_Read_replica["Read replica"]
    n_Sweeper_job["Sweeper job"]
    n_Kafka["Kafka"]
    n_Worker["Worker"]
    n_Validator["Validator"]
    n_CLIENT --> n_S3
    n_CLIENT --> n_API_Gateway_LB
    n_API_Gateway_LB --> n_Upload_Svc
    n_Upload_Svc --> n_Redis
    n_Upload_Svc --> n_DB
    n_Upload_Svc --> n_Kafka
    n_DB --> n_Read_replica
    n_Sweeper_job --> n_DB
    n_Kafka --> n_Worker
    n_Worker --> n_Validator
    n_Worker --> n_DB
    n_Worker --> n_Redis
    n_Upload_Svc --> n_Read_replica
    n_Sweeper_job --> n_Kafka
```
```
DIKKAT 2   5 min me download poora nahi hua (badi file)?
HAL        Umar sirf SHURU karne ki. Chalu download beech me nahi katta.

DIKKAT 3   5 min ke andar aage bhej diya?
HAL        5 min ka risk maana. Zyada sensitive -> CloudFront signed URL + IP restriction.
```

#### 9 NAAM REPORT.PDF, ANDAR EXE
```
DIKKAT 1   Kisi ne virus.exe ka naam badal ke report.pdf kiya aur type bhi "application/pdf" bheja.
           S3 me pahunch bhi gayi. Naam aur type dono client ne bheje the, jo jhooth bol sakta hai.
HAL        Client ka bheja naam / type kabhi sach mat maano. File khud batati wo kya hai:
             har type shuru me apni pehchaan likhti: PDF = "%PDF-" · PNG = "\x89PNG" · EXE = "MZ"
           Worker S3 se sirf pehle kuch byte mangata (range GET, poori 5 GB nahi).
           Naam .pdf, pehle byte "MZ" -> ye EXE hai -> reject.
```
```mermaid
flowchart TD
    n_CLIENT["CLIENT"]
    n_S3["S3"]
    n_API_Gateway_LB["API Gateway / LB"]
    n_Upload_Svc["Upload Svc"]
    n_Redis["Redis"]
    n_DB["DB"]
    n_Read_replica["Read replica"]
    n_Sweeper_job["Sweeper job"]
    n_Kafka["Kafka"]
    n_Worker["Worker<br/>+ magic bytes check"]
    n_Validator["Validator"]
    n_CLIENT --> n_S3
    n_CLIENT --> n_API_Gateway_LB
    n_API_Gateway_LB --> n_Upload_Svc
    n_Upload_Svc --> n_Redis
    n_Upload_Svc --> n_DB
    n_Upload_Svc --> n_Kafka
    n_DB --> n_Read_replica
    n_Sweeper_job --> n_DB
    n_Kafka --> n_Worker
    n_Worker --> n_Validator
    n_Worker --> n_DB
    n_Worker --> n_Redis
    n_Upload_Svc --> n_Read_replica
    n_Sweeper_job --> n_Kafka
    n_Worker --> n_S3
```
```
DIKKAT 2   Shuru ke byte PDF jaise, par andar virus / JS?
HAL        Antivirus scan (ClamAV) worker me, ya S3 malware scanning.

DIKKAT 3   Koi bheed / hamla?
HAL        Har user pe rate limit, sabse aage WAF, HTTPS, aur secrets vault me.
```

```
AAKHRI DABBE (sab dikkat ke baad):
  User -> Gateway / LB (login) -> Upload Svc (file ki jaankari + signed URL, bytes ko haath nahi)
  -> client SEEDHA S3 pe (tmp/, badi file multipart) -> complete -> Kafka -> Worker (jaanch, magic bytes)
  -> DB me haal -> Redis + read replica (haal poochhna) · download = owner check + chhota signed GET
  Sweeper + S3 lifecycle = atki / adhoori file

DB:  PostgreSQL -> trackingId se seedha dhoondhna simple, par haal
     (UPLOADING -> VALIDATING -> DONE / FAILED) ko ACID chahiye
     files: trackingId (KEY) | ownerId | status | s3_key

END ME POOCHEGA:
  "Bytes server se kyun nahi?" -> signed URL: server pe bojh nahi, seedha S3
  "10x?"            -> Upload Svc machine + LB · S3 khud scale · download CDN · haal cache + replica
                       · DB shard (trackingId) · worker queue ki lambai dekh ke badhao
  "SPOF?"           -> Validator (bahar wala): fuse (circuit breaker) + doosri company
  "Chal raha, kaise pata?" -> apne metric: queue ki lambai · Validator p99 + error rate
                       · VALIDATING me atki files (health check sirf "zinda hai" batata, "dheema" nahi)
```

AAKHRI DABBE ki tasveer:

```mermaid
flowchart TD
    n_CLIENT["CLIENT"]
    n_S3["S3"]
    n_API_Gateway_LB["API Gateway / LB"]
    n_Upload_Svc["Upload Svc"]
    n_Redis["Redis"]
    n_DB["DB"]
    n_Read_replica["Read replica"]
    n_Sweeper_job["Sweeper job"]
    n_Kafka["Kafka"]
    n_Worker["Worker"]
    n_Validator["Validator"]
    n_CLIENT --> n_S3
    n_CLIENT --> n_API_Gateway_LB
    n_API_Gateway_LB --> n_Upload_Svc
    n_Upload_Svc --> n_Redis
    n_Upload_Svc --> n_DB
    n_Upload_Svc --> n_Kafka
    n_DB --> n_Read_replica
    n_Sweeper_job --> n_DB
    n_Kafka --> n_Worker
    n_Worker --> n_Validator
    n_Worker --> n_DB
    n_Worker --> n_Redis
    n_Upload_Svc --> n_Read_replica
    n_Sweeper_job --> n_Kafka
    n_Worker --> n_S3
```

---

## 10 GUNA TRAFFIC — har dabba alag se

```
Upload Svc   -> stateless, kai machine + LB
S3           -> AWS khud scale karta · zyada download wali file -> CDN (CloudFront)
DB           -> haal poochhna -> CACHE + READ REPLICA · bahut files -> SHARD (trackingId)
Redis        -> purana haal na dikhe -> worker saath me update kare
Kafka        -> jaanch ki line lambi -> WORKER badhao (line ki lambai dekh ke)
adhoori file -> S3 lifecycle + sweeper

POOCHEGA: "How would you scale this to 10x?"      -> file ka raasta chalo, pehle jo toote
POOCHEGA: "What's the single point of failure?"   -> Validator (bahar wala): fuse + doosri company
POOCHEGA: "How do you know it's working?"
          (Chal raha hai, kaise pata?)
          MISAAL: raat ko Validator DHEEMA, line me ~5,000 file atki
          health check "dheema" nahi pakadta (ping ka jawab aa jaata) -> APNA METRIC pakadta:
             line ki lambai · Validator p99 + error rate · VALIDATING me purani files ki ginti
             -> had paar = alert (CloudWatch alarm / PagerDuty), user ke shikayat karne se pehle
          peeche: 3 fail -> fuse (CIRCUIT BREAKER) -> doosri company (bahar wale ki copy hum nahi chala sakte)
JAWAB:    Health check sirf batata 'zinda hai', 'dheema hai' nahi batata. Isliye apne teen number dekho:
          line kitni lambi, jaanch wali company kitni der me jawab de rahi aur kitni baar fail, aur
          kitni files 10 minute se 'jaanch ho rahi' pe atki.
          Had paar = alert, user ke shikayat karne se pehle. Company baar-baar fail ho to us pe call
          band (fuse) aur doosri company.
```

---

## POOCHE TO (gehra sawaal aaye tab)

```
API:      POST /upload/init { fileName, size }  -> signed S3 URL + trackingId (server sirf URL deta)
          -> client SEEDHA S3 pe PUT
          POST /upload/complete { trackingId }   -> "ho gaya, jaancho" (queue)
          GET  /status/{trackingId}              -> VALIDATING / DONE / FAILED
          GET  /download/{trackingId}            -> signed GET URL -> seedha S3

DB:       files: trackingId (KEY) | fileName | ownerId | status | s3_key | createdAt | updatedAt
          haal: UPLOADING -> VALIDATING -> DONE / FAILED ("kahan tak pahunchi" = gadbad sambhalne ki buniyaad)
          PostgreSQL: dhoondhna simple par haal ko ACID chahiye
          (bahut bada + sirf key-value -> NoSQL bhi chalta · paisa hota -> hamesha SQL)

DEDUP:    file ka hash (MD5 / SHA) -> pehle se hai to dobara store nahi -> jagah bachi
```

---

## AAKHRI DABBA + WRAP

```
Gateway / LB = login · Upload Svc = jaankari + signed URL, bytes ko haath nahi · S3 = bytes, tmp/ + lifecycle
DB = trackingId + haal + owner · Kafka + Worker = dheemi jaanch alag, magic bytes
Redis + replica = haal poochhna · Sweeper = atki file
```
```
JAWAB:    Server sirf paas deta, file seedha S3 me (badi ho to tukdon me).
          Jaanch 3 second ki, to Arpan ko turant T-101, worker line se jaanchta, file ke pehle byte
          dekhta, haal likhta.
          Haal baar-baar poochha jaata, to Redis + copy, haal badle to Redis bhi. Maalik check aur
          chhote paas se safe, S3 niyam + atki file wali job se safai.
          Aage: fail pe dobara koshish, download ke liye CDN.
```
```mermaid
flowchart TD
    n_CLIENT["CLIENT"]
    n_S3["S3"]
    n_API_Gateway_LB["API Gateway / LB"]
    n_Upload_Svc["Upload Svc"]
    n_Redis["Redis"]
    n_DB["DB"]
    n_Read_replica["Read replica"]
    n_Sweeper_job["Sweeper job"]
    n_Kafka["Kafka"]
    n_Worker["Worker"]
    n_Validator["Validator"]
    n_CLIENT --> n_S3
    n_CLIENT --> n_API_Gateway_LB
    n_API_Gateway_LB --> n_Upload_Svc
    n_Upload_Svc --> n_Redis
    n_Upload_Svc --> n_DB
    n_Upload_Svc --> n_Kafka
    n_DB --> n_Read_replica
    n_Sweeper_job --> n_DB
    n_Kafka --> n_Worker
    n_Worker --> n_Validator
    n_Worker --> n_DB
    n_Worker --> n_Redis
    n_Upload_Svc --> n_Read_replica
    n_Sweeper_job --> n_Kafka
    n_Worker --> n_S3
```

ARCHETYPE B+F · CONCEPTS: [message-queues](../../FOUNDATIONS/07_message_queues.md) · [reliability/SPOF](../../FOUNDATIONS/11_reliability_spof_cloud.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
