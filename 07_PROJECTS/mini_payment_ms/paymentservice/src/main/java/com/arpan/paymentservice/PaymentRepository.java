package com.arpan.paymentservice;

import org.springframework.data.jpa.repository.JpaRepository;

public interface PaymentRepository extends JpaRepository<Payment, Long> {
    // khaali — JpaRepository se save(), findAll(), findById() free milte
}
