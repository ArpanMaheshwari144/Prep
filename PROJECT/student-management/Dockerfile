# =====================================================
# YE KYA HAI: Dockerfile — app ki image banane ki recipe
# KYUN CHAHIYE: "Mere machine pe chal raha" problem solve.
#               Kahi bhi same chalega — dev, staging, production.
# KAB USE: docker build -t student-app . && docker run -p 8080:8080 student-app
# AGAR NAHI HOTA: Har machine pe manually Java install, jar copy, config set.
#                 Naye developer ko setup mein 2 din. Docker se 1 command mein done.
# =====================================================

# Step 1: Base image — ye ek mini OS hai jisme Java 17 already installed
# Bina iske: Container mein Java nahi hoga — app chalegi hi nahi
# "slim" = chhota OS (200MB vs 700MB) — fast download, kam space
FROM openjdk:17-jdk-slim

# Step 2: Working directory — container ke andar kaam karne ki jagah
# Jaise tu laptop pe C:/projects/ mein kaam karta — container mein /app mein karega
# Bina iske: Sab files root (/) mein bikhrengi — messy, manage mushkil
WORKDIR /app

# Step 3: Copy — apni jar file laptop se container ke andar daal
# build/libs/*.jar = Gradle ne build karke jar banai (tera app)
# app.jar = container mein is naam se save hogi
# Bina iske: Container mein tera app hoga hi nahi — kya chalayega?
COPY build/libs/*.jar app.jar

# Step 4: Expose — container ka ye port bahar dikhega
# 8080 = Spring Boot default port
# Bina iske: Container ke andar app chalegi lekin bahar se access nahi hoga
# Ye sirf documentation hai — actual port mapping docker run -p mein hota
EXPOSE 8080

# Step 5: Entrypoint — container start hote hi ye command chalega
# java -jar app.jar = Spring Boot app start
# Bina iske: Container start hoga lekin kuch chalega nahi — khaali baithega
ENTRYPOINT ["java", "-jar", "app.jar"]
