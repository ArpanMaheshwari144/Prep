package com.arpan.bank.factory;

import com.arpan.bank.model.*;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    Object creation centralize — client sirf "type" bole (e.g., "savings")
//    aur factory specific class return karti. Pizza counter analogy.
// ═══════════════════════════════════════════════════════════════════════
//
// ╔═══════════════════════════════════════════════════════════════════╗
// ║  🎨 DESIGN PATTERN: FACTORY                                        ║
// ╠═══════════════════════════════════════════════════════════════════╣
// ║                                                                   ║
// ║  Object creation logic ENCAPSULATE karta — caller bolta           ║
// ║  "mujhe SAVINGS account chahiye", factory decide karta kaunsa     ║
// ║  class instantiate karna.                                         ║
// ║                                                                   ║
// ║  WITHOUT factory:                                                 ║
// ║    Account a = new SavingsAccount(...);   ← caller tightly coupled║
// ║                                                                   ║
// ║  WITH factory:                                                    ║
// ║    Account a = AccountFactory.create("SAVINGS", ...);             ║
// ║    ↑ caller knows interface only, factory handles concrete class  ║
// ║                                                                   ║
// ║  📐 SOLID — OCP (Open/Closed):                                    ║
// ║  Naya account type add karna ho? Factory mein switch case add,    ║
// ║  caller code unchanged. Open for extension, closed for mod.       ║
// ║                                                                   ║
// ║  📐 SOLID — DIP (Dependency Inversion):                           ║
// ║  Caller `Account` (abstract type) pe depend, concrete subclass    ║
// ║  factory ke andar hidden — caller decoupled from implementations. ║
// ║                                                                   ║
// ║  🎤 INTERVIEW LINE:                                                ║
// ║  "AccountFactory pattern implement karta — caller `new            ║
// ║   SavingsAccount()` directly nahi karta, factory ke through.      ║
// ║   Naye types add karne mein flexibility — sirf factory mein       ║
// ║   switch case add karna padta, caller code unchanged."            ║
// ║                                                                   ║
// ╚═══════════════════════════════════════════════════════════════════╝

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
