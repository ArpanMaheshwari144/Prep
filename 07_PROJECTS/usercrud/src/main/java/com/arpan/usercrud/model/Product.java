package com.arpan.usercrud.model;

import jakarta.persistence.*;
import lombok.Data;

@Entity
@Data
public class Product {
    @Id @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;
    private String name;
    private int stock;

    // JPA is column ko khud manage karta har update pe version auto +1.
    // Update ke waqt DB-version se match karta —
    // mismatch (koi beech me badal gaya) -> OptimisticLockException.
    @Version
    private Long version;
}