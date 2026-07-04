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

## 7c. SAGA (compensating) — cross-service failure handling
```
   PROBLEM: MS me cross-service SINGLE transaction NAHI. order apne DB me commit + payment ALAG service/DB.
            @Transactional 2 service pe nahi phailta. order committed + payment fail -> order stuck "CREATED" = INCONSISTENT.
            committed row ko rollback bhi nahi kar sakte.

   SAGA = local commits ki chain + COMPENSATING action on failure.
   code me (chhote scale) = try-catch:
       try   -> payment call + order PAID
       catch -> order FAILED   (compensating: "rollback" ki jagah explicit mark)
   -> dono case save -> order kabhi inconsistent/atka nahi.

   TEST (jaan-boojh ke fail): payment-service BAND -> POST /order
     -> Feign RetryableException: "Connection refused ... 8082/pay" -> catch -> 200 OK + status=FAILED (crash NAHI).

   bade scale pe SAGA = coordinator / events / state-machine, par CORE idea wahi: step fail -> compensate.
   (= HLD Q13: committed rollback nahi -> compensating se undo.)
```

## 7d. API Gateway — single entry-point + routing (Spring Cloud Gateway)
```
   WHAT: ek SINGLE darwaza (port 8080) saari services ke aage. client sirf gateway se baat karta,
         gateway sahi service pe FORWARD karta (routing). client ko har service ka URL/port yaad nahi rakhna.
   WHY:  (1) single entry (client ko sirf 8080).  (2) cross-cutting (auth/rate-limit/logging/CORS) ek jagah.

   SERVLET vs REACTIVE gateway -> app ke STACK se match karo:
     app = spring-webmvc (servlet)  -> "Gateway" (gateway-server-webmvc)   <- HAMARA
     app = webflux (reactive)       -> "Reactive Gateway"

   CONFIG (application.yml) — ★ SAHI prefix (yahin galti hui thi):
     spring.cloud.gateway.server.webmvc.routes      <- servlet gateway (dependency = gateway-server-webmvc)
       - id · uri (kahan bheje) · predicates (Path=... kaunsi request)
     ❌ galti: "gateway.mvc.routes" -> galat namespace -> routes LOAD hi nahi hui -> har request 404.
     ✓ DEBUG lesson: dependency-artifact ke naam se namespace match karo -> "server-webmvc" -> "server.webmvc".
       (docs/net pe bharosa mat karo blindly — version alag hota; artifact-naam se derive karo.)

   ROUTE = PREDICATE (kaunsi request: Path=/order) + URI (kahan: http://localhost:8081).

   METHOD forwarding: gateway = TRANSPARENT PROXY. GET/POST/DELETE + body + headers JAISE-KE-TAISE forward.
     -> method-wise alag rule NAHI. ek "Path=/order/**" -> saare methods + subpaths -> order-service.
     -> gateway = "kis SERVICE" (path decide).  service = "kya karna" (@Get/@Post/@DeleteMapping dispatch).

   /** vs exact: single endpoint -> exact "Path=/order".  many endpoints/methods -> "Path=/order/**" (poora group route).

   Boot 4.1 gotcha: Spring Cloud compatibility-verifier off (Feign wala hi).
```

## 8. .gitignore — Windows case-insensitivity + anchor (silent config-loss gotcha)
```
   HUA KYA: "RESOURCES/" (private root folder ke liye) ne "src/main/resources/" ko bhi ignore kar diya
            -> dono services ki application.properties git me gayi hi nahi -> fresh clone pe config missing.

   WHY: .gitignore matching by DEFAULT case-SENSITIVE hai (Linux). PAR Windows/Mac filesystem case-INSENSITIVE
        -> git khud core.ignorecase=true karta -> "RESOURCES/" = "resources/" match hone laga.

   FIX (case ka nahi, ANCHOR ka): "RESOURCES/" -> "/RESOURCES/"  (leading slash = sirf ROOT).
        nested src/main/resources root pe nahi -> ab match nahi. private root RESOURCES safe (still ignored).

   LESSON: (1) Windows pe gitignore case-INSENSITIVE. (2) root/private folders HAMESHA "/name/" se anchor karo.
           (3) "git add -A" ke baad HAMESHA "git status" dekho -> silent-ignore/silent-delete aise hi chhup jaate.
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
