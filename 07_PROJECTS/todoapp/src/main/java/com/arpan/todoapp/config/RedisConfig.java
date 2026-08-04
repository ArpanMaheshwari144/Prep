package com.arpan.todoapp.config;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.data.redis.cache.RedisCacheConfiguration;
import org.springframework.data.redis.cache.RedisCacheManager;
import org.springframework.data.redis.connection.RedisConnectionFactory;
import org.springframework.data.redis.serializer.GenericJackson2JsonRedisSerializer;
import org.springframework.data.redis.serializer.RedisSerializationContext;
import org.springframework.data.redis.serializer.StringRedisSerializer;

import java.time.Duration;

// ═══════════════════════════════════════════════════════════════
// RedisConfig — STANDARD BOILERPLATE
//    95% Spring + Redis projects mein yeh same file dikhti
//    Sirf TTL duration project-wise change hota
//
// CACHE vs REDIS (confuse mat hona):
//    CACHE = CONCEPT (hot data fast rakho, DB dobara na hit ho).
//    REDIS = us concept ka ek TOOL (key-value store; Caffeine/Memcached bhi tools).
//    YE FILE = Spring me Redis ko CACHE ki tarah wire karti. @Cacheable method pe
//              Spring isi manager se result Redis me daalta (key -> JSON value, 5-min TTL).
//    => "cache = kya (idea) · Redis = kaise (tool) · ye config = jodne wala".
//
// PURPOSE:
//    Redis mein data save karte time JSON format use karo
//    (default JDK serializer = binary gibberish + Serializable
//     interface mandatory; Todo class implement nahi karta)
//
// BEAN: RedisCacheManager
//    @Cacheable / @CacheEvict annotations ke peeche ka engine
//    Spring khud yeh use karta jab annotation method execute ho
//
// INTERVIEW LINE:
//    "Custom RedisCacheManager with JSON serialization for
//     cross-language compatibility and Redis CLI readability.
//     5-minute default TTL as stale-data safety net."
// ═══════════════════════════════════════════════════════════════

@Configuration
public class RedisConfig {

    @Bean
    public RedisCacheManager cacheManager(RedisConnectionFactory connectionFactory) {

        RedisCacheConfiguration config = RedisCacheConfiguration.defaultCacheConfig()
                // TTL = 5 minutes (har cache entry auto-expire)
                .entryTtl(Duration.ofMinutes(5))
                // Keys = plain string ("todos::5" Redis CLI mein readable)
                .serializeKeysWith(
                    RedisSerializationContext.SerializationPair
                        .fromSerializer(new StringRedisSerializer())
                )
                // Values = JSON (Jackson) — cross-language friendly
                .serializeValuesWith(
                    RedisSerializationContext.SerializationPair
                        .fromSerializer(new GenericJackson2JsonRedisSerializer())
                );

        return RedisCacheManager.builder(connectionFactory)
                .cacheDefaults(config)
                .build();
    }
}
