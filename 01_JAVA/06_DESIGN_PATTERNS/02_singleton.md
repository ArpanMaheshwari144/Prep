# Singleton Pattern — Sirf Ek Instance, Pure App Mein

> **Design Patterns: Topic 2 — Creational Pattern (Most-Asked Interview Classic)**

---

## STORY — President of a Country

> Imagine **ek desh mein "President" pad**:
>
> **Sirf EK President** ho sakta at any time. Multiple Presidents = chaos.
>
> Koi bhi office bole *"President se baat karni"* — **same one person** se hi connect karayega.
>
> Tu **constructor private** kar deta — koi naya President direct **`new`** se nahi bana sake. Sirf **`getPresident()`** se access — wahi instance hamesha return hota.
>
> **Singleton pattern same kaam karta** — application mein **single global instance** maintain karta.

---

## The Problem — Multiple Instances Disaster

```java
// Without Singleton — bad config management
public class Config {
    public Config() {
        loadFromDisk();      // expensive operation
        connectToServer();    // expensive operation
    }
}

// Anywhere in code:
Config c1 = new Config();   // disk + network call
Config c2 = new Config();   // disk + network call AGAIN
Config c3 = new Config();   // disk + network call AGAIN

// 3 instances, 3x resources, possibly inconsistent state
```

**Problems:**
1. **Resource waste** — same expensive init repeated
2. **Inconsistent state** — instance1 ne update kiya, instance2 ko nahi pata
3. **Memory bloat** — multiple copies of same global state
4. **Configuration drift** — different parts of app see different values

---

## Singleton Solution

```java
// Constructor PRIVATE — koi external new karna nahi sakta
public class Config {
    private static Config instance;

    private Config() { ... }   // ← PRIVATE

    public static Config getInstance() {
        if (instance == null) {
            instance = new Config();
        }
        return instance;
    }
}

// Usage:
Config c1 = Config.getInstance();   // creates
Config c2 = Config.getInstance();   // returns SAME instance
Config c3 = Config.getInstance();   // returns SAME instance

c1 == c2 == c3   // true — single instance
```

---

## VISUAL — Single Instance Guarantee

```
   First call:                    Subsequent calls:
   ───────────                    ──────────────────

   Config.getInstance()           Config.getInstance()
        │                              │
        ▼                              ▼
   ┌─────────────┐                ┌─────────────┐
   │ instance    │                │ instance    │
   │ == null?    │                │ == null?    │
   │  YES → create│                │  NO → return │
   └─────┬───────┘                └─────┬───────┘
         │ new Config()                 │
         ▼                              │
   ┌─────────────┐                      │
   │  Config     │  ◄───────────────────┘
   │  instance   │     SAME object returned
   │  created  │
   └─────────────┘
```

---

## 4 Implementation Approaches

### 1 EAGER Initialization (simplest, thread-safe)

```java
public class Config {
    // Initialize at class load — JVM thread-safe automatic
    private static final Config INSTANCE = new Config();

    private Config() { /* private */ }

    public static Config getInstance() {
        return INSTANCE;
    }
}
```

**Pros:** Thread-safe automatic (class loading)
**Cons:** Always created, even if never used (wasteful for heavy init)

---

### 2 LAZY Initialization (NOT thread-safe)

```java
public class Config {
    private static Config instance;

    private Config() { }

    public static Config getInstance() {
        if (instance == null) {        // race condition
            instance = new Config();
        }
        return instance;
    }
}
```

**Race condition:**
```
   Thread A          Thread B
   ────────          ────────
   instance == null? true
                     instance == null? true (A hasn't created yet)
   instance = new Config()
                     instance = new Config()  ← 2 instances!
```

**Pros:** Lazy (created only when needed)
**Cons:** NOT thread-safe — DON'T use in multi-threaded code

---

### 3 DOUBLE-CHECKED LOCKING (DCL) — Industry classic

```java
public class Config {
    // volatile MANDATORY — prevents instruction reordering
    private static volatile Config instance;

    private Config() { }

    public static Config getInstance() {
        if (instance == null) {                   // 1st check (no lock)
            synchronized (Config.class) {
                if (instance == null) {           // 2nd check (locked)
                    instance = new Config();
                }
            }
        }
        return instance;
    }
}
```

**Pros:**
- Thread-safe
- Lazy
- Lock acquired only on first creation (after that, just read)

**Cons:**
- Verbose
- `volatile` MUST (else half-constructed object visible)

---

### 4 ENUM Singleton — BEST APPROACH (Joshua Bloch)

```java
public enum Config {
    INSTANCE;

    private final String bankName = "Konovo Bank";
    private final double maxLimit = 100000.0;

    public String getBankName() { return bankName; }
    public double getMaxLimit() { return maxLimit; }
}

// Usage:
String name = Config.INSTANCE.getBankName();
```

