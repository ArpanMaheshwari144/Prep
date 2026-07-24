package com.arpan.paymentservice;

import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

/**
 * PaymentController — REST entry. POST /pay -> PaymentService.pay() call karta.
 * (order-service Feign se ise call karta; body me { orderId, amount }.)
 */
@RestController
public class PaymentController {

    private final PaymentService service;   // final = ek baar inject, badlega nahi

    public PaymentController(PaymentService service) {
        this.service = service;
    }

    @PostMapping("/pay")
    public String pay(@RequestBody PayRequest req) {
        // ★ req.orderId() / req.amount() -- "get" BINA. KYUN? PayRequest ab record hai -> accessor getOrderId() se orderId() ban gaya.
        //   (purana class-version: req.getOrderId(), req.getAmount())
        Payment saved = service.pay(req.orderId(), req.amount());   // service ko values do, poora req nahi

        return "Payment DONE for order " + saved.getOrderId()
               + " (amount " + saved.getAmount() + "), paymentId=" + saved.getId();
    }
}
