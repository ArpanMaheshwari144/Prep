package com.arpan.todoapp.controller;
// ─── PACKAGE ─────────────────────────────────────────────────
// File: src/main/java/com/arpan/todoapp/controller/TodoController.java

import com.arpan.todoapp.model.Todo;
import com.arpan.todoapp.service.TodoService;
import jakarta.validation.Valid;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
// ─── IMPORTS ─────────────────────────────────────────────────
// Todo, TodoService     → entity + business logic
// @Valid                → trigger Bean Validation on request body
// ResponseEntity        → HTTP response wrapper (status + body)
// @RestController, etc. → REST mapping annotations
// List                  → return type for getAll

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    REST API LAYER for Todo
//    HTTP entry point — sirf routing + status codes
//    Business logic NAHI — TodoService ka kaam
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL — TodoController endpoints:
//
//    POST   /todos        → CREATE
//    GET    /todos        → READ ALL
//    GET    /todos/{id}   → READ ONE
//    PUT    /todos/{id}   → UPDATE
//    DELETE /todos/{id}   → DELETE
//
// KEY ANNOTATIONS:
//
//    @RestController
//       = @Controller + @ResponseBody combo
//       → Method return value AUTO JSON serialize
//       → REST APIs ka standard
//
//    @RequestMapping("/todos")
//       → Class-level base path
//       → Saare endpoints "/todos" prefix
//
//    HTTP method annotations:
//       @GetMapping     → READ      (idempotent, safe)
//       @PostMapping    → CREATE    (NOT idempotent)
//       @PutMapping     → UPDATE    (idempotent — full replace)
//       @DeleteMapping  → DELETE    (idempotent)
//
//    Parameter binding:
//       @PathVariable   → URL ke part se
//                          /todos/5 → Long id = 5
//
//       @RequestBody    → JSON body se
//                          POST {"title":"X"} → Todo object
//
//       @Valid          → trigger validation on @RequestBody
//                          Invalid input = 400 auto
//
// ResponseEntity — HTTP RESPONSE CONTROL:
//    ResponseEntity.ok(data)              → 200 + body
//    ResponseEntity.status(404).body(...) → 404 + body
//    ResponseEntity.noContent().build()   → 204 (DELETE)
//    ResponseEntity.created(uri).build()  → 201 (POST)
//
// CONSTRUCTOR INJECTION:
//    private final TodoService service;
//    public TodoController(TodoService service) {
//        this.service = service;
//    }
//    Same pattern as service layer
//
// SOLID:
//
//    SRP (Single Responsibility):
//       Controller ONE job = HTTP routing
//       No business logic here
//       No DB calls here
//       Receive HTTP → call service → return response
//
//    DIP (Dependency Inversion):
//       Depend on TodoService (interface conceptually)
//       Not directly on Repository
//       = Layered separation
//
//    OCP (Open/Closed):
//       New endpoints add → existing unchanged
//       Existing endpoint logic change → modify service, not controller
//
// INTERVIEW LINE:
//    "TodoController is the REST API layer — @RestController combines
//     @Controller + @ResponseBody for automatic JSON serialization.
//     @Valid triggers Bean Validation on @RequestBody. ResponseEntity
//     gives full HTTP response control. SRP: only HTTP routing,
//     business logic delegated to TodoService."
// ═══════════════════════════════════════════════════════════════════════

@RestController
@RequestMapping("/todos")
public class TodoController {

    // ─── SERVICE INJECTION ─────────────────────────────────────
    // Constructor injection (Spring auto-wires)
    private final TodoService service;

    public TodoController(TodoService service) {
        this.service = service;
    }

    // ─── POST /todos — CREATE ──────────────────────────────────
    // @Valid → request body Todo class ke constraints check karega
    // 200 OK + saved todo (with auto-generated id)
    @PostMapping
    public ResponseEntity<Todo> create(@Valid @RequestBody Todo todo) {
        return ResponseEntity.ok(service.create(todo));
    }

    // ─── GET /todos/{id} — READ ONE ────────────────────────────
    @GetMapping("/{id}")
    public ResponseEntity<Todo> get(@PathVariable Long id) {
        return ResponseEntity.ok(service.getById(id));
    }

    // ─── GET /todos — READ ALL ─────────────────────────────────
    @GetMapping
    public ResponseEntity<List<Todo>> getAll() {
        return ResponseEntity.ok(service.getAll());
    }

    // ─── PUT /todos/{id} — UPDATE ──────────────────────────────
    // @Valid sirf body pe, PathVariable pe nahi
    @PutMapping("/{id}")
    public ResponseEntity<Todo> update(@PathVariable Long id, @Valid @RequestBody Todo todo) {
        return ResponseEntity.ok(service.update(id, todo));
    }

    // ─── DELETE /todos/{id} — DELETE ───────────────────────────
    // 204 No Content — REST convention for successful deletion
    @DeleteMapping("/{id}")
    public ResponseEntity<Void> delete(@PathVariable Long id) {
        service.delete(id);
        return ResponseEntity.noContent().build();
    }
}
