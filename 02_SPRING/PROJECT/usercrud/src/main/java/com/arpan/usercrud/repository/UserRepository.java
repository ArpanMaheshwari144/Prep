package com.arpan.usercrud.repository;

import java.util.Optional;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import com.arpan.usercrud.model.User;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    DATA ACCESS LAYER — DB se baat karna
//    Service layer iske through CRUD karta
//    Direct SQL/JPQL nahi likhna padta = Spring Data JPA magic
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL — REPOSITORY HIERARCHY:
//    Repository (marker interface — kuch nahi karta)
//         ↑
//    CrudRepository
//    (save, findById, findAll, delete, count)
//         ↑
//    PagingAndSortingRepository
//    (findAll(Pageable), findAll(Sort))
//         ↑
//    JpaRepository                        ← TU YAHAN EXTEND
//    (saveAll, flush, deleteInBatch, JPA-specific)
//
//    Extend kare → POORA stack inherited
//
// WHY INTERFACE (Not Class)?
//    Spring Data JPA MAGIC:
//       Runtime pe proxy create karta
//       Tu sirf signature likhe — body Spring generate
//       Behind scenes: Reflection + JPA Criteria API
//
// WHAT YOU GET FREE (Inherited):
//    save(User)              → INSERT/UPDATE
//    findById(Long)          → SELECT WHERE id
//    findAll()               → SELECT *
//    delete(User)            → DELETE
//    deleteById(Long)        → DELETE WHERE id
//    count()                 → SELECT COUNT
//    existsById(Long)        → SELECT 1 WHERE id
//    findAll(Sort)           → ORDER BY
//    findAll(Pageable)       → pagination
//    saveAll(Iterable)       → batch insert
//    flush()                 → force write
//
// 🔑 DERIVED QUERY METHODS — Spring Magic:
//    Method NAME hi query ban jata!
//
//    findByEmail(String email)
//       → SELECT * FROM users WHERE email = ?
//
//    findByNameAndAge(String name, int age)
//       → SELECT * FROM users WHERE name=? AND age=?
//
//    findByAgeGreaterThan(int age)
//       → SELECT * FROM users WHERE age > ?
//
//    findByNameContainingIgnoreCase(String name)
//       → SELECT * FROM users WHERE LOWER(name) LIKE LOWER(?)
//
//    countByActive(boolean active)
//       → SELECT COUNT(*) FROM users WHERE active=?
//
//    existsByEmail(String email)
//       → SELECT 1 FROM users WHERE email=?
//
// 🔑 CUSTOM COMPLEX QUERIES — @Query:
//    @Query("SELECT u FROM User u WHERE u.age > :age AND u.role = :role")
//    List<User> findActiveAdults(@Param("age") int age, @Param("role") String role);
//
//    @Query(value = "SELECT * FROM users WHERE created_at > NOW() - INTERVAL 7 DAY",
//           nativeQuery = true)
//    List<User> findRecentUsers();
//
//    JPQL    = Java Persistence Query Language (default)
//    Native  = raw SQL (nativeQuery = true)
//
// 🔑 WHY @Repository ANNOTATION?
//    Strictly OPTIONAL with JpaRepository (auto-detected).
//
//    But add karte kyun?
//       1. CODE INTENT — DAO layer marker clear
//       2. EXCEPTION TRANSLATION — BIG benefit
//          SQLException/JpaSystemException
//          → Spring's DataAccessException family
//          = Database vendor change? Caller code unchanged
//
// 🔑 WHY Optional<User>?
//    Null safety:
//       User mil sakta hai (Optional.of)
//       Ya nahi (Optional.empty)
//
//    Caller MUST handle:
//       userRepo.findByEmail("x@y.com")
//               .orElseThrow(() -> new UserNotFoundException());
//       = No NullPointerException
//       = Explicit null handling
//
// HOW SPRING IMPLEMENTS AT RUNTIME:
//    At startup:
//       Spring sees: UserRepository interface
//       Scans methods + parents
//       Creates PROXY class (dynamic):
//          class UserRepository$Proxy implements UserRepository {
//              public Optional<User> findByEmail(String email) {
//                  // Parse "findByEmail" → JPQL
//                  // Execute via EntityManager
//              }
//          }
//       Bean register
//
//    @Autowired UserRepository repo;
//       = Tu actual interface use karta
//       = Spring inject karta proxy implementation
//
// SimpleBankSystem vs UserCRUD:
//    SimpleBankSystem (manual):
//       AccountRepository interface
//          + InMemoryAccountRepository (HashMap manual)
//
//    UserCRUD (Spring Data JPA):
//       UserRepository extends JpaRepository
//       = Spring auto-generates implementation
//       = Same Repository pattern, AUTOMATED
//
// 🎨 PATTERN: REPOSITORY (Spring Data JPA implementation)
//
// 📐 SOLID:
//    ISP — Interface focused (only User operations)
//    DIP — Service depends on INTERFACE, proxy auto-generated
//          MySQL/Postgres switch = service unchanged
//
// 🎤 INTERVIEW LINE:
//    "UserRepository extends JpaRepository — Spring Data JPA
//     auto-generates implementation via runtime proxy.
//     Free CRUD + paging + sorting inherited.
//     Custom queries via derived methods (findByEmail) or @Query.
//     @Repository optional but recommended for exception translation."
// ═══════════════════════════════════════════════════════════════════════

@Repository
public interface UserRepository extends JpaRepository<User, Long> {

    // Login feature ke liye email-based lookup
    Optional<User> findByEmail(String email);
}
