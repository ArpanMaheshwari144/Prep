# 🐳 Docker — Hands-On Journey (UserCRUD Project)

> **Date:** 2026-05-05
> **Goal:** Real MySQL container + Spring Boot UserCRUD connect
> **Mode:** Hands-on, step-by-step, har concept lock with REAL command output

---

## 🎯 OVERVIEW — Aaj Kya Hua

```
1. Docker Desktop install + verify
2. MySQL 8 image pull (Docker Hub se)
3. MySQL container run karna (PORT CONFLICT error + fix)
4. Spring Boot UserCRUD ko H2 se MySQL pe switch
5. pom.xml mein MySQL connector add
6. application.properties update (4 lines change)
7. End: Spring Boot ↔ Docker MySQL container live connection
```

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

# 📍 PART 9 — `pom.xml` Update (MySQL Connector)

## Added dependency:

```xml
<dependency>
    <groupId>com.mysql</groupId>
    <artifactId>mysql-connector-j</artifactId>
    <scope>runtime</scope>
</dependency>
```

## 🧠 Why mysql-connector-j (not -java)?
```
"mysql-connector-java" → DEPRECATED (since 8.0.31)
"mysql-connector-j"    → NEW official artifact name

Dono same library — sirf naam refresh
Spring Boot manages version (no <version> tag)
scope = runtime → compile time nahi chahiye, runtime pe load
```

## H2 status:
```
H2 dependency RAKHA hai (scope: runtime)
   → Tests run karne ke liye useful
   → Future fallback ke liye
   → Use nahi karenge actively (MySQL primary now)
```

---

# 📍 PART 10 — `application.properties` Switch

## 🎯 Concept first — DB switch karne ke liye 4 cheezein:

```
1. URL          → Database kahaan hai (host:port/dbname)
2. Driver       → Java se DB tak baat karne ki library
3. Credentials  → Username + password
4. Dialect      → Hibernate ko DB ka SQL flavor
```

**Bus yeh 4 change. App code mein ek line nahi badli — yahi JPA ka power.**

## Before (H2 — in-memory fake):

```properties
spring.datasource.url=jdbc:h2:mem:userdb
spring.datasource.driverClassName=org.h2.Driver
spring.datasource.username=sa
spring.datasource.password=
spring.jpa.database-platform=org.hibernate.dialect.H2Dialect
spring.h2.console.enabled=true
```

## After (MySQL — real, Docker container):

```properties
spring.datasource.url=jdbc:mysql://localhost:3307/userdb
spring.datasource.driverClassName=com.mysql.cj.jdbc.Driver
spring.datasource.username=root
spring.datasource.password=rootpass
spring.jpa.database-platform=org.hibernate.dialect.MySQLDialect
# Note: MySQL8Dialect REMOVED in Hibernate 7 (Spring Boot 4 uses Hibernate 7.2)
# Use MySQLDialect — auto-detects version from JDBC connection
# (Detail in PART 12 below)
# h2.console removed — not used anymore
```

## 🔍 Each line decoded:

```properties
spring.datasource.url=jdbc:mysql://localhost:3307/userdb
                       ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
                       jdbc:mysql:    → JDBC protocol + MySQL
                       //localhost:3307 → tera laptop ka 3307
                       /userdb        → database name (auto-created)

spring.datasource.driverClassName=com.mysql.cj.jdbc.Driver
                                  ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
                                  "cj" = Connector/J (MySQL Java driver)
                                  Yeh class pom.xml dependency mein hai

spring.datasource.username=root        ← docker run mein set
spring.datasource.password=rootpass    ← MYSQL_ROOT_PASSWORD value

spring.jpa.database-platform=org.hibernate.dialect.MySQLDialect
                                                    ↑↑↑↑↑↑↑↑↑↑↑↑↑↑
                                                    Hibernate ko bola
                                                    "MySQL ka SQL flavor use kar"
                                                    (universal — auto-detects 5.7/8.x)
```

---

# 🎨 BIG PICTURE — Connection Chain

```
   YOUR SPRING BOOT APP                    DOCKER CONTAINER
   ┌──────────────────────┐                ┌──────────────────┐
   │ application.properties│                │  mysql-userdb    │
   │                      │                 │  ┌────────────┐  │
   │ url: localhost:3307  │ ─── JDBC ────► │  │ MySQL:3306 │  │
   │ user: root           │   connection   │  │            │  │
   │ pass: rootpass       │                 │  │ DB: userdb │  │
   └──────────────────────┘                 │  └────────────┘  │
                                            └──────────────────┘
   App ke andar (Java code)                 -p 3307:3306 mapping
                                            (host:container port)
```

```
Spring Boot startup:
   1. App padhta application.properties
   2. URL: "MySQL pe localhost:3307 connect karu"
   3. Docker container 3307 pe listen kar raha (-p mapping ki vajah se)
   4. Connection bani → app + container baat karne lage
   5. Hibernate tables AUTO create karega (ddl-auto=update):
         users, refresh_token
   6. show-sql=true → CREATE TABLE queries console mein dikhenge
   7. App ready on port 8080 (Postman se hit kar sakte)
```

---

# 📋 QUICK COMMAND REFERENCE

```powershell
# Image management
docker pull mysql:8                  # Download image
docker images                        # List local images
docker rmi mysql:8                   # Delete image

# Container lifecycle
docker run -d --name X image         # Create + start (background)
docker ps                            # Running containers
docker ps -a                         # All (incl. stopped)
docker stop  X                       # Stop (preserve)
docker start X                       # Restart (data wahi)
docker rm    X                       # Delete container
docker rm -f X                       # Force delete (running bhi)

# Debugging
docker logs X                        # Container console output
docker logs -f X                     # Follow real-time
docker exec -it X bash               # Shell into container
docker inspect X                     # Detailed JSON info

# Our specific MySQL setup
docker run -d --name mysql-userdb \
   -e MYSQL_ROOT_PASSWORD=rootpass \
   -e MYSQL_DATABASE=userdb \
   -p 3307:3306 \
   mysql:8

# Connect to MySQL inside container (interactive shell)
docker exec -it mysql-userdb mysql -u root -prootpass
   USE userdb;
   SHOW TABLES;
   SELECT * FROM users;
```

---

# 🧠 CONCEPTS LOCKED (today)

```
✅ Image vs Container          (Recipe vs Dish — read-only vs running)
✅ Docker Hub registry          (mysql:8 cloud se pull)
✅ Layered file system          (caching = bandwidth save)
✅ Container ID + Name          (--name = friendly reference)
✅ Detached mode                (-d = background)
✅ Environment variables        (-e = runtime config injection)
✅ Port mapping                 (-p host:container = network bridge)
✅ Port conflict                (host port already used → use different)
✅ Container lifecycle          (run/stop/start/rm — stopped ≠ deleted)
✅ docker ps columns            (status decode)
✅ docker logs                  (verify readiness)
✅ Spring Boot DB switch        (URL+Driver+Creds+Dialect = 4 changes only)
✅ JDBC URL format              (jdbc:mysql://host:port/dbname)
✅ MySQL Connector/J            (com.mysql.cj.jdbc.Driver)
✅ Hibernate dialect            (DB-specific SQL flavor)
```

---

# ⚠️ TRAP BOX

