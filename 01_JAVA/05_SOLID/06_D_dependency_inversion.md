# D: Dependency Inversion (interface pe tiko)

**Niyam:** bada code (service) asli class pe nahi, interface pe tike; asli object bahar se aaye.

## Kahani

```java
class PaymentService {
    private final MySqlPaymentRepo repo = new MySqlPaymentRepo();   // MySQL se chipka
}
```

Kal Mongo chahiye -> PaymentService chhedo. Test me asli DB ki jagah mock daalna -> `new` andar likha hai, ho hi nahi
sakta. Charger seedha diwar ki wiring me solder kar diya jaisa.

## Fix

```java
interface PaymentRepository { void save(Payment p); }

@Service
class PaymentService {
    private final PaymentRepository repo;
    PaymentService(PaymentRepository repo) { this.repo = repo; }   // bahar se aaya (constructor injection)
}
```

Diwar ka socket (interface): koi bhi charger lagao. Spring ka DI yahi karta: kaunsa repo dena hai, wo Spring tay
karta, service ko pata hi nahi.

## Tera project

usercrud me constructor injection. Test me `new PaymentService(mockRepo)` seedha. PR drill me "andar `new` /
field pe `@Autowired`" pakadta hai = yahi.

## Interview line

```
Services depend on interfaces and get the implementation through constructor injection, so I can swap
MySQL for something else or pass a mock in tests without touching the service.
```
