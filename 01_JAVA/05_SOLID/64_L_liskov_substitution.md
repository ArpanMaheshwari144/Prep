# L = Liskov Substitution Principle (LSP)

> **V90 — SOLID: Topic 64**

---

## Definition

> **Child class ko parent ki jagah use kar pao — bina kuch break.**

Matlab: agar `Account` use ho raha hai, **`SavingsAccount` daalna safe** hona chahiye. Code crash nahi hona chahiye.

---

## STORY — FixedDeposit ki Galti

→ `Account` class banayi — har account ka `withdraw()` method
→ Saare account types extend kiye:
   - `SavingsAccount` — withdraw normal kaam
   - `CurrentAccount` — withdraw normal kaam
   - `FixedDeposit` — **kaam:** "FD mein withdraw nahi hota maturity se pehle"
   - Toh FixedDeposit mein **`withdraw()` ne exception throw kar diya**

```java
class Account {
    void withdraw(double amount) { /* normal logic */ }
}

class FixedDeposit extends Account {
    @Override
    void withdraw(double amount) {
        throw new RuntimeException("FD se withdraw nahi hota!");  // LSP violation
    }
}
```

→ **NotificationService** sab accounts pe `withdraw()` chala raha tha
→ Jab FD aaya — **CRASH **
→ **Caller ne `Account` expect kiya — Account ki guarantee thi `withdraw()` chalega**
→ FD ne us guarantee tod di — **LSP violation**
→ **Fix:** Hierarchy galat hai — FD ko Account ka child nahi banana chahiye

---

## Fix — Hierarchy Sahi Karo

```java
interface Withdrawable {
    void withdraw(double amount);
}

class SavingsAccount implements Withdrawable { /* withdraw kar sakta */ }
class CurrentAccount implements Withdrawable { /* withdraw kar sakta */ }

// FixedDeposit Withdrawable NAHI — alag class
class FixedDeposit {
    /* withdraw nahi hota */
    void closeAtMaturity() { ... }
}
```

→ Ab `Withdrawable` interface se kaam karne wala code **safe** — koi crash nahi
→ FD apna alag interface follow karta

---

## LSP Visualization

```
              BAD — Child breaks parent contract

  Account (parent)
    withdraw() — promise: chalega
        │
        ▼ extends
  ┌─────────────┬──────────────┐
  ▼             ▼              ▼
Savings    Current       FixedDeposit
withdraw   withdraw      withdraw → throw exception
kaam    kaam        (parent ka contract toot gaya!)


  Code:  for (Account a : accounts) a.withdraw(100);
                                            │
                                            ▼
                                       FD pe crash


              GOOD — sahi hierarchy

  Withdrawable (interface)
    withdraw()
        │
        ▼ implements
  ┌─────────┬──────────┐
  ▼         ▼
Savings    Current
safe    safe

  FixedDeposit — alag class, alag contract
                 (Withdrawable implement nahi karta)
```

---

## LSP Trap — Common Violation

> **Child class mein parent ke method ka behavior weaken nahi karna chahiye.**

Examples:
1. Child throws unexpected exception (jaisa upar)
2. Child accepts kam types of input (parent kuch bhi le, child sirf positive)
3. Child returns less general type (parent Map return, child sirf TreeMap)

**Fix:** Hierarchy redesign — agar child parent ki jagah safely nahi use ho sakti, **wo child hai hi nahi** us parent ki.

---

## POWER PHRASE

> *"Liskov Substitution means a child class must be substitutable for its parent without breaking anything. If FixedDeposit throws an exception on `withdraw()` which Account promises to support, that's a design violation — fix the hierarchy, not the caller."*
