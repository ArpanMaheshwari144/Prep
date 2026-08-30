package com.arpan.usercrud.controller;

import org.springframework.kafka.annotation.KafkaListener;
import org.springframework.kafka.support.KafkaHeaders;
import org.springframework.messaging.handler.annotation.Header;
import org.springframework.stereotype.Component;

import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

@Component
public class KafkaConsumer {

    // "register" of processed message-ids (demo: in-memory; real world: DB/Redis)
    private final Set<String> processedIds = ConcurrentHashMap.newKeySet();

    // Ye method topic pe naya message aate hi APNE-AAP chalta (Spring background me sunta rehta)
    @KafkaListener(topics = "user-events", groupId = "usercrud-group")
    public void listen(String message, @Header(KafkaHeaders.RECEIVED_PARTITION) int partition) {

        if(processedIds.contains(message)){
            System.out.println("=== DUPLICATE skipped (already processed): " + message);
            return;
        }

        System.out.println(">>> [" + Thread.currentThread().getName() + "]  partition=" + partition
                + "  CONSUMED: " + message);

        if (message.contains("fail")) {
            throw new RuntimeException("Poison message! Cannot process: " + message);
        }

        processedIds.add(message);
    }

    // DLT (dead-letter) topic pe jo message gira, use yahan pakdo
    @KafkaListener(topics = "user-events-dlt", groupId = "usercrud-dlt-group")
    public void listenDLT(String message) {
        System.out.println("XXX DEAD-LETTER (DLT) me gira: " + message);
    }
}
