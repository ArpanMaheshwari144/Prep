# DB Indexing — Practical (MySQL hands-on)

Hands-on session: 1 lakh rows banaye, bina-index query (slow/full-scan) vs index query (fast/1-row) ka farak APNI AANKHON se dekha. Copy-paste ready queries + why/how + B-tree magic + trade-off + decision logic + production audit.

CORE (ek line): Index = MySQL ke READS (search) ko fast karne ka tareeka — ek SORTED B-tree banata jis pe binary-search-style lookup hota (full scan ki jagah). Mechanics MySQL karta; humara kaam = DECIDE karna kis column pe.

---

## STEP 1 — Scratch table + 1 lakh rows

```sql
CREATE DATABASE IF NOT EXISTS idx_lab;
USE idx_lab;

DROP TABLE IF EXISTS users;
CREATE TABLE users (
  id   INT PRIMARY KEY AUTO_INCREMENT,
  name VARCHAR(50),
  city VARCHAR(50),
  age  INT
);

-- 1 lakh rows. NOTE: "WITH RECURSIVE" (CTE) sirf MySQL 8+; purane (5.7)
-- pe yeh CROSS-JOIN trick chalti (har version safe):
INSERT INTO users (name, city, age)
SELECT CONCAT('user', n),
       ELT(1+(n%5),'Delhi','Mumbai','Pune','Lucknow','Chennai'),
       18 + (n % 50)
FROM (
  SELECT d4.d*10000 + d3.d*1000 + d2.d*100 + d1.d*10 + d0.d AS n
  FROM (SELECT 0 d UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) d0,
       (SELECT 0 d UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) d1,
       (SELECT 0 d UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) d2,
       (SELECT 0 d UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) d3,
       (SELECT 0 d UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) d4
) nums;

SELECT COUNT(*) FROM users;   -- 100000
```

Cross-join trick: 5 lists (0-9) cross-join = 10^5 = 1,00,000 combos -> har combo se ek number n (0..99999) -> ek fake row. Bina CTE, ek hi INSERT, fast (~0.8 sec).

---

## STEP 2 — BINA INDEX query (slow / full scan)

```sql
-- EXPLAIN = chala nahi, sirf PLAN batata (MySQL kaise dhundega)
EXPLAIN SELECT * FROM users WHERE name = 'user54321';

-- actual query (time dekho)
SELECT * FROM users WHERE name = 'user54321';
```

EXPLAIN output (bina index):
```
   type   = ALL       -> FULL TABLE SCAN (poori table padhi) = SLOW
   key    = NULL      -> koi index use NAHI
   rows   = 100097    -> 1 user dhundne ko ~1 LAKH rows check!
   Extra  = Using where
```
Kyun slow: data UNSORTED (insertion order) hai -> binary search lag hi nahi sakta -> majboori se ek-ek (linear scan), 1 lakh tak. (Jaise 1000-page book mein bina index ke ek word dhundna.)

---

## STEP 3 — INDEX lagao, phir wahi query (fast / 1 row)

```sql
-- name column pe index banao
CREATE INDEX idx_name ON users(name);

-- wahi EXPLAIN dobara
EXPLAIN SELECT * FROM users WHERE name = 'user54321';

-- wahi query dobara
SELECT * FROM users WHERE name = 'user54321';
```

EXPLAIN output (index ke saath):
```
   type   = ref          -> index se SEEDHA pahuncha (ALL nahi)
   key    = idx_name      -> humara index USE hua
   rows   = 1             -> 1 lakh nahi, sirf 1 row check!
   ref    = const
```

### BEFORE vs AFTER (same query)
```
   ┌──────────────┬─────────────┬──────────────┐
   │              │ BINA INDEX  │ INDEX KE SAATH│
   ├──────────────┼─────────────┼──────────────┤
   │  type        │ ALL         │ ref          │
   │  key         │ NULL        │ idx_name     │
   │  rows checked│ 100097      │ 1            │
   └──────────────┴─────────────┴──────────────┘
   = same query, same answer, par 1 lakh scan -> 1 row
```

