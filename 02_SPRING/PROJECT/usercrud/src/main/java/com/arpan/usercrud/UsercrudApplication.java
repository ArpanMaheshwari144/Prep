package com.arpan.usercrud;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    Spring Boot ka ENTRY POINT (JVM yahaan se shuru)
//    main() → Spring container boot → app ready on :8080
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL BOOT FLOW:
//    java -jar app.jar
//         │
//         ▼
//    main() method called
//         │
//         ▼
//    SpringApplication.run()
//         │
//         ├── 1. ApplicationContext (IoC container) banaye
//         ├── 2. @ComponentScan — @Service/@Repository/@Controller dhundhe
//         ├── 3. @EnableAutoConfiguration — classpath dekh smart wiring
//         ├── 4. @Autowired wire beans together
//         ├── 5. Embedded Tomcat start (port 8080)
//         └── App READY 
//
// @SpringBootApplication — THE MAGIC ANNOTATION:
//    3 annotations ka COMBO:
//
//       @Configuration            → "Yeh class beans define karegi"
//                                    Legacy XML config ka replacement
//
//       @EnableAutoConfiguration  → "Classpath dekh smart decisions le"
//                                    spring-boot-starter-web? → Tomcat
//                                    spring-boot-starter-jpa? → DataSource
//                                    spring-security?         → Filter chain
//                                    = Convention over Configuration
//
//       @ComponentScan            → "Sub-packages mein scan kar"
//                                    @Component, @Service, @Repository,
//                                    @Controller, @RestController dhundhe
//                                    Default scan root = is class ka package
//
// SpringApplication.run() internally:
//    1. ApplicationContext create
//    2. Banner print
//    3. Environment setup (properties, profiles)
//    4. Component scan triggered
//    5. Beans created (in correct order)
//    6. Dependencies wired (@Autowired)
//    7. Lifecycle callbacks (@PostConstruct)
//    8. Embedded Tomcat start
//    9. App running
//
// WHY SO SMALL?
//    1 annotation = 3 features
//    1 method call = entire app boot
//    No XML, no web.xml, no manual DispatcherServlet
//    = Convention over Configuration philosophy
//
// INTERVIEW LINE:
//    "@SpringBootApplication = @Configuration + @EnableAutoConfiguration
//     + @ComponentScan. Combined annotation that bootstraps the entire
//     Spring context, auto-wires beans, configures based on classpath,
//     and starts embedded Tomcat."
// ═══════════════════════════════════════════════════════════════════════

@SpringBootApplication
public class UsercrudApplication {

	public static void main(String[] args) {
		SpringApplication.run(UsercrudApplication.class, args);
	}
}
