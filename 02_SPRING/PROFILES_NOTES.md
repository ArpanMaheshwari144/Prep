# 🎭 Spring Profiles — Environment-Specific Configuration

> **Spring Boot Topic — clean config switching (dev/prod/local/docker)**
> Hands-on: UserCRUD project mein local MySQL + Docker MySQL switch

---

## 🤔 The Problem (Why Profiles?)

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

❌ Without Profiles:
   Har deploy se pehle properties EDIT karo manually
   Fragile, error-prone, secrets accidental commit
   
✅ With Profiles:
   Multiple config files, ONE-LINE switch
   Clean, safe, production pattern
```

---

## 🎬 STORY — Clothes for Occasion

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

## 🎯 Core Concept (visual)

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

## 📁 File Naming Convention

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

## 🚀 3 Ways to Activate Profile

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

### 🏆 Precedence (highest → lowest):
```
1. Command line argument          ← strongest
2. Environment variable
3. application.properties value
4. Profile-specific file
5. Default values in code         ← weakest
```

---

## 🎨 Loading Visual (when profile=local)

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

## 🧪 UserCRUD Project — Real Setup (jo kiya)

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

## 🎯 Switch Commands (one-liner)

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

## 🌍 Production Use Cases

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

## 🔥 Multiple Active Profiles

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

## 🎤 Interview Q&A

**Q: "Spring Profiles kya hain aur kab use?"**

> *"Same app ko different environments mein chalane ke liye config separate. application-{profile}.properties files banao, `spring.profiles.active=X` se switch. Common: dev/staging/prod/test profiles. JAR ek hi banao, deploy time pe profile pick — flexibility + safety."*

**Q: "Activate karne ke 3 tareeke?"**

> *"1) application.properties mein `spring.profiles.active=X`. 2) CLI arg `--spring.profiles.active=X`. 3) Env var `SPRING_PROFILES_ACTIVE=X`. CLI highest priority — file value override karta. Production: env var (12-factor app principle)."*

**Q: "Profile mein @Profile annotation?"**

> *"Java code mein bhi profile-specific beans bana sakte:*
> ```java
> @Component
> @Profile("docker")
> public class DockerOnlyService { ... }
> 
> @Component
> @Profile({"prod", "staging"})
> public class MonitoringService { ... }
> ```
> *Active profile match nahi to bean register hi nahi hota."*

**Q: "Multiple profiles active kaise?"**

> *"`spring.profiles.active=docker,monitoring,prod-secrets` — comma-separated. Useful when partial configs combine karne hain (DB + logging + secrets alag profiles)."*

**Q: "Profile-specific @ConfigurationProperties?"**

> *"`@ConditionalOnProperty` ya `@ConfigurationProperties` + `@Profile` together. Type-safe configuration with profile-aware loading."*

---

## ⚠️ Trap Box

```
🪤 Trap 1: Profile filename typo
         ❌ application-prod.properties (correct)
            application-PROD.properties (case-sensitive!)
         ✅ Lowercase profile names follow naming exactly

🪤 Trap 2: Common settings duplicate in profile files
         ❌ Sab profile files mein JWT secret duplicate
         ✅ Common stuff master file mein, DB-specific profile mein

🪤 Trap 3: Profile-specific file load nahi hua
         ❌ Forgot spring.profiles.active=X
         ✅ Console verify: "1 profile is active: ..."

🪤 Trap 4: Production secrets profile file mein commit
         ❌ application-prod.properties mein DB password git mein
         ✅ Production: env vars / Vault / AWS Secrets Manager
            Files mein dummy/placeholder values

🪤 Trap 5: @Profile beans mismatch
         ❌ @Profile("dev") class but actively prod profile
         ✅ Bean register nahi hoga, NoSuchBeanDefinitionException

🪤 Trap 6: Default profile kabhi mention nahi
         ❌ No active profile + only profile-specific files
         ✅ application.properties (no suffix) ALWAYS loads
            Default profile name = "default" (implicit)

🪤 Trap 7: Profile + Docker combo
         ❌ Hardcode profile in JAR — Docker mein switch nahi
         ✅ Docker run mein env var:
             docker run -e SPRING_PROFILES_ACTIVE=docker ...
