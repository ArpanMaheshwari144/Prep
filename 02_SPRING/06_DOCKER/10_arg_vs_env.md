# Section F — ARG vs ENV (Build-Time vs Runtime Config)

> **Topic:** Two types of variables in Dockerfile — common interview confusion
> **Real-world:** Often misused, both look similar but very different scope

[← Back to README](00_README.md) | [← Multi-stage](09_multistage_builds.md)

---

## The Big Question

```
"Dockerfile mein ARG aur ENV — dono variables hi hain.
 Differ kya?"
```

**Short answer:** ARG = build-time only. ENV = runtime + buildtime.

---

## Docker Lifecycle (where each lives)

```
SOURCE → docker build → IMAGE → docker run → CONTAINER
            ↑                       ↑
            │                       │
            ARG zone                ENV zone
            (build only)            (runtime persistent)
```

---

## First — Build vs Run Difference (foundation)

```
docker build → READS Dockerfile, compiles into IMAGE
   • Reads recipe
   • Follows each instruction
   • Saves IMAGE on disk
   • Dockerfile ka kaam khatam yahan

docker run   → DOESN'T touch Dockerfile, runs IMAGE
   • Image pre-built use karta
   • Container instantiate + start
   • Recipe NOT re-read

Key insight: Image ek baar bani, container 100 chala sakte
```

---

## STORY — Cooking vs Eating

```
ARG = INGREDIENTS in kitchen
   ┌──────────────────────────┐
   │ Flour, eggs, oil          │
   │ Used DURING cooking       │
   │ Once dish ready, GONE     │
   │ Customer ko nahi dikhte    │
   └──────────────────────────┘

ENV = CONDIMENTS on table
   ┌──────────────────────────┐
   │ Salt, sauce, pepper       │
   │ ALWAYS available          │
   │ Customer use kar sakta    │
   │ Persistent during meal    │
   └──────────────────────────┘
```

---

## Crisp Definitions

```
ARG (Build-time):
   • Available DURING `docker build`
   • GONE in final image (not visible to running container)
   • Used for: build choices, version pinning, build-time tokens
   • Override: --build-arg flag

ENV (Runtime):
   • Available INSIDE running container
   • Process can read it (System.getenv() in Java)
   • Used for: app config, profile switching, runtime defaults
   • Override: -e flag, docker-compose environment, --env-file
```

---

## Dockerfile Example — Both Together

```dockerfile
# ─────────────────────────────────────
# ARG — build-time only
# ─────────────────────────────────────
ARG JAVA_VERSION=17                     # default value
ARG APP_VERSION=0.0.1-SNAPSHOT

FROM eclipse-temurin:${JAVA_VERSION}-jdk-jammy
WORKDIR /app
COPY target/usercrud-${APP_VERSION}.jar app.jar


# ─────────────────────────────────────
# ENV — available in running container
# ─────────────────────────────────────
ENV SPRING_PROFILES_ACTIVE=docker       # default profile
ENV TZ=Asia/Kolkata                      # timezone
ENV JAVA_OPTS="-Xms256m -Xmx512m"        # JVM options

EXPOSE 8080
ENTRYPOINT ["java", "-jar", "app.jar"]
```

---

## Override Examples

### ARG (build-time):
```cmd
REM Default ARG values used
docker build -t myapp .

REM Override ARG at build time
docker build --build-arg JAVA_VERSION=21 --build-arg APP_VERSION=1.0.0 -t myapp .
```

### ENV (runtime):
```cmd
REM Use Dockerfile defaults
docker run myapp

REM Override ENV at runtime
docker run -e SPRING_PROFILES_ACTIVE=prod -e TZ=UTC myapp
```

### docker-compose.yml ENV override:
```yaml
services:
  app:
    image: myapp
    environment:
      SPRING_PROFILES_ACTIVE: compose      # overrides Dockerfile ENV
      TZ: Asia/Kolkata
```

---

## Side-by-Side Comparison

