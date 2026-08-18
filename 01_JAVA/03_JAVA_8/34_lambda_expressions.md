# Lambda Expressions — Anonymous Function

> **V90 — Java 8: Topic 34**

---

## STORY — Comparator Boilerplate

→ Ek din manager ne bola — **"100 patients ki list age se sort karo."**
→ Purana style mein Comparator likhne baitha
→ — anonymous inner class, `@Override`, `compare` method, **10 lines sirf sorting ke liye**
→ Sochne laga — itna code sirf ek kaam ke liye? **Tab Lambda aaya**
→ **Ek line mein** kaam ho gaya
→ Lambda ke bina **Streams nahi chalti** — isliye pehle ye

---

## WHY — Lambda Kyu?

→ Purana style — Comparator likhne mein **10 lines boilerplate**
→ Anonymous inner class, `@Override`, return statement — **sab ek kaam ke liye**
→ Lambda ne ye sab **hata diya**
→ **Naam nahi, class nahi, seedha logic**
→ **Lambda = anonymous function** — naam nahi, class nahi, sirf `(parameters) -> { body }`
→ Boilerplate hatao → code readable banao → **Streams API** use kar pao

---

## Old vs New

### Java 7 — 10 lines boilerplate
```java
Collections.sort(patients, new Comparator<Patient>() {
    @Override
    public int compare(Patient a, Patient b) {
        return a.getName().compareTo(b.getName());
    }
});
```

### Java 8 Lambda — 1 line
```java
Collections.sort(patients, (a, b) -> a.getName().compareTo(b.getName()));
```

---

## Lambda Syntax — 4 Forms

```java
// 1. Zero parameter
() -> System.out.println("Hello")

// 2. Ek parameter — brackets optional
name -> System.out.println(name)
(name) -> System.out.println(name)        // same

// 3. Do parameters — return implicit
(a, b) -> a + b

// 4. Multiple lines — curly braces + explicit return
(a, b) -> {
    int sum = a + b;
    return sum;
}
```

---

## ★ KYUN CHALTA + BODY (samajh — 3 point)

### 1. Lambda KYUN chalta → FUNCTIONAL INTERFACE (sirf 1 method)
→ Lambda tabhi likh sakte jab interface pe **sirf EK abstract method** ho (= **functional interface**).
→ Lambda us **akele method ka BODY** deta.
   `Runnable` → 1 method `run()` → `() -> {...}` = us `run()` ka body.
→ **2 method** hote → lambda **NAHI** chalti (compiler confuse: kaun sa method?).

### 2. Lambda body `{}` = normal code block (kitne bhi operations)
→ `{}` ke andar **jo chahe, jitna chahe** — statements, loops, if-else, synchronized. **ek-operation ki limit NAHI.**
```java
() -> {
    op1();  op2();  int x = calc();
    for (int i=0;i<100;i++) count++;    // 10-20 operation, sab EK lambda me
}
```
→ ★ "1 method" ka matlab: lambda ek **METHOD** fill karta — **NA ki 1 operation.** body me unlimited.

### 3. Type INFERENCE — Runnable alag se likhna zaroori nahi
→ `new Thread(() -> {...})` — yahan `Runnable` likha hi nahi.
→ compiler `Thread` constructor ke **param-type** se INFER kar leta (wo Runnable maangta) → lambda = Runnable.
```java
Runnable r = () -> {...};  new Thread(r);   // explicit
new Thread(() -> {...});                    // inferred (short) -- dono SAME
```

---

## TRAP — Java mein `=>` NAHI hota

> **Java = `->` (dash arrow). JavaScript = `=>` (fat arrow). Java mein `=>` EXIST nahi karta.**
> Sawaal: lambda syntax? `->` chun. `=>` dikhe = **GALAT**.

---

## TRAP — Lambda Ke Bina Streams Nahi

> **Streams API har jagah Lambda use karti — `filter`, `map`, `forEach` sab Lambdas leti hain.**
> Pehle Lambda samjho, fir Streams.

---

## POWER PHRASE

> *"Lambda expressions are anonymous functions that reduce boilerplate by replacing anonymous inner classes with a concise syntax — `(parameters) -> body`. They enable functional programming in Java and are the foundation for Streams API."*

> **Yaad rakh:**
> Lambda = anonymous function, no name no class
> `(params) -> { body }`
> `->` (Java) ≠ `=>` (JS)
> Lambda = boilerplate killer + Streams ka foundation
