# String `intern()` — Pool Mein Force Karo

> **V90 — Brain Ticklers: Topic 48**

---

## STORY — Library ki Trick

> Imagine **library mein 1000 logon ne "Java Complete Reference"** book maangi.
>
> **Without intern():** Library 1000 alag-alag copies banake deti — **memory waste**
>
> **With intern():** Library bolti — *"Reference shelf pe ek hi copy hai. Sab waha se padho!"* — **memory save**

`String.intern()` JVM ko bolta — *"Yeh string already pool mein hai? Wahi reference do, naya banane ka kya kaam?"*

---

## MEMORY VISUAL — String Pool vs Heap

```
                JVM MEMORY
   ┌────────────────────────────────────────┐
   │                                        │
   │   STRING POOL (shared, in Heap area)   │
   │   ┌──────────────────────────────┐     │
   │   │   "Hello"  ◄──── s1        │     │
   │   │   "World"                    │     │
   │   └──────────────────────────────┘     │
   │                                        │
   │   HEAP (regular objects)               │
   │   ┌──────────────────────────────┐     │
   │   │   String("Hello") ◄──── s2 │     │
   │   │   (separate object — fresh)  │     │
   │   └──────────────────────────────┘     │
   │                                        │
   └────────────────────────────────────────┘

   s1 == s2 → false  (different memory locations)
```

After `s3 = s2.intern()`:

```
   POOL                            HEAP
   ┌──────────────────┐           ┌──────────────────┐
   │ "Hello" ◄──s1    │           │ String("Hello")  │
   │         ◄──s3  │           │            ◄──s2 │
   └──────────────────┘           └──────────────────┘

   s1 == s3 → true   (same pool reference)
   s1 == s2 → false  (s2 still in heap)
```

**`intern()` doesn't move s2** — it RETURNS the pool reference. s2 still in heap.

---

## CODE EXAMPLE (with execution flow)

```java
String s1 = "Hello";                  // STEP 1: pool mein bana ya already tha
String s2 = new String("Hello");      // STEP 2: heap mein NAYA object force kiya
String s3 = s2.intern();              // STEP 3: pool ka reference return

System.out.println(s1 == s2);    // false — heap vs pool
System.out.println(s1 == s3);    // true  — dono same pool ref
System.out.println(s2 == s3);    // false — s2 abhi bhi heap mein
```

### Step-by-step memory state:

```
After Step 1:                  After Step 2:                  After Step 3:
─────────────                  ─────────────                  ─────────────
POOL: "Hello" ← s1             POOL: "Hello" ← s1             POOL: "Hello" ← s1, s3
HEAP: (empty)                  HEAP: "Hello" ← s2             HEAP: "Hello" ← s2
```

---

## INTERVIEW TRAP — Common gotcha

```java
String a = "java";
String b = new String("java").intern();
System.out.println(a == b);  // ?
```

**Answer:** `true` — `intern()` ne `b` ko pool ka reference de diya, jo `a` ke saath same.

**90% candidates "false" bolte galti se** kyunki `new String()` dekh ke confused ho jate. **`intern()` baad mein call hua = pool mein jump kar gaya.**

---

## WHY `intern()`? — Performance Use Case

### Scenario: Bahut saari duplicate strings

```
XML parsing, CSV reading, JSON parsing —
1000+ rows mein same value repeat:

   Without intern:
   ┌────────────────────────────────┐
   │ HEAP                           │
   │  "Active" "Active" "Active"    │
   │  "Active" "Active" "Active"    │  ← 1000 alag objects
   │  "Active" ... (1000 copies)    │     1000 × ~40 bytes = 40 KB
   └────────────────────────────────┘

   With intern:
   ┌────────────────────────────────┐
   │ POOL                           │
   │  "Active"                      │  ← 1 object, 1000 references
   │                                │     ~40 bytes total
   └────────────────────────────────┘
```

**Memory savings massive jab strings repeat ho.**

---

## TRAP BOX — Don't overuse!

```
Trap 1: "Sab strings intern karo, memory save hogi"
         NAHI — pool memory limited hai
         Sirf duplicates pe karo

Trap 2: "intern() s2 ko pool mein move karta"
         NAHI — RETURN karta pool reference
         s2 still heap mein, s3 alag variable

Trap 3: "Java mein StringPool unlimited hai"
         NAHI — Java 7+ heap mein hai but limited
         Pre-Java 7: PermGen mein, OutOfMemoryError aa sakta tha
```

---

## String Pool — Java versions

| Version | Pool Location | Issue |
|---|---|---|
| Java 6 | PermGen | Fixed size → `OutOfMemoryError: PermGen` |
| Java 7+ | Main Heap | Garbage collected, no PermGen issues |
| Java 8+ | Main Heap (Metaspace removed) | Same as Java 7 |

---

## INTERVIEW TALKING POINT

**Q: "`String.intern()` kya karta hai?"**

> *"`intern()` JVM ka string deduplication mechanism hai. Heap mein bana hua String object le ke pool mein dhundhta — agar same value pool mein hai, **wahi reference return** karta. Memory save hota jab bahut saari duplicate strings ho — XML/CSV parsing mein useful. Pool main heap mein hai Java 7+ se."*

**Q: "Kab use karoge production mein?"**

> *"Sirf jab evidence ho ki specific strings highly duplicate hain — har string pe intern karna ulta memory waste karega kyunki pool itself memory leta. Profiler use karke confirm karte phir selective intern."*

---

## POWER PHRASE

> **"`intern()` returns the canonical pool reference of a string — useful for deduplication when many duplicate strings exist (XML/CSV parsing), but overuse risks filling the string pool memory."**

---

## MEMORY HOOK

```
new String("X")    → naya HEAP object (always)
"X"                → POOL ka reference (Java optimizes)
str.intern()       → POOL ka reference RETURN karta (not move)

Yaad rakh:
   Heap = personal copy
   Pool = shared library copy
   intern() = "library wali do, fresh print mat banao"
```
