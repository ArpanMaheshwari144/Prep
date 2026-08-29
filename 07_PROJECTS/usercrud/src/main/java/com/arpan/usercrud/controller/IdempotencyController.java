package com.arpan.usercrud.controller;

import org.springframework.web.bind.annotation.*;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

@RestController
public class IdempotencyController {

    private final ConcurrentHashMap<String, String> map = new ConcurrentHashMap<>();

    private final AtomicInteger count = new AtomicInteger(0);

    @PostMapping("/pay")
    public String pay(@RequestHeader("Idempotency-Key") String Key, @RequestParam int amount) throws InterruptedException {

        // ATOMIC claim: key pehle se hai? -> existing wapas | nahi? -> "PROCESSING" daal + null
        String existing = map.putIfAbsent(Key, "PROCESSING");
        if (existing != null) {
            return "DUPLICATE (key already claimed) -> " + existing
                    + "  | total orders = " + count.get();
        }

        // yahan sirf EK thread pahunchega (jisne putIfAbsent race jeeti)
        int orderId = count.incrementAndGet();
        String result = "Order #" + orderId + " created, amount=" + amount;

        map.put(Key, result);   // "PROCESSING" ko asli result se replace

        return "OK -> " + result + "  | total orders = " + count.get();

        // BROKEN: check aur put ALAG steps (beech me gap!)
//        if (map.containsKey(Key)) {
//            return "DUPLICATE -> " + map.get(Key)
//                    + "  | total orders = " + count.get();
//        }
//
//        Thread.sleep(50);   // "processing" (real me DB insert) -- gap widen -> race saaf dikhega
//
//        int orderId = count.incrementAndGet();
//        String result = "Order #" + orderId + " created, amount=" + amount;
//
//        map.put(Key, result);
//
//        return "OK -> " + result + "  | total orders = " + count.get();
    }
}
