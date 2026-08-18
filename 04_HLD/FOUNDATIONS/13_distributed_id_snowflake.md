# Distributed ID Generation (Snowflake)

> **HLD Topic 13 — Unique ID across many machines**

---

## STORY — Bank token counter

> Ek bank, ek counter → token: 1, 2, 3... (auto-increment). Simple.
>
> Ab **5 branch** khul gaye. Har branch apna 1,2,3 deta → do customers ko same "5" → **CLASH**.
>
> "Har branch ek central se poochhe" → wo central = **bottleneck + single point of failure**.
>
> Chahiye: har branch **khud** unique token de, bina kisi se poochhe. = distributed ID.

---

## Problem — auto-increment kyun toota

```
Single DB:  AUTO_INCREMENT → 1,2,3...  perfect (ek hi counter)
Multi DB / shards:
   DB-A: 1,2,3    DB-B: 1,2,3   → SAME id CLASH
   "central se poochho" → central = BOTTLENECK + SPOF
```

---

## 3 Options

### 1. UUID (128-bit random)  e.g. `f47ac10b-58cc-4372-a567-...`
- ✓ Har node **khud** bana le (coordination-free), clash ~impossible
- ✗ **Bada** (128-bit) · **random** → DB index unfriendly (sorted nahi → B-tree insert slow) · time-ordered nahi

### 2. Snowflake ID (Twitter, 64-bit)  ⭐ interview-favourite
```
64 bits = [ 1 unused ][ 41 TIMESTAMP ][ 10 MACHINE-ID ][ 12 SEQUENCE ]
             sign        ms since epoch   kaunsa node       same-ms counter
```
- **timestamp (41)** → roughly **time-ordered** (naya > purana) + DB-index friendly
- **machine-id (10)** → har node ka apna (1024 machines) → **do node kabhi clash nahi**
- **sequence (12)** → same machine + same-ms me multiple → 0,1,2... (4096/ms)
- ✓ coordination-free · unique · ~sorted · chhota (fits in `long`)

### 3. DB ticket-server / range
- Central se **BLOCK** lo (1–1000), khatam ho to agla block. Central se baar-baar nahi → bottleneck kam (par central still exist).

---

## Clash kyun nahi hota (core)
```
2 alag machines, same ms  → MACHINE-ID alag → clash nahi
1 machine, same ms, 2 req  → SEQUENCE (alag counter 0,1,2) → clash nahi
alag ms                    → TIMESTAMP alag → clash nahi
```

---

## Interview Talking Points

**Q: "Distributed system me unique ID?"**
> *"Snowflake — 64-bit = timestamp + machine-id + sequence. Coordination-free (har node khud banata), roughly time-ordered (DB-index friendly), machine-id se clash-proof."*

**Q: "UUID vs Snowflake?"**
> *"UUID coordination-free par 128-bit + random → index-unfriendly, un-ordered. Snowflake chhota (64-bit) + roughly sorted + time-ordered → jab ordering/index matter kare, Snowflake behtar."*

**Q: "Auto-increment kyun nahi?"**
> *"Single DB pe theek. Multi-shard me clash (do DB same id) + central-counter banaye to bottleneck/SPOF."*

---

## Power Phrase

> **"Auto-increment distributed me toota (clash / central-bottleneck) → Snowflake 64-bit [timestamp | machine-id | sequence]: coordination-free, unique, roughly-sorted. UUID = coordination-free par bada + random (index-unfriendly)."**

---

## Memory Hook

```
Snowflake 64-bit = timestamp | machine-id | sequence
   timestamp  → time-ordered (index-friendly)
   machine-id → do node clash-proof
   sequence   → same-machine same-ms counter

UUID     = coordination-free par bada + random (index bura)
Ticket   = central se BLOCK (bottleneck kam)

Sharding ka jodidar: har shard/node ko central-free unique ID
```

---

## Trap Box

```
Trap 1: "auto-increment sab jagah"
         multi-shard → clash / central bottleneck → Snowflake
Trap 2: "UUID hamesha best"
         random → DB index slow, un-ordered → Snowflake jab ordering chahiye
Trap 3: "machine-id set karna bhool"
         2 node same machine-id → clash. har node UNIQUE machine-id.
Trap 4: "clock backward (NTP sync)"
         timestamp peeche gaya → same id risk → clock-drift handle karna padta
```
