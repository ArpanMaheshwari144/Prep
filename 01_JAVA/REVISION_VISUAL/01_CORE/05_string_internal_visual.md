# 📜 String Internal — Visual Revision

---

## 1️⃣ Strings Are IMMUTABLE

```
Imagine STONE TABLET:
   ┌────────────────────────┐
   │  🪨  "Hello"            │
   └────────────────────────┘
   
   Once carved → CANNOT change
   Want "Hello World"?
      → Make NEW tablet
      → Old tablet untouched
```

### Java Code
```java
String s = "Hello";
s.concat(" World");           // creates NEW string

System.out.println(s);        // "Hello"  ← unchanged!

// Right way:
s = s.concat(" World");       // reassign reference
System.out.println(s);        // "Hello World"
```

```
String methods (concat, replace, etc.) NEVER modify original
   → Always return NEW string
```

---

## 2️⃣ String Pool (Memory Optimization)

### Library Analogy
```
Without pool:
   Arpan ko chahiye "Java book"   → nayi copy
   Suresh ko chahiye "Java book"  → ek aur copy
   Mukesh ko chahiye "Java book"  → ek aur
   = 3 copies, waste

With pool (smart library):
   Library mein ek master "Java book" rakha
   Sab → SAME book share
   = 1 copy
```

### Java mein Same
```java
String s1 = "Hello";    // pool mein "Hello" daala
String s2 = "Hello";    // pool mein already → reuse
String s3 = "Hello";    // reuse

s1 == s2;       // true (same memory)
s1 == s3;       // true
```

### Visual
```
   ┌──────────────────────────┐
   │  String Pool (Heap)       │
   │                            │
   │   "Hello"  ←──┐            │
   │               │            │
   └───────────────┼────────────┘
                   │
            ┌──────┴──────┬──────┐
            ▼             ▼      ▼
           s1            s2     s3
   
   3 references, 1 actual string in memory
```

---

## 3️⃣ String Pool vs new Keyword

```java
String s1 = "Hello";              // POOL
String s2 = "Hello";              // POOL (reuse)
String s3 = new String("Hello");  // HEAP (new object)
String s4 = new String("Hello");  // HEAP (another new object)

s1 == s2;          // true  (both pool)
s1 == s3;          // false (pool vs heap)
s3 == s4;          // false (different heap objects)

s1.equals(s3);     // true  (content same)
```

### Visual
```
   POOL                  HEAP
   ┌─────────┐          ┌─────────┐
   │ "Hello" │          │ "Hello" │ ← s3
   │   ↑↑    │          └─────────┘
   │   s1 s2 │          ┌─────────┐
   └─────────┘          │ "Hello" │ ← s4
                        └─────────┘

   "==" checks reference (same memory?)
   ".equals()" checks content
```

---

## 4️⃣ intern() — Force Pool Use

```java
String s3 = new String("Hello");     // HEAP
String s4 = s3.intern();             // FORCE pool

s4 == s1;       // true (s4 now points to pool)
```

```
intern() = "Pool mein daal ya match karwa do"
```

---

## 5️⃣ Why Strings Immutable?

```
1. SECURITY
   File path, DB URL passed as String
   Mutable → kisi ne change → security breach

2. CACHING
   Pool only works because immutable
   Mutable hota → pool worthless

3. THREAD SAFETY
   Immutable = inherently thread-safe
   Multiple threads read same string safely

4. HASH CACHING
   String hashCode cached
   HashMap key bahut fast
```

---

## 6️⃣ String Concatenation — Performance Trap

```java
// BAD — creates 1000 new String objects
String result = "";
for (int i = 0; i < 1000; i++) {
    result = result + i;          // each + = new string
}
```

```
Visual:
   ""
   "" + 0 = "0"          (new object)
   "0" + 1 = "01"        (new object)
   "01" + 2 = "012"      (new object)
   ...
   1000 new objects = SLOW + memory waste
```

### SOLUTION: StringBuilder
```java
// GOOD — single mutable buffer
StringBuilder sb = new StringBuilder();
for (int i = 0; i < 1000; i++) {
    sb.append(i);                 // modifies SAME object
}
String result = sb.toString();
```

```
StringBuilder = WHITEBOARD
   Tu likh, mita, badal — same board
   = MUTABLE
```

---

## 7️⃣ StringBuilder vs StringBuffer

```
┌────────────────────┬──────────────────┬──────────────────┐
│  Aspect            │  StringBuilder   │  StringBuffer    │
├────────────────────┼──────────────────┼──────────────────┤
│ Mutable?           │ YES              │ YES              │
│ Thread-safe?       │ NO               │ YES (synchronized│
│ Performance        │ FASTER            │ SLOWER (locking) │
│ Single-thread use  │ ✅                │ ❌ (overkill)    │
│ Multi-thread use   │ ❌                │ ✅               │
└────────────────────┴──────────────────┴──────────────────┘
```

```
Single thread? → StringBuilder
Multiple threads sharing? → StringBuffer
```

---

## 8️⃣ String vs StringBuilder vs StringBuffer

```
┌────────────────────┬──────────┬───────────────┬───────────────┐
│  Aspect            │  String  │ StringBuilder │ StringBuffer  │
├────────────────────┼──────────┼───────────────┼───────────────┤
│ Mutable            │ NO       │ YES           │ YES           │
│ Thread-safe        │ YES (imm)│ NO            │ YES           │
│ Pool                │ YES      │ NO            │ NO            │
│ Performance        │ Slow concat│ Fast        │ Medium        │
│ Use case           │ Read-only│ Build (1 thr) │ Build (m thr) │
└────────────────────┴──────────┴───────────────┴───────────────┘
```

---

## 9️⃣ Common Trap — String Comparison

```java
String s1 = "Hello";
String s2 = "Hello";
String s3 = new String("Hello");

s1 == s2;             // true  (pool)
s1 == s3;             // false (pool vs heap)
s1.equals(s3);        // true  (content)

// RULE: ALWAYS use .equals() for string content comparison
//       == only for reference equality
```

---

## 🎯 Memory Hooks

```
String           = 🪨 stone (immutable, pool sharing)
StringBuilder    = 🖍️  whiteboard single-thread
StringBuffer     = 🖍️  whiteboard multi-thread (locked)

"==" → reference (same memory?)
".equals()" → content (same letters?)

intern() → pool mein force daal
```

📚 [← Back to JAVA](../)
