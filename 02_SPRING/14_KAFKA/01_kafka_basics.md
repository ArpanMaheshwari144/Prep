# KAFKA — hands-on (usercrud me live chalaya, 30-Aug)

> Producer/consumer khud banaya usercrud me. LIVE dekha: message producer -> topic -> consumer.
> Ye note = jaise humne kiya, waisa hi (code + steps + why). Theory-compare alag: 06_COMPARES/14_kafka_vs_rabbitmq.md

---

## 1. EK LINE (saar)
Producer topic me message DAALTA -> Consumer background me POLL karke UTHATA.
Beech me DIRECT call NAHI -> **decoupled**. Jodने wala dhaaga = **TOPIC NAME**.

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

## 4. HANDS-ON — jaise add kiya (usercrud, step-by-step)

### STEP 1 — Kafka container (Docker)
todoapp me pehle se `kafka` + `kafka-ui` container the (apache/kafka:3.8.0) -> unhe reuse kiya:
```
docker start kafka kafka-ui        # 9092 pe broker
```
(usercrud compose me apna kafka-block DAALA tha -> "name /kafka already in use" conflict -> COMMENT kar diya, purana reuse.)

### STEP 2 — Dependency (pom.xml)
```xml
<dependency>
    <groupId>org.springframework.kafka</groupId>
    <artifactId>spring-kafka</artifactId>
</dependency>
```
(version nahi -> Spring Boot parent BOM manage karta.)

### STEP 3 — PRODUCER (REST endpoint -> topic)
`controller/KafkaProducerController.java`:
```java
@RestController
public class KafkaProducerController {

    private final KafkaTemplate<String, String> kafkaTemplate;
    private static final String TOPIC = "user-events";

    public KafkaProducerController(KafkaTemplate<String, String> kafkaTemplate) {
        this.kafkaTemplate = kafkaTemplate;   // Spring inject karta
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

### STEP 5 — CONFIG (★ Boot-4 gotcha ke wajah se KHUD banayi)
`config/KafkaConfig.java`:
```java
@Configuration
@EnableKafka   // @KafkaListener ko chalu karta (autoconfig karta tha, ab hum)
public class KafkaConfig {

    // ===== PRODUCER side =====
    @Bean
    public ProducerFactory<String, String> producerFactory() {
        Map<String, Object> config = new HashMap<>();
        config.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, "localhost:9092");
        config.put(ProducerConfig.KEY_SERIALIZER_CLASS_CONFIG, StringSerializer.class);
        config.put(ProducerConfig.VALUE_SERIALIZER_CLASS_CONFIG, StringSerializer.class);
        return new DefaultKafkaProducerFactory<>(config);
    }
    @Bean
    public KafkaTemplate<String, String> kafkaTemplate() {
        return new KafkaTemplate<>(producerFactory());   // <- YE wahi bean jo missing tha
    }

    // ===== CONSUMER side =====
    @Bean
    public ConsumerFactory<String, String> consumerFactory() {
        Map<String, Object> config = new HashMap<>();
        config.put(ConsumerConfig.BOOTSTRAP_SERVERS_CONFIG, "localhost:9092");
        config.put(ConsumerConfig.GROUP_ID_CONFIG, "usercrud-group");
        config.put(ConsumerConfig.AUTO_OFFSET_RESET_CONFIG, "earliest");
        config.put(ConsumerConfig.KEY_DESERIALIZER_CLASS_CONFIG, StringDeserializer.class);
        config.put(ConsumerConfig.VALUE_DESERIALIZER_CLASS_CONFIG, StringDeserializer.class);
        return new DefaultKafkaConsumerFactory<>(config);
    }
    @Bean
    public ConcurrentKafkaListenerContainerFactory<String, String> kafkaListenerContainerFactory() {
        ConcurrentKafkaListenerContainerFactory<String, String> factory =
                new ConcurrentKafkaListenerContainerFactory<>();
        factory.setConsumerFactory(consumerFactory());
        return factory;   // @KafkaListener isi factory se container banata
    }
}
```

---

## 5. CONFIG samajh — kyun/kya/kaise
- **KYUN:** Kafka ek ALAG process (Docker). App ko usse baat karne ki **WIRING** chahiye:
  broker-address + serializer (bhejne) + deserializer (padhne) + consumer-group.
- **Normally Spring Boot AUTOCONFIG ye khud banata** (application.properties se padh ke).
  ★★ **Spring Boot 4.0 me `KafkaAutoConfiguration` register hi NAHI hota** (condition-report me hai hi nahi)
     -> `KafkaTemplate` bean missing -> "bean of type KafkaTemplate could not be found" error.
     -> humne **khud 4 beans banaye** (autoconfig ka andar khol ke dekha).
- **4 beans (2 producer, 2 consumer):**
  ```
  ProducerFactory  = "bhejne wali machine" ki factory (broker + serializer)
  KafkaTemplate    = us factory se producer -> easy send() -> CONTROLLER use karta
  ConsumerFactory  = "padhne wali machine" ki factory (broker + group + deserializer)
  ListenerContainerFactory = @KafkaListener isse background-container banata (poll karta)
  ```
- **serializer** = message -> bytes (bhejne se pehle) | **deserializer** = bytes -> message (padhte waqt)
- **KAISE:** @Configuration -> Spring beans banata; @Bean -> return = bean; phir Spring KHUD inject karta
  (KafkaTemplate -> Controller ke constructor me; ListenerContainerFactory -> @KafkaListener use karta).

Ek line: config = tere simple code (send/@KafkaListener) aur asli Kafka-broker ke beech ka **pul**.

---

## 6. Interview lines
- "Kafka = pub-sub via topics; producer & consumer DECOUPLED, connected only by TOPIC NAME."
- "Consumer PULLS (polls) — broker doesn't push. @KafkaListener runs a poll loop in background."
- "Spring Kafka internally: ProducerFactory->KafkaTemplate (send), ConsumerFactory->ListenerContainerFactory (@KafkaListener)."
- "Serializer = object->bytes on send; deserializer = bytes->object on consume."

## 7. Dobara kaise chalaye (LIVE test)
```
docker start kafka             # broker up (9092)
usercrud run                   # app
curl -X POST "http://localhost:8080/kafka/send?message=hello"
-> app console: ">>> CONSUMED from user-events: hello"
```
message producer -> topic -> consumer, live. (2-3 message bhej ke dekh.)

## 8. AAGE (baaki hai - TO_STUDY / jab man kare)
- Retry + DLQ (fail -> dead-letter topic, drama)
- Consumer-group (2 consumer -> partitions baant lete = load-split)
- Idempotent consumer (same message dobara -> duplicate na ho)
