# Section 3 — Reference (Q&A + Traps + Power Phrase)

> **Use:** Quick lookup for interview prep

[← Back to README](00_README.md) | [← Compose Integration](02_compose_integration.md)

---

## Interview Q&A

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

## Trap Box

```
Trap 1: Profile filename typo
         application-prod.properties (correct)
            application-PROD.properties (case-sensitive!)
         Lowercase profile names follow naming exactly

Trap 2: Common settings duplicate in profile files
         Sab profile files mein JWT secret duplicate
         Common stuff master file mein, DB-specific profile mein

Trap 3: Profile-specific file load nahi hua
         Forgot spring.profiles.active=X
         Console verify: "1 profile is active: ..."

Trap 4: Production secrets profile file mein commit
         application-prod.properties mein DB password git mein
         Production: env vars / Vault / AWS Secrets Manager
            Files mein dummy/placeholder values

Trap 5: @Profile beans mismatch
         @Profile("dev") class but actively prod profile
         Bean register nahi hoga, NoSuchBeanDefinitionException

Trap 6: Default profile kabhi mention nahi
         No active profile + only profile-specific files
         application.properties (no suffix) ALWAYS loads
            Default profile name = "default" (implicit)

Trap 7: Profile + Docker combo
         Hardcode profile in JAR — Docker mein switch nahi
         Docker run mein env var:
             docker run -e SPRING_PROFILES_ACTIVE=docker ...
```

---

## Power Phrase (interview lock)

> *"Spring Profiles = environment-specific configuration. application-{profile}.properties files banao (local, docker, dev, prod). Activate via property/CLI/env var (env var highest priority for production). Master file mein common stuff, profile files mein DB+environment-specific. `@Profile` annotation se Java beans bhi profile-aware. JAR ek hi build karo, runtime pe profile decide — clean, safe, 12-factor app pattern."*

---

## Memory Hook

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

## Quick Reference

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

## Today's UserCRUD Setup (proven)

```
application.properties           — common (JPA, JWT, default profile)
application-local.properties     — Local MySQL57 (3306, root)
application-docker.properties    — Docker MySQL (3307, rootpass)
application-compose.properties   — Both Spring + MySQL containerized 

Switch tested:
--spring.profiles.active=local   → local MySQL pe writes (port 3306)
--spring.profiles.active=docker  → Docker MySQL pe writes (port 3307)
SPRING_PROFILES_ACTIVE=compose   → via env var in docker-compose.yml 
```

---

---

## Updated Memory Hook

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

## Updated Quick Reference

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

## Today's Compose Setup Verified Working

```
File structure:
   src/main/resources/
   ├── application.properties              (master)
   ├── application-local.properties        (Spring host + Local MySQL)
   ├── application-docker.properties       (Spring host + Docker MySQL)
   └── application-compose.properties   (Spring container + MySQL container)

docker-compose.yml:
   environment:
     SPRING_PROFILES_ACTIVE: compose

Result:
   Compose injects env var
   Spring auto-loads application-compose.properties
   jdbc:mysql://mysql:3306/userdb (service name + internal port)
   Spring Boot container connects to MySQL container by NAME
   Postman /auth/login → 200 OK + new JWT
   Same Arpan from yesterday's volume
```
