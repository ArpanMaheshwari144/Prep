# Type Casting — Upcasting & Downcasting

> **V90 — Core Java Extras: Topic 22**

---

## 📖 STORY — Animal Reference

→ Tune Animal reference mein **Dog store kiya** — Upcasting (automatic, safe)
→ Ab Dog ke special methods chahiye — `bark()`
→ Animal mein nahi hai. **Downcast karna padega**
→ **Lekin bina check ke kiya toh `ClassCastException`** runtime pe aata, compiler nahi pakdta
→ Isliye **hamesha `instanceof` pehle, phir cast**
→ `Animal a = new Dog()` → Dog andar hai, **Animal reference**
→ **Upcasting:** automatic, safe
→ **Downcasting:** manual, ClassCastException risk → `instanceof` se pehle check

---

## 💻 Code

### Upcasting — Child → Parent reference. Automatic. Always safe.
```java
Animal a = new Dog();      // ✅ Upcasting — automatic, koi cast nahi
a.eat();                   // ✅ Animal method — Animal reference se chalega
a.bark();                  // 🔴 bark() Animal mein nahi hai!
```

### Downcasting — Parent reference → Child type. Manual. Cast likhna padta.
```java
Animal a = new Dog();
Dog d = (Dog) a;           // ✅ Downcasting — andar Dog hai, safe
d.bark();                  // ✅ Dog method ab accessible

// 🔴 TRAP — andar Dog nahi hai
Animal a = new Animal();
Dog d = (Dog) a;           // 🔴 ClassCastException — andar Animal hai!

// ✅ Safe tarika — instanceof pehle
if (a instanceof Dog) {
    Dog d = (Dog) a;       // ✅ safe — confirm karke cast
    d.bark();
}
```

---

## 🧠 Visualization

```
              Type Casting Visualization

╔════════════════════════════════════════════════════════════╗
║ UPCASTING — Child → Parent (automatic, safe)               ║
╚════════════════════════════════════════════════════════════╝

      Dog dog = new Dog();
      Animal a = dog;          ◄── automatic upcast

  STACK            HEAP
                   ┌────────────┐
   dog ──────────►│            │
                   │  Dog obj   │   ← Dog = real object
   a   ──────────►│   (full)   │     dono SAME object pe point
                   └────────────┘     (sirf reference type alag)

  a.eat()    ✅  (Animal method, accessible)
  a.bark()   🔴  COMPILE ERROR — Animal mein bark() nahi


╔════════════════════════════════════════════════════════════╗
║ DOWNCASTING — Parent → Child (manual, risky)               ║
╚════════════════════════════════════════════════════════════╝

      Animal a = new Dog();      ← upcast pehle
      Dog d = (Dog) a;           ← manual downcast

   STACK            HEAP
                   ┌────────────┐
   a ────────────►│            │
                   │  Dog obj   │
   d ────────────►│            │   ← d ab Dog methods access kar sakta
                   └────────────┘

  d.bark()   ✅  (ab Dog reference, bark accessible)


╔════════════════════════════════════════════════════════════╗
║ DOWNCAST CRASH — andar Dog hai hi nahi                     ║
╚════════════════════════════════════════════════════════════╝

      Animal a = new Animal();   ← real object Animal
      Dog d = (Dog) a;           🔴 ClassCastException!
                                   (cast to Dog but object Animal hai)

   STACK            HEAP
                   ┌────────────┐
   a ────────────►│ Animal obj │   ← Dog NAHI, simple Animal
                   └────────────┘

   Dog ka bark() exist hi nahi → runtime crash


╔════════════════════════════════════════════════════════════╗
║ SAFE TARIKA — instanceof check                             ║
╚════════════════════════════════════════════════════════════╝

      if (a instanceof Dog) {
          Dog d = (Dog) a;       ← confirm karke cast
          d.bark();
      }
```

---

## 📊 Summary

| | Upcasting | Downcasting |
|--|-----------|-------------|
| **Direction** | Dog → Animal | Animal → Dog |
| **Cast likhna?** | NO (automatic) | YES (manual) |
| **Safe?** | Always | Bina check → ClassCastException |
| **Methods** | Sirf parent ke | Child ke extra access |

---

## 🔴 TRAP 1

> **Upcasting mein child ke extra methods access nahi hote.**
> `a.bark()` → compile error even if Dog andar hai!

## 🔴 TRAP 2

> **`ClassCastException` runtime pe aata hai — compiler nahi pakdta.**
> Isliye `instanceof` check **mandatory** hai.

---

## 💬 POWER PHRASE

> *"Upcasting is automatic and always safe — a child reference can always be assigned to a parent type. Downcasting is manual and requires an `instanceof` check first — without it you risk a ClassCastException at runtime."*

> **Yaad rakh:** Upcast = automatic. Downcast = manual + `instanceof` check. Bina check → ClassCastException.
