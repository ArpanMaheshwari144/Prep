# Payment System (UPI-style) — POORA ROUND (4 MOVE)

> **NAV** — ARCHETYPE C · DIL: paisa DO BAAR na kate. UP: [MASTER](../../00_MASTER_SHEET.md) · CONCEPTS: [db-what-when](../../FOUNDATIONS/09_databases_what_when.md) · [CAP](../../FOUNDATIONS/08_cap_theorem.md) · [saga/ms-comm](../../FOUNDATIONS/10_ms_communication.md) · saath: [bookmyshow](../10_bookmyshow/10_bookmyshow_INTERVIEW.md) · [stock-broker](../06_stock_broker_trading/06_stock_broker_trading.md)

> Finance HLD ka CORE. JP/GS ka favourite.
> 15-Sep: asli mock-video ke hisaab se dobara likha — koi 7-step rail nahi, sirf 4 move.
> Har jagah: **tu kya BOLTA hai · BOARD pe kya banta · FAISLA + KYUN**.
> HANDS-ON (Java idempotency, khud chalaya) = end me, poora ka poora.
>
> Problem (1 line): User A -> User B / merchant ko paisa bheje, BHAROSE ke saath.
> Concrete: Arpan -> merchant Rs. 500.

```
★★ CHAAR HARD PROBLEM jo paise ko special banate (poori file inhi ke ird-gird):
      IDEMPOTENCY  ·  CONSISTENCY  ·  FAILURE-HANDLING  ·  LEDGER / AUDIT
```

```
★★ TEEN NIYAM (poori file par lagte — [APPROACH_DELIVERY](../../HLD_APPROACH_DELIVERY.md) 5b)
   1. PERFECT design ek saath mat banao — chhote se shuru, dikkat pe badhao
   2. NUMBER ke peeche mat bhaago — bolo, ek faisla nikaalo, aage badho
   3. BOTTLENECK ratto mat — KHUD USER banke raasta chalao, khud dikh jaayega
```

---

# MOVE 1 — POOCHO (board pe abhi kuch nahi)

```
   TU: "Payment system bada hai — P2P transfer, merchant payment, refunds, settlement, fraud.
        Aap kis pe focus karwana chahenge? Main ek user se doosre tak paisa bhejne pe ja sakta hoon."

   TU: "Kuch cheezein confirm kar lun —
          - dono account EK hi bank/system me hain, ya ALAG banks me?
          - refund aur chargeback scope me hain?
          - koi external payment gateway (Razorpay/Stripe) use kar rahe hain ya khud settle karte hain?
          - audit/regulatory requirement hai?"

   ★ PEHLA sawaal poora design badal deta hai:
     ek hi DB      -> ek DB TRANSACTION kaafi (ACID)
     alag banks    -> distributed -> SAGA / 2PC ka poora khel
```

---

# MOVE 2 — DO CHHOTE BLOCK LIKHO

```
   ┌────────────────────────┐   ┌────────────────────────────────────┐
   │ Payment System         │   │ Use cases:                         │
   │   - User / Account     │   │   - A se B ko paisa bhejo          │
   │   - Transaction        │   │   - retry pe dobara na kate        │
   │   - Ledger entry       │   │   - fail ho to paisa wapas         │
   │   - Idempotency key    │   │   - har paisa traceable ho         │
   └────────────────────────┘   │                                    │
                                │ NOT in scope: fraud engine,        │
   ┌──────────────────────────┐ │   multi-currency, chargebacks      │
   │ PAISE KE CHAAR NIYAM:    │ └────────────────────────────────────┘
   │  x KHO na sake           │   (debit hua, credit nahi -> paisa gayab)
   │  x ATAK na sake          │   (network mara beech me -> paisa kahan?)
   │  x DUPLICATE na ho       │   (double-charge -> Rs. 1000)
   │  ✓ TRACEABLE ho          │   (audit)
   └──────────────────────────┘

   INTERVIEW-WORDS (ye chaar bolna): atomicity · idempotency · strong consistency · durability

   TU: "Ye chaar niyam hi mere chaar hard problems ban jaayenge —
        idempotency, consistency, failure-handling, aur ledger/audit."
```

