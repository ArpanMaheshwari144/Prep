# Spring Beans + IoC Container

> **V90 — Spring 101: Topic 2**

---

## 📖 STORY — Office Mein Resources

Office banaya — log, computers, printers. Tu khud manage karega? Login pe kaun aaya, kya use kiya?

**Nahi** — ek **manager** hota hai. Manager ko sab list pata. Koi resource chahiye? Manager se maango — nikal ke deta. Sab centralized.

**Spring IoC Container = manager.**
**Beans = resources** (objects manager ke paas).

---

## 🟡 WHY — Bean Aur Container Ka Concept

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

## 🧠 Bean Kya Hai?

> **Bean = Spring-managed object.**

Plain Java object — **but Spring ne banaya, manage kiya**.

| | Regular Object | Bean |
|--|---------------|------|
| Created by | `new SomeClass()` | Spring (auto) |
| Managed by | Tu | Spring |
| Lifecycle | Tu | Spring |
| Inject possible? | NO | YES |

---

## 🧠 IoC Container Visualization

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

## 🎯 ApplicationContext — The Container

**`ApplicationContext`** = Spring ka container interface. App start pe instantiate hota.

Spring Boot mein:
```
@SpringBootApplication          ← Main class pe
SpringApplication.run(...)      ← automatic ApplicationContext banata
```

Tu manually nahi banata.

---

## 🎯 Bean Scope (Important)

Container mein bean **kitne instances**?

| Scope | Behavior | Default? |
|-------|----------|----------|
| **`singleton`** | 1 instance poori app mein | ✅ DEFAULT |
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

## ⚠️ Singleton + Mutable State = TRAP

```java
@Component
class UserCache {
    List<User> cache = new ArrayList<>();   // 🔴 shared mutable state
}
```

**Singleton + mutable list + multi-thread = race condition!**

**Rule:** Singleton beans **stateless** rakho. State immutable / thread-safe banao.

---

## 📊 Bean Annotations Preview

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

## 💬 POWER PHRASE

> *"Spring's IoC container manages beans — it instantiates them at startup via component scanning, wires dependencies, and serves them on request. Default scope is singleton — one instance shared across the entire application. Use @Service, @Repository, @Controller for layer-specific semantics."*

---

## 🔴 TRAP

> **Bean ≠ special class.**
> Bean = plain Java object, just **created and managed BY Spring** (vs `new` manually). Class definition mein kuch magic nahi.
