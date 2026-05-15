package com.arpan.usercrud.model;

import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.Table;
import jakarta.validation.constraints.Email;
import jakarta.validation.constraints.Max;
import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    JPA ENTITY = Java class jo DB table represent karti
//    Har User object = ek ROW in 'users' table
//    Hibernate is class ko padhke table auto-banaye
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL — ENTITY TO DB MAPPING:
//    @Entity Java class           DB users table
//    ┌──────────────────────┐    ┌────────────────────┐
//    │  class User           │    │ id (PK, AUTO)       │
//    │   Long id             │ →  │ name (NOT NULL)     │
//    │   String name         │    │ email (UNIQUE)      │
//    │   String email        │    │ password (NOT NULL) │
//    │   String password     │    │ age                 │
//    │   int age             │    │ role (DEFAULT USER) │
//    │   String role         │    └────────────────────┘
//    └──────────────────────┘
//    Hibernate translates annotations → SQL constraints
//
// 3 CATEGORIES OF ANNOTATIONS:
//    1. JPA / HIBERNATE → DB mapping
//       @Entity, @Table, @Id, @GeneratedValue, @Column
//
//    2. BEAN VALIDATION → input validation
//       @NotBlank, @Email, @Size, @Min, @Max
//
//    3. LOMBOK → boilerplate killer
//       @Data, @NoArgsConstructor, @AllArgsConstructor
//
// 🔑 JPA ANNOTATIONS DEEP:
//
//    @Entity
//       "Hibernate, yeh class ko table samjho"
//
//    @Table(name = "users")
//       Plural convention — "user" is SQL reserved word in some DBs
//
//    @Id + @GeneratedValue(strategy = IDENTITY)
//       Primary key + DB auto-increment (MySQL preferred)
//       Strategies:
//          IDENTITY  → DB auto-increment
//          AUTO      → JPA decides
//          SEQUENCE  → DB sequence (Postgres)
//          TABLE     → separate ID table (rare)
//
//    @Column(nullable = false, unique = true)
//       nullable=false → NOT NULL in SQL
//       unique=true    → UNIQUE constraint (duplicate email blocked at DB)
//
// 🔑 BEAN VALIDATION:
//    @NotBlank   → null + empty + whitespace blocked (Strings)
//    @NotNull    → just not null (any type)
//    @NotEmpty   → not null + size > 0
//    @Size       → length range
//    @Email      → email format check
//    @Min/@Max   → numeric range
//    @Pattern    → regex
//
//    Trigger flow:
//       Controller: @Valid @RequestBody User u
//       Spring validates BEFORE method entry
//       Fail → MethodArgumentNotValidException → 400
//
// 🔑 LOMBOK — BOILERPLATE KILLER:
//    @Data
//       Auto-generate: getters/setters/toString/equals/hashCode
//       = ~50 lines saved
//
//    @NoArgsConstructor
//       Empty constructor — JPA REQUIRED
//       Hibernate uses reflection to create object
//       Without it: runtime exception
//
//    @AllArgsConstructor
//       Constructor with all fields
//       Manual object creation easy
//
//    Compile-time magic:
//       Source mein nahi dikhte methods
//       Compile time pe BYTECODE inject hota
//       .class file mein getters/setters honge
//
// WHY JPA NEEDS @NoArgsConstructor?
//    Hibernate flow when loading entity:
//       1. SELECT * FROM users WHERE id = ?
//       2. Hibernate uses REFLECTION to create object
//       3. Reflection needs EMPTY constructor
//       4. Then sets fields one by one
//    Missing → ❌ Hibernate cannot instantiate
//
// ENTITY vs DTO:
//    Entity (this file)      DTO (separate classes)
//       DB mapped               API request/response shape
//       Hibernate-managed       Cross API boundary
//       Inside repo/service     Hide entity internals (password!)
//
//    Production: keep separate (UserDTO ≠ User entity)
//    This project: simple — User used both (educational)
//
// 📐 SOLID — SRP:
//    Sirf User domain data + validation rules
//    No business logic, no DB access
//
// 🎤 INTERVIEW LINE:
//    "User class is JPA entity — @Entity marks it persistent,
//     @Table('users') uses plural to avoid SQL reserved word.
//     ID auto-generated via IDENTITY (MySQL auto-increment).
//     Bean Validation annotations trigger with @Valid in controller.
//     Lombok eliminates boilerplate. @NoArgsConstructor required for
//     JPA's reflection-based instantiation."
// ═══════════════════════════════════════════════════════════════════════

@Entity
@Table(name = "users")
@Data
@NoArgsConstructor
@AllArgsConstructor
public class User {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column(nullable = false)
    @NotBlank(message = "Name is required")
    @Size(min = 2, max = 50, message = "Name must be 2-50 characters")
    private String name;

    @Column(nullable = false, unique = true)
    @NotBlank(message = "Email is required")
    @Email(message = "Email must be valid")
    private String email;

    @Column(nullable = false)
    @NotBlank(message = "Password is required")
    @Size(min = 5, message = "Password must be at least 5 characters")
    private String password;

    @Min(value = 18, message = "Age must be at least 18")
    @Max(value = 120, message = "Age must be at most 120")
    private int age;

    // ─── ROLE — for authorization (admin/user permissions) ─────
    // Default = "USER" (normal user)
    // Spring Security ROLE_ prefix internally use karta —
    // hum simple "USER" / "ADMIN" rakhte, prefix authority creation
    // time pe add karenge (CustomUserDetailsService mein)
    @Column(nullable = false)
    private String role = "USER";
}
