# Checked vs Unchecked Exceptions

> **V90 Section 1 — Topic 9 (Interview MUST-KNOW)**

---

## STORY — Compiler ka Darwaan vs Runtime ka Dhoka

> Imagine **2 alag situations**:
>
> **Checked Exception (Darwaan analogy):**
> File read karna chahta — compiler **darwaze pe khada** "Bhai, file na mili toh? Pehle batao kya karega!" — **force karta handle karo.**
>
> **Unchecked Exception (Dhoka analogy):**
> Code chal raha tha smoothly, suddenly `null.length()` call kiya — **runtime pe dhoka** mil gaya. Compiler ne kuch nahi roka pehle.
>
> Difference: **Checked = compile-time darwaan, Unchecked = runtime ka surprise.**

---

## EXCEPTION HIERARCHY VISUAL

```
                       Throwable
                      /         \
                   Error       Exception
                   (JVM       /          \
                    issues)  RuntimeEx   (others)
                           (UNCHECKED) (CHECKED)
                              │            │
                ┌─────────────┼────────┐   ├─ IOException
                │             │        │   ├─ SQLException
        NullPtrEx    ArrayIdxEx  ArithmeticEx │   ├─ ClassNotFoundException
        ClassCastEx  IllegalArg  NumberFormat │   └─ FileNotFoundException
                                              │
                              All RuntimeException
                              + subclasses = UNCHECKED
                              All other Exception
                              + subclasses = CHECKED
```

**The rule:**
- `extends RuntimeException` → **Unchecked**
- `extends Exception` (but NOT RuntimeException) → **Checked**
- `extends Error` → don't catch (JVM-level, OutOfMemory etc.)

---

## CODE — Side by Side

### Checked Exception (compile-time enforced)

```java
public void readFile() {
    FileReader fr = new FileReader("test.txt");   // COMPILE ERROR
    // "Unhandled exception type FileNotFoundException"
}

// Fix Option 1: try-catch
public void readFile() {
    try {
        FileReader fr = new FileReader("test.txt");
    } catch (FileNotFoundException e) {
        // handle
    }
}

// Fix Option 2: throws declaration
public void readFile() throws FileNotFoundException {
    FileReader fr = new FileReader("test.txt");
}
```

### Unchecked Exception (runtime surprise)

```java
public void useNull() {
    String s = null;
    s.length();   // Compiles fine
                   // NullPointerException at runtime
}

// No compile force — but bug crashes app
```

---

## THE TRAP — Compiler Behavior

```
Checked Exception:                 Unchecked Exception:
─────────────────                  ────────────────────

Compiler:                          Compiler:
  "FileReader risky hai            "Null pointer? Bhai,
   — try-catch ya throws!"          mujhe kya pata, runtime
   → COMPILE ERROR                  pe pakda jaayega"
                                   → COMPILE OK
                                   → RUNTIME
```

**Compiler ka philosophy:**
- Checked = "External resources (file/DB/network) failure expected — plan ahead"
- Unchecked = "Programmer errors (null, out-of-bounds) — shouldn't happen if logic correct"

---

## FULL COMPARISON TABLE

| Property | Checked | Unchecked |
|---|---|---|
| **Parent class** | `Exception` (not RuntimeException) | `RuntimeException` |
| **Compile-time check** | Mandatory | Not enforced |
| **Must handle/declare?** | try-catch OR throws | Optional |
| **Source typically** | External (file, DB, network) | Internal bugs (null, index) |
| **Examples** | `IOException`, `SQLException`, `ClassNotFoundException` | `NullPointerException`, `ArrayIndexOutOfBoundsException`, `ArithmeticException`, `IllegalArgumentException` |
| **Spring `@Transactional` rollback** | NO by default | YES by default |
| **Method signature pollution** | High (`throws` everywhere) | None (clean signatures) |
| **Modern Java preference** | Reduced — most prefer Unchecked | Preferred for business logic |

---

## SPRING `@Transactional` ROLLBACK RULE

**Critical interview gotcha:**

```java
@Transactional
public void saveUser(User user) {
    userRepo.save(user);

    if (badThing) {
        throw new IOException("error");        // NO ROLLBACK (Checked)
    }

    if (worseThing) {
        throw new RuntimeException("error");   // Rollback happens (Unchecked)
    }
}
```

**Why?** Spring philosophy:
- RuntimeException = "unexpected programming error" → rollback to safety
- Checked Exception = "expected, recoverable" → caller will handle, commit

**Force rollback for checked:**

```java
@Transactional(rollbackFor = IOException.class)
public void saveUser(...) throws IOException {
    // Now IOException ALSO rolls back
}

@Transactional(rollbackFor = Exception.class)
public void saveUser(...) {
    // ALL exceptions rollback
}
```

**Production rule:** Custom domain exceptions hamesha extend `RuntimeException` — clean signatures + automatic rollback.

```java
// Recommended
public class UserNotFoundException extends RuntimeException {
    public UserNotFoundException(Long id) {
        super("User not found: " + id);
    }
}
```

---

## WHEN TO THROW WHICH

