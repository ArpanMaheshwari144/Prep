# Diamond Problem — 2 Interfaces Same Default Method

> **V90 — Brain Ticklers: Topic 55**

---

## STORY — Java 8 Default Methods Ne Naya Issue Diya

→ Java 8 se interfaces mein **default methods** allowed
→ 2 interfaces ne **same signature ka default method** define kiya
→ Class ne dono implement kiya — **compile error!**
→ Java confused: "A wala `show()` use karu ya B wala?"
→ Yehi **Diamond Problem** hai

```java
interface A { default void show() { System.out.println("A"); } }
interface B { default void show() { System.out.println("B"); } }

class C implements A, B { }     // COMPILE ERROR — kaunsa show()?
```

---

## Visualization — Diamond Shape

```
              Diamond Problem — 2 Interfaces, Same Default

                ┌──────────┐
                │ Interface│
                │    A     │
                │ default  │
                │ show()   │
                └──┬───────┘
                   │
                ┌──┴──┐
                │     │
            ┌───▼┐  ┌─▼──┐
            │  A │  │ B  │           ← 2 interfaces, same default
            │show│  │show│             (alag implementations)
            │"A" │  │"B" │
            └──┬─┘  └─┬──┘
               │      │
               └──┬───┘
                  ▼
               ┌─────────┐
               │ class C │           ← C dono implement karta
               │implements│
               │  A, B   │
               └─────────┘

  C ko show() chahiye:
       │
       ▼
  Java confused: "A wala use karu ya B wala?"
       │
       ▼
  COMPILE ERROR


╔════════════════════════════════════════════════════════════╗
║ Why Diamond Problem Sirf Default Methods Mein?             ║
╚════════════════════════════════════════════════════════════╝

  Java 8 SE PEHLE:
    interface A { void show(); }     ← sirf abstract
    interface B { void show(); }     ← sirf abstract
    class C implements A, B { }      ← C ne implement karna padega
                                       koi conflict nahi (kyunki body nahi)


  Java 8 KE BAAD:
    interface A { default void show() { print("A"); } }   ← body hai
    interface B { default void show() { print("B"); } }   ← body hai
    class C implements A, B { }                           ← BOTH bodies?
                                                            conflict
```

---

## Fix — Explicitly Override Karo

```java
class C implements A, B {
    @Override
    public void show() {
        A.super.show();         // ya B.super.show() — explicitly choose
    }
}
```

→ `A.super.show()` = A interface ka default method call karo
→ Class **explicitly bata** rahi kaunsa chahiye

---

## WHY Java Confused?

→ Class extends mein Java ne **single inheritance** rakhi (diamond avoid karne ke liye)
→ Lekin Java 8 mein interface mein default methods aaye → naya **diamond problem**
→ Fix: **Java force karta** class explicitly resolve kare

---

## POWER PHRASE

> *"When a class implements two interfaces with the same default method, the compiler can't decide which to use — Diamond Problem. Resolution: override the method explicitly and call the desired one via `InterfaceName.super.method()`."*
