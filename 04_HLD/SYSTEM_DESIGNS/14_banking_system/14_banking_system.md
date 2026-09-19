# Mini Banking System (accounts · transactions · balances) — POORA ROUND (4 MOVE)

> **NAV** — ARCHETYPE C · DIL: **ledger sach hai, balance sirf nateeja.** UP: [MASTER](../../00_MASTER_SHEET.md) ·
> CONCEPTS: [db-what-when](../../FOUNDATIONS/09_databases_what_when.md) · [CAP](../../FOUNDATIONS/08_cap_theorem.md) ·
> [saga/ms-comm](../../FOUNDATIONS/10_ms_communication.md) · [caching](../../FOUNDATIONS/04_caching.md) ·
> saath: [payment-system](../07_payment_system/07_payment_system.md) (iska bada bhai) · [stock-broker](../06_stock_broker_trading/06_stock_broker_trading.md)

> **KYUN YE DESIGN:** 19-Sep — Arpan ne ek reel se ye sawaal laaya, maine net pe verify kiya.
> Shakal confirm hui: candidates report karte hain — payment flow · **transaction ledger** ·
> ATM transactions · **do account ke beech internal transfer** (laakhon/din, sakht correctness, poora audit).
> Ye JP ka sabse sambhavit HLD sawaal hai, aur Arpan ki sabse majboot zameen bhi.
> (detail: `JP_INTERVIEW_INTEL.md` section 0e)
>
> Problem (1 line): ek bank ke andar accounts, un pe paisa daalo/nikaalo/bhejo, aur balance bhi dikhao.

```
★★ PAANCH CHEEZ JINPE YE ROUND TAY HOTA (source ka shabd: "ye interview INHI se tay hote hain")
      IDEMPOTENCY · TRANSACTIONS · QUEUES · RECONCILIATION · AUDIT
   + ACID vs eventual consistency  (partition me CORRECTNESS chunna, availability ki keemat pe)
   + core ledger ke liye RELATIONAL DB preferred -- NoSQL ko NAHI
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
   TU: "Banking system bahut bada hai. Pehle use-cases baandh lete hain —
        kya ye ek hi bank ke andar hai, ya dusre bank ko bhi bhejna hai?"
```

★ **Arpan ne asli mock me sabse pehla yahi poocha** — *"what will be the use cases, let's discuss use cases first"*.
Sahi pehla kadam: scope pehle baandho, warna interviewer jitna bhi bada system tumhare sar pe daal dega.

**Jawab jo mila (scope):**

```
ANDAR (karna hai):
   ek user ke ek ya kai ACCOUNT
   DEPOSIT   (paisa daalo)
   WITHDRAW  (paisa nikaalo)
   TRANSFER  (A -> B, DONO isi bank me)      <- ★ ye ek shabd poora design badal deta (neeche dekh)
   BALANCE dekho
   TRANSACTION HISTORY dekho

BAHAR (abhi nahi):
   dusre bank ko bhejna · loan · interest · card · KYC

SAKHT SHART: paisa na kabhi GUM ho, na kabhi BANE. har waqt hisaab barabar.
```

---

# MOVE 2 — DO CHHOTE BLOCK LIKHO

```
 ┌── FR (kya karega) ────────────────┐   ┌── NFR (kaisa hona chahiye) ──────────┐
 │  - Account (user ke kai ho sakte) │   │  - CORRECTNESS sabse upar             │
 │  - Deposit / Withdraw             │   │    (paisa na bane na mare)            │
 │  - Transfer A -> B (same bank)    │   │  - har paisa TRACEABLE (audit)        │
 │  - Balance dekho                  │   │  - retry pe DOBARA na ho (idempotent) │
 │  - History dekho                  │   │  - balance dekhna TEZ (sabse hot read)│
 └───────────────────────────────────┘   └───────────────────────────────────────┘
```

**NUMBER (bolo, ek faisla nikaalo, aage badho):**

```
   5 million account
   10 million transaction / din   ->  10^7 / 86400  ~=  120 write/sec
   read >> write                  ->  log balance CHECK karte hain transaction se kahin zyada

   ★ FAISLA: 120 write/sec = ek theek-thaak Postgres box ka DAS-VA hissa.
     -> SHARDING KI ZAROORAT NAHI. Single primary + read replica kaafi.
     (Arpan ne khud yahi kaha: "mere number itne bade nahi, normal DB chalega,
      replication agar zaroorat pade" — ye SAHI call hai. log yahin bina
      zaroorat sharding ghusa dete hain.)
```

