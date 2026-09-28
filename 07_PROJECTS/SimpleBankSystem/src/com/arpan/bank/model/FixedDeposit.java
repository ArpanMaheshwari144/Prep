package com.arpan.bank.model;

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    FixedDeposit = Locked-in deposit account
//    7% interest (highest among 3 types)
//    Money locked for fixed period
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL POSITION:
//    Account (abstract — parent)
//       │
//       │ extends
//       ▼
//    FixedDeposit  ← TU YAHAN (7% — highest interest)
//
// 3 ACCOUNT TYPES COMPARE:
//    ┌────────────────┬──────────────┬─────────────────┐
//    │  Account Type  │  Interest    │  Use Case        │
//    ├────────────────┼──────────────┼─────────────────┤
//    │ Savings        │  4%          │  Daily use       │
//    │ Current        │  0%          │  Business        │
//    │ FixedDeposit   │  7%          │  Locked savings  │
//    └────────────────┴──────────────┴─────────────────┘
//
// POLYMORPHISM IN ACTION:
//    Account[] accounts = {
//        new SavingsAccount(...),
//        new CurrentAccount(...),
//        new FixedDeposit(...)
//    };
//
//    for (Account a : accounts) {
//        a.calculateInterest();
//        // SAME method call, DIFFERENT behavior:
//        //    Savings → 4%
//        //    Current → 0%
//        //    FD      → 7%
//    }
//    = Runtime polymorphism
//
// PATTERN: Template Method (concrete implementation)
// SOLID — LSP: FixedDeposit fully substitutable for Account
// ═══════════════════════════════════════════════════════════════════════

public class FixedDeposit extends Account {

    private static final double INTEREST_RATE = 0.07;   // 7% — higher rate

    public FixedDeposit(String id, String name, double balance) {
        super(id, name, balance);
    }

    @Override
    public double calculateInterest() {
        return balance * INTEREST_RATE;
    }
}
