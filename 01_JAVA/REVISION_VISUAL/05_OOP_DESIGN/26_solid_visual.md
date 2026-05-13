# 🏗️ SOLID — Visual Revision (OOP Design)

---

## 1️⃣ SOLID = 5 Principles for Clean OOP

```
   S  →  Single Responsibility  (one class, one job)
   O  →  Open/Closed             (open for extension, closed for modification)
   L  →  Liskov Substitution     (child should replace parent safely)
   I  →  Interface Segregation   (don't force unused methods)
   D  →  Dependency Inversion    (depend on abstractions, not concrete)
```

---

## 2️⃣ S — Single Responsibility (SRP)

### Concept
```
Ek class ka SIRF ONE reason for change hona chahiye.
   = One responsibility per class
```

### Restaurant Analogy
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

### Code
```java
// ❌ VIOLATION
class User {
    void save() { ... }        // DB
    void sendEmail() { ... }   // Email
    void formatDate() { ... }  // Formatting
    // 3 responsibilities!
}

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

## 3️⃣ O — Open/Closed (OCP)

### Concept
```
Class OPEN for extension
Class CLOSED for modification

= Add new behavior WITHOUT changing existing code
```

### Phone Charger Analogy
```
   Charger me USB-C port hai
   
   Old phone? Use adapter (extension)
   New iPhone? Use adapter
   
   Charger KHUD nahi badla (closed)
   Adapters extend it (open)
```

### Code
```java
// ❌ VIOLATION (modify every time new shape)
class AreaCalculator {
    double calc(Object shape) {
        if (shape instanceof Circle) { ... }
        else if (shape instanceof Square) { ... }
        else if (shape instanceof Triangle) { ... }  // keep adding!
    }
}

// ✅ OCP — extend, don't modify
interface Shape {
    double area();
}

class Circle implements Shape { ... }
class Square implements Shape { ... }
class Triangle implements Shape { ... }
// New shape? Just implement Shape — no calc change

class AreaCalculator {
    double calc(Shape s) {
        return s.area();   // ✅ never changes
    }
}
```

---

## 4️⃣ L — Liskov Substitution (LSP)

### Concept
```
Child class should be USABLE in place of parent
   WITHOUT breaking behavior
```

### Square vs Rectangle Classic Trap
```
GALAT BELIEF:
   Square IS-A Rectangle
   So: class Square extends Rectangle
```

```java
// ❌ VIOLATES LSP
class Rectangle {
    int width, height;
    void setWidth(int w) { width = w; }
    void setHeight(int h) { height = h; }
}

class Square extends Rectangle {
    void setWidth(int w) {
        width = w;
        height = w;   // forced equal
    }
    void setHeight(int h) {
        width = h;
        height = h;
    }
}

// Test:
void test(Rectangle r) {
    r.setWidth(5);
    r.setHeight(10);
    assert r.area() == 50;   // ❌ FAILS with Square (area = 100)
}
```

```
✅ Fix:
   Don't force inheritance where behavior breaks
   Use composition or separate hierarchy
```

### Real-World
```
Parent: Bird (has fly method)
Child: Penguin extends Bird
   Penguin.fly() = ???  ❌ can't fly!
   
Fix:
   Bird interface
   FlyingBird, NonFlyingBird subtypes
```

---

## 5️⃣ I — Interface Segregation (ISP)

### Concept
```
Don't force class to implement methods it doesn't need.
Multiple SMALL interfaces > ONE BIG interface
```

### Restaurant Worker Analogy
```
❌ WRONG: "AllInOneWorker" interface
   • cookFood()
   • cleanFloor()
   • takeOrders()
   • managePayments()
   
   Chef? Forced to implement cleanFloor() too
   = Useless empty methods
```

```java
// ❌ VIOLATION
interface Worker {
    void cookFood();
    void cleanFloor();
    void takeOrders();
    void managePayments();
}

class Chef implements Worker {
    void cookFood() { ... }
    void cleanFloor() { }    // FORCED empty
    void takeOrders() { }     // FORCED empty
    void managePayments() { } // FORCED empty
}
```

```java
// ✅ ISP — segregate into smaller interfaces
interface Cook {       void cookFood(); }
interface Cleaner {    void cleanFloor(); }
interface Waiter {     void takeOrders(); }
interface Cashier {    void managePayments(); }

class Chef implements Cook { ... }
class FloorCleaner implements Cleaner { ... }

// HeadChef does cooking + cashier
class HeadChef implements Cook, Cashier { ... }
```

---

## 6️⃣ D — Dependency Inversion (DIP)

### Concept
```
HIGH-level modules should NOT depend on LOW-level modules.
BOTH should depend on ABSTRACTIONS (interfaces).
```

### Light Switch Analogy
```
❌ Bad design:
   Light switch HARDCODED to specific bulb brand
   "Philips bulb only"
   Need new bulb? Rewire entire switch

✅ Good design:
   Light switch works on "Bulb" interface
   Any bulb (Philips, GE, Syska) works
   = Abstraction-based
```

### Code
```java
// ❌ VIOLATION (hard-coded dependency)
class UserService {
    MySqlRepository repo = new MySqlRepository();  // concrete!
    
    void save(User u) {
        repo.save(u);
    }
}
// Change DB? Rewrite UserService.
```

```java
// ✅ DIP — depend on abstraction
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

## 7️⃣ Visual Summary

```
   S  →  ek class, ek kaam
   O  →  extend karo, modify nahi
   L  →  child parent ko replace karte safely
   I  →  chhote interfaces > bada interface
   D  →  abstraction par depend karo, concrete par nahi
```

---

## 8️⃣ Real Spring Boot Example (All 5 Together)

```java
// S — Each class has one responsibility
@Service
public class UserService {           // business logic
    private final UserRepository repo;
    
    // D — depends on interface, not concrete
    public UserService(UserRepository repo) {
        this.repo = repo;
    }
    
    public User create(User u) { ... }
}

// O — Open for extension via interface
public interface NotificationSender {
    void send(String msg);
}

public class EmailSender implements NotificationSender { ... }
public class SmsSender implements NotificationSender { ... }
// Add SlackSender? Just implement, no other code changes

// I — segregated interfaces
public interface UserReader {
    User findById(Long id);
}
public interface UserWriter {
    User save(User u);
}
// Different services need only the part they care about

// L — children can replace parents
@Service
public class CachedUserService extends UserService { ... }
// CachedUserService usable wherever UserService expected
```

---

## 9️⃣ Memory Hooks

```
S — Single Responsibility
    🎯 ek class ek kaam

O — Open/Closed
    🔌 charger + adapter (extend, don't change)

L — Liskov Substitution
    🐧 Penguin shouldn't extend FlyingBird

I — Interface Segregation
    👨‍🍳 Chef shouldn't implement cleanFloor()

D — Dependency Inversion
    💡 Switch works on Bulb interface, not Philips concrete
```

---

## 🔟 Why SOLID Matters

```
✅ Maintainable     — change one thing, others stable
✅ Testable         — easy to mock, swap
✅ Extensible       — new features without breaking
✅ Reusable         — small focused units
✅ Readable         — each class clear purpose

= Foundation for clean OOP code
= Spring's DI is built on D principle
```

📚 [← Back to JAVA](../../)
