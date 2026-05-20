# this vs super

> **V90 — Core Java Extras: Topic 13**

---

## STORY — Family Reference

> Imagine **family conversation**:
>
> **`this`** = *"Mai khud"* (current speaker)
>
> **`super`** = *"Mere papa"* (parent reference)
>
> Tu beta hai jo apne **bare mein bhi baat kar sakta** (`this.age`) **aur apne papa ki cheezein bhi access kar sakta** (`super.familyName`).
>
> Same Java mein — class apne fields/methods (`this`) aur parent ke fields/methods (`super`) dono access kar sakti.

---

## VISUAL — Object Reference Tree

```
   class Animal {
       String name = "Animal";   ← parent's field
       void eat() { ... }        ← parent's method
   }
                ▲ super
                │
   class Dog extends Animal {
       String name = "Dog";      ← child's field (HIDES parent's)
       void bark() { ... }       ← child's own
       
       void show() {
           System.out.println(this.name);    ← "Dog" (current)
           System.out.println(super.name);   ← "Animal" (parent)
           
           this.bark();                       ← own method
           super.eat();                       ← parent's method
       }
   }
                ▲ this
                │
       Dog d = new Dog();
       d.show();
```

---

## CODE — Full Example

```java
class Animal {
    String name = "Animal";
    int age = 0;
    
    Animal() {
        System.out.println("Animal constructor");
    }
    
    Animal(String name) {
        this.name = name;
        System.out.println("Animal(name) constructor");
    }
    
    void breathe() {
        System.out.println(this.name + " breathing");
    }
}

class Dog extends Animal {
    String name = "Dog";          // HIDES parent's name
    String breed;
    
    Dog(String breed) {
        super("Doggy");           // calls Animal(name) — must be FIRST line
        this.breed = breed;
    }
    
    void show() {
        System.out.println("this.name = " + this.name);     // Dog
        System.out.println("super.name = " + super.name);   // Doggy (set via super())
        
        this.breathe();           // calls inherited method
        super.breathe();          // same — but explicit parent reference
    }
}

public class Test {
    public static void main(String[] args) {
        Dog d = new Dog("Labrador");
        d.show();
    }
}
```

**Output:**
```
Animal(name) constructor       ← super("Doggy") triggered this
this.name = Dog
super.name = Doggy
Dog breathing                  ← this.breathe() — Dog.name used
Doggy breathing                ← super.breathe() — Animal.name used
```

---

## 5 Use Cases of `this`

### 1. Disambiguate field vs parameter
```java
public class Person {
    String name;
    
    public Person(String name) {
        this.name = name;     // "this.name" = field, "name" = parameter
    }
}
```

### 2. Call another constructor (constructor chaining)
```java
public class User {
    String name;
    int age;
    
    public User() {
        this("Anonymous", 0);   // ← this(...) calls another constructor
    }
    
    public User(String name, int age) {
        this.name = name;
        this.age = age;
    }
}
```

### 3. Pass current object to method
```java
public void register(Listener l) {
    l.notify(this);    // pass current object
}
```

### 4. Return current object (fluent API)
```java
public User withName(String name) {
    this.name = name;
    return this;       // method chaining
}

// Usage: user.withName("X").withAge(28);
```

### 5. Inner class disambiguation
```java
public class Outer {
    int x = 10;
    class Inner {
        int x = 20;
        void show() {
            System.out.println(this.x);              // 20 (Inner)
            System.out.println(Outer.this.x);        // 10 (Outer)
        }
    }
}
```

---

## 5 Use Cases of `super`

### 1. Call parent constructor
```java
class Dog extends Animal {
    Dog() {
        super();        // explicit
                        // (implicit if not written — first line)
    }
}
```

### 2. Access parent's hidden field
```java
class Animal { String name = "Animal"; }

class Dog extends Animal {
    String name = "Dog";
    void show() {
        System.out.println(super.name);  // "Animal"
    }
}
```

### 3. Call parent's overridden method
```java
class Animal {
    void eat() { System.out.println("Animal eats"); }
}

class Dog extends Animal {
    @Override
    void eat() {
        super.eat();    // call parent's version first
        System.out.println("Dog eats too");
    }
}

// Output: "Animal eats" + "Dog eats too"
```

### 4. Call default interface method (Java 8+)
```java
interface A { default void hello() { System.out.println("A"); } }
interface B { default void hello() { System.out.println("B"); } }

class C implements A, B {
    @Override
    public void hello() {
        A.super.hello();    // explicitly call A's default
    }
}
```

### 5. Implicit `super()` in every constructor
```java
class Dog extends Animal {
    Dog() {
        // super() implicit here — Java auto-adds if not written
        System.out.println("Dog");
    }
}
```

---

