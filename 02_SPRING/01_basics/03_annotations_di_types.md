# Spring Annotations + DI Types

> **V90 — Spring 101: Topic 3**

---

## WHY — Annotations Kyu?

**Pre Spring 2.5:** XML mein bean definitions:
```xml
<bean id="userService" class="com.app.UserService">
    <property name="userRepo" ref="userRepo"/>
</bean>
```
**100 beans = 1000 lines XML hell.**

**Spring 2.5+:** Annotations — same kaam, 1/10 code, Java mein hi sab.

---

## Component Annotations — Family Tree

```
                @Component
                (generic — Spring-managed bean)
                     │
                     │ specializations
                     ▼
        ┌────────────┼────────────┬────────────┐
        ▼            ▼            ▼            ▼
   @Service    @Repository   @Controller   @RestController
   (business)  (DB layer)    (HTTP MVC)    (REST API)
```

**Sab internally `@Component`.** Spring ke liye fark nahi — code readability ke liye semantic specialization.

---

## Specialization Table

| Annotation | Layer | Extra Behavior |
|-----------|-------|----------------|
| `@Component` | Generic / utility | None — basic bean |
| `@Service` | Business logic | None — just semantic |
| `@Repository` | DB / data access | **Auto translates DB exceptions** to Spring's `DataAccessException` |
| `@Controller` | Web MVC (returns view names) | None |
| `@RestController` | REST API (returns JSON) | `@Controller + @ResponseBody` |

**Real value-add only `@Repository`** — exception translation. Baki sirf semantic.

---

## `@Component` vs `@Bean`

### `@Component` — class-level (TUMHARI class)
```java
@Component
class MyService { }     // tumhari class
```

### `@Bean` — method-level (3rd party class)
```java
@Configuration
class AppConfig {
    @Bean
    RestTemplate restTemplate() {        // 3rd party class
        return new RestTemplate();
    }
}
```

**`RestTemplate`** Spring/library ka class — **source modify nahi kar sakte**. Toh `@Bean` method banao.

---

## `@Autowired` — 3 Types of DI

### 1. Field Injection (DISCOURAGED)
```java
@Service
class UserService {
    @Autowired
    private UserRepository userRepo;
}
```
- `final` field nahi
- Test mein mock pass mushkil
- Hidden dependencies

### 2. Setter Injection (Rare)
```java
@Service
class UserService {
    private UserRepository userRepo;

    @Autowired
    public void setUserRepo(UserRepository userRepo) {
        this.userRepo = userRepo;
    }
}
```
**Use case:** Optional dependencies.

### 3. Constructor Injection (BEST)
```java
@Service
class UserService {
    private final UserRepository userRepo;     // final

    public UserService(UserRepository userRepo) {
        this.userRepo = userRepo;
    }
}
```
- `final` fields
- Test easy (`new UserService(mockRepo)`)
- Required deps enforced
- Spring official recommendation

---

## DI Comparison Table

| Feature | Field | Setter | Constructor |
|---------|-------|--------|-------------|
| `final` field | | | |
| Test without Spring | Mushkil | OK | Easy |
| Required deps enforced | | | |
| Industry standard | | | YES |

---

## ★ CIRCULAR DEPENDENCY — A ko B chahiye, B ko A (8-Sep, deep-grill)

> Do bean ek doosre pe depend: `A(B b)` aur `B(A a)`. Kaun pehle bane?

**CONSTRUCTOR injection pe FAIL (deadlock):**
```
Rule: A tabhi bane jab HAATH me B ho; B tabhi bane jab HAATH me A ho.
Spring: "A banau -> B chahiye -> B banau -> A chahiye -> A banau..." GOL-GOL -> BeanCurrentlyInCreationException.

   A ──needs──► B
   ▲           │
   └──needs────┘     dono ek-doosre ka intezaar -> koi paida nahi -> DEADLOCK
```
Darwaze pe do dost: "pehle TU aa" / "nahi pehle TU". Koi andar nahi aata.

**KYUN fail:** constructor me "paida hona" aur "dependency milna" EK step hai -> khaali paida ho hi nahi sakta -> cycle todne ka mauka hi nahi.

**SETTER/FIELD injection pe TOOT jaata (movie):**
Yahan paida-hona pehle, inject baad me. Spring ek DIARY (cache) me aadha-bana pata likhta:
```
1. A ka KHAALI shell paida (constructor bina B ke chal gaya)     A=[khaali, B missing]
2. Spring diary me likha: "A yahan hai -> [address]"             DIARY:{A->addr}
3. Ab B banao. B ko A chahiye -> diary dekha -> "A to yaha hai" -> B ko A ka address diya. B READY.
4. Wapas A ke khaali haath me ready-B pakda diya (setter).        A READY.
5. DONO ready -> cycle TOOT gaya.
```

**★ 3-LEVEL CACHE (interviewer kholega) = wahi DIARY, 3 khaane:**
```
1. singletonObjects       -> poore READY bean
2. earlySingletonObjects  -> aadhe-bane (expose ho chuke, init baaki)
3. singletonFactories     -> factory jo "early reference" bana ke deti
```
Level-3 sirf isliye: agar A ko PROXY (cover) me lapetna ho (@Transactional), to B ko jo early-reference mile wo PROXY ho, raw nahi. [[proxy-jdk-vs-cglib]]

**FIX (order):**
```
1. @Lazy ek constructor-param pe -> Spring nakli-lazy proxy inject, asli bean pehli use pe -> chicken-egg toota
2. REFACTOR (BEST) -> teesri class nikaal ke cycle hatao (cycle aksar DESIGN-SMELL)
3. setter/field injection -> Spring diary-trick se todta (PAR Boot 2.6+ me by-default BAND ->
   spring.main.allow-circular-references=true chahiye; LAST option)
```
★ NOTE: field/setter cycle todta ZAROOR, par "field injection accha" NAHI -> general best abhi bhi CONSTRUCTOR; cycle pe pehle @Lazy/refactor, field-injection last.

---

## Multiple Beans Resolution

```java
@Service class JpaUserRepo implements UserRepository { }
@Service class MongoUserRepo implements UserRepository { }
```

**Ambiguity!** Spring confused — kaunsa inject kare?

### Solution 1: `@Primary` (default pick)
```java
@Primary
@Service class JpaUserRepo implements UserRepository { }
```

### Solution 2: `@Qualifier` (explicit)
```java
@Autowired
@Qualifier("mongoUserRepo")
private UserRepository userRepo;
```

---

## POWER PHRASES

> *"`@Component` is the generic Spring-managed bean. `@Service`, `@Repository`, `@Controller` are semantic specializations — only `@Repository` adds real behavior (exception translation to Spring's DataAccessException)."*

> *"Constructor injection is recommended in Spring — final fields, easy testing, required dependencies enforced. Field injection via `@Autowired` is discouraged for immutability/testability/null-safety reasons — it hides dependencies and can mask circular-dependency design smells."*

> ★ FIX (8-Sep): "field injection discouraged kyunki reflection" GALAT tha — constructor injection bhi reflection use karta. Asli reason = **immutability (final), testability (`new UserService(mock)`), null-safety, hidden-deps, circular-dep masking** — reflection nahi.

---

## TRAPS

| Trap | Reality |
|------|---------|
| "@Component vs @Bean — same hai" | NO — Component class-level, Bean method-level for 3rd party |
| "Field injection easy" | Yes but discouraged — final, testing, required-check fail |
| "Multiple beans = error" | Solve via `@Primary` ya `@Qualifier` |
| "@Repository = just label" | NO — adds exception translation, real value |
