# Quick Java Q&A — Interview Traps Visual

---

## 1️⃣ length / size — 3 Different Things

```java
// Array
int[] arr = {1, 2, 3};
arr.length;          // 3 — FIELD (no parens)

// String
String s = "Arpan";
s.length();          // 5 — METHOD (parens)

// Collection
List<Integer> list = new ArrayList<>();
list.size();         // METHOD (parens)
```

```
Memory hook:
   Array  → length      (field, no parens)
   String → length()    (method)
   List   → size()      (method)
   
   Trap: array.length() compile error
         string.length  compile error
```

### DEEP — Why `array.length` Field, Not Method?

```
Array = SPECIAL JVM object
   Created by JVM (not regular class)
   Tu nahi dekh sakta "Array class ka source code"
   = Internal JVM construct
```

```
Length properties:
   Decided at: creation time (new int[5])
   Changes:    NEVER (fixed once created)
   = IMMUTABLE size

Why FIELD vs Method?
   FIELD:   arr.length    → direct memory read, O(1)
            JVM stores in array header
   METHOD:  arr.length()  → invocation overhead (small but unnecessary)
```

```
Memory layout:
   int[] arr = new int[5];
   
   ┌──────────────────────────────────┐
   │  Array header                     │
   │  ┌─────────────┐                  │
   │  │ length: 5   │  ← stored field  │
   │  └─────────────┘                  │
   │                                    │
   │  data: [0, 0, 0, 0, 0]            │
   └──────────────────────────────────┘
```

```
String contrast:
   class String {
       private final char[] value;
       
       public int length() {
           return value.length;   ← wraps array's field
       }
   }
   
   String = proper class
   Method exposed for encapsulation
   Internal char array still uses FIELD
```

---

## 2️⃣ int vs Integer

```
   int     = primitive (4 bytes)
   Integer = wrapper class (object)
```

```
┌──────────────┬────────────┬─────────────┐
│  Aspect      │  int       │  Integer    │
├──────────────┼────────────┼─────────────┤
│ null         │ NPE      │          │
│ Collections  │          │ (List)   │
│ Methods      │          │          │
│ Default      │ 0          │ null        │
│ Performance  │ Fast       │ Slower      │
└──────────────┴────────────┴─────────────┘
```

---

## 3️⃣ == vs equals()

```java
String a = "Hello";
String b = "Hello";
String c = new String("Hello");

a == b;              // true (string pool)
a == c;              // false (different memory)
a.equals(c);         // true (content same)
```

```
Rule:
   ==       → reference (same memory location?)
   .equals()→ content (same data?)
   
   Strings/Objects → ALWAYS .equals()
   Primitives → == only option
```

---

## 4️⃣ final / finally / finalize — 3 DIFFERENT

```
   final     → keyword
                final variable: cannot reassign
                final method: cannot override
                final class: cannot extend
   
   finally   → block
                try-catch-FINALLY (always runs)
   
   finalize() → method (DEPRECATED Java 9+)
                GC ke before called (don't use)
```

```java
final int X = 5;   // X = 10 

try { ... }
finally { 
    // ALWAYS runs
}
```

---

## 5️⃣ throw vs throws

```java
// throw — RAISE exception (verb, action)
if (age < 0) {
    throw new IllegalArgumentException();
}

// throws — DECLARE method MIGHT throw (declaration)
public void readFile() throws IOException {
    // ...
}
```

```
throw    = "EXCEPTION raise kar abhi" (action)
throws   = "yeh method exception throw KAR SAKTI" (declaration)
```

---

## 6️⃣ Overload vs Override

```
OVERLOAD (compile-time):
   Same name, DIFFERENT parameters
   Same class
   
   int add(int a, int b) { ... }
   int add(int a, int b, int c) { ... }    ← overload

OVERRIDE (runtime):
   SAME signature
   Parent → Child class
   
   class Animal { void eat() {...} }
   class Dog extends Animal {
       @Override
       void eat() {...}    ← override
   }
```

---

## 7️⃣ Constructor — Overload vs Override?