## this vs super — COMPARISON

| Property | `this` | `super` |
|---|---|---|
| **Refers to** | Current object | Parent class |
| **Access fields** | `this.field` (own) | `super.field` (parent) |
| **Access methods** | `this.method()` | `super.method()` (parent's version) |
| **Constructor call** | `this(args)` — same class | `super(args)` — parent class |
| **Position in constructor** | Any (after super, but only one of this()/super() first) | MUST be first line if called |
| **In static methods?** | NO (no instance) | NO (no instance) |
| **Use case** | Disambiguation, chaining, return self | Override, hidden field, parent constructor |

---

## CRITICAL RULES

### Rule 1: `super()` must be FIRST line in constructor

```java
class Dog extends Animal {
    Dog() {
        System.out.println("Hi");   // COMPILE ERROR
        super();                     // must be first
    }
    
    Dog(int x) {
        super();                     // OK
        System.out.println("Hi");
    }
}
```

### Rule 2: `this()` and `super()` mutually exclusive

```java
class Dog extends Animal {
    Dog() {
        this(0);
        super();    // COMPILE ERROR — only one allowed
    }
    
    Dog(int x) { }
}
```

### Rule 3: Implicit `super()` if not written

```java
class Dog extends Animal {
    Dog() {
        // Java implicitly inserts: super();
        // calls Animal() no-arg constructor
    }
}
```

**Trap:** If parent has NO no-arg constructor, child constructor MUST explicitly call `super(args)` — otherwise compile error.

### Rule 4: Static methods can't use `this`/`super`

```java
class Dog extends Animal {
    static void info() {
        this.name;         // COMPILE ERROR
        super.name;        // COMPILE ERROR
    }
}
```

**Why?** `this`/`super` are object-references, static methods don't belong to any object.

---

## INTERVIEW TALKING POINT

**Q: "`this` aur `super` mein difference?"**

> *"`this` current object ka reference deta — apne fields/methods access karne ke liye, ya constructor chain karne (`this(args)`). `super` parent class ka reference — parent ke methods/fields access karne (`super.method()`, `super.field`), ya parent ka constructor call karne (`super(args)`).
>
> 5 main use cases:
> - **`this`:** field-parameter disambiguation, constructor chaining, return self (fluent API), inner class outer access
> - **`super`:** parent constructor, hidden parent field, overridden method ka parent version
>
> 2 rules critical:
> 1. `super()` constructor mein **first line** mandatory (if explicitly called)
> 2. `this()`/`super()` **dono ek saath** nahi — only one
>
> Static methods mein **dono allowed nahi** — instance reference nahi hota."*

**Q: "`this()` vs `super()` — kab konsa?"**

> *"`this()` same class ka doosra constructor call karta — overloaded constructors mein code reuse. `super()` parent ka constructor call karta — inheritance setup. Constructor chain mein top pe always parent constructor chalti hai (Java implicit `super()` daalta agar tu nahi likhta)."*

**Q: "Hidden field kya hota?"**

> *"Jab child class mein same naam ka field declare karte parent ke field jaisa — child's version 'hides' parent's. `this.name` child ka, `super.name` parent ka. Yeh **shadowing** kehlata. Methods mein 'overriding' hota (different from hiding for fields)."*

---

## POWER PHRASE

> **"`this` = current object reference — fields, methods, constructor chain (`this(args)`). `super` = parent class reference — overridden methods, hidden fields, parent constructor (`super(args)`). `super()` constructor mein first line mandatory. Static methods mein dono allowed nahi."**

---

## MEMORY HOOK

```
this   → "Mai khud"          → current object
                                fields, methods, this(args), return self
super  → "Mere papa"         → parent class  
                                parent fields, parent methods, super(args)

Constructor rules:
   super()  → FIRST line mandatory (if used)
   this()   → FIRST line OR not at all
   Both     → mutually exclusive (only ek)

Implicit super():
   class Dog extends Animal {
       Dog() {
           // super(); implicitly inserted
       }
   }

Static methods:
   this.field      (no current instance)
   super.method()  (no parent instance reference)
```

---

## TRAP BOX

```
Trap 1: "super() can be anywhere in constructor"
         MUST be FIRST line if explicitly called
         Otherwise compile error

Trap 2: "this() and super() can both be in same constructor"
         Only ONE allowed
         Mutually exclusive

Trap 3: "Static method mein this use ho sakta"
         NAHI — no instance context
         Static methods class-level, not object-level

Trap 4: "super.field overridden hota"
         Fields HIDDEN hote (shadowing)
         Methods overridden hote, fields hidden

Trap 5: "Implicit super() always works"
         Parent ke pas no-arg constructor MUST ho
         Otherwise child mein super(args) explicit
```
