# Section D — Visual Revision (Quick Recap)

> **Use:** Final lock-in recap, interview prep visual reference
> **Read after:** Sections 1-7 done

[← Back to README](00_README.md) | [← Reference](07_reference.md)

---

# DOCKER MEGA VISUAL RECAP

---

## 1️⃣ THE BIG 5 — Docker Building Blocks

```
┌──────────────────────────────────────────────────────────────────┐
│                                                                    │
│   IMAGE        CONTAINER       VOLUME         NETWORK     COMPOSE  │
│   (recipe)     (running)       (storage)      (talk)      (orchestrate)│
│                                                                    │
│   ┌─────┐      ┌─────────┐    ┌──────────┐   ◄─── ───►   ┌─────┐  │
│   │  │ run  │      │    │       │              │  │  │
│   │     │ ───► │ alive   │ ───│ persist  │     bridge    │ YAML│  │
│   │     │      │ process │ -v │ data     │     network   │ all │  │
│   └─────┘      └─────────┘    └──────────┘                └─────┘  │
│                                                                    │
│   read-only    ephemeral      independent     custom DNS    stack │
└──────────────────────────────────────────────────────────────────┘
```

---

## 2️⃣ IMAGE vs CONTAINER (Recipe vs Dish)

```
   IMAGE (mysql:8)              CONTAINER (mysql-userdb)
   ──────────────                ────────────────────────
   ┌──────────────┐              ┌──────────────────┐
   │ Recipe paper │   docker     │ Actual dish       │
   │ "MySQL 8 +   │   run        │ • Process running │
   │  Linux +     │ ───────►     │ • Memory usage    │
   │  config"     │              │ • Apna filesystem │
   │              │              │ • Apna network    │
   │ Read-only    │              │ • Mutable         │
   │ Disk         │              │ • Stoppable       │
   └──────────────┘              └──────────────────┘

   1 image → unlimited containers
```

---

## 3️⃣ CONTAINER LIFECYCLE

```
   IMAGE
     │
     │ docker run
     ▼
   ┌──────────┐  docker stop   ┌──────────┐  docker start  ┌──────────┐
   │ RUNNING  │ ─────────────► │ STOPPED  │ ─────────────► │ RUNNING  │
   │          │ ◄───── docker ─│ (exists) │                │  again   │
   └────┬─────┘     restart     └────┬─────┘                └──────────┘
        │                            │
        │ docker rm -f               │ docker rm
        ▼                            ▼
   ┌──────────┐                  ┌──────────┐
   │  GONE │                  │  GONE │
   └──────────┘                  └──────────┘

   (volume mounted? data SAFE — container gone, volume zinda)
```

---

## 4️⃣ DAY 1 — Spring on HOST + MySQL Container

```
   TERA LAPTOP (host)                        DOCKER
   ┌─────────────────────────┐              ┌──────────────────────┐
   │                          │              │                       │
   │  ┌──────────────────┐   │              │  ┌─────────────────┐  │
   │  │ Spring Boot      │   │  localhost   │  │ MySQL Container │  │
   │  │ (java -jar)       │ ──┼─── 3307 ────┼─►│  (mysql-userdb) │  │
   │  │                   │   │  (mapped)   │  │                  │  │
   │  │ application.     │   │              │  │  port 3306       │  │
   │  │ properties:      │   │              │  │  (internal)      │  │
   │  │ url=localhost:3307│   │              │  │                  │  │
   │  └──────────────────┘   │              │  └─────────────────┘  │
   │                          │              │      │                │
   │                          │              │      ▼                │
   │                          │              │  ┌──────────────┐    │
   │                          │              │  │ mysql_data   │    │
   │                          │              │  │ (volume —    │    │
   │                          │              │  │  persistent) │    │
   │                          │              │  └──────────────┘    │
   └─────────────────────────┘              └──────────────────────┘

   Profile: docker
   Connection: HOST → port mapping → CONTAINER
   Workbench: localhost:3307
```

---

## 5️⃣ VOLUMES — Disaster + Recovery PROOF

```
   WITHOUT VOLUME:                  WITH VOLUME:
   ┌──────────────┐                 ┌──────────────┐    ┌──────────────┐
   │ Container    │                 │ Container    │ -v │ Volume       │
   │ /var/lib/    │                 │ /var/lib/    │ ──►│ mysql_data   │
   │   mysql      │                 │   mysql      │    │ (host disk)  │
   │ (data here)  │                 │              │    │ (data here)  │
   └──────┬───────┘                 └──────┬───────┘    └──────────────┘
          │ docker rm -f                   │ docker rm -f       │
          ▼                                ▼                    │
        ALL                          Container               │
        DATA                            GONE                    │
        GONE                                                   
                                                          DATA SAFE
                                                                │
                                        Naya container          │
                                        + same -v ◄─────────────┘
                                                ▼
                                          DATA WAHI 

   Lesson: Container ephemeral, Volume persistent
```

---

## 6️⃣ DAY 2 — BOTH Containers (Compose stack)

