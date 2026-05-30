# Builder Pattern — Step-by-Step Object Construction

> **Design Patterns: Topic 1 — Creational Pattern**

---

## STORY — Subway Sandwich vs Pre-made Burger

> Imagine **lunch order karna**:
>
> **Pre-made burger (no Builder):**
> Counter pe sirf 4-5 fixed combos available. *"Aapko Veggie Deluxe chahiye? Bina cheese, extra mayo? Sorry, fixed menu hai."* — limited options.
>
> **Subway sandwich (Builder pattern):**
> Tu **step-by-step build karta**: bread choose → veggies → sauce → cheese → toast. **Har step optional.** Final mein "build" button daba — sandwich ready.
>
> **Builder pattern same kaam karta** — complex object **piece-by-piece** banata, with optional fields.

---

## The Problem — Telescoping Constructor

Imagine **User class with 8 fields** (name, age, email, phone, address, role, isActive, createdAt):

```java
// Constructor telescoping nightmare
public class User {
    public User(String name) { ... }
    public User(String name, int age) { ... }
    public User(String name, int age, String email) { ... }
    public User(String name, int age, String email, String phone) { ... }
    public User(String name, int age, String email, String phone, String addr) { ... }
    // ... 7-8 overloaded constructors
}

// Caller side — readability nightmare
User u = new User("Arpan", 28, "x@y.com", null, "Delhi", null, true, null);
                                            ↑              ↑           ↑
                                    "phone null?"    "role?"     "createdAt?"
```

**Problems:**
1. **Too many constructors** — combinatorial explosion
2. **Null parameters confusing** — kya hai kya nahi?
3. **Order matters** — galat order = wrong field set
4. **No validation** between fields

---

## Builder Solution

```java
User u = User.builder()
    .name("Arpan")
    .age(28)
    .email("x@y.com")
    .role("ADMIN")
    .isActive(true)
    .build();
```

**Benefits:**
- **Readable** — har field ka naam clear
- **Optional fields** — sirf jo chahiye woh set
- **Order flexible** — kuch bhi pehle, kuch bhi baad
- **Immutable result** — `build()` ke baad object lock

---

## Visual — Builder Workflow

```
   User builder = User.builder()
        │
        │ .name("Arpan")     ← step 1
        ▼
   ┌────────────────┐
   │  Builder obj   │
   │  name=Arpan    │
   │  age=null      │
   │  email=null    │
   │  ...           │
   └────────┬───────┘
            │ .age(28)        ← step 2
            ▼
   ┌────────────────┐
   │  Builder obj   │
   │  name=Arpan    │
   │  age=28        │
   │  email=null    │
   └────────┬───────┘
            │ .email("x@y.com") ← step 3
            ▼
            ...
            │ .build()         ← final step
            ▼
   ┌────────────────┐
   │  IMMUTABLE     │
   │  User object   │  ← fields LOCKED, no setters
   │  name=Arpan    │
   │  age=28        │
   │  ...           │
   └────────────────┘
```

---

## PART 1: Manual Builder Implementation

```java
public class User {

    // ─── Fields (final = immutable after build) ──────
    private final String name;
    private final int age;
    private final String email;
    private final String role;
    private final boolean isActive;

    // ─── Private constructor — sirf Builder use karega
    private User(Builder builder) {
        this.name = builder.name;
        this.age = builder.age;
        this.email = builder.email;
        this.role = builder.role;
        this.isActive = builder.isActive;
    }

    // ─── Static factory method — User.builder() entry point
    public static Builder builder() {
        return new Builder();
    }

    // ─── Getters (no setters — immutable!)
    public String getName()    { return name; }
    public int getAge()         { return age; }
    public String getEmail()    { return email; }
    public String getRole()     { return role; }
    public boolean isActive()   { return isActive; }

    // ═══════════════════════════════════════════════
    //  INNER STATIC BUILDER CLASS
    // ═══════════════════════════════════════════════
    public static class Builder {

        // Mutable fields (to be set step by step)
        private String name;
        private int age;
        private String email;
        private String role = "USER";       // default value!
        private boolean isActive = true;     // default value!

        // Each setter returns "this" — fluent API
        public Builder name(String name)  {
            this.name = name;
            return this;
        }

        public Builder age(int age) {
            if (age < 0) throw new IllegalArgumentException("Age can't be negative");
            this.age = age;
            return this;
        }

        public Builder email(String email) {
            this.email = email;
            return this;
        }

        public Builder role(String role) {
            this.role = role;
            return this;
        }

        public Builder active(boolean active) {
            this.isActive = active;
            return this;
        }

        // FINAL build() — returns immutable User
        public User build() {
            // Optional: cross-field validation
            if (name == null || name.isBlank()) {
                throw new IllegalStateException("Name is required");
            }
            return new User(this);
        }
    }
}
```

