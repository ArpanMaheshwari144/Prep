# Spring Annotations + DI Types

## Kahani

2005 me har bean XML me likhna padta tha:

```xml
<bean id="userService" class="UserService">
    <property name="userRepo" ref="userRepo"/>
</bean>
```

Bade app me ye XML hazaaron line ka ho jaata tha. Phir Spring 2.5 me annotations aaye. Ab tu class ke upar `@Service` likh de, aur Spring use apne aap dhoondh ke uska bean bana leta hai.

## Tasveer

```
            @Component  (sabka baap)
     ┌──────────┼───────────┬──────────────┐
  @Service  @Repository  @Controller  @RestController
  (sirf     (DB exception (MVC request  (@Controller +
   label)    badalta)      handler)      @ResponseBody, JSON)
```

`@Component` sabka baap hai. `@Service` sirf ek label hai, `@Repository` DB ke exception badal deta hai, `@Controller` MVC ki request sambhalta hai, aur `@RestController` matlab `@Controller` ke saath `@ResponseBody`, yaani seedha JSON lautata hai.

## 4 khaas baat

1. **`@Component` teri apni class pe lagta hai, aur `@Bean` doosre ki class ke liye hota hai.** `RestTemplate` jaisi library class ka code tu badal nahi sakta, us pe annotation nahi laga sakta. Isliye `@Configuration` class me ek `@Bean` method likhte hain jo `return new RestTemplate();` karta hai.
2. **Injection ke 3 tareeke hain, aur constructor injection sabse sahi hai.** Isme field `final` reh sakta hai, test me seedha `new UserService(mockRepo)` likh sakte ho, aur koi zaroori dependency chhoot nahi sakti. Field pe `@Autowired` isliye kharab hai ki usme na `final` milta hai, na test aasaan hota hai, aur dependencies chhupi rehti hain. Dhyaan rahe, wajah reflection nahi hai, kyunki constructor injection bhi reflection hi use karta hai.
3. **Ek interface ke do bean hon to Spring ulajh jaata hai ki kaunsa de.** `@Primary` lagao to wo bean default ban jaata hai, aur `@Qualifier("naam")` lagao to tu khud chun leta hai ki kaunsa chahiye.
4. **Circular dependency:** A ko B chahiye aur B ko A chahiye, jaise darwaze pe do dost "pehle tu, pehle tu" karte khade hain aur koi andar nahi jaata. Constructor injection me ye fail hota hai (`BeanCurrentlyInCreationException`), aur Boot 2.6 se field injection me bhi ye by default band hai. Ilaaj:
   - **Sabse sahi ilaaj:** ek teesri class nikaal ke cycle hata do, kyunki circular dependency aksar design ki galti hoti hai.
   - **Ya phir:** ek parameter pe `@Lazy` laga do.

## Prod wala trap

```java
@Service class SmsSender implements Sender { }
@Service class EmailSender implements Sender { }   // naya joda

public OrderService(Sender sender) { ... }          // app start hi nahi hua
```

Kal tak ek hi `Sender` tha, aaj teammate ne doosra jod diya. Ab Spring ko samajh nahi aata ki `OrderService` ko kaunsa `Sender` de, isliye startup pe `NoUniqueBeanDefinitionException` aata hai aur poori app hi nahi uthti. Ilaaj `@Primary` ya `@Qualifier` hai.

## Tera project

usercrud ke main code me field pe `@Autowired` ek bhi jagah nahi hai, har jagah constructor injection hai. `AuthController` ki 5 dependencies sab `private final` hain: `UserRepository`, `RefreshTokenRepository`, `JwtService`, `AuthenticationManager` aur `PasswordEncoder`. Class me ek hi constructor hai, isliye `@Autowired` likhna bhi nahi pada.
