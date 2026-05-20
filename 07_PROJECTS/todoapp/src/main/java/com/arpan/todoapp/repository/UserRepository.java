package com.arpan.todoapp.repository;
// ─── PACKAGE ─────────────────────────────────────────────────
// File: src/main/java/com/arpan/todoapp/repository/UserRepository.java

import com.arpan.todoapp.model.User;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import java.util.Optional;
// ─── IMPORTS ─────────────────────────────────────────────────
// User           → entity hum handle karenge
// JpaRepository  → Spring Data JPA power-pack
// @Repository    → DAO marker + exception translation
// Optional       → null-safe wrapper for findByEmail result

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    DATA ACCESS LAYER for User entity
//    Authentication flow yaha se DB hits karta
//    Login pe email se user khojna = findByEmail()
// ═══════════════════════════════════════════════════════════════════════
//
// FREE METHODS (Inherited from JpaRepository):
//    save(User), findById(Long), findAll(), delete(User),
//    deleteById(Long), count(), existsById(Long), etc.
//
// findByEmail KYU CHAHIYE:
//
//    LOGIN FLOW:
//       1. User POST /auth/login {email, password}
//       2. UserService:
//             userRepo.findByEmail(email)
//                .orElseThrow(() -> new UserNotFoundException());
//       3. Password match BCrypt se
//       4. JWT token return
//
//    REGISTRATION FLOW:
//       1. User POST /auth/register
//       2. UserService check:
//             userRepo.findByEmail(email)
//                .ifPresent(u -> throw new DuplicateEmailException());
//       3. Naya user save
//
//    = Email = login identifier (unique field)
//    = findByEmail = ESSENTIAL for auth
//
// Optional<User> WHY:
//    Email DB mein mil sakta → User return
//    Email nahi mila → Optional.empty()
//
//    Caller MUST handle:
//       userRepo.findByEmail("x@y.com")
//               .orElseThrow(() -> new UserNotFoundException());
//
//    = No NullPointerException
//    = Explicit "might not exist" semantic
//
// SPRING DERIVED QUERY MAGIC:
//    findByEmail(String email)
//       Spring parser:
//          "findBy" prefix → SELECT
//          "Email" property → WHERE email = ?
//       Generated SQL:
//          SELECT * FROM users WHERE email = ?
//
//    No SQL written by you!
//    Just method name = query
//
// INTERVIEW LINE:
//    "UserRepository extends JpaRepository providing free CRUD.
//     Custom findByEmail leverages Spring Data JPA's derived query
//     mechanism — method name parsed to JPQL automatically.
//     Returns Optional for null-safe lookup, mandatory for
//     authentication flow."
// ═══════════════════════════════════════════════════════════════════════

@Repository
public interface UserRepository extends JpaRepository<User, Long> {

    // Login + registration ke liye email-based lookup
    // SELECT * FROM users WHERE email = ?
    Optional<User> findByEmail(String email);
}
