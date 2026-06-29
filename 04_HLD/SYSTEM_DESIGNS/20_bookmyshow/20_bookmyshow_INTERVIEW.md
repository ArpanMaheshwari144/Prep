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
   PROBLEM: 2 ALAG user ek SAATH same seat A1 book karein -> dono ko "available" dikhe -> dono book -> 1 seat 2 ko = disaster.
            (= wahi "check-then-act RACE" jo payment idempotency + trading no-double-match me tha.)

   FIX (mark seat atomically -> doosra dekhe taken -> ek hi jeete):
   1. ATOMIC conditional UPDATE (best):
        UPDATE seats SET status='booked', user=X WHERE seat_id='A1' AND status='available'
        -> 2 request ek saath: DB me sirf EK ka update lagega (atomic).
           doosre ka WHERE 'available' nahi milega -> 0 rows -> FAIL -> "seat taken, doosri lo".
        -> "check(available?) + mark(booked)" EK ATOMIC step -> race khatam.
   2. ya ROW LOCK: SELECT ... FOR UPDATE (seat row lock -> check -> book -> release).

   SEAT HOLD with TTL: select kare -> seat 'held' (5 min) jab tak pay kare ->
        pay success -> 'booked' | TTL expire -> wapas 'available' (na double-book, na forever-block).
```

### ⚠ idempotency-key vs atomic-mark (yeh confuse hota — clarify)
```
   2 ALAG user, ek seat (double-booking) -> ATOMIC mark / row-lock   (race BETWEEN users)   <- yahan
   1 SAME user, duplicate (retry/double-click "Pay") -> IDEMPOTENCY key (retry-dedup)        <- alag problem
   -> dono booking me hote, par ALAG problem ke liye. tool ko problem se match karo.
   (Arpan ka "seat mark-booked, doosra taken dekhe" = CORRECT; sirf "idempotency" word galat tha -> woh atomic-mark hai.)
```

---
> CORE: clarify -> scale -> API -> boxes -> data+DB(kyun) -> DEEP DIVE -> bottleneck -> wrap.
> BookMyShow twist: CONSISTENCY (no double-book) = concurrency control (lock/atomic) + CP. browse=cache/replica, spike=queue.
