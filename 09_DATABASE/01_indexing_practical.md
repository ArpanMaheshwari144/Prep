# DB Indexing

## Scene: `demo.big_users`, 5,24,288 row (21-Aug wala table, 9-Nov ko dobara chalaya)

Ek email dhoondha:

```
bina index (IGNORE INDEX):   type=ALL   key=NULL        rows=523260   Using where
index ke saath:              type=ref   key=idx_email   rows=1        ref=const
```

Ek email ke liye 5.2 lakh row padhne ki jagah sirf 1 row.

## Kyun

Table me data jis kram me daala gaya usi kram me pada hai, sorted nahi. Isliye DB ko ek-ek row padhni padti hai, jo DSA wala linear scan O(n)
hai. `CREATE INDEX` ek alag **sorted B-tree** banata hai jisme email alphabet ke kram me hain aur har email ke saath row ka pata hai. Ab DB
binary search jaisa kood ke seedha pahunchta hai. Asli B-tree ke ek node me saikdon key hoti hain, isliye crore row me bhi 3-4 kadam lagte hain.
Index naya column nahi hai, alag dhaancha hai jo rows ki taraf ishaara karta hai. PRIMARY KEY pe index apne aap banta hai.

## Kis column pe lagao

- Jis pe WHERE, JOIN ya ORDER BY baar-baar lagta ho.
- Jisme alag-alag value zyada hon (email, user_id). Gender (M/F) pe bekaar hai, kyunki aadhi table phir bhi bachti hai aur MySQL scan hi karega.
- Muft nahi hai: har INSERT / UPDATE pe tree bhi theek karna padta hai, to writes dheemi hoti hain aur jagah bhi lagti hai.

## Scene: UPI chai tapri, "har column pe index laga do"

Paytm ki payments table me 30 crore row hain. `WHERE upi_id = ...` bina index ke har row padhti hai (full table scan), 5 minute. upi_id pe
index lagaya to MySQL peeche ek tree banata hai, BST jaisa: chhota left, bada right, sab sorted. Kitaab ka beech ka page kholo, cheez
badi hai to right, chhoti hai to left, aise chhalang maarte hue pahuncho. Search O(log n) ho gaya, 30 crore me bhi ~29 kadam, milliseconds.

**Maan lo developer ne socha "index itna tez hai to har column pe laga do"** (upi_id, amount, status, time, merchant, note): search to
tez rehti hai, par tapri pe har payment ka "processing..." dheema ho jaata hai. Kyunki har nayi payment likhte waqt bhi tree me
dhoondhna padta hai ki ise kahan rakhun, idhar gaya, phir udhar. Aur har index ka apna alag tree hai, to 6 index = har payment pe 6 tree
me jagah dhoondh ke likhna. **Hal:** index sirf us column pe jahan WHERE lagta hai (yahan upi_id), baaki pe nahi.

## Teen cheez jo poochhi jaati hain

- **Composite `(city, age)`:** `WHERE city=...` chalega, `city + age` chalega, sirf `WHERE age > 25` nahi chalega (bayan wala column chhoda).
  Kram: pehle `=` wala, phir range wala.
- **Covering:** `SELECT city, age ... WHERE city='Pune'`. Saare column index me hi hain, to row padhne jaana hi nahi padta (`Extra = Using index`).
- **InnoDB:** PRIMARY KEY ke tree me asli row hoti hai. Baaki index me row ki jagah PK hota hai, to wahan 2 baar dhoondhna padta hai. Isliye
  PK chhota rakho.

## Prod wala trap (9-Nov chala ke dekha)

```
WHERE LOWER(email) = 'user500000@x.com'   ->  type=ALL  key=NULL  rows=523260
```

Index `email` pe hai, `LOWER(email)` pe nahi. Column pe function lagaya to index bekaar ho gaya aur wapas 5.2 lakh row ka scan. Yahi
`LIKE '%gmail'` (shuru me %) ke saath bhi hota hai. Hal: column ko jaisa hai waisa chhodo aur value ko badlo, ya data hi lowercase me save
karo. Index lagaya hai to EXPLAIN se hamesha check karo ki `key` me wo dikh raha hai ya nahi.

---

## Dobara chalana ho to (setup)

5 lakh row wala `demo.big_users` (1 row daalo, phir har baar double):

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

Phir:

```sql
USE demo;
EXPLAIN SELECT * FROM big_users IGNORE INDEX (idx_email) WHERE email='user500000@x.com';   -- type=ALL
CREATE INDEX idx_email ON big_users(email);                                                 -- agar nahi hai
EXPLAIN SELECT * FROM big_users WHERE email='user500000@x.com';                             -- type=ref, rows=1
```

1 lakh row wala `idx_lab.users` (cross-join trick, MySQL 5.7 pe bhi chalti, `WITH RECURSIVE` sirf 8+):

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


Poore DB ke saare index dekhne ho (sirf padhna, safe):

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
