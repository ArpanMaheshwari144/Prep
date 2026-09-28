package com.arpan.bank.config;

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    Bank-wide config (bankName, maxTransactionLimit, currency).
//    Ek hi instance pure app mein — BankConfig.INSTANCE se access.
// ═══════════════════════════════════════════════════════════════════════
//
// PATTERN: SINGLETON (Enum-based)
//
// VISUAL MAP:
//    ┌─────────────────────────────────┐
//    │  BankConfig (ENUM Singleton)     │
//    ├─────────────────────────────────┤
//    │  INSTANCE  ← only ONE            │
//    │                                   │
//    │  - bankName = "Arpan Bank"       │
//    │  - maxTransactionLimit = 100000  │
//    │  - currency = "INR"              │
//    └─────────────────────────────────┘
//
// THEORY ↔ CODE:
//    Singleton needs:        →    Yahan code mein:
//       Only ONE instance    →    enum INSTANCE
//       Private constructor  →    enum constructor private (auto)
//       Thread-safe          →    JVM guarantee (enum init)
//       Reflection-safe      →    enum can't be reflected
//
// USAGE FLOW:
//    Main.java                AccountService.java
//       │                          │
//       │  BankConfig.INSTANCE.getBankName()
//       ▼                          ▼
//    ┌──────────────────────────────┐
//    │   BankConfig.INSTANCE         │
//    │   (SAME object both jagah)    │
//    └──────────────────────────────┘
//
// SOLID — SRP:
//    Only configuration provide karta. No business logic.
//
// INTERVIEW LINE:
//    "Enum singleton — Joshua Bloch's recommended approach.
//     Thread-safe + serialization-safe + reflection-safe automatic.
//     Simpler than manual synchronized getInstance()."
// ═══════════════════════════════════════════════════════════════════════

public enum BankConfig {
    INSTANCE;   // SINGLE instance, JVM guarantee

    // Bank-wide config (real world: file/env se load)
    private final String bankName = "Arpan Bank";
    private final double maxTransactionLimit = 100000.0;
    private final String currency = "INR";

    public String getBankName() { return bankName; }
    public double getMaxTransactionLimit() { return maxTransactionLimit; }
    public String getCurrency() { return currency; }
}
