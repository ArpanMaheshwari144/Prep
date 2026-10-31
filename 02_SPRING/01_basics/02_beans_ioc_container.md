# Spring Beans + IoC Container

## Kahani

Office me log, computer aur printer hain. Har banda apna printer khud nahi kharidta. Ek manager hai jiske paas sab ki list hai: jise jo chahiye, manager se maango aur wo nikaal ke de deta hai.

Spring ka **container (ApplicationContext) wahi manager hai**. Jo cheezein uske paas rakhi hain, unhe **bean** kehte hain, yaani wo objects jinhe Spring ne banaya aur jinhe wahi sambhalta hai.

## Tasveer

Manager ka register:

```
App start:  scan (@Service, @Repository...) -> banao -> jodo -> register me rakho
Register:   "userService" -> UserService object
            "userRepo"    -> UserRepository object
Koi maange: register se nikaal ke de do
```

Naye bean ki belt (employee onboarding jaisi):

```
1 object bana -> 2 deps bhari (@Autowired) -> 3 [BPP pehle] -> 4 @PostConstruct
-> 5 [BPP baad: YAHAN proxy lapetta] -> 6 kaam pe -> 7 shutdown pe @PreDestroy
```

## 4 khaas baat

1. **Bean koi khaas class nahi hai.** Ye seedha-saadha Java object hai, farak bas itna hai ki use `new` ki jagah Spring ne banaya. `@Service`, `@Repository`, `@Controller` sab andar se `@Component` hi hain, bas naam alag hai (`@Repository` DB ke exception bhi badal deta hai).
2. **Default singleton hai: poori app me ek hi copy.** Wo app start pe banti hai aur shutdown pe marti hai. Request khatam hone pe bean nahi marta, agli request wahi copy dobara use karti hai. Baaki scope: prototype me har baar naya bean, request scope me har HTTP request ka apna, session scope me har user ka apna.
3. **Kram pakka hai: pehle deps bharti hain, phir `@PostConstruct` chalta hai.** Isliye `@PostConstruct` ke andar deps ko bina dar use kar sakte ho.
4. **BeanPostProcessor (BPP) belt ka checkpoint hai jo har bean pe chalta hai.** Init ke baad wala checkpoint bean ko proxy me lapet deta hai, aur `@Transactional` / `@Async` isi wajah se chalte hain. BPP jo lauta de, container me wahi rakha jaata hai.

## Prod wala trap

```java
@Service
class OrderService {
    private int count = 0;           // field me state
    void place() { count++; }        // 100 request ek saath
}
```

Singleton ka matlab hai saari requests ek hi object use karti hain. Isliye `count` sab threads me share hota hai, aur update kho jaate hain. Ye wahi `count++` race hai jo multithreading me dekhi thi.

Ilaaj:
- **Ek request ka data:** field me nahi, local variable ya parameter me rakho.
- **Sach me saajhi ginti:** `AtomicInteger` lagao.

Field me sirf wahi rakho jo kabhi nahi badalta, jaise repo, client ya config.

## Tera project

- **usercrud me har tarah ka bean:** `UserController` (`@RestController`), `UserService` (`@Service`), `BookRepository` (`@Repository`, interface hai, implementation Spring ne khud banaya), `KafkaConfig` (`@Configuration`).
- **Tera apna BPP:** `demo/SpringInternalsDemo.java` me `LoggingBeanPostProcessor` likha hai. Prototype bean 2 baar maanga to 4 print aaye (har baar before + after). Singleton pe kuch print nahi hua, kyunki wo startup pe hi belt se guzar chuka tha.

## Interview line

```
A bean is an object created and managed by the Spring container. At startup Spring scans,
creates and wires them; the default scope is singleton, so beans must be stateless.
Lifecycle: create, inject dependencies, @PostConstruct, then BeanPostProcessor after-init,
which is where AOP proxies for @Transactional and @Async are applied; @PreDestroy on shutdown.
```
