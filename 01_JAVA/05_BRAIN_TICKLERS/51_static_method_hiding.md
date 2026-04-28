# Static Method Override? — NAHI, Method Hiding Hai

> **V90 — Brain Ticklers: Topic 51**

---

## 📖 STORY

→ `class A` mein **static** `show()` likha — print "A"
→ `class B extends A` mein bhi **static** `show()` likha — print "B"
→ `A obj = new B();` → `obj.show()` call kiya
→ Result: **"A"** — "B" nahi! Kyu?
→ **Static method override NAHI hota** — sirf **method hiding** hota
→ **Reference type** decide karta kaunsa chalega (compile-time), object type nahi

```java
class A { static void show() { System.out.println("A"); } }
class B extends A { static void show() { System.out.println("B"); } }

A obj = new B();
obj.show();      // "A" — NOT "B"!
```

---

## 🟡 WHY "A" Aaya?

→ **Static = class se bind hota hai, object se nahi**
→ Compiler **reference type dekhta** = `A`
→ `A.show()` call hota → "A"
→ Agar **non-static** hota, runtime pe **object dekhta = B**, "B" print hota (true polymorphism)
→ **Static mein polymorphism NAHI hota** — ye **method hiding** hai, **overriding nahi**

---

## 🧠 Visualization — Compile vs Runtime Resolution

```
              Static Method Hiding — Compile vs Runtime

╔════════════════════════════════════════════════════════════╗
║ Reference Type vs Object Type                              ║
╚════════════════════════════════════════════════════════════╝

  A obj = new B();
  ▲             ▲
  │             │
  reference     object
  type = A      type = B
  (compile-time) (runtime)


╔════════════════════════════════════════════════════════════╗
║ Static Method = Compile-Time Resolved (REFERENCE TYPE)     ║
╚════════════════════════════════════════════════════════════╝

  Compiler ka decision:
       │
       ▼
  obj.show()  →  reference type kya hai?  →  A
       │
       ▼
  A.show()  call hua  →  print "A"   ✅


╔════════════════════════════════════════════════════════════╗
║ Agar NON-static hota toh — RUNTIME (OBJECT TYPE)           ║
╚════════════════════════════════════════════════════════════╝

  // non-static methods (true polymorphism)
  class A { void show() { print("A"); } }
  class B extends A { @Override void show() { print("B"); } }

  Runtime ka decision:
       │
       ▼
  obj.show()  →  object type kya hai?  →  B
       │
       ▼
  B.show()  call hua  →  print "B"   ✅
                          (overriding works)


╔════════════════════════════════════════════════════════════╗
║ Side-by-Side                                               ║
╚════════════════════════════════════════════════════════════╝

                Static               Non-Static
                (Hiding)             (Overriding)
                ────────             ────────────
  Decided:      Compile time         Runtime
  Looks at:     Reference type       Object type
  Output:       "A"                  "B"
  Polymorphism: ❌ NO                ✅ YES
```

---

## 🆚 Override vs Hiding

| | Method Override | Method Hiding |
|--|----------------|---------------|
| Methods | Non-static | Static |
| Decided | Runtime (object) | Compile-time (reference) |
| Polymorphism? | YES | NO |

---

## 💬 POWER PHRASE

> *"Static methods are bound to the class, not to the object. They can be redeclared in a subclass — but this is method hiding, not overriding. The reference type decides which static method runs at compile time."*