```
Overload constructor:    YES (different params)
   public User() { ... }
   public User(String name) { ... }
   public User(String name, int age) { ... }

Override constructor:    NEVER POSSIBLE
   Constructors NOT inherited
   = Trap question
```

### DEEP — Why Constructor CANNOT Be Overridden?

```
OVERRIDE requires:
   1. Method INHERITED from parent
   2. Child redefines with SAME signature
   3. Polymorphism at runtime
```

```
Constructor rules:
   1. Constructor NOT inherited
   2. Constructor name = CLASS name
   
   Parent constructor: Parent()
   Child constructor:  Child()
   = Different names = different identity
```

```java
class Animal {
    public Animal() {           ← name: Animal
        // ...
    }
}

class Dog extends Animal {
    public Dog() {              ← name: Dog (NOT Animal!)
        super();                ← calls parent's, not overrides
    }
}
```

```
Constructor's JOB:
   Initialize THIS class's fields
   = Per-class concern
   
Constructor CAN:
   OVERLOAD (multiple in same class)
   CHAIN via super() (call parent)
   
Constructor CANNOT:
   Override
```

```
Side note — Also cannot override:
   • static method (class-level, not inherited)
   • final method  (locked by parent)
   • private method (not visible to child)
```

---

## 8️⃣ HashMap vs Hashtable

```
┌────────────────┬──────────────┬──────────────┐
│  Aspect        │  HashMap     │  Hashtable   │
├────────────────┼──────────────┼──────────────┤
│ Thread-safe    │            │ (sync)    │
│ null key/value │ allowed   │           │
│ Speed          │ Fast         │ Slow         │
│ Modern         │            │ Legacy       │
└────────────────┴──────────────┴──────────────┘
```

```
Modern alternative to Hashtable:
   ConcurrentHashMap (better perf + thread-safe)
```

---

## 9️⃣ Vector vs ArrayList

```
┌────────────────┬──────────────┬──────────────┐
│  Aspect        │  Vector      │  ArrayList   │
├────────────────┼──────────────┼──────────────┤
│ Thread-safe    │ (sync)    │           │
│ Growth         │ 2x           │ 1.5x         │
│ Era            │ Java 1.0     │ Java 1.2+    │
│ Performance    │ Slow         │ Fast         │
└────────────────┴──────────────┴──────────────┘
```

```
Modern: ArrayList always
   Need thread-safe? → Collections.synchronizedList()
```

---

## Why String Immutable?

```
4 reasons:

1. SECURITY
   File paths, URLs as String — mutable = security risk

2. HASHCODE CACHING
   String.hashCode() cached (immutable allows this)
   HashMap key fast

3. STRING POOL
   Immutability lets multiple references share same object
   Memory efficient

4. THREAD SAFETY
   Immutable = inherently thread-safe
   Multiple threads read same string safely
```

---

## 1️⃣1️⃣ Wrapper Cache (-128 to 127)

```java
Integer a = 100;
Integer b = 100;
a == b;              // TRUE (cached!)

Integer c = 200;
Integer d = 200;
c == d;              // FALSE (different objects)
```

```
Integer.valueOf() internally:
   -128 to 127 → CACHED, reused
   Beyond → new Integer() always
   
Always use .equals() for Integer comparison!
```

---

## 1️⃣2️⃣ Why Java Not 100% OOP?

```
Java has PRIMITIVES (int, long, double, etc.)
   Primitives are NOT objects
   
Pure OOP language:
   Everything is object (Smalltalk, Ruby)
   
Java made trade-off:
   Primitives for PERFORMANCE
   = ~95% OOP, not 100%
```

---

## 1️⃣3️⃣ Why Platform Independent? (WORA)

```
Write Once, Run Anywhere

   .java
     ↓ javac
   .class (bytecode — UNIVERSAL)
     ↓
   Windows JVM    Linux JVM    Mac JVM
     ↓               ↓              ↓
   Windows OS     Linux OS      Mac OS

   Each OS has its OWN JVM
   Bytecode = same everywhere
   = Platform independence
```

