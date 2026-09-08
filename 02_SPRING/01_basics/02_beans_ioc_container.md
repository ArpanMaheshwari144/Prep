# Spring Beans + IoC Container

> **V90 — Spring 101: Topic 2**

---

## STORY — Office Mein Resources

Office banaya — log, computers, printers. Tu khud manage karega? Login pe kaun aaya, kya use kiya?

**Nahi** — ek **manager** hota hai. Manager ko sab list pata. Koi resource chahiye? Manager se maango — nikal ke deta. Sab centralized.

**Spring IoC Container = manager.**
**Beans = resources** (objects manager ke paas).

---

## WHY — Bean Aur Container Ka Concept

Pre-Spring:
```java
class OrderService {
    OrderRepository repo = new MySQLOrderRepository();   // tu khud banaya
}
```

Spring:
- Tu sirf class likhta + `@Component`
- Spring scan karta — "ye class manage karna hai"
- Banata, container mein rakhta
- Jab koi maange — **inject** kar deta

**Tu sirf intent batata. Spring sab kaam karta.**

---

## Bean Kya Hai?

> **Bean = Spring-managed object.**

Plain Java object — **but Spring ne banaya, manage kiya**.

| | Regular Object | Bean |
|--|---------------|------|
| Created by | `new SomeClass()` | Spring (auto) |
| Managed by | Tu | Spring |
| Lifecycle | Tu | Spring |
| Inject possible? | NO | YES |

---

## IoC Container Visualization

```
         ┌──────────────────────────────────────────────┐
         │   Spring IoC Container                       │
         │   (ApplicationContext)                       │
         │                                              │
         │   Bean Registry (HashMap-jaisa):             │
         │   ┌────────────────┬─────────────────┐       │
         │   │ "userService"  │  UserService    │       │
         │   │                │  (Bean instance)│       │
         │   ├────────────────┼─────────────────┤       │
         │   │ "userRepo"     │  UserRepository │       │
         │   ├────────────────┼─────────────────┤       │
         │   │ "userController"│ UserController │       │
         │   └────────────────┴─────────────────┘       │
         │                                              │
         │   App startup pe: scan → banaya → store      │
         │   Inject pe: lookup → return                 │
         └──────────────────────────────────────────────┘
```

**App start hote hi:**
1. **Component scan** — `@Component`/`@Service`/`@Repository` classes dhundhi
2. **Instantiate** — har bean ka object banaya
3. **Wire** — dependencies inject ki
4. **Store** — internal registry mein rakha (key = naam, value = object)
5. **Ready** — koi maange toh nikal ke deta

---

## ApplicationContext — The Container

**`ApplicationContext`** = Spring ka container interface. App start pe instantiate hota.

Spring Boot mein:
```
@SpringBootApplication          ← Main class pe
SpringApplication.run(...)      ← automatic ApplicationContext banata
```

Tu manually nahi banata.

---

## Bean Scope (Important)

Container mein bean **kitne instances**?

| Scope | Behavior | Default? |
|-------|----------|----------|
| **`singleton`** | 1 instance poori app mein | DEFAULT |
| **`prototype`** | Har baar NAYA instance | |
| **`request`** | 1 per HTTP request | (web app) |
| **`session`** | 1 per user session | (web app) |

```java
@Component                  // default = singleton
@Scope("prototype")          // override
class TempService { }
```

**95% beans singleton** — UserService, Repository, etc. share karte.

---

## Singleton + Mutable State = TRAP

```java
@Component
class UserCache {
    List<User> cache = new ArrayList<>();   // shared mutable state
}
```

**Singleton + mutable list + multi-thread = race condition!**

**Rule:** Singleton beans **stateless** rakho. State immutable / thread-safe banao.

---

## ★ Bean KAB MARTA (scope-wise lifespan) — 8-Sep

> Sawaal: "ek request ka kaam khatam -> bean destroy hoga?" Jawab SCOPE pe depend karta:

