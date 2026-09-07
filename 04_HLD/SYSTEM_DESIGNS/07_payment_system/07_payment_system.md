# Payment System (UPI-style) — 7-STEP RAIL (single spine, revise top→bottom)

> Finance HLD CORE. JP/GS favourite. 4 hard problems jo paise ko special banate: Idempotency, Consistency, Failure-handling, Ledger/Audit.
> RAIL (04_HLD/HLD_APPROACH_DELIVERY.md). Merged into clean 7-step 7-Sep. HANDS-ON (Java idempotency) = end me.
> Problem (1 line): User A -> User B/merchant paisa bheje, RELIABLY. Concrete: Arpan -> merchant Rs.500.

---

## STEP 1 — REQUIREMENTS (paisa = sacred)

```
FUNCTIONAL:  User A -> User B/merchant paisa bheje. RELIABLY.
NON-FUNCTIONAL (paisa ke 4 rule):
   x KHO na sake       (debit hua, credit nahi -> paisa gayab)
   x ATAK na sake      (network mara beech -> paisa kahan?)
   x DUPLICATE na ho   (double-charge -> Rs.1000)
   ok TRACEABLE        (audit)
INTERVIEW-WORDS: atomicity · idempotency · strong-consistency · durability.
# ye 4 constraint = 4 HARD problems: idempotency . consistency . failure-handling . ledger/audit.
```

---

## STEP 2 — ESTIMATE (★ payment ka asli insight)

```
   ~100M users. Txns: avg low-hundreds/sec, peak few-thousand/sec (sale/festival).
   POINT: bhale hi throughput UPI-scale ho -> design CORRECTNESS-dominated hai (idempotency + ACID + ledger), na raw-throughput.
   ★ KEY LINE: "Payment high-THROUGHPUT problem nahi, high-CORRECTNESS problem hai —
                scale se pehle ATOMIC + IDEMPOTENT + LEDGER pe focus."
   -> DB choice yahin: SQL/RDBMS (ACID, strong-consistency) + append-only LEDGER (audit). NoSQL nahi (money = strong + multi-row txn).
```

---

## STEP 3 — API (idempotency-key = STAR)

```
   POST /pay { from, to, amount, Idempotency-Key (UUID) } -> { status, txn-id }
   # client HAR naye payment ke liye UNIQUE key banata; RETRY pe WAHI key.
```

---

## STEP 4 — DATA MODEL

```
   LEDGER (double-entry):  har txn = 1 debit + 1 credit (barabar, saath). IMMUTABLE (delete/edit nahi).
   IDEMPOTENCY register:   key -> { status, result }   (TTL ~24h, delete MAT karo)
   TXN STATUS (durable, write-ahead):  INITIATED -> PENDING -> SUCCESS / FAILED
   INVARIANT:  sum(debits) == sum(credits)  HAMESHA. (match na kare -> paisa kho/ban gaya -> turant pakad.)
```

---

## STEP 5 — HL BOXES (request kahan-kahan guzarti)

```
   User App -> [LB / API Gateway] -> Payment Service -> [Idempotency check]
             -> DB (ledger debit+credit, txn = PENDING)
             -> ★ external PSP / GATEWAY (Razorpay/Stripe)  <- ASLI paisa YAHI move karta, TERA server nahi
             -> PSP result (success/fail) -> ledger + status update -> response
   STATE-MACHINE:  INITIATED -> PENDING (PSP ko bheja) -> SUCCESS / FAILED.
   # PSP-call EXTERNAL + ASYNC -> isliye PENDING state + failure-handling (STEP 6).
   # naive flow simple (App->Service->Ledger), PAR crash/retry/network -> 4 hard problem.
```

---

## STEP 6 — DEEP DIVE: 4 HARD PROBLEMS (asli khel)

