# Covariant Return Type

> **V90 — Brain Ticklers: Topic 58**

---

## Covariant Return Type Kya Hai?

**Simple line:** Override karte waqt **child class ka return type, parent ke return type ka SUBTYPE ho sakta hai.**

---

## Real-World Analogy

**Parent ka promise:** "Main tujhe **gaadi** dunga"
**Child kahta:** "Main tujhe **Honda Car** dunga"

Honda Car = Gaadi hi hai (specific gaadi). **Promise toot nahi raha** — tu happy.

Same in code:
- Parent: "Main `Animal` return karunga"
- Child: "Main `Dog` return karunga" (Dog = Animal hi hai)

---

## Java 5 Se Pehle vs Baad

### Before Java 5 — STRICT
```java
class Animal {
    Animal create() { return new Animal(); }
}

class Dog extends Animal {
    @Override
    Animal create() { return new Dog(); }    // allowed — return type EXACT same
}
```

Pehle: child ka return type **EXACTLY same** hona chahiye tha — `Animal`.

### Java 5+ — RELAXED (Covariant)
```java
class Dog extends Animal {
    @Override
    Dog create() { return new Dog(); }       // NOW ALLOWED — Dog IS-A Animal
}
```

Java 5 se: child ka return type **subtype** ho sakta hai. `Dog` allowed kyunki Dog IS-A Animal.

---

## Asli Faayda — No Casting

Bina covariant ke:
```java
class Dog extends Animal {
    Animal create() { return new Dog(); }    // return type Animal hi
}

Dog d = new Dog();
Animal a = d.create();        // Animal mila
Dog d2 = (Dog) a;             // ugly cast karna pada
```

**Covariant ke saath:**
```java
class Dog extends Animal {
    Dog create() { return new Dog(); }       // return type Dog
}

Dog d = new Dog();
Dog d2 = d.create();          // direct Dog mila — NO CAST
```

**Cleaner code, type-safe** — caller ko cast nahi karna padta.

---

## WHY Allow Hua?

→ Dog **Animal hai** (IS-A relationship)
→ Caller ne `Animal` expect kiya tha — Dog mil gaya, **Dog bhi Animal hi hai**
→ **Liskov substitution principle** — child must work where parent expected
→ Java 5+ se allowed

---

## Kya Allowed NAHI?

Return type **unrelated** type nahi ho sakta:

```java
class Animal {
    Animal create() { return new Animal(); }
}

class Dog extends Animal {
    String create() { return "..."; }    // INVALID — String Animal nahi hai
    Cat create() { return new Cat(); }   // INVALID — Cat Animal ka SIBLING, child nahi
}
```

**Sirf subtype** allowed — parent ki **family** mein hi rehna hai.

---

## Asli Use Case — Factory / Clone Methods

```java
class Document {
    Document copy() { ... }
}

class PDFDocument extends Document {
    @Override
    PDFDocument copy() { ... }    // PDF return karta — covariant
}

PDFDocument pdf = new PDFDocument();
PDFDocument duplicate = pdf.copy();   // no cast — clean
```

**Factory patterns** mein bahut common — specific subtype return karna useful.

---

## 1-Line Yaad

> **Covariant Return Type = override mein return type child ka subtype ho sakta. Caller ko cast nahi karna padta. Java 5+ feature.**

---

## POWER PHRASE

> *"Covariant return type allows an overriding method to return a subtype of the parent's return type — Java 5+ supports this for cleaner factory and clone methods, eliminating the need for explicit casting at the call site."*