```
🪤 Trap 1: Container "Up" = ready
         ❌ Up status milta jaldi, but app inside takes time to init
         ✅ docker logs mysql-userdb → "ready for connections" wait

🪤 Trap 2: Same port use kar sakte multiple containers
         ❌ Host port 3307 ek hi container use kar sakta
         ✅ Different host ports for different containers

🪤 Trap 3: Container delete = data gone
         ❌ docker rm = container record + writable layer GONE
         ✅ Volumes use karo data persist karne ke liye (next session)

🪤 Trap 4: Image delete = container affected
         ❌ docker rmi running container ki image delete nahi karega
         ✅ Stop + remove containers FIRST, phir image delete

🪤 Trap 5: localhost in container ≠ localhost on host
         ❌ Inside container "localhost" = container itself
         ✅ Host se connect: localhost:3307 (mapped port)
            Container-to-container: container name as hostname

🪤 Trap 6: H2 dialect with MySQL = error
         ❌ Hibernate dialect MUST match actual DB
         ✅ MySQL8Dialect for MySQL, H2Dialect for H2

🪤 Trap 7: -p without -d = terminal stuck
         ❌ Foreground mode + bound port = can't Ctrl+C cleanly
         ✅ Always -d (detached) for services
```

---

# 📍 PART 11 — Maven Install (Detour)

## 🤔 Problem:
```powershell
mvn clean package -DskipTests
```
**Error:** `mvn is not recognized as the name of a cmdlet...`

**Root cause:** Maven not in PATH. Java was there but Maven separate.

## 🛠️ 3 Approaches Tried

### ❌ Approach 1: winget (Windows Package Manager)
```powershell
winget install Apache.Maven
```
**Result:** "No package found" — Apache Maven not in winget repo.

### ❌ Approach 2: Chocolatey
```powershell
choco install maven -y
```
**Result:** "Access denied" — Chocolatey needs admin (current session not elevated).

### ✅ Approach 3: Manual Download (worked, NO ADMIN)

```powershell
# Step 1 — Download Maven binary zip from Apache archive
$url = "https://archive.apache.org/dist/maven/maven-3/3.9.9/binaries/apache-maven-3.9.9-bin.zip"
Invoke-WebRequest -Uri $url -OutFile "$env:TEMP\maven.zip"
# Output: 9.5 MB downloaded

# Step 2 — Extract to USER folder (no admin needed for Users\...)
$installDir = "C:\Users\ArpanMaheshwari\Tools"
New-Item -ItemType Directory -Path $installDir -Force | Out-Null
Expand-Archive -Path "$env:TEMP\maven.zip" -DestinationPath $installDir -Force
# Result: C:\Users\ArpanMaheshwari\Tools\apache-maven-3.9.9\

# Step 3 — Set USER environment variables (no admin)
$mavenHome = "$installDir\apache-maven-3.9.9"
[Environment]::SetEnvironmentVariable("MAVEN_HOME", $mavenHome, "User")

$userPath = [Environment]::GetEnvironmentVariable("Path", "User")
[Environment]::SetEnvironmentVariable("Path", "$userPath;$mavenHome\bin", "User")

# Step 4 — Verify (NEW PowerShell window for PATH refresh)
mvn --version
```

**Output (success):**
```
Apache Maven 3.9.9 (8e8579a9e76f7d015ee5ec7bfcdc97d260186937)
Maven home: C:\Users\ArpanMaheshwari\Tools\apache-maven-3.9.9
Java version: 26
OS name: "windows 11"
```

## 💡 Concept — User vs System env vars

```
SYSTEM env vars  → All users on machine, needs ADMIN
USER env vars    → Only current user, NO ADMIN needed
                   Stored in: HKCU:\Environment registry

We used USER level → Admin nahi padi
```

## 🆚 Maven Wrapper alternative

Spring Initializr har project mein bundle karta:
```powershell
.\mvnw.cmd clean package -DskipTests
```
- No global Maven install needed
- Project-local
- Project mein commit hota — har dev same version
- Pehli baar slow (correct Maven download)

---

# 📍 PART 12 — Hibernate 7 Dialect Trap (BIG GOTCHA!)

## 🚨 The Error:

`mvn clean package` → `BUILD SUCCESS` ✅
`java -jar target/usercrud-0.0.1-SNAPSHOT.jar` → ❌ ERROR:

```
WARN HHH100046: Could not obtain connection to query JDBC database metadata

org.hibernate.boot.registry.selector.spi.StrategySelectionException:
   Unable to resolve name [org.hibernate.dialect.MySQL8Dialect] 
   as strategy [org.hibernate.dialect.Dialect]
```

## 🔍 Root Cause:

```
Spring Boot 4.0.6 → uses Hibernate 7.2.x

Hibernate dialect timeline:
   Hibernate 5.x   →  org.hibernate.dialect.MySQL8Dialect ✅ exists
   Hibernate 6.x   →  MySQL8Dialect DEPRECATED ⚠️
   Hibernate 7.x   →  MySQL8Dialect REMOVED ❌

Old guides + StackOverflow → MySQL8Dialect bata rahe (5.x era)
Tu Spring Boot 4 use kar raha → Hibernate 7 → class hi nahi
```

## 🛠️ Fix (in `application.properties`):

```properties
# Old (Hibernate 5/6 era):
# spring.jpa.database-platform=org.hibernate.dialect.MySQL8Dialect

# New (Hibernate 7+):
spring.jpa.database-platform=org.hibernate.dialect.MySQLDialect
```

**`MySQLDialect`** = universal MySQL dialect (auto-detects version from JDBC connection).

## 💎 Interview Gold

**Q: "Spring Boot 3 → 4 migration mein kya breaks?"**

> *"Hibernate version jump 6→7. Sabse common: dialect classes removed (MySQL8Dialect, PostgreSQL10Dialect etc.). Solution: universal dialect (MySQLDialect, PostgreSQLDialect) — auto-detects version. Also Jakarta EE 11 (javax → jakarta), some deprecated APIs gone."*

---

# 📍 PART 13 — Build + Run

## 🔨 Build:
```powershell
cd C:\DSA_PRACTICE\02_SPRING\PROJECT\usercrud
mvn clean package -DskipTests
```

### Phases (Maven lifecycle):
```
clean      → target/ folder delete (purana build hatao)
package    → compile + test (skipped) + create jar
-DskipTests → tests skip (faster build, dev mode)
```

### Pehli baar slow (~3-5 min):
- Maven plugins download
- Spring Boot dependencies (~50-100 MB)
- Hibernate, JJWT, MySQL Connector, Lombok, etc.
- All cached in `~/.m2/repository/` (next time fast)

### Last expected output:
```
[INFO] BUILD SUCCESS
[INFO] Total time:  XX s
```

**Generated artifact:** `target/usercrud-0.0.1-SNAPSHOT.jar` (~50 MB fat jar)

## 🚀 Run:
```powershell
java -jar target\usercrud-0.0.1-SNAPSHOT.jar
```

Spring Boot startup begins, console output flows.

---

# 📍 PART 14 — Spring Boot Connection SUCCESS ✅

## 🎯 Real Output Captured:

```
Database info:
   Database JDBC URL [jdbc:mysql://localhost:3307/userdb]
   Database driver: MySQL Connector/J
   Database dialect: MySQLDialect
   Database version: 8.4.9
   Default catalog/schema: userdb/undefined
   Isolation level: REPEATABLE_READ [default REPEATABLE_READ]
   Pool: DataSourceConnectionProvider

Hibernate: create table refresh_tokens (
    id bigint not null auto_increment,
    expires_at datetime(6) not null,
    token varchar(255) not null,
    user_id bigint not null,
    primary key (id)
) engine=InnoDB

Hibernate: create table users (
    id bigint not null auto_increment,
    age integer not null check ((age>=18) and (age<=120)),
    email varchar(255) not null,
    name varchar(50) not null,
    password varchar(255) not null,
    role varchar(255) not null,
    primary key (id)
) engine=InnoDB

Hibernate: alter table refresh_tokens add constraint UK... unique (token)
Hibernate: alter table users add constraint UK... unique (email)

Initialized JPA EntityManagerFactory for persistence unit 'default'
Tomcat started on port 8080 (http) with context path '/'
Started UsercrudApplication in 8.254 seconds
```

