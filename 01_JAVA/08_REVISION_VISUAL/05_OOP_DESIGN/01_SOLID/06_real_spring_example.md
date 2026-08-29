# Real Spring Example — All 5 SOLID Together

---

## Concept

```
Real Spring Boot service mein
   saare 5 SOLID principles ek saath kaise apply hote
   = One example, 5 principles visible
```

---

## Complete Code Example

```java
// ═══════════════════════════════════════════════════
// S — Single Responsibility
// Each class has ONE job
// ═══════════════════════════════════════════════════

@Service
public class UserService {           // ← only business logic

    // ═══════════════════════════════════════════════════
    // D — Dependency Inversion
    // Depends on INTERFACE, not concrete class
    // ═══════════════════════════════════════════════════
    private final UserRepository repo;

    public UserService(UserRepository repo) {  // constructor injection
        this.repo = repo;
    }

    public User create(User u) {
        return repo.save(u);
    }
}


// ═══════════════════════════════════════════════════
// O — Open/Closed
// Extend via new implementations, no modification
// ═══════════════════════════════════════════════════

public interface NotificationSender {
    void send(String msg);
}

public class EmailSender implements NotificationSender {
    public void send(String msg) { /* email */ }
}

public class SmsSender implements NotificationSender {
    public void send(String msg) { /* sms */ }
}

// Add SlackSender? Just implement, no other code changes!


// ═══════════════════════════════════════════════════
// I — Interface Segregation
// Small focused interfaces, not one big interface
// ═══════════════════════════════════════════════════

public interface UserReader {
    User findById(Long id);
    List<User> findAll();
}

public interface UserWriter {
    User save(User u);
    void delete(Long id);
}

// Read-only service → UserReader only
// Full CRUD service → both interfaces


// ═══════════════════════════════════════════════════
// L — Liskov Substitution
// Subclass usable wherever parent expected
// ═══════════════════════════════════════════════════

@Service
public class CachedUserService extends UserService {

    private final Cache cache;

    @Override
    public User create(User u) {
        User saved = super.create(u);
        cache.put(u.getId(), saved);
        return saved;
    }
}

// CachedUserService usable WHEREVER UserService expected
// No surprises — same contract
// LSP satisfied
```

---

## Visual Summary

```
   UserService (S — single job)
        │
        ├── depends on UserRepository INTERFACE (D)
        │
        ├── UserReader / UserWriter — small interfaces (I)
        │
        ├── NotificationSender — extensible (O)
        │
        └── CachedUserService extends — substitutable (L)

   = All 5 SOLID applied in ONE realistic service
```

---

## Memory Hook

```
Spring + SOLID = natural fit:
   @Service           → SRP
   @Autowired         → DIP
   Multiple impls     → OCP
   Small interfaces   → ISP
   Override safely    → LSP

   = Production-grade architecture
```

[← Back to SOLID](00_overview.md)
