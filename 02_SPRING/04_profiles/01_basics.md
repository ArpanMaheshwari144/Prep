# Section 1 — Spring Profiles Basics

> **Topic:** Environment-specific config switching
> **Setup:** Day 1 — local + docker profile files

[← Back to README](00_README.md) | [Compose Integration →](02_compose_integration.md)

---

# Spring Profiles — Environment-Specific Configuration

> **Spring Boot Topic — clean config switching (dev/prod/local/docker)**
> Hands-on: UserCRUD project mein local MySQL + Docker MySQL switch

---

## The Problem (Why Profiles?)

```
Same app → DIFFERENT environments mein chalti
   • Local laptop  (dev)
   • Staging server
   • Production
   • Docker container

Har environment mein DIFFERENT config:
   • Different DB URLs
   • Different passwords
   • Different log levels
   • Different feature flags

Without Profiles:
   Har deploy se pehle properties EDIT karo manually
   Fragile, error-prone, secrets accidental commit

With Profiles:
   Multiple config files, ONE-LINE switch
   Clean, safe, production pattern
```

---

## STORY — Clothes for Occasion

```
   Ek banda (= App)
       │
       ├─ Office → Suit            (= Production config)
       ├─ Gym    → Shorts            (= Dev config)
       └─ Home   → Pajamas           (= Local config)

Same banda, different OUTFIT for situation.

Spring Profiles = different "outfits" (configs) for different environments.
```

---

## Core Concept (visual)

```
   Spring Boot App
        │
        │ application.properties (always loaded — common stuff)
        │
        │ + Active profile chosen at startup
        │
        ▼
   ┌────────────────────────────────────────┐
   │  Profile-specific:                       │
   │                                          │
   │  application-local.properties      OR   │
   │  application-docker.properties     OR   │
   │  application-prod.properties            │
   │                                          │
   │  Profile-specific ke values OVERRIDE    │
   │  master file ke values (jab conflict)   │
   └────────────────────────────────────────┘
```

---

## File Naming Convention

```
src/main/resources/
├── application.properties              ← MASTER (always loaded)
├── application-{profile}.properties    ← PROFILE-specific
│   • application-dev.properties
│   • application-prod.properties
│   • application-local.properties
│   • application-docker.properties
│   • application-test.properties
│
└── application.yml (alternative — YAML format)
```

**Naming rule:** `application-<profilename>.properties` — Spring auto-detects.

---

## 3 Ways to Activate Profile

### 1. **In `application.properties` (default)**
```properties
spring.profiles.active=local
```
Default jab koi specify nahi.

### 2. **Command Line (override)**
```cmd
java -jar app.jar --spring.profiles.active=docker
```
Highest priority — overrides file value.

### 3. **Environment Variable**
```cmd
set SPRING_PROFILES_ACTIVE=prod
java -jar app.jar
```
Useful for CI/CD, Docker, K8s deployment.

### Precedence (highest → lowest):
```
1. Command line argument          ← strongest
2. Environment variable
3. application.properties value
4. Profile-specific file
5. Default values in code         ← weakest
```

---

## Loading Visual (when profile=local)

```
   App start
       │
       ▼
   Spring reads:
   ┌─────────────────────────────────────┐
   │ STEP 1: application.properties      │
   │   → spring.profiles.active=local    │
   │   → spring.jpa.dialect=MySQLDialect │
   │   → jwt.secret=...                  │
   └─────────────────────────────────────┘
       │
       │ "Profile = local detected"
       ▼
   ┌─────────────────────────────────────┐
   │ STEP 2: application-local.properties│
   │   → spring.datasource.url=...3306   │
   │   → spring.datasource.password=root │
   └─────────────────────────────────────┘
       │
       │ MERGE (local overrides master if conflict)
       ▼
   ┌─────────────────────────────────────┐
   │ FINAL CONFIG (in memory):           │
   │   url=jdbc:mysql://3306/userdb     │
   │   password=root                     │
   │   dialect=MySQLDialect              │
   │   jwt.secret=...                    │
   └─────────────────────────────────────┘
       │
       ▼
   App starts with merged config
```

---

## UserCRUD Project — Real Setup (jo kiya)

### File structure:
```
src/main/resources/
├── application.properties              ← profile-active=local + JPA + JWT
├── application-local.properties        ← MySQL 3306 (local MySQL57)
└── application-docker.properties       ← MySQL 3307 (Docker container)
```

### `application.properties` (MASTER):
```properties
spring.application.name=usercrud
spring.profiles.active=local                        # Default profile

# JPA (common to all profiles)
spring.jpa.database-platform=org.hibernate.dialect.MySQLDialect
spring.jpa.hibernate.ddl-auto=update
spring.jpa.show-sql=true

# JWT (common)
jwt.secret=...
jwt.expiration=900000
jwt.refresh.expiration=604800000
```

### `application-local.properties`:
```properties
spring.datasource.url=jdbc:mysql://localhost:3306/userdb
spring.datasource.driverClassName=com.mysql.cj.jdbc.Driver
spring.datasource.username=root
spring.datasource.password=root
```

### `application-docker.properties`:
```properties
spring.datasource.url=jdbc:mysql://localhost:3307/userdb
spring.datasource.driverClassName=com.mysql.cj.jdbc.Driver
spring.datasource.username=root
spring.datasource.password=rootpass
```

---

## Switch Commands (one-liner)

```cmd
REM Default (local profile from application.properties)
java -jar target\usercrud-0.0.1-SNAPSHOT.jar

REM Override to local explicitly
java -jar target\usercrud-0.0.1-SNAPSHOT.jar --spring.profiles.active=local

REM Override to Docker
java -jar target\usercrud-0.0.1-SNAPSHOT.jar --spring.profiles.active=docker
```

### Console mein verification:
```
The following 1 profile is active: "local"     ← ya "docker"

HikariPool-1 - Starting...
HikariPool-1 - Added connection com.mysql.cj.jdbc.ConnectionImpl@xxx
   (port 3306 = local OR 3307 = docker)
```

---

## Production Use Cases

```
COMMON PROFILES (industry standard):

dev          → Developer local environment (laptop)
   • H2 / local DB
   • Verbose logging (DEBUG)
   • Hot reload

staging      → Pre-production (testing environment)
   • Production-like DB
   • Production-like data
   • Some debug logging

prod         → Live production
   • Real production DB
   • ERROR-only logs
   • Optimized JVM, security hardened

test         → Automated tests
   • In-memory DB (H2)
   • Auto-cleanup

docker       → Inside containerized deployment
   • Service hostnames (mysql-userdb)
   • Container-friendly paths
```

---

## Multiple Active Profiles

```properties
# application.properties
spring.profiles.active=docker,prod-secrets,monitoring
```

**Use case:** Combine partial configs.
- `docker` → DB connection
- `prod-secrets` → real passwords
- `monitoring` → metrics endpoints enabled

**Files load order:** Last in list = highest priority.

---

