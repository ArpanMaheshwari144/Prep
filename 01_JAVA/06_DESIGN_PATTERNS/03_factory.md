# 🏭 Factory Pattern — Object Creation Encapsulate

> **Design Patterns: Topic 3 — Creational Pattern (Interview Classic)**

---

## 🎬 STORY — Pizza Counter

> Imagine **pizza shop pe order karna**:
>
> 🍕 **Without Factory:**
> Tu **khud kitchen mein jaake** dough banaye, sauce lagaye, oven start kare, cheese melt kare. **Caller tightly coupled** to internal kitchen process.
>
> 🍕 **With Factory:**
> Tu counter pe bolta — *"Margherita pizza"*. Counter wala **decide karta** kaunsa chef, kaunsa oven, kya recipe. Tu sirf **"type bola, pizza mila"** — internal kitchen logic hidden.
>
> **Factory pattern same kaam karta** — caller bolta "type", factory **decide karta** kaunsa concrete class instantiate karna.

---

## 🤔 The Problem — Tightly Coupled Object Creation

```java
// Without Factory — caller knows ALL concrete classes
public class BankApp {
    public Account openAccount(String type, String id, double balance) {
        if (type.equals("savings")) {
            return new SavingsAccount(id, balance);          // ← concrete class
        } else if (type.equals("current")) {
            return new CurrentAccount(id, balance);          // ← concrete class
        } else if (type.equals("fd")) {
            return new FixedDeposit(id, balance);            // ← concrete class
        }
        throw new IllegalArgumentException();
    }
}
```

**Problems:**
1. 🔴 **Caller knows all concrete classes** — coupling everywhere
2. 🔴 **Naya account type add** = caller modify (OCP violation)
3. 🔴 **Same logic duplicated** wherever account creation needed
4. 🔴 **Testing hard** — concrete classes everywhere

---

## ✨ Factory Solution

```java
public class AccountFactory {
    public static Account create(String type, String id, double balance) {
        switch (type.toLowerCase()) {
            case "savings": return new SavingsAccount(id, balance);
            case "current": return new CurrentAccount(id, balance);
            case "fd":      return new FixedDeposit(id, balance);
            default:        throw new IllegalArgumentException("Unknown: " + type);
        }
    }
}

// Caller (clean):
Account a = AccountFactory.create("savings", "A101", 5000);
//          ↑ caller doesn't know SavingsAccount exists
```

**Benefits:**
- ✅ **Caller decoupled** from concrete classes
- ✅ **Single place** to add new types
- ✅ **DRY** — creation logic ek jagah
- ✅ **Easy to test** (mock factory possible)

---

## 🎨 VISUAL — Factory ka Flow

```
   CALLER                          FACTORY                    CONCRETE
   ───────                          ─────────                  ─────────
   
   AccountFactory.create("savings", "A1", 5000)
        │
        ├──► factory.create() called
        │           │
        │           │ switch on type
        │           │
        │           ├──► "savings" → new SavingsAccount(...)
        │           │
        │           ├──► "current" → new CurrentAccount(...)
        │           │
        │           └──► "fd"      → new FixedDeposit(...)
        │
        ◄──── Account ref returned
        
   Caller gets Account (interface) — concrete class hidden inside factory
```

---

## 💻 PART 1: Simple Static Factory (most common)

```java
public class AccountFactory {
    
    public static Account create(String type, String id, double balance) {
        switch (type.toLowerCase()) {
            case "savings":
                return new SavingsAccount(id, balance);
            case "current":
                return new CurrentAccount(id, balance);
            case "fd":
                return new FixedDeposit(id, balance);
            default:
                throw new IllegalArgumentException("Unknown account type: " + type);
        }
    }
}
```

**Pros:** Simple, easy to understand
**Cons:** Static method — hard to mock for testing, no polymorphism

---

## 💻 PART 2: Map-based Registry (extensible)

```java
public class AccountFactory {
    
    // Registry — type → supplier
    private static final Map<String, Function<AccountParams, Account>> REGISTRY = new HashMap<>();
    
    static {
        REGISTRY.put("savings", p -> new SavingsAccount(p.id, p.balance));
        REGISTRY.put("current", p -> new CurrentAccount(p.id, p.balance));
        REGISTRY.put("fd",      p -> new FixedDeposit(p.id, p.balance));
    }
    
    public static Account create(String type, AccountParams params) {
        Function<AccountParams, Account> creator = REGISTRY.get(type.toLowerCase());
        if (creator == null) {
            throw new IllegalArgumentException("Unknown type: " + type);
        }
        return creator.apply(params);
    }
    
    // Add new type at runtime — true OCP!
    public static void register(String type, Function<AccountParams, Account> creator) {
        REGISTRY.put(type, creator);
    }
}

// Naye type add karna ho? Existing code untouched:
AccountFactory.register("crypto", p -> new CryptoAccount(p.id, p.balance));
```

