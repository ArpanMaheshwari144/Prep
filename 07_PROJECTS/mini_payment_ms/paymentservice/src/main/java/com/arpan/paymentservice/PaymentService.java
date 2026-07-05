package com.arpan.paymentservice;

import org.springframework.kafka.core.KafkaTemplate;
import org.springframework.stereotype.Service;

@Service
public class PaymentService {

    private final PaymentRepository repo; // DB access ke liye
    private final KafkaTemplate<String, String> kafka;

    public PaymentService(PaymentRepository repo, KafkaTemplate<String, String> kafka) { // constructor injection
        this.repo = repo;
        this.kafka = kafka;
    }

    public Payment pay(String orderId, double amount) {
        Payment p = new Payment(orderId, amount, "DONE"); // orderId+amount+status set karo
        Payment savedEntity = repo.save(p); // DB me save -> id ke saath wapas
        kafka.send("payment-done", "Payment DONE for order : " + orderId + " amount :" + amount + "");
        return savedEntity;
    }
}