---

## 1️⃣4️⃣ Static Method — Override?

```java
class Parent {
    static void greet() {
        System.out.println("Parent");
    }
}

class Child extends Parent {
    static void greet() {           // HIDING, not override!
        System.out.println("Child");
    }
}

Parent.greet();          // "Parent"
Child.greet();           // "Child"

Parent p = new Child();
p.greet();               // "Parent" (not "Child"!)
```

```
Static methods: HIDDEN, not overridden
   Resolved at COMPILE time
   Based on reference type, not actual object
```

---

## 1️⃣5️⃣ Diamond Problem

```
Multiple inheritance issue:

       A
      / \
     B   C
      \ /
       D
   
   D extends B and C
   B and C both have method()
   Which method() D inherits?
   = AMBIGUOUS
```

```
Java solution:
   Class: NO multiple class inheritance
   Interface: Multiple allowed
              Default methods conflict?
              Child MUST override + specify
```

---

## 1️⃣6️⃣ transient Keyword

```java
class User implements Serializable {
    String name;
    transient String password;  // ← skip during serialize
}
```

```
transient = "Mat serialize karo yeh field"
   Use: passwords, temp data, cached values
```

---

## 1️⃣7️⃣ Marker Interfaces

```
EMPTY interfaces — no methods
Purpose: SIGNAL to JVM

   Serializable   → "yeh class serialize ho sakti"
   Cloneable      → "yeh class clone ho sakti"
   RandomAccess   → "yeh list O(1) access supports"
```

---

## 1️⃣8️⃣ Shallow vs Deep Copy

```
SHALLOW COPY:
   Object copy — but REFERENCES (nested) shared
   
   User u1 = ...
   User u2 = u1.clone()  // shallow
   u2.address = ...
   = Same address object (shared)

DEEP COPY:
   FULL copy of object + ALL nested objects
   
   u1 and u2 = INDEPENDENT
   = No sharing
```

---

## 1️⃣9️⃣ Initialization Order

```
Java class loading order:
   
   1. STATIC fields            (class load)
   2. STATIC blocks             (class load)
   3. INSTANCE fields           (constructor)
   4. INSTANCE blocks           (constructor)
   5. CONSTRUCTOR body
   
   Parent before Child:
      Parent.static → Child.static
      Parent.instance → Child.instance
```

---

## 2️⃣0️⃣ equals + hashCode Contract

```
RULES:
   1. equals(a, b) = true → MUST hashCode same
   2. hashCode same → NOT necessarily equal
   3. Override equals → MUST override hashCode

CRITICAL:
   If you override one without other:
      HashMap/HashSet break!
      Same object lost in collection
```

```java
class Person {
    String name;
    
    @Override
    public boolean equals(Object o) { ... }
    
    @Override
    public int hashCode() { ... }    // MUST also override
}
```

---

## Memory Hook (One-Line Each)

```
1.  length      → array.length (field), string.length() (method)
2.  int/Integer → primitive vs wrapper, Integer = null + collections
3.  ==/equals   → reference vs content
4.  3 finals    → keyword / try-block / GC-method
5.  throw/throws→ verb (raise) vs noun (declare)
6.  load/ride   → diff params (compile) vs same sig (runtime)
7.  ctor        → overload YES, override NO (name = class name)
8.  HashMap/HT  → non-sync vs sync (use ConcurrentHashMap)
9.  Vector/AL   → legacy sync vs modern fast
10. String      → immutable (security, cache, pool, threads)
11. Wrapper -128→127 → cached, == works in range
12. !100% OOP   → primitives for perf
13. WORA        → bytecode universal, JVM per OS
14. static      → hidden, not overridden (compile-time)
15. Diamond     → class no, interface yes (override required)
16. transient   → skip serialization
17. Markers     → empty signal interfaces
18. shallow/deep→ refs shared vs deep copy
19. Init order  → static first, instance second, parent before child
20. equals/hash → contract — override both
```

[← Back to JAVA](../../)
