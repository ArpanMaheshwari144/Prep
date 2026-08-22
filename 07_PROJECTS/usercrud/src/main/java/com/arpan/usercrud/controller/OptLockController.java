package com.arpan.usercrud.controller;

import com.arpan.usercrud.model.Product;
import com.arpan.usercrud.repository.ProductRepository;
import org.springframework.orm.ObjectOptimisticLockingFailureException;
import org.springframework.web.bind.annotation.*;

@RestController
public class OptLockController {

    // Constructor Injcetion
    private final ProductRepository repo;

    public OptLockController(ProductRepository repo) {
        this.repo = repo;
    }

    @GetMapping("/product/{id}")
    public Product get(@PathVariable Long id) {
        return repo.findById(id).orElse(null);
    }

    @PutMapping("/product/{id}")
    public String update(@PathVariable Long id, @RequestParam int stock, @RequestParam Long version) {
        Product p = new Product();
        p.setId(id);
        p.setName("Arpan");
        p.setStock(stock);
        p.setVersion(version);
        try {
            repo.save(p);
            return "OK updated -> stock=" + stock;
        }
        catch (ObjectOptimisticLockingFailureException e) {
            return "409 CONFLICT: version STALE! kisi aur ne beech me update kar diya. Dobara padho + retry.";
        }
    }
}