```
   Numbers:
     - ~100 M users
     - transactions: average low-hundreds / sec, peak few-thousand / sec (sale / festival)

   ★★ ASLI INSIGHT (ye line bolni hai — yahi payment ko baaki designs se alag karti hai):
      "Payment high-THROUGHPUT problem nahi hai, high-CORRECTNESS problem hai.
       Scale se pehle ATOMIC + IDEMPOTENT + LEDGER pe focus karunga."

   FAISLA (yahin DB tay ho gaya):
      correctness chahiye  ──►  SQL / RDBMS (ACID, strong consistency)
                                + append-only LEDGER (audit ke liye)
      NoSQL nahi           ──►  paisa = strong consistency + multi-row transaction
```

---

# MOVE 3 — BOXES BANAO (chhota banao, phir dikkat pe badhao)

```
   TU: "Sabse simple cheez se shuru."

        USER ──► [ Payment Service ] ──► [ DB ]
                                          Arpan  -= 500
                                          Merchant += 500

   TU: "Kaagaz pe ye kaam kar raha hai. Ab isme crash, retry aur network daal ke dekhte hain."
```

### dikkat 1 — "beech me crash ho gaya: Arpan ka -500 hua, merchant ka +500 nahi"

```
        Arpan -500  ✓
             │
          CRASH
             │
        Merchant +500  ✗        ──►  Rs. 500 GAYAB ho gaye

   FAISLA: dono ek ATOMIC step me — DB TRANSACTION (ACID)

        BEGIN
            Arpan    -= 500
            Merchant += 500
        COMMIT            (dono, ya dono me se koi nahi -> ROLLBACK)

   ★ SEE-SAW wali soch: debit aur credit do sire hain jo SAATH hilte hain.
     Paisa na banta hai na marta — sirf MOVE hota hai (conserved).
   ★ INVARIANT: sum(debits) == sum(credits)  HAMESHA.
     Match na kare -> paisa kho ya ban gaya -> turant pakda jaana chahiye.
   ★ LINE: "Money movement ek ATOMIC flip hai — both-or-neither. Paisa conserved rehta hai."
```

### dikkat 2 — "Arpan ne do baar tap kar diya / app ne retry maar diya — Rs. 1000 kat gaye"

```
        tap 1 ──► server ──► 500 kata
        tap 2 ──► server ──► 500 aur kata        ->  DOUBLE CHARGE

   FAISLA: IDEMPOTENCY KEY

        client HAR naye payment ke liye ek UNIQUE key (UUID) banata hai
        RETRY pe WAHI key bhejta hai

        server:
            register me key hai?
                NAHI  -> process karo (Rs.500 move) + key aur RESULT store karo
                HAAN  -> process MAT karo, STORED RESULT wapas kar do
                         (block nahi karna — ye DEDUP hai, reject nahi)

   ★ RACE: do request ek saath, same key -> dono ne "naya hai" samjha -> double
        FIX: check + store EK ATOMIC step me
             DB UNIQUE CONSTRAINT   (durable, sabse accha — insert khud lock leta hai)
             + Redis SETNX          (tez, in-flight layer)
             key ka STATUS: IN_PROGRESS -> DONE

   ★ TRAP: kaam hone ke baad key DELETE MAT karo
        late retry (30 second baad) aayi -> "naya" samjh ke DOBARA charge
        -> TTL ~24h + result store karke rakho

   ★ CONFUSION CLEAR (ye poocha jaata hai):
        genuine do payment (100 + 100) -> client ALAG-ALAG key banata -> dono process honge (200) ✓
        retry -> WAHI key -> ek hi baar
        => idempotency sirf WAHI attempt ke duplicate ko rokti hai,
           alag genuine payment ko NAHI.

   ★ DSA-connect: idempotency register = hashmap "pehle dekha?" = contains-duplicate
   ★ LINE: "Same key, same outcome — chahe kitni baar aaye, paisa ek hi baar kate.
            Alag intent = alag key = alag payment."
```

### dikkat 3 — "asli paisa to hamara server hilata hi nahi"

```
   FAISLA: external PSP / GATEWAY (Razorpay / Stripe / bank rails) — ASLI paisa WAHI move karta hai

        [ Payment Service ] ──► [ PSP / Gateway ] ──► asli settlement
                │                      │
          txn = PENDING          jawab aane me time lagta hai (external + async)

   STATE MACHINE:
        INITIATED  ──►  PENDING (PSP ko bhej diya)  ──►  SUCCESS
                                                    └──►  FAILED

   TU: "PSP ka call external aur async hai — isliye ek PENDING state chahiye.
        Naive flow (App -> Service -> Ledger) tab tak simple lagta hai jab tak
        crash, retry aur network beech me na aayein."
```

