package com.arpan.orderservice;

import java.util.HashMap;
import java.util.Map;

import org.springframework.stereotype.Service;
import io.github.resilience4j.circuitbreaker.annotation.CircuitBreaker;
// import org.springframework.web.client.RestTemplate;

/**
 * OrderService — order banata + payment-service ko call karta.
 * FLOW: order save (CREATED) -> Feign se payment call (SYNC) -> PASS=PAID /
 * FAIL=FAILED (SAGA compensating).
 */
@Service
public class OrderService {

    private final OrderRepository repo; // order ka apna DB (orderdb) — save/find

    private final PaymentClient client; // Feign client -> payment-service (8082) ko call

    // constructor injection — Spring repo + client khud inject karta
    public OrderService(OrderRepository repo, PaymentClient client) {
        this.repo = repo;
        this.client = client;
    }

    // ★ CIRCUIT BREAKER — payment-service DOWN ho to baar-baar call karke atakne se bachao (fail-fast).
    // ye method (createOrder) payment ko call karta hai -> isliye ispe CB lagaya.
    // @CircuitBreaker(name, fallbackMethod):
    //   name = "paymentService" -> ek LABEL. isi naam se config (application.properties) me rules diye.
    //   fallbackMethod = "payFallback" -> jab call FAIL ho ya CB OPEN ho -> wo method chalega (crash nahi).
    // 3 state: CLOSED (normal, call jaa rahi) -> baar-baar fail -> OPEN (call BAND, seedha fallback)
    //          -> thodi der baad HALF-OPEN (1 test call) -> theek? CLOSED : phir OPEN.
    @CircuitBreaker(name = "paymentService", fallbackMethod = "payFallback")
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

        // ★ SAGA (compensating transaction) — microservices me cross-service SINGLE
        // transaction NAHI hota
        // (order apne DB me commit ho chuka, payment ALAG service/DB hai ->
        // @Transactional yahan nahi phailta).
        // isliye: payment PASS -> order PAID | payment FAIL -> order FAILED
        // (compensating action).
        // committed order ko "rollback" nahi kar sakte -> explicit FAILED mark karo ->
        // state consistent, crash NAHI.

        // Why we comment this -> hum CB try kar rahe the isliye error lane ke liye isko comment kiya gaya hai
        // try {
        // String ans = client.pay(body);
        // order.setStatus("PAID");
        // System.out.println(ans);
        // } catch (Exception e) {
        // order.setStatus("FAILED");
        // e.printStackTrace();
        // }

        // ab try-catch NAHI (upar comment kiya) -> client.pay() FAIL hua to exception BAHAR jaayega
        // -> tabhi CB use "failure" count karta (try-catch andar kha jaata to CB ko pata hi na chalta).
        // call theek -> PAID.  call fail / CB OPEN -> Spring khud payFallback bula lega.
        String ans = client.pay(body);
        order.setStatus("PAID");
        System.out.println(ans);

        return repo.save(order);
    }

    // FALLBACK — CB isko tab bulaata jab createOrder ki payment-call FAIL ho / CB OPEN ho.
    // RULE: signature = createOrder ke SAME params (item, amount) + ek EXTRA Throwable t (last me),
    //       aur return type SAME (Order). naam wahi jo annotation me fallbackMethod = "payFallback".
    // yahan: payment down -> naya order FAILED mark karke laut do (order-service hang/crash nahi hota).
    public Order payFallback(String item, double amount, Throwable t) {
        System.out.println("CB fallback -> payment down: " + t.getMessage());
        return repo.save(new Order(item, amount, "FAILED")); // fail-fast: FAILED order
    }
}
