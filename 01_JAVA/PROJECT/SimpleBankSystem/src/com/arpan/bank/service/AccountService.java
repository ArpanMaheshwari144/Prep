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
//    Bank ka business logic — account create, get, transfer karta. Layers
//    ka beech wala — Controller niche bulayega, ye Repository/Publisher
//    ko coordinate karta.
// ═══════════════════════════════════════════════════════════════════════
//
// ╔═══════════════════════════════════════════════════════════════════╗
// ║  📐 SOLID PRINCIPLES APPLIED                                       ║
// ╠═══════════════════════════════════════════════════════════════════╣
// ║                                                                   ║
// ║  ✅ SRP (Single Responsibility):                                  ║
// ║  Yeh class SIRF business logic handle karti — account creation,   ║
// ║  transfers, balance ops. Data access NAHI (Repository ka kaam),   ║
// ║  notification NAHI (EventListener ka kaam), creation NAHI         ║
// ║  (Factory ka kaam). Single reason to change.                      ║
// ║                                                                   ║
// ║  ✅ DIP (Dependency Inversion):                                   ║
// ║  Service `AccountRepository` INTERFACE pe depend karta, concrete  ║
// ║  `InMemoryAccountRepository` pe nahi.                             ║
// ║                                                                   ║
// ║      private final AccountRepository repository;  ← INTERFACE     ║
// ║                                                                   ║
// ║      public AccountService(AccountRepository repo) {              ║
// ║          this.repository = repo;   ← CONSTRUCTOR INJECTION        ║
// ║      }                                                            ║
// ║                                                                   ║
// ║  Future MySQL switch? Sirf wiring badlo — service untouched.      ║
// ║                                                                   ║
// ║  ✅ CONSTRUCTOR INJECTION — best practice:                        ║
// ║  • Field final ban sakta (immutable)                              ║
// ║  • NPE impossible (object banega hi nahi without dependency)      ║
// ║  • Testing easy (mock pass karke construct)                       ║
// ║                                                                   ║
// ║  🎤 INTERVIEW LINE:                                                ║
// ║  "AccountService SRP follow karta — sirf business logic, data     ║
// ║   access aur notification alag classes. DIP bhi — repository      ║
// ║   interface pe depend, constructor injection use kiya. Spring     ║
// ║   @Autowired constructor injection same DIP principle automate    ║
// ║   karta."                                                         ║
// ║                                                                   ║
// ╚═══════════════════════════════════════════════════════════════════╝

/**
 * AccountService — Business Logic Layer
 *
 * SOLID applied:
 *   • SRP — sirf account business rules, DB nahi
 *   • DIP — AccountRepository (interface) pe depend
 *   • Constructor Injection — best practice for testability
 *
 * Java 8 demo: Optional handling, Streams (getAccountHolderNames)
 * Multithreading: transfer() ke andar manual rollback (Spring @Transactional jaisa)
 */
public class AccountService {

    // DIP — interface pe depend, concrete class pe nahi
    private final AccountRepository repository;
    private final EventPublisher eventPublisher;    // Observer — events fire karne ke liye

    // Constructor Injection — outside se repository + publisher pass honge
    // Best practice (vs field injection or setter injection)
    public AccountService(AccountRepository repository, EventPublisher eventPublisher) {
        this.repository = repository;
        this.eventPublisher = eventPublisher;
    }

    // Naya account create — business rule: duplicate ID allowed nahi
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
       return repository.findById(accountId).orElseThrow(() -> new AccountNotFoundException(accountId));
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

    // SOURCE → stream → INTERMEDIATE (map: Account → String) → TERMINAL (collect to list)
    public List<String> getAccountHolderNames() {
       return repository.findAll()                       // SOURCE — saare accounts
                .stream()                                // pipeline shuru
                .map(Account::getHolderName)             // INTERMEDIATE — Account → name
                .collect(Collectors.toList());           // TERMINAL — list mein collect
    }
}
