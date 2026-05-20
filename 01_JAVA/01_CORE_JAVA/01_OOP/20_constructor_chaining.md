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

---

## POWER PHRASE

> *"Constructor chaining allows one constructor to call another using `this()` for the same class or `super()` for the parent class — must always be the first statement in the constructor."*
