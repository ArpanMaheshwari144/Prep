package com.arpan.bank.service;

import java.util.stream.Collectors;
import java.util.List;

import com.arpan.bank.exception.AccountNotFoundException;
import com.arpan.bank.model.Account;
import com.arpan.bank.observer.EventPublisher;
import com.arpan.bank.observer.TransactionEvent;
import com.arpan.bank.repository.AccountRepository;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    Business logic ka MIDDLE layer
//    • Controller niche bulayega
//    • Repository + Publisher ko coordinate kare
//    • Sirf business RULES — data access NAHI
// ═══════════════════════════════════════════════════════════════════════
//
// LAYERED ARCHITECTURE:
//    Main / Controller (caller)
//         │
//         ▼
//    ┌────────────────────────────┐
//    │  AccountService             │  ← TU YAHAN
//    │  (business logic)           │
//    └────┬──────────────┬────────┘
//         │              │
//         ▼              ▼
//    AccountRepository  EventPublisher
//    (data access)      (notifications)
//
// METHODS IN THIS FILE:
//    1. createAccount(Account)
//       Duplicate check + save via repository
//
//    2. getAccount(String id)
//       Fetch + throw AccountNotFoundException if missing
//
//    3. transfer(fromId, toId, amount)
//       Validation + Manual rollback if fail + Fire event
//
//    4. getAccountHolderNames()
//       Streams API demo
//
// CONSTRUCTOR INJECTION (Big Win):
//    private final AccountRepository repository;   // INTERFACE
//    private final EventPublisher eventPublisher;
//
//    public AccountService(AccountRepository repo,
//                          EventPublisher publisher) { ... }
//
//    Benefits:
//       ✅ final fields (immutable)
//       ✅ NPE impossible (must pass at construction)
//       ✅ Easy testing (mock pass karke)
//       ✅ Spring @Autowired constructor = same pattern
//
// TRANSFER — MANUAL ROLLBACK PATTERN (DEEP):
//
//    Without rollback:
//       Step 1: from.withdraw(500)  ✅ success
//       Step 2: to.deposit(500)     ❌ fail (DB crash)
//
//       Result: from -500, to nothing → ₹500 VANISH
//       = DATA INCONSISTENT
//
//    With manual rollback:
//       try {
//           from.withdraw(amount);    // step 1
//           to.deposit(amount);       // step 2
//       } catch (RuntimeException e) {
//           from.deposit(amount);     // UNDO step 1
//           throw e;                   // caller knows
//       }
//       = CONSISTENT (atomicity preserved)
//
//    SAME GOAL as Spring @Transactional:
//       @Transactional
//       public void transfer(...) {
//           from.withdraw(amount);
//           to.deposit(amount);
//           // No catch, no manual rollback
//           // Exception aayi? Spring AUTO ROLLBACK
//       }
//
//    Behind the scenes (Spring AOP):
//       BEGIN TRANSACTION (conn.setAutoCommit(false))
//          method execute
//       Success? COMMIT (write all)
//       Fail?    ROLLBACK (undo all)
//
//    Same JDBC under the hood:
//       BEGIN TRANSACTION
//       UPDATE accounts SET balance = balance - 500 WHERE id=1;
//       UPDATE accounts SET balance = balance + 500 WHERE id=2;
//       COMMIT;  -- or ROLLBACK on error
//
//    Manual vs Spring:
//       Manual (this code) = try/catch/undo, verbose, easy to forget
//       Spring @Transactional = annotation, AOP proxy handles all
//       = SAME goal: ACID atomicity
//
// STREAMS API DEMO (getAccountHolderNames):
//    findAll()                          // SOURCE
//      .stream()                        // pipeline shuru
//      .map(Account::getHolderName)     // INTERMEDIATE (LAZY)
//      .collect(Collectors.toList())    // TERMINAL (EAGER — triggers!)
//
// 🔑 LAZY vs EAGER (Streams):
//    INTERMEDIATE ops — LAZY (do nothing immediately):
//       filter(), map(), sorted(), distinct(), limit()
//       = Pipeline build hota, EXECUTE nahi hota
//
//    TERMINAL ops — EAGER (trigger entire pipeline):
//       collect(), forEach(), count(), reduce(),
//       findFirst(), anyMatch(), min(), max(), toList()
//       = Pipeline EXECUTE — saare intermediate ops run
//
//    Example:
//       Stream<Integer> pipeline = list.stream()
//           .filter(n -> { sout("Filter"); return n%2==0; })
//           .map(n -> { sout("Map"); return n*10; });
//       // NOTHING PRINTED YET — pipeline lazy
//
//       pipeline.toList();   // ★ NOW prints!
//
//    Why Lazy?
//       1. EFFICIENCY — filter().limit(5) early stop possible
//       2. SHORT-CIRCUIT — findFirst() stops on first match
//
//    Common trap:
//       list.stream().map(x -> sout(x));   // ❌ map NEVER runs
//                                            // No terminal!
//
//    In our code:
//       .map(Account::getHolderName)   ← LAZY (defers)
//       .collect(toList())              ← ★ EAGER (triggers map for each)
//
// 🔑 METHOD REFERENCE (Account::getHolderName):
//    LAMBDA (longer):     .map(account -> account.getHolderName())
//    METHOD REF (short):  .map(Account::getHolderName)
//    = SAME thing, less code
//
//    Account::getHolderName
//       │       │
//       │       method name
//       class name
//
//    4 types of method references:
//       1. Static:           Integer::parseInt
//       2. Instance on class: Account::getHolderName  ← YAHAN
//       3. Instance on obj:  System.out::println
//       4. Constructor:      ArrayList::new
//
// 🔑 LAMBDA SUPPLIER in orElseThrow:
//    .orElseThrow(() -> new AccountNotFoundException(accountId));
//                  ↑
//                  This lambda = Supplier<AccountNotFoundException>
//
//    Supplier<T> functional interface:
//       public interface Supplier<T> {
//           T get();    // no input, returns T
//       }
//
//    Why Lambda (not direct exception)?
//       ❌ .orElseThrow(new Ex(id));   ← EAGER — banti hai ABHI
//       ✅ .orElseThrow(() -> new Ex(id));  ← LAZY — banti hai when needed
//
//    = Resource saving (object create only on need)
//
// OPTIONAL USAGE:
//    repository.findById(id)
//        .orElseThrow(() -> new AccountNotFoundException(id));
//    = Null-safe by design
//
// 🔑 Optional.isPresent() in createAccount:
//    if (repository.findById(id).isPresent()) {
//        throw new IllegalArgumentException("Account already exists");
//    }
//    repository.save(account);
//
//    Optional methods cheat:
//       .isPresent()    → boolean (value hai?)
//       .isEmpty()      → boolean (Java 11+)
//       .get()          → T (USE only after isPresent check)
//       .orElse(x)      → T or default
//       .orElseGet(sup) → T or lazy default
//       .orElseThrow()  → T or throw
//
//    Why isPresent() here?
//       createAccount logic: "Already hai → ERROR"
//       = REVERSE check (existence = bad)
//       = orElse awkward here, isPresent perfect
//
// 📐 SOLID:
//    SRP — Only business logic
//          Data access = repository's job
//          Notifications = publisher's job
//
//    DIP — Depends on AccountRepository INTERFACE
//          InMemoryAccountRepository pluggable
//
// 🎤 INTERVIEW LINE:
//    "AccountService SRP follow karta — sirf business logic.
//     DIP — repository INTERFACE pe depend, constructor injection.
//     Spring @Autowired constructor injection same DIP principle automate karta."
// ═══════════════════════════════════════════════════════════════════════

