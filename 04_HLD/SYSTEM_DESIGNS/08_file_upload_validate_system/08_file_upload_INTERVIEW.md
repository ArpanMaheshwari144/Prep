# File Upload + Validate + Track — POORA ROUND (4 MOVE)

> **NAV** — ARCHETYPE B+F · DIL: upload -> validate -> track. UP: [MASTER](../../00_MASTER_SHEET.md) · CONCEPTS: [message-queues](../../FOUNDATIONS/07_message_queues.md) · [reliability/SPOF](../../FOUNDATIONS/11_reliability_spof_cloud.md)

> ★ JP ne ye ACTUALLY poocha tha (asli SDE-3 writeup): file/folder lo -> third-party se validate karao
> (2-3 second lagta hai) -> store karo -> user ko tracking link do. General product design hai, finance nahi.
> 15-Sep: asli mock-video ke hisaab se dobara likha — koi 7-step rail nahi, sirf 4 move.
> Har jagah: **tu kya BOLTA hai · BOARD pe kya banta · FAISLA + KYUN**.

```
★★ TEEN NIYAM (poori file par lagte — [APPROACH_DELIVERY](../../01_DELIVERY.md) 5b)
   1. PERFECT design ek saath mat banao — chhote se shuru, dikkat pe badhao
   2. NUMBER ke peeche mat bhaago — bolo, ek faisla nikaalo, aage badho
   3. BOTTLENECK ratto mat — KHUD USER banke raasta chalao, khud dikh jaayega
```

---

# MOVE 1 — POOCHO (board pe abhi kuch nahi)

```
   TU: "Kuch cheezein confirm kar lun pehle —
          - file ka SIZE limit kya hai? (10 MB ya 5 GB — design bilkul badal jaata hai)
          - kaunse file TYPE allow hain?
          - validation me kitna time lagta hai? (aapne 2-3 second kaha)
          - sirf FILE, ya FOLDER bhi upload hoga?
          - user ko result turant chahiye, ya baad me status dekh lega?"

   ★ pehla aur aakhri sawaal sabse zaroori hain:
        bada file    -> presigned URL + multipart ka raasta
        turant nahi  -> async + tracking ka raasta
```

---

# MOVE 2 — DO CHHOTE BLOCK LIKHO

```
   ┌──────────────────────┐    ┌────────────────────────────────────┐
   │ File Upload System   │    │ Use cases:                         │
   │   - File (+ metadata)│    │   - file upload karo               │
   │   - Upload / Tracking│    │   - third-party se VALIDATE karao  │
   │   - Validation job   │    │   - store karo                     │
   │   - Status           │    │   - tracking link / status do      │
   │   - Owner (user)     │    │                                    │
   └──────────────────────┘    │ NOT in scope: editing, sharing,    │
                               │   versioning                       │
   ┌──────────────────────────┐└────────────────────────────────────┘
   │ Kya chahiye (NFR):       │
   │  - user WAIT na kare     │ <- DIL
   │  - data LOST na ho       │
   │  - status HAMESHA pata   │
   │    ho (kahan tak pahuncha│
   │  - scale + secure        │
   └──────────────────────────┘

   TU: "Sabse badi baat: validation me 2-3 second lagte hain. Main user ko utni der
        rukne nahi dunga — usko turant ek tracking id de dunga aur kaam peeche chalta rahega."
```

```
   Numbers:
     - maan lo 1 lakh file / day    ->  ~1-2 writes / sec   (zyada nahi)
     - PAR log status BAAR-BAAR check karte hain -> reads bahut zyada -> READ-HEAVY

     TRICK: 1 din ~ 1,00,000 sec  ->  per-day / 1,00,000 = per-sec

   HAR NUMBER SE EK FAISLA:
     reads >> writes    ──►  status ke liye CACHE + read replica
     bada file          ──►  bytes DB me nahi — BLOB (S3) me
     validation slow    ──►  QUEUE + worker (user ko block mat karo)
```

---

# MOVE 3 — BOXES BANAO (chhota banao, phir dikkat pe badhao)

```
   TU: "Sabse simple se shuru."

        CLIENT ──► [ App Server ] ──► [ S3 ]
                        │
                        └──► third-party validate (2-3 sec) ──► jawab wapas

   TU: "Kaam ho raha hai. Ab isme bada file, slow validation aur bheed daal ke dekhte hain."
```

