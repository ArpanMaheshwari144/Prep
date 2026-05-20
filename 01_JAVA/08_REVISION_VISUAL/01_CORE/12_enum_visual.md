# Enum — Visual Revision

---

## 1️⃣ Problem — Fixed Set Of Values

```
Imagine traffic light:
   Sirf 3 values possible:
      RED
      YELLOW
      GREEN
   
   Koi 4th value nahi hoti
   = FIXED SET
```

```
Same Java mein:
   • Days: MON, TUE, WED, THU, FRI, SAT, SUN
   • Direction: NORTH, SOUTH, EAST, WEST
   • Status: ACTIVE, INACTIVE, PENDING
   
   Limited choices = use ENUM
```

---

## 2️⃣ Without Enum (Painful Way)

```java
class TrafficLight {
    public static final int RED = 1;
    public static final int YELLOW = 2;
    public static final int GREEN = 3;
}

// Usage:
int light = TrafficLight.RED;

// Problems:
//   Could pass ANY int (light = 99)
//   No type safety
//   Hard to read in debugger (int 1 = what?)
//   Switch errors easy
```

---

## 3️⃣ With Enum (Clean)

```java
enum TrafficLight {
    RED, YELLOW, GREEN
}

// Usage:
TrafficLight light = TrafficLight.RED;
// light = 99 → compile error
// = TYPE SAFE
```

```
Benefits:
   Limited to defined values
   Type safe
   Readable
   Switch-friendly
```

---

## 4️⃣ Enum Built-in Methods

```java
enum Day { MON, TUE, WED, THU, FRI, SAT, SUN }

// values() — get all
Day[] all = Day.values();
// [MON, TUE, WED, THU, FRI, SAT, SUN]

// valueOf(String) — parse from string
Day d = Day.valueOf("MON");

// name() — get string name
String s = Day.MON.name();    // "MON"

// ordinal() — position (0-indexed)
int pos = Day.WED.ordinal();  // 2 (0-indexed)
```

---

## 5️⃣ Enum in Switch (Clean)

```java
enum Status { ACTIVE, INACTIVE, PENDING }

void handle(Status s) {
    switch (s) {
        case ACTIVE:
            // ...
            break;
        case INACTIVE:
            // ...
            break;
        case PENDING:
            // ...
            break;
    }
}

// Compiler warns if you miss a case
```

---

## 6️⃣ Enum with FIELDS (Power Mode)

```java
enum Planet {
    EARTH(5.97e24, 6_378_137),
    MARS(6.42e23, 3_389_500),
    JUPITER(1.90e27, 69_911_000);
    
    private final double mass;
    private final double radiusMeters;
    
    Planet(double mass, double radius) {
        this.mass = mass;
        this.radiusMeters = radius;
    }
    
    public double getMass() { return mass; }
    public double getRadius() { return radiusMeters; }
}

// Usage:
Planet.EARTH.getMass();        // 5.97e24
Planet.MARS.getRadius();       // 3389500
```

```
Enum sirf constants nahi —
   Full class banaya ja sakta hai
   Fields, methods, constructor (private)
```

---

## 7️⃣ Enum with ABSTRACT Methods (Polymorphism!)

```java
enum Operation {
    PLUS {
        public int apply(int a, int b) { return a + b; }
    },
    MINUS {
        public int apply(int a, int b) { return a - b; }
    },
    TIMES {
        public int apply(int a, int b) { return a * b; }
    };
    
    public abstract int apply(int a, int b);
}

// Usage:
Operation.PLUS.apply(5, 3);    // 8
Operation.MINUS.apply(5, 3);   // 2
Operation.TIMES.apply(5, 3);   // 15
```

```
Each enum constant = own implementation
Polymorphism within Enum
= Strategy pattern in Enum
```

---

## 8️⃣ Singleton via Enum (Joshua Bloch's Recommendation)

```java
public enum DatabaseConnection {
    INSTANCE;
    
    public void query(String sql) {
        // ...
    }
}

// Usage:
DatabaseConnection.INSTANCE.query("SELECT *");
```

```
WHY enum singleton?
   Thread-safe by default
   Serialization safe
   Reflection-attack safe
   Concise (one line)
   
   = "Best way to implement Singleton" — Joshua Bloch
```

---

## 9️⃣ Visual Comparison

```
┌──────────────────┬────────────────┬─────────────────┐
│  Aspect          │  Constants     │  Enum           │
├──────────────────┼────────────────┼─────────────────┤
│ Type safety      │             │              │
│ Limited values   │             │              │
│ Switch support   │ ints only   │ proper        │
│ Built-in methods │             │              │
│ Custom fields    │             │              │
│ Polymorphism     │             │              │
│ Singleton-safe   │ Manual         │ built-in     │
└──────────────────┴────────────────┴─────────────────┘
```

---

## Common Use Cases

```
Days of week
Months
Status states (PENDING, APPROVED, REJECTED)
HTTP methods (GET, POST, PUT, DELETE)
User roles (ADMIN, USER, GUEST)
Card suits (HEARTS, SPADES, ...)
Singleton implementation
Strategy pattern (operation enum example)
```

---

## Memory Hooks

```
Enum = Traffic light (fixed set, no others)

Without Enum: int constants → unsafe
With Enum   : type-safe, readable, powerful

Built-ins:
   values()    → all constants array
   valueOf()   → string → enum
   name()      → string name
   ordinal()   → position

Power:
   • Custom fields + constructor
   • Abstract methods (polymorphism)
   • Singleton (Bloch's favorite)
```

[← Back to JAVA](../)
