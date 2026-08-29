# Optional — Visual Revision

---

## 1 Problem (NullPointerException Hell)

```
Imagine searching for a user:

User user = userService.findByEmail("xyz@email.com");
String name = user.getName();   // NPE if user is null!

= Java mein NPE 90% bugs ka root cause
```

```
Optional = "I might have a value, I might not"
   Wrapper that forces null check
```

---

## 2 Envelope Analogy

```
Imagine TUJHE 2 envelopes mili:

   EMPTY envelope (no letter)
   LETTER inside

   Tu envelope KHOLE bina kuch nahi dekh sakta
   = Forced to check before using
```

```
Optional<T> = the envelope
   T = the letter (may exist or not)
```

---

## 3 Creating Optional

```java
// Filled
Optional<String> name = Optional.of("Arpan");

// Empty
Optional<String> empty = Optional.empty();

// May or may not be present
Optional<String> maybe = Optional.ofNullable(possiblyNull);
```

```
of(value)           → NPE if value null!
ofNullable(value)   → safe (empty if null)
empty()             → explicitly empty
```

---

## 4 Old NPE Code vs Optional

### OLD (Null Check)
```java
User user = userService.findByEmail("xyz@email.com");
if (user != null) {
    Address addr = user.getAddress();
    if (addr != null) {
        String city = addr.getCity();
        if (city != null) {
            // use city
        }
    }
}
```

### NEW (Optional)
```java
String city = userService.findByEmail("xyz@email.com")
    .map(User::getAddress)
    .map(Address::getCity)
    .orElse("Unknown");
```

```
= 1 chain replaces nested if-null pyramid
```

---

## 5 Checking Presence

```java
Optional<String> opt = Optional.of("Hi");

opt.isPresent();    // true
opt.isEmpty();      // false  (Java 11+)

// OLD STYLE (avoid):
if (opt.isPresent()) {
    String val = opt.get();
    // use val
}

// MODERN STYLE:
opt.ifPresent(val -> System.out.println(val));
```

---

## 6 Getting Value (4 Ways)

```java
Optional<String> opt = ...;

// 1. get() — DANGEROUS (NoSuchElementException if empty)
String val = opt.get();

// 2. orElse(default) — return default if empty
String val = opt.orElse("default");

// 3. orElseGet(supplier) — lazy default
String val = opt.orElseGet(() -> expensiveCall());

// 4. orElseThrow() — throw custom exception
String val = opt.orElseThrow(() -> new UserNotFoundException());
```

```
Use:
   orElse(x)        → cheap default
   orElseGet(sup)   → expensive default (lazy)
   orElseThrow(...) → must have, else fail
   get()            → AVOID (lazy NPE replacement)
```

---

## 7 Visual — 4 Get Patterns

```
   Optional<String> name
        │
        ▼
   ┌─────────────────────────────┐
   │  Empty?                       │
   └──┬──────────────────────────┘
      │
      ├── opt.orElse("default")
      │       → "default" if empty
      │
      ├── opt.orElseGet(() -> compute())
      │       → compute() if empty (lazy)
      │
      ├── opt.orElseThrow(() -> new Ex())
      │       → throw if empty
      │
      └── opt.get()
              → NoSuchElementException if empty
              (avoid)
```

---

## 8 Map / Filter on Optional

```java
Optional<String> name = Optional.of("Arpan");

// Map — transform if present
Optional<Integer> len = name.map(String::length);
// Optional[5]

// Filter — keep if predicate true
Optional<String> filtered = name.filter(s -> s.startsWith("A"));
// Optional[Arpan]

// Chaining
String upper = Optional.of("hello")
    .map(String::toUpperCase)
    .filter(s -> s.length() > 3)
    .orElse("EMPTY");
// "HELLO"
```

```
Stream-like API on Optional
   Empty propagates through chain
```

---

## 9 ifPresent / ifPresentOrElse

```java
Optional<String> opt = Optional.of("Hi");

// ifPresent — run action if present
opt.ifPresent(s -> System.out.println(s));

// ifPresentOrElse (Java 9+) — both branches
opt.ifPresentOrElse(
    s -> System.out.println("Got: " + s),
    () -> System.out.println("Empty")
);
```

---

## Common Use Cases

### Repository Returns
```java
public interface UserRepository {
    Optional<User> findByEmail(String email);
}

// Usage:
User user = userRepo.findByEmail(email)
    .orElseThrow(() -> new UserNotFoundException(email));
```

### Chaining
```java
String city = userService.find(id)
    .map(User::getAddress)
    .map(Address::getCity)
    .orElse("Unknown");
```

---

## 11 Common Traps

### Trap 1: Using get() everywhere
```java
// Defeats Optional purpose
if (opt.isPresent()) {
    return opt.get();
}
return "default";

// Use orElse instead:
return opt.orElse("default");
```

### Trap 2: Optional fields/parameters
```java
// DON'T
class User {
    private Optional<String> name;   // anti-pattern
}

// DON'T
public void setName(Optional<String> name) { }

// Use Optional only as RETURN TYPE
public Optional<String> getName() {
    return Optional.ofNullable(name);
}
```

### Trap 3: Optional with primitives
```java
// Boxing overhead
Optional<Integer> optInt;

// Use specialized:
OptionalInt optInt;
OptionalLong optLong;
OptionalDouble optDouble;
```

---

## 12 Real Production Example

```java
// Spring Data JPA repository
public interface UserRepository extends JpaRepository<User, Long> {
    Optional<User> findByEmail(String email);
}

// Service
public UserDTO getUser(String email) {
    return userRepo.findByEmail(email)
        .map(this::toDTO)               // transform if present
        .orElseThrow(() ->              // fail if absent
            new UserNotFoundException(email)
        );
}
```

---

## Memory Hooks

```
Optional = envelope (may have content or empty)

Create:
   of(val)         → NPE if null
   ofNullable(val) → safe
   empty()         → explicit empty

Get:
   orElse(x)         → default
   orElseGet(sup)    → lazy default
   orElseThrow(...)  → fail if empty
   get()             → avoid

Chain:
   map() / filter() — stream-like
   ifPresent() / ifPresentOrElse()

Use cases:
   Return type (Repository methods)
   Fields, parameters
```

[← Back to JAVA](../)
