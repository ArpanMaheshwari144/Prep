package com.arpan.todoapp.controller;
// ─── PACKAGE ─────────────────────────────────────────────────
// File: src/main/java/com/arpan/todoapp/controller/UserController.java

import com.arpan.todoapp.model.User;
import com.arpan.todoapp.service.UserService;
import jakarta.validation.Valid;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

// ─── IMPORTS ─────────────────────────────────────────────────
// User, UserService     → entity + business logic
// @Valid                → trigger Bean Validation
// ResponseEntity        → HTTP response wrapper
// @RestController, etc. → REST mapping annotations

// ═══════════════════════════════════════════════════════════════════════
// 📌 YE FILE KYA HAI:
//    REST API LAYER for User
//    HTTP entry — routing only, business logic UserService mein
//    Auth endpoints (login/register flow) abhi yahin
//    Future: JWT add karne pe AuthController alag bana sakte
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL — UserController endpoints (3 methods):
//
//    POST   /users        → CREATE  (register)
//    GET    /users/{id}   → READ ONE
//    PUT    /users/{id}   → UPDATE
//
//    ❌ NO delete (tune nahi mangya)
//    ❌ NO get-all (tune nahi mangya)
//    ❌ NO login flow yet (JWT layer ke saath aayega)
//
// 🔑 KEY ANNOTATIONS (same as TodoController):
//
//    @RestController        → @Controller + @ResponseBody (auto JSON)
//    @RequestMapping("/users") → class-level base path
//    @PostMapping, @GetMapping, @PutMapping → HTTP verbs
//    @PathVariable          → URL part binding
//    @RequestBody + @Valid  → JSON body + validation trigger
//
// 🔑 SERVICE INJECTION (constructor):
//    private final UserService service;
//    public UserController(UserService service) { this.service = service; }
//    → Spring auto-injects single constructor (no @Autowired)
//
// 📐 SOLID:
//
//    SRP (Single Responsibility):
//       Controller ONE job — HTTP routing
//       Business logic UserService mein, DB UserRepository mein
//
//    DIP (Dependency Inversion):
//       Depends on UserService (Spring bean)
//       Not directly on UserRepository
//
//    OCP (Open/Closed):
//       Naye endpoint add easy (e.g., login later)
//       Existing endpoint behavior change = modify service, not here
//
// 🎤 INTERVIEW LINE:
//    "UserController exposes 3 REST endpoints for user lifecycle —
//     create, get, update. SRP keeps it purely HTTP-focused, with
//     UserService handling business logic. Constructor injection
//     ensures testability. @Valid triggers Bean Validation on
//     incoming JSON, returning 400 on invalid input automatically."
// ═══════════════════════════════════════════════════════════════════════

@RestController
@RequestMapping("/users")
public class UserController {

    // ─── SERVICE INJECTION ─────────────────────────────────────
    private final UserService service;

    public UserController(UserService service) {
        this.service = service;
    }

    // ─── POST /users — CREATE ──────────────────────────────────
    // Register-like — naya user save
    // @Valid → User class ke validation rules apply (NotBlank, Email, Size)
    // 200 OK + saved user (with auto-generated id)
    @PostMapping
    public ResponseEntity<User> create(@Valid @RequestBody User user) {
        return ResponseEntity.ok(service.register(user));
    }

    // ─── GET /users/{id} — READ ONE ────────────────────────────
    @GetMapping("/{id}")
    public ResponseEntity<User> get(@PathVariable Long id) {
        return ResponseEntity.ok(service.getById(id));
    }

    // ─── PUT /users/{id} — UPDATE ──────────────────────────────
    @PutMapping("/{id}")
    public ResponseEntity<User> update(@PathVariable Long id, @Valid @RequestBody User user) {
        return ResponseEntity.ok(service.update(id, user));
    }
}
