# Strategy Pattern — Algorithm Switching

> **Design Patterns: Topic 6 — Behavioral Pattern (Interview Gold)**

---

## STORY — Payment Counter

> Tu Amazon checkout pe payment select kar raha:
>
> Credit Card / UPI / Wallet / NetBanking
>
> Har payment ka **alag algorithm** — but checkout flow same:
> ```
>   process(amount) → success/failure
> ```
>
> Tu kaunsa method choose karta — runtime pe decide. Amazon ka code **switch nahi karta** harek payment ke liye `if-else` mein. Har payment **alag class**, common interface, runtime swap.
>
> **Strategy pattern same kaam karta** — algorithms interchangeable + selectable at runtime.

---

## The Problem — If-Else Hell

```java
public class PaymentService {
    public void pay(String type, double amount) {
        if (type.equals("CARD")) {
            // 50 lines of card processing logic
        } else if (type.equals("UPI")) {
            // 50 lines of UPI logic
        } else if (type.equals("WALLET")) {
            // 50 lines of wallet logic
        } else if (type.equals("NETBANKING")) {
            // 50 lines of netbanking logic
        }
    }
}
```

**Problems:**
1. **God class** — single class 200+ lines
2. **OCP violation** — naya method add = class modify
3. **Hard to test** each algo independently
4. **Coupling** — payment types hardcoded

---

## Strategy Solution

```java
// 1. Strategy interface
public interface PaymentStrategy {
    void pay(double amount);
}

// 2. Concrete strategies — each in its own class
public class CardPayment implements PaymentStrategy {
    public void pay(double amount) {
        // card processing
    }
}

public class UpiPayment implements PaymentStrategy {
    public void pay(double amount) {
        // UPI processing
    }
}

public class WalletPayment implements PaymentStrategy {
    public void pay(double amount) {
        // wallet processing
    }
}

// 3. Context — uses strategy
public class PaymentService {
    private PaymentStrategy strategy;

    public void setStrategy(PaymentStrategy s) {
        this.strategy = s;
    }

    public void checkout(double amount) {
        strategy.pay(amount);   // delegate to current strategy
    }
}

// Usage:
PaymentService service = new PaymentService();
service.setStrategy(new UpiPayment());
service.checkout(1000);

service.setStrategy(new CardPayment());   // SWITCH algorithm at runtime
service.checkout(500);
```

**Benefits:**
- Each algorithm in own class (SRP)
- Add new payment type → new class only (OCP)
- Runtime switching possible
- Each tested independently

---

## VISUAL

```
   CONTEXT                   STRATEGY (interface)         CONCRETE STRATEGIES
   ───────                   ─────────────────────        ───────────────────

   PaymentService            PaymentStrategy              ┌──► CardPayment
        │                         │                       │
        │ uses                    │ implements ───────────┼──► UpiPayment
        │                         │                       │
        │ ──────────────────►     │                       └──► WalletPayment
        │                         │
        │ strategy.pay(amount)    │
        │                         │
        │                         │ runtime dispatch
        │                         ▼
        │              ◄───── chosen concrete strategy
```

**Caller swaps strategy at runtime** — context code unchanged.

---

## Real-World Examples

### 1. **`Comparator`** (Collections.sort)
```java
List<User> users = ...;
users.sort(Comparator.comparing(User::getName));     // strategy 1
users.sort(Comparator.comparing(User::getAge));       // strategy 2 (different algorithm)
```

### 2. **Spring's `PasswordEncoder`** (already use kar raha)
```java
@Bean
public PasswordEncoder passwordEncoder() {
    return new BCryptPasswordEncoder();   // strategy
    // OR new PBKDF2PasswordEncoder();    // swap
    // OR new SCryptPasswordEncoder();
}
```

### 3. **`AuthenticationProvider`** (Spring Security) — multiple auth strategies (DB, LDAP, OAuth) implement same interface.

### 4. **Compression algorithms** — ZIP, GZIP, BZIP2 → all implement same interface.

---

## SOLID Connection

- **OCP** — Naya strategy add karne mein context unchanged (perfect example)
- **SRP** — Each strategy ek algorithm ka responsibility
- **DIP** — Context interface pe depend, concrete strategies pe nahi
- **LSP** — Sab strategies interchangeable

**4 SOLID principles ek pattern mein** — interview gold.

---

## Strategy vs State (common confusion)

```
Strategy:
   Client EXPLICITLY swaps strategy
   strategy = new CardPayment();

State:
   State changes INTERNALLY based on logic
   if (count > 10) state = WARNING_STATE;
```

**Strategy = client-driven swap. State = self-managed transitions.**

---

## INTERVIEW TALKING POINT

**Q: "Strategy pattern kya hai aur kab use karte ho?"**

> *"Strategy = behavioral pattern jo **algorithms encapsulate** karta separate classes mein, **interchangeable** banata. If-else chain ki jagah polymorphism use karta. Context strategy interface pe depend karta — concrete implementation runtime pe inject hota.
>
> Use cases: payment methods (Card/UPI/Wallet), sorting algorithms (Comparator), authentication providers (DB/LDAP/OAuth), compression (ZIP/GZIP). Spring mein **PasswordEncoder, AuthenticationProvider** — sab Strategy pattern follow karte. **OCP + SRP + DIP + LSP — 4 SOLID principles** ek pattern mein."*

**Q: "Strategy vs Factory?"**

> *"Different concerns:
> - **Factory** = object **creation** encapsulate (which class to instantiate)
> - **Strategy** = algorithm **behavior** encapsulate (which way to execute)
>
> Often used together — Factory creates Strategy. e.g., `PaymentStrategyFactory.create('UPI')` returns UpiPayment strategy."*

---

## POWER PHRASE

> **"Strategy pattern algorithms ko separate classes mein encapsulate karta — runtime swap possible. If-else chains eliminate, OCP/SRP/DIP/LSP saare follow. Spring's PasswordEncoder + AuthenticationProvider perfect examples."**

---

## MEMORY HOOK

```
Strategy = "Payment counter — choose your method"
   • Common interface (PaymentStrategy.pay)
   • Multiple implementations (Card, UPI, Wallet)
   • Context delegates to chosen strategy
   • Runtime swap possible

Code structure:
   1. Strategy interface (common contract)
   2. Concrete strategies (each algorithm in own class)
   3. Context (uses strategy reference, delegates)

Real-world:
   • Comparator (sort strategy)
   • PasswordEncoder (BCrypt/PBKDF2/SCrypt)
   • AuthenticationProvider (DB/LDAP/OAuth)
   • Compression (ZIP/GZIP/BZIP2)

Interview gold:
   "4 SOLID principles in 1 pattern: OCP + SRP + DIP + LSP"
```

---

## TRAP BOX

```
Trap 1: "Strategy = State pattern"
         Different — Strategy client-swapped, State self-managed
         Strategy = behavior choice, State = lifecycle

Trap 2: "Strategy slow due to extra class"
         Negligible overhead — JIT optimizes
         Maintainability gain >> tiny perf cost

Trap 3: "Use Strategy for everything with if-else"
         Overkill for 2-3 simple cases
         Use when 3+ variants OR future expansion expected
```
