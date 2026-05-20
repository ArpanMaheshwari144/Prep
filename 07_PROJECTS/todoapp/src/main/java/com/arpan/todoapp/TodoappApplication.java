package com.arpan.todoapp;
// ─── PACKAGE DECLARATION ─────────────────────────────────────
// Folder structure: src/main/java/com/arpan/todoapp/
// Dot (.) = folder separator
// Full unique name: com.arpan.todoapp.TodoappApplication
// All sub-folders under this = auto-scanned by @ComponentScan

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cache.annotation.EnableCaching;
// ─── IMPORTS ─────────────────────────────────────────────────
// SpringApplication        → Bootstrap class with .run() static method
// @SpringBootApplication   → THE magic annotation (3-in-1 combo)

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    TodoappApplication = ENTRY POINT of the entire app
//    Spring Initializr auto-generated yeh file
//    "main" method = JVM start point
//    Yahaan se sab Spring magic shuru hoti hai
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL FLOW:
//    ./mvnw spring-boot:run
//          │
//          ▼
//    JVM searches → "public static void main(String[])"
//          │
//          ▼
//    TodoappApplication.main(args)
//          │
//          ▼
//    SpringApplication.run(...)
//          │
//          ▼
//    @SpringBootApplication triggered
//       ├─ @Configuration (allow @Bean methods)
//       ├─ @EnableAutoConfiguration (auto setup from classpath)
//       └─ @ComponentScan (scan com.arpan.todoapp + sub-folders)
//          │
//          ▼
//    Spring Context ready → Tomcat starts on :8080 → APP RUNNING ✅
//
// 🔑 @SpringBootApplication — 3-IN-1 SUPERPOWER:
//
//    1. @Configuration
//       "Yeh class @Bean methods declare kar sakti"
//       (manual bean definition allowed)
//
//    2. @EnableAutoConfiguration
//       Spring khud detect karta kya configure:
//          • pom mein Tomcat detected → auto start Tomcat
//          • JPA detected → auto setup DataSource
//          • Redis detected → auto RedisTemplate
//          • MySQL detected → auto driver config
//
//    3. @ComponentScan
//       "Iss package + sub-packages mein scan karo"
//       Auto-detect:
//          @Controller, @RestController
//          @Service
//          @Repository
//          @Component
//       SAB beans ban jaate, DI wired automatically
//
// 🔑 SpringApplication.run() ke andar kya hota:
//    1. Banner print (Spring logo)
//    2. Environment setup (profiles, properties)
//    3. Auto-config triggered (based on classpath)
//    4. @ComponentScan classpath scan
//    5. Bean factory populate (all @Component classes)
//    6. Dependency Injection wiring
//    7. Embedded Tomcat start (default port 8080)
//    8. DB / Redis connect (if config available)
//    9. Application ready → listeners notify
//    10. Console: "Started TodoappApplication in X seconds"
//
// 🔑 MAIN METHOD BREAKDOWN:
//    public      → JVM ke liye accessible
//    static      → object banaye bina call ho sake (JVM rule)
//    void        → kuch return nahi karna
//    main        → JAVA RULE — entry method ka naam EXACTLY "main"
//    String[] args → command-line arguments (--port=9090 etc.)
//
// 🔑 TodoappApplication.class:
//    "Class reference" — class ka metadata pointer
//    Spring ko batata "iss class se @ComponentScan shuru karo"
//    Base package = com.arpan.todoapp
//    Sub-folders auto-scan honge:
//       • controller/  ← @RestController detect
//       • service/     ← @Service detect
//       • repository/  ← @Repository detect
//       • model/       ← @Entity detect
//
// 🎤 INTERVIEW LINE:
//    "TodoappApplication is the entry point — standard Java
//     class with main() method. @SpringBootApplication is a
//     meta-annotation combining @Configuration,
//     @EnableAutoConfiguration, and @ComponentScan.
//     SpringApplication.run() bootstraps the Spring context,
//     scans for beans, auto-configures based on classpath,
//     and starts the embedded Tomcat server."
// ═══════════════════════════════════════════════════════════════════════
@SpringBootApplication
@EnableCaching   // enables @Cacheable / @CacheEvict / @CachePut annotations
public class TodoappApplication {

    public static void main(String[] args) {
        // ▶ BOOT THE APP:
        // • Scan beans starting from this package
        // • Auto-configure based on classpath dependencies
        // • Start embedded Tomcat
        // • Wire dependency injection graph
        SpringApplication.run(TodoappApplication.class, args);
    }

}
