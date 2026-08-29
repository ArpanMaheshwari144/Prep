package com.arpan.usercrud.service;

import org.springframework.cache.annotation.Cacheable;
import org.springframework.stereotype.Service;

@Service
public class CacheDemoService {

    @Cacheable("users")
    public String getUser(Long id) throws InterruptedException {
        System.out.println(">> METHOD CHAL RAHA (DB) id=" + id + "  -- ye sirf MISS pe dikhega");
        Thread.sleep(200);
        return "User-" + id;
    }
}
