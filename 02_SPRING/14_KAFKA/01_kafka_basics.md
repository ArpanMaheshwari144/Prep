# KAFKA — hands-on (usercrud me live chalaya, 30-Aug)

> Producer/consumer + DLQ khud banaya usercrud me. LIVE dekha: message producer -> topic -> consumer,
> aur fail -> retry -> dead-letter. Ye note = jaise humne kiya, waisa hi (code + steps + why + jo dikkat aayi).
> Theory-compare alag: 06_COMPARES/14_kafka_vs_rabbitmq.md
>
> INDEX: 1 saar · 2 flow · 3 crux · 4 setup(clean) · 5 config(autoconfig) · 6 Boot-4 gotcha(root-cause+fix)
>        · 7 DLQ(retry+DLT) · 8 interview · 9 rerun · 10 aage

---

## 1. EK LINE (saar)
Producer topic me message DAALTA -> Consumer background me POLL karke UTHATA.
Beech me DIRECT call NAHI -> **decoupled**. Jodne wala dhaaga = **TOPIC NAME**.

## 2. Flow (analogy: naamzad DABBA / mailbox)
```
[Controller/Producer] --send--> [Topic "user-events"] --poll--> [@KafkaListener/Consumer]
  kafkaTemplate.send(TOPIC,msg)      (Kafka broker :9092)          listen(msg) -> print
```
- Producer: message topic me daal ke BHOOL jaata (kisko jaana = uski tension nahi)
- Consumer: usne bola "main ye topic sunूंga" (@KafkaListener topics="user-events")
- Match = TOPIC NAME same -> pahuncha. Naam alag -> kabhi na milta.

## 3. Teen CRUX (interview me poochte)
```
1. Consumer ko kaise pata message aaya?
   -> Consumer KHUD PULL karta (poll). Broker PUSH nahi karta.
      @KafkaListener andar-andar ek LOOP chala raha jo baar-baar broker se poochta.
2. Kisne bheja - kaise pata?
   -> Consumer ko pata NAHI, zaroorat bhi nahi. YE decoupling hai. (producer anonymous)
3. Kyun usi consumer ko mila?
   -> usne KHUD us topic ko subscribe kiya tha (@KafkaListener).
```

---

## 4. HANDS-ON — setup (usercrud, step-by-step)

### STEP 1 — Kafka container (Docker)
todoapp me pehle se `kafka` + `kafka-ui` container the (apache/kafka:3.8.0) -> unhe reuse kiya:
```
docker start kafka kafka-ui        # 9092 pe broker
```
(usercrud compose me apna kafka-block DAALA tha -> "name /kafka already in use" conflict -> COMMENT kar diya, purana reuse.)

### STEP 2 — Dependency (pom.xml) ✅ CLEAN
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-kafka</artifactId>
</dependency>
```
> ★ `spring-boot-starter-kafka` (na ki raw `spring-kafka`) — kyun, wo section 6 me. Ye starter library + Boot AUTOCONFIG dono laata.

### STEP 3 — PRODUCER (REST endpoint -> topic)
`controller/KafkaProducerController.java`:
```java
@RestController
public class KafkaProducerController {

    private final KafkaTemplate<String, String> kafkaTemplate;   // Spring AUTO inject karta
    private static final String TOPIC = "user-events";

    public KafkaProducerController(KafkaTemplate<String, String> kafkaTemplate) {
        this.kafkaTemplate = kafkaTemplate;
    }

    @PostMapping("/kafka/send")
    public String send(@RequestParam String message) {
        kafkaTemplate.send(TOPIC, message);    // topic me daal do -> bhool jao
        return "Sent to topic '" + TOPIC + "': " + message;
    }
}
```
+ SecurityConfig me permit: `.requestMatchers("/kafka/**").permitAll()` (JWT block na kare)

### STEP 4 — CONSUMER (@KafkaListener -> print)
`controller/KafkaConsumer.java`:
```java
@Component
public class KafkaConsumer {

