# Student Management System — Complete Spring Boot Notes
### By Arpan Maheshwari | Deep Dive — Har Annotation, Har Concept
---

# TABLE OF CONTENTS

1. [Big Picture — Poora Flow](#1-big-picture)
2. [Spring Boot Internals — DispatcherServlet, AutoConfig, Tomcat](#2-spring-boot-internals)
3. [JDBC vs Hibernate vs JPA — Pehle Kya Tha, Ab Kya Hai](#3-jdbc-vs-hibernate-vs-jpa)
4. [Maven (pom.xml) vs Gradle (build.gradle)](#4-maven-vs-gradle)
5. [Main Class — PracticeApplication.java](#5-main-class)
6. [Entity — Student.java](#6-entity)
7. [DTO — StudentDTO.java](#7-dto)
8. [Repository — StudentRepository.java](#8-repository)
9. [Service — StudentService.java](#9-service)
10. [Controller — StudentController.java](#10-controller)
11. [Auth — AuthController.java](#11-auth)
12. [Exception Handling — GlobalExceptionHandler.java](#12-exception-handling)
13. [Security — JWT (JwtUtil, JwtFilter, SecurityConfig)](#13-security--jwt)
14. [Filter — RequestLoggingFilter.java](#14-filter)
15. [Interceptor — LoggingInterceptor.java + WebConfig.java](#15-interceptor)
16. [Swagger — SwaggerConfig.java](#16-swagger)
17. [Caching — Redis](#17-caching--redis)
18. [Docker — Dockerfile & Compose](#18-docker)
19. [Config — application.properties & build.gradle](#19-config)
20. [MySQL — Queries, Joins, Indexing (Code Examples)](#20-mysql)
21. [Filter vs Interceptor — Deep Comparison](#21-filter-vs-interceptor)
22. [Request ka Poora Safar (Life of a Request)](#22-request-ka-safar)
23. [Interview Questions — Detailed](#23-interview-questions)
24. [Kubernetes — Deployment & Service (k8s)](#24-kubernetes)

---

# 1. BIG PICTURE

### REAL WORLD ANALOGY
Soch ek HOTEL. Customer (client) aaya — pehle MAIN GATE pe CCTV record karta, phir SECURITY GUARD pass dekhta, phir FLOOR RECEPTION specific floor ka log rakhti, phir FRONT DESK order leta, KITCHEN mein chef banata, STORAGE se ingredients aate, GODOWN mein sab maal stored hai, aur baar baar same order? FAST COUNTER ready rakhta. Menu card pe sab dishes ek page pe. Complaint aaye? Error desk handle karta. Poora hotel ek tiffin box mein pack kar ke kahi le ja sakte.

### YE KYA HAI?
Bhai yehi tera Spring Boot app hai — har layer ek department, sab alag kaam karte, sab ek doosre se connected. Hotel chalane ke liye jaise alag alag staff chahiye — waise hi app chalane ke liye alag alag layers (Filter, Controller, Service, Repository, DB, Cache) chahiye.

### Project Structure
```
com.example.practice/
├── PracticeApplication.java        — Entry point. App yahan se start.
├── entity/Student.java             — DB table ka Java object
├── dto/StudentDTO.java             — Client ko safe data bhejne ka wrapper
├── repository/StudentRepository.java — DB se CRUD + custom queries
├── service/StudentService.java     — Business logic + caching
├── controller/
│   ├── StudentController.java      — REST API endpoints (CRUD)
│   └── AuthController.java         — Login/Register + JWT
├── exception/GlobalExceptionHandler.java — Global error handling
├── security/
│   ├── JwtUtil.java                — Token banana/verify/extract
│   ├── JwtFilter.java              — Har request pe token check
│   └── SecurityConfig.java         — Security rules define
├── filter/RequestLoggingFilter.java — Servlet level logging
├── interceptor/LoggingInterceptor.java — Spring level logging
├── config/
│   ├── WebConfig.java              — Interceptor register
│   └── SwaggerConfig.java          — API docs config
```

### Request ka Flow (Ek Line Mein)
```
Client → Filter → Security (JWT) → Interceptor → Controller → Service → Repository → DB
                                                                            ↑ Cache check
```

### VISUALIZE — Poora App Ek HOTEL Hai
```
  ┌─────────────────────────────────────────────────────────────────────┐
  │                        HOTEL (Spring Boot App)                      │
  │                                                                     │
  │  🚪 MAIN GATE (Filter)          — Har visitor log. CCTV camera.    │
  │   │                                                                 │
  │  🔒 SECURITY CHECK (JwtFilter)  — Pass (JWT) dikhaao. Nakli? BAHAR.│
  │   │                                                                 │
  │  📋 FLOOR RECEPTION (Interceptor) — Specific floor ka log.          │
  │   │                                                                 │
  │  🛎️ FRONT DESK (Controller)     — "Kya chahiye?" Request samjho.   │
  │   │                                                                 │
  │  👨‍🍳 KITCHEN (Service)           — Order process. Logic. Cooking.   │
  │   │                                                                 │
  │  🏪 STORAGE (Repository)         — Ingredients laao (DB se data).   │
  │   │                                                                 │
  │  📦 GODOWN (Database - MySQL)    — Sab maal yahan stored.          │
  │   │                                                                 │
  │  ⚡ FAST COUNTER (Cache/Redis)   — Baar baar same order? Ready hai! │
  │                                                                     │
  │  📖 MENU CARD (Swagger)          — Sab dishes (APIs) ek page pe.   │
  │  📋 ERROR DESK (ExceptionHandler)— Complaint aaye? Handle karo.    │
  │  🐳 CONTAINER (Docker)           — Poora hotel ek box mein pack.   │
  └─────────────────────────────────────────────────────────────────────┘
```

### Layered Architecture

#### REAL WORLD ANALOGY
Restaurant hi le le — waiter (Front Desk) customer se order leta, kitchen mein NAHI ghusta. Chef (Kitchen) khana banata, customer se seedha baat NAHI karta. Storekeeper (Storage) ingredients nikalta, khana NAHI banata. Sab ka kaam alag, sab specialist.

#### YE KYA HAI?
Layered architecture yehi karta — code ko ALAG ALAG LAYER mein divide karta. Har layer ka ek specific kaam. Koi layer doosre ka kaam NAHI karti.

```
  ┌──────────────┐
  │  Controller   │  ← HTTP handle. "Kya order hai?"
  │  (Front Desk) │     Request le, response de. Logic NAHI.
  ├──────────────┤
  │  Service      │  ← LOGIC. "Order kaise banana?"
  │  (Kitchen)    │     Process, validate, transform. HTTP nahi jaanta.
  ├──────────────┤
  │  Repository   │  ← DB ACCESS. "Ingredients laao."
  │  (Storage)    │     Data le, save kar. Logic nahi jaanta.
  ├──────────────┤
  │  Database     │  ← Actual data. MySQL table.
  │  (Godown)     │
  └──────────────┘

  FAYDA: Kal DB badli → sirf Repository badlo. Kitchen same. Front desk same.
         Kal menu badli → sirf Controller badlo. Kitchen same. Storage same.
```

#### NAHI HUA TO (Without Layered Architecture)?
```
SOCH — Agar sab ek file mein hota:
  StudentController.java mein:
    - HTTP handle
    - DB connection
    - SQL queries
    - Business logic
    - Error handling
    - Caching

  PROBLEM:
    1. 5000 lines ki ek file — test kaise karo?
    2. DB badli → 50 jagah code badlo
    3. 2 developers same file mein — merge conflict ROTA
    4. Bug dhundho → kahan hai? Sab mila hua

  WITH LAYERS:
    Controller badlo → Service safe
    Service test karo → Controller nahi chahiye
    DB switch → sirf Repository touch karo
    = PROFESSIONAL code. Interview mein ye samjhao.
```

### Mermaid — Big Picture Architecture

```mermaid
graph TD
    A["Client<br/>(Browser / Postman)"] --> B["RequestLoggingFilter<br/>(CCTV Camera)"]
    B --> C["JwtFilter<br/>(Security Guard)"]
    C --> D["LoggingInterceptor<br/>(Floor Receptionist)"]
    D --> E["Controller<br/>(Front Desk)"]
    E --> F["Service<br/>(Kitchen)"]
    F --> G{"Cache Check<br/>(Fridge)"}
    G -->|"HIT"| H["Return Cached"]
    G -->|"MISS"| I["Repository<br/>(Storage)"]
    I --> J["MySQL<br/>(Godown)"]
    J --> K["Cache Store"]
    K --> H
    H --> L["Response to Client"]

    M["Swagger<br/>(Menu Card)"] -.-> E
    N["GlobalExceptionHandler<br/>(Complaint Desk)"] -.-> E
    O["Docker<br/>(Tiffin Box)"] -.-> |"packs everything"| P["All Components"]
    Q["Kubernetes<br/>(Fleet Manager)"] -.-> |"manages multiple"| O

    style A fill:#e1f5fe
    style C fill:#ffcdd2
    style F fill:#fff9c4
    style J fill:#c8e6c9
    style G fill:#ffccbc
    style Q fill:#e8eaf6
```

---

# 2. SPRING BOOT INTERNALS — DispatcherServlet, AutoConfig, Tomcat

### REAL WORLD ANALOGY
Soch ek FACTORY start ho rahi hai. Pehle BUILDING ready (Tomcat), phir HR staff dhundhta (Component Scan) — "tu receptionist banega, tu chef, tu storekeeper". Phir DEPARTMENTS setup (Auto Configuration) — MySQL driver mila toh DB desk lagao, Redis mila toh fast counter lagao. Sab staff apni jagah, sab connected. Factory OPEN — visitors aao!

### YE KYA HAI?
Spring Boot Internals = yehi factory setup sequence. `SpringApplication.run()` ek command mein — Tomcat start, beans scan, auto configure, dependency inject. Tu bas ignition key ghumao, Spring baaki sab khud karta.

### VISUALIZE — SpringApplication.run() ke andar kya hota?
```
  Tu likh: SpringApplication.run(PracticeApplication.class, args);
  
  ANDAR YE HOTA (imagine karo ek FACTORY start ho rahi):

  ┌──────────────────────────────────────────────────────┐
  │ STEP 1: FACTORY KA BUILDING READY (Tomcat Start)     │
  │                                                      │
  │   Tomcat = building. Port 8080 = main gate.          │
  │   Building ready, gate open, visitors aa sakte.      │
  │   [Embedded — building app ke ANDAR hai, alag nahi]  │
  └──────────────────────┬───────────────────────────────┘
                         ↓
  ┌──────────────────────────────────────────────────────┐
  │ STEP 2: STAFF DHUNDHO (Component Scan)               │
  │                                                      │
  │   HR (Spring) building mein jaake staff dhundhta:    │
  │   @Controller wale → "tu FRONT DESK pe baith"        │
  │   @Service wale    → "tu KITCHEN mein ja"            │
  │   @Repository wale → "tu STORAGE sambhaal"           │
  │   @Component wale  → "tu general duty pe"            │
  │   Sab ka ID card bana (Bean) → register mein daal    │
  │   (ApplicationContext)                                │
  └──────────────────────┬───────────────────────────────┘
                         ↓
  ┌──────────────────────────────────────────────────────┐
  │ STEP 3: DEPARTMENTS SETUP (Auto Configuration)       │
  │                                                      │
  │   MySQL driver mila? → "DB connection desk lagao"    │
  │   Security mila?     → "Security guard bithao"       │
  │   Redis mila?        → "Fast counter lagao (cache)"  │
  │   Spring KHUD dekhta kya available → auto setup.     │
  │   Tu bas saamaan (dependency) laao — Spring lagayega. │
  └──────────────────────┬───────────────────────────────┘
                         ↓
  ┌──────────────────────────────────────────────────────┐
  │ STEP 4: SAB READY (Beans Wired)                      │
  │                                                      │
  │   Controller ko Service chahiye → Spring de diya     │
  │   Service ko Repository chahiye → Spring de diya     │
  │   Sab staff apni jagah, sab connected.               │
  │   Console: "Tomcat started on port(s): 8080"         │
  │   HOTEL OPEN! Visitors (requests) aao!               │
  └──────────────────────────────────────────────────────┘
```

### VISUALIZE — DispatcherServlet = CALL CENTER OPERATOR
```
  Imagine: ek CALL CENTER hai. SAB calls PEHLE OPERATOR (DispatcherServlet) ke paas.
  Operator decide karta: "ye call kaunse department ko route karu?"

  ┌─────────┐    ┌──────────────────┐    ┌─────────────────────┐
  │ Customer │───→│   OPERATOR       │───→│ Department matching │
  │ (Client) │    │ (DispatcherServlet)│   │  (HandlerMapping)   │
  └─────────┘    └────────┬─────────┘    └─────────────────────┘
                          │
                          │  "GET /api/students" → StudentController.getAll()
                          │  "POST /api/auth/login" → AuthController.login()
                          ↓
                 ┌──────────────────┐
                 │   DEPARTMENT      │
                 │  (Controller)     │ → Service → Repository → DB
                 └────────┬─────────┘
                          │
                          ↓ (Java object return)
                 ┌──────────────────┐
                 │  TRANSLATOR       │
                 │ (Jackson/JSON)    │  Java Object → {"name":"Arpan","age":25}
                 └────────┬─────────┘
                          │
                          ↓
                 ┌──────────────────┐
                 │  Customer ko     │
                 │  response milta   │  Status 200 + JSON body
                 └──────────────────┘

  BINA OPERATOR: Har department ka ALAG phone number. 50 departments = 50 numbers yaad karo.
  WITH OPERATOR: Ek number call karo. Operator sahi department bhejega. = DispatcherServlet.
```

### Embedded Tomcat — Kya Hai?
```
PURANA TARIKA (Spring without Boot):
  1. Tomcat server alag se install karo
  2. App ka WAR file banao
  3. WAR file Tomcat ke webapps/ folder mein daalo
  4. Tomcat start karo
  = 4 steps sirf deploy ke liye.

SPRING BOOT:
  Tomcat app ke ANDAR hai (embedded).
  java -jar app.jar → Tomcat auto start → app ready.
  = 1 step. Bas.

spring-boot-starter-web mein included:
  - Tomcat (embedded server)
  - Spring MVC (DispatcherServlet, controllers)
  - Jackson (JSON conversion)
```

### VISUALIZE — ApplicationContext = WAREHOUSE + IoC = ZOMATO
```
  ApplicationContext = WAREHOUSE (godown) jahan sab ready-made items (beans) rakhe.

  ┌────────────────────────────────────────────┐
  │        ApplicationContext (WAREHOUSE)        │
  │                                              │
  │  📦 StudentController (bean)                 │
  │  📦 StudentService (bean)                    │
  │  📦 StudentRepository (bean)                 │
  │  📦 JwtFilter (bean)                         │
  │  📦 LoggingInterceptor (bean)                │
  │  📦 SecurityFilterChain (bean)               │
  │  ... sab beans yahan stored ...              │
  └────────────────────────────────────────────┘

  @Autowired = "mujhe ye chahiye" → Spring warehouse se nikal ke de deta.

  IoC (Inversion of Control) = ZOMATO MODEL:
  ┌─────────────────────────────────────────────────────────────────┐
  │ PURANA (Bina IoC):                                              │
  │   Tu khud market ja → ingredients le → khana bana → khud kha   │
  │   = Tu khud "new Repository()", "new Service()" banata          │
  │   Problem: Sab TUJHE karna padta. Complex. Tightly coupled.    │
  │                                                                 │
  │ NAYA (IoC — Spring):                                            │
  │   Tu Zomato pe order de → Zomato restaurant dhundhe → deliver   │
  │   = Tu @Autowired bolo → Spring bean dhundhe → inject kare      │
  │   Fayda: Tujhe pata nahi kaise bana, kaun bana. Bas milta hai. │
  └─────────────────────────────────────────────────────────────────┘

  BEAN LIFECYCLE (birth to death):
    Scan → Born (create) → Fed (inject dependencies) → Ready (@PostConstruct)
    → Working (serving requests) → Retiring (@PreDestroy) → Dead (destroy)
```

### @Component vs @Service vs @Repository vs @Controller
```
SAB @COMPONENT HAIN. Spring bean banata sab ka. FUNCTIONALLY SAME.

PHIR ALAG KYUN?

@Component    — Generic. Kuch bhi. "Ye ek Spring bean hai."
@Service      — Business logic. "Ye service layer hai."
@Repository   — DB access. "Ye data layer hai." BONUS: DB exception translation.
@Controller   — HTTP handler. "Ye web layer hai."

INTERVIEW MEIN: "Functionally same hain, but semantically different.
                 Code padhne wale ko INTENT samajh aata.
                 @Repository ka bonus: DB exceptions → Spring DataAccessException."
```

### Mermaid — SpringApplication.run() Startup Flow

```mermaid
flowchart TD
    A["SpringApplication.run()"] --> B["Tomcat Start<br/>(Building ready, gate open)<br/>Port 8080"]
    B --> C["Component Scan<br/>(HR staff dhundh raha)"]
    C --> D["@Controller found<br/>→ Front Desk pe bithao"]
    C --> E["@Service found<br/>→ Kitchen mein bhejo"]
    C --> F["@Repository found<br/>→ Storage sambhalo"]
    C --> G["@Component found<br/>→ General duty"]
    D --> H["Auto Configuration<br/>(Departments setup)"]
    E --> H
    F --> H
    G --> H
    H --> I{"MySQL driver<br/>milaa?"}
    I -->|"Yes"| J["DataSource auto configure"]
    H --> K{"Security<br/>mila?"}
    K -->|"Yes"| L["Security filters setup"]
    H --> M{"Redis<br/>mila?"}
    M -->|"Yes"| N["Cache manager setup"]
    J --> O["Beans Wired<br/>(Sab connected)"]
    L --> O
    N --> O
    O --> P["APP READY!<br/>Tomcat started on port 8080"]

    style A fill:#e1f5fe
    style P fill:#c8e6c9
    style H fill:#fff9c4
```

### NAHI HUA TO (Without Spring Boot Internals)?
```
BINA @SpringBootApplication:
  Spring ko pata nahi ki beans kahan hain → Component scan nahi → sab null.
  Auto configuration nahi → manually Tomcat configure, DataSource configure, Security configure.
  = 200 lines XML configuration (Spring 2.x ka zamana).

BINA Embedded Tomcat:
  Alag se Tomcat install → WAR file banao → webapps mein daalo → Tomcat start.
  = 4 steps sirf deploy ke liye. Spring Boot mein 1 step.

BINA DispatcherServlet:
  50 APIs = 50 Servlet classes. Har URL ka alag Servlet. Nightmare.
  DispatcherServlet = ek entry point, sab route karta.
```

---

# 3. JDBC vs HIBERNATE vs JPA — Pehle Kya Tha, Ab Kya Hai

### REAL WORLD ANALOGY
Khana banane ka evolution soch:
- JDBC = Khud market jao, ingredients lao, recipe padho, stove pe bana, bartan dho — 30 steps, thakaawat.
- Hibernate = Zomato pe order, "Biryani" bolo, bana banaya aa jaaye — tu sirf bola "kya chahiye".
- JPA = Zomato ka INTERFACE — Zomato/Swiggy koi bhi chalao, order same.
- Spring Data JPA = Alexa se order — "Alexa, biryani mangwa" — tune interface likha, Spring ne deliver kiya.

### YE KYA HAI?
Java se database connect karne ka TARIKA evolve hua. Pehle manual tha (JDBC), phir automation aayi (Hibernate), phir standardization (JPA), phir magic (Spring Data JPA). Har level pe boilerplate code KAM hota gaya.

### VISUALIZE — Evolution (Khana Banana)
```
  JDBC       = Khud market ja, ingredients le, recipe padh, stove pe bana, bartan dho.
               30 steps. Galti ka chance bahut. Thakaawat.

  Hibernate  = Zomato pe restaurant select, "Biryani" bolo, aa jaaye.
               Tu sirf bola "kya chahiye" — kitchen (Hibernate) ne SQL banaya.

  JPA        = Zomato ka INTERFACE. Zomato ya Swiggy koi bhi use karo, order same.
               JPA = spec, Hibernate = Zomato, EclipseLink = Swiggy.

  Spring Data JPA = Voice assistant se order. "Alexa, biryani mangwa."
                    Tu INTERFACE likha — Spring ne implementation de di. MAGIC.

  ┌──────────┐   ┌──────────┐   ┌──────────┐   ┌──────────────────┐
  │   JDBC    │   │Hibernate │   │   JPA    │   │ Spring Data JPA   │
  │ 30 lines  │──→│ 5 lines  │──→│ Standard │──→│ 1 line (FREE)    │
  │ Manual SQL│   │ Auto SQL │   │Interface │   │ repo.findAll()    │
  │ Manual Map│   │ Auto Map │   │Portable  │   │ DONE!             │
  └──────────┘   └──────────┘   └──────────┘   └──────────────────┘
```

### Level 1: Raw JDBC — Sab Khud Likho
```java
// JDBC = Java Database Connectivity. Java se DB connect karne ka PEHLA tarika.
// Sab MANUALLY likho — connection, query, result mapping.

// Step 1: Driver load
Class.forName("com.mysql.cj.jdbc.Driver");

// Step 2: Connection banao
Connection conn = DriverManager.getConnection(
    "jdbc:mysql://localhost:3306/student_db", "root", "root");

// Step 3: Query likho
String sql = "SELECT * FROM students WHERE id = ?";
PreparedStatement ps = conn.prepareStatement(sql);
ps.setLong(1, 5);    // ? mein 5 daalo

// Step 4: Execute karo
ResultSet rs = ps.executeQuery();

// Step 5: Result manually map karo — EK EK COLUMN
if (rs.next()) {
    Student s = new Student();
    s.setId(rs.getLong("id"));
    s.setName(rs.getString("name"));
    s.setEmail(rs.getString("email"));
    s.setAge(rs.getInt("age"));
}

// Step 6: Close karo (bhool gaye toh connection LEAK)
rs.close();
ps.close();
conn.close();

// PROBLEM:
// - 30 lines sirf ek SELECT ke liye
// - Har query mein same boilerplate (connect, close, try-catch)
// - Column naam galat likha → Runtime error (compile time pe nahi pata)
// - SQL injection ka risk (String concatenation mein)
// - Connection management tu khud karo (pool bhi tu banao)
```

### Level 2: Hibernate — ORM (Object Relational Mapping)
```java
// HIBERNATE = Java object ↔ DB table AUTOMATIC mapping.
// Tu Java object de, Hibernate SQL generate karega.

// Student.java → students table. Automatic.
// student.setName("Arpan") → UPDATE students SET name='Arpan'

// SELECT — findById
Session session = sessionFactory.openSession();
Student student = session.get(Student.class, 5L);
// Hibernate internally: SELECT * FROM students WHERE id = 5
// Result → Student object AUTOMATIC map. Tujhe ResultSet nahi chhedna.
session.close();

// INSERT — save
Session session = sessionFactory.openSession();
Transaction tx = session.beginTransaction();
session.save(student);     // INSERT INTO students (name, email, age) VALUES (...)
tx.commit();               // Hibernate ne SQL generate kiya + execute kiya
session.close();

// HIBERNATE BENEFITS:
// - SQL khud generate karta (tu sirf Java objects use karo)
// - Column mapping automatic (@Entity, @Column)
// - Caching built-in (1st level cache = session, 2nd level = application)
// - DB independent — MySQL se PostgreSQL switch karo, code SAME
// - Lazy loading — related data tab le jab zaroorat ho
// - Connection pooling built-in (HikariCP)
```

### Level 3: JPA — Standard Interface
```
JPA = Java Persistence API. Ye SPECIFICATION hai, IMPLEMENTATION nahi.

ANALOGY:
  JPA = Interface (rules define karta)
  Hibernate = Implementation (rules follow karta)

  Jaise: List = interface, ArrayList = implementation.
  JPA = interface, Hibernate = implementation.

KYUN JPA:
  Bina JPA: Code mein "Hibernate" ke classes directly use.
            Kal EclipseLink pe switch karo → poora code badlo.
  With JPA: Code mein JPA interfaces use (@Entity, EntityManager).
            Hibernate ya EclipseLink — kuch bhi use karo, code SAME.

SPRING DATA JPA:
  JPA ke upar ek aur layer. Tu sirf INTERFACE likho (JpaRepository extend).
  Spring implementation AUTOMATIC de deta.

  3 LINES se sab CRUD ready:
  public interface StudentRepository extends JpaRepository<Student, Long> { }
  // save, findAll, findById, delete — sab FREE. Ek line SQL nahi likhi.
```

### Comparison Table
```
                    JDBC              HIBERNATE           SPRING DATA JPA
SQL likhna:         Khud likho        Auto generate        Auto generate
Mapping:            Manual (ResultSet) Automatic (@Entity)  Automatic
Boilerplate:        BAHUT             Kam                   MINIMAL
DB switch:          Code badlo        Config badlo          Config badlo
Learning:           Easy              Medium                Easy (Hibernate jaanne ke baad)
Production mein:    Legacy code       Still used            MOST USED

EVOLUTION: JDBC → Hibernate → JPA → Spring Data JPA
           (Manual)  (ORM)    (Standard)  (Magic)
```

### Mermaid — JDBC vs Hibernate vs JPA Evolution

```mermaid
graph TD
    A["JDBC<br/>Raw SQL + Manual Mapping<br/>30 lines per query"] -->|"Boilerplate bahut!"| B["Hibernate<br/>ORM — Auto SQL + Auto Map<br/>5 lines per query"]
    B -->|"Vendor lock-in"| C["JPA<br/>Standard Interface<br/>@Entity, @Id, @Table"]
    C -->|"Still some code"| D["Spring Data JPA<br/>1 LINE FREE!<br/>extends JpaRepository"]

    A -.- A1["Connection manage<br/>ResultSet loop<br/>Column by column map<br/>Close connection<br/>Try-catch everywhere"]
    B -.- B1["session.save(obj)<br/>Auto SQL generate<br/>Caching built-in<br/>DB independent"]
    C -.- C1["Interface only<br/>Hibernate implements<br/>EclipseLink bhi<br/>Portable code"]
    D -.- D1["save() findAll() FREE<br/>findByName() auto<br/>Pagination built-in<br/>ZERO boilerplate"]

    style A fill:#ffcdd2
    style B fill:#fff9c4
    style C fill:#bbdefb
    style D fill:#c8e6c9
```

### NAHI HUA TO (Without ORM/JPA)?
```
IMAGINE: Tujhe 20 tables hain, har table ka 10 columns.
  JDBC se: 20 tables * 5 operations (CRUD + search) = 100 methods.
           Har method mein: Connection, PreparedStatement, ResultSet, Mapping, Close.
           = 100 * 30 lines = 3000 lines sirf DB code!

  Spring Data JPA se: 20 repositories extend JpaRepository.
                      = 20 interfaces, 0 lines implementation.
                      3000 lines → 20 lines. MAGIC.
```

### Hibernate Internals — Interview ke liye
```
SESSION:
  Hibernate ka DB connection wrapper. Ek unit of work.
  Session mein operations karo → commit → DB mein save.

SESSION FACTORY:
  Session banane ki factory. Application mein EK hota (heavy object).
  Spring Boot mein EntityManagerFactory isko wrap karta.

ENTITY MANAGER:
  JPA version of Session. Same kaam, JPA standard interface.
  Spring Boot mein @PersistenceContext se inject hota.

FIRST LEVEL CACHE:
  Session level. Same session mein same entity 2 baar find →
  Pehli baar DB se. Doosri baar cache se. Automatic.

HQL vs JPQL:
  HQL = Hibernate Query Language (Hibernate specific)
  JPQL = Java Persistence Query Language (JPA standard)
  Dono almost same. JPQL prefer karo (standard).

LAZY vs EAGER LOADING:
  @OneToMany(fetch = LAZY)  — related data TAB le jab access karo (default)
  @OneToMany(fetch = EAGER) — related data ABHI le saath mein
  Lazy = performance better (extra data tab le jab chahiye)
  Eager = N+1 problem (1 query + N extra queries — SLOW)
```

---

# 4. MAVEN (pom.xml) vs GRADLE (build.gradle)

### REAL WORLD ANALOGY
Amazon se shopping soch. Tu order do → Amazon khud saamaan manage karta: dhundhta, pack karta, deliver karta, version check karta. Bina Amazon — tu khud 50 dukaan ghoomo, saamaan le aao, home delivery khud manage. Build tool = Amazon. Bina build tool = khud market chakkar.

### YE KYA HAI?
Build Tool = Project build karne ka automation tool. Dependencies download, code compile, tests run, JAR package banao — sab automate. Maven (XML based, purana) aur Gradle (Groovy/Kotlin, naya & fast) — 2 major options.

```
BUILD TOOL KE KAAM:
  - Dependencies download (libraries laao)
  - Code compile (Java → class files)
  - Tests run
  - JAR/WAR banao (deployable package)
  - Plugins run (code generate, check, etc.)

MAVEN = XML based (pom.xml). Purana, stable.
GRADLE = Groovy/Kotlin based (build.gradle). Naya, fast.
```

### Maven — pom.xml (Agar ye project Maven hota)
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0">
    <modelVersion>4.0.0</modelVersion>

    <!-- Project info -->
    <groupId>com.example</groupId>         <!-- Company/org -->
    <artifactId>practice</artifactId>      <!-- Project naam -->
    <version>0.0.1-SNAPSHOT</version>      <!-- Version -->

    <!-- Spring Boot parent — version management free -->
    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>3.2.5</version>
    </parent>

    <properties>
        <java.version>17</java.version>
    </properties>

    <!-- Dependencies — libraries -->
    <dependencies>
        <!-- REST API -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
            <!-- Version nahi likha — parent se auto milega -->
        </dependency>

        <!-- JPA/Hibernate -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-data-jpa</artifactId>
        </dependency>

        <!-- MySQL -->
        <dependency>
            <groupId>com.mysql</groupId>
            <artifactId>mysql-connector-j</artifactId>
            <scope>runtime</scope>   <!-- Sirf runtime pe chahiye, compile nahi -->
        </dependency>

        <!-- Lombok -->
        <dependency>
            <groupId>org.projectlombok</groupId>
            <artifactId>lombok</artifactId>
            <scope>provided</scope>  <!-- Compile time, JAR mein nahi jaayega -->
        </dependency>
    </dependencies>

    <build>
        <plugins>
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
            </plugin>
        </plugins>
    </build>
</project>

<!-- COMMANDS:
     mvn clean install     — build + test + jar banao
     mvn spring-boot:run   — app start
     mvn test              — sirf tests
     mvn dependency:tree   — sab dependencies dikhao
-->
```

### Gradle — build.gradle (Ye project Gradle use karta)
```groovy
plugins {
    id 'java'
    id 'org.springframework.boot' version '3.2.5'
    id 'io.spring.dependency-management' version '1.1.7'
}

group = 'com.example'
version = '0.0.1-SNAPSHOT'

java {
    toolchain {
        languageVersion = JavaLanguageVersion.of(17)
    }
}

repositories {
    mavenCentral()    // Dependencies yahan se download hongi
}

dependencies {
    implementation 'org.springframework.boot:spring-boot-starter-web'
    implementation 'org.springframework.boot:spring-boot-starter-data-jpa'
    implementation 'org.springframework.boot:spring-boot-starter-security'
    implementation 'org.springframework.boot:spring-boot-starter-validation'
    implementation 'org.springframework.boot:spring-boot-starter-data-redis'
    implementation 'org.springdoc:springdoc-openapi-starter-webmvc-ui:2.3.0'
    implementation 'io.jsonwebtoken:jjwt-api:0.11.5'
    runtimeOnly 'io.jsonwebtoken:jjwt-impl:0.11.5'
    runtimeOnly 'io.jsonwebtoken:jjwt-jackson:0.11.5'
    compileOnly 'org.projectlombok:lombok'
    runtimeOnly 'com.mysql:mysql-connector-j'
    annotationProcessor 'org.projectlombok:lombok'
    testImplementation 'org.springframework.boot:spring-boot-starter-test'
}

// COMMANDS:
// ./gradlew build          — build + test + jar banao
// ./gradlew bootRun        — app start
// ./gradlew test           — sirf tests
// ./gradlew dependencies   — sab dependencies dikhao
```

### Maven vs Gradle — Comparison
```
                     MAVEN                    GRADLE
Config file:         pom.xml (XML)            build.gradle (Groovy/Kotlin)
Readability:         Verbose (XML bloat)      Concise (3x chhota)
Speed:               Slow                     FAST (incremental build, cache)
Flexibility:         Rigid (convention)       Flexible (code likh sakte)
Industry:            Legacy projects          Modern projects
Spring Boot:         Supported                Supported (recommended now)
Learning:            Easy (XML samajho)       Medium (Groovy syntax)

GRADLE KI SPEED KYUN?
  - Incremental build: sirf changed files compile
  - Build cache: pehle build kiya tha toh reuse
  - Daemon: background process, startup fast

INTERVIEW MEIN: "Dono build tools hain. Maven XML based, Gradle Groovy based.
                 Gradle faster hai incremental build se. Modern projects Gradle prefer."
```

### Mermaid — Build Tool Flow

```mermaid
flowchart TD
    A["Source Code<br/>(.java files)"] --> B["Build Tool<br/>(Gradle/Maven)"]
    B --> C["Download<br/>Dependencies"]
    B --> D["Compile<br/>(Java → .class)"]
    B --> E["Run Tests"]
    B --> F["Package<br/>(.jar file)"]
    F --> G["Deploy<br/>(java -jar app.jar)"]

    C -.- C1["Maven Central<br/>se download"]
    D -.- D1["javac command<br/>internally"]
    F -.- F1["FAT JAR<br/>= app + all libs + Tomcat"]

    style B fill:#fff9c4
    style F fill:#c8e6c9
```

### NAHI HUA TO (Without Build Tool)?
```
BINA GRADLE/MAVEN:
  1. Manually har library ki JAR download karo (Spring, Hibernate, Jackson — 50+ JARs)
  2. Classpath mein manually add karo
  3. javac se manually compile karo
  4. jar command se manually package karo
  5. Library version conflict? — tu khud solve kar
  = 2 ghante ka kaam har build pe. Build tool se: 1 command, 30 seconds.
```

### Dependency Scopes — Kya Matlab?
```
MAVEN SCOPES:
  compile (default) — sab jagah chahiye (compile + runtime + test)
  runtime           — sirf runtime pe (MySQL driver — compile mein nahi, run mein chahiye)
  provided          — compile mein chahiye, JAR mein mat daalo (Lombok, Tomcat)
  test              — sirf test mein (JUnit)

GRADLE EQUIVALENTS:
  implementation    — compile + runtime (default use)
  runtimeOnly       — sirf runtime (MySQL driver)
  compileOnly       — sirf compile (Lombok)
  annotationProcessor — compile time annotation process (Lombok)
  testImplementation — sirf test mein (JUnit)
```

---

# 5. MAIN CLASS

### REAL WORLD ANALOGY
Gaadi ki IGNITION KEY — key nahi ghumai toh engine chalu hi nahi hoga. Chahe gaadi mein petrol ho, AC ho, music system ho — key nahi = kuch bhi nahi. Key ghumaya → engine start → sab systems activate → gaadi ready to drive.

### YE KYA HAI?
Main Class = tera Spring Boot app ki IGNITION KEY. `PracticeApplication.java` mein `main()` method — jab tu ye run karta, tab Spring Boot start hota, Tomcat chalu, beans scan, auto configure — sab activate. Bina iske app start hi nahi hogi.

### PracticeApplication.java

```java
@SpringBootApplication    // 3-in-1 annotation
@EnableCaching            // Redis/in-memory caching ON karo
public class PracticeApplication {
    public static void main(String[] args) {
        SpringApplication.run(PracticeApplication.class, args);
    }
}
```

### @SpringBootApplication — Ye 3 kaam karta:
| Inside Annotation | Kya Karta Hai |
|---|---|
| `@Configuration` | Ye class configuration source hai — beans define kar sakte |
| `@EnableAutoConfiguration` | Spring khud sab configure karega — Tomcat, JPA, Security |
| `@ComponentScan` | Is package + sub-packages mein @Component, @Service, @Controller dhundhega |

### @EnableCaching — Kyun chahiye?
```
BINA @EnableCaching:  @Cacheable, @CachePut, @CacheEvict — sab ignore honge. Kuch nahi hoga.
WITH @EnableCaching:  Spring proxy banata hai, cache logic apply karta hai.

Ye main class pe lagta hai — ek baar lagao, poore app mein caching ON.
```

### Mermaid — @SpringBootApplication 3-in-1

```mermaid
graph TD
    A["@SpringBootApplication"] --> B["@Configuration<br/>Ye class = config source<br/>@Bean methods yahan"]
    A --> C["@EnableAutoConfiguration<br/>Classpath dekhke auto setup<br/>MySQL? → DataSource<br/>Redis? → Cache"]
    A --> D["@ComponentScan<br/>Is package mein scan<br/>@Service, @Controller,<br/>@Repository dhundho"]

    B --> E["Beans define"]
    C --> F["Auto config apply"]
    D --> G["Beans discover"]
    E --> H["APP READY"]
    F --> H
    G --> H

    style A fill:#e1f5fe
    style H fill:#c8e6c9
```

### NAHI HUA TO (Without Main Class)?
```
BINA @SpringBootApplication:
  App start hi nahi hogi. Tomcat nahi chalega. Beans scan nahi honge.
  Ye = app ka ignition key. Key nahi toh gaadi start nahi.

BINA @EnableCaching:
  @Cacheable laga liya service mein → but kuch nahi hoga.
  Har request DB jaayegi. 1000 same requests = 1000 DB calls.
  @EnableCaching = switch ON karna.
  Switch OFF hai toh bulb lagaya kya fayda.
```

---

# 6. ENTITY

### REAL WORLD ANALOGY
Tera AADHAAR CARD soch. Sarkar ke database mein ek entry hai — unique Aadhaar number (ID), naam, DOB, address, photo — sab printed fields. Har nagrik ka alag entry. Aadhaar number se government tujhe track karta, pull karta, update karta.

### YE KYA HAI?
Entity yehi karta Spring mein — `@Entity` class = database table ka Java representation. Har object = ek row (Aadhaar entry ki tarah). Har field = ek column (Aadhaar pe printed info). `@Id` = Aadhaar number (unique identifier). Bina `@Entity` ke Spring ko pata nahi ye DB table hai — jaise bina Aadhaar ke tu government ke liye exist nahi karta.

### Student.java — DB Table ka Java Version

```java
@Entity                    // JPA ko bolo: ye class = DB table
@Table(name = "students")  // Table ka naam "students" hoga
@Data                      // Lombok: getter + setter + toString + equals + hashCode
@NoArgsConstructor         // Lombok: empty constructor — Student()
@AllArgsConstructor        // Lombok: full constructor — Student(id, name, email, age)
public class Student {

    @Id                              // PRIMARY KEY mark
    @GeneratedValue(strategy = GenerationType.IDENTITY)  // AUTO INCREMENT — DB khud id dega
    private Long id;

    @NotBlank(message = "Name is required")   // null ya "" diya toh error
    private String name;

    @Email(message = "Invalid email")         // email format check (regex)
    @NotBlank(message = "Email is required")
    @Column(unique = true)                    // DB level UNIQUE constraint — duplicate insert = error
    private String email;

    @Min(value = 18, message = "Age must be at least 18")   // 18 se kam diya toh validation fail
    private int age;
}
```

### @Entity — Deep
```
YE KYA HAI:  JPA annotation. Ye class ek database table represent karti hai.
KYUN:        Bina @Entity ke JPA isko table nahi samjhega — save/find kuch nahi hoga.
KAISE KAAM:  Hibernate (JPA implementation) ye class dekhta hai → DB mein table banata:
             CREATE TABLE students (id BIGINT AUTO_INCREMENT, name VARCHAR, email VARCHAR, age INT)
```

### @Table(name = "students") — Deep
```
YE KYA HAI:  Table ka custom naam de.
BINA ISKE:   Table naam = class naam hoga → "Student" (capital S — kuch DB mein problem)
ISKE SAATH:  Table naam "students" hoga (lowercase, plural — convention)
```

### @Data (Lombok) — Deep
```
YE KYA HAI:  Compile time pe ye code AUTO GENERATE karta:
             - getId(), setId(), getName(), setName()... (sab fields ke getter/setter)
             - toString() — print karne ke liye
             - equals() + hashCode() — comparison ke liye

KYUN:        50 line ka boilerplate code ek annotation se khatam.
BINA ISKE:   Manually har field ka getter/setter likhna padta.
```

### @Id + @GeneratedValue — Deep
```
@Id = ye field PRIMARY KEY hai. Har student ka unique identifier.
@GeneratedValue(IDENTITY) = DB khud id assign karega (1, 2, 3...)
                            Tujhe id set nahi karni — DB ka kaam.
```

### Validation Annotations — Deep
```
@NotBlank  — null, "", "   " (spaces) SAB reject. Sirf String pe kaam.
@Email     — regex se email format check. "abc" reject, "abc@xyz.com" accept.
@Min(18)   — numeric value 18 se kam toh reject.
@Column(unique = true) — DB level constraint. Duplicate insert pe SQL exception.

IMPORTANT: Ye validations TABHI chalte jab Controller mein @Valid lagao.
           @Valid nahi lagaya → @NotBlank kuch nahi karega. Silently pass.
```

### Mermaid — Entity to Table Mapping

```mermaid
graph LR
    subgraph "Java Entity (Student.java)"
        A["@Entity<br/>@Table(name='students')"]
        B["@Id Long id"]
        C["@NotBlank String name"]
        D["@Email @Column(unique) String email"]
        E["@Min(18) int age"]
    end

    subgraph "MySQL Table (students)"
        F["id BIGINT<br/>AUTO_INCREMENT<br/>PRIMARY KEY"]
        G["name VARCHAR(255)<br/>NOT NULL"]
        H["email VARCHAR(255)<br/>NOT NULL UNIQUE"]
        I["age INT NOT NULL"]
    end

    A -->|"Hibernate maps"| F
    B --> F
    C --> G
    D --> H
    E --> I

    style A fill:#bbdefb
    style F fill:#c8e6c9
```

### FIELD TO COLUMN MAPPING — Detailed
```
ENTITY = tera AADHAAR CARD:
  @Id         = Aadhaar number (unique ID, kabhi change nahi)
  @Entity     = "ye ek registered person hai" (DB mein tracked)
  name field  = Aadhaar pe naam
  email field = Aadhaar pe address
  age field   = Date of birth se calculated
```

---

# 7. DTO

### REAL WORLD ANALOGY
Tera FULL MEDICAL REPORT soch — blood group, diseases, surgeries, mental history, reports, medications — sab private details. Ye tu sirf DOCTOR ko dega. Ab INSURANCE COMPANY ko kya dega? Full report? NAHI! Sirf ek chhota form — naam, age, basic info. Doctor ko full → insurance ko filtered. Kyun? PRIVACY. Zaroori nahi sabko sab dena.

### YE KYA HAI?
DTO (Data Transfer Object) yehi karta — Entity = full medical report (DB mein sab fields: password, internal flags, createdAt, role). DTO = insurance form (client ko sirf safe fields: id, name, email, age). `fromEntity()` = report se form banao (DB → client). `toEntity()` = form se report banao (client → DB). Sensitive data NEVER leaks to client.

### StudentDTO.java — Client ko Safe Data

```java
@Data @NoArgsConstructor @AllArgsConstructor
public class StudentDTO {
    private Long id;
    private String name;
    private String email;
    private int age;

    // Entity → DTO (DB se client ke liye)
    public static StudentDTO fromEntity(Student student) { ... }

    // DTO → Entity (client se DB ke liye)
    public Student toEntity() { ... }
}
```

### DTO Kyun Chahiye — Real Reason
```
PROBLEM:  Entity mein password, createdAt, internalFlags hote — client ko nahi dikhane.
          Entity seedha return karoge → sensitive data LEAK.

SOLUTION: DTO = sirf wahi fields jo client ko dikhani hain.
          Entity (DB):  id, name, email, age, password, createdAt, role
          DTO (Client): id, name, email, age  ← password NAHI!

BONUS:    DB schema badla toh API response nahi badlega.
          Entity mein 10 fields add karo — DTO same rahega.
```

### fromEntity() vs toEntity()
```
fromEntity(Student) → StudentDTO   = DB se data aaya → client ke liye DTO banao
toEntity()          → Student      = Client se data aaya → DB ke liye Entity banao

YE PATTERN = "Mapper" kehte. Real projects mein MapStruct library use hoti.
```

### Mermaid — DTO as Security Gate

```mermaid
flowchart LR
    subgraph "DB Side (Entity)"
        A["Student<br/>id, name, email, age<br/>password, createdAt,<br/>role, internalFlags"]
    end

    subgraph "Security Filter (DTO)"
        B["fromEntity()<br/>Filter out<br/>sensitive fields"]
        C["toEntity()<br/>Add defaults<br/>for DB"]
    end

    subgraph "Client Side (DTO)"
        D["StudentDTO<br/>id, name, email, age<br/>NO password<br/>NO internal fields"]
    end

    A -->|"DB → Client"| B --> D
    D -->|"Client → DB"| C --> A

    style A fill:#ffcdd2
    style D fill:#c8e6c9
    style B fill:#fff9c4
    style C fill:#fff9c4
```

### NAHI HUA TO (Without DTO)?
```
BINA DTO — Entity seedha return kiya:
  1. Client ko password dikhega → SECURITY DISASTER
  2. DB schema badla (10 naye columns) → API response badal gaya → frontend BREAK
  3. Har internal field exposed → attacker ko internal structure pata chal gaya

WITH DTO:
  1. password NAHI jaata → SAFE
  2. DB badla → DTO same → API same → frontend SAFE
  3. Sirf zaroori fields → clean, professional API
```

---

# 8. REPOSITORY

### REAL WORLD ANALOGY
SBI ka ATM soch — tu card dalta, PIN deta, amount bolta, cash nikalta. Andar kya chal raha? Network call? DB check? Hardware? Security? TUJHE PATA NAHI — kisi ne KHUD ATM bana ke de diya, tu sirf USE karta. Ab imagine kar — tu khud ATM banaye. Hardware, software, cash management, bank integration, security — 6 mahine ka kaam sirf ek ATM ke liye.

### YE KYA HAI?
Repository yehi hai — `JpaRepository` extend karte hi tujhe READY-MADE "ATM" mil jaata. `save()`, `findAll()`, `findById()`, `delete()` — sab FREE, bina ek line SQL likhe. Jaise SBI ne ATM bana ke diya, Spring Data JPA ne Repository bana ke diya. Tu sirf interface likho — implementation Spring ka kaam. JDBC se karta toh 30 lines per method — yahan 0 lines.

### StudentRepository.java — DB Se Baat

```java
@Repository
public interface StudentRepository extends JpaRepository<Student, Long> {
    // FREE: save(), findAll(), findById(), deleteById(), count(), existsById()

    Optional<Student> findByEmail(String email);         // Derived Query
    List<Student> findByName(String name);               // Derived Query
    List<Student> findByAgeGreaterThan(int age);         // Derived Query

    @Query(value = "SELECT * FROM students WHERE age > :age", nativeQuery = true)
    List<Student> findStudentsOlderThan(@Param("age") int age);    // Native SQL

    @Query("SELECT s FROM Student s WHERE s.email LIKE %:domain")
    List<Student> findByEmailDomain(@Param("domain") String domain);  // JPQL
}
```

### @Repository — Deep
```
YE KYA HAI:  Spring ko bolo: ye class DB access layer hai.
KAISE KAAM:  1. Spring isko bean banata (singleton object)
             2. DB exceptions ko Spring exceptions mein convert karta (DataAccessException)
             3. Component scan mein detect hota

KYUN LAGATE: @Component bhi chal jaata — lekin @Repository INTENT batata ki ye DB layer hai.
             Plus exception translation bonus milta.
```

### JpaRepository<Student, Long> — Deep
```
Student = kaunsi Entity ke liye? Student table.
Long    = Primary Key ka type? Long (id field).

EXTEND KARTE HI FREE MILTA:
  save(entity)        — INSERT ya UPDATE (id hai toh update, nahi toh insert)
  findById(id)        — SELECT WHERE id = ?
  findAll()           — SELECT * (sab records)
  findAll(Pageable)   — SELECT with LIMIT + OFFSET (pagination)
  deleteById(id)      — DELETE WHERE id = ?
  count()             — SELECT COUNT(*)
  existsById(id)      — EXISTS check

EK LINE BHI CODE NAHI LIKHA — Spring ne sab implement kar diya.
```

### 3 Tarike se Query — Deep

**1. Derived Query (Method naam se auto)**
```java
findByName(String name)           → SELECT * FROM students WHERE name = ?
findByAgeGreaterThan(int age)     → SELECT * FROM students WHERE age > ?
findByEmailAndName(email, name)   → SELECT * FROM students WHERE email = ? AND name = ?

KAISE: Spring method ka naam parse karta:
       "findBy" + "Name" → field "name" pe WHERE condition.
       "GreaterThan" → > operator.
       Tu bas method likho — implementation Spring karega.
```

**2. JPQL (Java Persistence Query Language)**
```java
@Query("SELECT s FROM Student s WHERE s.email LIKE %:domain")
// Student = CLASS NAAM (table nahi!)
// s.email = FIELD NAAM (column nahi!)
// DB independent — MySQL se PostgreSQL pe switch karo, query same chalegi.
```

**3. Native SQL (Raw SQL)**
```java
@Query(value = "SELECT * FROM students WHERE age > :age", nativeQuery = true)
// students = TABLE NAAM (class nahi!)
// nativeQuery = true batata ki ye raw SQL hai
// DB dependent — MySQL syntax likhi hai, PostgreSQL pe tod sakti hai.
```

### @Param — Kya hai?
```java
@Param("age") int age
// Method parameter "age" ko query mein ":age" se bind karta.
// Bina @Param: Spring ko pata nahi konsa parameter kahan jaaye.
```

### Mermaid — Repository Query Resolution

```mermaid
flowchart TD
    A["Service calls:<br/>studentRepository.findByEmail('a@b.com')"] --> B{"How does Spring<br/>know what SQL<br/>to generate?"}

    B --> C["Derived Query<br/>Method naam parse karo"]
    B --> D["JPQL<br/>@Query with entity names"]
    B --> E["Native SQL<br/>@Query with nativeQuery=true"]

    C --> C1["findByEmail<br/>→ findBy + Email<br/>→ WHERE email = ?"]
    D --> D1["SELECT s FROM Student s<br/>→ CLASS naam use<br/>→ DB independent"]
    E --> E1["SELECT * FROM students<br/>→ TABLE naam use<br/>→ DB dependent"]

    C1 --> F["Hibernate generates SQL<br/>→ JDBC execute<br/>→ ResultSet → Entity"]
    D1 --> F
    E1 --> F

    style C fill:#c8e6c9
    style D fill:#bbdefb
    style E fill:#ffcdd2
```

### NAHI HUA TO (Without Repository)?
```
BINA JpaRepository:
  Har CRUD ke liye manually:
    - Connection open karo
    - SQL likho
    - PreparedStatement banao
    - Parameters set karo
    - Execute karo
    - ResultSet loop karo
    - Entity mein map karo
    - Connection close karo
    - Exception handle karo
  = 30 lines per method. 5 CRUD methods = 150 lines.

WITH JpaRepository:
  Interface likho — 0 lines implementation.
  save(), findAll(), findById(), delete() — sab FREE.
  = 150 lines → 0 lines. MAGIC.
```

---

# 8B. HIKARICP — CONNECTION POOL

### REAL WORLD ANALOGY
Office aur Restaurant soch. Office mein 100 workers, saamne Restaurant (MySQL DB). Har worker khana khane jaata — **shuttle service** chahiye.
- Bina shuttle (no pool): Har worker paidal jaaye → 30 min lagta → sab bhookhe marein.
- With shuttles (pool): Office ke paas **10 shuttles ready**. Worker chahiye → shuttle le → restaurant jao → khao → shuttle wapas → next worker ke liye tayyar.

**Problem kab hoti?** Agar koi worker shuttle le ke restaurant mein fas gaya (long lock), shuttle wapas nahi aa rahi. Ek ek karke 10 shuttles gone. 11th worker shuttle maange → 30 sec wait → **TIMEOUT** → "ja paidal khao" → WORK FAIL.

### YE KYA HAI?
**HikariCP yehi karta** — Spring Boot ka default **Java connection pool library**. Tera app (office) aur MySQL (restaurant) ke beech mein **reusable DB connections ka bucket** manage karta.
- Har DB query ke liye new TCP connection = SLOW (100ms+ handshake per query)
- Pool se reuse = FAST (<1ms overhead)
- Default: 10 connections, 30 sec connection-timeout

### KYUN CHAHIYE?
- **Performance** — connection banana expensive hai, reuse karo
- **Concurrency** — multiple requests simultaneously handle
- **Resource management** — DB ko overwhelm nahi karte (infinite connections se DB crash)
- **Stability** — pool size tune karke app ka DB load cap karo

### NAHI HUA TO KYA HOGA?
- **Bina pool:** har request = new connection = DB crash under load
- **Default pool too small:** high-traffic → pool exhaustion → request timeout → 500 errors
- **No timeout config:** stuck query pool hog karegi → cascade failure
- **No monitoring:** silent exhaustion → production down before alert

### KEY CONFIGS (application.properties)

```properties
# Max concurrent DB connections from this app
spring.datasource.hikari.maximum-pool-size=10

# Always keep this many ready (warm)
spring.datasource.hikari.minimum-idle=2

# Max wait when pool is full (milliseconds)
spring.datasource.hikari.connection-timeout=30000    # 30s

# Drop unused connections after this
spring.datasource.hikari.idle-timeout=600000         # 10 min

# Force recycle connection
spring.datasource.hikari.max-lifetime=1800000        # 30 min

# Readable name in logs
spring.datasource.hikari.pool-name=StudentMgmtHikariPool
```

### 🔥 PRODUCTION INCIDENT PATTERN — "Pool Exhaustion"

```
Scenario (real incident reference):
1. External reporting tool runs huge UPDATE — locks tables 15 min
2. Your app's SELECT queries hit same tables → wait for locks
3. Connections HELD (not returned to pool)
4. Pool fills: active=10, idle=0, waiting=50+
5. Naya request aaya → 30 sec wait → TIMEOUT
6. Error log: "Connection is not available, request timed out after 30000ms"
7. App fails → downstream services affected → customers angry
```

**Debugging steps:**
1. **Check HikariCP logs** — "Connection is not available" signature error
2. **Check pool metrics** — active/idle/waiting counts via Actuator
3. **Go to DB** — RDS top waits, find blocking queries
4. **Fix root cause** — kill blocking query, fix code, separate pools

### ANALOGY DEEPER — Pool Exhaustion

```
Shuttle Fleet (Pool):
  [Shuttle 1] [Shuttle 2] [Shuttle 3] ... [Shuttle 10]

Normal Day:
  Active: 3 shuttles in use
  Idle: 7 shuttles at office ready
  Health: 

Bad Day (External Tool Running UPDATE):
  All 10 shuttles sent to restaurant
  Workers there stuck — restaurant locked up (tables locked)
  Back at office: 50 workers in queue for shuttle
  New worker arrives → 30 sec wait → timeout → "Go paidal"

Fix: 
  - Kill the blocking batch job (unlock tables)
  - OR: query timeout so stuck queries auto-release
  - OR: separate shuttle fleet for reporting (separate pool)
```

### INTERVIEW TALKING POINTS

**Q: "What connection pool do you use?"**
> "HikariCP — Spring Boot's default. It's the fastest Java connection pool library. In our student project, I've explicitly configured it with maxPoolSize 10, connection-timeout 30s, and a named pool for log readability."

**Q: "What happens when the pool is exhausted?"**
> "All connections in use and new requests piled up. After connection-timeout (30s default), HikariCP throws `SQLTransientConnectionException: Connection is not available, request timed out after 30000ms`. I've actually debugged this in production — root cause was an external tool locking tables, which held our app's connections."

**Q: "How would you prevent pool exhaustion?"**
> "Multiple layers:
> 1. Monitor pool metrics — alert on utilization >80%
> 2. Set query timeouts so stuck queries auto-release connections
> 3. Separate pools for OLTP vs batch/reporting workloads
> 4. Read replicas for reporting to offload main DB
> 5. Rightsize pool — not too big (DB suffers), not too small (app bottleneck)"

---

# 9. SERVICE

### REAL WORLD ANALOGY
Restaurant soch. WAITER customer se order leta — "Table 5 ko biryani". Waiter kitchen mein NAHI ghusta, order slip kitchen ko deta. CHEF kitchen mein khana banata — ingredients check, recipe follow, spice tune, taste test, plate mein serve. Chef customer se SEEDHA baat NAHI karta — waiter ke through hi. Baar baar same dish aaye? Ready plate cache mein rakhi (display fridge).

### YE KYA HAI?
Service yehi hai — tera Chef. Controller (waiter) order leta, Service (chef) actual kaam karta — validation, business logic, DB se data lao, transform, cache, return. `@Service` annotation = Spring ko batata ye kitchen hai. `@Autowired` se ingredients (Repository) inject hote. `@Cacheable` = ready dish display fridge mein. Separation of concerns — chef kitchen mein, waiter bahar, storekeeper storage mein.

### StudentService.java — Business Logic Layer

```java
@Service
public class StudentService {

    @Autowired
    private StudentRepository studentRepository;

    @CacheEvict(value = "students", allEntries = true)
    public StudentDTO addStudent(StudentDTO dto) { ... }

    public Page<StudentDTO> getAllStudents(Pageable pageable) { ... }

    @Cacheable(value = "student", key = "#id")
    public StudentDTO getStudentById(Long id) { ... }

    @CachePut(value = "student", key = "#id")
    public StudentDTO updateStudent(Long id, StudentDTO dto) { ... }

    @CacheEvict(value = "student", key = "#id")
    public void deleteStudent(Long id) { ... }
}
```

### @Service — Deep
```
YE KYA HAI:  Spring ko bolo: ye class BUSINESS LOGIC layer hai.
KAISE KAAM:  Spring isko bean banata — singleton object create, dependency inject.
ANDAR KYA:   @Service = @Component ka specialized version. Functionally same.

KYUN @Service AUR @Component NAHI?
  - @Component = generic. Kuch bhi ho sakta.
  - @Service   = INTENT batata: ye business logic hai.
  - Interview mein puche: "Service mein kya aata?" → Logic. Validation. Transformation. Transaction.
  - Controller mein logic MAT likho — testable nahi hoga, reusable nahi hoga.

BINA @Service: Spring isko bean nahi banayega → @Autowired fail → NullPointerException
```

### @Autowired — Deep
```java
@Autowired
private StudentRepository studentRepository;

YE KYA HAI:  Spring khud object dega — tu new StudentRepository() MAT kar.
KAISE KAAM:  Spring container mein StudentRepository ka bean dhundhta → inject karta.
             Ye = Dependency Injection (DI). Spring ka core concept.

BINA @Autowired: studentRepository = null → NullPointerException pehli call pe.

BETTER TARIKA (Constructor Injection):
  private final StudentRepository repo;
  public StudentService(StudentRepository repo) { this.repo = repo; }
  // @Autowired ki zaroorat nahi — Spring automatically constructor se inject karega
  // Interview mein bolo: "Constructor injection better hai kyunki field immutable hoti, test mein mock easy"
```

### @Cacheable — Deep
```java
@Cacheable(value = "student", key = "#id")
public StudentDTO getStudentById(Long id) { ... }

YE KYA HAI:  Pehli call → DB se le, cache mein rakh. Agli call → cache se de, DB skip.
value = "student" → cache ka naam (like a HashMap ka naam)
key = "#id"       → cache key (like HashMap ka key) — har id ka alag entry

FLOW:
  getStudentById(1) — PEHLI BAAR:
    Cache mein "student::1" dhundha → NAHI MILA → DB se laya → cache mein rakha → return
  getStudentById(1) — DOOSRI BAAR:
    Cache mein "student::1" dhundha → MIL GAYA → DB call SKIP → seedha return

BINA CACHE: 1000 users same student dekhe = 1000 DB calls. SLOW.
CACHE SE:   1 DB call + 999 cache hits. FAST.
```

### @CachePut — Deep
```java
@CachePut(value = "student", key = "#id")
public StudentDTO updateStudent(Long id, StudentDTO dto) { ... }

YE KYA HAI:  HAMESHA method chalao (DB update) + cache bhi update karo.

@Cacheable vs @CachePut:
  @Cacheable — cache mein hai? → haan → DB SKIP, cache se de.
  @CachePut  — cache mein hai? → FARAK NAHI PADTA → method HAMESHA chalao → naya result cache mein daalo.

KYUN UPDATE PE @CachePut:
  Student update kiya DB mein → agar cache mein purana data raha → client ko GALAT data dikhega.
  @CachePut ensure karta: DB + cache DONO mein latest data.
```

### @CacheEvict — Deep
```java
@CacheEvict(value = "student", key = "#id")      // DELETE pe — ek entry hatao
@CacheEvict(value = "students", allEntries = true) // CREATE pe — poora cache saaf

YE KYA HAI:  Cache se entry hatao.

DELETE PE: Student delete kiya → cache mein kyun rakhe? Ghost data dikhega.
CREATE PE: Naya student add kiya → purani list cache mein hai → outdated.
           allEntries = true → poora "students" cache saaf karo.
```

### orElseThrow() — Deep
```java
studentRepository.findById(id)
    .orElseThrow(() -> new RuntimeException("Student not found with id: " + id));

findById(id) → Optional<Student> return karta (null safe wrapper)
orElseThrow() → Student mila? → return. Nahi mila? → exception throw.
Ye exception GlobalExceptionHandler pakdega → client ko 404 JSON response dega.
```

### Mermaid — Service Layer Cache Strategy

```mermaid
flowchart TD
    subgraph "CREATE (addStudent)"
        A1["@CacheEvict<br/>allEntries=true"] --> A2["students cache<br/>POORA SAAF"]
        A2 --> A3["DB mein INSERT"]
    end

    subgraph "READ (getStudentById)"
        B1["@Cacheable<br/>key=#id"] --> B2{"Cache mein hai?"}
        B2 -->|"HIT"| B3["Return from cache<br/>DB SKIP"]
        B2 -->|"MISS"| B4["DB se fetch"]
        B4 --> B5["Cache mein store"]
        B5 --> B3
    end

    subgraph "UPDATE (updateStudent)"
        C1["@CachePut<br/>key=#id"] --> C2["DB mein UPDATE"]
        C2 --> C3["Cache mein bhi<br/>NAYA data store"]
    end

    subgraph "DELETE (deleteStudent)"
        D1["@CacheEvict<br/>key=#id"] --> D2["DB se DELETE"]
        D2 --> D3["Cache se bhi<br/>entry REMOVE"]
    end

    style B3 fill:#c8e6c9
    style A2 fill:#ffcdd2
    style C3 fill:#fff9c4
    style D3 fill:#ffcdd2
```

### CHEF KA KAAM — Step by Step
```
CHEF (SERVICE) KA KAAM:
  1. Order aaya (DTO) → check karo valid hai
  2. Ingredients maango (Repository.findById)
  3. Cook karo (business logic)
  4. Plate mein serve (DTO banao → return)
  5. Agar same order baar baar aaye → ready plate dedo (Cache)

WAITER (CONTROLLER) KABHI KITCHEN MEIN NAHI JAATA.
CHEF KABHI CUSTOMER SE SEEDHA BAAT NAHI KARTA.
= SEPARATION OF CONCERNS. Professional code.
```

---

# 10. CONTROLLER

### REAL WORLD ANALOGY
Hotel ka FRONT DESK RECEPTIONIST soch. Customer aaya — receptionist poochhti: "Kya chahiye sir?" Customer bolta: "Room 502 chahiye." Receptionist form fill karwati, ID check karti (validation), phir housekeeping ko message bhejti — "502 prepare karo." Khud room mein NAHI jaati, bed nahi banati — bas intermediary hai customer aur service ke beech.

### YE KYA HAI?
Controller yehi hai — tera RECEPTIONIST. HTTP request leta, parse karta, validate karta (`@Valid`), phir Service ko bolta "ye kaam karo." Khud BUSINESS LOGIC NAHI likhta. `@RestController` = "ye receptionist REST API handle karti", `@RequestMapping("/api/students")` = "ye receptionist students department ki hai." `@RequestBody` = form padho, `@PathVariable` = URL se info nikalo, `ResponseEntity` = response envelope banao with proper status code.

### StudentController.java — REST API Endpoints

```java
@RestController                      // REST API — JSON response
@RequestMapping("/api/students")     // Base URL
public class StudentController {

    @Autowired
    private StudentService studentService;

    @PostMapping                     // POST /api/students
    public ResponseEntity<StudentDTO> create(@Valid @RequestBody StudentDTO dto) { ... }

    @GetMapping                      // GET /api/students?page=0&size=5&sort=name,asc
    public ResponseEntity<Page<StudentDTO>> getAll(Pageable pageable) { ... }

    @GetMapping("/{id}")             // GET /api/students/5
    public ResponseEntity<StudentDTO> getById(@PathVariable Long id) { ... }

    @PutMapping("/{id}")             // PUT /api/students/5
    public ResponseEntity<StudentDTO> update(@PathVariable Long id, @Valid @RequestBody StudentDTO dto) { ... }

    @DeleteMapping("/{id}")          // DELETE /api/students/5
    public ResponseEntity<String> delete(@PathVariable Long id) { ... }

    @GetMapping("/search")           // GET /api/students/search?name=Arpan
    public ResponseEntity<List<StudentDTO>> search(@RequestParam ...) { ... }
}
```

### @RestController — Deep
```
YE KYA HAI:  2-in-1 = @Controller + @ResponseBody
@Controller     = Spring MVC controller (HTML pages return karta)
@ResponseBody   = return value ko JSON mein convert karo (Jackson library)
@RestController = dono combine — controller hai + JSON return karega

BINA @RestController:  Spring isko controller nahi samjhega → URL map nahi hoga → 404
```

### @RequestMapping("/api/students") — Deep
```
YE KYA HAI:  Base URL set karo. Andar ke sab mappings iske aage add honge.
EXAMPLE:     @GetMapping("/{id}") → actual URL = GET /api/students/{id}
             @PostMapping         → actual URL = POST /api/students

KYUN /api/ prefix: Frontend routes (/home, /about) aur API routes (/api/students) alag.
```

### @Valid — Deep
```java
public ResponseEntity<StudentDTO> create(@Valid @RequestBody StudentDTO dto)

YE KYA HAI:  Request body ki validation trigger karo.

BINA @Valid:  @NotBlank, @Email, @Min — sab IGNORE honge. Kuch nahi hoga.
WITH @Valid:  Har validation check hoga. Fail? → MethodArgumentNotValidException → 400 error.

COMMON GALTI: Entity pe validations lagaye, Controller mein @Valid bhool gaye → sab pass ho jaata.
```

### @RequestBody — Deep
```java
@RequestBody StudentDTO dto

YE KYA HAI:  HTTP request ka JSON body → Java object mein convert.
CLIENT NE BHEJA: { "name": "Arpan", "email": "a@b.com", "age": 25 }
SPRING NE KIYA:  StudentDTO dto = new StudentDTO(); dto.setName("Arpan"); ...

BINA @RequestBody: dto = null ya empty → NullPointerException.
JACKSON library ye JSON ↔ Java conversion karta (Spring Boot mein auto-included).
```

### @PathVariable — Deep
```java
@GetMapping("/{id}")
public ResponseEntity<StudentDTO> getById(@PathVariable Long id)

YE KYA HAI:  URL se value nikalo.
URL:          GET /api/students/5
@PathVariable: id = 5

DIFFERENCE: @PathVariable = URL ka hissa (/students/5)
            @RequestParam = query parameter (/students?id=5)
```

### @RequestParam — Deep
```java
@GetMapping("/search")
public ResponseEntity<List<StudentDTO>> search(
    @RequestParam(required = false) String name,
    @RequestParam(required = false) Integer minAge)

YE KYA HAI:  URL ke ? ke baad ke parameters.
URL:          GET /api/students/search?name=Arpan&minAge=20
              name = "Arpan", minAge = 20

required = false → nahi diya toh null (error nahi)
required = true (default) → nahi diya toh 400 error
```

### Pageable — Deep
```java
public ResponseEntity<Page<StudentDTO>> getAll(Pageable pageable)

YE KYA HAI:  Spring automatically URL se pagination parameters le leta.
URL:          GET /api/students?page=0&size=5&sort=name,asc
SPRING KARTA: Pageable object banata → page=0, size=5, sort=name ASC

RESPONSE:
{
  "content": [ ... 5 students ... ],
  "totalPages": 10,
  "totalElements": 50,
  "number": 0        ← current page
}

BINA PAGEABLE: findAll() → 10 lakh records ek saath → memory full → app crash.
```

### ResponseEntity — Deep
```java
return new ResponseEntity<>(saved, HttpStatus.CREATED);   // 201
return ResponseEntity.ok(data);                            // 200
return ResponseEntity.badRequest().body(error);            // 400
return ResponseEntity.status(401).body(error);             // 401

YE KYA HAI:  HTTP response ka full control — body + status code + headers.
BINA ISKE:   Hamesha 200 return hoga — client ko pata nahi ki create hua ya error.
WITH:        201 = created, 400 = bad request, 404 = not found. Proper REST.
```

### Mermaid — REST API Endpoints Map

```mermaid
flowchart TD
    subgraph "StudentController (/api/students)"
        A["POST /<br/>create()"] --> A1["201 Created<br/>{student data}"]
        B["GET /<br/>getAll()"] --> B1["200 OK<br/>{paginated list}"]
        C["GET /{id}<br/>getById()"] --> C1["200 OK<br/>{single student}"]
        D["PUT /{id}<br/>update()"] --> D1["200 OK<br/>{updated student}"]
        E["DELETE /{id}<br/>delete()"] --> E1["200 OK<br/>'Deleted'"]
        F["GET /search<br/>search()"] --> F1["200 OK<br/>{filtered list}"]
    end

    style A fill:#c8e6c9
    style B fill:#bbdefb
    style C fill:#bbdefb
    style D fill:#fff9c4
    style E fill:#ffcdd2
    style F fill:#bbdefb
```

### NAHI HUA TO (Without Controller)?
```
BINA CONTROLLER:
  Client request bheje → koi sun hi nahi raha → 404 Not Found.
  Receptionist nahi toh visitor kahan jaaye?

BINA @Valid:
  Client blank name bheje → save ho jaayega → DB mein garbage data.
  @Valid = guard jo check karta "form complete hai?"

BINA @RequestBody:
  Client ne JSON bheja → Spring ko pata nahi → dto = null → NullPointerException.
  @RequestBody = "JSON ko Java object mein badlo".

BINA ResponseEntity:
  Hamesha 200 OK → client ko pata nahi create hua ya error hua.
  Professional API = proper status codes. 201, 400, 404, 500.
```

---

# 11. AUTH

### REAL WORLD ANALOGY
MEAT GRINDER soch. Meat daalo → grinder ghumao → keema nikle. Keema se WAPAS meat NAHI banta — one way hai. Same meat 2 baar daalo → har baar ALAG keema (kyun? machine mein thoda spice/salt add hota — salt = randomness). Ab koi thief keema chura le? Useless — usse wapas meat nahi ban sakta. Asli meat verify karna? Thoda meat lo, grinder mein daalo, compare keema banaya hua.

### YE KYA HAI?
BCrypt yehi hai — PASSWORD ka MEAT GRINDER. Register pe password → BCrypt hash (keema). DB mein hash store, password NAHI. Login pe user password deta → BCrypt us password ko grind karta same salt ke saath → compare with stored hash. Match? Login. Nahi? Reject. DB leak = sirf hashes leak = attacker password recover NAHI kar sakta. Saath mein JWT token milta login ke baad — ye next section mein.

### AuthController.java — Login/Register

```java
@RestController
@RequestMapping("/api/auth")
public class AuthController {

    private Map<String, String> users = new HashMap<>();    // In-memory user store
    private BCryptPasswordEncoder encoder = new BCryptPasswordEncoder();

    @PostMapping("/register")   // POST /api/auth/register
    public ResponseEntity<String> register(@RequestBody Map<String, String> request) {
        String username = request.get("username");
        String password = request.get("password");
        users.put(username, encoder.encode(password));   // HASH karke store
        return ResponseEntity.ok("Registered successfully");
    }

    @PostMapping("/login")      // POST /api/auth/login
    public ResponseEntity<Map<String, String>> login(@RequestBody Map<String, String> request) {
        // BCrypt.matches() se verify → sahi toh JWT token do
        String token = JwtUtil.generateToken(username);
        return ResponseEntity.ok(Map.of("token", token));
    }
}
```

### BCryptPasswordEncoder — Deep
```
YE KYA HAI:  Password hashing algorithm. One-way. Reverse NAHI ho sakta.

FLOW:
  Register: "mypassword" → BCrypt → "$2a$10$xYz..." (hash) → DB mein store
  Login:    "mypassword" diya → BCrypt.matches("mypassword", storedHash) → true/false

KYUN BCRYPT:
  1. One-way — hash se password nahi nikal sakta.
  2. Salt — same password ka har baar ALAG hash. Rainbow table attack fail.
  3. Slow (intentionally) — brute force attack slow ho jaata.

GALAT: password store as plain text → DB leak = sab passwords leak.
SAHI:  BCrypt hash store → DB leak = hashes leak → password recover NAHI ho sakta.
```

### In-Memory Users (HashMap) — Note
```
Ye DEMO ke liye hai. Real project mein:
  - User entity banao (username, password, roles)
  - UserRepository banao
  - DB mein save karo
  - App restart = users safe (HashMap mein restart = sab gone)
```

### Mermaid — Auth Flow (Register + Login)

```mermaid
sequenceDiagram
    participant C as Client
    participant AC as AuthController
    participant BC as BCrypt
    participant HM as HashMap (Users)
    participant JU as JwtUtil

    Note over C,JU: === REGISTER ===
    C->>AC: POST /api/auth/register<br/>{username: "arpan", password: "abc123"}
    AC->>HM: users.containsKey("arpan")?
    HM-->>AC: false (naya user)
    AC->>BC: encode("abc123")
    BC-->>AC: "$2a$10$xYzSaltHash..."
    AC->>HM: put("arpan", "$2a$10$xYz...")
    AC-->>C: 200 "Registered successfully"

    Note over C,JU: === LOGIN ===
    C->>AC: POST /api/auth/login<br/>{username: "arpan", password: "abc123"}
    AC->>HM: get("arpan")
    HM-->>AC: "$2a$10$xYzSaltHash..."
    AC->>BC: matches("abc123", "$2a$10$xYz...")
    BC-->>AC: true (password MATCH!)
    AC->>JU: generateToken("arpan")
    JU-->>AC: "eyJhbGciOiJIUzI1NiJ9..."
    AC-->>C: 200 {token: "eyJhbGci..."}
```

### BCrypt Hash Sample — Salt + One-Way Detail
```
PLAIN TEXT PASSWORD = SABSE BADI GALTI. DB leak → sab passwords leak.

  "abc123" → "$2a$10$salt1...hash1"  (pehli baar)
  "abc123" → "$2a$10$salt2...hash2"  (doosri baar — ALAG hash!)

  matches() kaise kaam karta:
    Hash ke andar salt embedded → extract → same salt se input hash → compare.
    Attacker ke paas hash hai lekin password nahi nikal sakta. SAFE.
```

---

# 12. EXCEPTION HANDLING

### REAL WORLD ANALOGY
HOSPITAL EMERGENCY ROOM soch. Patient (request) aaya, doctor (controller) ne kuch galat kiya — ab kya? Patient ki body road pe mat pheko! Emergency room (handler) sambhalta — family ko CLEAN message: "Minor issue hua, handle kar diya." Internal details (surgery error, wrong medicine) public mein NAHI. Na family ko darr lage, na hospital ki reputation kharab.

### YE KYA HAI?
GlobalExceptionHandler yehi hai — tera EMERGENCY ROOM. Kahi bhi exception throw ho — `@RestControllerAdvice` classes ke beech beech pakadti hai aur CLEAN JSON response bhejti. Stack trace client tak nahi jaata (security risk). `@ExceptionHandler(RuntimeException.class)` = "ye handler runtime errors ke liye." `MethodArgumentNotValidException` = validation fail → 400 error with field details. Client ko professional message, internal paths hidden.

### GlobalExceptionHandler.java

```java
@RestControllerAdvice
public class GlobalExceptionHandler {

    @ExceptionHandler(RuntimeException.class)
    public ResponseEntity<Map<String, String>> handleRuntime(RuntimeException ex) {
        return new ResponseEntity<>(Map.of("error", ex.getMessage()), HttpStatus.NOT_FOUND);
    }

    @ExceptionHandler(MethodArgumentNotValidException.class)
    public ResponseEntity<Map<String, String>> handleValidation(MethodArgumentNotValidException ex) {
        // Har field ki error nikalo aur map mein daalo
        return new ResponseEntity<>(errors, HttpStatus.BAD_REQUEST);
    }
}
```

### @RestControllerAdvice — Deep
```
YE KYA HAI:  Global exception handler. KAHI BHI exception throw ho — ye pakdega.
KAISE KAAM:  Spring scan karta → @RestControllerAdvice mila → sab controllers ke exceptions yahan route.

@ControllerAdvice = HTML error pages return karta.
@RestControllerAdvice = JSON error response return karta. (@ControllerAdvice + @ResponseBody)

BINA ISKE:  Client ko ugly stack trace dikhega:
  "java.lang.RuntimeException: Student not found at com.example..."
WITH:       Clean JSON:
  { "error": "Student not found with id: 99" }
```

### @ExceptionHandler — Deep
```java
@ExceptionHandler(RuntimeException.class)

YE KYA HAI:  Ye method TAB chalega jab RuntimeException throw ho.
SPRING:     Exception type match karta → closest match wala handler call karta.

RuntimeException → "Student not found" → 404
MethodArgumentNotValidException → @Valid fail → 400 with field errors

EXAMPLE 400 response:
{
  "name": "Name is required",
  "email": "Invalid email",
  "age": "Age must be at least 18"
}
```

### Mermaid — Exception Handling Flow

```mermaid
flowchart TD
    A["Controller/Service<br/>throws Exception"] --> B["@RestControllerAdvice<br/>GlobalExceptionHandler"]
    B --> C{"Exception<br/>Type?"}

    C -->|"RuntimeException<br/>'Student not found'"| D["handleRuntime()"]
    D --> E["404 NOT FOUND<br/>{error: 'Student not found'}"]

    C -->|"MethodArgument<br/>NotValidException<br/>(@Valid failed)"| F["handleValidation()"]
    F --> G["400 BAD REQUEST<br/>{name: 'required',<br/>email: 'invalid'}"]

    C -->|"Any other<br/>Exception"| H["Default handler"]
    H --> I["500 INTERNAL ERROR<br/>{error: 'Something wrong'}"]

    style E fill:#fff9c4
    style G fill:#ffcdd2
    style I fill:#ef9a9a
```

### REAL EXAMPLE — Before vs After
```
BINA EXCEPTION HANDLER:
  "java.lang.RuntimeException: Student not found at com.example.practice.service..."
  = Ugly error message client ko → security risk (internal paths leak)

WITH EXCEPTION HANDLER:
  { "error": "Student not found with id: 99" }
  = Professional. Clean. Secure.
```

---

# 13. SECURITY — JWT

### REAL WORLD ANALOGY
AMUSEMENT PARK soch. Counter pe ticket ka paisa do → ek WRISTBAND milta. Band pe likha: tera naam, entry time, valid till 3 PM, park ka stamp (seal). Har ride pe guard: "Band dikhaao!" — stamp asli? time baaki? naam match? Sab ✓ → "Enjoy!" Nakli band? "Niklo!" Server pe kuch store NAHI — sab info BAND mein hai. 10 park servers mein kisi pe bhi band dikhaao — chalega. Band khud proof hai.

### YE KYA HAI?
JWT (JSON Web Token) yehi hai — tera DIGITAL WRISTBAND. Login ke baad server ek token banata (signed + timed). Client har request ke saath token bhejta (Authorization header). Server token verify karta — signature valid? expired nahi? — phir access deta. Server pe SESSION nahi rakhta — token self-contained hai. 3 files milke ye karte: `JwtUtil` (band banata), `JwtFilter` (har request pe check karta), `SecurityConfig` (kaunse URLs open, kaunse protected).

### VISUALIZE — JWT = AMUSEMENT PARK WRISTBAND
```
  ┌──────────────────────────────────────────────────────────────────┐
  │                    AMUSEMENT PARK (App)                           │
  │                                                                  │
  │  TICKET COUNTER (AuthController):                                │
  │    Register: "Naam batao, ID dikhaao" → record mein likho        │
  │    Login:    "ID verify karo" → ✓ sahi → WRISTBAND (JWT) do     │
  │                                  ✗ galat → "Niklo yahan se" 401  │
  │                                                                  │
  │  WRISTBAND (JWT Token) mein likha:                               │
  │    ┌───────────────────────────────────────┐                     │
  │    │ NAAM: Arpan          (payload/sub)    │                     │
  │    │ TIME: 2:00 PM        (issued at)      │                     │
  │    │ VALID TILL: 3:00 PM  (expiry)         │                     │
  │    │ STAMP: 🔏 park ka seal (signature)    │                     │
  │    └───────────────────────────────────────┘                     │
  │                                                                  │
  │  HAR RIDE (API request):                                         │
  │    GUARD (JwtFilter): "Wristband dikhaao!"                       │
  │    → Stamp asli? ✓  (signature valid)                            │
  │    → Time baaki? ✓  (not expired)                                │
  │    → Naam match? ✓  (username valid)                             │
  │    → "Jaao, enjoy karo!" → Controller tak pahuncho               │
  │                                                                  │
  │    → Stamp nakli? ✗ → "Ye NAKLI band hai!" → 403 Forbidden      │
  │    → Time khatam? ✗ → "Band expire ho gaya!" → 403              │
  │    → Band nahi?   ✗ → "Pehle counter pe jao!" → 403             │
  │                                                                  │
  │  OPEN AREAS (permitAll):                                         │
  │    Ticket counter (login/register) → bina band                   │
  │    Park map (Swagger docs) → bina band                           │
  │                                                                  │
  │  SERVER PE KUCH STORE NAHI — sab info BAND mein hai.             │
  │  10 parks (servers) mein kisi mein bhi band dikhaao — chalega.   │
  │  Ye = STATELESS. Scalable.                                       │
  └──────────────────────────────────────────────────────────────────┘
```

### 3 Files ka Flow:
```
LOGIN:
  Client → POST /api/auth/login → AuthController → JwtUtil.generateToken() → token return

HAR REQUEST:
  Client → "Authorization: Bearer <token>" → JwtFilter check:
    → Valid token?   → SecurityContext set → Controller tak pahuncha
    → Invalid/nahi?  → 403 Forbidden
```

### JwtUtil.java — Token Helper

```
SECRET KEY:  Token sign karne ke liye. Server ke paas honi chahiye sirf.
             Real mein: environment variable se laao. Code mein KABHI nahi.

generateToken(username):
  JWT builder → subject=username, issuedAt=now, expiry=1hr, sign=HMAC-SHA256 → token string

extractUsername(token):
  Token parse → signature verify → claims nikalo → subject (username) return

isTokenValid(token, username):
  Token se username nikalo → match karo + expiry check → true/false
```

### JWT Token Structure
```
eyJhbGciOiJIUzI1NiJ9.eyJzdWIiOiJBcnBhbiJ9.abc123signature

3 parts (dot separated):
  HEADER:    {"alg": "HS256"}              — algorithm
  PAYLOAD:   {"sub": "Arpan", "exp": ...}  — data (claims)
  SIGNATURE: HMAC-SHA256(header+payload, SECRET)  — tamper proof

Client token modify kare → signature match nahi → INVALID.
```

### JwtFilter.java — Gatekeeper

```java
public class JwtFilter extends OncePerRequestFilter {
    // OncePerRequestFilter — guarantee: ek request = ek baar filter

    protected void doFilterInternal(request, response, filterChain) {
        // 1. Authorization header nikalo
        // 2. "Bearer " hatao, sirf token lo
        // 3. Token se username nikalo
        // 4. Valid? → SecurityContext mein set (Spring jaane ye authenticated hai)
        // 5. filterChain.doFilter() — aage bhejo
    }
}
```

### SecurityContextHolder — Deep
```
YE KYA HAI:  Current authenticated user store karta. ThreadLocal based.
             Ek thread = ek request = ek user.

JwtFilter mein set kiya → Controller/Service mein kahi bhi access:
  SecurityContextHolder.getContext().getAuthentication().getName() → "Arpan"
```

### SecurityConfig.java — Rules

```java
@Configuration @EnableWebSecurity
public class SecurityConfig {

    @Bean
    public SecurityFilterChain filterChain(HttpSecurity http) {
        http
            .csrf(csrf -> csrf.disable())              // CSRF off — REST stateless hai
            .sessionManagement(STATELESS)               // Session off — JWT based
            .authorizeHttpRequests(auth -> auth
                .requestMatchers("/api/auth/**").permitAll()      // Login/Register OPEN
                .requestMatchers("/swagger-ui/**").permitAll()    // Swagger OPEN
                .anyRequest().authenticated()                     // Baaki sab PROTECTED
            )
            .addFilterBefore(new JwtFilter(), UsernamePasswordAuthenticationFilter.class);
    }
}
```

### CSRF Disable Kyun?
```
CSRF = Cross-Site Request Forgery. Browser-based attack.
REST API mein CSRF off kyunki:
  - Stateless hai — no cookies, no sessions.
  - JWT token Authorization header mein jaata — browser auto-attach nahi karta.
  - CSRF attack tab hota jab browser cookie auto-send karta — yahan cookies use hi nahi.
```

### STATELESS Session Kyun?
```
BINA JWT:  Server pe session store (JSESSIONID cookie) → user ka state server pe.
           Problem: 10 servers hain → user server-3 pe login → server-7 pe request gaya → session nahi mila!

JWT SE:    Server pe KUCH store nahi. Token mein sab info hai.
           Kisi bhi server pe request jaaye — token verify = access.
           Ye = STATELESS. Scalable.
```

### Mermaid — Security Config Decision Tree

```mermaid
flowchart TD
    A["Request aaya"] --> B{"URL kya hai?"}

    B -->|"/api/auth/**"| C["permitAll()<br/>OPEN — bina JWT"]
    B -->|"/swagger-ui/**"| C
    B -->|"/v3/api-docs/**"| C

    B -->|"Koi bhi aur URL"| D["authenticated() required"]
    D --> E{"JWT token<br/>header mein hai?"}

    E -->|"Nahi"| F["403 Forbidden<br/>'Pehle login karo'"]
    E -->|"Hai"| G{"Token valid?<br/>Expired nahi?<br/>Signature sahi?"}

    G -->|"Invalid/Expired"| H["403 Forbidden<br/>'Nakli/expired token'"]
    G -->|"Valid"| I["SecurityContext set<br/>Request → Controller"]

    C --> J["Request → Controller<br/>(No auth needed)"]

    style C fill:#c8e6c9
    style F fill:#ffcdd2
    style H fill:#ffcdd2
    style I fill:#c8e6c9
```

### Mermaid — Session vs JWT (Stateful vs Stateless)

```mermaid
graph TD
    subgraph "SESSION BASED (Purana)"
        SA["User login"] --> SB["Server creates SESSION<br/>JSESSIONID cookie"]
        SB --> SC["Server stores<br/>session in memory"]
        SC --> SD{"Load Balancer<br/>10 servers"}
        SD -->|"Request → Server 3"| SE["Session FOUND<br/>on Server 3"]
        SD -->|"Request → Server 7"| SF["Session NOT FOUND<br/>on Server 7<br/>FAIL!"]
    end

    subgraph "JWT BASED (Naya)"
        JA["User login"] --> JB["Server creates JWT<br/>Token = self-contained"]
        JB --> JC["Server stores<br/>NOTHING"]
        JC --> JD{"Load Balancer<br/>10 servers"}
        JD -->|"Request → Server 3"| JE["Token verify<br/>ACCESS!"]
        JD -->|"Request → Server 7"| JF["Token verify<br/>ACCESS!"]
    end

    style SF fill:#ffcdd2
    style JE fill:#c8e6c9
    style JF fill:#c8e6c9
```

### NAHI HUA TO (Without Security)?
```
BINA SECURITY CONFIG:
  Spring Security add karte hi SAB URLs blocked. Login page aata.
  SecurityConfig = rulebook. Guard ko batata "kisko rokna, kisko jaane dena."

BINA JWT FILTER:
  SecurityConfig mein .authenticated() lagaya → but koi CHECK nahi karta →
  Sab requests blocked → 403 har jagah.
  JwtFilter = actual guard jo token check karta.

BINA JWT UTIL:
  Login ke baad client ko kya doge? Session? Non-scalable.
  JWT = self-contained ticket. Server pe kuch store nahi. 10 servers mein koi bhi verify kar sake.

SECRET KEY LEAKED:
  Attacker apna token bana lega → kisi bhi user ban jaayega → TOTAL BREACH.
  Secret key = master key. Leak = poora building compromised.
```

---

# 14. FILTER

### REAL WORLD ANALOGY
Building ka MAIN GATE CCTV soch. Har visitor record — chahe delivery boy ho, guest ho, courier ho, cleaning staff ho — SAB log. Kisi ko miss NAHI karta. Broad coverage. Plus ek BOOM BARRIER bhi gate pe hai — barrier uthao toh visitor andar jaaye, nahi uthao toh gate pe hi ruk jaaye.

### YE KYA HAI?
Filter yehi hai — Servlet level ka GATE CCTV. Tomcat level pe kaam karta, Spring se bhi PEHLE. Har HTTP request — images, CSS, JS, API — SAB pe chalta. `doFilter()` = main method, `chain.doFilter()` = boom barrier uthao (aage bhejo). Nahi uthaya = request yahi ruk jaaye. `@Component` lagao = Spring auto-register karega. Broad logging aur CORS/encoding jaise kaam ke liye best.

### RequestLoggingFilter.java — Servlet Level

```java
@Component
public class RequestLoggingFilter implements Filter {

    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) {
        // LOG: Method, URL, IP
        chain.doFilter(request, response);   // aage bhejo
        // LOG: Status, Time
    }
}
```

### Filter — Deep
```
YE KYA HAI:  Servlet API ka part. Tomcat level pe kaam.
KAB CHALTA:  SABSE PEHLE — Spring se bhi pehle. Har HTTP request pe.
KYA LOG:     Images, CSS, JS, API — SAB kuch.

@Component lagaya → Spring auto-register karega → har request pe chalega.

IMPLEMENTS Filter (jakarta.servlet.Filter):
  - init()      — filter start pe ek baar
  - doFilter()  — HAR request pe
  - destroy()   — filter band hote waqt

chain.doFilter() ZAROORI:
  Call nahi kiya → request YAHI ruk jaayegi → controller tak nahi pahunchegi.
  Ye = "aage bhejo" kehna.
```

### Mermaid — Filter Execution Flow

```mermaid
sequenceDiagram
    participant C as Client
    participant F as RequestLoggingFilter
    participant DS as DispatcherServlet
    participant CT as Controller

    C->>F: GET /api/students/5
    Note over F: LOG: Method=GET, URL=/api/students/5, IP=127.0.0.1
    Note over F: startTime = System.currentTimeMillis()
    F->>DS: chain.doFilter() → AAGE BHEJO
    DS->>CT: Route to StudentController.getById(5)
    CT-->>DS: ResponseEntity (200 OK, JSON)
    DS-->>F: Response wapas aaya
    Note over F: LOG: Status=200, Time=15ms
    F-->>C: JSON Response
```

### NAHI HUA TO (Without Filter)?
```
Bug aaya production mein → pata nahi kaunsi request fail hui →
Kab hui? → Kaunsi IP se aayi? → Kitna time laga? → SAB ANDHERE MEIN.
Filter se sab logged. Debug easy.
```

---

# 15. INTERCEPTOR

### REAL WORLD ANALOGY
Hotel ke SPECIFIC FLOOR ki RECEPTIONIST soch. Gate ka CCTV (Filter) sab visitors record karta, lekin 5th floor ki receptionist sirf US FLOOR ke visitors note karti. Delivery boy gate se gaadi mein gaya → receptionist ko pata nahi. Sirf room ke guests ka record. Focused logging — floor-specific.

Saath mein WAITER jaisa bhi hai — guest ke andar jaane se PEHLE "kuch bhi chahiye?" (preHandle), guest ke jaane ke BAAD "table saaf karo" (afterCompletion).

### YE KYA HAI?
Interceptor yehi hai — Spring MVC level ka FLOOR RECEPTIONIST. Filter ke baad, Controller ke pehle/baad chalta. Sirf registered URLs pe — `/api/**` pe lagaya toh sirf API calls. CSS/JS/images IGNORE. `preHandle()` = andar jaane se pehle, `postHandle()` = controller ke baad, `afterCompletion()` = sab ke baad (cleanup). `WebConfig` mein register karna ZAROORI — nahi toh receptionist ko kaam nahi mila (dead).

### LoggingInterceptor.java — Spring Level

```java
@Component
public class LoggingInterceptor implements HandlerInterceptor {

    public boolean preHandle(request, response, handler) {
        // Controller se PEHLE — request aa rahi
        return true;    // true = jaane do, false = BLOCK
    }

    public void afterCompletion(request, response, handler, ex) {
        // Controller ke BAAD — response ja rahi
    }
}
```

### WebConfig.java — Interceptor Register
```java
@Configuration
public class WebConfig implements WebMvcConfigurer {

    @Autowired
    private LoggingInterceptor loggingInterceptor;

    public void addInterceptors(InterceptorRegistry registry) {
        registry.addInterceptor(loggingInterceptor)
                .addPathPatterns("/api/**");   // SIRF /api/ URLs pe
    }
}
```

### Interceptor — Deep
```
YE KYA HAI:  Spring MVC ka part. Controller level pe kaam.
KAB CHALTA:  Filter ke BAAD, Controller se pehle/baad mein.
KYA LOG:     Sirf woh URLs jo tune register kiye (/api/**).
             Static files (CSS/JS/images) pe NAHI chalta.

3 METHODS:
  preHandle()       — Controller se PEHLE. return true = jaane do. false = block.
  postHandle()      — Controller ke baad, view render se pehle. (REST mein rarely use)
  afterCompletion() — SAB ke baad. Response bhi gaya. Cleanup/logging ke liye.

WebConfig mein REGISTER KARNA ZAROORI:
  Bina register: Interceptor banana = kuch nahi hua. Spring ko pata hi nahi.
  addPathPatterns("/api/**") → sirf API routes pe. "/**" lagao toh sab pe.
```

### Mermaid — Interceptor 3 Hook Points

```mermaid
sequenceDiagram
    participant C as Client
    participant I as LoggingInterceptor
    participant CT as Controller
    participant S as Service

    C->>I: Request aaya
    Note over I: preHandle()<br/>[PRE] GET /api/students/5<br/>startTime = now()
    I->>CT: return true → JAANE DO
    CT->>S: getStudentById(5)
    S-->>CT: StudentDTO
    CT-->>I: Response ready
    Note over I: afterCompletion()<br/>[POST] GET /api/students/5 → 200 (12ms)
    I-->>C: Response
```

### WebConfig — Interceptor Registration (Code from project)
```java
// WebConfig.java — ZAROOR padhna! Bina iske interceptor DEAD hai.
@Configuration
public class WebConfig implements WebMvcConfigurer {

    @Autowired
    private LoggingInterceptor loggingInterceptor;

    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        registry.addInterceptor(loggingInterceptor)
                .addPathPatterns("/api/**");
        // /api/students     → INTERCEPT
        // /api/auth/login   → INTERCEPT
        // /swagger-ui.html  → SKIP (api nahi hai)
        // /css/style.css    → SKIP
    }
}
```

### NAHI HUA TO (Without Interceptor)?
```
BINA INTERCEPTOR:
  Sirf Filter hai → SAB requests log (images, CSS, font files bhi).
  Useful logs mein FLOOD aa jaayega. API ka log dhundhna = needle in haystack.

WITH INTERCEPTOR:
  /api/** pe lagaya → sirf API calls log.
  CSS/JS/images skip → clean logs. Debug FAST.

BINA WebConfig REGISTRATION:
  Interceptor class banayi + @Component lagaya → but DEAD padha rahega.
  = CCTV camera kharida lekin install nahi kiya.
  WebConfig = camera ko wall pe lagao + wire karo.
```

---

# 16. SWAGGER

### REAL WORLD ANALOGY
Restaurant ka MENU CARD soch. Sab dishes ek page pe — naam, price, ingredients, photo. Naya customer aaya? Menu padhe → order kare. Waiter se har dish poochhne ki zaroorat nahi. Aur jab chef naya dish add kare → menu AUTO update — redesign nahi karna.

### YE KYA HAI?
Swagger yehi hai — tera API ka DIGITAL MENU CARD. `springdoc-openapi` library automatically tere `@RestController` classes ko scan karke sab APIs ka documentation generate karti. Browser mein `/swagger-ui.html` kholo → sab endpoints, input format, output format, "Try it out" button. API change karo → Swagger AUTO update. Frontend developers ke liye life saver.

### SwaggerConfig.java
```java
@Configuration
public class SwaggerConfig {
    @Bean
    public OpenAPI customOpenAPI() {
        return new OpenAPI()
            .info(new Info().title("Student Management API").version("1.0"));
    }
}
```

### Swagger — Deep
```
YE KYA HAI:  API documentation AUTOMATICALLY generate. Browser mein sab APIs dikhta.
URL:          http://localhost:8080/swagger-ui.html
DEPENDENCY:   springdoc-openapi-starter-webmvc-ui

KYA MILTA:
  - Sab APIs ek page pe (GET, POST, PUT, DELETE)
  - Har API ka input format, output format
  - "Try it out" button — browser se seedha test (Postman nahi chahiye)
  - Request/Response examples

SECURITY CONFIG: /swagger-ui/** aur /v3/api-docs/** permitAll — bina JWT access
```

### SwaggerConfig.java — Code from Project
```java
@Configuration
public class SwaggerConfig {
    @Bean
    public OpenAPI customOpenAPI() {
        return new OpenAPI()
                .info(new Info()
                        .title("Student Management API")
                        .version("1.0")
                        .description("Spring Boot REST API with JWT Security — by Arpan"));
    }
}
// Ye OPTIONAL hai — bina config ke bhi swagger kaam karega.
// Ye sirf page ka title aur description customize karta.
// DEPENDENCY: springdoc-openapi-starter-webmvc-ui
// Add karo → auto scan → /swagger-ui.html ready.
```

### Mermaid — Swagger Auto-Documentation Flow

```mermaid
flowchart TD
    A["@RestController<br/>classes scan"] --> B["springdoc library<br/>annotations parse"]
    B --> C["OpenAPI Spec<br/>JSON generate"]
    C --> D["Swagger UI<br/>/swagger-ui.html"]
    D --> E["Developer opens<br/>browser"]

    E --> F["See all APIs"]
    E --> G["Try it out button"]
    E --> H["Input/Output format"]

    style D fill:#c8e6c9
    style G fill:#fff9c4
```

### BINA vs WITH SWAGGER — Practical Difference
```
BINA SWAGGER:
  Frontend dev: "Kaunsi API hai?"
  Tu: "Email mein list bhejta hoon."
  Frontend dev: "Request body kya hai?"
  Tu: "Slack pe bhejta."
  Tu API update kiya → document update bhool gaya → OUTDATED.

WITH SWAGGER:
  http://localhost:8080/swagger-ui.html kholo.
  Sab APIs. Sab inputs. Sab outputs. "Try it out" button.
  API update kiya → Swagger AUTO update (code se generate hota).
```

### NAHI HUA TO (Without Swagger)?
```
1. Frontend developers ko har API manually batao → time waste
2. API change kiya → document update bhool gaya → frontend break
3. Testing ke liye Postman setup karo har developer pe → Swagger mein browser se seedha test
4. New team member aaya → "APIs kya hain?" → 2 din samjhao. Swagger se 2 minute.
```

---

# 17. CACHING — Redis

### REAL WORLD ANALOGY
Ghar ka FRIDGE soch. Chai banani hai → fridge kholo → doodh hai? Le lo → chai bana. NAHI hai? → market jao → doodh lao → fridge mein rakho → chai bana. Next time? Fridge se seedha nikaal lo. 100 cup chai: bina fridge = 100 market trips (SLOW). Fridge se = 1 market trip + 99 fridge trips (FAST).

REDIS = COMMUNITY FRIDGE — apartment building ka shared fridge. Tera ghar ka restart ho (app crash) → community fridge mein khana safe. 10 ghar (10 servers) ek hi fridge use kare → sab ko same cheez mile.

### YE KYA HAI?
Caching yehi hai — DB query ka FRIDGE. Baar baar same data DB se lane ki zaroorat nahi. `@Cacheable` = "cache kholo, hai? → do. Nahi? → DB se lao + cache mein rakh do." `@CachePut` = "update karo + cache ko bhi fresh rakho." `@CacheEvict` = "cache se entry hatao" (jab data stale ho). Redis = production ready external cache. Default in-memory = sirf dev.

### VISUALIZE — Cache = CHAI KI TAPRI KA BANDA
```
  BINA CACHE (seedha DB):
  ┌────────┐         ┌──────┐         ┌──────┐
  │ Client │────────→│ API  │────────→│  DB  │  Har baar DB tak jaana padta
  │        │←────────│      │←────────│      │  100 requests = 100 DB trips
  └────────┘  5ms    └──────┘  5ms    └──────┘  = SLOW (500ms total)

  WITH CACHE:
  ┌────────┐         ┌──────┐    ┌─────────┐    ┌──────┐
  │ Client │────────→│ API  │──→│ CACHE   │    │  DB  │
  │        │←────────│      │←──│(Redis)  │    │      │
  └────────┘  0.1ms  └──────┘   └─────────┘    └──────┘
                                  ↑ HIT!          ↑ miss toh DB
                                  99 baar          1 baar

  ANALOGY:
    DB    = Market (door hai, time lagta, har baar jaana padta)
    Cache = Fridge (ghar mein, instant, pehle yahan dekhna)

    Doodh chahiye?
      Fridge kholo (cache) → hai? → le lo (0.1ms). FAST.
      Nahi hai? → Market ja (DB) → le aao → fridge mein rakh (cache) → next time instant.

  100 baar doodh chahiye:
    Bina fridge: 100 baar market = SLOW
    Fridge se: 1 baar market + 99 baar fridge = FAST
```

### Redis vs Default In-Memory
```
DEFAULT (ConcurrentHashMap):
  - App restart = cache GONE
  - Multiple servers = har server ka ALAG cache (inconsistent)
  - Testing ke liye theek

REDIS (External Server):
  - App restart = cache SAFE (Redis mein hai)
  - Multiple servers = SAME cache share (consistent)
  - Production ke liye ZAROORI
```

### 3 Annotations Summary
```
@Cacheable  → GET:    Cache mein hai? → de do. Nahi? → DB se le, cache mein rakh.
@CachePut   → UPDATE: Hamesha run karo. Naya result cache mein daalo.
@CacheEvict → DELETE: Cache se entry hatao.

@EnableCaching → Main class pe. Bina iske TEEN-ON annotation DEAD.
```

### Mermaid — Redis Cache Architecture

```mermaid
graph TD
    subgraph "Without Cache"
        A1["1000 Requests"] --> B1["1000 DB Queries"]
        B1 --> C1["SLOW<br/>500ms avg"]
    end

    subgraph "With Redis Cache"
        A2["1000 Requests"] --> B2{"Redis<br/>Cache"}
        B2 -->|"999 Cache HIT"| C2["Instant Return<br/>0.1ms"]
        B2 -->|"1 Cache MISS"| D2["1 DB Query<br/>5ms"]
        D2 --> E2["Store in Redis"]
        E2 --> C2
    end

    style C1 fill:#ffcdd2
    style C2 fill:#c8e6c9
    style B2 fill:#fff9c4
```

### NAHI HUA TO (Without Caching)?
```
1. Har request DB jaayegi → 1000 users same student dekhe = 1000 DB calls
2. DB pe load bahut → slow responses → users frustrated
3. DB connection pool exhaust → app crash
4. Monthly AWS bill → cloudwatch dekhoge toh RDS (DB) ka bill 10x

WITH CACHING:
  1 DB call + 999 cache hits → DB happy, users happy, bill chhota.
```

---

# 18. DOCKER

### REAL WORLD ANALOGY
TIFFIN BOX soch. Maa ne roti + sabzi + chutney ek tiffin mein pack kar di. Tu office le ja → kholo → SAME taste. Dost ke ghar le ja → kholo → SAME taste. Train mein, plane mein, kahi bhi — same packaging, same contents, same experience. Maa ke kitchen ka khana har jagah same milta.

Ab bada scenario: RESTAURANT CHAIN sochh. Har branch ka tiffin (container), branch manager (orchestrator), 10 branches ek din mein same recipe — koi variation nahi.

### YE KYA HAI?
Docker yehi karta — tera app + dependencies (Java, MySQL, Redis, config) ek CONTAINER mein pack. Kahi bhi run karo — dev, staging, production — same behaviour guaranteed. "Mere machine pe chal raha" problem khatam. `Dockerfile` = tiffin ki recipe. Image = bana hua tiffin (read-only template). Container = tiffin ka actual running instance. `docker-compose` = pura meal set (app + DB + cache ek saath).

### VISUALIZE — Docker = TIFFIN BOX
```
  BINA DOCKER:
    "Mere machine pe chal raha" → tere pe nahi → kyun? →
    Java version alag, MySQL version alag, Redis nahi, config alag →
    2 din setup mein waste.

  WITH DOCKER:
    App + MySQL + Redis — sab ek TIFFIN BOX (container) mein pack.
    Kahi bhi kholo — same taste (same behaviour). Dev, Staging, Production — same.

  ┌────────────────────────────────────────────────────────────┐
  │                    TIFFIN BOX (Docker)                      │
  │                                                            │
  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐    │
  │  │ ROTI (App)   │  │ SABZI (MySQL)│  │ CHUTNEY      │    │
  │  │ Spring Boot  │  │ Database     │  │ (Redis/Cache)│    │
  │  │ Port 8080    │  │ Port 3306    │  │ Port 6379    │    │
  │  └──────┬───────┘  └──────────────┘  └──────────────┘    │
  │         │ depends_on (pehle sabzi ready, phir roti)       │
  │                                                            │
  │  docker-compose up   → sab start                           │
  │  docker-compose down → sab band                            │
  └────────────────────────────────────────────────────────────┘

  IMAGE vs CONTAINER:
    Image     = Recipe (Dockerfile) → read-only template
    Container = Bana hua khana → running instance
    1 recipe se 10 khana bana sakte = 1 image se 10 containers.
    Analogy: Image = Class. Container = Object.
```

### Dockerfile — EVERY LINE EXPLAINED
```dockerfile
# ============================================================
# Dockerfile — App ki IMAGE (recipe) banane ka file.
# IMAGE = recipe. CONTAINER = recipe se bana hua khana.
# 1 Dockerfile → 1 image → 100 containers bana sakte.
# ============================================================

FROM openjdk:17-jdk-slim
# BASE IMAGE — ye ek mini OS hai jisme Java 17 already installed.
# "slim" = chhota version (200MB vs 700MB) — fast download, kam disk.
# Docker Hub se download hota (like npm registry for containers).
# BINA ISKE: Container mein Java nahi → app chalegi nahi.
# ANALOGY: Naya ghar banane se pehle ZAMEEN chahiye. FROM = zameen.

WORKDIR /app
# Container ke andar KAAM KARNE KI JAGAH set karo.
# Jaise tu laptop pe "cd C:/projects" karta — ye container mein "cd /app".
# Iske baad sab commands /app folder mein chalenge.
# BINA ISKE: Sab files root (/) mein bikhrengi — messy.

COPY build/libs/*.jar app.jar
# Laptop se JAR file → Container ke andar /app/app.jar copy.
# build/libs/*.jar = Gradle ne build karke banai JAR (tera app).
# app.jar = container mein iss naam se save.
# BINA ISKE: Container mein tera app hoga hi nahi — empty box!

EXPOSE 8080
# "Ye container port 8080 pe sunna chahta hai."
# NOTE: Ye sirf DOCUMENTATION hai — actual port mapping "docker run -p" mein hota.
# Docker aur developers ko signal: "is port pe connect karo."
# BINA ISKE: App chalegi but kisi ko pata nahi kaunsa port.

ENTRYPOINT ["java", "-jar", "app.jar"]
# Container start hote hi YE COMMAND chalega.
# java -jar app.jar = Spring Boot app start.
# BINA ISKE: Container start hoga lekin kuch chalega nahi — KHAALI DABBA.
# ENTRYPOINT vs CMD: ENTRYPOINT fixed command. CMD override ho sakta.
```

### Dockerfile — Build & Run Commands
```bash
# Step 1: JAR banao (Gradle se)
./gradlew clean build

# Step 2: Docker IMAGE banao (Dockerfile se)
docker build -t student-app .
# -t student-app = image ka naam
# . = current directory mein Dockerfile hai

# Step 3: CONTAINER chalao (Image se)
docker run -p 8080:8080 student-app
# -p 8080:8080 = laptop_port:container_port
# Laptop pe localhost:8080 hit karo → container ke 8080 pe jaayega

# Other useful commands:
docker images           # sab images dikhao
docker ps               # running containers dikhao
docker ps -a            # sab containers (stopped bhi)
docker stop student-app # container roko
docker rm student-app   # container delete karo
docker rmi student-app  # image delete karo
```

### docker-compose.yml — EVERY LINE EXPLAINED
```yaml
# ============================================================
# Docker Compose — MULTIPLE containers EK COMMAND se chalao.
# Problem: App, MySQL, Redis — 3 alag containers manually start karna.
# Solution: docker-compose.yml mein define → "docker-compose up" → DONE.
# ============================================================

version: '3.8'
# Compose file ka version. Features define karta.
# 3.8 = latest stable.

services:
# Kitne containers chalane — har ek "service" = ek container.

  # ---- MySQL Database Container ----
  mysql:
  # Service ka naam. Docker network mein ISI naam se connect karte.
  # App mein: jdbc:mysql://mysql:3306/student_db (localhost NAHI — "mysql"!)
    image: mysql:8.0
    # Docker Hub se MySQL 8 ki READY-MADE image lo.
    # Tu MySQL install nahi kar raha — Docker ne kar diya.
    container_name: student-db
    # Container ka naam — docker ps mein dikhega.
    environment:
    # Container ke andar ENV VARIABLES set karo.
    # MySQL image ye variables padh ke configure hota.
      MYSQL_ROOT_PASSWORD: root
      # root user ka password. BINA ISKE MySQL start nahi hoga.
      MYSQL_DATABASE: student_db
      # Ye database AUTO CREATE hoga. Manually banane ki zaroorat nahi.
    ports:
      - "3306:3306"
      # laptop_port:container_port
      # Laptop se localhost:3306 pe MySQL access hoga.
      # BINA ISKE: MySQL container ke ANDAR chalega lekin bahar se connect nahi.

  # ---- Spring Boot App Container ----
  app:
    build: .
    # Current folder ka Dockerfile use karke IMAGE bana.
    # "." = current directory. Dockerfile yahi hai.
    # BINA ISKE: Pehle manually "docker build" karna padta.
    container_name: student-app
    ports:
      - "8080:8080"
      # Browser se localhost:8080 pe app access.
    depends_on:
      - mysql
      - redis
      # PEHLE MySQL + Redis start ho → PHIR app start.
      # BINA ISKE: App start hogi lekin MySQL nahi mila → connection error → CRASH.
      # NOTE: depends_on = start ORDER guarantee. READY guarantee NAHI.
      # MySQL start hua != MySQL ready for connections. Health check lagao for production.
    environment:
      SPRING_DATASOURCE_URL: jdbc:mysql://mysql:3306/student_db
      # "mysql" = upar wale MySQL container ka naam.
      # Docker network mein containers NAAM SE connect hote.
      # localhost NAHI kyunki dono ALAG containers hain (alag machines samjho).
      SPRING_DATASOURCE_USERNAME: root
      SPRING_DATASOURCE_PASSWORD: root
      SPRING_DATA_REDIS_HOST: redis
      # "redis" = Redis container ka naam.
      SPRING_DATA_REDIS_PORT: 6379

  # ---- Redis Cache Container ----
  redis:
    image: redis:7-alpine
    # Redis 7, alpine = bahut chhoti image (5MB vs 100MB). Fast.
    container_name: student-redis
    ports:
      - "6379:6379"
      # Redis default port.
```

### Docker Compose Commands
```bash
docker-compose up        # sab containers start (foreground)
docker-compose up -d     # sab containers start (background/detached)
docker-compose down      # sab containers stop + remove
docker-compose logs      # sab containers ke logs
docker-compose logs app  # sirf app container ke logs
docker-compose ps        # running services dikhao
docker-compose build     # images rebuild (code change ke baad)
```

### Docker Concepts — Deep
```
IMAGE vs CONTAINER:
  Image    = Recipe (read-only template). Dockerfile se banti.
  Container = Running instance of image. Image se banta.
  Analogy:  Image = Class. Container = Object.
  1 image se 10 containers bana sakte — sab independent.

DOCKERFILE:
  FROM       — base image (OS + runtime)
  WORKDIR    — working directory set
  COPY       — files copy laptop → container
  EXPOSE     — port document (not actual mapping)
  ENTRYPOINT — container start pe ye command chale
  RUN        — build time pe command chala (apt install, npm install)
  ENV        — environment variable set
  CMD        — default command (override ho sakta)

DOCKER-COMPOSE:
  Problem: 3 containers manually start → 3 docker run commands, network setup, order manage
  Solution: docker-compose.yml mein define → "docker-compose up" → sab start
  
  depends_on:  order define (mysql pehle, phir app)
  environment: env variables pass (DB URL, password)
  ports:       laptop port → container port mapping
  build:       Dockerfile se image bana
  image:       ready-made image use (Docker Hub se)

DOCKER NETWORK:
  docker-compose up → Docker EK network banata.
  Sab services uss network mein. Naam se connect hote.
  App mein "mysql" likhoge → Docker resolve karega MySQL container ka IP.
  localhost = container KHUD. "mysql" = MySQL container.
```

### Mermaid — Docker Ecosystem

```mermaid
flowchart TD
    A["Dockerfile"] -->|"docker build"| B["Docker IMAGE<br/>(Recipe/Template)"]
    B -->|"docker run"| C["CONTAINER 1<br/>(Running App)"]
    B -->|"docker run"| D["CONTAINER 2<br/>(Another Copy)"]
    B -->|"docker run"| E["CONTAINER 3<br/>(Another Copy)"]

    F["docker-compose.yml"] -->|"docker-compose up"| G["Orchestrate"]
    G --> H["MySQL Container"]
    G --> I["Redis Container"]
    G --> J["App Container"]
    J -->|"depends_on"| H
    J -->|"depends_on"| I

    subgraph "Docker Network (auto created)"
        H
        I
        J
    end

    style B fill:#bbdefb
    style C fill:#c8e6c9
    style D fill:#c8e6c9
    style E fill:#c8e6c9
    style F fill:#fff9c4
```

### Mermaid — Docker vs Docker Compose vs Kubernetes

```mermaid
graph TD
    subgraph "Docker<br/>(Ek Container)"
        A["1 Dockerfile<br/>= 1 App container"]
        A1["'docker run'<br/>Ek container chalao"]
    end

    subgraph "Docker Compose<br/>(Multiple Containers)"
        B["1 docker-compose.yml<br/>= App + MySQL + Redis"]
        B1["'docker-compose up'<br/>3 containers ek command"]
        B2["DEV/Testing ke liye<br/>Single machine"]
    end

    subgraph "Kubernetes<br/>(Container Fleet)"
        C["deployment.yaml + service.yaml<br/>= 10 App pods + auto-heal"]
        C1["'kubectl apply'<br/>Fleet manage karo"]
        C2["PRODUCTION ke liye<br/>Multiple machines/cloud"]
    end

    A --> B
    B --> C

    A -.- A2["Analogy:<br/>Ek TIFFIN banao"]
    B -.- B2_label["Analogy:<br/>Poora KHANA set<br/>(roti+sabzi+chutney)"]
    C -.- C3["Analogy:<br/>50 BRANCHES<br/>ka franchise manage"]

    style A fill:#bbdefb
    style B fill:#fff9c4
    style C fill:#c8e6c9
```

### Docker vs Compose vs K8s — Quick Summary
```
DOCKER (ek container):        Tiffin box mein biryani — ek app pack.
DOCKER COMPOSE (multiple):    Poora meal — app + DB + cache ek command mein.
KUBERNETES (production fleet): Restaurant chain manager — 50 branches, auto-heal, scale.
```

### NAHI HUA TO (Without Docker)?
```
BINA DOCKER:
  1. "Mere machine pe chal raha" → tere pe nahi → Java version alag
  2. Naye developer ko onboard: "Install Java 17, MySQL 8, Redis 7, ye config, wo ENV..."
     = 2 din setup mein waste.
  3. Dev pe chal raha, production pe nahi → config mismatch.

WITH DOCKER:
  1. "docker-compose up" — 1 command. Sab start. Sab same version.
  2. Naye developer: "Install Docker. Run docker-compose up." = 10 minute done.
  3. Dev, staging, production — same container = same behaviour.
```

---

# 19. CONFIG

### REAL WORLD ANALOGY
Restaurant ka SETTINGS REGISTER soch. Is register mein likha: godown (warehouse) kahan hai, godown ka lock key, table nahi hai toh banao ya nahi, fast counter (cache) kahan hai, restaurant ka gate number. Manager is register ke basis pe pura restaurant chalata — har worker ko isi se pata lagta kaise setup karna.

### YE KYA HAI?
`application.properties` yehi hai — Spring Boot ka SETTINGS REGISTER. Database URL, username, password, Redis host, port, Hibernate mode — sab isme define. Spring startup pe ye padh ke auto configure karta: DB connection kaise banana, cache setup, server port kya rakhna. `build.gradle` = dependencies ki list (kaunsi libraries chahiye). Dono ek saath = app ka setup blueprint.

### application.properties
```properties
# MySQL
spring.datasource.url=jdbc:mysql://localhost:3306/student_db
spring.datasource.username=root
spring.datasource.password=root

# Hibernate
spring.jpa.hibernate.ddl-auto=update    # App start pe table auto create/update
spring.jpa.show-sql=true                # Console pe SQL dikhao (debug ke liye)

# Redis
spring.data.redis.host=localhost
spring.data.redis.port=6379
```

### ddl-auto Options
```
none     — kuch mat kar (production)
update   — table hai toh update, nahi toh create (development)
create   — har baar drop + create (data gone!)
validate — sirf check karo, match nahi toh error
```

### build.gradle — Key Dependencies
```
spring-boot-starter-web          — REST API (Tomcat embedded, Jackson JSON)
spring-boot-starter-data-jpa     — Hibernate + JPA (DB ORM)
spring-boot-starter-security     — Spring Security (authentication/authorization)
spring-boot-starter-validation   — @Valid, @NotBlank, @Email
spring-boot-starter-data-redis   — Redis cache integration
springdoc-openapi                — Swagger UI auto-generate
mysql-connector-j                — MySQL JDBC driver
jjwt (api + impl + jackson)      — JWT token create/parse
lombok                           — Boilerplate code reduce
```

### Mermaid — Dependencies Ecosystem

```mermaid
graph TD
    A["build.gradle"] --> B["spring-boot-starter-web<br/>Tomcat + MVC + Jackson"]
    A --> C["spring-boot-starter-data-jpa<br/>Hibernate + HikariCP"]
    A --> D["spring-boot-starter-security<br/>Filters + BCrypt"]
    A --> E["spring-boot-starter-validation<br/>@NotBlank @Email @Valid"]
    A --> F["spring-boot-starter-data-redis<br/>Lettuce + Cache"]
    A --> G["springdoc-openapi<br/>Swagger UI"]
    A --> H["jjwt<br/>JWT tokens"]
    A --> I["mysql-connector-j<br/>JDBC driver"]
    A --> J["lombok<br/>@Data @Slf4j"]

    B --> K["REST APIs work"]
    C --> L["DB access works"]
    D --> M["Security works"]
    E --> N["Validation works"]
    F --> O["Caching works"]

    style A fill:#fff9c4
    style K fill:#c8e6c9
    style L fill:#c8e6c9
    style M fill:#c8e6c9
```

### NAHI HUA TO (Without Proper Config)?
```
Spring Boot ko pata nahi DB kahan hai → Connection refused.
ddl-auto nahi diya → table nahi banega → SQL error.
Redis host galat → cache fail → har request DB jaayegi → SLOW.
```

---

# 20. MySQL — Queries, Joins, Indexing (Code Examples)

### REAL WORLD ANALOGY
LIBRARY soch. Books ki shelves = TABLES. Har book = ROW. Book ke pages ka detail (title, author, year) = COLUMNS. Kisi topic ki book chahiye? Librarian INDEX kholta (direct jump to shelf). Bina index? Har shelf ek ek kar ke check (full scan — slow). Multiple topics combine karke search karna ho? JOIN karte multiple shelves. Transaction? Paisa de ke book issue karna + register mein entry — dono ya kuch nahi.

### YE KYA HAI?
MySQL yehi hai — tera LIBRARY. Structured data ko rows aur columns mein store karta. SQL = librarian ko batane ki language. CRUD = Create, Read, Update, Delete. JOIN = multiple tables milao (students + enrollments). INDEX = fast search ke liye (book ki index ki tarah). Transaction = atomic operations (paisa transfer — A se nikle + B mein daale → dono saath). Hibernate andar andar yeh sab SQL generate karta, tujhe khud nahi likhni.

### Basic CRUD — Ye Queries Hibernate Generate Karta
```sql
-- CREATE TABLE (ddl-auto=update pe Hibernate ye banata)
CREATE TABLE students (
    id BIGINT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    email VARCHAR(255) NOT NULL UNIQUE,
    age INT NOT NULL,
    CHECK (age >= 18)
);

-- INSERT (save() call pe)
INSERT INTO students (name, email, age) VALUES ('Arpan', 'arpan@gmail.com', 25);
INSERT INTO students (name, email, age) VALUES ('Rahul', 'rahul@gmail.com', 22);
INSERT INTO students (name, email, age) VALUES ('Priya', 'priya@yahoo.com', 20);
INSERT INTO students (name, email, age) VALUES ('Amit', 'amit@gmail.com', 30);

-- SELECT ALL (findAll() call pe)
SELECT * FROM students;
-- Result:
-- | id | name  | email             | age |
-- |----|-------|-------------------|-----|
-- | 1  | Arpan | arpan@gmail.com   | 25  |
-- | 2  | Rahul | rahul@gmail.com   | 22  |

-- SELECT BY ID (findById(1) call pe)
SELECT * FROM students WHERE id = 1;

-- UPDATE (save() with existing id)
UPDATE students SET name = 'Arpan M', age = 26 WHERE id = 1;

-- DELETE (deleteById(1) call pe)
DELETE FROM students WHERE id = 1;
```

### WHERE Clause — Filtering
```sql
-- Derived Query: findByName("Arpan")
SELECT * FROM students WHERE name = 'Arpan';

-- Derived Query: findByAgeGreaterThan(21)
SELECT * FROM students WHERE age > 21;

-- Multiple conditions
SELECT * FROM students WHERE age > 20 AND name LIKE 'A%';
-- LIKE patterns:
--   'A%'    → A se start ho (Arpan, Amit)
--   '%com'  → com pe end ho
--   '%an%'  → andar "an" ho (Arpan)

-- IN operator
SELECT * FROM students WHERE age IN (20, 25, 30);

-- BETWEEN
SELECT * FROM students WHERE age BETWEEN 20 AND 25;

-- ORDER BY
SELECT * FROM students ORDER BY age DESC;        -- bade se chhote
SELECT * FROM students ORDER BY name ASC;         -- A to Z

-- LIMIT + OFFSET (pagination)
SELECT * FROM students ORDER BY id LIMIT 5 OFFSET 0;   -- page 0, size 5
SELECT * FROM students ORDER BY id LIMIT 5 OFFSET 5;   -- page 1, size 5
-- Pageable ye internally karta hai
```

### Aggregate Functions
```sql
SELECT COUNT(*) FROM students;                    -- kitne students
SELECT AVG(age) FROM students;                    -- average age
SELECT MAX(age) FROM students;                    -- sabse bada age
SELECT MIN(age) FROM students;                    -- sabse chhota age
SELECT SUM(age) FROM students;                    -- total age

-- GROUP BY — group karke count
SELECT age, COUNT(*) as count FROM students GROUP BY age;
-- | age | count |
-- |-----|-------|
-- | 20  | 3     |
-- | 25  | 5     |

-- HAVING — group ke baad filter
SELECT age, COUNT(*) as count FROM students GROUP BY age HAVING count > 2;
-- Sirf woh ages jinke 2 se zyada students hain
```

### JOINS — Multiple Tables
```sql
-- Imagine: students + courses + enrollments tables

CREATE TABLE courses (
    id BIGINT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    credits INT
);

CREATE TABLE enrollments (
    id BIGINT AUTO_INCREMENT PRIMARY KEY,
    student_id BIGINT,
    course_id BIGINT,
    grade VARCHAR(2),
    FOREIGN KEY (student_id) REFERENCES students(id),
    FOREIGN KEY (course_id) REFERENCES courses(id)
);

-- INNER JOIN — dono tables mein match ho
SELECT s.name, c.name as course, e.grade
FROM students s
INNER JOIN enrollments e ON s.id = e.student_id
INNER JOIN courses c ON c.id = e.course_id;
-- Sirf enrolled students dikhenge. No enrollment = no show.

-- LEFT JOIN — left table ke sab rows + right se match
SELECT s.name, c.name as course
FROM students s
LEFT JOIN enrollments e ON s.id = e.student_id
LEFT JOIN courses c ON c.id = e.course_id;
-- SAB students dikhenge. Enrolled nahi toh course = NULL.

-- RIGHT JOIN — right table ke sab rows
-- LEFT JOIN ka opposite. Rarely use.

-- SELF JOIN — table khud se join
SELECT a.name as student1, b.name as student2
FROM students a, students b
WHERE a.age = b.age AND a.id != b.id;
-- Same age wale student pairs dhundho

-- INTERVIEW MEIN:
-- "INNER JOIN = dono mein match. LEFT JOIN = left sab + right match. 
--  Use case: students with/without enrollments."
```

### Subqueries
```sql
-- Average se zyada age wale students
SELECT * FROM students WHERE age > (SELECT AVG(age) FROM students);

-- Students jo kisi course mein enrolled hain
SELECT * FROM students WHERE id IN (SELECT student_id FROM enrollments);

-- Students jo kisi course mein enrolled NAHI hain
SELECT * FROM students WHERE id NOT IN (SELECT student_id FROM enrollments);
```

### INDEX — Performance
```sql
-- Index banao — search FAST
CREATE INDEX idx_email ON students(email);
CREATE INDEX idx_name ON students(name);
CREATE INDEX idx_age ON students(age);

-- Composite index — multiple columns
CREATE INDEX idx_name_age ON students(name, age);

-- INDEX KYA KARTA:
-- Bina index:  SELECT * FROM students WHERE email = 'x' → FULL TABLE SCAN (sab rows check)
-- Index se:    SELECT * FROM students WHERE email = 'x' → direct jump (like book index)
--
-- KAB LAGAO:
-- - WHERE mein baar baar use hone wale columns
-- - JOIN conditions mein use hone wale columns
-- - ORDER BY columns
-- - UNIQUE constraint wale columns (auto index banta)
--
-- KAB MAT LAGAO:
-- - Chhoti tables (100 rows — scan fast hai, index overhead)
-- - Jis column pe bahut INSERT/UPDATE hote (index bhi update = slow)
--
-- @Column(unique = true) → Hibernate automatically UNIQUE INDEX banata

-- EXPLAIN — query performance check
EXPLAIN SELECT * FROM students WHERE email = 'arpan@gmail.com';
-- type: ref (index use hua) vs ALL (full scan — BAD)
```

### Transactions
```sql
-- TRANSACTION = multiple operations ek unit mein. Sab ya kuch nahi.
START TRANSACTION;
    UPDATE accounts SET balance = balance - 1000 WHERE id = 1;  -- A se nikalo
    UPDATE accounts SET balance = balance + 1000 WHERE id = 2;  -- B mein daalo
COMMIT;    -- sab sahi → save
-- ya
ROLLBACK;  -- kuch galat → sab undo

-- SPRING MEIN:
-- @Transactional service method pe lagao
-- Method successful → auto COMMIT
-- Exception throw → auto ROLLBACK
-- Ye = ACID properties (Atomicity, Consistency, Isolation, Durability)
```

### Mermaid — JOIN Types Visual

```mermaid
graph TD
    subgraph "INNER JOIN"
        A1["Students"] --- B1["Matched rows ONLY"]
        B1 --- C1["Enrollments"]
    end

    subgraph "LEFT JOIN"
        A2["ALL Students"] --- B2["Matched + NULL"]
        B2 --- C2["Enrollments"]
    end

    subgraph "RIGHT JOIN"
        A3["Students"] --- B3["NULL + Matched"]
        B3 --- C3["ALL Enrollments"]
    end

    style B1 fill:#c8e6c9
    style B2 fill:#bbdefb
    style B3 fill:#fff9c4
```

### NAHI HUA TO (Without Proper SQL)?
```
BINA INDEX:
  1 lakh students mein email search → FULL SCAN → 500ms.
  Index lagao → 1ms. 500x improvement.

BINA PAGINATION:
  findAll() → 10 lakh records ek saath → OutOfMemoryError → app CRASH.
  Pageable se: 20 records per page → safe, fast.

BINA TRANSACTIONS:
  A se paise nikale (1000 minus) → B mein daalne se pehle error → A ka paisa GAYA!
  Transaction se: error aaya → ROLLBACK → A ka paisa wapas. SAFE.
```

### Common Interview SQL Queries
```sql
-- 1. Second highest salary
SELECT MAX(age) FROM students WHERE age < (SELECT MAX(age) FROM students);
-- ya
SELECT DISTINCT age FROM students ORDER BY age DESC LIMIT 1 OFFSET 1;

-- 2. Duplicate emails dhundho
SELECT email, COUNT(*) FROM students GROUP BY email HAVING COUNT(*) > 1;

-- 3. Nth record
SELECT * FROM students ORDER BY id LIMIT 1 OFFSET (N-1);

-- 4. Delete duplicates (keep lowest id)
DELETE s1 FROM students s1
INNER JOIN students s2
WHERE s1.email = s2.email AND s1.id > s2.id;
```

---

# 21. FILTER vs INTERCEPTOR — Deep Comparison

### REAL WORLD ANALOGY
Mall soch. Building ka MAIN GATE CCTV (Filter) — har aane jaane wala record karta — chahe shopper ho, cleaning staff ho, delivery ho, security ho. Broad coverage, Mall ke andar kuch bhi jaane se pehle gate.

Mall ke andar har FLOOR ka alag RECEPTIONIST (Interceptor) — sirf us floor ke visitors record karti. Shopper 3rd floor gaya → 3rd floor receptionist ne note kiya. Cleaning staff basement gaya → 3rd floor receptionist ko pata nahi. Focused, floor-specific.

### YE KYA HAI?
Dono request logging/manipulation ke tools — par alag LEVELS pe. Filter = Servlet API ka part (Tomcat level, sab requests), Interceptor = Spring MVC ka part (controller URLs only). Filter PEHLE chalta, Interceptor BAAD mein. Dono ka use case alag — Filter broad (CORS, encoding, basic logging), Interceptor specific (API logging, auth checks, rate limiting).

```
                        FILTER                    INTERCEPTOR
Belongs to:             Servlet API (Tomcat)      Spring MVC
Scope:                  SAB requests              Sirf controller URLs
                        (images, CSS, API sab)    (.addPathPatterns() se control)
Registration:           @Component auto           WebConfig mein manual register
When runs:              Sabse PEHLE               Filter ke BAAD, controller se pehle
Access to:              Request/Response only     Request/Response + Handler (controller method)
Methods:                doFilter()                preHandle(), postHandle(), afterCompletion()
Can block?:             chain.doFilter() na call  preHandle() return false
Use case:               Logging, CORS, encoding   Auth check, rate limiting, API logging

ORDER:  Client → Filter → DispatcherServlet → Interceptor → Controller
```

### Mermaid — Filter vs Interceptor Side by Side

```mermaid
flowchart TD
    A["HTTP Request"] --> B["FILTER<br/>(Tomcat/Servlet Level)"]
    B --> C["DispatcherServlet"]
    C --> D["INTERCEPTOR<br/>(Spring MVC Level)"]
    D --> E["Controller"]

    B -.- B1["Scope: SAB requests<br/>Images, CSS, JS, API<br/>Registration: @Component auto<br/>Method: doFilter()"]
    D -.- D1["Scope: SIRF controller URLs<br/>addPathPatterns('/api/**')<br/>Registration: WebConfig manual<br/>Methods: preHandle, postHandle,<br/>afterCompletion"]

    style B fill:#ffcdd2
    style D fill:#c8e6c9
```

### KYUN DONO CHAHIYE?
```
CCTV (Filter) = broad security. Sab traffic monitor.
Receptionist (Interceptor) = focused logging. Sirf important APIs.
```

---

# 22. REQUEST KA POORA SAFAR (Life of a Request)

### REAL WORLD ANALOGY
AMAZON ORDER ka journey soch. Tu order place kiya — ye trip start. Delivery hub pe scan + log (Filter). Package ka tracking ID valid? Check (JwtFilter). Kaunse floor ka order? Route (Interceptor). Order sheet padho, kitchen ko bolo (Controller). Kitchen mein pehle ready shelf check (Cache) — hai? De do. Nahi? Warehouse se maango (Repository) → actual shelf (DB) se utaro → pack karo → ship → tujhe mila. Har step log hota, har step alag department ka kaam.

### YE KYA HAI?
Ek HTTP request ka complete lifecycle — client se DB tak aur wapas. 8 stages: Filter (gate log) → JwtFilter (security) → Interceptor (API log) → Controller (receptionist) → Service (chef) → Cache check (fridge) → Repository (storage) → DB (godown). Response wapsi bhi same order mein reverse. Har layer apna kaam, log, transform karta. Poora app isi journey pe chalta.

### VISUALIZE — Ek Request Ka Journey (Hotel Analogy)
```
  Client bolta: "GET /api/students/5" + JWT token

  ┌─────────┐
  │ CLIENT  │  "Mujhe student #5 chahiye"
  └────┬────┘
       │
       ↓
  ┌──────────────────────────────────────────────────────────────────┐
  │ 1. MAIN GATE — RequestLoggingFilter                              │
  │    📹 "Visitor aaya: GET /api/students/5, IP: 192.168.1.1"      │
  │    Log kiya. Aage bheja.                                         │
  └──────────────────────────┬───────────────────────────────────────┘
                             ↓
  ┌──────────────────────────────────────────────────────────────────┐
  │ 2. SECURITY CHECK — JwtFilter                                    │
  │    🔒 "Wristband (JWT) dikhaao!"                                │
  │    Token nikala → parse → "Arpan" → valid? expiry baaki?         │
  │    ✓ VALID → SecurityContext: "Arpan logged in"                  │
  │    ✗ INVALID → "NIKLO!" → 403 Forbidden (YAHI RUKA)             │
  └──────────────────────────┬───────────────────────────────────────┘
                             ↓
  ┌──────────────────────────────────────────────────────────────────┐
  │ 3. FLOOR RECEPTION — LoggingInterceptor.preHandle()              │
  │    📋 "[PRE] GET /api/students/5" — log                         │
  │    return true → "jaao andar"                                    │
  └──────────────────────────┬───────────────────────────────────────┘
                             ↓
  ┌──────────────────────────────────────────────────────────────────┐
  │ 4. FRONT DESK — StudentController.getById(5)                     │
  │    🛎️ "Student #5 chahiye? Service ko bolta hoon"               │
  │    studentService.getStudentById(5) call                         │
  └──────────────────────────┬───────────────────────────────────────┘
                             ↓
  ┌──────────────────────────────────────────────────────────────────┐
  │ 5. KITCHEN — StudentService (with Cache Proxy)                   │
  │    👨‍🍳 @Cacheable check: "student::5" cache mein hai?"           │
  │                                                                  │
  │    ┌─────────────┐  HAI!                                         │
  │    │ FAST COUNTER │──────→ seedha return (DB SKIP) ⚡ 0.1ms     │
  │    │ (Redis Cache)│                                               │
  │    └──────┬──────┘                                                │
  │           │ NAHI!                                                 │
  │           ↓                                                      │
  │    ┌─────────────┐                                               │
  │    │ STORAGE     │  studentRepository.findById(5)                │
  │    │ (Repository)│                                                │
  │    └──────┬──────┘                                                │
  │           ↓                                                      │
  │    ┌─────────────┐                                               │
  │    │ GODOWN (DB) │  Hibernate → SELECT * FROM students WHERE id=5│
  │    │ MySQL       │  → Student object → cache mein rakh           │
  │    └─────────────┘  → DTO banao → return                         │
  └──────────────────────────┬───────────────────────────────────────┘
                             ↓
  ┌──────────────────────────────────────────────────────────────────┐
  │ 6. FRONT DESK — Controller                                       │
  │    ResponseEntity.ok(studentDTO) → Jackson → JSON convert        │
  └──────────────────────────┬───────────────────────────────────────┘
                             ↓
  ┌──────────────────────────────────────────────────────────────────┐
  │ 7. FLOOR RECEPTION — LoggingInterceptor.afterCompletion()        │
  │    📋 "[POST] GET /api/students/5 → 200 (15ms)"                 │
  └──────────────────────────┬───────────────────────────────────────┘
                             ↓
  ┌──────────────────────────────────────────────────────────────────┐
  │ 8. MAIN GATE — RequestLoggingFilter                              │
  │    📹 "Status: 200, Time: 18ms" — response log                  │
  └──────────────────────────┬───────────────────────────────────────┘
                             ↓
  ┌─────────┐
  │ CLIENT  │  { "id": 5, "name": "Arpan", "email": "a@b.com", "age": 25 }
  └─────────┘
```

### Mermaid — Request Lifecycle (Complete Journey)

```mermaid
sequenceDiagram
    participant C as Client
    participant F as RequestLoggingFilter
    participant JF as JwtFilter
    participant I as Interceptor
    participant CT as Controller
    participant S as Service
    participant Cache as Redis Cache
    participant R as Repository
    participant DB as MySQL

    C->>F: GET /api/students/5 + JWT
    Note over F: LOG: Method, URL, IP

    F->>JF: chain.doFilter()
    Note over JF: Extract token<br/>Verify signature<br/>Check expiry
    JF->>JF: SecurityContext.set("Arpan")

    JF->>I: chain.doFilter()
    Note over I: preHandle() LOG

    I->>CT: getById(5)
    CT->>S: getStudentById(5)

    S->>Cache: "student::5" hai?
    alt Cache HIT
        Cache-->>S: StudentDTO (instant!)
    else Cache MISS
        S->>R: findById(5)
        R->>DB: SELECT * FROM students WHERE id=5
        DB-->>R: Row data
        R-->>S: Student entity
        S->>Cache: Store "student::5"
    end

    S-->>CT: StudentDTO
    CT-->>I: ResponseEntity(200, JSON)
    Note over I: afterCompletion() LOG
    I-->>F: Response
    Note over F: LOG: Status, Duration
    F-->>C: {"id":5, "name":"Arpan", "email":"a@b.com", "age":25}
```

### WAPSI KA RAASTA (Response Flow)
```
Godown → Warehouse → Kitchen → Front Desk → Floor Reception → Main Gate → Customer
= DB → Repository → Service → Controller → Interceptor → Filter → Client
```

---

# 23. INTERVIEW QUESTIONS — Detailed

### Spring Boot Internals
```
Q: DispatcherServlet kya hai?
A: Spring MVC ka front controller. SAB requests pehle iske paas.
   HandlerMapping se controller dhundhta → execute → JSON response bhejta.
   Bina iske har URL ke liye alag Servlet likhna padta.

Q: Spring Boot embedded Tomcat kya hai?
A: Tomcat server app ke ANDAR hai. Alag install nahi karna.
   java -jar app.jar → Tomcat auto start. spring-boot-starter-web mein aata.

Q: ApplicationContext kya hai?
A: Spring Container. Sab beans (objects) ka ghar. Beans yahan store hote.
   @Autowired karte toh ApplicationContext se bean nikalta → inject karta.

Q: IoC (Inversion of Control) kya hai?
A: Tu objects nahi banata, Spring banata. Tu sirf @Autowired bolo, Spring dega.
   new StudentService() — GALAT. @Autowired StudentService — SAHI.
   Object creation ka control tera nahi, Spring ka = Inversion of Control.

Q: Bean lifecycle?
A: Scan → Create → Inject dependencies → @PostConstruct → READY → @PreDestroy → Destroy.
```

### JDBC, Hibernate, JPA
```
Q: JDBC vs Hibernate?
A: JDBC = manual SQL + manual mapping. 30 lines ek SELECT ke liye.
   Hibernate = ORM. Java object de, SQL auto generate. 3 lines mein done.

Q: JPA kya hai? Hibernate se kya fark?
A: JPA = specification (interface). Hibernate = implementation.
   List = interface, ArrayList = implementation. Same relation.
   Code mein JPA use karo → Hibernate ya koi bhi swap kar sakte.

Q: Spring Data JPA kya add karta?
A: JPA ke upar ek aur layer. Interface likho (JpaRepository extend) → implementation FREE.
   save, findAll, findById, delete — ek line SQL nahi.

Q: Lazy vs Eager loading?
A: Lazy = related data TAB le jab access karo. Performance better.
   Eager = related data ABHI le saath mein. N+1 problem ka risk.

Q: N+1 problem kya hai?
A: 10 students fetch → 1 query. Har student ke courses → 10 queries. Total = 11.
   Fix: JOIN FETCH ya @EntityGraph.

Q: First Level Cache vs Second Level Cache?
A: 1st = Session level. Automatic. Same session mein same entity = cache hit.
   2nd = Application level. Configure karna padta (EhCache/Redis). Cross-session.
```

### Maven vs Gradle
```
Q: Maven aur Gradle mein fark?
A: Maven = XML (pom.xml), slow, rigid. Gradle = Groovy (build.gradle), fast, flexible.
   Gradle incremental build + daemon se fast. Modern projects mein Gradle prefer.

Q: dependency scope kya hota?
A: compile = sab jagah. runtime = sirf chalate waqt (MySQL driver).
   test = sirf testing. provided = compile mein hai, JAR mein nahi (Lombok).
```

### MySQL
```
Q: INNER JOIN vs LEFT JOIN?
A: INNER = dono tables mein match ho tabhi show. LEFT = left sab + right se match.
   Student enrolled nahi → INNER mein nahi dikhega, LEFT mein NULL ke saath dikhega.

Q: INDEX kya karta?
A: Search fast. Bina index = full scan (sab rows check). Index = direct jump.
   WHERE, JOIN, ORDER BY columns pe lagao. Book ki index jaisa.

Q: Transaction kya hai? ACID?
A: Multiple ops ek unit. Sab ya kuch nahi. A se paise nikalo + B mein daalo = atomic.
   A=Atomicity, C=Consistency, I=Isolation, D=Durability.
```

### Spring Boot Core
```
Q: @SpringBootApplication kya karta?
A: 3-in-1: @Configuration (bean source) + @EnableAutoConfiguration (auto config)
   + @ComponentScan (beans dhundho). App ka entry point.

Q: Controller mein logic kyun nahi likhte?
A: Separation of concerns. Controller = HTTP handle. Service = logic.
   Fayda: Service independently testable, reusable, transaction manage.

Q: @Autowired kaise kaam karta?
A: Spring container mein matching bean dhundhta → inject karta.
   Constructor injection better — immutable, testable.
```

### Entity & Repository
```
Q: @Entity vs @Table?
A: @Entity = ye class DB table hai (required). @Table = custom table naam (optional).

Q: JpaRepository extend karne se kya milta?
A: Free CRUD: save(), findAll(), findById(), delete(), count(), pagination.
   Method naam se auto query: findByName(), findByAgeGreaterThan().

Q: JPQL vs Native Query?
A: JPQL = class/field naam (DB independent). Native = table/column naam (DB specific).
   JPQL prefer karo — DB switch pe kaam.
```

### Service Layer
```
Q: @Service kya karta?
A: Spring bean mark + intent batata (ye business logic layer hai).
   Functionally @Component jaisa. But semantically meaningful.

Q: @Transactional kab lagao?
A: Multiple DB operations ek saath — ek fail toh sab rollback.
   Example: Transfer — A se paise nikalo + B mein daalo. Ek fail = dono rollback.
   Service layer pe lagao (controller pe nahi).
```

### Security
```
Q: JWT flow explain karo.
A: Register → Login (credentials verify) → JWT token return →
   Client har request mein "Bearer token" bheje → JwtFilter verify →
   Valid → SecurityContext set → Controller access.
   Invalid → 403 Forbidden.

Q: Stateless kyun?
A: Server pe session nahi rakhte. Token mein sab info.
   10 servers hain — kisi pe bhi request jaaye, token verify = access.
   Scalable. Load balancer friendly.

Q: CSRF kyun off?
A: REST API stateless — cookies use nahi. JWT header mein jaata.
   CSRF attack tab hota jab browser cookie auto-send karta.
```

### Caching
```
Q: @Cacheable vs @CachePut?
A: Cacheable = cache mein hai toh DB SKIP. Performance ke liye.
   CachePut = hamesha method chala, result cache mein update. Consistency ke liye.

Q: Redis kyun, default cache kyun nahi?
A: Default = app restart = gone. Single server only.
   Redis = persistent. Multiple servers share. Production ready.
```

### Filter vs Interceptor
```
Q: Filter aur Interceptor mein fark?
A: Filter = Servlet level, sab requests (images bhi). Tomcat ka part.
   Interceptor = Spring level, sirf controller URLs. Spring MVC ka part.
   Filter PEHLE chalta, Interceptor BAAD mein.
```

### Docker
```
Q: Dockerfile vs docker-compose?
A: Dockerfile = EK image ki recipe (app ka container kaise bane).
   Docker-compose = MULTIPLE containers orchestrate (app + DB + Redis ek command).

Q: Container mein "mysql" naam se kyun connect?
A: Docker network mein containers naam se resolve hote.
   localhost = container khud. "mysql" = MySQL container.
```

---

## Quick Revision — Ek Line Mein
```
@Entity           — Class = DB table
@Repository       — Interface = DB access (free CRUD)
@Service          — Class = business logic
@RestController   — Class = REST API (JSON return)
@Autowired        — Dependency inject (Spring object de)
@Valid            — Validation trigger (bina iske annotations dead)
@Cacheable        — Cache se de, DB skip
@CachePut         — DB + cache dono update
@CacheEvict       — Cache se hatao
@RestControllerAdvice — Global error handler
@ExceptionHandler — Specific exception pakdo
@Configuration    — Bean definitions
@EnableCaching    — Caching ON karo
@EnableWebSecurity — Security ON karo
@Bean             — Method return = Spring managed object
ResponseEntity    — HTTP response + status code control
```

---

## MERMAID DIAGRAMS

> Visual diagrams of the entire Student Management System — architecture, flows, comparisons. Interview mein whiteboard pe draw karo!

---

### 1. Project Architecture — Full Stack Flow

```mermaid
graph TD
    A["🌐 Client (Browser/Postman)"] --> B["🚪 RequestLoggingFilter"]
    B --> C["🔒 JwtFilter (Security)"]
    C --> D["📋 LoggingInterceptor"]
    D --> E["🛎️ Controller"]
    E --> F["👨‍🍳 Service"]
    F --> G["🏪 Repository"]
    G --> H["📦 MySQL Database"]
    F --> I["⚡ Redis Cache"]

    style A fill:#e1f5fe
    style C fill:#ffcdd2
    style F fill:#fff9c4
    style H fill:#c8e6c9
    style I fill:#ffccbc
```

---

### 2. Layered Architecture — Left to Right Flow

```mermaid
graph LR
    A["🛎️ Controller Layer"] --> B["👨‍🍳 Service Layer"]
    B --> C["🏪 Repository Layer"]
    C --> D["📦 MySQL"]
    B --> E["⚡ Redis Cache"]

    A -.- A1["@RestController<br/>HTTP handle karo<br/>Logic NAHI"]
    B -.- B1["@Service<br/>Business Logic<br/>Caching, Validation"]
    C -.- C1["@Repository<br/>DB Access<br/>JpaRepository extends"]

    style A fill:#bbdefb
    style B fill:#fff9c4
    style C fill:#c8e6c9
    style D fill:#a5d6a7
    style E fill:#ffccbc
```

---

### 3. JWT Authentication Flow

```mermaid
sequenceDiagram
    participant U as 🧑 User
    participant AC as 🔐 AuthController
    participant BC as 🔑 BCrypt
    participant JU as 🎫 JwtUtil
    participant JF as 🛡️ JwtFilter
    participant SC as 🔓 SecurityContext
    participant C as 🛎️ Controller

    Note over U,C: === LOGIN FLOW ===
    U->>AC: POST /auth/login {email, password}
    AC->>BC: matches(rawPassword, encodedPassword)
    BC-->>AC: ✅ Password Match
    AC->>JU: generateToken(username)
    JU-->>AC: JWT Token (eyJhbGci...)
    AC-->>U: 200 OK {token: "eyJhbGci..."}

    Note over U,C: === PROTECTED REQUEST FLOW ===
    U->>JF: GET /api/students (Header: Bearer token)
    JF->>JU: extractUsername(token)
    JU-->>JF: username
    JF->>JU: validateToken(token, userDetails)
    JU-->>JF: ✅ Valid
    JF->>SC: setAuthentication(authToken)
    SC-->>C: Request forwarded with auth
    C-->>U: 200 OK {students data}
```

---

### 4. Request Lifecycle — Life of a Request

```mermaid
flowchart TD
    A["🌐 Client Request"] --> B["🚪 RequestLoggingFilter<br/>(Servlet Level — ALL requests)"]
    B --> C["🔒 JwtFilter<br/>(Extract & Validate Token)"]
    C -->|"❌ Invalid Token"| D["🚫 401 Unauthorized"]
    C -->|"✅ Valid Token"| E["📋 Interceptor preHandle()<br/>(Spring Level — /api/**)"]
    E --> F["🛎️ Controller<br/>(@GetMapping, @PostMapping)"]
    F --> G["👨‍🍳 Service<br/>(@Service — Business Logic)"]
    G --> H{"⚡ Cache Check<br/>(@Cacheable)"}
    H -->|"✅ Cache HIT"| I["📤 Return Cached Data"]
    H -->|"❌ Cache MISS"| J["🏪 Repository<br/>(JpaRepository)"]
    J --> K["📦 MySQL Database"]
    K --> L["💾 Store in Cache"]
    L --> I
    I --> M["📋 Interceptor postHandle()"]
    M --> N["🚪 Filter (Response Logging)"]
    N --> O["🌐 Client Response"]

    style D fill:#ffcdd2
    style H fill:#fff9c4
    style I fill:#c8e6c9
```

---

### 5. Entity to DTO Flow — Data Transformation

```mermaid
graph TD
    subgraph "📤 Response Flow (DB → Client)"
        DB1["📦 Database"] --> E1["🗂️ Entity<br/>(Student.java)<br/>id, name, email, password"]
        E1 --> F1["🔄 fromEntity()<br/>Static Factory Method"]
        F1 --> D1["📋 DTO<br/>(StudentDTO.java)<br/>id, name, email<br/>❌ NO password"]
        D1 --> J1["📝 JSON Response"]
        J1 --> C1["🌐 Client"]
    end

    subgraph "📥 Request Flow (Client → DB)"
        C2["🌐 Client"] --> J2["📝 JSON Request"]
        J2 --> D2["📋 DTO<br/>(StudentDTO.java)<br/>@Valid checks"]
        D2 --> F2["🔄 toEntity()<br/>Convert Method"]
        F2 --> E2["🗂️ Entity<br/>(Student.java)"]
        E2 --> DB2["📦 Database"]
    end

    style D1 fill:#c8e6c9
    style D2 fill:#bbdefb
    style F1 fill:#fff9c4
    style F2 fill:#fff9c4
```

---

### 6. Exception Handling Flow

```mermaid
graph TD
    A["🛎️ Controller throws Exception"] --> B["📋 @RestControllerAdvice<br/>GlobalExceptionHandler"]

    B --> C{"🔍 Exception Type?"}

    C -->|"RuntimeException<br/>(Student not found)"| D["handleRuntimeException()"]
    D --> E["📤 404 NOT FOUND<br/>{error: 'Student not found with id: 5'}"]

    C -->|"MethodArgumentNotValidException<br/>(@Valid failed)"| F["handleValidationExceptions()"]
    F --> G["📤 400 BAD REQUEST<br/>{name: 'Name is required',<br/>email: 'Invalid email format'}"]

    C -->|"Other Exception"| H["handleGenericException()"]
    H --> I["📤 500 INTERNAL SERVER ERROR<br/>{error: 'Something went wrong'}"]

    style E fill:#ffcdd2
    style G fill:#fff9c4
    style I fill:#ef9a9a
```

---

### 7. Cache Flow — @Cacheable (GET)

```mermaid
flowchart TD
    A["📥 GET /api/students/1"] --> B["@Cacheable check"]
    B --> C{"⚡ Redis mein hai?"}
    C -->|"✅ HIT"| D["📤 Return from Redis<br/>DB SKIP — 0.1ms"]
    C -->|"❌ MISS"| E["🏪 Repository.findById()"]
    E --> F["📦 MySQL Query"]
    F --> G["💾 Redis mein store"]
    G --> H["📤 Return to Client"]
    
    style D fill:#c8e6c9
```

### Cache Flow — @CachePut (UPDATE)

```mermaid
flowchart TD
    A["📥 PUT /api/students/1"] --> B["@CachePut"]
    B --> C["🏪 Repository.save()"]
    C --> D["📦 MySQL Update"]
    D --> E["⚡ Redis Cache Update<br/>Naya value overwrite"]
    E --> F["📤 Return Updated"]
    
    style E fill:#fff9c4
```

### Cache Flow — @CacheEvict (DELETE)

```mermaid
flowchart TD
    A["📥 DELETE /api/students/1"] --> B["@CacheEvict"]
    B --> C["🏪 Repository.deleteById()"]
    C --> D["📦 MySQL Delete"]
    D --> E["⚡ Redis se hatao<br/>Key delete"]
    E --> F["📤 Return 204"]
    
    style E fill:#ffcdd2
```

---

### 8. Docker Compose — Container Orchestration

```mermaid
graph TD
    A["🐳 docker-compose up"] --> B["📦 MySQL Container<br/>Port: 3306<br/>Volume: mysql-data"]
    A --> C["📦 Redis Container<br/>Port: 6379"]
    A --> D["📦 App Container<br/>(Spring Boot)<br/>Port: 8080"]

    D -->|"depends_on"| B
    D -->|"depends_on"| C
    D -->|"JDBC connect<br/>mysql:3306"| B
    D -->|"Redis connect<br/>redis:6379"| C

    E["📄 Dockerfile"] -->|"builds"| D
    F["📄 docker-compose.yml"] -->|"orchestrates"| A

    subgraph "🌐 Docker Network"
        B
        C
        D
    end

    style A fill:#bbdefb
    style B fill:#c8e6c9
    style C fill:#ffccbc
    style D fill:#fff9c4
```

---

### 9. Filter vs Interceptor — Deep Comparison

```mermaid
graph TD
    A["🌐 HTTP Request"] --> B["🚪 Filter<br/>(Servlet Level)<br/>ALL requests<br/>Images, CSS, JS bhi"]
    B --> C["⚙️ DispatcherServlet<br/>(Spring MVC Entry)"]
    C --> D["📋 Interceptor<br/>(Spring Level)<br/>Only /api/**<br/>Controller URLs only"]
    D --> E["🛎️ Controller"]

    B -.- B1["☕ javax.servlet.Filter<br/>Tomcat manages<br/>web.xml / @Component<br/>🔧 doFilter()"]
    D -.- D1["🍃 HandlerInterceptor<br/>Spring manages<br/>WebConfig register<br/>🔧 preHandle() / postHandle()"]

    style B fill:#ffcdd2
    style C fill:#e1bee7
    style D fill:#c8e6c9
    style B1 fill:#ffebee
    style D1 fill:#e8f5e9
```

---

### 10. JDBC vs Hibernate vs JPA — Evolution

```mermaid
graph TD
    A["📝 JDBC<br/>(Pure Java)<br/>30+ lines code<br/>Manual SQL<br/>Manual mapping<br/>Connection manage"] 
    -->|"Too much boilerplate 😫"| B["🔄 Hibernate<br/>(ORM Framework)<br/>5 lines code<br/>Auto SQL generate<br/>Object mapping<br/>session.save()"]
    -->|"Vendor lock-in 🔒"| C["📋 JPA<br/>(Standard/Specification)<br/>Interface only<br/>No implementation<br/>@Entity, @Table<br/>Hibernate implements"]
    -->|"Still boilerplate 😤"| D["🚀 Spring Data JPA<br/>(Magic Layer)<br/>1 line FREE!<br/>extends JpaRepository<br/>findAll(), save()<br/>Custom queries too"]

    style A fill:#ffcdd2
    style B fill:#fff9c4
    style C fill:#bbdefb
    style D fill:#c8e6c9
```

---

---

# 24. KUBERNETES — Deployment & Service (k8s)

### REAL WORLD ANALOGY
RESTAURANT CHAIN KA FRANCHISE MANAGER soch. Tu 50 restaurants ka owner hai — har restaurant ek POD. Manager kya karta?
- "Hamesha 3 restaurants open rahne chahiye" (replicas: 3) — ek band ho gaya? NAYA khol do automatically.
- "Diwali traffic badha?" → 3 se 10 restaurants scale karo — turant.
- "Naya menu deploy karna?" → ek ek restaurant update, zero downtime.
- Customer ko "1-800 number" do (Service/LoadBalancer) — koi bhi restaurant call uthaye, customer ko farak nahi padta.

### YE KYA HAI?
Kubernetes yehi hai — CONTAINER FLEET MANAGER for production. Docker = ek container. Docker Compose = ek laptop pe multiple containers. Kubernetes = CLOUD pe 100s of containers across multiple machines. Deployment = "itne replicas chahiye" declare karo, K8s maintain karega. Service = stable address for pods (load balancer). Pod = smallest unit (usually 1 container). Auto-heal, auto-scale, rolling update — production mein game changer.

### VISUALIZE — Kubernetes = RESTAURANT CHAIN KA FRANCHISE MANAGER
```
  DOCKER = Ek restaurant banaya (container).
  DOCKER COMPOSE = Ek restaurant mein kitchen + storage + counter (multiple containers).
  KUBERNETES = 50 restaurants ka FRANCHISE MANAGER:

  ┌──────────────────────────────────────────────────────────────────────┐
  │                  FRANCHISE MANAGER (Kubernetes)                       │
  │                                                                      │
  │  "3 restaurants (pods) chahiye"                                      │
  │  ┌──────────┐  ┌──────────┐  ┌──────────┐                          │
  │  │ Restaurant│  │ Restaurant│  │ Restaurant│                          │
  │  │ 1 (Pod)  │  │ 2 (Pod)  │  │ 3 (Pod)  │                          │
  │  │ App +    │  │ App +    │  │ App +    │                          │
  │  │ Port 8080│  │ Port 8080│  │ Port 8080│                          │
  │  └──────────┘  └──────────┘  └──────────┘                          │
  │       ↑              ↑              ↑                                │
  │       └──────────────┼──────────────┘                                │
  │                      │                                               │
  │              ┌───────────────┐                                       │
  │              │  COUNTER NO.  │  ← Service (LoadBalancer)             │
  │              │  (ek number)  │     Customer ko EK address de.        │
  │              │  Port 80      │     Kaunsa waiter (pod) handle kare   │
  │              └───────────────┘     — customer ko farak nahi.         │
  │                                                                      │
  │  FEATURES:                                                           │
  │  - Restaurant band? (pod crash) → NAYA KHOLO automatically          │
  │  - Festival? (traffic badha) → 3 → 10 restaurants scale up          │
  │  - Renovation? (update) → ek ek restaurant update (zero downtime)   │
  │  - Health check → "ye restaurant theek chal raha?"                  │
  └──────────────────────────────────────────────────────────────────────┘
```

### Kubernetes Concepts — Deep
```
CLUSTER = Poora infrastructure. Multiple machines (nodes) ka group.
  ANALOGY: Poora city jahan restaurants hain.

NODE = Ek machine (physical ya virtual). Pods yahan chalte.
  ANALOGY: Ek building jahan restaurant open ho sakta.
  2 types: Master node (manager) + Worker node (actual kaam).

POD = Smallest unit. Ek ya ek se zyada containers.
  Usually: 1 Pod = 1 Container = 1 App instance.
  ANALOGY: Ek restaurant = ek pod.
  Pod crash? Kubernetes NAYA pod banata.

DEPLOYMENT = "Mujhe 3 pods chahiye" — declare karo. K8s manage karega.
  replicas: 3 → 3 pods hamesha running. 1 crash → naya start.
  ANALOGY: "Hamesha 3 restaurants open rahne chahiye."

SERVICE = Pods ke upar ek STABLE address.
  3 pods hain — kaunse pe request jaaye? Service decide karta.
  Load balancer jaisa — traffic distribute.
  ANALOGY: Ek phone number — call karo, koi bhi restaurant pickup kare.

NAMESPACE = Logical grouping. Dev, staging, production alag namespaces.
  ANALOGY: City mein alag zones — commercial, residential.
```

### deployment.yaml — EVERY LINE EXPLAINED
```yaml
# ============================================================
# Deployment — Kubernetes ko batao: "Ye app chalao, itni copies rakhna"
# ============================================================

apiVersion: apps/v1
# Kubernetes API version. "apps/v1" = stable Deployment API.
# Kubernetes ke multiple API versions hain — ye production ready.

kind: Deployment
# RESOURCE TYPE: Deployment = pods manage karo, replicas rakhna, auto-heal.
# Other kinds: Pod (single), Service (networking), ConfigMap (config), etc.

metadata:
  name: student-app
  # Deployment ka naam. kubectl get deployments mein dikhega.

spec:
  replicas: 3
  # 3 PODS chahiye. Kubernetes hamesha 3 running rakhega.
  # 1 crash? → automatically naya start.
  # Traffic badha? → replicas: 10 karo → 10 pods.
  # ANALOGY: "Hamesha 3 waiters on duty."

  selector:
    matchLabels:
      app: student-app
    # KAUNSE PODS manage karne — label match karo.
    # Deployment dekhta: "mujhe woh pods chahiye jinpe label 'app: student-app' hai"
    # Ye template.metadata.labels se MATCH hona chahiye — nahi toh error.

  template:
    # POD KA TEMPLATE — har naye pod ka blueprint.
    metadata:
      labels:
        app: student-app
        # Pod pe label lagao. Service aur Deployment isse dhundhte.
        # ANALOGY: Restaurant pe board lagao "Student App Branch" — manager dhundh sake.
    spec:
      containers:
      - name: student-app
        # Container ka naam (pod ke andar).
        image: student-app:latest
        # Docker IMAGE — Dockerfile se bani. Docker Hub ya local registry se aati.
        # "latest" = newest version. Production mein specific version use karo (v1.2.3).
        ports:
        - containerPort: 8080
          # Container ANDAR app kaunse port pe sun raha.
          # Spring Boot default = 8080.
        env:
        - name: SPRING_DATASOURCE_URL
          value: jdbc:mysql://mysql-service:3306/student_db
          # "mysql-service" = Kubernetes Service naam — Docker Compose mein "mysql" tha.
          # K8s mein services naam se resolve hote (DNS).
        - name: SPRING_DATASOURCE_USERNAME
          value: root
        - name: SPRING_DATASOURCE_PASSWORD
          value: root
          # PRODUCTION MEIN: Secrets use karo (kubectl create secret).
          # Plain text mein password = SECURITY RISK.
        - name: SPRING_DATA_REDIS_HOST
          value: redis-service
        - name: SPRING_DATA_REDIS_PORT
          value: "6379"
          # Quotes mein kyunki YAML mein number ko string banana padta env variable ke liye.
```

### service.yaml — EVERY LINE EXPLAINED
```yaml
# ============================================================
# Service — Pods ke upar ek STABLE address. Traffic route karo.
# ============================================================

apiVersion: v1
# Core API version. Service basic Kubernetes resource hai.

kind: Service
# RESOURCE TYPE: Service = networking. Pods ko expose karo.

metadata:
  name: student-app-service
  # Service ka naam. App se isse use karo: http://student-app-service

spec:
  selector:
    app: student-app
    # KAUNSE PODS se connect — label match karo.
    # Deployment ne pods banaye with label "app: student-app"
    # Service unhi pods ko dhundhta → traffic route karta.
    # ANALOGY: "board pe 'Student App' likha hai? → tera order isko do."

  ports:
  - port: 80
    # BAHAR se ye port use. User 80 pe hit kare.
    targetPort: 8080
    # Pod ke ANDAR ka actual port. Spring Boot 8080 pe sun raha.
    # Translation: bahar 80 → andar 8080.
    # ANALOGY: Restaurant ka main door (80) → kitchen window (8080).

  type: LoadBalancer
  # SERVICE TYPE:
  #   ClusterIP  = sirf cluster ke ANDAR access (default). Bahar se nahi.
  #   NodePort   = node ka IP + port se access. Development ke liye.
  #   LoadBalancer = cloud provider ka load balancer create. PRODUCTION ke liye.
  #                  AWS → ELB create. GCP → Load Balancer create.
  #                  Traffic 3 pods mein distribute kare.
  #
  # ANALOGY:
  #   ClusterIP  = Internal phone (sirf office ke andar call)
  #   NodePort   = Direct line (specific extension number)
  #   LoadBalancer = 1-800 number (call karo, koi bhi agent uthaaye)
```

### Kubernetes Commands
```bash
# Deployment create/update
kubectl apply -f k8s/deployment.yaml
kubectl apply -f k8s/service.yaml

# Status check
kubectl get pods                    # sab pods dikhao
kubectl get deployments             # deployments dikhao
kubectl get services                # services dikhao

# Logs dekho
kubectl logs <pod-name>             # specific pod ke logs
kubectl logs -f <pod-name>          # live streaming logs

# Scale up/down
kubectl scale deployment student-app --replicas=5   # 3 → 5 pods

# Describe (details + events)
kubectl describe pod <pod-name>     # pod ki details + errors

# Delete
kubectl delete -f k8s/deployment.yaml
kubectl delete -f k8s/service.yaml
```

### Mermaid — Kubernetes Architecture

```mermaid
graph TD
    subgraph "Kubernetes Cluster"
        subgraph "Master Node (Control Plane)"
            M1["API Server<br/>(kubectl talks here)"]
            M2["Scheduler<br/>(pod kahan chalao?)"]
            M3["Controller Manager<br/>(replicas maintain)"]
            M4["etcd<br/>(cluster state store)"]
        end

        subgraph "Worker Node 1"
            W1A["Pod 1<br/>student-app"]
            W1B["Pod 2<br/>student-app"]
        end

        subgraph "Worker Node 2"
            W2A["Pod 3<br/>student-app"]
        end
    end

    U["User / kubectl"] --> M1
    M1 --> M2
    M2 --> W1A
    M2 --> W1B
    M2 --> W2A
    M3 -->|"replicas: 3<br/>maintain"| W1A
    M3 --> W1B
    M3 --> W2A

    LB["LoadBalancer<br/>Service<br/>(port 80)"] --> W1A
    LB --> W1B
    LB --> W2A

    style LB fill:#c8e6c9
    style M1 fill:#bbdefb
    style W1A fill:#fff9c4
    style W1B fill:#fff9c4
    style W2A fill:#fff9c4
```

### Mermaid — Deployment to Service Flow

```mermaid
flowchart TD
    A["User hits<br/>http://app:80"] --> B["Service<br/>(LoadBalancer)<br/>port 80"]
    B --> C["Pod 1<br/>port 8080"]
    B --> D["Pod 2<br/>port 8080"]
    B --> E["Pod 3<br/>port 8080"]

    C --> F["MySQL Service<br/>port 3306"]
    D --> F
    E --> F
    C --> G["Redis Service<br/>port 6379"]
    D --> G
    E --> G

    style B fill:#bbdefb
    style C fill:#fff9c4
    style D fill:#fff9c4
    style E fill:#fff9c4
    style F fill:#c8e6c9
    style G fill:#ffccbc
```

### Mermaid — Pod Self-Healing

```mermaid
flowchart TD
    A["Deployment:<br/>replicas: 3"] --> B["Pod 1: Running"]
    A --> C["Pod 2: Running"]
    A --> D["Pod 3: Running"]

    D -->|"CRASH!"| E["Pod 3: Dead"]
    E --> F["Controller Manager<br/>detects: only 2 pods!<br/>Need 3!"]
    F --> G["Pod 4: NEW<br/>(replacement)"]

    style D fill:#ffcdd2
    style E fill:#ef9a9a
    style G fill:#c8e6c9
    style F fill:#fff9c4
```

### Docker Compose vs Kubernetes — When to Use What
```
                    DOCKER COMPOSE              KUBERNETES
Environment:        Dev / Testing               Production / Cloud
Scale:              1 machine                   Multiple machines (cluster)
Auto-heal:          NAHI (crash = manual fix)   HAI (crash = auto restart)
Scaling:            Manual (replicas nahi)       Auto scale (replicas: 3→10)
Load Balancing:     Nahi (ya nginx lagao)       Built-in (Service type)
Rolling Update:     Nahi                        HAI (zero downtime deploy)
Complexity:         Simple (1 file)             Complex (multiple YAMLs)
Learning Curve:     Easy                        STEEP (but worth it)

RULE OF THUMB:
  Local dev   → docker-compose up
  Production  → kubectl apply (Kubernetes)
  
  "Docker Compose = tera laptop. Kubernetes = AWS/GCP/Azure."
```

### Kubernetes Terms — Restaurant Chain Mapping
```
CLUSTER    = Poora city (infrastructure)
NODE       = Ek building (machine)
POD        = Ek restaurant (app instance)
DEPLOYMENT = "3 restaurants HAMESHA open rakhna" (desired state)
SERVICE    = Ek phone number for ALL restaurants (load balancer)
NAMESPACE  = City ke zones (dev zone, prod zone)
CONFIGMAP  = Restaurant menu (configuration)
SECRET     = Restaurant safe ki locker combination (passwords)

REAL SCENARIO:
  Tu bolta: "3 pods chahiye" (replicas: 3)
  Kubernetes: "Done. 3 chal rahe."
  
  Ek crash hua?
  Kubernetes: "Detect kiya. Naya start kar raha... Done. Phir 3."
  
  Diwali traffic?
  Tu: "10 pods chahiye" (kubectl scale --replicas=10)
  Kubernetes: "7 naye start... Done. 10 chal rahe."
  
  Naya version deploy?
  Kubernetes: "Ek ek pod update karta... purana band, naya start... zero downtime."
```

### NAHI HUA TO (Without Kubernetes)?
```
BINA KUBERNETES:
  1. App crash? → manually restart. Raat ko 3 baje? → subah tak down.
  2. Traffic spike? → manually new server provision → 2 ghante. Users lost.
  3. Deploy naya version? → old band → naya start → 5 min downtime. Users angry.
  4. 10 servers manage? → manually. Konsa server pe kya chal raha? Confusion.

WITH KUBERNETES:
  1. Crash? → 5 second mein naya pod. Users ko pata bhi nahi.
  2. Traffic? → auto scale. 30 seconds mein 10 pods ready.
  3. Deploy? → rolling update. Zero downtime. Users notice nahi karte.
  4. 100 pods? → kubectl get pods. Sab ek dashboard pe. Clean.
```

---

# 25. HOW TO RUN — Complete Guide

### REAL WORLD ANALOGY
GAADI CHALANE KA SEQUENCE soch. Chalane ke liye kya chahiye? Engine (Java), petrol (MySQL data store), AC (Redis cache — bina iske chalega but uncomfortable), mechanic ka toolkit (Gradle build tool). Phir step by step: engine start → petrol check → AC on → gaadi assemble → ignition key ghumaao (gradlew bootRun) → steering wheel lo (browser mein test) → road pe niklo.

### YE KYA HAI?
Ye section tere liye complete RUNBOOK hai — project start karne ka step-by-step guide. Prerequisites (kya install karna), setup steps (MySQL/Redis/build), run command (gradlew), test karne ka tarika (Swagger/curl), common errors aur unka fix. Naya developer aaye toh direct ye section de do — 15 min mein chal raha hoga.

## Prerequisites — Pehle Ye Install Hona Chahiye

```
┌────────────────────────────────────────────────────────────────────────┐
│  TOOL              VERSION         CHECK COMMAND          PORT         │
│  ──────────────────────────────────────────────────────────────────    │
│  Java (JDK)        17+             java -version          -            │
│  MySQL             5.7 / 8.0       mysql --version        3306         │
│  Redis             latest          redis-cli ping         6379         │
│  Gradle            (wrapper)       ./gradlew --version    -            │
│  Git               latest          git --version          -            │
└────────────────────────────────────────────────────────────────────────┘

ANALOGY:
  Java    = Engine (bina engine gaadi nahi chalegi)
  MySQL   = Petrol tank (data store — bina data app kya karega?)
  Redis   = AC (cache — bina AC gaadi chalegi but slow + uncomfortable)
  Gradle  = Mechanic (build tool — sab parts jodta)
```

## Step by Step Run Guide

```
STEP 1: MySQL Start (Engine Start Karo)
────────────────────────────────────────
  Windows:
    Services → MySQL57 → Right Click → Start
    ya CMD:  net start MySQL57

  Linux/Mac:
    sudo service mysql start
    ya: sudo systemctl start mysql

  Verify:
    mysql -u root -proot -e "SELECT 1;"
    → "1" aaya = chal raha.

STEP 2: Database Create (Petrol Dalo)
─────────────────────────────────────
  mysql -u root -proot -e "CREATE DATABASE IF NOT EXISTS student_db;"

  Verify:
    mysql -u root -proot -e "SHOW DATABASES;" | grep student_db
    → "student_db" dikhna chahiye.

STEP 3: Redis Start (AC On Karo)
────────────────────────────────
  Windows:
    redis-server
    (naya terminal mein chalne do, band mat karo)

  Linux/Mac:
    sudo service redis-server start
    ya: redis-server

  Verify:
    redis-cli ping
    → "PONG" aaya = chal raha.

STEP 4: Build (Mechanic Gaadi Assemble Kare)
─────────────────────────────────────────────
  cd student-management/
  ./gradlew build -x test

  -x test = tests skip (pehle run ke liye — baad mein test chalana)
  BUILD SUCCESSFUL dikhna chahiye.

STEP 5: Run (Gaadi Chalao!)
────────────────────────────
  ./gradlew bootRun

  Console mein dikhega:
    Tomcat started on port(s): 8080
    Started StudentManagementApplication in X.XX seconds

  App chal raha! 🎉

STEP 6: Test in Browser (Test Drive)
─────────────────────────────────────
  Browser mein jaao:
    http://localhost:8080/swagger-ui/index.html

  Swagger UI dikhega = SAB KAAM KAR RAHA.
```

## Common Errors and Fixes

```
┌─────────────────────────────────────────────────────────────────────────────┐
│  ERROR                              FIX                                    │
│  ────────────────────────────────────────────────────────────────────────── │
│                                                                            │
│  "Port 8080 already in use"         2 gaadi 1 parking spot mein nahi aati! │
│    → Koi aur process 8080 pe hai.                                          │
│    Windows: netstat -ano | findstr :8080                                   │
│             taskkill /PID <pid> /F                                          │
│    Linux:   lsof -i :8080                                                  │
│             kill -9 <pid>                                                   │
│    Ya application.properties mein:                                         │
│      server.port=8081                                                      │
│                                                                            │
│  ─────────────────────────────────────────────────────────────────────────  │
│                                                                            │
│  "Can't connect to MySQL"           Engine start nahi kiya!                │
│    → MySQL service band hai.                                               │
│    Windows: Services → MySQL57 → Start                                     │
│    Linux:   sudo service mysql start                                       │
│    Check:   mysql -u root -proot -e "SELECT 1;"                            │
│    ALSO:    application.properties mein username/password check karo.       │
│                                                                            │
│  ─────────────────────────────────────────────────────────────────────────  │
│                                                                            │
│  "Unable to connect to Redis"       AC on nahi kiya!                       │
│    → Redis server band hai.                                                │
│    Start:  redis-server (naye terminal mein)                               │
│    Check:  redis-cli ping → "PONG" aana chahiye                            │
│    Port:   Default 6379. application.properties mein check karo.           │
│                                                                            │
│  ─────────────────────────────────────────────────────────────────────────  │
│                                                                            │
│  "Cannot serialize" /                Letter bina envelope ke bheja!        │
│  "SerializationException"                                                  │
│    → 2 cheezein check karo:                                                │
│                                                                            │
│    FIX 1: RedisConfig.java mein GenericJackson2JsonRedisSerializer use karo │
│      Default JdkSerializer binary format use karta — error prone.          │
│      RedisConfig mein JSON serializer set karo (already done in project).  │
│                                                                            │
│    FIX 2: DTO class mein "implements Serializable" lagao                   │
│      StudentDTO implements Serializable ← ye ZAROORI.                      │
│      Bina iske = courier bina packing ke bheja = reject.                   │
│                                                                            │
│    DONO chahiye:                                                           │
│      RedisConfig = envelope ka TYPE define (JSON envelope)                 │
│      Serializable = object ko permission de (haan, mujhe envelope mein    │
│                     daal sakte ho)                                          │
│                                                                            │
│  ─────────────────────────────────────────────────────────────────────────  │
│                                                                            │
│  "Whitelabel Error Page"            Route galat ya controller nahi mila.   │
│    → URL check karo. Swagger: /swagger-ui/index.html                       │
│    → API: /api/students (not /students)                                    │
│                                                                            │
│  ─────────────────────────────────────────────────────────────────────────  │
│                                                                            │
│  "Access Denied" / 403 Forbidden    Bina ticket ride pe baithne ki koshish │
│    → JWT token missing ya expired.                                         │
│    → Pehle /api/auth/login karo → token lo → Authorization header mein do. │
│                                                                            │
└─────────────────────────────────────────────────────────────────────────────┘
```

## API Test Commands (curl examples)

```bash
# ──────────────────────────────────────────────────────────────
# 1. REGISTER — Naya user banao
# ──────────────────────────────────────────────────────────────
curl -X POST http://localhost:8080/api/auth/register \
  -H "Content-Type: application/json" \
  -d '{
    "username": "arpan",
    "password": "password123"
  }'

# Response: { "message": "User registered successfully" }

# ──────────────────────────────────────────────────────────────
# 2. LOGIN — Token lo (ye sab protected APIs mein lagega)
# ──────────────────────────────────────────────────────────────
curl -X POST http://localhost:8080/api/auth/login \
  -H "Content-Type: application/json" \
  -d '{
    "username": "arpan",
    "password": "password123"
  }'

# Response: { "token": "eyJhbGciOiJIUzI1NiJ9..." }
# IS TOKEN KO COPY KARO — neeche use hoga!

# ──────────────────────────────────────────────────────────────
# 3. CREATE STUDENT — Naya student banao (Bearer token chahiye!)
# ──────────────────────────────────────────────────────────────
curl -X POST http://localhost:8080/api/students \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer eyJhbGciOiJIUzI1NiJ9...PASTE_TOKEN_HERE" \
  -d '{
    "name": "Arpan Maheshwari",
    "email": "arpan@example.com",
    "age": 27
  }'

# Response: { "id": 1, "name": "Arpan Maheshwari", "email": "arpan@example.com", "age": 27 }

# ──────────────────────────────────────────────────────────────
# 4. GET ALL STUDENTS — Sab students lao
# ──────────────────────────────────────────────────────────────
curl -X GET http://localhost:8080/api/students \
  -H "Authorization: Bearer eyJhbGciOiJIUzI1NiJ9...PASTE_TOKEN_HERE"

# Response: [{ "id": 1, "name": "Arpan", ... }, ...]

# ──────────────────────────────────────────────────────────────
# 5. GET STUDENT BY ID — Ek specific student lao
# ──────────────────────────────────────────────────────────────
curl -X GET http://localhost:8080/api/students/1 \
  -H "Authorization: Bearer eyJhbGciOiJIUzI1NiJ9...PASTE_TOKEN_HERE"

# Response: { "id": 1, "name": "Arpan Maheshwari", "email": "arpan@example.com", "age": 27 }

# ──────────────────────────────────────────────────────────────
# 6. UPDATE STUDENT — Student update karo
# ──────────────────────────────────────────────────────────────
curl -X PUT http://localhost:8080/api/students/1 \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer eyJhbGciOiJIUzI1NiJ9...PASTE_TOKEN_HERE" \
  -d '{
    "name": "Arpan Updated",
    "email": "arpan.updated@example.com",
    "age": 28
  }'

# ──────────────────────────────────────────────────────────────
# 7. DELETE STUDENT — Student delete karo
# ──────────────────────────────────────────────────────────────
curl -X DELETE http://localhost:8080/api/students/1 \
  -H "Authorization: Bearer eyJhbGciOiJIUzI1NiJ9...PASTE_TOKEN_HERE"

# ──────────────────────────────────────────────────────────────
# 8. REDIS CHECK — Cache mein kya hai?
# ──────────────────────────────────────────────────────────────
redis-cli KEYS "*"
# Output:
#   1) "students::1"
#   2) "students::all"

redis-cli GET "students::1"
# Output: {"@class":"com.example.practice.dto.StudentDTO","id":1,"name":"Arpan",...}

redis-cli TTL "students::1"
# Output: (integer) 3540   ← seconds remaining (59 min)

# CACHE CLEAR (jab test karna ho):
redis-cli FLUSHALL
```

## What Connects to What

```
┌─────────────────────────────────────────────────────────────────────────┐
│                    STUDENT MANAGEMENT SYSTEM                           │
│                                                                        │
│    ┌─────────────┐     ┌────────────────┐     ┌──────────────────┐     │
│    │  BROWSER     │     │   SPRING BOOT  │     │     MySQL        │     │
│    │  (Client)    │────→│   APP (8080)   │────→│   (3306)         │     │
│    │              │     │                │     │   student_db     │     │
│    │  Swagger UI  │     │  Tomcat Server │     │   └─ students    │     │
│    │  curl/Postman│     │  REST APIs     │     │   └─ users       │     │
│    └─────────────┘     │  JWT Security  │     └──────────────────┘     │
│                        │                │                              │
│                        │                │     ┌──────────────────┐     │
│                        │                │────→│     Redis         │     │
│                        │                │     │   (6379)          │     │
│                        └────────────────┘     │   Cache keys:     │     │
│                                               │   └─ students::1  │     │
│                                               │   └─ students::all│     │
│                                               └──────────────────┘     │
│                                                                        │
│    CONNECTIONS:                                                         │
│      App → MySQL (3306) → student_db → students table, users table     │
│      App → Redis (6379) → cache keys (students::1, students::all)      │
│      App → Tomcat (8080) → Swagger UI, REST endpoints                  │
└─────────────────────────────────────────────────────────────────────────┘
```

### Mermaid — What Connects to What

```mermaid
graph TD
    subgraph "CLIENT SIDE"
        A["Browser<br/>Swagger UI"] 
        B["curl / Postman"]
    end

    subgraph "APPLICATION (Port 8080)"
        C["Embedded Tomcat"]
        D["Spring Boot App"]
        E["REST Controllers"]
        F["JWT Security Filter"]
        G["Service Layer"]
        H["Redis Cache Layer"]
    end

    subgraph "DATA STORES"
        I["MySQL (Port 3306)<br/>student_db<br/>├─ students table<br/>└─ users table"]
        J["Redis (Port 6379)<br/>Cache<br/>├─ students::1<br/>├─ students::all<br/>└─ TTL: 60 min"]
    end

    A -->|"HTTP Request"| C
    B -->|"HTTP Request"| C
    C --> F
    F -->|"JWT Valid"| E
    E --> G
    G --> H
    H -->|"Cache MISS"| I
    H -->|"Cache HIT"| G
    G -->|"Store in cache"| J
    G -->|"DB Query"| I

    style C fill:#bbdefb
    style F fill:#fff9c4
    style I fill:#c8e6c9
    style J fill:#ffccbc
```

### Mermaid — Startup Flow (gradlew → App Running)

```mermaid
flowchart TD
    A["./gradlew bootRun"] --> B["Gradle Compile<br/>Java files → .class files"]
    B --> C["Spring Boot Start<br/>SpringApplication.run()"]
    C --> D["Component Scan<br/>@Controller, @Service,<br/>@Repository, @Configuration<br/>sab dhundho"]
    
    D --> E["MySQL Connect<br/>localhost:3306<br/>student_db"]
    D --> F["Redis Connect<br/>localhost:6379"]
    D --> G["Tomcat Start<br/>Embedded Server<br/>Port 8080"]
    
    E -->|"SUCCESS"| H["Hibernate:<br/>Tables create/validate"]
    E -->|"FAIL"| I["ERROR:<br/>'Cannot connect to MySQL'<br/>FIX: MySQL service start karo"]
    
    F -->|"SUCCESS"| J["Redis Cache<br/>Manager Ready"]
    F -->|"FAIL"| K["ERROR:<br/>'Unable to connect to Redis'<br/>FIX: redis-server start karo"]
    
    G -->|"SUCCESS"| L["Swagger UI Ready<br/>localhost:8080/swagger-ui/index.html"]
    G -->|"FAIL"| M["ERROR:<br/>'Port 8080 in use'<br/>FIX: kill process on 8080"]

    H --> N["APP READY!<br/>'Started in X.XX seconds'"]
    J --> N
    L --> N

    style N fill:#c8e6c9
    style I fill:#ffcdd2
    style K fill:#ffcdd2
    style M fill:#ffcdd2

    %% ANALOGY:
    %% gradlew = ignition key
    %% Compile = engine warmup
    %% Spring Boot = gaadi start
    %% MySQL = petrol tank connect
    %% Redis = AC on
    %% Tomcat = steering ready (port 8080 = road pe niklo)
```

### Mermaid — Redis Cache Flow (Request → Cache Check → Hit/Miss)

```mermaid
flowchart TD
    A["Client: GET /api/students/5"] --> B["JwtFilter:<br/>Token valid?"]
    B -->|"Invalid"| C["403 Forbidden"]
    B -->|"Valid"| D["Controller:<br/>getById(5)"]
    
    D --> E["Service:<br/>@Cacheable 'students'"]
    E --> F{"Redis mein<br/>'students::5'<br/>key hai?"}
    
    F -->|"HAI = CACHE HIT<br/>(Fast! DB call nahi)"| G["Redis se JSON lao<br/>Deserialize → StudentDTO"]
    G --> H["Return to Client<br/>(DB TOUCH NAHI KIYA!)"]
    
    F -->|"NAHI = CACHE MISS<br/>(Pehli baar)"| I["DB Query:<br/>SELECT * FROM students<br/>WHERE id = 5"]
    I --> J["StudentDTO banao"]
    J --> K["Redis mein store:<br/>KEY = 'students::5'<br/>VALUE = JSON<br/>TTL = 60 min"]
    K --> L["Return to Client"]

    M["60 min baad..."] --> N["Redis: TTL expire<br/>Key auto-delete"]
    N --> O["Next request =<br/>CACHE MISS again<br/>Cycle repeat"]

    style G fill:#c8e6c9
    style H fill:#c8e6c9
    style I fill:#fff9c4
    style C fill:#ffcdd2
    style N fill:#ffcdd2

    %% ANALOGY:
    %% Cache HIT = Fridge mein khana mila — microwave karo, khaao. Fast!
    %% Cache MISS = Fridge khaali — market jao (DB), khana banao, fridge mein rakho.
    %% TTL expire = Khana expire — fenko, fresh banao.
```

### Mermaid — Error Troubleshooting Flowchart

```mermaid
flowchart TD
    A["App start kiya<br/>./gradlew bootRun"] --> B{"Kya error aaya?"}
    
    B -->|"Port 8080 already in use"| C["2 gaadi 1 parking spot!"]
    C --> C1["netstat -ano | findstr :8080"]
    C1 --> C2["taskkill /PID xxx /F"]
    C2 --> C3["Phir se ./gradlew bootRun"]
    
    B -->|"Cannot connect to MySQL"| D["Engine start nahi kiya!"]
    D --> D1["Services → MySQL57 → Start"]
    D1 --> D2{"mysql -u root -proot<br/>-e 'SELECT 1;'"}
    D2 -->|"Success"| D3["Phir se bootRun"]
    D2 -->|"Fail"| D4["Password check karo<br/>application.properties"]
    
    B -->|"Unable to connect to Redis"| E["AC on nahi kiya!"]
    E --> E1["Naye terminal mein:<br/>redis-server"]
    E1 --> E2{"redis-cli ping"}
    E2 -->|"PONG"| E3["Phir se bootRun"]
    E2 -->|"Error"| E4["Redis install check karo"]
    
    B -->|"Cannot serialize /<br/>SerializationException"| F["Letter bina envelope!"]
    F --> F1{"RedisConfig.java<br/>mein JSON serializer<br/>lagaya?"}
    F1 -->|"Nahi"| F2["GenericJackson2Json<br/>RedisSerializer set karo"]
    F1 -->|"Haan"| F3{"DTO mein<br/>implements Serializable<br/>hai?"}
    F3 -->|"Nahi"| F4["StudentDTO implements<br/>Serializable lagao"]
    F3 -->|"Haan"| F5["DTO mein no-arg<br/>constructor hai? Check karo<br/>@NoArgsConstructor"]
    
    B -->|"403 Forbidden"| G["Bina ticket ride<br/>pe baitha!"]
    G --> G1["Pehle /api/auth/login karo"]
    G1 --> G2["Token copy karo"]
    G2 --> G3["Header mein do:<br/>Authorization: Bearer TOKEN"]
    
    B -->|"No error! App running!"| H["Test karo!"]
    H --> H1["Browser: localhost:8080<br/>/swagger-ui/index.html"]

    style C fill:#ffcdd2
    style D fill:#ffcdd2
    style E fill:#ffcdd2
    style F fill:#ffcdd2
    style G fill:#ffcdd2
    style H fill:#c8e6c9
    style C3 fill:#c8e6c9
    style D3 fill:#c8e6c9
    style E3 fill:#c8e6c9
    style H1 fill:#c8e6c9
```

### Summary — Analogies Recap

```
┌─────────────────────────────────────────────────────────────────────────┐
│  ANALOGY RECAP — Yaad Rakh!                                            │
│                                                                        │
│  MySQL Start     = Engine start.                                       │
│                    Bina engine gaadi nahi chalegi.                      │
│                    Bina MySQL app data kahan se laayega?                │
│                                                                        │
│  Redis Start     = AC on.                                              │
│                    Bina AC gaadi chalegi but uncomfortable.             │
│                    Bina Redis app chalega but SLOW (har baar DB query). │
│                                                                        │
│  Port Conflict   = 2 gaadi 1 parking spot mein.                        │
│                    Ek ko hatana padega (kill process).                  │
│                    Ya doosri jagah park karo (change port).             │
│                                                                        │
│  Serialization   = Letter envelope mein.                               │
│                    Object = letter. Redis = POST office.                │
│                    Bina envelope (Serializable) = POST reject.          │
│                    JSON serializer = transparent envelope (readable).   │
│                    Jdk serializer = sealed black box (unreadable).      │
│                                                                        │
│  Cache HIT       = Fridge mein khana mila — microwave karo, khaao.     │
│  Cache MISS      = Fridge khaali — market jao, khana banao, fridge     │
│                    mein rakho next time ke liye.                        │
│  TTL Expire      = Khana expire — fenko, fresh banao.                  │
│                                                                        │
│  JWT Token       = Amusement park wrist band.                          │
│                    Ek baar lo (login) → har ride pe dikhao (API call).  │
│                    Expired? Naya band lo (re-login).                    │
│                                                                        │
│  Startup Flow    = Gaadi start sequence:                                │
│                    Key ghumao (gradlew) → Engine warm (compile) →       │
│                    Petrol check (MySQL) → AC on (Redis) →              │
│                    Steering ready (Tomcat 8080) → DRIVE!               │
└─────────────────────────────────────────────────────────────────────────┘
```

---

*Student Management System — by Arpan Maheshwari*
*"Spring Boot = Annotations ka khel. Samjho kya karta hai, kyun lagaya, bina iske kya hota — interview crack."*
