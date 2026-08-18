# Functional Interface — Lambda Kahan Store?

> **V90 — Java 8: Topic 35**

---

## WHY — Lambda Store Kahan Hota?

→ Lambda ko **store karne ke liye special interface** chahiye — jisme **sirf 1 abstract method** ho
→ Isko **Functional Interface** bolte
→ `@FunctionalInterface` annotation lagao → compiler check karega, **agar 2 abstract methods likhe toh compile error**
→ Lambda → store → Functional Interface (sirf 1 abstract method)

---

## Custom Functional Interface

```java
@FunctionalInterface
interface MathOperation {
    int operate(int a, int b);     // sirf ek abstract method
}

MathOperation add      = (a, b) -> a + b;
MathOperation multiply = (a, b) -> a * b;

System.out.println(add.operate(5, 3));         // 8
System.out.println(multiply.operate(5, 3));    // 15
```

---

## Java 8 Ke 4 Built-In FIs — **PFCS** (Memory Trick)

```
P  Predicate    →  Filter (haan ya nahi)
F  Function     →  Convert (A se B)
C  Consumer     →  Action (karo, return nahi)
S  Supplier     →  Factory (banao, input nahi)
```

---

## Code — Char FIs

### 1. `Predicate<T>` — le: T, de: boolean (test karo)
```java
Predicate<String> isEmpty = s -> s.isEmpty();
isEmpty.test("");           // true
isEmpty.test("Arpan");      // false
```

### 2. `Function<T, R>` — le: T, de: R (transform karo)
```java
Function<String, Integer> length = s -> s.length();
length.apply("Arpan");      // 5
```

### 3. `Consumer<T>` — le: T, de: kuch nahi (use karo, return nahi)
```java
Consumer<String> print = s -> System.out.println(s);
print.accept("Arpan");      // Arpan
```

### 4. `Supplier<T>` — le: kuch nahi, de: T (produce karo)
```java
Supplier<String> greet = () -> "Hello";
greet.get();                // Hello
```

---

## TRAP — Method Names Mix Mat Karna

> **`Predicate.test()`** — boolean
> **`Function.apply()`** — transform
> **`Consumer.accept()`** — action (void)
> **`Supplier.get()`** — produce

**Interview mein specifically poochhte:**
- `filter()` ke andar kya jaata? → **Predicate** (`test()`)
- `map()` ke andar? → **Function** (`apply()`)
- `forEach()` ke andar? → **Consumer** (`accept()`)

---

## PFCS Yaad Rakhne Ka Trick

```
Predicate  → "Filter karo. Le: T → De: boolean"     (haan / nahi)
Function   → "Convert karo. Le: T → De: R"          (T se R)
Consumer   → "Action karo. Le: T → De: void"        (karo, return nahi)
Supplier   → "Factory hai. Le: nothing → De: T"     (banao, input nahi)
```

---

## POWER PHRASE

> *"A Functional Interface has exactly one abstract method — it is the target type for lambda expressions. `@FunctionalInterface` is a compiler check that ensures only one abstract method exists. Java 8 provides four core functional interfaces: Predicate for boolean test, Function for transformation, Consumer for side-effect actions with no return, and Supplier for object creation with no input. These are the building blocks used heavily in Streams API."*

> **Yaad rakh:**
> Functional Interface = sirf 1 abstract method
> Lambda store karne ki jagah
> PFCS = Predicate, Function, Consumer, Supplier
> Streams ka backbone
