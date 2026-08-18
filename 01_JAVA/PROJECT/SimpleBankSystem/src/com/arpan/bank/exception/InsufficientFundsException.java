package com.arpan.bank.exception;

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    InsufficientFundsException = CUSTOM exception
//    Withdraw amount > balance hua → ye throw hoti
// ═══════════════════════════════════════════════════════════════════════
//
// CONCEPTS:
//    1. CUSTOM EXCEPTION (same pattern as AccountNotFoundException)
//       Domain-specific
//
//    2. extends RuntimeException → UNCHECKED
//       No forced try-catch by compiler
//
//    3. Constructor with custom message
//       super(message) — parent stores it
//
// USED IN Account.withdraw():
//    public synchronized void withdraw(double amount) {
//        if (amount > balance) {
//            throw new InsufficientFundsException(
//                "Account " + id + ": balance " + balance + ", asked " + amount
//            );
//        }
//        balance -= amount;
//    }
//
// DIFFERENT FROM AccountNotFoundException:
//    AccountNotFoundException  → Account NAHI exist (data missing)
//    InsufficientFundsException → Account EXIST but no money
//
//    Caller handle differently:
//       catch (AccountNotFoundEx)    → "Invalid account ID"
//       catch (InsufficientFundsEx)  → "Add money to account"
//
// SELF-DOCUMENTING:
//    Stack trace mein:
//       InsufficientFundsException: Account S101: balance 100, asked 1000
//    = Naam + message = clear picture
//    = Generic RuntimeException se WAY better
// ═══════════════════════════════════════════════════════════════════════

public class InsufficientFundsException extends RuntimeException {

    public InsufficientFundsException(String message) {
        super(message);    // parent constructor — message store
    }
}
