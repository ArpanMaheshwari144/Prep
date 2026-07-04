package com.arpan.paymentservice;

import org.springframework.stereotype.Service;

@Service
public class PaymentService {

    private final PaymentRepository repo;   // DB access ke liye

    public PaymentService(PaymentRepository repo) {   // constructor injection
        this.repo = repo;
    }

    public Payment pay(String orderId, double amount) {
        Payment p = new Payment(orderId, amount, "DONE");   // orderId+amount+status set karo
        return repo.save(p);   // DB me save -> id ke saath wapas
    }
}
