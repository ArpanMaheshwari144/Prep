package com.arpan.bank.repository;

import com.arpan.bank.model.Account;
import java.util.List;
import java.util.Optional;

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    AccountRepository = STORAGE ka CONTRACT (interface)
//    Save/find/delete ke rules define karta
//    Actual storage class (HashMap/DB/Mongo) handle karegi
// ═══════════════════════════════════════════════════════════════════════
//
// PATTERN: REPOSITORY
//
// VISUAL:
//    AccountService (caller)
//         │
//         │ depends on
//         ▼
//    ┌──────────────────────────┐
//    │  AccountRepository        │  ← INTERFACE (this file)
//    │  (contract only)          │
//    │                            │
//    │  save()                    │
//    │  findById()                │
//    │  findAll()                 │
//    │  delete()                  │
//    └──────────┬───────────────┘
//               │ implements
//        ┌──────┼──────┬──────────┐
//        ▼      ▼      ▼          ▼
//    InMemory  MySQL  Mongo  PostgreSQL
//    (current) (later) (later) (later)
//
// WHY INTERFACE (BIG WIN)?
//    Tomorrow DB change: InMemory → MySQL
//    Without interface: AccountService REWRITE
//    With interface: Just write MySQLAccountRepository, wire up
//                    AccountService UNCHANGED
//
// Optional<Account> WHY?
//    findById returns Optional<Account>
//    Without Optional:  Account a = findById("X");  a.deposit(500);  ← NPE risk
//    With Optional:     Optional<Account> a = findById("X");
//                       a.ifPresent(acc -> acc.deposit(500));
//                       = Null-safe by design
//
// SOLID:
//    ISP — Focused interface (account data ops only, not 20 random methods)
//    DIP — Service depends on INTERFACE, not concrete
//          Implementation swap without touching service
//
// REAL WORLD:
//    Spring Data JPA's JpaRepository = SAME pattern
//    Framework-level Repository abstraction
//
// INTERVIEW LINE:
//    "Repository pattern data access abstract karta —
//     interface + implementation. Service interface se baat karta,
//     future mein MySQL/Mongo switch kare = service code unchanged.
//     Spring Data JPA same pattern follow karta."
// ═══════════════════════════════════════════════════════════════════════

public interface AccountRepository {

    void save(Account account);
    Optional<Account> findById(String accountId);    // Optional — null safe
    List<Account> findAll();
    void delete(String accountId);
}