```
singleton (default) -> poori app ke liye 1 copy. banta STARTUP pe, marta SHUTDOWN pe (@PreDestroy).
   -> ek request nipta -> destroy NAHI. wahi zinda rehta -> agli request REUSE karti. (yahi point: banao-ek-baar chalao-hamesha)
   -> GC bhi nahi uthata: container strong-reference poore time pakde rehta.
   -> isiliye STATELESS rakho (request-data usme mat rakho, warna 2 request gadbad).

prototype -> har baar NAYA. Spring banake haath me de deta, phir BHOOL jaata.
   -> @PreDestroy NAHI chalta. GC tab uthata jab tera code reference chhod de.

request (web) -> 1 per HTTP request. request KHATAM = destroy.
session (web) -> 1 per user session. logout/timeout = destroy.
```

**Ek line:** default singleton beech me kabhi destroy nahi — reuse ke liye app-bhar zinda, sirf shutdown pe marta. "Kaam khatam, ab rahega?" -> haan, kyunki agli baar bhi wahi kaam aayega.

---

## Bean Annotations Preview

| Annotation | Role |
|-----------|------|
| `@Component` | generic bean |
| `@Service` | @Component + business logic semantic |
| `@Repository` | @Component + DB layer (exception translation) |
| `@Controller` | @Component + HTTP handler |
| `@RestController` | @Controller + @ResponseBody |
| `@Bean` | method-level (3rd party objects) |
| `@Configuration` | class with @Bean methods |

**Internally sab `@Component`** — sirf semantic specialization.

---

## ★ BEAN LIFECYCLE (poora) — naye employee ka onboarding (8-Sep, deep-grill)

> Upar 5-step startup (scan/instantiate/wire/store/ready) mota hai. Andar bean ka POORA lifecycle = employee onboarding:

```
1. INSTANTIATE        -> HR ne hire kiya. bas body, khaali desk. (Spring ne `new` — object bana, khaali)
2. POPULATE / INJECT  -> laptop+tools+team di. (@Autowired deps bhar di — ab sab hai)
3. AWARE callbacks    -> "tu IS company ka, ye tera id". (BeanNameAware/ApplicationContextAware — container ka pata)
4. BeanPostProcessor  -> setup-desk pre-check. YAHI PROXY wrap hota.
   (BEFORE init)         (@Transactional/@Async ka jaadu yahin — bean ko cover me lapetna)
5. INIT               -> "pehla din setup". (@PostConstruct / afterPropertiesSet chalta —
   (@PostConstruct)      NOTE: ab tak deps aa CHUKI, isliye yahan safely use kar sakta)
6. BeanPostProcessor  -> final wrap/proxy laga.
   (AFTER init)
7. READY (in use)     -> kaam kar raha, requests serve. (bean live)
8. DESTROY            -> exit: laptop wapas, cleanup. (@PreDestroy — app shutdown pe, DB conn band waghera)
```

**3 crux (interviewer kholega):**
```
1. ORDER: banao -> deps bharo -> init -> ready -> destroy. dependency HAMESHA init se pehle.
2. @PostConstruct init pe chalta = deps aa chuki -> usme unhe SAFELY use kar sakta
   (constructor me kabhi-kabhi nahi aayi hoti).
3. BeanPostProcessor = wo jagah jahan AOP PROXY banta (bean ko cover me lapetna)
   -> isiliye @Transactional/@Async kaam karte. [[proxy-jdk-vs-cglib]]
```

---

## POWER PHRASE

> *"Spring's IoC container manages beans — it instantiates them at startup via component scanning, wires dependencies, and serves them on request. Default scope is singleton — one instance shared across the entire application. Use @Service, @Repository, @Controller for layer-specific semantics."*

> *"A bean's full lifecycle: instantiate → populate dependencies → Aware callbacks → BeanPostProcessor before-init → @PostConstruct/afterPropertiesSet → BeanPostProcessor after-init → ready → @PreDestroy on shutdown. The BeanPostProcessor step is where AOP proxies (@Transactional, @Async) get applied, and @PostConstruct runs after dependencies are injected so it can safely use them."*

---

## TRAP

> **Bean ≠ special class.**
> Bean = plain Java object, just **created and managed BY Spring** (vs `new` manually). Class definition mein kuch magic nahi.
