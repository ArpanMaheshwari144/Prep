# Mini Payment MS — LEARNINGS (jo BANAKE seekha, 4-Jul)

> "bol sakta" se "bana sakta" — theory ko CODE me dekh liya. revise ke liye.

---

## 0. OVERALL FLOW (poora picture)
```
        POST /order  { item, amount }
             |
             v
   ┌──────────────┐  route (Path=/order)  ┌──────────────┐
   │ api-gateway  │ ────────────────────► │ order-service│
   │ (8080)       │  single entry-point   │ (8081)       │
   └──────────────┘                       └──────┬───────┘
                                                 │  Feign  ★ SYNC (RUKO jab tak jawab)
                                                 v
                                       ┌──────────────────┐
                                       │ payment-service  │ ◄── PRODUCER
                                       │ (8082)           │
                                       │ 1. DB save (paydb)│
                                       │ 2. kafka.send(   │
                                       │  "payment-done", │
                                       │   "Payment...")  │
                                       └──────┬───────────┘
                                              │  ★ ASYNC (bhej ke aage, wait nahi)
                                              v
                     ╔════════════════════════════════════════╗
                     ║   KAFKA BROKER (docker, :9092)          ║
                     ║   TOPIC "payment-done"                  ║
                     ║    partition 0: [offset 0] "Payment     ║
                     ║                  DONE for order:1..."   ║ ◄── message PADA (persist, disk)
                     ╚═══════════════╤════════════════════════╝
                                     │  @KafkaListener("payment-done", groupId="notification-group")
                                     v
                           ┌──────────────────────┐
                           │ notification-service │ ◄── CONSUMER
                           │ (8083)  -> "Email    │
                           │  send -> Payment..." │
                           └──────────────────────┘

   SYNC  (Feign) : order↔payment -> jawab TURANT chahiye -> ruko.
   ASYNC (Kafka) : payment→notification -> "ho gaya, ab email" -> bhej ke aage, user wait nahi.
   CONNECTOR     : topic naam "payment-done" -> producer send() + consumer @KafkaListener, DONO me SAME.
   OFFSET        : message ka partition me position (0,1,2...). consumer isi se track "kahan tak padha".
   DB-per-service: order->orderdb · payment->paydb · notification->koi DB nahi (sirf sune+print).
```

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

## 7e. Kafka — async messaging (producer / topic / consumer)  [phase-2, code me dekha]
```
   SYNC (Feign)  = order ne payment ko call kiya, jawab tak RUKA. direct, wait.
   ASYNC (Kafka) = "fire-and-forget" -> event daal ke aage badh gaya, jawab ka wait nahi.

   3 cheez:
     PRODUCER  -> KafkaTemplate<String,String> inject -> kafka.send("payment-done", message). (payment-service)
     TOPIC     -> naamit channel/mailbox ("payment-done") jahan message padta.
     CONSUMER  -> @KafkaListener(topics="payment-done", groupId="...") wala METHOD -> message aate hi Spring khud call karta. (notification-service)
   -> producer & consumer DECOUPLED: seedha nahi jaante, sirf TOPIC ke through. no polling-loop (Spring listener bulaata).

   kab kya: jawab TURANT chahiye -> SYNC (Feign). "ho gaya, ab background kaam (email)" -> ASYNC (Kafka).
     -> payment ho gaya -> order turant confirm -> email peeche apni marzi se (user wait nahi karta). decouple + spike-absorb + 1 event N consumer.

   SETUP: Kafka broker chahiye -> docker-compose.yml (KRaft mode, Zookeeper-free, port 9092) -> "docker compose up -d".
   config (services me): spring.kafka.bootstrap-servers=localhost:9092 (+ consumer group-id, auto-offset-reset=earliest).
   analogy: Feign = PHONE call (ruko jab tak jawab) · Kafka = LETTERBOX (chitthi daal ke chale gaye, jisko chahiye padhe).
   tested: POST /order -> payment saves + event -> notification console: "Email send -> Payment DONE for order 1..." (async, no wait).
```

