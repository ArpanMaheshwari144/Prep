package com.arpan.orderservice;

import java.util.Map;

import org.springframework.cloud.openfeign.FeignClient;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;

/**
 * PaymentClient — Feign DECLARATIVE client. sirf interface likha; Spring khud HTTP call banata.
 * order-service (CALLER) me rehta, payment-service (CALLEE) ko point karta -> service boundary (class share nahi, JSON se baat).
 */
@FeignClient(name = "payment-service", url = "http://localhost:8082") // name=label, url=kahan call
public interface PaymentClient {
    // remote endpoint ka contract MIRROR karo -> Spring inhi annotations se request banata. koi body nahi -> Spring implement karega.
    @PostMapping("/pay")
    String pay(@RequestBody Map<String, Object> body);
}
