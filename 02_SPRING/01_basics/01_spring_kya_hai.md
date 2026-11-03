# Spring: kya hai, kyun aaya

## Kahani

2002 ka ek Java dev OrderService likh raha hai:

```java
class OrderService {
    OrderRepository repo = new MySQLOrderRepository();
    EmailService email  = new SendGridEmailService();
}
```

Maan lo kal MongoDB pe jaana pada. Tab service ka code hi badalna padega, kyunki MySQL wala repo service ke andar `new` se likha hua hai. Test me nakli repo daalna ho to bhi yahi `new` beech me aa jaata hai, kyunki class ne apna auzaar khud bana rakha hai. Jab har class apne auzaar khud banati hai, to ek chhota sa badlaav bhi 50 jagah karna padta hai.

Spring ne kaha: "Tu sirf kaam ka logic likh. Object banana, unhe aapas me jodna aur unhe sambhalna mera kaam hai."

## Tasveer

Pizza counter socho. Pehle tu khud kitchen jaata tha: atta goondhta, oven jalata. Ab tu counter pe bas bolta hai "Margherita chahiye", aur bana-banaya pizza mil jaata hai.

```
pehle:   OrderService --new--> MySQLRepo            (tu khud banata tha)
Spring:  Container --banata + daalta--> OrderService (tu sirf maangta hai)
```

## 4 khaas baat

1. **IoC (Inversion of Control) ka matlab hai ki control ulta ho gaya.** Ab object tu nahi banata, Spring ka container (ApplicationContext) app start hote hi bana deta hai. Andar wo bhi `new` ya reflection hi use karta hai, isme koi jaadu nahi hai.
2. **DI (Dependency Injection) ek pattern hai, Spring ka apna feature nahi hai.** Ise tu haath se bhi kar sakta hai, jaise `new AccountService(repo)`. Spring bas ye kaam apne aap kar deta hai. Constructor injection sabse sahi tareeka hai, aur agar class me ek hi constructor ho to `@Autowired` likhna bhi zaroori nahi.
3. **`new` chhodne ke 2 gehre fayde aur hain:**
   - **Ek hi copy sab use karte hain (singleton).** Isliye 10 service ke liye 10 DB pool nahi khulte, sab ek hi pool share karte hain.
   - **Proxy milta hai.** Spring tera object ek cover me lapet ke deta hai, aur `@Transactional` aur `@Async` isi cover ki wajah se chalte hain.
4. **Spring Boot matlab Spring ke upar apne aap hone wali config aur andar baitha server (Tomcat).** Isliye server alag se install nahi karna padta, `java -jar app.jar` chalao aur app chal jaati hai.

## Prod wala trap

```java
@Service
class OrderService {
    PaymentService pay = new PaymentService();   // khud bana liya
    // PaymentService ke andar @Transactional laga hai
}
```

`new` likhne se tujhe raw object milta hai, Spring ka cover (proxy) nahi milta. Isliye `@Transactional` bina koi error diye chup-chaap kaam karna band kar deta hai: aadha data save ho jaata hai aur rollback nahi hota. Ye wahi singleton aur proxy wali kahani hai jo multithreading aur design patterns me dekhi thi.

## Tera project

usercrud ka `UserController`:

```java
private final UserService service;
public UserController(UserService service) {   // maanga, Spring ne de diya
    this.service = service;
}
```

Poore code me kahin bhi `new UserService()` nahi likha. Controller ne constructor me service maangi aur Spring ne de di, yahi constructor injection hai jo tere project me live chal raha hai.

## Interview line

```
Spring is an IoC container: it creates objects (beans), wires their dependencies
through Dependency Injection, and manages their lifecycle, so my code only has business logic.
Beans are shared singletons and can be wrapped in proxies, which is how @Transactional works.
Spring Boot adds auto-configuration and an embedded server.
```
