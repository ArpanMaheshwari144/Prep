# KAFKA — hands-on (usercrud me live chalaya, 30-Aug)

> Producer/consumer + DLQ khud banaya usercrud me. LIVE dekha: message producer -> topic -> consumer,
> aur fail -> retry -> dead-letter. Ye note = jaise humne kiya, waisa hi (code + steps + why + jo dikkat aayi).
> Theory-compare alag: 06_COMPARES/14_kafka_vs_rabbitmq.md
>
> INDEX: **0 KYUN Kafka + har shabd kis dikkat se (pehle ye padho)** · 1 saar · 2 flow · 3 crux · 4 setup(clean)
>        · **4B broker-infra(KRaft/localhost-vs-kafka/retry-spam)**
>        · 5 config(autoconfig) · 6 Boot-4 gotcha(root-cause+fix) · 7 DLQ(retry+DLT) · 7B consumer-group/partitions
>        · 7C idempotent · 8 interview · 9 rerun · 10 aage

---

## 0. KYUN KAFKA + HAR SHABD KIS DIKKAT SE PAIDA HUA

> Poore section me ek hi misaal: **Order Service** har order pe event bhejti hai,
> **SMS / Email / Fraud / Analytics** use padhte hain.

### 0A. Kafka ke BINA — seedhi call ki 3 dikkat
```
User "order place"
   |
[Order Service] --call--> [Payment]
               --call--> [Inventory]
               --call--> [SMS]
               --call--> [Email]
               --call--> [Analytics]
```
```
1. SLOW            user tab tak ruka jab tak paanchon jawab na de. SMS 3 sec le -> order bhi 3 sec ka.
2. EK GIRA SAB GIRA SMS down -> order bhi fail, jabki SMS order ke liye zaroori hi nahi tha.
3. JAKDA HUA       kal "Fraud" chahiye -> Order Service ka code badlo + deploy. Order ko har sunne wale ka pata ho.
+  JHATKA          sale me 1 lakh order / min -> neeche wali services seedha dab jaati.
```

### 0B. Kafka ke SAATH — beech me ek dabba
```
[Order Service] --"order placed" event--> [ KAFKA  topic "order-events" ]
                                            |       |        |        |
                                          [SMS]  [Email] [Analytics] [Fraud]    (har koi khud padhta)
```
```
1. TEZ             event daala -> user ko turant "order ho gaya". baaki kaam peeche.
2. EK GIRA TO BAAKI CHALTE  SMS down -> uske events Kafka me pade rehte, wapas aaya to wahin se padh leta.
3. KHULA HUA       Fraud jodna = bas topic sunna shuru kare. Order Service ka code nahi chhoona.
+  JHATKA SAMBHALTA 1 lakh order Kafka me jama, consumer apni speed se nikaalte.
```
**EK LINE:** Kafka = services ke beech ka dabba, taaki BHEJNE wala aur SUNNE wala ek-doosre pe tike na rahein.

### 0C. Kya Kafka "MQ" hai? — bolchaal me haan, asal me LOG
```
RabbitMQ (asli QUEUE) = POST OFFICE
   chitthi di -> pahunchi -> ack -> GAYAB

Kafka (LOG)           = TAPE RECORDER / diary
   har event LIKHA jaata aur RUKTA hai (retention tak, jaise 7 din)
   har sunne wala apna BOOKMARK (offset) rakhta
   chahe to peeche jaake dobara padhe (REPLAY)
```
Isi wajah se: ek hi event SMS, Email, Fraud **teeno alag-alag** padh paate (har group ka apna bookmark),
aur crash ke baad service bookmark se wapas shuru karti, message khota nahi.
```
bahut events, kai sunne wale, replay chahiye        -> KAFKA
ek kaam ek worker ko, complex routing, deliver+khatam -> RABBITMQ   (poora: 06_COMPARES/14_kafka_vs_rabbitmq.md)
```

### 0D. Har shabd = ek dikkat ka ilaaj

**YAAD KI TASVEER = AKHBAAR (newspaper):**
```
reporter khabar bhejta            = PRODUCER
akhbaar ka daftar (chhapta + archive rakhta) = BROKER
section: sports / business        = TOPIC
ek section ke kai printing machine = PARTITION
page number                       = OFFSET
padhne wala                       = CONSUMER
ek GHAR ke log ek akhbaar BAANT ke padhte (papa sports, mummy business) = CONSUMER GROUP
alag ghar = apni POORI copy        = alag groupId
```

