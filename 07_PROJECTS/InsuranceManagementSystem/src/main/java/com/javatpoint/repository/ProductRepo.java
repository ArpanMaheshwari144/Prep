package com.javatpoint.repository;

import org.springframework.data.jpa.repository.JpaRepository;

import com.javatpoint.model.Product;

// REPOSITORY: product ka DB-access, sirf CRUD (koi custom query nahi).
// JpaRepository = CrudRepository + paging/sorting extra (findAll(Pageable), batch ops).
public interface ProductRepo extends JpaRepository<Product, Integer>{

}
