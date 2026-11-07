# Spring Beans + IoC Container

## Scene

Office me har banda apna printer khud nahi kharidta. Manager ke paas ek register hai, jise jo chahiye wo manager se maangta hai. Spring me ye manager container (ApplicationContext) hai, aur register me rakhi har cheez ek **bean** hai.

```
App start:  @Service, @Repository dhoondo -> banao -> jodo -> register me rakho
Register:   "userService" -> UserService object
            "userRepo"    -> UserRepository object
Koi maange: register se nikaal ke de do
```

## Dikkat

Default me bean **singleton** hota hai, matlab poori app me uski ek hi copy hoti hai aur har request wahi copy use karti hai. To agar field me ginti rakhi, aur 100 thread ne 1 lakh order ek saath maare:

```java
@Service
class OrderService {
    int count = 0;
    void place() { count++; }
}
```

```
int count       = 99652   (chahiye 100000)
AtomicInteger   = 100000
```

348 order gayab ho gaye. Ye wahi multithreading wala `count++` race hai: saare threads ek hi object ki ek hi field pe ladte hain, aur kuch update kho jaate hain.

## Hal

- Data sirf ek request ka hai, to field me mat rakho, local variable ya parameter me rakho.
- Sach me saajhi ginti chahiye, to `AtomicInteger` lagao.
- Field me sirf wo rakho jo kabhi nahi badalta: repo, client, config.

## Bean ki zindagi (belt)

```
1 object bana -> 2 deps bhari -> 3 [BPP pehle] -> 4 @PostConstruct
-> 5 [BPP baad: YAHAN proxy lapetta] -> 6 kaam pe -> 7 shutdown pe @PreDestroy
```

Deps pehle bharti hain aur `@PostConstruct` baad me chalta hai, isliye `@PostConstruct` me dependencies bina dar ke use kar sakte ho. Step 5 wala BPP (BeanPostProcessor) bean ko proxy me lapet-ta hai, aur isi se `@Transactional` aur `@Async` chalte hain.

Baaki scope: prototype me har baar maangne pe naya bean, request scope me har HTTP request ka apna, session scope me har user ka apna.

Tera usercrud: `demo/SpringInternalsDemo.java` me tera apna `LoggingBeanPostProcessor` hai. Prototype bean 2 baar maanga to 4 print aaye (har baar before + after). Singleton pe kuch print nahi hua, kyunki wo startup pe hi belt se guzar chuka tha.

## Trap

`@Service`, `@Repository`, `@Controller` sab andar se `@Component` hi hain. Ek farak hai: `@Repository` DB ka exception Spring ke exception me badal deta hai. DB wali class pe `@Service` lagaya to ye translation nahi milega.
