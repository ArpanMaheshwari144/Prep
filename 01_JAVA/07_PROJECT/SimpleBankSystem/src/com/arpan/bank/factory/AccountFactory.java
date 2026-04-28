package com.arpan.bank.factory;

import com.arpan.bank.model.*;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    Object creation centralize — client sirf "type" bole (e.g., "savings")
//    aur factory specific class return karti. Pizza counter analogy.
//
// 📌 JAVA KYA FOLLOW + KYU + KAISE:
//    • Design Pattern — FACTORY METHOD
//    • SOLID — OCP (naya account type? sirf factory mein case add)
//             DIP (caller concrete classes pe nahi, factory pe depend)
//    • Polymorphism — return type Account (parent), actual subtype variable
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
