# Docker — Complete Notes (UserCRUD Project)

> **Dates:** 2026-05-05 (Day 1) + 2026-05-06 (Day 2)
> **Goal:** Spring Boot + MySQL + Docker — full production-grade stack
> **Mode:** Hands-on, step-by-step, every concept locked with REAL command output

---

## READ ORDER (Strict — section by section)

```
1. 00_README.md          ← yeh file (start here)
                              Overview + reading order

2. 01_foundation.md       ← Docker basics
                              Image vs Container, install, run, ps, logs
                              [Day 1, ~30 min read]

3. 02_spring_mysql_host.md ← Spring on HOST + MySQL container
                              localhost:3307 pattern (port mapping)
                              [Day 1, ~45 min read]

4. 03_volumes.md          ← Data persistence
                              Disaster + recovery LIVE PROOF
                              [Day 1, ~30 min read]

5. 04_transition.md       ← KEY BRIDGE — Day 1 → Day 2
                              Spring location → URL pattern
                              [~10 min, MUST READ before Day 2]

6. 05_networks.md         ← Container-to-container DNS
                              Custom bridge network, service names
                              [Day 2, ~20 min read]

7. 06_docker_compose.md   ← Full stack orchestration
                              docker-compose.yml + Spring container
                              [Day 2, ~45 min read]

8. 07_reference.md         ← Quick lookups
                              Commands, traps, power phrase, workflow
                              [reference — visit anytime]

9. 08_revision_visual.md   ← Visual recap (mega summary)
                              All 10 concepts in diagrams
                              [revision — visit before interview]

10. 09_multistage_builds.md ← Production image optimization
                              40% size reduction PROVEN
                              JDK build → JRE runtime pattern
                              [Day 2 — production gold]

11. 10_arg_vs_env.md       ← Build-time vs runtime variables
                              ARG (build-only) vs ENV (runtime)
                              Secrets handling, override priority
                              [Day 2 — interview classic]
```

---

## What You'll Learn

```
DAY 1 — Foundation + Integration:
   Docker Desktop install (WSL 2 backend)
   Image vs Container concept (Recipe vs Dish)
   docker pull / run / ps / logs / exec / stop / rm
   Port mapping (-p host:container)
   Environment variables (-e KEY=value)
   Container isolation (local vs Docker MySQL)
   Spring Boot ↔ Docker MySQL real integration
   Maven manual install (user-level, no admin)
   Hibernate 7 dialect trap fix
   Postman tests proven working
   VOLUMES — disaster + recovery LIVE PROOF

DAY 2 — Networks + Orchestration:
   Custom Bridge Networks (DNS magic)
   Container-to-container talk via service NAME
   docker-compose.yml structure
   Spring Boot ALSO containerized
   Multi-container production stack
   Healthcheck + depends_on
   External volume reuse
   Eclipse Temurin migration (openjdk deprecated)
   Spring Profiles + Compose env var integration
```

---

## KEY INSIGHT — Spring Location Determines URL Pattern

```
┌────────────────────────────────────────────────────────────────┐
│  Spring Location  │  URL pattern        │  Why?                │
├────────────────────────────────────────────────────────────────┤
│  HOST (laptop)    │  localhost:3307     │  Host's localhost    │
│  (Day 1)          │                     │  + port mapping       │
│                   │                     │                      │
│  CONTAINER        │  mysql:3306         │  Service NAME        │
│  (Day 2)          │                     │  + Docker DNS        │
│                   │                     │  + same network      │
└────────────────────────────────────────────────────────────────┘

This is THE main concept that ties everything together.
```

---

## Project Architecture (Final State — Day 2)

```
TERA LAPTOP
  ┌─────────────────────────────────────────────────────────────┐
  │  Postman / Workbench                                         │
  │       │                                                      │
  │       │ HTTP / JDBC                                          │
  │       ▼                                                      │
  │  DOCKER                                                      │
  │  ┌──────────────┐                       ┌──────────────┐    │
  │  │ Spring Boot  │                       │ MySQL        │    │
  │  │ Container    │   ─── mysql:3306 ───► │ Container    │    │
  │  │ (usercrud-app)│                      │ (mysql-userdb)│   │
  │  │              │                       │              │    │
  │  │ port 8080    │                       │ port 3306    │    │
  │  │ (mapped)     │                       │ + 3307 mapped│    │
  │  └──────────────┘                       └──────────────┘    │
  │                                                  │           │
  │                                                  │           │
  │                                                  ▼           │
  │                                            mysql_data        │
  │                                            (volume —         │
  │                                             persistent)      │
  └─────────────────────────────────────────────────────────────┘
```

---

## Files Created During This Journey

```
TERA PROJECT MEIN:
   02_SPRING/PROJECT/usercrud/
   ├── Dockerfile                       ← Spring image recipe
   ├── .dockerignore                    ← exclude from build context
   ├── docker-compose.yml               ← multi-container orchestration
   ├── pom.xml                          ← MySQL Connector dependency added
   └── src/main/resources/
       ├── application.properties        ← master (common config)
       ├── application-local.properties  ← Spring host + Local MySQL57
       ├── application-docker.properties ← Spring host + Docker MySQL (port 3307)
       └── application-compose.properties ← Spring container + MySQL container

NOTES (yahan):
   02_SPRING/DOCKER/
   ├── 00_README.md                      ← yeh file
   ├── 01_foundation.md
   ├── 02_spring_mysql_host.md
   ├── 03_volumes.md
   ├── 04_transition.md
   ├── 05_networks.md
   ├── 06_docker_compose.md
   └── 07_reference.md
```

---

## What's Pending (Docker Phase Remaining)

```
ENTRYPOINT vs CMD          (interview classic — ~30 min)
Multi-stage builds         (production gold — ~45 min)
ARG vs ENV                 (build vs runtime — ~15 min)
HEALTHCHECK                (deepen, already touched — ~15 min)
Image push to Docker Hub   (optional, sharing — ~30 min)

= ~2 hours focused work for Docker FULL completion
```

---

## Power Phrase (interview lock)

> *"Docker = Image (template) + Container (instance) + Volume (persist) + Network (talk) + Compose (orchestrate). Spring on host = localhost+port-mapping; Spring in container = service-name+DNS. Compose YAML declarative entire stack — depends_on healthcheck for ordering, external volumes for data reuse. Spring profile via SPRING_PROFILES_ACTIVE env var (Compose injects, Spring auto-loads matching properties). openjdk deprecated → Eclipse Temurin. Production multi-container = Compose pattern, K8s migration easy from here."*

---

## Next Step (after reading these notes)

```
1. Read all sections in order (above 1-8)
2. Try docker-compose up -d --build (verify your stack)
3. Postman test (login should work)
4. Move to advanced Docker topics (ENTRYPOINT, multi-stage)
5. Then K8s
```

**Ready? Start with [01_foundation.md](01_foundation.md)** →