### Usage

```java
User u = User.builder()
    .name("Arpan")
    .age(28)
    .email("arpan@x.com")
    .role("ADMIN")
    .build();           // isActive default true

System.out.println(u.getName());  // Arpan
```

---

## Visual — How `return this` enables chaining

```
   User.builder()                    ← returns Builder b1
        .name("Arpan")               ← sets name on b1, returns b1
        .age(28)                     ← sets age on b1, returns b1
        .email("x@y.com")            ← sets email on b1, returns b1
        .build()                     ← returns User (NEW immutable object)

   Each setter returns SAME builder reference → chain ho jata
   Final build() switches from Builder → User
```

---

## PART 2: Lombok @Builder (Production Way)

**Manual builder = 50+ lines of boilerplate.** Lombok ek annotation se sab generate kar deta:

```java
import lombok.Builder;
import lombok.Getter;

@Getter
@Builder
public class User {
    private String name;
    private int age;
    private String email;
    private String role;
    private boolean isActive;
}

// Lombok automatically generates:
// • Builder inner class
// • All setter methods returning this
// • build() method
// • static builder() factory method
// • Getters (from @Getter)
```

### Usage (same as before)

```java
User u = User.builder()
    .name("Arpan")
    .age(28)
    .email("arpan@x.com")
    .role("ADMIN")
    .isActive(true)
    .build();
```

### Lombok Builder advanced features

```java
@Builder(toBuilder = true)   // Allow rebuild from existing
public class User {
    private String name;

    @Builder.Default
    private String role = "USER";        // default if not set

    @Builder.Default
    private boolean isActive = true;
}

// Modify existing:
User user2 = u.toBuilder()
    .age(29)         // change age, keep rest
    .build();
```

---

## Manual vs Lombok — Comparison

| | Manual Builder | Lombok `@Builder` |
|---|---|---|
| **Code size** | ~80 lines for User | ~10 lines |
| **Maintenance** | Add field → update Builder | Auto-updated |
| **Custom validation** | Easy (add in setter/build) | Limited (use `@Builder.toBuilder` + custom build) |
| **Default values** | Easy in Builder fields | `@Builder.Default` annotation |
| **Production preference** | Avoid (boilerplate) | Industry standard |
| **Interview demonstration** | Good — shows pattern understanding | "Lombok use karta production" |

---

## PROJECT USAGE — UserCRUD mein Builder kahan kahan?

**Tu UserCRUD project mein Builder pattern 2 jagah USE kiya** — 100% live evidence:

### 1 `JwtService.java` — Token generate karte time

**File:** `02_SPRING/02_PROJECT/usercrud/src/main/java/com/arpan/usercrud/security/JwtService.java`

```java
public String generateToken(User user) {
    Date now = new Date();
    Date expiryDate = new Date(now.getTime() + expiration);

    return Jwts.builder()                           // ← BUILDER PATTERN
            .subject(user.getId().toString())       // step 1: sub claim
            .claim("name", user.getName())          // step 2: custom claim
            .claim("email", user.getEmail())        // step 3
            .claim("role", user.getRole())          // step 4
            .issuedAt(now)                          // step 5
            .expiration(expiryDate)                 // step 6
            .signWith(getSigningKey())              // step 7: signature
            .compact();                             // FINAL build (returns String JWT)
}
```

