package com.example.practice.config;

/*
 * ============================================================
 * YE KYA HAI:
 *   Redis Cache Config — Redis mein data KAISE store ho ye define karta.
 *   Ye Spring ko batata: "Bhai, cache mein data JSON format mein rakh, binary nahi."
 *
 * KYUN CHAHIYE:
 *   Default Redis serializer Java binary (JdkSerializationSerializer) use karta.
 *   Binary = unreadable garbage in Redis. Debug impossible. Errors aate.
 *   Ye config JSON format mein store karta — readable + error free + debug easy.
 *
 * NAHI HUA TO:
 *   @Cacheable pe "Cannot serialize" / "SerializationException" error aayega.
 *   Cache kaam nahi karega. App crash ya silent fail.
 *   Redis CLI mein dekhoge toh binary gibberish dikhega — kuch samajh nahi aayega.
 *
 * REAL WORLD ANALOGY:
 *   Bina config: courier company (Redis) ko SEALED BLACK BOX (binary) mein bheja →
 *     receiver ko kholna mushkil, delivery boy ko bhi pata nahi kya hai.
 *   With config: TRANSPARENT BOX (JSON) mein bheja →
 *     receiver easily khol le, delivery boy bhi dekh sakta kya hai, debug easy.
 * ============================================================
 *
 * SERIALIZATION KYA HAI (Letter → Envelope Analogy):
 *
 *   Serialization = Java object ko bytes mein convert karna.
 *   Deserialization = bytes se wapas Java object banana.
 *
 *   ANALOGY:
 *     Tere paas ek LETTER hai (Java Object — StudentDTO).
 *     Tu POST office (Redis) se bhej raha.
 *     POST office kehta: "Seedha letter nahi le sakta. ENVELOPE mein daal ke de."
 *
 *     Serialization   = letter ko envelope mein daalna (Object → bytes/JSON)
 *     Deserialization = envelope se letter nikalna (bytes/JSON → Object)
 *     Redis           = POST office — sirf envelopes (bytes) accept karta
 *     Bina envelope   = POST reject kare = "Cannot serialize" error
 *
 *   REAL FLOW:
 *     StudentDTO {name:"Arpan", age:27}
 *       → serialize → {"@class":"com.example.practice.dto.StudentDTO","name":"Arpan","age":27}
 *       → Redis mein store (as bytes/string)
 *
 *     Cache hit pe:
 *       Redis se bytes aaye → deserialize → StudentDTO object wapas mil gaya
 *
 * ============================================================
 * 2 SERIALIZER — GenericJackson2JsonRedisSerializer vs JdkSerializationSerializer:
 *
 *   JdkSerializationSerializer (DEFAULT — BURA):
 *     - Java ka built-in serializer (ObjectOutputStream).
 *     - Binary format — Redis mein "\xac\xed\x00\x05..." dikhega.
 *     - Human readable? NAHI. Debug? IMPOSSIBLE.
 *     - Class rename/move kiya? → Deserialization FAIL. Old cache useless.
 *     - Cross-language? NAHI. Sirf Java samjhe.
 *     - Analogy: Letter ko SEALED BLACK BOX mein bheja — koi padh nahi sakta.
 *
 *   GenericJackson2JsonRedisSerializer (HUM USE KAR RAHE — ACHA):
 *     - Jackson library use karta. JSON format.
 *     - Redis mein clean JSON dikhega: {"name":"Arpan","age":27}
 *     - Human readable? HAI. Debug? EASY. redis-cli se dekh sakte.
 *     - @class field auto add karta — deserialize mein pata chale kaunsa class.
 *     - Cross-language? Partial — JSON toh koi bhi padh le.
 *     - Analogy: Letter ko TRANSPARENT envelope mein bheja — sab padh sakte.
 *
 *   COMPARISON TABLE:
 *     Feature                    Jdk (Default)          Jackson JSON (Humara)
 *     ---------------------------------------------------------------------------
 *     Format                     Binary (ugly)           JSON (clean)
 *     Readable in Redis CLI      NAHI                    HAI
 *     Debug friendly             NAHI                    HAI
 *     Class rename safe          NAHI (break)            HAI (@class se)
 *     Size                       Smaller (thoda)         Bigger (thoda — @class ki wajah se)
 *     Speed                      Faster (thoda)          Slightly slower (JSON parse)
 *     Cross-language             NAHI (Java only)        HAI (JSON universal)
 *
 *   VERDICT: JSON use karo. Thoda size/speed trade-off but DEBUGGING aur READABILITY >>>
 *
 * ============================================================
 * @class FIELD KYUN DIKHTA IN REDIS JSON:
 *
 *   Jab GenericJackson2JsonRedisSerializer use karte toh Redis mein ye store hota:
 *     {"@class":"com.example.practice.dto.StudentDTO","name":"Arpan","age":27}
 *
 *   @class KYUN?
 *     Redis ko nahi pata ye kaunsa Java class tha.
 *     Jab deserialize karega (bytes → Object), usse batana padega: "Ye StudentDTO hai."
 *     @class field = type info embedded in JSON.
 *     Iske bina Jackson ko pata nahi chalega kaunsa class banaye → ClassCastException.
 *
 *   "Generic" ka matlab:
 *     GenericJackson2Json... = KISI BHI class ko serialize kar sakta.
 *     StudentDTO ho ya TeacherDTO — @class se pata chal jaata.
 *
 * ============================================================
 * VISUALIZE — Redis ke Andar Kya Hota Hai:
 *
 *   Step 1: Client calls GET /api/students/5
 *   Step 2: @Cacheable("students") check karta — Redis mein "students::5" key hai?
 *
 *   CACHE MISS (pehli baar):
 *     Redis: KEYS * → (empty)
 *     → DB se fetch: SELECT * FROM students WHERE id=5
 *     → StudentDTO {id:5, name:"Arpan", email:"arpan@mail.com", age:27}
 *     → Serialize → JSON: {"@class":"...StudentDTO","id":5,"name":"Arpan",...}
 *     → Redis STORE:
 *         KEY:   "students::5"          (StringRedisSerializer — clean readable key)
 *         VALUE: {"@class":"...","id":5,"name":"Arpan",...}  (JSON — GenericJackson2Json)
 *         TTL:   60 minutes
 *     → Response return to client
 *
 *   CACHE HIT (doosri baar, 60 min ke andar):
 *     Redis: GET "students::5" → JSON found!
 *     → Deserialize → StudentDTO object ban gaya
 *     → DB CALL NAHI HUA! Direct return. FAST.
 *
 *   CACHE EXPIRE (60 min baad):
 *     Redis: GET "students::5" → nil (expired, auto deleted)
 *     → DB se phir fetch → Redis mein phir store → cycle repeat
 *
 *   Redis CLI se check:
 *     $ redis-cli
 *     127.0.0.1:6379> KEYS *
 *     1) "students::5"
 *     2) "students::all"
 *
 *     127.0.0.1:6379> GET "students::5"
 *     {"@class":"com.example.practice.dto.StudentDTO","id":5,"name":"Arpan","email":"arpan@mail.com","age":27}
 *
 *     127.0.0.1:6379> TTL "students::5"
 *     (integer) 3420    <-- 3420 seconds left (57 min)
 *
 *   BINA YE CONFIG:
 *     KEY:   "\xac\xed\x00\x05t\x00\x0bstudents::5"  <-- UGLY binary prefix
 *     VALUE: "\xac\xed\x00\x05sr\x00-com.example..."   <-- unreadable binary blob
 *     Debug? IMPOSSIBLE. Error fix? NIGHTMARE.
 * ============================================================
 */

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.data.redis.connection.RedisConnectionFactory;
import org.springframework.data.redis.cache.RedisCacheConfiguration;
import org.springframework.data.redis.cache.RedisCacheManager;
import org.springframework.data.redis.serializer.GenericJackson2JsonRedisSerializer;
import org.springframework.data.redis.serializer.RedisSerializationContext;
import org.springframework.data.redis.serializer.StringRedisSerializer;
import java.time.Duration;