### dikkat 1 — "user 2-3 second tak ruka hua hai"

```
        upload ──► validate (2-3 sec) ──► jawab
                        │
                   user tab tak baitha hai, aur server ka thread bhi block hai

   TEEN OPTION (teeno bolna, phir chunna):

     1. SYNCHRONOUS          -> user 2-3 sec ruke, server block  => NAHI
     2. ★ ASYNC + POLLING    -> upload pe TURANT trackingId + status "VALIDATING"
                                validation background me (queue + worker)
                                user GET /status se poll kar le                 => WINNER
     3. ASYNC + WEBHOOK/push -> kaam khatam hone pe user ko notify
                                (UX behtar, par setup zyada)

   FAISLA:
        upload ──► turant trackingId + "VALIDATING" ──► [ QUEUE ] ──► [ WORKER ] ──► validate
                                                                          │
                                                          status update (DONE / FAILED)
                                                                          │
                                                                  user dekhe / notify

   TU: "User kabhi block nahi hota, aur validation ka load queue se smooth ho jaata hai."
```

### dikkat 2 — "file ke bytes mere app server se guzar rahe hain"

```
        CLIENT ──5 GB──► [ App Server ] ──5 GB──► [ S3 ]
                              │
                    bandwidth DOUBLE lagi, server ka thread block,
                    aur app server ko scale karna pada — jabki usne kuch kiya hi nahi

        => ye ANTI-PATTERN hai

   FAISLA: ★ PRESIGNED URL — client SEEDHA S3 pe daale

        1. client ──► server : "ye file upload karni hai"
        2. server ──► client : short-lived SIGNED URL + trackingId
        3. client ──► S3     : bytes SEEDHE (server beech me hai hi nahi)
        4. client ──► server : "ho gaya" (/upload/complete)

   TU: "Server sirf metadata aur URL deta hai — bytes ko haath hi nahi lagata.
        Bandwidth aadhi, server free, aur S3 apne aap scale karta hai."

   ★ yahi cheez DOWNLOAD me bhi: presigned GET URL -> client seedha S3 se le
```

### dikkat 3 — "5 GB file 90% pe toot gayi"

```
        poori file dobara? -> user maar dega

   FAISLA: MULTIPART / RESUMABLE upload

        file ──► chunks me todo (e.g. 5 MB ke tukde)
                  ├─ chunk 1 ✓
                  ├─ chunk 2 ✓
                  ├─ chunk 3 ✗ fail   ──► sirf YE chunk retry
                  └─ ...
        sab chunk ho gaye ──► S3 ko bolo "complete multipart" ──► wo jod deta hai

   => fail hua to sirf wahi tukda dobara, poori file nahi = resumable
```

### dikkat 4 — "user ne init to liya, par complete kabhi nahi kiya"

```
        bytes S3 me padi reh gayi, DB me status adhoora
        = ORPHAN file (paisa lagta hai, gandagi badhti hai)

   FAISLA: S3 LIFECYCLE RULE
        upload pehle tmp/ prefix me -> VALIDATE hone pe asli jagah copy
        lifecycle: "tmp/ me 1 DIN se purana -> auto DELETE"
        adhoore multipart uploads ke liye bhi "abort incomplete multipart" rule (ye bhi DIN me)
        ★ lifecycle rule GHANTON me nahi, DINO me chalta (kam se kam 1 din), aur DB ka status
          nahi dekh sakta. Jaldi saaf karna ho to DB status dekh ke chalne wala sweeper job.

   + validation FAILED wali file -> delete, ya "quarantine" bucket me daal do
     aur DB me status = FAILED

   ★ ye edge case interviewer zaroor kuredega — khud bol dena achha lagta hai
```

### dikkat 5 — "user har 2 second status poll kar raha hai (aur sab kar rahe hain)"

```
        read-heavy ban gaya -> har poll DB pe

   FAISLA: CACHE (Redis) + READ REPLICA

   ★★ PAR EK JAAL HAI — CACHE STALENESS:
        worker ne status VALIDATING -> DONE kar diya
        par cache me abhi bhi purana "VALIDATING" pada hai
        -> user ko galat status dikhta rahega

        FIX: worker jab status update kare, TABHI cache bhi
             write-through kare ya invalidate kare
             (ya bahut chhota TTL rakho)

   TU: "Status wala cache normal cache se alag hai — yahan galat purana dikhna
        seedha user ko dikhega, isliye invalidation update ke saath hi hona chahiye."
```

