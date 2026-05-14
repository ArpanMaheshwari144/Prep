# 🚀 Section E — Multi-Stage Builds (Production Gem)

> **Topic:** Production-grade image optimization
> **Real proof:** UserCRUD image 256 MB → 154 MB (40% smaller)
> **Day 2 (continued)** — added after ENTRYPOINT vs CMD

📚 [← Back to README](00_README.md) | [← Visual Recap](08_revision_visual.md)

---

## 🤔 The Problem (Single-Stage Dockerfile)

```dockerfile
# Current Dockerfile (single-stage):
FROM eclipse-temurin:17-jdk-jammy        ← FULL JDK (~250 MB)
WORKDIR /app
COPY target/usercrud-0.0.1-SNAPSHOT.jar app.jar
ENTRYPOINT ["java", "-jar", "app.jar"]
```

### Issues:
```
1. JDK has compile tools (javac, jar, jlink)
   → BUT runtime mein sirf java -jar chahiye
   → JDK = JRE + javac + tools = WASTED in production
   
2. Build leftovers in final image
   → Maven cache, downloaded deps
   → Bigger attack surface (security)
   
3. Slower deploys
   → 256 MB content × 100 servers = 25.6 GB total
   → CI/CD pipelines slower (push/pull big images)
```

---

## 💡 The Solution — Multi-Stage Build

```
Stage 1: BUILD       (heavy — JDK + Maven, build tools)
   ↓ produces: app.jar
Stage 2: RUNTIME     (light — JRE only, just enough to run)
   ↓ copies: app.jar (from Stage 1)
   ↓ Stage 1 DISCARDED

Final image = ONLY Stage 2 content
```

---

## 🎬 Story — Restaurant Kitchen vs Plate

```
TWO ROOMS:

KITCHEN (build stage):                 DINING TABLE (runtime stage):
  • Full appliances (JDK)               • Just plate + food (JRE + jar)
  • Knives, mixers (Maven, javac)        • No knives needed (already cooked)
  • Raw ingredients (source code)        • No mixer (compile done)
  • Heat / mess (cache, leftovers)       • Clean
  • Tools                                • Result only
  ↓
  Cook karo (mvn package)
  ↓
  PLATE on table (final dish — app.jar) ← User sirf yahi dekhta

Customer ko kitchen ki zarurat nahi — sirf finished plate.
Yahi multi-stage = build mess discard, runtime clean.
```

---

## 🎨 Visual — Single vs Multi-Stage

```
SINGLE-STAGE (current):
┌────────────────────────────┐
│  eclipse-temurin:17-JDK     │  ← 250 MB base (heavy)
│  + jar (50 MB)              │
│  + Maven leftovers           │
│  + build tools               │
│  = ~256 MB FINAL CONTENT     │
└────────────────────────────┘
   ↓
   Final image = THIS (everything bundled)


MULTI-STAGE:
┌────────────────────────────┐  ┌────────────────────────────┐
│  STAGE 1 (build):           │  │  STAGE 2 (runtime):         │
│  maven:3.9-eclipse-temurin  │  │  eclipse-temurin:17-JRE     │
│  Maven + JDK + source code  │  │  + jar (only)               │
│  Build jar                   │──►│                              │
│                              │  │  COPY --from=builder         │
│  → produces: app.jar         │  │     /build/target/*.jar      │
│  (THIS STAGE DISCARDED)      │  │                              │
│                              │  │  = ~154 MB FINAL CONTENT     │
└────────────────────────────┘  └────────────────────────────┘
                                  ↑
                                  Final image = ONLY this stage
                                  (Stage 1 thrown away)
```

---

## 📝 Multi-Stage Dockerfile — Complete

```dockerfile
# ════════════════════════════════════════════════════════════
# STAGE 1: BUILDER (heavy — JDK + Maven)
# ════════════════════════════════════════════════════════════
FROM maven:3.9-eclipse-temurin-17 AS builder
WORKDIR /build

# Cache deps separately (slow-changing layer)
COPY pom.xml .
RUN mvn dependency:go-offline -B

# Source + build (fast-changing layer)
COPY src ./src
RUN mvn clean package -DskipTests
# Output: /build/target/usercrud-0.0.1-SNAPSHOT.jar


# ════════════════════════════════════════════════════════════
# STAGE 2: RUNTIME (light — JRE only)
# ════════════════════════════════════════════════════════════
FROM eclipse-temurin:17-jre-jammy
WORKDIR /app

# Copy ONLY the jar from Stage 1
COPY --from=builder /build/target/usercrud-*.jar app.jar

# Production: non-root user (security best practice)
RUN groupadd -r spring && useradd -r -g spring spring
USER spring

EXPOSE 8080
ENTRYPOINT ["java", "-jar", "app.jar"]
```

