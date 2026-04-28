# abstract keyword

> **V90 — Core Java Extras: Topic 16**

---

## 📖 STORY — Shape Class

→ Tune `Shape` class banayi. `area()` method likhna chahta tha
→ Lekin `Shape` abstract hai — **kaunsa formula?**
→ **Circle ka alag, Rectangle ka alag**
→ `Shape` khud area calculate nahi kar sakti
→ — **sirf ye guarantee deti hai ki har shape ka area hoga**
→ **abstract method = body nahi, sirf declaration**
→ Child implement kare — **warna child bhi abstract ban jayegi**
→ `Shape` → `area()` pata nahi. Circle → `3.14*r*r`. Rectangle → `l*b`
→ `Shape` abstract → body nahi, child implement kare. `new Shape()` → **error**

---

## 💻 Code

```java
abstract class Shape {
    abstract double area();              // body nahi — sirf declaration
    void display() { /* normal method */ }   // ye likh sakti hai
}

class Circle extends Shape {
    double area() { return 3.14 * r * r; }   // ✅ implement kiya
}

class Rectangle extends Shape {
    double area() { return l * b; }          // ✅ implement kiya
}
```

---

## 🔴 TRAP 1 — `static abstract` = ILLEGAL Combo

→ `static` = class ka, override nahi hota
→ `abstract` = child implement kare
→ Dono **contradictory** — ek saath nahi chal sakte

```java
abstract class A {
    static abstract void show();    // 🔴 COMPILE ERROR
}
```

---

## 🔴 TRAP 2 — Interface Mein Private Method (Java 9+)

→ Java 9 se interface mein **private methods allowed**
→ Sirf **internal helper** ke liye — default/static methods ke andar use hote hain
→ **Bahar se access nahi hota**

```java
interface A {
    private void helper() { ... }                    // ✅ Java 9+ valid
    default void show() { helper(); }                // sirf andar use karo
}
```

---

## 🔴 TRAP 3 — Abstract Class ka Object KABHI Nahi Banta

→ Chahe **ek bhi abstract method na ho**
→ Agar class `abstract` declare ki hai → direct object nahi ban sakta
→ Hamesha **compile error**

```java
abstract class A { }                  // koi abstract method nahi
new A();                              // 🔴 COMPILE ERROR
```

**WHY?**
→ Abstract class = **adhoori hai** ya tu khud ne bola "ye sirf extend karne ke liye hai"
→ Soch: `Vehicle` ka `start()` abstract hai → `new Vehicle()` pe `start()` mein **kya chalega?**
→ **Kuch nahi.** Isliye Java rokta hai — **adhoori cheez se real object mat banao**

---

## 💬 POWER PHRASE

> *"Abstract class is an incomplete blueprint — it can have both abstract methods with no body and normal methods. Subclasses must implement all abstract methods. You cannot instantiate an abstract class directly."*
