# NORMALIZATION (1NF → 2NF → 3NF) + Denormalization

> Table ko organize karo taaki REDUNDANCY (data baar-baar repeat) + ANOMALIES kam ho. Bade flat table ko chhoti related tables me todo.

---

## KYUN — ek BAD table + 3 anomalies

```
ENROLLMENTS (bad -- sab ek table me)
| student_id | student_name | course_id | course_name | instructor | instr_phone |
| 1          | Arpan        | C1        | DBMS        | Rao        | 99999       |
| 1          | Arpan        | C2        | OS          | Iyer       | 88888       |
| 2          | Ravi         | C1        | DBMS        | Rao        | 99999       |   <- DBMS/Rao/phone REPEAT
```

Same data repeat → 3 problem:
- **UPDATE anomaly:** Rao ka phone badla → HAR row me badalna padega. Ek miss = data inconsistent.
- **INSERT anomaly:** naya course "AI" (abhi koi student nahi) add NAHI kar sakte (student_id chahiye).
- **DELETE anomaly:** Ravi ki last row hataayi → uske saath DBMS course ka data bhi GUM.

---

## KAISE — 1NF → 2NF → 3NF (isi table ko evolve karo)

### 1NF — har cell ATOMIC (ek hi value; no list / repeating-group)
- Bad: `courses` column me `"C1, C2"` (list).
- 1NF: har course ALAG row. (upar wali table already 1NF hai -- cells atomic.)

### 2NF — 1NF + koi PARTIAL dependency nahi
non-key column POORI key pe depend kare, key ke ek-hisse pe nahi.
- PK = (student_id, course_id). `course_name` sirf **course_id** pe depend (student_id se matlab nahi) = partial.
- Fix → alag nikaalo:
```
STUDENTS(student_id, student_name)
COURSES(course_id, course_name, instructor, instr_phone)
STUDENT_COURSE(student_id, course_id)          <- sirf link
```

### 3NF — 2NF + koi TRANSITIVE dependency nahi
non-key column doosre NON-KEY pe depend na kare, sirf key pe.
- COURSES me: `instr_phone` → **instructor** pe depend (jo khud non-key) = transitive.
- Fix → instructor alag:
```
COURSES(course_id, course_name, instructor_id)
INSTRUCTORS(instructor_id, name, phone)
```

---

## ★ GOLD one-liner (ye poori normalization)
> **"Har non-key column KEY pe depend kare — the KEY, the WHOLE key, and NOTHING BUT the key."**
>   1NF = atomic cells · 2NF = whole key (partial nahi) · 3NF = nothing but key (transitive nahi).

---

## Normalize ka TRADE-OFF
- **Faayda:** no redundancy · no anomalies · data consistent · kam storage.
- **Cost:** data toota → padhne ko JOINS lagane padte → slow reads.

## DENORMALIZATION (ulta)
Jaan-boojh ke REDUNDANCY wapas daalna (tables merge / data duplicate) → joins kam → **fast reads**.
Cost: redundancy + update-anomaly wapas (maintain karna padta).

## Kab kya (decision)
| Situation | Kya karo |
|-----------|----------|
| Write-heavy, consistency-critical (payments/orders/banking = **OLTP**) | **NORMALIZE** (3NF) — default |
| Read-heavy, speed-critical (analytics/dashboards = **OLAP**) | **DENORMALIZE** — jab joins bottleneck banein |
| Cache / NoSQL | denormalized by design |

## ★ CONNECT (breadth)
- **MongoDB embed = DENORMALIZED** — document me sab ghusa dete → join avoid (see 04_joins.md). Wahi ye concept.
- **HLD scaling:** read-heavy at scale → denormalize + cache (fast read), consistency thoda eventual.

---

## 1-line recall
Redundancy/anomaly (update/insert/delete) → todo tables. 1NF=atomic · 2NF=whole-key · 3NF=nothing-but-key.
"KEY, WHOLE key, NOTHING BUT the key." Normalize=correctness(writes) · Denormalize=speed(reads). Sweet-spot=3NF. Mongo-embed=denormalized.