| | **ARG** | **ENV** |
|---|---|---|
| Available during build | Yes | Yes |
| Available at runtime | No | Yes |
| Override mechanism | `--build-arg` | `-e` or `environment:` |
| Inside container env | Not visible | Visible |
| Default in Dockerfile | `ARG KEY=val` | `ENV KEY=val` |
| Typical use | Build choices | Runtime config |
| Bake into image | Yes (during build) | Default (overridable) |
| Docker history visible | Yes (security risk) | Yes (security risk) |

---

## Visual — Where Each Lives

```
┌─────────────────────────────────────┐
│  Dockerfile                          │
│                                      │
│  ARG JAVA_VERSION=17                 │  ← build-time only
│  ARG APP_VERSION=1.0                 │
│                                      │
│  FROM eclipse-temurin:${JAVA_VERSION}│  ← ARG used here
│                                      │
│  ENV SPRING_PROFILES_ACTIVE=docker   │  ← runtime, baked
│  ENV JAVA_OPTS=-Xmx512m              │
└─────────────────────────────────────┘
       │
       │ docker build (ARG processed, gone)
       ▼
┌─────────────────────────────────────┐
│  IMAGE (template)                    │
│  • App jar                           │
│  • ENV vars baked (SPRING_PROFILES,  │
│    JAVA_OPTS)                         │
│  • ARG vars NOT here               │
└─────────────────────────────────────┘
       │
       │ docker run (or with -e to override)
       ▼
┌─────────────────────────────────────┐
│  CONTAINER (running)                 │
│  • Process can read:                  │
│      $SPRING_PROFILES_ACTIVE          │
│      $JAVA_OPTS                      │
│  • Can override at runtime via -e     │
│  • ARG vars NOT visible inside        │
└─────────────────────────────────────┘
```

---

## Common Patterns

### Pattern 1 — Build-time version pinning (ARG)
```dockerfile
ARG JAVA_VERSION=17
FROM eclipse-temurin:${JAVA_VERSION}-jre-jammy
```
**Use case:** Single Dockerfile, multiple Java versions.
```cmd
docker build --build-arg JAVA_VERSION=17 -t myapp:java17 .
docker build --build-arg JAVA_VERSION=21 -t myapp:java21 .
```

---

### Pattern 2 — ARG → ENV bridge
```dockerfile
ARG APP_VERSION=0.0.1-SNAPSHOT
ENV APP_VERSION=${APP_VERSION}    # bridge: build-time → runtime
```
**Use case:** Want build-time value visible at runtime too (e.g., version endpoint).

---

### Pattern 3 — Runtime defaults with override
```dockerfile
ENV SPRING_PROFILES_ACTIVE=docker     # default if not overridden
```
```yaml
# docker-compose.yml
environment:
  SPRING_PROFILES_ACTIVE: compose       # override from compose
```
**Use case:** Sensible default, overridable per environment.

---

### Pattern 4 — Multi-stage with ARG
```dockerfile
ARG JAVA_VERSION=17

FROM maven:3.9-eclipse-temurin-${JAVA_VERSION} AS builder
# ... build ...

FROM eclipse-temurin:${JAVA_VERSION}-jre-jammy
ARG JAVA_VERSION    # MUST RE-DECLARE — ARG scope is per stage
# ... runtime ...
```
**Use case:** Same version across stages.

---

## Common Traps

