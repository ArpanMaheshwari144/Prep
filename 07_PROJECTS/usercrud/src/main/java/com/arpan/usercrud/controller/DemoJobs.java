package com.arpan.usercrud.controller;

import com.arpan.usercrud.UsercrudApplication;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.scheduling.annotation.Async;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;

@Component
public class DemoJobs {

    private static final Logger logger = LoggerFactory.getLogger(DemoJobs.class);

    // @ASYNC: alag thread pe chalega -> caller wait nahi karta
    @Async
    public void sendEmail(String to) throws InterruptedException {
        logger.info(">> sendEmail START to={} | THREAD = {}", to, Thread.currentThread().getName());
        Thread.sleep(3000); // maano email bhejne me 3 sec lage
        logger.info(">> sendEmail DONE  to={} | THREAD = {}", to, Thread.currentThread().getName());
    }

    // @SCHEDULED: har 3 sec khud fire (koi trigger nahi)
    // @Scheduled(fixedRate = 3000)
    public void heartbeat() {
        logger.info("## heartbeat @ {} | THREAD = {}", System.currentTimeMillis(),
                Thread.currentThread().getName());
    }
}
