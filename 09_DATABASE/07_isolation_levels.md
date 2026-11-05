# Isolation Levels: ek transaction doosre ka kitna dekhe

## Scene: munshi likh raha, cashier padh raha

Mausi ke mart me ghee ka hisaab ek register me hai. Munshi (transaction A) register me likh raha hai, aur cashier (transaction
B) usi register se padh raha hai. Isolation ka matlab hai ki cashier ko munshi ka adhoora kaam kitna dikhe. Teen gadbad ho
sakti hain:

```
1. DIRTY READ           munshi ne pencil se likha "ghee: 999" (commit nahi), cashier ne padh ke 999 ka maal
                        bech diya, munshi ne mita diya (rollback) -> 999 kabhi tha hi nahi
2. NON-REPEATABLE READ  cashier ne padha 500, beech me munshi ne 300 likh ke pakka kar diya, cashier ne
                        dobara dekha 300 -> ek hi bill me ek hi line ke do jawab
3. PHANTOM              cashier ne gina "500 se zyada wale item: 10", munshi ne naya item jod diya,
                        dobara gina 11 -> nayi line bhoot ki tarah aa gayi
```

Isolation level batata hai ki inme se kitni gadbad roki jaaye:

```
READ UNCOMMITTED   kuch nahi rokta
READ COMMITTED     dirty rokta        <- Postgres / Oracle ka default
REPEATABLE READ    + non-repeatable   <- MySQL InnoDB ka default (MVCC + next-key lock se phantom bhi zyadatar)
SERIALIZABLE       sab rokta, sabse dheema
```

Jitna kada level, utne zyada taale, aur utna dheema. Isliye banking me kada level chahiye, aur reporting me dheela chal jaata
hai. DB badla to default bhi badal jaata hai.

## Prod wala trap

```java
@Transactional                                         // isolation = DB ka default
public void withdraw(long id, int amt) {
    int bal = repo.getBalance(id);                     // dono request ne 100 padha
    if (bal >= amt) repo.setBalance(id, bal - amt);    // dono ne 20 likha -> 160 nikle
}
```

`@Transactional` laga diya to log maan lete hain ki safe ho gaya, par nahi hua. Ye MT 04 wala hi "padho, phir likho" jhagda
hai, bas ab DB me, aur isse lost update kehte hain. Ilaaj wahi hai jo banking aur e-commerce me kiya: ek hi query me shart
(`0 row` aaye to mana), ya pehle `SELECT ... FOR UPDATE` se row pe taala.

```sql
UPDATE account SET balance = balance - 80 WHERE id = ? AND balance >= 80;   -- 0 row = mana
```

## Tera hands-on (21-Aug, 2 MySQL session)

```sql
CREATE TABLE account (id INT PRIMARY KEY, balance INT);  INSERT INTO account VALUES (1, 100);

-- READ UNCOMMITTED (dirty read dikha)
-- B: SET SESSION TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;
-- A: START TRANSACTION; UPDATE account SET balance=999 WHERE id=1;   (commit nahi)
-- B: SELECT balance ...  -> 999
-- A: ROLLBACK;
-- B: SELECT balance ...  -> 100   (999 kabhi tha hi nahi)

-- READ COMMITTED (dirty gaayab)
-- B: SET SESSION TRANSACTION ISOLATION LEVEL READ COMMITTED;
-- A: START TRANSACTION; UPDATE account SET balance=555 WHERE id=1;   (commit nahi)
-- B: SELECT balance ...  -> 100
-- A: COMMIT;
-- B: SELECT balance ...  -> 555
```

Ek baat jo us din pakdi thi: Workbench ke 2 tab ek hi connection (session) hote hain, aur ek session apna uncommitted badlaav
hamesha dekh leta hai, isliye isolation test ke liye 2 alag connection chahiye. Aur Workbench me autocommit ON hota hai, to
`START TRANSACTION` likhna zaroori hai, warna UPDATE turant commit ho jaata hai.

## Interview line

```
Isolation decides how much two concurrent transactions see of each other. The three
anomalies are dirty read, non-repeatable read and phantom. Going from READ UNCOMMITTED to SERIALIZABLE
is safer but slower; MySQL defaults to REPEATABLE READ and Postgres to READ COMMITTED. For
read-then-write on money I don't rely on the level at all: @Transactional alone still loses updates,
so I use an atomic conditional UPDATE or SELECT FOR UPDATE.
```

[← DATABASE folder](.)
