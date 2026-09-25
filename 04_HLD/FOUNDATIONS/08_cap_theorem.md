# CAP Theorem — The Distributed Systems Classic

> **NAV** — KYA: CAP (CP vs AP kab). UP: [MASTER](../00_MASTER_SHEET.md) · trade-off: [strong vs eventual](../TRADEOFFS.md) · lagta hai: [bookmyshow](../SYSTEM_DESIGNS/10_bookmyshow/10_bookmyshow_INTERVIEW.md) · [payment](../SYSTEM_DESIGNS/07_payment_system/07_payment_system.md) · [google-docs](../SYSTEM_DESIGNS/12_google_docs_collab/12_google_docs_collab.md)

> **HLD Topic 8 — Pick 2 of 3 — interview ka favorite**
> Real talk: "Perfect kuch nahi hota" — YouTube/Uber/Facebook sab kahin na kahin trade-off karte

---

## The Theorem (Full Form)

```
DISTRIBUTED SYSTEM mein 3 cheezein chahiye:

   C = CONSISTENCY        (Har read = latest data, har jagah)
   A = AVAILABILITY        (Har request ka jawaab milega)
   P = PARTITION           (Network failure ke baad bhi system chale)
       TOLERANCE

THEOREM SAYS:
   Sirf 2 mil sakte hain. TEEN kabhi nahi.
```

---

## Real-World Truth — "Perfect Kuch Nahi Hota"

```
TUNE SAHI BOLA — every big system somewhere trade-off karta:

   YouTube down ho gaya     → AVAILABILITY lose
   Uber surge flicker       → CONSISTENCY trade
   Facebook stale feed       → CONSISTENCY trade
   Bank "system unavailable" → AVAILABILITY lose

Yeh all CAP THEOREM ke real-world examples.
```

---

## The Triangle (Full Form Visual)

```
                  ┌─────────────────────┐
                  │   CONSISTENCY        │
                  │   (latest data       │
                  │    everywhere)        │
                  └────────┬─────────────┘
                           │
                  ◢━━━━━━━━┴━━━━━━━━◣
                ◢                        ◣
              ◢   PICK ONLY 2 OF 3        ◣
            ◢                              ◣
          ◢                                 ◣
        ◢                                    ◣
   ┌────────────────┐              ┌────────────────────┐
   │  AVAILABILITY  │              │  PARTITION         │
   │  (system       │              │  TOLERANCE         │
   │   responds)    │              │  (survives network │
   │                │              │   failure)         │
   └────────────────┘              └────────────────────┘
```

---

## STORY — Bank Branches Example

### Setup:
```
Bank ke 2 branches: Mumbai + Delhi
Connected via phone (network)
Customer balance = ₹10,000

   ┌────────────┐  phone  ┌────────────┐
   │ Mumbai      │ ◄─────► │ Delhi       │
   │ Bal: 10000  │         │ Bal: 10000  │
   └────────────┘         └────────────┘
   Both consistent
   Both available
   Phone working (no partition)
```

### Phone Breaks (PARTITION):
```
   ┌────────────┐       ┌────────────┐
   │ Mumbai      │ phone │ Delhi       │
   │ Bal: 10000  │   dead   │ Bal: 10000  │
   └────────────┘         └────────────┘

   Customer Delhi mein ₹10,000 withdraw kar leta
   Delhi balance: ₹0
   Mumbai ka balance ABHI BHI 10000 dikha raha
   (sync nahi ho saka)

   Customer Mumbai aata, ₹10,000 withdraw karna chahta...
```

### Bank ka Decision:
```
OPTION CP (Consistency + Partition Tolerance):
   "Sorry, system down — withdraw nahi kar sakte"
   Wrong data nahi serve hua
   Customer ko service nahi mili (AVAILABILITY lost)

OPTION AP (Availability + Partition Tolerance):
   "Withdraw kar lo" → Mumbai bhi ₹10,000 de deta
   Customer ko service mili
   Account ne ₹20,000 nikla while balance ₹10,000 (CONSISTENCY lost)
```