### dikkat 6 — "prompt me file YA folder tha — folder ka kya?"

```
   FOLDER = N files

        PARENT trackingId
            ├── child trackingId  (file 1)   apna /upload/init
            ├── child trackingId  (file 2)
            └── child trackingId  (file 3)

        parent ka status = ROLLUP
            saare child DONE  -> parent DONE
            koi FAILED        -> parent PARTIAL / FAILED

   ★ bahut saari chhoti files ho -> client zip kar de -> ek upload -> server unzip kare
```

### dikkat 7 — "kisi ne DUSRE ka trackingId daal ke file maang li"

```
        trackingId ek anuman-layak string hai,
        aur usme "ye kiski file hai" likha hi NAHI

   FAISLA: /upload/init pe user authenticated hai (JWT)
           -> us waqt record me ownerId LIKH do
           -> /status aur /download pe check: maangne wala == owner

   ★ AUTHENTICATION vs AUTHORIZATION -- do ALAG cheezein hain:
        AUTHN  "tum kaun ho"                -> filter / JWT me, EK jagah, dikhta bhi nahi
        AUTHZ  "IS FILE pe tumhara haq hai?" -> filter ye kar hi NAHI sakta,
                                                usne trackingId dekha hi nahi

   ★ yahi wo shakal hai jo PR-REVIEW checklist me sabse upar hai:
     "request me kisi ki CHEEZ ka naam aaye -> maalik ka check kahan hai?"
```

### dikkat 8 — "owner ko S3 ka SEEDHA link diya, usne wo link aage bhej diya"

```
        link hamesha ke liye chal raha hai  ->  ab kisi ko bhi chalega
        aur humara owner-check beech me aata hi nahi -- S3 seedha de raha hai

   FAISLA: PRE-SIGNED URL, chhoti umar ka (5-15 minute)

   ★ owner-check AB BHI hum karte hain -- URL tabhi BANTA hai jab check paas ho
     URL "chaabi" nahi hai, "5 minute ka PAAS" hai
```

### dikkat 9 — "naam .pdf tha, Content-Type bhi application/pdf tha, par file asal me exe thi — aur wo S3 me pad chuki hai"

```
        dono cheezein CLIENT ne bheji hain  ->  dono pe bharosa nahi
        aur bytes PEHLE HI S3 me hain (direct-to-S3 upload ka natija, dikkat 2)
        -> "system apne aap reject kar dega" yahan hota hi nahi,
           file pehle GIRTI hai, pakdi BAAD me jaati hai

   FAISLA: worker file ke PEHLE BYTE padhe (magic number) aur type KHUD tay kare
              PDF  %PDF-        PNG  \x89PNG        EXE  MZ
           mel nahi khaya  ->  status REJECTED + file delete

   ★ client ka bheja hua data KABHI sach nahi maana jaata -- naam bhi nahi,
     Content-Type bhi nahi
```

### ab poora naksha (jahan pahunche) + har box ka KYUN

```
                      CLIENT
                        │  (1) init
                        ▼
        ┌──────────────────────────────┐
        │  API GATEWAY / LB            │
        └──────────────┬───────────────┘
                       ▼
        ┌──────────────────────────────┐        (2) presigned URL + trackingId
        │      UPLOAD SERVICE          │ ─────────────────────────────────────►  CLIENT
        │  metadata + URL banata       │                                            │
        └───────┬──────────────┬───────┘                                            │
                │              │                                       (3) bytes SEEDHE
                ▼              ▼                                                    │
        ┌──────────────┐   ┌──────────────┐                                         ▼
        │  DB (SQL)    │   │ QUEUE(Kafka) │                                     ┌────────┐
        │ trackingId,  │   └──────┬───────┘                                     │   S3   │
        │ status, owner│          ▼                                             └────────┘
        └──────▲───────┘   ┌──────────────┐                                          ▲
               │           │   WORKER     │ ──► third-party VALIDATE (2-3 sec)       │
               └───────────┤ status update│ ──► magic-byte check (type sach me kya hai) │
                 cache     └──────────────┘ ──► fail? quarantine / delete ───────────┘
              invalidate
                  │
           [ CACHE (Redis) ] + READ REPLICA   <- status polling yahan se

     API GW / LB    : traffic + auth
     Upload Service : sirf metadata + presigned URL — bytes ko haath nahi lagata
     S3             : bade bytes ka ghar, khud scalable; download pe aage CDN
     DB (SQL)       : trackingId + status + owner — chhota data, par status ACID chahiye
     QUEUE          : 2-3 sec wali slow validation ko user se alag karti hai
     WORKER         : magic-byte check + validate + status update (+ cache invalidate)
     CACHE          : status read-heavy hai -> DB bachaya
```

