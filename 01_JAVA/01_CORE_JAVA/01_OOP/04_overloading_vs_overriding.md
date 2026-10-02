# Method Overloading vs Overriding

> **V90 Section 1 — Topic 8**

---

## WHY — Dono Alag Problems

| | Solve karta |
|--|-------------|
| **Overloading** | Same kaam, **alag inputs** (sendNotification — sirf message? message + phone? message + email + priority?) |
| **Overriding** | Parent ka generic logic, **child apna special** chahiye (Manager apna calculateBonus, Employee se alag) |

---

## STORY

### Overloading wala
→ NotificationService mein sawaal aaya — `sendNotification` ek rakhun ya alag-alag?
→ Ek jagah sirf **message**, ek jagah **message + phone**, ek jagah **message + email + priority**
→ **Same naam, alag parameters** — Overloading, **compile time** pe decide hota hai

### Overriding wala
→ Aur jab Manager ne apna alag `calculateBonus()` likha **parent ke upar se**
→ **Overriding, runtime** pe decide
→ Dono alag problems ke alag solutions

---

## Visualization

```
OVERLOADING (Compile Time)              OVERRIDING (Runtime)
┌─────────────────────────┐             ┌──────────────────────────┐
│   NotificationService   │             │  Employee (Parent)       │
│                         │             │  calculateBonus(){       │
│  send(String msg)       │             │    return 10%            │
│  send(String, String)   │             │  }                       │
│  send(String, String,   │             └────────────┬─────────────┘
│       int)              │                          │ extends
│                         │             ┌────────────▼─────────────┐
│  SAME class, SAME naam  │             │  Manager (Child)         │
│  ALAG parameters        │             │  calculateBonus(){       │
└─────────────────────────┘             │    return 25%            │
                                        │  }                       │
                                        │  CHILD class REDEFINE    │
                                        │  parent method           │
                                        └──────────────────────────┘
```

---

## Comparison

| | Overloading | Overriding |
|--|-------------|-----------|
| **Class** | SAME class | Parent → Child |
| **Method name** | SAME | SAME |
| **Parameters** | **ALAG** | **SAME** |
| **When decided** | Compile time | Runtime |
| **Polymorphism type** | Static | Dynamic |
| **Annotation** | None | `@Override` |

★ Overloading me sirf RETURN TYPE alag = compile error — parameters alag hone chahiye.
★ Overloading child class me bhi ho sakti (parent ka `send(String)` + child ka naya `send(String, int)`).

---

## TRAP

> **`@Override` annotation HAMESHA lagao — typo pakdega compiler.**
> **Private/Static methods OVERRIDE nahi hote.**

---

## ★ TRAP — Static method "override"? NAHI, method HIDING hai

```java
class A { static void show() { System.out.println("A"); } }
class B extends A { static void show() { System.out.println("B"); } }

A obj = new B();
obj.show();      // "A" — NOT "B"!
```

**WHY "A" aaya?**
→ **Static = class se bind hota hai, object se nahi**
→ Compiler **reference type dekhta** = `A` → `A.show()` call → "A"
→ Agar **non-static** hota, runtime pe **object dekhta = B** → "B" (true polymorphism)
→ **Static mein polymorphism NAHI** — ye **method hiding** hai, overriding nahi

```
  A obj = new B();
  ▲             ▲
  reference     object
  type = A      type = B
  (compile-time) (runtime)

                Static               Non-Static
                (Hiding)             (Overriding)
                ────────             ────────────
  Decided:      Compile time         Runtime
  Looks at:     Reference type       Object type
  Output:       "A"                  "B"
  Polymorphism: NO                   YES
```

**Dabba analogy (2-Oct):**
```
A obj = new B();
  A obj   -> dabbe pe LABEL "A"          new B() -> dabbe ke ANDAR asli saamaan B

  static     -> Java LABEL padhta (compile time)   -> A.show() -> "A"
  non-static -> Java dabba KHOLTA (runtime)        -> B.show() -> "B"

  dabbe pe "Mithai" likha, andar laddoo:
     static     = "dabbe pe kya likha?" -> Mithai (A)
     non-static = "andar kya hai?"      -> Laddoo (B)
```