### dikkat 4 — "PSP ko call kiya aur crash ho gaya — ab pata hi nahi paisa gaya ya nahi"

```
   ★ HARD TRUTH (ye bolna): crash ke baad pata nahi ki debit/credit hua tha,
     ya sirf RESPONSE kho gaya. Isliye "bas rollback kar do" SAFE NAHI hai.
     Niyam: kabhi ASSUME mat karo — RECORD karo, phir RESOLVE karo.

   TEEN cheezein saath chalengi:

     1. STATUS TRACKING (write-ahead)
          kuch karne se PEHLE "PENDING" likh do (durable)
          -> crash hua to baad me pata chalega "ye pending thi" -> resolve kar lenge
          -> koi payment GUM nahi hoti

     2. RECONCILIATION (pull / batch)
          pending dhoondho -> bank/PSP se poocho "ye hui thi?"
             hui      -> SUCCESS mark karo
             nahi hui -> retry (idempotency-safe hai)
             pakka fail -> FAILED + refund

     2b. WEBHOOK (gateway PUSH)
          gateway khud call-back karta hai kaam poora hone pe -> turant status update
          => PUSH (webhook, instant) + PULL (reconcile, safety-net) — DONO rakhne hain

     3. IDEMPOTENCY + RETRY  -> dobara process ho bhi jaaye to nuksan nahi

   ★ COURIER ANALOGY: har parcel ka tracking number + status hota hai.
     Courier wala gir gaya to parcel gum nahi hota — status dekho aur resolve karo.
   ★ LINE: "Kabhi assume nahi karta. Har payment ka STATUS record hota hai.
            Crash ho to reconcile. Paisa hamesha done, undone, ya being-checked hota hai — gum kabhi nahi."
```

### dikkat 5 — "A aur B alag-alag bank me hain — ek DB transaction possible hi nahi"

```
        [ Bank A ]  Arpan -500        [ Bank B ]  Merchant +500
             │                              │
             └──── ek hi DB transaction nahi ho sakti ────┘

   OPTION 1 — ★ SAGA (compensating transaction)

        single DB me rollback FREE milta hai (ACID ka 'A').
        Alag DBs me nahi milta -> apna UNDO khud likhna padta hai.

        TRIP ANALOGY:  Flight ✓  Hotel ✓  Cab ✗
                       -> ulte kram me undo: cancel hotel -> cancel flight -> saaf

        PAYMENT:  BankA: Arpan -500 ✓
                  BankB: Merchant +500 ✗ FAIL
                  -> COMPENSATE: BankA: Arpan +500 WAPAS

        ★ rollback DB nahi karta — HAMARA code compensating step chalata hai
        ★ trade-off: SAGA = loose + scalable, PAR EVENTUAL (beech me thodi der inconsistency)

   OPTION 2 — 2PC (Two-Phase Commit)

        PHASE 1  PREPARE : coordinator sabse poochta "commit karne ko taiyaar?"
                           -> resource LOCK -> "YES" / "NO"
        PHASE 2  COMMIT / ABORT : sab YES -> COMMIT | koi NO -> ABORT (rollback)

        DIKKAT: participants LOCK pakde baithe rehte hain jab tak coordinator na bole
                coordinator CRASH -> participants ATKE -> system thapp
        -> scale pe SAGA preferred

   ★ TRADE-OFF (JP ye sunna chahta hai):
        2PC  = strict, sync, LOCKING -> strong par SLOW / blocking
        SAGA = async compensating undo -> eventual, scalable, no long lock
```

### dikkat 6 — "regulator poochhe: ye paisa kahan se aaya aur kahan gaya?"

```
   FAISLA: LEDGER — har txn ka PERMANENT, IMMUTABLE record (delete/edit nahi)

        DOUBLE-ENTRY (500 saal purana tareeka):
             har txn = 1 DEBIT + 1 CREDIT (barabar, saath me)
             sum(debits) = sum(credits)  hamesha

        PERMANENT DIARY: pen se likha hua — mitta nahi.
             galti ho gayi -> NAYI correction entry daalo (purani mat mitao)
             -> poori history bachi rehti = audit trail

        regulators (RBI / SEC) yahi maangte hain: har paisa kab-kahan-kyun traceable ho

   DATA MODEL isi se banta hai:
        LEDGER (double-entry)   : har txn = debit + credit, IMMUTABLE
        IDEMPOTENCY register    : key -> { status, result }   (TTL ~24h, delete MAT karo)
        TXN STATUS              : INITIATED -> PENDING -> SUCCESS / FAILED (durable, write-ahead)
        INVARIANT               : sum(debits) == sum(credits)
```