## ✅ Yeh DEKHA:

```
✅ MySQL container 8.4.9 connected (localhost:3307)
✅ Schema auto-created (ddl-auto=update)
✅ engine=InnoDB (MySQL transactional storage)
✅ Constraints applied:
      • check (age>=18 AND age<=120) — Bean Validation se aaya
      • unique email
      • unique token
✅ HikariPool active (Spring Boot's connection pooling)
✅ Tomcat listening on port 8080
✅ App started in 8.254 seconds
```

## 🧠 Concept — REPEATABLE_READ Isolation

```
MySQL InnoDB default isolation level:
   • Same transaction mein same query → same result (consistent reads)
   • Phantom reads possible
   
Other levels (lowest → highest isolation):
   READ UNCOMMITTED < READ COMMITTED < REPEATABLE READ < SERIALIZABLE

Trade-off: Higher isolation → more locks → less concurrency
```

---

# 📍 PART 15 — Postman Test 1: Register

## 📤 Request:
```http
POST http://localhost:8080/auth/register
Content-Type: application/json

{
  "name": "Arpan",
  "email": "arpan@test.com",
  "age": 27,
  "password": "test123"
}
```

## 📥 Response (200 OK):
```json
{
  "id": 1,
  "name": "Arpan",
  "email": "arpan@test.com",
  "password": "$2a$10$qG1NHoJ1GOELldSA6xSv2uV7bBuS01fFhafROy4/tFStgVE5ngalC",
  "age": 27,
  "role": "USER"
}
```

## 💎 Important Observations:

```
1. id: 1 → MySQL auto-increment (DB ne diya, app ne nahi)

2. password → BCrypt HASHED (NOT plain "test123"):
      $2a$10$qG1NHoJ1...
       │  │  └─ actual hash
       │  └──── cost factor (10 = 2^10 rounds)
       └─────── BCrypt version
   
   Production-grade security:
   • Plain "test123" NEVER stored
   • Hash one-way (cannot reverse)
   • Same password = different hash each time (salt)

3. role: "USER" → default role assigned by Spring Security

4. End-to-end flow:
      Postman → HTTP → Spring Boot → BCrypt hash 
            → JPA save → JDBC INSERT → MySQL container → row stored
```

---

# 📍 PART 16 — Postman Test 2: Login (JWT + Refresh)

## 📤 Request:
```http
POST http://localhost:8080/auth/login
Content-Type: application/json

{
  "email": "arpan@test.com",
  "password": "test123"
}
```

## 📥 Response (200 OK, 374ms):
```json
{
  "accessToken": "eyJhbGciOiJIUzI1NiJ9.eyJzdWIiOiJhcnBhbkB0ZXN0LmNvbSIsImlhdCI6...",
  "refreshToken": "9ff5a083-5af3-4b94-aec3-c675876b9b23"
}
```

## 💎 Two Token Types — Why?

```
ACCESS TOKEN (JWT format)
   • header.payload.signature (3 parts, base64-encoded)
   • eyJhbGc... → decodes to {"alg":"HS256","typ":"JWT"}
   • Self-contained — backend verifies offline (no DB hit)
   • Short-lived: 15 min (jwt.expiration=900000 ms)
   • Stateless — server doesn't store
   • Trade-off: cannot be revoked instantly until expires

REFRESH TOKEN (UUID format)
   • Random UUID (9ff5a083-5af3-4b94-aec3-c675876b9b23)
   • Server-side STORED in MySQL refresh_tokens table
   • Long-lived: 7 days (jwt.refresh.expiration=604800000 ms)
   • REVOKABLE — DELETE from DB = token dead
   • Trade-off: DB hit needed for refresh
```

## 🔄 Token Flow (interview):

```
Login           → access (15 min) + refresh (7 days)
Use access      → Bearer header in API requests
Access expires  → 401 Unauthorized
Send refresh    → /auth/refresh endpoint
Validate        → DB lookup (token exists + not expired)
                  Issue new access token
Logout          → DELETE refresh from DB → can't refresh again
```

## 🧠 Backend Behind The Scenes:

```
1. POST /auth/login received
2. AuthController.login(LoginRequest)
3. CustomUserDetailsService.loadUserByUsername(email)
   → MySQL query: SELECT * FROM users WHERE email = ?
4. BCryptPasswordEncoder.matches(plain, storedHash)
   → algorithmic match (NOT raw string compare)
5. JwtService.generateAccessToken(email)
   → eyJ... 15-min JWT
6. RefreshTokenService.create(user)
   → INSERT INTO refresh_tokens (UUID, user_id, expires_at = NOW + 7 days)
7. Return TokenResponse(access, refresh)
```

---

# 📍 PART 17 — MySQL Verify (Real Data Inside Container)

## 🔍 Command:
```powershell
docker exec -it mysql-userdb mysql -u root -prootpass
```

### Flag breakdown:
```
docker exec    → Container ke andar command run
-i             → Interactive (stdin open)
-t             → TTY allocate (proper terminal)
-it            → -i + -t combined (interactive shell)
mysql-userdb   → Container ka naam
mysql          → Container ke ANDAR mysql client run
-u root        → User: root
-prootpass     → Password (NO space after -p — MySQL convention)
```

## Inside MySQL prompt:

### Show all tables
```sql
USE userdb;
SHOW TABLES;
```
**Output:**
```
+------------------+
| Tables_in_userdb |
+------------------+
| refresh_tokens   |
| users            |
+------------------+
```

### Users data
```sql
SELECT id, name, email, age, role,
       SUBSTRING(password, 1, 30) AS pass_preview
FROM users;
```
**Output:**
```
+----+-------+-----------------+-----+------+--------------------------------+
| id | name  | email           | age | role | pass_preview                   |
+----+-------+-----------------+-----+------+--------------------------------+
|  1 | Arpan | arpan@test.com  |  27 | USER | $2a$10$qG1NHoJ1GOELldSA6xSv2u  |
+----+-------+-----------------+-----+------+--------------------------------+
```

### Refresh tokens
```sql
SELECT id, token, user_id, expires_at FROM refresh_tokens;
```
**Output:**
```
+----+--------------------------------------+---------+---------------------+
| id | token                                | user_id | expires_at          |
+----+--------------------------------------+---------+---------------------+
|  1 | 9ff5a083-5af3-4b94-aec3-c675876b9b23 |       1 | 2026-05-12 ...      |
+----+--------------------------------------+---------+---------------------+
```

### Exit:
```sql
\q
```
or `exit`

## 💎 FULL CIRCLE PROOF:

```
Postman ne dikhaya  → "refreshToken": "9ff5a083-5af3-4b94-..."
MySQL container mein → token: 9ff5a083-5af3-4b94-...
                       SAME UUID ✅

Spring Boot → MySQL container → SAME data
End-to-end persistence VERIFIED
```

---

# 📍 PART 18 — Local MySQL vs Docker MySQL (ISOLATION DEMO)

## 🔍 Discovery Command:
```powershell
Get-Service | Where-Object { $_.Name -match 'MySQL|mysqld' }
```

**Output:**
```
Name      Status   DisplayName
----      ------   -----------
MySQL57   Running  MySQL57
```

**Surprise:** Tera laptop pe local MySQL57 service already running thi (kabhi install kiya hoga, password forgot).

