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

**DATA SE JO TEEN SAWAAL KHADE HOTE HAIN (jawab abhi nahi — MOVE 3 me):**

```
   1. ek transfer me DO row badalti hain     ->  dono badlein ya koi nahi. Kaun sambhalega?
   2. ek hi account pe do transfer ek saath  ->  kaun rokega?
   3. balance kabhi -ve nahi hona chahiye    ->  ye niyam kahan likha jaayega?
```

★ DB kaunsa lenge ye ABHI nahi bol raha -- wo MOVE 3 me tay hoga, jab ye teeno
  sach me TOOTENGE. Board pe abhi sirf sawaal likhe hain.

---

# MOVE 3 — BOXES BANAO (chhota banao, phir dikkat pe badhao)

**Shuruaati naksha (jaan-boojh ke seedha):**

```
   Client  ->  [ Banking Service (business logic) ]  ->  [ SQL DB ]
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

★★ **AUR YAHIN DB KA FAISLA HO GAYA — RELATIONAL (Postgres / Oracle / MySQL):**

```
   abhi jo BEGIN..COMMIT likha, wo poora hi ACID ka wada hai --
   "do row badlein ya koi nahi" wala kaam DB khud karta hai.

   NoSQL me ye kamzor / seemit hai (MongoDB 4.0+ aur DynamoDB TransactWriteItems me hai, par
   constraints + joins ke saath relational me native hai).
   Wahan ye poora sambhalna APP ko padta -- yaani wahi SAGA, bina zaroorat ke.

   -> MOVE 2 ka SAWAAL 1 yahan JAWAB paa gaya.
```
(source-confirmed: JP core ledger ke liye relational hi preference deta hai)

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

   DIKKAT (chhoti): transfer ke baad SMS · fraud-check · statement bhi chahiye —
                   transfer ko inke liye ROKNA nahi, aur inme se koi gira to transfer na gire
   KAFKA KA KAAM: COMMIT ke BAAD ki khabar bahar bhejna
      ★ JAAL: COMMIT hua aur Kafka bhejne se pehle app gira -> event GAYAB.
        ilaaj = OUTBOX: event ko USI transaction me outbox table me likho, alag process bheje.
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
               10M txn/din  ->  3 saal me ~11 ARAB txn = double-entry se ~22 ARAB ledger rows
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

★ **MOVE 2 ka SAWAAL 3 yahan band hua:** "balance -ve na ho" ka niyam CODE me nahi,
DB me likha jaata hai -- kyunki code ke kai raaste ho sakte hain (API, batch, kisi ka
manual update), par DB ek hi hai. Aur constraint bhi relational ki hi den hai.

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

### dikkat 6 — "salary day: ek Banking Service box bhara, aur wahi gira to poora bank band"

```
   FAISLA: kai Banking Service instance + aage LOAD BALANCER
           service STATELESS hai (sab DB me) -> koi bhi box koi bhi request le
```

### dikkat 7 — "log balance / history baar-baar dekh rahe — sab padhai PRIMARY pe, transfer dheeme"

```
   FAISLA: READ REPLICA — balance / history ka read replica se, write primary pe
   ★ JAAL: apna abhi-kiya transfer PRIMARY se padho (replica thoda peeche ho sakti)
           warna user ko "paisa gaya hi nahi" dikhega -> dobara bhejega
   (shard abhi NAHI — MOVE 2 ka hisaab: single primary + replica kaafi)
```

---

# MOVE 4 — BOLTE-BOLTE JODO (jo poocha jaaye, wahi kholo)

## ► "API kya hogi?"

★ Arpan: *"ye to aasan hai, isme itni badi dikkat nahi hai."* — **sahi hai, yahan ruko mat.**

```
GET   /accounts/{id}/balance
GET   /accounts/{id}/transactions?cursor=...&limit=20     <- ismein pench hai (neeche)
POST  /transfers                  { from, to, amount }
POST  /accounts/{id}/deposit      { amount }
POST  /accounts/{id}/withdraw     { amount }
```

Ek hi cheez dhyan dene layak: **har POST me `Idempotency-Key` header jaata hai.** Baaki kuch nahi.

---

## ► "11 arab row pe history ka page kaise dikhaoge?"

**Seedha jawab jo log dete hain:**

```sql
SELECT * FROM ledger_entries WHERE account_id = ?
ORDER BY ts DESC LIMIT 20 OFFSET 100000;
```

★ Arpan ne OFFSET ka matlab sahi bola: *"skip karna — pehle page pe 0, agle pe 10 skip, aise chalta rahega."*
**Jo chhoot raha tha: DB skip KARTA KAISE hai.** Wo aage KOODTA nahi — **padhta hai, phir phenkta hai:**

```
OFFSET 100000 LIMIT 20

   row 1      padho -> phenko
   row 2      padho -> phenko
   ...
   row 100000 padho -> phenko
   row 100001 padho -> RAKHO   ... (20 tak)

   = 1,00,020 row ka KAAM, 20 row dene ke liye