### dikkat 7 — "festival aaya, 3000 txn/sec — ek Payment Service box ka CPU khatam"

```
        request queue me lag gayi -> timeout -> user ne DOBARA tap kiya
        aur wahi ek box gira to POORA payment band -- ek bhi txn nahi

        FAISLA: Payment Service pehle se STATELESS hai
                (state DB aur idempotency register me hai, box ki memory me nahi)
                -> isi liye kai instance chala sakte hain, aur aage LB

        ★ agar service stateless na hoti to LB se kuch na hota --
          yahi wajah hai ki "stateless rakho" wala faisla PEHLE liya gaya tha
```

### dikkat 8 — "merchant dashboard ki reporting query usi DB pe chal rahi hai, aur asli txn ka write ruk raha hai"

```
        FAISLA: READ REPLICA -- dashboard / report replica se padhein

        ★ par PAYMENT ka read replica se NAHI --
          balance aur txn status HAMESHA primary se
          (replica lag ek rupaye ka farak bhi dikha sakta hai, aur paise me ye chalega nahi)
```

### dikkat 9 — "ek hi DB me 50 crore txn row, likhai dheemi padne lagi"

```
        FAISLA: SHARD by account_id

        ★ aur yahin ek NAYA dard paida hota hai:
          A aur B alag shard pe hue to transfer ab ek LOCAL transaction nahi raha
          -> wapas SAGA wali baat (dikkat 5)
        ★ yaad rakhna: is design me asli bottleneck throughput nahi,
          DISTRIBUTED TRANSACTION hai
```

### ab poora naksha (jahan pahunche) + har box ka KYUN

```
        USER APP
           │
      [ LB / API GATEWAY ]
           │
      [ PAYMENT SERVICE ] ──► [ IDEMPOTENCY register ]   key -> status/result (Redis + DB unique)
           │
           ├──► [ DB (SQL) ]   ledger: debit + credit (ek transaction me), txn = PENDING
           │
           ├──► [ PSP / GATEWAY ]  Razorpay / Stripe   <- ASLI paisa yahan move hota hai
           │          │
           │       webhook  ──────► status update (SUCCESS / FAILED)
           │
           └──► [ RECONCILIATION job ]  pending dhoondho -> PSP se poocho -> resolve

      [ LEDGER (append-only, immutable) ]  audit trail — regulators ke liye

     LB/Gateway      : traffic + auth + rate-limit
     Payment Service : stateless -> scale aasan
     Idempotency reg : same key dobara aaye to dobara paisa na kate
     SQL DB          : ACID — debit+credit ek atomic flip
     PSP             : asli settlement (external, async -> isliye PENDING state)
     Webhook         : PUSH — turant status
     Reconciliation  : PULL — safety net, koi payment gum na ho
     Ledger          : immutable double-entry — audit + invariant check
```

---

# MOVE 4 — BOLTE-BOLTE JODO (jo poocha jaaye, wahi kholo)

## ► "API kya hogi?"

```
   POST /pay
        {
          from, to, amount,
          Idempotency-Key: <UUID>     <- ★ ye STAR hai
        }
   ->   { status, txn_id }

   ★ client HAR naye payment ke liye NAYI unique key banata hai;
     RETRY pe WAHI key bhejta hai.
```

## ► "Idempotency-Key banti kaise hai?" — TAP vs RETRY (28-Aug ki confusion, ab saaf)