```

---

## 💎 Power Phrase (interview lock)

> *"Spring Profiles = environment-specific configuration. application-{profile}.properties files banao (local, docker, dev, prod). Activate via property/CLI/env var (env var highest priority for production). Master file mein common stuff, profile files mein DB+environment-specific. `@Profile` annotation se Java beans bhi profile-aware. JAR ek hi build karo, runtime pe profile decide — clean, safe, 12-factor app pattern."*

---

## 🧠 Memory Hook

```
Profiles = "Outfits for occasion"
   • Office → Suit (prod)
   • Gym    → Shorts (dev)
   • Home   → Pajamas (local)

File naming:
   application.properties              ← always loaded (common)
   application-{name}.properties       ← profile-specific override

Activate (3 ways, priority high→low):
   1. CLI:       --spring.profiles.active=X
   2. Env var:   SPRING_PROFILES_ACTIVE=X
   3. Property:  spring.profiles.active=X (in master file)

Common profiles:
   dev / staging / prod / test / local / docker

Multi-active:
   spring.profiles.active=docker,monitoring,secrets

Java code:
   @Profile("dev")  // bean only in dev profile
   @Profile({"prod", "staging"})  // multi-profile

Production pattern:
   • Master file: structure + defaults
   • Profile files: env-specific
   • Secrets: env vars / Vault (NEVER commit)
```

---

## 📋 Quick Reference

```cmd
REM File create karna
src/main/resources/application-{name}.properties

REM Switch profile (3 ways):
java -jar app.jar --spring.profiles.active=local
java -jar app.jar --spring.profiles.active=docker
java -jar app.jar --spring.profiles.active=prod

REM Multi-profile active
java -jar app.jar --spring.profiles.active=docker,monitoring

REM Env variable (Windows)
set SPRING_PROFILES_ACTIVE=docker
java -jar app.jar

REM Env variable (Linux/Mac)
export SPRING_PROFILES_ACTIVE=docker
java -jar app.jar

REM Verify active profile (console first lines)
"The following 1 profile is active: \"local\""
```

---

## 🎯 Today's UserCRUD Setup (proven)

```
✅ application.properties           — common (JPA, JWT, default profile)
✅ application-local.properties     — Local MySQL57 (3306, root)
✅ application-docker.properties    — Docker MySQL (3307, rootpass)
✅ application-compose.properties   — Both Spring + MySQL containerized 🆕

Switch tested:
✅ --spring.profiles.active=local   → local MySQL pe writes (port 3306)
✅ --spring.profiles.active=docker  → Docker MySQL pe writes (port 3307)
✅ SPRING_PROFILES_ACTIVE=compose   → via env var in docker-compose.yml 🆕
```

---

# 🐙 PART 2 — DOCKER-COMPOSE + Spring Profiles (Day 2)

> **Date:** 2026-05-06
> **Real production-pattern:** Spring Boot bhi container mein, MySQL bhi container mein

---

## 🤔 Problem (jo hua aaj)

```
Pehle 2 profiles the:
   • local   → Spring on host, Local MySQL on host
   • docker  → Spring on host, MySQL container (localhost:3307)

Aaj need:
   • Compose mein Spring ALSO container → mysql container ko 
     localhost:3307 se NAHI reach kar sakta
     (localhost = container ka APNA self, not host)
   
   Solution: Compose network mein "mysql" service NAME se reach
     URL: jdbc:mysql://mysql:3306/userdb
                       ↑↑↑↑↑       ↑↑↑↑
                       service     INTERNAL port (NOT mapped 3307)
                       name (DNS)
```

---

## 🆕 New Profile File — `application-compose.properties`

```properties
spring.datasource.url=jdbc:mysql://mysql:3306/userdb
spring.datasource.driverClassName=com.mysql.cj.jdbc.Driver
spring.datasource.username=root
spring.datasource.password=rootpass
```

### Compare 3 Profiles Now:

```
┌──────────────────────────────────────────────────────────────────┐
│ Profile     │ Spring Location │ MySQL Location  │ JDBC URL        │
├──────────────────────────────────────────────────────────────────┤
│ local       │ HOST            │ HOST (MySQL57)  │ localhost:3306  │
│ docker      │ HOST            │ Docker container│ localhost:3307  │
│             │                 │ (port mapping)  │ (mapped port)   │
│ compose 🆕  │ Container       │ Container       │ mysql:3306      │
│             │                 │                 │ (service name + │
│             │                 │                 │  internal port) │
└──────────────────────────────────────────────────────────────────┘
```

---

## 🔥 KEY INSIGHT — Compose & Spring Coordinate via ENV VAR

> **Question:** "docker-compose ne file load ki?"
> **Answer:** **NO. Spring ne ki — Compose ne sirf env var inject ki.**

### The Mechanism (visual):

```
     docker-compose.yml
   ┌──────────────────────────────────┐
   │ services:                         │
   │   app:                            │
   │     environment:                  │
   │       SPRING_PROFILES_ACTIVE:     │
   │         compose                   │  ← Compose ka kaam yahin tak
   └────────┬─────────────────────────┘
            │
            │ container start hota hai
            │ Compose env var INJECT karta
            │
            ▼
   ┌──────────────────────────────────┐
   │ Spring Boot inside container      │  ← Yahan se Spring ka kaam
   │                                    │
   │ Step 1: Read env var               │
   │   SPRING_PROFILES_ACTIVE=compose   │
   │                                    │
   │ Step 2: Always load                │
   │   application.properties (master)  │
   │                                    │
   │ Step 3: Detect active profile      │
   │   "compose" detected               │
   │                                    │
   │ Step 4: AUTO-LOAD                  │
   │   application-compose.properties   │
   │                                    │
   │ Step 5: Merge configs              │
   │   profile values override master   │
   │   → datasource URL = mysql:3306    │
   └──────────────────────────────────┘
