# Mini Banking System (accounts · transactions · balances)

> Ek bank ke andar accounts, un pe paisa daalo / nikaalo / bhejo, aur balance dikhao.
> Is design ka dil: **ledger SACH hai, balance sirf NATEEJA** + paisa na bane na mare.
> KYUN YE DESIGN (19-Sep): Arpan ek reel se laaya, net pe verify kiya — candidates report karte: payment flow · transaction
> ledger · ATM · do account ke beech internal transfer. JP ka sabse sambhavit HLD, Arpan ki sabse majboot zameen.
> (detail: `JP_INTERVIEW_INTEL.md` section 0e)

```
PAANCH CHEEZ JINPE YE ROUND TAY HOTA (source: "ye interview INHI se tay hote hain"):
   IDEMPOTENCY · TRANSACTIONS · QUEUES · RECONCILIATION · AUDIT
   + ACID vs eventual (partition me CORRECTNESS chuno, availability ki keemat pe)
   + core ledger ke liye RELATIONAL DB preferred, NoSQL nahi
```

---

## TASVEER (ByteByteGo / Alex Xu · CC BY-NC-ND 4.0)

![Read Replica Pattern](https://assets.bytebytego.com/diagrams/0312-read-replica-pattern.png)
Source: [Read Replica Pattern](https://bytebytego.com/guides/read-replica-pattern/)
(dikkat 7 — transfer ke baad purana balance = replica lag; ilaaj: turant wala read PRIMARY se)

---

## SHURU — poocho + numbers

```
POOCHO:  "Bahut bada hai — use-cases baandh lete hain. Ek hi bank ke andar, ya doosre bank ko bhi?"
         (Arpan ne asli mock me sabse pehle yahi poocha: "let's discuss use cases first" — scope pehle,
          warna interviewer jitna bada system chaahe sar pe daal dega)

FR:      user ke ek / kai ACCOUNT · DEPOSIT · WITHDRAW · TRANSFER A -> B (DONO isi bank) · BALANCE · HISTORY
         ★ "dono isi bank" — ye ek shabd poora design badalta (dikkat 1)
         bahar: doosra bank · loan · interest · card · KYC
NFR:     CORRECTNESS sabse upar (paisa na GUM, na BANE, hisaab har waqt barabar) · har paisa TRACEABLE (audit)
         retry pe DOBARA nahi (idempotent) · balance dekhna TEZ (sabse hot read)

NUMBERS: 5M account · 10M txn / din -> 10^7 / 86400 ~ 120 write / sec · read >> write (balance check zyada)
         120 / sec = ek theek-thaak Postgres box ka DAS-VA hissa -> SHARDING KI ZAROORAT NAHI, primary + replica kaafi
         (Arpan khud: "mere number itne bade nahi, normal DB chalega" — SAHI call; log bina zaroorat shard ghusa dete)

DATA SE TEEN SAWAAL (jawab dikkat me):
   1. transfer me DO row badalti -> dono ya koi nahi, kaun sambhalega?   (dikkat 1)
   2. ek account pe do transfer ek saath -> kaun rokega?                 (dikkat 5)
   3. balance -ve na ho -> niyam kahan likha?                            (dikkat 5)
```

---

## DABBA 0 — sabse simple

```
SOLUTION: Banking Service (business logic) -> SQL DB · jaan-boojh ke seedha
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_Banking_Svc["Banking Svc"]
    n_SQL_DB["SQL DB"]
    n_USER --> n_Banking_Svc
    n_Banking_Svc --> n_SQL_DB
```

---

## DIKKAT 1 — transfer ke beech system gira: A ka -500 hua, B ka +500 nahi

```
DIKKAT:   transfer ke beech system gira: A se paisa kat gaya, B ko mila nahi -> paisa gayab

SOLUTION: mera niyam (is design ki reedh): ek DB me -> @Transactional. Alag service / bank -> SAGA.
          Dono account ek DB me hain, to bas EK local transaction: debit aur credit dono ek saath commit
          ya dono rollback. Beech me girne ki jagah hi nahi.
          Sabse badi galti: ek DB me bhi SAGA / queue / event ghusa dena — debit aur credit alag ho jaate,
          phir "kata par fail?" jaisi dikkat design ne banayi, problem ne nahi.
          SAGA tab jab do side ke alag maalik hon (alag service / DB / doosra bank) — undo khud likhna.
          Bahar call ho to pehle PENDING + webhook + reconciliation.
          DB: RELATIONAL (Postgres / Oracle / MySQL) — transaction ka pakka wada. NoSQL me transaction
          kamzor / seemit, wahan app ko sambhalna padta = bina zaroorat SAGA.

NAYA:     koi dabba nahi — DB transaction
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_Banking_Svc["Banking Svc"]
    n_SQL_DB["SQL DB<br/>debit + credit EK transaction"]
    n_USER --> n_Banking_Svc
    n_Banking_Svc --> n_SQL_DB
```
```
BOARD PE: BEGIN;
            UPDATE accounts SET balance = balance - 500 WHERE id = A;
            UPDATE accounts SET balance = balance + 500 WHERE id = B;
          COMMIT;

POOCHEGA: "What if the server crashes in the middle of a transfer?"
BOL:      "Both accounts are in one database, so the debit and credit are one local transaction — it commits
           fully or rolls back. A saga only comes in when the two sides belong to different services or banks."

POOCHEGA: "Consistency or availability — which do you pick?"
BOL:      "Transfer and balance are CP — I'd rather reject than give a wrong balance. SMS, statements and
           analytics after the commit are AP; a couple of seconds late is fine."

AGLA SAWAAL (tere jawab se):
  "Crash pe DB ko kaise pata kya wapas karna hai?"
   -> DB pehle WAL me likhta ("A -500, B +500, COMMIT"). Uthte hi WAL padhta: COMMIT wala txn poora,
      bina COMMIT wala undo -> aadha kabhi nahi
  "A aur B alag bank me?"
   -> tab ek txn nahi -> PENDING + SAGA / reconciliation (payment wala raasta)
```

---

## DIKKAT 2 — regulator: "paisa kahan se aaya, kahan gaya?" + transfer ke baad SMS / fraud / statement

```
DIKKAT:   regulator poochta "paisa kahan se aaya, kahan gaya?" Aur transfer ke baad SMS / fraud check /
          statement — inke liye transfer ruke nahi, aur inme koi gire to transfer na gire.

SOLUTION: LOG aur LEDGER alag cheez: log = debugging, ghoomta / mit-ta rehta. Ledger = sach, append-only
          (kisne, kab, kitna, kyun), na mit-ta na badalta.
          DOUBLE-ENTRY: har transaction ki do entry, jod hamesha zero — paisa sirf hilta. Poore ledger ka
          jod = bank ka kul paisa, mismatch turant pakda jaata.
          Ledger ka ghar DB hai, Kafka nahi: ledger pe query chahiye, aur ledger usi transaction me likhna
          jisme paisa hila — Kafka us transaction ka hissa nahi.
          Kafka ka kaam: commit ke BAAD baaki sab ko khabar (notification, fraud, analytics, statement).
          Jaal: commit hua, Kafka bhejne se pehle app gira -> event gayab -> OUTBOX: event usi transaction me
          outbox table me, relay Kafka bheje. Relay dobara bhi bhej sakta, to consumers eventId se idempotent.

NAYA:     Kafka (outbox relay ke through)

KAISE (outbox relay):
          relay har ~1 sec: SELECT * FROM outbox WHERE sent = false ORDER BY id -> Kafka (acks=all) -> sent = true
          ya CDC (Debezium): DB ka WAL padh ke outbox ki nayi row seedha Kafka me (polling ka bojh nahi)
          relay 'sent' likhne se pehle gira -> event dobara -> consumers eventId se idempotent
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_Banking_Svc["Banking Svc"]
    n_SQL_DB["SQL DB"]
    n_Kafka["Kafka"]
    n_USER --> n_Banking_Svc
    n_Banking_Svc --> n_SQL_DB
    n_SQL_DB --> n_Kafka
```
```
BOARD PE: A -500, B +500 -> jod = 0
          commit -> outbox row -> relay -> Kafka (acks=all) -> SMS / fraud / statement · fail -> DLQ

POOCHEGA: "How do you make sure the SMS / fraud event is never lost?"
BOL:      "The event is written to an outbox table in the same transaction as the transfer, and a relay
           publishes it to Kafka with acks=all. Consumers are idempotent on event id and failures go to a DLQ."

AGLA SAWAAL (tere jawab se):
  "SMS ka kram (pehle debit SMS, phir credit)?"
   -> Kafka key = account_id -> ek account ke event ek partition me kram se
  "Outbox table bhar jaayegi?"
   -> sent rows roz saaf ya partition drop
```

---

## DIKKAT 3 — balance aata kahan se? (is design ka ASLI sawaal)

```
DIKKAT:   balance aata kahan se? (a) account me balance ka column — padhna sasta, par sirf ek likha hua
          number, galat bhi ho sakta. (b) ledger ki saari entries ka jod — hamesha sach, par arabon rows,
          aur balance sabse zyada dekha jaata, har baar hazaaron entries jodna nahi chalega.

SOLUTION: DONO rakho: ledger = sach, balance column = pehle se joda hua nateeja (cache).
          Trick: ledger ki entries aur balance update — sab EK hi transaction me. Ek commit me hain, to alag
          ho hi nahi sakte. Isliye yahan queue nahi — "baad me" kiya to alag ho jaayenge.
          Kabhi farak aaya (bug, manual edit, migration) to LEDGER jeetega, balance usse dobara banta.
          RECONCILIATION: raat ko har account ka ledger jod vs balance — farak = ALERT. Chupchap theek mat
          karo, pehle pata karo KYUN (bug abhi zinda hai).

NAYA:     Reconciliation job (raat ko ledger ka jod aur balance milaane wala)
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_Banking_Svc["Banking Svc"]
    n_SQL_DB["SQL DB"]
    n_Kafka["Kafka"]
    n_Reconciliation_job["Reconciliation job"]
    n_USER --> n_Banking_Svc
    n_Banking_Svc --> n_SQL_DB
    n_SQL_DB --> n_Kafka
    n_Reconciliation_job --> n_SQL_DB
```
```
BOARD PE: 10M / din -> 3 saal ~11 arab txn = ~22 arab row · purana account 5,000-50,000 entry
          BEGIN;
            INSERT INTO ledger_entries (...)   -- A: -500
            INSERT INTO ledger_entries (...)   -- B: +500
            UPDATE accounts SET balance = balance - 500 WHERE id = A;
            UPDATE accounts SET balance = balance + 500 WHERE id = B;
          COMMIT;

POOCHEGA: "How do you know the system is working?"
BOL:      "Ledger is the source of truth and balance is its derived cache; I write both in one transaction, so
           they never drift. Reads hit the balance — one row. A nightly reconciliation compares them, and on a
           mismatch the ledger wins. Plus p99, error rate, queue lag, DB connections with alerts, and a trace id
           per transfer."

AGLA SAWAAL (tere jawab se):
  "Reconciliation me farak mila, kya karoge?"
   -> alert + us account ko ruk ke dekho (freeze?), KYUN farak (bug) dhoondho, phir ledger se balance theek
  "~22 arab row (11 arab txn) pe raat ka SUM har account ka?"
   -> snapshot + sirf aaj ki entries jodo (DIKKAT 9 wala snapshot), poora nahi
```

---

## DIKKAT 4 — do baar tap / client retry -> Rs. 1000 kat gaye

```
DIKKAT:   do baar tap / client retry -> ek transfer do baar, paisa do baar kata

SOLUTION: IDEMPOTENCY KEY: har transfer request ke saath ek key. Pehle dekhi hai -> purana result. Nahi
          -> kaam karo aur record karo.
          Sabse accha: us key pe DB UNIQUE constraint — insert khud lock jaisa, do me se ek hi jeetega.
          App me "check phir insert" likha to race.
          @Transactional idempotency ki jagah nahi leta: transaction = "aadha nahi hoga", idempotency =
          "dobara nahi hoga" — dono chahiye.

NAYA:     koi dabba nahi — DB me idempotency_keys (UNIQUE)
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_Banking_Svc["Banking Svc"]
    n_SQL_DB["SQL DB<br/>+ idempotency_keys (UNIQUE)"]
    n_Kafka["Kafka"]
    n_Reconciliation_job["Reconciliation job"]
    n_USER --> n_Banking_Svc
    n_Banking_Svc --> n_SQL_DB
    n_SQL_DB --> n_Kafka
    n_Reconciliation_job --> n_SQL_DB
```
```
AGLA SAWAAL (tere jawab se):
  "Same key, par body alag (amount 500 ki jagah 5000)?"
   -> key ke saath request ka hash bhi rakho -> mismatch = 422 error, purana result nahi
  "Key table hamesha badhegi?"
   -> 24-48 ghante baad saaf (retry window ke baad kaam ki nahi)
```

---

## DIKKAT 5 — ek hi account pe DO transfer ek saath

```
DIKKAT:   ek hi account pe do transfer ek saath. Pehle maine "locking chahiye" bola — galat tha.
          DB ka atomic update (balance = balance - 500) row lock leta, dono line me lagte, lost update hota
          hi nahi. Race tab hoti jab app padhe, jode, phir likhe.

SOLUTION: do chhoti par asli cheez bachti:
          (a) balance minus me na jaaye, ye check kahan? App me check kiya to dono transfer purana balance
          dekh lete. Isliye check DB me, usi update ke andar ("tabhi ghatao jab balance kaafi ho" — 0 row =
          reject), ya DB constraint. Niyam code me nahi DB me, kyunki raaste kai (API / batch / manual), DB ek.
          (b) DEADLOCK: A->B aur B->A ek saath — dono ek doosre ke lock ka intezaar. DB ek ko maar deta.
          Ilaaj: lock hamesha ek tay kram me lo (chhoti account id pehle).

NAYA:     koi dabba nahi
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_Banking_Svc["Banking Svc"]
    n_SQL_DB["SQL DB<br/>+ idempotency_keys (UNIQUE)"]
    n_Kafka["Kafka"]
    n_Reconciliation_job["Reconciliation job"]
    n_USER --> n_Banking_Svc
    n_Banking_Svc --> n_SQL_DB
    n_SQL_DB --> n_Kafka
    n_Reconciliation_job --> n_SQL_DB
```
```
BOARD PE: A ke paas 300, 200-200 ek saath, check app me -> dono 300 dekhe -> -100
          UPDATE accounts SET balance = balance - 200 WHERE id = A AND balance >= 200;  (0 row = REJECT)
          Txn1 A->B (A lock, B ka intezaar) · Txn2 B->A (B lock, A ka) -> MySQL 1213 (09_DATABASE/08_deadlock.md)

POOCHEGA: "Two transfers hit the same account at the same time — what happens?"
DHYAAN:   2 user ek cheez = atomic / lock · 1 user ka retry = idempotency
BOL:      "Concurrency doesn't need extra locking — the UPDATE does the math inside the database, so there's no
           lost update. Two things matter: the balance check lives in the database, WHERE balance >= amount, and
           locks are always taken in a fixed order so transfers can't deadlock."

AGLA SAWAAL (tere jawab se):
  "WHERE balance >= 200 ne 0 row diya, user ko kya?"
   -> 'insufficient balance' (422), aur txn rollback (credit wala UPDATE bhi nahi)
  "Deadlock fir bhi aaya (kram ke bawajood)?"
   -> DB victim ko rollback karta -> app chhota retry (idempotent key ke saath)
```

---

## DIKKAT 6 — salary day: ek Banking Svc bhara, wahi gira to bank band

```
DIKKAT:   salary day: ek Banking service bhar gayi, aur wahi giri to bank band

SOLUTION: kai Banking service box + aage LB. Service stateless (sab DB me), health check fail = pool se bahar.
          DB primary gira to SYNC (ya semi-sync) replica ko promote karo — async hoti to aakhri transfer kho
          sakta. Replica alag AZ me.

NAYA:     LB
BADLA:    Banking Svc ek se DO — bojh bat gaya, ek gire to doosra chale (asal me zaroorat jitne, diagram me 2)

KAISE (failover kaun karta):
          Patroni (Postgres) / RDS Multi-AZ: health check primary pe -> mara -> sync replica promote
          -> app jis DB address (DNS / endpoint) pe likhta wo naye primary pe point -> ~30-60 sec me wapas
          SYNC kyun: commit tabhi jab replica ne bhi likha -> promote hone wale ke paas har confirmed transfer
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_LB["LB"]
    n_Banking_Svc_x_N_1["Banking Svc 1"]
    n_Banking_Svc_x_N_2["Banking Svc 2"]
    n_SQL_DB["SQL DB"]
    n_Kafka["Kafka"]
    n_Reconciliation_job["Reconciliation job"]
    n_USER --> n_LB
    n_LB --> n_Banking_Svc_x_N_1
    n_LB --> n_Banking_Svc_x_N_2
    n_Banking_Svc_x_N_1 --> n_SQL_DB
    n_Banking_Svc_x_N_2 --> n_SQL_DB
    n_SQL_DB --> n_Kafka
    n_Reconciliation_job --> n_SQL_DB
```
```
POOCHEGA: "What happens if a server or the DB goes down?"
BOL:      "Services are stateless behind a load balancer with health checks. The primary has a synchronous
           replica in another zone that's promoted, so a confirmed transfer is never lost."

AGLA SAWAAL (tere jawab se):
  "Sync replica = har transfer dheema?"
   -> haan, thoda (ek network hop). Paisa me ye keemat theek; async me aakhri transfer kho sakta
  "Failover ke 30 sec me transfer?"
   -> error -> client retry (same idempotency key) -> double nahi
```

---

## DIKKAT 7 — sab balance / history PRIMARY pe padh rahe, transfer dheeme

```
DIKKAT:   sab balance / history primary se padh rahe -> transfer dheeme. Padhna likhne se bahut zyada,
          sab ek hi box pe.

SOLUTION: READ REPLICA: balance / history replica se, write primary pe. (Shard abhi nahi.)
          Jaal: transfer kiya, turant balance dekha -> PURANA dikha (replica thodi peeche) -> user sochega paisa
          gaya hi nahi, dobara bhejega.
          READ-YOUR-OWN-WRITES: jisne abhi likha uska balance primary se. (Balance jaisi cheez hamesha primary
          se bhi chal jaati.)
          Doosri wajah failover: write replica tak pahuncha hi nahi aur wahi promote ho gayi -> sync replication.

NAYA:     Read replica

KAISE (replica peeche kyun + read-your-own-writes):
          primary WAL ko replica pe stream karta (async) -> replica thodi peeche (~ms se sec)
          app ko kaise pata 'ye user abhi likh chuka': session / token me 'last_write_at'
          -> uske N sec (jaise 5) tak us user ke reads PRIMARY se, baaki replica se
          (ya replica ka WAL position >= user ke write ka position tabhi replica se)
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_LB["LB"]
    n_Banking_Svc_x_N_1["Banking Svc 1"]
    n_Banking_Svc_x_N_2["Banking Svc 2"]
    n_Read_replica["Read replica"]
    n_SQL_DB["SQL DB"]
    n_Kafka["Kafka"]
    n_Reconciliation_job["Reconciliation job"]
    n_USER --> n_LB
    n_LB --> n_Banking_Svc_x_N_1
    n_LB --> n_Banking_Svc_x_N_2
    n_Banking_Svc_x_N_1 --> n_Read_replica
    n_Banking_Svc_x_N_2 --> n_Read_replica
    n_Banking_Svc_x_N_1 --> n_SQL_DB
    n_Banking_Svc_x_N_2 --> n_SQL_DB
    n_SQL_DB --> n_Kafka
    n_Reconciliation_job --> n_SQL_DB
```
```
BOARD PE: replica ~200 ms peeche: primary 4000, replica 5000

POOCHEGA: "I transferred money but my balance still shows the old value. Why?"
BOL:      "Most likely replica lag: the write went to the primary, the read hit a replica that hadn't caught up.
           For balance I read from the primary, at least for the user who just wrote. If it were a failover
           losing writes, sync replication fixes that."

AGLA SAWAAL (tere jawab se):
  "Doosre device se khola (session alag)?"
   -> last_write_at user ke account pe (Redis) rakho, device pe nahi
  "Replica bahut peeche (minute)?"
   -> lag metric pe alert; had paar -> us replica ko read pool se hatao
```

---

## DIKKAT 8 — ~22 arab row (11 arab txn) pe history ka page

```
DIKKAT:   arabon rows me history ka page — OFFSET se. DB skip karta nahi koodta, PADHTA aur PHENKTA:
          aage ke page pe lakh row padh ke 20 deta, page 5000 pe atakta. Aur page 1 dekhte waqt nayi
          transaction aayi to sab khisak gaya — page 2 pe wahi entry dobara.

SOLUTION: CURSOR / KEYSET pagination: "kahan chhoda" yaad rakho — pichhle page ki aakhri entry, aur
          agla page "usse purana" se shuru. Index pe seedha wahan koodta: page 1 ho ya 5000, kharcha wahi,
          aur kuch khiskta nahi.
          Jaise YouTube / Instagram — page number nahi, sirf scroll. Ye kami nahi, faisla hai.
          Keemat: "page 500 pe jao" nahi kar sakte, sirf agla / pichhla (statement me theek; admin panel me
          offset chal jaayega).
          Index (account_id, ts DESC, id DESC) ke bina dono nahi chalenge.

NAYA:     koi dabba nahi — query + index
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_LB["LB"]
    n_Banking_Svc_x_N_1["Banking Svc 1"]
    n_Banking_Svc_x_N_2["Banking Svc 2"]
    n_Read_replica["Read replica"]
    n_SQL_DB["SQL DB<br/>+ index (account_id, ts, id)"]
    n_Kafka["Kafka"]
    n_Reconciliation_job["Reconciliation job"]
    n_USER --> n_LB
    n_LB --> n_Banking_Svc_x_N_1
    n_LB --> n_Banking_Svc_x_N_2
    n_Banking_Svc_x_N_1 --> n_Read_replica
    n_Banking_Svc_x_N_2 --> n_Read_replica
    n_Banking_Svc_x_N_1 --> n_SQL_DB
    n_Banking_Svc_x_N_2 --> n_SQL_DB
    n_SQL_DB --> n_Kafka
    n_Reconciliation_job --> n_SQL_DB
```
```
BOARD PE: SELECT * FROM ledger_entries WHERE account_id = ? ORDER BY ts DESC LIMIT 20 OFFSET 100000;
            -> 1,00,020 row padhi, 20 di
          page 1: ... ORDER BY ts DESC, id DESC LIMIT 20;   aakhri (ts, id) = cursor
          page 2: ... AND (ts, id) < (:last_ts, :last_id) ORDER BY ts DESC, id DESC LIMIT 20;

AGLA SAWAAL (tere jawab se):
  "Same ts pe do entry, cursor me koi chhoot jaaye?"
   -> isliye (ts, id) dono cursor me -> id tie todti, kuch nahi chhoota
  "Cursor client ko kaise doge?"
   -> (ts, id) ko base64 string bana ke 'next_cursor' -> client agli baar bheje
```

---

## DIKKAT 9 — 5 saal purana data, sab ek table me

```
DIKKAT:   5 saal ka data ek hi table me. Pichhle kuch mahine roz dekhte, purana saal me ek baar ya
          regulator maange. Har query, index, backup arabon rows ke saath. Aur bank me DELETE hota hi nahi —
          kanoon saalon tak rakhwata.

SOLUTION: (a) DELETE se nahi (crore rows = table lock, ghante). Table ko MAHINE-MAHINE partition karo;
          purana partition alag (detach) karna turant hota, phir cold storage. "Pichhle 3 mahine" ki query
          baaki partitions chhooti hi nahi.
          (b) purana ghar: S3 (Parquet) ya alag cold DB, zaroorat pe query / restore.
          (c) saath me ye tootega: reconciliation ab ledger ka jod adhoora dekhega -> har account mismatch.
          Ilaaj: OPENING BALANCE SNAPSHOT — har period ke end ka balance (kabhi archive nahi). Reconciliation =
          snapshot + uske baad ki entries. Bank statement ke upar "opening balance" isi wajah se.
          Archive != sharding: archive size ghatata, shard likhne ka load baant-ta.

NAYA:     Archive (S3)
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_LB["LB"]
    n_Banking_Svc_x_N_1["Banking Svc 1"]
    n_Banking_Svc_x_N_2["Banking Svc 2"]
    n_Read_replica["Read replica"]
    n_SQL_DB["SQL DB"]
    n_Kafka["Kafka"]
    n_Archive_S3["Archive (S3)"]
    n_Reconciliation_job["Reconciliation job"]
    n_USER --> n_LB
    n_LB --> n_Banking_Svc_x_N_1
    n_LB --> n_Banking_Svc_x_N_2
    n_Banking_Svc_x_N_1 --> n_Read_replica
    n_Banking_Svc_x_N_2 --> n_Read_replica
    n_Banking_Svc_x_N_1 --> n_SQL_DB
    n_Banking_Svc_x_N_2 --> n_SQL_DB
    n_SQL_DB --> n_Kafka
    n_Reconciliation_job --> n_SQL_DB
    n_SQL_DB --> n_Archive_S3
```
```
BOARD PE: ledger_2026_07, ledger_2026_08 ... -> DETACH (metadata, turant) -> cold storage -> drop
          account_balance_snapshot (account_id, period_end, balance) · 31-Mar-2023 A = 45,000

POOCHEGA: "Data keeps growing — what happens in 3 years?"
BOL:      "Partition the ledger by month and detach closed months to cold storage — nothing is ever deleted. An
           opening-balance snapshot per period keeps reconciliation correct after archiving."

AGLA SAWAAL (tere jawab se):
  "Regulator ne 2021 ka statement maanga?"
   -> S3 ke Parquet pe Athena query ya us mahine ka partition restore
  "Archive file badal na sake (audit)?"
   -> S3 Object Lock (WORM) -> likhi file na delete na badle, saalon tak
```

---

## 10x SCALE — har dabba alag

```
Arpan: "dabba bada hua to shard + replication — wo to har design me ho gaya"
KRAM:  sasta pehle, SHARD aakhir
  1. READ REPLICA (pehla kadam, read >> write) · keemat lag -> apna abhi-kiya txn PRIMARY se
  2. PARTITION by month (archive + query dono)
  3. SHARD by account_id = AAKHRI, keemat badi: A shard-1, B shard-2 -> transfer LOCAL txn NAHI -> wapas SAGA / 2PC
     -> dikkat 1 ki saari problem khud wapas · humare 120 / sec pe sawaal hi nahi; pehle vertical + replica
  WRITES se DB maar kha raha (replica ke baad bhi): cache / index sirf READ ilaaj (har naya index INSERT dheema)
     -> SHARD by account_id (ek account ke saare txn ek shard) · non-critical writes (statement, SMS) queue pe
  BADI TABLE (size) -> month partition + archive (DELETE nahi)
  ✗ "badi transaction chhoti karo" = galat: debit + credit EK txn me hi
  replica sirf READ baantti, write ke liye SHARD · country / date = bura key (skew)

POOCHEGA: "Replicas are in, but writes are still killing the DB. What now?"
BOL:      "Replicas only spread reads, and caching or extra indexes help reads, not writes; extra indexes actually
           slow inserts. For write volume at this scale I'd shard by account id, so each account's transactions live
           on one shard, and move non-critical writes like statements to a queue. For table size, partition by month
           and archive closed months to cold storage; nothing is ever deleted."
POOCHEGA: "How would you scale this to 10x?"      -> pehle jo toote (reads -> replica, size -> partition, writes -> shard)
POOCHEGA: "What's the single point of failure?"   -> DB primary (sync replica), Banking Svc (LB + N)
```

---

## POOCHE TO (deep-dive)

```
API:      GET /accounts/{id}/balance · GET /accounts/{id}/transactions?cursor=...&limit=20
          POST /transfers { from, to, amount } · POST /accounts/{id}/deposit { amount } · POST /accounts/{id}/withdraw { amount }
          har POST me Idempotency-Key header (Arpan: "ye to aasan hai" — sahi, yahan ruko mat)

DB:       ledger_entries (SACH, append-only, month partition) · accounts.balance (CACHE, derived)
          idempotency_keys (UNIQUE) · outbox · account_balance_snapshot (period_end ka balance)
```

---

## AAKHRI DABBA + WRAP

```
LB · Banking Svc = stateless, idempotency check · SQL DB = EK local txn (ledger + balance + key + outbox)
Kafka = commit ke BAAD (SMS / fraud / analytics / statement) · Read replica = balance / history (apna txn primary se)
Reconciliation = snapshot + live entries vs balance -> ALERT · Archive (S3) = band mahine, delete kabhi nahi
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_LB["LB"]
    n_Banking_Svc_x_N_1["Banking Svc 1"]
    n_Banking_Svc_x_N_2["Banking Svc 2"]
    n_Read_replica["Read replica"]
    n_SQL_DB["SQL DB"]
    n_Kafka["Kafka"]
    n_Archive_S3["Archive (S3)"]
    n_Reconciliation_job["Reconciliation job"]
    n_USER --> n_LB
    n_LB --> n_Banking_Svc_x_N_1
    n_LB --> n_Banking_Svc_x_N_2
    n_Banking_Svc_x_N_1 --> n_Read_replica
    n_Banking_Svc_x_N_2 --> n_Read_replica
    n_Banking_Svc_x_N_1 --> n_SQL_DB
    n_Banking_Svc_x_N_2 --> n_SQL_DB
    n_SQL_DB --> n_Kafka
    n_Reconciliation_job --> n_SQL_DB
    n_SQL_DB --> n_Archive_S3
```
```
beech me crash        -> ek local transaction (@Transactional), ek DB me SAGA nahi
dobara tap / retry    -> Idempotency-Key + DB UNIQUE
paisa kahan gaya      -> append-only LEDGER, double-entry (jod zero)
balance tez           -> balance = derived CACHE, ledger ke SAATH usi txn
cache vs sach         -> raat ka RECONCILIATION, ledger jeetega
-ve balance           -> check DB me (WHERE balance >= x / CHECK)
ulte kram ke transfer -> lock TAY KRAM (id sort) -> deadlock nahi
11 arab history       -> CURSOR + index (account_id, ts DESC, id DESC)
purana data           -> month PARTITION -> DETACH -> cold storage, DELETE kabhi nahi
archive ke baad       -> OPENING BALANCE snapshot
load badha            -> replica -> partition -> (aakhir) shard
```
```
BOL: "Everything for a transfer — both ledger entries, both balance updates, the idempotency key and an outbox
      event — commits in one local transaction in a relational database, so money is never lost or created.
      The ledger is the truth and balance is a cache of it; a nightly reconciliation checks them. Balance checks
      and lock ordering live in the database. Reads go to a replica, history uses cursor pagination, old months
      are archived with opening-balance snapshots, and side effects go out through Kafka after the commit."
```

ARCHETYPE C · CONCEPTS: [db-what-when](../../FOUNDATIONS/09_databases_what_when.md) · [CAP](../../FOUNDATIONS/08_cap_theorem.md) · [saga/ms-comm](../../FOUNDATIONS/10_ms_communication.md) · [caching](../../FOUNDATIONS/04_caching.md) · saath: [payment-system](../06_payment_system/06_payment_system.md) (iska bada bhai) · [stock-broker](../05_stock_broker_trading/05_stock_broker_trading.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
