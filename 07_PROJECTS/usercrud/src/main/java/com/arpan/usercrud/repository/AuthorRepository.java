package com.arpan.usercrud.repository;

import com.arpan.usercrud.model.Author;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;

import java.util.List;

public interface AuthorRepository extends JpaRepository<Author, Long> {
    @Query("Select Distinct a from Author a left join fetch a.books")
    List<Author> findAllWithBooks();
}
