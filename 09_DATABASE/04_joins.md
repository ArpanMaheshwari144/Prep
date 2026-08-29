# SQL JOINS — visual, concrete (MySQL / Postgres / MongoDB)

> Ratta nahi. Do table leke har join ka RESULT dekho. Join = do table ko ek chaabi (key) pe jodna.

---

## Setup — 2 tables (poore me yehi chalega)

**employees**
| id | name  | dept_id |
|----|-------|---------|
| 1  | Arpan | 10      |
| 2  | Ravi  | 20      |
| 3  | Sia   | NULL    | ← kisi dept me nahi

**departments**
| id | dept_name |
|----|-----------|
| 10 | Backend   |
| 20 | Frontend  |
| 30 | DevOps    | ← koi employee nahi

Join ki **chaabi**: `employees.dept_id = departments.id`

---

## 1. INNER JOIN — sirf MATCH wale (dono taraf ho)
```sql
SELECT e.name, d.dept_name
FROM employees e
INNER JOIN departments d ON e.dept_id = d.id;
```
| name  | dept_name |
|-------|-----------|
| Arpan | Backend   |
| Ravi  | Frontend  |

→ Sia (dept nahi) OUT, DevOps (emp nahi) OUT. Sirf **dono taraf match** = andar.

---

## 2. LEFT JOIN — LEFT ka SAB + right jo mile (na mile → NULL)
```sql
FROM employees e
LEFT JOIN departments d ON e.dept_id = d.id;
```
| name  | dept_name |
|-------|-----------|
| Arpan | Backend   |
| Ravi  | Frontend  |
| Sia   | NULL      | ← left ka banda raha, right khaali

→ **Saare employees** aaye (Sia bhi), uska dept nahi mila to NULL. DevOps out (wo right pe hai).
**Sabse zyada use** — "saare X, chahe unka Y ho ya na ho".

---

## 3. RIGHT JOIN — RIGHT ka SAB + left jo mile
```sql
FROM employees e
RIGHT JOIN departments d ON e.dept_id = d.id;
```
| name  | dept_name |
|-------|-----------|
| Arpan | Backend   |
| Ravi  | Frontend  |
| NULL  | DevOps    | ← dept raha, koi emp nahi

→ **Saare departments** (DevOps bhi), Sia out.
(LEFT ka ulta — practice me LEFT zyada; table order ulta karke kaam chal jaata.)

---

## 4. FULL OUTER JOIN — dono ka SAB (jahan match nahi → NULL)
| name  | dept_name |
|-------|-----------|
| Arpan | Backend   |
| Ravi  | Frontend  |
| Sia   | NULL      |
| NULL  | DevOps    |

→ Left ka bacha (Sia) + Right ka bacha (DevOps) **dono** aaye. Poora union.

---

## 5. CROSS JOIN — har row × har row (cartesian, koi condition nahi)
3 emp × 3 dept = **9 rows**. Har employee ko har dept ke saath joda. Real me kam use (combinations chahiye tab).

## 6. SELF JOIN — table ko KHUD se join
`employees` me `manager_id` ho (jo bhi ek employee hai) → employee ko uske manager se jodo.
Ek hi table, do baar alias:
```sql
SELECT e.name AS emp, m.name AS manager
FROM employees e
JOIN employees m ON e.manager_id = m.id;
```

---

## Venn (dimaag me chipka)
```
INNER  = beech ka overlap (∩)
LEFT   = poora LEFT circle (+ overlap)
RIGHT  = poora RIGHT circle (+ overlap)
FULL   = dono circle poore (∪)
```

---

## MySQL vs Postgres (asli interview-diff)
- INNER / LEFT / RIGHT / CROSS → **dono me same** syntax.
- ★ **FULL OUTER JOIN**: **Postgres = supported. MySQL = NAHI!**
  MySQL me emulate: `LEFT JOIN ... UNION ... RIGHT JOIN`.  ← ye interview me poochte.
- Postgres = standard-compliant/strict; MySQL kuch quirks (joins ke liye ~95% same).
- Dono me `USING(col)` shorthand (column-naam same ho tab) + `NATURAL JOIN` (auto-match — **avoid**, chhupa bug).

---

## MongoDB — joins hote hi nahi (alag soch)
- Mongo = document DB → data usually **EMBEDDED** (denormalized). Employee ke andar hi dept ghusa dete → join ki zaroorat hi nahi.
- Jab sach me do collection jodni ho → aggregation me **`$lookup`** (= LEFT OUTER JOIN jaisa):
```js
db.employees.aggregate([
  { $lookup: {
      from: "departments",
      localField: "dept_id",
      foreignField: "id",
      as: "dept"          // matched dept ek ARRAY me aata
  }}
])
```
- **SQL soch:** normalize → query-time pe JOIN.
- **Mongo soch:** embed/denormalize → join avoid; sirf kabhi-kabhi `$lookup` (heavier, sirf left-outer style).

---

## 1-line recall
INNER = overlap · LEFT = saara-left · RIGHT = saara-right · FULL = dono-poore · CROSS = har×har · SELF = khud-se.
MySQL me FULL-OUTER nahi (UNION se). Mongo = embed, warna $lookup.
