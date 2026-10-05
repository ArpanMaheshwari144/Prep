# BookMyShow (Ticket Booking)

> Movie / show dekho -> seat chuno -> pay -> ticket. JP-relevant: consistency-critical (trading ke no-double-spend jaisa).
> Is design ka dil: **do log EK seat na lein** (concurrency) + **popular release ki bheed**.

---

## TASVEER (ByteByteGo / Alex Xu · CC BY-NC-ND 4.0)

![CAP Theorem: One of the Most Misunderstood Terms](https://assets.bytebytego.com/diagrams/0131-cap-theorem.jpeg)
Source: [CAP Theorem: One of the Most Misunderstood Terms](https://bytebytego.com/guides/cap-theorem-one-of-the-most-misunderstood-terms/)
(booking CP, search AP)

![Pessimistic vs Optimistic Locking](https://assets.bytebytego.com/diagrams/0301-pessimistic-vs-optimistic-locking.png)
Source: [Pessimistic vs Optimistic Locking](https://bytebytego.com/guides/pessimistic-vs-optimistic-locking/)
(ek seat do log = SELECT FOR UPDATE (pessimistic) ya version check (optimistic))

---

## SHURU — poocho + numbers

```
POOCHO:  "Browse, booking, payment, refund, recommendation — main seat BOOKING pe, asli dikkat wahin."
         popular release pe ek saath kitne log same show pe?  <- spike = doosra bada dushman (pehla: double booking)
         payment khud ya external gateway? · seat chunne ke baad kitni der HOLD? · refund scope me?

FR:      movies / shows dekho · seat map · seat BOOK (payment) · ticket confirm
         scope bahar: refund · recommendation · review
NFR:     EK seat DO ko na bike (CONSISTENCY, dil) · browse fast · popular show ki bheed jhele · reliable

NUMBERS: 10M user · DO ALAG load (ek number mat bolo):
         BROWSE (shows / seat map) = sab karte -> lakhon read -> READ-HEAVY -> CACHE + READ REPLICA
         BOOKING = kam log, par concurrency-critical + SPIKY (same seat pe ek saath) -> QUEUE
         data chhota (movies / shows / seats gine-chune) -> SHARDING KI ZAROORAT NAHI (ye bolna)
```
```
POOCHEGA: "Consistency or availability — which do you pick?"  (chhota: "network toota, aakhri seat pe do log — C ya A?")
DHYAAN:   ek hi system me dono: BOOKING = CP (partition me ek side REJECT) · SEARCH = AP (seat count 2 sec purana chalega)
BOL:      "For the booking itself I'd pick consistency: I'd rather reject a write than double-book the seat.
           The search path stays available and eventually consistent."
```

---

## DABBA 0 — sabse simple

```
SOLUTION: seats(seat_id, show_id, status) · "book" dabao -> status = 'booked'
```
```
  USER
    │
    ▼
  [ App ]
    │
    ▼
  [ SQL DB ]
```

---

## DIKKAT 1 — do ALAG user ne EK SAATH A1 book kar di

```
DIKKAT:   X: "A1 available?" haan · Y: "A1 available?" haan · X book · Y book = EK seat DO ko
          (check-then-act RACE — payment idempotency aur trading no-double-match wala)

SOLUTION: ATOMIC CONDITIONAL UPDATE (sabse accha):
            UPDATE seats SET status = 'booked', user_id = X
             WHERE seat_id = 'A1' AND status = 'available'
          DB me sirf EK ka lagega · doosre ko 0 row -> "seat ja chuki, doosri chuno"
          check + mark = EK atomic step -> race khatam
          ya ROW LOCK: SELECT ... FOR UPDATE (lock -> check -> book -> release)
          ya OPTIMISTIC: version column

NAYA:     koi dabba nahi — SQL ka atomic update
```
```
POOCHEGA: "Two users book the same seat at the same time — what happens?"
DHYAAN:   2 user ek cheez = atomic / lock · 1 user ka retry = idempotency (dikkat 3)
BOL:      "Doing it in two steps always leaves a gap, so I put the condition inside the UPDATE — WHERE
           status is available. The database lets only one win; the other gets zero rows and 'seat taken'."
```

---

## DIKKAT 2 — seat chuni, ab 3 min payment kar raha

```
DIKKAT:   'available' rakhi -> koi aur le gaya · 'booked' kar di -> payment fail = seat HAMESHA block

SOLUTION: SEAT HOLD + TTL: select -> 'held', held_until = now + 5 min · pay SUCCESS -> 'booked' · expire -> khuli
          ★ PAR SQL me TTL NAHI (27-Sep mock me yahi atka): row apne aap nahi badalti,
            10:06 pe bhi 'held' likha rahega. "time nikla -> available" = GALAT, kisi ko KARNA padta.
            B aaya 10:06 -> purana WHERE status = 'available' -> 0 row -> galti se "taken"
          RAASTA 1 — booking UPDATE hi expired hold ko KHALI maane (job nahi):
            UPDATE seats SET status = 'held', user_id = 'B', held_until = now() + INTERVAL 5 MINUTE
             WHERE seat_id = 'A1'
               AND ( status = 'available' OR (status = 'held' AND held_until < now()) );
            A ka hold zinda -> 0 row · expire -> 1 row, B jeeta (atomic) · seat map pe bhi yahi check
          RAASTA 2 — SWEEPER (har minute):
            UPDATE seats SET status = 'available', user_id = NULL, held_until = NULL
             WHERE status = 'held' AND held_until < now();
            kami: ~1 min tak 'held' dikhegi
          DONO saath: UPDATE ka check = SAHI-PAN · sweeper = SAFAI

NAYA:     Sweeper job
```
```
  USER
    │
    ▼
  [ App ]
    │
    ▼
  [ SQL DB ]
    ▲
    │
  [ Sweeper job ]
```
```
POOCHEGA: "Who releases the hold after 5 minutes?"
BOL:      "SQL has no TTL, so either the booking UPDATE treats an expired hold as free — status held and
           held_until before now — or a sweeper job flips expired holds back every minute. I'd put the check
           in the UPDATE for correctness and keep the sweeper for cleanup."
```

---

## DIKKAT 3 — payment page pe "Pay" do baar daba diya

```
DIKKAT:   ek booking ka do baar charge

SOLUTION: IDEMPOTENCY KEY (payment wala tool) — client banata, retry pe SAME
          server atomic claim (UNIQUE constraint / Redis SET NX) -> dobara aaye to STORED result, error nahi
          ★ DO ALAG CHEEZ (confuse hota):
            2 ALAG user, EK seat -> race BETWEEN users -> ATOMIC mark / lock (dikkat 1)
            1 SAME user, duplicate request -> retry dedup -> IDEMPOTENCY (ye)
          (Arpan ki mock line "seat mark-booked kar do, doosra taken dekhe" SAHI thi — wo ATOMIC MARK hai,
           sirf "idempotency" shabd lag gaya tha)

NAYA:     Payment Svc (external, idempotency key ke saath)
```
```
  USER
    │
    ▼
  [ App ] ──► [ Payment Svc ]
    │
    ▼
  [ SQL DB ]
    ▲
    │
  [ Sweeper job ]
```
```
POOCHEGA: "What if the user clicks Pay twice / the client retries?"
BOL:      "The client sends the same idempotency key on a retry; the server claims it atomically with a
           unique constraint and returns the stored result the second time."
```

---

## DIKKAT 4 — book koi-koi karta, seat map SAB dekh rahe

```
DIKKAT:   browse lakhon read, booking kam par nazuk — ek hi DB pe

SOLUTION: dono raaste ALAG:
          browse  -> REDIS (seat map + show data, ~99% hit) + READ REPLICA
          booking -> SQL PRIMARY (atomic update)
          seat map thoda purana chalega — booking pe atomic check hai hi

NAYA:     Redis · Read replica
```
```
  USER
    │
    ▼
  [ App ] ──► [ Payment Svc ]
    │
    ├──► [ Redis ]
    ├──► [ Read replica ]
    │
    ▼
  [ SQL primary ]
    ▲
    │
  [ Sweeper job ]
```

---

## DIKKAT 5 — popular release: lakhon log, wahi show, wahi seat

```
DIKKAT:   spike seedha DB -> hot row contention -> DB thapp

SOLUTION: QUEUE (Kafka) + PER-SHOW WORKER -> us show ki request ek-ek karke -> atomic mark, contention kam
          QUEUE kyun, replica / LB nahi: replica READ scale karti, write spike QUEUE absorb karti
          VIRTUAL WAITING ROOM: "aapka number 12,340" -> load smooth
          ARPAN KA IDEA (27-Sep) = ADMISSION CONTROL: seat 3000, user 5000 -> darwaze pe ginti,
            pehle 3000 andar, 2000 ko TURANT "housefull" / waiting room (flash sale pattern)
            counter bhi ATOMIC (Redis DECR), warna counter pe race
          ★ BMS pe kahan tootta: user KHAAS seat (A1) chunta. pehle 3000 me X aur Y dono A1
            -> dono ko turant "booked" -> worker: Y ka 0 row -> "sorry, cancel" = sabse bura UX
            ginti batati "TOTAL bachi?", ye nahi "TERI wali bachi?" -> "booked" TABHI jab us seat ka UPDATE jeete
          SAHI JODA: 1 GATE counter · 2 TURANT "Booking in progress..." · 3 WORKER atomic UPDATE
                     4 BATAO: jeeta "confirmed" + email / SMS · haara "ye seat gayi" (poll / WebSocket)
          seat number NAHI (concert standing, sale stock) -> Arpan ka idea jaisa hai poora sahi
          "BookMyShow bhi aise karta" MAT bolo (andar public nahi) -> "a common pattern in flash sales"

NAYA:     Kafka · Booking worker · gate counter (Redis me)
```
```
  USER
    │
    ▼
  [ App ] ──► [ Payment Svc ]
    │
    ├──► [ Redis ]
    ├──► [ Read replica ]
    │
    ▼
  [ Kafka ]
    │
    ▼
  [ Booking worker ]
    │
    ▼
  [ SQL primary ]
    ▲
    │
  [ Sweeper job ]
```
```
POOCHEGA: "What if traffic suddenly spikes 10x?"
BOL:      "I'd put a counter at the door so only as many users as there are seats get in, and the rest see
           'sold out' right away. But since users pick specific seats, I only confirm a booking after that
           seat's atomic UPDATE wins — until then the user sees 'in progress' and gets the result by polling
           or a push. Plus per-user rate limits and pre-scaling before a known release."
```

---

## DIKKAT 6 — Redis restart: 99% browse seedha primary pe, booking ke update ruk gaye

```
DIKKAT:   browse ki kharabi ne BOOKING maar di (halke ne bhaari ko le dooba)

SOLUTION: Redis CLUSTER (ek node mare, baaki chalein)
          browse ka read REPLICA se, booking ka update PRIMARY pe — DONO RAASTE ALAG
          SQL: replica + auto-failover (Patroni / RDS Multi-AZ; Sentinel Redis ka hai)
          STAMPEDE: mutex (ek hi rebuild, baaki wait karke cache se)

BADLA:    Redis -> Redis Cluster · SQL primary ab auto-failover ke saath
```
```
POOCHEGA: "What if the cache goes down?"
BOL:      "Redis runs as a cluster. Browse reads fall back to the read replica, never the primary, so
           bookings keep working, and one request rebuilds a hot key while others wait."
```

---

## DIKKAT 7 — ek App box pe 500 log seat chun rahe, box gira

```
DIKKAT:   unke 3 min ke HOLD ka kya?

SOLUTION: HOLD DB me (status 'held' + held_until), box ki memory me NAHI -> seat abhi bhi held, TTL pe chhutegi
          user dobara jude -> DOOSRE box pe, wahi hold dikhe
          kai App + LB (stateless isliye chalta) · health check: 2-3 fail = pool se bahar
          ye sirf isliye chala ki hold box ke BAHAR rakha (dikkat 2 ka faisla)
          stateful copies ALAG AZ me

NAYA:     LB
BADLA:    App -> App x N
```
```
  USER
    │
    ▼
  [ LB ]
    │
    ▼
  [ App x N ] ──► [ Payment Svc ]
    │
    ├──► [ Redis Cluster ]
    ├──► [ Read replica ]
    │
    ▼
  [ Kafka ]
    │
    ▼
  [ Booking worker ]
    │
    ▼
  [ SQL primary ]
    ▲
    │
  [ Sweeper job ]
```
```
POOCHEGA: "What happens if an app server goes down?"
BOL:      "Holds live in the database, not in the box, so they survive. The load balancer health-checks the
           box out and the user's next request goes to another instance and sees the same hold."
```

---

## 10x SCALE — har dabba alag

```
App          -> stateless, box badhao
browse       -> Redis cluster (99%) + read replica
booking      -> atomic conditional UPDATE / row lock · HOLD + TTL
popular show -> Kafka + per-show serialize + gate counter + waiting room · hot row = ek hi jeete
payment      -> idempotency key
SQL          -> replica + auto-failover · data chhota, shard NAHI
AAGE:        virtual waiting room · Redlock agar kai DB · seat TTL tune · popular show analytics

POOCHEGA: "How would you scale this to 10x?"      -> user ka raasta chalo, pehle jo toote
POOCHEGA: "What's the single point of failure?"   -> SQL primary (failover), Redis (cluster)
POOCHEGA: "How do you know it's working?"         -> booking p99 · 0-row (taken) rate · Kafka lag · hold expiry count · alert
```

---

## POOCHE TO (deep-dive)

```
API:      GET /movies?city=BLR · GET /shows?movieId=X · GET /shows/{id}/seats (available / held / booked)
          POST /bookings { showId, seats, user } -> seat HOLD (atomic) -> bookingId + TTL
          POST /bookings/{id}/pay -> success 'booked' · fail release

DB:       seats(seat_id, show_id, status ['available' / 'held' / 'booked'], user_id, held_until, version)
          bookings(booking_id, user_id, show_id, seat_ids, status, created_at)
          SQL kyun: consistency = dil -> ACID + row lock -> double booking assambhav
                    NoSQL eventual = do node alag-alag "available" keh sakte -> RISKY
          version = optimistic locking (row lock ki jagah chuno to)
          data chhota -> SHARDING NAHI (bina zaroorat shard = over-engineering)

DOUBLE BOOKING (dil):  UPDATE ... WHERE seat_id = 'A1' AND status = 'available'
                       ek -> 1 row SUCCESS · doosra -> 0 row "taken" · ya SELECT ... FOR UPDATE
```

---

## AAKHRI DABBA + WRAP

```
LB · App = stateless · Redis Cluster = browse 99% · Read replica = baaki browse
Kafka + Booking worker = spike + per-show serialize · SQL primary = ACID, atomic UPDATE, failover
Sweeper = expired hold saaf · Payment Svc = external, idempotency key
```
```
  USER
    │
    ▼
  [ LB ]
    │
    ▼
  [ App x N ] ──► [ Payment Svc ]
    │
    ├──► [ Redis Cluster ]
    ├──► [ Read replica ]
    │
    ▼
  [ Kafka ]
    │
    ▼
  [ Booking worker ]
    │
    ▼
  [ SQL primary ]
    ▲
    │
  [ Sweeper job ]
```
```
BOL: "Browse goes to Redis and a read replica; booking goes to the SQL primary with an atomic conditional
      UPDATE, so one seat can never go to two people. Selecting a seat holds it for five minutes — the
      UPDATE treats an expired hold as free and a sweeper cleans up. A queue with per-show workers absorbs
      the release-day spike, and payment uses an idempotency key. SQL because consistency is the heart of
      it; the data is small, so no sharding."
```

Block kab lagana (need -> block) = MASTER SHEET §4 BLOCK MENU.

ARCHETYPE C · CONCEPTS: [db-what-when](../../FOUNDATIONS/09_databases_what_when.md) · [CAP](../../FOUNDATIONS/08_cap_theorem.md) · saath: [payment](../07_payment_system/07_payment_system.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