```

---

## 🎯 Two Independent Systems — Their Roles:

```
DOCKER-COMPOSE ka role:
   ❌ Spring config files load karna
   ✅ Container start pe ENV VAR inject karna
   ✅ Network/volume setup
   ✅ Service ordering (depends_on)
   ✅ Healthcheck monitoring
   
SPRING ka role:
   ✅ Env vars + properties read karna
   ✅ Profile-based file selection (Spring's standard mechanism)
   ✅ Config merging (master + profile)
   
BRIDGE: Single env variable (SPRING_PROFILES_ACTIVE)
        Compose injects → Spring reads → magic
```

---

## 📝 Real Setup From Today (working code)

### docker-compose.yml — relevant section:
```yaml
services:
  app:
    build: .
    environment:
      SPRING_PROFILES_ACTIVE: compose    # ← THE TRIGGER
    depends_on:
      mysql:
        condition: service_healthy
```

### application-compose.properties — gets auto-loaded:
```properties
spring.datasource.url=jdbc:mysql://mysql:3306/userdb
spring.datasource.username=root
spring.datasource.password=rootpass
```

### Verification (Spring Boot startup logs):
```
The following 1 profile is active: "compose"
HikariPool-1 - Added connection com.mysql.cj.jdbc.ConnectionImpl@xxxx
   (connecting to mysql:3306 successfully)
Tomcat started on port 8080
```

---

## 🌍 Production Reality Check (12-factor extension)

```
SAME compose YAML file, SAME image, SAME code → multiple environments

DEV:
   environment:
     SPRING_PROFILES_ACTIVE: dev
     DB_PASSWORD: ${DEV_DB_PASS}    # from CI vars

STAGING:
   environment:
     SPRING_PROFILES_ACTIVE: staging
     DB_PASSWORD: ${STAGING_DB_PASS}

PROD:
   environment:
     SPRING_PROFILES_ACTIVE: prod
     DB_PASSWORD: ${PROD_DB_PASS}   # from AWS Secrets Manager

Code change: ZERO
Image: SAME
Behavior: DIFFERENT (env var driven)
```

---

## 🎯 Activation Methods (Updated 4-way list)

```
1. application.properties        (default profile fallback)
   spring.profiles.active=local

2. Command line argument         (override)
   java -jar app.jar --spring.profiles.active=docker

3. OS Environment variable       (highest, useful in shells)
   set SPRING_PROFILES_ACTIVE=docker
   java -jar app.jar

4. Docker/Compose env var 🆕     (production deployment)
   environment:
     SPRING_PROFILES_ACTIVE: compose
   
PRECEDENCE (highest → lowest):
   CLI arg > Env var > properties file value > default
```

---

## 🎤 Updated Interview Q&A

**Q: "Docker compose mein Spring profile kaise activate karte?"**

> *"docker-compose.yml mein service ke `environment:` block mein `SPRING_PROFILES_ACTIVE: <name>` set karte. Container start pe Compose env var inject karta. Spring Boot inside container env var read karta, matching application-{name}.properties auto-load karta. Compose aur Spring 2 alag systems hain, env var bridge kar deta — clean separation."*

**Q: "Spring application.properties container mein kaise jaata?"**

> *"Properties file JAR ke andar pre-baked hota (resources folder). Container start pe Spring filesystem se nahi, classpath se read karta. Profile activation = property + profile combine. JAR build time pe properties package, runtime pe env var driven profile selection."*

**Q: "External database password Compose mein kaise inject?"**

> *"Compose YAML mein env var directly hardcode = anti-pattern. Production: `.env` file (git-ignored), or Compose secrets, or AWS Secrets Manager passthrough. Spring uses placeholder ${DB_PASSWORD} — env var resolves at runtime."*

---

## ⚠️ New Trap (Compose-specific)

```
🪤 Trap 8: Compose env var typo
         ❌ SPRING_PROFILES_ACTIVE: compose-profile
            (no application-compose-profile.properties exists)
         ✅ SPRING_PROFILES_ACTIVE: compose
            (matches application-compose.properties suffix exactly)

🪤 Trap 9: localhost in compose profile
         ❌ application-compose.properties:
            url=jdbc:mysql://localhost:3307/userdb
            → Spring container looks for MySQL inside ITSELF
            → connection refused
         ✅ Use service name from compose YAML:
            url=jdbc:mysql://mysql:3306/userdb
            → Container DNS resolves to MySQL container

🪤 Trap 10: Profile file not in JAR
         ❌ application-compose.properties bahar accidentally
            (target/classes mein nahi gaya)
         ✅ src/main/resources/application-compose.properties
            (Maven package process auto-include karta)
            Verify: jar tf target/X.jar | grep compose
```

---

## 💎 Updated Power Phrase (Day 1 + Day 2)

> *"Spring Profiles = environment-specific configuration. application-{profile}.properties files banao (local, docker, compose, dev, prod). Activate via property/CLI/env var/Compose YAML — env var highest priority for production. Master file mein common stuff, profile files mein env-specific. Docker-Compose se Spring = `SPRING_PROFILES_ACTIVE` env var bridge — Compose inject karta, Spring auto-loads matching file. Container DNS pattern: profile mein service name use (jdbc:mysql://mysql:3306) NOT localhost. JAR ek hi build, runtime pe profile decide — 12-factor app pattern."*

---

## 🧠 Updated Memory Hook

```
Profiles = "Outfits for occasion"

File pattern:
   application.properties              ← always loaded (common)
   application-{name}.properties       ← profile-specific override

Activate (4 ways now, priority high→low):
   1. CLI:       --spring.profiles.active=X
   2. Env var:   SPRING_PROFILES_ACTIVE=X (incl. Docker/Compose)
   3. Property:  spring.profiles.active=X (in master file)
   4. Default:   none = "default" profile

Compose integration:
   docker-compose.yml → environment block → SPRING_PROFILES_ACTIVE
   ↓
   Spring inside container auto-reads + auto-loads matching file
   ↓
   2 systems coordinate via env var bridge

Naming for context:
   local     → host dev with local MySQL
   docker    → host Spring + container MySQL (localhost:mapped-port)
   compose   → both containerized (service-name:internal-port)
   prod      → real production env vars / Vault
```

---

## 📋 Updated Quick Reference

```cmd
REM Profile via different mechanisms:

REM 1. CLI
java -jar app.jar --spring.profiles.active=local

REM 2. OS env var (Windows)
set SPRING_PROFILES_ACTIVE=docker
java -jar app.jar

REM 3. OS env var (Linux/Mac)
export SPRING_PROFILES_ACTIVE=prod
java -jar app.jar

REM 4. Docker run -e flag
docker run -e SPRING_PROFILES_ACTIVE=docker my-app:1.0

REM 5. docker-compose.yml
services:
  app:
    environment:
      SPRING_PROFILES_ACTIVE: compose

REM 6. Kubernetes
env:
  - name: SPRING_PROFILES_ACTIVE
    value: "prod"

REM Verify active profile (console first lines):
"The following 1 profile is active: \"compose\""
```

---

## ✅ Today's Compose Setup Verified Working

```
File structure:
   src/main/resources/
   ├── application.properties              (master)
   ├── application-local.properties        (Spring host + Local MySQL)
   ├── application-docker.properties       (Spring host + Docker MySQL)
   └── application-compose.properties 🆕   (Spring container + MySQL container)

docker-compose.yml:
   environment:
     SPRING_PROFILES_ACTIVE: compose

Result:
   ✅ Compose injects env var
   ✅ Spring auto-loads application-compose.properties
   ✅ jdbc:mysql://mysql:3306/userdb (service name + internal port)
   ✅ Spring Boot container connects to MySQL container by NAME
   ✅ Postman /auth/login → 200 OK + new JWT
   ✅ Same Arpan from yesterday's volume
```