---

## The 3 Combinations Decoded

### **1) Consistency + Partition Tolerance** (CP)
```
Network fails → System REJECTS requests
   Data correct hamesha
   During failure, system DOWN

Real-world:
   • Banking transactions
   • Inventory systems
   • Distributed locks
   • Configuration storage
   = "Wrong answer worse than no answer"
```

### **2) Availability + Partition Tolerance** (AP)
```
Network fails → System keeps responding (might serve stale data)
   Always responds
   Data might be outdated

Real-world:
   • Social media feeds
   • Product catalogs
   • Like/comment counts
   • DNS
   = "Stale answer better than no answer"
```

### **3) Consistency + Availability** (CA)
```
NOT REALLY POSSIBLE in distributed system
   • Distributed = network = partitions WILL happen
   • Only achievable in single-machine system

   = Theoretical only, no real distributed system
```

---

## Big Tech Examples (Real World)

```
SERVICE              CHOICE                       WHY
──────              ──────                        ────
YouTube             AP (video milta rahe)         CDN-served, available > strict
                                                   consistency; stale view-count/
                                                   comment OK (eventual) — Facebook
                                                   feed jaisa hi

Bank/UPI            CP (sometimes "unavailable")  Wrong balance = legal disaster

Facebook Feed       AP (might show stale)          User happy with old feed
                                                   vs error screen

Uber Ride           AP-leaning                    Ride milti rahe, exact
                                                   surge price flicker OK

Twitter Feed        AP                            Stale tweets OK, error not

Google Search       AP                            Slightly outdated results OK

Instagram Likes     AP                            Like count eventually consistent

DynamoDB            AP                            Tunable, mostly availability

MongoDB             CP (default)                  Can be tuned

Cassandra           AP                            Built for availability + scale
```

---

## The "E" — PACELC Theorem (CAP Extended)

CAP only covers PARTITION time. What about NORMAL operation?

```
PACELC = full form:

   P  =  PARTITION (network failure)
   A  =  AVAILABILITY
   C  =  CONSISTENCY
   E  =  ELSE (normal operation, NO partition)  NEW
   L  =  LATENCY (speed)
   C  =  CONSISTENCY
```

**Reading:**
```
When PARTITION happens — pick AVAILABILITY OR CONSISTENCY
ELSE (normal ops)      — pick LATENCY OR CONSISTENCY
```

---

## PACELC Visual

```
   System lifecycle:

   ┌─────────────────────────────────────────────┐
   │  NORMAL OPERATION (no partition)             │
   │  ────────────────                            │
   │  CHOOSE:                                      │
   │     Low LATENCY (fast response)            │
   │           OR                                  │
   │     Strong CONSISTENCY (latest data)       │
   │                                               │
   │  Trade-off because:                           │
   │  Strong consistency = wait for all replicas   │
   │                       = SLOWER                │
   │  Low latency        = respond fast            │
   │                       = stale possible        │
   └─────────────────────────────────────────────┘
            │
            │ network fails
            ▼
   ┌─────────────────────────────────────────────┐
   │  PARTITION (network failure)                 │
   │  ────────────────                            │
   │  CHOOSE:                                      │
   │     AVAILABILITY (keep responding)         │
   │           OR                                  │
   │     CONSISTENCY (refuse if can't sync)     │
   └─────────────────────────────────────────────┘
```

---

## PACELC Real Examples

```
CASSANDRA:
   Partition  → AVAILABILITY (PA)
   Normal ops → LATENCY (EL)
   = "Always respond fast, eventual consistency OK"

MONGODB (default):
   Partition  → CONSISTENCY (PC)
   Normal ops → CONSISTENCY (EC)
   = "Always correct, accept some latency"

DYNAMODB:
   Partition  → AVAILABILITY (PA)
   Normal ops → LATENCY (EL)
   = "Always fast + always available, tunable consistency"

POSTGRES (with sync replication):
   Partition  → CONSISTENCY (PC)
   Normal ops → CONSISTENCY (EC)
   = "Banking-grade correctness, latency accepted"
```