    // topic pe naya message aate hi APNE-AAP chalta (background me sunta rehta)
    @KafkaListener(topics = "user-events", groupId = "usercrud-group")
    public void listen(String message) {
        System.out.println(">>> CONSUMED from user-events: " + message);
    }
}
```

---

## 5. CONFIG — clean/autoconfig way (application.properties)
Kafka ek ALAG process (Docker). App ko usse baat karne ki **WIRING** chahiye:
broker-address + serializer (bhejne) + deserializer (padhne) + consumer-group.
**Spring Boot ye SAB khud banata** — bas `application.properties` me batao:
```
spring.kafka.bootstrap-servers=localhost:9092
spring.kafka.consumer.group-id=usercrud-group
spring.kafka.consumer.auto-offset-reset=earliest
spring.kafka.consumer.key-deserializer=org.apache.kafka.common.serialization.StringDeserializer
spring.kafka.consumer.value-deserializer=org.apache.kafka.common.serialization.StringDeserializer
spring.kafka.producer.key-serializer=org.apache.kafka.common.serialization.StringSerializer
spring.kafka.producer.value-serializer=org.apache.kafka.common.serialization.StringSerializer
```
Isse Spring KHUD bana deta: `ProducerFactory`, `KafkaTemplate`, `ConsumerFactory`,
`ListenerContainerFactory`, `KafkaAdmin` — sab. Tu bas `KafkaTemplate` inject karta + `@KafkaListener` lagata.
- **serializer** = message -> bytes (bhejne se pehle) | **deserializer** = bytes -> message (padhte waqt)
- Ek line: config = tere simple code (send/@KafkaListener) aur asli Kafka-broker ke beech ka **pul** — jo Spring khud banata.

---

## 6. ★★ BOOT-4 GOTCHA — "itna manual config kyun?" (root-cause + fix)

> Ye section = ek asli engineer wali kahani (gussa -> root cause -> fix). Interview gold. Isko yaad rakhna.

**Dikkat:** Pehle raw `spring-kafka` dependency thi. App start pe error:
```
Parameter 0 of constructor ... KafkaTemplate ... could not be found
```
Majboori me `KafkaConfig` me ~60 line MANUAL config likhni padi (producerFactory, kafkaTemplate,
consumerFactory, listenerContainerFactory, kafkaAdmin sab khud). Ye Spring ka NORMAL feel NAHI —
khatka laga "itna code kyun? interview me koi nahi likhta."

**ROOT CAUSE (docs se confirm — Boot 4.0 modularization):**
- Boot 4.0 me autoconfiguration chhote per-tech modules me tod diya.
- Kafka autoconfig ab naye package me: `org.springframework.boot.kafka.autoconfigure`
  (Boot 3 me tha `org.springframework.boot.autoconfigure.kafka`).
- Wo autoconfig `spring-boot-starter-kafka` module ke andar aata hai.
- Humare paas tha **raw `spring-kafka`** -> sirf LIBRARY, Boot ka **autoconfig-module NAHI**.
  -> `KafkaAutoConfiguration` register hi nahi hua -> `KafkaTemplate` auto nahi bana.
```
raw  org.springframework.kafka : spring-kafka             -> library HI (autoconfig nahi)  X
Boot org.springframework.boot  : spring-boot-starter-kafka -> library + AUTOCONFIG          OK
```

**FIX (3 change):**
1. pom: `spring-kafka` -> `spring-boot-starter-kafka` (section 4 STEP 2).
2. `application.properties` me `spring.kafka.*` (section 5).
3. `KafkaConfig` se saare manual bean DELETE (producer/consumer factory, template, admin).

**Natija:** ~60 line config -> ~15 line (sirf DLQ ke 2 bean bache, section 7). `KafkaTemplate` Spring ne khud banaya.

**Sabaq:** (a) "zyada code" ki khunak = aksar version/dependency ka issue, Spring ki galti nahi.
(b) autoconfig = "library + Boot ka autoconfig-module" DONO chahiye. (c) log/screen = sach, guess nahi.

---

## 7. DLQ — Dead Letter Queue (retry -> dead-letter, LIVE drama)

### 7a. Problem jo DLQ solve karta
Consumer ko message mila, process karne gaya, **FAIL** ho gaya. 2 wajah:
- Message hi kharab (bad data, parse-fail) = "poison message"
- Ya downstream down tha (DB/API abhi nahi chal raha) = temporary

**Bina DLQ ke:** Kafka wahi message BAAR-BAAR deta rehta (commit nahi hua na) -> consumer usi kharab
message pe atka -> **poore topic ka flow ruk gaya**. Ek sada aam poore truck ko rok deta.

### 7b. Ilaaj — 2 step
```
1. RETRY: pehle 2-3 baar try karo (shayad temporary tha, DB wapas aa jaye)
2. Phir bhi fail -> ALAG topic me daal do = "user-events-dlt"
   -> main flow AAGE (unblocked)  |  kharab message side-room me park (baad me dekho)
