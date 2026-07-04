package com.arpan.paymentservice;

import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class PaymentController {

    private final PaymentService service;   // final = ek baar inject, badlega nahi

    public PaymentController(PaymentService service) {
        this.service = service;
    }

    @PostMapping("/pay")
    public String pay(@RequestBody PayRequest req) {
        Payment saved = service.pay(req.getOrderId(), req.getAmount());   // service ko values do, poora req nahi
        return "Payment DONE for order " + saved.getOrderId()
               + " (amount " + saved.getAmount() + "), paymentId=" + saved.getId();
    }
}