---

## Decision Matrix (Real World)

```
PICK CONSISTENCY (CP) IF:
   Wrong data = serious damage (money, locks, inventory)
   "Better to fail than show wrong info"
   Examples:
      • Banking
      • Payment systems
      • Inventory management
      • Distributed locks
      • Config storage (etcd, K8s)

PICK AVAILABILITY (AP) IF:
   Stale data acceptable (social, ads, recommendations)
   "Better to show old data than error"
   Examples:
      • Social media feeds
      • Product catalogs
      • Like/view counts
      • Search results
      • Content feeds
```

---

## Connect to Existing Knowledge

```
REPLICATION (already done):
   Sync replication  = CP-leaning (wait for all replicas)
   Async replication = AP-leaning (don't wait)

SHARDING (already done):
   Cross-shard queries = CAP trade-offs
   2PC (transactions)  = CP
   Eventual consistency = AP

MESSAGE QUEUES (already done):
   delivery semantics (at-least-once / at-most-once) CAP NAHI hain — wo "message kitni baar
   pahuncha" ka sawaal hai. CAP ka sawaal "partition me broker likhe ya mana kare" hai
   (Kafka: acks=all + min.insync.replicas=2 -> kam replica bache to write MANA = C chuna)

= CAP TIES ALL FOUNDATIONS TOGETHER
```

```
PER-CRITICALITY, not whole-system (Arpan's insight 2026-06-06):
   CAP choice ek poore system pe nahi — DATA ki criticality pe hota.

   FINANCE bhi pura CP nahi:
      money-path (ledger / transaction / balance)  → CP  (galat/lag bardaasht nahi)
      non-critical (statement / report / feed)      → AP  (thoda stale chalega)

   = wahi "money-path SYNC, baaki ASYNC" wala replication insight,
     CAP ki bhasha mein. Ek system mein dono CP aur AP zones ho sakte.
```

---

## ★★ DEPTH-PASS (25-Sep) — CAP asal me kya kehta hai + consistency ki choice kaise maarti hai

> Upar = CP vs AP kab. Ye section = definition ki barikiyaan jo interview me pakadti hain,
> choice asal me kis KNOB se hoti hai (quorum), aur wo tarike jinse ye choice khud system todti hai.

### A. "Pick 2 of 3" poora sach nahi — asli line

```
Network partition HOGA hi (distributed = network). To asli sawaal ek hi hai:
   "PARTITION ke WAQT, jo node doosron se kat gaya — wo jawab de (A) ya mana kare (C)?"
Partition nahi hai to C aur A dono milte hain. Tab ka trade-off LATENCY vs C hai (PACELC).
```

**Teen shabd jinka matlab log galat lete hain:**
```
CAP ka C  = har read ko LATEST likha hua dikhe (jaise ek hi copy ho) = "linearizable"
            ACID ka C NAHI (wo "rules intact" hai) -> dekh: 09_DATABASE/06_acid.md
CAP ka A  = har zinda node har request ka error-free jawab de
            "99.99% uptime" NAHI. CP system bhi 99.99% uptime pe chal sakta hai.
PARTITION = sirf taar katna nahi. GC pause, overloaded node, slow network bhi —
            node ko pata hi nahi chalta "wo MARA ya DHEEMA" -> TIMEOUT tay karta hai
            -> choice yahin hoti hai
```
```
=> "YouTube down ho gaya" = outage hai, CAP nahi. CAP sirf partition ke waqt ki CHOICE hai.
```

### B. Choice asal me kis KNOB se hoti hai — QUORUM (R + W > N)