```
**Anchor (postman):** package deliver nahi ho raha (galat address) -> 3 baar try -> phir poore route ko
rokne ke bajaye "undelivered mail room" me daal deta. Yahi DLQ. (DLT = Dead Letter Topic)

### 7c. CODE — 4 tukde

**(i) Consumer ko jaan-boojh ke FAIL karwana** (`KafkaConsumer.java`):
```java
@KafkaListener(topics = "user-events", groupId = "usercrud-group")
public void listen(String message) {
    System.out.println(">>> CONSUMED from user-events: " + message);
    if (message.contains("fail")) {                                  // poison message
        throw new RuntimeException("Poison message! Cannot process: " + message);
    }
}
```
> exception = "processing fail ho gaya" ka signal. Exception uthte hi error-handler jaagta.

**(ii) Error-handler = retry + DLT recoverer** (`KafkaConfig.java`):
```java
@Bean
public DefaultErrorHandler errorHandler(KafkaTemplate<String, String> kafkaTemplate) {
    DeadLetterPublishingRecoverer recoverer = new DeadLetterPublishingRecoverer(kafkaTemplate);  // DLT me publish karne wala
    FixedBackOff backOff = new FixedBackOff(1000L, 2L);   // 1 sec gap, 2 extra try (total 3)
    return new DefaultErrorHandler(recoverer, backOff);
}
```
> ★ Boot ki auto-container-factory ye `DefaultErrorHandler` bean **KHUD utha leti** — factory se jodna nahi padta.

**(iii) DLT listener — gire hue message pakdo** (`KafkaConsumer.java`):
```java
@KafkaListener(topics = "user-events-dlt", groupId = "usercrud-dlt-group")
public void listenDLT(String message) {
    System.out.println("XXX DEAD-LETTER (DLT) me gira: " + message);
}
```
> Bilkul pehle jaisa listener — bas ALAG topic + ALAG groupId. "undelivered mail room ka clerk".

**(iv) DLT topic banwa do** (`KafkaConfig.java`):
```java
@Bean
public NewTopic deadLetterTopic() {
    return new NewTopic("user-events-dlt", 1, (short) 1);   // naam, 1 partition, 1 replica
}
```
> auto `KafkaAdmin` startup pe `NewTopic` bean khud bana deta.

### 7d. ★ GOTCHA — DLT topic ka NAAM (galti jo pakdi)
Pehle socha default naam `user-events.DLT` hoga. **GALAT.** Log ne sach dikhaya:
```
DeadLetterPublishingRecoverer: ... partition user-events-dlt-0
{user-events-dlt=UNKNOWN_TOPIC_OR_PARTITION}
Record in retry and not yet recovered   (baar-baar = infinite loop)
```
Default suffix is version me = **`-dlt`** (hyphen), na `.DLT`. Aur topic exist na karne se publish fail ->
recover fail -> message infinite-retry me phasa. **Fix:** NewTopic se topic banwa do + listener naam `user-events-dlt` se match.

### 7e. LIVE test — jo dekha
```
curl -X POST "http://localhost:8080/kafka/send?message=hello"    -> ek baar CONSUMED, DLT me kuch nahi
curl -X POST "http://localhost:8080/kafka/send?message=failme"   -> ye sequence:
   >>> CONSUMED from user-events: failme      <- 1st try (fail)
   Record in retry and not yet recovered
   >>> CONSUMED from user-events: failme      <- retry 1
   >>> CONSUMED from user-events: failme      <- retry 2
   XXX DEAD-LETTER (DLT) me gira: failme      <- YAHI asli maal
```
3 try -> phir DLT me gira -> loop ruk gaya. Main flow saaf.

### 7f. FINAL KafkaConfig (autoconfig ke baad — sirf 2 bean)
```java
@Configuration
@EnableKafka
public class KafkaConfig {

    @Bean   // DLQ error-handler (retry -> DLT) — Boot ki auto-factory ise khud utha leti
    public DefaultErrorHandler errorHandler(KafkaTemplate<String, String> kafkaTemplate) {
        DeadLetterPublishingRecoverer recoverer = new DeadLetterPublishingRecoverer(kafkaTemplate);
        return new DefaultErrorHandler(recoverer, new FixedBackOff(1000L, 2L));
    }

    @Bean   // DLT topic — auto KafkaAdmin startup pe bana deta
    public NewTopic deadLetterTopic() {
        return new NewTopic("user-events-dlt", 1, (short) 1);
    }
}
```
> Sirf 2 bean — dono genuinely CUSTOM DLQ maal (normal project me bhi likhte). Baaki plumbing Spring ke haath me.

---

## 8. Interview lines
- "Kafka = pub-sub via topics; producer & consumer DECOUPLED, connected only by TOPIC NAME."
- "Consumer PULLS (polls) — broker doesn't push. @KafkaListener runs a poll loop in background."
- "Serializer = object->bytes on send; deserializer = bytes->object on consume."
- "DLQ = retry N times -> phir bhi fail -> DeadLetterPublishingRecoverer se `<topic>-dlt` me route; main consumer unblocked, poison message parked."
- "DefaultErrorHandler(recoverer, FixedBackOff) = retry policy + jahan gire."
- **Root-cause story:** "Boot 4 me raw `spring-kafka` autoconfig nahi laata — modularization ke baad autoconfig `spring-boot-starter-kafka` module me hai. Dependency-level pe root cause pakda, docs se confirm, ~60 line manual config ko 2 bean tak clean kiya."

## 9. Dobara kaise chalaye (LIVE test)
```
docker start kafka             # broker up (9092)
usercrud run                   # app
curl -X POST "http://localhost:8080/kafka/send?message=hello"    # -> CONSUMED
curl -X POST "http://localhost:8080/kafka/send?message=failme"   # -> 3 try -> DLT me gira
```

## 10. AAGE (baaki - TO_STUDY / jab man kare)
- Consumer-group (2 consumer -> partitions baant lete = load-split)
- Idempotent consumer (same message dobara -> duplicate na ho)
- Manual vs auto commit, offsets deep
