# 🟡 Section B2 — Docker Compose (Multi-Container Orchestration) (Day 2)

> **Goal:** Entire stack in one YAML file, one command
> **Tools:** docker-compose.yml + application-compose.properties

📚 [← Back to README](00_README.md) | [← Networks](05_networks.md) | [Reference →](07_reference.md)

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