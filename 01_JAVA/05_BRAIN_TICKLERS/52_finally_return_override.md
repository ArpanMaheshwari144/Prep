# `finally` Mein Return — Sab Override Kar Deta

> **V90 — Brain Ticklers: Topic 52**

---

## 📖 STORY

```java
int test() {
    try { return 1; }
    catch(Exception e) { return 2; }
    finally { return 3; }
}
// Result: 3 — finally ka return SAB OVERRIDE!
```

→ try mein `return 1` "ready" hua — JVM ne pakad ke rakha
→ Lekin `finally` HAMESHA chalta hai return **se PEHLE**
→ `finally` mein bhi return hai → **purana return discard, naya set**
→ **3 return hota.** Bad practice — try ka return chup-chap udd jaata

---

## 🔴 TRAP

> **`finally` mein return KABHI mat likho.**
> **`finally` = cleanup (close connections, release resources) — return NAHI.**

---

## 🟡 WHY?

→ try ne 1 set kiya, lekin finally ne **override** kar diya
→ Debug nightmare — log mein 1 dikhta tha pehle, ab 3 — kahan se aaya?
→ **Convention:** try/catch return karte. finally sirf cleanup.

---

## 💬 POWER PHRASE

> *"A `return` inside `finally` silently overrides any return from `try` or `catch` — never put a return in `finally`. Use it only for cleanup."*
