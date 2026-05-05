# D = Dependency Inversion Principle (DIP)

> **V90 — SOLID: Topic 66**

---

## 🎯 Definition

> **High-level modules should depend on ABSTRACTIONS, not concrete implementations.**

Matlab: `OrderService` ko **`OrderRepository` interface** pe depend karna chahiye, **`MySQLOrderRepository` (concrete class) pe NAHI**.

---

## 📖 STORY — Tight Coupling Hell

→ Tune **OrderService** likhi — directly **MySQLOrderRepository** use kiya:

```java
class OrderService {
    MySQLOrderRepository repo = new MySQLOrderRepository();   // 🔴 tight coupling

    void placeOrder() {
        repo.save(...);
    }
}
```

→ **Kal MongoDB chahiye** — OrderService **rewrite karni padti**
→ **Test mein mock karna mushkil** — `new MySQLOrderRepository()` hardcoded
→ **2 alag DBs support nahi ho sakte** — sirf MySQL hi pakda
→ **DIP violation** — concrete class pe depend karta tha

---

## ✅ Fix — Interface Pe Depend Karo

```java
// 1. Interface define karo
interface OrderRepository {
    void save(Order o);
}

// 2. Concrete classes implement karein
class MySQLOrderRepository implements OrderRepository { ... }
class MongoOrderRepository implements OrderRepository { ... }

// 3. OrderService interface pe depend kare
class OrderService {
    OrderRepository repo;     // ← interface, koi specific class nahi

    OrderService(OrderRepository repo) {
        this.repo = repo;     // ← inject from outside
    }
}

// 4. Caller decide karta kaunsa DB
OrderService svc1 = new OrderService(new MySQLOrderRepository());
OrderService svc2 = new OrderService(new MongoOrderRepository());

// Test mein:
OrderService svc3 = new OrderService(new MockOrderRepository());   // ✅ easy mock
```

→ **OrderService ko pata hi nahi kaunsa DB hai** — bas interface pata
→ **Dependency outside se inject** hoti — Dependency Injection (DI)

---

## 🧠 DIP Visualization

```
              ❌ BAD — High-level depends on low-level

              OrderService (high-level — business logic)
                    │
                    ▼
              MySQLOrderRepository (low-level — DB code)

   Problem: business logic = DB-specific. Tight coupled.
   Naya DB? Service rewrite. Test mein? Mock impossible.


              ✅ GOOD — Both depend on abstraction

              OrderService (high-level)
                    │
                    ▼
              ┌──────────────────┐
              │ OrderRepository  │  ← abstraction (interface)
              │  (interface)     │
              └────────┬─────────┘
                       │ implements
              ┌────────┼────────┐
              ▼        ▼        ▼
            MySQL    Mongo    Mock
            (concrete classes)

   Service interface pe depend, koi specific impl nahi.
   Naya DB? Naya impl, service untouched.
```

---

## 🔴 DIP TRAP — DIP ≠ Dependency Injection

> **DIP = PRINCIPLE.** Goal — abstraction pe depend karo.
> **DI = TECHNIQUE** to achieve DIP — dependency outside se pass karo.
>
> Spring's `@Autowired` = DI tool jo DIP achieve karta.

```java
// DIP achieve via DI (Spring style):
@Service
class OrderService {
    @Autowired
    OrderRepository repo;     // Spring inject karta — abstraction
}
```

---

## 💬 POWER PHRASE

> *"Dependency Inversion means high-level modules should depend on abstractions, not on concrete implementations. Spring's `@Autowired` achieves this — the service depends on the repository interface, not on a specific database class. DIP is the principle; DI is one way to achieve it."*
