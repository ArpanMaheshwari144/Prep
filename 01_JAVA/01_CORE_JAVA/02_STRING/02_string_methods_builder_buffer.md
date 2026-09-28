# String Methods + StringBuilder vs StringBuffer

> **V90 Section 1 — Topic 5**

---

## WHY — String Methods Original Nahi Badalte

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

## String vs StringBuilder vs StringBuffer

| Class | Detail |
|-------|--------|
| **String** | **Immutable**. Har change = naya object. Loop mein = memory waste |
| **StringBuilder** | **Mutable**. Thread safe NAHI. **Fast** — mostly yahi use karo |
| **StringBuffer** | **Mutable**. Thread safe HAI. Thoda slow — **sirf multithreading** mein |

---

## STORY (V90 ka rule)

→ **Loop mein concat** karte ho String se → har iteration nayi object → memory waste
→ Use **StringBuilder** for string manipulation in loops — it is mutable and faster
→ Use **StringBuffer** ONLY in multithreaded code where thread safety is needed
→ String = shared → ek ne badla toh sabka badle → **isliye immutable**
→ **Compare = `.equals()`**. **Loop = StringBuilder**.

---

## ★ TRAP — length / length() / size() — 3 alag cheezein

```java
int[] arr = {1, 2, 3};
arr.length;          // 3 — FIELD (no parens)

String s = "Arpan";
s.length();          // 5 — METHOD (parens)

List<Integer> list = new ArrayList<>();
list.size();         // METHOD (parens)
```

```
Array  → length      (field, no parens)
String → length()    (method)
List   → size()      (method)

Trap: array.length()  → compile error
      string.length   → compile error
```

**Array ka `length` FIELD kyun, method kyun nahi?**
```
Array = SPECIAL JVM object — JVM banata, koi "Array class ka source" nahi (internal construct)
Length: creation pe decide (new int[5]), kabhi NAHI badalti = fixed size
FIELD:  arr.length   → direct memory read, O(1) — JVM array HEADER mein store karta
METHOD: arr.length() → invocation overhead (chhota par bekaar)

   int[] arr = new int[5];
   ┌──────────────────────────────┐
   │ Array header: length: 5      │  ← stored field
   │ data: [0, 0, 0, 0, 0]        │
   └──────────────────────────────┘

String contrast — String proper class hai, encapsulation ke liye method:
   class String {
       private final byte[] value;   // Java 8 tak char[], Java 9+ byte[] (compact strings)
       public int length() { return value.length; }   ← andar array ka FIELD hi wrap kiya
   }
```

---

## POWER PHRASES

> *"String methods always return a NEW string — original is never modified because String is immutable."*

> *"Use StringBuilder for string manipulation in loops — it is mutable and faster. Use StringBuffer only in multithreaded code where thread safety is needed."*
