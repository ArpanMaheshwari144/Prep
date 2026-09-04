# Method Overloading vs Overriding

> **V90 Section 1 — Topic 8**

---

## WHY — Dono Alag Problems

| | Solve karta |
|--|-------------|
| **Overloading** | Same kaam, **alag inputs** (sendNotification — sirf message? message + phone? message + email + priority?) |
| **Overriding** | Parent ka generic logic, **child apna special** chahiye (Manager apna calculateBonus, Employee se alag) |

---

## STORY

### Overloading wala
→ NotificationService mein sawaal aaya — `sendNotification` ek rakhun ya alag-alag?
→ Ek jagah sirf **message**, ek jagah **message + phone**, ek jagah **message + email + priority**
→ **Same naam, alag parameters** — Overloading, **compile time** pe decide hota hai

### Overriding wala
→ Aur jab Manager ne apna alag `calculateBonus()` likha **parent ke upar se**
→ **Overriding, runtime** pe decide
→ Dono alag problems ke alag solutions

---

## Visualization

```
OVERLOADING (Compile Time)              OVERRIDING (Runtime)
┌─────────────────────────┐             ┌──────────────────────────┐
│   NotificationService   │             │  Employee (Parent)       │
│                         │             │  calculateBonus(){       │
│  send(String msg)       │             │    return 10%            │
│  send(String, String)   │             │  }                       │
│  send(String, String,   │             └────────────┬─────────────┘
│       int)              │                          │ extends
│                         │             ┌────────────▼─────────────┐
│  SAME class, SAME naam  │             │  Manager (Child)         │
│  ALAG parameters        │             │  calculateBonus(){       │
└─────────────────────────┘             │    return 25%            │
                                        │  }                       │
                                        │  CHILD class REDEFINE    │
                                        │  parent method           │
                                        └──────────────────────────┘
```

---

## Comparison

| | Overloading | Overriding |
|--|-------------|-----------|
| **Class** | SAME class | Parent → Child |
| **Method name** | SAME | SAME |
| **Parameters** | **ALAG** | **SAME** |
| **When decided** | Compile time | Runtime |
| **Polymorphism type** | Static | Dynamic |
| **Annotation** | None | `@Override` |

---

## TRAP

> **`@Override` annotation HAMESHA lagao — typo pakdega compiler.**
> **Private/Static methods OVERRIDE nahi hote.**

---

## POWER PHRASES

> *"Overloading is compile-time polymorphism — same method name, different parameters in the same class."*

> *"Overriding is runtime polymorphism — child class redefines a parent method. Always use `@Override` so the compiler catches mistakes."*