**1. PRODUCER = bhejne wala**
```
[Order Service]  --"order #101 placed"-->
```
Jo event Kafka me DAALTA. Sirf ROLE hai, alag machine nahi — tera Spring app `kafkaTemplate.send(...)` karke producer ban jaata.
Kyun: bhejne wala event daal ke aage badhe, rukna na pade.

**2. CONSUMER = padhne wala**
```
              --> [SMS Service]    (@KafkaListener)
              --> [Fraud Service]
```
Jo event Kafka se PADHTA (khud PULL / poll karta, broker push nahi karta). Ye bhi ROLE — ek app producer + consumer dono ho sakta.
Kyun: har service apni speed se padhe.

**3. BROKER = beech ka server, events disk pe rakhta**
DIKKAT: producer ne bheja, consumer us waqt band hai — event rakhega kaun?
```
[Producer] ---> [ BROKER (Kafka server :9092) ] ---> [Consumer]
                  events DISK pe likhe rehte
```
Broker = Kafka ka asli server (Docker wala container). Producer / consumer sirf broker se baat karte, ek-doosre se KABHI nahi.
Asli setup = 3+ broker (CLUSTER). Ek mara -> doosre ke paas COPY (replication), event nahi khota.

**4. TOPIC = events ka naamzad khaana**
DIKKAT: broker me order, payment, user sab aa raha. SMS ko sirf order wale chahiye.
```
BROKER
  topic "order-events"    : #101 #102 #103 ...
  topic "payment-events"  : ...
  topic "user-events"     : ...
```
Producer: "order-events me daalo". Consumer: "main order-events sunta hoon". Jodne wala dhaaga = sirf NAAM.

**5. PARTITION = ek topic ke andar kai LINE**
DIKKAT: 1 lakh order / sec. Ek hi line = ek machine likhe + ek consumer padhe = BOTTLENECK.
```
topic "order-events"
  P0: #101 #104 #107 ...
  P1: #102 #105 #108 ...
  P2: #103 #106 #109 ...
```
3 line -> alag broker pe reh sakti + 3 consumer ek saath padhein = 3 guna tez.
KEEMAT: order sirf EK LINE ke andar pakka, poore topic me nahi.
-> ek user ke saare events ek line me chahiye? **key = userId** do. Same key -> same partition -> order bana rehta.
(partition kaun chunta + bina key kya hota = 7B-e)

**6. OFFSET = line me event ka NUMBER (bookmark)**
DIKKAT: consumer crash hua, wapas aaya — kahan se padhe? Shuru se = SMS dobara jaayega. Aage se = beech ke chhoot jaayenge.
```
P0:  [0] #101   [1] #104   [2] #107   [3] #110
                             ^
                 SMS group ne yahan tak padha -> offset 2 COMMIT kiya
                 crash -> wapas -> 3 se shuru
```
Har event ko line me number: 0, 1, 2... Consumer padh ke bolta "2 tak pahuncha" (COMMIT).
Isliye Kafka padhne ke baad event DELETE nahi karta — har reader apna bookmark rakhta.

**7. CONSUMER GROUP = ek kaam karne wali TEAM**
DIKKAT (do alag):
```
(a) SMS ka ek instance 1 lakh/sec nahi jhel sakta -> 3 instance chahiye.
    par teeno ko wahi event mila to 3 SMS jaayenge.
(b) SMS aur Fraud DONO ko HAR event chahiye.
```
```
group "sms-service"  (3 instance)          group "fraud-service" (1 instance)
   A <- P0   B <- P1   C <- P2                X <- P0, P1, P2
   kaam BAANTA, har event ek hi baar          ise bhi SAB events, apni copy
```
```
group ke ANDAR -> partition baante, ek partition = EK hi member -> duplicate nahi   = (a) ka ilaaj
ALAG groups    -> har group SAB padhta, apne offset ke saath                     = (b) ka ilaaj
-> isliye OFFSET har GROUP ka alag: SMS group P0 me 2 pe, Fraud group 50 pe ho sakta.
```

### 0E. Poori tasveer ek saath
```
[Order Service]                      BROKER CLUSTER
  PRODUCER  --key=userId-->   topic "order-events"
                                P0: 0 1 2 3 ...   --> group sms:  A   | group fraud: X
                                P1: 0 1 2 ...     --> group sms:  B   | group fraud: X
                                P2: 0 1 2 3 4 ... --> group sms:  C   | group fraud: X
                              (har group ka har partition pe APNA offset)
```

