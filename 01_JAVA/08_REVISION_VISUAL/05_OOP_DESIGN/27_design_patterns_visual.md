# 🎨 Design Patterns — Visual Revision

---

## 1️⃣ BUILDER — Step by Step Object Creation

### Problem Pehle

```
Imagine User class banana:

   class User {
      String name;
      String email;
      String phone;
      int age;
      String role;
      String address;
      boolean isActive;
   }
```

```java
// Constructor banana ho with all:
new User("Arpan", "a@gmail.com", "9999", 27,
        "USER", "Bangalore", true);
```

```
Problems:
   ❌ 7 params — order yaad rakhna mushkil
   ❌ Kuch optional hain (phone null bhi ho sakta)
   ❌ Constructor mein 7 versions banane padenge
        new User(name)
        new User(name, email)
        new User(name, email, phone)
        ...
   = MESS
```

---

### Pizza Analogy

```
Tu pizza order karta hai —
   sab toppings ek saath nahi bolta
   step by step kahta:
      "size medium"
      "cheese extra"
      "pepperoni daalo"
      "ab bana do"
```

---

### Builder Solution

```java
class User {
    String name;
    String email;
    String phone;
    int age;
    String role;
    
    // Private constructor — only Builder banae
    private User(Builder b) {
        this.name = b.name;
        this.email = b.email;
        this.phone = b.phone;
        this.age = b.age;
        this.role = b.role;
    }
    
    public static class Builder {
        String name;
        String email;
        String phone;
        int age;
        String role = "USER";   // default
        
        public Builder name(String n) {
            this.name = n;
            return this;        // ← KEY: return Builder for chain
        }
        public Builder email(String e) {
            this.email = e;
            return this;
        }
        public Builder phone(String p) {
            this.phone = p;
            return this;
        }
        public Builder age(int a) {
            this.age = a;
            return this;
        }
        public Builder role(String r) {
            this.role = r;
            return this;
        }
        
        public User build() {
            return new User(this);   // ← creates actual User
        }
    }
}
```

---

### Usage Visual

```java
User user = new User.Builder()
                    .name("Arpan")        // returns Builder
                    .email("a@gmail.com") // returns Builder
                    .age(27)              // returns Builder
                    .build();             // FINALLY returns User
```

```
Flow:
   new Builder()    → empty Builder
        .name(x)    → Builder with name
        .email(x)   → Builder with name + email
        .age(x)     → Builder with name + email + age
        .build()    → User object created
```

---

### Magic = `return this;`

```
Har setter method Builder return karta
Iss reason se .name().email().age() chain hota
Last mein .build() final User banake deta

= FLUENT API
```

---

### WITHOUT `return this` — Kya Hoga?

```java
public Builder name(String n) {
    this.name = n;
    // no return → void
}

// ❌ Chain TOOT jata:
new User.Builder()
        .name("Arpan")      // returns void!
        .email("...")        // compile error
        .build();
```

```
WITHOUT return this:
   Builder b = new Builder();
   b.name("Arpan");
   b.email("a@gmail");
   User u = b.build();
   = Multi-line manual, ugly

WITH return this:
   ONE-line chain
   = Builder ka asli magic
```

---

### Lombok Shortcut (Real World)

```java
@Builder
class User {
    String name;
    String email;
    int age;
}

// Lombok ne automatically Builder class generate ki
User u = User.builder()
             .name("Arpan")
             .email("a@gmail.com")
             .age(27)
             .build();
```

```
Production:
   Manually Builder class nahi likhta
   @Builder annotation = ban jata
```

---

### Real Examples Around You

```java
// JWT token banana
Jwts.builder()
    .subject(userId)
    .issuedAt(now)
    .expiration(expiry)
    .signWith(key)
    .compact();

// HTTP request
HttpRequest.newBuilder()
           .uri(URI.create("..."))
           .GET()
           .build();

// Stream collecting
Stream.builder()
      .add("a")
      .add("b")
      .build();
```

---

### When Use Builder?

```
✅ Class mein > 4 parameters
✅ Some optional fields
✅ Immutable object banana hai
✅ Readable construction chahiye

❌ 2-3 params ki simple class
❌ Java Record (already concise)
```

---

### Memory Hook

```
Builder = 🍕 Pizza step-by-step

Constructor mein 10 params NAHI
   .field().field().field().build()
   
"return this" = chain ka secret
```

---

## 2️⃣ SINGLETON — Only ONE Instance

