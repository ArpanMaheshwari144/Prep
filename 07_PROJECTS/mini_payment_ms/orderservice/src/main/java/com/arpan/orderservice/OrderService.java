package com.arpan.orderservice;

import java.util.HashMap;
import java.util.Map;

import org.springframework.stereotype.Service;
// import org.springframework.web.client.RestTemplate;

/**
 * OrderService — order banata + payment-service ko call karta.
 * FLOW: order save (CREATED) -> Feign se payment call (SYNC) -> PASS=PAID / FAIL=FAILED (SAGA compensating).
 */
@Service
public class OrderService {

    private final OrderRepository repo;   // order ka apna DB (orderdb) — save/find

    private final PaymentClient client;   // Feign client -> payment-service (8082) ko call

    // constructor injection — Spring repo + client khud inject karta
    public OrderService(OrderRepository repo, PaymentClient client) {
        this.repo = repo;
        this.client = client;
    }

    public Order createOrder(String item, double amount) {
        // 1. order save karo -> status CREATED, DB se id milti
        Order order = repo.save(new Order(item, amount, "CREATED"));

        // Ye dursi service ko call karne ke liye(MS)
        // Call via RestTemplate
        // RestTemplate rest = new RestTemplate();
        // Map<String, Object> body = new HashMap<>();
        // body.put("orderId", order.getId().toString());
        // body.put("amount", amount);
        // String res = rest.postForObject("http://localhost:8082/pay", body,
        // String.class);
        // System.out.println(res);

        // Ye dursi service ko call karne ke liye(MS)
        // Call via FeignClient
        Map<String, Object> body = new HashMap<>();
        body.put("orderId", order.getId().toString());
        body.put("amount", amount);

        // ★ SAGA (compensating transaction) — microservices me cross-service SINGLE transaction NAHI hota
        //   (order apne DB me commit ho chuka, payment ALAG service/DB hai -> @Transactional yahan nahi phailta).
        //   isliye: payment PASS -> order PAID  |  payment FAIL -> order FAILED (compensating action).
        //   committed order ko "rollback" nahi kar sakte -> explicit FAILED mark karo -> state consistent, crash NAHI.
        try {
            String ans = client.pay(body);
            order.setStatus("PAID");
            System.out.println(ans);
        } catch (Exception e) {
            order.setStatus("FAILED");
            e.printStackTrace();
        }
        return repo.save(order);
    }
}
