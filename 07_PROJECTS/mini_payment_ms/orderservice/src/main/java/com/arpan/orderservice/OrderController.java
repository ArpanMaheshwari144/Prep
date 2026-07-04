package com.arpan.orderservice;

import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class OrderController {

    private final OrderService service;

    public OrderController(OrderService service) {
        this.service = service;
    }

    @PostMapping("/order")
    public String createOrder(@RequestBody OrderRequest req) {
        Order saved = service.createOrder(req.getItem(), req.getAmount());
        return "Order placed: " + saved.getItem()
               + " (orderId=" + saved.getId() + ", status=" + saved.getStatus() + ")";
    }
}