---

## 🔍 Decode — Each Magic Line

```
FROM maven:3.9-eclipse-temurin-17 AS builder
   ↑↑↑↑↑↑↑↑↑                              ↑↑↑↑↑↑↑↑↑↑↑
   First stage (build env)                Named "builder"
                                          (referenced later)

COPY pom.xml + RUN mvn dependency:go-offline
   ↑
   This LAYER cached. Only re-runs jab pom.xml change.
   Smart caching = fast subsequent builds.

COPY src ./src + RUN mvn package
   ↑
   Source code copy + build (fast-changing).
   target/*.jar produced.

FROM eclipse-temurin:17-jre-jammy
   ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
   NEW STAGE — completely separate image
   JRE only (no compile tools needed at runtime)

COPY --from=builder /build/target/*.jar app.jar
   ↑↑↑↑↑↑↑↑↑↑↑↑↑↑
   FROM builder stage — reach back, copy ONLY the jar
   Stage 1 ka baaki sab DISCARD ho jata
```

---

## 🧪 Hands-On — Both Built + Compared (REAL output)

### Build commands:
```cmd
cd C:\DSA_PRACTICE\02_SPRING\PROJECT\usercrud

REM Single-stage (default Dockerfile)
docker build -t usercrud-app:single .

REM Multi-stage (Dockerfile.multistage)
docker build -f Dockerfile.multistage -t usercrud-app:multi .
```

### Comparison output (REAL):
```
docker images usercrud-app

IMAGE                  DISK USAGE   CONTENT SIZE
─────                  ──────────   ────────────
usercrud-app:latest    758 MB       256 MB        (compose-built)
usercrud-app:multi     502 MB       154 MB ⭐
usercrud-app:single    758 MB       256 MB
```

### Real Savings:
```
Content Size: 256 MB → 154 MB
            = 102 MB SAVED
            = ~40% SMALLER

Disk Usage: 758 MB → 502 MB
          = 256 MB SAVED
          = ~34% SMALLER
```

---

## 💎 Production Impact (Real Math)

```
SCENARIO — 100 servers running this image:

   Single-stage:  100 × 256 MB = 25.6 GB total
   Multi-stage:   100 × 154 MB = 15.4 GB total
   
   SAVED: 10.2 GB across cluster
   
PLUS:
   ✅ CI/CD pipeline 40% faster (image push/pull)
   ✅ Bandwidth saved per deploy
   ✅ Container startup faster (less to load)
   ✅ Smaller attack surface (no compile tools in prod)
   ✅ Cleaner CVE scans
```

---

## 🎨 Layer Caching — Optimization Trick

```
Smart Dockerfile order:

GOOD (fast subsequent builds):
   COPY pom.xml .                      ← rare changes
   RUN mvn dependency:go-offline       ← cached most builds
   COPY src ./src                      ← frequent changes
   RUN mvn package                     ← only re-runs when src changes


BAD (cache busted every build):
   COPY . .                            ← any file change = invalidate cache
   RUN mvn package                     ← re-downloads all deps every time
```

---

## 📊 Image Size Spectrum (further optimization)

```
Variant                              Final Size
───────                              ──────────
Single-stage JDK                     ~256 MB
Multi-stage JRE-jammy (full ubuntu)  ~154 MB ⭐ (yahi banaya)
Multi-stage JRE-alpine               ~80 MB  ⭐⭐
Multi-stage jlink (custom JRE)       ~50 MB  ⭐⭐⭐

Production = JRE-jammy ya alpine usually
Custom jlink = max optimization for resource-constrained
```

---

## 🎤 Interview Q&A

**Q: "Multi-stage builds kya?"**

