# 🏭 FACTORY — Type-based Object Creation

---

## Problem Pehle

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

## Pizza Shop Analogy

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

## Factory Solution

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

## Visual Flow

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

## Benefits

```
✅ Client doesn't know concrete classes
✅ Add new pizza? Update Factory ONLY
✅ Loose coupling (depends on interface)
✅ Single place for creation logic
```

---

## Real Production Example

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

## Java Built-in Factories

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

## When Use Factory?

```
✅ Multiple implementations of an interface
✅ Type decided at runtime (config, user input)
✅ Hide concrete classes from client
✅ Object creation has logic/complexity

❌ Only ONE implementation
❌ Direct 'new' is fine for simple cases
```

---

## Memory Hook

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

📚 [← Back to Design Patterns](00_overview.md)
