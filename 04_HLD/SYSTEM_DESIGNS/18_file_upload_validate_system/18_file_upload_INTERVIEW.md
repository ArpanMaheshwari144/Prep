# File Upload + Validate + Track System — INTERVIEW (8-step framework)

> JP ne ye ACTUALLY poocha (real SDE-3 writeup): file/folder lo -> third-party se validate (2-3 sec)
> -> store -> user ko tracking-link do. General product design (NOT finance-specific).
> Framework: 04_HLD/INTERVIEW_FRAMEWORK.md

---

## STEP 1 — REQUIREMENTS clarify (solution pe mat kudo)
```
   FUNCTIONAL:  file upload -> third-party se validate -> store -> tracking-link/status do
   NON-FUNCTIONAL:  fast response (user wait na kare), data lost na ho, scalable, status hamesha pata
   clarifying Qs:  file size limit? file types? validation kitna time? folder bhi ya sirf file?
```

## STEP 2 — SCALE / numbers
```
   maano 1 lakh files/day -> ~1-2 writes/sec
   log status BAAR-BAAR check karte -> reads zyada -> READ-HEAVY
   TRICK: 1 din ~ 100,000 sec -> per-day / 100,000 = per-sec
   -> ye batata: read-heavy = cache; bada data = shard; spike = queue
```

## STEP 3 — API design
```
   POST /upload  {file}        -> turant trackingId wapas (BANANA -> POST)
   GET  /status/{trackingId}   -> status poocho: VALIDATING/DONE/FAILED (LAANA -> GET)
```

## STEP 4 — HIGH-LEVEL boxes
```
   Client -> API Gateway -> Upload Service -> [ file bytes -> S3 / object storage ]
                                            -> [ metadata  -> DB ]
                                            -> [ validation (2-3 sec) -> QUEUE -> Worker ]
   KEY decision: file bytes (bada) -> S3 (DB nahi, DB slow+mehnga badi cheez ke liye)
                 metadata (chhota: naam/status/owner) -> DB
                 slow validation -> queue+worker (user wait na kare)
```

## STEP 5 — DATA MODEL + DB (KYUN bolo)
```
   files: trackingId(KEY) | fileName | ownerId | status | s3_url | createdAt
   status = UPLOADING / VALIDATING / DONE / FAILED  (status = "kahan tak pahuncha" -- failure-handling base)
   DB: simple key-lookup, status ko ACID chahiye -> PostgreSQL (SQL)
       (massive scale + simple key-value -> NoSQL; money -> hamesha SQL/ACID)
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

## STEP 7 — BOTTLENECK / scale
```
   - Upload Service ek -> kai instances + LB
   - DB read load (status) -> READ REPLICAS
   - bahut files -> SHARD (trackingId pe)
   - validation backlog -> WORKERS auto-scale (queue-depth pe)
   - S3 -> already scalable (managed)
```

## STEP 8 — WRAP
```
   Client -> LB -> Upload Service -> file=S3, metadata=DB, validation=async(queue+worker),
   status track. Scale: replicas + shard + worker auto-scale.
   Improve: virus-scan, retry on fail, CDN for downloads.
```

---
> CORE pattern (every design): clarify -> scale -> API -> boxes -> data+DB(kyun) -> DEEP DIVE(options->choose)
> -> bottleneck -> wrap. META: think out loud, trade-off har choice, chup mat baitho.
