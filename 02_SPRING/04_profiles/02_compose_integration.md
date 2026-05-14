# 🟡 Section 2 — Compose + Profiles Integration (Day 2)

> **Topic:** How Docker Compose triggers Spring profile loading
> **Key:** SPRING_PROFILES_ACTIVE env var bridge

📚 [← Back to README](00_README.md) | [← Basics](01_basics.md) | [Reference →](03_reference.md)

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

