# instanceof Operator

> **V90 — Core Java Extras: Topic 18**

---

## 📖 STORY — Polymorphism + Loop

→ Tune polymorphism use kiya
→ Animal reference mein **Dog, Cat, Bird** sab aa sakte hain
→ Loop mein processing karni thi — Dog pe `bark()`, Cat pe `meow()`. Seedha call karo?
→ **Compile error**
→ — Animal reference pe Dog ka method **nahi hota**
→ `instanceof` se pehle confirm karo ki **andar kya hai**, phir safely cast karo
→ `instanceof`: **object kis class ka hai?**
→ `Animal a = new Dog()` → `a instanceof Dog` = **true**
→ Cast karne se pehle check karo → `ClassCastException` avoid hoga

---

## 💻 Code

```java
Animal a = new Dog();          // andar Dog hai

a instanceof Dog               // ✅ true — andar Dog hai
a instanceof Animal            // ✅ true — Dog, Animal ka child hai
a instanceof Cat               // ❌ false — andar Cat nahi hai

// Real use — safely cast karne se pehle check karo
void makeSound(Animal a) {
    if (a instanceof Dog) {
        Dog d = (Dog) a;       // ✅ safe — pehle confirm kiya
        d.bark();
    } else if (a instanceof Cat) {
        Cat c = (Cat) a;
        c.meow();              // ✅ safe
    }
}
```

---

## 🔴 TRAP — Bina Check Cast Kiya

```java
Animal a = new Animal();
Dog d = (Dog) a;               // 🔴 ClassCastException — andar Dog hai hi nahi!

// ✅ Safe tarika
if (a instanceof Dog) {
    Dog d = (Dog) a;           // pehle check, phir cast
}
```

> **`instanceof` sirf tab kaam karta jab object bana ho. `extends` sirf blueprint hai.**
> **Bina object ke `instanceof` ka koi matlab nahi.**

---

## 🧠 Visualization

```
                    Class Hierarchy

                  ┌──────────┐
                  │  Animal  │  (parent)
                  └─────┬────┘
                        │ extends
              ┌─────────┼─────────┐
              ▼         ▼         ▼
          ┌──────┐  ┌──────┐  ┌──────┐
          │ Dog  │  │ Cat  │  │ Bird │
          └──────┘  └──────┘  └──────┘


            Reference Type vs Object Type

  Animal a = new Dog();
        ▲             ▲
        │             │
   reference        object
   type = Animal    type = Dog
   (compile-time)   (runtime)


            instanceof — Runtime Check

  a instanceof Dog       →  true   ✅  (object Dog hai)
  a instanceof Animal    →  true   ✅  (Dog, Animal ka child)
  a instanceof Cat       →  false  ❌  (object Cat nahi)


            Bina Check Cast = Crash

  Animal a = new Animal();           ← object Animal hai
  Dog d = (Dog) a;                   🔴 ClassCastException
                                       (andar Dog hai hi nahi)

            Safe Tarika

  if (a instanceof Dog) {            ← pehle confirm
      Dog d = (Dog) a;               ✅ ab safe
      d.bark();
  }
```

---

## 💬 POWER PHRASE

> *"`instanceof` checks if an object is an instance of a class or its subclass at runtime — always use it before downcasting to avoid ClassCastException."*
