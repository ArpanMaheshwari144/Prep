package com.arpan.bank.model;

import com.arpan.bank.exception.InsufficientFundsException;

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    Account = saare account types ka PARENT (abstract).
//    Direct object nahi banta — Savings/Current/FixedDeposit extend karte.
// ═══════════════════════════════════════════════════════════════════════
//
// PATTERN: TEMPLATE METHOD (via abstract class)
//
// VISUAL CLASS HIERARCHY:
//    ┌────────────────────────────────────┐
//    │  Account (abstract)                 │
//    ├────────────────────────────────────┤
//    │  - accountId, holderName, balance   │
//    │                                      │
//    │  + deposit()           ← CONCRETE   │
//    │  + withdraw()          ← CONCRETE   │
//    │  + calculateInterest() ← ABSTRACT   │
//    └──────────┬─────────────────────────┘
//               │ extends
//      ┌────────┼────────┐
//      ▼        ▼        ▼
//   Savings  Current  FixedDeposit
//   (4%)     (0%)     (7%)
//
// CONCEPTS IN THIS FILE:
//    1. ABSTRACTION    →  abstract class (direct new nahi)
//    2. ENCAPSULATION  →  private fields, controlled getters
//    3. INHERITANCE    →  children extend Account
//    4. POLYMORPHISM   →  calculateInterest har child alag
//    5. SYNCHRONIZED   →  thread-safe deposit/withdraw
//    6. CUSTOM EXCEPTION → InsufficientFundsException
//
// TEMPLATE METHOD PATTERN:
//    Common steps (deposit/withdraw) — parent mein CONCRETE
//    Specific step (calculateInterest) — child mein ABSTRACT
//    = "Skeleton + Customizable parts"
//
// SOLID:
//    OCP — Naya account type chahiye? Just extend Account
//          (parent modify NAHI karna)
//    LSP — Account a = new SavingsAccount();
//          Account a = new FixedDeposit();
//          Sab safely substitute karte
//
// ACCESS MODIFIERS — Code Mein:
//    private final accountId   → identity, no change ever
//    private final holderName  → identity, no change ever
//    protected double balance  → CHILD ko access chahiye
//                                (interest calc karne ke liye)
//
//    Why protected NOT private?
//       SavingsAccount: return balance * INTEREST_RATE;
//                              ↑ parent ka protected field
//       Agar private hota → child access nahi kar pata
//
// GETTERS ONLY, NO SETTERS:
//    Immutable identity:
//       accountId — account banta TAB assign, ZINDAGI BHAR fixed
//       holderName — similarly fixed
//
//    balance:
//       Setter hota = anyone "balance = 999999" kar sakta
//       Instead: deposit() / withdraw() — VALIDATED change
//       = Business rules enforced (encapsulation power)
//
// IllegalArgumentException (deposit method):
//    RuntimeException (unchecked) — "method ko galat input diya"
//    Use when: invalid argument (negative amount, null)
//    Different from:
//       • InsufficientFundsException → business rule violation
//       • NullPointerException       → programmer error
//
// SYNCHRONIZED — Mutex Lock (DEEP):
//
//    RACE CONDITION pehle (without sync):
//       Thread A:                Thread B:
//       read balance = 100       read balance = 100
//       100 + 50 = 150           100 + 30 = 130
//       write balance = 150      write balance = 130
//       Final = 130 (NOT 180!)
//       = Thread A update LOST
//
//    WITH synchronized:
//       Thread A enters → LOCK
//                          update karta
//                          release
//       Thread B waits  → acquire
//                          update karta
//                          release
//       = MUTUAL EXCLUSION (mutex)
//       = Atomicity + Visibility BOTH solved
//
//    SYNCHRONIZED vs VOLATILE:
//       synchronized:
//          Atomicity (multi-step undivided)
//          Visibility
//          Use: multi-step state change (balance update)
//
//       volatile:
//          NO atomicity
//          Visibility only
//          Use: single boolean flag (isRunning)
//          volatile count++; → STILL race! (count++ = 3 ops)
//
//    Why synchronized HERE (not volatile)?
//       deposit() has MULTIPLE steps:
//          1. validate amount
//          2. read balance
//          3. balance + amount
//          4. write balance
//       = Multi-step = synchronized NEEDED
//
// INTERVIEW LINE:
//    "Account abstract class — Template Method pattern.
//     deposit()/withdraw() concrete (saare accounts same),
//     calculateInterest() abstract (har subclass apna formula).
//     OCP + LSP follow karta."
// ═══════════════════════════════════════════════════════════════════════

public abstract class Account {

    // Encapsulation — fields private
    private final String accountId;
    private final String holderName;
    protected double balance;          // protected — child use kare

    public Account(String accountId, String holderName, double balance) {
        this.accountId = accountId;
        this.holderName = holderName;
        this.balance = balance;
    }

    // Getters (no setters — controlled access)
    public String getAccountId() { return accountId; }
    public String getHolderName() { return holderName; }
    public double getBalance() { return balance; }

    // Common methods — sab accounts mein same kaam
    // synchronized — thread-safe (race condition se bachao)
    public synchronized void deposit(double amount) {
        if (amount <= 0) throw new IllegalArgumentException("Amount > 0 hona chahiye");
        balance += amount;
    }

    public synchronized void withdraw(double amount) {
        if (amount > balance) {
            throw new InsufficientFundsException(
                "Account " + accountId + ": balance " + balance + ", asked " + amount
            );
        }
        balance -= amount;
    }

    // Abstract — har account type apna logic dega (Polymorphism)
    public abstract double calculateInterest();
}
