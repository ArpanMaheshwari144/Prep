# ACID — transaction ke 4 guarantee

> **Transaction** = ek logical kaam jisme 1+ DB operations, jo ek unit ki tarah chalte.
> Classic example: **A ke account se B ko ₹100 transfer** = 2 step → `A -100` **aur** `B +100`.
> ACID = ye 4 guarantee jo DB transaction pe deta.

---

## A — Atomicity (all-or-nothing)
Dono step ya to **poore** ho, ya **koi nahi**. Beech me crash (A se -100 hua, B ko +100 hone se pehle bijli gayi) → poora **ROLLBACK**. Aadha-adhura kabhi commit nahi. Paisa gayab nahi hoga.
> soch: **switch ON ya OFF — beech me nahi.**

## C — Consistency (rules kabhi na tootein)
Transaction ke baad DB **valid state** me hi ho — saare constraint/rule intact. Transfer se pehle total = ₹500, baad me bhi ₹500 (paisa banta/gayab nahi). Balance rule (>= 0) toota → transaction reject.
> soch: **valid → valid; invalid beech-state kabhi permanent nahi.**

## I — Isolation (concurrent transactions ek-doosre ko disturb na karein)
Do transaction **ek saath** chalein to ek doosre ki adhoori cheez na dekhein. **Poora** "jaise ek-ke-baad-ek chale" sirf SERIALIZABLE level pe milta; default level (MySQL REPEATABLE_READ) thoda dheela hai. Ek ka aadha-update dusri transaction ko dikhna nahi chahiye.
> soch: **2 log ek hi form bhar rahe — ek ka aadha kaam dusre ko na dikhe.**
> **kitna** strict = **ISOLATION LEVELS** (Read Committed / Repeatable Read / Serializable + dirty/phantom read) = deeper sub-topic, alag file.

## D — Durability (commit = pakka)
Ek baar **commit** ho gaya → power-cut/crash bhi ho, data **bacha rehta** (disk pe likh diya, sirf RAM me nahi).
> soch: **"save" dabaya = ab bijli jaaye tab bhi safe.**

---

## ★ Interview CONNECT
- **Spring `@Transactional`** = yahi deta: method me exception → **rollback** (Atomicity), aur `isolation=` param se **Isolation-level** set.
- **CAP theorem ka C** (consistency) ≠ **ACID ka C** — CAP = distributed nodes me sab same data; ACID = single-DB constraints valid. (naam same, matlab alag — interview trap.)
- **NoSQL** aksar full-ACID nahi deta (eventual consistency); **RDBMS** (Postgres/MySQL-InnoDB) full-ACID. Isliye banking/payments = RDBMS.
  (MongoDB 4.0+ multi-document transaction deta hai, par mehenga — design embed karke usse bachta.)
- ★ **InnoDB andar se har letter kaise deta** (02_transaction_internals se):
  A = UNDO log (rollback) · D = REDO log / WAL (crash ke baad replay) · I = ROW LOCK + MVCC · C = constraint + upar ke teen
- ★ **ATOMIC ≠ race-safe** (26-Sep payment me pakda): transaction ek payment ko poora ya kuch nahi karta, par
  do payment ek saath ek hi balance padh ke ghatayein to minus me ja sakta. Wo ISOLATION ka kaam hai ->
  `UPDATE ... WHERE balance >= ?` ya `SELECT ... FOR UPDATE`.

---

## 1-line recall
Transaction = 1 unit. **A**tomicity=all-or-nothing(rollback) · **C**onsistency=rules-intact(valid→valid) · **I**solation=concurrent-lage-serial · **D**urability=commit-ke-baad-crash-proof(disk). Spring `@Transactional` = A + isolation. ACID-C ≠ CAP-C.
