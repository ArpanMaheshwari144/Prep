# Exceptions — Visual Revision

---

## 1 Concept (Cooking Analogy)

```
Tu khana bana raha — recipe follow:
   1. Vegetables cut karo
   2. Tel garam karo
   3. Pakao
   4. Plate mein nikalo
```

```
PROBLEM scenarios:
   • Gas khatam ho gayi    (resource missing)
   • Sabzi jal gayi         (logic error)
   • Knife haath kat gaya   (unexpected)

Without "exception handling":
   Program crash → kitchen mein chaos

With exception handling:
   Plan B ready → smoothly handle
```

---

## 2 try-catch-finally Structure

```java
try {
    // Recipe (main code)
    cookFood();
}
catch (GasOverException e) {
    // Plan B for "gas khatam"
    orderZomato();
}
catch (KnifeException e) {
    // Plan B for "knife haath kata"
    applyBandage();
}
finally {
    // ALWAYS runs (success ya failure)
    cleanKitchen();
}
```

```
   try    = recipe / risky code
   catch  = plan B for specific problem
   finally = cleanup (always)
```

---

## 3 Visual Flow

```
   ┌───────────────────┐
   │     try block     │
   │  (risky code)     │
   └──────┬────────────┘
          │
       Exception?
       /        \
      NO         YES
      │           │
      │           ▼
      │       ┌──────────────────┐
      │       │  catch block     │
      │       │  (handle error)  │
      │       └────────┬─────────┘
      │                │
      └────────┬───────┘
               │
               ▼
         ┌──────────────┐
         │ finally block│  ← ALWAYS runs
         │ (cleanup)    │
         └──────────────┘
```

---

## 4 Real Code Example

```java
FileReader reader = null;
try {
    reader = new FileReader("data.txt");
    // ... read file
}
catch (FileNotFoundException e) {
    System.out.println("File missing: " + e.getMessage());
}
catch (IOException e) {
    System.out.println("Read error: " + e.getMessage());
}
finally {
    if (reader != null) {
        reader.close();    // close resource ALWAYS
    }
}
```

---

## 5 Exception Hierarchy

```
                Throwable
                /        \
              Error      Exception
                            │
                ┌───────────┴────────────┐
                │                         │
            Checked              RuntimeException
            (compile-time)       (Unchecked, runtime)

            • IOException        • NullPointerException
            • SQLException       • ArrayIndexOutOfBounds
            • ClassNotFoundEx    • IllegalArgumentException
                                  • ArithmeticException
```

---

## 6 Checked vs Unchecked

```
CHECKED (compile-time enforced):
   Java compiler bolega "handle karo ya throws likh"

   ┌─────────────────────────────────┐
   │  IOException, SQLException...   │
   │                                  │
   │  Code: file read karte time      │
   │  Compile error if not handled    │
   └─────────────────────────────────┘
```

```
UNCHECKED (runtime, RuntimeException):
   Compiler chup
   Code chal jata, fail at runtime

   ┌─────────────────────────────────┐
   │  NullPointerException           │
   │  ArrayIndexOutOfBounds...        │
   │                                  │
   │  Code chala, kuch nahi diya      │
   │  Runtime crash                   │
   └─────────────────────────────────┘
```

```
RULE:
   Checked    = bahar ki problem (file, DB, network)
                = MUST handle

   Unchecked  = code mein bug
                = optional handle (better to fix code)
```

---

## 7 Code Examples

```java
// CHECKED (must handle)
try {
    FileReader r = new FileReader("file.txt");  // throws IOException
} catch (IOException e) { ... }
   // OR
public void readFile() throws IOException { ... }

// UNCHECKED (no force)
String s = null;
s.length();    // NullPointerException at runtime
   // No try-catch needed by compiler
   // But system crash
```

---

## 8 throw vs throws

```
throw  = raise an exception (verb, action)
throws = method declares it MIGHT throw (declaration)
```

```java
// throws (in method signature)
public void readFile(String path) throws IOException {
    // method declares it may throw IOException
}

// throw (inside method body)
public void validate(int age) {
    if (age < 0) {
        throw new IllegalArgumentException("Age can't be negative");
    }
}
```

---

## 9 Multi-Catch (Java 7+)

```java
try {
    riskyOp();
}
catch (IOException | SQLException e) {
    // Both handled in ONE block
    System.out.println("Error: " + e.getMessage());
}
```

```
| pipe = OR
   IOException   handle here
   SQLException  handle here
   = same logic, less code
```

---

## try-with-resources (Java 7+)

```
PROBLEM with manual close:
   try {
       reader = new FileReader(...);
   } catch (...) { }
   finally {
       if (reader != null) reader.close();  // boilerplate
   }
```

```
SOLUTION:
   try (FileReader reader = new FileReader("file.txt")) {
       // use reader
   } catch (IOException e) { ... }

   // reader.close() AUTOMATICALLY called
   // (resource implements AutoCloseable)
```

```java
// Modern way
try (FileReader r = new FileReader("a.txt");
     BufferedReader br = new BufferedReader(r)) {
    // use both
} catch (IOException e) {
    // handle
}
// Both resources auto-closed
```

---

## 11 Custom Exception

```java
public class InsufficientFundsException extends RuntimeException {
    public InsufficientFundsException(String msg) {
        super(msg);
    }
}

// Use:
public void withdraw(double amount) {
    if (balance < amount) {
        throw new InsufficientFundsException(
            "Balance is " + balance);
    }
    balance -= amount;
}
```

```
Custom exceptions = domain-specific errors
   Better than generic RuntimeException
   Self-documenting code
```

---

## 12 Common Traps

### Trap 1: finally OVERRIDES return
```java
public int test() {
    try {
        return 1;
    } finally {
        return 2;    // ← THIS WINS
    }
}
// Returns 2, NOT 1!
```

### Trap 2: Order of catches matters
```java
try { ... }
catch (Exception e) { ... }       // ← catches everything
catch (IOException e) { ... }      // unreachable!

// FIX: child first, parent last
try { ... }
catch (IOException e) { ... }
catch (Exception e) { ... }
```

### Trap 3: Empty catch
```java
catch (Exception e) {
    // SILENT — terrible!
}
// At least:
catch (Exception e) {
    log.error("Failed", e);
}
```

---

## Memory Hooks

```
try     → recipe (risky code)
catch   → plan B per error type
finally → kitchen cleanup (always)

Checked   = bahar ki problem (must handle)
Unchecked = code bug (runtime crash)

throw     = raise exception (verb)
throws    = method declaration (noun)

try-with-resources = auto-close cleanup
```

[← Back to JAVA](../)
