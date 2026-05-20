# D — Dependency Inversion (DIP)

---

## Concept

```
HIGH-level modules should NOT depend on LOW-level modules.
BOTH should depend on ABSTRACTIONS (interfaces).
```

---

## Light Switch Analogy

```
Bad design:
   Light switch HARDCODED to specific bulb brand
   "Philips bulb only"
   Need new bulb? Rewire entire switch

Good design:
   Light switch works on "Bulb" interface
   Any bulb (Philips, GE, Syska) works
   = Abstraction-based
```

---

## Code

### VIOLATION (hard-coded dependency)
```java
class UserService {
    MySqlRepository repo = new MySqlRepository();  // concrete!
    
    void save(User u) {
        repo.save(u);
    }
}
// Change DB? Rewrite UserService.
```

### DIP — Depend on Abstraction
```java
interface UserRepository {
    void save(User u);
}

class MySqlRepository implements UserRepository { ... }
class PostgresRepository implements UserRepository { ... }

class UserService {
    private final UserRepository repo;
    
    public UserService(UserRepository repo) {  // inject
        this.repo = repo;
    }
    
    void save(User u) {
        repo.save(u);
    }
}

// Spring injects whichever bean is configured
// = Switch DB without touching UserService
```

---

## Spring Connection

```
Spring's entire DI mechanism = DIP principle automated:

   @Service
   public class UserService {
       
       @Autowired              ← inject abstraction
       private UserRepository repository;
   }

   = Tu interface use karta
   = Spring concrete inject karta
   = Pure DIP
```

---

## Memory Hook

```
Switch works on Bulb interface, not Philips concrete
   "Concrete pe nahi, abstraction pe depend"
   Spring DI = DIP automated
```

[← Back to SOLID](00_overview.md)