## ✅ Concept Lock — Two MySQLs running

```
TERA LAPTOP (host)

  ┌─────────────────────────┐
  │ LOCAL MySQL57           │  ← Windows service
  │ Port: 3306              │  ← purana install
  │ Apna data store         │
  │ Spring Boot ne          │
  │ CONNECT NAHI KIYA ❌    │
  └─────────────────────────┘
  
  ┌─────────────────────────┐
  │ DOCKER CONTAINER        │
  │ MySQL 8 inside          │
  │ Port mapping: 3307→3306 │
  │ Spring Boot CONNECTED ✅│
  │ userdb + tables yahan   │
  └─────────────────────────┘
  
  Yeh DO ALAG MySQL servers hain.
  Inn dono ka kuch lena-dena nahi.
  Yahi Docker ka CORE concept = ISOLATION
```

## Key Insights:

```
1. Container = isolated (filesystem, network, process tree)
2. Local MySQL stop karu → Docker MySQL pe ZERO impact
3. Docker container delete → local MySQL UNTOUCHED
4. Yahi reason Docker = "works on my machine" solver
5. Production environment = container behavior IDENTICAL everywhere
```

## Local MySQL handle (deferred):

```
Abhi Docker MySQL primary → local MySQL57 ko abhi chedna nahi
Future cleanup options:
   1. Stop service:    Stop-Service MySQL57       (admin needed)
   2. Disable boot:    Set-Service -StartupType Disabled
   3. Uninstall:       Control Panel → MySQL → Uninstall
```

---

# 📋 FULL COMMAND REFERENCE (Aaj Use Hua Sab)

## 🔧 PowerShell / System Commands

```powershell
# Verify Docker installation
docker --version                          # Show Docker version
docker info                               # Detailed engine info

# Check what's running on which port (debugging)
Get-Service | Where-Object { $_.Name -match 'MySQL' }
                                          # Find local MySQL services
netstat -an | Select-String ":3306"      # Who's using port 3306?

# Maven install (manual, no admin)
Invoke-WebRequest -Uri $url -OutFile $tempZip
                                          # Download file
Expand-Archive -Path $zip -DestinationPath $dir -Force
                                          # Extract zip
[Environment]::SetEnvironmentVariable("MAVEN_HOME", $value, "User")
                                          # Set env var (user-level)
mvn --version                             # Verify Maven
```

## 🐳 Docker Image Commands

```powershell
docker pull mysql:8                       # Download image from Docker Hub
docker images                             # List local images
docker rmi mysql:8                        # Delete image (containers must be removed first)
docker image prune                        # Remove unused images
```

## 🐳 Docker Container Lifecycle

```powershell
# Create + start (background)
docker run -d --name mysql-userdb \
   -e MYSQL_ROOT_PASSWORD=rootpass \
   -e MYSQL_DATABASE=userdb \
   -p 3307:3306 \
   mysql:8

# Each flag means:
#   -d                     → detached (background)
#   --name X               → friendly name
#   -e KEY=VALUE           → environment variable
#   -p host:container      → port mapping (forward host port → container port)
#   image:tag              → which image to run

# Container management
docker ps                                 # Running containers
docker ps -a                              # All (incl. stopped)
docker ps -q                              # Only IDs (scripting)
docker stop  mysql-userdb                 # Stop (preserves data, can start again)
docker start mysql-userdb                 # Restart stopped container
docker restart mysql-userdb               # Stop + start
docker rm    mysql-userdb                 # Delete container (data gone if no volume)
docker rm -f mysql-userdb                 # Force delete (running bhi)
```

## 🔍 Docker Debugging

```powershell
docker logs    mysql-userdb               # Show container's stdout/stderr
docker logs -f mysql-userdb               # Follow real-time (like tail -f)
docker logs --tail 50 mysql-userdb        # Last 50 lines
docker inspect mysql-userdb               # Detailed JSON (network, mounts, env)
docker stats                              # Live CPU/memory usage
docker top mysql-userdb                   # Processes inside container
```

## 🚪 Docker Exec (Container ke Andar)

```powershell
# General shell
docker exec -it <container> bash          # Linux bash shell
docker exec -it <container> sh            # Minimal shell (alpine)

# Direct MySQL client
docker exec -it mysql-userdb mysql -u root -prootpass

# Inside MySQL prompt:
USE userdb;                               # Switch database
SHOW DATABASES;                           # List all DBs
SHOW TABLES;                              # List tables
DESCRIBE users;                           # Table schema
SELECT * FROM users;                      # Query data
SELECT * FROM refresh_tokens;
\q   or  exit                             # Exit MySQL
```

## ☕ Maven + Java Run

```powershell
# Inside project folder
cd C:\DSA_PRACTICE\02_SPRING\PROJECT\usercrud

# Build (clean + compile + package)
mvn clean package -DskipTests

# Phases ki order:
#   clean        → target/ delete
#   validate     → project structure OK?
#   compile      → .java → .class
#   test         → unit tests run (skip with -DskipTests)
#   package      → jar/war banao
#   verify, install, deploy... (later phases)

# Other useful:
mvn dependency:tree                       # Dependency graph
mvn dependency:resolve                    # Just download deps
mvn -X clean package                      # Verbose debug

# Run jar
java -jar target\usercrud-0.0.1-SNAPSHOT.jar
java -jar -Dserver.port=8090 target\...   # Override property
```

## 🔧 Spring Boot Property Overrides (runtime)

```powershell
# Command-line override (highest precedence)
java -jar usercrud.jar --spring.datasource.url=jdbc:mysql://localhost:3307/userdb

# Environment variable override
$env:SPRING_DATASOURCE_URL="jdbc:mysql://..."
java -jar usercrud.jar
```

---

# 🧠 CONCEPTS LOCKED (FULL LIST after today)

```
✅ Image vs Container             (Recipe vs Dish)
✅ Docker Hub registry            (cloud images se pull)
✅ Layered file system            (caching saves bandwidth)
✅ Container ID + Name            (--name = friendly reference)
✅ Detached mode                  (-d = background)
✅ Environment variables          (-e = runtime config injection)
✅ Port mapping                   (-p host:container)
✅ Port conflict resolution       (use different host port)
✅ Container lifecycle            (run/stop/start/rm)
✅ docker ps columns              (status decode)
✅ docker logs                    (verify readiness)
✅ docker exec                    (container ke andar enter)
✅ Container isolation            (local MySQL vs Docker MySQL)
✅ Spring Boot DB switch          (URL+Driver+Creds+Dialect = 4 changes)
✅ JDBC URL format                (jdbc:mysql://host:port/dbname)
✅ MySQL Connector/J              (com.mysql.cj.jdbc.Driver)
✅ Hibernate dialect              (DB-specific SQL flavor)
✅ Hibernate 7 migration trap     (MySQL8Dialect REMOVED → MySQLDialect)
✅ HikariPool                     (Spring Boot connection pooling)
✅ ddl-auto=update                (Hibernate auto-schema)
✅ Maven manual install           (user-level env vars, no admin)
✅ MAVEN_HOME + PATH              (user vs system env vars)
✅ Maven Wrapper (mvnw)           (project-bundled Maven)
✅ Maven phases                   (clean/compile/test/package)
✅ BCrypt password hashing        ($2a$10$... format)
✅ JWT vs Refresh token           (stateless 15min vs DB-stored 7 days)
✅ End-to-end flow                (Postman → Spring → JDBC → MySQL container)
✅ MySQL InnoDB engine            (transactional, default)
✅ REPEATABLE_READ isolation      (MySQL default)
```

---

