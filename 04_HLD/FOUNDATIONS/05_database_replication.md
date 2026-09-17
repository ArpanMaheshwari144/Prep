# Database Replication

> **NAV** — KYA: replication (leader/follower, lag, failover). UP: [MASTER](../00_MASTER_SHEET.md) · farak: [replication vs sharding](../TRADEOFFS.md) · lagta hai: [distributed-cache](../SYSTEM_DESIGNS/11_distributed_cache/11_distributed_cache.md) · [message-queue](../SYSTEM_DESIGNS/13_message_queue_kafka/13_message_queue_kafka.md)

> **HLD Topic 5 — Read scaling + High Availability**

---

## STORY — Library Master Copy + Photocopies

> Library mein 1 important book — agar kho gayi, sab kuch gone.
>
> **Without replica:**
> 1 master copy. Phati → data gayab. 1 banda padh sakta at a time.
>
> **With replicas:**
> Master + 5 photocopies. 5 log simultaneously padh sakte. Master phati → photocopies se restore.
>
> **Master = Primary DB**
> **Photocopies = Replicas**

---

## Why Replication?

### Without
```
1M reads/sec → Single DB → crash
   • Single point of failure
   • All reads + writes hit one DB
   • No geographic distribution
```

### With
```
   ┌─────────────┐ writes
   │  PRIMARY DB │ ◄────── App writes
   └──────┬──────┘
          │ replicates
   ┌──────┼─────────┐
   ▼      ▼         ▼
┌─────┐┌─────┐┌─────┐
│Rep-1││Rep-2││Rep-3│ ◄─ App reads
└─────┘└─────┘└─────┘
```

**Benefits:** Read scaling, fault tolerance, geo-distribution, live backup.

---

## ★★ CHAAR WAJAH — aur BAAKI RAASTE KYUN KAM PADE

> Upar ki list yaad karne ki nahi hai. Ye chaar **alag-alag dikkatein** hain — ek naam, chaar kaam.
> Interview me "replication kyun?" poochha jaaye to ye chaar ginwa dena kaafi hai.

```
1. LOAD           padhne wale bahut zyada  ->  saare readers copies pe baant do
                                               leader sirf likhna sambhale

2. LEADER MARA    ek copy ko upar utha do (PROMOTE)
                                               dhandha band nahi hua

3. USER DOOR HAI  ek copy India me, ek US me
                                               India ka banda apne paas se padhe
                                               200 ms  ->  20 ms

4. DISK MAR GAYA  ek machine ki disk jal gayi, data doosri pe zinda
                                               ★ ye LOAD ka mamla hai hi nahi —
                                                 ye DATA BACHANE ka hai
```

### Load ke liye do aur raaste the — dono kam pade

```
A. MACHINE BADI KAR DO  (16 core -> 64 core)
      har machine ki ek CHHAT hoti hai — ek na ek din khatam
      64 ke baad agla size 2x mehnga, 1.3x tez  ->  paisa doob raha hai
      aur badi hone pe bhi wo EK hi machine hai  ->  wo gayi, sab gaya

B. AAGE CACHE LAGA DO
      cache tabhi chalta jab EK HI CHEEZ baar-baar maangi jaaye
         toppers list      -> sab wahi maangte      -> HIT, DB bacha

      par jab har banda ALAG cheez maange
         apna result / apna order / apna profile
         -> har request ek NAYI cheez              -> MISS
         -> miss = seedha DB pe gaya
         -> 10 lakh alag-alag request = 10 lakh baar DB pe

      ★ aur cache AVAILABILITY deta hi nahi:
           cache gira -> DB hi jhelega
           DB gira    -> cache tujhe bacha nahi sakta
```

> **★ NICHOD:** jo load cache kha sakti hai (sabki ek jaisi cheez) wo cache khaayegi;
> jo nahi kha sakti (har bande ki apni cheez) usko **copies pe baantna** padta hai.
> Dono ek doosre ki jagah nahi lete — saath chalte hain.
> (load pehle ALAG karna = [04_caching.md](04_caching.md) ka "pehle load ko alag karo" wala block.)

---

## Master-Slave Architecture

```
   App Servers
        │
   ┌────┴───────┐
   ▼            ▼
WRITES        READS
   │            │
   ▼            ▼
┌──────────┐  ┌──────────┐
│ PRIMARY  │  │ REPLICAS │
│ (write)  │──│ (read)   │
└──────────┘  └──────────┘
```

**Rule:** Writes → Primary only. Reads → any replica.

