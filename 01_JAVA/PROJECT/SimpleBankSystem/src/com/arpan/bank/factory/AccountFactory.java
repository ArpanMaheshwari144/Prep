package com.arpan.bank.factory;

import com.arpan.bank.model.*;

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    AccountFactory = Object creation ka GATEWAY
//    Client bolta type ("savings"), factory decide karti class
// ═══════════════════════════════════════════════════════════════════════
//
// PATTERN: FACTORY
//
// VISUAL FLOW:
//    CLIENT (Main.java / Service)
//       │
//       │  "savings" type chahiye
//       ▼
//    ┌──────────────────────────────┐
//    │   AccountFactory.create()    │
//    │                                │
//    │   switch (type):               │
//    │     "savings" → SavingsAccount │
//    │     "current" → CurrentAccount │
//    │     "fd"      → FixedDeposit   │
//    └──────────┬───────────────────┘
//               │
//               ▼ returns Account interface
//    ┌─────────────────────┐
//    │   Account            │
//    │   (caller uses)      │
//    └─────────────────────┘
//
// WITHOUT vs WITH FACTORY:
//    WITHOUT (tight coupling):
//       Account a = new SavingsAccount(id, name, balance);
//                        ↑ caller HARDCODED to concrete class
//
//    WITH (loose coupling):
//       Account a = AccountFactory.create("savings", id, name, balance);
//                                              ↑ just TYPE string
//       Caller knows Account interface only
//
// CONCEPTS:
//    • static method — no instance needed
//    • Single point for object creation
//    • Returns abstract type, hides concrete
//
// SOLID:
//    OCP — Naya account type? Just add case in factory
//          Caller code UNCHANGED
//    DIP — Caller depends on Account abstract
//          Concrete subclasses HIDDEN inside factory
//
// INTERVIEW LINE:
//    "AccountFactory pattern — caller `new SavingsAccount()` directly
//     nahi karta, factory ke through. Naye types add karne mein
//     flexibility — sirf factory mein switch case add karna padta."
// ═══════════════════════════════════════════════════════════════════════

public class AccountFactory {

    // static method — object banane ke liye instance ki zarurat nahi
    public static Account create(String type, String id, String name, double balance) {
        switch (type.toLowerCase()) {
            case "savings":
                return new SavingsAccount(id, name, balance);
            case "current":
                return new CurrentAccount(id, name, balance);
            case "fd":
                return new FixedDeposit(id, name, balance);
            default:
                throw new IllegalArgumentException("Unknown account type: " + type);
        }
    }
}
