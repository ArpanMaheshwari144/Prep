# System Design Interview — THE FRAMEWORK (har design isme ghusta)

> Content (LB/cache/idempotency) tu jaanta. Ye = interview kaise DRIVE karna.
> Yehi PATTERN — Payment/Stock-Broker/URL/Twitter, koi bhi design isi 8-step mein.
> (Wahi DSA "12-template" insight, HLD ke liye.)

## THE 8-STEP FLOW
```
   1. REQUIREMENTS clarify   -> FR (kya banana) + NFR (scale/latency/consistency)
                                + clarifying questions. SEEDHA solution pe jump MAT karo.
   2. SCALE / ESTIMATION     -> users, QPS, read:write ratio, storage (order-of-magnitude)
                                -> ye design decisions DRIVE karta (read-heavy->cache, billions->shard)
   3. API design             -> 2-3 main endpoints (clean contract)
   4. HIGH-LEVEL design      -> BOXES: client -> LB -> app -> cache -> DB -> queue (tera spatial home)
   5. DATA MODEL + DB choice  -> schema + SQL vs NoSQL + KYUN (trade-off bolo)
   6. DEEP DIVE (asli khel)  -> 1-2 hard part. multiple options compare -> ek chuno with reason.
                                JP-FINANCE flavor: idempotency / consistency / ACID / ledger / failure
   7. BOTTLENECK / SCALE      -> "ye break hoga -> aise fix" (cache/shard/replica/async-queue)
   8. WRAP                    -> summary + "aage ye improve karta"
```

## META-BEHAVIORS (interview-side jo content notes mein nahi)
```
   - THINK OUT LOUD (chup = death)
   - TRADE-OFF har choice pe ("X liya kyunki..., Y nahi kyunki...")
   - CLARIFYING questions poochо (interviewer ye chahta)
   - koi PERFECT answer nahi -> REASONING dikhao (curveball: derive, ratta nahi)
   - HONEST: na aaye to "ye use nahi kiya, par aise approach karunga"
   - JP: STEP 6 pe "idempotency + audit-trail + ACID" drop karo -> instant impress
```

## DEMO (URL Shortener, 20 Jun) — flow live chalaya
```
   1. clarify: long->short, click->redirect; read-heavy, low-latency, HA
   2. scale: 100M/month, R:W 100:1, ~4000 reads/s, 5yr ~6B -> shard + cache
   3. API: POST /shorten {longUrl}->{short}; GET /{code}->302 redirect
   4. boxes: Client -> LB -> App -> Redis -> DB (miss->DB->fill)
   5. data: code(PK)|longUrl|expiresAt; key-value -> NoSQL (ya sharded PG)
   6. DEEP DIVE -- short-code: hash(collision) vs random(check) vs COUNTER+Base62(best,unique)
                   + counter RANGE per server (no single bottleneck)
   7. bottleneck: cache+CDN, shard+replica, counter-range, TTL cleanup, async analytics(Kafka)
   8. wrap: summary + custom-URL/rate-limit/geo improve
```

> NEXT: practice -- har HLD design ko is flow mein KHUD bolo (Payment, Stock-Broker...).
> Framework reps se cement hota, ek baar mein nahi.
