package com.arpan.usercrud.repository;

import java.util.Optional;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import com.arpan.usercrud.model.User;

/* ════════════════════════════════════════════════════════════════
 *  📌 REPOSITORY — Data Access Layer
 * ════════════════════════════════════════════════════════════════
 *  Iss layer ka kaam: DB se baat karo. Service layer iske through
 *  CRUD karta hai — direct SQL nahi likhna padta.
 *
 *  ─── INTERFACE KYU? Class kyu nahi? ─────────────────────────────
 *  Spring Data JPA runtime pe automatic implementation generate
 *  karta hai (proxy pattern). Tujhe sirf method signature likhna
 *  hai, body nahi. Magic ke peeche reflection + JPA criteria.
 *
 *  ─── REPOSITORY HIERARCHY (interview favorite) ──────────────────
 *
 *      Repository (marker interface — kuch nahi karta)
 *           ↑
 *      CrudRepository (save, findById, findAll, delete, count)
 *           ↑
 *      PagingAndSortingRepository (findAll(Pageable), findAll(Sort))
 *           ↑
 *      JpaRepository (saveAll, flush, deleteInBatch, findAll(Example))
 *
 *  Hum JpaRepository extend kar rahe hain → poore stack ka access.
 *
 *  ─── @Repository — yeh kyu lagana hai? ─────────────────────────
 *  Strictly speaking, JpaRepository extend karne par yeh OPTIONAL
 *  hai (Spring auto-detect karta hai). Phir bhi lagaate hain kyunki:
 *
 *  • Code intent clear hota hai (yeh DAO layer hai)
 *  • Exception Translation milta hai — DB-specific exceptions
 *    (SQLException, JpaSystemException) Spring ke
 *    DataAccessException family mein translate hote hain
 *
 *  ─── DERIVED QUERY METHODS — Spring ka jadoo ────────────────────
 *  Method ka NAAM hi query ban jata hai. No SQL, no JPQL.
 *
 *  • findByEmail(String email)
 *      → SELECT * FROM users WHERE email = ?
 *  • findByNameAndAge(String n, int a)
 *      → SELECT * FROM users WHERE name=? AND age=?
 *  • findByAgeGreaterThan(int a)
 *      → SELECT * FROM users WHERE age > ?
 *
 *  Custom complex query chahiye? → @Query("SELECT u FROM User u...")
 *
 *  ─── Optional<User> kyu return karte hain? ──────────────────────
 *  Null safety. Jab user mil bhi sakta hai, nahi bhi — Optional
 *  bolta hai "isko handle karna padega". null pointer exceptions
 *  se bachata hai.
 *
 *  ════════════════════════════════════════════════════════════════
 *  🎨 DESIGN PATTERN: REPOSITORY (Spring Data JPA implementation)
 *  📐 SOLID: ISP, DIP
 *  ════════════════════════════════════════════════════════════════
 *
 *  Yeh interface JpaRepository extend karti — Spring Data JPA
 *  AUTOMATIC implementation generate karta runtime pe:
 *    • save(), findById(), findAll(), delete() — inherited
 *    • findByEmail() — derived query (method naam se SQL banta)
 *
 *  Hamne SimpleBankSystem mein MANUAL Repository pattern banaya
 *  tha — yahaan Spring Data JPA WAHI pattern automate karta.
 *
 *  📐 SOLID — ISP (Interface Segregation):
 *  Interface FOCUSED hai — sirf User-related operations. Other
 *  domain operations (Order, Product) alag interfaces mein.
 *
 *  📐 SOLID — DIP (Dependency Inversion):
 *  UserService iss INTERFACE pe depend karta, koi concrete
 *  implementation pe nahi. Spring runtime pe proxy generate karta —
 *  H2 / MySQL / PostgreSQL switch karne pe service code unchanged.
 *
 *  🎤 INTERVIEW LINE:
 *  "UserRepository Spring Data JPA Repository pattern follow karta —
 *   JpaRepository extend karne se basic CRUD inherited, custom
 *   methods derived query syntax se (findByEmail). Manual
 *   AccountRepository jaisa pattern, but Spring automate karta —
 *   ISP + DIP dono follow."
 * ════════════════════════════════════════════════════════════════
 */
@Repository
public interface UserRepository extends JpaRepository<User, Long> {

    // Login feature ke liye email-based lookup (future use)
    Optional<User> findByEmail(String email);
}
