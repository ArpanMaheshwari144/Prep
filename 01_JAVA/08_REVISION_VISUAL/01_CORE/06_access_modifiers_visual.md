# Access Modifiers — Visual Revision

---

## 1 Concept (House Door Analogy)

```
Imagine ek HOUSE with different rooms + doors:

   ┌──────────────────────────────────────┐
   │  Tera HOUSE                           │
   │                                        │
   │  ┌────────────────────────────────┐  │
   │  │  PUBLIC ROOM                    │  │
   │  │  Open to ALL — public hall      │  │
   │  │  Anyone can come                │  │
   │  └────────────────────────────────┘  │
   │                                        │
   │  ┌────────────────────────────────┐  │
   │  │  FAMILY ROOM (PROTECTED)        │  │
   │  │  Family + relatives             │  │
   │  └────────────────────────────────┘  │
   │                                        │
   │  ┌────────────────────────────────┐  │
   │  │  COLONY ROOM (DEFAULT/PACKAGE)  │  │
   │  │  Same colony residents only     │  │
   │  └────────────────────────────────┘  │
   │                                        │
   │  ┌────────────────────────────────┐  │
   │  │  BEDROOM (PRIVATE)              │  │
   │  │  Sirf TU                         │  │
   │  └────────────────────────────────┘  │
   └──────────────────────────────────────┘
```

---

## 2 4 Modifiers Quick Table

```
┌─────────────┬──────────────┬──────────────────────────┐
│ Modifier    │ Visibility   │ Real-life equivalent     │
├─────────────┼──────────────┼──────────────────────────┤
│ public      │ EVERYWHERE   │ Public hall               │
│ protected   │ Package +    │ Family + relatives        │
│             │ subclasses   │                           │
│ default     │ Same package │ Same colony               │
│ (no keyword)│              │                           │
│ private     │ Same class   │ Bedroom                   │
└─────────────┴──────────────┴──────────────────────────┘
```

---

## 3 Visual Scope Diagram

```
                    PUBLIC
   ┌─────────────────────────────────────────┐
   │                  PROTECTED               │
   │   ┌──────────────────────────────────┐  │
   │   │              DEFAULT              │  │
   │   │   ┌──────────────────────────┐   │  │
   │   │   │           PRIVATE         │   │  │
   │   │   │     ┌────────────┐        │   │  │
   │   │   │     │   class A   │        │   │  │
   │   │   │     │ (only here) │        │   │  │
   │   │   │     └────────────┘        │   │  │
   │   │   │ same package only        │   │  │
   │   │   └──────────────────────────┘   │  │
   │   │ same package + subclasses        │  │
   │   └──────────────────────────────────┘  │
   │ ANYWHERE in project                      │
   └─────────────────────────────────────────┘
```

---

## 4 Concrete Example

```java
package com.bank;

public class Account {
    public    String accountNumber;     // ANYONE can read
    protected double balance;            // subclasses + package
    String    branchCode;                // package only (default)
    private   String pin;                // class only

    public void displayPin() {
        System.out.println(pin);         // allowed (same class)
    }
}
```

```java
package com.bank;
class Branch {
    void test() {
        Account a = new Account();
        a.accountNumber;   // public
        a.balance;         // protected (same package)
        a.branchCode;      // default (same package)
        a.pin;             // private — NO ACCESS
    }
}
```

```java
package com.other;
class Outsider {
    void test() {
        Account a = new Account();
        a.accountNumber;   // public
        a.balance;         // protected (not same package, not subclass)
        a.branchCode;      // default (not same package)
        a.pin;             // private
    }
}
```

```java
package com.other;
class SavingsAccount extends Account {  // subclass
    void test() {
        accountNumber;     // public
        balance;           // protected (subclass)
        branchCode;        // default (different package)
        pin;               // private
    }
}
```

---

## 5 Visibility Cheatsheet

```
┌─────────────┬──────────┬──────────┬──────────┬──────────┐
│ Modifier    │  Class   │ Package  │ Subclass │ World    │
├─────────────┼──────────┼──────────┼──────────┼──────────┤
│ public      │       │       │       │       │
│ protected   │       │       │       │       │
│ default     │       │       │       │       │
│ private     │       │       │       │       │
└─────────────┴──────────┴──────────┴──────────┴──────────┘
```

---

## 6 Common Use Cases

```
PUBLIC:
   API endpoints (controllers)
   Constants meant for everyone
   Public method exposed for use

PROTECTED:
   Helper methods for subclasses
   Template Method pattern hooks
   Internal API for inheritance

DEFAULT (package-private):
   Same-package internal helpers
   Implementation details within module

PRIVATE:
   Internal data (encapsulation)
   Helper methods used only inside class
   Constants used only by class
```

---

## 7 Interview Trap

```
Q: protected vs default — kya farak?

A: protected = package + SUBCLASSES (even if different package)
   default   = ONLY same package, no subclass access if outside

Q: Class can be private?
A: Top-level class — NO (only public or default)
   Inner class — YES (can be private)

Q: Constructor can be private?
A: YES — used for Singleton pattern
   private constructor → no external "new"
```

---

## Memory Hooks

```
public     = hall (all welcome)
protected  = family room (parents + kids)
default    = colony (same package)
private    = bedroom (only me)
```

[← Back to JAVA](../)
