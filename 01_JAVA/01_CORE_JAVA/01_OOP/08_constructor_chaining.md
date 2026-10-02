# Constructor Chaining

> **V90 — Core Java Extras: Topic 20**

---

## STORY — Student class

→ Tune Student class banayi — **teen constructors**: naam only, naam+age, naam+age+course
→ Teeno mein **same initialization code** — validation, defaults
→ Kal validation badlegi toh **teeno mein change** karna padega
→ `this()` se **ek constructor doosre ko call** karo — code ek jagah, baaki delegate
→ `this()` = same class ka constructor call
→ `super()` = parent class ka constructor call
→ **Hamesha pehli line mein** hona chahiye. **Dono ek saath ek constructor mein nahi**

---

## Code

```java
class Student {
    String name; int age; String course;

    Student(String name) {
        this(name, 18);                       // → doosra constructor call
    }

    Student(String name, int age) {
        this(name, age, "Java");              // → teesra constructor call
    }

    Student(String name, int age, String course) {
        this.name = name;                     // ← actual kaam yahan hota
        this.age = age;
        this.course = course;
    }
}

Student s = new Student("Arpan");
// "Arpan" → age default 18 → course default "Java"
```

```java
// Parent class mein — super() se
class Dog extends Animal {
    String breed;
    Dog(String name, String breed) {
        super(name);                          // → Animal ka constructor call
        this.breed = breed;
    }
}
```

---

## TRAP

> **`this()` ya `super()` HAMESHA constructor ki PEHLI line!**

```java
Dog(String name, String breed) {
    System.out.println("hello");              // compile error!
    super(name);                              // pehli line nahi
}
```

> **Ek constructor mein dono ek saath nahi aa sakte — ya `this()` ya `super()`.**

(Naya Java: JDK 25 "flexible constructor bodies" — `super()` se pehle kuch statements likh sakte,
 jaise argument validate karna, par `this` use nahi kar sakte. Interview me classic rule bolo,
 naya pooche to ye line.)

## ★ TRAP — `super()` na likho to compiler KHUD daal deta

```java
class Animal {
    Animal(String name) { }          // sirf ye constructor, no-arg WALA NAHI
}
class Dog extends Animal {
    Dog() { }                        // compiler andar "super();" daalta
}                                    // -> Animal() hai hi nahi -> COMPILE ERROR
```
Fix: `Dog() { super("Tommy"); }` ya Animal me no-arg constructor jodo.
Kram: `new Dog()` -> pehle Animal ka constructor chalta, phir Dog ka (parent pehle).

---

## ★ TRAP — Constructor overload ho sakta, OVERRIDE kabhi nahi

```
Overload constructor:    YES (different params)
   public User() { ... }
   public User(String name) { ... }
   public User(String name, int age) { ... }

Override constructor:    NEVER POSSIBLE — constructors inherit hi nahi hote
```

**Kyun?** Override ke liye chahiye: (1) method parent se INHERIT hua ho (2) child SAME signature se redefine kare (3) runtime polymorphism.
Constructor: (1) inherit NAHI hota (2) naam = CLASS ka naam → Parent() aur Child() = alag naam = alag pehchaan.

```java
class Animal {
    public Animal() {           // name: Animal
    }
}
class Dog extends Animal {
    public Dog() {              // name: Dog (NOT Animal!)
        super();                // parent ka CALL karta, override nahi
    }
}
```

```
Constructor ka kaam = APNI class ke fields init karna (per-class concern)
CAN:    OVERLOAD (same class mein kai) · CHAIN via this()/super()
CANNOT: Override

Side note — ye bhi override nahi hote:
   • static method  (class-level → hiding hota hai)
   • final method   (parent ne lock kiya)
   • private method (child ko dikhta hi nahi)
```

---

## POWER PHRASE

> *"Constructor chaining allows one constructor to call another using `this()` for the same class or `super()` for the parent class — must always be the first statement in the constructor."*
