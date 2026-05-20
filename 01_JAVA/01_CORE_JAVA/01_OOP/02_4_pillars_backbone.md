# 4 Pillars — OOP ka Backbone

> **V90 Section 1 — Topic 2 (Interview MUST-KNOW)**

---

## STORY — Office mein 4 Problems, 4 Solutions

> Imagine **office mein tu architect hai** — 4 alag problems aati hain, har problem ka solution = ek pillar.
>
> **Day 1:** *"Salary field public hai — koi bhi -5000 set kar sakta!"* → **Encapsulation**
>
> **Day 2:** *"Manager bhi User hai — code copy-paste kar raha har naya role banane mein"* → **Inheritance**
>
> **Day 3:** *"Developer/Manager/Intern ka bonus alag formula — but ek loop mein process karna chahta"* → **Polymorphism**
>
> **Day 4:** *"HR ko bonus chahiye, formula nahi — internal logic hide karo"* → **Abstraction**
>
> **4 alag problems → 4 pillars** OOP banaate hain.

---

## THE 4 PILLARS — VISUAL

```
                     OOP BUILDING
                  ┌──────────────┐
                  │   Java App   │
                  └──────┬───────┘
                         │
          ┌──────────────┼──────────────┐
          │              │              │
          ▼              ▼              ▼              ▼
     ┌─────────┐   ┌─────────┐   ┌──────────┐   ┌──────────┐
     │  ENCAP   │   │  INHERIT │   │  POLY    │   │  ABSTRACT│
     │  ─────   │   │  ──────  │   │  ────    │   │  ──────  │
     │ Hide     │   │ Reuse    │   │ Same naam│   │ Show WHAT│
     │ private  │   │ extends  │   │ alag work│   │ hide HOW │
     │ + getters│   │          │   │ runtime  │   │          │
     └─────────┘   └─────────┘   └──────────┘   └──────────┘
                                              
       LOCK          TREE         MASKS            CURTAIN
```

---

## 1. ENCAPSULATION — Hide Internal State

### Real-world analogy: ATM Machine

```
   You see:           Inside (hidden):
   ─────────          ─────────────────
   Touchscreen     Circuits
   Buttons         Memory chips
   Cash slot       Network calls
   
   Tu sirf BUTTONS dabata — circuit kaise kaam karta, mat soch.
   ATM ne tujhe SAFE INTERFACE diya.
```

### Code

```java
public class BankAccount {
    private double balance;          // hidden
    
    public double getBalance() {     // controlled access
        return balance;
    }
    
    public void deposit(double amount) {
        if (amount <= 0) throw new IllegalArgumentException();   // validation
        balance += amount;
    }
    
    public void withdraw(double amount) {
        if (amount > balance) throw new InsufficientFunds();      // rule enforced
        balance -= amount;
    }
}

// Usage:
BankAccount acc = new BankAccount();
acc.balance = -5000;      // COMPILE ERROR (private)
acc.deposit(-5000);       // Caught by validation → exception
```

### Why Encapsulation Wins

| Without | With |
|---|---|
| `acc.balance = -5000;` data corrupt | `setter validation` rule enforced |
| Internal state exposed | Internal hidden, controlled access |
| Class invariant breakable | Invariant guaranteed |

---

## 2. INHERITANCE — Code Reuse via Family Tree

### Real-world analogy: Family Tree

```
                  Animal
                  (eat, sleep, breathe)
                  /    |    \
              Dog    Cat    Bird
              /\
         GoldenRetriever  Pug
```

> Dog ko `eat()`, `sleep()`, `breathe()` **wapas se nahi likhna** — Animal extend kiya, **automatic mil gaye**.

### Code

```java
public class Animal {
    public void eat() { System.out.println("Eating"); }
    public void sleep() { System.out.println("Sleeping"); }
}

public class Dog extends Animal {
    public void bark() { System.out.println("Bark!"); }
}

// Usage:
Dog d = new Dog();
d.eat();      // inherited from Animal
d.sleep();    // inherited
d.bark();     // Dog's own
```

### Inheritance Rules

| Rule | Example |
|---|---|
| **Single class extension** | `Dog extends Animal` (not multiple classes) |
| **Multiple interface impl** | `Dog implements Pet, Trainable` (multiple OK) |
| **`super.method()`** | Parent's version call karne ke liye |
| **`final class`** | Inheritance prevent karta (e.g., `String`) |
| **`final method`** | Override prevent karta |

---

## 3. POLYMORPHISM — Same Naam, Alag Kaam

### Real-world analogy: Same Word, Different Meaning

```
   Word "Run":
   ────────────
   Athlete:     "Run a marathon"     → physically running
   Programmer:  "Run a script"       → executing code
   Politician:  "Run for office"     → campaign for election
   
   Same word, alag context = alag meaning.
   OOP mein same method name, alag class = alag behavior.
```

### Code

```java
public abstract class Shape {
    public abstract double area();
}

public class Circle extends Shape {
    private double radius;
    public double area() { return Math.PI * radius * radius; }
}

public class Square extends Shape {
    private double side;
    public double area() { return side * side; }
}

public class Triangle extends Shape {
    private double base, height;
    public double area() { return 0.5 * base * height; }
}

// Polymorphic loop — same call, different math
List<Shape> shapes = List.of(new Circle(5), new Square(4), new Triangle(3, 4));
for (Shape s : shapes) {
    System.out.println(s.area());   // Runtime decides which area() runs
}
```

### Visual — Runtime Dispatch

```
   Shape s = new Circle(5);
              │
              │ s.area() called
              ▼
   ┌────────────────────────┐
   │  JVM checks ACTUAL type│
   │  Circle? → Circle.area()│
   │  Square? → Square.area()│
   │  Triangle? → Triangle.area()│
   └────────────────────────┘
   
   "Late binding" / "Runtime polymorphism"
```

