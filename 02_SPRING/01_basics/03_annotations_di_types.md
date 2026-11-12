# Spring Annotations + DI Types

## Scene

Kal tak sirf `SmsSender` tha. Aaj teammate ne `EmailSender` jod diya, dono `Sender` interface ke:

```java
@Service class SmsSender implements Sender { }
@Service class EmailSender implements Sender { }   // naya joda

@Service
class OrderService {
    OrderService(Sender sender) { ... }             // kaunsa Sender?
}
```

App start karo to:

```
NoUniqueBeanDefinitionException: expected single matching bean but found 2: smsSender,emailSender
APPLICATION FAILED TO START
```

## Dikkat

Spring ko samajh nahi aata ki `OrderService` ko kaunsa `Sender` de, isliye poori app hi nahi uthti, sirf ek feature nahi.

## Hal

- `@Primary` jis bean pe lagao wo default ban jaata hai: jahan kuch na bolo, wahan wahi milega.
- `@Qualifier("emailSender")` constructor parameter pe lagao, to tu khud chunta hai ki yahan kaunsa chahiye.

## Injection ke 3 tareeke

```
constructor  ->  final rakh sakte, test me new OrderService(mockSender), koi dep chhoote nahi  (SAHI)
setter       ->  optional dependency ke liye
field        ->  @Autowired private X x;  na final, na aasaan test, dep chhupi rehti
```

Field wala isliye kharab nahi hai ki wo reflection use karta hai, kyunki constructor injection bhi reflection hi use karta hai. Asli wajah upar ki teen cheezein hain.

`@Component` vs `@Bean`: apni class pe `@Component` / `@Service` lagate hain. Library ki class (jaise `RestTemplate`) ka code tu badal nahi sakta, us pe annotation nahi laga sakta, isliye `@Configuration` class me ek `@Bean` method likhte hain jo `return new RestTemplate();` karta hai.

Tera usercrud: `AuthController` ki 5 dependencies sab `private final` hain aur ek hi constructor hai, isliye `@Autowired` likhna bhi nahi pada. Poore main code me field `@Autowired` ek bhi jagah nahi hai.

## Lombok `@RequiredArgsConstructor` (PR me aata hai, bug NAHI)

Tere mini_payment_ms ke `OrderService` me constructor haath se likha hai:

```java
@Service
public class OrderService {
    private final OrderRepository repo;
    private final PaymentClient client;

    // constructor injection — Spring repo + client khud inject karta
    public OrderService(OrderRepository repo, PaymentClient client) {
        this.repo = repo;
        this.client = client;
    }
}
```

Yahi constructor har class me haath se likhna bore karta hai, isliye Lombok ka annotation use karte hain:

```java
@Service
@RequiredArgsConstructor          // upar wala constructor khud bana deta hai
public class OrderService {
    private final OrderRepository repo;
    private final PaymentClient client;
}
```

`@RequiredArgsConstructor` jitne `final` field hain unka constructor apne aap bana deta hai. Compile ke baad dono code bilkul ek jaise hain, dono constructor injection hi hain. Isliye PR me `@RequiredArgsConstructor` + `private final` dikhe to wo SAHI hai. Bug tab hai jab field pe `@Autowired private Repo repo;` ho: pehchaan = `@Autowired` hai aur `final` nahi.

## Trap (circular)

A ko B chahiye aur B ko A, jaise darwaze pe do dost "pehle tu, pehle tu" karte khade hain aur koi andar nahi jaata. Constructor injection me ye `BeanCurrentlyInCreationException` deta hai, aur Boot 2.6 se field injection me bhi ye by default band hai. Sahi ilaaj hai teesri class nikaal ke cycle todo, kyunki aksar ye design ki galti hoti hai. Jaldi ka jugaad hai ek parameter pe `@Lazy`.
