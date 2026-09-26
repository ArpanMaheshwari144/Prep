# BookMyShow (Ticket Booking) — POORA ROUND (4 MOVE)

> **NAV** — ARCHETYPE C · DIL: do log EK seat na lein. UP: [MASTER](../../00_MASTER_SHEET.md) · CONCEPTS: [db-what-when](../../FOUNDATIONS/09_databases_what_when.md) · [CAP](../../FOUNDATIONS/08_cap_theorem.md) · saath: [payment](../07_payment_system/07_payment_system.md)

> JP-relevant: CONSISTENCY-critical (seat double-book na ho = trading ke no-double-spend jaisa).
> CONCURRENCY = asli khel.
> 15-Sep: asli mock-video ke hisaab se dobara likha — koi 7-step rail nahi, sirf 4 move.
> Har jagah: **tu kya BOLTA hai · BOARD pe kya banta · FAISLA + KYUN**.

```
★★ TEEN NIYAM (poori file par lagte — [01_DELIVERY](../../01_DELIVERY.md) 5b)
   1. PERFECT design ek saath mat banao — chhote se shuru, dikkat pe badhao
   2. NUMBER ke peeche mat bhaago — bolo, ek faisla nikaalo, aage badho
   3. BOTTLENECK ratto mat — KHUD USER banke raasta chalao, khud dikh jaayega
```

---

# MOVE 1 — POOCHO (board pe abhi kuch nahi)

```
   TU: "BookMyShow me kai hisse hain — browse/search, seat booking, payment, refunds,
        recommendations. Aap kis pe focus karwana chahenge?
        Main seat BOOKING wale hisse pe ja sakta hoon — wahin asli dikkat hai."

   TU: "Kuch cheezein confirm kar lun —
          - popular release pe ek saath kitne log same show pe aayenge?
          - payment humein khud handle karna hai ya external gateway?
          - seat select karne ke baad kitni der HOLD rakhna chahiye?
          - refund / cancellation scope me hai?"

   ★ pehla sawaal sabse zaroori — spike hi is design ka dusra sabse bada dushman hai
     (pehla dushman: double booking)
```

---

# MOVE 2 — DO CHHOTE BLOCK LIKHO

```
   ┌──────────────────────┐    ┌────────────────────────────────────┐
   │ BookMyShow           │    │ Use cases:                         │
   │   - Movie            │    │   - movies / shows dekho           │
   │   - Show (time+hall) │    │   - seat map dekho                 │
   │   - Seat             │    │   - seat BOOK karo (payment)       │
   │   - Booking          │    │   - ticket confirm                 │
   │   - User             │    │                                    │
   └──────────────────────┘    │ NOT in scope: refunds, recommend-  │
                               │   ations, reviews                  │
   ┌──────────────────────────┐└────────────────────────────────────┘
   │ Kya chahiye (NFR):       │
   │  - EK seat DO logon ko   │ <- DIL
   │    na bike (CONSISTENCY) │
   │  - browse fast ho        │
   │  - popular show ki bheed │
   │    jhel sake             │
   │  - reliable              │
   └──────────────────────────┘

   TU: "Yahan consistency hi dil hai — double booking disaster hai, paise jaisa mamla hai.
        Isliye CAP me main CP ki taraf jhukunga: partition me reject kar dunga,
        par ek seat do logon ko kabhi nahi dunga."
```

```
   Numbers — yahan DO ALAG load hain (ye farak bolna, ek number mat bolna):

     10 M users

     1. BROWSE   (shows/seat-map dekhna)   -> SAB karte hain -> lakhs reads -> READ-HEAVY
     2. BOOKING  (seat book karna)         -> kam log, PAR concurrency-critical + SPIKY
                                              (popular release -> ek saath SAME seats pe)

   HAR LOAD SE EK FAISLA:
     browse read-heavy   ──►  CACHE + READ REPLICA
     booking spiky       ──►  QUEUE (spike absorb)
     data chhota hai     ──►  SHARDING KI ZAROORAT NAHI
                              (movies/shows/seats gine-chune hain — ye bolna,
                               warna log bina soche shard bol dete hain)
```

