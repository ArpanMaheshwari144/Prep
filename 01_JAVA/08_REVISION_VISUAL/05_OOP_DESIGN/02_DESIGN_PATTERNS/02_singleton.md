# SINGLETON — Only ONE Instance

---

## Problem Pehle

```
Imagine DB connection:

   class DatabaseConnection {
      // costly setup (10 sec connect)
      // pool of 50 connections
   }
```

```java
// Without Singleton — har koi naya banaye:
DatabaseConnection db1 = new DatabaseConnection();  // 10 sec
DatabaseConnection db2 = new DatabaseConnection();  // 10 sec
DatabaseConnection db3 = new DatabaseConnection();  // 10 sec
```

```
Problems:
   Memory waste
   Database connections duplicate
   Connection pool exhaust
   Inconsistent state
```

---

## President Analogy

```
Country mein PRESIDENT sirf EK
   2 presidents ban nahi sakte
   Sab unhi ko reference karte

   = SINGLETON
```

---

## Simple Approach

```java
public class DatabaseConnection {

    // 1. STATIC instance (class-level)
    private static DatabaseConnection instance;

    // 2. PRIVATE constructor (bahar ka 'new' block kare)
    private DatabaseConnection() {
        // costly setup
    }

    // 3. Public method to get instance
    public static DatabaseConnection getInstance() {
        if (instance == null) {
            instance = new DatabaseConnection();
        }
        return instance;
    }
}
```

### Usage

```java
DatabaseConnection db1 = DatabaseConnection.getInstance();
DatabaseConnection db2 = DatabaseConnection.getInstance();

db1 == db2;   // TRUE (same object)

// 'new' nahi kar sakte:
new DatabaseConnection();   // compile error (private)
```

---

## Visual Flow

```
   First call:
   getInstance()
        │
        ▼
   instance == null? → YES
        │
        ▼
   instance = new DatabaseConnection()  ← created here
        │
        ▼
   return instance

   Second/Third call:
   getInstance()
        │
        ▼
   instance == null? → NO
        │
        ▼
   return EXISTING instance
```

---

## Thread Issue (Simple Approach Problem)

```
Thread A:                Thread B:
   getInstance()            getInstance()
   instance == null?        instance == null?
   YES                      YES                ← BOTH check SAME time!
   create instance          create instance    ← BOTH create

   = 2 instances created
   = Singleton BROKEN
```

---

## BEST Approach — Enum (Joshua Bloch)

```java
public enum DatabaseConnection {
    INSTANCE;

    public void query(String sql) {
        // ...
    }
}
```

### Usage

```java
DatabaseConnection.INSTANCE.query("SELECT *");
```

```
Why Enum BEST?
   JVM guarantee karta — only ONE instance
   Thread-safe AUTOMATIC
   Serialization safe (default)
   Reflection se bhi break nahi hota
   ONE line code
```

---

## Real Spring Example

```java
@Service
public class UserService { ... }

// Spring beans = SINGLETON by default
// Same UserService instance everywhere
```

```
Spring uses Singleton heavily:
   @Service     → singleton
   @Component   → singleton
   @Repository  → singleton
   @Controller  → singleton

Tu kahin bhi @Autowired UserService karega
   = SAME object pura app mein
```

---

## When Use Singleton?

```
Database connection pool
Configuration manager
Logger
Cache
Thread pool

Stateless objects
Multi-instance needed (different configs)
```

---

## Memory Hook

```
Singleton = President (only ONE)

3 cheez chahiye:
   1. private constructor
   2. static instance
   3. public getInstance()

BEST = Enum INSTANCE (one line, all safe)
```

[← Back to Design Patterns](00_overview.md)
