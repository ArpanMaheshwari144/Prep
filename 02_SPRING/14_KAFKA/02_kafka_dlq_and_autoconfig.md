# KAFKA — DLQ + AUTOCONFIG FIX (usercrud me live, 30-Aug)

> 01_kafka_basics.md ke BAAD ka safar. 3 cheez ek saath:
> (1) DLQ (retry -> dead-letter) live banaya
> (2) Boot-4 me manual-config kyun likhna pad raha tha -> ROOT CAUSE pakda
> (3) Fix kiya -> saara manual config DELETE -> clean autoconfig version
> Ye poora arc = ek asli engineer wali kahani (gussa -> root cause -> fix). Interview gold.

---

## PART 1 — DLQ (Dead Letter Queue)

### 1a. Problem jo DLQ solve karta
Consumer ko message mila, process karne gaya, **FAIL** ho gaya. 2 wajah:
- Message hi kharab (bad data, parse-fail) = "poison message"
- Ya downstream down tha (DB/API us waqt nahi chal raha) = temporary

**Bina DLQ ke:** Kafka wahi message BAAR-BAAR consumer ko deta rehta (commit nahi hua na).
Consumer usi ek kharab message pe atka reh jaata -> **poore topic ka flow ruk gaya**.
Ek sada aam poore truck ko rok deta.

### 1b. DLQ ka ilaaj — 2 step
```
1. RETRY: pehle 2-3 baar try karo (shayad temporary tha, DB wapas aa jaye)
2. Phir bhi fail -> us message ko ALAG topic me daal do = "user-events-dlt"
   -> main flow AAGE badh gaya (unblocked)
   -> kharab message side-room me park (baad me dekho kyun fail hua)
```

**Anchor (postman):** courier package deliver nahi kar pa raha (galat address). 3 baar try karta.
Phir bhi nahi -> poore route ko rokne ke bajaye package ko **"undelivered mail room"** me daal deta.
Baaki delivery chalti rehti. Yahi DLQ = Kafka ka undelivered-mail room. (DLT = Dead Letter Topic)

---

### 1c. CODE — jaise banaya (3 tukde)

**(i) Consumer ko jaan-boojh ke FAIL karwana** (`controller/KafkaConsumer.java`):
```java
@KafkaListener(topics = "user-events", groupId = "usercrud-group")
public void listen(String message) {
    System.out.println(">>> CONSUMED from user-events: " + message);

    if (message.contains("fail")) {                                  // poison message
        throw new RuntimeException("Poison message! Cannot process: " + message);
    }
}
```
> Kafka/Spring ke liye **exception = "processing fail ho gaya"** ka signal. Exception uthte hi error-handler jaagta.

**(ii) Error-handler = retry + DLT recoverer** (`config/KafkaConfig.java`):
```java
@Bean
public DefaultErrorHandler errorHandler(KafkaTemplate<String, String> kafkaTemplate) {
    // FAIL hone pe kahan bhejna: DLT topic me publish karne wala
    DeadLetterPublishingRecoverer recoverer = new DeadLetterPublishingRecoverer(kafkaTemplate);

    // 2 baar retry, har baar 1 sec gap; phir bhi fail -> recoverer (DLT)
    FixedBackOff backOff = new FixedBackOff(1000L, 2L);

    return new DefaultErrorHandler(recoverer, backOff);
}
```
- `DeadLetterPublishingRecoverer(kafkaTemplate)` = "DLT me publish karne wala" -> isiliye `kafkaTemplate` chahiye (bhejne ka tool).
- `FixedBackOff(1000, 2)` = retry policy: 1000ms gap, 2 extra attempts (total 3 tries = 1 original + 2 retry).
- ★ Autoconfig-version me: Boot ki auto-container-factory ye `DefaultErrorHandler` bean **khud utha leti** -> humein factory se jodna nahi padta.

**(iii) DLT listener — gire hue message pakdo** (`controller/KafkaConsumer.java`):
```java
@KafkaListener(topics = "user-events-dlt", groupId = "usercrud-dlt-group")
public void listenDLT(String message) {
    System.out.println("XXX DEAD-LETTER (DLT) me gira: " + message);
}
```
> Bilkul waisa hi listener jaisa pehla — bas ALAG topic (`user-events-dlt`) + ALAG groupId. "undelivered mail room ka clerk".

**(iv) DLT topic banwa do** (`config/KafkaConfig.java`):
```java
@Bean
public NewTopic deadLetterTopic() {
    return new NewTopic("user-events-dlt", 1, (short) 1);   // naam, 1 partition, 1 replica
}
```
> `KafkaAdmin` (autoconfig me auto) startup pe `NewTopic` bean khud bana deta.

---

### 1d. ★★ GOTCHA — DLT topic ka NAAM (galti jo pakdi)
Pehli baar maine socha default naam `user-events.DLT` hoga. **GALAT.** Log ne sach dikhaya:
```
DeadLetterPublishingRecoverer: ... partition user-events-dlt-0
{user-events-dlt=UNKNOWN_TOPIC_OR_PARTITION}
```
Is version ka default suffix = **`-dlt`** (hyphen), na ki `.DLT`. Aur us topic ke EXIST na karne se:
```
Record in retry and not yet recovered   (baar-baar)
```
= message DLT me publish nahi ho pa raha -> recover fail -> **infinite retry loop** me phas gaya.
**Fix:** (a) `NewTopic` bean se topic banwa do, (b) listener ka naam `user-events-dlt` se match karo.
**Sabaq:** log = sach. Screen pe jo literally dikhe wahi naam, apna guess nahi.

