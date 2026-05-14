# 👨‍🍳 I — Interface Segregation (ISP)

---

## Concept

```
Don't force class to implement methods it doesn't need.
Multiple SMALL interfaces > ONE BIG interface
```

---

## Restaurant Worker Analogy

```
❌ WRONG: "AllInOneWorker" interface
   • cookFood()
   • cleanFloor()
   • takeOrders()
   • managePayments()
   
   Chef? Forced to implement cleanFloor() too
   = Useless empty methods
```

---

## Code

### ❌ VIOLATION
```java
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

### ✅ ISP — Segregate
```java
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

## Real Java Example

```java
// Java's Collections has segregated interfaces
List<String> list = ...;

// Different services need only part they care about
public interface UserReader {
    User findById(Long id);
}
public interface UserWriter {
    User save(User u);
}
// Read-only service → UserReader only
// Full CRUD service → both
```

---

## Memory Hook

```
👨‍🍳 Chef shouldn't implement cleanFloor()
   "Chhote-chhote interfaces banao,
    ek class ko sirf zaruri implement karne do"
```

📚 [← Back to SOLID](00_overview.md)