**Notice:**
- `Jwts.builder()` returns a `JwtBuilder` instance
- Each setter (`subject()`, `claim()`, etc.) returns `this` → fluent chain
- `compact()` is the "build()" equivalent — returns final signed JWT string
- **All claims optional** — only set what's needed
- **Order flexible** — claim() ko subject() se pehle ya baad mein set kar sakta

**Yeh exactly Builder pattern hai** — jjwt library iska use karke clean, readable token generation provide karta.

---

### 2 `CustomUserDetailsService.java` — Spring UserDetails build karte time

**File:** `02_SPRING/02_PROJECT/usercrud/src/main/java/com/arpan/usercrud/security/CustomUserDetailsService.java`

```java
@Override
public UserDetails loadUserByUsername(String email) {
    User user = userRepository.findByEmail(email)
            .orElseThrow(() -> new UsernameNotFoundException(...));

    return org.springframework.security.core.userdetails.User.builder()  // ← BUILDER
            .username(user.getEmail())
            .password(user.getPassword())
            .authorities("ROLE_" + user.getRole())
            .build();                                                     // ← BUILD
}
```

**Notice:**
- Spring Security ka **`User.builder()`** static factory
- Setters: `username()`, `password()`, `authorities()` — fluent
- **`build()`** finalizes immutable `UserDetails` object
- **disabled, accountLocked** jaisi optional flags bhi set kar sakte hain — jo nahi set kiye, defaults

---

### Interview-ready statement

> *"Maine UserCRUD project mein **Builder pattern 2 jagah use kiya**:
> 1. **JWT generate** karte time `Jwts.builder()` se token banaya — fluent API se claims set kiye, `signWith()` se signature lagaya, `compact()` se final string return.
> 2. **Spring Security UserDetails** build karte time `User.builder()` use kiya — username, password, authorities set karke `build()` se immutable UserDetails object banaya.
>
> Both use cases mein **same pattern**: step-by-step optional setters, fluent chaining via `return this`, final `build()`/`compact()` immutable target return karta. Yeh production-grade Builder pattern ka real evidence hai mere code mein."*

---

## Real-World Examples (already familiar)

### 1. `StringBuilder` (Java built-in)
```java
String s = new StringBuilder()
    .append("Hello")
    .append(" ")
    .append("World")
    .toString();   // "Hello World"
```

### 2. `Stream.Builder` (Java 8+)
```java
Stream<String> stream = Stream.<String>builder()
    .add("a")
    .add("b")
    .add("c")
    .build();
```

### 3. **Lombok `@Builder`** (most production code uses this)
```java
@Builder
public class User { ... }

User u = User.builder().name("Arpan").age(28).build();
```

### 4. **Spring `RestTemplate`/`HttpClient` builders**
```java
HttpRequest request = HttpRequest.newBuilder()
    .uri(URI.create("https://api.com"))
    .header("Auth", "Bearer xyz")
    .GET()
    .build();
```

### 5. **Project mein dekha — JWT (jjwt)**
```java
String jwt = Jwts.builder()
    .subject(userId)
    .claim("role", role)
    .expiration(date)
    .signWith(key)
    .compact();
```

**Tu Builder pattern already use kar chuka** — bus consciously identify nahi kiya tha.

---

## When to Use Builder?

| Use Builder when... | Don't use when... |
|---|---|
| Class has **many fields** (5+) | Class has 2-3 fields |
| **Optional parameters** common | All fields mandatory |
| **Immutable object** chahiye | Mutable object preferred |
| **Step-by-step construction** | Simple direct construction OK |
| **Validation across fields** needed | Simple validation per field |

---

## Key Design Decisions

1. **Inner static class** — `User.Builder` accessible without User instance
2. **Private constructor** — only Builder can construct User (forces builder use)
3. **Final fields** — User immutable after `build()`
4. **`return this`** — enables method chaining (fluent API)
5. **build() validates** — last-chance to enforce invariants

---

## INTERVIEW TALKING POINT

**Q: "Builder pattern kya hai aur kab use karte ho?"**

