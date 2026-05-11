# ☕ OOP 4 Pillars — Visual Revision

---

## 🎯 4 Pillars Overview

```
1. ENCAPSULATION   → data hide kar (capsule)
2. INHERITANCE     → parent se traits (family)
3. POLYMORPHISM    → ek naam, kayi roop
4. ABSTRACTION     → essential dikhao, details chhupao
```

---

## 1️⃣ ENCAPSULATION (Capsule Analogy)

```
Imagine ek MEDICINE CAPSULE:
   ┌────────────────────────┐
   │      CAPSULE            │
   │  ┌──────────────────┐  │
   │  │  inner medicine  │  │  ← HIDDEN
   │  │  (powder)        │  │
   │  └──────────────────┘  │
   │      shell hides         │
   └────────────────────────┘

Tu medicine khaata
   Bhitar ki POWDER nahi dikhti
   Bus capsule ke through use kar
```

### Java Code
```java
class BankAccount {
    private double balance;       // HIDDEN
    
    public void deposit(double x) { // way to access
        balance += x;
    }
    
    public double getBalance() {  // way to read
        return balance;
    }
}

// Outside class:
account.balance = 999;          // ❌ NOT ALLOWED (private)
account.deposit(100);            // ✅ allowed
account.getBalance();            // ✅ allowed
```

```
Kya hide karte:
   • Internal data (private fields)
   • Implementation details

Kya expose karte:
   • Public methods (controlled access)
```

---

## 2️⃣ INHERITANCE (Family Analogy)

```
   Daadaji  (Grandpa)
      │
      ▼
   Papa     (Daadaji's surname, traits)
      │
      ▼
   Tu       (Papa's surname, traits)
   
   Tu unique bhi ho
   Lekin parent ki cheezein auto mili
```

### Java Code
```java
class Animal {              // PARENT (super class)
    String name;
    void eat() { ... }
    void sleep() { ... }
}

class Dog extends Animal {  // CHILD (sub class)
    void bark() { ... }     // Dog ki own
}

Dog tommy = new Dog();
tommy.eat();    // ✅ inherited
tommy.sleep();  // ✅ inherited
tommy.bark();   // ✅ own
```

### Visual
```
   ┌──────────────────┐
   │     Animal        │  ← parent
   │   • name          │
   │   • eat()         │
   │   • sleep()       │
   └────────┬─────────┘
            │
            │ extends
            ▼
   ┌──────────────────┐
   │     Dog           │  ← child
   │   • name (inh)    │
   │   • eat() (inh)   │
   │   • sleep() (inh) │
   │   • bark() (own)  │
   └──────────────────┘
```

### Inheritance Types
```
Single:
   class B extends A           ✅

Multi-level:
   class C extends B
   class B extends A           ✅ (chain)

Multiple inheritance (2 parents):
   class C extends A, B        ❌ NOT ALLOWED
                                  (diamond problem)
   
   Solution: Interfaces
   class C implements A, B     ✅
```

### Why Inheritance?
```
1. CODE REUSE
   Animal mein eat() likha
   Dog, Cat, Cow sab use karte
   
2. HIERARCHY natural
   Dog IS-A Animal
   = Real world reflect karta
```

### Trap
```
extends    = inheritance (class)
implements = inheritance (interface)

Ek class:
   ✅ extends ONE class
   ✅ implements MULTIPLE interfaces
```

---

## 3️⃣ POLYMORPHISM (Same Word, Many Meanings)

```
"Polymorphism" = poly (many) + morph (forms)
   = Ek hi cheez, kayi roop
```

### Real World Analogy
```
Tu = ek hi insaan
   But different roles:
   
   • Office mein  → Engineer (work)
   • Ghar mein    → Beta (respect parents)
   • Friends mein → Yaar (masti)
   • Cricket mein → Player
   
   SAME tu, DIFFERENT behavior per context
```

### 2 Types
```
1. COMPILE-TIME (Method Overloading)
2. RUNTIME (Method Overriding)
```

### A) COMPILE-TIME — Method Overloading

```
Calculator ka "add" button:

   add(2, 3)         → integer add → 5
   add(2.5, 3.7)     → double add  → 6.2
   add("Hi", "Bye")  → string concat → "HiBye"
   add(1, 2, 3)      → 3 numbers add → 6
   
   SAME function name "add"
   DIFFERENT inputs handled
```

```java
class Calculator {
    int add(int a, int b) { return a + b; }
    double add(double a, double b) { return a + b; }
    String add(String a, String b) { return a + b; }
    int add(int a, int b, int c) { return a + b + c; }
}
```

```
Compiler decides AT COMPILE TIME which method to call
based on argument types
= Compile-time polymorphism
```

### Rules
```
Overloading = same name + DIFFERENT SIGNATURE
   Different = parameter types OR count OR order

❌ NOT overloading: just different return type
   int add(int, int)
   double add(int, int)  ← compile error!
```

### B) RUNTIME — Method Overriding

```
Parent class: Animal
   makeSound() → "Some sound"

Child classes inherit BUT redo:
   Dog       → "Bhow Bhow"
   Cat       → "Meow"
   Cow       → "Moo"
   Lion      → "Roar"
```