**Pros:** Truly OCP-compliant, runtime extensible
**Cons:** Slightly more complex

---

## 🚀 PROJECT USAGE — `AccountFactory.java`

**File:** `01_JAVA/07_PROJECT/SimpleBankSystem/src/com/arpan/bank/factory/AccountFactory.java`

```java
public class AccountFactory {

    // static method — instance ki zarurat nahi
    public static Account create(String type, String id, String name, double balance) {
        switch (type.toLowerCase()) {
            case "savings":
                return new SavingsAccount(id, name, balance);
            case "current":
                return new CurrentAccount(id, name, balance);
            case "fd":
                return new FixedDeposit(id, name, balance);
            default:
                throw new IllegalArgumentException("Unknown account type: " + type);
        }
    }
}
```

**Caller (Main.java):**
```java
Account a1 = AccountFactory.create("savings", "S101", "Arpan", 5000);
Account a2 = AccountFactory.create("current", "C201", "Rahul", 10000);
Account a3 = AccountFactory.create("fd",      "F301", "Priya", 50000);
```

**Notice:**
- Caller **`new SavingsAccount(...)` directly nahi karta**
- Sirf type string deta — factory decide karta
- Naya type add karna ho? Sirf factory mein switch case add → caller untouched

**🎤 Interview line:**
> *"AccountFactory pattern implement karta SimpleBankSystem mein — caller `new SavingsAccount()` directly nahi karta, factory ke through banata. Naye types add karne mein flexibility — sirf factory mein switch case add karna padta. Real-world Spring BeanFactory same concept use karta automated way mein."*

---

## 🌍 Real-World Examples

### 1. **Java built-in `Calendar.getInstance()`**
```java
Calendar cal = Calendar.getInstance();   // returns GregorianCalendar / BuddhistCalendar based on locale
```

### 2. **`NumberFormat.getInstance()`**
```java
NumberFormat fmt = NumberFormat.getCurrencyInstance(Locale.US);
// returns DecimalFormat with US currency settings
```

### 3. **Spring `BeanFactory` / `ApplicationContext`**
```java
ApplicationContext ctx = new AnnotationConfigApplicationContext(Config.class);
UserService service = ctx.getBean(UserService.class);
// Spring decides which concrete class, manages lifecycle
```

### 4. **`ExecutorService` factories**
```java
ExecutorService pool = Executors.newFixedThreadPool(5);   // factory method
ExecutorService cached = Executors.newCachedThreadPool();  // different impl
```

### 5. **Database connection factories**
```java
Connection conn = DriverManager.getConnection(url, user, pass);
// returns MySQL/PostgreSQL/Oracle connection based on URL
```

---

## 📊 3 Factory Variants (interview gold)

### 1. Simple Factory (humara approach)
Static method with switch — most common in real code.

### 2. Factory Method (GoF pattern)
Abstract class with abstract `create()` — subclasses override.

```java
abstract class Dialog {
    abstract Button createButton();   // factory method
    
    void render() {
        Button b = createButton();    // calls subclass's version
        b.draw();
    }
}

class WindowsDialog extends Dialog {
    Button createButton() { return new WindowsButton(); }
}

class MacDialog extends Dialog {
    Button createButton() { return new MacButton(); }
}
```

### 3. Abstract Factory (family of related products)
```java
interface UIFactory {
    Button createButton();
    Checkbox createCheckbox();
    Dialog createDialog();
}

class WindowsUIFactory implements UIFactory {
    public Button createButton()     { return new WindowsButton(); }
    public Checkbox createCheckbox() { return new WindowsCheckbox(); }
    public Dialog createDialog()     { return new WindowsDialog(); }
}
```

**Difference:**
| Pattern | What it creates |
|---|---|
| **Simple Factory** | Single object based on input (Account types) |
| **Factory Method** | Single object via subclass override (Dialog → Button) |
| **Abstract Factory** | Family of related objects (Windows: Button + Checkbox + Dialog) |

---

## 🎯 When to Use Factory?

| Use Factory when... | Don't use when... |
|---|---|
| Multiple related types share common interface | Single class, no variants |
| Object creation logic complex | Simple `new` works fine |
| Caller shouldn't know concrete classes | Caller intentionally manages instances |
| Dynamic type selection (config-driven) | Static known type |
| Testing requires substituting types | No substitution needed |

---

## 💡 Key Design Principles

1. **Static method or instance** — both valid (static common, instance more flexible)
2. **Return abstract type** — `Account`, not `SavingsAccount` (interface decoupling)
3. **Throw on unknown type** — `IllegalArgumentException` better than null
4. **Single source of truth** — all creation logic ek jagah
5. **Caller decoupled** — caller `new` nahi karta concrete class

---

## 🎤 INTERVIEW TALKING POINT

