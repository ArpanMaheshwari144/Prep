# S = Single Responsibility Principle (SRP)

> **V90 — SOLID: Topic 62**

---

## Definition

> **Ek class ka SIRF EK reason to change hona chahiye.**

Matlab: ek class **ek hi kaam** ke liye responsible — ek hi cheez badle toh class touch ho.

---

## STORY — ShoppingCart Bug

→ ShoppingCart class banayi — ek dum smart, **3 kaam** karti thi:
   1. Cart logic (add item, remove item, total)
   2. DB save (Cart save karna database mein)
   3. Email bhejo (order confirmation email)

```java
class ShoppingCart {
    void addItem(Item i) { ... }
    void calculateTotal() { ... }
    void saveToDB() { ... }       // DB team ka kaam
    void sendEmail() { ... }      // Marketing ka kaam
}
```

→ **Marketing ne email format badla** → ShoppingCart **toot gayi**
→ **DB team ne schema badla** → ShoppingCart **phir toot gayi**
→ **3 alag reasons to change** = SRP violation
→ **Fix:** Split kar do — har class ka apna kaam

---

## Fix — Split Karo

```java
// Cart logic only
class CartService {
    void addItem(Item i) { ... }
    void calculateTotal() { ... }
}

// DB ka kaam
class CartRepository {
    void save(Cart c) { ... }
}

// Email ka kaam
class EmailService {
    void sendOrderConfirmation(Cart c) { ... }
}
```

**Ab:**
- Marketing email change kare → **EmailService** touch
- DB team schema change kare → **CartRepository** touch
- Cart logic change ho → **CartService** touch
- **Teen alag reasons, teen alag classes — koi cross-impact nahi**

---

## SRP TRAP — "One Method Per Class" GALAT

> **SRP ≠ "ek method per class"**
> **SRP = "ek REASON TO CHANGE"**

```java
// SAHI — multiple methods, ONE reason to change
class CartService {
    void addItem() { }
    void removeItem() { }
    void calculateTotal() { }
    void applyDiscount() { }
}
// Ye sab "cart logic" hi hai — ONE reason to change (cart business rules)
```

---

## SRP Spring Boot Mein

```java
@Controller   ← HTTP request handle (web layer)
class OrderController { ... }

@Service      ← business logic (calculate, validate)
class OrderService { ... }

@Repository   ← DB ka kaam
class OrderRepository { ... }
```

**Spring ne pre-built SRP layers diye** — har annotation ka ek hi kaam.

---

## POWER PHRASE

> *"Single Responsibility means a class should have only one reason to change. If ShoppingCart handles cart logic, DB saving, and email notifications — three different teams can break it for three different reasons — split them."*
