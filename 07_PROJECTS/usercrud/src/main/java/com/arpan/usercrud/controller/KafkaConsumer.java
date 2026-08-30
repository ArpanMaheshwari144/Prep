package com.arpan.usercrud.controller;

import org.springframework.kafka.annotation.KafkaListener;
import org.springframework.stereotype.Component;

@Component
public class KafkaConsumer {

    // Ye method topic pe naya message aate hi APNE-AAP chalta (Spring background me sunta rehta)
    @KafkaListener(topics = "user-events", groupId = "usercrud-group")
    public void listen(String message) {
        System.out.println(">>> CONSUMED from user-events: " + message);

        if (message.contains("fail")) {
            throw new RuntimeException("Poison message! Cannot process: " + message);
        }
    }

    // DLT (dead-letter) topic pe jo message gira, use yahan pakdo
    @KafkaListener(topics = "user-events-dlt", groupId = "usercrud-dlt-group")
    public void listenDLT(String message) {
        System.out.println("XXX DEAD-LETTER (DLT) me gira: " + message);
    }
}
