# BRIDGE — Day 1 → Day 2 Transition (READ THIS!)

> **Critical insight:** Spring location decides URL pattern
> **Day 1:** Spring on host → localhost:3307
> **Day 2:** Spring in container → mysql:3306

[← Back to README](00_README.md) | [← Volumes](03_volumes.md) | [Networks →](05_networks.md)

---

# DAY 1 → DAY 2 TRANSITION (READ THIS FIRST!)

> **Yeh section sabse pehle padh — Day 1 vs Day 2 ka KEY difference**
> Spring ki LOCATION badli — wahi reason URL pattern bhi badla

---

## The Big Picture — Spring kahan hai?

```
Spring Boot = APPLICATION
   • Kahan chal raha = HOST (laptop pe) ya CONTAINER (Docker)?
   • Yeh decide karta connection URL kaisa hoga

MySQL = DATABASE
   • Kal aur aaj DONO din container mein tha
   • Wo same — bus Spring ki location badli
```

---

## DAY 1 (KAL) — Spring on HOST

```
TERA LAPTOP                            DOCKER
┌─────────────────────────┐           ┌──────────────────────┐
│                          │           │                       │
│  Spring Boot             │           │  MySQL Container     │
│  (java -jar)             │           │  (mysql-userdb)      │
│                          │           │                       │
│  HOST pe directly         │   ───►   │  Internal port: 3306 │
│  chal raha tha            │           │  Mapped to host: 3307│
│                          │           │                       │
│  application.properties:  │           │                       │
│  url=localhost:3307       │           │                       │
│       ↑                   │           │                       │
│       host ka 3307         │           │                       │
└─────────────────────────┘           └──────────────────────┘

Why localhost:3307 worked?
   1. Spring on HOST → "localhost" = tera laptop
   2. -p 3307:3306 mapping = host:3307 forwards to container:3306
   3. Spring → host:3307 → mapped to → MySQL container:3306
   4. Connection successful

Profile use: docker
Setup commands (kal):
   docker run -d --name mysql-userdb -p 3307:3306 ... mysql:8
   java -jar target\usercrud-...jar     ← Spring on host
```

---

## DAY 2 (AAJ) — Spring ALSO in CONTAINER

```
TERA LAPTOP
┌─────────────────────────────────────────────────────────────┐
│                                                              │
│  DOCKER                                                      │
│  ┌──────────────┐                       ┌──────────────┐    │
│  │ Spring Boot  │                       │ MySQL        │    │
│  │ Container    │   ──── mysql:3306 ──► │ Container    │    │
│  │ (usercrud-app)│                      │ (mysql-userdb)│   │
│  │              │                       │              │    │
│  │ application- │                       │ Internal:3306│    │
│  │ compose.    │                        │              │    │
│  │ properties:  │                        │              │    │
│  │ url=mysql:3306│                       │              │    │
│  │     ↑        │                       │              │    │
│  │     SERVICE  │                       │              │    │
│  │     NAME     │                       │              │    │
│  └──────────────┘                       └──────────────┘    │
│        │                                       │             │
│        └───────────────────────────────────────┘             │
│        Same network (usercrud_default — auto-created)        │
│                                                              │
└─────────────────────────────────────────────────────────────┘

Why "mysql:3306" worked (NOT localhost)?
   1. Spring AB CONTAINER mein → "localhost" = container apna
   2. Container ke localhost pe MySQL NAHI (Spring + jar only)
   3. Solution: MySQL container ko NAME se reach karo
   4. "mysql" = service name from docker-compose.yml
   5. Docker DNS resolves "mysql" → MySQL container's IP
   6. Connection container-to-container (no port mapping needed!)

Profile use: compose
Setup commands (aaj):
   docker-compose up -d --build      ← ek command sab kuch
```

---

## KEY INSIGHT — Spring ki LOCATION = URL Pattern

```
┌──────────────────────────────────────────────────────────────┐
│  Spring Location  │  URL pattern        │  Reason             │
├──────────────────────────────────────────────────────────────┤
│  HOST (laptop)    │  localhost:3307     │  Host ka localhost  │
│                   │                     │  + port mapping     │
│                   │                     │                     │
│  CONTAINER        │  mysql:3306         │  Service name       │
│                   │                     │  + Docker DNS       │
│                   │                     │  + same network     │
└──────────────────────────────────────────────────────────────┘
```

---

## Common Confusion CLEARED

### "Spring container ke localhost pe MySQL kyu nahi?"

```
GALAT MENTAL MODEL:
   "Hum ne MySQL ko container mein move kar diya kya?"

SAHI MENTAL MODEL:
   • MySQL kal AND aaj — ALAG container mein hi hai (HAMESHA)
   • Spring container = SIRF Java + jar (NO MySQL ever)
   • Each container = isolated world
   • Container ke andar localhost = wo container ka self

Spring container mein MySQL kabhi tha hi nahi.
Spring container ka localhost = Spring container apna.
Apne paas MySQL nahi → localhost se connect = connection refused.

Solution: MySQL container ko NAME (mysql) se reach karo
   → Docker DNS network mein resolve karta
   → Connection ban jati container-to-container
```

---

### "Aaj ne kya badla?"

```
KAL          → Spring on HOST + MySQL container = port mapping (localhost:3307)
AAJ          → Spring AB container mein bhi gaya
SAME         → MySQL hamesha container mein tha (no change)
ALAG         → Connection method:
                  Kal: localhost:3307 (port mapping)
                  Aaj: mysql:3306 (service name + DNS)
```

---

## Day 2 Mein 3 Cheezein Add Hui

```
1. application-compose.properties     (new profile file)
   → url=mysql:3306 (service name use kara)

2. docker-compose.yml                  (orchestrator)
   → mysql + app dono services define
   → "build: ." Spring ko Dockerfile se image bana

3. Same Dockerfile (kal banayi thi)    (Spring container recipe)
   → Java + jar package
   → ENTRYPOINT to run app
```

```
docker-compose up -d --build
        ↓
Compose YAML padhi
        ↓
Spring image build (Dockerfile use)
        ↓
MySQL container start (image se)
        ↓
Spring container start (image se, after MySQL healthy)
        ↓
Both join auto-network
        ↓
Spring uses "mysql:3306" → MySQL container reached by NAME
        ↓
Working — Spring in container + MySQL in container
```

---

## Bottom Line — Mental Model Lock:

```
1. MySQL hamesha container mein tha (Day 1 + Day 2)
2. Spring ki location badli (Day 1 host → Day 2 container)
3. Connection URL Spring ki location pe depend karta:
      • Spring on host → localhost:host-port (port mapping)
      • Spring in container → service-name:internal-port (DNS)
4. Container ke andar "localhost" = wo container ka apna world
5. Container-to-container talk = service NAME via Docker DNS
```

**Yeh transition samjho — phir aage Networks (PART 20) + Compose (PART 21) clear ho jayenge.**

---