```
   CONFUSION: "main abhi 100 bheju aur agle second phir 100 — idempotency kyun nahi lagti?
               App do alag key kaise bana leti hai?"

   CRUX: Idempotency-Key ek RANDOM number hai. App har "Pay" TAP pe NAYA random banati hai
         (dice roll). Naya tap = naya dice.
         Key REQUEST banne ke pal banti hai (button TAP pe), bhejne ke waqt nahi.

     (A) DO baar TU tap karta hai (do genuine payment):
             tap 1 -> key 555 -> 100
             tap 2 -> key 888 -> 100        = 200 ✓   (key alag -> idempotency chup rehti)

     (B) EK tap + RETRY (network hang):
             tap 1 -> key 555 bheja, jawab nahi aaya
             phone / network KHUD dobara WAHI 555 bhejta hai
             server: "555 to aa chuka" -> BLOCK -> 100 ✓

   ASLI FARAK = KAUN dobara bhej raha hai:
        TU tap karta hai      -> nayi key -> naya payment
        SYSTEM auto-retry     -> same key -> block
        (retry me tu button dobara nahi dabata)

   MAQSAD: do baar pay karne se ROKNA nahi hai;
           sirf network-retry ki wajah se ek payment do baar count na ho.

   ★ CHEQUE ANALOGY: key = cheque number.
     Do alag cheque (alag number) 100+100 -> bank dono cash karega = 200.
     Ek hi cheque number do baar -> reject.
     Bank AMOUNT nahi dekhta duplicate ke liye, NUMBER dekhta hai. Waise hi Idempotency-Key.
```

## ► "Kahan tootega / scale?"

```
   ★ RATTO MAT — paise ka raasta chalao:

      user ne Pay dabaya
          │
          ├─► gateway      -> abuse / flood            -> rate limiting
          ├─► service      -> stateless hai            -> instance badha do
          ├─► idempotency  -> do request ek saath      -> atomic (unique constraint / SETNX)
          ├─► DB           -> debit+credit atomic?     -> ek transaction (ACID)
          │                   read load                -> read replica . shard by account
          ├─► PSP          -> slow / down / timeout    -> PENDING + retry + circuit breaker
          │                   jawab hi nahi aaya       -> webhook + RECONCILE
          └─► ledger       -> saal bhar ka data        -> append-only, purana cold storage me

      ★ ASLI BOTTLENECK: distributed transaction (alag banks/DB) — yahi payment ka sabse
        mushkil hissa hai, raw throughput nahi.
```

## ► WRAP (ek line har problem ki)

```
   idempotency  -> same key, paisa EK baar        (hashmap "pehle dekha?")
   consistency  -> debit + credit ek atomic FLIP  (see-saw; ACID ya SAGA)
   failure      -> status record + webhook + RECONCILE  (courier tracking — gum kabhi nahi)
   ledger/audit -> immutable double-entry, debits = credits  (permanent diary)

   AAGE badhata to: fraud checks, rate limiting, multi-currency, chargebacks.
```

---

## ★ REAL-WORLD ANCHOR — SCAM 1992 (Harshad Mehta): ye 4 control KYUN bane

> (Arpan ne SCAM 1992 dekhte-dekhte KHUD connect kiya — ledger ka asli WEIGHT yahin samajh aata hai.)

```
   1990-91: computer nahi the -> PAPER ledger + manual "Bank Receipt (BR)"
            -> editable, forge ho sakte the, koi real-time cross-check nahi

   SCAM KA TAREEKA: FAKE / khaali BR (unbacked entry — securities the hi nahi)
                    + bank settlement ke "float" ko stock market me laga diya
                    + koi real-time verify / reconciliation nahi
                    -> mahino chalta raha, der se pakda gaya (~Rs. 4000 cr)

   ★ HUMARE CHAAR CONTROL = THEEK ISI GAIRHAAZRI ka fix:

     fake / unbacked entry          -> INVARIANT sum(debit)=sum(credit) + backed-only
                                       (unbacked entry possible hi nahi)
     paisa gaya, securities nahi    -> ATOMIC settlement (both-or-neither; ACID / SAGA)
     manual editable BR             -> IMMUTABLE double-entry LEDGER + AUDIT TRAIL (tamper-proof)
     float ka gap, der se pakad     -> real-time RECONCILIATION (mismatch turant dikhe)

   PAPER -> DIGITAL: ledger MARA nahi — kaagaz wala roop mara.
                     Concept zinda hai (DB, immutable, ACID).
                     BLOCKCHAIN = tamper-proof ledger ka extreme roop.

   ★ LINE: "Ledger immutable aur reconciliation kyun? — Scam 1992 me theek inhi ki gairhaazri thi:
            manual editable BRs aur koi real-time verify nahi.
            India ka electronic settlement / NSDL demat / T+1 isi ke baad aaya."
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

---

[← MASTER SHEET](../../00_MASTER_SHEET.md)