## 7f. Kafka — queue KAHAN hai + kaise DEKHE (broker, CLI, kafka-ui)
```
   QUEUE Spring me NAHI -> Kafka BROKER (docker container) me. Spring sirf CLIENT (producer/consumer).
     messages broker ke DISK pe persist -> Spring restart pe bhi safe.
   topic AUTO-created -> producer ne pehli baar "payment-done" pe bheja -> Kafka ne khud bana diya.

   ★ LOG-based (classic queue se farak):
     classic queue -> message consume hua = HAT gaya.
     Kafka -> message topic me RAHTA (retention tak) -> multiple consumers/groups padh sakte + --from-beginning REPLAY.
     OFFSET = message ka partition me position (0,1,2..). consumer isi se track "kahan tak padha". groupId per offset.

   DEKHNE ke 2 tareeke:
     CLI (docker exec):
       kafka-topics.sh --list --bootstrap-server localhost:9092
       kafka-console-consumer.sh --topic payment-done --from-beginning --bootstrap-server localhost:9092
       (git-bash pe path-mangle -> MSYS_NO_PATHCONV=1 prefix. PowerShell me seedha.)
     kafka-UI (GRAPHICAL, browser):
       docker-compose me kafbat/kafka-ui add (port 8090) -> browser localhost:8090.
       ★ networking: kafka-ui CONTAINER hai -> broker tak INTERNAL listener chahiye (kafka:29092).
         isliye broker pe DUAL listener: PLAINTEXT(localhost:9092=host services) + DOCKER(kafka:29092=ui container).
         advertised-listener = broker jo address client ko batata; container vs host ke liye alag chahiye.
       UI me: Brokers (health) · Topics (payment-done -> Messages -> offset/value dekho) · Consumers (group lag).
     __consumer_offsets = Kafka ka INTERNAL topic (50 partitions default) jo har group ka offset track karta.
```

## 7g. Idempotency — same request 2 baar, charge SIRF 1 baar  [phase-2, khud banaya+tested 6-Jul]
```
   WHAT: idempotent = SAME operation 1 baar karo ya 10 baar -> effect/result SAME rahe, extra side-effect NAHI.
   WHY payment me: client double-click / network retry / timeout-retry -> same /pay 2 baar aa jaye
                   -> 2 baar CHARGE. galat. idempotency ye rokta.
   analogy: cloakroom TOKEN / cinema ticket-stub. token pehle se register -> "already ho chuka, dobara nahi"
            -> purana result laut do. naya token -> process + register. (token = IDEMPOTENCY KEY.)

   KAISE banaya (3 badlav, payment-service):
     1. Payment entity : orderId ko @Column(unique=true) -> DB-level safety-net (same orderId 2 row nahi).
     2. PaymentRepository : Optional<Payment> findByOrderId(String) -> Spring Data method-NAAM se query khud banata.
     3. PaymentService.pay() : CHECK-then-ACT ->
            findByOrderId PEHLE -> mila (present) -> WAHI return (no save, no Kafka).
            nahi mila (else) -> ab new Payment + save + kafka.send + return.
     (key = orderId; hamare case 1 order = 1 payment, isliye orderId hi idempotency-key ban gaya.)

   ★ BUG jo khud pakda (ORDER matters):
     pehle save-then-check kiya tha (ULTA) ->
       1st call: save -> phir check -> apni hi entry mil gayi -> Kafka SKIP (notification nahi gaya). ✗
       2nd call: save -> UNIQUE constraint VIOLATION -> CRASH. ✗
     FIX: CHECK sabse PEHLE (create/save se bhi pehle). trace kar ke dono case galat dikhe -> theek kiya.

   TEST (Postman, POST localhost:8082/pay, body {"orderId":12,"amount":500} DO baar):
     1st -> paymentId=1 (1.76s: save+Kafka hua).
     2nd -> paymentId=1 SAME (13ms: existing laut diya, save+Kafka SKIP) -> double-charge BACHA. ✓ PASS.
     (kafka-ui: "payment-done" me sirf 1 message -> 2nd ne send nahi kiya.)

   ★ 2 LAYER idempotency (mazedaar connect): startup ProducerConfig me enable.idempotence=true dikha ->
     ye KAFKA ki APNI idempotency (producer retry pe duplicate MESSAGE na jaye, broker-level).
     humne APP-level banayi (payment double na ho). SAME concept, 2 ALAG layer (app + broker).
```

