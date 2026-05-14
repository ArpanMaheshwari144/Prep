# 🎯 S — Single Responsibility (SRP)

---

## Concept

```
Ek class ka SIRF ONE reason for change hona chahiye.
   = One responsibility per class
```

---

## Restaurant Analogy

```
WRONG (1 person doing all):
   Chef ne:
      ✅ Khana banaya
      ❌ Bartan dhoye
      ❌ Floor pocha
      ❌ Cash counter handle
   = Chaos, overworked, errors

RIGHT (specialized roles):
   Chef       → khana
   Cleaner    → cleaning
   Cashier    → money
   Manager    → coordinate
```

---

## Code

```java
// ❌ VIOLATION
class User {
    void save() { ... }        // DB
    void sendEmail() { ... }   // Email
    void formatDate() { ... }  // Formatting
    // 3 responsibilities!
}
```

```java
// ✅ SRP
class User { ... }              // Just data
class UserRepository {           // Just DB
    void save(User u) { ... }
}
class EmailService {             // Just email
    void send(User u) { ... }
}
class DateFormatter {            // Just dates
    String format(Date d) { ... }
}
```

---

## Real Spring Boot Example

```java
@Service
public class UserService {         // Only business logic
    // Data access → UserRepository
    // Notifications → EmailService
    // Each class = ONE job
}
```

---

## Memory Hook

```
🎯 ek class ek kaam
   "Ek class change karne ke liye SIRF EK reason"
```

📚 [← Back to SOLID](00_overview.md)
