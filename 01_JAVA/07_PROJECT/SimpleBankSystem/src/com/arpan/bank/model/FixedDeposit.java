package com.arpan.bank.model;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    FixedDeposit = locked-in deposit account. Highest interest (7%).
//
// 📌 JAVA KYA FOLLOW + KYU + KAISE:
//    • OOPS — Inheritance (extends Account)
//             Polymorphism (calculateInterest() override — 7% rate)
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
