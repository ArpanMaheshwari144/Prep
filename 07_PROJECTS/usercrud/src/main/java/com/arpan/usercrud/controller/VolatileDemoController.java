package com.arpan.usercrud.controller;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class VolatileDemoController {

    // JAAN-BOOJH ke plain (volatile NAHI) — pehle bug dekhenge
    private volatile boolean running = true;

    @GetMapping("/volatile/start")
    public String start() {
        running = true;
        new Thread(() -> {
            while (running) {
                System.out.println("WORKER: loop shuru, running=" + running);
                long count = 0;
                while (running) {
                    count++;
                }
                System.out.println("WORKER: loop RUKA! count=" + count);
            }
        }).start();
        return "worker started (running=true)";
    }

    @GetMapping("/volatile/stop")
    public String stop() {
        running = false;
        System.out.println("MAIN: running=false set kar diya");
        return "stop signal bheja (running=false)";
    }
}