### Types of Polymorphism

| Type | Example | When decided |
|---|---|---|
| **Compile-time (Overloading)** | `add(int, int)` vs `add(int, int, int)` | Compile time |
| **Runtime (Overriding)** | `Animal.makeSound()` overridden in Dog/Cat | Runtime |

---

## 4. ABSTRACTION — Show WHAT, Hide HOW

### Real-world analogy: TV Remote

```
   You press:           TV does (hidden):
   ──────────           ─────────────────
   Power button      Voltage regulation
                        Signal processing
                        LED activation
                        Pixel rendering
   
   Tu sirf BUTTON dabata — TV ka internal circuit kaise kaam karta, jaane ki zaroorat nahi.
   Remote ne tujhe ABSTRACT interface diya.
```

### Code — Abstract Class

```java
public abstract class Payment {
    public abstract void pay(double amount);   // WHAT (no HOW)
    
    public final void process(double amount) {  // template method
        validate();
        pay(amount);                             // delegated to subclass
        sendReceipt();
    }
}

public class UPIPayment extends Payment {
    @Override
    public void pay(double amount) {
        // UPI-specific logic — connect to bank, OTP, etc.
    }
}

public class CreditCardPayment extends Payment {
    @Override
    public void pay(double amount) {
        // Card-specific logic — gateway, 3D secure, etc.
    }
}

// Caller:
Payment p = new UPIPayment();
p.process(1000);   // pay() ka actual implementation hidden — caller ko fark nahi
```

### Abstraction vs Encapsulation (interview gotcha)

| | Abstraction | Encapsulation |
|---|---|---|
| **Focus** | What it does (interface) | How data is protected (state) |
| **Mechanism** | Abstract classes, Interfaces | Private fields, getters/setters |
| **Hides** | Implementation details | Internal data |
| **Example** | `List` interface (don't care if ArrayList/LinkedList) | `BankAccount.balance` private |

**Both hide stuff, but at different levels.**

---

## 4 PILLARS COMPARISON TABLE

| Pillar | Java mechanism | Solves |
|---|---|---|
| **Encapsulation** | `private` + getters/setters | Data protection / invariants |
| **Inheritance** | `extends` keyword | Code reuse + hierarchy |
| **Polymorphism** | Method overriding (runtime) | Extensibility — add new types easily |
| **Abstraction** | `abstract class`, `interface` | Hide complexity, expose contract |

---

## INTERVIEW TALKING POINT

**Q: "OOP ke 4 pillars kya hain aur kyu zaroori?"**

> *"Har pillar ek specific design problem solve karta:
>
> 1. **Encapsulation** — internal state private rakhke getters/setters expose karta. ATM analogy — circuit hidden, button-based safe interface. Data integrity ensure karta.
>
> 2. **Inheritance** — `extends` keyword se parent ka code reuse hota. Family tree analogy — Animal mein eat/sleep, Dog automatically inherit kar leta. Code duplication kam.
>
> 3. **Polymorphism** — same method name, alag classes mein alag behavior. Runtime dispatch karta JVM. Shape.area() call karo, Circle ya Square ka implementation chalega — extensibility ke liye gold.
>
> 4. **Abstraction** — `abstract class`/`interface` se contract define karte, implementation hide. TV remote analogy — button dabao, internal circuit ki chinta nahi. Caller decoupled implementation se."*

**Q: "Abstraction vs Encapsulation difference?"**

> *"Dono hide karte but alag levels pe. Encapsulation **state** hide karta — `private fields`. Abstraction **implementation** hide karta — `abstract methods`/`interface`. Encapsulation = how to access data (getters/setters). Abstraction = what is offered (contract)."*

---

## POWER PHRASES (1-line per pillar)

| Pillar | Phrase |
|---|---|
| **Encapsulation** | *"Hides internal state with private fields and exposes behavior through public methods — like an ATM where you press buttons without touching the circuit."* |
| **Inheritance** | *"Lets a child class reuse parent class code — Dog extends Animal and inherits eat() and sleep() without rewriting them."* |
| **Polymorphism** | *"Same method name behaves differently at runtime depending on the object — shape.area() computes differently for Circle vs Square."* |
| **Abstraction** | *"Exposes WHAT a class does, not HOW — like a TV remote, you press ON without knowing the internal circuit."* |

---

## MEMORY HOOK — One liner each

```
ENCAPSULATION   → Private fields + getters/setters     "Andar ki baat andar"
INHERITANCE     → extends keyword, parent code reuse   "Child gets parent's stuff"
POLYMORPHISM    → Same name, alag behavior runtime    "Same word, different meanings"
ABSTRACTION     → abstract class / interface          "Show WHAT, hide HOW"

The 4 problems they solve:
   Encap     → DATA PROTECTION
   Inherit   → CODE REUSE
   Poly      → EXTENSIBILITY  
   Abstract  → SIMPLICITY
```

---

## TRAP BOX

```
Trap 1: "Encapsulation = data hiding only"
         Incomplete — also enforces invariants via setters
         Hide + protect via controlled access

Trap 2: "Java multiple inheritance support karta"
         Classes mein NAHI — sirf interfaces multiple
         extends 1 class, implements multiple interfaces

Trap 3: "Overloading polymorphism nahi hai"
         Compile-time polymorphism hai (static)
         Overriding = runtime polymorphism (dynamic)

Trap 4: "Abstract class = interface"
         Different — abstract can have constructors, fields, concrete methods
         Interface (Java 8+) can have default/static, but no state
```
