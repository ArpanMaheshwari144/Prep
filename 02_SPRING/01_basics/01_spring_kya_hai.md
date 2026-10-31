# Spring: kya hai, kyun aaya

## Kahani

2002 ka Java dev OrderService likh raha hai:

```java
class OrderService {
    OrderRepository repo = new MySQLOrderRepository();
    EmailService email  = new SendGridEmailService();
}
```

Kal MongoDB pe jaana pada, to service ka code hi badalna padega. Test me nakli repo daalna ho, to `new` beech me aa jaata hai. Har class apne auzaar khud bana rahi hai, isliye ek chhota badlaav 50 jagah karna padta hai.

Spring ne kaha: "Tu sirf kaam ka logic likh. Object banana, unhe jodna aur sambhalna mera kaam hai."

## Tasveer

Pizza counter. Pehle tu khud kitchen jaata tha: atta goondho, oven jalao. Ab counter pe bas bolta hai "Margherita chahiye", aur bana-banaya mil jaata hai.

```
pehle:   OrderService --new--> MySQLRepo            (tu banata)
Spring:  Container --banata + daalta--> OrderService (tu sirf maangta)
```

## 4 khaas baat

1. **IoC (Inversion of Control) ka matlab hai control ulta ho gaya.** Object tu nahi banata, Spring ka container (ApplicationContext) app start hote hi banata hai. Andar wo bhi `new` / reflection hi use karta hai, koi jaadu nahi hai.
2. **DI (Dependency Injection) ek pattern hai, Spring ka apna feature nahi.** Haath se bhi kar sakte ho: `new AccountService(repo)`. Spring bas ye kaam apne aap kar deta hai. Constructor injection sabse sahi tareeka hai, aur ek hi constructor ho to `@Autowired` likhna bhi zaroori nahi.
3. **`new` chhodne ke 2 gehre fayde aur hain:**
   - **Ek hi copy sab use karte hain (singleton).** 10 service ke liye 10 DB pool nahi khulte, ek hi share hota hai.
   - **Proxy milta hai.** Spring object ko ek cover me lapet ke deta hai, aur isi cover ki wajah se `@Transactional` aur `@Async` chalte hain.
4. **Spring Boot = Spring + apne aap ki config + andar ka server (Tomcat).** Server alag install nahi karna padta, `java -jar app.jar` se app chal jaati hai.

## Prod wala trap

```java
@Service
class OrderService {
    PaymentService pay = new PaymentService();   // khud bana liya
    // PaymentService ke andar @Transactional laga hai
}
```

`new` se raw object milta hai, Spring ka cover (proxy) nahi. Isliye `@Transactional` bina koi error diye kaam hi nahi karega: aadha save hoga aur rollback nahi hoga. Ye wahi singleton aur proxy wali kahani hai jo multithreading aur design patterns me dekhi thi.

## Tera project

usercrud ka `UserController`:

```java
private final UserService service;
public UserController(UserService service) {   // maanga, Spring ne de diya
    this.service = service;
}
```

Kahin bhi `new UserService()` nahi likha. Ye constructor injection hai, bilkul live.

## Interview line

```
Spring is an IoC container: it creates objects (beans), wires their dependencies
through Dependency Injection, and manages their lifecycle, so my code only has business logic.
Beans are shared singletons and can be wrapped in proxies, which is how @Transactional works.
Spring Boot adds auto-configuration and an embedded server.
```
