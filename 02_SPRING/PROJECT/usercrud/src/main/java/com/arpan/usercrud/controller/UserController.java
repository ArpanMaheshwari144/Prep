package com.arpan.usercrud.controller;

import java.util.List;

import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.arpan.usercrud.model.User;
import com.arpan.usercrud.service.UserService;

import jakarta.validation.Valid;

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    UserController = REST API LAYER (HTTP Entry Point)
//    Standard CRUD endpoints for User
//    Business logic NAHI — sirf HTTP routing
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL:
//    ┌──────────────────────────────────────────┐
//    │  UserController @ /users/*             │
//    │                                           │
//    │  POST   /users        → CREATE            │
//    │  GET    /users/{id}   → READ ONE          │
//    │  GET    /users        → READ ALL          │
//    │  PUT    /users/{id}   → UPDATE            │
//    │  DELETE /users/{id}   → DELETE            │
//    │                                           │
//    │  POST   /users/demo/rollback → DEMO       │
//    └──────────────────────────────────────────┘
//
//    All protected — JWT mandatory (per SecurityConfig)
//
// @RestController vs @Controller:
//    @Controller       → Traditional MVC, returns view name
//                        (Thymeleaf/JSP page render)
//
//    @RestController   → @Controller + @ResponseBody
//                        → Methods auto JSON return
//                        → REST APIs ke liye DEFAULT
//                        → Modern Spring Boot apps mein yahi
//
// @RequestMapping("/users"):
//    Class-level base path
//    Saare endpoints "/users" se start
//    Sub-paths method level pe (jaise /{id})
//
// HTTP METHOD ANNOTATIONS:
//    @GetMapping     → READ      (idempotent, safe)
//    @PostMapping    → CREATE    (NOT idempotent)
//    @PutMapping     → UPDATE    (idempotent — full replace)
//    @PatchMapping   → PARTIAL UPDATE (idempotent)
//    @DeleteMapping  → DELETE    (idempotent)
//
//    Idempotent = same request 100 baar bhejo, result same
//    POST alag = har baar naya resource banta
//
// PARAMETER BINDING:
//    @PathVariable   → URL ke part se
//                      /users/5  →  Long id = 5
//
//    @RequestBody    → JSON body se
//                      POST {"name":"X"} → User user (deserialize)
//
//    @RequestParam   → Query string se
//                      /users?role=admin  →  String role = "admin"
//
// @Valid — Request Body Validation:
//    @RequestBody ke saath @Valid lagao
//    → Spring User class ke validation annotations check karega
//    → Fail? MethodArgumentNotValidException → 400
//
//    GET aur DELETE mein @Valid nahi — body nahi hai
//
// ResponseEntity — HTTP Response Full Control:
//    • Status code set kar sakte (200, 201, 204, 404...)
//    • Headers add kar sakte
//    • Body set kar sakte
//
//    ResponseEntity.ok(data)              → 200 + body
//    ResponseEntity.status(404).body(...) → 404 + body
//    ResponseEntity.noContent().build()   → 204 (DELETE standard)
//    ResponseEntity.created(uri).build()  → 201 (POST)
//
// SOLID:
//    SRP — Sirf HTTP layer (request/response)
//          Business logic = UserService ka kaam
//          Validation = @Valid
//          Exception → GlobalExceptionHandler
//
//    DIP — UserService inject (constructor injection)
//          Spring abstractions used (ResponseEntity)
//
//    OCP — Naye endpoints add easy, existing unchanged
//
// INTERVIEW LINE:
//    "UserController is REST API layer — @RestController combines
//     @Controller + @ResponseBody for auto JSON.
//     @Valid triggers Bean Validation on @RequestBody.
//     ResponseEntity gives full HTTP response control.
//     Sirf HTTP routing — business logic UserService mein."
// ═══════════════════════════════════════════════════════════════════════

@RestController
@RequestMapping("/users")
public class UserController {

    private final UserService service;

    public UserController(UserService service) {
        this.service = service;
    }

    // ─── POST /users — CREATE ───────────────────────────────────
    // @Valid → request body User class ke constraints check karega
    // 200 OK + saved user (with auto-generated id)
    @PostMapping
    public ResponseEntity<User> create(@Valid @RequestBody User user) {
        return ResponseEntity.ok(service.create(user));
    }

    // ─── GET /users/{id} — READ ONE ─────────────────────────────
    // /users/5  →  id = 5L
    // Found → 200 + user; Not found → exception → 404 (handler se)
    @GetMapping("/{id}")
    public ResponseEntity<User> get(@PathVariable Long id) {
        return ResponseEntity.ok(service.getById(id));
    }

    // ─── GET /users — READ ALL ──────────────────────────────────
    // 200 OK + array of users (empty list bhi 200 hi return karega)
    @GetMapping
    public ResponseEntity<List<User>> getAll() {
        return ResponseEntity.ok(service.getAll());
    }

    // ─── PUT /users/{id} — UPDATE ───────────────────────────────
    // @Valid sirf body pe lagta hai, PathVariable pe nahi
    @PutMapping("/{id}")
    public ResponseEntity<User> update(@PathVariable Long id, @Valid @RequestBody User user) {
        return ResponseEntity.ok(service.update(id, user));
    }

    // ─── DELETE /users/{id} — DELETE ────────────────────────────
    // 204 No Content — REST convention for successful deletion
    // (200 bhi acceptable, 204 cleaner kyunki body nahi)
    @DeleteMapping("/{id}")
    public ResponseEntity<Void> delete(@PathVariable Long id) {
        service.delete(id);
        return ResponseEntity.noContent().build();
    }

    // ═══════════════════════════════════════════════════════════
    //  DEMO ENDPOINT — @Transactional rollback proof
    // ═══════════════════════════════════════════════════════════
    //  Test sequence (Postman / browser):
    //
    //  1. POST /users/demo/rollback
    //     → Console mein dikhega:
    //        Step 1: User saved with ID = 1
    //        Step 2: Simulating failure...
    //     → API response: 500 (RuntimeException → GlobalExceptionHandler)
    //
    //  2. GET /users
    //     → Empty list [] (rolled-back user MISSING) proves rollback
    //
    //  3. H2 console (http://localhost:8080/h2-console) → SELECT * FROM users
    //     → Empty table — DB-level confirmation
    //
    //  ─── To prove WITHOUT @Transactional ───
    //  • UserService.createWithSimulatedFailure() pe se @Transactional hata
    //  • Same endpoint hit kar
    //  • GET /users → user DIKHEGA (no rollback, save committed)
    //  • Wapas @Transactional laga ke verify kar
    @PostMapping("/demo/rollback")
    public ResponseEntity<User> demoRollback() {
        User dummyUser = new User(
            null,                       // id (auto-generate)
            "RollbackTest",             // name
            "rollback-test@x.com",      // email
            "password123",              // password
            25,                         // age
            "USER"                      // role
        );
        // Service throws RuntimeException intentionally
        // → @Transactional rollback
        // → GlobalExceptionHandler converts to 500 response
        return ResponseEntity.ok(service.createWithSimulatedFailure(dummyUser));
    }
}