**DB CHUNAV — RELATIONAL (Postgres / Oracle / MySQL). Teen wajah:**

```
   1. ACID + multi-row ATOMICITY  ->  debit aur credit ek saath, ya dono nahi
   2. ROW LOCK                     ->  ek hi account pe do transfer ek saath = sambhal jaata
   3. CONSTRAINT                   ->  balance >= 0 ko DB KHUD pakde, code ke bharose nahi
```

★ NoSQL yahan kyun nahi: multi-row atomicity aur constraint uska kaam hi nahi hai.
(source-confirmed: JP core ledger ke liye relational hi preference deta hai)

---

# MOVE 3 — BOXES BANAO (chhota banao, phir dikkat pe badhao)

**Shuruaati naksha (jaan-boojh ke seedha):**

```
   Client  ->  LOAD BALANCER  ->  [ Banking Service (business logic) ]  ->  [ SQL DB ]
```

Bas itna. Ab isme dikkat daalte hain, aur har dikkat pe ek box badhta hai.

---

### dikkat 1 — "transfer ke beech me system gir gaya: A ka -500 ho gaya, B ka +500 nahi"

★★ **Arpan ne ye niyam KHUD bola (ye is design ki REEDH hai):**

> *"transaction ek hi DB me ho to `@Transactional` use karo. Agar cross-service ho to SAGA."*

```
   DONO ACCOUNT EK HI DB ME (hamara scope)  ->  EK LOCAL TRANSACTION. Bas.

      BEGIN;
        UPDATE accounts SET balance = balance - 500 WHERE id = A;
        UPDATE accounts SET balance = balance + 500 WHERE id = B;
      COMMIT;

   -> beech me girne ki JAGAH HI NAHI. DB ya dono karega ya kuch nahi.
```

★★ **YAHI SABSE BADI GALTI HAI JO LOG KARTE HAIN:** dono account ek hi DB me hote hain,
phir bhi SAGA / queue / event-driven ghusa dete hain. Us se kya hota hai —

```
   debit aur credit ALAG ho gaye  ->  ab ye do NAYI dikkat KHUD paida ho gayi:
      "paisa kat gaya phir fail ho gaya?"
      "paisa kat gaya aur consumer down hai, juda hi nahi?"
   -> ye dikkat DESIGN ne banayi, problem ne nahi. Apne liye problem khareedna.
```

**SAGA kab lagti:** jab do side **alag maalikon** ke paas ho — alag service, alag DB, ya **dusra bank**.
Tab ek transaction chal hi nahi sakti, isliye compensating-undo likhna padta hai.
Wo case is scope se BAHAR hai (poora treatment: [payment-system](../07_payment_system/07_payment_system.md) dikkat-5).

```
   NIYAM (ek line me yaad rakho):
      ek DB      ->  @Transactional
      cross-DB / cross-service / cross-bank  ->  SAGA (compensating undo)
```

---

### dikkat 2 — "regulator poochhe: ye paisa kahan se aaya kahan gaya?" + "DB dhoka de sakta hai"

★★ **Arpan ne LOG aur LEDGER ko alag-alag bola — ye farak bahut kam log karte hain:**

```
   LOG     ->  DEBUGGING ke liye. "kya chal raha, kaise chal raha."
               rotate hota hai, delete hota hai, kisi ko farak nahi padta.

   LEDGER  ->  SACH ka record. APPEND-ONLY.
               kisne, kab, kitna, kyun. mitega nahi, badlega nahi.
               (isme ched-chaad ho hi nahi sakti — wahi uski taakat hai)
```

**LEDGER = DOUBLE-ENTRY (500 saal purana tareeka):**

```
   har transaction = DO entry, jinka jod hamesha ZERO
      A ka account   : -500   (debit)
      B ka account   : +500   (credit)
                       -----
                jod  :    0

   -> paisa na BANTA hai na MARTA hai — sirf HILTA hai (conserved).
   -> kisi bhi waqt poore ledger ka jod = bank me kul paisa. mismatch = turant pakda jaata.
```