```
(1) IDEMPOTENCY (double-charge na ho):
    Arpan 2 baar tap / app-retry -> same payment 2x server pe -> bina protection Rs.1000.
    FIX: register me key hai? NAHI -> process (Rs.500 move) + key+result STORE. HAAN -> process mat karo, stored result WAPAS (block nahi = DEDUP).
    RACE: 2 req ek-saath same key -> dono "naya" samjhe -> double. fix -> check+store ATOMIC:
          DB UNIQUE-CONSTRAINT (durable, best; insert = auto-lock) + Redis SETNX (fast in-flight layer). key STATUS: IN_PROGRESS/DONE.
    TRAP: done pe key DELETE mat karo -> late-retry (30s baad) "naya" samjhe -> dobara charge. -> TTL(24h)/result store.
    CONFUSION-CLEAR: genuine 2 payment (100+100) -> client ALAG key -> dono process (200). retry = WAHI key -> ek baar.
          -> idempotency sirf WAHI-attempt duplicate rokti, alag-genuine payment (alag key) NAHI. duplicate pe REJECT nahi -> SAVED result wapas.
    DSA-connect: idempotency-register = hashmap "pehle dekha?" = CONTAINS-DUPLICATE.
    LINE: "same key, same outcome — chahe kitni baar aaye, paisa ek baar. (ALAG intent = ALAG key = alag payment.)"

(2) CONSISTENCY (debit+credit dono ya koi = atomicity):
    Arpan -500 -> crash -> merchant +500 nahi -> Rs.500 GAYAB.
    SEE-SAW: debit+credit dono sire SAATH hilte; paisa banta/marta nahi, sirf MOVE (conserved). INVARIANT sum(debit)=sum(credit).
    FIX depends accounts kahan:
      SAME DB   -> DB TRANSACTION (ACID): BEGIN -> Arpan-=500; Merchant+=500 -> COMMIT (dono ya ROLLBACK). ACID 'A' = both-or-neither.
      ALAG BANKS-> DISTRIBUTED (ek DB txn nahi): SAGA ya 2-Phase-Commit.
    LINE: "money movement = ek ATOMIC flip (both-or-neither), paisa conserved."

(3) SAGA (distributed rollback): single-DB me rollback FREE (ACID 'A'); alag-DBs me nahi -> KHUD ka undo (COMPENSATING) likho.
    TRIP-analogy: Flight✓ Hotel✓ Cab✗ -> reverse-order undo: cancel-hotel -> cancel-flight -> clean.
    Payment: BankA Arpan-500✓ -> BankB merchant+500✗ FAIL -> compensate: BankA Arpan+500 WAPAS.
    (rollback DB nahi karta -> HAMARA code compensating step.) trade-off: SAGA = loose/scalable PAR EVENTUAL (beech thodi inconsistency).

(3b) 2PC (2-Phase Commit): coordinator sabko manage.
    PHASE-1 PREPARE: coordinator sab (BankA,BankB) se "ready to commit?" -> resource LOCK -> "YES"/"NO".
    PHASE-2 COMMIT/ABORT: sab-YES -> COMMIT | koi-NO -> ABORT (rollback).
    DIKKAT: participants LOCK pakde rehte jab tak coordinator na bole; coordinator CRASH -> participants ATKE (system thapp) -> scale pe SAGA preferred.
    2PC vs SAGA: 2PC=strict/sync/LOCKING (strong, SLOW/blocking) | SAGA=async compensating-undo (eventual, scalable, no-lock).

(4) FAILURE-HANDLING (crash -> paisa kahan?):
    HARD TRUTH: crash ke baad pata nahi debit/credit hua ya sirf RESPONSE kho gaya -> "bas rollback" SAFE nahi (record+resolve).
    FIX (kabhi assume nahi -> RECORD + RESOLVE):
      1) STATUS tracking (write-ahead): karne se PEHLE "PENDING" likho (durable) -> crash pe "ye pending thi" pata -> resolve. kabhi GUM nahi.
      2) RECONCILIATION (pull/batch): pending dhoondho -> bank se poocho "hui?" -> success | retry(idempotency-safe) | FAILED+refund.
      2b) WEBHOOK (gateway PUSH): gateway KHUD call-back complete pe -> turant status update. = PUSH(webhook,instant) + PULL(reconcile,safety-net) DONO.
      3) idempotency+retry (double-process safe).
    COURIER-analogy: har parcel = tracking# + status; courier gira -> parcel gum nahi -> status dekho -> resolve.
    LINE: "kabhi assume nahi. Har payment ka STATUS record. Crash -> reconcile. Paisa hamesha done/undone/being-checked, kabhi gum nahi."
```

---

## STEP 7 — BOTTLENECK / LEDGER / SCALE

