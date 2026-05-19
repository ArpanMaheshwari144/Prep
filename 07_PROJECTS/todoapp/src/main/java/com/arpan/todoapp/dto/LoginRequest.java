package com.arpan.todoapp.dto;
// ─── PACKAGE ─────────────────────────────────────────────────
// File: src/main/java/com/arpan/todoapp/dto/LoginRequest.java
// dto/ folder = request/response shapes (NOT entities)

import jakarta.validation.constraints.Email;
import jakarta.validation.constraints.NotBlank;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    LoginRequest = LOGIN ka HTTP request shape
//    POST /auth/login body mein yeh JSON aayega:
//       { "email": "a@b.com", "password": "secret123" }
//
//    AuthController @RequestBody mein @Valid lagayega
//    Validation fail = auto 400 Bad Request
// ═══════════════════════════════════════════════════════════════════════
//
// 🔑 WHY DTO (separate from User entity):
//    User entity has: id, name, email, password
//    Login only needs: email + password
//
//    Using User entity = caller could send extra fields
//    DTO = strict input shape (only what's needed)
//
//    Plus: entity changes don't break API
//    DTO = stable contract
//
// 🔑 ONLY 2 FIELDS:
//    email    → identifier (matches User.email DB unique)
//    password → raw password (will be BCrypt-checked in service)
//
//    NO name, NO id — only login essentials

@Data
@NoArgsConstructor
@AllArgsConstructor
public class LoginRequest {

    // ─── EMAIL ──────────────────────────────────────────────
    // @NotBlank = null/empty/whitespace reject
    // @Email = format validation (@x.y)
    @NotBlank(message = "Email required")
    @Email(message = "Valid email required")
    private String email;

    // ─── PASSWORD ───────────────────────────────────────────
    // @NotBlank = raw password must be present
    // No @Size here — let UserService check against stored hash
    @NotBlank(message = "Password required")
    private String password;
}
