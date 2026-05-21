# ─── BASE IMAGE ─────────────────────────────────────
# Eclipse Temurin 21 JRE = lightweight Java 21 runtime
# JRE chahiye sirf (JDK nahi — humne jar pehle build kiya)
# Production image = small (jre only, no full JDK)
FROM eclipse-temurin:21-jre

# ─── WORKING DIRECTORY ──────────────────────────────
# Container ke andar /app folder banao + waha cd kar
WORKDIR /app

# ─── COPY JAR ───────────────────────────────────────
# Maven build output (target/*.jar) ko container mein copy
# Local path: target/todoapp-0.0.1-SNAPSHOT.jar
# Container path: /app/app.jar (rename kar diya simplicity ke liye)
COPY target/todoapp-0.0.1-SNAPSHOT.jar app.jar

# ─── PORT EXPOSE ────────────────────────────────────
# Spring Boot default port 8080
# Docker ko bata "is container ka 8080 port external use ho sakta"
EXPOSE 8080

# ─── STARTUP COMMAND ────────────────────────────────
# Container start = "java -jar app.jar" run kare
# ENTRYPOINT = override nahi ho sakta easily (CMD ke vs)
ENTRYPOINT ["java", "-jar", "app.jar"]
