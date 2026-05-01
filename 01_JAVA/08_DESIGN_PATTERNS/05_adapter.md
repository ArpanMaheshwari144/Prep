# 🔌 Adapter Pattern — Incompatible Interfaces ko Connect

> **Design Patterns: Topic 5 — Structural Pattern**

---

## 🎬 STORY — Travel Plug Adapter

> Tu USA gaya — laptop charger ka pin **Indian** type hai (round), socket **American** (flat).
>
> 🔌 **Direct fit nahi** — interface incompatible.
>
> 💡 **Solution:** Travel adapter — ek side Indian pin lene wala, doosri side American socket compatible. Physical wire same, bas **interface translate** karta.
>
> **Adapter pattern same kaam karta** — 2 incompatible interfaces ke beech **bridge** banata.

---

## 🤔 The Problem

Imagine **legacy logger** (LegacyLogger) hai — `logMessage(String)` method.
Naya code **modern logger interface** (Logger) expect karta — `info(String)` method.

```java
// Legacy class — modify nahi kar sakte
public class LegacyLogger {
    public void logMessage(String msg) {
        System.out.println("LEGACY LOG: " + msg);
    }
}

// Modern interface — naya code yahin pe based
public interface Logger {
    void info(String message);
}

// Naya code:
public class UserService {
    private Logger logger;   // expects Logger interface
    
    public void doWork() {
        logger.info("Working...");   // ❌ LegacyLogger doesn't fit
    }
}
```

**Direct fit possible nahi** — LegacyLogger source code modify nahi kar sakte.

---

## ✨ Adapter Solution

```java
// Adapter — wraps Legacy, exposes Modern interface
public class LoggerAdapter implements Logger {
    private final LegacyLogger legacy;
    
    public LoggerAdapter(LegacyLogger legacy) {
        this.legacy = legacy;
    }
    
    @Override
    public void info(String message) {
        legacy.logMessage(message);   // delegate to legacy
    }
}

// Usage:
LegacyLogger old = new LegacyLogger();
Logger modern = new LoggerAdapter(old);   // adapt!
userService.setLogger(modern);
```

**Now LegacyLogger fits modern Logger interface** — no source modification needed.

---

## 🎨 VISUAL

```
   CLIENT                  ADAPTER              ADAPTEE (legacy)
   ──────                  ──────────           ────────────────
   
   Logger logger;          LoggerAdapter        LegacyLogger
        │                  implements Logger    
        │                       │                       
        │ logger.info(msg)      │ adapts                
        │ ───────────────────►  │                       
        │                       │ legacy.logMessage(msg)
        │                       │ ──────────────────►   
        │                       │                       │ prints
        │ ◄────────────────────  │ ◄─────────────────── │
```

---

## 🌍 Real-World Examples

### 1. **Java I/O — `InputStreamReader`**
```java
InputStream byteStream = ...;        // bytes
Reader charReader = new InputStreamReader(byteStream);   // ADAPTER → chars
// Adapts InputStream (bytes) to Reader (chars)
```

### 2. **`Arrays.asList()` — array → List**
```java
String[] arr = {"a", "b", "c"};
List<String> list = Arrays.asList(arr);   // adapter
```

### 3. **Spring's `HandlerAdapter`** — different controller types → unified Spring MVC interface

---

## 🚀 INTERVIEW USE CASE

**Common scenario:** Third-party library integration.

```java
// You're using Stripe payment library
public interface PaymentProcessor {
    boolean charge(double amount);
}

// Stripe's class (third-party — can't modify):
public class StripeAPI {
    public StripeResponse processPayment(double amt, String currency) { ... }
}

// Your adapter:
public class StripeAdapter implements PaymentProcessor {
    private final StripeAPI stripe = new StripeAPI();
    
    @Override
    public boolean charge(double amount) {
        StripeResponse res = stripe.processPayment(amount, "USD");
        return res.isSuccess();
    }
}
```

**Result:** Stripe ka API tera `PaymentProcessor` contract follow karta — bina library modify kiye.

---

## 📐 SOLID Connection

- **OCP** — Adapter naya class hai, existing code unchanged
- **DIP** — Client adapter ke through abstract interface use karta, concrete legacy class nahi
- **SRP** — Adapter ka single role: interface translation

---

## 🎤 INTERVIEW TALKING POINT

**Q: "Adapter pattern kya hai aur kab use karte ho?"**

> *"Adapter structural pattern jo **2 incompatible interfaces ko bridge** karta. Ek class doosre interface ke contract ke according present karta — translation layer ki tarah. Travel plug analogy — Indian pin laptop, American socket → adapter beech mein.
>
> Real-world use cases: legacy code integration (purana logger naye Logger interface mein), third-party library wrapping (Stripe API → apna PaymentProcessor), Java I/O `InputStreamReader` (bytes → chars). Modern code ko legacy/external systems se decouple karne mein gold."*

**Q: "Adapter vs Decorator difference?"**

> *"Both wrap existing class but **purpose alag**:
> - **Adapter** = interface convert karta (incompatible → compatible)
> - **Decorator** = same interface rakhke behavior add karta (extra functionality)
>
> Adapter = translator. Decorator = enhancer."*

---

## 💎 POWER PHRASE

> **"Adapter pattern incompatible interfaces ke beech bridge banata — wraps existing class aur target interface ke according expose karta. Legacy code integration, third-party library wrapping ke liye gold."**

---

## 🧠 MEMORY HOOK

```
Adapter = "Travel plug"
   • Indian pin (existing class)
   • American socket (target interface)
   • Adapter beech mein — translate karta

Code structure:
   1. Target interface (kya chahiye client ko)
   2. Adaptee class (jo hai already, can't modify)
   3. Adapter implements Target, wraps Adaptee
   4. Client uses Target interface — sees adapted version

Real-world:
   • InputStreamReader (bytes → chars)
   • Arrays.asList() (array → List)
   • Spring HandlerAdapter
   • Third-party library wrappers
```

---

## ⚠️ TRAP BOX

```
🪤 Trap 1: "Adapter = Decorator"
         ❌ Adapter changes interface, Decorator keeps same
         ✅ Different intent

🪤 Trap 2: "Adapter modifies adaptee"
         ❌ NEVER — adaptee unchanged (composition only)
         ✅ Adapter wraps + delegates

🪤 Trap 3: "Use Adapter for everything"
         ❌ Only when interface incompatibility exists
         ✅ Direct call if interfaces match
```
