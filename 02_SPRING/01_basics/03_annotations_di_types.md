# Spring Annotations + DI Types

## Kahani

2005 me har bean XML me likhna padta tha:

```xml
<bean id="userService" class="UserService">
    <property name="userRepo" ref="userRepo"/>
</bean>
```

Bade app me hazaaron lines ka XML ho jaata tha. Spring 2.5 me annotations aaye. Ab class ke upar `@Service` likh do, aur Spring use apne aap dhoondh ke bean bana leta hai.

## Tasveer

```
            @Component  (sabka baap)
     ┌──────────┼───────────┬──────────────┐
  @Service  @Repository  @Controller  @RestController
  (sirf     (DB exception (MVC request  (@Controller +
   label)    badalta)      handler)      @ResponseBody, JSON)
```

## 4 khaas baat

1. **`@Component` teri apni class pe lagta hai, `@Bean` doosre ki class ke liye.** `RestTemplate` jaisi library class ka code tu badal nahi sakta, isliye `@Configuration` class me ek `@Bean` method likh ke `return new RestTemplate();` karte hain.
2. **Injection ke 3 tareeke hain, aur constructor injection sabse sahi hai.** Field `final` rehta hai, test me `new UserService(mockRepo)` seedha likh sakte ho, aur zaroori dependency chhoot nahi sakti. Field `@Autowired` isliye kharab hai ki usme na `final` milta hai, na aasaan test, aur dependencies chhupi rehti hain. Wajah reflection nahi hai, constructor injection bhi reflection use karta hai.
3. **Ek interface ke do bean hon to Spring ulajh jaata hai.** `@Primary` lagao to wo default ban jaata hai, `@Qualifier("naam")` lagao to tu khud chun leta hai.
4. **Circular dependency:** A ko B chahiye aur B ko A, darwaze pe do dost "pehle tu, pehle tu" karte khade hain. Constructor injection me ye fail hota hai (`BeanCurrentlyInCreationException`), aur Boot 2.6 se field injection me bhi by default band hai. Ilaaj:
   - **Sabse sahi:** teesri class nikaal ke cycle hatao, kyunki ye aksar design ki galti hoti hai.
   - **Ya:** ek param pe `@Lazy` lagao.

## Prod wala trap

```java
@Service class SmsSender implements Sender { }
@Service class EmailSender implements Sender { }   // naya joda

public OrderService(Sender sender) { ... }          // app start hi nahi hua
```

Kal tak ek hi `Sender` tha, aaj teammate ne doosra jod diya. Ab startup pe `NoUniqueBeanDefinitionException` aata hai aur poori app nahi uthti. Ilaaj `@Primary` ya `@Qualifier` hai.

## Tera project

usercrud ke main code me field `@Autowired` ek bhi nahi hai, har jagah constructor injection hai. `AuthController` ki 5 dependencies sab `private final` hain: `UserRepository`, `RefreshTokenRepository`, `JwtService`, `AuthenticationManager` aur `PasswordEncoder`. Ek hi constructor hai, isliye `@Autowired` likhna bhi nahi pada.

## Interview line

```
@Component marks my class as a bean; @Service, @Repository and @Controller are specializations,
where @Repository adds exception translation. @Bean is for third-party classes.
I use constructor injection: final fields, easy testing, required dependencies.
Two beans of one type need @Primary or @Qualifier; circular dependencies are best fixed by refactoring.
```
