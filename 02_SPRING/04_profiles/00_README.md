# 🎭 Spring Profiles — Complete Notes

> **Topic:** Environment-specific configuration in Spring Boot
> **Hands-on:** UserCRUD project — local + docker + compose profiles
> **Real demo:** Profile switching Day 1 + Compose integration Day 2

---

## 📚 READ ORDER

```
1. 📖 00_README.md          ← yeh file (start here)
                              Overview + reading order

2. 🟢 01_basics.md          ← Why profiles, file naming, activation
                              3 ways to activate, file load mechanism
                              [Day 1 — locked]

3. 🟡 02_compose_integration.md ← Day 2 update
                              Docker Compose + Spring profile bridge
                              Env var SPRING_PROFILES_ACTIVE = trigger
                              Spring auto-loads matching file
                              [Day 2 — locked]

4. 🔵 03_reference.md        ← Q&A + Traps + Power Phrase
                              Interview-ready quick lookups
```

---

## 🎯 What You'll Learn

```
✅ Why profiles (env-specific config)
✅ File naming convention (application-{name}.properties)
✅ 4 activation ways (CLI / env var / properties / Compose YAML)
✅ Activation precedence (CLI > env > properties)
✅ Spring auto-load mechanism
✅ Profile-specific @Profile annotation
✅ Multi-active profiles
✅ Compose integration (Day 2)
✅ Production reality (CI/CD + AWS deployment patterns)
✅ Secrets handling (Vault / Secrets Manager)
✅ 12-factor app principle
```

---

## 💎 Project Files Created

```
src/main/resources/
├── application.properties              ← master (common config)
├── application-local.properties        ← Spring host + Local MySQL57 (3306)
├── application-docker.properties       ← Spring host + Docker MySQL (3307)
└── application-compose.properties      ← Spring container + MySQL container (mysql:3306)

Switch via:
   --spring.profiles.active=local       (CLI)
   SPRING_PROFILES_ACTIVE=docker         (env var)
   environment in docker-compose.yml     (YAML inject)
```

---

## 🎯 Three Profiles — When to Use What

```
┌────────────────────────────────────────────────────────────────┐
│ Profile     │ Spring Location  │ MySQL Location  │ JDBC URL      │
├────────────────────────────────────────────────────────────────┤
│ local       │ HOST             │ HOST (MySQL57)  │ localhost:3306│
│ docker      │ HOST             │ Docker container│ localhost:3307│
│             │                  │                 │ (port mapping)│
│ compose     │ Container        │ Container       │ mysql:3306    │
│             │                  │                 │ (service name)│
└────────────────────────────────────────────────────────────────┘
```

---

## 💎 Power Phrase

> *"Spring Profiles = environment-specific configuration. application-{profile}.properties files banao (local, docker, compose, dev, prod). Activate via property/CLI/env var/Compose YAML. Master file mein common stuff, profile files mein env-specific. Docker-Compose se Spring = SPRING_PROFILES_ACTIVE env var bridge — Compose inject karta, Spring auto-loads matching file. Container DNS pattern: profile mein service name use (mysql:3306) NOT localhost. JAR ek hi build, runtime pe profile decide — 12-factor app pattern."*

---

**Ready? Start with [01_basics.md](01_basics.md)** →