```
Trap 1: ARG mein secret rakhna
         ARG DB_PASSWORD=secret123
            → Visible in docker history
         Use BuildKit secrets (--secret) for build-time secrets

Trap 2: ENV mein sensitive data permanently
         ENV DB_PASSWORD=secret123
            → ENV permanently in image, leaks if image shared
         ENV mein placeholder; real value via -e at runtime
            OR via Vault / AWS Secrets Manager

Trap 3: ARG before FROM not visible after FROM
         ARG JAVA_VERSION=17
            FROM eclipse-temurin:${JAVA_VERSION}-jdk
            RUN echo $JAVA_VERSION   ← empty here!
         Re-declare ARG after FROM:
            ARG JAVA_VERSION=17
            FROM eclipse-temurin:${JAVA_VERSION}-jdk
            ARG JAVA_VERSION         ← re-declare to use
            RUN echo $JAVA_VERSION   ← now works

Trap 4: Multi-stage ARG scope confusion
         Stage 1 mein ARG declare, Stage 2 mein use
         Each FROM = new stage = re-declare ARG

Trap 5: Confusing ENV override priority
         Docker priority (highest → lowest):
         1. docker run -e               (CLI)
         2. docker-compose environment  (YAML)
         3. Dockerfile ENV              (default)

Trap 6: ARG visible in docker history
         Anyone with image can: docker history myapp
            → Sees all ARG values used at build time
         Don't put secrets in ARG. Use BuildKit secrets:
            RUN --mount=type=secret,id=token cat /run/secrets/token
```

---

## Security Note — Where to Put Secrets

```
NEITHER ARG NOR ENV ideally for production secrets!

OPTIONS:
   BuildKit secrets (build-time secrets, not in history)
   AWS Secrets Manager (runtime fetch)
   HashiCorp Vault (runtime fetch)
   Kubernetes Secrets (runtime mount)
   Docker Secrets (Swarm)
   env-file at deploy time (.env, gitignored)

PROD PATTERN:
   spring.datasource.password=${DB_PASSWORD}    ← placeholder
   ↓
   At runtime: env var injected from Secrets Manager
   Container reads ENV → resolves placeholder
```

---

## Interview Q&A

**Q1: "ARG vs ENV difference?"**

> *"ARG = build-time only, available during `docker build` but GONE in final image. ENV = runtime + buildtime, available inside running container. ARG for build choices (Java version pinning, build args), ENV for app config (profile, JVM opts). Both can have defaults; both overridable — ARG via `--build-arg`, ENV via `-e` or compose environment block."*

**Q2: "Secret kahan rakhu — ARG ya ENV?"**

> *"NEITHER ideally for production secrets. ARG visible in `docker history` (security risk). ENV permanently in image (leaks if image shared). Production: BuildKit secrets for build-time, AWS Secrets Manager / Vault for runtime — inject via env at deploy time, NEVER bake into image."*

**Q3: "ARG before FROM works?"**

> *"FROM ke pehle declare kiya ARG = sirf FROM line mein use ho sakta. After FROM use karna hai? Re-declare karo phir. Each FROM in multi-stage = separate ARG scope — re-declare in each stage."*

**Q4: "ENV override priority?"**

> *"Highest priority: `docker run -e KEY=value` (CLI). Then `docker-compose environment` block. Lowest: `ENV KEY=value` in Dockerfile (default). CLI/compose override Dockerfile defaults."*

---

## Power Phrase

> *"ARG = build-time only (gone after build), ENV = runtime + build (available in running container). ARG for build choices (Java version, --build-arg override). ENV for runtime config (profile, JVM opts, -e override). Secrets in NEITHER — use BuildKit secrets / Vault / Secrets Manager. ARG before FROM = visible only in FROM; re-declare after FROM. ENV baked into image as default, overridable at runtime."*

---

## Memory Hook

```
ARG = "ingredients" (cooking only — gone at table)
ENV = "salt/sauce" (table mein hamesha available)

Build time:    ARG   ENV
Runtime:       ARG   ENV

Override syntax:
   ARG  → docker build --build-arg KEY=value
   ENV  → docker run -e KEY=value
          docker-compose: environment:

Scope rule:
   ARG before FROM = only in FROM line
   ARG after FROM  = available in that stage
   Multi-stage = re-declare in each stage
```

---

## Concept Locked

```
ARG = build-time only
ENV = runtime + buildtime
Override mechanisms (--build-arg vs -e)
ARG/FROM scope rules
Secret handling (NEITHER for prod secrets)
Real patterns (version pinning, ARG→ENV bridge)
Common traps (history visibility, scope confusion)
Production secrets handling (BuildKit / Vault / Secrets Manager)
```

[← Back to README](00_README.md) | [← Multi-stage](09_multistage_builds.md)
