# Wrapper Classes — Autoboxing & Unboxing

> **V90 — Core Java Extras: Topic 15**

---

## STORY — List<int>?

→ Tune integers ki list banayi — `List<int>` → **compile error**
→ **Collections mein sirf objects jaate**, primitives nahi
→ **Integer wrapper class** use karo
→ Aur Java itna smart hai ki ye conversion **khud karta** — `Integer a = 10` likhte hi Java ne `Integer.valueOf(10)` kar diya
→ Ye **autoboxing** hai
→ **Ek trap** — Integer cache `-128 to 127` tak hai. Uske baad `==` se compare karo toh **false** aayegi

---

## WHY — `-128 se 127` Cache?

→ Loop counters, array indexes, age, quantity — **ye numbers BAHUT zyada use hote**
→ Java ne socha — **har baar naya object banane se memory waste** hogi
→ **Toh ye common range pehle se cache** kar li
→ Bahar ka number rare hai, uske liye naya object **theek** hai
→ Collections mein objects chahiye → int nahi, **Integer chahiye**
→ **Autoboxing:** Java khud `Integer.valueOf(10)` kar deta
→ **Unboxing:** Java khud `intValue()` kar deta

---

## Mapping

| Primitive | Wrapper |
|-----------|---------|
| `int` | `Integer` |
| `double` | `Double` |
| `char` | `Character` |
| `boolean` | `Boolean` |

---

## Code

```java
// Autoboxing — Java khud suit pehna deta hai
Integer a = 10;        // tune int diya → Java ne Integer.valueOf(10) kiya 

// Unboxing — Java khud suit utaar deta hai
int b = a;             // tune Integer diya → Java ne a.intValue() kiya 

// Integer cache: -128 to 127 → SAME object
Integer x = 127;
Integer y = 127;
x == y                 // true — same cached object

// Bahar range — naye objects
Integer p = 128;
Integer q = 128;
p == q                 // false — naye alag objects
p.equals(q)            // true
```

---

## Visualization — Integer Cache

```
              Integer Cache — JVM Pre-Built Range

        -128 ─────────────────────────────────► 127
        ┌────────────────────────────────────────┐
        │  cache: same object reuse              │
        │  Integer.valueOf(127) → SAME instance  │
        └────────────────────────────────────────┘
                      ▲
                      │
       Integer a = 100;  ─────┐
                              ├─► dono SAME object (cache se)
       Integer b = 100;  ─────┘
                              ▼
                       a == b  →  TRUE


       128, 200, 1000... (cache range bahar)
       ┌──────────┐    ┌──────────┐
       │ Integer  │    │ Integer  │   ← naye-naye objects
       │ p = 200  │    │ q = 200  │     har baar new
       └──────────┘    └──────────┘
              ▲              ▲
              │              │
          Integer p     Integer q

          p == q       →  FALSE (alag objects)
          p.equals(q)  →  TRUE  (content same)
```

---

## TRAP

> **Integer compare karne mein bhi HAMESHA `.equals()` use karo.**
> **`==` kabhi true kabhi false dega!**

---

## POWER PHRASE

> *"Wrapper classes wrap primitives as objects — needed for Collections. Autoboxing converts primitive to wrapper automatically via `Integer.valueOf()`. Unboxing does the reverse via `intValue()`. Integer caches -128 to 127 — always use `.equals()` for comparison."*