# ⚠️ TRAP BOX (Updated)

```
🪤 Trap 1: Container "Up" = ready
         ❌ Up status milta jaldi, but app inside takes time
         ✅ docker logs X → "ready for connections" wait

🪤 Trap 2: Same host port = multiple containers conflict
         ❌ Host port 3307 ek hi container use kar sakta
         ✅ Different host ports for different containers

🪤 Trap 3: Container delete = data gone (no volumes)
         ❌ docker rm = container record + writable layer GONE
         ✅ Volumes use karo data persist (NEXT TOPIC)

🪤 Trap 4: Image delete with running containers
         ❌ docker rmi running container ki image nahi delete karega
         ✅ Stop + remove containers FIRST, phir image delete

🪤 Trap 5: localhost in container ≠ localhost on host
         ❌ Inside container "localhost" = container itself
         ✅ Host se: localhost:3307 (mapped port)
            Container-to-container: container name as hostname

🪤 Trap 6: Wrong Hibernate dialect = startup fail
         ❌ MySQL8Dialect with Hibernate 7 → CLASS NOT FOUND
         ✅ MySQLDialect (universal, auto-detects version)

🪤 Trap 7: -p without -d = terminal stuck
         ❌ Foreground + bound port = can't Ctrl+C cleanly
         ✅ Always -d (detached) for services

🪤 Trap 8: Port conflict from local services
         ❌ Local MySQL57 service silently uses 3306
         ✅ Get-Service check + use different port (3307)

🪤 Trap 9: Old PowerShell window after env var change
         ❌ Same window mein PATH update reflect nahi
         ✅ NAYA terminal khol — env var refresh

🪤 Trap 10: -p on no space
         ❌ -p mysql -prootpass dono different
         ✅ -p (port mapping) → -p HOST:CONTAINER
            -p (mysql password) → -prootpass (NO space) [MySQL convention]

🪤 Trap 11: Spring Boot password verification raw compare
         ❌ Plain string compare passwords ke liye
         ✅ BCryptPasswordEncoder.matches() — algorithmic
```

---

# 🎯 NEXT STEPS

## ✅ Aaj Done (5 May 2026):

```
✅ Docker Desktop install (WSL 2 backend)
✅ MySQL 8 image pull + run (port 3307)
✅ Maven install (manual user-level)
✅ Hibernate 7 dialect fix
✅ Spring Boot ↔ Docker MySQL connection LIVE
✅ Postman: Register → BCrypt-hashed user in DB
✅ Postman: Login → JWT + UUID refresh tokens
✅ docker exec verify — both tables full circle
✅ Container isolation concept locked
```

## 🔜 Next Session — VOLUMES (data persistence)

```
Concept: "Container delete = data gone" — abhi yahi state
   Demo:
      1. docker rm -f mysql-userdb        → container delete
      2. Naya container start              → SAB DATA GONE 💀
      3. Volume add: -v mysql_data:/var/lib/mysql
      4. Disaster repeat                   → Data SURVIVE ✅

Then:
   • Networks (multi-container talk via container name)
   • docker-compose (multi-service in one YAML)
   • ENTRYPOINT vs CMD (interview classic)
   • Multi-stage builds (production image small)
   • Image push to Docker Hub
```

---

# 💎 POWER PHRASE (interview lock)

> *"Docker Image read-only template, Container running instance. `docker run` flags = control: `-d` background, `--name` reference, `-e` env vars, `-p host:container` port mapping. Containers isolated — local MySQL aur Docker MySQL alag worlds. Spring Boot DB switch = 4 properties only (URL, driver, creds, dialect) — JPA abstracts rest. `docker exec` se container ke andar enter, real data inspection. Container delete = data gone (default) — volumes for persistence. Hibernate 7 migration: MySQL8Dialect REMOVED, use universal MySQLDialect (auto-detect version)."*

---

# 📊 TODAY'S COMPLETE WORKFLOW (single view)

```
1. Docker install (WSL2)
            ↓
2. docker pull mysql:8
            ↓
3. docker run with -p 3306 → ❌ port conflict (local MySQL57)
            ↓
4. docker rm + retry with -p 3307:3306 → ✅ container UP
            ↓
5. docker logs → "ready for connections" ✅
            ↓
6. pom.xml: add mysql-connector-j dependency
            ↓
7. application.properties: H2 → MySQL (4 lines)
            ↓
8. mvn → ❌ "mvn not recognized"
            ↓
9. Manual Maven install (download + env vars)
            ↓
10. mvn clean package → ✅ BUILD SUCCESS
            ↓
11. java -jar → ❌ MySQL8Dialect not found (Hibernate 7)
            ↓
12. application.properties: MySQL8Dialect → MySQLDialect
            ↓
13. mvn rebuild + java -jar → ✅ Connection LIVE!
            ↓
14. Postman /auth/register → 200 OK, user in DB
            ↓
15. Postman /auth/login → 200 OK, JWT + refresh
            ↓
16. docker exec → SELECT * FROM users → REAL DATA seen
            ↓
17. Get-Service → local MySQL57 discovered
            ↓
   = ISOLATION concept fully understood
            ↓
                 (Docker Topic 1: Image+Container DONE)
            ↓
   ───────────────────────────────────────────
            ↓
                 Docker Topic 2: VOLUMES BEGINS
            ↓
18. Disaster demo: docker rm -f → naya container empty (no volume)
            ↓
19. SHOW TABLES → Empty (proof: data gone with container)
            ↓
20. docker volume create mysql_data
            ↓
21. docker run -v mysql_data:/var/lib/mysql ... (volume mount)
            ↓
22. Spring Boot restart → Hibernate creates tables IN VOLUME
            ↓
23. Postman register Arpan again → data goes to volume
            ↓
24. docker rm -f mysql-userdb → CONTAINER DELETED
            ↓
25. docker volume ls → mysql_data SURVIVES (208.5 MB)
            ↓
26. NEW container with SAME volume → SELECT * FROM users
            ↓
   = ARPAN ALIVE 🎉  VOLUMES CONCEPT LOCKED
```

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

# 📍 PART 20 — NETWORKS (Container DNS Magic) — Day 2

> **Date:** 2026-05-06
> **Goal:** Multi-container setup — containers ek doosre se NAME se baat karein

---

## 🎬 STORY — Apartment Building

```
Tera Docker host = APARTMENT BUILDING
   ├── Container 1 (Spring Boot)  = Flat 101
   ├── Container 2 (MySQL)         = Flat 102
   ├── Container 3 (Redis)         = Flat 103
   └── Container 4 (RabbitMQ)      = Flat 104

Without network:
   Flats sealed — koi flat se baat nahi kar sakta
   
With network (custom bridge):
   Building intercom system
   Flat 101 → 102 ko call kar sakta BY NAME
   "Hey 102, mujhe DB query bhejna"
```

---

## 🎯 3 Network Types

```
1. BRIDGE (default — most common)
   ┌─────────────────────────────────┐
   │  Custom Bridge Network "app-net"│
   │                                  │
   │  ┌──────────┐    ┌──────────┐   │
   │  │ Spring   │◄───┤ MySQL    │   │
   │  │ Boot     │    │ Container│   │
   │  └──────────┘    └──────────┘   │
   │                                  │
   │  Container name = DNS hostname  │
   │  "mysql-userdb" se reach        │
   └─────────────────────────────────┘

2. HOST (no isolation — fast)
   Container directly host network use
   Less secure, fast networking
   
3. NONE (no network)
   Container fully isolated
   Use case: batch jobs, no internet
```

---

## ⭐ KEY INSIGHT — Default Bridge vs Custom Bridge

