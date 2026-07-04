package com.arpan.orderservice;

import org.springframework.data.jpa.repository.JpaRepository;

public interface OrderRepository extends JpaRepository<Order, Long> {
    // JpaRepository se save(), findAll(), findById() free milte
}