### Problem Pehle

```
Imagine DB connection:

   class DatabaseConnection {
      // costly setup (10 sec connect)
      // pool of 50 connections
   }
```

```java
// Without Singleton — har koi naya banaye:
DatabaseConnection db1 = new DatabaseConnection();  // 10 sec
DatabaseConnection db2 = new DatabaseConnection();  // 10 sec
DatabaseConnection db3 = new DatabaseConnection();  // 10 sec
```

```
Problems:
   ❌ Memory waste
   ❌ Database connections duplicate
   ❌ Connection pool exhaust
   ❌ Inconsistent state
```

---

### President Analogy

```
Country mein PRESIDENT sirf EK
   2 presidents ban nahi sakte
   Sab unhi ko reference karte
   
   = SINGLETON
```

---

### Simple Approach

```java
public class DatabaseConnection {
    
    // 1. STATIC instance (class-level)
    private static DatabaseConnection instance;
    
    // 2. PRIVATE constructor (bahar ka 'new' block kare)
    private DatabaseConnection() {
        // costly setup
    }
    
    // 3. Public method to get instance
    public static DatabaseConnection getInstance() {
        if (instance == null) {
            instance = new DatabaseConnection();
        }
        return instance;
    }
}
```

### Usage

```java
DatabaseConnection db1 = DatabaseConnection.getInstance();
DatabaseConnection db2 = DatabaseConnection.getInstance();

db1 == db2;   // TRUE (same object)

// 'new' nahi kar sakte:
new DatabaseConnection();   // ❌ compile error (private)
```

---

### Visual Flow

```
   First call:
   getInstance()
        │
        ▼
   instance == null? → YES
        │
        ▼
   instance = new DatabaseConnection()  ← created here
        │
        ▼
   return instance

   Second/Third call:
   getInstance()
        │
        ▼
   instance == null? → NO
        │
        ▼
   return EXISTING instance
```

---

### Thread Issue (Simple Approach Problem)

```
Thread A:                Thread B:
   getInstance()            getInstance()
   instance == null?        instance == null?
   YES                      YES                ← BOTH check SAME time!
   create instance          create instance    ← BOTH create
   
   = 2 instances created
   = Singleton BROKEN
```

---

### BEST Approach — Enum (Joshua Bloch)

```java
public enum DatabaseConnection {
    INSTANCE;
    
    public void query(String sql) {
        // ...
    }
}
```

### Usage

```java
DatabaseConnection.INSTANCE.query("SELECT *");
```

```
Why Enum BEST?
   ✅ JVM guarantee karta — only ONE instance
   ✅ Thread-safe AUTOMATIC
   ✅ Serialization safe (default)
   ✅ Reflection se bhi break nahi hota
   ✅ ONE line code
```

---

### Real Spring Example

```java
@Service
public class UserService { ... }

// Spring beans = SINGLETON by default
// Same UserService instance everywhere
```

```
Spring uses Singleton heavily:
   @Service     → singleton
   @Component   → singleton
   @Repository  → singleton
   @Controller  → singleton

Tu kahin bhi @Autowired UserService karega
   = SAME object pura app mein
```

---

### When Use Singleton?

```
✅ Database connection pool
✅ Configuration manager
✅ Logger
✅ Cache
✅ Thread pool

❌ Stateless objects
❌ Multi-instance needed (different configs)
```

---

### Memory Hook

```
Singleton = 🏛️ President (only ONE)

3 cheez chahiye:
   1. private constructor
   2. static instance
   3. public getInstance()

BEST = Enum INSTANCE (one line, all safe)
```

---

## 3️⃣ FACTORY — Type-based Object Creation

### Problem Pehle

```
Imagine pizza shop simulation:

   class MargheritaPizza { ... }
   class FarmhousePizza { ... }
   class MexicanPizza { ... }
```

```java
// Without Factory — client mein if-else mess:
public Pizza orderPizza(String type) {
    if (type.equals("MARGHERITA")) {
        return new MargheritaPizza();
    } else if (type.equals("FARMHOUSE")) {
        return new FarmhousePizza();
    } else if (type.equals("MEXICAN")) {
        return new MexicanPizza();
    }
    // Naya pizza add = har jagah if-else update
}
```

```
Problems:
   ❌ if-else har jagah duplicate
   ❌ Client ko HAR concrete class ka naam pata
   ❌ Naya pizza add = sab clients update
   ❌ Tight coupling
```

---