### 0F. Ek-ek line (revise ke liye bas ye)
```
producer  = event daalne wala               -> bhejne wala ruke nahi
consumer  = event padhne wala (pull)        -> apni speed se padhe
broker    = beech ka server, disk pe rakhta -> koi band ho to bhi event bacha rahe
topic     = events ki category              -> sirf apne kaam ka sune
partition = topic ki kai line               -> load baante, parallel chale (order sirf line ke andar)
offset    = line me event ka number         -> crash ke baad wahin se shuru
group     = ek kaam karne wali team         -> andar baanto, alag group ko poori copy
```

### 0G. BOL (interview)
```
"Kafka decouples services. The producer writes an event and moves on, and every interested service
 reads it independently, at its own speed. If a consumer goes down, the events stay in Kafka and it
 resumes from its offset. Technically it's a distributed append-only log, not a classic queue:
 messages are retained, so multiple consumer groups can read the same events, and they can be replayed."
```

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
- Consumer: usne bola "main ye topic sununga" (@KafkaListener topics="user-events")
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
usercrud ke apne `docker-compose.yml` me bhi kafka-block hai (KRaft single-node). Broker chalu karne ka clean tareeka + poori BROKER-INFRA samajh = **section 4B** (9-Sep me detail me kiya).

### STEP 2 — Dependency (pom.xml) ✅ CLEAN
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-kafka</artifactId>
</dependency>
```
> ★ `spring-boot-starter-kafka` (na ki raw `spring-kafka`) — kyun, wo section 6 me. Ye starter library + Boot AUTOCONFIG dono laata.

### STEP 3 — PRODUCER (REST endpoint -> topic)
[`controller/KafkaProducerController.java`](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/KafkaProducerController.java):
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
[`controller/KafkaConsumer.java`](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/KafkaConsumer.java):
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

## 4B. ★★ BROKER INFRA — Docker/KRaft, localhost-vs-kafka, retry-spam (9-Sep hands-on)

> Ab tak upar wale sections = "app-side" (producer/consumer/DLQ). Ye section = "broker-side" —
> Kafka KHUD kaise chalta, app usse kaise connect hota, aur ek asli galti (retry-spam) ka root-cause.
> Ye interview me "Kafka setup/infra" wale sawaal + real-debug story dono deta.

### 4B-a. Kafka broker = ALAG process (app ke andar nahi)
```
Spring Boot app  =/=  Kafka.
Kafka ek ALAG server (broker) hai jo Docker container me chalta, :9092 pe sunta.
App usse NETWORK pe baat karta (bootstrap-servers=...:9092).
-> App chal jaaye par broker band ho -> app baar-baar connect try karega (retry-spam, 4B-d).
```

### 4B-b. KRaft — single-node broker config (zookeeper-free)
Purana Kafka ko ek alag **Zookeeper** chahiye tha (coordination ke liye). Naya Kafka = **KRaft** mode:
broker KHUD apna coordinator (controller) ban jaata -> zookeeper ki zaroorat KHATAM. Single-node ke liye perfect.

`docker-compose.yml` ka kafka-block (har line ka matlab):
```yaml
kafka:
  image: apache/kafka:3.8.0
  container_name: kafka
  ports:
    - "9092:9092"                                    # host:container -> Boot yahin connect
  environment:
    KAFKA_NODE_ID: 1                                 # is node ki id
    KAFKA_PROCESS_ROLES: broker,controller           # ★ KRaft: ek hi node = broker + controller dono
    KAFKA_LISTENERS: PLAINTEXT://0.0.0.0:9092,CONTROLLER://0.0.0.0:9093
                                                     # andar kaha-kaha sunega (9092 client, 9093 controller)
    KAFKA_ADVERTISED_LISTENERS: PLAINTEXT://localhost:9092   # ★ "client mujhe IS pate pe dhoondhe" (4B-c)
    KAFKA_CONTROLLER_LISTENER_NAMES: CONTROLLER
    KAFKA_CONTROLLER_QUORUM_VOTERS: 1@localhost:9093 # controller-election: node-1, 9093 pe
    KAFKA_LISTENER_SECURITY_PROTOCOL_MAP: CONTROLLER:PLAINTEXT,PLAINTEXT:PLAINTEXT
    KAFKA_OFFSETS_TOPIC_REPLICATION_FACTOR: 1        # single-node -> replica 1 (warna topic banega hi nahi)
