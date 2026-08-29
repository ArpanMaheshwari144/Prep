package com.arpan.usercrud.controller;

import com.arpan.usercrud.model.Author;
import com.arpan.usercrud.model.Book;
import com.arpan.usercrud.repository.AuthorRepository;
import com.arpan.usercrud.repository.BookRepository;
import org.springframework.boot.CommandLineRunner;
import org.springframework.stereotype.Component;

@Component
public class N1Seeder implements CommandLineRunner {

    private final AuthorRepository authorRepo;
    private final BookRepository bookRepo;

    public N1Seeder(AuthorRepository authorRepo, BookRepository bookRepo) {
        this.authorRepo = authorRepo;
        this.bookRepo = bookRepo;
    }


    // ye method program run hua and turant chal gaya
    // books and author db mei save kar diya bus or kush nahi kiya yaha
    @Override
    public void run(String... args) throws Exception {
        if (authorRepo.count() > 0) return;   // pehle se hai to skip
        for (int i = 1; i <= 5; i++) {
            Author a = new Author();
            a.setName("Author" + i);
            authorRepo.save(a);
            for (int j = 1; j <= 3; j++) {
                Book b = new Book();
                b.setTitle("Book" + i + "-" + j);
                b.setAuthor(a);
                bookRepo.save(b);
            }
        }
    }
}
