# URL Shortener — INTERVIEW DRIVE (8-step framework)

> Arpan ne KHUD derive kiya (21 Jun) — pehla full solo HLD drive. Yahan jaise tune chalaya +
> jo chhoti corrections seekhi. Reference: micro-read ke liye. (Framework: INTERVIEW_FRAMEWORK.md)

---

## STEP 1 — REQUIREMENTS clarify (chup mat baitho)
```
   FUNCTIONAL:  long URL -> short URL banao;  short pe click -> original pe redirect
   NON-FUNCTIONAL:  fast redirect (low latency), high availability, READ-heavy
   clarifying Qs:  custom short-URL allow? links expire hote ya hamesha?
```

## STEP 2 — SCALE / numbers
```
   maano 100M writes/day.  TRICK: 1 din ~ 100,000 sec (10^5) -> easy division
   100M/day = 10^8 / 10^5 = ~1000 writes/sec
   read = 100x = ~100,000 reads/sec  -> READ-HEAVY (click >> create)
   (interview mein PRECISE math nahi -> round karo, ~1000 kaafi)
   -> ye number drive karta: read-heavy = cache+CDN; billions = sharding
```

## STEP 3 — API design
```
   POST /shorten   {longUrl} -> {shortUrl}      (BANANA -> POST: data bhej rahe / naya bana rahe)
   GET  /{code}    -> 302 redirect to longUrl   (LAANA  -> GET: fetch kar rahe)
   YAAD: banana=POST, laana/padhna=GET  (ye swap mat karna)
```

## STEP 4 — HIGH-LEVEL boxes
```
   Client -> CDN -> LB -> App Servers -> Redis Cache -> Database
   read flow (cache-aside):
      cache check -> HIT: turant return (DB nahi gaye)
                  -> MISS: DB se laao -> cache mein daalo (hot ban gaya) -> return
   HOT URLs hi cache (sab nahi -- billions cache nahi kar sakte; popular wale)
```

## STEP 5 — DATA MODEL + DB choice (KYUN bolo)
```
   schema:  shortCode (KEY) -> longUrl   (+ createdAt, expiresAt)
   KEY = shortCode (kyunki redirect mein short se long laana hota -> lookup by shortCode)
   DB = NoSQL (DynamoDB/Cassandra) -- KYUN? pure key-value lookup, no joins/relations,
        read-heavy -> fast key-lookup + easy horizontal scale.
        (SQL bhi chalता par yahan relations nahi -> NoSQL behtar)
```

## STEP 6 — DEEP DIVE: short code kaise generate? (asli khel — 3 options)
```
   1. MD5 HASH       -> collision ho sakta (2 hash same) + compute cost
   2. RANDOM (62 ch) -> collision check ke liye HAR BAAR DB hit (slow)
   3. COUNTER + Base62 (BEST) -> counter (1,2,3...) -> Base62 encode -> short code
        guaranteed UNIQUE (counter kabhi repeat nahi -> zero collision -> no DB check)
        + chhota (Base62 = a-z,A-Z,0-9). 1000000 -> "4c92"
        counter single point na bane -> har server ko RANGE do (1-1000, 1001-2000...)
   -> WINNER: counter + Base62
```

## STEP 7 — BOTTLENECK / SCALE
```
   - rate limiting (requests/server, abuse rokо)
   - READ scale -> read REPLICAS  (read-heavy, reads baant)
   - WRITE scale -> SHARDING  (data tukdon mein; "write replicas" nahi hota --
                    replicas sirf read ke liye)
   - shard by shortCode (billions ek DB mein nahi)
   - async analytics (click-count queue se -> redirect block na ho)
   - geo-based routing (global -> nearest region, latency kam)
```

## STEP 8 — WRAP
```
   Client->CDN->LB->App->Redis->NoSQL(sharded); counter+Base62 codes; read-replicas+cache
   for reads; async analytics. Aage: custom URLs, expiry/TTL cleanup, geo-distribution.
```

---
> CORRECTIONS seekhi (small label slips, soch sahi thi): GET/POST swap (banana=POST),
> "write replicas" -> sharding, KEY = shortCode. Asli reasoning (read-heavy, NoSQL+kyun,
> hot-cache, counter+Base62, shard) sab khud sahi derive kiya.
