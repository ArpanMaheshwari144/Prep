# Exception Handling — try-catch-finally

> **V90 — Core Java Extras: Topic 23**

---

## Throwable Hierarchy

```
                 Throwable
                /         \
   ┌───────────┘           └───────────┐
   │                                   │
Error (JVM level)                  Exception
RECOVER NAHI HOTA               /            \
   │                      Checked         RuntimeException
   │                  (compiler force)     (UNCHECKED)
   ├ OutOfMemoryError    │                    │
   ├ StackOverflowError  ├ IOException        ├ NullPointerException
                         ├ SQLException       └ ... aur
```

**Yaad rakh:**
- **Error** = JVM level — app handle nahi karti
- **Checked** = compiler pakda, **handle ya declare karo**
- **Unchecked (RuntimeException)** = runtime bugs, fix karo

---

## STORY — DB se Data Fetch

→ Tune DB se data fetch karna tha
→ Kahin **connection fail**, kahin **query fail**, kahin **result empty**
→ Sab cases handle karne the
→ **`try`** mein risky code, **`catch`** mein handle, **`finally`** mein resources close
→ — chaahe exception aaye ya na aaye, **`finally` = maa ki daant**
→ ghar se bhaag bhi jaaye toh bhi padegi
→ **`System.exit()` ke alawa koi nahi rok sakta**

---

## Code

### Basic Flow
```java
try {
    int result = 10 / 0;                       // ArithmeticException
    System.out.println(result);                // skip ho jayega
} catch (ArithmeticException e) {
    System.out.println("divide by zero!");     // pakda
} finally {
    System.out.println("finally chala");       // HAMESHA
}
```

### Multi-catch — ek catch mein multiple exceptions
```java
try {
    // risky code
} catch (IOException | SQLException e) {       // multi-catch
    System.out.println(e.getMessage());
}
```

---

## TRAP 1 — `finally` after return

```java
int test() {
    try {
        return 1;                              // return kiya
    } finally {
        System.out.println("finally!");        // YE BHI CHALTA HAI
    }
}
// Output: "finally!" print hoga, PHIR 1 return hoga
```

```java
int test2() {
    try { return 1; }
    finally { return 2; }                      // 2 return hoga, 1 LOST!
}
```

> **`finally` mein return likhna = `try` ka return overwrite ho jaata. KABHI nahi karna!**

---

## ★ TRAP — `finally` ka return = "LAST WORD WINS" (poori variations)

**Analogy:** interview mein — try: "mera answer 1", catch: "ya phir 2", finally: "actually FINAL answer 3". Interviewer: "last said wins — 3." Java mein bhi **`finally` ka `return` last word ban jata, sab override.**

```java
public int test() {
    try {
        return 1;       // STEP 1: 1 ready to return
    } catch (Exception e) {
        return 2;       // (would be 2 if exception)
    } finally {
        return 3;       // STEP 2: 3 OVERRIDES — 1 ya 2 LOST
    }
}
System.out.println(test());   // Output: 3
```

Log pehli baar `1` bolte — par `3` return hota. Try ka return **silently lost** = bug source.

**Variation — exception SWALLOWED by finally return:**
```java
int x() {
    try {
        throw new RuntimeException("boom");
    } finally {
        return 99;   // Exception SUPPRESSED!
    }
}
// Returns: 99 — exception gone, no trace!
```
Dangerous — exception silently kha gaya. Production debugging nightmare.

**Variation — reference return hua, finally ne object modify kiya:**
```java
List<Integer> list = new ArrayList<>();
list.add(1);
list = mutate(list);
System.out.println(list);  // [1, 99] — finally ne add kiya

List<Integer> mutate(List<Integer> input) {
    try { return input; }
    finally { input.add(99); }   // Object modified after "return"
}
```
Reference try mein return ho gaya, finally ne underlying object badal diya (mutable type).

**Variation — throw in finally:**
```java
try { throw new IOException(); }
finally { throw new RuntimeException(); }   // original exception lost
```

| Scenario | finally runs? | Effect |
|---|---|---|
| Normal try completion | Yes | Cleanup code runs |
| Exception in try (caught) | Yes | After catch |
| Exception in try (uncaught) | Yes | Before propagation |
| try has `return` | Yes | Before actual return |
| try has `return X`, finally has `return Y` | Yes | Returns Y (overrides X) |
| try throws, finally has `return` | Yes | Exception SUPPRESSED |
| `System.exit(0)` in try | NO | JVM kill, finally skipped |
| JVM crash | NO | Process dies |
| Infinite loop in try | NO | Never reaches finally |

