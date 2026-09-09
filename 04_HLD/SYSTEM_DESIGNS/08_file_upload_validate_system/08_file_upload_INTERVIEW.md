# File Upload + Validate + Track System — INTERVIEW (7-step RAIL)

> JP ne ye ACTUALLY poocha (real SDE-3 writeup): file/folder lo -> third-party se validate (2-3 sec)
> -> store -> user ko tracking-link do. General product design (NOT finance-specific).
> RAIL: 04_HLD/HLD_APPROACH_DELIVERY.md — Requirements → Estimate → API → Data model → HL boxes → Deep-dive → Bottleneck

---

## STEP 1 — REQUIREMENTS clarify (solution pe mat kudo)
```
   FUNCTIONAL:  file upload -> third-party se validate -> store -> tracking-link/status do
   NON-FUNCTIONAL:  fast response (user wait na kare), data lost na ho, scalable, status hamesha pata
   clarifying Qs:  file size limit? file types? validation kitna time? folder bhi ya sirf file?
```

## STEP 2 — ESTIMATE (scale / numbers)
```
   maano 1 lakh files/day -> ~1-2 writes/sec
   log status BAAR-BAAR check karte -> reads zyada -> READ-HEAVY
   TRICK: 1 din ~ 100,000 sec -> per-day / 100,000 = per-sec
   -> ye batata: read-heavy = cache; bada data = shard; spike = queue
```

## STEP 3 — API design
```
   POST /upload/init {fileName,size} -> presigned S3 URL + trackingId  (server sirf URL deta)
   -> client SEEDHA S3 pe PUT kare (bytes app-server se nahi guzarte)
   POST /upload/complete {trackingId} -> "uploaded, ab validate karo" (queue me daalo)
   GET  /status/{trackingId}          -> status poocho: VALIDATING/DONE/FAILED (LAANA -> GET)
```

## STEP 4 — DATA MODEL + DB (KYUN bolo)
```
   files: trackingId(KEY) | fileName | ownerId | status | s3_url | createdAt
   status = UPLOADING / VALIDATING / DONE / FAILED  (status = "kahan tak pahuncha" -- failure-handling base)
   DB: simple key-lookup, status ko ACID chahiye -> PostgreSQL (SQL)
       (massive scale + simple key-value -> NoSQL; money -> hamesha SQL/ACID)
```

## STEP 5 — HL BOXES
```
   Client --presigned URL--> S3 (DIRECT, bytes app-server se nahi jaate)
   Client -> API Gateway -> Upload Service -> [ metadata + presigned URL dena -> DB ]
                                            -> [ validation (2-3 sec) -> QUEUE -> Worker ]
   KEY decision: file bytes (bada) -> client SEEDHA S3 (presigned URL); server = metadata + URL only
                 (bytes app-server se guzaarna = anti-pattern: bandwidth double, server block)
                 metadata (chhota: naam/status/owner) -> DB
                 slow validation -> queue+worker (user wait na kare)
```

## STEP 6 — DEEP DIVE (asli khel): validation 2-3 sec delay kaise handle?
```
   options compare:
   1. SYNCHRONOUS -> user 2-3 sec ruke -> bura UX, server block. NAHI.
   2. ASYNC + POLLING (BEST) -> upload pe turant trackingId + status "VALIDATING" ->
        validation BACKGROUND (queue+worker) -> user GET /status se poll kare
   3. ASYNC + WEBHOOK/push -> done pe user ko notify (behtar UX, zyada setup)
   WINNER: ASYNC (queue + worker)
     flow: upload -> turant trackingId + "VALIDATING" -> queue -> worker validate ->
           status update (DONE/FAILED) -> user dekhe/notify
     -> user kabhi block nahi, scalable

--- PRESIGNED URL + MULTIPART (file-upload ka asli maal) ---
★ PRESIGNED URL (bada file ka sahi tareeka):
   client server se short-lived signed URL maange -> SEEDHA S3 pe upload.
   fayda: bytes app-server se nahi guzarte -> server free, bandwidth aadha, S3 scale khud.
   server sirf metadata + URL deta (bytes ko haath nahi lagata).

★ MULTIPART / RESUMABLE (bade file):
   file ko chunks me todo (e.g. 5MB) -> har chunk alag upload -> S3 jodta (complete-multipart).
   fail hua -> sirf woh chunk retry (poora file dobara nahi) = resumable.

★ DEDUP (optional): file ka content-hash (MD5/SHA) -> already hai to dobara store nahi (storage bache).
```

