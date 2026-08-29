package com.arpan.usercrud.controller;

import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class RateLimitController {

    private int LIMIT = 5;               // 1 window me max itni request
    private long windowStart = 0;        // current window kab shuru hua (millis)
    private int count = 0;               // is window me ab tak kitni aayi

    @GetMapping("/rate-demo")
    public synchronized ResponseEntity<String> hit() {
        long now = System.currentTimeMillis();

        // 1. window purani ho gayi? (1 second beet gaya) -> reset
        if(now - windowStart > 1000) {
            windowStart = now;
            count = 0;
        }

        // 2. is request ko gino
        count++;

        // 3. limit paar? -> 429
        if(count > LIMIT) {
            return ResponseEntity.status(HttpStatus.TOO_MANY_REQUESTS)
                    .body("429 Too Many Requests - " + LIMIT + "/sec limit paar (count=" + count + ")");
        }

        // 4. allowed -> 200
        return ResponseEntity.ok("OK - request #" + count + " (limit " + LIMIT + "/sec)");
    }
}
