# 🟢 Section A1 — Docker Foundation (Day 1)

> **Concepts:** Image vs Container, basic commands, first run
> **Prereq:** None — start here
> **Next:** [02_spring_mysql_host.md](02_spring_mysql_host.md) — Spring on host + MySQL container

📚 [← Back to README](00_README.md)

---

# 📍 PART 1 — Docker Install + Verification

## 🔧 Install Configuration (Docker Desktop)

```
✅ Use WSL 2 instead of Hyper-V (recommended)
   → Lighter than Hyper-V, better performance
   → Standard for Java/Spring development

☐ Allow Windows Containers (UNCHECKED)
   → Sirf legacy .NET apps ke liye
   → Hum Linux containers chalayenge — not needed

✅ Add shortcut to desktop
   → Convenience
```

## ✅ Verify Install

```powershell
docker --version
```

**Expected output:**
```
Docker version 29.4.1, build 055a478
```

```powershell
docker ps
```

**Expected output (empty initially):**
```
CONTAINER ID   IMAGE   COMMAND   CREATED   STATUS   PORTS   NAMES
```

> 🧠 **Concept:** `docker ps` = list of RUNNING containers. Empty = koi nahi chal raha abhi.

---

# 📍 PART 2 — Image vs Container (THE foundation)

## 🎬 Story — Recipe vs Dish

```
RECIPE (paper)                    DISH (actual cake)
   ↓                                     ↓
"Cake banana hai —                = recipe execute karke bana
 flour + butter + sugar,          = real, edible
 180°C, 30 min"                   = ek recipe → 100 cakes
                                    (har ek separate dish)
= blueprint
= read-only
= no actual cake here
= unlimited cakes from same recipe


RECIPE  =  IMAGE       ← read-only template/blueprint
DISH    =  CONTAINER   ← running instance from image
```

## 🎯 Crisp Definitions

```
IMAGE
   • Read-only template
   • Stored on disk (laptop / Docker Hub)
   • Includes: OS layer + Java/MySQL + your jar + config
   • "openjdk:17-jdk-slim" = Java 17 image
   • "mysql:8" = MySQL 8 image
   • Immutable

CONTAINER
   • Running instance OF an image
   • Lives in memory + small writable layer on disk
   • Isolated — apna filesystem, network, process tree
   • Stoppable, deletable, recreatable
   • 1 image → unlimited containers (each isolated)
```

## 🎨 Visual

```
   ┌──────────────────┐
   │  IMAGE           │  ← disk pe pada
   │  mysql:8         │     read-only, ~600 MB
   └────────┬─────────┘
            │ docker run
            ├──────────────┬──────────────┐
            ▼              ▼              ▼
   ┌──────────────┐ ┌──────────────┐ ┌──────────────┐
   │  Container 1 │ │  Container 2 │ │  Container 3 │
   │  Port 3307   │ │  Port 3308   │ │  Port 3309   │
   └──────────────┘ └──────────────┘ └──────────────┘
   3 alag-alag MySQL instances, EK image se
```

## ⚡ Lifecycle Commands

```
docker run image          → naya container START
docker stop  container    → STOPPED (still EXISTS on disk)
docker start container    → wapas chalu (data wahi)
docker rm    container    → DELETED (purr gaya)
docker rmi   image        → IMAGE delete (containers nahi)
```

> 🧠 **Key insight:** Stopped container ≠ deleted. Container has writable layer that persists across stop/start.

## 🆚 Quick Table

| | **Image** | **Container** |
|---|---|---|
| Nature | Read-only template | Running instance |
| Storage | Disk (Docker Hub / local) | Memory + writable layer |
| Mutability | Immutable | Mutable while running |
| Count | 1 image → many containers | Each unique |
| Analogy | Recipe / Class | Dish / Object |

---

# 📍 PART 3 — `docker pull mysql:8`

## Command:
```powershell
docker pull mysql:8
```

