# String — Immutable Kyun Hai?

> **V90 Section 1 — Topic 4**

---

## 🟡 WHY — String Immutable Kyun?

3 reasons:

1. **String Pool** possible ho sake → same value = same object → **memory save**
2. **Thread safe** → multiple threads same String use karein → change nahi ho sakti
3. **HashMap key safe** → key change ho jaaye → hashCode alag → value milega hi nahi!

---

## 📖 STORY — Login Bug

→ Ek baar login system mein bug aaya
→ Username validate karne ka function tha — usme pass kiya `"arpan"`
→ Function ne internally modify kiya
→ Wapas aaya toh original **corrupt** tha, wrong audit log gaya
→ Tab socha — String agar mutable hoti toh ye problem hamesha rehti
→ Java ne isliye String ko **immutable** banaya — ek baar bana, koi nahi badal sakta
→ Aur **String Pool** tab hi kaam karta hai jab immutable ho
→ warna ek ne badla, sab ke liye badal gaya

---

## 🧠 String Pool Visualization

```
STACK              HEAP — String Pool          HEAP (outside pool)
                   ┌────────────────┐
s1 ────────►       │   "Arpan"      │           ┌──────────────┐
                   │                │           │ new "Arpan"  │
s2 ────────►       │   (shared)     │   s3 ───► │  (separate)  │
                   │                │           └──────────────┘
                   │   "Singh"      │
                   └────────────────┘
```

- `s1 = "Arpan"`, `s2 = "Arpan"` → **SAME pool object** (`s1 == s2` → TRUE)
- `s3 = new String("Arpan")` → **DIFFERENT heap object** (`s1 == s3` → FALSE)
- `s1.equals(s3)` → TRUE (value same)

---

## 💻 Behaviour

```java
String s = "Arpan";
s = s + " Singh";
// "Arpan" wala object safe
// naya "Arpan Singh" bana
// s naye object ko point kare
```

```java
String s1 = "Arpan";              // pool mein bana
String s2 = "Arpan";              // SAME pool object
String s3 = new String("Arpan");  // pool bypass, naya heap

s1 == s2          // true  (pool)
s1 == s3          // false (heap)
s1.equals(s3)     // true  (value same)
```

---

## 🔴 TRAP

> **`==` = same OBJECT check. `.equals()` = same VALUE check.**
> **HAMESHA `.equals()` use karo strings compare karne ke liye.**

---

## 💬 POWER PHRASES

> *"String is immutable because it is a shared object — if one reference changes it, all references would be affected. Immutability enables the String Pool, thread safety, and safe use as HashMap keys."*

> *"String literals go to the String Pool — same value reuses the same object in memory. `new String()` forces a new heap object. Always use `.equals()` for value comparison, never `==`."*
