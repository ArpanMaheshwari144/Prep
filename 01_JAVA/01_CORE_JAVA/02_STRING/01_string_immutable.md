# String — Immutable Kyun Hai?

> **V90 Section 1 — Topic 4**

---

## WHY — String Immutable Kyun?

3 reasons:

1. **String Pool** possible ho sake → same value = same object → **memory save**
2. **Thread safe** → multiple threads same String use karein → change nahi ho sakti
3. **HashMap key safe** → key change ho jaaye → hashCode alag → value milega hi nahi!

**★ 2 aur reasons (interview mein "4 reasons" poochte):**
4. **SECURITY** → file paths, URLs, DB credentials String mein jaate — mutable hoti to check ke baad koi badal deta = security risk
5. **HASHCODE CACHING** → `String.hashCode()` ek baar compute ho ke cache hota (immutable hai isliye safe) → HashMap key fast

---

## STORY — Login Bug

→ Ek baar login system mein bug aaya
→ Username validate karne ka function tha — usme pass kiya `"arpan"`
→ Function ne internally modify kiya
→ Wapas aaya toh original **corrupt** tha, wrong audit log gaya
→ Tab socha — String agar mutable hoti toh ye problem hamesha rehti
→ Java ne isliye String ko **immutable** banaya — ek baar bana, koi nahi badal sakta
→ Aur **String Pool** tab hi kaam karta hai jab immutable ho
→ warna ek ne badla, sab ke liye badal gaya

---

## String Pool Visualization

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

## Behaviour

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

## TRAP

> **`==` = same OBJECT check. `.equals()` = same VALUE check.**
> **HAMESHA `.equals()` use karo strings compare karne ke liye.**

---

## ★ TRAP — `intern()` (pool mein force karo)

**Story — Library ki trick:** 1000 logon ne "Java Complete Reference" maangi.
- **Without intern():** library 1000 alag copies banake deti → memory waste
- **With intern():** "Reference shelf pe ek hi copy hai, sab waha se padho" → memory save

`String.intern()` JVM ko bolta — *"Yeh string already pool mein hai? Wahi reference do, naya banane ka kya kaam?"*

```java
String s1 = "Hello";                  // STEP 1: pool mein bana ya already tha
String s2 = new String("Hello");      // STEP 2: heap mein NAYA object force kiya
String s3 = s2.intern();              // STEP 3: pool ka reference return

System.out.println(s1 == s2);    // false — heap vs pool
System.out.println(s1 == s3);    // true  — dono same pool ref
System.out.println(s2 == s3);    // false — s2 abhi bhi heap mein
```

```
After Step 1:                  After Step 2:                  After Step 3:
─────────────                  ─────────────                  ─────────────
POOL: "Hello" ← s1             POOL: "Hello" ← s1             POOL: "Hello" ← s1, s3
HEAP: (empty)                  HEAP: "Hello" ← s2             HEAP: "Hello" ← s2
```

**`intern()` s2 ko MOVE nahi karta** — pool ka reference RETURN karta. s2 still heap mein.

**Interview gotcha:**

```java
String a = "java";
String b = new String("java").intern();
System.out.println(a == b);  // true
```

`true` — `intern()` ne `b` ko pool ka reference de diya, jo `a` ke saath same. Log `new String()` dekh ke "false" bol dete — par `intern()` baad mein call hua = pool mein jump kar gaya.

**WHY intern() — use case:** XML / CSV / JSON parsing mein 1000+ rows mein same value repeat ("Active"):

```
Without intern:  HEAP mein 1000 alag "Active" objects  → 1000 × ~40 bytes = 40 KB
With intern:     POOL mein 1 "Active", 1000 references → ~40 bytes total
```

**Don't overuse:**

```
Trap 1: "Sab strings intern karo, memory save hogi"
         NAHI — pool memory limited hai, sirf duplicates pe karo
Trap 2: "intern() s2 ko pool mein move karta"
         NAHI — RETURN karta pool reference; s2 still heap mein, s3 alag variable
Trap 3: "Java mein StringPool unlimited hai"
         NAHI — Java 7+ heap mein hai but limited
         Pre-Java 7: PermGen mein, OutOfMemoryError aa sakta tha
```