---

# MOVE 3 — BOXES BANAO (chhota banao, phir dikkat pe badhao)

```
   TU: "Sabse simple se shuru."

        USER ──► [ App ] ──► [ DB ]
                              seats(seat_id, show_id, status)
                              "book" dabao -> status = 'booked'

   TU: "Ek akela banda book kar raha ho to chalta hai. Ab do log bithaate hain."
```

### dikkat 1 — "do ALAG user ne EK SAATH seat A1 book kar diya"

```
        User X: "A1 available hai?" -> haan ──┐
        User Y: "A1 available hai?" -> haan ──┤   dono ne available dekha
        User X: book karo -> booked           │
        User Y: book karo -> booked           ┘   EK seat DO logon ko = DISASTER

        (= wahi "check-then-act RACE" jo payment idempotency me tha,
           aur trading ke no-double-match me bhi)

   FAISLA 1 — ★ ATOMIC CONDITIONAL UPDATE (sabse accha):

        UPDATE seats
           SET status = 'booked', user_id = X
         WHERE seat_id = 'A1' AND status = 'available'

        do request ek saath aayein -> DB me sirf EK ka update lagega (atomic hai)
        doosre ko WHERE me 'available' milega hi nahi -> 0 rows updated -> FAIL
        -> "seat ja chuki, doosri chuniye"

        => "check (available?) + mark (booked)" EK ATOMIC step ban gaya -> race khatam

   FAISLA 2 — ya ROW LOCK:  SELECT ... FOR UPDATE
        (seat row lock -> check -> book -> release)

   TU: "Do step me karne se hamesha gap rahega. Isliye condition ko UPDATE ke andar hi
        daal deta hoon — DB khud tay kar dega ki kaun jeeta."
```

### dikkat 2 — "user ne seat chuni, ab 3 minute payment kar raha hai"

```
        3 minute tak seat ka kya status ho?
            'available' rakhi     -> beech me koi aur le gaya -> user ka payment bekaar
            'booked' kar di       -> payment fail hua to seat HAMESHA ke liye block

   FAISLA: SEAT HOLD + TTL

        seat select   ──►  status = 'held', held_until = now + 5 min   (SQL me TTL nahi hota —
                           isliye held_until column; "khali?" = status='available' OR held_until < now,
                           ya ek sweeper job purane hold wapas 'available' kare)
                              │
              payment SUCCESS ──►  status = 'booked'
              TTL EXPIRE      ──►  status = 'available'  (wapas sabke liye khuli)

        => na double booking, na hamesha ke liye block
```

### dikkat 3 — "payment page pe user ne do baar 'Pay' daba diya"

```
   FAISLA: IDEMPOTENCY KEY (payment wala tool)

   ★★ YE DO CHEEZEIN ALAG HAIN — ye confuse hota hai, clear rakhna:

      2 ALAG user, EK seat (double booking)
          -> race BETWEEN users
          -> ilaaj: ATOMIC mark / row lock                    <- dikkat 1 wala

      1 SAME user, duplicate request (retry / double click "Pay")
          -> retry ka dedup
          -> ilaaj: IDEMPOTENCY key                            <- ye wala

      dono booking flow me hote hain, par ALAG problem ke liye.
      Tool ko problem se match karo.

   (Arpan ki mock-line "seat mark-booked kar do, doosra taken dekhe" bilkul SAHI thi —
    sirf uspe "idempotency" shabd lag gaya tha; wo ATOMIC MARK hai.)
```

### dikkat 4 — "book koi-koi karta hai, par seat-map SAB dekh rahe hain"

```
        browse : lakhs reads  (shows, timings, seat map)
        booking: kam, par nazuk

   FAISLA: dono raaste ALAG kar do

        browse  ──►  [ CACHE (Redis) ]  seat-map + show data  (~99% hit)
                     [ READ REPLICA ]   baaki reads
        booking ──►  [ SQL primary ]    atomic update wala raasta

   TU: "Browse ko primary DB tak jaane hi nahi dunga — wo cache aur replica se nipat jaayega.
        Primary sirf booking ke liye bacha rahega."
   ★ seat-map thoda purana dikh jaaye to chalega (booking ke waqt atomic check hai hi)
```

