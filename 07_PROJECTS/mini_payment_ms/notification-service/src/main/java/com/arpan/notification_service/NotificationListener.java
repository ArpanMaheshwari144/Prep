package com.arpan.notification_service;

import org.springframework.kafka.annotation.KafkaListener;
import org.springframework.stereotype.Component;

@Component
public class NotificationListener {

    @KafkaListener(topics = "payment-done", groupId = "notification-group")
    public void onPaymentDone(String message) {
        System.out.println("Email send -> " + message);
    }
}
