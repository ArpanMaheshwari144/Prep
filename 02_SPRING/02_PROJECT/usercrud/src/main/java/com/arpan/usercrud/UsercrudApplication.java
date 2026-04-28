package com.arpan.usercrud;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

/* ════════════════════════════════════════════════════════════════
 *  📌 SPRING BOOT — Application Entry Point
 * ════════════════════════════════════════════════════════════════
 *  Yeh class poore Spring Boot app ka starting point hai.
 *  `main()` chalega → Spring container boot hoga → saare beans
 *  scan honge → embedded Tomcat start hoga → app ready on :8080.
 *
 *  ─── @SpringBootApplication kya hai? (interview gold) ───────────
 *  Yeh ek "meta-annotation" hai — 3 annotations ka combo:
 *
 *    1. @Configuration         → "Yeh class beans define karegi"
 *                                (legacy XML config ka replacement)
 *
 *    2. @EnableAutoConfiguration
 *                              → "Classpath dekho aur smart guesses
 *                                karo" — H2 mila? DataSource bana do.
 *                                spring-boot-starter-web hai? Tomcat
 *                                + DispatcherServlet wire kar do.
 *
 *    3. @ComponentScan         → "Iss package + sub-packages mein
 *                                ghuumo, @Component / @Service /
 *                                @Repository / @Controller dhundo,
 *                                sabko bean banake register kar do"
 *
 *  ─── SpringApplication.run() kya karta hai? ─────────────────────
 *  • ApplicationContext (IoC container) banata hai
 *  • Component scan → beans create + dependencies inject
 *  • Embedded Tomcat start (port 8080)
 *  • App "running" state mein chala jata hai
 * ════════════════════════════════════════════════════════════════
 */
@SpringBootApplication
public class UsercrudApplication {

	public static void main(String[] args) {
		SpringApplication.run(UsercrudApplication.class, args);
	}
}
