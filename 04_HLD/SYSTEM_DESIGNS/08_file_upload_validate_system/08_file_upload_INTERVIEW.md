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
```

## STEP 6b — PRESIGNED URL + MULTIPART (file-upload ka asli maal)
```
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
> CORE pattern (every design): Requirements -> Estimate -> API -> Data-model+DB(kyun) -> HL boxes -> DEEP DIVE(options->choose)
> -> bottleneck -> wrap. META: think out loud, trade-off har choice, chup mat baitho.
