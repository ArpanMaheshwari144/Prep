package com.arpan.orderservice;

import java.util.HashMap;
import java.util.Map;

import org.springframework.stereotype.Service;
import org.springframework.web.client.RestTemplate;

@Service
public class OrderService {

    private final OrderRepository repo;

    public OrderService(OrderRepository repo) {
        this.repo = repo;
    }

    public Order createOrder(String item, double amount) {
        Order order = repo.save(new Order(item, amount, "CREATED"));

        // Ye dursi service ko call karne ke liye(MS)
        RestTemplate rest = new RestTemplate();
        Map<String, Object> body = new HashMap<>();
        body.put("orderId", order.getId().toString());
        body.put("amount", amount);
        String res = rest.postForObject("http://localhost:8082/pay", body, String.class);
        System.out.println(res);
        order.setStatus("PAID");

        return repo.save(order);
    }
}
