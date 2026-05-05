# Compile Time Constant Folding (String Trap)

> **V90 — Brain Ticklers: Topic 60**

---

## 📖 STORY — `==` Compare Mein Trap

```java
String s1 = "Hello";
String s2 = "Hel" + "lo";       // dono LITERALS

s1 == s2     // ✅ TRUE — kaise?
```

**Reason:** Compiler ne **compile time pe hi** `"Hel" + "lo"` calculate karke **`"Hello"` bana diya** → pool mein same object share. Yehi **compile-time constant folding** hai.

---

## 🧠 Visualization — Case 1 (Literals + Literals)

```java
String s1 = "Hello";
String s2 = "Hel" + "lo";   // compiler folds → "Hello"
```

**Compile time pe javac ne dekha:** `"Hel"` aur `"lo"` dono **constants** hain. Toh **compile time pe hi `"Hello"` bana diya**. Bytecode mein dono `s1` aur `s2` ko `"Hello"` literal mil gaya.

```
STACK                    HEAP — String Pool
                         ┌─────────────┐
s1  ─────────────────────►│  "Hello"   │
                         │             │
s2  ─────────────────────►│  (SHARED)  │
                         └─────────────┘

DONO same pool object pe point kar rahe!
s1 == s2  →  TRUE  ✅
```

---

## 🚨 Trap — Variable Beech Mein Aaya?

```java
String s1 = "Hello";
String x = "Hel";
String s3 = x + "lo";           // 🔴 x VARIABLE hai, literal nahi

s1 == s3     // ❌ FALSE — runtime pe naya object banta
```

**Variable runtime tak unknown** — compiler concat compile-time pe nahi kar sakta.

---

## 🧠 Visualization — Case 2 (Variable + Literal)

```java
String s1 = "Hello";    // pool
String x = "Hel";       // pool
String s3 = x + "lo";   // RUNTIME concat → naya heap object
```

**Compiler ka problem:** `x` ka value runtime tak nahi pata. **Compile-time fold nahi kar sakta**. Toh **JVM runtime pe**:
1. Internally `StringBuilder` banata
2. `x` ka value (`"Hel"`) append karta
3. `"lo"` append karta
4. `toString()` call karta — ye **NAYA String object heap mein** banata (pool mein NAHI)

```
STACK              HEAP — String Pool         HEAP (pool ke bahar)
                   ┌─────────────┐            ┌─────────────┐
s1  ──────────────►│  "Hello"    │            │  "Hello"    │
                   │             │            │   (NAYA     │
x   ──────────────►│  "Hel"      │            │   heap obj) │
                   └─────────────┘     ◄──────┤             │
                                              │             │
                                              └─────────────┘
                                              ▲
                                              │
                                          s3 ─┘
```

**Dono `"Hello"` content rakhte — but ALAG objects.**

```
s1 (pool ka "Hello") ≠ s3 (heap ka "Hello")
s1 == s3       →  FALSE   ❌  (alag objects)
s1.equals(s3)  →  TRUE    ✅  (content same)
```

---

## 🔑 Bottom Line

| Case | Compiler ka kaam | Result location | `==` ka result |
|------|------------------|-----------------|----------------|
| `"Hel" + "lo"` | **Compile-time fold** → "Hello" literal | Pool (shared) | ✅ true |
| `x + "lo"` | **Cannot fold** (x runtime variable) | Heap (new object) | ❌ false |

---

## 🔴 TRAP

> **Literals + literals** = compile-time fold = **same pool object** = `==` true
> **Variable + literal** = runtime concat = **naya heap object** = `==` false
> **HAMESHA `.equals()` use karo strings compare karne ko**

---

## 💬 POWER PHRASE

> *"Java compiler folds compile-time constant expressions into a single pool string — `"Hel" + "lo"` becomes `"Hello"` at compile time. But `variable + literal` is computed at runtime via StringBuilder, creating a new heap object outside the pool. Always use `.equals()` for content comparison."*
