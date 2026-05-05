package com.arpan.bank.config;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    Bank-wide config (bankName, maxTransactionLimit, currency). Ek hi
//    instance pure app mein — har jagah BankConfig.INSTANCE se access.
// ═══════════════════════════════════════════════════════════════════════
//
// ╔═══════════════════════════════════════════════════════════════════╗
// ║  🎨 DESIGN PATTERN: SINGLETON (Enum-based)                         ║
// ╠═══════════════════════════════════════════════════════════════════╣
// ║                                                                   ║
// ║  Yahaan Singleton pattern use kiya — ENUM approach.               ║
// ║                                                                   ║
// ║  WHY enum singleton?                                              ║
// ║  • Thread-safe automatic (JVM enum init guarantee)                ║
// ║  • Serialization-safe (no broken singletons)                      ║
// ║  • Reflection-safe (can't instantiate via reflection)             ║
// ║  • Lazy init built-in                                             ║
// ║                                                                   ║
// ║  Joshua Bloch (Effective Java) ka recommended approach —          ║
// ║  manual `synchronized + getInstance()` se simpler aur safer.      ║
// ║                                                                   ║
// ║  Usage: BankConfig.INSTANCE.getBankName();                        ║
// ║                                                                   ║
// ║  📐 SOLID — SRP (Single Responsibility):                          ║
// ║  Yeh class sirf configuration provide karti — koi business        ║
// ║  logic NAHI.                                                      ║
// ║                                                                   ║
// ║  🎤 INTERVIEW LINE:                                                ║
// ║  "BankConfig ko enum singleton banaya — Joshua Bloch's            ║
// ║   recommended approach. Thread-safe + serialization-safe +        ║
// ║   reflection-safe automatic. Manual synchronized getInstance()    ║
// ║   se simpler aur secure."                                         ║
// ║                                                                   ║
// ╚═══════════════════════════════════════════════════════════════════╝

public enum BankConfig {
    INSTANCE;   // SINGLE instance, JVM ne guarantee diya

    // Bank-wide config (real world: file/env se load)
    private final String bankName = "Arpan Bank";
    private final double maxTransactionLimit = 100000.0;
    private final String currency = "INR";

    public String getBankName() { return bankName; }
    public double getMaxTransactionLimit() { return maxTransactionLimit; }
    public String getCurrency() { return currency; }
}
