package com.arpan.paymentservice;

import org.springframework.kafka.core.KafkaTemplate;
import org.springframework.stereotype.Service;

/**
 * PaymentService — payment save karta + Kafka par event bhejta (PRODUCER).
 * FLOW: payment DB me save -> "payment-done" topic par event (ASYNC) -> aage badh gaya (notification wait nahi).
 */
@Service
public class PaymentService {

    private final PaymentRepository repo; // DB access ke liye (paydb)
    private final KafkaTemplate<String, String> kafka; // Kafka PRODUCER -> topic par message bhejne ka tool

    public PaymentService(PaymentRepository repo, KafkaTemplate<String, String> kafka) { // constructor injection
        this.repo = repo;
        this.kafka = kafka;
    }

    public Payment pay(String orderId, double amount) {
        Payment p = new Payment(orderId, amount, "DONE"); // orderId+amount+status set karo
        Payment savedEntity = repo.save(p); // DB me save -> id ke saath wapas
        // ★ Kafka event bhejo -> "payment-done" topic (ASYNC, fire-and-forget). notification-service ise sunta hai.
        kafka.send("payment-done", "Payment DONE for order : " + orderId + " amount :" + amount + "");
        return savedEntity;
    }
}