### dikkat 5 — "popular release — ek saath lakhs log, wahi show, wahi seats"

```
        [ spike ]  ──►  seedha DB pe  ──►  hot row pe contention  ──►  DB thapp

   FAISLA: QUEUE (Kafka) + per-show SERIALIZE

        USER ──► [ App ] ──► [ QUEUE ]  spike yahin absorb ho gaya
                                 │
                                 ▼
                        per-show WORKER  -> us show ke seat-request ek-ek karke
                                            -> atomic mark race-safe aur contention kam

   ★ QUEUE hi kyun (replica/LB nahi): replica READ scale karta hai,
     write ka spike QUEUE hi absorb karti hai. Ye farak bolna.

   ★ aage: VIRTUAL WAITING ROOM (bade release pe user ko line me lagana —
     "aapka number 12,340 hai") -> load smooth ho jaata hai
```

### dikkat 6 — "Redis restart hua — aur 99% browse traffic seedha primary DB pe gir gaya, jahan booking ke atomic UPDATE chal rahe the"

```
        seat-map ka poora read Redis se ja raha tha (dikkat 4 wala faisla)
        Redis gaya   ->  wahi read ab PRIMARY pe
                     ->  primary ka CPU bhar gaya
                     ->  aur ab BOOKING ka atomic update bhi ruk gaya

        = BROWSE ki kharabi ne BOOKING maar di
          (browse me galti chalegi, booking me nahi -- par yahan halke ne bhaari ko le dooba)

        FAISLA: Redis CLUSTER            ek node mare to baaki chalein
                + browse ka read REPLICA se, booking ka update PRIMARY pe
                + SQL pe replica + auto-failover (Patroni / RDS Multi-AZ; Sentinel Redis ka hai)

        ★ DONO RAASTE ALAG KARO -- warna halka kaam bhaari kaam ko le doobta hai
```

### dikkat 7 — "ek App box pe 500 log seat chun rahe the, aur wo box gir gaya"

```
        unke 3-minute wale HOLD ka kya hua?

        -> HOLD DB me hai (seats.status='held' + held_until), box ki memory me NAHI
           -> seat ab bhi hold hai, apne TTL pe hi chhutegi
           -> user dobara judega to kisi DOOSRE box pe jayega, aur wahi hold dekhega

        FAISLA: kai App instance + LB (App stateless hai, isliye chal jaata hai)

        ★ ye SIRF is liye kaam karta hai ki HOLD box ke BAHAR rakha gaya tha --
          wo faisla dikkat 2 me liya gaya tha, aur wahi yahan bacha raha hai
        ★ agar hold app ki memory me hota to box girte hi 500 seat ka pata hi na chalta
```

### ab poora naksha (jahan pahunche) + har box ka KYUN

```
        USER
          │
     [ LOAD BALANCER ]
          │
     [ APP SERVERS ]
          ├──► [ CACHE (Redis) ]     seat-map + show data  — browse read-heavy (~99% hit)
          ├──► [ READ REPLICA ]      baaki browse reads
          ├──► [ QUEUE (Kafka) ]     booking spike absorb -> per-show worker serialize
          │             │
          │             ▼
          ├──► [ DB (SQL, primary) ] seats + bookings — ACID, row lock, atomic conditional UPDATE
          │                          (+ replica + failover)
          └──► [ PAYMENT SERVICE ]   external -> retry + idempotency key

     LB        : traffic baantna
     App       : stateless -> instance badha do
     Cache     : browse ko DB tak jaane hi mat do
     Replica   : bache-khuche read
     Queue     : write ka SPIKE absorb + per-show serialize (replica ye nahi kar sakti)
     SQL       : consistency = dil -> ACID + row-lock -> double booking assambhav
     Payment   : bahar ka -> retry-safe banao
```

---

# MOVE 4 — BOLTE-BOLTE JODO (jo poocha jaaye, wahi kholo)

