# Mini Payment — MICROSERVICES (kal ek-dum-shuru ke liye)

> GOAL: **microservice KAISE kaam karti hai** samajhna (code sikhna nahi — controller/service/repo tu jaanta).
> Asli seekh = **do alag service AAPAS ME BAAT karti hain**. Bas itna, saral.

---

## STEP 0 — 2 service generate karo (start.spring.io, ~3 min each)

```
   https://start.spring.io  pe jao, ye settings:
     Project   : Maven
     Language  : Java
     Spring Boot: 3.x (latest stable — 4.x-preview NAHI)
     Java      : 17
     Dependencies (sirf 3):  Spring Web  ·  Spring Data JPA  ·  H2 Database

   2 baar generate karo:
     1. artifact = order-service
     2. artifact = payment-service
   dono ko is folder (mini_payment_ms/) me unzip karo.
```

---

## STEP 1 — payment-service (chhota, pehle isko khada karo)

```
   src/main/resources/application.properties:
     server.port=8082
     spring.h2.console.enabled=true
     spring.datasource.url=jdbc:h2:mem:paydb

   ek endpoint:  POST /pay   { orderId, amount }  ->  "payment done for <orderId>"
   (controller -> service -> bas ek print/return. DB abhi optional.)
   RUN -> browser/Postman se POST /pay test -> chalna chahiye. (akeli service = normal Spring, ye tu jaanta.)
```

## STEP 2 — order-service (port 8081)

```
   application.properties:  server.port=8081  ·  spring.datasource.url=jdbc:h2:mem:orderdb
   ek endpoint:  POST /order   { item, amount }  ->  order banao (abhi memory/DB me)
   RUN -> POST /order test -> chalna chahiye.
```

## STEP 3 — ★ ASLI SEEKH: order → payment ko CALL karvao

```
   jab POST /order aaye, order-service KHUD payment-service ko call kare (POST /pay).
   TOOL: RestTemplate (simple) — order-service me:
       restTemplate.postForObject("http://localhost:8082/pay", body, String.class)

   FLOW jo SEE karega:
     Postman -> POST /order (8081) -> order-service andar-andar -> POST /pay (8082) -> payment-service
                                    <- "order placed + payment done" wapas

   YAHI microservice ka DIL: ek service ne doosri ko network pe call kiya.
```

---

## SAMAJHNE KE POINTS (yahi seekhna hai, code nahi)

```
   1. DB-per-service : order ka apna DB (orderdb), payment ka apna (paydb). ALAG kyun?
                       -> fault-isolation + independent scale. ek service ka DB doosre se coupled nahi.
   2. alag PORT      : 8081 vs 8082 -> do alag process/service, ek machine pe abhi (baad me alag machine).
   3. inter-service  : order ne payment ko HTTP se call kiya (RestTemplate). yehi "services talk".
   4. failure        : payment-service BAND karke POST /order maar -> order-service ko error aayega
                       -> YAHIN resilience samajh aayega (retry/circuit-breaker ki zaroorat kyun).
```

---

## KAL KA FLOW (Claude ke saath)

```
   1. Claude pehle CONCEPT padhaayega (2 service kaise baat karti — teach-first).
   2. tu generate + step 1,2,3 karega (main minimal, tu code — skeleton tu jaanta).
   3. STEP 3 (inter-service call) pe rukenge -> woh SEE karega chalte hue -> yahi asli cheez.
   4. failure-case try -> resilience samajh -> bas. (~1-2 ghante.)
```

> Naya sirf: **inter-service call + DB-per-service + failure**. Baaki (controller/service/repo) tera purana.

---

## PHASE 2 (aage — coming weeks, phase-1 poora hone ke BAAD)

```
   REASON: Arpan HLD me Kafka/LB "bol" sakta, par CODE me kaise kaam karta — nahi pata (recognition≠implementation gap).
           project me add karke woh band hoga: "bol sakta" -> "bana sakta". JP-relevant bhi.

   ADD (ek-ek karke, phase-1 ke baad):
     1. @FeignClient : RestTemplate ki jagah DECLARATIVE inter-service call (interface likho, Spring call banata).
                       phase-1 me RestTemplate (explicit, seekhne ke liye) -> phase-2 me Feign (clean/real-world).
     2. KAFKA        : payment -> notification ASYNC event (sync REST ki jagah). code-level: producer/consumer/topic.
     3. API GATEWAY  : ek entry-point (Spring Cloud Gateway) — routing + cross-cutting.
     4. LB / CIRCUIT-BREAKER : resilience.

   ★ abhi NAHI (phase-1 khinch jaayega). pehle basic MS (order+payment+call+failure) done karo.
```