```properties
# App config
spring.datasource.write-url=jdbc:mysql://primary:3306/db
spring.datasource.read-url=jdbc:mysql://replica:3306/db
```

---

## 2 Replication Modes

### **Synchronous**
```
App → Primary (write)
Primary → ALL replicas
Wait for all confirm
THEN respond to app

Pros: Strong consistency
Cons: Slow writes
Use:  Banking, financial
```

### **Asynchronous** (most common)
```
App → Primary (write)
Primary responds IMMEDIATELY
Replicas update in background

Pros: Fast writes
Cons: Eventual consistency
Use:  Most apps
```

### **Semi-Synchronous** (middle ground)
```
Primary waits for AT LEAST ONE replica.
Compromise speed + safety.
```

---

## Sync vs Async

| | Synchronous | Asynchronous |
|---|---|---|
| Write speed | Slow | Fast |
| Consistency | Strong | Eventual |
| Data loss risk | Zero | Small (lag) |
| Use case | Banking | Most apps |
| Production default | Rare | Common |

---

## Failover

### Automatic (preferred)
```
Health check detects Primary down
   ↓
Promote highest replica → new Primary
   ↓
DNS/config auto-update
   ↓
Service self-heals (~60 sec)
```

**Tools:** AWS RDS Multi-AZ, MongoDB ReplicaSet, MySQL Group Replication.

### Manual
```
Crash → admin alert → manually promote → update config
Downtime: minutes (slow)
```

---

## Failover — Deeper (cascading + self-heal + backup ka "end")

> Sawaal: master mara → replica promote kiya. Par naya master BHI mar jaaye to? End kya hai? Infinite marna?

```
   Nahi — wahi cycle, par system khud ko BHAR leta:

   1 master + 4 replica
      master mara  → R1 promote   (1 master + 3 replica)
      SYSTEM turant nayi replica banata (master se copy) → wapas 4
      R1 mara      → R2 promote   → phir nayi replica add
   = har death ke baad pool wapas full. Endless marna nahi.
```

```
   "END" / asli safety net — 3 layer:
   1. REDUNDANCY  — copies alag machine/rack/region pe → saath marna almost impossible
   2. SELF-HEAL   — har failover ke baad nayi replica add → count maintain
   3. BACKUP      — sab mar bhi jaaye (rare) → snapshot se restore
                    (thodi loss last-backup tak, par data bach jaata)
   = End = stable healthy cluster wapas. Final guarantee = backup.
```

---

## Split-Brain (do master problem — KHATARNAK)

> Sawaal: purana master CRASH ke baad WAPAS zinda ho gaya, par usko nahi pata ki naya master ban chuka. Ab system mein 2 master? Aise kaise chalega?

```
   Chalna NAHI chahiye. Do master = dono writes lene lage →
   data alag-alag → conflict → corruption. Disaster.

   Isliye system 2 master banne hi NAHI deta — 2 tarike:
```

```
   1. QUORUM (majority vote):
      master wahi jise AADHE-SE-ZYADA node maane.
      5 node → naye master ke paas 3 (majority) = woh asli.
      Purana akela laut aaya = 1 (minority) → "tu master nahi" → step down.
      = akela banda master nahi ban sakta.

   2. FENCING (purane ko force isolate / "STONITH"):
      naya master promote karte waqt, purane ko forcibly cut-off →
      woh writes le hi na sake.
```

```
   Purana master wapas aaya → use bola jaata "tu ab REPLICA hai" →
   DEMOTE ho ke naye master ki copy banta, sync kar leta. Shaanti.

   = 2 master kabhi saath active nahi. Quorum/fencing = sirf 1 master.
   FINANCE: split-brain = paise mein corruption = isiliye banks strict
   quorum-based failover rakhte (do-master kabhi nahi).
```

---

## META — No Perfect System (replication ka asli sabak)

```
   "Sab FAIL ho sakta hai"   → 100% sach (failure = norm, exception nahi).
   "Sab RECOVER ho sakta hai" → haan, PAR har recovery ka ek COST/TRADE-OFF:
        - zyada copies = zyada paisa + complexity
        - sync (safe) = slow ;  async (fast) = loss-risk
        - instant failover = quorum/monitoring ka overhead

   = Har problem ka solution hai, par har solution naya cost laata.
     PERFECT (zero-cost, zero-loss) system NAHI hota.

   Engineer ka kaam = solution dhoondna nahi (woh exist karte) —
   apne case ka SAHI TRADE-OFF chunna:
        Finance → cost do, loss/inconsistency mat lo (sync/quorum)
        Social  → thodi lag/loss chalega, par fast + sasta (async)
   (Same sabak jo Caching + Sharding mein bhi aaya — koi perfect system nahi.)
```

