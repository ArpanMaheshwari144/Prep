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


  METHOD AREA (class-level memory)
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

## POWER PHRASE

> *"Static members belong to the class, not to any instance — shared across all objects and accessible without creating an object."*
