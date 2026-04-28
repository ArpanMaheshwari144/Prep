# final vs finally vs finalize

> **V90 — Brain Ticklers: Topic 57 (Interview Classic)**

---

## 🆚 Teeno Bilkul Alag Cheezein

| | `final` | `finally` | `finalize()` |
|--|--------|-----------|--------------|
| **Kya hai** | Keyword | Block | Method |
| **Use** | Variable/method/class lock | try-catch ke baad cleanup | GC se pehle call hota tha |
| **Kab chalta** | Compile time check | Try-catch ke baad hamesha | Object GC hone se pehle |

---

## 💻 Code

```java
// final — change nahi
final int x = 10;
x = 20;                             // 🔴 COMPILE ERROR

// finally — try-catch ke baad
try { riskyCall(); }
finally { conn.close(); }           // ✅ HAMESHA chalta

// finalize() — DEPRECATED Java 9+
@Override
protected void finalize() { ... }   // 🔴 use mat karo, try-with-resources use karo
```

---

## 🔴 TRAP — Naam Mat Confuse Karna

> Interview classic: **"final, finally, finalize mein farak?"**

| Naam | Ek Line Mein |
|------|--------------|
| **final** | Reassign / override / extend roko (variable / method / class) |
| **finally** | try-catch ke baad cleanup ka block |
| **finalize()** | GC se pehle call (Java 9+ DEPRECATED — bhool jao) |

---

## 💬 POWER PHRASE

> *"`final` is a keyword that prevents reassignment, overriding, or extension. `finally` is a try-catch block that always runs for cleanup. `finalize()` is a deprecated Object method that ran before garbage collection — use try-with-resources instead."*
