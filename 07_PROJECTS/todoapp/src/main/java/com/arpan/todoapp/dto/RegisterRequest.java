package com.arpan.todoapp.dto;
// ─── PACKAGE ─────────────────────────────────────────────────
// File: src/main/java/com/arpan/todoapp/dto/RegisterRequest.java

import jakarta.validation.constraints.Email;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    RegisterRequest = REGISTER ka HTTP request shape
//    POST /auth/register body mein:
//       { "name": "Arpan", "email": "a@b.com", "password": "secret123" }
//
//    AuthController @RequestBody mein @Valid → auto 400 on invalid
// ═══════════════════════════════════════════════════════════════════════
//
// 🔑 3 FIELDS (sab User ke jaise minus id):
//    name      → display name
//    email     → unique identifier (DB constraint matches)
//    password  → raw password (BCrypt hash before save)
//
//    NO id (auto-generated in DB)
//
// 🔑 VALIDATION SAME AS User ENTITY:
//    Consistency — same rules at API + entity level
//    User entity validation triggers if entity directly validated
//    DTO validation triggers at request time (caller-facing)

@Data
@NoArgsConstructor
@AllArgsConstructor
public class RegisterRequest {

    // ─── NAME ───────────────────────────────────────────────
    @NotBlank(message = "Name required")
    @Size(min = 2, max = 100, message = "Name 2-100 chars")
    private String name;

    // ─── EMAIL ──────────────────────────────────────────────
    @NotBlank(message = "Email required")
    @Email(message = "Valid email required")
    @Size(max = 150)
    private String email;

    // ─── PASSWORD ───────────────────────────────────────────
    // @Size: min 8 chars (security baseline)
    //         max 255 (covers reasonable input)
    @NotBlank(message = "Password required")
    @Size(min = 8, max = 255, message = "Password min 8 chars")
    private String password;
}