## 7h. Circuit Breaker (Resilience4j) — dead service ko baar-baar call se bachao  [phase-2, khud banaya+tested 7-Jul]
```
   WHAT: ek "switch" jo ek service ke calls pe nazar rakhe. call baar-baar FAIL -> switch OPEN ->
         us service ko call karna BAND -> turant FALLBACK (fail-fast). caller healthy rehta.
   WHY:  payment DOWN -> order har baar usko call (Feign) -> har call TIMEOUT tak WAIT -> order ke threads
         atak jaate -> order-service bhi slow/hang. ek ka failure DUSRE ko le dooba (CASCADE). CB ye rokta.
   analogy: ghar ka FUSE. short-circuit -> fuse TRIP -> current band -> baaki ghar bacha. der baad reset-try.

   3 STATE (interview isko poochta):
     CLOSED    = normal, calls jaa rahe (fuse chalu).
     OPEN      = fail-threshold cross -> calls BAND, seedha fallback (service ko chhuta bhi nahi).
     HALF-OPEN = wait-duration baad -> 1-2 TEST call -> theek? -> CLOSED : phir OPEN.

   KAISE banaya (Resilience4j, 4 tukde — mostly boilerplate; order-service me, kyunki wahi payment ko call karta):
     1. dependency: spring-cloud-starter-circuitbreaker-resilience4j, VERSION 5.0.2 (explicit — is pom me koi
        Spring Cloud BOM nahi, isliye Feign ki tarah har cloud-dep ko apni version deni padti; CB starter Feign se
        alag sub-project -> apni 5.0.x version).
     2. annotation: createOrder() pe @CircuitBreaker(name="paymentService", fallbackMethod="payFallback").
        import io.github.resilience4j.circuitbreaker.annotation.CircuitBreaker.
     3. fallback method: public Order payFallback(String item, double amount, Throwable t) -> FAILED order.
        ★ RULE: signature = annotated-method ke SAME params + ek Throwable (last), return type SAME.
     4. config (application.properties): sliding-window-size, minimum-number-of-calls, failure-rate-threshold(%),
        wait-duration-in-open-state, permitted-number-of-calls-in-half-open-state. instance-naam = annotation ka "name".

   ★ TRY-CATCH HATANA pada: CB failures tabhi COUNT karta jab exception method se BAHAR aaye. purana SAGA
     try-catch use ANDAR hi kha jaata tha -> CB ko fail dikhta hi nahi. hataya -> fallback hi naya "catch" ban gaya.
     (self-invocation gotcha nahi: createOrder controller se call hota -> proxy ke through -> CB lagta.)

   TEST (payment-service BAND, POST localhost:8081/order baar-baar):
     call 1-3: log "Connection refused ...8082/pay" -> CB CLOSED, payment TRY kiya, fail -> fallback FAILED.
     call 4  : log "CircuitBreaker 'paymentService' is OPEN and does not permit further calls"
               -> CB OPEN (3 fail + 50%+ threshold) -> payment ko TRY KIYE BINA short-circuit. ★ yahi CB ka kaam.
     der baad: HALF-OPEN -> ek test call -> still down -> refused -> wapas OPEN.
     -> teeno state live: CLOSED -> OPEN -> HALF-OPEN -> OPEN.
     PROOF: "is OPEN...does not permit" wali line = CB ne dead payment ko chhua tak nahi (fail-fast).
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