```
> ★ Sabse zaroori jodi: `PROCESS_ROLES=broker,controller` + `CONTROLLER_QUORUM_VOTERS`.
>   Ye MISSING ho -> broker ka controller set hi nahi hota -> broker theek se start nahi hota (4B-d ka asli kaaran).

**Broker chalu karne ka CLEAN tareeka (project ke compose se):**
```
docker compose up -d kafka
# agar "name /kafka already in use" -> purana container hata ke phir up:
docker rm -f kafka
docker compose up -d kafka
```

### 4B-c. ★★ ADVERTISED_LISTENERS + "localhost vs kafka:9092" (#1 confusion)
Ye Kafka ka sabse tricky config. **advertised.listeners = wo pata jo broker CLIENT ko batata "mujhe yahan connect karo".**
```
Client pehle broker se poochta "tera address?" -> broker jo advertised-listener bola, WAHI client use karta.
   advertised = localhost:9092  -> client "localhost:9092" pe connect karega.
```
Ab **localhost ka matlab = "jo poochh raha hai, usi ka apna machine/container":**
```
App HOST pe (IntelliJ se chala)      -> localhost = teri Windows -> kafka wahin (9092)  -> SAHI (abhi yahi)
App CONTAINER me (compose profile)   -> localhost = app-ka-apna-container -> kafka wahan NAHI -> CONNECT FAIL
                                        (container me kafka ka DNS-naam = "kafka:9092", localhost nahi)
```
Isiliye tune `application-compose.properties` me `spring.kafka.bootstrap-servers=kafka:9092` daala (9-Sep).
Aur agar sach me app-in-container chalaye -> compose me `KAFKA_ADVERTISED_LISTENERS` bhi `kafka:9092` karna padega.
**Anchor:** advertised-listener = broker ka "visiting card" pe likha address. Card pe "localhost" likha to jo bhi padhega
apne-ghar samajhega -> host se theek, doosre container se galat.

### 4B-d. ★ REAL BUG — "Rebootstrapping" retry-spam (root-cause + sabak)
**Kya hua:** logs me baar-baar spam:
```
[AdminClient] Rebootstrapping ... / Connection to node -1 could not be established
```
**Galti (meri):** noise-fix ke naam pe maine `docker run -p 9092:9092 apache/kafka` chala diya — BINA KRaft env-vars ke.
-> `PROCESS_ROLES`/`QUORUM_VOTERS` missing -> controller null -> broker properly up NAHI -> app connect na kar paaya -> spam.
**Fix:** us aadhe-config broker ko hata, project ke apne compose-block (poore KRaft config wala) se broker uthaya (4B-b).
Phir "partitions assigned" log aaya = kaam ho gaya (4B-e).

**★ Asli root-cause (Arpan ka sawaal tha "dikkat commenting se thi?"):**
```
NAHI. compose-block comment hona = broker AUTO-start nahi hua (config-on-demand), bas.
Kuch TOOTA nahi tha. Asli dikkat = app-code Kafka expect kar raha tha PAR koi
   theek-se-configured broker chal hi nahi raha tha -> retry-spam.
-> Theek-configured broker chalu karte hi (compose se) -> spam khatam, connect ho gaya.
```
**Sabaq:** (a) Kafka container ALWAYS poore KRaft env ke saath uthao; adhoora `docker run` = controller-less = spam.
(b) "spam/connect-fail" = pehle dekho broker sach me UP + sahi-config hai kya (na ki app-code me kuch toota).
(c) log = sach; "Rebootstrapping" seedha bata raha tha "broker mil hi nahi raha".

### 4B-e. ★ "partitions assigned" log = sab theek (kaise padhe)
Jab broker sahi chala + app connect hua, ye log aaya:
```
usercrud-group:     partitions assigned: [user-events-0, user-events-1, user-events-2]
usercrud-dlt-group: partitions assigned: [user-events-dlt-0]
```
Matlab:
```
- broker mil gaya + topics maujood hain + consumers subscribe ho gaye.
- "user-events" ke 3 partition (0,1,2) -> concurrency=3 ke 3 thread ko baant diye = REBALANCE/assignment.
- "user-events-dlt" ka partition DLT-consumer ko mila -> dead-letter path bhi ready.
-> "partitions assigned" dikhe = CONNECTED + WORKING. (isse pehle tak = abhi connect nahi hua.)
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

> ★ PROFILE-INHERITANCE gotcha (9-Sep): `spring.kafka.*` sirf DEFAULT `application.properties` me hai.
> `application-compose.properties` me kafka-line na ho -> wo default se `localhost:9092` INHERIT karega ->
> par app-in-container me `localhost` = app-khud = galat (chahiye `kafka:9092`). Isiliye compose-profile me
> `spring.kafka.bootstrap-servers=kafka:9092` add kiya. (kyun localhost-vs-kafka = section 4B-c.)

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

