# 🔌 O — Open/Closed (OCP)

---

## Concept

```
Class OPEN for extension
Class CLOSED for modification

= Add new behavior WITHOUT changing existing code
```

---

## Phone Charger Analogy

```
Charger me USB-C port hai
   
   Old phone? Use adapter (extension)
   New iPhone? Use adapter
   
   Charger KHUD nahi badla (closed)
   Adapters extend it (open)
```

---

## Code

### ❌ VIOLATION (modify every time)
```java
class AreaCalculator {
    double calc(Object shape) {
        if (shape instanceof Circle) { ... }
        else if (shape instanceof Square) { ... }
        else if (shape instanceof Triangle) { ... }
        // keep adding ifs for new shapes!
    }
}
```

### ✅ OCP — Extend, Don't Modify
```java
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

## Real Spring Example

```java
public interface NotificationSender {
    void send(String msg);
}

public class EmailSender implements NotificationSender { ... }
public class SmsSender implements NotificationSender { ... }
// Add SlackSender? Just implement, no other code changes
```

---

## Memory Hook

```
🔌 charger + adapter
   "Naye phone aaye, charger nahi badla — adapter add"
   Extend karo, modify nahi
```

📚 [← Back to SOLID](00_overview.md)
