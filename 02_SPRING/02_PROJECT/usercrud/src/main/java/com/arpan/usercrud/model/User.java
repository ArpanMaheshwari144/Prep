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

/* ════════════════════════════════════════════════════════════════
 *  📌 ENTITY — Database Table ka Java Representation
 * ════════════════════════════════════════════════════════════════
 *  Yeh class JPA entity hai — har object = ek row in `users` table.
 *  Spring Data JPA + Hibernate is class ko padhke automatic table
 *  bana deta hai (kyunki application.properties mein
 *  spring.jpa.hibernate.ddl-auto=update set hai).
 *
 *  ─── ANNOTATIONS BREAKDOWN (interview puchega) ──────────────────
 *
 *  ── JPA / Hibernate ──
 *  @Entity          → "Hibernate, yeh class ko table samjho"
 *  @Table(name="..")→ Table ka naam custom (default: class ka naam)
 *                    "user" reserved word hai SQL mein, isliye "users"
 *  @Id              → Primary key field
 *  @GeneratedValue  → Auto-increment ID (DB se generate hoga)
 *    strategy = IDENTITY → DB ka auto-increment use kar
 *    (alternatives: AUTO, SEQUENCE, TABLE)
 *  @Column          → Column-level constraints
 *    nullable=false → NOT NULL constraint
 *    unique=true    → UNIQUE constraint (duplicate email rokega)
 *
 *  ── Bean Validation (jakarta.validation) ──
 *  @NotBlank   → null + empty + whitespace nahi (sirf String ke liye)
 *  @Size       → Length range (String/Collection)
 *  @Email      → Valid email format
 *  @Min/@Max   → Numeric range
 *
 *  Yeh @Valid ke saath controller mein trigger hote hain. Fail hua
 *  toh MethodArgumentNotValidException throw hota hai → 400.
 *
 *  ── Lombok (boilerplate killer) ──
 *  @Data              → getters + setters + toString + equals + hashCode
 *  @NoArgsConstructor → empty constructor (JPA ko CHAHIYE — proxy banane ke liye)
 *  @AllArgsConstructor→ saare fields wala constructor
 *
 *  ─── ENTITY vs DTO (concept clarity) ────────────────────────────
 *  Entity = DB se mapped class (yeh wala)
 *  DTO    = API ke liye data shape (jaise ErrorResponse)
 *  Production mein dono alag rakhte hain — security + flexibility.
 *  Yahaan simple project hai, isliye User dono jagah use hua.
 * ════════════════════════════════════════════════════════════════
 */
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
}
