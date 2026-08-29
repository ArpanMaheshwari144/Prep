# Access Modifier Override Rule — Same ya Wider, Narrow Nahi

> **V90 — Brain Ticklers: Topic 59**

---

## STORY

→ Parent class mein method `public` tha. Child ne override karke **`private` kar diya** — **compile error**
→ Rule: **Child mein access modifier SAME ya WIDER ho sakta**, NARROWER nahi

```java
// GALAT — narrower
class Parent { public void show() { } }
class Child extends Parent {
    private void show() { }      // COMPILE ERROR — public se private = narrow
}

// SAHI — same ya wider
class Parent { protected void show() { } }
class Child extends Parent {
    public void show() { }       // protected → public = WIDER, valid
}
```

---

## WHY?

→ Caller ne **parent reference se** call kiya:
```java
Parent p = new Child();
p.show();      // public expect kiya
```
→ Agar child ne `private` kar diya — caller ko **expected access nahi mila** → contract toot gaya
→ Liskov substitution — child must honour parent's contract

---

## Allowed Transitions

```
Parent              Child (allowed)
private          override hi nahi hota
default      →   default, protected, public
protected    →   protected, public
public       →   public ONLY
```

---

## POWER PHRASE

> *"Override mein child ka access modifier SAME ya WIDER hona chahiye — never narrower. Public can only stay public, protected can widen to public, private cannot be overridden at all."*