★ **Arpan ka KAFKA wala jod (khud jodaa, aur shakal bilkul sahi hai):**

```
   Kafka   ->  append-only log, padhne se mitta nahi, sirf aage judta
   Ledger  ->  append-only, edit nahi, sirf aage judta
```

**PAR EK HADD — ye interview me phasa sakti hai:**

```
   LEDGER KA GHAR = DB, Kafka NAHI.  Do wajah:
      1. ledger pe QUERY karni padti hai ("is account ka pichhle mahine ka hisaab") — Kafka wo nahi deta
      2. ledger USI transaction me likhna hota hai jisme paisa hila — Kafka us transaction ka hissa nahi ban sakta

   KAFKA KA KAAM: COMMIT ke BAAD ki khabar bahar bhejna
      COMMIT  ->  event  ->  notification · fraud-check · analytics · statement
      (yahan eventual consistency chalti hai — SMS 2 second late aaye to koi nahi marta)
```

---

### dikkat 3 — "balance aata kahan se hai?" (is design ka ASLI sawaal)

**Do raaste hain:**

```
   (a) accounts.balance ek COLUMN ho, har transaction pe update
          + padhna sasta (1 row)
          - ye ek likha hua number hai: galat ho sakta, purana ho sakta, buggy code bigaad sakta

   (b) balance = SUM(us account ki saari ledger entries)
          + hamesha SACH (entries se jhooth bolna mushkil)
          - ★ GINTI DEKH:
               10M txn/din  ->  3 saal me ~11 ARAB ledger rows
               ek purana account  ->  5,000 - 50,000 entries
               balance dekhna SABSE ZYADA hone wala kaam hai (read >> write)
            -> har baar app kholne pe 20,000 row jodo, aur lakhon log ek saath. NAHI chalega.
```

★ Arpan ne **(b)** chuna — *"ledger main source of truth hai, DB dhoka de sakta hai"* — aur wo soch SAHI hai.
Banking ka model yahi hai: **balance ek NATEEJA hai, SACH nahi.**

★★ **ASLI JAWAB — DONO RAKHO** (ye hissa Arpan ne nahi bataya tha, yahan seekha — 19-Sep):

```
   ledger_entries    ->  SACH.  append-only. har entry ka record.
   accounts.balance  ->  CACHE. ledger ka nateeja, pehle se jod ke rakha hua.
```

**Aur asli trick — DONO EK HI TRANSACTION ME LIKHO:**

```sql
BEGIN;
  INSERT INTO ledger_entries (txn_id, account_id, amount, type, ts);   -- SACH  (A: -500)
  INSERT INTO ledger_entries (txn_id, account_id, amount, type, ts);   -- SACH  (B: +500)
  UPDATE accounts SET balance = balance - 500 WHERE id = A;            -- CACHE
  UPDATE accounts SET balance = balance + 500 WHERE id = B;            -- CACHE
COMMIT;
```

```
   -> balance dekhna = 1 ROW padhna. koi jod nahi, koi 20,000-row scan nahi.
   -> dono EK SAATH commit hote hain, isliye ALAG HO HI NAHI SAKTE.
      ("cache stale ho jaayega" wali dikkat yahan hai hi nahi — cache aur sach ek hi commit me chalte hain)
   -> agar kabhi farak aa gaya (bug, manual DB edit, migration) -> LEDGER JEETEGA.
      balance column dobara bana lenge, ledger se.
```

★ **Isi liye yahan MQ/queue NAHI lagti** — queue ka matlab hota "baad me hoga", aur baad me hone se
dono alag ho jayenge. Jo cheez ek saath honi chahiye, use alag mat karo.

**RECONCILIATION (ye hamesha iske saath jaata hai — aur source ne isi shabd ko grading-point kaha):**

```
   raat ko ek job chale:
      har account ke liye  SUM(ledger_entries)  nikaalo
      accounts.balance se milao
      farak mila  ->  ALERT
      ★ chupchap theek MAT karo — pehle pata karo KYUN hua (wo bug abhi bhi zinda hai)
```

**★ EK LINE (interview me bolne layak):**

