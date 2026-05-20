# Type Casting + instanceof — Visual Revision

---

## 1️⃣ Primitive Type Casting (Container Analogy)

```
Imagine 2 containers:
   Small glass (int)        — capacity 4 bytes
   Big bucket (long)         — capacity 8 bytes
```

### Widening (Implicit) — Small → Big
```
   glass (int = 5)
        │
        ▼ pour into big bucket
   bucket (long = 5L)
   
   = NO data loss
   = Java does AUTOMATICALLY
   = "widening"
```

```java
int small = 5;
long big = small;          // automatic widening
// 5 → 5L (no cast needed)
```

### Narrowing (Explicit) — Big → Small
```
   bucket (long = 5000000000)
        │
        ▼ squeeze into small glass
   glass (int = ???)
   
   = May LOSE data (overflow)
   = Java FORCES you to cast (explicit)
   = "narrowing"
```

```java
long big = 5000000000L;
int small = (int) big;     // FORCED cast
// May overflow → data loss
```

### Order of Sizes
```
   byte → short → int → long → float → double
   ←──── widening ────→
   ←──── narrowing ────
   (explicit cast required)
```

---

## 2️⃣ Reference Type Casting

### Boss-Employee Analogy
```
Office hierarchy:
   Employee (parent class)
        ↑
        │ extends
        │
   Manager (child)
        ↑
        │ extends
        │
   Director (grandchild)
```

### UPCASTING (Implicit) — Child → Parent
```
   Director  d = new Director();
   Manager   m = d;            // upcast (automatic)
   Employee  e = d;            // upcast (automatic)
   
   "Director IS-A Employee" — always true
   = NO cast needed
```

```
Reason:
   Director is also Manager is also Employee
   Treating as Employee = safe (has all Employee fields)
```

### DOWNCASTING (Explicit) — Parent → Child
```java
Employee e = new Director();   // upcast
Director d = (Director) e;     // downcast (explicit)
```

```
Java FORCES cast because:
   "Employee" type could be anything — not necessarily Director
   You must explicitly assure: "Trust me, it's a Director"
```

### DANGER — Wrong Downcast
```java
Employee e = new Manager();    // actually a Manager
Director d = (Director) e;     // ClassCastException at runtime!
```

```
Compile time: passes (Java trusts your cast)
Runtime: explodes — "you said Director, but actual is Manager"
```

---

## 3️⃣ Solution = instanceof Check

```java
Employee e = getSomeEmployee();   // could be anything

if (e instanceof Director) {
    Director d = (Director) e;    // SAFE now
    d.directorMethod();
}
```

```
instanceof = "Is this object actually a Director?"
   true  → safe to cast
   false → don't cast
```

---

## 4️⃣ Visual Flow

```
   Employee e = ???
        │
        ▼
   instanceof Director?
        │
   ┌────┴────┐
   ▼         ▼
   YES       NO
   │         │
   ▼         ▼
   Cast      Don't cast
   safely    (skip)
```

---

## 5️⃣ Java 16+ Pattern Matching

```java
// OLD style
if (e instanceof Director) {
    Director d = (Director) e;
    d.directorMethod();
}

// NEW style (Java 16+)
if (e instanceof Director d) {       // cast + assign in one
    d.directorMethod();
}
```

---

## 6️⃣ Real Use Case

```java
public void processAnimal(Animal a) {
    if (a instanceof Dog d) {
        d.bark();                    // Dog-specific
    }
    else if (a instanceof Cat c) {
        c.meow();                    // Cat-specific
    }
    else {
        a.makeSound();               // generic
    }
}
```

---

## 7️⃣ Common Trap

```java
Animal a = new Dog();
Cat c = (Cat) a;              // ClassCastException

// Dog is NOT a Cat (siblings, not parent-child)
// instanceof would have caught this:
if (a instanceof Cat c2) {    // false → skip
    ...
}
```

---

## 8️⃣ Memory Hooks

```
PRIMITIVE:
   Small → Big      = automatic (widening)
   Big → Small      = (cast) required (narrowing, may lose)

REFERENCE:
   Child → Parent   = automatic (upcast — IS-A)
   Parent → Child   = (cast) + instanceof check (downcast)

instanceof          = "kya ye actually X type ka hai?"
```

---

## 9️⃣ Summary Visual

```
Type Casting Tree:

   ┌── Primitive
   │      ├── Widening (auto)
   │      └── Narrowing (explicit)
   │
   └── Reference
          ├── Upcasting (auto)
          └── Downcasting (explicit + instanceof)
```

[← Back to JAVA](../)
