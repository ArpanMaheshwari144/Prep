# Spring Basics — Visual Revision

---

## 1 Spring Kya Hai? (Hotel Analogy)

```
WITHOUT SPRING (DIY hotel):
   Tu khud:
      • Room dhundh
      • Chabhi banao
      • Bed lagao
      • Breakfast banao
   = Thakaan

WITH SPRING (5-star hotel):
   Tu reception pe:
      "Mujhe room chahiye"
      Hotel ne automatically:
         Room assign kiya
         Chabhi di
         Bed ready
         Breakfast schedule
   = Tu sirf USE kar
```

```
SPRING = 5-star hotel for your Java objects.
```

---

## 2 One-Line Truth

```
"Spring tere objects ko BANATA hai
 aur AAPAS MEIN JODTA hai automatically."
```

---

## 3 Without vs With Spring (Code)

### Without Spring
```java
UserRepository repo = new UserRepository();
EmailService email = new EmailService();
UserService userService = new UserService(repo, email);

// Manual plumbing
// Bohot dependencies = chaos
```

### With Spring
```java
@Service
public class UserService {
    @Autowired UserRepository repo;
    @Autowired EmailService email;
}

// Spring ne KHUD bana liya
// Spring ne KHUD jod diya
```

---

## 4 IoC + DI = Spring Core

```
   IoC = Inversion of Control
   DI  = Dependency Injection
```

### NORMAL (without IoC)
```
   "Mei objects banaunga jab chaiye"
   Tu CONTROL karta — kab banega, kaha banega.
```

### INVERSION (with IoC)
```
   "Spring banaye, mai bus use karu"
   CONTROL flip ho gaya
   Spring banata, tu use karta
   = INVERSION OF control
```

---

## 5 Spring Container — Visual

```
   ┌──────────────────────────────────────┐
   │       SPRING CONTAINER               │
   │       (IoC Container)                │
   │                                       │
   │   ┌────────┐  ┌────────┐  ┌────────┐│
   │   │UserSvc │  │UserRepo│  │EmailSvc││
   │   │ BEAN   │  │ BEAN   │  │ BEAN   ││
   │   └───┬────┘  └────┬───┘  └────┬───┘│
   │       │            │            │   │
   │       └────────────┴────────────┘   │
   │           Auto-wired together         │
   └──────────────────────────────────────┘

   Container ke andar SAB objects ("beans")
   Container hi banata, jodta, manage karta
```

---

## 6 Bean Types — Same Magic, Different Names

```
┌─────────────┬──────────────────────────────────┐
│  Annotation │  Use For                          │
├─────────────┼──────────────────────────────────┤
│ @Component  │ Generic bean                      │
│ @Service    │ Business logic                    │
│ @Repository │ DB access                         │
│ @Controller │ REST API                          │
│ @RestCtrl   │ REST API + JSON response          │
└─────────────┴──────────────────────────────────┘

   Sab ka kaam SAME — Spring ko bolna "yeh bean hai"
   Sirf naam alag = readability ke liye
```

---

## 7 @Autowired Visual

```java
@Service
public class UserService {

    @Autowired
    private UserRepository repo;
    //              ▲
    //              │
    //  "Spring, mujhe UserRepository ka object de"
    //   Spring container se bean inject hota
}
```

```
FLOW:
   App start hota
        ↓
   Spring scan karta @Service, @Repository, @Component, @Controller
        ↓
   Sab beans container mein register
        ↓
   @Autowired dekhke wire kar deta
        ↓
   App ready
```

---

## 8 DI — 3 Types

```
1. CONSTRUCTOR (recommended)
   ┌────────────────────────────────┐
   │ @Service                        │
   │ public class UserService {     │
   │     private final UserRepo repo;│
   │     public UserService(         │
   │              UserRepo repo) {   │
   │         this.repo = repo;       │
   │     }                           │
   │ }                               │
   └────────────────────────────────┘
   final possible (immutable)
   Mandatory dependencies
   Test friendly

2. SETTER
   @Autowired
   public void setRepo(UserRepo repo) {...}
   = Optional dependencies

3. FIELD
   @Autowired
   private UserRepo repo;
   Can't be final
   Hard to test
```

```
INDUSTRY CHOICE: Constructor (Lombok @RequiredArgsConstructor)
```

---

## 9 Bean Scope — Singleton vs Prototype

```
┌──────────────┬──────────────────────────────┐
│  Scope       │  Behavior                    │
├──────────────┼──────────────────────────────┤
│ singleton    │ ONE instance for whole app   │
│ (default)    │ shared across all classes    │
├──────────────┼──────────────────────────────┤
│ prototype    │ NEW instance every time      │
│              │ injected/requested           │
├──────────────┼──────────────────────────────┤
│ request      │ Per HTTP request             │
│ session      │ Per HTTP session             │
└──────────────┴──────────────────────────────┘

   90% beans = singleton (default)
   = Memory efficient, thread-safe stateless
```

---

## Full App Flow (Visual)

```
   STARTUP
      ↓
   @SpringBootApplication
      ↓
   Spring Boot scans packages
      ↓
   Finds @Service, @Repository, @Controller, @Component
      ↓
   Creates beans → puts in container
      ↓
   Resolves @Autowired → wires beans together
      ↓
   App READY
      ↓
   Request aati → @Controller → @Service → @Repository → DB
      ↓
   Response back
```

---

## 11 80% Spring = Yeh Pattern

```
   3 cheezein samjho:

   1. CONTAINER  → Spring ka manager
   2. BEAN       → container ke andar managed object
   3. AUTOWIRE   → bean ko inject kar

   = 80% Spring code yahi pattern
   = Baki sab variations
```

---

## 12 Memory Hook

```
Hotel = Spring Container
Receptionist = @Autowired ("yeh dega")
Room ready = Bean ready
Manager managing = IoC (control flip)

Tu USE karta, Spring BANATA + JODTA.
```

[← Back to SPRING](../)
