# Spring Boot Auto-Configuration (starter ka "magic" — jo magic nahi)

> **Spring 101: Topic 5** — "dependency daali, sab khud chal gaya" ka andar. [[04_proxy_jdk_cglib]] (koi jaadu nahi theme)

---

## Core: naukar jo tera SAAMAN dekh ke khud setup kar deta

Ek naya naukar (Spring Boot) app-start pe teri **almari (classpath/pom) kholta** — "is ghar me kya-kya saaman (libraries) pada?" — aur uske hisaab se zaroori cheezein KHUD set kar deta.

```
Almari me BAT dikha  -> "sir cricket khelte honge" -> khud ground+kit taiyaar
Almari me BAT nahi   -> cricket ka koi setup NAHI (zaroorat hi nahi)
```

Asli me:
```
pom me MySQL library daali  -> Spring "database chahiye" samajh ke connection-setup khud
MySQL library nahi          -> wo setup nahi karega
```

---

## ★ TERE usercrud pom.xml se — turant click (real project)

Har `<dependency>` = almari me ek saaman. Spring ne start pe har ek dekh ke KHUD setup kiya:

```
ALMARI ME (pom)                    ->  SPRING NE KHUD KYA BANAYA
─────────────────────────────────────────────────────────────────────────
starter-data-jpa                   ->  DataSource + HikariCP pool + EntityManager + Hibernate
                                       (tu bas @Entity/@Repository likhta)
starter-webmvc                     ->  embedded TOMCAT (port 8080 khud chalu) + DispatcherServlet + JSON
                                       (tu bas @RestController)
h2 + mysql-connector-j             ->  DB driver dekh ke DataSource us DB ka (properties me url daalo -> wahi)
starter-validation                 ->  @Valid/@NotNull/@Email chalne lage
starter-graphql                    ->  GraphQL endpoint + schema wiring
starter-cache                      ->  @Cacheable chalne laga
starter-kafka                      ->  KafkaTemplate + listener-container
starter-actuator + prometheus      ->  /actuator/health, /actuator/prometheus endpoints khud
starter-security                   ->  filter-chain + BCrypt + login setup
```
**PUNCH:** tune kisi ka manual setup-code NAHI likha — na Tomcat start, na DB-connection, na Kafka wiring. Bas starter pom me daala, Spring ne almari dekh ke sab khud kiya. **Yahi auto-configuration.**

---

## "Khud peeche hat jaata" (tune apna diya to)

```
Tune kuch nahi diya   -> Spring apna DEFAULT setup (jaise H2 in-memory)
Tune value/bean diya  -> Spring TERA use karta, apna peeche hata leta
```
(usercrud pom comment: `application.properties` me `spring.datasource.url=...` diya -> Spring wahi value use karta, default chhod deta.)

---

## Andar ka mechanism (interview naam — halka)

```
1. @SpringBootApplication = @Configuration + @ComponentScan + @EnableAutoConfiguration
2. @EnableAutoConfiguration -> har jar me ek LIST-file dhoondhta:
      purana (Boot 2.6-): META-INF/spring.factories
      naya (Boot 2.7+/3): META-INF/spring/...AutoConfiguration.imports
   -> usme "auto-config classes" (jaise DataSourceAutoConfiguration)
3. Har class @Conditional se GUARDED:
      @ConditionalOnClass(DataSource.class) -> sirf jab wo class classpath pe (dependency daali)
      @ConditionalOnMissingBean             -> sirf jab TU ne apna bean na banaya ho
      @ConditionalOnProperty("...")         -> sirf jab wo property set ho
4. starter-data-jpa daali -> DataSource class classpath pe -> @ConditionalOnClass match
   -> DataSourceAutoConfiguration fire -> DataSource bean bana
   -> par tune apna banaya to @ConditionalOnMissingBean -> Spring peeche hat gaya
```
STARTER = bas dependencies ka BUNDLE (sahi jars ek saath kheech leta). "Convention over configuration."

---

## POWER PHRASE

> *"Spring Boot auto-configuration reads what's on the classpath and conditionally creates the beans you'd otherwise configure by hand. @EnableAutoConfiguration loads auto-config classes listed in each jar (spring.factories, or AutoConfiguration.imports in Boot 2.7+/3.x), each guarded by @Conditional — @ConditionalOnClass (dependency present) and @ConditionalOnMissingBean (you haven't defined your own). A starter is just a curated bundle of dependencies; adding it makes auto-config wire everything."*

> **Yaad rakh:**
> Auto-config = "classpath pe kya hai" dekh ke @Conditional-guarded beans banana. Koi jaadu nahi.
> @ConditionalOnClass (dependency hai?) + @ConditionalOnMissingBean (tune khud to nahi banaya?)
> List = spring.factories (purana) -> AutoConfiguration.imports (Boot 2.7+/3). Starter = dependency bundle.
