# DB Deadlock — 2 transactions ek-doosre ka lock, circular wait

> 2 txn ek-doosre ki row ka lock maangein -> koi aage nahi badh sakta = DEADLOCK.
> MySQL khud detect karke ek ko maar deta (victim rollback). Concurrency ka classic issue.

---

## CONCEPT

```
Transaction jab row UPDATE karti -> us row pe ROW-LOCK le leti (commit/rollback tak).
Deadlock tab: A ke paas row-1 ka lock + row-2 chahiye
              B ke paas row-2 ka lock + row-1 chahiye
   -> A, B ka wait kare | B, A ka wait kare -> CIRCULAR WAIT -> koi aage nahi.

MySQL ka DEADLOCK-DETECTOR cycle pakadta -> ek ko VICTIM chunta (jiska rollback sasta)
   -> victim ki txn ROLLBACK + error 1213 -> doosra bach jaata (aage badh sakta).
   (warna dono hamesha atke rehte.)
```

## OS/multithreading se CONNECT
```
Ye wahi deadlock hai jo DeadlockDemo.java (multithreading) me tha:
   2 thread 2 lock ULTE order me maangein -> circular wait.
DB me bhi same, bas resource = ROWS. Concept ek hi (circular wait for resources).
```

---

## ═══ HANDS-ON — Deadlock LIVE (2 MySQL sessions, 22-Aug) ═══

> account table, 2 rows (id=1, id=2). 2 ALAG sessions. Dono ne apni-apni row lock ki,
> phir ek-doosre ki row maangi -> circular wait -> MySQL ne Session B ko victim banaya (error 1213).

### Setup
```sql
USE demo;
-- account me 2 rows chahiye
INSERT INTO account VALUES (2, 200);   -- (id=1 pehle se tha)
SELECT * FROM account;                 -- 2 rows: id=1, id=2
```

### Deadlock banao (ORDER important)
```sql
-- ① Session A:  START TRANSACTION;  UPDATE account SET balance=balance+1 WHERE id=1;   (row-1 LOCK)
-- ② Session B:  START TRANSACTION;  UPDATE account SET balance=balance+1 WHERE id=2;   (row-2 LOCK)
-- ③ Session A:  UPDATE account SET balance=balance+1 WHERE id=2;   -> B ka lock chahiye -> A WAIT (atak gaya)
-- ④ Session B:  UPDATE account SET balance=balance+1 WHERE id=1;   -> A ka lock chahiye -> CIRCULAR WAIT
--
--    -> MySQL ne Session B pe:
--       ERROR 1213 (40001): Deadlock found when trying to get lock; try restarting transaction
--    B = victim (rollback). A bach gaya.
```

### Cleanup
```sql
-- Session A:  ROLLBACK;   (ya COMMIT -> lock chhodo)
```

---

## ★★ FIX / PREVENTION (interview gold)
```
1. SAME LOCK ORDER -> sab txns rows ko EK hi kram me lock karein (jaise hamesha id ASC).
   dono pehle id=1 phir id=2 lein -> cycle banti hi nahi. (sabse asli fix)
2. RETRY on 1213 -> app deadlock-error catch kare aur txn DOBARA chalaye.
   (error khud kehta: "try restarting transaction")
3. Transactions CHHOTI + fast rakho -> lock kam der pakde -> deadlock chance kam.
4. (advanced) lock timeout / lower isolation jahan chale.
```

## GEMS
```
1. Deadlock = CIRCULAR WAIT (A->B->A). MySQL auto-detect karke ek ko maarta (1213). App retry kare.
2. Ye multithreading-deadlock (DeadlockDemo.java) jaisa hi -> resource = rows.
3. #1 fix = consistent lock ORDERING (same order me lock lo -> cycle impossible).
4. Deadlock != lock-wait-timeout: deadlock = circular (turant detect); timeout = ek der tak wait karke haar gaya.
```

## INTERVIEW LINE
```
"Deadlock tab jab 2 txns ek-doosre ki lock-ki-hui rows maangein -> circular wait. MySQL ka deadlock-detector
 cycle pakadta aur ek txn ko victim bana ke rollback karta (error 1213) taaki doosra aage badhe.
 Prevention: consistent lock ordering (same kram me lock), retry on 1213, chhoti transactions.
 Ye OS-thread deadlock jaisa hi hai -- resource sirf DB rows ban jaate."
```

### Dobara kaise
```
2 sessions -> dono START TRANSACTION -> A id=1 lock, B id=2 lock -> phir A id=2 maange, B id=1 maange
-> ERROR 1213 (ek victim).
```

[← DATABASE folder](.)