---

## WHY FAST — B-tree magic (andar kya hua)

```
   BINA INDEX: data UNSORTED pada (insertion order) -> linear scan majboori

   CREATE INDEX ne ek ALAG SORTED TREE (B-tree) banaya name ka:

                      [user50000]          <- root (beech)
                     /           \
              [user25000]      [user75000]
              /      \           /        \
        [user12500][user37500][user62500][user87500]
                       ...  [user54321] <- leaf (+ row pointer)

   LOOKUP "user54321":
   root user50000 -> 54321>50000? RIGHT (left ka aadha uda)
   user75000      -> 54321<75000? LEFT  (phir aadha uda)
   user62500      -> 54321<62500? LEFT  ...
   ~17 hops mein pahuncha (log2(100000) ~= 17), 1 lakh scan nahi
   = har node pe "idhar ya udhar" decide = BINARY SEARCH on tree

   = index = "pehle se sorted structure" taaki jump (BS) kar sako
   = tree na hota to binary search kahan karte? (unsorted = scan)
```
Connection: yeh wahi Binary Search pattern hai (sorted + decide + halve = O(log n)) — DB ke andar chal raha. Actual structure = B-tree (B+ tree) = binary search tree ka multi-way bada bhai.

---

## INDEX != COLUMN (important clear-up)

```sql
SHOW CREATE TABLE users;
```
```
CREATE TABLE users (
  id ..., name ..., city ..., age ...,   <- yeh 4 COLUMNS (asli data)
  PRIMARY KEY (id),                       <- id ka AUTO index
  KEY idx_name (name)                     <- humara index (COLUMN NAHI!)
)
```
- `KEY idx_name (name)` = COLUMN nahi, INDEX ki definition. "KEY" = "INDEX" (MySQL synonym).
- Index = ALAG sorted B-tree (rows ko point karta), table mein naya column NAHI.
- Book analogy: book ke peeche ka index = naya chapter nahi, alag section jo pages point karta.

---

## PRIMARY KEY = auto index

```sql
SHOW INDEX FROM users;
```
```
   PRIMARY  | id   | BTREE   <- MySQL ne KHUD banaya (id lookups already fast)
   idx_name | name | BTREE   <- HUMNE banaya (name search fast)
```
- PRIMARY KEY khud-ba-khud ek index hota -> "WHERE id = X" already fast.
- idx_name alag banaya kyunki hum `name` pe search kar rahe the (PRIMARY sirf id pe).
- RULE: har index sirf APNE column ki search fast karta. Alag column search? -> us column ka alag index.

---

## TRADE-OFF (index free nahi — kyun har column pe nahi)

```
   1. WRITES SLOW: har INSERT/UPDATE/DELETE pe index B-tree bhi
      update karna padta -> SORTED jagah pe daalo + (node full ->
      SPLIT/re-balance). 5 index = har insert pe 5 trees maintain!
   2. SPACE: index = alag B-tree structure = extra disk.
   3. ISLIYE: sirf un columns pe index jo SEARCH/filter karte ho.

   READ-heavy column  -> index FAYDA
   WRITE-heavy table  -> index ka cost > fayda (soch ke)
   = index = trade-off (reads fast, writes slow + space), free lunch nahi
```

---

## DECISION — kis column pe index, kaise decide

