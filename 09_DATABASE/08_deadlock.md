# DB Deadlock: do transaction, ek doosre ki row ka intezaar

## Scene: wahi chaabi aur register, ab DB ki row

MT 07 wala galle ki chaabi aur register ka jhagda, bas ab DB ki do row pe. Ek hi second me do transaction: Arpan se Ravi 100,
aur Ravi se Arpan 50. Pehli ne Arpan ki row pakdi aur Ravi ki maangi, doosri ne Ravi ki pakdi aur Arpan ki maangi. Gol chakkar
ban gaya.

```
TXN-1 (Arpan -> Ravi)          TXN-2 (Ravi -> Arpan)
lock row ARPAN                 lock row RAVI
maange row RAVI  ... ruka      maange row ARPAN ... ruka
        \_____________ circular wait ____________/
MySQL detector cycle pakadta -> ek ko VICTIM bana ke ROLLBACK (error 1213), doosra aage badh jaata
```

Java aur DB me farak ye hai ki Java me app chupchaap latka reh jaata tha, par DB khud dekhta rehta hai. Gol chakkar dikhte hi
wo ek transaction ko victim chunta hai, uska poora kaam rollback kar deta hai, aur doosra aage badh jaata hai.

## Hal

Sab ek hi kram me lock lein, jaise chhoti id pehle. Dono transaction pehle Arpan ki row maangenge, to cycle ban hi nahi sakti.
Saath me transaction chhoti rakho, taaki taala kam der pakda rahe. Aur deadlock aur lock-wait-timeout alag cheez hain: deadlock
gol chakkar hai jo turant pakda jaata hai, aur timeout ka matlab hai der tak line me khade rahe aur haar gaye.

## Prod wala trap

```java
@Transactional
public void transfer(...) {
    try { debit(from); credit(to); }
    catch (CannotAcquireLockException e) {   // 1213 Spring me aise aata
        debit(from); credit(to);             // retry ISI txn me -> bekaar
    }
}
```

DB victim ki poori transaction pehle hi rollback kar chuka hai, isliye usi ke andar dobara try karna bekaar hai. Retry bahar se,
nayi transaction me karo: caller ek-do baar dobara bulaye, ya `@Retryable` us method pe lagao jo `@Transactional` method ko
call karta hai. Aur retry tabhi safe hai jab idempotency key ho, jaisi banking me thi.

## Tera hands-on (22-Aug, 2 MySQL session)

```sql
INSERT INTO account VALUES (2, 200);   -- id=1 pehle se tha
-- 1. A: START TRANSACTION; UPDATE account SET balance=balance+1 WHERE id=1;   (row-1 lock)
-- 2. B: START TRANSACTION; UPDATE account SET balance=balance+1 WHERE id=2;   (row-2 lock)
-- 3. A: UPDATE account SET balance=balance+1 WHERE id=2;   -> B ka lock chahiye, A atak gaya
-- 4. B: UPDATE account SET balance=balance+1 WHERE id=1;   -> gol chakkar
--    B pe: ERROR 1213 (40001): Deadlock found when trying to get lock; try restarting transaction
--    B = victim (rollback), A bach gaya.  Cleanup: A: ROLLBACK;
```
