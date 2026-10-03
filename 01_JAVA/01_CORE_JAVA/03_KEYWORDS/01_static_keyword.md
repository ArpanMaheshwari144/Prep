# static keyword

> **V90 — Core Java Extras: Topic 11**

---

## STORY — School ka Naam

→ Tune Student class banayi
→ School ka naam — har student ka same hai **"DPS"**
→ Agar har Student object apna alag school naam store kare → **1000 students = 1000 baar same string** memory mein
→ Bekar. **static field = class ka property** — sab share karte hain, **ek baar memory**
→ **static method = object banaye bina call karo**
→ `Math.sqrt()` yaad karo — `new Math()` nahi kiya kabhi
→ **static = class ka** → sab share karte hain
→ **Non-static = object ka** → har object ka apna
→ `Student.school` → object ki zarurat nahi

---

## Code

```java
class Student {
    static String school = "DPS";   // class ka — sab share
    String name;                    // student ka — har ek ka apna
}

Student.school   // object ki zarurat nahi — directly access
```

---

## Visualization — Class-level vs Instance-level Memory

```
              static vs Non-Static — Memory Layout

  class Student {
      static String school = "DPS";    ← class ka (sab share)
      String name;                      ← object ka (har ek apna)
  }


  CLASS-LEVEL memory (Java 8+: static field Class object ke saath HEAP me; class ki info METASPACE me)
  ┌──────────────────────────────────┐
  │ Student class                    │
  │   static school = "DPS"   ◄──┐   │   ← ek hi copy, sab access kare
  └──────────────────────────────┼───┘
                                 │
                                 │ shared
                                 │
  HEAP (object memory)           │
  ┌────────────────┐             │
  │ Student obj 1  │             │
  │   name="Arpan" │             │
  │   ──────────── ─┼────────────┤
  └────────────────┘             │
                                 │
  ┌────────────────┐             │
  │ Student obj 2  │             │
  │   name="Rahul" │             │
  │   ──────────── ─┼────────────┤
  └────────────────┘             │
                                 │
  ┌────────────────┐             │
  │ Student obj 3  │             │
  │   name="Priya" │             │
  │   ──────────── ─┼────────────┘
  └────────────────┘

  → School field 1 jagah, sab objects share karte
  → Naam field har object ka apna


╔════════════════════════════════════════════════════════════╗
║ Access                                                     ║
╚════════════════════════════════════════════════════════════╝

  Student.school           class se direct access (no object needed)
  arpan.name               object se access
  arpan.school            chalega lekin bad practice — object se mat karo
  Student.name             static nahi hai, class se access nahi


╔════════════════════════════════════════════════════════════╗
║ Static method mein 'this' nahi                             ║
╚════════════════════════════════════════════════════════════╝

  static void hello() {
      this.name;          ← 'this' = object ka reference
                              static method object se nahi juda
                              EXIST hi nahi karta object
  }
```

---

## TRAP

> **Static method mein `this` use nahi kar sakte — `this` = object ka reference.**
> **Static method tab exist karta hai jab object bana hi nahi hota!**

---

## ★ TRAP — Initialization order (static block / instance block / constructor)

```
Ek class ke andar:
   STATIC fields + STATIC blocks   -> class load pe, EK baar, jis KRAM me likhe hain usi kram me
   INSTANCE fields + INSTANCE blocks -> har object pe, likhe kram me
   CONSTRUCTOR body                -> uske baad

Parent + Child (new Child() — chala ke dekha):
   1 Parent static        (sirf pehli baar)
   2 Child static         (sirf pehli baar)
   3 Parent instance block
   4 Parent constructor
   5 Child instance block
   6 Child constructor

   doosra new Child()  -> sirf 3,4,5,6 (static dobara nahi)
   ★ trap: Parent ka CONSTRUCTOR, Child ke instance block se PEHLE chalta (super() pehle)
```

(Static method "override" = hiding, poora = `01_OOP/04_overloading_vs_overriding.md`)

---

## POWER PHRASE

> *"Static members belong to the class, not to any instance — shared across all objects and accessible without creating an object."*
