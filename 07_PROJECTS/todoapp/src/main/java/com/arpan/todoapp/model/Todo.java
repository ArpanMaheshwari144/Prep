package com.arpan.todoapp.model;
// ─── PACKAGE ─────────────────────────────────────────────────
// File path: src/main/java/com/arpan/todoapp/model/Todo.java

import jakarta.persistence.*;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Size;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
// ─── IMPORTS ─────────────────────────────────────────────────
// jakarta.persistence.* → JPA (@Entity, @Id, @Column, etc.)
// jakarta.validation    → Bean Validation (@NotBlank, @NotNull, @Size)
// lombok.*              → Boilerplate killers (@Data, constructors)

// ═══════════════════════════════════════════════════════════════════════
// YE FILE KYA HAI:
//    Todo = ENTITY class
//    = Java class jo "todos" DB table represent karti
//    = Har Todo object = 1 row in MySQL
//    = Hibernate Java ↔ DB ka bridge banata
// ═══════════════════════════════════════════════════════════════════════
//
// VISUAL — JAVA ↔ MYSQL MAPPING:
//
//    JAVA CLASS:                  MYSQL TABLE:
//
//    @Entity                       CREATE TABLE todos (
//    class Todo {                    id          BIGINT PK AUTO_INCREMENT,
//        Long id;                     title       VARCHAR(200) NOT NULL,
//        String title;                description VARCHAR(1000),
//        String description;          user_id     BIGINT NOT NULL
//        Long userId;              );
//    }
//         │                              ▲
//         └──── Hibernate translates ────┘
//
// KEY ANNOTATIONS:
//
//    @Entity              → "Yeh class DB table mapped hai"
//    @Table(name="todos") → DB table name override (plural convention)
//    @Data (Lombok)       → getters/setters/toString/equals AUTO
//    @NoArgsConstructor   → public Todo() {} auto (JPA needs)
//    @AllArgsConstructor  → constructor with all fields auto
//
//    @Id                          → PRIMARY KEY marker
//    @GeneratedValue(IDENTITY)    → MySQL AUTO_INCREMENT
//
//    @NotBlank   → null + empty + whitespace REJECT (Strings)
//    @NotNull    → just null check (any type)
//    @Size       → length range
//
//    @Column     → DB column customization (name, nullable, length)
//
// WHY userId NOT @ManyToOne User?
//    @ManyToOne would map object relation (cleaner OO)
//    BUT keeping simple: store userId as Long only
//    = Less complexity, easier serialization
//    = Trade-off: lose lazy User object fetch
//
// INTERVIEW LINE:
//    "Todo is a JPA entity mapping to 'todos' MySQL table.
//     @Id with @GeneratedValue(IDENTITY) uses MySQL auto-increment.
//     Validation annotations enforce input constraints when used
//     with @Valid in controllers. Lombok @Data removes
//     getter/setter boilerplate."
// ═══════════════════════════════════════════════════════════════════════

@Entity
@Table(name = "todos")
@Data
@NoArgsConstructor
@AllArgsConstructor
public class Todo {

    // ─── PRIMARY KEY ──────────────────────────────────────────
    // @Id → marks as PK
    // @GeneratedValue(IDENTITY) → MySQL AUTO_INCREMENT
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    // ─── TITLE (HEADING) ───────────────────────────────────────
    // Required field — todo ka asli content
    // @NotBlank: null/empty/whitespace REJECT
    // @Size: max 200 chars
    // @Column: DB-level NOT NULL + VARCHAR(200)
    @NotBlank(message = "Title required")
    @Size(max = 200, message = "Title max 200 chars")
    @Column(nullable = false, length = 200)
    private String title;

    // ─── DESCRIPTION (OPTIONAL) ────────────────────────────────
    // Extra detail — optional
    // @Size: max 1000 chars (no NotBlank since optional)
    // @Column: nullable allowed (default), VARCHAR(1000)
    @Size(max = 1000, message = "Description max 1000 chars")
    @Column(length = 1000)
    private String description;

    // ─── USER OWNERSHIP (FK to User) ───────────────────────────
    // Todo kis user ka — userId store karte
    // @NotNull: validation required
    // @Column(name="user_id"): camelCase userId → snake_case user_id
    @NotNull(message = "userId required")
    @Column(name = "user_id", nullable = false)
    private Long userId;
}