| Version | Pool Location | Issue |
|---|---|---|
| Java 6 | PermGen | Fixed size → `OutOfMemoryError: PermGen` |
| Java 7+ | Main Heap | Garbage collected, no PermGen issues |
| Java 8+ | Main Heap (PermGen hata, Metaspace aaya) | Same as Java 7 |

**Q: "`String.intern()` kya karta hai?"**
> *"`intern()` JVM ka string deduplication mechanism hai. Heap mein bana hua String object le ke pool mein dhundhta — agar same value pool mein hai, **wahi reference return** karta. Memory save hota jab bahut saari duplicate strings ho — XML/CSV parsing mein useful. Pool main heap mein hai Java 7+ se."*

**Q: "Kab use karoge production mein?"**
> *"Sirf jab evidence ho ki specific strings highly duplicate hain — har string pe intern karna ulta memory waste karega kyunki pool itself memory leta. Profiler use karke confirm karte phir selective intern."*

```
new String("X")    → naya HEAP object (always)
"X"                → POOL ka reference (Java optimizes)
str.intern()       → POOL ka reference RETURN karta (not move)

Heap = personal copy · Pool = shared library copy
intern() = "library wali do, fresh print mat banao"
```

---

## ★ TRAP — Compile-time constant folding (`"Hel" + "lo"`)

```java
String s1 = "Hello";
String s2 = "Hel" + "lo";       // dono LITERALS → compiler folds → "Hello"
s1 == s2     // TRUE

String x = "Hel";
String s3 = x + "lo";           // x VARIABLE hai, literal nahi
s1 == s3     // FALSE — runtime pe naya object banta
s1.equals(s3)  // TRUE  — content same
```

**Case 1 (literal + literal):** javac ne dekha `"Hel"` aur `"lo"` dono constants → **compile time pe hi `"Hello"` bana diya**. Bytecode mein s1 aur s2 dono ko `"Hello"` literal mila → same pool object.

**Case 2 (variable + literal):** `x` ka value runtime tak unknown → fold nahi ho sakta. JVM runtime pe:
1. Internally `StringBuilder` banata
2. `x` ka value (`"Hel"`) append karta
3. `"lo"` append karta
4. `toString()` → **NAYA String object heap mein** (pool mein NAHI)

```
STACK              HEAP — String Pool         HEAP (pool ke bahar)
                   ┌─────────────┐            ┌─────────────┐
s1  ──────────────►│  "Hello"    │            │  "Hello"    │
x   ──────────────►│  "Hel"      │            │ (NAYA obj)  │◄── s3
                   └─────────────┘            └─────────────┘
```

| Case | Compiler ka kaam | Result location | `==` ka result |
|------|------------------|-----------------|----------------|
| `"Hel" + "lo"` | **Compile-time fold** → "Hello" literal | Pool (shared) | true |
| `x + "lo"` | **Cannot fold** (x runtime variable) | Heap (new object) | false |

> **Literals + literals** = compile-time fold = same pool object = `==` true
> **Variable + literal** = runtime concat = naya heap object = `==` false
> **HAMESHA `.equals()` use karo**

---

## POWER PHRASES

> *"String is immutable because it is a shared object — if one reference changes it, all references would be affected. Immutability enables the String Pool, thread safety, and safe use as HashMap keys."*

> *"String literals go to the String Pool — same value reuses the same object in memory. `new String()` forces a new heap object. Always use `.equals()` for value comparison, never `==`."*

> *"`intern()` returns the canonical pool reference of a string — useful for deduplication when many duplicate strings exist (XML/CSV parsing), but overuse risks filling the string pool memory."*

> *"Java compiler folds compile-time constant expressions into a single pool string — `"Hel" + "lo"` becomes `"Hello"` at compile time. But `variable + literal` is computed at runtime via StringBuilder, creating a new heap object outside the pool. Always use `.equals()` for content comparison."*
