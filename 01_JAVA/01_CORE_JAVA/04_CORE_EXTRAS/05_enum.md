# Enum — Fixed Set of Constants

> **V90 — Core Java Extras: Topic 21**

---

## STORY — Payment System

→ Tune payment system banaya. `PaymentType` int mein — `1=CASH, 2=CARD, 3=UPI`
→ Koi developer ne `99` pass kar diya — **invalid value, compiler nahi roka**
→ **Runtime crash**
→ **Enum banao** — sirf allowed values. **Bahar kuch diya toh compile error**
→ Switch mein use karo — cleaner, type-safe
→ `PaymentType = 1 (CASH), 2 (CARD), 3 (UPI)` → koi bhi `99` de sakta hai, **compiler nahi rokta**
→ **Enum:** sirf allowed values define karo. Bahar kuch diya → compile error
→ **Type safety**

---

## Code

### Basic Enum
```java
enum PaymentType { CASH, CARD, UPI }

PaymentType p = PaymentType.CASH;     // valid
PaymentType p = PaymentType.EMI;      // compile error — defined nahi
```

### Enum with fields + constructor
```java
enum Day {
    MON("Weekday"), SAT("Weekend"), SUN("Weekend");

    private String type;
    Day(String type) { this.type = type; }   // private by default
    public String getType() { return type; }
}

Day.MON.getType();    // "Weekday"
Day.SAT.getType();    // "Weekend"
```

### Switch mein use
```java
switch(p) {
    case CASH: System.out.println("Cash liya"); break;
    case CARD: System.out.println("Card swipe"); break;
    case UPI:  System.out.println("UPI scan"); break;
}
```

### Built-in methods
```java
Day.MON.name();        // "MON"  — string naam
Day.MON.ordinal();     // 0       — index (0 se start)
Day.valueOf("SAT");    // Day.SAT — string se enum
Day.values();          // [MON, SAT, SUN] — sab values array
```

---

## TRAP 1

> **Enum constructor `public` nahi ho sakta — `private` ya package-private only.**

## TRAP 2

> **`ordinal()` pe DB mein depend mat karo — order change kiya toh sab values shift!**

## TRAP 3

> **Enum `==` se compare karo, `.equals()` nahi — enum values singletons hain, `==` safe.**

## TRAP 4 — Singleton wala best way

> **Enum se Singleton banao — JVM guarantee deta hai ek hi instance.**
> ```java
> enum AppConfig { INSTANCE; }
> ```
> BEST Singleton implementation.

---

## POWER PHRASE

> *"Enum defines a fixed set of named constants with full type safety — the compiler rejects any value not in the enum. Each enum constant is a singleton object so `==` comparison is safe and preferred over `.equals()`."*

> **Yaad rakh:** Enum = type-safe constants. `==` se compare. `ordinal()` pe depend mat karo. Singleton ka best way bhi Enum hai.
