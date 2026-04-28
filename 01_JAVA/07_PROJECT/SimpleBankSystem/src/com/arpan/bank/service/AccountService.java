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
//
// 📌 JAVA KYA FOLLOW + KYU + KAISE:
//    • SOLID — SRP (sirf business rules — DB nahi, email nahi)
//              DIP (Repository + EventPublisher INTERFACES inject)
//    • Constructor Injection — final fields, test mein mock easy
//    • Java 8 — Optional (orElseThrow), Streams (map/collect)
//    • Custom Exception — AccountNotFoundException throw
//    • Manual Transaction — try-catch + rollback (Spring @Transactional preview)
//    • Observer — transfer success pe event fire (publish karta)
// ═══════════════════════════════════════════════════════════════════════

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
