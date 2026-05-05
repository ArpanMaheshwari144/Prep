# 📊 Interview Prep — Progress Tracker

> **Last updated:** 2026-05-04 (Day 1 of HLD)
> **Bangalore arrival:** June 15, 2026 (~42 days)
> **Goal:** Strong profile + apply mode in Bangalore

---

## ✅ DONE — Java Side (FULL)

### 📂 `01_JAVA/01_CORE_JAVA/` — Foundation (22 topics)
- ✅ OOP fundamentals (4 pillars)
- ✅ Abstract Class vs Interface
- ✅ Method Overloading vs Overriding
- ✅ Constructor Chaining, Type Casting, instanceof
- ✅ String (immutable, pool, intern), StringBuilder vs StringBuffer
- ✅ Keywords: static, final, this, super
- ✅ JDK vs JRE vs JVM
- ✅ Access Modifiers, Wrapper Classes & Autoboxing
- ✅ Garbage Collection, Enum, Pass by Value
- ✅ Exceptions (Checked vs Unchecked, try-catch-finally)

### 📂 `01_JAVA/02_COLLECTIONS/` — 11 topics
- ✅ HashMap Internal (treeify, load factor)
- ✅ HashMap vs ConcurrentHashMap
- ✅ ArrayList, LinkedList, Iterator vs ListIterator
- ✅ HashMap / LinkedHashMap / TreeMap, HashSet
- ✅ Comparable vs Comparator + thenComparing
- ✅ fail-fast vs fail-safe, PriorityQueue (Min-Heap)

### 📂 `01_JAVA/03_JAVA_8/` — 5 topics
- ✅ Lambda, Functional Interfaces (PFCS), Method References, Streams API, Optional

### 📂 `01_JAVA/04_MULTITHREADING/` — 10 topics
- ✅ Thread basics, Lifecycle, synchronized, volatile, Deadlock
- ✅ wait() vs sleep(), notify() deep
- ✅ ExecutorService / Thread Pool
- ✅ Future vs CompletableFuture
- ✅ Producer-Consumer pattern

### 📂 `01_JAVA/05_SOLID/` — 5 + 1 topics
- ✅ SOLID intro + S, O, L, I, D principles individually