**Correct — finally sirf cleanup:**
```java
Connection conn = null;
try {
    conn = ds.getConnection();
    return processQuery(conn);
} finally {
    if (conn != null) conn.close();   // cleanup, no return
}

// Modern — try-with-resources (preferred)
try (Connection conn = ds.getConnection()) {
    return processQuery(conn);
}
// close() automatic, no finally needed, no override risk
```

**Q: "Yeh code kya return karega?" (finally + return)**
> *"`finally` ka return try ka return override kar deta. Java spec: jab method `return X` se exit hone wala ho, finally PEHLE chalta — agar finally mein bhi `return Y` hai, woh last word ban jata. Original X silently lost. Ye anti-pattern hai — exception bhi suppress kar sakta. Production mein finally sirf cleanup ke liye, return/throw kabhi nahi. Modern Java mein try-with-resources — close() automatic, override risk nahi."*

**Q: "finally mein exception throw kare?"**
> *"Same problem — original exception suppress ho jati. Java 7+ mein suppressed exceptions track kar sakte (`Throwable.getSuppressed()`), but practice mein avoid karte. Cleanup karna hai toh chup-chaap karo, propagate na karo."*

**Q: "Production debugging mein issue?"**
> *"Log mein result inconsistent dikhta — try ne `return success` kiya, finally ne `return failure` kar diya. Exception suppress hua to stack trace bhi gone. Code review mein pakadte — static analyzers (SonarQube, SpotBugs) ye pattern flag karte hain."*

```
finally ka return = "Last word wins"
   try return X        →   X ready
   finally return Y    →   Y OVERRIDES X
   try throws E1       →   E1 ready to propagate
   finally throws E2   →   E2 wins, E1 suppressed

Trap: "finally hamesha 100% chalta" → System.exit(), JVM crash, infinite loop → skip
Trap: "reference returned hai, safe hai" → finally object MODIFY kar sakta (mutable) → defensive copy / immutable

GOLDEN RULE: finally = sirf cleanup, no return, no throw. Modern: try-with-resources.
```

> **"`finally` mein `return` likhna anti-pattern hai — try ka return silently override karta + exceptions suppress karta. `finally` sirf cleanup ke liye. Modern Java mein `try-with-resources` use karo — automatic close, no override risk."**

---

## TRAP 2 — Multi-catch order

> **Parent exception pehle likha toh child catch UNREACHABLE = compile error!**

```java
catch (Exception e) { ... }              // pehle nahi
catch (NullPointerException e) { ... }   // unreachable
```

---

## TRAP 3 — `catch(Exception e)` sab pakda?

> **Lekin `Error` (OutOfMemoryError) NAHI pakdega.**
> **Error = JVM level, recover nahi hota.**

---

## throw vs throws — naam same, kaam alag

```
   throw  -> exception PHENKO (abhi, khud). ek statement.              -> action
   throws -> "ye method exception phenk SAKTA" — method signature pe.  -> warning/declaration
```

```java
// throw — abhi khud phenkna
void setAge(int age) {
    if (age < 0)
        throw new IllegalArgumentException("age galat");   // KHUD phenka, abhi
    this.age = age;
}

// throws — method ke signature pe, "warning" (phenkta nahi, batata)
void readFile() throws IOException {     // "main IOException phenk sakta, caller dhyaan rakhe"
    FileReader f = new FileReader("a.txt");
}

// dono saath
void process() throws IOException {      // throws = declare (caller handle kare)
    if (badData)
        throw new IOException("data kharab");   // throw = actual phenkna
}
```

| | throw | throws |
|---|---|---|
| **kaam** | exception phenkta (abhi) | "phenk sakta" batata (declare) |
| **kahan** | method ke andar, statement | method signature pe |
| **kitne** | ek baar mein 1 exception | kai likh sakte (`throws A, B`) |
| **grammar** | verb (karta hai) | warning label (ho sakta hai) |

> **throw = phenkna (abhi). throws = "phenk sakta" ka warning (signature pe). checked exception ho to throws/try-catch compiler force karta.**

---

## POWER PHRASE

> *"The `finally` block always executes regardless of whether an exception was thrown or a return statement was hit — the only exception is `System.exit()`. Never put a return statement in `finally` as it will silently override the return from `try`."*

> **Sequence:** try → catch → finally (hamesha).
> **Multi-catch:** `IOException | SQLException`.
> **`finally` overrides try return.**
