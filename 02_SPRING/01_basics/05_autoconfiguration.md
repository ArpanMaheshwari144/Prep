# Spring Boot Auto-Configuration

## Scene

Tere usercrud me tune Tomcat chalane ya DB connection banane ki ek bhi line nahi likhi, phir bhi app 8080 pe chal jaati hai aur DB se judi rehti hai. Ye kaun karta hai? Tera pom dekh:

```
almari me (pom)            ->  Spring ne khud banaya
starter-data-jpa           ->  DataSource + HikariCP pool + Hibernate
starter-webmvc             ->  Tomcat (8080) + DispatcherServlet + JSON
starter-kafka              ->  KafkaTemplate + listener
starter-actuator           ->  /actuator/health
starter-security           ->  filter chain + default login
```

## Kaise

Spring Boot ek naukar jaisa hai. App start hote hi wo almari (classpath) kholta hai: bat dikha to cricket ka setup kar deta hai, bat nahi dikha to kuch nahi.

- **Starter:** bas sahi jars ka bundle.
- **`@SpringBootApplication`:** andar se `@Configuration` + `@ComponentScan` + `@EnableAutoConfiguration`. Teesra wala har jar ki list padhta hai (Boot 2.7 / 3 me `AutoConfiguration.imports`, purane me `spring.factories`), jisme `DataSourceAutoConfiguration` jaisi classes likhi hoti hain.
- **Har auto-config ek shart (`@Conditional`) ke peeche hoti hai:**
  - `@ConditionalOnClass`: wo library classpath pe ho tabhi chalo.
  - `@ConditionalOnMissingBean`: tune apna bean nahi banaya, tabhi Spring apna banaye.
  - `@ConditionalOnProperty`: wo property set ho tabhi chalo.

## Dikkat (prod trap)

Tune apna DataSource khud bana liya:

```java
@Bean
DataSource dataSource() {
    return new HikariDataSource(...);
}
```

`@ConditionalOnMissingBean` ki wajah se Spring peeche hat jaata hai, ye theek hai. Par saath me `application.properties` ki `spring.datasource.hikari.maximum-pool-size=50` bhi chup-chaap lagni band ho jaati hai, aur pool default 10 pe chalta rehta hai. Load aaya to connection khatam aur requests atak jaati hain. Ye wahi HikariCP pool wali kahani hai jo tune prod me pakdi thi.

## Hal

- Apna bean banaya hai to pool size apne code me hi set karo, ya bean mat banao aur properties se chalne do.
- Kaunsi auto-config chali aur kaunsi nahi, ye dekhna ho to app `--debug` ke saath chalao. Uski "condition evaluation report" saaf bata deti hai.

Tera usercrud: 8 starter (data-jpa, webmvc, validation, graphql, cache, kafka, actuator, security), setup code zero. Ek cheez tune khud di hai: BCrypt, jo tere `SecurityConfig` ka apna `@Bean` hai.
