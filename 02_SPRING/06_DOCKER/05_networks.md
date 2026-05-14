# 🟡 Section B1 — Networks (Container DNS Magic) (Day 2)

> **Goal:** Container-to-container talk via service NAME
> **Tool:** Custom bridge network

📚 [← Back to README](00_README.md) | [← Transition](04_transition.md) | [Compose →](06_docker_compose.md)

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