```
DEFAULT bridge (jo abhi tha):
   ❌ Containers communicate via IP only
   ❌ DNS by name = NAHI work karta (deprecated --link)
   ❌ Production mein avoid

CUSTOM bridge (jo banaya — app-net):
   ✅ Containers communicate via NAME (DNS auto)
   ✅ "mysql-userdb" hostname se reach
   ✅ Production standard
```

---

## 🎯 Hands-On Sequence (jo kiya step by step)

### Step 1 — Existing networks dekho:
```cmd
docker network ls
```
**Output:**
```
NETWORK ID     NAME      DRIVER    SCOPE
838bdc0ae5ab   bridge    bridge    local
858110cc6ae4   host      host      local
91a0392e2026   none      null      local
```
**Samjh:** 3 default networks dikhe — yahi har Docker install pe hote.

---

### Step 2 — Existing container delete (volume safe):
```cmd
docker rm -f mysql-userdb
```
**Reason:** Yesterday wala container default `bridge` pe tha. Custom network mein recreate karenge. **Volume `mysql_data` SAFE** (data nahi gaya).

---

### Step 3 — Custom network create:
```cmd
docker network create app-net
```
**Output:** `32c36736600cf775defc04f337c45ef39b0be2236e6b661eda9bb5cc015e2105`
(network ID — random hash)

---

### Step 4 — Verify:
```cmd
docker network ls
```
**Output:**
```
NETWORK ID     NAME      DRIVER    SCOPE
32c36736600c   app-net   bridge    local      ← naya
838bdc0ae5ab   bridge    bridge    local
858110cc6ae4   host      host      local
91a0392e2026   none      null      local
```

---

### Step 5 — MySQL container ON app-net:
```cmd
docker run -d ^
   --name mysql-userdb ^
   --network app-net ^                       ← KEY new flag
   -e MYSQL_ROOT_PASSWORD=rootpass ^
   -e MYSQL_DATABASE=userdb ^
   -p 3307:3306 ^
   -v mysql_data:/var/lib/mysql ^             ← reuse volume
   mysql:8
```

**Internal IP assigned:** `172.18.0.2`

---

### Step 6 — Network inspect (proof):
```cmd
docker network inspect app-net
```

**Key parts of output:**
```json
{
  "Subnet": "172.18.0.0/16",
  "Gateway": "172.18.0.1",
  "Containers": {
    "de2a98bb...": {
      "Name": "mysql-userdb",
      "IPv4Address": "172.18.0.2/16"
    }
  }
}
```
**Samjh:** mysql-userdb officially app-net ka member, IP assigned.

---

### Step 7 — 🎬 THE MAGIC — Container talks to Container BY NAME:

```cmd
docker run -it --rm --network app-net mysql:8 mysql -h mysql-userdb -u root -prootpass
```

**Decode:**
```
docker run               → naya container
-it                      → interactive
--rm                     → auto-delete on exit (temporary client)
--network app-net        → SAME network as mysql-userdb (KEY!)
mysql:8                  → image (reuse)
mysql                    → command inside (mysql client)
-h mysql-userdb          → HOST = container NAME ⭐ (Docker DNS magic)
-u root                  → username
-prootpass               → password
```

**Inside MySQL prompt:**
```sql
USE userdb;
SHOW TABLES;
SELECT * FROM users;
```

**Output:**
```
+----+-------+-----------------+-----+------+
| id | name  | email           | age | role |
+----+-------+-----------------+-----+------+
|  1 | Arpan | arpan@test.com  |  27 | USER |
+----+-------+-----------------+-----+------+
```

---

### 💡 What just happened (visual):

```
   ┌─────────────────────────────────────────┐
   │  app-net (custom bridge network)        │
   │                                          │
   │  ┌──────────────┐    ┌──────────────┐   │
   │  │ Temporary    │    │ mysql-userdb │   │
   │  │ MySQL Client │───►│ (mysql:8)    │   │
   │  │ (--rm)       │    │              │   │
   │  │              │    │ IP: 172.18.0.2│  │
   │  └──────────────┘    └──────────────┘   │
   │       │                                  │
   │       │ "mysql -h mysql-userdb"          │
   │       │  (NAME, not IP!)                 │
   │       ▼                                  │
   │  Docker DNS resolver                     │
   │       │                                  │
   │       │ "mysql-userdb → 172.18.0.2"     │
   │       ▼                                  │
   │  Connection established → Login OK ✅    │
   └─────────────────────────────────────────┘
```

---

## 💎 Networks — Concept Locked

```
✅ docker network create / ls / inspect / rm
✅ Default vs Custom bridge (DNS difference)
✅ --network flag (container join network)
✅ Container name = DNS hostname (in custom bridge)
✅ Subnet auto-assigned (172.18.0.0/16 etc.)
✅ Each container gets internal IP
✅ Multiple containers same network = name-based talk
✅ Production pattern (microservices baseline)
```

---

# 📍 PART 21 — DOCKER-COMPOSE (Multi-Container Orchestration) — Day 2

> **Date:** 2026-05-06
> **Goal:** Manual `docker run` commands kill — entire stack ek YAML file mein

---

## 🤔 Why Compose?

```
MANUAL approach (jo Phase 1 mein kiya):
   docker network create app-net
   docker volume create mysql_data        (already)
   docker run -d --name mysql-userdb --network app-net ... mysql:8
   docker run -d --name spring-app --network app-net ... usercrud
   docker run -d --name redis --network app-net redis     (if needed)
   
   = 5+ commands, har baar yaad rakho exactly
   = order matter karta (mysql pehle, app baad mein)
   = manual healthcheck wait
   = error prone

WITH Compose:
   docker-compose up -d --build
   
   = 1 command — sab kuch start (network + volumes + containers + healthcheck)
   = order automatic (depends_on)
   = declarative (file mein likha kya chahiye)
   = reproducible (same file → same setup any machine)
```

---

## 🎬 STORY — Restaurant Recipe vs Whole Meal

```
docker run        = ek dish banao manually (ek-ek command)
docker-compose    = WHOLE THALI ka recipe ek file mein
                    1 command se sab dishes ready

Manually 5 commands type karna = stove pe 5 dishes individually banana
Compose YAML = recipe book khol ke "thali ready"
```

---

## 📝 TERA SETUP — 2 Files Banaaye

### File 1: `application-compose.properties`

**Path:** `src/main/resources/application-compose.properties`

```properties
spring.datasource.url=jdbc:mysql://mysql:3306/userdb
spring.datasource.driverClassName=com.mysql.cj.jdbc.Driver
spring.datasource.username=root
spring.datasource.password=rootpass
```

### Why ye specifically?

```
Compare to other profiles:

application-local.properties:
   url=jdbc:mysql://localhost:3306/userdb
   ↑ Spring on host + Local MySQL57

application-docker.properties:
   url=jdbc:mysql://localhost:3307/userdb
   ↑ Spring on host + MySQL container (mapped port)

application-compose.properties:  ← NEW
   url=jdbc:mysql://mysql:3306/userdb
   ↑↑↑↑↑                       ↑↑↑↑
   Service name (DNS!)          Internal port (NOT mapped)
   
   Why? Because Spring is ALSO in container now.
   Container-to-container = service name + INTERNAL port
   No port mapping needed for internal traffic.
```

---

### File 2: `docker-compose.yml`

**Path:** `C:\DSA_PRACTICE\02_SPRING\PROJECT\usercrud\docker-compose.yml`

