package com.arpan.usercrud.controller;

import org.springframework.kafka.annotation.KafkaListener;
import org.springframework.messaging.handler.annotation.Payload;
import org.springframework.stereotype.Component;

@Component
public class KafkaConsumer {

    // Ye method topic pe naya message aate hi APNE-AAP chalta (Spring background me sunta rehta)
    @KafkaListener(topics = "user-events", groupId = "usercrud-group")
    public void listen(String message) {
        System.out.println(">>> CONSUMED from user-events: " + message);
    }
}
