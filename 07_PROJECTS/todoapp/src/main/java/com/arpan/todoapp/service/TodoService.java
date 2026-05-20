package com.arpan.todoapp.service;
// ─── PACKAGE ─────────────────────────────────────────────────
// File: src/main/java/com/arpan/todoapp/service/TodoService.java

import java.util.List;

import org.springframework.cache.annotation.CacheEvict;
import org.springframework.cache.annotation.Cacheable;
import org.springframework.cache.annotation.Caching;
import org.springframework.stereotype.Service;

import com.arpan.todoapp.model.Todo;
import com.arpan.todoapp.repository.TodoRepository;
// ─── IMPORTS ─────────────────────────────────────────────────
// Todo, TodoRepository → entity + DAO
// @Service              → Spring stereotype for service layer
// List                  → return type for getAll

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    SERVICE LAYER for Todo
//    Business logic yaha — Controller iske through DB hit karta
//    Controller (HTTP) → Service (logic) → Repository (DB)
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL — LAYERED ARCHITECTURE:
//
//    [Browser/Postman]
//          │ HTTP
//          ▼
//    ┌──────────────────┐
//    │  TodoController  │  ← HTTP layer
//    │  (@RestController)│    (endpoints, status codes)
//    └────────┬─────────┘
//             │ calls service
//             ▼
//    ┌──────────────────┐
//    │  TodoService     │  ← BUSINESS LOGIC (TU YAHAN)
//    │  (@Service)      │    (rules, orchestration)
//    └────────┬─────────┘
//             │ calls repo
//             ▼
//    ┌──────────────────┐
//    │  TodoRepository  │  ← DATA layer
//    │  (@Repository)   │    (DB queries)
//    └────────┬─────────┘
//             ▼
//          MySQL DB
//
// @Service ANNOTATION:
//    Stereotype annotation = Spring "yeh service layer hai"
//    Functionally = @Component (bean register hota)
//    Why separate? CODE INTENT clear (architecture layer marker)
//
// CONSTRUCTOR INJECTION:
//
//    Yaha TodoRepository inject karte:
//
//       private final TodoRepository repo;
//
//       public TodoService(TodoRepository repo) {
//           this.repo = repo;
//       }
//
//    Spring magic:
//       Single constructor + Spring sees → auto-injects
//       (No @Autowired needed since Spring 4.3+)
//
//    Why CONSTRUCTOR (not @Autowired field)?
//       final field = immutable dependency
//       Easy to test (manual injection in tests)
//       Fail-fast (NullPointerException at startup, not runtime)
//       Modern Spring best practice
//
// 5 METHODS — CRUD OPERATIONS:
//
//    create(Todo todo)
//       → New todo save
//       → repo.save() returns saved entity with auto-generated id
//
//    getById(Long id)
//       → Single todo fetch
//       → Optional<Todo> from repo
//       → .orElseThrow() → throw if not found
//
//    getAll()
//       → All todos
//       → repo.findAll() returns List
//
//    update(Long id, Todo updated)
//       → Fetch existing, modify fields, save
//       → Dirty checking handles UPDATE SQL
//
//    delete(Long id)
//       → repo.deleteById()
//       → No return value
//
// .orElseThrow() — NULL SAFETY:
//
//    repo.findById(id) returns Optional<Todo>
//       (Optional empty if not found, no NPE)
//
//    .orElseThrow(() -> new RuntimeException("Todo not found"))
//       → Found → unwrap Todo
//       → Not found → throw exception
//
//    Later: replace RuntimeException with TodoNotFoundException
//    + GlobalExceptionHandler → 404 response
//
// SOLID:
//    SRP — Service has ONE job (Todo business logic)
//    DIP — Depends on REPOSITORY interface (proxy auto)
//
// INTERVIEW LINE:
//    "TodoService is the business logic layer between
//     Controller and Repository. @Service marks it as a
//     Spring-managed bean. Constructor injection ensures
//     immutable dependencies and easy testability.
//     Uses Optional from repository for null safety."
// ═══════════════════════════════════════════════════════════════════════
@Service
public class TodoService {

    // ─── REPOSITORY INJECTION ─────────────────────────────────
    // final = immutable after constructor
    // Spring constructor injection (no @Autowired needed)
    private final TodoRepository repo;

    public TodoService(TodoRepository repo) {
        this.repo = repo;
    }

    // ─── CREATE ────────────────────────────────────────────────
    // Flow:
    //   1. DB mein naya todo save
    //   2. "todos" list cache se wipe (purani list ab stale)
    @CacheEvict(value = "todos", allEntries = true)
    public Todo create(Todo todo) {
        return repo.save(todo);
    }

    // ─── GET ONE ───────────────────────────────────────────────
    // Flow:
    //   Pehli call (id=5):  DB hit → Redis mein "todo::5" store
    //   Agli call (id=5):   Cache hit → Redis se direct (DB skip)
    //   5 min baad:         TTL expire → next call DB hit again
    @Cacheable(value = "todo", key = "#id")
    public Todo getById(Long id) {
        return repo.findById(id)
                .orElseThrow(() -> new RuntimeException("Todo not found: " + id));
    }

    // ─── GET ALL ───────────────────────────────────────────────
    // Flow:
    //   Pehli call: DB findAll → "todos" cache mein store
    //   Agli call:  Cache hit → Redis se direct (DB skip)
    @Cacheable(value = "todos")
    public List<Todo> getAll() {
        return repo.findAll();
    }

    // ─── UPDATE ────────────────────────────────────────────────
    // Flow:
    //   1. DB mein todo update
    //   2. "todo::{id}" cache wipe (specific entry stale)
    //   3. "todos" list cache wipe (list bhi affected)
    //   4. Agli read = cache miss → DB se fresh fetch
    @Caching(evict = {
        @CacheEvict(value = "todo", key = "#id"),
        @CacheEvict(value = "todos", allEntries = true)
    })
    public Todo update(Long id, Todo updated) {
        Todo existing = getById(id);  // throws if not found
        existing.setTitle(updated.getTitle());
        existing.setDescription(updated.getDescription());
        existing.setUserId(updated.getUserId());
        return repo.save(existing);
    }

    // ─── DELETE ────────────────────────────────────────────────
    // Flow:
    //   1. DB se todo delete
    //   2. "todo::{id}" cache wipe
    //   3. "todos" list cache wipe
    @Caching(evict = {
        @CacheEvict(value = "todo", key = "#id"),
        @CacheEvict(value = "todos", allEntries = true)
    })
    public void delete(Long id) {
        repo.deleteById(id);
    }
}
