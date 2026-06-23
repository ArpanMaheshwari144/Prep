# Payment System (UPI-style) — HLD

> Finance HLD ka CORE. JP/GS interview favourite. 4 hard problems jo paise ko special banate:
> Idempotency, Consistency, Failure-handling, Ledger/Audit.

---

## SCOPE
User A, User B (ya merchant) ko paisa bheje — RELIABLY.
Concrete: Arpan, merchant ko Rs.500 bhejta hai.

## Naive flow (sabse simple)
```
        [User App]
            |  "Rs.500 bhejo merchant ko"
            v
     [Payment Service]
            |
            v
   +---------------------+
   |   LEDGER / Bank     |
   |  Arpan:    -Rs.500  |   (debit payer)
   |  Merchant: +Rs.500  |   (credit payee)
   +---------------------+
```

## Kyun itna simple NAHI — paisa SACRED hai
```
   X  KHO nahi sakta         (debit hua, credit nahi -> Rs.500 gayab)
   X  DUPLICATE nahi ho sakta (double-charge -> Arpan se Rs.1000)
   X  ATAK nahi sakta         (network mara beech mein -> paisa kahan?)
   OK Har paisa TRACEABLE ho  (audit)
```

## 4 HARD PROBLEMS (JP yehi poochta)
```
   1. IDEMPOTENCY      -> double-charge na ho
   2. CONSISTENCY      -> debit + credit DONO ya KOI nahi (atomicity)
   3. FAILURE-HANDLING -> crash beech mein -> paisa ka kya
   4. LEDGER / AUDIT   -> permanent, traceable record
```

---

## 1. IDEMPOTENCY  (double-charge na ho)

**Problem:** Arpan "Pay Rs.500" tap karta, network slow, woh DOBARA tap karta (ya app retry).
Same payment 2 baar server pe -> bina protection -> Rs.1000 kat gaye. (Ya: response kho gaya -> client retry -> double process.)

**Core:** ek LOGICAL payment kai baar aa sakta (user/app/network retry). Chahiye:
chahe 5 baar aaye, paisa SIRF EK BAAR move ho.

**Solution — IDEMPOTENCY KEY** (analogy: ticket/token number):
```
   Client har payment ke liye UNIQUE key banata (UUID "abc-123"), request ke saath bhejta.
   RETRY pe WAHI key (nayi nahi).

   Server ke paas "processed register":
        key register mein HAI?
          NAHI -> process karo (Rs.500 move) -> key + result STORE karo
          HAAN -> process MAT karo -> STORED result wapas bhejo

        Tap 1 (abc-123): register khaali -> process -> store "done"
        Tap 2 (abc-123): register mein hai -> SKIP -> wahi "done" wapas
        -> paisa SIRF EK BAAR moved
```

**DSA connection:** idempotency-register = wahi hashmap "pehle dekha?" check = CONTAINS DUPLICATE jaisa.

**Subtle — RACE CONDITION:** do requests EK SAATH (same key) -> dono "register check" karein
store se pehle -> dono "naya" samajh ke process -> double-charge.
=> check + store ATOMIC hona chahiye (lock / DB unique-constraint) -> ek hi jeete.

**Atomic kaise (drill se — 23 Jun):**
```
   - DB UNIQUE CONSTRAINT (saaf): key pe unique column -> req1 INSERT success, req2 same-key INSERT -> DB REJECT
     -> DB ka atomic insert = automatic lock (alag lock-service nahi chahiye). Money ke liye DURABLE (best).
   - Redis SETNX (set-if-not-exists, atomic): fast in-flight dedup. PAR cache volatile -> money ke liye DB bhi chahiye.
   - BEST = Redis SETNX (fast layer) + DB unique-constraint (durable backstop, asli source of truth).
   key STATUS rakho: IN_PROGRESS / DONE. req2 dekhe -> IN_PROGRESS=ruko | DONE=stored result wapas.
```
**TRAP:** done pe key DELETE mat karo! late retry (network timeout, 30 sec baad) -> key gayab -> "naya" samjhe -> DOBARA charge.
   -> TTL (24h) ya result store rakho; cache temporary, DB = final truth.

```
   Idempotency = "same key, same outcome — chahe kitni baar aaye, paisa ek baar"
```

---

## 2. CONSISTENCY  (debit + credit dono ya koi nahi = atomicity)

**Problem:** debit (Arpan -Rs.500) aur credit (Merchant +Rs.500) — DONO saath, ya KOI nahi.
```
   Arpan -Rs.500 ho gaya -> CRASH -> Merchant +Rs.500 NAHI -> Rs.500 GAYAB
```

**Analogy (SEE-SAW):** debit aur credit = see-saw ke do sire. Dono saath hilte ya koi nahi.
Paisa banta/marta nahi — sirf MOVE hota (conserved).

**Solution — depends accounts kahan:**
```
   Case A — DONO SAME database:  (easy)
        DB TRANSACTION (ACID):
            BEGIN
              Arpan -= 500 ; Merchant += 500
            COMMIT     <- dono commit ya dono ROLLBACK
        ACID ka 'A' (Atomicity) = both-or-neither. Crash -> auto rollback.

   Case B — ALAG banks/services (DISTRIBUTED):  (hard, ek DB txn nahi)
        - SAGA pattern  -> steps + COMPENSATING transaction (step fail -> pichle ka UNDO)
        - 2-Phase Commit -> coordinator "ready?" phir "commit" — par slow/blocking
```