**Pros:** ALL benefits at once:
- **Thread-safe automatic** (JVM enum initialization guarantee)
- **Serialization-safe** (no broken singletons via deserialization)
- **Reflection-safe** (can't instantiate enum via reflection)
- **Lazy initialization** built-in
- **Concise** — minimal code

**Joshua Bloch (Effective Java) Item 3:** *"Enum is the BEST way to implement Singleton."*

---

## PROJECT USAGE — `BankConfig.java`

**File:** `01_JAVA/07_PROJECT/SimpleBankSystem/src/com/arpan/bank/config/BankConfig.java`

```java
public enum BankConfig {
    INSTANCE;   // ← single guaranteed instance

    private final String bankName = "Arpan Bank";
    private final double maxTransactionLimit = 100000.0;
    private final String currency = "INR";

    public String getBankName() { return bankName; }
    public double getMaxTransactionLimit() { return maxTransactionLimit; }
    public String getCurrency() { return currency; }
}

// Usage anywhere in app:
String name = BankConfig.INSTANCE.getBankName();
double limit = BankConfig.INSTANCE.getMaxTransactionLimit();
```

**Why enum singleton chosen:**
- Bank-wide config — sirf ek instance pure app mein
- Thread-safe automatic — multiple services concurrently access kar sakte
- Serialization-safe — DB se restore ho ya naya banaye, same instance
- No `synchronized` boilerplate, no `volatile` worry — JVM handles all

**Interview line:**
> *"BankConfig ko enum singleton banaya — Bloch ki Effective Java mein recommended approach. Thread-safe + serialization-safe + reflection-safe automatic. Manual synchronized + getInstance() approach se simpler aur secure — mere project mein use kiya hai."*

---

## 4 Approaches Comparison

| Approach | Thread-safe? | Lazy? | Code complexity | Recommended? |
|---|---|---|---|---|
| **Eager** | Yes | No | Low | OK for light objects |
| **Lazy (no sync)** | NO | Yes | Low | NEVER for production |
| **Double-Checked Locking** | Yes (with volatile) | Yes | High | Industry classic |
| **Enum** | Yes | Yes | LOWEST | BEST (Bloch's recommendation) |

---

## Real-World Examples

### 1. `Runtime` (Java built-in)
```java
Runtime r = Runtime.getRuntime();   // Singleton — JVM ka one instance
```

### 2. **Spring Bean Default Scope = Singleton**
```java
@Service              // Spring container ek hi instance banata
@Repository           // unless @Scope("prototype") explicitly set
@Component
public class MyService { ... }
```

### 3. `Logger` instances (Log4j, SLF4J)
```java
private static final Logger log = LoggerFactory.getLogger(MyClass.class);
// Logger framework caches/reuses instances per class
```

### 4. Database connection pool managers, Cache managers, Configuration loaders

### 5. **Operating System resources** — Print spooler, file system manager

---

## When to Use vs When NOT

### Use Singleton when:
- **Configuration** that needs single source of truth
- **Connection pools** (DB, HTTP client)
- **Caches** (one cache instance)
- **Loggers**
- **Hardware resources** (printer manager, file system)

### DON'T use Singleton when:
- Plain data class (use POJO)
- Stateless utility (just static methods)
- Testing-heavy code (singletons hard to mock)
- "Convenience" reason (often anti-pattern — creates global state)

---

## Breaking Singleton — 4 attack vectors

### 1. Reflection
```java
// Hacker code:
Constructor<Config> c = Config.class.getDeclaredConstructor();
c.setAccessible(true);   // bypass private
Config rogue = c.newInstance();   // creates 2nd instance!
```
**Defense:** Throw exception in constructor if instance exists. **Enum is reflection-proof automatic.**

### 2. Serialization
```java
// Standard singleton:
Config c1 = Config.getInstance();
serialize(c1, "file.dat");
Config c2 = deserialize("file.dat");   // different instance!
```
**Defense:** Implement `readResolve()`. **Enum handles this automatically.**

### 3. Cloning
```java
public class Config implements Cloneable {
    public Object clone() { return super.clone(); }   // creates 2nd!
}
```
**Defense:** Override `clone()` to throw exception. **Enum doesn't support clone.**

### 4. Multiple ClassLoaders
Different classloaders = different `Config.class` = different singletons.
**Defense:** Specify classloader explicitly OR use enum (less affected).

**Conclusion:** **Enum Singleton bypasses ALL 4 attacks automatically** — yahin reason hai Bloch's recommendation.

---

## INTERVIEW TALKING POINT

**Q: "Singleton pattern kya hai aur kab use karte ho?"**

> *"Singleton creational pattern jo class ka **single instance** ensure karta poore application mein. Constructor private rakhke `getInstance()` method se controlled access dete. Use cases — configuration, connection pools, caches, loggers — jahaan single source of truth chahiye.
>
> Implementation 4 ways: Eager (class load pe), Lazy (sync nahi), Double-Checked Locking (industry standard), aur **Enum (Bloch's recommended best)** — thread-safe + serialization-safe + reflection-safe automatic. Mere SimpleBankSystem mein `BankConfig` enum singleton use kiya — 1 line mein production-grade."*

**Q: "DCL kyu volatile chahiye?"**

> *"Without volatile, **instruction reordering** ho sakta — JVM optimization mein constructor invocation aur reference assignment alag-alag execute. Half-constructed object dusri thread ko visible ho sakta. `volatile` happens-before guarantee deta — initialization complete hone tak no other thread sees the reference."*

**Q: "Spring beans singleton kyu default hain?"**

> *"Most beans stateless services hote (Service, Repository, Controller) — multiple instances unnecessary. Singleton resource-efficient + thread-safe (jab tak instance state mutate na ho). Stateful beans ke liye `@Scope("prototype")` use kar sakte. Spring's singleton scope **per ApplicationContext** hota — not classic 'one per JVM'."*

**Q: "Singleton anti-pattern kab banata?"**

> *"Hidden global state — testing hard (mock impossible without DI), tight coupling, breaks DIP. **Spring DI** singleton ka cleaner approach — manually `getInstance()` likhne ke bajaye container-managed singletons inject karte. So Singleton + DI = Spring's pattern. Manual Singleton sirf rare cases (BankConfig style enums)."*

---

## POWER PHRASE

> **"Singleton pattern application mein single global instance maintain karta — constructor private + static getInstance(). Best implementation Enum (Bloch's recommended) — thread-safe, serialization-safe, reflection-safe automatic. Spring beans default singleton scope use karte — manual implementation rare ho gayi modern code mein."**

---

## MEMORY HOOK

```
Singleton = "One President per country"
            • Private constructor (can't make new)
            • Static getInstance() (controlled access)
            • Same instance always returned

4 implementations:
   1. Eager     → class load pe — simple, not lazy
   2. Lazy      → on-demand — NOT thread-safe
   3. DCL       → industry standard — verbose, needs volatile
   4. Enum      → BEST (Bloch) — auto-everything

Key invariant:
   Config.getInstance() == Config.getInstance()  ← always true

Real-world (already use kar raha):
   • Spring beans (default singleton scope)
   • java.lang.Runtime
   • Loggers
   • BankConfig in SimpleBankSystem

Attack vectors (need defense):
   1. Reflection      → enum proof
   2. Serialization   → enum proof
   3. Clone           → enum proof
   4. Multi-classloader → enum mostly proof
```

---

## TRAP BOX

```
Trap 1: "Lazy singleton thread-safe hai"
         Without sync — race condition
         DCL ya enum use karo

Trap 2: "DCL mein volatile optional hai"
         Mandatory — instruction reordering protect karta
         private static volatile Config instance

Trap 3: "Singleton hamesha thread-safe behavior deta"
         Singleton instance hai, but uska state mutable ho sakta
         Internal state synchronized rakho if mutable

Trap 4: "Spring bean = classic Singleton"
         Spring singleton per ApplicationContext, classic per JVM
         Concept similar but scope different

Trap 5: "Enum singleton mein constructor nahi"
         Enum mein constructor LIKH SAKTE — sirf private (default)
         enum INSTANCE; + private constructor + methods OK

Trap 6: "Singleton testing easy hai"
         Hard to mock — DI not built-in
         Spring's container singletons better — mockable via @MockBean
```

---

## Singleton vs Static Methods

| | Singleton | Static class |
|---|---|---|
| **State** | Can hold instance state | Only class-level (static) |
| **Polymorphism** | Can implement interfaces | No interfaces |
| **Lazy init** | Possible | Loaded with class |
| **Mock for testing** | Hard | Very hard |
| **Inheritance** | Possible | No |
| **Use case** | Stateful global resource | Pure utility (Math.random()) |

---

## Quick Recall Summary

```
WHAT     → Single instance per application
WHY      → Single source of truth, resource control
HOW      → Private constructor + static getInstance()
BEST     → Enum (Bloch's recommendation)
EXAMPLES → Runtime, Spring beans, BankConfig (SimpleBank project)
TRAP     → Reflection/Serialization breaks (enum-safe)
ANTI-PATTERN → When used for "global convenience" — use DI instead
```
