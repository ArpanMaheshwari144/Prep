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
#   • openjdk:17-jdk-slim = Official OpenJDK 17 + slim Linux (Debian)
#   • "slim" = sirf zaruri files (~200 MB), full Debian (~700 MB) nahi
#   • Java 17 already pre-installed — humein sirf jar daalna
#
# Trade-off:
#   slim       → small + fast deploy, lekin missing utilities (curl, etc.)
#   full       → bigger but more tools
#   alpine     → even smaller (~80 MB), but musl libc (sometimes Java issues)
FROM openjdk:17-jdk-slim


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
# LAYER 5: Container start command
# ─────────────────────────────────────────────
# CMD = "Container start hote hi yeh command chalao"
#   • Exec form ["a", "b", "c"] preferred over shell form "a b c"
#   • Reason: Linux signals (SIGTERM) properly handle hote
#   • java -jar app.jar = Spring Boot up, embedded Tomcat shuru, port 8080 listen
CMD ["java", "-jar", "app.jar"]


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
