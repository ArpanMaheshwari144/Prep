package com.arpan.usercrud.model;

import jakarta.persistence.*;
import lombok.Data;

@Entity
@Data
public class Book {
    @Id @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;
    private String title;

    // kai books -> 1 author (FK author_id)
    @ManyToOne
    @JoinColumn(name = "author_id")
    private Author author;
}