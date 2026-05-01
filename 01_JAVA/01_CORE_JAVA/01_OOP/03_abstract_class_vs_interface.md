# Abstract Class vs Interface

> **V90 Section 1 — Topic 3 (Interview MUST-KNOW)**

---

## 🎬 STORY — IS-A vs CAN-DO

> Imagine **2 different conversations** about your dog:
>
> 🐕 **"Mera Dog ek ANIMAL hai"** → **IS-A** relationship → **Abstract Class** (`Dog extends Animal`)
>
> 🐕 **"Mera Dog DRIVING kar sakta?"** → **CAN-DO** capability → **Interface** (`Dog implements Drivable`?)
>
> *"What it IS"* (identity) → **abstract class**
> *"What it CAN DO"* (capability) → **interface**
>
> Yeh distinction interview mein 80% candidates mix kar dete.

---

## 🎨 IS-A vs CAN-DO Visual

```
   IS-A (Abstract Class)          CAN-DO (Interface)
   ─────────────────────          ──────────────────
   
        Animal (abstract)          Swimmable (interface)
       /   |   \                    │
     Dog  Cat  Bird                 │ implements
                                    ▼
   Dog IS-A Animal              Dog        Fish        Duck
   (identity / classification)  Cat        Frog        Penguin
                                
                                These CAN swim
                                (capability / behavior)
                                regardless of identity
```

**Key insight:** Dog IS-A Animal (one parent identity), but **Dog CAN-DO Swim, CAN-DO Eat, CAN-DO Bark** (many capabilities).

---

## 💻 CODE EXAMPLE — Notification System

### Problem
Email, SMS, Push notifications — sab **send** karte but with **shared retry logic**.

### Abstract Class — Shared Implementation

```java
public abstract class Notification {
    
    // ─── Shared state ──────────────────
    protected String recipient;
    protected int retryCount = 0;
    protected static final int MAX_RETRIES = 3;
    
    // ─── Shared concrete method ───────
    public final void sendWithRetry(String message) {
        while (retryCount < MAX_RETRIES) {
            try {
                send(message);
                log("Sent successfully");
                return;
            } catch (Exception e) {
                retryCount++;
                log("Retry " + retryCount);
            }
        }
    }
    
    // ─── Subclass MUST implement ──────
    protected abstract void send(String message);
    
    // ─── Shared helper ────────────────
    protected void log(String msg) {
        System.out.println("[" + this.getClass().getSimpleName() + "] " + msg);
    }
}

public class EmailNotification extends Notification {
    @Override
    protected void send(String message) {
        // SMTP-specific logic
    }
}

public class SMSNotification extends Notification {
    @Override
    protected void send(String message) {
        // Twilio-specific logic
    }
}
```

### Interface — Pure Contract

```java
public interface Approvable {
    void approve();
    void reject();
}

// Some notifications ARE Approvable, some not
public class HighValueTransferNotification extends Notification implements Approvable {
    @Override protected void send(String msg) { /* ... */ }
    @Override public void approve()           { /* ... */ }
    @Override public void reject()            { /* ... */ }
}

public class WelcomeEmailNotification extends Notification {
    // Doesn't need approval — just extends Notification
}
```

---

## 📊 FULL COMPARISON TABLE

| Feature | Abstract Class | Interface |
|---|---|---|
| **Purpose** | Shared partial implementation | Capability contract |
| **Relationship** | IS-A (identity) | CAN-DO (behavior) |
| **Inheritance** | Single (1 parent only) | Multiple (multiple impl OK) |
| **State (fields)** | ✅ Any type, instance fields | ❌ Only `public static final` (constants) |
| **Constructor** | ✅ Yes (via `super()`) | ❌ No |
| **Methods** | abstract + concrete + static | abstract + default (Java 8+) + static + private (Java 9+) |
| **Access modifiers** | public/protected/private/default | Public only (effectively) |
| **`final` allowed?** | ✅ Yes (prevents override) | ❌ Methods can't be final (defeats purpose) |
| **Object creation** | ❌ Can't instantiate directly | ❌ Can't instantiate |
| **Use case** | Animal, Vehicle, Notification | Comparable, Runnable, Iterable |

---

## 🎯 WHEN TO USE WHICH — Decision Tree

```
                    ┌──────────────────────────┐
                    │  Naya class banana hai   │
                    └────────────┬─────────────┘
                                 │
                    ┌────────────┴──────────────┐
                    │                           │
            "Shared code +                "Pure capability
             state hai?"                   contract?"
                    │                           │
                    ▼                           ▼
            ABSTRACT CLASS                  INTERFACE
            
   Examples:                          Examples:
   • Animal (eat, sleep)              • Comparable (compareTo)
   • Vehicle (start, stop)            • Runnable (run)
   • DatabaseConnection (open, close) • Serializable (marker)
   • HttpRequest (parse, validate)    • Iterator (next, hasNext)
```

---

## 🆕 Java 8+ Interface Features (interview gold)

### Default methods
```java
public interface Vehicle {
    void start();
    
    // Default method — has implementation
    default void honk() {
        System.out.println("Beep beep!");
    }
}

public class Car implements Vehicle {
    @Override
    public void start() { /* ... */ }
    // honk() inherited automatically
}
```