```java
class Animal {
    void makeSound() {
        System.out.println("Some sound");
    }
}

class Dog extends Animal {
    @Override
    void makeSound() {
        System.out.println("Bhow Bhow");
    }
}

// Magic moment:
Animal a = new Dog();
a.makeSound();        // → "Bhow Bhow" (Dog's!)
```

```
Compiler:
   "a is Animal type — compile pe kuch nahi pata"

Runtime:
   "Object actually Dog hai"
   "Dog ki overridden method call karu"

= Runtime polymorphism
```

### Overloading vs Overriding

```
┌───────────────────┬─────────────────┬─────────────────┐
│  Aspect           │  Overloading     │  Overriding     │
├───────────────────┼─────────────────┼─────────────────┤
│ When decided      │ Compile time    │ Runtime         │
│ Same class?       │ Yes              │ No (parent-child│
│ Method signature  │ DIFFERENT        │ SAME            │
│ Return type       │ Can differ      │ Same (or covar) │
│ Inheritance need? │ NO               │ YES (extends)   │
│ Annotation        │ None             │ @Override       │
└───────────────────┴─────────────────┴─────────────────┘
```

### Power
```java
// Method accepts ANY Animal
void feedAnimal(Animal a) {
    a.makeSound();   // each animal sounds differently
    a.eat();
}

feedAnimal(new Dog());   // Bhow Bhow
feedAnimal(new Cat());   // Meow
feedAnimal(new Lion());  // Roar

// 1 method handles ALL animals
// = Polymorphism gives flexibility
```

---

## 4️⃣ ABSTRACTION (Show What Matters, Hide The Rest)

```
"Abstraction" = essential dikhao, internal details chhupao
```

### Car Driving Analogy
```
Tu car chala raha:
   ┌────────────────────────────┐
   │  Steering wheel             │  ← TU YEH USE KARTA
   │  Brake pedal                │
   │  Accelerator                │
   │  Gear stick                 │
   └────────────────────────────┘
   
   Bhitar kya ho raha:
      • Engine combustion
      • Crankshaft rotation
      • Fuel injection
      • Hydraulic brake pressure
   
   = TUJHE NAHI PATA = TUJHE NAHI CHAHIYE PATA
```

### Java Mein 2 Tools
```
1. Abstract Class
2. Interface
```

### A) Abstract Class
```java
abstract class Animal {
    
    abstract void makeSound();   // no body
    
    void eat() {                  // has body
        System.out.println("Eating...");
    }
}

class Dog extends Animal {
    @Override
    void makeSound() {            // MUST implement
        System.out.println("Bhow Bhow");
    }
}
```

```
Rules:
   ✅ 'abstract' keyword required
   ✅ Can have abstract + concrete methods
   ✅ Child MUST implement abstract methods
   ❌ Can't directly instantiate
      new Animal()  ← compile error
```

### B) Interface (Pure Abstraction)
```java
interface Vehicle {
    void start();        // implicitly abstract
    void stop();
    void accelerate();
}

class Car implements Vehicle {
    public void start() { ... }
    public void stop() { ... }
    public void accelerate() { ... }
}
```

```
Rules:
   ✅ All methods abstract by default (Java 7)
   ✅ Java 8+ allows default + static methods
   ✅ Multiple inheritance via interfaces
```

### Abstract Class vs Interface
```
┌──────────────────────┬─────────────────┬─────────────────┐
│  Aspect              │  Abstract Class  │  Interface      │
├──────────────────────┼─────────────────┼─────────────────┤
│ Methods              │ abstract +       │ abstract +      │
│                      │ concrete         │ default (J8+)   │
│ Variables            │ any type         │ public static   │
│                      │                  │ final only      │
│ Inheritance          │ extends ONE      │ implements      │
│                      │                  │ MULTIPLE        │
│ Constructor          │ Yes              │ No              │
│ Use when             │ Common code +    │ Pure contract   │
│                      │ shared state     │ (no impl)       │
└──────────────────────┴─────────────────┴─────────────────┘
```

### Real World Examples
```
Abstract Class:
   Vehicle (has fuel, engine — common state)
      ├── Car
      ├── Bike
      └── Truck
   = "IS-A" relationship + shared code

Interface:
   Comparable, Serializable, Runnable
   = Pure contract, no shared code
   = "CAN-DO" capability
```

### Encapsulation vs Abstraction
```
ENCAPSULATION:
   "Data hide karo, access via methods"
   private balance + public deposit()

ABSTRACTION:
   "Implementation hide karo, interface dikhao"
   abstract makeSound() — child decides how

   Encapsulation = HOW (mechanism)
   Abstraction   = WHAT (concept)
```

---

## 🎯 OOP 4 Pillars — Final Summary

```
1. ENCAPSULATION  → capsule (data hide, access via methods)
2. INHERITANCE    → family (parent traits inherited)
3. POLYMORPHISM   → ek naam kayi roop (over/over-riding)
4. ABSTRACTION    → car steering (essential dikhao, hide rest)
```

📚 [← Back to JAVA](../)
