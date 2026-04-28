package com.arpan.bank.model;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    SavingsAccount = Account ka concrete child. Bank ka savings account.
//
// 📌 JAVA KYA FOLLOW + KYU + KAISE:
//    • OOPS — Inheritance (extends Account → parent ke fields/methods mile)
//             Polymorphism (calculateInterest() override — apna 4% logic)
//    • super() call — parent constructor invoke
// ═══════════════════════════════════════════════════════════════════════

public class SavingsAccount extends Account {

    private static final double INTEREST_RATE = 0.04;   // 4% — class-level constant

    public SavingsAccount(String id, String name, double balance) {
        super(id, name, balance);    // parent constructor call
    }

    @Override
    public double calculateInterest() {
        return balance * INTEREST_RATE;   // SavingsAccount ka apna logic
    }
}
