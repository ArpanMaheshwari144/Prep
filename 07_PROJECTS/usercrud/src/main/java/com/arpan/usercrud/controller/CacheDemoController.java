package com.arpan.usercrud.controller;

import com.arpan.usercrud.service.CacheDemoService;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.HashMap;
import java.util.Map;

@RestController
@RequestMapping("/cache-demo")
public class CacheDemoController {

    private final CacheDemoService cacheDemoService;

    public CacheDemoController(CacheDemoService cacheDemoService) {
        this.cacheDemoService = cacheDemoService;
    }

    private final Map<Long, String> cache = new HashMap<>();

    @GetMapping("/{id}")
    public String getUser(@PathVariable Long id) throws InterruptedException {
        // 1. pehle CACHE me dekho
        if (cache.containsKey(id)) {
            System.out.println(">> CACHE HIT  id=" + id + "  (fast, DB tak gaye hi nahi)");
            return cache.get(id);
        }

        // 2. cache me nahi mila (MISS) -> DB jao (SLOW)
        System.out.println(">> CACHE MISS id=" + id + "  -> DB ja rahe hain...");
        Thread.sleep(200);  // maano DB 200ms leta
        String nameFromDb = "User-" + id;

        // 3. DB se mila -> CACHE me daal do (agli baar yahin se)
        cache.put(id, nameFromDb);
        return nameFromDb;
    }

    @GetMapping("/anno/{id}")
    public String getUserAnno(@PathVariable Long id) throws InterruptedException {
        return cacheDemoService.getUser(id);
    }
}
