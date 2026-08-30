package com.arpan.usercrud.controller;

import org.springframework.kafka.core.KafkaTemplate;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class KafkaProducerController {

    private final KafkaTemplate<String, String> kafkaTemplate;
    private static final String TOPIC = "user-events";
    public KafkaProducerController(KafkaTemplate<String, String> kafkaTemplate) {
        this.kafkaTemplate = kafkaTemplate;
    }

    @PostMapping("/kafka/send")
    public String send(@RequestParam String message) {
        kafkaTemplate.send(TOPIC, message, message);
        return "Sent to topic '" + TOPIC + "': " + message;
    }
}