```yaml
services:

  mysql:                                # ← service name = DNS hostname
    image: mysql:8
    container_name: mysql-userdb        # ← actual container name
    environment:
      MYSQL_ROOT_PASSWORD: rootpass
      MYSQL_DATABASE: userdb
    ports:
      - "3307:3306"                     # ← Workbench connect (optional)
    volumes:
      - mysql_data:/var/lib/mysql       # ← persist data
    healthcheck:                        # ← MySQL "ready?" check
      test: ["CMD", "mysqladmin", "ping", "-h", "localhost", "-prootpass"]
      interval: 5s
      retries: 10

  app:                                  # ← service name
    build: .                             # ← build from Dockerfile
    container_name: usercrud-app
    ports:
      - "8080:8080"                     # ← Postman connect
    depends_on:
      mysql:
        condition: service_healthy      # ← wait MySQL HEALTHY
    environment:
      SPRING_PROFILES_ACTIVE: compose   # ← KEY: profile activator

volumes:
  mysql_data:
    external: true                      # ← reuse existing volume (Arpan!)
```

---

## 🔥 KEY MECHANISM — Profile Auto-Load (tera question)

> **"Compose ne file load ki?" — NO. Spring ne ki.**

### Visual flow:
```
docker-compose.yml
   ┌──────────────────────────────────┐
   │ services:                         │
   │   app:                            │
   │     environment:                  │
   │       SPRING_PROFILES_ACTIVE:     │
   │         compose                   │
   └────────┬─────────────────────────┘
            │ container start time
            │ (compose injects env var)
            ▼
   ┌──────────────────────────────────┐
   │ Spring Boot starts inside container│
   │                                    │
   │ 1. Reads env var:                  │
   │    SPRING_PROFILES_ACTIVE=compose  │
   │                                    │
   │ 2. Loads application.properties    │
   │    (master, ALWAYS)                │
   │                                    │
   │ 3. Detects active profile=compose  │
   │                                    │
   │ 4. AUTO-LOADS application-compose  │
   │    .properties (profile-specific)  │
   │                                    │
   │ 5. Compose values OVERRIDE master  │
   │    where conflict                  │
   │    (datasource URL = mysql:3306)   │
   └──────────────────────────────────┘
```

### Two Independent Systems Coordinating:

```
DOCKER-COMPOSE ka role:
   ❌ Spring config files load karna
   ✅ Container start pe ENV VAR inject karna
   ✅ Network/volume setup
   ✅ Service ordering (depends_on)
   ✅ Healthcheck monitoring
   
SPRING ka role:
   ✅ Env vars + properties read karna
   ✅ Profile-based file selection (auto)
   ✅ Config merging (master + profile)
   
BRIDGE BETWEEN: Environment Variable (SPRING_PROFILES_ACTIVE)
```

**Production implication:**
```
Same compose file, same image, same code →
Different environments by changing env var:
   SPRING_PROFILES_ACTIVE=compose    (local dev)
   SPRING_PROFILES_ACTIVE=staging    (test)
   SPRING_PROFILES_ACTIVE=prod       (production)
```

---

## 🎯 Hands-On Sequence (step by step jo kiya)

### Step 1 — Stop existing container:
```cmd
docker rm -f mysql-userdb
```
**Reason:** Compose will create new container with same name → conflict. Volume safe.

---

### Step 2 — Build JAR (Spring Boot ke liye):
```cmd
cd C:\DSA_PRACTICE\02_SPRING\PROJECT\usercrud
mvn clean package -DskipTests
```
**Reason:** Dockerfile mein `COPY target/usercrud-0.0.1-SNAPSHOT.jar` — JAR pehle se hona chahiye.
**Output:** `BUILD SUCCESS` + `target/usercrud-0.0.1-SNAPSHOT.jar`

---

### Step 3 — First Compose Run:
```cmd
docker-compose up -d --build
```

**Flags:**
```
up        → start services (create + run)
-d        → detached (background)
--build   → build app image (Dockerfile se) before start
```

**Output (FAILED):**
```
ERROR: docker.io/library/openjdk:17-jdk-slim: not found
failed to solve: openjdk:17-jdk-slim ... not found
```

---

### Step 4 — Trap Discovered: openjdk Image DEPRECATED

```
Yeh real production gotcha hai (2024-25 onwards):

PROBLEM:
   Docker Hub se "openjdk" official image ARCHIVED kar diya
   Naye tags (17-jdk-slim, 21-jdk, etc.) PULL nahi hote
   Old guides + StackOverflow → still openjdk batate

INDUSTRY MIGRATION:
   • Eclipse Temurin (Eclipse Adoptium) — most common
   • Amazon Corretto (AWS)
   • Liberica (BellSoft)
   • Oracle's GraalVM
   
   "OpenJDK Docker image deprecated" = production reality
```

---

### Step 5 — Fix: Dockerfile updated

**Before:**
```dockerfile
FROM openjdk:17-jdk-slim
```

**After:**
```dockerfile
FROM eclipse-temurin:17-jdk-jammy
```

**Eclipse Temurin breakdown:**
```
eclipse        → Eclipse Foundation (maintainer)
temurin        → product name (Eclipse's OpenJDK distribution)
17             → Java version
jdk            → full JDK (compile + runtime)
jammy          → Ubuntu 22.04 LTS base

Alternatives:
   eclipse-temurin:17-jre-jammy    → JRE only (smaller, ~150 MB)
   eclipse-temurin:17-jdk-alpine   → smallest (~80 MB), uses musl libc
   amazoncorretto:17                → AWS distribution
```

---

### Step 6 — Re-run Compose (SUCCESS):
```cmd
docker-compose up -d --build
```

**What happened internally:**
```
1. Compose reads docker-compose.yml
2. Pull eclipse-temurin:17-jdk-jammy (~250 MB, first time)
3. Build "usercrud-app" image:
      • FROM eclipse-temurin:17-jdk-jammy
      • COPY jar
      • EXPOSE 8080
      • CMD ["java", "-jar", "app.jar"]
4. Pull mysql:8 (already cached)
5. Auto-create network "usercrud_default"
6. Use external volume "mysql_data" (Arpan zinda)
7. Start "mysql-userdb" container (mysql service)
8. Wait for healthcheck pass:
      mysqladmin ping → "alive" → service_healthy
9. Start "usercrud-app" container (app service)
      • Inject env var SPRING_PROFILES_ACTIVE=compose
      • Spring loads application-compose.properties
      • Connects to "mysql:3306" by service NAME
10. Both containers running, healthy
11. Returns terminal (-d)
```

---

### Step 7 — Verify:
```cmd
docker-compose ps
```

**Output:**
```
NAME            IMAGE              STATUS                    PORTS
mysql-userdb    mysql:8            Up X seconds (healthy)    0.0.0.0:3307->3306/tcp
usercrud-app    usercrud-app       Up X seconds              0.0.0.0:8080->8080/tcp
```

```cmd
docker-compose logs --tail 20 app
```

**Spring Boot startup logs visible:**
```
The following 1 profile is active: "compose"
HikariPool-1 - Start completed.
Tomcat started on port 8080
Started UsercrudApplication in X seconds
```

---

### Step 8 — Postman Test:
```http
POST http://localhost:8080/auth/login

{
  "email": "arpan@test.com",
  "password": "test123"
}
```

**Response (200 OK):**
```json
{
  "accessToken": "eyJhbGciOi...",
  "refreshToken": "41cd0972-e17a-4fc7-bc62-833e0ba8e3ff"
}
```

---

## 💎 Full Circle Proof:

