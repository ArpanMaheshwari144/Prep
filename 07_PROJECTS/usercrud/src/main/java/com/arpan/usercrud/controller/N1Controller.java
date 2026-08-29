package com.arpan.usercrud.controller;

import com.arpan.usercrud.model.Author;
import com.arpan.usercrud.repository.AuthorRepository;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
public class N1Controller {
    private final AuthorRepository authorRepo;

    public N1Controller(AuthorRepository authorRepo) {
        this.authorRepo = authorRepo;
    }

    @GetMapping("n1/bad")
    public int bad() {
        List<Author> authors = authorRepo.findAll();
        int count = 0;
        for (Author author : authors) {
            count += author.getBooks().size();
        }
        return count;
    }

    @GetMapping("n1/good")
    public int good() {
        List<Author> authors = authorRepo.findAllWithBooks();
        int count = 0;
        for (Author author : authors) {
            count += author.getBooks().size();
        }
        return count;
    }
}