## Output (real):
```
8: Pulling from library/mysql
edf85873f64e: Pull complete       ← Layer 1 done
7fb083db72a6: Pull complete       ← Layer 2 done
a995cd42599a: Pull complete       ← Layer 3 done
8528ed92354d: Pull complete       ← Layer 4 done
7640bc9e91df: Pull complete
1b34fede4754: Pull complete
91119dbf849e: Pull complete
32ab7a201068: Pull complete
946e9b141a33: Pull complete
5b19d1ea67df: Download complete
46e91b60ed02: Download complete
Digest: sha256:21d5c55b7f74a97dcbd281dfbb10356a16b1199da956a63b55493418afe2b940
Status: Downloaded newer image for mysql:8
docker.io/library/mysql:8
```

## 🔍 Behind the scenes:

```
1. Docker → Docker Hub (cloud registry) se baat
2. mysql:8 image ke "layers" search
3. Har layer ek tar.gz file:
       ┌── Layer 1: Debian base OS
       ├── Layer 2: glibc, system tools
       ├── Layer 3: MySQL server install
       ├── Layer 4: Default config files
       └── Layer 5: Entrypoint scripts
4. Multiple layers parallel download
5. Extract → verify checksum
6. Local cache mein store
```

## 💡 Why layers? (Cache magic)

```
Tu next time pull "mysql:8.1" karega:
   • Bahut sare layers SAME as mysql:8 (Debian, MySQL binary)
   • Docker WAHI re-use karega — no re-download
   • Sirf NEW layers (config differences) pull
   • = Fast pull, less bandwidth
```

> 🧠 **Concept:** Image = stack of layers. Layer caching = bandwidth + time savings. Layer level pe deduplication.

---

# 📍 PART 4 — `docker run` (PEHLI ATTEMPT — FAILED)

## Command (with port 3306):
```powershell
docker run -d --name mysql-userdb -e MYSQL_ROOT_PASSWORD=rootpass -e MYSQL_DATABASE=userdb -p 3306:3306 mysql:8
```

## Output:
```
89e7c48961c2b4ea6586caa84261856c946e9f87a45e87ce681ad9e66a3e88d2
                     (container ID returned — banaya)

docker: Error response from daemon:
   ports are not available: exposing port TCP 0.0.0.0:3306
   bind: Only one usage of each socket address... is normally permitted
```

## 🔥 Error Decode — PORT CONFLICT

```
HOST LAPTOP                                   DOCKER CONTAINER
─────────────────                             ─────────────
Port 3306 → ALREADY TAKEN (local MySQL)      Port 3306 → MySQL chahta
                ↓
            CONFLICT 💥
            (OS rule: 2 services same port pe nahi)
```

> 🧠 **Mera laptop pe pehle se MySQL service chal rahi thi (kabhi install kiya hoga).**
> Docker container 3306 nahi le sakta kyunki **already taken**.

## 🛠️ Fix — Different host port use kar

```
HOST LAPTOP                                   DOCKER CONTAINER
─────────────────                             ─────────────
Port 3306 → local MySQL ke paas               Port 3306 → MySQL
Port 3307 → free                              chahta
                ↓
            -p 3307:3306 (host:container)
```

**Spring Boot ab `localhost:3307` se connect karega.**

---

# 📍 PART 5 — Cleanup + Re-run (SUCCESS)

## Step A: Failed container delete
```powershell
docker rm mysql-userdb
```

**Output:**
```
mysql-userdb
```

> 🧠 Container created tha but started nahi hua port error ki vajah se. `docker rm` se naam free + container record delete.

## Step B: Re-run with port 3307
```powershell
docker run -d --name mysql-userdb -e MYSQL_ROOT_PASSWORD=rootpass -e MYSQL_DATABASE=userdb -p 3307:3306 mysql:8
```

**Output:**
```
86f87cda5d6abf4308f4994b8289508c704b80ce76ad0b6506c818edca15dd41
                     (container ID — successful start)
```

## Step C: Verify
```powershell
docker ps
```

**Output:**
```
CONTAINER ID   IMAGE     COMMAND                  CREATED         STATUS         PORTS                          NAMES
86f87cda5d6a   mysql:8   "docker-entrypoint.s…"   7 seconds ago   Up 6 seconds   0.0.0.0:3307->3306/tcp         mysql-userdb
```