> *"Dockerfile mein multiple FROM statements — har FROM = separate build stage. Final image sirf last stage hota. Earlier stages mein heavy tools (JDK, Maven, npm), final mein bus runtime + result. Use case: Java JDK build → JRE runtime, Node npm install → distroless serve. ~40% size reduction common."*

**Q: "Final image kya contain karega?"**

> *"Sirf last FROM se aaya stage. Earlier stages discarded. `COPY --from=builder` se selective copy — sirf jar/binary. Tools, source code, intermediate files — sab gone."*

**Q: "Layer caching kaise optimize?"**

> *"Slow-changing layers PEHLE (FROM, deps install), fast-changing LAST (source code copy). pom.xml separate copy karke `mvn dependency:go-offline` chalao — cache hit deps, source change pe sirf compile re-run."*

**Q: "JDK vs JRE in containers?"**

> *"JDK = compile + runtime (Java compile tools, jar tool, jlink). JRE = runtime only (just enough to run jar). Container ka kaam = run jar (java -jar app.jar). So JRE perfect for production runtime stage. Build stage mein JDK chahiye. Multi-stage = JDK build → JRE runtime, get best of both."*

**Q: "Production mein non-root user kyu?"**

> *"Default container runs as root. Container compromise hua = attacker has root inside container. Privilege escalation possible. Best practice: create non-root user (e.g., 'spring'), USER directive switch. Reduces blast radius if breach."*

---

## ⚠️ Traps

```
🪤 Trap 1: COPY . . in early stage
         ❌ COPY everything → cache busted on any file change
         ✅ COPY pom.xml first, then COPY src — split layers

🪤 Trap 2: Forgetting --from=stage
         ❌ COPY /target/jar app.jar (from current stage — empty)
         ✅ COPY --from=builder /build/target/*.jar app.jar

🪤 Trap 3: Using JDK in runtime stage
         ❌ FROM eclipse-temurin:17-jdk in Stage 2 (waste)
         ✅ FROM eclipse-temurin:17-jre (lighter, sufficient)

🪤 Trap 4: Wildcard mismatch
         ❌ COPY --from=builder /build/target/usercrud.jar
            → version suffix mismatch (0.0.1-SNAPSHOT)
         ✅ COPY --from=builder /build/target/usercrud-*.jar app.jar

🪤 Trap 5: Forgetting non-root user
         ❌ Default root user in production
         ✅ groupadd + useradd + USER directive

🪤 Trap 6: Building tests in stage
         ❌ mvn package (runs tests, slow)
         ✅ mvn package -DskipTests (CI runs tests separately)
```

---

## 💎 Power Phrase

> *"Multi-stage Dockerfile = multiple FROM statements. Stage 1 (builder) heavy with JDK + Maven, builds jar. Stage 2 (runtime) light with JRE only, COPY --from=builder sirf jar. Final image = Stage 2 only — earlier stages discarded. Real proof: 256 MB → 154 MB (~40% reduction) for Spring Boot. Layer caching trick: pom.xml copy + dependency:go-offline early (cached), source last (changes often). Production: JRE not JDK, non-root USER, multi-stage = security + size + speed."*

---

## 🧠 Memory Hook

```
Multi-stage = "Kitchen + Plate"
   • Kitchen = Stage 1 (heavy build tools, mess)
   • Plate   = Stage 2 (clean, just food/result)
   • Final image = ONLY plate (kitchen ki zarurat nahi customer ko)

Pattern:
   FROM heavy AS builder     ← Stage 1
   ... build steps ...
   FROM light                 ← Stage 2 (NEW image)
   COPY --from=builder X      ← reach back, pull result
   ENTRYPOINT runtime command

Savings:
   ~40% smaller final image
   = production gold

Layer cache:
   Slow changes early (pom)   ← cache hit
   Fast changes late (src)    ← rebuild often
```

---

## ✅ Concept Locked

```
✅ Multi-stage syntax (FROM ... AS name)
✅ COPY --from=stage selective copy
✅ JDK build → JRE runtime pattern
✅ Layer caching optimization (pom first, src last)
✅ Non-root user for security
✅ Maven base image (maven:3.9-eclipse-temurin-17)
✅ Real size reduction PROVEN (40% smaller)
✅ Why production grade vs single-stage
```

📚 [← Back to README](00_README.md) | [← Visual Recap](08_revision_visual.md)