## ► "API kya hogi?"

```
   GET  /movies?city=BLR                 ->  movies list
   GET  /shows?movieId=X                 ->  show timings
   GET  /shows/{id}/seats                ->  seat map (available / held / booked)
   POST /bookings  {showId, seats, user} ->  seats HOLD (atomic) -> bookingId + TTL
   POST /bookings/{id}/pay               ->  pay success -> 'booked'  |  fail -> release
```

## ► "DB me kya rakhoge, aur kaunsa DB?"

```
   seats(seat_id, show_id, status ['available' / 'held' / 'booked'], user_id, held_until, version)
   bookings(booking_id, user_id, show_id, seat_ids, status, created_at)

   ★ DB = SQL (NoSQL nahi) — KYUN:
        consistency = dil hai. ACID + row-lock chahiye atomic seat update ke liye
        (UPDATE ... WHERE status='available')  -> double booking assambhav
        NoSQL ki eventual consistency yahan RISKY hai — do node alag-alag "available" keh sakte hain.

   ★ data chhota hai (movies / shows / seats gine-chune) -> SHARDING NAHI
     (ye bolna — bina zaroorat shard bolna over-engineering hai)

   ★ version column: optimistic locking ke liye (agar row-lock ki jagah wo chunna ho)
```

## ► "Double booking exactly kaise roka?" (deep-dive ka dil — upar dikkat-1)

```
   UPDATE seats SET status='booked', user_id=X
    WHERE seat_id='A1' AND status='available'

        ek request     -> 1 row updated -> SUCCESS
        doosri request -> 0 rows        -> "seat taken"

   ya: SELECT ... FOR UPDATE (row lock -> check -> book -> release)

   + HOLD with TTL: 'held' 5 min -> pay -> 'booked' | expire -> 'available'
```

## ► "Kahan tootega / 10x pe?"

```
   ★ RATTO MAT — user ka raasta chalao:

      user app kholta hai
          │
          ├─► browse       -> lakhs reads         -> CACHE (99%) + READ REPLICA
          ├─► seat select  -> do log ek seat pe   -> ATOMIC conditional UPDATE / row lock
          │                   pay me time lag raha-> HOLD + TTL
          ├─► popular show -> spike, same seats   -> QUEUE absorb + per-show serialize
          │                   HOT ROW contention  -> serialize + atomic (ek hi jeetega)
          ├─► payment      -> retry / double tap  -> idempotency key
          └─► koi box gira -> SPOF                -> replicate: Redis cluster, DB replica + failover

      AAGE badhata to: virtual waiting room (bade release ki bheed),
                       distributed lock (Redlock) agar kai DB ho jaayein,
                       seat-TTL tune karna, popular-show analytics.
```

## ► WRAP (ek saans me)

```
   "USER -> LB -> App servers.
    Browse cache (99%) aur read replica se; booking SQL primary pe atomic conditional UPDATE
    (ya row lock) se — isliye ek seat do logon ko ja hi nahi sakti.
    Seat select pe 'held' + 5 min TTL — payment aaya to 'booked', warna wapas 'available'.
    Popular release ka spike queue absorb karti hai, aur per-show worker requests serialize karta hai.
    Payment external hai to retry + idempotency key.
    DB SQL rakha kyunki consistency hi dil hai; data chhota hai isliye sharding nahi."
```

---

> ★ BookMyShow ka TWIST ek line me: CONSISTENCY (no double-book) = concurrency control
> (atomic mark / lock) + CP. Browse = cache + replica. Spike = queue.

---

## ═══ APPENDIX — BLOCK-KAB REFLEX (reusable, HAR design pe lagao) ═══
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
   concurrency (no double-X)     ->  LOCK / atomic / single-thread-per-resource
   ─────────────────────────────────────────────────
   PRINCIPLE: need dekho -> block match karo. (har block ka apna kaam; mismatch mat karo
              e.g. spike ke liye replica NAHI -- replica read-scale hai, write-spike absorb queue karta)
```

---

[← MASTER SHEET](../../00_MASTER_SHEET.md)
