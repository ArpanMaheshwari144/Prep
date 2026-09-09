package com.arpan.usercrud.controller;

import com.arpan.usercrud.model.Author;
import com.arpan.usercrud.repository.AuthorRepository;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
public class LazyDemoController {

    private final AuthorRepository authorRepo;

    public LazyDemoController(AuthorRepository authorRepo) {
        this.authorRepo = authorRepo;
    }

    @GetMapping("/lazy/fail")
    public String fail() {
        Author author = authorRepo.findById(1L).orElseThrow();
        int n = author.getBooks().size();
        return "books = " + n;

    }


    @Transactional
    @GetMapping("/lazy/fix-txn")
    public String fixTxn() {
        Author author = authorRepo.findById(1L).orElseThrow();
        int n = author.getBooks().size();
        return "books = " + n;
    }

    @GetMapping("/lazy/fix-fetch")
    public String fixFetch() {
        List<Author> authors = authorRepo.findAllWithBooks();
        return "authors = " + authors.size() + ", first author books = " + authors.get(0).getBooks().size();

    }
}
