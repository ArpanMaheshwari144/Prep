package com.arpan.bank.model;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    CurrentAccount = business/company ka account. Interest 0% (business rule).
//
// 📌 JAVA KYA FOLLOW + KYU + KAISE:
//    • OOPS — Inheritance (extends Account)
//             Polymorphism (calculateInterest() override — return 0)
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