---

## Replication Lag (key challenge)

```
T=0:   Primary writes
T=5ms: Replica-1 catches up
T=50ms: Replica-2 still updating

User: POST /profile (update name)
      → Primary
      → Returns success

User: GET /profile (read)
      → Hits Replica-2 (lagging)
      → Returns OLD name
```

### Solutions

#### 1. Read your own writes
Briefly read from primary post-write.

#### 2. Sticky sessions
User-A → same replica always.

#### 3. Force primary for critical reads
Write-after-read paths.

#### 4. Synchronous for critical
Slower but consistent.

---

## ★★ DO ALAG DARD — ek ka ilaaj doosre pe NAHI chalta

> Ye is poore topic ki sabse phisalne wali jagah hai. Dono me "likha hua nahi dikha" hota hai,
> par wajah alag hai — isliye auzaar bhi alag hai. Ek ko doosre ka ilaaj de diya = galat jawab.

```
STALE READ                                  DATA LOSS
----------                                  ---------
sab zinda hai, sab theek hai                LEADER MAR GAYA
bas khabar abhi pahunchi nahi               copies tak khabar gayi hi nahi thi

banda ne likha -> LEADER pe gaya  ✓         banda ne likha -> leader ne "ho gaya" bola  ✓
turant refresh -> COPY pe gaya              bhejne se PEHLE leader mar gaya
copy abhi peeche thi -> PURANA naam         copy upar uthi, uske paas wo likha hai HI NAHI

= DER ka mamla                              = BHAROSE ka mamla
ILAAJ: us bande ko LEADER se padhao         ILAAJ: SYNC / SEMI-SYNC
       (read-your-own-writes)                      (ack ka intezaar karo, tab "ho gaya" bolo)

★ leader-se-padhao yahan kaam NAHI karega:
  leader hi mar gaya hai, aur wo write uske paas ke alawa kahin thi hi nahi.
```

### "Theek hai, to SAARE reads leader pe hi bhej do?"

Ye sawaal interviewer lagbhag hamesha poochta hai. Jawab do line ka hai:

```
padhne wale  ~100    :    likhne wale  ~1

saare reads leader pe bhej diye
   ->  tu wapas EK machine pe aa gaya
   ->  jo dikkat copies ne hatayi thi, wahi laut aayi
   ->  aur leader ab SPOF bhi hai
```

Isliye **poora nahi, sirf zaroorat bhar:**

```
tu ne abhi likha?      ->  agle ~5 second TERE reads leader pe   (sirf tere)
baaki duniya           ->  copies pe hi
5 second baad tu bhi   ->  copies pe wapas

leader pe load THODA badha — poora nahi aaya.
```

> Ye wo faisla hai jo interview me achha lagta hai: **sabke liye mehnga ilaaj nahi kiya,
> sirf jisko zaroorat thi usko diya.**

### Async ya Sync — poore system pe ek nahi, DATA dekh ke

```
profile ka naam / like / comment   ->  ASYNC
                                       kho gaya to buri baat, duniya nahi rukti
                                       har write pe banda rokna mehnga sauda

paisa / order / booking            ->  SEMI-SYNC
                                       ek copy ka ack maango (teeno ka nahi)
                                       "Paid" dikha ke paisa gayab = maafi nahi
```

> **BOLNE WALI LINE:** *"Replication async rakhunga, kyunki reads bahut zyada hain aur har write
> pe user ko rokna nahi chahta. Iski keemat ye hai ki leader crash pe aakhri kuch writes ja
> sakti hain — isliye paise wale raaste pe semi-sync karunga, wahan ek ack ka intezaar sasta hai."*

---

## Real-World Tools

### **MySQL/PostgreSQL**
- Built-in master-slave
- Async by default
- Tools: MySQL Group Replication, PG streaming

### **MongoDB**
- Replica Sets (1 primary + secondaries)
- Auto-failover
- Quorum-based reads

### **AWS RDS Multi-AZ**
- Managed sync replication
- Auto-failover ~60 sec
- Hides complexity

### **Cassandra**
- Multi-master peer-to-peer
- Tunable consistency (quorum)

---

## Master-Master (Advanced)

```
   ┌────────┐  ◄────►  ┌────────┐
   │ Master │ replicate│ Master │
   │   1    │          │   2    │
   │(write) │          │(write) │
   └────────┘          └────────┘
```

