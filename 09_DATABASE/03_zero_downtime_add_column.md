# Zero-Downtime: Huge Hot Table me Column Add Karna

> **DATABASE Q&A — Topic 03** (backend/HLD classic, JP-relevant)

---

## QUESTION

> Ek table hai jispe **millions of users har second read + write** kar rahe (hot table).
> Ab mujhe usme **ek naya column ADD** karna hai — **NO DOWNTIME**. Kaise?

---

## PROBLEM — naive ALTER kyun khatarnaak

Naive `ALTER TABLE users ADD COLUMN ...` purane DB/versions me:
- **Poori table LOCK** kar deta (reads/writes block) jab tak table **rewrite** hota
- Millions users ke liye = **DOWNTIME**

---

## ANSWER — solutions (chhote-se-bade order me)

### 1. Modern DB — native INSTANT add (chhota/nullable column)
- **MySQL 8.0+:** nullable column **end me** add = **INSTANT** (metadata-only, NO table rewrite).
  ya `ALTER TABLE ... ADD COLUMN ..., ALGORITHM=INPLACE, LOCK=NONE` (writes chalte rehte).
- **PostgreSQL 11+:** nullable / constant-default column add = **instant** (metadata-only).
- **KEY:** **nullable + no-heavy-default = FAST.** **NOT NULL + computed default = full REWRITE (bura).**

### 2. Online-schema-change TOOLS (badi table / purana DB / complex change)
**gh-ost (GitHub)** ya **pt-online-schema-change (Percona):**
1. Ek **shadow/ghost table** banate (original + naya column)
2. Data ko **chhote BATCHES** me background me copy
3. Ongoing writes ko **SYNC** rakhte (pt-osc = triggers, gh-ost = binlog)
4. Catch-up pe **atomic RENAME swap** (near-instant cutover)
5. Original pe reads/writes chalte rehte → **ZERO downtime**

### 3. EXPAND-CONTRACT pattern (safest, app-level — jab NOT NULL/default chahiye)
- **Expand:** column **nullable, no-constraint** add (fast/safe)
- **Backfill:** background job purane rows **BATCHES me** bhare (1 bada UPDATE NAHI -> lock + replication-lag)
- **Migrate:** app naye column pe read/write shift kare
- **Contract:** baad me NOT-NULL/constraint add, ya purana column hatao

---

## KEY DON'Ts
- Huge table pe **NOT NULL + default EK-SHOT me NAHI** (rewrite/lock)
- Backfill **ek bade UPDATE me NAHI** — hamesha **BATCH** (1k-10k rows at a time; warna lock + replica-lag)

---

## POWER LINE (interview)
> *"Add the column as nullable — on MySQL 8 / Postgres 11 that's an instant metadata change, no rewrite. For large tables or backfills, use gh-ost / pt-online-schema-change (shadow table + batched copy + live sync + atomic swap), or the expand-contract pattern: add nullable → backfill in batches → then add constraints. Never a single big ALTER with NOT NULL default on a hot table."*

> **Yaad rakh:**
> nullable-add = instant (modern DB) · badi table = gh-ost/pt-osc (shadow+batch+swap)
> NOT-NULL/default chahiye = expand-contract (nullable -> backfill-batches -> constraint)
> backfill ALWAYS batched, kabhi single-big-UPDATE nahi