---

# MOVE 4 — BOLTE-BOLTE JODO (jo poocha jaaye, wahi kholo)

## ► "API kya hogi?"

```
   POST /upload/init      { fileName, size }   ->  presigned S3 URL + trackingId
                                                   (server SIRF url deta hai)
        -> phir client SEEDHA S3 pe PUT karta hai (bytes app-server se nahi guzarte)

   POST /upload/complete  { trackingId }       ->  "upload ho gaya, ab validate karo"
                                                   (queue me daal do)

   GET  /status/{trackingId}                   ->  VALIDATING / DONE / FAILED
   GET  /download/{trackingId}                 ->  presigned GET URL
                                                   -> client S3 se SEEDHA download kare
```

## ► "DB me kya, aur kaunsa DB?"

```
   files:  trackingId (KEY) | fileName | ownerId | status | s3_url | createdAt

   status =  UPLOADING  ->  VALIDATING  ->  DONE
                                        ->  FAILED

   ★ status ka matlab: "kahan tak pahuncha" — yahi poore failure-handling ki buniyaad hai
     (crash ho jaaye to bhi pata rahega ki kya adhoora tha)

   DB choice: simple key-lookup hai, par status ko ACID chahiye -> PostgreSQL (SQL)
        (agar scale bahut bada + pure key-value hota -> NoSQL bhi chalta;
         paisa hota -> hamesha SQL/ACID)
```

## ► "Bade file ka kya karoge?" (deep-dive ka dil — upar dikkat 2 aur 3)

```
   ★ PRESIGNED URL
        client server se short-lived signed URL maangta hai -> phir SEEDHA S3 pe upload
        faayda: bytes app-server se nahi guzarte -> server free, bandwidth aadhi,
                S3 apne aap scale karta hai
        server sirf metadata + URL deta hai

   ★ MULTIPART / RESUMABLE
        file ko chunks me todo (e.g. 5 MB) -> har chunk alag upload -> S3 jod deta hai
        fail hua -> sirf WO chunk retry (poori file dobara nahi)

   ★ DEDUP (optional, bolne layak)
        file ke content ka hash (MD5/SHA) nikaalo -> pehle se hai to dobara store mat karo
        -> storage bachti hai
```

## ► "Kahan tootega / 10x pe?"

```
   ★ RATTO MAT — file ka raasta chalao:

      client upload karta hai
          │
          ├─► Upload Service -> ek instance kaafi nahi      -> kai instance + LB
          ├─► S3             -> managed hai, khud scale karta
          │                     popular download            -> aage CDN (CloudFront)
          ├─► DB             -> status polling read-heavy    -> CACHE + READ REPLICA
          │                     bahut files                  -> SHARD (trackingId pe)
          │                     cache purana status dikha raha-> write-through / invalidate
          ├─► QUEUE          -> validation ka backlog        -> WORKER auto-scale (queue-depth pe)
          └─► adhoore upload -> ORPHAN bytes                 -> S3 lifecycle rule (auto delete/abort)
```

## ► WRAP (ek saans me)

```
   "Client -> LB -> Upload Service. File ke bytes presigned URL se SEEDHE S3 me jaate hain,
    metadata DB me. Validation 2-3 second leti hai isliye wo async hai — queue + worker —
    aur user ko turant trackingId mil jaata hai jise wo poll kar sakta hai.
    Bade file ke liye multipart/resumable, adhoore upload ke liye S3 lifecycle rule,
    aur status read-heavy hai to cache + read replica — par cache ko worker ke update ke
    saath hi invalidate karna padega.
    Aage badhata to: fail pe retry, aur downloads ke liye CDN."
```

---

[← MASTER SHEET](../../00_MASTER_SHEET.md)