**Bina DLQ ke:** consumer usi kharab message pe atka rehta (offset commit nahi hua) -> uske peeche ki
line ruk jaati. Ek sada aam poore truck ko rok deta.
★ Barikhi: Spring Kafka ka DEFAULT `DefaultErrorHandler` hamesha atka nahi rakhta — ~10 baar try karke
  message ko LOG karke CHHOD deta (skip + commit). Flow chalta rehta, par message CHUPCHAAP KHO jaata.
  DLT ka fayda = kharab message khota nahi, alag topic me park hota, baad me dekh sakte.

**offset/commit ka funda (isiliye same message dobara aata):**
```
success -> offset AAGE badha (agla message aata)
fail    -> offset ATKA -> isiliye SAME message dobara-dobara aata (jab tak recover ya DLT)
```

### 7b. Ilaaj — 2 step
```
1. RETRY: pehle 2-3 baar try karo (shayad temporary tha, DB wapas aa jaye)
2. Phir bhi fail -> ALAG topic me daal do = "user-events-dlt"
   -> main flow AAGE (unblocked)  |  kharab message side-room me park (baad me dekho)
```
**Anchor (postman):** package deliver nahi ho raha (galat address) -> 3 baar try -> phir poore route ko
rokne ke bajaye "undelivered mail room" me daal deta. Yahi DLQ. (DLT = Dead Letter Topic)

**POORA VISUAL FLOW (retry loop -> DLT):**
```
   POST failme
       |
       v
  [user-events] --> listen() --> throw!  (try 1)
       ^                            |
       |  1 sec ruk (backoff)       |  DefaultErrorHandler pakadta
       +----------------------------+
       |                            |
   try 2 --> throw!                 |
   try 3 --> throw!  (backoff khatam: 2 extra done)
       |
       v  recoverer publish
  [user-events-dlt] --> listenDLT() --> "XXX DEAD-LETTER me gira"
       |
       v
  main [user-events] AAGE badh gaya (offset commit, unblocked)
```

**KAB DLT tak jaata (poison vs temporary):**
```
Temporary fail (DB 1 sec down)  -> retry me hi theek ho jaata -> DLT tak NAHI pahunchta
Poison message (bad data)       -> har try pe fail -> 3 ke baad DLT me park
```
> Isiliye retry PEHLE (temporary ko mauka), DLT BAAD me (jo sach me kharab hai usko park).

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

**(ii) Error-handler = retry + DLT recoverer** ([`KafkaConfig.java`](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/config/KafkaConfig.java)):
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
>
> ★ DHYAAN (partition): `DeadLetterPublishingRecoverer` default me message ko DLT ke USI partition number pe bhejta
>   jis partition se aaya tha (P2 ka message -> `user-events-dlt-2`). Section 7B me `user-events` 3 partition ka
>   ho gaya, par DLT 1 partition ka hai -> P1 / P2 se gira message publish nahi hoga. Ilaaj: DLT ko bhi utne hi
>   partition do (`new NewTopic("user-events-dlt", 3, (short) 1)`) ya recoverer me destination resolver de ke
>   partition 0 pe bhejo. (Spring Kafka docs ka niyam; 7e ka live test 1-partition wale time ka tha.)

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

## 7B. CONSUMER-GROUP + PARTITIONS (scaling — LIVE demo)

### 7B-a. Concept
**Partition:** ek topic andar se kai tukdo (partitions) me bata hota; har partition = ek alag ordered line (log).
Message kis partition me jaayega = **key ke hash se** (key na ho to sticky/round-robin). Same key -> same partition (order bana rehta).
```
Topic "user-events" (3 partitions)
  P0: m1 m4 ...   |   P1: m2 m5 ...   |   P2: m3 m6 ...
```
**Consumer-group:** ek hi `groupId` ke kai consumer -> partitions aapas me BAANT lete (parallel = load-split).
```
group "usercrud-group", 3 consumer, 3 partition:
  Consumer-A <- P0  |  Consumer-B <- P1  |  Consumer-C <- P2   (3x parallel)
```
**GOLDEN rule:** ek partition = SIRF EK consumer (us group me).
```
consumers <= partitions -> sab busy (achha)
consumers >  partitions -> extra consumer KHAALI (partition hi nahi bacha)
consumers <  partitions -> ek consumer KAI partition padhta (2 consumer, 3 partition -> A: P0,P1 · B: P2)
```
**ALAG groupId = ALAG group = us group ko BHI saare messages** (har group apni copy padhta, aapas me nahi baant-te).
```
topic "payment-events"
  group "sms-service"   -> saare messages padhta
  group "fraud-service" -> wahi saare messages ALAG se padhta
```
Interview sawaal "3 partition, message kis consumer ko?" -> key se partition, partition se us GROUP ka ek consumer.
Parallelism ki max limit = **partition count**. **Anchor:** dukaan (topic), 3 counter (partition), 3 cashier (consumer) -> 3x tez; 4th cashier ko counter nahi -> khaali.
**Rebalance (1 line):** consumer aaya/gaya -> Kafka partitions dobara baant deta (auto).