```
   YAHAN LAGAO (candidates):
   - WHERE clause wale (filter):       WHERE email = ...   -> email
   - JOIN keys (foreign/join):          JOIN ON user_id    -> user_id
   - ORDER BY / GROUP BY wale (sort):   ORDER BY created_at-> created_at

   KAISE DECIDE:
   1. Apni QUERIES dekho (app kya WHERE/JOIN/ORDER karta) -> wahi columns
   2. CARDINALITY (KEY point):
      HIGH distinct (email/user_id, 1 lakh unique) -> GREAT index (~1 row)
      LOW distinct  (gender M/F, boolean)          -> BEKAAR (aadhi table
                    bachti -> MySQL aksar full scan hi karega)
   3. READ vs WRITE balance

   MAT LAGAO: low cardinality (M/F), rarely-queried, chhoti tables,
              bahut frequently updated columns.

   VERIFY: index laga ke EXPLAIN -> "key" mein index dikhe + rows kam ho.
```
Interview line: "Index = jin columns pe SEARCH/JOIN/SORT karte ho + jinki cardinality HIGH ho. Low-cardinality (M/F) + rarely-queried = mat lagao."

---

## PRODUCTION — kaunse table pe kaunsa index? (audit)

```sql
-- EK table ke indexes:
SHOW INDEX FROM <table>;
SHOW CREATE TABLE <table>;

-- POORE DB ke SAARE indexes (production audit — read-only, safe):
SELECT TABLE_NAME, INDEX_NAME, COLUMN_NAME,
       SEQ_IN_INDEX, NON_UNIQUE, INDEX_TYPE
FROM information_schema.STATISTICS
WHERE TABLE_SCHEMA = 'tododb'     -- yahan apne/prod DB ka naam
ORDER BY TABLE_NAME, INDEX_NAME, SEQ_IN_INDEX;
```
```
   OUTPUT samajhna:
   TABLE_NAME   -> kis table pe
   INDEX_NAME   -> PRIMARY (auto) ya custom (idx_name)
   COLUMN_NAME  -> kis column pe
   NON_UNIQUE   -> 0 = unique index, 1 = normal
   INDEX_TYPE   -> BTREE (almost always)
```
Company prod DB: same query, bas TABLE_SCHEMA = 'prod_db_name'. Workbench mein bhi: table expand -> "Indexes" section (GUI).

Raw B-tree nodes/pages = InnoDB ke internal binary files (.ibd) mein -> SQL se directly NAHI dikhte (advanced tool innodb_ruby se, par deep DBA). Metadata + EXPLAIN se confirm ho jaata index kaam kar raha.

---

## POWER PHRASES

- "Index = reads fast karne ka sorted B-tree; full scan (O(n)) -> binary-search lookup (O(log n))."
- "Bina index data UNSORTED -> BS possible nahi -> scan. Index = sorted structure -> BS possible."
- "KEY = INDEX (synonym). Index COLUMN nahi, alag B-tree jo rows point karta."
- "PRIMARY KEY = auto index (id fast). Har column ki search ke liye us column ka alag index."
- "Trade-off: reads fast, writes slow (B-tree re-balance) + space. Index only searched columns."
- "Kis column? WHERE/JOIN/ORDER wale + HIGH cardinality. Low cardinality (M/F) = bekaar."
- "Audit: information_schema.STATISTICS se poore DB ke indexes."

---

## TRAP BOX

```
┌─────────────────────────────────────────────────────────┐
│ TRAP 1: "Sab columns pe index" = writes crawl + space     │
│   waste. Sirf SEARCH/JOIN/SORT wale columns.              │
│                                                          │
│ TRAP 2: Low-cardinality (gender M/F) pe index             │
│   -> aadhi table bachti, MySQL full scan hi karega = bekaar│
│                                                          │
│ TRAP 3: Index = naya column samajhna                      │
│   -> NAHI. Alag B-tree structure (KEY = INDEX).           │
│                                                          │
│ TRAP 4: Index laga diya, par EXPLAIN se verify nahi kiya  │
│   -> ho sakta MySQL use hi na kare (functions on column,  │
│      leading wildcard LIKE '%x', low cardinality).        │
│                                                          │
│ TRAP 5: WITH RECURSIVE (CTE) data-gen MySQL 5.7 pe        │
│   -> error. Cross-join trick use karo (har version).      │
└─────────────────────────────────────────────────────────┘
```
