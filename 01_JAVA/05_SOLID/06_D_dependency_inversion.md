# D: Dependency Inversion (interface pe tiko)

**Niyam:** bada code (jaise service) kisi asli class pe nahi, interface pe tika hona chahiye, aur asli object bahar se aana chahiye.

## Kahani

```java
class PaymentService {
    private final MySqlPaymentRepo repo = new MySqlPaymentRepo();   // MySQL se chipka
}
```

Yahan `PaymentService` ne andar hi `new MySqlPaymentRepo()` likh diya, to wo MySQL se chipak gaya. Kal Mongo chahiye hua, to
PaymentService ko chhedna padega. Aur test me asli DB ki jagah mock daalna ho, to wo ho hi nahi sakta, kyunki `new` andar
likha hai aur bahar se kuch dene ka raasta hi nahi hai. Ye aisa hai jaise charger ko seedha diwar ki wiring me solder kar diya ho.

## Fix

```java
interface PaymentRepository { void save(Payment p); }

@Service
class PaymentService {
    private final PaymentRepository repo;
    PaymentService(PaymentRepository repo) { this.repo = repo; }   // bahar se aaya (constructor injection)
}
```

Ab service sirf `PaymentRepository` interface jaanti hai, aur asli repo constructor se bahar se aata hai. Ye diwar ke socket
jaisa hai: socket interface hai, usme koi bhi charger laga do. Spring ka DI yahi karta hai: kaunsa repo dena hai, ye Spring tay
karta hai, aur service ko pata bhi nahi hota ki andar MySQL hai ya kuch aur.

## Tera project

usercrud me constructor injection hi use hua hai. Isliye test me seedha `new PaymentService(mockRepo)` likh ke mock daal sakte
ho. PR drill me jab tu "andar `new` likha hai" ya "field pe `@Autowired` lagaya hai" wali galti pakadta hai, to wo yahi niyam
toot raha hota hai.

## Interview line

```
Services depend on interfaces and get the implementation through constructor injection, so I can swap
MySQL for something else or pass a mock in tests without touching the service.
```