```
   TERA LAPTOP
   ┌─────────────────────────────────────────────────────────────┐
   │                                                              │
   │  DOCKER (compose-managed network: usercrud_default)         │
   │  ┌──────────────┐                       ┌──────────────┐    │
   │  │ Spring Boot  │                       │ MySQL        │    │
   │  │ Container    │   ── mysql:3306 ───►  │ Container    │    │
   │  │ (usercrud-app)│  (service NAME +    │ (mysql-userdb)│   │
   │  │              │   internal port)      │              │    │
   │  │ port 8080    │                       │ port 3306    │    │
   │  │ (mapped to   │                       │ (mapped 3307 │    │
   │  │  host)        │                       │  for Workbench)│ │
   │  └──────────────┘                       └──────────────┘    │
   │       │                                       │              │
   │       │                                       ▼              │
   │       │                                  mysql_data (volume) │
   │       │                                                      │
   │   Postman ───► localhost:8080                               │
   │   Workbench ──► localhost:3307                               │
   └─────────────────────────────────────────────────────────────┘

   Profile: compose
   Connection: container → service name DNS → container
   No port mapping needed for INTERNAL talk
```

---

## 7️⃣ NETWORKS — Apartment Building

```
   DOCKER HOST (apartment building)

   default bridge network            custom bridge "app-net"
   ──────────────────────            ────────────────────
   ┌──────┐  ┌──────┐                ┌──────┐  ┌──────┐
   │ C1   │  │ C2   │                │ C1   │◄►│ C2   │
   └──────┘  └──────┘                └──────┘  └──────┘
                                       ▲          ▲
   Talk only by IP                    │   DNS   │
   No name resolution                 └──────────┘
   deprecated for app                Container NAME = hostname
                                        production pattern
```

---

## 8️⃣ COMPOSE FLOW (1 command = full stack)

```
   docker-compose.yml
       │
       │ docker-compose up -d --build
       ▼
   ┌─────────────────────────────────────┐
   │ COMPOSE engine:                      │
   │  1. Reads YAML                       │
   │  2. Auto-creates network              │
   │  3. Builds Spring image (Dockerfile) │
   │  4. Starts MySQL container            │
   │  5. Waits HEALTHCHECK pass           │
   │  6. Starts Spring container           │
   │  7. Injects env vars (PROFILE)        │
   │  8. Both join network                 │
   └─────────────────────────────────────┘
       │
       ▼
   Stack LIVE 

   docker-compose down → all containers stop + clean
   docker-compose down -v → also volumes (DATA GONE)
```

---

## 9️⃣ PROFILE BRIDGE — Compose ↔ Spring

```
   docker-compose.yml
   ┌────────────────────────────────────┐
   │ services:                           │
   │   app:                              │
   │     environment:                    │
   │       SPRING_PROFILES_ACTIVE:       │
   │         compose                     │  ← Compose ka kaam yahan tak
   └─────────┬──────────────────────────┘
             │
             │ container start time — ENV VAR INJECT
             ▼
   ┌────────────────────────────────────┐
   │ Spring Boot inside container        │  ← Spring's job here
   │   1. Reads env var                  │
   │   2. Detects profile=compose         │
   │   3. Auto-loads:                     │
   │      application-compose.properties  │
   │   4. Datasource URL = mysql:3306     │
   └────────────────────────────────────┘

   COMPOSE = inject env var
   SPRING = read env var, load profile, configure
   BRIDGE = single env variable
```

---

## ENTRYPOINT PATTERN (Production)

```
   CMD (overridable):                ENTRYPOINT (fixed):
   ─────────────────                 ──────────────────
   docker run img echo X             docker run img --debug
        ↓                                 ↓
   CMD REPLACED                      ENTRYPOINT same
   Runs: echo X                      Runs: java -jar app.jar --debug
   accidental risk                 args APPEND, base safe

   Production:
      ENTRYPOINT ["java", "-jar", "app.jar"]    ← fixed binary
      Config via ENV VARS (12-factor)
```

---

## MENTAL MODEL — Spring Location = URL Pattern

```
┌────────────────────────────────────────────────────────────────┐
│  Spring kahan?  │  URL pattern        │  Why?                │
├────────────────────────────────────────────────────────────────┤
│  HOST (laptop)  │  localhost:3307     │  Host's localhost    │
│                 │                     │  + port mapping       │
│  CONTAINER      │  mysql:3306         │  Service NAME        │
│                 │                     │  + Docker DNS        │
└────────────────────────────────────────────────────────────────┘

LOCK THIS — yeh Day 1 vs Day 2 ka asli farak
```

---

## INTERVIEW POWER PHRASE

```
"Docker = Image (template) + Container (instance) + Volume (persist)
+ Network (talk) + Compose (orchestrate). Spring on host = localhost
+ port mapping; Spring in container = service name + DNS. Compose
YAML declarative entire stack — depends_on healthcheck for ordering,
external volumes for data reuse. Profile via SPRING_PROFILES_ACTIVE
env var = Compose injects, Spring auto-loads matching properties.
ENTRYPOINT for fixed binary, CMD for default args. openjdk deprecated
→ Eclipse Temurin. Production multi-container = Compose pattern,
K8s migration easy from here."
```

---

## ULTIMATE MEMORY HOOK

```
Image       = Recipe  (read-only)
Container   = Dish     (running, ephemeral)
Volume      = Cabinet  (persistent, host disk)
Network     = Phone   (container talk by name)
Compose     = Recipe  (whole meal in one YAML)
Profile     = Outfit  (env-specific config)
ENTRYPOINT  = Fixed   (immutable base command)
```

[← Back to README](00_README.md) | [← Reference](07_reference.md)