### 📂 `01_JAVA/06_DESIGN_PATTERNS/` — 6 patterns done
- ✅ Builder (with project usage — Jwts.builder, User.builder)
- ✅ Singleton (4 implementations, enum-best, Bloch's recommendation)
- ✅ Factory (with project usage — AccountFactory)
- ✅ Observer (with project usage — EventPublisher)
- ✅ Adapter (lean format)
- ✅ Strategy (lean format)
- 🔲 Decorator (paused — low priority)
- 🔲 Composite (paused — low priority)

### 📂 `01_JAVA/07_BRAIN_TICKLERS/` — 13 interview classics
- ✅ String intern, equals/hashCode trap, immutable defensive copy
- ✅ Static method hiding, finally return override, sleep lock
- ✅ TreeMap sorted order, Diamond problem, Map null rules
- ✅ final vs finally vs finalize, covariant return type
- ✅ Access modifier override, compile-time constant folding

### 📂 `01_JAVA/PROJECT/SimpleBankSystem/` — RUNNABLE PROJECT ✅
- 16 files (model, repo, service, factory, observer, config, exception, Main)
- All concepts integrated + inline pattern + SOLID comments
- Patterns: **Singleton, Factory, Observer, Template Method, Repository**
- SOLID: SRP, OCP, LSP, ISP, DIP all applied + documented
- Compile clean, runs successfully

---

## ✅ DONE — Spring Side (FULL)

### 📂 `02_SPRING/01_basics/` — Core
- ✅ Spring kya hai (IoC, container)
- ✅ Beans + IoC Container deep
- ✅ Annotations + DI types

### 📂 `02_SPRING/PROJECT/usercrud/` — RUNNABLE PROJECT (UPGRADED 2026-05-05) ✅
- ✅ Full CRUD (POST/GET/PUT/DELETE)
- ✅ Spring Data JPA + **MySQL 8 (Docker container)** ⭐ upgraded from H2
- ✅ Bean Validation (`@Valid`, `@NotBlank`, etc.)
- ✅ Global Exception Handler (`@RestControllerAdvice`)
- ✅ DTO pattern (LoginRequest, RegisterRequest, TokenResponse, RefreshRequest, ErrorResponse)
- ✅ Custom exceptions (UserNotFoundException)
- ✅ `@Transactional` integrated (write ops + readOnly + demo rollback endpoint)
- ✅ JWT + Spring Security (full integration):
  - JwtService (jjwt 0.12.6)
  - JwtFilter (extends OncePerRequestFilter)
  - SecurityConfig (filter chain, BCrypt, stateless sessions)
  - CustomUserDetailsService
  - RefreshToken entity + repository (DB-stored)
  - AuthController (login/register/refresh/logout)
- ✅ Inline pattern + SOLID comments throughout
- ✅ Compile clean, **end-to-end tested** with MySQL container
- ⭐ **Docker integration:**
  - `Dockerfile` (heavy comments)
  - `.dockerignore` (heavy comments)
  - MySQL Connector/J in pom.xml
  - application.properties → MySQL JDBC (`localhost:3307`)
  - Hibernate 7 dialect fix (MySQLDialect, not MySQL8Dialect)
  - **Real MySQL container** running with named volume `mysql_data`
  - Postman tested: register/login working with MySQL
  - **Volume persistence PROVEN** — container delete + recreate = data alive

### 📂 `02_SPRING/03_transactional/` — Deep theory ✅
- ✅ Propagation (7 types) + Isolation (4 levels) + Rollback rules
- ✅ Self-invocation pitfall + private/final method gotchas
- ✅ MySQL/JDBC equivalence section
- ✅ ACID, decision tree visuals

### 📂 `02_SPRING/04_jwt_security/` — Deep theory ✅
- ✅ JWT structure (header/payload/signature)
- ✅ Stateless deep dive (ATM analogy)
- ✅ Auth flow (login/protected/refresh/logout)
- ✅ Spring Security integration (filter chain, beans)
- ✅ Access + Refresh token lifecycle
- ✅ 7 security pitfalls + interview Q&A

---

## ✅ DONE — LLD (12 problems)

### 📂 `03_LLD/`
- ✅ 01_parking_lot
- ✅ 02_bookmyshow
- ✅ 03_tictactoe
- ✅ 04_snake_ladder
- ✅ 06_elevator
- ✅ 07_atm_machine
- ✅ 08_bank_account
- ✅ 09_shopping_cart
- ✅ 10_library
- ✅ 11_vending_machine
- ✅ 12_hotel_booking

---

## 🔄 IN PROGRESS — HLD (Day 1 done)

### 📂 `04_HLD/` — Foundation (7 of ~10)
- ✅ 01_hld_kya_hai (HLD intro, vs LLD, 4-question mindset)
- ✅ 02_capacity_estimation (back-of-envelope math)
- ✅ 03_load_balancing (Layer 4/7, algorithms, Nginx, AWS ALB/NLB)
- ✅ 04_caching (strategies, eviction, Redis vs Memcached)
- ✅ 05_database_replication (sync vs async, master-slave, lag, failover)
- ✅ 06_database_sharding (Range/Hash/Geo/Directory, Consistent Hashing)
- ✅ 07_message_queues (Kafka vs RabbitMQ, delivery semantics, DLQ)
- 🔜 08_cap_theorem
- 🔜 09_microservices_communication
- 🔜 10_consistency_models

### System Designs — 🎯 2 DEEP WFH, baaki Post-Bangalore

> **Strategy refined 2026-05-05 (40-day WFH target):**
> *"Base mazboot ke liye 2 system design WFH mein, baaki Bangalore jaane ke baad — wahan apply mode + design parallel."*

**WFH PHASE — 2 system designs DEEP (~14 days)**
- 🔜 **1. URL Shortener** — base ka base (LB + Cache + DB + Sharding + Hashing — all building blocks ek hi system mein)
- 🔜 **2. Rate Limiter** — different paradigm (stateful, concurrent, algorithmic)
   - Token bucket, sliding window, distributed via Redis
   - Pairs with URL Shortener for max base coverage

**Why these 2 specifically?**
- URL Shortener = **storage paradigm** (stateless, write-heavy, hashing)
- Rate Limiter = **algorithmic paradigm** (stateful, concurrent, race conditions)
- 2 different paradigms → base maximally diverse, building blocks deeply locked
- *Bruce Lee: ONE kick × 10,000 times > 10,000 kicks × ONE time*

**POST-BANGALORE PHASE (June 15+, apply mode parallel)**
- 🔜 Notification System (fan-out, queues, delivery guarantees)
- 🔜 Chat System (WhatsApp — real-time, WebSockets)
- 🔜 Twitter Feed (fan-out on read vs write)
- 🔜 Uber (geospatial, matching engine)
- 🔜 Payment Gateway (idempotency, transactions)
- 🔜 + Instagram / YouTube / BookMyShow / Search Autocomplete / Distributed Cache (as needed)

---

## 🔄 IN PROGRESS — Docker (Day 1 done, ~60% locked)

### 📂 `02_SPRING/DOCKER_NOTES.md` — Comprehensive (~1900 lines)

**✅ DONE — Foundation + Volumes (HANDS-ON proven)**
- ✅ Docker Desktop install (WSL 2 backend)
- ✅ Image vs Container concept (recipe vs dish — proven)
- ✅ docker pull / run / ps / logs / exec / rm / stop / start
- ✅ Port mapping (-p) + port conflict resolution
- ✅ Environment variables (-e) — MySQL config injection
- ✅ Detached mode (-d), Container naming (--name)
- ✅ Container isolation (local MySQL57 vs Docker MySQL — separate worlds)
- ✅ Spring Boot ↔ Docker MySQL real integration (JDBC)
- ✅ Hibernate 7 dialect trap fix (MySQL8Dialect → MySQLDialect)
- ✅ **VOLUMES — DEEP locked with disaster + recovery proof**
  - Named / Bind / tmpfs types
  - `docker volume create` / `ls` / `inspect` / `rm` / `prune`
  - Anonymous volume orphan trap
  - LIVE PROOF: container delete + recreate + same volume = data alive

**🔜 PENDING (next session, ~3-4 days)**
- 🔜 Networks (multi-container talk, bridge/host/overlay, container DNS)
- 🔜 docker-compose (Spring Boot + MySQL + Redis ek YAML mein)
- 🔜 ENTRYPOINT vs CMD (interview classic)
- 🔜 Multi-stage builds (production image small)
- 🔜 ARG vs ENV (build-time vs runtime)
- 🔜 HEALTHCHECK
- 🔜 Image push to Docker Hub

---

## 🔲 PENDING — K8s (after Docker complete)

- 🔲 Pod, Deployment, Service (foundation)
- 🔲 Ingress, ConfigMap, Secret
- 🔲 HPA (auto-scale), PV/PVC
- 🔲 Notes: V90 format like Docker
- ⭐ Docker Desktop has K8s integrated — bonus when time

---

## 🔲 PENDING — Hibernate Gotchas (alongside Docker remaining)

- 🔲 **JPA vs Hibernate** distinction (Hibernate IS used in UserCRUD via Spring Data JPA)
- 🔲 **N+1 problem** + JOIN FETCH solution
- 🔲 **LAZY vs EAGER** fetch types
- 🔲 **Dirty checking** + first-level cache
- 🔲 Connect to project: yeh sab UserCRUD mein already chal raha hai under hood

---

## 🔲 PENDING — Post Bangalore (June 15+)

### 1. Microservices
- 🔲 Spring Cloud, Eureka (service discovery)
- 🔲 Feign client (REST inter-service)
- 🔲 Resilience4j (circuit breaker)
- 🔲 API Gateway (Spring Cloud Gateway)
- 🔲 Distributed tracing (Sleuth, Zipkin)

### 2. Infrastructure
- 🔲 **Docker basics** + Dockerfile
- 🔲 **Docker Compose** (multi-container)
- 🔲 **Kubernetes** (pod, deployment, service, configmap, ingress)
- 🔲 K8s networking + service mesh

### 3. Messaging & Async
- 🔲 Kafka deep (producer/consumer/topics/partitions)
- 🔲 RabbitMQ (exchanges/queues)
- 🔲 Spring `@Async` + `@Scheduled`

### 4. Cloud
- 🔲 AWS hands-on (EC2, RDS, S3, Lambda)
- 🔲 AWS deployment patterns
- 🔲 CloudWatch + monitoring

### 5. DSA (paused per Arpan's call)
- 🔲 Pattern recognition (his weakest area, slow track)
- 🔲 Will resume post-Bangalore based on interview demands

### 6. Mock Interviews
- 🔲 Daily voice mocks (post-revision deep lock)
- 🔲 30+ mocks before serious interviews
- 🔲 System design mocks specifically

### 7. Optional Java Topics
- 🔲 `record` (Java 14+) — immutable data class
- 🔲 Sealed classes (Java 17+)
- 🔲 Pattern matching (Java 21+)

---

## 📅 Approach — 40-Day WFH Plan (locked 2026-05-05)

> **Target:** 5 May → 15 June (~40 days, base mazboot)

```
WFH PHASE (40 days) — base mazboot karna hai

   Phase 1 — HLD foundations 3 baki (~6 days)
      🔜 CAP Theorem, MS Communication, Consistency Models
      
   Phase 2 — Docker advanced + K8s theory (~10 days)
      ENTRYPOINT vs CMD, multi-stage, compose
      K8s: Pod, Deployment, Service, Ingress
      (Hands-on deferred — laptop constraint)
      
   Phase 3 — 2 SYSTEM DESIGNS DEEP (~14 days)
      URL Shortener  → storage paradigm (LB+Cache+DB+Shard+Hash)
      Rate Limiter   → algorithmic paradigm (token/window, distributed)
      Two paradigms = base maximally diverse
   
   Phase 4 — Java/Spring revision (~10 days, parallel)
      ✅ Tu khud — Claude help nahi (already locked)
      
   + Buffer/rest/mocks parallel

15 June  → 🚀 BANGALORE ARRIVAL (fixed)
            = Apply mode + remaining system designs parallel
            = Notification, Chat, Twitter, Uber, Payment, etc.
```

**WFH goal: BASE MAZBOOT.** Sab system designs nahi — sirf 2 deep + foundations + Docker theory. Bangalore mein continue.

---

## 🌟 Wins So Far

- **70+ Java notes** (V90 format with visualizations)
- **3 enhanced topical sections** (@Transactional, JWT, Producer-Consumer)
- **6 Design Pattern notes** with project usage
- **7 HLD foundation notes** (intro/capacity/LB/cache/replication/sharding/MQ)
- **2 runnable projects** (SimpleBankSystem + UserCRUD)
- **End-to-end JWT auth** in UserCRUD (tested, working)
- **All inline pattern + SOLID comments** in both projects
- **⭐ DOCKER_NOTES.md** (~1900 lines, V90, hands-on captured)
- **⭐ UserCRUD ↔ Docker MySQL** end-to-end (Postman tested)
- **⭐ Volumes proven** via live disaster + recovery demo
- **⭐ Maven manual install** (user-level, no admin)
- **GitHub clean structure**: Java → Spring (project + Docker) → LLD → HLD → Reference

---

## 💎 Real Position (May 5) — Phase-Wise

**Java foundation:** Theory ✅ + 4yr practice ✅ = ALL DONE
**Spring foundation:** Theory ✅ + project ✅ + JWT tested ✅ = ALL DONE
**UserCRUD project:** ⭐ MySQL container integration DONE + Volumes proven
**LLD:** 12 problems ✅ = DONE
**HLD foundations:** 7/10 done (Sharding + MQ done 5 May) — ON TRACK
**HLD system designs:** 0/2 WFH (URL Shortener + Rate Limiter — next phase)
**Docker:** ⭐ ~60% done — Foundation + Volumes deep+hands-on (proven)
              Pending: Networks, Compose, ENTRYPOINT, Multi-stage (~3-4 days)
**K8s:** Pending (~5 days theory, after Docker complete)
**DSA:** Paused (Arpan's call)
**Mocks:** Pilot done, articulation drills pending (post-foundations)

**Realistic verdict:** **June 15 Bangalore = base mazboot (HLD foundations + 2 deep systems + Docker/K8s theory + Java/Spring revised).** Apply mode + remaining system designs parallel from there.

---

## 🎯 Daily Routine — MORNING-ONLY (locked 2026-05-04)

> *"Sab subah hi karenge"* — peak focus window, fresh brain, deep retention.

```
🌅 MORNING (deep work — 2-3 hr)
   → 1 HLD topic mastered DEEPLY (foundation OR system design)
   → 1 topic DEEP > 3 topics shallow
   → Active recall, voice mocks, diagrams

☀️ Afternoon onwards
   → Light revision (read existing notes)
   → Rest / non-prep time
   → Apply mode (post-Bangalore)
```

**Why morning-only?**
- Brain freshest → best concept locking
- Avoids burnout, sustainable till Bangalore + beyond
- Quality > hours-grinded

**Rest days when needed.** No grinding without break.

---

## 🔥 Mindset Anchors

- **DEPTH > SPEED** — "kitne bhi din lage, koi dikkat nahi"
- **Master each topic** before moving — surface knowledge useless
- **No timeline pressure** — Bangalore arrival fixed, prep flexible
- **🥋 Bruce Lee philosophy** — *"I fear not the man who practiced 10,000 kicks once, but the man who practiced ONE kick 10,000 times"*
  - Same interview questions repeat → master those deeply
  - Don't chase 1000 new topics — pel the SAME critical ones repeatedly
- **"AI multiplier, not replacement"** — orchestrate, don't follow
- **Effort + direction (in control) → universe writes outcomes**
- **Pattern over content** — same template across system designs
- **Theory > rote** — concept lock > memorization
- **Revision > volume** — same notes 4x cycle for permanence
