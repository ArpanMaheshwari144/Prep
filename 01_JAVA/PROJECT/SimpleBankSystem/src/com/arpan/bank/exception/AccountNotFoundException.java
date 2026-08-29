package com.arpan.bank.exception;

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    AccountNotFoundException = CUSTOM exception
//    Agar accountId se account nahi mila → throw hoti
// ═══════════════════════════════════════════════════════════════════════
//
// CONCEPTS:
//    1. CUSTOM EXCEPTION
//       Domain-specific exception
//       Generic RuntimeException se behtar
//
//    2. extends RuntimeException
//       = UNCHECKED exception
//       Compiler force NAHI karta handle
//       Caller chahe handle, chahe na
//
//    3. Constructor calling super()
//       Parent (RuntimeException) ka message slot fill
//       Stack trace mein descriptive message dikhega
//
// USED VIA Optional.orElseThrow():
//    repository.findById(id)
//        .orElseThrow(() -> new AccountNotFoundException(id));
//
// WHY CUSTOM EXCEPTION (vs Generic RuntimeException)?
//
//    Generic throw:
//       throw new RuntimeException("Account not found");
//       Catch: catch (RuntimeException e) { ... }
//              ↑ catches EVERYTHING — hard to distinguish
//
//    Custom throw:
//       throw new AccountNotFoundException(id);
//       Catch: catch (AccountNotFoundException e) { ... }
//              ↑ SPECIFIC catch — clean handling
//
//    = Self-documenting in stack trace
//    = Naam se hi pata "yeh exception kya hai"
//
// DIFFERENT FROM InsufficientFundsException:
//    AccountNotFoundException  → Account NAHI exist (data missing)
//    InsufficientFundsException → Account EXIST but no money
//    = Caller handle alag-alag
// ═══════════════════════════════════════════════════════════════════════

public class AccountNotFoundException extends RuntimeException {

    public AccountNotFoundException(String accountId) {
        super("Account not found: " + accountId);   // descriptive message
    }
}
