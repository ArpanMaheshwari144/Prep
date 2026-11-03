# Spring Boot Auto-Configuration

## Kahani

Ek naya naukar hai, Spring Boot. App start hote hi wo teri almari kholta hai, yaani tera pom aur classpath. Almari me bat dikha to wo samajh jaata hai ki sahab cricket khelte hain, aur ground aur kit khud taiyaar kar deta hai. Bat nahi dikha to wo cricket ka koi setup nahi karta.

## Tasveer

Tere usercrud ka pom:

```
almari me (pom)            ->  Spring ne khud banaya
starter-data-jpa           ->  DataSource + HikariCP pool + Hibernate
starter-webmvc             ->  Tomcat (8080) + DispatcherServlet + JSON
starter-kafka              ->  KafkaTemplate + listener
starter-actuator           ->  /actuator/health
starter-security           ->  filter chain + default login
```

Pom me jo starter rakha, uske hisaab se Spring ne cheezein khud bana di. Tune Tomcat chalane ya DB connection banane ki ek bhi line code me nahi likhi.

## 4 khaas baat

1. **Starter bas dependencies ka ek bundle hai.** Wo sahi jars ek saath le aata hai. Asli kaam auto-configuration karta hai.
2. **`@SpringBootApplication` andar se teen annotation hai: `@Configuration`, `@ComponentScan` aur `@EnableAutoConfiguration`.** Teesra wala har jar me rakhi ek list padhta hai. Boot 2.7 aur 3 me ye list `AutoConfiguration.imports` file me hoti hai, aur purane Boot me `spring.factories` me. Us list me `DataSourceAutoConfiguration` jaisi classes likhi hoti hain.
3. **Har auto-config class ek shart (`@Conditional`) ke peeche chhupi hoti hai:**
   - `@ConditionalOnClass` ka matlab hai ki wo library classpath pe ho, tabhi ye config chalo.
   - `@ConditionalOnMissingBean` ka matlab hai ki tune apna bean nahi banaya, tabhi Spring apna banaye.
   - `@ConditionalOnProperty` ka matlab hai ki wo property set ho, tabhi ye config chalo.
4. **Tune apna kuch diya to Spring peeche hat jaata hai.** Kuch nahi diya to Spring ka default chalta hai, jaise H2. Tune `spring.datasource.url` di ya apna bean banaya, to Spring tera wala use karta hai.

## Prod wala trap

```java
@Bean
DataSource dataSource() {               // apna bana liya
    return new HikariDataSource(...);
}
```

Tune apna DataSource bean bana liya, to `@ConditionalOnMissingBean` ki wajah se Spring apna bean nahi banata. Iske saath ek chupi cheez hoti hai: `application.properties` me likhi `spring.datasource.hikari.maximum-pool-size` jaisi settings bhi chup-chaap lagna band ho jaati hain, aur pool default size pe chalta rehta hai. Ye wahi HikariCP pool exhaustion wali kahani hai jo tune prod me pakdi thi. Kaunsi auto-config chali aur kaunsi nahi, ye dekhna ho to app `--debug` ke saath chalao, uski "condition evaluation report" saaf bata deti hai.

## Tera project

usercrud ke pom me 8 starter hain: data-jpa, webmvc, validation, graphql, cache, kafka, actuator aur security. Inke liye setup ka code tune kahin nahi likha. Haan, BCrypt Spring ne nahi diya, wo tere `SecurityConfig` ka apna `@Bean` hai.

## Interview line

```
Spring Boot auto-configuration looks at the classpath and creates beans conditionally:
@EnableAutoConfiguration loads auto-config classes listed in each jar, each guarded by
@ConditionalOnClass and @ConditionalOnMissingBean, so if I define my own bean, Boot backs off.
A starter is just a curated bundle of dependencies.
```
