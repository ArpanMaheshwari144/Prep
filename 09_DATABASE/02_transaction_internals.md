# Transaction Internals — DB ke ANDAR kya hota hai (MySQL InnoDB)

> Sawaal: jab hum `START TRANSACTION` karke UPDATE karte hain, DB ke **andar** kya hota hai?
> Kaise track karta? Commit tak kya "pending" rehta? Ye note wahi deep-dive.
> (Related: `../02_SPRING/02_transactional/` — @Transactional isi ka wrapper hai.)

---

## InnoDB kya hai? (28-Sep, Arpan ka sawaal)

```
MySQL = gaadi · InnoDB = uska ENGINE
MySQL ke 2 hisse:
  upar  SQL layer       query samajhna, plan banana (optimizer)        -> sab engine ke liye same
  neeche STORAGE ENGINE  data disk pe kaise rakhe, lock, transaction    -> InnoDB (MySQL 5.5 se DEFAULT)
purana engine MyISAM: transaction NAHI, poori TABLE lock, crash pe data kharab -> ab koi use nahi karta
InnoDB: transaction (ACID) · ROW lock · foreign key · crash recovery (redo) · MVCC (undo)
AWS RDS MySQL (tera kaam) bhi InnoDB hi chalata hai
```

---

## MENTAL MODEL (pehle ye theek karo)

> UPDATE karte hi change **TURANT ho jaati hai** (memory/buffer pool me), par wo
> **"pending / reversible / doosron ko invisible"** hoti hai. Commit use "official" banata.
> "Mark karke commit pe update" — aisa NAHI. Update ho jaata, bas reversible + hidden rehta.

---

## Jab TRANSACTION me UPDATE karte ho — 3 cheezein hoti hain

### 1. UNDO LOG  (rollback + doosron ke reads ke liye)
Row modify karne se **PEHLE**, InnoDB uski **PURANI value UNDO LOG me** likhta hai.
- **Rollback** hua -> undo se purani value WAPAS.
- Doosri transactions ko **purani (committed) value isi undo se** dikhti -> teri adhoori change nahi dikhti. (= MVCC)

### 2. REDO LOG = WAL (Write-Ahead Log)  (durability ke liye)
Change **REDO LOG me DISK pe** likhi jaati -- data page abhi disk pe permanent na bhi ho.
- "Write-ahead" = data likhne se PEHLE log likho.
- **Commit ke baad CRASH** ho -> redo log se change **REPLAY** -> data gum nahi hota. = **Durability**.

### 3. LOCK  (isolation ke liye)
Jis row ko UPDATE kar raha, uspe **ROW-LOCK** -> doosri transaction use modify nahi kar sakti
jab tak tu commit/rollback na kare. = **Isolation**.

★ InnoDB lock asal me **INDEX record** pe lagta hai. UPDATE ke WHERE wale column pe index NAHI ->
  InnoDB ko jitni rows SCAN karni padti, un sab pe lock (lagbhag poori table) -> baaki INSERT / UPDATE ruk jaate.
  **Tera HikariCP wala din yahi tha:** badi write query ne email table pe lock pakde rakhe, inserts atke,
  connections wait me -> pool khatam. Lamba transaction = lamba lock.

---

## COMMIT karne pe
- Redo log **DISK pe FLUSH** -> ab durable, crash-proof.
  (commit se pehle redo zyada tar memory ke log buffer me hota; commit pe flush = innodb_flush_log_at_trx_commit=1 default)
- Locks **RELEASE**.
- Teri change ab **sabko VISIBLE + PERMANENT**. Undo log baad me purge.

## ROLLBACK karne pe
- **UNDO LOG** se saari purani values **RESTORE**.
- Locks release. Jaise kuch hua hi nahi.

---

## UNDO vs REDO — ulti disha (confuse mat karo)

| | UNDO Log | REDO Log (WAL) |
|---|---|---|
| Kya rakhta | **PURANI** value (before) | **NAYI** change (what happened) |
| Kis liye | **PEECHE** jaana: ROLLBACK + doosron ko purana dikhana (MVCC) | **AAGE** jaana: crash ke baad REPLAY (durability) |
| Direction | backward | forward |

- **Undo** = "wapas kaise jaun" (rollback / old version).
- **Redo** = "crash hua to aage kaise pahunchu" (commit ke baad data na gum-e).
- **Lock** = doosron ko us row pe rukna (isolation).

---

## TRACK-CHANGES ANALOGY (concrete)

Word doc me "track changes" ON karke edit karna jaisa:
- Tu edit karta (change memory me ho gayi), par **purana text bhi saved** (undo log).
- Ek **log** banta ki kya badla (redo log).
- Us section pe **lock** -> koi aur edit nahi kar sakta.
- Baaki log abhi **purana published version** dekhte (jab tak "accept" na kare).
- **COMMIT = "Accept all + publish"** -> sabko naya dikhta, permanent.
- **ROLLBACK = "Reject all"** -> track-changes discard, purana wapas.

---

## ONE-LINE (yaad rakh)

> UPDATE turant hoti (buffer pool) + **UNDO** (purana bacha ke) + **REDO/WAL** (crash-recovery) + **LOCK** (isolation).
> COMMIT -> redo flush + visible + permanent.  ROLLBACK -> undo se restore.
>
> **Transaction = undo (peeche) + redo/WAL (aage) + lock (isolation). Har cheez logged -> isliye reliable (ACID).**

---

## INTERVIEW me kaise use kare
Jab @Transactional / ACID / "how does rollback work" pooche -> ye andar wali baat bolo:
> "Internally InnoDB writes the old row to an **undo log** (for rollback + MVCC reads), writes the change to the
> **redo log / WAL** (for crash-recovery durability), and takes a **row lock** (for isolation). Commit flushes redo
> and makes it visible; rollback restores from undo."

-> most candidates abstraction ke peeche nahi dekhte -> ye reasoning STRONG dikhta.