**Invariant:** har waqt SUM(debits) == SUM(credits). Paisa create/destroy nahi hota.

```
   Consistency = "money movement = ek ATOMIC flip (both-or-neither). Paisa conserved."
```

---

### SAGA — gehra (distributed system ka ROLLBACK)

**Core (ek line):** distributed system ka rollback — ek service fail -> peeche chalo, sabka UNDO -> bas.

**Kyun chahiye:** single DB mein rollback AUTOMATIC milta (ACID ka 'A' — DB khud undo karta).
ALAG DBs/services mein woh free rollback NAHI milta -> isliye KHUD har step ka undo (compensating
action) likhna padta. SAGA = "manually banaya hua rollback".

**Analogy (TRIP BOOKING — flight + hotel + cab, 3 alag websites se):**
```
   Step 1: Flight book  -> DONE
   Step 2: Hotel book   -> DONE
   Step 3: Cab book     -> FAIL
   => peeche chalo, sabka undo (REVERSE order):
        CANCEL hotel  ->  CANCEL flight
   -> system wapas clean, koi adhoora booking nahi pada
```

**Payment mein:**
```
   Step 1: Bank A se Arpan -500           -> DONE
   Step 2: Bank B mein Merchant +500      -> FAIL
   Compensating: Bank A mein Arpan +500 wapas (undo step 1)
   -> paisa wapas, kuch gayab nahi
```

**Trade-off (JP isko sunna chahta):**
```
   SAGA          -> local steps + compensating-undo. Loose, scalable. PAR beech mein thodi der
                    inconsistency dikh sakti (eventual) -> phir undo se theek.
   2-Phase-Commit -> coordinator "ready? -> commit". Strict, par SLOW / BLOCKING.
```

```
   Saga = distributed rollback. ACID ka free rollback nahi (alag DBs) -> khud ka undo banaya.
          step gira -> peeche chal ke sabka compensating-undo -> bas.
```

---

## 3. FAILURE-HANDLING  (crash beech mein -> paisa kahan)

**HARD TRUTH:** crash ke baad tujhe aksar PATA NAHI:
debit hua? credit hua? poora ho gaya bas RESPONSE kho gaya?
=> "bas rollback" SAFE nahi (kya pata SUCCESS ho chuki ho).

**Solution — kabhi ASSUME nahi, hamesha RECORD + RESOLVE:**
```
   (1) STATUS tracking (write-ahead):
       har payment ka status, durable storage mein:
          PENDING -> PROCESSING -> SUCCESS / FAILED
       Kuch karne se PEHLE "PENDING" likho (intent).
       -> crash ke baad "yeh PENDING thi" pata -> resolve. Kabhi GUM nahi.

   (2) RECONCILIATION (background job):
       PENDING/atke payments dhoondho -> bank se POOCHO "yeh hui?"
          haan -> SUCCESS ; nahi -> retry (idempotency safe) ya FAILED + refund
       -> koi payment LIMBO mein nahi

   (3) IDEMPOTENCY + retry: retry safe (double-process rokta)
```

**Analogy (COURIER TRACKING):** har parcel = tracking number + status. Courier gir gaya ->
parcel gum nahi -> status dekho, resolve (deliver/return/check). Tracking = kabhi lost nahi.

**Honest status ka jaadu:** status "not delivered/PENDING" -> system ko PATA woh unresolved hai
-> SILENTLY kho nahi sakta. Resolve hota: AUTO (reconciliation) ya MANUAL (user complain -> support status dekhe).
Record = single source of truth.

```
   Failure-handling = "kabhi assume nahi. Har payment ka STATUS record. Crash -> reconcile.
                       Paisa hamesha: done / undone / being-checked — kabhi gum nahi."
```

---

## 4. LEDGER / AUDIT  (finance ka dil)

```
   Har transaction ka PERMANENT, IMMUTABLE record — kabhi delete/edit nahi.
   -> 6 mahine baad dispute -> record dekho -> sach
   -> regulators (RBI/SEC) maangte: har paisa kab-kahan-kyun traceable
```

**DOUBLE-ENTRY** (500-saal purana, abhi bhi use):
```
   Har transaction = ek DEBIT + ek CREDIT (barabar), SAATH likhe:
        Arpan -> Merchant Rs.500:
            Arpan account:    DEBIT  Rs.500
            Merchant account: CREDIT Rs.500
   -> har waqt SUM(debits) == SUM(credits)  (invariant)
   -> match na kare -> GADBAD (paisa kho/ban gaya) -> turant pakad
```

**Analogy (PERMANENT DIARY):** ledger = diary jisme har entry PEN se (mit nahi sakti).
Galti -> NAYI entry se correct karo (purani mitao mat) -> history bachi rahe = audit-trail.

```
   Ledger/Audit = "permanent, immutable, double-entry record. Debits=Credits hamesha.
                   Kuch bhi traceable, kabhi mitega nahi = TRUST ka base."
```

---

## EK LINE MEIN (revision)
```
   Idempotency      -> same key, paisa ek baar (hashmap "pehle dekha?")
   Consistency      -> debit+credit ek atomic flip (see-saw, ACID/Saga)
   Failure-handling -> status record + reconcile (courier tracking, kabhi gum nahi)
   Ledger/Audit     -> immutable double-entry, debits=credits (permanent diary)
```
> Yeh 4 = poora finance HLD ka core. JP/GS mein in concepts pe trade-off articulate karna = win.