✅ **Container UP and running**

---

# 📍 PART 6 — `docker run` Flags Deep Dive

```powershell
docker run -d --name mysql-userdb \
           -e MYSQL_ROOT_PASSWORD=rootpass \
           -e MYSQL_DATABASE=userdb \
           -p 3307:3306 \
           mysql:8
```

## Each flag = important Docker concept:

### `-d` — DETACHED mode
```
Without -d:  Container foreground mein → terminal stuck, logs flood
With -d:     Container background → returns ID, terminal free

Like:
   java -jar app.jar          ← terminal stuck
   java -jar app.jar &        ← background (Linux)
```

### `--name mysql-userdb` — Container ka friendly naam
```
Without --name: Random name (e.g., "happy_curie")
With --name:    Easy reference

Examples:
   docker stop mysql-userdb     ← naam se stop
   docker logs mysql-userdb     ← naam se logs
   docker rm   mysql-userdb     ← naam se delete
```

### `-e VAR=value` — Environment variable inject
```
-e MYSQL_ROOT_PASSWORD=rootpass
   ↓
   MySQL image ke startup script ko ye env var milta
   Wo padh ke "root user ka password rootpass set kar do"
   (Documented in MySQL Docker image's README)

-e MYSQL_DATABASE=userdb
   ↓
   "userdb" naam ka database AUTO create kar dena startup pe
```

> 🧠 **Container ko configure karne ka standard way = env vars.**
> Production: secrets, API keys, DB URLs sab env vars se.

### `-p host:container` — PORT MAPPING ⭐ (most important)
```
-p 3307:3306
   ↑    ↑
   host container

Without -p: Container chalega but unreachable from host
With -p:    Host laptop ka 3307 → container ka 3306 forward
```

**Visual:**
```
   YOUR LAPTOP (host)               MYSQL CONTAINER
   ┌─────────────────┐             ┌─────────────────┐
   │ localhost:3307  │ ─── -p ───► │ port 3306       │
   │                 │   (bridge)  │ MySQL listening │
   │ Spring Boot     │             │ DB: userdb      │
   │ connects here   │             │ root: rootpass  │
   └─────────────────┘             └─────────────────┘
```

### `mysql:8` — Image + tag
```
"mysql"   → image name
"8"       → tag (version)
Default tag if skipped = "latest"
```

---

# 📍 PART 7 — `docker ps` Output Decoded

```
CONTAINER ID   → Unique ID (first 12 chars dikha — full = 64 chars)
IMAGE          → Kaunsi image se bana — mysql:8
COMMAND        → Container ke andar kya chal raha
                 "docker-entrypoint.s..." = MySQL startup script
CREATED        → Kab bana — 7 seconds ago
STATUS         → "Up 6 seconds" = ALIVE + healthy
                 Other states:
                   "Exited (0)" = clean stop (no error)
                   "Exited (1)" = crashed
                   "Restarting" = auto-restart loop
PORTS          → 0.0.0.0:3307->3306/tcp
                 host port → container port (TCP protocol)
NAMES          → mysql-userdb (humne diya)
```

## Useful variations:
```powershell
docker ps          → SIRF running containers
docker ps -a       → All (running + stopped + exited)
docker ps -q       → Sirf container IDs (scripting ke liye)
```

---

# 📍 PART 8 — `docker logs` (verify readiness)

```powershell
docker logs mysql-userdb
```

## Why?
```
"Up 6 seconds" status = container chalu hai
But MySQL pehli baar startup mein 15-20 sec lagte:
   • Database initialize
   • root user create
   • userdb database create
   • Listener start

Tab tak Spring Boot connect karega → connection refused
```

## Verify line:
```
[System] [MY-010931] /usr/sbin/mysqld: ready for connections.
                     ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
                     yeh dikhe = MySQL accepting connections ✅
```

> 🧠 **`docker logs <name>` = container ke andar jo console output ho raha, host pe dikhao.**
> Real-time follow: `docker logs -f mysql-userdb`

---