```
N = kitni copy (replica)       W = kitni copy pe likhna zaroori     R = kitni se padhna

N=3, W=2, R=2   ->  2 + 2 > 3  ->  likhne aur padhne wale set me KAM SE KAM ek common node
                                -> latest hamesha milega  (C side)
N=3, W=1, R=1   ->  1 + 1 < 3  ->  tez, par purana padh sakta  (A / latency side)

         likha:   [n1] [n2]  n3
         padha:        [n2] [n3]      <- n2 dono me = latest mil gaya
```
```
Partition me: QUORUM wale ko 2 node nahi mile -> ERROR (C chuna, A chhoda)
              ONE wala -> jo mila usse jawab (A chuna, purana ho sakta)
=> Cassandra / DynamoDB "tunable" isi ka naam hai — HAR REQUEST pe ye knob.
=> isiliye "Cassandra = AP" label adhoora hai. Sahi jawab: "setting batao".
```

### C. ★★ CHHE TARIKE JINSE CONSISTENCY KI CHOICE KHUD SYSTEM TODTI HAI

```
1. SPLIT BRAIN — do LEADER
   partition hua, dono taraf ne socha "doosra mara" -> dono leader -> dono likh rahe
   -> heal hone pe do alag sach, kaunsa sahi?
   ilaaj: MAJORITY quorum (3 ya 5 node — odd). Jiske paas majority nahi, wo likhna BAND.
          (etcd / ZooKeeper / Raft yahi karte)

2. ZOMBIE LEADER
   leader GC pause me 20 sec atka -> baaki ne naya leader chun liya
   purana jaaga, use pata hi nahi -> purane token se likhne laga
   ilaaj: FENCING TOKEN — har naye leader ka number bada; storage chhote number wala write mana kare

3. AP me dono taraf likha -> merge pe ek write CHUPCHAAP GAYAB
   Last-Write-Wins: jiska timestamp bada wo jeeta
   par do machine ki GHADI alag (clock skew) -> "baad" wala write haar gaya
   ilaaj: version vector / CRDT / app khud merge kare.  PAISA kabhi LWW pe nahi.

4. READ-YOUR-OWN-WRITE toota
   user ne naam badla -> refresh -> read replica se padha, wo peeche -> purana naam
   -> user ne socha fail hua, DOBARA click -> double action (order / payment!)
   ilaaj: apne write ke baad kuch sec LEADER se padho, ya session ko leader se chipkao
   (ye replication notes ka REPLICA LAG hi hai, user ki nazar se)

5. POORA system CP bana diya -> jo chal sakta tha wo bhi band
   partition pe sab mana -> statement / catalog / feed bhi band, jo purane data pe chal jaate
   ilaaj: upar wala PER-CRITICALITY — money-path CP, baaki AP
   (LB ke PANIC MODE wali shakal: aadhi kharabi ko poori mat banao)

6. TIMEOUT galat
   bahut chhota -> har GC pause / network blip = "partition" -> baar-baar failover -> khud hi toofan
   bahut bada  -> sach me mara node pe minute bhar intezaar, users latke
   ilaaj: timeout + lagaataar-fail threshold (LB health check wali hi soch)
```

### D. Asli duniya: bank ATM bhi AP chalte hain — par HAD ke saath

```
ATM ka bank se link kata -> ATM mana nahi karta, CHOTI RAKAM de deta (limit ke andar)
baad me hisaab milaata; zyada nikla to overdraft / penalty
=> inconsistency ko POORA roka nahi, usko SASTA aur SEEMIT bana diya (business rule se)
=> senior jawab: "CP ya AP" ke saath "AP chuna to galti ki keemat kaise bandhi" bhi batao
```

### E. Ek shakal (wahi LB / cache / CDN wali)

```
C bachane ko quorum / leader lagaya  -> galat timeout pe wahi toofan (failover loop, split brain)
A bachane ko sab jagah likhne diya   -> merge pe data chupchaap gaya
=> bachane wali cheez ka apna failure mode. Choice ke saath uska ilaaj bhi bolo.
```