**Q: "Factory pattern kya hai aur kab use karte ho?"**

> *"Factory creational pattern jo **object creation logic encapsulate** karta. Caller bolta 'mujhe SAVINGS chahiye' — factory decide karta kaunsa concrete class instantiate karna. Tightly coupled `new SavingsAccount()` calls remove ho jate caller side se.
>
> Mere SimpleBankSystem mein **AccountFactory** banaya — `create("savings", ...)` se SavingsAccount, CurrentAccount, FixedDeposit return karta. Caller `Account` (abstract type) pe depend karta, concrete classes hidden. **OCP + DIP** dono follow karta — naya account type add karne mein factory mein switch case add bus, caller code unchanged. Real-world Spring BeanFactory same concept hai automated."*

**Q: "3 factory variants mein difference?"**

> *"3 patterns:
> 1. **Simple Factory** — static method with switch (most common, mera AccountFactory)
> 2. **Factory Method (GoF)** — abstract method, subclass override karte (e.g., Dialog → createButton)
> 3. **Abstract Factory** — family of related products (Windows UI: Button + Checkbox + Dialog ek consistent set)
>
> Real production code mein 80% Simple Factory, 15% Abstract Factory (cross-platform UI/themes), 5% pure Factory Method."*

**Q: "Static factory method advantages over constructor?"**

> *"4 main:
> 1. **Naam de sakte** — `BigInteger.probablePrime()` clearer than `new BigInteger(...)` with 10 args
> 2. **Caching possible** — `Boolean.valueOf()` returns cached instances (TRUE/FALSE)
> 3. **Subtype return** — `Calendar.getInstance()` returns specific subclass based on locale
> 4. **Reduces constructor count** — multiple factory methods cleaner than telescoping constructors
>
> Joshua Bloch (Effective Java Item 1) prefers static factory methods over constructors in many cases."*

**Q: "Spring mein Factory pattern kaha hai?"**

> *"Bahut jagah:
> - **BeanFactory** — Spring container itself ek factory hai
> - **`@Bean` methods** — har @Bean factory method, return type subclass
> - **`FactoryBean<T>`** interface — custom complex bean creation
> - **Spring Data JPA** — Repository proxies factory-generated runtime
>
> Hum project mein manual Factory banaya understanding ke liye — Spring same concept automate karta production mein."*

---

## 💎 POWER PHRASE

> **"Factory pattern object creation logic encapsulate karta — caller `new` directly nahi karta, factory decide karta kaunsa concrete class. SimpleBankSystem mein AccountFactory implement kiya — OCP + DIP follow. Spring BeanFactory same concept automate karta production mein."**

---

## 🧠 MEMORY HOOK

```
Factory = "Pizza counter"
   • Caller bolta type ("Margherita")
   • Counter (factory) decide karta — kaunsi recipe, oven, chef
   • Caller ko kitchen ka pata nahi
   
Code structure:
   1. Caller calls factory.create(type)
   2. Factory switch on type
   3. Returns concrete instance via abstract type
   4. Caller decoupled from concrete classes

3 variants:
   Simple Factory   → static method + switch (mera approach)
   Factory Method   → abstract method, subclass override
   Abstract Factory → family of related products

Real-world (already use kiya):
   • Calendar.getInstance()
   • Executors.newFixedThreadPool()
   • Spring BeanFactory
   • DriverManager.getConnection()
   • AccountFactory in SimpleBankSystem ✅

Use when:
   ✓ Multiple related types
   ✓ Creation logic complex
   ✓ Caller decoupling needed
   ✓ Dynamic type selection
```

---

## ⚠️ TRAP BOX

```
🪤 Trap 1: "Factory must be static"
         ❌ Both static and instance valid
         ✅ Static common, instance more testable

🪤 Trap 2: "Factory and Singleton same"
         ❌ Different — Factory creates, Singleton ensures one instance
         ✅ Combined sometimes (single factory instance)

🪤 Trap 3: "Switch case OCP violation"
         ❌ Strictly yes — but acceptable trade-off for simplicity
         ✅ For true OCP — Map registry approach

🪤 Trap 4: "Factory returns concrete class"
         ❌ Defeats purpose
         ✅ Always return abstract / interface type

🪤 Trap 5: "Factory Method = Simple Factory"
         ❌ Different — Method is GoF pattern with abstract creator
         ✅ Simple Factory is a static utility method
```

---

## 🎯 Quick Recall Summary

```
WHAT     → Object creation encapsulation
WHY      → Decouple caller from concrete classes + DRY creation logic
HOW      → Factory class with create() method that switches on type
BEST     → Simple Factory for 80% cases, Map registry for true OCP
EXAMPLES → AccountFactory (project), Calendar.getInstance(), Spring BeanFactory
TRAP     → Don't return concrete type — always abstract/interface
```