```
LEDGER / AUDIT (finance ka DIL): har txn PERMANENT, IMMUTABLE record (delete/edit nahi).
   DOUBLE-ENTRY (500-saal purana): har txn = 1 debit + 1 credit (barabar, saath). sum(debits)=sum(credits) hamesha.
   PERMANENT-DIARY: pen se likha (mit nahi); galti -> NAYI correction-entry (purani mitao mat) -> history bachi = audit-trail.
   regulators (RBI/SEC) maangte: har paisa kab-kahan-kyun traceable.

DISTRIBUTED-TXN = payment ka ASLI bottleneck: single-DB tak rollback FREE (ACID). services/DB badhne pe (BankA/BankB alag) ->
   ek DB txn possible nahi -> SAGA (compensating, reverse-order undo) [2PC bhi option par strict/slow/locking].
LOAD badhe -> LB + multiple Payment-Service instances (stateless) ; DB read-replica / shard-by-account.
TRADE-OFF (JP sunna chahta): 2PC = strong/sync/locking (slow) | SAGA = async/eventual (scalable, no long-lock).

WRAP (ek line each):
   idempotency  -> same key, paisa EK baar (hashmap "pehle dekha?").
   consistency  -> debit+credit ek atomic FLIP (see-saw; ACID / SAGA).
   failure      -> status record + webhook + RECONCILE (courier-tracking, kabhi gum nahi).
   ledger/audit -> immutable double-entry, debits=credits (permanent diary).
IMPROVE: fraud-checks, rate-limit, multi-currency, chargebacks.
```

---

## ★ REAL-WORLD ANCHOR — SCAM 1992 (Harshad Mehta): ye 4 control KYUN bane
> (Arpan ne SCAM 1992 dekhte-dekhte KHUD connect kiya — ledger concept ka asli WEIGHT yahin.)
```
   1990-91: computer nahi -> PAPER ledger + manual "Bank Receipt (BR)" -> editable, forgeable, no real-time cross-check.
   SCAM MECHANISM: FAKE/khaali BR (unbacked entry — securities the hi nahi) + bank-settlement "float" me stock-market me laga diya
                   + koi real-time verify/reconciliation nahi -> mahino chala, late pakda (~Rs.4000cr).
   ★ HUMARE 4 CONTROL = THEEK ISKI GAIRHAAZRI ka fix:
     fake/unbacked entry        -> INVARIANT sum(debit)=sum(credit) + backed-only (unbacked impossible)
     paisa-gaya-securities-nahi -> ATOMIC settlement (both-or-neither; ACID/SAGA)
     manual-editable BR         -> IMMUTABLE double-entry LEDGER + AUDIT-TRAIL (tamper-proof)
     float/gap exploit, late    -> real-time RECONCILIATION (mismatch turant)
   PAPER->DIGITAL: ledger MARA nahi, kaagaz-form mara. concept zinda (DB, immutable, ACID). BLOCKCHAIN = tamper-proof-ledger ki extreme.
   LINE: "Ledger immutable + reconciliation kyun? — Scam 1992 exactly inki gairhaazri thi: manual editable BRs + no real-time verify.
          India ka electronic-settlement / NSDL-demat / T+1 isi ke baad aaya."
```

---

## ═══ HANDS-ON — Idempotency LIVE (usercrud, khud kiya, 27-Aug) ═══

> IdempotencyController: /pay POST. ConcurrentHashMap = key->result store. AtomicInteger = asli order counter (proof).
> Core (Arpan ki line): "map me key hai? -> kuch mat karo, wahi wapas. Nahi? -> process + map me daal do." Bas.

### Controller (FINAL — atomic, concurrent-safe)
```java
@RestController
public class IdempotencyController {

    // key -> pehle jo result diya (duplicate pe wahi wapas)
    private final ConcurrentHashMap<String, String> processed = new ConcurrentHashMap<>();
    // asli orders ka counter (SAABIT karega double hua ya nahi)
    private final AtomicInteger orderCounter = new AtomicInteger(0);

    @PostMapping("/pay")
    public String pay(@RequestHeader("Idempotency-Key") String key,
                      @RequestParam int amount) {

        // ATOMIC claim: key hai? -> existing wapas | nahi? -> "PROCESSING" daal + null return
        String existing = processed.putIfAbsent(key, "PROCESSING");
        if (existing != null) {
            return "DUPLICATE (key already claimed) -> " + existing
                 + "  | total orders = " + orderCounter.get();
        }

        // yahan sirf EK thread pahunchega (jisne putIfAbsent race JEETI)
        int orderId = orderCounter.incrementAndGet();
        String result = "Order #" + orderId + " created, amount=" + amount;

        processed.put(key, result);   // "PROCESSING" ko asli result se replace

        return "OK -> " + result + "  | total orders = " + orderCounter.get();
    }
}
```
+ SecurityConfig: `.requestMatchers("/pay").permitAll()`

