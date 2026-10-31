# Spring Boot Auto-Configuration

## Kahani

Naya naukar (Spring Boot) app start hote hi teri almari (pom / classpath) kholta hai. Almari me bat dikha to samajh jaata hai ki sahab cricket khelte hain, aur ground aur kit khud taiyaar kar deta hai. Bat nahi dikha to cricket ka koi setup nahi karta.

## Tasveer

Tera usercrud pom:

```
almari me (pom)            ->  Spring ne khud banaya
starter-data-jpa           ->  DataSource + HikariCP pool + Hibernate
starter-webmvc             ->  Tomcat (8080) + DispatcherServlet + JSON
starter-kafka              ->  KafkaTemplate + listener
starter-actuator           ->  /actuator/health
starter-security           ->  filter chain + default login
```

Tune Tomcat chalane ya DB connection banane ka ek bhi line code nahi likha.

## 4 khaas baat

1. **Starter bas dependencies ka bundle hai.** Wo sahi jars ek saath le aata hai. Kaam auto-configuration karta hai.
2. **`@SpringBootApplication` = `@Configuration` + `@ComponentScan` + `@EnableAutoConfiguration`.** Teesra wala har jar me rakhi ek list padhta hai: Boot 2.7 / 3 me `AutoConfiguration.imports`, purane Boot me `spring.factories`. Us list me `DataSourceAutoConfiguration` jaisi classes hoti hain.
3. **Har auto-config class ek shart (`@Conditional`) ke peeche hai:**
   - `@ConditionalOnClass`: wo library classpath pe hai, tabhi chalo.
   - `@ConditionalOnMissingBean`: tune apna bean nahi banaya, tabhi apna banao.
   - `@ConditionalOnProperty`: wo property set hai, tabhi chalo.
4. **Tune apna diya to Spring peeche hat jaata hai.** Kuch nahi diya to Spring ka default chalta hai (jaise H2). Tune `spring.datasource.url` di ya apna bean banaya, to Spring tera wala use karta hai.

## Prod wala trap

```java
@Bean
DataSource dataSource() {               // apna bana liya
    return new HikariDataSource(...);
}
```

Tune apna DataSource bean banaya, to `@ConditionalOnMissingBean` ki wajah se Spring ka bean nahi banta. Iske saath `application.properties` ki `spring.datasource.hikari.maximum-pool-size` jaisi settings bhi chup-chaap lagna band ho jaati hain, aur pool default size pe chalta hai. Ye wahi HikariCP pool exhaustion wali kahani hai jo tune prod me pakdi thi. Kaunsi auto-config chali aur kaunsi nahi, ye dekhna ho to app `--debug` se chalao: "condition evaluation report" saaf bata deta hai.

## Tera project

usercrud ke pom me 8 starter hain: data-jpa, webmvc, validation, graphql, cache, kafka, actuator aur security. Inke liye setup code tune kahin nahi likha. BCrypt Spring ne nahi diya, wo tere `SecurityConfig` ka apna `@Bean` hai.

## Interview line

```
Spring Boot auto-configuration looks at the classpath and creates beans conditionally:
@EnableAutoConfiguration loads auto-config classes listed in each jar, each guarded by
@ConditionalOnClass and @ConditionalOnMissingBean, so if I define my own bean, Boot backs off.
A starter is just a curated bundle of dependencies.
```