## STEP 7 — BOTTLENECK / scale
```
   - Upload Service ek -> kai instances + LB
   - DB read load (status) -> READ REPLICAS
   - bahut files -> SHARD (trackingId pe)
   - validation backlog -> WORKERS auto-scale (queue-depth pe)
   - S3 -> already scalable (managed)

   WRAP: Client -> LB -> Upload Service -> file=S3, metadata=DB, validation=async(queue+worker),
         status track. Scale: replicas + shard + worker auto-scale.
         Improve: virus-scan, retry on fail, CDN for downloads.
```

---

## STEP 8 — FOLLOW-UPS (interviewer yahan kuredega — ye PEHLE SE pata hone chahiye)

> Ye 5 cheezein core-design ke baad ke follow-up hain. Live-derive nahi hote — padh ke rakho taaki wahan bol pao.

### 8a. FOLDER upload kaise (prompt me "file YA folder" tha)
```
folder = bunch of files. Do tareeke:
  1. Client folder ko FLATTEN kare -> har file ka apna /upload/init -> apni presigned-URL + child-trackingId
     -> ek PARENT trackingId (folder-level), uske neeche child (per-file). status = sab child DONE -> parent DONE.
  2. (chhoti files) client zip banaye -> ek file jaisa upload -> server unzip -> validate.
-> interview line: "folder = parent-trackingId + N child uploads; parent status = rollup of children."
```

### 8b. ORPHAN / FAILED cleanup (classic follow-up)
```
Problem: user ne /upload/init liya (presigned-URL mila) par /complete kabhi nahi kiya
   -> bytes S3 me pade (ya aadhe) -> ORPHAN. Ya validation FAILED -> file rakhein kya?
Fix:
  - S3 LIFECYCLE RULE: jo object X ghante me /complete na ho -> auto-delete (S3 khud karta).
  - FAILED validation -> file delete ya "quarantine" bucket me move (audit ke liye), DB status=FAILED.
  - multipart adhoora -> S3 "abort incomplete multipart upload" lifecycle-rule (N din baad clean).
-> line: "orphan/incomplete uploads ko S3 lifecycle-rule se auto-clean; failed ko quarantine/delete."
```

### 8c. ★ CACHE STALENESS / invalidation (ye CORE hai — humne cache chuna tha)
```
Humne read-heavy -> status pe CACHE (Redis) rakha. Dikkat: status VALIDATING->DONE badla,
   par cache me abhi bhi "VALIDATING" -> user purana dekhta = STALE.
Fix (koi ek bolo):
  1. WRITE-THROUGH: worker jab DB status update kare, SAATH HI cache bhi update kare -> hamesha fresh.
  2. TTL: cache entry pe chhota TTL (e.g. 5-10 sec) -> thodi der baad khud refresh (eventual, simplest).
  3. INVALIDATE-on-change: status change pe cache key delete -> agli read DB se fresh + re-cache.
-> best yahan: write-through (worker done pe cache update) -> status turant sahi dikhta.
-> line: "cache staleness -> worker status-update pe write-through/invalidate, warna user stale status dekhega."
```

### 8d. SECURITY (presigned URL + auth)
```
- presigned URL = SHORT-LIVED (e.g. 5-15 min expiry) -> leak ho to bhi jaldi bekaar.
- auth: /upload/init pe user authenticated (JWT) -> ownerId set -> koi doosre ki file na chhede.
- GET /status -> sirf owner apni trackingId dekhe (authz check).
- validation = virus/malware scan bhi (third-party) -> poison file store na ho.
-> line: "presigned URL short-lived + owner-scoped authz + validation me virus-scan."
```

### 8e. DOWNLOAD path (validated file wapas kaise)
```
- GET /download/{trackingId} -> server presigned GET-URL deta -> client S3 se SEEDHA download
  (bytes phir bhi app-server se nahi guzarte — upload jaisa hi ulta).
- bahut download / bade file -> CDN (CloudFront) S3 ke aage -> edge se fast + S3 load kam.
-> line: "download bhi presigned GET (direct S3); popular files CDN se."
```

---
> CORE pattern (every design): Requirements -> Estimate -> API -> Data-model+DB(kyun) -> HL boxes -> DEEP DIVE(options->choose)
> -> bottleneck -> wrap. META: think out loud, trade-off har choice, chup mat baitho.
> ★ FOLLOW-UPS (step 8): folder-rollup · orphan-cleanup(S3 lifecycle) · cache-staleness(write-through) · security(short-lived presigned+authz) · download(presigned GET+CDN).
