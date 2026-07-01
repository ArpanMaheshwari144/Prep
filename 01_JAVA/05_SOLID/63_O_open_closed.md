# O = Open / Closed Principle (OCP)

> **V90 — SOLID: Topic 63**

---

## Definition

> **Class extension ke liye OPEN, modification ke liye CLOSED.**

Matlab: naya feature add karo **naya class likh ke** — existing tested code ko **chhedo mat**.

---

## ★ REAL-LIFE ANALOGY (JP: "real-life use batao")

> Electric board me **naya appliance** lagana → naya **socket** add karo, poori building ki wiring mat kholo.
> (extension = naya socket | modification = poori wiring redo → galat.)
> **Code:** naya payment mode → nayi class (`CryptoPayment`), purani `Payment` class chhuo mat.

---

## STORY — Payment Processor Mess

→ Pehle PayPal payment tha. Code:

```java
class PaymentService {
    void process(String type) {
        if (type.equals("paypal")) {
            // paypal logic
        }
    }
}
```

→ **Stripe add karna tha** — `else if` lagaya:

```java
if (type.equals("paypal")) { ... }
else if (type.equals("stripe")) { ... }
```

→ **Phir Razorpay aaya, UPI aaya, Crypto aaya** — har baar `if-else` ladder badhta gaya
→ Existing **tested code chhedna pad raha** — bug ka risk
→ **OCP violation** — class **extend karne ke liye open**, **modification karne ke liye closed nahi tha**

---

## Fix — Interface + Naya Class Add

```java
// 1. Interface define karo
interface PaymentProcessor {
    void process(double amount);
}

// 2. Har payment ka apna class
class PayPalProcessor implements PaymentProcessor {
    public void process(double amount) { /* paypal logic */ }
}

class StripeProcessor implements PaymentProcessor {
    public void process(double amount) { /* stripe logic */ }
}

// 3. Service ne sirf interface dekhi
class PaymentService {
    void process(PaymentProcessor processor, double amount) {
        processor.process(amount);
    }
}
```

**Ab Razorpay add karna hai?**
```java
class RazorpayProcessor implements PaymentProcessor {
    public void process(double amount) { /* razorpay logic */ }
}
```

→ **Naya class banao**. Existing PayPal/Stripe **untouched**, PaymentService **untouched**
→ Tested code mein **ZERO change** — sirf naya class add hua

---

## OCP Visualization

```
                BAD — if-else chain

                  PaymentService
                ┌──────────────────┐
                │ if(paypal)       │  ← naya add = chhedo
                │ else if(stripe)  │
                │ else if(upi)     │
                │ else if(razor)   │     ← bug risk har baar
                └──────────────────┘


                GOOD — interface + new class

                  PaymentProcessor (interface)
                          │
                ┌─────────┼──────────┬─────────┐
                ▼         ▼          ▼         ▼
              PayPal   Stripe       UPI    Razorpay
              (class)  (class)    (class)  (class)

              Naya add? Naya class. Existing UNTOUCHED.
```

---

## OCP Spring Boot Mein

```java
// Interface
interface OrderRepository {
    Order save(Order o);
}

// MySQL impl
@Repository
class MySQLOrderRepository implements OrderRepository { ... }

// Kal MongoDB chahiye? Naya class:
@Repository
class MongoOrderRepository implements OrderRepository { ... }

// OrderService UNTOUCHED — sirf interface se kaam karta
@Service
class OrderService {
    @Autowired OrderRepository repo;    // interface, koi specific impl nahi
}
```

**Spring `@Repository` interface + `@Autowired` = OCP** — App never knows DB impl.

---

## POWER PHRASE

> *"Open/Closed means you should add new features by writing new classes — not by editing existing tested code. Use interfaces so new implementations can plug in without touching the existing logic."*
