package com.arpan.todoapp.controller;
// ─── PACKAGE ─────────────────────────────────────────────────
// File: src/main/java/com/arpan/todoapp/controller/UserController.java

import com.arpan.todoapp.dto.UserResponse;
import com.arpan.todoapp.model.User;
import com.arpan.todoapp.service.UserService;
import jakarta.validation.Valid;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

// ─── IMPORTS ─────────────────────────────────────────────────
// User, UserService     → entity + business logic
// UserResponse          → response DTO (password strip — entity leak na ho)
// @Valid                → trigger Bean Validation
// ResponseEntity        → HTTP response wrapper
// @RestController, etc. → REST mapping annotations

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
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
//    NO delete (tune nahi mangya)
//    NO get-all (tune nahi mangya)
//    NO login flow yet (JWT layer ke saath aayega)
//
// KEY ANNOTATIONS (same as TodoController):
//
//    @RestController        → @Controller + @ResponseBody (auto JSON)
//    @RequestMapping("/users") → class-level base path
//    @PostMapping, @GetMapping, @PutMapping → HTTP verbs
//    @PathVariable          → URL part binding
//    @RequestBody + @Valid  → JSON body + validation trigger
//
// SERVICE INJECTION (constructor):
//    private final UserService service;
//    public UserController(UserService service) { this.service = service; }
//    → Spring auto-injects single constructor (no @Autowired)
//
// SOLID:
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
// INTERVIEW LINE:
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

    // ═══ RETURN = UserResponse (NOT User entity) — teeno endpoint mein ═══
    //    KYUN: service.register()/getById()/update() -> User deta (password ke SAATH)
    //          User seedha return karte -> password (hash) response me LEAK
    //    FIX:  UserResponse.from(user) -> sirf id/name/email map (password chhoda)
    //          -> API response me password KABHI nahi jaata (entity internal-only)
    //    from() = static factory (mapping ek jagah, DRY)
    // ═══════════════════════════════════════════════════════════════════

    // ─── POST /users — CREATE ──────────────────────────────────
    // Register-like — naya user save
    // @Valid → User class ke validation rules apply (NotBlank, Email, Size)
    // register() = password BCrypt-hash karke save
    @PostMapping
    public ResponseEntity<UserResponse> create(@Valid @RequestBody User user) {
        return ResponseEntity.ok(UserResponse.from(service.register(user)));  // entity -> DTO (password strip)
    }

    // ─── GET /users/{id} — READ ONE ────────────────────────────
    @GetMapping("/{id}")
    public ResponseEntity<UserResponse> get(@PathVariable Long id) {
        return ResponseEntity.ok(UserResponse.from(service.getById(id)));
    }

    // ─── PUT /users/{id} — UPDATE ──────────────────────────────
    @PutMapping("/{id}")
    public ResponseEntity<UserResponse> update(@PathVariable Long id, @Valid @RequestBody User user) {
        return ResponseEntity.ok(UserResponse.from(service.update(id, user)));
    }
}