> **BOLNE WALI LINE:** *"CAP ka asli sawaal ye hai ki partition ke waqt kata hua node jawab de ya
> mana kare — baaki waqt trade-off latency vs consistency ka hai (PACELC). Ye choice main poore
> system ke liye nahi, data ke hisaab se leta hoon: ledger pe majority quorum aur fencing (split
> brain aur zombie leader se bachne ko), feed aur catalog pe AP. Aur AP jahan chuna, wahan batata
> hoon ki conflict merge kaise hoga — last-write-wins paise pe kabhi nahi."*

---

## Quick Interview Sense

**Q1: "CAP theorem explain karo?"**

> *"Distributed system mein Consistency, Availability, Partition Tolerance — sirf 2 mil sakte. Real distributed = Partition Tolerance required (network failures inevitable). So choice CP (correctness over availability) ya AP (availability over correctness). Banking = CP, social media = AP. CA = sirf single-machine mein possible."*

**Q2: "MongoDB CP ya AP?"**

> *"Default MongoDB = CP. PACELC mein PC + EC (consistent during partition aur normal). Tunable via read preferences. Use case: banking, inventory — correctness paramount."*

**Q3: "Cassandra ka choice?"**

> *"AP-leaning. PACELC: PA + EL (availability + low latency normally). Tunable consistency level (ONE/QUORUM/ALL). Use: scale-out NoSQL where availability + write throughput > strict consistency."*

**Q4: "Real-world example?"**

> *"Banking — CP (correct balance > available). Social media — AP (feed dikhe > error). YouTube — AP (video available rahe, stale view-count OK). Facebook stale feed — AP. Sab somewhere trade-off karte — perfect distributed system NAHI hota."*

**Q5: "PACELC kya hai?"**

> *"CAP extended. CAP only covers partition. PACELC adds Else case (normal operation): Latency vs Consistency trade-off. MongoDB = PC + EC. Cassandra = PA + EL. Senior signal in interview."*

---

## Power Phrase (full form)

> *"CAP theorem: distributed system mein Consistency, Availability, Partition Tolerance — sirf 2. Real distributed = Partition Tolerance required (network failures inevitable), so choice CP ya AP. Banking = CP, social = AP. PACELC extension covers normal operation: Latency vs Consistency trade-off. MongoDB = PC + EC, Cassandra = PA + EL, DynamoDB = PA + EL. No perfect distributed system — engineering = right trade-off for use case. Connects to Replication (sync = CP, async = AP), Sharding (cross-shard txns = CP), MQ (delivery semantics)."*

---

## Memory Hook

```
CAP = "Pick 2 of 3"
   • CONSISTENCY    (latest data)
   • AVAILABILITY   (always respond)
   • PARTITION      (survive network fail)
     TOLERANCE

Real choice (P always required):
   CP = banking-style (correct over up)
   AP = social-style (up over correct)

PACELC = CAP + Else case
   P-A or P-C   (during Partition)
   E-L or E-C   (Else: normal — Latency vs Consistency)

Truth bomb (LOCK):
   "Perfect kuch nahi hota — sab somewhere trade-off"
   YouTube down, Facebook stale, Uber flicker
   = production reality
```

---

## Trap Box

```
Trap 1: "CA system possible"
         "I want consistency + availability + no partition"
         Distributed = P required (network unreliable)
            CA = single-machine only

Trap 2: "CP = always slow"
         "CP means bad performance"
         CP affects availability during partition only
            Normal ops can be fast

Trap 3: "Eventual consistency = no consistency"
         "AP system unreliable"
         Eventually converges (seconds usually)
            Just not strict instant consistency

Trap 4: "Database is purely CP or AP"
         Black-and-white categorization
         Often tunable (DynamoDB consistency level)
            Per-operation CAP choice possible

Trap 5: "PACELC ignored"
         Only mention CAP in interview
         Senior signal: mention PACELC
            (covers normal ops, not just partition)

Trap 6: "Perfect distributed system exists"
         "Some company has solved CAP"
         NO ONE HAS — fundamental theorem
            Engineering = right trade-off, not avoidance
```
