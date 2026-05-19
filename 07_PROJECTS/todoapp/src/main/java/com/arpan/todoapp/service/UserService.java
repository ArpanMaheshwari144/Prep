package com.arpan.todoapp.service;
// ─── PACKAGE ─────────────────────────────────────────────────
// File: src/main/java/com/arpan/todoapp/service/UserService.java

import com.arpan.todoapp.model.User;
import com.arpan.todoapp.repository.UserRepository;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.stereotype.Service;

// ─── IMPORTS ─────────────────────────────────────────────────
// User, UserRepository → entity + DAO
// PasswordEncoder      → BCrypt hashing (SecurityConfig bean)
// @Service             → Spring stereotype

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    SERVICE LAYER for User — authentication flow
//    register + lookup + update
//    BCrypt password hashing integrated via PasswordEncoder
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
//    │    .register(user)    │ ← BCrypt hash karke save
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
// 🔑 DEPENDENCIES (constructor inject — 2):
//    1. UserRepository  → DB operations
//    2. PasswordEncoder → BCrypt hashing (from SecurityConfig bean)
//
// 🔑 BCrypt INTEGRATION:
//    register(): raw password → encoder.encode() → BCrypt hash → save
//    update():   same logic — never store raw password
//    Login flow uses encoder.matches(raw, hash) via AuthManager
//
// 🔑 SECURITY GUARANTEE:
//    DB mein password = "$2a$10$..." (60-char BCrypt hash)
//    DB leak ho jaaye = passwords cracking nearly impossible (BCrypt strong)
//    NEVER plain text stored
//
// 🎤 INTERVIEW LINE:
//    "UserService handles user lifecycle for authentication —
//     register, fetch by id/email, and update. Constructor
//     injection of UserRepository and BCryptPasswordEncoder.
//     All password operations go through encoder.encode() before
//     persistence — never plain text in DB. Login verification
//     handled by Spring's AuthenticationManager which uses
//     encoder.matches() internally."
// ═══════════════════════════════════════════════════════════════════════

@Service
public class UserService {

    private final UserRepository repo;
    private final PasswordEncoder encoder;

    // Constructor injection — Spring auto-wires
    // PasswordEncoder bean = SecurityConfig.passwordEncoder() (BCrypt)
    public UserService(UserRepository repo, PasswordEncoder encoder) {
        this.repo = repo;
        this.encoder = encoder;
    }

    // ─── REGISTER ──────────────────────────────────────────────
    // POST /auth/register
    // 🔑 SECURITY-CRITICAL: BCrypt hash password BEFORE save
    public User register(User user) {
        // Hash raw password — never store plain text
        // encoder.encode() = BCrypt + auto-salt + 10 rounds default
        // Output: "$2a$10$xyz..." (~60 chars)
        user.setPassword(encoder.encode(user.getPassword()));
        return repo.save(user);
    }

    // ─── GET BY ID ─────────────────────────────────────────────
    public User getById(Long id) {
        return repo.findById(id)
                   .orElseThrow(() -> new RuntimeException("User not found: " + id));
    }

    // ─── GET BY EMAIL ──────────────────────────────────────────
    // Login flow yahan se start hota (AuthController calls)
    public User getByEmail(String email) {
        return repo.findByEmail(email)
                   .orElseThrow(() -> new RuntimeException("User not found: " + email));
    }

    // ─── UPDATE ────────────────────────────────────────────────
    // PUT /users/{id} — modify existing
    // Pattern: fetch → modify fields → hash password → save
    public User update(Long id, User updated) {
        User existing = getById(id);
        existing.setName(updated.getName());
        existing.setEmail(updated.getEmail());
        // Hash new password too (same security rule)
        existing.setPassword(encoder.encode(updated.getPassword()));
        return repo.save(existing);
    }
}
