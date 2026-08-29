# Isolation Levels — kitna "alag" 2 transactions chalein (+ concurrency anomalies)

> ACID ka **I** = Isolation. "kitna strict" = ISOLATION LEVEL.
> (06_acid.md ne isi file ko point kiya tha — ye wahi deep-dive.)
> 2 transaction ek saath -> ek doosre ka aadha-kaam kितna dikhe? Level decide karta.

---

## 3 ANOMALIES (jo isolation kam hone pe hoti — inhe roken)

```
1. DIRTY READ        = ek txn ne UNCOMMITTED change ki, doosri ne wo padh li -> agar rollback ho gaya
                       to jo padha wo kabhi tha hi nahi. (paisa 999 padha, phir 100 -> galat faisla)

2. NON-REPEATABLE READ = ek txn ne ek row 2 baar padhi, beech me doosri ne COMMIT kar diya
                       -> dono baar ALAG value. "same query, alag answer."

3. PHANTOM READ      = ek txn ne ek RANGE query 2 baar chalayi (WHERE age>18), beech me doosri ne
                       naya row INSERT/DELETE commit kiya -> doosri baar row-COUNT badla. "nayi rows aa gayi."
```

---

## 4 ISOLATION LEVELS (ladder — neeche jitna, utna strict + safe, par slow)

```
┌────────────────────┬───────────┬──────────────────┬──────────┐
│  LEVEL             │ Dirty read│ Non-repeatable   │ Phantom  │
├────────────────────┼───────────┼──────────────────┼──────────┤
│ READ UNCOMMITTED   │  HO SAKTA │  HO SAKTA        │ HO SAKTA │  <- sabse dheela (koi safety nahi)
│ READ COMMITTED     │  ROKA     │  ho sakta        │ ho sakta │  <- sirf committed padho
│ REPEATABLE READ ★  │  ROKA     │  ROKA            │ ho sakta*│  <- MySQL InnoDB DEFAULT
│ SERIALIZABLE       │  ROKA     │  ROKA            │ ROKA     │  <- sabse strict (jaise serial chale)
└────────────────────┴───────────┴──────────────────┴──────────┘
   * MySQL ka REPEATABLE READ (MVCC + next-key locks) practically phantom bhi kaafi rok deta.

   TRADE-OFF: neeche jao -> zyada safe PAR zyada locking -> slow + kam concurrency.
   -> jitni safety CHAHIYE utni hi lo. (banking = high | analytics/reporting = low chalega)
```

---

## ═══ HANDS-ON — Dirty read LIVE dekha (2 MySQL sessions, 21-Aug) ═══

> account(id, balance=100). 2 ALAG session (A, B). A ne balance badla par commit na kiya ->
> READ UNCOMMITTED pe B ne wo 999 padh li (DIRTY) -> A ne rollback -> B ne aisi value padhi jo kabhi thi hi nahi.
> READ COMMITTED pe wahi cheez -> B ne 999 NAHI padha (sirf committed) -> dirty gayab.

### Setup
```sql
CREATE DATABASE demo; USE demo;
CREATE TABLE account (id INT PRIMARY KEY, balance INT);
INSERT INTO account VALUES (1, 100);
```

### Dirty read (READ UNCOMMITTED)
```sql
-- Session B:  SET SESSION TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;
-- Session A:  START TRANSACTION;  UPDATE account SET balance=999 WHERE id=1;   (COMMIT nahi)
-- Session B:  SELECT balance ...   -> 999   (DIRTY READ! uncommitted padh li)
-- Session A:  ROLLBACK;
-- Session B:  SELECT balance ...   -> 100   (999 kabhi thi hi nahi)
```

### Fix (READ COMMITTED)
```sql
-- Session B:  SET SESSION TRANSACTION ISOLATION LEVEL READ COMMITTED;
-- Session A:  START TRANSACTION;  UPDATE account SET balance=555 WHERE id=1;   (COMMIT nahi)
-- Session B:  SELECT balance ...   -> 100   (555 NAHI -> sirf committed padha -> dirty GAYAB)
-- Session A:  COMMIT;
-- Session B:  SELECT balance ...   -> 555   (ab committed -> dikha)
```

### ★★ GEMS / gotchas
```
1. ★ 2 Workbench TABS = ek hi CONNECTION = ek hi SESSION! isolation test ke liye 2 ALAG connection chahiye.
   (2 mysql CLI windows, ya Workbench me 2 alag connection tabs). same session apne uncommitted change
   ko hamesha dekh leta -> tab me test "galat" chalta.
2. "SESSION" = ek connection/login. Har connection ka apna transaction + isolation state.
3. MySQL InnoDB DEFAULT = REPEATABLE READ (dirty + non-repeatable dono roke).
4. Level ek SESSION pe set hota: SET SESSION TRANSACTION ISOLATION LEVEL <level>;
5. TRADE-OFF: strict level = zyada locks = slow. Safety-vs-speed. Jitni chahiye utni lo.
6. Workbench autocommit ON -> explicit txn ke liye START TRANSACTION zaroori (warna UPDATE turant commit).
```

### Dobara kaise
```
2 mysql sessions -> ek me START TRANSACTION + UPDATE (no commit) -> doosre me SELECT.
level badal ke (UNCOMMITTED vs COMMITTED) SELECT ka result badalta -> yehi isolation.
```

---

## INTERVIEW LINE
```
"Isolation levels decide karte 2 concurrent txns ek-doosre ka uncommitted/committed kितna dekhein.
 3 anomalies: dirty read (uncommitted padh li), non-repeatable read (same row 2 baar alag), phantom (range me nayi rows).
 Ladder: READ UNCOMMITTED (kuch nahi roke) -> READ COMMITTED (dirty roke) -> REPEATABLE READ (MySQL default,
 non-repeatable bhi roke) -> SERIALIZABLE (phantom bhi, sabse strict).
 Trade-off = safety vs speed; strict = zyada locking. Maine 2 sessions me dirty read live dekha:
 READ UNCOMMITTED pe uncommitted value dikhi, READ COMMITTED pe gayab."
```

[← DATABASE folder](.)