### 7B-b. CODE — jo change kiya (~3 line, zyada code nahi; Spring khud karta)
`application.properties` — 3 consumer-thread ek app me:
```
spring.kafka.listener.concurrency=3
```
`KafkaConfig.java` — topic ko 3-partition declare (KafkaAdmin startup pe badha deta):
```java
@Bean
public NewTopic userEventsTopic() {
    return new NewTopic("user-events", 3, (short) 1);
}
```
`KafkaConsumer.java` — dekhne ke liye thread + partition print + `@Header` se partition uthao:
```java
@KafkaListener(topics = "user-events", groupId = "usercrud-group")
public void listen(String message,
                   @Header(KafkaHeaders.RECEIVED_PARTITION) int partition) {
    System.out.println(">>> [" + Thread.currentThread().getName() + "]  partition=" + partition
                       + "  CONSUMED: " + message);
    ...
}
```
`KafkaProducerController.java` — ★ KEY ke saath bhejo (warna sab ek partition me):
```java
kafkaTemplate.send(TOPIC, message, message);   // (topic, KEY, value) — key = message
```

### 7B-c. ★★ GOTCHA — bina KEY sab ek partition me (sticky)
Pehli baar bina-key (`send(TOPIC, message)`) bheja -> saare 6 message **partition=0, ek hi thread** pe gire.
Wajah: **sticky partitioner** — bina key, ek burst ke message efficiency ke liye same partition me batch hote.
**Fix:** key de do (`send(TOPIC, message, message)`) -> alag key = alag hash = alag partition.
**Seekh:** KEY = load-distribution ka control. Bina key = sab ek jagah; key ke saath = spread across partitions.

