package com.arpan.bank.repository;

import com.arpan.bank.model.Account;
import java.util.List;
import java.util.Optional;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    Storage layer ka CONTRACT (interface). Account save/find/delete ke
//    rules define karta — actual storage kaisa hoga, koi concrete class
//    handle karegi.
// ═══════════════════════════════════════════════════════════════════════
//
// ╔═══════════════════════════════════════════════════════════════════╗
// ║  🎨 DESIGN PATTERN: REPOSITORY                                     ║
// ╠═══════════════════════════════════════════════════════════════════╣
// ║                                                                   ║
// ║  Data access ko ABSTRACT karta — service layer bolta "save",      ║
// ║  repository decide karta DB / file / in-memory wherever.          ║
// ║                                                                   ║
// ║  Yeh INTERFACE hai (contract only) — implementations alag:        ║
// ║    • InMemoryAccountRepository — HashMap-backed (current)         ║
// ║    • MySQLAccountRepository    — DB-backed (future)               ║
// ║    • MongoAccountRepository    — NoSQL-backed (future)            ║
// ║                                                                   ║
// ║  Caller code (AccountService) UNCHANGED — sirf wiring badle.      ║
// ║                                                                   ║
// ║  📐 SOLID — ISP (Interface Segregation):                          ║
// ║  Yeh interface FOCUSED hai — sirf account data ops (save,         ║
// ║  findById, findAll, delete). 20 unrelated methods nahi —          ║
// ║  cohesive contract.                                               ║
// ║                                                                   ║
// ║  📐 SOLID — DIP (Dependency Inversion):                           ║
// ║  AccountService iss INTERFACE pe depend karta, concrete           ║
// ║  InMemoryAccountRepository pe nahi. Implementation swap kar       ║
// ║  sakte without changing service.                                  ║
// ║                                                                   ║
// ║  Real-world: Spring Data JPA JpaRepository — same Repository      ║
// ║  pattern at framework level. Hum manual implement kar rahe.       ║
// ║                                                                   ║
// ║  🎤 INTERVIEW LINE:                                                ║
// ║  "Repository pattern data access abstract karta —                 ║
// ║   AccountRepository interface, InMemoryAccountRepository          ║
// ║   implementation. Service layer interface se baat karta — future  ║
// ║   mein MySQL ya MongoDB switch kare, sirf nayi implementation     ║
// ║   likhni — service code unchanged. Spring Data JPA same pattern   ║
// ║   follow karta."                                                  ║
// ║                                                                   ║
// ╚═══════════════════════════════════════════════════════════════════╝

public interface AccountRepository {

    void save(Account account);
    Optional<Account> findById(String accountId);    // Optional — null safe
    List<Account> findAll();
    void delete(String accountId);
}