```

```
   -> page 1 turant, page 5000 pe request ATAK jaati. jitna aage, utna DHEEMA.
   -> aur ek chhupi dikkat: page-1 dekhte waqt nayi transaction aa gayi
      -> saari row khisak gayi -> page-2 pe WAHI entry DOBARA dikhegi
         (offset GINTI se chalta hai, aur ginti hil gayi)
```

**ILAAJ — offset ki jagah "kahan chhoda tha" yaad rakho (CURSOR / KEYSET):**

```sql
-- page 1
SELECT * FROM ledger_entries WHERE account_id = ?
ORDER BY ts DESC, id DESC LIMIT 20;
-- aakhri row ka (ts, id) yaad rakho -> CURSOR banake client ko bhej do

-- page 2  (skip NAHI -- SEEDHA wahan se)
SELECT * FROM ledger_entries WHERE account_id = ?
  AND (ts, id) < (:last_ts, :last_id)
ORDER BY ts DESC, id DESC LIMIT 20;
```

```
   -> index (account_id, ts DESC, id) pe DB SEEDHA us jagah koodta hai, 20 row uthata hai
   -> page 1 ho ya page 5000 -- KHARCHA WAHI
   -> nayi transaction se kuch khiskta nahi (hum GINTI nahi, JAGAH yaad rakh rahe hain)
```

★★ **Arpan ka anchor (khud joda):** *"ye cursor based cheez ho gayi, jaise YouTube karta hai."*

```
   PEHCHAN: jahan PAGE NUMBER nahi dikhte, sirf SCROLL chalta hai -- wahan CURSOR hai.
            YouTube · Instagram · Twitter -- kisi me "page 5000 pe jao" nahi hota.
            ★ wo missing feature NAHI, wo FAISLA hai: page-number chhoda taaki har page EK JAISA tez rahe.

   ULTA:    Google search me page number DIKHTE hain -- kyunki wahan top ~1000 se aage jaane hi nahi dete.
            HADD laga di, to offset chal jaata hai.
```

**KEEMAT:** "page 500 pe seedha jao" ab nahi ho sakta — sirf agla/pichhla.
Bank statement ke liye ye theek hai (log scroll karte hain, ya DATE se filter karte hain).
Admin panel ke liye offset chalega — wahan data chhota hota hai.

★ **Aur ek cheez jiske bina DONO me se kuch nahi chalega:** index `(account_id, ts DESC, id)`.
Wo na ho to 11 arab ka scan, aur baat yahin khatam.

---

## ► "5 saal purana data kahan rakhoge?"

```
   pichhle 3 mahine   ->  log ROZ dekhte hain
   3 saal purana      ->  saal me shayad ek baar, ya regulator maange tab

   ek hi table me sab pada hai -- garam bhi, thanda bhi.
   har query, har index, har backup us 11 arab ke bojh ke saath chal raha hai.
```

★ **Arpan ka jawab:** *"purana data DB se hatao, archive kar do — on-demand wapas aa sakta.
DELETE nahi kar sakte, wo galat hoga."*

**DELETE waali baat sabse zaroori thi — bank me delete hota hi nahi. Kanoon saalon tak rakhne ko kehta hai.**

### (a) "Hatao" kaise? — `DELETE` se NAHI

```
   30 crore row ek-ek karke DELETE  ->  table lock · transaction log full · ghanton ka kaam

   ILAAJ: table ko MAHINE-MAHINE me baanto (PARTITION)

      ledger_2026_07   ledger_2026_08   ledger_2026_09  ...

      archive = us mahine ki partition DETACH kar do   (meta-data operation -- TURANT)
                file uthao -> cold storage -> partition drop
```

```
   DELETE          ->  ROW-BY-ROW kaam karta
   DETACH PARTITION->  poore TUKDE pe ek nishaan hata deta
   -> aasmaan-zameen ka farak
