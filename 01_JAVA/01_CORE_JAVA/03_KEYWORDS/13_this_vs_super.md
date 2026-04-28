# this vs super

> **V90 — Core Java Extras: Topic 13**

---

## 📖 STORY — Dog extends Animal

→ Tune Dog class banayi — extends Animal
→ Dog ke constructor mein **Animal ko bhi initialize** karna tha — naam, age
→ `super(name, age)` se parent ka constructor call kiya
→ Aur Dog ke andar apna `name` field tha, **Animal mein bhi** `name`
→ `this.name` = Dog ka, `super.name` = parent ka
→ **Bina ye samjhe** constructor chaining mein **compile errors** aate
→ **`this` = current object ka reference**
→ **`super` = parent ka reference**
→ `super()` → **pehli line mein hona chahiye** constructor mein

---

## 💻 Code

```java
class Animal {
    String name = "Animal";
    Animal() { System.out.println("Animal created"); }
}

class Dog extends Animal {
    String name = "Dog";

    Dog() {
        super();                            // parent constructor call
        System.out.println(super.name);     // "Animal" — parent ka field
        System.out.println(this.name);      // "Dog" — apna field
    }
}
```

---

## 🔴 TRAP 1

> **`super()` HAMESHA constructor ki PEHLI line mein honi chahiye — warna compile error!**

## 🔴 TRAP 2

> **Static method mein `this` aur `super` DONO nahi — dono object se related hain.**

---

## 💬 POWER PHRASE

> *"`this` refers to the current object. `super` refers to the parent class — used to call the parent constructor or access parent fields hidden by the child."*
