package com.arpan.paymentservice;

import java.util.Optional;

import org.springframework.data.jpa.repository.JpaRepository;

public interface PaymentRepository extends JpaRepository<Payment, Long> {
    // khaali — JpaRepository se save(), findAll(), findById() free milte

    public Optional<Payment> findByOrderId(String orderId);
}
