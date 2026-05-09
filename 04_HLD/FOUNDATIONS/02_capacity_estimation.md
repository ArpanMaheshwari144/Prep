# 📊 Capacity Estimation — Back-of-Envelope Math

> **HLD Topic 2 — First Weapon (Day 1)**

---

## 🎬 STORY — Restaurant Capacity

> Tu **restaurant kholne** chal raha:
>
> 🍽️ Pehla question: *"Kitne customer ek din mein expect karu?"*
> - 100/day → 1 chef, 5 tables
> - 10,000/day → 10 chefs, 50 tables, 3 kitchens, parking lot
>
> **Bina yeh math kiye** — *"chef rakh lo, dekha jayega"* → **chaos.**
>
> Software mein same — *"server lagao 1, dekha jayega"* → **crash at 10K users.**
>
> **HLD mein har question ka pehla step:** numbers calculate karo.

---

## 🎯 Why Capacity Estimation?

Interview mein **"Design Twitter"** pucha jaye — agla question:
> *"Kitne users? Kitne tweets per day? Kitna storage? Kitne servers?"*

**Without numbers, design abstract reh jata.** Numbers se concrete decisions:
- "1 server enough" vs "100 servers chahiye"
- "Cache lagao" vs "Direct DB OK"
- "Sharding zaroori" vs "Single DB chalegi"

---

## 🧮 5 Magic Numbers — Memorize

```
1 day      = 86,400 seconds (~10^5)        ← MOST IMPORTANT
1 month    = ~2.5 million seconds (~10^6)
1 year     = ~30 million seconds (~10^7.5)

Storage units:
1 KB = 10^3 bytes
1 MB = 10^6 bytes
1 GB = 10^9 bytes
1 TB = 10^12 bytes
1 PB = 10^15 bytes
```

**Trick:** **1 day = 10^5 seconds** — most calculations isse start.

---

## 🎨 Example — Twitter Estimation

**Step 1: DAU**
```
Total users:       ~500M
Daily active:      ~200M (40%)
```

**Step 2: Tweets per day**
```
Per user/day:      2 tweets
Total/day:         200M × 2 = 400M tweets
```

**Step 3: TPS (writes)**
```
TPS = 400M / 10^5 = 4,000 tweets/sec
```

**Step 4: Reads (much higher)**
```
Each tweet read by ~100 followers
Reads/sec = 4K × 100 = 400,000 reads/sec
```

**Step 5: Storage**
```
Tweet size: ~1 KB (text + metadata)
Daily:      400M × 1KB = 400 GB/day
Yearly:     400 GB × 365 = ~150 TB/year
```

**Step 6: Bandwidth**
```
Read TPS × size = 400K × 1KB = 400 MB/sec
Convert to Gbps: 3.2 Gbps read bandwidth
```

---

## 🎨 Visual — 4 Things to Calculate

```
┌─────────────────────────────────────────────┐
│   1. TRAFFIC          2. STORAGE             │
│   • DAU                • Per record size     │
│   • Requests/day       • Daily storage       │
│   • RPS / TPS          • Yearly storage      │
│                                              │
│   3. BANDWIDTH        4. MEMORY              │
│   • Read MB/s          • Cache size          │
│   • Write MB/s         • Hot data %          │
└─────────────────────────────────────────────┘
```

---

## 💡 Common Assumptions Cheat Sheet

```
Read:Write ratios:
   Twitter/Instagram:    100:1 (read-heavy)
   Logging/Analytics:    1:100 (write-heavy)
   E-commerce:           10:1

Daily active percentages:
   Social media:         30-40%
   E-commerce:           5-10%
   Banking:              20-30%

Per-user activity:
   Twitter posts:        1-2/day
   Instagram views:      ~50/day
   E-commerce orders:    ~0.05/day
```

---

## 🎤 Interview Approach

When asked *"Design X — capacity estimate"*:

```
1. Clarify: "Kitne total users?"
2. Calculate DAU (~30% of total)
3. Per user activity (assume X actions/day)
4. RPS = (DAU × actions) / 100,000
5. Storage = (records × size) × 365
6. Bandwidth = TPS × payload
7. Note read:write ratio (drives caching)
```

**Key: Assumptions BOLO explicitly.** Round numbers OK. Powers of 10 (100K, 1M, 10M).

---

## 🎯 Quick Practice — WhatsApp

```
Total users:           ~2B
DAU:                   ~1.5B
Messages/user/day:     ~50
Total messages/day:    1.5B × 50 = 75B
Per second:            75B / 10^5 = 750K msg/sec
Avg msg size:          ~100 bytes
Daily bandwidth:       7.5 TB/day
Yearly storage:        ~2.7 PB/year
```

**Massive scale** — sharding + distributed storage + queues mandatory.

---

## 💎 Power Phrase

> **"Capacity estimation = HLD ka first step. Numbers calculate karo (DAU, RPS, storage, bandwidth) — phir architecture decisions concrete bante. 1 day = 10^5 seconds — most math iss se start."**

---

## 🧠 Memory Hook

```
4 things to calculate:
   1. TRAFFIC    (DAU, RPS/TPS)
   2. STORAGE    (per record × records × time)
   3. BANDWIDTH  (TPS × payload size)
   4. MEMORY     (cache size, hot data %)

Magic numbers:
   1 day = 10^5 sec    ← the key
   1 GB  = 10^9 bytes
   1 TB  = 10^12 bytes

Process:
   Total users → DAU (30%) → activity/day → 
   /10^5 = TPS → multiply by size = storage/bandwidth
```

---

## ⚠️ Trap Box

```
🪤 Trap 1: "Exact numbers calculate karna"
         ❌ Calculator nikalna
         ✅ Round powers of 10 (100K, 1M, 10M)

🪤 Trap 2: "Sirf write load consider"
         ❌ Reads ignore
         ✅ Reads typically 10-100x writes — both calculate

🪤 Trap 3: "Year 1 numbers se design"
         ❌ 3-5 year growth ignore
         ✅ Future projections plan for

🪤 Trap 4: "Storage forget metadata + indexes + replication"
         ❌ Pure data size
         ✅ +metadata (~3x base data common)
```
