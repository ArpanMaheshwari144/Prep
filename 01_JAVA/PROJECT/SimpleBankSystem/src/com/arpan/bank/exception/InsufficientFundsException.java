package com.arpan.bank.exception;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    Custom Exception — withdraw amount > balance hua toh ye throw hoti.
//
// 📌 JAVA KYA FOLLOW + KYU + KAISE:
//    • Exception Handling — RuntimeException extend (UNCHECKED)
//    • Generic RuntimeException ki jagah specific = clean catch:
//        catch (InsufficientFundsException e) { ... }
//    • Self-documenting in stack trace (naam se hi pata)
// ═══════════════════════════════════════════════════════════════════════

public class InsufficientFundsException extends RuntimeException {

    public InsufficientFundsException(String message) {
        super(message);    // parent constructor — message store
    }
}