### Use Checked when:
- **Recoverable** error from EXTERNAL source (file/network/DB)
- Caller can reasonably handle (retry, fallback)
- Avoid for business logic (signature pollution)

### Use Unchecked when:
- Programming error (null check fail, invalid argument)
- Business rule violation
- Caller usually can't recover (bug in code)
- Preferred in modern Spring/REST APIs

### Real-world examples

```java
// Checked — file system
public byte[] readFile(String path) throws IOException {
    return Files.readAllBytes(Paths.get(path));
}

// Unchecked — business
public User getUserById(Long id) {
    return userRepo.findById(id)
        .orElseThrow(() -> new UserNotFoundException(id));
}

// Unchecked — input validation
public void setAge(int age) {
    if (age < 0) throw new IllegalArgumentException("Age can't be negative");
    this.age = age;
}
```

---

## INTERVIEW TRAPS

### Trap 1: Error vs Exception confusion

```
Throwable
  ├─ Error          ← JVM-level (OutOfMemoryError, StackOverflowError)
  │                    DON'T catch — app should die gracefully
  ├─ Exception
  │  ├─ RuntimeException + subclasses  → Unchecked
  │  └─ Other subclasses                → Checked
```

### Trap 2: Custom exception choosing parent

```java
// Avoid (Checked) — pollutes signatures
public class BusinessRuleException extends Exception { }

// Modern preferred (Unchecked) — clean signatures
public class BusinessRuleException extends RuntimeException { }
```

### Trap 3: Catching `Exception` catches both

```java
try {
    riskyMethod();
} catch (Exception e) {
    // Catches both Checked and Unchecked
    // Too broad — usually catch specific
}
```

### Trap 4: NullPointerException in throws clause

```java
// Useless — Unchecked, compiler doesn't enforce
public void method() throws NullPointerException { }

// Just throw it normally
public void method() {
    if (something == null) throw new NullPointerException();
}
```

---

## INTERVIEW TALKING POINT

**Q: "Checked vs Unchecked exception kya hai?"**

> *"Checked exceptions compile-time pe enforce hote — `Exception` extend karte hain. Compiler force karta `try-catch` ya `throws` declare karna. External resource failures (file, DB, network) ke liye design hue. Examples: `IOException`, `SQLException`.
>
> Unchecked exceptions runtime pe aate — `RuntimeException` extend karte. Compiler enforce nahi karta. Programming bugs ya business rule violations ke liye. Examples: `NullPointerException`, `IllegalArgumentException`, custom `UserNotFoundException`.
>
> Modern Java practice: business exceptions Unchecked banao — clean method signatures + Spring `@Transactional` default rollback compatible."*

**Q: "Spring mein checked exception pe rollback kyu nahi hota?"**

> *"Spring philosophy — RuntimeException = unexpected programmer error → rollback safe. Checked exception = expected, caller will handle → commit. Override karna ho toh `@Transactional(rollbackFor = Exception.class)` lagao. Best practice: custom exceptions `RuntimeException` extend karo — automatic rollback bhi mil jata."*

**Q: "Custom exception likhna ho — kaunsa parent extend karu?"**

> *"99% cases mein `RuntimeException` — modern preference. Reasons:
> 1. Method signatures clean (no `throws` clutter)
> 2. Spring `@Transactional` default rollback compatible
> 3. Functional interfaces (Supplier, Function) checked allow nahi karte — Unchecked seamless work karta
> 4. Caller force nahi hota handle karne — flexibility deta
>
> Sirf jab caller MUST handle (recoverable failure pattern) tab Checked banao."*

---

## POWER PHRASES

> **"Checked = compile-time darwaan jo handle force karta. Unchecked = runtime ka surprise. Modern Java mein business logic Unchecked banao — clean signatures + `@Transactional` default rollback."**

> **"`@Transactional` only rolls back on RuntimeException by default — for IOException you must add `rollbackFor = IOException.class`."**

---

## MEMORY HOOK

```
Checked          → "Compiler darwaan"     → handle / throws MUST
                                            extends Exception

Unchecked        → "Runtime ka dhoka"     → optional handling
                                            extends RuntimeException

Spring rollback default:
   Checked    → commit (no rollback)
   Unchecked  → rollback

Custom exceptions:
   Modern preference → extends RuntimeException

Examples:
   Checked   → IOException, SQLException, FileNotFoundException
   Unchecked → NullPointerException, IllegalArgument, ArrayIndexOutOfBounds
```

---

## TRAP BOX

```
Trap 1: "@Transactional checked exception pe rollback karta"
         NAHI — sirf RuntimeException by default
         rollbackFor = Exception.class lagao

Trap 2: "Custom exception Exception extend karu"
         Modern practice — RuntimeException extend better
         Clean signatures + Spring compatibility

Trap 3: "throws NullPointerException likhna useful hai"
         Unchecked — compiler enforce nahi karta
         Just throw it, no declaration needed

Trap 4: "Error catch kar lo"
         Error = JVM-level (OutOfMemory) — fatal
         Let app die — recovery rarely possible

Trap 5: "catch (Exception e) sab catch karta"
         Error subclasses NAHI catch hote
         catch (Throwable t) sab catch (rare use)
```