| | Method Override | Method Hiding |
|--|----------------|---------------|
| Methods | Non-static | Static |
| Decided | Runtime (object) | Compile-time (reference) |
| Polymorphism? | YES | NO |

> *"Static methods are bound to the class, not to the object. They can be redeclared in a subclass — but this is method hiding, not overriding. The reference type decides which static method runs at compile time."*

---

## ★ TRAP — Access modifier override rule: SAME ya WIDER, narrow NAHI

```java
// GALAT — narrower
class Parent { public void show() { } }
class Child extends Parent {
    private void show() { }      // COMPILE ERROR — public se private = narrow
}

// SAHI — same ya wider
class Parent { protected void show() { } }
class Child extends Parent {
    public void show() { }       // protected → public = WIDER, valid
}
```

**WHY?** Caller ne **parent reference se** call kiya:
```java
Parent p = new Child();
p.show();      // public expect kiya
```
→ Child ne `private` kar diya → caller ko expected access nahi mila → contract toot gaya
→ Liskov substitution — child must honour parent's contract

```
Parent              Child (allowed)
private          override hi nahi hota
default      →   default, protected, public
protected    →   protected, public
public       →   public ONLY
```

> *"Override mein child ka access modifier SAME ya WIDER hona chahiye — never narrower. Public can only stay public, protected can widen to public, private cannot be overridden at all."*

---

## ★ TRAP — Covariant return type (override mein return type SUBTYPE ho sakta)

**Simple line:** override karte waqt child ka return type, parent ke return type ka **SUBTYPE** ho sakta hai.

**Analogy:** Parent ka promise "Main tujhe **gaadi** dunga" · Child: "Main tujhe **Honda Car** dunga". Honda Car = gaadi hi hai → promise toota nahi. Code mein: Parent `Animal` return, Child `Dog` return (Dog = Animal hi hai).

```java
// Before Java 5 — STRICT: return type EXACT same
class Animal { Animal create() { return new Animal(); } }
class Dog extends Animal {
    @Override
    Animal create() { return new Dog(); }    // allowed — return type EXACT same
}

// Java 5+ — RELAXED (Covariant)
class Dog extends Animal {
    @Override
    Dog create() { return new Dog(); }       // NOW ALLOWED — Dog IS-A Animal
}
```

**Asli faayda — no casting:**
```java
// Bina covariant:
Dog d = new Dog();
Animal a = d.create();        // Animal mila
Dog d2 = (Dog) a;             // ugly cast karna pada

// Covariant ke saath:
Dog d = new Dog();
Dog d2 = d.create();          // direct Dog mila — NO CAST
```

**WHY allow hua?** Dog **Animal hai** (IS-A) → caller ne `Animal` expect kiya, Dog mila, Dog bhi Animal → Liskov substitution (child must work where parent expected).

**Kya allowed NAHI** — unrelated type:
```java
class Dog extends Animal {
    String create() { return "..."; }    // INVALID — String Animal nahi hai
}
```
Sirf parent ke return type (`Animal`) ka subtype chalega.
★ Dhyaan: `Cat create()` bhi COMPILE hoga (Cat bhi Animal hai) — rule sirf "Animal ka subtype" hai,
"apni hi class" nahi. Ajeeb lagega par galat nahi.

**Use case — factory / clone methods:**
```java
class Document { Document copy() { ... } }
class PDFDocument extends Document {
    @Override
    PDFDocument copy() { ... }    // PDF return karta — covariant
}
PDFDocument duplicate = new PDFDocument().copy();   // no cast — clean
```

> **Covariant Return Type = override mein return type child ka subtype ho sakta. Caller ko cast nahi karna padta. Java 5+ feature.**

> *"Covariant return type allows an overriding method to return a subtype of the parent's return type — Java 5+ supports this for cleaner factory and clone methods, eliminating the need for explicit casting at the call site."*

---

## POWER PHRASES

> *"Overloading is compile-time polymorphism — same method name, different parameters in the same class."*

> *"Overriding is runtime polymorphism — child class redefines a parent method. Always use `@Override` so the compiler catches mistakes."*
