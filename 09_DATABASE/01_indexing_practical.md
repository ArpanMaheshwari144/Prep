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

---

## B-TREE — index sorted kaise? (visual, 2-Jul sekha)

```
   Index andar se B-TREE banata -> ek node me KAI keys (SORTED). node full -> SPLIT, middle key UPAR.
   (max 2 keys/node maan le.)

   Insert 10,20:   [10 , 20]

   Insert 5:       [5,10,20] FULL -> middle(10) UPAR:
                        [10]
                       /    \
                    [5]      [20]

   Insert 6,12,30:      [10 , 20]
                       /    |    \
                   [5,6]  [12]   [30]
   -> node ke andar sorted + nodes ke beech sorted (left<key<right). ALAG se sort NAHI karna padta.

   KYUN INDEXING me: hamesha sorted -> search O(log n) (binary-search jaisa) + RANGE query fast
                     (WHERE age BETWEEN 20-30 -> sorted range seedha).
   MULTI-WAY (kai keys/node) -> tree CHOTA (kam levels) -> disk pe kam jumps -> DB ke liye perfect.
   TRADE-OFF: sorted maintain -> har insert/update pe tree adjust -> writes thode slow + storage. (selective index.)
```

---

## ═══ HANDS-ON — Index ka asar LIVE dekha (khud kiya, 21-Aug) ═══

> Upar theory. Ye section = wahi cheez REAL MySQL me chala ke apni aankhon se dekhi:
> 5 lakh rows pe ek email dhoondha -> bina index = 5,23,260 rows padhi | index ke saath = SIRF 1 row.

### 0. Maqsad
```
5 lakh row ka table banao -> ek email search karo -> EXPLAIN se dekho DB ne kaise dhoondha.
Bina index: full table scan (saari rows padhi). Index lagao: seedha 1 row.
```

### 1. SETUP — table + 5 lakh rows (doubling-trick)
```sql
CREATE DATABASE demo; USE demo;
CREATE TABLE big_users (id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(50), age INT, email VARCHAR(100));
-- id = PK -> pehle se indexed. email = koi index NAHI (isi pe demo).

-- 1 row daalo, phir procedure baar-baar DOUBLE kare jab tak 5 lakh na ho:
INSERT INTO big_users (name,age,email) VALUES ('u',20,'x@x.com');
DELIMITER $$
CREATE PROCEDURE fill_rows()
BEGIN
  WHILE (SELECT COUNT(*) FROM big_users) < 500000 DO
    INSERT INTO big_users (name,age,email) SELECT name,age,email FROM big_users;  -- rows DOUBLE
  END WHILE;
END$$
DELIMITER ;
CALL fill_rows();                    -- 1->2->4->...->524288

-- har row ko UNIQUE email do (taaki search exactly 1 dhoondhe):
UPDATE big_users SET email = CONCAT('user', id, '@x.com') WHERE id > 0;
```

### 2. BINA INDEX — slow + full scan
```sql
SELECT * FROM big_users WHERE email = 'user500000@x.com';
EXPLAIN SELECT * FROM big_users WHERE email = 'user500000@x.com';
```
EXPLAIN ne dikhaya:
```
type = ALL       <- POORA table scan
key  = NULL      <- koi index use nahi
rows = 523260    <- ek email ke liye 5.2 LAKH rows padhi
Extra= Using where
```

### 3. INDEX LAGAO — fast + index lookup
```sql
CREATE INDEX idx_email ON big_users(email);         -- 5 lakh rows -> 2-4 sec
SELECT * FROM big_users WHERE email = 'user500000@x.com';
EXPLAIN SELECT * FROM big_users WHERE email = 'user500000@x.com';
```
EXPLAIN ab:
```
type = ref            <- index lookup
key  = idx_email      <- index use hua
ref  = const, rows = 1  <- SIRF 1 row chhui
```

### 4. FARAK (yahi asli baat)
```
                   type      key         rows
   bina index  ->  ALL       NULL        523260   (poori kitaab padhi)
   index ke saath -> ref     idx_email   1        (seedha 1 row)

   5,23,260  ->  1     (ek email dhoondhne me)
```

### 5. KYUN (mechanism + DSA connect)
```
Index = B-tree = SORTED structure (email alphabet-order me).
 -> DB ko scan nahi karna padta, wo seedha JUMP kar jaata (dictionary me 'M' -> beech se kholo, page-1 se nahi).
 -> ref=const, rows=1 = direct hit.

DSA CONNECT: ye wahi hai jo tu jaanta --
   unsorted array me linear scan = O(n)   (= full table scan)
   sorted array me binary search  = O(log n)  (= index)
   Index = DB pe binary-search laga diya.
```

### 6. ★★ GEMS / gotchas
```
1. EXPLAIN padhna: type=ALL (scan, bura) vs type=ref/const (index, achha) . key=NULL vs key-name . rows=kitni padhi.
   -> interview me "index kaam kar raha?" ka proof = EXPLAIN.
2. SAFE-UPDATE (Error 1175): Workbench bina WHERE-on-key mass UPDATE rokta (galti se pura table na bigde).
   FIX: WHERE id>0 (PK) add karo. (ya safe-mode off, par WHERE behtar.)
3. 5 lakh rows ka UPDATE khud ~17 sec laga -> bade table ko chhoona bhi mehnga (writes/migrations soch-samajh ke).
4. INDEX free nahi: read fast PAR har insert/update pe B-tree adjust -> writes thode slow + extra storage.
   -> har column pe index MAT lagao; sirf jahan WHERE/JOIN/ORDER-BY me baar-baar search ho.
```

### 7. Dobara kaise chalaye (quick)
```sql
USE demo;
EXPLAIN SELECT * FROM big_users WHERE email='user500000@x.com';   -- index hai to type=ref
DROP INDEX idx_email ON big_users;                                -- hata ke dekho -> wapas type=ALL
```