```

**BONUS:** partition se query bhi tez — "pichhle 3 mahine" poochha to DB baaki 33 partition ko **chhuta hi nahi**.

### (b) Archive rakha kahan

S3 (Parquet file) ya ek alag **cold DB**. On-demand wahin se padho (Athena jaisi cheez se, ya restore karke).
Sasta, aur mukhya DB pe koi bojh nahi.

### (c) ★★ EK CHEEZ JO SAATH ME TOOTEGI — aur ye is design se hi judi hai

```
   humne kaha tha: reconciliation roz  SUM(ledger)  nikaal ke  accounts.balance  se milayega.

   ab 3 saal purani entries ARCHIVE ho gayin
      -> SUM(ledger) ab ADHOORA hai (purani entries usme hain hi nahi)
      -> reconciliation HAR account pe mismatch dikhayega
```

**ILAAJ — har period ka OPENING BALANCE snapshot rakho:**

```
   account_balance_snapshot (account_id, period_end, balance)

      31-Mar-2023 ko A ka balance = 45,000    <- ye HAMESHA rahega, kabhi archive nahi hoga

   ab reconciliation:
      snapshot ka balance  +  uske BAAD wali (live) entries ka jod   =   aaj ka balance
```

★ Ab purani entries archive ho sakti hain aur hisaab phir bhi barabar rehta hai.
**Yahi cheez bank statement me bhi dikhti** — har statement ke upar *"opening balance"* likha hota hai. Isi wajah se.

---

## ► "Kahan tootega / aur bada ho gaya to?"

★ Arpan: *"dabba bada hua to shard + replication — wo to har design me ho gaya."*

```
   1. READ REPLICA          <- pehla kadam. read >> write hai, aur balance/history READ hain.
                               write primary pe, read replica pe.
                               ★ keemat: replication lag -- transfer ke turant baad balance
                                  replica se padha to purana dikh sakta
                                  -> "apna abhi kiya hua transaction" PRIMARY se padho (read-your-own-writes)

   2. PARTITION by month    <- upar wala. archive + query dono ka fayda.

   3. ★ SHARD = AAKHRI raasta, aur yahan uski KEEMAT badi hai:

         shard by account_id  ->  A shard-1 pe, B shard-2 pe
         -> transfer ab EK LOCAL TRANSACTION NAHI RAHA
         -> wapas SAGA / 2PC ki duniya me ghus gaye
         -> yaani dikkat-1 wali saari problem KHUD SE wapas bula li

      ★ isi liye hamare number pe (120 write/sec) sharding ka sawaal hi nahi uthta.
        pehle vertical (bada box) + replica. shard tabhi jab ek machine sach me chuk jaaye.
```

---

## ► WRAP (ek line har problem ki)

```
   beech me crash               ->  ek local transaction (@Transactional). ek DB hai, SAGA ki zaroorat nahi.
   dobara tap / retry           ->  Idempotency-Key + DB UNIQUE constraint
   "paisa kahan se kahan gaya"  ->  append-only LEDGER, double-entry (jod hamesha zero)
   balance dekhna tez           ->  accounts.balance = derived CACHE, ledger ke SAATH usi txn me likha
   cache aur sach alag ho gaye  ->  raat ka RECONCILIATION job. farak mile to LEDGER jeetega.
   overdraft / -ve balance      ->  check DB me (WHERE balance >= x / CHECK constraint), app me nahi
   do transfer ulte kram me     ->  lock hamesha TAY KRAM me (account id sort) -> deadlock nahi
   11 arab row pe history       ->  CURSOR pagination + index (account_id, ts DESC, id)
   purana data                  ->  month PARTITION -> DETACH -> cold storage. DELETE kabhi nahi.
   archive ke baad hisaab       ->  OPENING BALANCE snapshot (statement ke upar wali line)
   load badha                   ->  read replica -> partition -> (aakhri me) shard
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

      + read replica  (balance/history read)  -- apna abhi-kiya txn PRIMARY se padho
      + raat ko RECONCILIATION job : snapshot + live entries  vs  accounts.balance -> mismatch = ALERT
      + ledger_entries MONTH-wise partition  -> purana DETACH -> S3 / cold DB (delete KABHI nahi)
      + account_balance_snapshot (period_end ka balance) -- archive ke baad bhi hisaab barabar
```

---