public class AccountService {

    // DIP — interface pe depend, concrete class pe nahi
    private final AccountRepository repository;
    private final EventPublisher eventPublisher;    // Observer — events fire

    // Constructor Injection — best practice
    public AccountService(AccountRepository repository, EventPublisher eventPublisher) {
        this.repository = repository;
        this.eventPublisher = eventPublisher;
    }

    // Naya account create — duplicate ID allowed nahi
    public void createAccount(Account account) {
        if (repository.findById(account.getAccountId()).isPresent()) {
            throw new IllegalArgumentException(
                "Account already exists: " + account.getAccountId()
            );
        }
        repository.save(account);
    }

    // Account fetch by ID — nahi mila toh AccountNotFoundException
    public Account getAccount(String accountId) {
       return repository.findById(accountId)
                .orElseThrow(() -> new AccountNotFoundException(accountId));
    }

    // Transfer — atomic operation. Fail toh manual rollback.
    // (Spring @Transactional this kaam automatic karta hai)
    public void transfer(String fromId, String toId, double amount) {
        if (amount <= 0) {
            throw new IllegalArgumentException("Amount > 0 hona chahiye");
        }

        Account from = getAccount(fromId);
        Account to = getAccount(toId);

        try {
            from.withdraw(amount);    // step 1 — fail toh exception
            to.deposit(amount);       // step 2
        } catch (RuntimeException e) {
            // step 1 succeed, step 2 fail = manual rollback
            from.deposit(amount);
            throw e;
        }

        // Observer Pattern — transfer succeed → event fire
        // (try-catch ke BAHAR — sirf actual success pe publish hota)
        eventPublisher.publish(new TransactionEvent(
            fromId, toId, amount, System.currentTimeMillis()
        ));
    }

    // Streams API: SOURCE → INTERMEDIATE (map) → TERMINAL (collect)
    public List<String> getAccountHolderNames() {
       return repository.findAll()                       // SOURCE — saare accounts
                .stream()                                // pipeline shuru
                .map(Account::getHolderName)             // INTERMEDIATE — Account → name
                .collect(Collectors.toList());           // TERMINAL — list mein collect
    }
}
