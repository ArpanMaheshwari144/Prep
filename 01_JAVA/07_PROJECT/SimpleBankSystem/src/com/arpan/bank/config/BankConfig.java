package com.arpan.bank.config;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    Bank-wide config (bankName, maxTransactionLimit, currency). Ek hi
//    instance pure app mein — har jagah BankConfig.INSTANCE se access.
//
// 📌 JAVA KYA FOLLOW + KYU + KAISE:
//    • Design Pattern — SINGLETON (best way: enum)
//    • Enum singleton = JVM guarantee (1 instance, thread-safe,
//      reflection-safe, serialization-safe)
//    • Use: BankConfig.INSTANCE.getBankName() — har jagah same object
// ═══════════════════════════════════════════════════════════════════════

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