### 7B-d. LIVE output (key ke baad — load baanta)
```
[..#0-0-C-1]  partition=0  CONSUMED: h1
[..#0-0-C-1]  partition=0  CONSUMED: h2
[..#0-2-C-1]  partition=2  CONSUMED: h3
[..#0-1-C-1]  partition=1  CONSUMED: h4
[..#0-2-C-1]  partition=2  CONSUMED: h5
[..#0-0-C-1]  partition=0  CONSUMED: h6
```
3 alag thread (#0-0 / #0-1 / #0-2), 3 alag partition -> kaam 3 me baant gaya. Yahi consumer-group scaling.

### 7B-e. KEY -> partition kaise DECIDE hota (asli mechanism)
Ye decision **PRODUCER** karta hai (bhejne se pehle), 2 tareeke se:

**CASE 1 — KEY ke saath (split hua):** producer ek simple formula lagata:
```
partition = murmur2(key bytes) % (total partitions)   (hash = murmur2)

key "h3" -> hash -> % 3 -> P2
key "h4" -> hash -> % 3 -> P1
key "h1" -> hash -> % 3 -> P0
```
Alag key -> alag hash -> alag partition. + BONUS: same key HAMESHA same partition -> us key ka ORDER guaranteed
(jaise ek user ke saare events ek hi partition -> sequence bani rehti).

**CASE 2 — bina KEY (sab ek jagah):** hash kis cheez ka? kuch nahi. To Kafka **sticky partitioner**:
```
"ek partition PAKAD lo (P0), is burst ke saare message wahin daalo -> baad me switch"
```
Wajah = EFFICIENCY (ek partition me batch = network sasta). Isiliye burst ke 6 message sab P0 me.

**Ek line:**
```
KEY hai      -> partition = hash(key)%N  -> deterministic spread (control tere haath)
KEY nahi hai -> sticky: ek partition pakad ke batch -> burst me sab ek jagah
```
**Anchor:** parcel pe **pincode (key)** likha -> alag pincode alag area (partition). Pincode na ho -> courier "abhi sab ek truck me daal do" -> ek area.

### 7B-f. ★★ "Message kis CONSUMER ko?" — DO KADAM (groupId akela jawab NAHI)
```
KADAM 1: kaunsa PARTITION?  -> KEY tay karti, faisla PRODUCER karta (broker NAHI)
  producer bhejne se PEHLE hisaab: key hai -> hash(key) % 3 ("user42" -> P1) · key nahi -> sticky
  phir seedha us partition wale broker ko bhejta. BROKER sirf RAKHTA, chunta nahi.

KADAM 2: us partition se kaunsa CONSUMER?  -> GROUP tay karta
  group "sms" (A,B,C):  A <- P0   B <- P1   C <- P2
  P1 me aaya -> group "sms" me P1 kiske paas? B -> B padhega
  group "fraud" (X):    X <- P0,P1,P2  -> wahi message X ko BHI, apni copy
```
groupId ka kaam = **kaun-kaun si TEAM** ko milega (har group ko poori copy). Team ke **andar kaun member** = partition tay karta.
**YAAD:** key se PARTITION · partition se CONSUMER · groupId se TEAM.
**BOL:**
```
"Two steps. First, the producer picks the partition: hash of the key mod the number of partitions,
 or the sticky partitioner if there's no key. The broker just stores it.
 Second, inside each consumer group, every partition is owned by exactly one consumer,
 so the consumer that owns that partition reads it. The group id decides which group,
 and every group gets its own copy of the message."
```

---

## 7C. IDEMPOTENT CONSUMER (duplicate se bachao)

### 7C-a. Kyun chahiye — Kafka "at-least-once"
Kafka guarantee = message **kam-se-kam ek baar** (kabhi-kabhi ZYADA baar bhi = duplicate).
```
Consumer process karta -> offset COMMIT se PEHLE crash/rebalance
   -> Kafka "commit nahi hua, dobara bhejta hoon" -> SAME message dobara -> DOUBLE process
```
Dikkat jab side-effect ho: "$100 add" -> $200 (double-charge!) · "email" -> 2 email · "order" -> 2 order.

**COMMIT KAB? — kaam se PEHLE ya BAAD (ye hi duplicate ki jad)**
Misaal: SMS consumer, event #107 (offset 2).
```
TAREEKA 1: PEHLE commit, PHIR kaam
  padha #107 -> commit "2 ho gaya" -> SMS bhejna shuru -> CRASH (SMS gaya hi nahi)
  wapas -> bookmark bolta 2 ho gaya -> 3 se shuru -> #107 ka SMS KABHI nahi gaya = KHO GAYA
  = AT-MOST-ONCE (zyada se zyada ek baar, kabhi ZERO)

TAREEKA 2: PEHLE kaam, PHIR commit
  padha #107 -> SMS bhej diya -> commit se pehle CRASH
  wapas -> bookmark abhi 1 pe -> #107 DOBARA -> SMS DOBARA = DUPLICATE
  = AT-LEAST-ONCE (EK BAAR TO JAAYEGA HI, kabhi do baar)
```
**Chunte = TAREEKA 2.** Khona zyada bura (payment event kho gaya = paisa gaya, pata bhi nahi). Duplicate rokna aasan -> neeche 7C-b.
**Spring:** `@KafkaListener` method poora chal ke return ho, TAB Spring offset commit karta = default hi Tareeka 2.
**YAAD:**
```
pehle commit   -> kho sakta        (at-most-once)
baad me commit -> dobara aa sakta  (at-least-once) + dedup = asli duniya ka default
```
**BOL:**
```
"We commit the offset only after processing, so a crash means the message is redelivered, not lost.
 That's at-least-once, so the consumer has to be idempotent: we store the event ID and skip
 anything we've already processed."
```
"Exactly-once" (Kafka transactions) poochhe -> "at-least-once plus idempotent consumer is the common practical approach."

### 7C-b. Ilaaj — dedup by unique ID (ESSENCE, Arpan-line)
```
set me HAI    -> skip (ho chuka)
set me NAHI   -> process + set me daal do
```
"jo dekh liya wo dobara nahi." Bas itna. **Anchor:** darwaze pe register — naam likha hai to dobara entry nahi.

### 7C-c. CODE (`KafkaConsumer.java`)
```java
// "register" of processed message-ids (demo: in-memory; real world: DB/Redis)
private final Set<String> processedIds = ConcurrentHashMap.newKeySet();

@KafkaListener(topics = "user-events", groupId = "usercrud-group")
public void listen(String message, @Header(KafkaHeaders.RECEIVED_PARTITION) int partition) {

    if (processedIds.contains(message)) {                 // pehle dekha?
        System.out.println("=== DUPLICATE skipped (already processed): " + message);
        return;                                           // -> skip, koi double side-effect nahi
    }

    System.out.println(">>> ... CONSUMED: " + message);

    if (message.contains("fail")) {
        throw new RuntimeException("Poison message! Cannot process: " + message);
    }

    processedIds.add(message);   // mark processed -- SIRF success ke BAAD
}
```
★ `add()` **aakhir me** (success ke baad): poison (`fail`) mark NAHI hoga -> retry/DLT abhi bhi kaam karega
  (agar upar mark karte to retry pe skip ho ke DLQ toot jaata).

### 7C-d. SET vs MAP (Arpan-Q: Map se bhi ho sakta?)
Haan. Farak:
```
Set  -> sirf "seen or not" (membership) -- HAMARA case (bas skip karna hai)
Map  -> id ke saath VALUE bhi chahiye:
        Map<id, result>    -> duplicate pe WAHI result return karo (skip nahi) <- payment idempotency
        Map<id, timestamp> -> TTL cleanup (purani ids hata do)
        Map<id, status>    -> processing/done/failed track
```
★ Mazedaar: `ConcurrentHashMap.newKeySet()` = andar se Map HI hai (KeySetView<K,Boolean> = ek CHM ka key-view).
  To Set = "Map with dummy Boolean value". Isiliye dono ek hi cheez ke roop.

### 7C-e. ★ CONNECT — payment idempotency
Ye WAHI idempotency jo tune payment me ki: "tap pay twice = ek hi charge, idempotency-key se."
Yahaan **message-id = wahi idempotency key**, processedIds = wahi dedup store. Same funda, alag jagah. [[idempotency]]

### 7C-f. LIVE output
```
curl ...?message=hello   (do baar):
   >>> ... CONSUMED: hello                              <- 1st (process)
   === DUPLICATE skipped (already processed): hello     <- 2nd (skip!)
```

---

## 8. Interview lines
- "Kafka = pub-sub via topics; producer & consumer DECOUPLED, connected only by TOPIC NAME."
- "Consumer PULLS (polls) — broker doesn't push. @KafkaListener runs a poll loop in background."
- "Serializer = object->bytes on send; deserializer = bytes->object on consume."
- "DLQ = retry N times -> phir bhi fail -> DeadLetterPublishingRecoverer se `<topic>-dlt` me route; main consumer unblocked, poison message parked."
- "DefaultErrorHandler(recoverer, FixedBackOff) = retry policy + jahan gire."
- "Consumer-group = same groupId ke consumers partitions baant lete (load-split); 1 partition = 1 consumer; parallelism max = partition count. Message KEY = partition-routing (same key -> same partition -> order)."
- "Idempotent consumer: Kafka is at-least-once, so dedup by unique message-id -> seen? skip : process+record. Same idea as payment idempotency-key. Set for membership, Map when you need to return the cached result on duplicate."
- **Root-cause story:** "Boot 4 me raw `spring-kafka` autoconfig nahi laata — modularization ke baad autoconfig `spring-boot-starter-kafka` module me hai. Dependency-level pe root cause pakda, docs se confirm, ~60 line manual config ko 2 bean tak clean kiya."
- **Broker-infra:** "Kafka = alag process; KRaft single-node (broker+controller ek hi node, zookeeper-free). advertised.listeners = jo pata broker client ko batata; container me `localhost` = app-khud isliye `kafka:9092` chahiye. 'partitions assigned' log = consumer connected + rebalanced."
- **Debug story (retry-spam):** "'Rebootstrapping' spam dekha -> root cause = broker adhoore config (KRaft env missing) se properly up nahi tha, app-code me kuch toota nahi tha. Poore-config broker uthate hi theek. Log ne seedha bataya broker mil nahi raha."

## 9. Dobara kaise chalaye (LIVE test)
```
docker start kafka                 # broker up (9092)  -- container pehle se bana ho to
# ya project-compose se:  docker compose up -d kafka   (conflict -> docker rm -f kafka phir up)
usercrud run                       # app (IntelliJ/host = default profile = localhost:9092)
curl -X POST "http://localhost:8080/kafka/send?message=hello"    # -> CONSUMED
curl -X POST "http://localhost:8080/kafka/send?message=failme"   # -> 3 try -> DLT me gira
# app-log me "partitions assigned: [user-events-0/1/2]" = connected+working
```

## 10. AAGE (baaki - TO_STUDY / jab man kare)
- ~~Consumer-group / partitions (load-split)~~ -> DONE (section 7B, LIVE demo)
- ~~Idempotent consumer (same message dobara -> duplicate na ho)~~ -> DONE (section 7C, LIVE demo)
- Manual vs auto commit, offsets deep (ek level aur; jab man kare)

> KAFKA STATUS: basics + DLQ + autoconfig-fix + consumer-group + idempotent + source-dive
>              + broker-infra (KRaft/docker/localhost-vs-kafka/retry-spam debug, 9-Sep) = COMPLETE.
