package com.arpan.orderservice;

import java.util.Map;

import org.springframework.cloud.openfeign.FeignClient;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;

@FeignClient(name = "payment-service", url = "http://localhost:8082")
public interface PaymentClient {
    @PostMapping("/pay")
    String pay(@RequestBody Map<String, Object> body);
}