```
Postman (localhost:8080)
   ↓ HTTP
[usercrud-app container] (Spring Boot in container)
   ↓ JDBC: jdbc:mysql://mysql:3306/userdb     ← SERVICE NAME
   ↓ via "usercrud_default" network (auto-created by Compose)
[mysql-userdb container] (MySQL in container)
   ↓ /var/lib/mysql
[mysql_data volume] (host disk — external: true)
   ↓ Yesterday's Arpan record + new refresh token
✅ Login authenticated
✅ Response back through chain
```

---

## 📋 Compose Commands Reference

```cmd
REM Build + Start
docker-compose up -d              # start (background), use existing images
docker-compose up -d --build      # rebuild app image first
docker-compose up                 # foreground (logs flood)

REM Status
docker-compose ps                  # services status
docker-compose ps -a               # all (stopped bhi)

REM Logs
docker-compose logs                # all services logs
docker-compose logs app            # specific service
docker-compose logs -f app         # follow (live tail)
docker-compose logs --tail 20 app  # last 20 lines

REM Stop / Remove
docker-compose stop                # stop containers (keep images, volumes)
docker-compose start               # restart stopped
docker-compose restart             # stop + start
docker-compose down                # stop + remove containers + remove network
docker-compose down -v             # also remove volumes (DATA GONE — careful!)

REM Inside container
docker-compose exec mysql bash     # shell into mysql service
docker-compose exec app bash       # shell into app service

REM Build only (no start)
docker-compose build               # build all
docker-compose build app           # build specific
```

---

## 🆚 docker-compose.yml — Key Sections Explained

```yaml
services:                  # Each service = one container type

  mysql:                    # Service name (DNS-able)
    image: mysql:8           # Pre-built image use
    container_name: ...      # Actual container name (default: <project>_<service>)
    environment: {...}       # Env vars (compose ke time inject)
    ports: ["host:cont"]     # Port mapping (optional)
    volumes: [...]           # Mounts
    healthcheck: {...}       # Readiness check
    depends_on: {...}        # Wait for other service
    restart: unless-stopped  # Auto-restart policy

  app:
    build: .                  # Build from Dockerfile (no pre-built image)
    # Same options as above

volumes:                    # Named volumes declaration
  mysql_data:
    external: true            # Use pre-existing volume

networks:                   # Custom networks (optional — default auto-created)
  my-net:
    driver: bridge
```

---

## 🎯 Compose vs Manual — Side by Side

| Aspect | Manual `docker run` | `docker-compose` |
|---|---|---|
| Setup | 5+ commands | 1 YAML + 1 command |
| Reproducibility | Memory based | Declarative file |
| Order | Manual sequence | `depends_on` + healthcheck |
| Network | Manual create + flag | Auto-created |
| Volumes | Manual create + flag | Auto via YAML |
| Stop all | Multiple `docker stop` | `docker-compose down` |
| Env vars | Per-command flags | YAML environment block |
| Production-ready | Limited | Yes (with care) |
| K8s migration | Hard | Easy (YAML similar concept) |

---

## ⚠️ TRAPS (new — Compose specific)

```
🪤 Trap 16: openjdk image deprecated
         ❌ FROM openjdk:17-jdk-slim  → "not found" 2024+
         ✅ FROM eclipse-temurin:17-jdk-jammy

🪤 Trap 17: external volume forgot = naya empty
         ❌ volumes: mysql_data: (no external: true)
            → compose creates "<project>_mysql_data" empty
            → Yesterday's Arpan GONE
         ✅ external: true to reuse pre-created volume

🪤 Trap 18: depends_on without healthcheck
         ❌ depends_on: - mysql
            → app starts before MySQL ready
            → connection refused on first try
         ✅ condition: service_healthy + healthcheck on mysql

🪤 Trap 19: Service name ≠ container name
         ❌ Confusion: "mysql" service, "mysql-userdb" container
         ✅ Service name = YAML key (DNS), container_name = actual
            Both useful, different purposes

🪤 Trap 20: Build without rebuild
         ❌ docker-compose up after Dockerfile change → uses old image
         ✅ docker-compose up --build  (force rebuild)

🪤 Trap 21: Profile env var typo
         ❌ SPRING_PROFILES_ACTIVE: compose-profile
         ✅ SPRING_PROFILES_ACTIVE: compose
            (must match application-{X}.properties suffix)

🪤 Trap 22: Port conflict with existing
         ❌ host:3307 already used by another container
         ✅ Stop old container first, OR change host port
```

---

## ✅ Day 2 Concepts LOCKED

```
✅ Custom bridge networks (vs default)
✅ Container DNS (service name = hostname)
✅ docker network create / inspect / connect
✅ --network flag for container creation
✅ Internal IP assignment (subnet auto-managed)
✅ Multi-container talk by name
✅ docker-compose YAML structure
✅ services / image / build / environment
✅ depends_on + healthcheck (proper ordering)
✅ external volumes (reuse pre-created)
✅ Auto-generated network (Compose default)
✅ docker-compose up / down / ps / logs / exec
✅ Eclipse Temurin migration (openjdk deprecated)
✅ Profile activation via env var
✅ Spring auto-loads application-{profile}.properties
✅ Compose env var = bridge between Compose and Spring
✅ Production-grade pattern (multi-container stack)
```

---

## 💎 ULTIMATE POWER PHRASE (Day 1 + Day 2)

> *"Docker = Image (template) + Container (instance) + Volume (persist) + Network (talk) + Compose (orchestrate). Custom bridge network gives container-name DNS — no IP needed. Compose YAML declarative = entire stack one file, depends_on + healthcheck for ordering. Service name = DNS hostname; container_name = actual identity. External volumes reuse existing data. Spring profile via SPRING_PROFILES_ACTIVE env var = compose injects, Spring auto-loads matching properties file. openjdk deprecated → Eclipse Temurin (eclipse-temurin:17-jdk-jammy). Production-grade multi-container apps = Compose pattern, K8s migration easy from here."*

---

## 📊 TODAY'S COMPLETE FLOW (Day 2 single view)

```
PHASE 1 — Networks (~1 hr)
   1. docker network ls (saw 3 default)
   2. docker rm -f mysql-userdb (cleanup, volume safe)
   3. docker network create app-net
   4. docker run --network app-net + -v mysql_data
   5. docker network inspect (verify)
   6. docker run -it --rm --network app-net mysql:8 mysql -h mysql-userdb
      → Connect by NAME (no IP, no localhost)
      → SELECT * FROM users → Arpan visible
   = NETWORKS LOCKED ✅

PHASE 2 — Compose (~1.5 hr)
   7. CREATE: application-compose.properties
      (url=jdbc:mysql://mysql:3306/userdb)
   8. CREATE: docker-compose.yml (services + volumes)
   9. docker rm -f mysql-userdb (cleanup)
   10. mvn clean package -DskipTests (build JAR)
   11. docker-compose up -d --build
       → ❌ openjdk:17-jdk-slim NOT FOUND
   12. FIX: Dockerfile FROM eclipse-temurin:17-jdk-jammy
   13. docker-compose up -d --build → ✅ SUCCESS
       • mysql container healthy
       • app container running (waited for mysql)
       • Both on auto-created network
   14. docker-compose ps → both Up
   15. docker-compose logs app → Tomcat started
   16. Postman /auth/login → 200 OK + new refresh token
       → Arpan's data from yesterday's volume + Compose stack working
   = COMPOSE LOCKED ✅

   Profile auto-load mechanism understood:
      docker-compose YAML sets SPRING_PROFILES_ACTIVE=compose
         ↓ env var inject at container start
      Spring Boot inside container reads env var
         ↓
      Spring AUTO-LOADS application-compose.properties
         ↓
      Datasource URL becomes mysql:3306 (service name)
```
