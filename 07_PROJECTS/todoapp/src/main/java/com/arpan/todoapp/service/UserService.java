package com.arpan.todoapp.service;
// ─── PACKAGE ─────────────────────────────────────────────────
// File: src/main/java/com/arpan/todoapp/service/UserService.java

import com.arpan.todoapp.model.User;
import com.arpan.todoapp.repository.UserRepository;
import org.springframework.stereotype.Service;

// ─── IMPORTS ─────────────────────────────────────────────────
// User, UserRepository → entity + DAO
// @Service             → Spring stereotype

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    SERVICE LAYER for User
//    Authentication flow yaha — register + lookup + update
//    AuthController iske through user create / fetch karta
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL — AUTH FLOW:
//
//    POST /auth/register {name, email, password}
//          │
//          ▼
//    ┌──────────────────────┐
//    │  AuthController       │
//    │    .register()        │
//    └──────────┬───────────┘
//               │ calls userService.register(user)
//               ▼
//    ┌──────────────────────┐
//    │  UserService          │  ← TU YAHAN
//    │    .register(user)    │
//    │    .getById(id)       │
//    │    .getByEmail(email) │
//    │    .update(id, user)  │
//    └──────────┬───────────┘
//               │ calls userRepo.save()
//               ▼
//    ┌──────────────────────┐
//    │  UserRepository       │
//    └──────────────────────┘
//
// 🔑 METHODS (4):
//
//    register(User user)
//       → New user save
//       → ⚠️ Password hashing baad mein add karenge
//          jab SecurityConfig + PasswordEncoder bean ban jaye
//       → Abhi raw password save (TEMP — security mein fix)
//
//    getById(Long id)
//       → Single user fetch
//       → Optional unwrap with orElseThrow
//
//    getByEmail(String email)
//       → Login flow ke liye
//       → UserRepository.findByEmail() use karta
//
//    update(Long id, User updated)
//       → Fetch existing, modify fields, save
//       → Pattern same as TodoService.update()
//
// 🔑 CONSTRUCTOR INJECTION (same pattern):
//    private final UserRepository repo;
//    public UserService(UserRepository repo) { this.repo = repo; }
//
// 🔑 ⚠️ PASSWORD HASHING — TODO:
//    Abhi register/update raw password save kar raha
//    SecurityConfig file banayenge → BCryptPasswordEncoder bean
//    Tab UserService mein:
//       private final PasswordEncoder encoder;
//       user.setPassword(encoder.encode(user.getPassword()));
//    Production-grade banega
//
// 🎤 INTERVIEW LINE:
//    "UserService handles user lifecycle for authentication —
//     register, fetch by id/email, and update. Constructor
//     injection of UserRepository. Password hashing via Spring
//     Security's PasswordEncoder will integrate with the security
//     layer, ensuring BCrypt-hashed credentials before persistence."
// ═══════════════════════════════════════════════════════════════════════

@Service
public class UserService {

    private final UserRepository repo;

    public UserService(UserRepository repo) {
        this.repo = repo;
    }

    // ─── REGISTER ──────────────────────────────────────────────
    // POST /auth/register
    // ⚠️ Password hashing TODO — SecurityConfig ke baad
    public User register(User user) {
        return repo.save(user);
    }

    // ─── GET BY ID ─────────────────────────────────────────────
    public User getById(Long id) {
        return repo.findById(id)
                   .orElseThrow(() -> new RuntimeException("User not found: " + id));
    }

    // ─── GET BY EMAIL ──────────────────────────────────────────
    // Login flow yahan se start hota
    public User getByEmail(String email) {
        return repo.findByEmail(email)
                   .orElseThrow(() -> new RuntimeException("User not found: " + email));
    }

    // ─── UPDATE ────────────────────────────────────────────────
    // PUT /users/{id} — modify existing
    // Pattern: fetch → modify fields → save
    public User update(Long id, User updated) {
        User existing = getById(id);
        existing.setName(updated.getName());
        existing.setEmail(updated.getEmail());
        existing.setPassword(updated.getPassword());  // ⚠️ raw — hash later
        return repo.save(existing);
    }
}