> *"Ledger source of truth hai, balance uska derived cache. Dono ek hi transaction me likhta hoon,
> isliye kabhi alag nahi hote. Read balance se hota hai — ek row. Aur ek reconciliation job
> roz dono milata hai; farak mile to ledger jeetega."*

---

### dikkat 4 — "user ne do baar tap kar diya / client ne retry maar diya — Rs. 1000 kat gaye"

★ Arpan ne ye pain-point khud pakda tha (mock ka #1): *"idempotency — retry hua to check karke block karo."*

```
   FAISLA: IDEMPOTENCY KEY
      client har transfer ke liye ek unique key bhejta hai
      server: wo key pehle dekhi?  ->  purana result WAPAS BHEJO (naya kaam mat karo)
                        nahi dekhi?  ->  kaam karo + key record karo

   ★ SABSE ACCHA TAREEKA: DB UNIQUE CONSTRAINT us key pe.
     (insert khud lock le leta — do concurrent request me se ek hi jeetegi.
      application-level "pehle check phir insert" me race bach jaati hai.)
```

★ Dhyan: `@Transactional` idempotency ki jagah nahi leta. Transaction kehta "aadha kaam nahi hoga".
Idempotency kehti "dobara kaam nahi hoga". **Do alag cheezein, dono chahiye.**

Poora treatment + LIVE hands-on (20 concurrent request, same key):
[payment-system](../07_payment_system/07_payment_system.md) dikkat-2 aur uska HANDS-ON section.

---

### dikkat 5 — "ek hi account pe DO transfer ek saath aa gaye"

★★ **ARPAN KA PUSHBACK (19-Sep) — aur wo SAHI tha. Maine "locking chahiye" bola, usne kaata:**

> *"ye ho hi nahi sakta. hamara DB atomic constraint ke saath hai, wo do transaction ko
> ek saath modify karne hi nahi dega. race ho sakti hai baat sahi, par DB hone nahi dega."*

**Kyun wo sahi hai:**

```sql
UPDATE accounts SET balance = balance - 500 WHERE id = A;
```
```
   hisaab DB KHUD kar raha hai, us row pe jo LOCK uske paas hai.
   do transfer ek saath A ko chhuein -> DB unhe KATAAR me laga deta hai
      ek chalega, dusra ruk ke uske BAAD.
   -> LOST UPDATE ho hi nahi sakta. application-level lock ki zaroorat NAHI.
```

**Race tab hoti jab app PADH ke, JOD ke, WAPAS likhe** — aur wo humne kiya hi nahi:

```java
int bal = read(A);        // 1000
bal = bal - 500;          // 500
write(A, bal);            // 500   <- dusra bhi 1000 PADH chuka tha = lost update
```

→ **SANSHODHAN: "concurrency ke liye locking chahiye" = GALAT.** Ye baat file me thi, Arpan ne hatayi.

---

**PAR DO CHEEZ BACHTI HAI — dono chhoti, aur dono asli:**

### (a) `balance >= 0` ka CHECK KAHAN hai?

```
   A ke paas 300. do withdrawal, 200-200, ek saath.

   check APP me ho to:
      if (read(A).balance >= 200) { ... }   // DONO pass -- dono ne 300 dekha
      update(...);                          // -100

   -> dono UPDATE apni baari se chalenge (bilkul jaisa Arpan keh raha), PAR
      CHECK dono ke PEHLE ho chuka tha. Balance -100.
```

**Ilaaj wahi jo Arpan keh raha — DB se karwao, app se nahi:**

```sql
UPDATE accounts SET balance = balance - 200
WHERE id = A AND balance >= 200;
-- rows affected == 0  ->  paisa kam tha, REJECT
```
ya seedha `CHECK (balance >= 0)` constraint. **Dono me faisla DB ka, app ka nahi.**

### (b) DEADLOCK — ye atomic UPDATE ke BAAD bhi rehta hai

```
   Txn1 : A -> B     A ka lock liya, ab B ka INTEZAAR
   Txn2 : B -> A     B ka lock liya, ab A ka INTEZAAR
          -> dono ek doosre ka intezaar. koi aage nahi. CIRCULAR WAIT.
```

```
   dono UPDATE bilkul SAHI hain, ATOMIC hain -- phir bhi ATAK gaye.
   DB detect karke EK ko maar deta hai (MySQL error 1213, victim rollback).
   ★ Arpan ne ye LIVE chala ke dekha hua hai: 09_DATABASE/08_deadlock.md
```

**Ilaaj: hamesha EK TAY KRAM me lock lo** — account id sort karke chhota pehle.

```
   Txn1 : A -> B   ->  lock(A) phir lock(B)
   Txn2 : B -> A   ->  ★ lock(A) phir lock(B)   (kaam ulta hai, LOCK ka kram wahi)
   -> dono ek hi DISHA me chal rahe -> circle ban hi nahi sakta
```

---

★ **IS HISSE KA NICHOD (ek line):**

> *"Concurrency ke liye alag locking nahi chahiye — UPDATE me hisaab DB khud karta hai,
> to lost update hota hi nahi. Jo dhyan dena hai wo do cheez hai: balance ka check
> DB me ho (app me nahi), aur lock hamesha ek tay kram me liya jaaye warna deadlock."*

---

# MOVE 4 — BOLTE-BOLTE JODO

```
   ⏳ ABHI BAAKI — agli baithak me:
      ► "API kya hogi?"
      ► "History ka page kaise dikhaoge?" (pagination on 11 arab rows)
      ► "Kahan tootega / scale?"  (read replica · balance read · ledger ka size · archive)
      ► "2 din purana data chahiye vs 5 saal purana" (hot/cold, partition by month)
      ► WRAP (ek line har problem ki)
```

---

## ★ AB TAK KA NAKSHA

```
                    ┌──────────────┐
   Client  ───────► │ LOAD BALANCER│
                    └──────┬───────┘
                           │
                 ┌─────────▼──────────┐
                 │  Banking Service   │   business logic
                 │  (kai instance)    │   + idempotency key check
                 └─────────┬──────────┘
                           │  EK LOCAL TRANSACTION
                 ┌─────────▼──────────────────────────┐
                 │            SQL DB                  │
                 │  ledger_entries  (SACH, append-only)│
                 │  accounts.balance (CACHE, derived)  │
                 │  idempotency_keys (UNIQUE)          │
                 └─────────┬──────────────────────────┘
                           │  COMMIT ke BAAD
                           ▼
                    ┌─────────────┐
                    │    KAFKA    │ ──► notification · fraud-check · analytics · statement
                    └─────────────┘

      + read replica  (balance/history read)
      + raat ko RECONCILIATION job : SUM(ledger) vs accounts.balance -> mismatch = ALERT
```

---

## ★ IS MOCK KA IMAANDAR HISAAB (19-Sep)

```
   ARPAN NE KHUD NIKALA:
     use-case pehle poochna (pehla hi kadam)
     "number bade nahi -> sharding nahi, normal DB + replication" (sahi call, log yahan over-engineer karte)
     ★ "ek DB -> @Transactional, cross-service -> SAGA"  (is design ki REEDH)
     SQL kyun: ACID, consistency, joins, atomic
     LB + kai server
     ★ LOG aur LEDGER ko ALAG bolna + ledger APPEND-ONLY + "ched-chaad nahi"
     ★ ledger = source of truth (aur wajah: "DB/API dhoka de sakta hai")
     idempotency ko pain-point #1 pe rakhna
     ★ ledger ka Kafka se jod (append-only shakal)

   YAHAN SEEKHA (pehle se nahi tha):
     balance = derived CACHE, ledger ke SAATH usi transaction me likha
       (sirf ledger se SUM karoge to 20,000 row/read -> nahi chalega)
     reconciliation job = dono ko milane wala
     Kafka ki HADD: ledger ka ghar DB hai, Kafka commit ke BAAD ki khabar hai

   ★★ ARPAN NE MUJHE KAATA (aur wo sahi tha):
     maine dikkat-5 me "concurrency ke liye locking chahiye" likha tha.
     usne kaata: "DB atomic hai, do transaction ek saath modify karne hi nahi dega."
     -> SAHI. UPDATE me hisaab DB khud karta hai, lost update hota hi nahi.
        line hata di. bacha sirf: CHECK kahan lagaya + lock ka KRAM (deadlock).

   ABHI TEST HI NAHI HUA:
     API shape · pagination on 11 arab rows · hot-cold data / archive
```