### Static methods
```java
public interface Calculator {
    int calculate(int a, int b);
    
    static Calculator addition() {
        return (a, b) -> a + b;
    }
}

Calculator add = Calculator.addition();
add.calculate(5, 3);   // 8
```

### Private methods (Java 9+)
```java
public interface Logger {
    default void info(String msg)  { log("INFO", msg); }
    default void error(String msg) { log("ERROR", msg); }
    
    private void log(String level, String msg) {   // private helper
        System.out.println(level + ": " + msg);
    }
}
```

---

## 🪤 INTERVIEW TRAPS

### Trap 1: Abstract class can have constructors

```java
public abstract class Animal {
    private String name;
    
    public Animal(String name) {     // ✅ Yes!
        this.name = name;
    }
}

public class Dog extends Animal {
    public Dog(String name) {
        super(name);                 // Must call parent
    }
}
```

**Interface cannot have constructor.** Tu sirf default method de sakta initialization-like behavior ke liye.

### Trap 2: Interface variables auto-public-static-final

```java
public interface Constants {
    int MAX_SIZE = 100;   // implicitly public static final
    // No need to write public static final
}
```

### Trap 3: Multiple inheritance via interfaces

```java
public interface Walkable { void walk(); }
public interface Swimmable { void swim(); }

public class Duck implements Walkable, Swimmable {  // ✅ Multiple OK
    public void walk() { /* */ }
    public void swim() { /* */ }
}
```

### Trap 4: Diamond problem with default methods

```java
public interface A {
    default void hello() { System.out.println("A"); }
}

public interface B {
    default void hello() { System.out.println("B"); }
}

public class C implements A, B {
    // ❌ COMPILE ERROR — must override
    @Override
    public void hello() {
        A.super.hello();   // explicitly choose
    }
}
```

---

## 🎤 INTERVIEW TALKING POINT

**Q: "Abstract class vs Interface — kab konsa use karoge?"**

> *"Decision driven by 2 questions:
>
> 1. **IS-A ya CAN-DO?**
>    - 'Dog IS-A Animal' (identity) → Abstract class (Animal)
>    - 'Dog CAN swim' (capability) → Interface (Swimmable)
>
> 2. **Shared state chahiye?**
>    - Yes (fields, constructor) → Abstract class
>    - No (pure contract) → Interface
>
> Ek class **single abstract** extend kar sakti, **multiple interfaces** implement. Modern Java mein interface bhi default methods de sakti — but **state sirf abstract class** mein possible. **Notification system pattern** mein both use karte — `Notification` abstract class shared retry logic ke liye, `Approvable` interface jo specific notifications implement karte."*

**Q: "Java 8 ke baad interface aur abstract class mein difference reduce ho gaya?"**

> *"Default methods se gap kam hua, but **fundamental difference state** hai. Interface mein instance fields nahi rakh sakte (sirf constants). Abstract class mein full state — fields, constructor, instance variables. Plus single inheritance limit abstract class pe lagta hai, interfaces multiple. Strategic distinction baki hai — purpose alag."*

**Q: "Diamond problem Java mein hota?"**

> *"Classes mein NAHI — Java single class inheritance allow karta. Interfaces mein default methods Java 8+ ke baad **possible** ho sakta agar 2 interfaces same default method de — compile error, override mandatory. `A.super.method()` syntax se explicitly choose karte."*

---

## 💎 POWER PHRASE

> **"Abstract class IS-A relationship + shared state ke liye (Animal, Vehicle). Interface CAN-DO capability + multiple inheritance ke liye (Comparable, Runnable). Ek class single abstract extend, multiple interface implement. Java 8+ default methods se interface bhi shared behavior de sakti — but state sirf abstract class mein."**

---

## 🧠 MEMORY HOOK

```
Abstract Class  → "What it IS"      → IS-A relationship
                                      Has STATE (fields, constructor)
                                      SINGLE inheritance
                                      Examples: Animal, Vehicle, Shape

Interface       → "What it CAN DO"   → CAN-DO capability
                                      NO STATE (only constants)
                                      MULTIPLE inheritance
                                      Examples: Comparable, Runnable, Iterable

Decision:
   Need shared code + state?      → Abstract class
   Pure capability contract?      → Interface
   Both?                          → Both! (Class extends abstract + implements interfaces)
```

---

## ⚠️ TRAP BOX

```
🪤 Trap 1: "Interface mein constructor hota"
         ❌ NO — interface mein constructor IMPOSSIBLE
         ✅ Abstract class mein hota

🪤 Trap 2: "Java multiple class inheritance support karta"
         ❌ NO — single class only
         ✅ Multiple interfaces OK

🪤 Trap 3: "Interface mein instance fields rakh sakte"
         ❌ Sirf public static final (constants)
         ✅ Abstract class mein any field type

🪤 Trap 4: "Abstract class instantiate kar sakte"
         ❌ NO — sirf concrete subclass se
         ✅ Anonymous class trick available though

🪤 Trap 5: "Default method = abstract class"
         ❌ Different — interface mein state nahi possible
         ✅ Default methods sirf shared behavior, not state
```
