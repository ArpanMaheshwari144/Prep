# Spring: kya hai, kyun aaya

## Scene

Tera OrderService order save karta hai. Kal boss bola "MySQL chhodo, MongoDB pe jao."

```java
class OrderServiceOld {                  // khud banata hai
    OrderRepo repo = new MySQLRepo();
}
class OrderServiceNew {                  // maangta hai
    private final OrderRepo repo;
    OrderServiceNew(OrderRepo repo) { this.repo = repo; }
}

new OrderServiceOld().place("pizza");
new OrderServiceNew(new MySQLRepo()).place("pizza");
new OrderServiceNew(new MongoRepo()).place("pizza");
```

```
MySQL me save: pizza
MySQL me save: pizza
Mongo me save: pizza
```

## Dikkat

Old wala MySQL se chipka hua hai. Mongo pe jaane ke liye service ka code hi kholna padega. Test me nakli repo daalna ho tab bhi yahi `new` beech me aata hai. Aur 50 class me aisa likha ho to 50 jagah badlaav karna padega.

## Hal

New wala repo khud nahi banata, constructor me maangta hai. Bahar se jo do wahi chalega, MySQL ho ya Mongo, service ka code wahi rehta hai. Spring yahi "bahar se dena" khud karta hai.

- **IoC (Inversion of Control):** object banana ab tera kaam nahi raha. Spring ka container (ApplicationContext) app start hote hi bana deta hai.
- **DI (Dependency Injection):** wo bana hua object tere constructor me daal deta hai. Class me ek hi constructor ho to `@Autowired` likhna bhi zaroori nahi.
- **Bonus:** sab ek hi copy share karte hain (singleton), isliye 10 service ke liye 10 DB pool nahi khulte.

Tere usercrud me yahi chal raha hai: `UserController(UserService service)`. Poore code me kahin `new UserService()` nahi likha, controller ne maanga aur Spring ne de diya.

## Prod wala trap

```java
@Service
class OrderService {
    PaymentService pay = new PaymentService();   // khud bana liya
}
// PaymentService ke andar @Transactional hai
```

`new` likhne se tujhe asli object milta hai, Spring ka cover (proxy) nahi milta. `@Transactional` usi cover se chalta hai, isliye yahan wo bina koi error diye kaam karna band kar deta hai: aadha data save ho jaata hai aur rollback nahi hota. Ye wahi singleton wale trap jaisa hai: Spring ki cheez ho to Spring se maango, `new` mat karo.
