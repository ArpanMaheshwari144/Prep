package com.example.practice;

/*
 * ============================================================
 * YE KYA HAI: Main class — Spring Boot app yahan se start hoti. Ye ENTRY POINT hai.
 * KYUN CHAHIYE: Bina iske app chalegi hi nahi. Java mein main() method se sab shuru hota.
 * KAB USE: App start karte waqt — java -jar ya IDE se run karo.
 *
 * REAL WORLD ANALOGY:
 *   Ye teri DUKAAN ka MAIN SWITCH hai.
 *   Switch ON kiya → bijli aayi → sab machines (beans) start hui → dukaan OPEN.
 *   Switch OFF → sab band. Bina switch ke dukaan chalegi hi nahi.
 *
 * ANDAR KYA HOTA (SpringApplication.run ke andar):
 *   1. Embedded TOMCAT start hota (port 8080) — alag Tomcat install nahi karna padta
 *      ANALOGY: Dukaan ke andar hi generator hai — bahar se bijli line nahi lagani.
 *   2. Component Scan — is package + sub-packages mein @Component, @Service, @Repository, @Controller dhundhta
 *      ANALOGY: Manager poore building mein jaake sab employees ko dhundh ke laata (beans).
 *   3. Auto Configuration — classpath dekhta:
 *      - mysql-connector mila? → DataSource auto configure (JDBC connection pool)
 *      - spring-data-jpa mila? → Hibernate + EntityManager auto configure
 *      - spring-security mila? → Security filters auto configure
 *      ANALOGY: Smart electrician — dekhta kaunse appliances hain, uske hisaab se wiring kar deta.
 *   4. Sab beans create → dependencies inject (@Autowired) → Application READY
 *
 * BEAN KYA HAI:
 *   Spring managed object. Tu "new Service()" NAHI karta — Spring karta.
 *   @Service, @Repository, @Controller lagaya → Spring bean bana deta.
 *   ApplicationContext (Spring Container) mein store hota.
 *   @Autowired bolo → Spring container se nikal ke de deta.
 *   Ye = IoC (Inversion of Control) — object creation tera nahi, Spring ka kaam.
 *   ANALOGY: Hotel mein khana tu nahi banata — order do (annotation lagao), kitchen (Spring) banake de deta.
 * ============================================================
 */

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cache.annotation.EnableCaching;

/*
 * ============================================================
 * @SpringBootApplication
 *
 * YE KYA HAI: 3-in-1 COMBO annotation — teen annotations ka shortcut.
 *   @Configuration          — ye class beans define kar sakti (config source hai)
 *   @EnableAutoConfiguration — Spring classpath dekhke auto configure karega
 *                              (DataSource, Hibernate, Security — sab auto setup)
 *   @ComponentScan          — is package + sub-packages mein beans dhundhega
 *                              (@Service, @Repository, @Controller sab scan karega)
 *
 * KYUN LAGAYA: Iske bina Spring ko pata hi nahi ki:
 *   - Beans kahan hain (scan nahi karega)
 *   - Kya configure karna hai (auto-config nahi hoga)
 *   - Config source kya hai (beans define nahi honge)
 *
 * NAHI LAGAYA TO KYA HOGA:
 *   - Component scan nahi hoga → @Service, @Repository detect nahi → beans nahi banenge
 *   - Auto configuration nahi → DataSource, Hibernate, Security manually configure karo
 *   - Basically app EK BADA DISASTER hoga — kuch kaam nahi karega
 *
 * REAL WORLD ANALOGY:
 *   @SpringBootApplication = restaurant ka OPENING KIT (3-in-1).
 *     @Configuration = menu card (kya kya serve karna hai — beans define)
 *     @EnableAutoConfiguration = smart chef (fridge mein kya hai dekh ke dish banata — auto config)
 *     @ComponentScan = manager (sab staff ko dhundh ke laata — beans detect)
 *   Kit nahi hai toh: menu nahi, chef confused, staff nahi mila → restaurant band.
 * ============================================================
 */
@SpringBootApplication

/*
 * ============================================================
 * @EnableCaching
 *
 * YE KYA HAI: Spring ka Caching feature ON karo. Ye ek SWITCH hai caching ke liye.
 *
 * KYUN LAGAYA: Bina iske @Cacheable, @CachePut, @CacheEvict — sab DEAD annotations hain.
 *   Lagaya toh Spring proxy banata → cache logic wrap karta → annotations kaam karte.
 *
 * NAHI LAGAYA TO KYA HOGA:
 *   - @Cacheable lagaya Service mein → KUCH NAHI HOGA. Dead annotation.
 *   - Har request seedha DB pe jaayegi → SLOW performance
 *   - Redis configure kiya, DTO Serializable banaya — sab WASTE. Kyunki switch hi OFF hai.
 *   - Jaise AC laga diya but MCB ON nahi kiya → AC dead padha rahega.
 *
 * REAL WORLD ANALOGY:
 *   @EnableCaching = AC ka MCB switch ON karna.
 *   AC (Redis) laga diya, remote (annotations) bhi hai.
 *   But MCB OFF toh AC chalega? NAHI. Pehle MCB ON karo (@EnableCaching).
 *   @Cacheable = remote se temperature set karna — MCB ON hai tabhi kaam karega.
 *
 * INTERNALLY KYA HOTA (PROXY):
 *   Spring tera original object wrap karta ek proxy mein.
 *   getStudentById(1) call → proxy pehle cache check → miss? → original method call → cache mein rakh.
 *   Ye = AOP (Aspect Oriented Programming) — method ke aas paas extra logic.
 *   ANALOGY: Tere aur DB ke beech ek MIDDLEMAN (proxy) baith gaya.
 *   Pehle middleman se poocho → uske paas hai toh de dega → nahi toh DB se laayega.
 * ============================================================
 */
@EnableCaching
public class PracticeApplication {

	public static void main(String[] args) {
		/*
		 * ============================================================
		 * SpringApplication.run()
		 *
		 * YE KYA HAI: Spring Boot application START karne ka single method.
		 *
		 * KYUN LAGAYA: Ye ek line mein SACH MEIN bahut kuch karta:
		 *   1. Embedded Tomcat server start (port 8080)
		 *   2. Package scan karke sab beans detect
		 *   3. Classpath se auto-configuration
		 *   4. Beans create + dependency inject (@Autowired)
		 *   5. Application READY!
		 *
		 * NAHI LAGAYA TO KYA HOGA:
		 *   App start hi nahi hogi. main() method khaali rahegi. Kuch nahi chalega.
		 *
		 * REAL WORLD ANALOGY:
		 *   Ye teri car ki IGNITION KEY hai.
		 *   Key ghumayi → engine start (Tomcat) → fuel pump on (bean scan) →
		 *   AC on (auto config) → dashboard lights on (beans ready) → DRIVE!
		 *   Bina key ghumaaye → car mein baithke dekhte raho.
		 *
		 * Console pe dikhega: "Tomcat started on port(s): 8080"
		 * ============================================================
		 */
		SpringApplication.run(PracticeApplication.class, args);
	}

}
