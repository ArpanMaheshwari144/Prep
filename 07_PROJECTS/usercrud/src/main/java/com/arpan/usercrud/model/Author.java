package com.arpan.usercrud.model;

import jakarta.persistence.*;
import java.util.List;
import lombok.Data;

@Entity
@Data
public class Author {
    @Id @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;
    private String name;

    // 1 author -> kai books. LAZY (default) = books tabhi load jab access karo -> yehi N+1 ki jad
    @OneToMany(mappedBy = "author")
    private List<Book> books;
}