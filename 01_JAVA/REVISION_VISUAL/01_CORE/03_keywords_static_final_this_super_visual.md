# 🔑 Keywords — static / final / this / super (Visual Revision)

---

## 1️⃣ static (Shared by All)

### Analogy — School Noticeboard
```
   School noticeboard:
   ┌──────────────────────────┐
   │  📋 SCHOOL NOTICE         │
   │  "Holiday on Friday"      │
   └──────────────────────────┘
   = Sab students dekhte
   = School ki property (one copy)
   
   Each student ka apna bag:
   ┌──────┐  ┌──────┐  ┌──────┐
   │ Bag1 │  │ Bag2 │  │ Bag3 │
   └──────┘  └──────┘  └──────┘
   = Each student's own bag
```

```
SCHOOL = class
NOTICEBOARD (shared) = static
BAG (per student) = instance variable
```

### Java Code
```java
class Student {
    static String schoolName = "DPS";    // shared by ALL
    String studentName;                   // per object
    
    void greet() {
        System.out.println("Hi from " + studentName 
                          + " of " + schoolName);
    }
}

Student s1 = new Student();
s1.studentName = "Arpan";

Student s2 = new Student();
s2.studentName = "Suresh";

Student.schoolName = "DAV";               // change one place

s1.greet();   // "Hi from Arpan of DAV"
s2.greet();   // "Hi from Suresh of DAV"

// Both see new school name = static is SHARED
```

### Memory Layout
```
   ┌────────────────────────────────────┐
   │  CLASS LEVEL (static area)          │
   │                                      │
   │  schoolName = "DAV"  ← ONE copy      │
   └────────────────────────────────────┘
   
   ┌──────────────┐  ┌──────────────┐
   │  Object s1    │  │  Object s2    │
   │  ───────────  │  │  ───────────  │
   │ studentName=  │  │ studentName=  │
   │   "Arpan"     │  │   "Suresh"    │
   └──────────────┘  └──────────────┘
```

### Static Methods
```java
class MathUtil {
    static int add(int a, int b) {
        return a + b;
    }
}

MathUtil.add(2, 3);   // direct call, no object needed
// = utility method
```

```
USE static when:
   ✅ Shared data (counters, constants)
   ✅ Utility methods (Math.sqrt, etc.)
   ✅ Factory methods

DON'T use static when:
   ❌ Per-object data (each user's name)
```

---

## 2️⃣ final (Cannot Change)

### Analogy — Engraved in Stone
```
   ┌────────────────────────────┐
   │  🪨 STONE TABLET             │
   │  "ARPAN MAHESHWARI"         │
   │  (engraved permanently)     │
   └────────────────────────────┘
   
   Once engraved → CANNOT CHANGE
```

### final variable
```java
final int MAX_USERS = 100;
MAX_USERS = 200;        // ❌ COMPILE ERROR
```

### final method
```java
class Animal {
    final void breathe() {        // child cannot override
        System.out.println("Breathing");
    }
}

class Dog extends Animal {
    void breathe() { ... }        // ❌ COMPILE ERROR
}
```

### final class
```java
final class String { ... }        // cannot extend

class MyString extends String { } // ❌ COMPILE ERROR
```

### Visual
```
   final variable    →  🪨 frozen value (no reassign)
   final method      →  🔒 child cannot override
   final class       →  🚫 cannot extend
```

---

## 3️⃣ this (Current Object Reference)

### Analogy
```
   Tu apne baare mein bolta:
      "MAIN Arpan hu"   ← MAIN = this
      "MERA naam X"     ← MERA = this
   
   "this" = current object
            = "yeh main hu, mera referencing"
```

### Java Code — Why this is needed
```java
class House {
    String color;
    String owner;
    
    // Parameter name == field name
    public House(String color, String owner) {
        this.color = color;       // this.color = field
                                   // color (right) = parameter
        this.owner = owner;
    }
}
```

```
Without "this" — confusion!
   color = color  ← which color is which?

With "this" — clarity:
   this.color = color
   field      = parameter
```

### Other Uses of this
```java
class A {
    void method1() {
        this.method2();        // explicit current object call
    }
    void method2() { ... }
    
    public A() {
        this("default");       // call other constructor
    }
    public A(String s) { ... }
}
```

---

## 4️⃣ super (Parent Object Reference)

### Analogy
```
   Tu apne father ke baare mein bolta:
      "MERE PAPA ka naam X"      ← MERE PAPA = super
      "PAPA ne yeh sikhaya"      ← PAPA = super
   
   "super" = parent class reference
            = "ye mera papa, uski cheezein"
```

### Java Code — super.method()
```java
class Animal {
    void eat() {
        System.out.println("Animal eating");
    }
}

class Dog extends Animal {
    void eat() {
        super.eat();              // call parent method first
        System.out.println("Dog eating biscuit");
    }
}

new Dog().eat();
// Output:
//   Animal eating
//   Dog eating biscuit
```

### super in Constructor
```java
class Animal {
    String name;
    
    public Animal(String n) {
        name = n;
    }
}

class Dog extends Animal {
    String breed;
    
    public Dog(String n, String b) {
        super(n);            // call parent constructor first
        breed = b;
    }
}

new Dog("Tommy", "Labrador");
// Animal constructor runs → name = "Tommy"
// Dog constructor → breed = "Labrador"
```

```
Rule:
   super(...) MUST be FIRST line in child constructor
   If not written explicitly = Java auto-adds super()
```

---

## 🆚 this vs super

```
┌────────────────┬──────────────────┬──────────────────┐
│  Aspect        │  this            │  super           │
├────────────────┼──────────────────┼──────────────────┤
│ Refers to      │ Current object   │ Parent object    │
│ Variable use   │ this.field       │ super.field      │
│ Method use     │ this.method()    │ super.method()   │
│ Constructor    │ this(...)         │ super(...)       │
│ Position rule  │ First line       │ First line       │
└────────────────┴──────────────────┴──────────────────┘
```

```
Constructor first line — ONLY ONE allowed:
   this(...)  OR  super(...)
   not both
```

---

## 🎯 All 4 Together — Memory Hook

```
static  → 🏫 school noticeboard (shared, class-level)
final   → 🪨 engraved in stone (cannot change)
this    → 👈 "yeh main hu" (current object)
super   → 👨 "yeh mera papa" (parent reference)
```

📚 [← Back to JAVA](../)