**Pros:** No single bottleneck
**Cons:** Conflict resolution HARD (both write same row)
**Use:** Multi-region active (rare, complex)

---

## ★★ KIS DESIGN ME, AUR KYUN — (list ratne ki nahi, NIYAM lagane ki)

> Ye table yaad karne ke liye nahi hai. Upar wala niyam lagao, jawab khud nikal aata hai:

```
READ ka load bhaari hai        ->  replicas  (load baant do)
system ka ZINDA rehna zaroori  ->  replicas  (leader mare to koi upar aaye)
```

```
DESIGN                  replication kis KAAM aayi          kaun sa mode
--------------------------------------------------------------------------
url shortener           padhna ~100x zyada                 async
                        -> READ ka load

twitter feed            padhna bahut zyada                 async
                        -> READ ka load

news aggregator         sab wahi khabar padh rahe          async
                        -> READ ka load

payment system          padhna kam hai                     ★ SEMI-SYNC
                        -> READ ke liye NAHI                 (paisa kho nahi sakta)
                        -> DATA BACHANE + failover ke liye

bookmyshow              ★ replication se THEEK HOTA HI NAHI
stock broker            ek hi seat / ek hi symbol pe 500 log
                        = WRITE ki ladai hai, read ki nahi
                        -> ilaaj: LEADER pe atomic update / lock / single-thread
                        (replication yahan sirf failover ke liye rehti hai)
```

### ★ DO SEEKH (yahi asli maal hai)

```
1. ek naam, do bilkul alag kaam:
      READ ka load utaana      (url, twitter, news)
      DATA/uptime bachana      (payment — yahan read-load hai hi nahi)

2. replication WRITE ki ladai theek NAHI karti.
      do log ek hi row pe lade -> copies badhane se kuch nahi hoga
      (aur copies badhao to lag aur badhega)
      -> isliye bookmyshow me lock / atomic conditional UPDATE aaya tha, replica nahi
```

> Ye wahi cheez hai jo [TRADEOFFS](../TRADEOFFS.md) me "replication vs sharding" ke saath padhni hai:
> **replication = ek hi data ki kai copies (read + survival)** ·
> **sharding = alag-alag data alag machine pe (data bada hai)**. Dono alag dikkat ke ilaaj hain.

---

## Interview Talking Points

**Q: "DB replication kya?"**

> *"Primary + multiple replicas — primary writes, replicas reads. Read scaling + fault tolerance + geo-distribution. Master-Slave common."*

**Q: "Sync vs Async?"**

> *"Sync — primary waits all replicas confirm. Strong consistency, slow writes. Banking. Async — primary responds immediately, replicas update background. Fast, eventual consistency. Most apps async."*

**Q: "Replication lag handle?"**

> *"Lag = replica behind primary by ms-sec. Solutions: read own writes (post-write read primary), sticky sessions, sync for critical paths. Trade-off: latency vs consistency."*

**Q: "Failover?"**

> *"Primary crash → health check → auto-promote replica → DNS update → ~60 sec recovery. AWS RDS Multi-AZ managed yahi karti."*

---

## Power Phrase

> **"Replication = Primary + read replicas. Read scaling + HA. Sync (slow, consistent) vs Async (fast, eventual). Replication lag main challenge — read-own-writes or sticky sessions fix. AWS RDS Multi-AZ production default."**

---

## Memory Hook

```
Replication = "Library master + photocopies"
   • Master = writes
   • Replicas = reads
   • Phir backup + HA + geo

Modes:
   Sync       → wait all replicas (slow, consistent)
   Async      → respond immediately (fast, eventual) — most apps
   Semi-sync  → wait one replica (middle)

Lag fixes:
   1. Read own writes (post-write → primary)
   2. Sticky sessions
   3. Sync for critical
   4. Quorum reads

Failover:
   Auto-promote replica → DNS update → ~60 sec
   Tools: AWS RDS Multi-AZ, MongoDB ReplicaSet

Master-Master:
   Both writable but conflict hell — avoid
```

---

## Trap Box

```
Trap 1: "Reads always from replica"
         Replication lag — user might miss own write
         Critical reads → primary

Trap 2: "Sync = always safe"
         All replicas fail = primary blocked
         Quorum-based (n/2 + 1) better

Trap 3: "Master-master > master-slave"
         Conflict resolution complex
         Master-slave simpler, sufficient

Trap 4: "Replicas = backup"
         Replicas LIVE — accidental delete propagates
         Replicas + REAL snapshots both needed

Trap 5: "Async = data loss certain"
         Lag is ms — practically negligible
         For non-critical fine; banking use sync
```
