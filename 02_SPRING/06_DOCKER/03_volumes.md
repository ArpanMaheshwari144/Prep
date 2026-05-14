# 🟢 Section A3 — Volumes (Data Persistence)

> **Why:** Container delete = data gone (default). Volumes solve this.
> **Demo:** Live disaster + recovery proof

📚 [← Back to README](00_README.md) | [← Spring+MySQL](02_spring_mysql_host.md) | [Transition →](04_transition.md)

---

# 📍 PART 19 — VOLUMES (Data Persistence) — LIVE DEMO PROOF

## 🎬 STORY — Filing Cabinet vs Paper-on-Desk

```
   ❌ WITHOUT VOLUME                    ✅ WITH VOLUME
   ────────────────────                 ─────────────────────
   Paper on desk                        Filing cabinet (separate)
                                            ↑
   Desk hata diya?                      Desk hata diya?
       ↓                                    ↓
   Paper bhi gaya 💀                    Cabinet ZINDA
                                            ↓
                                       Naya desk lao
                                            ↓
                                       Cabinet wapas connect
                                            ↓
                                       Paper SAFE ✅
```

```
Container = DESK (temporary, replaceable)
Volume    = FILING CABINET (permanent, separate)
```

---

## 🎯 THE BIG PICTURE — Image vs Container vs Volume

```
   ┌────────────────────────────────────────────────────────────┐
   │                                                              │
   │   IMAGE          CONTAINER         VOLUME                    │
   │   (recipe)       (running)         (storage)                 │
   │                                                              │
   │   ┌─────┐        ┌──────────┐      ┌──────────────┐          │
   │   │     │        │          │      │              │          │
   │   │ jar │───────▶│ running  │      │ persistent   │          │
   │   │ os  │ "run"  │ instance │◀────▶│ data store   │          │
   │   │ deps│        │ (process)│ "-v" │ (host disk)  │          │
   │   │     │        │          │      │              │          │
   │   └─────┘        └──────────┘      └──────────────┘          │
   │                                                              │
   │   Read-only      Mutable while     Independent of           │
   │   Disk           running           container lifecycle       │
   │   GBs            Memory + few MB   GBs (data)               │
   │   Many ←─ 1      One per run       1 → many containers       │
   │   Long-lived     Ephemeral         Long-lived                │
   │                                                              │
   └────────────────────────────────────────────────────────────┘

   Lifecycle:
      Image:     build/pull → push → pull (rare delete)
      Container: run → stop → start → rm (frequent)
      Volume:    create → mount → unmount → rm (rare delete)

   Data flow (with volume):
      App writes → Container path /var/lib/mysql 
                → Mounted to volume mysql_data
                → Lands on host disk (persistent)
```

---

## 🚨 LIVE DEMO — Disaster + Recovery (jo kiya)

### 🎬 Phase 1: WITHOUT volume — proves data dies

```
   ┌─────────────────────────────┐
   │  CONTAINER (Arpan ka data)  │
   │  ┌────────────────────┐     │
   │  │ /var/lib/mysql     │     │
   │  │ • users (Arpan)    │     │
   │  │ • refresh_tokens   │     │
   │  └────────────────────┘     │
   │  Data INSIDE container       │
   └──────────┬──────────────────┘
              │
              │  docker rm -f mysql-userdb
              ▼
        ┌──────────┐
        │  💀💀💀  │
        │ ALL GONE │
        └──────────┘
```

```
   Naya container WITHOUT volume:
      docker run -d --name mysql-userdb ... mysql:8
   
   ┌─────────────────────────────┐
   │  NEW CONTAINER (different ID)│
   │  ┌────────────────────┐     │
   │  │ /var/lib/mysql     │     │
   │  │ EMPTY              │     │
   │  └────────────────────┘     │
   └─────────────────────────────┘
   
   docker exec → SHOW TABLES;
      → Empty set        ← PROOF: container delete = data gone
```

---

### 🎬 Phase 2: WITH volume — proves data survives

```
                    ┌──────────────────┐
                    │  VOLUME          │
                    │  mysql_data      │
                    │  (host disk)     │
                    │  ┌────────────┐  │
                    │  │ users      │  │
                    │  │ • Arpan    │  │
                    │  │ refresh_   │  │
                    │  │ tokens     │  │
                    │  └────────────┘  │
                    └──────────────────┘
                            ▲
                            │ -v mysql_data:/var/lib/mysql
                            │
   ┌─────────────────────────────┐
   │  CONTAINER (with -v flag)   │
   │  /var/lib/mysql ────────────┘
   │  All writes → volume         │
   └──────────┬──────────────────┘
              │
              │  docker rm -f mysql-userdb
              ▼
        ┌──────────┐
        │ Container│
        │  GONE 💀 │
        └──────────┘
                            ┌──────────────────┐
                            │  VOLUME          │
                            │  mysql_data      │
                            │  ✅ STILL ALIVE  │
                            │  Data INTACT     │
                            └──────────────────┘
```

