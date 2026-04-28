package com.arpan.bank.exception;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    Custom Exception — agar accountId se account nahi mila toh throw hoti.
//
// 📌 JAVA KYA FOLLOW + KYU + KAISE:
//    • Exception Handling — RuntimeException extend (UNCHECKED)
//    • Service.getAccount() Optional → orElseThrow() se ye throw hoti
//    • Clean error reporting — message mein specific accountId
// ═══════════════════════════════════════════════════════════════════════

public class AccountNotFoundException extends RuntimeException {

    public AccountNotFoundException(String accountId) {
        super("Account not found: " + accountId);   // descriptive message
    }
}
