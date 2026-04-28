# String Methods + StringBuilder vs StringBuffer

> **V90 Section 1 — Topic 5**

---

## 🟡 WHY — String Methods Original Nahi Badalte

→ String **immutable** hai → koi method (concat, toUpperCase, replace) **original nahi badalti**
→ Hamesha **NEW String return** karte hain
→ `s.concat(" World")` karo aur result save nahi kiya → **original `s` unchanged**
→ **Hamesha `s = s.concat(...)`** likhna padta

```java
String s = "Hello";
s.concat(" World");      // ← return value lost!
System.out.println(s);   // "Hello"  (unchanged)

s = s.concat(" World");  // ← ab assign kiya
System.out.println(s);   // "Hello World"
```

---

## 📊 String vs StringBuilder vs StringBuffer

| Class | Detail |
|-------|--------|
| **String** | **Immutable**. Har change = naya object. Loop mein = memory waste ❌ |
| **StringBuilder** | **Mutable**. Thread safe NAHI. **Fast** — mostly yahi use karo ✅ |
| **StringBuffer** | **Mutable**. Thread safe HAI. Thoda slow — **sirf multithreading** mein |

---

## 📖 STORY (V90 ka rule)

→ **Loop mein concat** karte ho String se → har iteration nayi object → memory waste
→ Use **StringBuilder** for string manipulation in loops — it is mutable and faster
→ Use **StringBuffer** ONLY in multithreaded code where thread safety is needed
→ String = shared → ek ne badla toh sabka badle → **isliye immutable**
→ **Compare = `.equals()`**. **Loop = StringBuilder**.

---

## 💬 POWER PHRASES

> *"String methods always return a NEW string — original is never modified because String is immutable."*

> *"Use StringBuilder for string manipulation in loops — it is mutable and faster. Use StringBuffer only in multithreaded code where thread safety is needed."*
