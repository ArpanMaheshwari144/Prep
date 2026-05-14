package com.arpan.bank.model;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    CurrentAccount = Business/Company ka account
//    0% interest (business rule — current accounts mein interest nahi)
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL POSITION:
//    Account (abstract — parent)
//       │
//       │ extends
//       ▼
//    CurrentAccount  ← TU YAHAN (0% interest)
//
// CONCEPTS IN THIS FILE:
//    1. INHERITANCE       → extends Account
//    2. SUPER CONSTRUCTOR → super(id, name, balance)
//    3. POLYMORPHISM      → @Override returns 0
//
// DIFFERENCE FROM SavingsAccount:
//    SavingsAccount     CurrentAccount
//       4% interest        0% interest
//       Same pattern, different value
//       = Polymorphism in action
//
// REAL WORLD WHY 0%?
//    Current accounts = business owners use
//    Daily transactions (deposits/withdrawals)
//    Bank doesn't pay interest — they get usage of money
//    = Business rule
//
// 🎨 PATTERN: Template Method (concrete implementation)
// 📐 SOLID — LSP: CurrentAccount fully substitutable for Account
// ═══════════════════════════════════════════════════════════════════════

public class CurrentAccount extends Account {

    public CurrentAccount(String id, String name, double balance) {
        super(id, name, balance);
    }

    @Override
    public double calculateInterest() {
        return 0;    // Current account = no interest (business rule)
    }
}
