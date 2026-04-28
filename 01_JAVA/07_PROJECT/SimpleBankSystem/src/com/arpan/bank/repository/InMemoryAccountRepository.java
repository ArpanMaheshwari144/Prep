package com.arpan.bank.repository;

import com.arpan.bank.model.Account;
import java.util.*;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    AccountRepository ka concrete implementation. HashMap mein accounts
//    store karta — production mein iski jagah MySQL/Mongo wala impl aaye.
//
// 📌 JAVA KYA FOLLOW + KYU + KAISE:
//    • Collections — HashMap<String, Account> (O(1) lookup by ID)
//                  — ArrayList (defensive copy in findAll)
//    • Java 8 — Optional.ofNullable() — null-safe wrap
//    • OOPS — implements AccountRepository (interface contract)
//    • SOLID — DIP achieve (service interface pe depend, ye concrete pluggable)
// ═══════════════════════════════════════════════════════════════════════

public class InMemoryAccountRepository implements AccountRepository {

    // Collections — HashMap key=accountId, value=Account
    private final Map<String, Account> accounts = new HashMap<>();

    @Override
    public void save(Account account) {
        accounts.put(account.getAccountId(), account);
    }

    @Override
    public Optional<Account> findById(String accountId) {
        return Optional.ofNullable(accounts.get(accountId));
        // map.get() null bhi de sakta — ofNullable safe wrap
    }

    @Override
    public List<Account> findAll() {
        return new ArrayList<>(accounts.values());
        // defensive copy — caller modify kare toh internal safe
    }

    @Override
    public void delete(String accountId) {
        accounts.remove(accountId);
    }
}
