# ============================================================
# 🐳 UserCRUD — Dockerfile
# ============================================================
# Yeh file = "recipe" jo Docker ko batati hai:
#   1. Kaunsa base OS + Java use karna
#   2. Tera jar kaha rakhna
#   3. Container start hote hi kya chalana
#
# Build:  docker build -t usercrud:1.0 .
# Run:    docker run -d -p 8080:8080 --name usercrud-app usercrud:1.0
# ============================================================


# ─────────────────────────────────────────────
# LAYER 1: Base image
# ─────────────────────────────────────────────
# FROM = "Yeh image se shuru kar"
#
# IMPORTANT: openjdk:17-jdk-slim DEPRECATED on Docker Hub (2024-25 onwards)
#   Official "openjdk" image archived — use Eclipse Temurin (industry standard)
#
#   • eclipse-temurin:17-jdk-jammy = Eclipse Temurin Java 17 + Ubuntu Jammy
#   • Eclipse Temurin = OpenJDK build by Eclipse Adoptium (production-grade)
#   • "jammy" = Ubuntu 22.04 LTS base (~250 MB)
#
# Alternatives:
#   eclipse-temurin:17-jre-jammy   → JRE only (smaller, ~150 MB) — enough for running jar
#   eclipse-temurin:17-jdk-alpine  → smallest (~80 MB), uses musl libc
#   amazoncorretto:17               → Amazon's OpenJDK distribution
FROM eclipse-temurin:17-jdk-jammy


# ─────────────────────────────────────────────
# LAYER 2: Working directory inside container
# ─────────────────────────────────────────────
# WORKDIR = "Container ke andar /app folder bana, wahi se kaam"
#   • cd /app jaise — saare baad ke COPY/RUN/CMD yahi se relative
#   • Best practice: explicit folder rakho, root mein clutter nahi
WORKDIR /app


# ─────────────────────────────────────────────
# LAYER 3: Copy the JAR
# ─────────────────────────────────────────────
# COPY <source-on-host> <dest-in-container>
#   • Source: tera laptop ka path (relative to Dockerfile location)
#   • Dest:   container ke andar /app/app.jar
#
# Spring Boot fat jar:
#   target/usercrud-0.0.1-SNAPSHOT.jar
#   = code + saari dependencies + embedded Tomcat (sab ek jar mein)
#
# Rename "app.jar" simple rakha — version bata na pade CMD mein
COPY target/usercrud-0.0.1-SNAPSHOT.jar app.jar


# ─────────────────────────────────────────────
# LAYER 4: Expose port (DOCUMENTATION)
# ─────────────────────────────────────────────
# EXPOSE = "Yeh container kis port pe listen karega — readers ko bata"
#   • SIRF documentation purpose — actual port binding nahi karta
#   • Real port binding: `docker run -p 8080:8080` mein hota hai
#   • Spring Boot default 8080 pe — application.properties se override possible
EXPOSE 8080


# ─────────────────────────────────────────────
# LAYER 5: Container start command (PRODUCTION PATTERN)
# ─────────────────────────────────────────────
# ENTRYPOINT vs CMD — interview classic:
#
# OLD (CMD only):
#   CMD ["java", "-jar", "app.jar"]
#   → Easy override: docker run img echo X (replaces entire CMD)
#   → Risk: accidental override
#
# NEW (ENTRYPOINT — fixed binary):
#   ENTRYPOINT ["java", "-jar", "app.jar"]
#   → Always runs java -jar app.jar (immutable)
#   → docker run img --debug → java -jar app.jar --debug (args APPEND)
#   → Override needs --entrypoint flag (intentional)
#
# Config via ENV VARS (12-factor app style):
#   docker-compose.yml mein:
#     environment:
#       SPRING_PROFILES_ACTIVE: compose
#   docker run -e flag:
#     docker run -e SPRING_PROFILES_ACTIVE=docker my-app
#
# Form: EXEC ["a", "b"] (JSON array) — PID 1 = java process
#   → Signals (SIGTERM) directly reach Spring Boot → graceful shutdown ✅
# Avoid SHELL form (java -jar app.jar) → wraps in /bin/sh, signals broken
ENTRYPOINT ["java", "-jar", "app.jar"]


# ============================================================
# 🧠 Concept Recap (after reading above):
# ============================================================
# Dockerfile          → instructions text file (recipe)
# docker build  .     → reads Dockerfile, creates IMAGE
# docker run    image → image se naya CONTAINER start
# Container           → tera Spring Boot app live, isolated, portable
#
# Layered file system:
#   Har instruction = ek "layer" (cached separately)
#   Slow-changing layers PEHLE (FROM, WORKDIR, base deps)
#   Fast-changing layers BAAD MEIN (COPY jar)
#   → Build fast hota subsequent times
# ============================================================
