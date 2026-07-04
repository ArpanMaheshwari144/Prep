# Mini Payment MS — LEARNINGS (jo BANAKE seekha, 4-Jul)

> "bol sakta" se "bana sakta" — theory ko CODE me dekh liya. revise ke liye.

---

## 1. Microservice = alag programs, HTTP pe baat
```
   single-app  -> paymentService.pay()   = SAME program ke andar METHOD call
   microservice-> rest.post(".../pay")    = DOOSRE program ko URL/HTTP call (network pe)

   koi magic nahi — bas alag-alag programs ek doosre ke URL pe HTTP call karte hain.
   order-service (8081) ne payment-service (8082) ko call kiya -> "Payment DONE" wapas.
```

## 2. RestTemplate = sirf ek HTTP client
```
   rest.postForObject(url, body, String.class)
     = "is URL pe ye body POST karo, jawab is type me lo."
   wahi cheez jo Postman/curl karta -> bas JAVA CODE se ki, haath se nahi.
   (aage: @FeignClient = declarative version — interface likho, Spring call banata.)
```

## 3. DB-per-service
```
   order-service -> apna DB (orderdb)   |   payment-service -> apna DB (paydb)
   alag kyun: fault-isolation + independent scale. ek service ka DB doosre se coupled nahi.
```

## 4. HikariCP = connection pool (Spring DEFAULT, har SQL DB ke liye)
```
   logs me HikariCP dikha jabki MySQL nahi lagaya -> kyunki H2 bhi SQL DB hai,
   aur Spring Boot HAR JDBC DB ke liye default HikariCP use karta (MySQL-specific nahi).

   pool kya: har request pe naya connection = mehnga (TCP handshake + auth).
             POOL = ready connections ka set -> borrow -> use -> RETURN (reuse).
             (hotel-waiter analogy: set-of-waiters sab guests handle karte.)
   shutdown pe pool close hota -> isliye "HikariPool shutdown" message.
   -> yeh HLD wala connection-pooling (why-how QA Q16) hi hai, ab CODE me live dikha.
```

## 5. Spring me boilerplate bahut — aur usse bachne ke tools
```
   boilerplate: entity + getters/setters + DTO + repo + controller wiring (repetitive).
   reducers: record (DTO auto) · Lombok @Data (getters/setters auto) ·
             Spring Data JpaRepository (SQL nahi, CRUD free) · Spring Boot auto-config (XML nahi).
   (interview me poochte "boilerplate kaise kam karoge?")
```

## 6. Bug jo khud pakda — getId() null before save
```
   new Order(...) banaya par SAVE nahi kiya -> order.getId() = NULL (@GeneratedValue id DB save pe milta).
   fix: payment call se PEHLE repo.save karo -> tab id -> phir bhejo.
```

## record vs class — accessor
```
   record -> compiler sab auto banata, accessor = orderId()  (no "get")
   class  -> tu khud getters likhta, accessor = getOrderId()  (with "get")
```

## 7b. @FeignClient — RestTemplate ka clean/declarative version
```
   PROBLEM: bahut saari services -> har jagah RestTemplate + Map + URL + postForObject = boilerplate, repeat.
   FEIGN: ek INTERFACE likho jo remote endpoint ko DESCRIBE kare -> Spring KHUD implementation banata (HTTP call).

     @FeignClient(name="payment-service", url="http://localhost:8082")   // CALLER side (order-service) me
     interface PaymentClient {
         @PostMapping("/pay")                        // remote endpoint ka CONTRACT mirror
         String pay(@RequestBody Map<String,Object> body);   // no body -> Spring implement karega
     }

   use: client.pay(body)   // dikhta local method, peeche HTTP call (RestTemplate jaisा) -> boilerplate gaya.

   ★ 3 key baatein:
     - interface CALLER side pe (jo call karta), CALLEE ko point karta.
     - method annotations (@PostMapping/@RequestBody/return) = remote contract se MATCH karne chahiye.
     - SERVICE BOUNDARY: order payment ka class (PayRequest) IMPORT nahi kar sakta (alag service/jar).
       -> Map use kiya (ya apna copy banao). services JSON-contract se baat karti, code share NAHI.
     - enable: main app pe @EnableFeignClients + dependency spring-cloud-starter-openfeign.
   result: RestTemplate ka SAME kaam, kam code.
```

## 7. Feign / Spring Cloud — version compatibility verifier (real-world gotcha)
```
   Feign = Spring CLOUD ka part. Spring Cloud ki version, Spring Boot version se MATCH honi chahiye.

   humara Boot = 4.1.0 (bahut naya) -> Spring Cloud (OpenFeign 5.0.1) ki compatibility-LIST me
   abhi 4.1.0 tha hi nahi -> startup pe "compatibility VERIFIER" ne "INCOMPATIBLE" bolke app rok di
   -> mvn install ka contextLoads test FAIL (app uthi hi nahi).

   FIX (application.properties):  spring.cloud.compatibility-verifier.enabled=false
   -> verifier check OFF -> app uth gayi -> build success.

   analogy: verifier = bouncer + "allowed-versions" list. tera Boot itna naya ki list me nahi -> roka.
            humne bouncer se kaha "chhod de" (check off).

   LESSON: bleeding-edge Boot + Spring Cloud = version-mismatch aa sakta. tera CODE galat nahi, version-newness.
           real-world me aksar aise hi verifier off/version pin karke aage badhte.
```
