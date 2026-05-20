# BUILDER — Step by Step Object Creation

---

## Problem Pehle

```
Imagine User class banana:

   class User {
      String name, email, phone, role, address;
      int age;
      boolean isActive;
   }
```

```java
// Constructor banana ho with all:
new User("Arpan", "a@gmail.com", "9999", 27,
        "USER", "Bangalore", true);
```

```
Problems:
   7 params — order yaad rakhna mushkil
   Kuch optional hain (phone null bhi ho sakta)
   Constructor mein 7 versions banane padenge
        new User(name)
        new User(name, email)
        new User(name, email, phone)
        ...
   = MESS
```

---

## Pizza Analogy

```
Tu pizza order karta hai —
   sab toppings ek saath nahi bolta
   step by step kahta:
      "size medium"
      "cheese extra"
      "pepperoni daalo"
      "ab bana do"
```

---

## Builder Solution

```java
class User {
    String name;
    String email;
    String phone;
    int age;
    String role;
    
    // Private constructor — only Builder banae
    private User(Builder b) {
        this.name = b.name;
        this.email = b.email;
        this.phone = b.phone;
        this.age = b.age;
        this.role = b.role;
    }
    
    public static class Builder {
        String name;
        String email;
        String phone;
        int age;
        String role = "USER";   // default
        
        public Builder name(String n) {
            this.name = n;
            return this;        // ← KEY: return Builder for chain
        }
        public Builder email(String e) {
            this.email = e;
            return this;
        }
        public Builder phone(String p) {
            this.phone = p;
            return this;
        }
        public Builder age(int a) {
            this.age = a;
            return this;
        }
        public Builder role(String r) {
            this.role = r;
            return this;
        }
        
        public User build() {
            return new User(this);   // ← creates actual User
        }
    }
}
```

---

## Usage Visual

```java
User user = new User.Builder()
                    .name("Arpan")        // returns Builder
                    .email("a@gmail.com") // returns Builder
                    .age(27)              // returns Builder
                    .build();             // FINALLY returns User
```

```
Flow:
   new Builder()    → empty Builder
        .name(x)    → Builder with name
        .email(x)   → Builder with name + email
        .age(x)     → Builder with name + email + age
        .build()    → User object created
```

---

## Magic = `return this;`

```
Har setter method Builder return karta
Iss reason se .name().email().age() chain hota
Last mein .build() final User banake deta

= FLUENT API
```

---

## WITHOUT `return this` — Kya Hoga?

```java
public Builder name(String n) {
    this.name = n;
    // no return → void
}

// Chain TOOT jata:
new User.Builder()
        .name("Arpan")      // returns void!
        .email("...")        // compile error
        .build();
```

```
WITHOUT return this:
   Builder b = new Builder();
   b.name("Arpan");
   b.email("a@gmail");
   User u = b.build();
   = Multi-line manual, ugly

WITH return this:
   ONE-line chain
   = Builder ka asli magic
```

---

## Lombok Shortcut (Real World)

```java
@Builder
class User {
    String name;
    String email;
    int age;
}

// Lombok ne automatically Builder class generate ki
User u = User.builder()
             .name("Arpan")
             .email("a@gmail.com")
             .age(27)
             .build();
```

```
Production:
   Manually Builder class nahi likhta
   @Builder annotation = ban jata
```

---

## Real Examples Around You

```java
// JWT token banana
Jwts.builder()
    .subject(userId)
    .issuedAt(now)
    .expiration(expiry)
    .signWith(key)
    .compact();

// HTTP request
HttpRequest.newBuilder()
           .uri(URI.create("..."))
           .GET()
           .build();

// Stream collecting
Stream.builder()
      .add("a")
      .add("b")
      .build();
```

---

## When Use Builder?

```
Class mein > 4 parameters
Some optional fields
Immutable object banana hai
Readable construction chahiye

2-3 params ki simple class
Java Record (already concise)
```

---

## Memory Hook

```
Builder = Pizza step-by-step

Constructor mein 10 params NAHI
   .field().field().field().build()
   
"return this" = chain ka secret
```

[← Back to Design Patterns](00_overview.md)