/*
 * ============================================================
 * @Configuration
 *
 * YE KYA HAI: Spring config class — @Bean methods ke liye.
 *
 * KYUN LAGAYA: CacheManager bean banana hai → @Bean method → @Configuration class chahiye.
 *
 * NAHI LAGAYA TO KYA HOGA:
 *   - @Bean method detect nahi hoga → CacheManager bean nahi banega
 *   - Spring DEFAULT CacheManager use karega → JdkSerializer → binary garbage
 *   - "Cannot serialize" errors → cache DEAD → har request DB pe
 *
 * REAL WORLD ANALOGY:
 *   @Configuration = POST OFFICE ka MANAGER.
 *   Manager bole: "Transparent envelope (JSON) use karo, 60 min ke baad letters expire."
 *   Bina manager: default rules — sealed black box (binary), no expiry.
 * ============================================================
 */
@Configuration
public class RedisConfig {

    /*
     * ============================================================
     * CacheManager @Bean — Redis cache KAISE configure karna ye batata hai.
     *
     * YE KYA HAI: Spring ko batao cache ka behavior — serializer, TTL, null handling.
     *
     * KYUN LAGAYA: Default settings KHARAB hain (binary, no TTL, null cache hota).
     *   Custom config se: JSON format, 60 min TTL, null cache nahi.
     *
     * NAHI LAGAYA TO KYA HOGA (CacheManager bean nahi banayi):
     *   - Spring default JdkSerializer use karega → binary garbage Redis mein
     *   - "Cannot serialize" / "SerializationException" error → cache DEAD
     *   - No TTL → cache FOREVER → stale data → wrong responses
     *   - Null values cache honge → memory waste + confusing behavior
     *
     * REAL WORLD ANALOGY:
     *   CacheManager = FRIDGE ka SETTINGS PANEL.
     *   Temperature set karo (TTL), kya rakhna kya nahi (null disable), format set karo (JSON).
     *   Bina settings: fridge full blast chale (no TTL), khaali dabbey bhi rakh le (null cache),
     *   labels nahi (binary) → sab confusing.
     *
     * RedisConnectionFactory — Spring auto inject karega (application.properties se config le ke)
     *   = localhost:6379 pe Redis connection
     * ============================================================
     */
    @Bean
    public RedisCacheManager cacheManager(RedisConnectionFactory connectionFactory) {

        RedisCacheConfiguration config = RedisCacheConfiguration.defaultCacheConfig()

                /*
                 * ============================================================
                 * entryTtl(Duration.ofMinutes(60)) — Cache Expiry Time
                 *
                 * YE KYA HAI: TTL = Time To Live = cache kitni der valid rahega.
                 *   60 min baad Redis AUTOMATICALLY delete kar dega cached data.
                 *
                 * KYUN LAGAYA: Stale (purana) data se bachne ke liye.
                 *   DB mein data badla but cache mein purana → client ko WRONG response milega.
                 *   TTL se 60 min baad automatic refresh — fresh data DB se aayega.
                 *
                 * NAHI LAGAYA TO KYA HOGA:
                 *   - Cache FOREVER rahega → DB mein data badla but cache purana
                 *   - Stale data → wrong responses → angry users
                 *   - Redis memory bhar jaayegi — koi data expire nahi ho raha
                 *
                 * REAL WORLD ANALOGY:
                 *   entryTtl = MILK ki EXPIRY DATE.
                 *   "60 min ke baad use mat karo, fresh laao (DB se)."
                 *   Expiry nahi → purana doodh pee liya → pet kharab (stale data → bugs).
                 * ============================================================
                 */
                .entryTtl(Duration.ofMinutes(60))

                /*
                 * ============================================================
                 * disableCachingNullValues() — Null Cache Band Karo
                 *
                 * YE KYA HAI: Agar DB se NULL aaya (student nahi mila), toh Redis mein null STORE MAT KARO.
                 *
                 * KYUN LAGAYA:
                 *   1. Memory waste — null store karke kya faayda? Redis ki jagah khaali khaao.
                 *   2. Student baad mein create hua → cache mein null pada → galat response
                 *      Client ko lagega "student nahi hai" — but actually hai!
                 *
                 * NAHI LAGAYA TO KYA HOGA:
                 *   - null cache ho jaayega → "student nahi hai" response aata rahega
                 *     even jab student actually create ho chuka hai
                 *   - Memory waste — Redis mein lakhon null entries
                 *   - Confusing bugs — "data hai DB mein but API bol raha nahi hai"
                 *
                 * REAL WORLD ANALOGY:
                 *   disableCachingNullValues = FRIDGE mein khaali DABBA mat rakh.
                 *   Khaali dabba rakhoge → jagah waste + dekhke lagta "kuch hai" → khologe toh khaali.
                 *   Khaali dabba mat rakho → jagah bachi + confusion nahi.
                 * ============================================================
                 */
                .disableCachingNullValues()

                /*
                 * ============================================================
                 * serializeKeysWith — Cache KEY ka format (String)
                 *
                 * YE KYA HAI: Cache KEY ko String format mein store karo — "students::5".
                 *
                 * KYUN LAGAYA: Readable keys — Redis CLI mein KEYS * karo toh samajh aaye.
                 *
                 * NAHI LAGAYA TO KYA HOGA:
                 *   - KEY binary mein store hoga: "\xac\xed\x00\x05t\x00\x0bstudents::5"
                 *   - Redis CLI mein dekhoge → "ye kya hai?" → debug IMPOSSIBLE
                 *   - KEYS * karo → sab binary mess — kaunsi key kaunse student ki? PATA NAHI
                 *
                 * REAL WORLD ANALOGY:
                 *   String key = FILE par LABEL likha (readable — "Student Arpan, Roll 5").
                 *   Binary key = FILE par barcode laga (machine padhe but insaan nahi).
                 * ============================================================
                 */
                .serializeKeysWith(
                        RedisSerializationContext.SerializationPair
                                .fromSerializer(new StringRedisSerializer())
                )

                /*
                 * ============================================================
                 * serializeValuesWith — Cache VALUE ka format (JSON)
                 *
                 * YE KYA HAI: Cache VALUE ko JSON format mein store karo.
                 *   StudentDTO → {"@class":"...StudentDTO","name":"Arpan","age":27}
                 *
                 * KYUN LAGAYA: JSON = human readable, debug friendly, cross-language.
                 *
                 * NAHI LAGAYA TO KYA HOGA:
                 *   - VALUE binary mein: "\xac\xed\x00\x05sr\x00-com.example..."
                 *   - Redis CLI mein GET karo → gibberish → debug IMPOSSIBLE
                 *   - Class rename kiya → old cache deserialize FAIL → errors
                 *   - "Cannot serialize" errors common — Serializable chahiye strictly
                 *
                 * REAL WORLD ANALOGY:
                 *   JSON value = TRANSPARENT envelope (andar kya hai dikh raha).
                 *   Binary value = SEALED black box (kya hai pata nahi, kholna mushkil).
                 * ============================================================
                 */
                .serializeValuesWith(
                        RedisSerializationContext.SerializationPair
                                .fromSerializer(new GenericJackson2JsonRedisSerializer())
                );
        /*
         * GenericJackson2JsonRedisSerializer — JSON + @class field:
         *
         *   StudentDTO { name: "Arpan", age: 27 }
         *   → Redis mein store:
         *     {"@class":"com.example.practice.dto.StudentDTO","name":"Arpan","age":27}
         *
         *   @class = type info — deserialize karte waqt pata chale kaunsa class hai.
         *   Iske bina: Jackson confuse → "kaunsa object banau?" → ClassCastException.
         *   "Generic" = kisi bhi class ke liye kaam kare (StudentDTO, TeacherDTO, etc.)
         */

        return RedisCacheManager.builder(connectionFactory)
                .cacheDefaults(config)    // ye default config SAB caches pe apply hoga
                .build();
        // Builder pattern — config set kiya → build() → RedisCacheManager return → Spring use karega
    }
}
