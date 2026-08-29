package com.arpan.usercrud.controller;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class AsyncDemoController {

    private DemoJobs demoJobs;

    public AsyncDemoController(DemoJobs demoJobs) {
        this.demoJobs = demoJobs;
    }

    @GetMapping("/async-demo")
    public String trigger() throws InterruptedException {
        demoJobs.sendEmail("a@x.com");
        demoJobs.sendEmail("b@x.com");
        demoJobs.sendEmail("c@x.com");
        return "3 emails triggered - check logs (endpoint TURANT return hua, emails background me)";
    }
}