```
   Naya container WITH SAME volume:
      docker run ... -v mysql_data:/var/lib/mysql ... mysql:8
   
                    ┌──────────────────┐
                    │  VOLUME          │
                    │  mysql_data      │
                    │  ✅ Arpan inside │
                    └──────────────────┘
                            ▲
                            │ same -v reattach
                            │
   ┌─────────────────────────────┐
   │  NEW CONTAINER (different ID)│
   │  /var/lib/mysql ────────────┘
   │  Reads existing data         │
   └─────────────────────────────┘
   
   docker exec → SELECT * FROM users;
      → Arpan (id=1, age=27)   ← VOLUME SAVED THE DAY ✅
```

---

## 📋 Volume Commands (sab kuch jo kiya/karenge)

```cmd
REM Create named volume
docker volume create mysql_data

REM List all volumes (named + anonymous)
docker volume ls

REM Inspect volume (details: location, mount points, etc.)
docker volume inspect mysql_data

REM Delete a specific volume (must have no containers using it)
docker volume rm mysql_data

REM Cleanup ALL unused (orphaned) volumes
docker volume prune -f

REM Run container WITH volume mount
docker run -d --name mysql-userdb ^
   -e MYSQL_ROOT_PASSWORD=rootpass ^
   -e MYSQL_DATABASE=userdb ^
   -p 3307:3306 ^
   -v mysql_data:/var/lib/mysql ^
   mysql:8
```

### Volume mount syntax variations:

```
-v mysql_data:/var/lib/mysql              → Named volume
-v C:\my\data:/var/lib/mysql              → Bind mount (host folder)
-v /var/lib/mysql                         → Anonymous volume (auto-name)

Modern syntax (--mount, more verbose):
--mount type=volume,source=mysql_data,target=/var/lib/mysql
--mount type=bind,source=C:\my\data,target=/var/lib/mysql
```

---

## 🎨 3 Volume Types (visual)

```
┌─────────────────────────────────────────────────────────────────┐
│  1. NAMED VOLUME (most common, recommended)                     │
│                                                                  │
│     docker run -v mysql_data:/var/lib/mysql ...                 │
│                                                                  │
│     ┌──────────────────┐                                        │
│     │ Docker manages   │  → easy reuse, easy backup             │
│     │ Location: hidden │  → portable across hosts               │
│     │ Name: friendly   │  → BEST for production DBs             │
│     └──────────────────┘                                        │
│                                                                  │
├─────────────────────────────────────────────────────────────────┤
│  2. BIND MOUNT (host folder ↔ container path)                   │
│                                                                  │
│     docker run -v C:\my\data:/var/lib/mysql ...                 │
│                                                                  │
│     ┌──────────────────┐                                        │
│     │ Direct host path │  → useful for DEV (code reload)        │
│     │ You manage       │  → less portable                        │
│     │ Real folder      │  → IDE can edit while running           │
│     └──────────────────┘                                        │
│                                                                  │
├─────────────────────────────────────────────────────────────────┤
│  3. TMPFS (RAM-based, ephemeral)                                │
│                                                                  │
│     docker run --tmpfs /tmp ...                                 │
│                                                                  │
│     ┌──────────────────┐                                        │
│     │ Memory only      │  → super fast                          │
│     │ Lost on restart  │  → for sensitive temp data             │
│     │ No disk I/O      │  → caches, scratch                     │
│     └──────────────────┘                                        │
└─────────────────────────────────────────────────────────────────┘
```

---

## 💀 Anonymous Volumes Trap (tu jo dikha)

```
   docker run mysql:8                    (no -v flag)
                ↓
   mysql image's Dockerfile says:
      VOLUME /var/lib/mysql
                ↓
   Docker AUTO-CREATES anonymous volume
      → Random hash name: 38aaeb36f1acd232f0eb37e53b3eb1428b9d76d80f5...
                ↓
   Container delete:
      → Container gone
      → Anonymous volume ORPHANED (still exists, hard to reuse)
                ↓
   Tu 3 baar mysql:8 chala without -v:
      → 3 anonymous volumes piled up (visible in `docker volume ls`)
```

**Cleanup:**
```cmd
docker volume prune -f       REM Removes all unused (anonymous orphans)
                              REM mysql_data SURVIVES (in use by container)
```

---

## 🆚 Lifecycle Visual — All 3 Together

