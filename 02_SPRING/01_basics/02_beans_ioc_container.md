# Spring Beans + IoC Container

## Kahani

Ek office socho jisme log hain, computer hain, printer hain. Wahan har banda apna printer khud nahi kharidta. Ek manager hai jiske paas sab cheezon ki list hai: jise jo chahiye, wo manager se maangta hai aur manager nikaal ke de deta hai.

Spring ka **container (ApplicationContext) wahi manager hai**. Jo cheezein uske paas rakhi hain, unhe **bean** kehte hain. Matlab bean wo objects hain jinhe Spring ne banaya hai aur jinhe Spring hi sambhalta hai.

## Tasveer

Manager ka register aise banta hai:

```
App start:  scan (@Service, @Repository...) -> banao -> jodo -> register me rakho
Register:   "userService" -> UserService object
            "userRepo"    -> UserRepository object
Koi maange: register se nikaal ke de do
```

Matlab app start hote hi Spring saari `@Service`, `@Repository` jaisi classes dhoondhta hai, unke object banata hai, unhe aapas me jodta hai aur register me rakh deta hai. Phir jo bhi maange, use register se nikaal ke de deta hai.

Naya bean ek belt pe se guzarta hai, bilkul naye employee ki onboarding jaisa:

```
1 object bana -> 2 deps bhari (@Autowired) -> 3 [BPP pehle] -> 4 @PostConstruct
-> 5 [BPP baad: YAHAN proxy lapetta] -> 6 kaam pe -> 7 shutdown pe @PreDestroy
```

Pehle object banta hai, phir uski dependencies bhari jaati hain. Uske baad BPP ka pehla checkpoint aata hai, phir `@PostConstruct` chalta hai, phir BPP ka doosra checkpoint aata hai jahan proxy lapeta jaata hai. Iske baad bean kaam pe lag jaata hai, aur app band hote waqt `@PreDestroy` chalta hai.

## 4 khaas baat

1. **Bean koi khaas class nahi hai.** Ye seedha-saadha Java object hai, farak bas itna hai ki use `new` ki jagah Spring ne banaya hai. `@Service`, `@Repository` aur `@Controller` sab andar se `@Component` hi hain, sirf naam alag hai. Ek farak ye hai ki `@Repository` DB ke exception ko Spring ke exception me badal deta hai.
2. **Default scope singleton hai, matlab poori app me us bean ki ek hi copy hoti hai.** Wo copy app start pe banti hai aur app shutdown pe marti hai. Request khatam hone pe bean nahi marta, agli request wahi copy dobara use karti hai. Baaki scope ye hain: prototype me har baar maangne pe naya bean milta hai, request scope me har HTTP request ka apna bean hota hai, aur session scope me har user ka apna bean hota hai.
3. **Kram pakka hai: pehle dependencies bharti hain, phir `@PostConstruct` chalta hai.** Isliye `@PostConstruct` ke andar tu dependencies ko bina dar ke use kar sakta hai, kyunki wo tab tak aa chuki hoti hain.
4. **BeanPostProcessor (BPP) belt ka ek checkpoint hai jo har bean pe chalta hai.** Init ke baad wala checkpoint bean ko proxy me lapet deta hai, aur isi wajah se `@Transactional` aur `@Async` chalte hain. BPP jo object lauta deta hai, container me wahi rakha jaata hai.

## Prod wala trap

```java
@Service
class OrderService {
    private int count = 0;           // field me state
    void place() { count++; }        // 100 request ek saath
}
```

Singleton ka matlab hai ki saari requests ek hi object use karti hain. Isliye `count` field sab threads me share hoti hai, aur jab 100 request ek saath `count++` karti hain to kuch update kho jaate hain. Ye wahi `count++` race hai jo multithreading me dekhi thi.

Ilaaj:
- **Agar data sirf ek request ka hai,** to use field me mat rakho, local variable ya parameter me rakho.
- **Agar sach me saajhi ginti chahiye,** to `AtomicInteger` lagao.

Field me sirf wahi cheez rakho jo kabhi nahi badalti, jaise repo, client ya config.

## Tera project

- **usercrud me har tarah ka bean hai.** `UserController` ek `@RestController` hai, `UserService` ek `@Service` hai, `BookRepository` ek `@Repository` hai (ye interface hai aur iski implementation Spring ne khud banayi), aur `KafkaConfig` ek `@Configuration` hai.
- **Tune apna BPP bhi likha hai.** `demo/SpringInternalsDemo.java` me `LoggingBeanPostProcessor` hai. Prototype bean 2 baar maanga to 4 print aaye, kyunki har baar before aur after dono chale. Singleton pe kuch print nahi hua, kyunki wo startup pe hi belt se guzar chuka tha.

## Interview line

```
A bean is an object created and managed by the Spring container. At startup Spring scans,
creates and wires them; the default scope is singleton, so beans must be stateless.
Lifecycle: create, inject dependencies, @PostConstruct, then BeanPostProcessor after-init,
which is where AOP proxies for @Transactional and @Async are applied; @PreDestroy on shutdown.
```