> *"Builder = creational pattern jo complex objects ko **step-by-step build** karta with optional fields. Solves **telescoping constructor problem** — jab class mein 5+ fields ho with mix of mandatory + optional. Caller code readable hota — har field naam ke saath set karte, order flexible.
>
> Manual implementation: **inner static Builder class**, har setter `return this` returns (fluent chaining), final `build()` immutable target object return karta. Production mein **Lombok `@Builder`** use karte — annotation se boilerplate auto-generated. Real examples — `StringBuilder`, `Jwts.builder()` (jjwt), `HttpRequest.newBuilder()` (Java 11+)."*

**Q: "Telescoping constructor problem kya?"**

> *"Class mein bahut saare overloaded constructors — class fields zyada hone par. `User(name)`, `User(name, age)`, `User(name, age, email)` — combinatorial explosion. Caller side `null` parameters confusing. Builder pattern solve karta — fluent API, named-parameters style, optional fields skip kar sakte."*

**Q: "Builder ke alternatives?"**

> *"3 main:
> 1. **Setter-based** — but mutable + no validation enforcement
> 2. **Static factory methods** (`User.of(...)`) — limited combinations
> 3. **Builder** — most flexible, immutable, validated
>
> Modern Java mein **Records (Java 14+)** + factory methods bhi alternative — but Builder still preferred for >5 fields."*

**Q: "Lombok @Builder vs manual?"**

> *"Production mein `@Builder` use karta — boilerplate eliminate, auto-update on field change. Interview mein manual show kar sakta to demonstrate understanding. `@Builder.Default` defaults ke liye, `toBuilder = true` immutable update pattern ke liye."*

---

## POWER PHRASE

> **"Builder pattern complex objects ko step-by-step construct karta — solves telescoping constructor problem. Inner static Builder class, fluent API (`return this`), immutable target object via private constructor + final fields. Production mein Lombok `@Builder` boilerplate eliminate karta."**

---

## MEMORY HOOK

```
Builder = "Subway sandwich"
   • Step-by-step build
   • Optional ingredients
   • Final assemble

Manual code:
   1. Inner static Builder class
   2. Builder fields = mutable
   3. Setters return this (chaining)
   4. build() returns immutable target
   5. Target's constructor private (forces builder use)

Lombok shortcut:
   @Builder annotation = sab auto-generate

Real-world examples:
   • StringBuilder
   • StreamBuilder
   • Jwts.builder()  (JWT mein use kiya)
   • HttpRequest.newBuilder()
   • Lombok @Builder

Use when:
   5+ fields
   Optional params common
   Immutable object needed
```

---

## TRAP BOX

```
Trap 1: "Builder = setter pattern"
         NAHI — setter mutable, Builder immutable
         Builder construct karta, finalize ek baar

Trap 2: "Builder always thread-safe"
         NAHI — Builder itself mutable, NOT thread-safe
         Final User object thread-safe (immutable)

Trap 3: "Lombok @Builder defaults silent kaam karta"
         Field initialization SE NAHI — @Builder.Default chahiye
         Without it, default ignored when build() called

Trap 4: "Builder chain mein order matter karta"
         NAHI — independent setters, any order
         Validation cross-field hai → build() mein check karo

Trap 5: "Builder pattern slow hai"
         Negligible overhead — JIT optimizes
         Readability gain >> performance cost
```

---

## Builder vs Other Creational Patterns

| Pattern | Use Case | Complexity |
|---|---|---|
| **Builder** | Many fields, step-by-step, immutable | Medium |
| **Factory** | Choose subtype based on input | Low |
| **Abstract Factory** | Family of related objects | High |
| **Singleton** | Single global instance | Low |
| **Prototype** | Clone existing object | Medium |

---

## Quick Recall Summary

```
WHAT     → Step-by-step object construction with optional fields
WHY      → Solves telescoping constructor + improves readability
HOW      → Inner static Builder class + fluent setters + build()
WHERE    → 5+ fields, immutable object, mix of optional/mandatory
PRODUCTION → Lombok @Builder
EXAMPLES → StringBuilder, Jwts.builder(), HttpRequest.newBuilder()
```