### Pizza Shop Analogy

```
Tu Dominos jata:
   "Mujhe Margherita pizza chahiye"
   
   Tu CLASS ka naam NAHI bolta:
      ❌ "MargheritaPizza class banao"
      
   Tu sirf TYPE bolta:
      ✅ "Margherita do"
   
   Shop (factory):
      Type match karta
      Right class instance banata
      Tujhe deta
```

---

### Factory Solution

```java
// 1. Common interface
interface Pizza {
    void prepare();
    void bake();
}

// 2. Concrete implementations
class MargheritaPizza implements Pizza {
    public void prepare() { System.out.println("Margherita preparing"); }
    public void bake() { System.out.println("Baking margherita"); }
}

class FarmhousePizza implements Pizza {
    public void prepare() { System.out.println("Farmhouse preparing"); }
    public void bake() { System.out.println("Baking farmhouse"); }
}

class MexicanPizza implements Pizza {
    public void prepare() { System.out.println("Mexican preparing"); }
    public void bake() { System.out.println("Baking mexican"); }
}

// 3. FACTORY class
class PizzaFactory {
    public static Pizza create(String type) {
        return switch (type) {
            case "MARGHERITA" -> new MargheritaPizza();
            case "FARMHOUSE"  -> new FarmhousePizza();
            case "MEXICAN"    -> new MexicanPizza();
            default -> throw new IllegalArgumentException("Unknown: " + type);
        };
    }
}
```

### Usage

```java
// Client just specifies TYPE
Pizza p1 = PizzaFactory.create("MARGHERITA");
p1.prepare();
p1.bake();

Pizza p2 = PizzaFactory.create("FARMHOUSE");
p2.prepare();
p2.bake();

// Client doesn't know concrete classes!
// Just deals with Pizza interface
```

---

### Visual Flow

```
   CLIENT
      │
      │  PizzaFactory.create("MARGHERITA")
      ▼
   ┌──────────────────────┐
   │   PizzaFactory        │
   │                        │
   │   switch (type):       │
   │     "MARGHERITA" →     │
   │       new MargheritaPizza()
   │     "FARMHOUSE" →      │
   │       new FarmhousePizza()
   │     ...                 │
   └──────────┬───────────┘
              │
              ▼ returns Pizza interface
   ┌─────────────────────┐
   │   Pizza (interface)  │
   └─────────────────────┘
              │
              ▼
   CLIENT uses pizza methods
   (kaunsi concrete class = pata nahi care)
```

---

### Benefits

```
✅ Client doesn't know concrete classes
✅ Add new pizza? Update Factory ONLY
✅ Loose coupling (depends on interface)
✅ Single place for creation logic
```

---

### Real Production Example

```java
interface Account {
    void deposit(double amt);
}

class SavingsAccount implements Account { /* 4% interest */ }
class CurrentAccount implements Account { /* overdraft */ }
class FixedDepositAccount implements Account { /* locked period */ }

class AccountFactory {
    public static Account create(String type) {
        return switch (type) {
            case "SAVINGS" -> new SavingsAccount();
            case "CURRENT" -> new CurrentAccount();
            case "FIXED"   -> new FixedDepositAccount();
            default -> throw new IllegalArgumentException();
        };
    }
}

// Bank teller:
Account acc = AccountFactory.create(userChoice);
acc.deposit(5000);
```

---

### Java Built-in Factories

```java
// Calendar
Calendar c = Calendar.getInstance();
// Returns GregorianCalendar internally

// NumberFormat
NumberFormat nf = NumberFormat.getCurrencyInstance();

// JDBC
Connection conn = DriverManager.getConnection(url);

// Spring Framework
@Autowired UserService service;
// Spring's BeanFactory = giant factory
```

---

### When Use Factory?

```
✅ Multiple implementations of an interface
✅ Type decided at runtime (config, user input)
✅ Hide concrete classes from client
✅ Object creation has logic/complexity

❌ Only ONE implementation
❌ Direct 'new' is fine for simple cases
```

---

### Memory Hook

```
Factory = 🏭 Pizza shop

Tu TYPE bolta — "Margherita"
Factory decide karta concrete class
Tujhe interface milta

Pattern:
   Interface
   Multiple concrete classes
   Factory.create(type) → returns interface

Real examples:
   Calendar.getInstance()
   NumberFormat.getInstance()
   DriverManager.getConnection()
   Spring's BeanFactory
```

📚 [← Back to JAVA](../../)
