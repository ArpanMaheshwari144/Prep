package com.arpan.todoapp.model;
// ─── PACKAGE ─────────────────────────────────────────────────
// File path: src/main/java/com/arpan/todoapp/model/User.java

import jakarta.persistence.*;
import jakarta.validation.constraints.Email;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
// ─── IMPORTS ─────────────────────────────────────────────────
// jakarta.persistence.* → JPA annotations
// jakarta.validation    → Bean Validation (@Email khaas)
// lombok.*              → Boilerplate killers

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    User = ENTITY for AUTHENTICATION + OWNERSHIP
//    Har User = ek registered user
//    Login pe yahaan se credentials verify hote
//    Todo.userId yaha ke User.id se link
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL — JAVA ↔ MYSQL MAPPING:
//
//    @Entity Java class               DB "users" table:
//    ┌────────────────────────┐      ┌──────────────────────────┐
//    │  class User              │     │  id        BIGINT PK      │
//    │    Long id               │  →  │  name      VARCHAR(100)   │
//    │    String name           │     │  email     VARCHAR(150)   │
//    │    String email          │     │            UNIQUE          │
//    │    String password       │     │  password  VARCHAR(255)   │
//    └────────────────────────┘      └──────────────────────────┘
//                                          │
//                                          │ User.id linked to
//                                          ▼ Todo.user_id (FK)
//
// KEY ANNOTATIONS (same as Todo + extras):
//
//    @Entity                  → DB table mapped
//    @Table(name="users")     → plural convention
//                                ("user" is SQL reserved in some DBs)
//
//    @Email                   → email format validation
//                                Valid: a@b.com
//                                Invalid: "abc", "@x.com", "a@"
//
//    @Column(unique=true)     → UNIQUE constraint in DB
//                                Duplicate email blocked at INSERT
//                                → DataIntegrityViolationException
//
// EMAIL = LOGIN IDENTIFIER:
//    Email unique → login pe use kar sakte
//    name = display name (e.g., "Arpan Maheshwari")
//    email = login + identifier (e.g., arpan@gmail.com)
//
// PASSWORD STORAGE (SECURITY CRITICAL):
//    NEVER store plain password
//       DB leak = all users compromised
//
//    STORE BCRYPT HASH only
//       UserService.register() hash karta:
//          passwordEncoder.encode(rawPassword)
//
//       DB mein:
//          password = "$2a$10$xyz123abc..." (60-char hash)
//
//       Login pe match:
//          passwordEncoder.matches(rawInput, storedHash)
//          → BCrypt internally compares
//
//    @Size(min=8, max=255):
//       min 8 chars validation pe input check
//       max 255 because hash itself ~60 chars
//                   + flexibility for longer
//
// PASSWORD IN RESPONSE — DTO PATTERN:
//    User entity has password field
//    BUT API responses mein password NEVER bheja jaye
//    → DTO use karenge (UserResponse without password)
//    → Entity = internal only
//
// INTERVIEW LINE:
//    "User entity holds authentication credentials and identity.
//     Email is unique, used as login identifier. Password is
//     stored as BCrypt hash via Spring Security's PasswordEncoder
//     — never plain text. API responses use a DTO that strips
//     password, so the entity is internal-only."
// ═══════════════════════════════════════════════════════════════════════

@Entity
@Table(name = "users")
@Data
@NoArgsConstructor
@AllArgsConstructor
public class User {

    // ─── PRIMARY KEY ──────────────────────────────────────────
    // Long > int (accommodates billions of users)
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    // ─── DISPLAY NAME ──────────────────────────────────────────
    // Human-readable name (e.g., "Arpan Maheshwari")
    // @NotBlank: null/empty/whitespace REJECT
    // @Size: reasonable length cap
    @NotBlank(message = "Name required")
    @Size(min = 2, max = 100, message = "Name 2-100 chars")
    @Column(nullable = false, length = 100)
    private String name;

    // ─── EMAIL (LOGIN IDENTIFIER, UNIQUE) ──────────────────────
    // @Email: format check (@ + TLD required)
    // @Column(unique=true): DB-level uniqueness
    //                       Duplicate registration auto-blocked
    @NotBlank(message = "Email required")
    @Email(message = "Valid email required")
    @Size(max = 150)
    @Column(nullable = false, unique = true, length = 150)
    private String email;

    // ─── PASSWORD (BCRYPT HASH) ────────────────────────────────
    // Stored: $2a$10$... (60-char BCrypt hash)
    // NEVER plain text — UserService hash karta save se pehle
    // @Size: min 8 input pe, max 255 for hash flexibility
    @NotBlank(message = "Password required")
    @Size(min = 8, max = 255, message = "Password min 8 chars")
    @Column(nullable = false, length = 255)
    private String password;
}
