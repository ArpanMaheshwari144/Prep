# Database Replication

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