### Sequential test (3 curl)
```
A: POST /pay?amount=100  header Idempotency-Key: abc-123  -> OK Order #1 | total orders = 1
B: SAME dobara (retry)                                    -> DUPLICATE Order #1 | total orders = 1   <- 2 NAHI
C: POST /pay?amount=200  header Idempotency-Key: xyz-999  -> OK Order #2 | total orders = 2
```

### ★★ RACE + ATOMIC (interview-gold) ★★
```
NAIVE galti = check-then-put (2 alag steps, GAP):
   if(containsKey) return stored;  ... process ... put();
   Thread A: "nahi"  |  Thread B bhi: "nahi"  -> dono gap me ghus ke DONO order banaye = DOUBLE!

FIX = putIfAbsent = 1 ATOMIC step:
   "key nahi? -> daal + null. Hai? -> existing return (overwrite NAHI)."  koi gap nahi -> sirf EK jeet-ta.
```
★ SHART: atomicity `putIfAbsent` NAAM se nahi, **ConcurrentHashMap** se aati.
   ConcurrentHashMap.putIfAbsent = ATOMIC (bucket lock/CAS). plain HashMap ka = NOT thread-safe. => isiliye ConcurrentHashMap.

### LIVE before/after — 20 concurrent requests, SAME key "race-1"
```
BROKEN (containsKey+put + Thread.sleep(50) gap-widen):
   same key pe -> Order #1 AUR Order #2 bane -> total orders = 2   <- DOUBLE-CHARGE live
ATOMIC (putIfAbsent):
   same key pe -> sirf 1 "OK Order #1", baaki 19 DUPLICATE -> total orders = 1   <- SAFE
   (pehli DUPLICATE line "-> PROCESSING" = duplicate winner ke process-karte-waqt = reserve-then-fill live)
```
20 parallel (PowerShell): `1..20 | ForEach-Object { Start-Job { curl.exe -s -X POST ".../pay?amount=100" -H "Idempotency-Key: race-1" } } | Wait-Job | Receive-Job`

### GEMS + INTERVIEW LINE
```
1. Idempotency = same op 2x -> effect 1x. Client Idempotency-Key(UUID) -> server: seen? stored : process+store.
2. Naive check-then-put = RACE. Fix = ATOMIC putIfAbsent ON ConcurrentHashMap.
3. Real store = Redis / DB unique-constraint (in-memory restart-pe-udd) + key TTL. Reserve-then-fill (PROCESSING placeholder).
INTERVIEW: "Idempotency-Key client bhejta; server dekhta already-processed. Naive check-then-put race-prone (concurrent gap me double).
 Fix = atomic putIfAbsent on ConcurrentHashMap -- check+insert ek indivisible step, ek jeet-ta. Prod store = Redis/DB unique-constraint + TTL."
```

### ★ KEY KAISE BANTI — "TAP vs RETRY" (28-Aug, confusion clear)
```
CONFUSION: "main 100 abhi bheju, agle sec 100 phir -> idempotency kyu nahi lagti? app do alag key kaise banati?"
CRUX: Idempotency-Key = ek RANDOM number. App har "Pay" TAP pe NAYA random (dice-roll). naya tap = naya dice. Key banti REQUEST-BANANE (button TAP) ke pal, bhejne pe nahi.
  (A) Do baar TU tap (do genuine payment): tap1->555->100, tap2->888->100 = 200 ✓. Number ALAG -> idempotency CHUP.
  (B) Ek tap + RETRY (network hang): tap1->555 bheja, jawab nahi -> phone/network KHUD dobara WAHI 555 -> server "555 aa chuka" BLOCK -> 100 ✓.
ASLI FARAK = KAUN dobara bhej raha: TU tap -> naya key -> naya payment | SYSTEM auto-retry -> same key -> blocked. retry me button nahi dabate.
MAQSAD: do-baar-pay se ROKNA nahi; sirf network-retry se ek-payment do-baar-count na ho.
CHEQUE ANALOGY: key = cheque-number. Do alag cheque (alag number) 100+100 -> bank dono cash = 200. Ek hi cheque-number do baar -> reject.
   Bank AMOUNT nahi, NUMBER dekhta duplicate-check ko. Waise hi Idempotency-Key.
```

---

[← HLD README](../README.md)
