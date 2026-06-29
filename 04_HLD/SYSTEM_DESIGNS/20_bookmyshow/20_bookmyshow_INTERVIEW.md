# BookMyShow (Ticket Booking) — INTERVIEW (8-step framework)

> JP-relevant: CONSISTENCY-critical (seat double-book na ho = trading ka no-double-spend jaisа). Concurrency = asli khel.
> Framework: 04_HLD/INTERVIEW_FRAMEWORK.md

---

## ★★ BLOCK-KAB REFLEX (reusable — har design pe lagao) ★★

```
   NEED                          ->  BLOCK
   ─────────────────────────────────────────────────
   read-heavy (browse/feed)      ->  CACHE + read-REPLICA
   SPIKE absorb (sudden burst)   ->  QUEUE (Kafka)          <- replica/LB nahi! queue buffer karta
   write / storage scale (big)   ->  SHARDING (shard-key = query-key)
   traffic distribute            ->  LOAD BALANCER
   availability / failover       ->  REPLICATION (copies)
   consistency vs availability   ->  CAP choice (money/booking=CP, social=AP)
   slow/async work, decouple     ->  QUEUE
   concurrency (no double-X)      ->  LOCK / atomic / single-thread-per-resource
   ─────────────────────────────────────────────────
   PRINCIPLE: need dekho -> block match karo. (har block ka apna kaam; mismatch mat karo
              e.g. spike ke liye replica NAHI -- replica read-scale hai, write-spike absorb queue karta)
```

---

## STEP 1 — REQUIREMENTS
```
   FUNCTIONAL: movies/shows dekho · seats dekho · seat BOOK (payment) · ticket confirm
   NON-FUNCTIONAL: ek seat 2 logon ko NA bike (CONSISTENCY = dil) · fast browse · scale (popular show bheed) · reliable
   Qs: users? booking concurrency? payment integrate?
   KEY: CONSISTENCY-critical (double-book = disaster, paisа jaisа) -> CAP me CP-leaning.
```

## STEP 2 — SCALE
```
   10M users. 2 load:
   - BROWSE (shows/seats dekhna) -> SAB karte -> READ-HEAVY (lakhs reads)   -> CACHE + read-REPLICA
   - BOOKING (seat book) -> kam par CONCURRENCY-critical + SPIKY (popular release -> ek saath same seats) -> QUEUE (spike absorb)
   DB sharding NAHI (data chhota: movies/shows/seats finite).
```

## STEP 3-5 (next) — API · BOXES · DATA+DB
```
   (to be filled)
```

## STEP 6 — DEEP DIVE: seat DOUBLE-BOOKING kaise roke? (concurrency — asli khel)
```
   (to be filled — JP-relevant: 2 log ek saath same seat -> ek ko hi mile)
```

---
> CORE: clarify -> scale -> API -> boxes -> data+DB(kyun) -> DEEP DIVE -> bottleneck -> wrap.
> BookMyShow twist: CONSISTENCY (no double-book) = concurrency control (lock/atomic) + CP. browse=cache/replica, spike=queue.
