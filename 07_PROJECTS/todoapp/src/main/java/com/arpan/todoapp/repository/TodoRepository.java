package com.arpan.todoapp.repository;
// ─── PACKAGE ─────────────────────────────────────────────────
// File: src/main/java/com/arpan/todoapp/repository/TodoRepository.java

import com.arpan.todoapp.model.Todo;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import java.util.List;
// ─── IMPORTS ─────────────────────────────────────────────────
// Todo            → entity hum handle karenge
// JpaRepository   → Spring Data JPA power-pack
// @Repository     → DAO marker + exception translation
// List            → return type for collection queries

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    DATA ACCESS LAYER for Todo entity
//    Service layer iske through DB operations karta
//    Direct SQL nahi — Spring Data JPA magic auto-handle
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL — REPOSITORY HIERARCHY:
//
//    Repository<T,ID>             (marker interface)
//         ↑
//    CrudRepository<T,ID>
//    (save, findById, findAll, delete, count)
//         ↑
//    PagingAndSortingRepository<T,ID>
//    (findAll(Pageable), findAll(Sort))
//         ↑
//    JpaRepository<T,ID>           ← TU YAHAN EXTEND
//    (saveAll, flush, JPA-specific extras)
//
// INTERFACE KYU, CLASS NAHI?
//    Spring Data JPA MAGIC:
//       • Tu sirf signature likhe
//       • Body Spring runtime pe generate (proxy)
//       • Reflection + JPA Criteria API se SQL banta
//
// FREE METHODS (Inherited):
//    save(Todo)               → INSERT or UPDATE
//    findById(Long)           → SELECT WHERE id = ?
//    findAll()                → SELECT *
//    delete(Todo)             → DELETE
//    deleteById(Long)         → DELETE WHERE id = ?
//    count()                  → SELECT COUNT(*)
//    existsById(Long)         → SELECT 1 WHERE id = ?
//
//    = Yeh sab tu likhe bina mil jaate
//
// DERIVED QUERY METHODS — Spring magic:
//    Method NAME hi query ban jata.
//    findBy + FieldName → SELECT WHERE field = ?
//
//    findByUserId(Long userId)
//       → SELECT * FROM todos WHERE user_id = ?
//
// WHY findByUserId YAHAN ZAROORI:
//
//    Multi-user TODO app:
//       User A ko sirf apne todos chahiye
//       findAll() = SAB users ke todos return 
//       findByUserId(A's id) = sirf A's todos 
//
//    Service layer mein:
//       todoRepo.findByUserId(loggedInUserId)
//       = ownership enforcement
//
// @Repository ANNOTATION:
//    OPTIONAL with JpaRepository (auto-detected)
//    BUT ADD karte kyu:
//       1. CODE INTENT — DAO layer marker clear
//       2. EXCEPTION TRANSLATION:
//          SQLException/JpaSystemException
//          → Spring's DataAccessException family
//          = DB vendor switch? Caller code unchanged
//
// HOW SPRING IMPLEMENTS AT RUNTIME:
//    1. Spring scan finds TodoRepository (interface)
//    2. Method signatures parse
//    3. Dynamic proxy create:
//       class TodoRepository$Proxy implements TodoRepository {
//           public List<Todo> findByUserId(Long userId) {
//               // Parse method name → JPQL
//               // Execute via EntityManager
//           }
//       }
//    4. Bean register in context
//
//    @Autowired TodoRepository repo;
//       = Tu interface use karta
//       = Spring proxy inject karta
//
// INTERVIEW LINE:
//    "TodoRepository extends JpaRepository — Spring Data JPA
//     auto-generates implementation via runtime proxy. Inherited
//     CRUD from hierarchy. Custom derived methods like
//     findByUserId map to JPQL via method-name parser.
//     @Repository optional but recommended for exception
//     translation."
// ═══════════════════════════════════════════════════════════════════════

@Repository
public interface TodoRepository extends JpaRepository<Todo, Long> {

    // Ek user ke saare todos fetch karne ke liye
    // SELECT * FROM todos WHERE user_id = ?
    List<Todo> findByUserId(Long userId);
}
