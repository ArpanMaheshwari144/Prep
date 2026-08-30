package com.arpan.usercrud.config;

import org.apache.kafka.clients.admin.NewTopic;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.kafka.annotation.EnableKafka;
import org.springframework.kafka.core.*;
import org.springframework.kafka.listener.DeadLetterPublishingRecoverer;
import org.springframework.kafka.listener.DefaultErrorHandler;
import org.springframework.util.backoff.FixedBackOff;

import java.util.HashMap;
import java.util.Map;

@Configuration
@EnableKafka   // @KafkaListener ko chalu karta (autoconfig karta tha, ab hum)
public class KafkaConfig {

    // ===== DLQ: error-handler (retry -> phir DLT me bhej do) =====
    @Bean
    public DefaultErrorHandler errorHandler(KafkaTemplate<String, String> kafkaTemplate) {

        // FAIL hone pe kahan bhejna: <topic>.DLT topic (default naam)
        DeadLetterPublishingRecoverer recoverer = new DeadLetterPublishingRecoverer(kafkaTemplate);

        // 2 baar retry, har baar 1 sec gap; phir bhi fail -> recoverer (DLT)
        FixedBackOff backOff = new FixedBackOff(1000L, 2L);

        return new DefaultErrorHandler(recoverer, backOff);

    }

    @Bean
    public NewTopic deadLetterTopic() {
        return new NewTopic("user-events-dlt", 1, (short) 1);   // naam, 1 partition, 1 replica
    }

    @Bean
    public NewTopic userEventsTopic() {
        return new NewTopic("user-events", 3, (short) 1);   // 3 partition, 1 replica
    }
}
