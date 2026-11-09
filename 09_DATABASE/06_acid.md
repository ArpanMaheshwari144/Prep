# ACID

## Scene: prod ticket, "order 101 cancel karo aur ₹500 user ke wallet me wapas daalo"

```sql
START TRANSACTION;
UPDATE orders SET status = 'CANCELLED' WHERE id = 101;
UPDATE wallet SET balance = balance + 500 WHERE user_id = 7;
COMMIT;
```

Ye dono kaam ek saath hone chahiye. ACID inhi chaar baaton ka naam hai:

- **A, Atomicity (dono ya koi nahi):** pehla UPDATE hua aur doosre se pehle server gir gaya, to order cancel dikhega par paisa wapas nahi
  aayega. Isliye transaction aadha kabhi pakka nahi hota: ya dono, ya ROLLBACK. Ye undo log se hota hai (02 wali file).
- **C, Consistency (niyam na toote):** transaction ke baad bhi DB ke niyam sahi rahein, jaise "balance minus me nahi" ya "order ka customer
  asli ho" (FK). Koi niyam toota to poora transaction reject.
- **I, Isolation (beech me koi na ghuse):** tu ye kar raha hai, usi waqt teammate ko adhoori haalat (order cancel par paisa nahi) na dikhe,
  aur wo usi row ko na badle. Ye undo log aur row lock se hota hai, wahi "chai wala" scene.
- **D, Durability (commit = pakka):** COMMIT hua aur agle second server band ho gaya, phir bhi dono badlaav bache rehte hain. Ye redo log se
  hota hai.

## Teen cheez jo poochhi jaati hain

- **Spring `@Transactional`** yahi A deta hai: method me RuntimeException aaye to rollback (checked exception ke liye `rollbackFor`), aur
  `isolation =` se I ka level set hota hai.
- **ACID ka C aur CAP ka C alag hain.** ACID ka C = ek DB ke niyam sahi rahein. CAP ka C = kai machine pe sabko ek jaisa data dikhe. Naam
  same, matlab alag.
- **Atomic hona race se nahi bachata:** do refund ek saath ek hi balance padh ke apna-apna likhein to ek ka kaam mit sakta hai. Ye I ka kaam
  hai: `UPDATE ... SET balance = balance + ?` (DB khud jode) ya `SELECT ... FOR UPDATE`.

## Prod wala trap (9-Nov MySQL 5.7.44 pe chalaya)

```sql
CREATE TABLE wallet (user VARCHAR(10), balance INT, CHECK (balance >= 0));
INSERT INTO wallet VALUES ('Arpan', 100);
UPDATE wallet SET balance = balance - 500 WHERE user = 'Arpan';
-> balance = -400     (koi error nahi)
```

MySQL 5.7 `CHECK` likhne deta hai, par use lagata hi nahi. Ye MySQL 8.0.16 se lagna shuru hua. Lagega "minus ka niyam laga diya", aur balance
chup-chaap minus me chala jaata hai. Isliye 5.7 pe aisa niyam query me hi lagao (`WHERE balance >= 500`) ya app me check karo, aur version
dekh ke hi CHECK pe bharosa karo.
