package com.arpan.notification_service;

import org.springframework.kafka.annotation.KafkaListener;
import org.springframework.stereotype.Component;

/**
 * NotificationListener — Kafka CONSUMER. "payment-done" topic ko sunta hai.
 * message aate hi Spring khud onPaymentDone() bulaata (no loop/polling) -> "email bheja" print.
 */
@Component
public class NotificationListener {

    // @KafkaListener -> is topic ka har naya message -> ye method chalega. groupId = consumer-group (offset track + scale/broadcast).
    @KafkaListener(topics = "payment-done", groupId = "notification-group")
    public void onPaymentDone(String message) {
        System.out.println("Email send -> " + message); // asli app me yahan email/SMS jaata
    }
}
