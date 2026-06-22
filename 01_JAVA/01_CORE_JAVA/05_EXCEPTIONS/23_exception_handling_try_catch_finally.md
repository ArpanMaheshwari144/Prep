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
