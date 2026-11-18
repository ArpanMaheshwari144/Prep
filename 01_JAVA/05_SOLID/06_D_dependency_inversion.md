# D: Dependency Inversion (interface pe tiko)

**Koi poochhe "Dependency Inversion kya hai" to aise bolo:**
```
Service andar new MySqlRepo() nahi likhti, interface leti hai aur asli object bahar se
(constructor se) aata hai. Isliye kal Mongo lagao ya test me nakli repo, service nahi badalti.
Spring ka constructor injection yahi karta hai. Socket interface hai, charger koi bhi.
```

## Kahani

```java
class PaymentService {
    private final PaymentRepository repo = new MySqlPaymentRepo();   // MySQL se chipka
}
```

Service andar hi `new MySqlPaymentRepo()` likh ke MySQL se chipak gayi. Test likhne baithe, laptop pe MySQL chalu nahi tha.

Chala ke dekha:

```
--- test: andar new MySqlPaymentRepo() ---
TEST FAIL: MySQL se connect nahi ho paya (localhost:3306)
--- test: constructor se nakli repo ---
pay = OK, saved = [arpan -500]
```

**Dikkat:** `new` andar likha hai, bahar se kuch dene ka raasta hi nahi, to har test asli DB maangta hai. Kal Mongo chahiye to
bhi service kholni padegi. Charger ko diwar ki wiring me solder kar diya jaisa.

## Fix

```java
interface PaymentRepository { void save(Payment p); }

@Service
class PaymentService {
    private final PaymentRepository repo;
    PaymentService(PaymentRepository repo) { this.repo = repo; }   // bahar se aaya (constructor injection)
}
```

Service sirf interface jaanti hai. Asli MySQL repo Spring deta hai, test me `new PaymentService(nakliRepo)`.

## Trap + tera project

usercrud me constructor injection hi hai, isliye test me seedha mock daal sakte ho. PR drill me "andar `new` likha hai" ya "field pe
`@Autowired`" pakadta hai, wo yahi niyam toot raha. Field `@Autowired` me bhi test me bina Spring ke repo daalne ka seedha raasta nahi.