```
   IMAGE                                                            
   │                                                                
   │ docker run                                                     
   ▼                                                                
   CONTAINER ◄──── -v ────► VOLUME                                  
   │ (running)              │ (persistent)                           
   │                        │                                        
   │ docker stop            │                                        
   ▼                        │                                        
   STOPPED                  │ (still alive, container stopped)        
   │                        │                                        
   │ docker rm              │                                        
   ▼                        │                                        
   GONE 💀                  │ (still alive, container deleted)        
                            │                                        
                            │ docker run -v same_volume ...           
                            ▼                                        
   NEW CONTAINER ◄──── -v ────► SAME VOLUME (data intact)           
                                                                     
                                                                     
   Volume independent of container lifecycle.                       
   Container = process, Volume = data file (separate concerns).     
```

---

## 🎤 Interview Q&A — Volumes

**Q: "Container delete pe data preserve kaise?"**

> *"Volumes use karte. Named volume create karo (`docker volume create X`), phir container mein mount karo (`-v X:/path/in/container`). Container delete = volume zinda. Same volume naye container mein mount = data wahi."*

**Q: "Named volume vs Bind mount?"**

> *"Named volume = Docker-managed, portable, hidden location, BEST for production DBs. Bind mount = direct host folder ↔ container path, useful for dev (code hot-reload), less portable, manual host path."*

**Q: "Anonymous volumes kya hain?"**

> *"`docker run` mein -v specify nahi karte but image ka Dockerfile `VOLUME /path` declare karta — Docker auto-create anonymous volume (random hash). Container delete pe orphan rehta. Cleanup: `docker volume prune -f`."*

**Q: "Multiple containers same volume share kar sakte?"**

> *"Haan — same volume multiple containers mein mount possible. Use case: read-only data sharing, or app + DB backup container. Concurrent writes risky for stateful data — care needed."*

---

## ⚠️ Volume Traps (4 new)

```
🪤 Trap 12: Mount path container ke andar mismatch
         ❌ -v mysql_data:/var/mysql      (wrong path)
         ✅ -v mysql_data:/var/lib/mysql  (MySQL ka actual data path)
            Image documentation se exact path le

🪤 Trap 13: Volume + container delete cleanup
         ❌ docker rm container → volume orphan ho jata
         ✅ docker rm -v container        (volume bhi delete)
            Or: explicit `docker volume rm X` baad mein

🪤 Trap 14: Permissions issue (Linux containers)
         ❌ Bind mount Windows folder → Linux container 
            permission errors (UID mismatch)
         ✅ Named volume preferred (Docker handles permissions)
            OR set proper UID in Dockerfile

🪤 Trap 15: Volume stale data trap
         ❌ Volume mein purana DB schema, naya app version
            → migration error
         ✅ Volume + DB version sync rakho
            Reset: `docker volume rm X` + recreate
```

---

## ✅ Volumes Concepts LOCKED (today)

```
✅ Container ephemeral (data dies with container by default)
✅ Volume = persistent, container-independent storage
✅ Named volume vs Bind mount vs tmpfs (3 types)
✅ Volume lifecycle (create → mount → unmount → rm)
✅ docker volume create / ls / inspect / rm / prune
✅ -v volume_name:/container/path mount syntax
✅ Anonymous volumes auto-created (Dockerfile VOLUME directive)
✅ Volume orphan cleanup (docker volume prune)
✅ Same volume → multiple containers (data sharing/migration)
✅ Production rule: ANY DB / persistent data → ALWAYS volume
✅ Visual proof: container delete + recreate + same volume = data intact
```

---

## 🎯 NEXT (after Volumes — pending Docker concepts)

```
🔜 NETWORKS
   • Multi-container talk
   • Bridge / host / overlay
   • Container name as hostname

🔜 DOCKER-COMPOSE
   • Multi-service in one YAML (Spring Boot + MySQL + Redis)
   • One command up/down all
   • Production-like local setup

🔜 ENTRYPOINT vs CMD
   • Interview classic
   • Difference + when to use which

🔜 MULTI-STAGE BUILDS
   • Final image small (build deps removed)
   • Production must

🔜 DOCKER REGISTRY (push/pull)
   • Docker Hub
   • Private registries

🔜 K8s (after Docker fully done)
```

---

## 💎 ENHANCED POWER PHRASE (with Volumes)

> *"Docker = Image (read-only template) + Container (running instance) + Volume (persistent storage). Container ephemeral by default — delete = data gone. Volume independent — container lifecycle se alag. Production rule: ANY data persistence MUST go in volume (`-v volume_name:/container/path`). Same volume reuse karke naye container ko same data se chalu kar sakte. Named volume Docker-managed (production), bind mount host folder direct (dev), tmpfs RAM-only (sensitive temp). Anonymous volumes (Dockerfile VOLUME directive) auto-create — orphan trap, prune se cleanup. Containers + volumes together = persistent + portable production stack."*

---