### 1e. LIVE test — jo dekha
```
curl -X POST "http://localhost:8080/kafka/send?message=hello"    -> ek baar CONSUMED, DLT me kuch nahi

curl -X POST "http://localhost:8080/kafka/send?message=failme"   -> ye sequence:
   >>> CONSUMED from user-events: failme      <- 1st try (fail)
   Record in retry and not yet recovered
   >>> CONSUMED from user-events: failme      <- retry 1
   Record in retry and not yet recovered
   >>> CONSUMED from user-events: failme      <- retry 2
   XXX DEAD-LETTER (DLT) me gira: failme      <- YAHI asli maal
```
3 try -> phir DLT me gira -> loop ruk gaya. Main flow saaf.

---

## PART 2 — AUTOCONFIG FIX (manual config kyun tha -> delete kaise hua)

### 2a. Dikkat jo khatak rahi thi
DLQ tak sab kaam kar gaya, PAR `KafkaConfig` me itna manual code likhna pad raha tha:
producerFactory, kafkaTemplate, consumerFactory, listenerContainerFactory, kafkaAdmin...
**Ye Spring ka normal experience NAHI.** Normally 3-4 line `application.properties` me daalo, bas.
Sawaal: itna manual code kyun? Interview me koi nahi likhta.

### 2b. ROOT CAUSE (docs se confirm — Boot 4.0 modularization)
- Spring Boot 4.0 me ek **bada refactor**: autoconfiguration chhote-chhote per-tech modules me tod diya.
- Kafka ka autoconfig ab naye package me: `org.springframework.boot.kafka.autoconfigure`
  (Boot 3 me tha `org.springframework.boot.autoconfigure.kafka`).
- Ye autoconfig ek ALAG module (`spring-boot-starter-kafka`) ke andar aata hai.
- Humare pom me tha **raw `spring-kafka`** -> wo sirf LIBRARY laata hai, Boot ka **autoconfig-module NAHI**.
- Isiliye `KafkaAutoConfiguration` register hi nahi hua (condition-report me "matched" aur "negative-matches"
  dono me absent) -> `KafkaTemplate` bean auto nahi bana -> "bean could not be found" -> sab haath se likhna pada.

```
raw  org.springframework.kafka : spring-kafka          -> library HI (autoconfig nahi)  ❌
Boot org.springframework.boot  : spring-boot-starter-kafka -> library + AUTOCONFIG      ✅
```

### 2c. FIX — 3 chhote change

**(1) pom.xml — dependency swap** (raw spring-kafka hataya):
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-kafka</artifactId>
</dependency>
```

**(2) application.properties — jo pehle beans me tha, ab config me:**
```
spring.kafka.bootstrap-servers=localhost:9092
spring.kafka.consumer.group-id=usercrud-group
spring.kafka.consumer.auto-offset-reset=earliest
spring.kafka.consumer.key-deserializer=org.apache.kafka.common.serialization.StringDeserializer
spring.kafka.consumer.value-deserializer=org.apache.kafka.common.serialization.StringDeserializer
spring.kafka.producer.key-serializer=org.apache.kafka.common.serialization.StringSerializer
spring.kafka.producer.value-serializer=org.apache.kafka.common.serialization.StringSerializer
```

**(3) KafkaConfig.java — SAB manual bean DELETE** (producerFactory / kafkaTemplate /
consumerFactory / kafkaListenerContainerFactory / kafkaAdmin). Sab auto ho gaya.

### 2d. FINAL clean KafkaConfig (~60 line -> ~15 line)
```java
@Configuration
@EnableKafka
public class KafkaConfig {

    // DLQ error-handler (retry -> DLT) — Boot ki auto-factory ise KHUD utha leti
    @Bean
    public DefaultErrorHandler errorHandler(KafkaTemplate<String, String> kafkaTemplate) {
        DeadLetterPublishingRecoverer recoverer = new DeadLetterPublishingRecoverer(kafkaTemplate);
        return new DefaultErrorHandler(recoverer, new FixedBackOff(1000L, 2L));
    }

    // DLT topic — auto KafkaAdmin ise startup pe bana deta
    @Bean
    public NewTopic deadLetterTopic() {
        return new NewTopic("user-events-dlt", 1, (short) 1);
    }
}
```
> Sirf 2 bean bache — dono genuinely CUSTOM DLQ maal (ye normal project me bhi likhte).
> Baaki plumbing (template/factories/admin) Spring ke haath me wapas.

### 2e. Result verify
Swap ke baad rebuild+run -> `failme` bheja -> wahi output (3 try -> DLT me gira),
PAR ab peeche saara manual config gayab. `KafkaTemplate` Spring ne khud banaya. = "worth it" version.

---

## PART 3 — INTERVIEW / TAKEAWAY

- "Kafka DLQ = retry N times -> phir bhi fail -> DeadLetterPublishingRecoverer se `<topic>-dlt` me route.
   Main consumer unblocked, poison message parked for inspection."
- "DefaultErrorHandler(recoverer, FixedBackOff) = retry policy + jahan gire."
- **Root-cause story (asli engineer signal):** "Boot 4 me raw `spring-kafka` autoconfig nahi laata —
   modularization ke baad autoconfig `spring-boot-starter-kafka` module me hai. Maine dependency-level pe
   root cause pakda, docs se confirm kiya, aur ~60 line manual config ko 2 bean tak clean kiya."
- Sabaq: (a) log/screen = sach, guess nahi. (b) 'zyada code' ki khunak = aksar version/dependency ka issue,
   Spring ki galti nahi. (c) autoconfig = "library + Boot ka autoconfig-module" dono chahiye.

## PART 4 — AAGE (baaki, jab man kare)
- Consumer-group (2 consumer -> partitions baant lete = load-split)
- Idempotent consumer (same message dobara -> duplicate na ho)
- Manual vs auto commit, offsets deep
