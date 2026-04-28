package com.arpan.bank.repository;

import com.arpan.bank.model.Account;
import java.util.List;
import java.util.Optional;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    Storage layer ka CONTRACT (interface). Account save/find/delete ke
//    rules define karta — actual storage kaisa hoga, koi concrete class
//    handle karegi.
//
// 📌 JAVA KYA FOLLOW + KYU + KAISE:
//    • OOPS — Interface (sirf rule, body nahi)
//    • SOLID — DIP applied. Service is interface pe depend, koi specific
//      DB ya HashMap pe nahi → kal MySQL chahiye? naya impl, service untouched.
//    • Java 8 — Optional<Account> return (null-safe API)
// ═══════════════════════════════════════════════════════════════════════

public interface AccountRepository {

    void save(Account account);
    Optional<Account> findById(String accountId);    // Optional — null safe
    List<Account> findAll();
    void delete(String accountId);
}
