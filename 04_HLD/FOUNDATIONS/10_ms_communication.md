# Microservices Communication

> **HLD Topic 10 — How services talk in distributed systems**

---

## Why This Matters?

### Monolith
```
userService.create(user)  ← method call
   ↓
   Same JVM, same memory
   = Instant, simple, reliable
```

### Microservices
```
User Service (Java)  ───?───  Order Service (Python)
                     ───?───  Payment Service (Node)
                     ───?───  Email Service (Go)

Different processes
Different machines
Different networks
   = HOW do they communicate?
```

**Yeh foundation hai distributed systems ki — galat choice = production disaster.**

---

## STORY — Office Communication

> 4-person office, 1 room (monolith):
>    "Bhai file de" → turn around → done.
>    = Fast, no overhead.
>
> 50 employees, 5 floors (microservices):
>    Floor 2 needs data from Floor 4.
>    Options:
>       **Phone call** → wait for pickup, get answer (SYNC — REST/gRPC)
>       **Email** → drop message, continue work, reply later (ASYNC — Queue)
>
> **Critical task?** Phone call (waiting OK).
> **Non-urgent?** Email (don't block your work).

---

## 2 BIG Categories

```
1. SYNCHRONOUS  → Caller WAITS for response
                  "Bhai answer de phir aage badhunga"
                  Tools: REST, gRPC

2. ASYNCHRONOUS → Caller doesn't wait
                  "Yeh kaam kar lo, mein chala"
                  Tools: Kafka, RabbitMQ, SQS
```

---

## 1️⃣ SYNCHRONOUS — REST vs gRPC

### REST (HTTP + JSON)
```
Order Service ──POST /users/123──→ User Service
              ←───── JSON ────────

Format: JSON
Protocol: HTTP/1.1
Contract: OpenAPI/Swagger (loose)
```

```
Pros:
   • Simple, browser-friendly
   • Easy debug (curl, Postman)
   • Universal — every lang supports
   • Human-readable

Cons:
   • JSON heavy (verbose)
   • No strict contract
   • Slower (~5-10x vs gRPC)
   • Each call = new TCP connection (HTTP/1.1)
```

### gRPC (HTTP/2 + Protobuf)
```
Order Service ──proto binary──→ User Service
              ←──── binary─────

Format: Protocol Buffers (binary)
Protocol: HTTP/2 (multiplexed)
Contract: .proto file (strict, code-generated)
```

```
Pros:
   • 5-10x faster than REST
   • Strict contract (compile-time errors)
   • Streaming support (bidirectional)
   • Smaller payload (binary)

Cons:
   • Browser unfriendly (needs grpc-web)
   • Hard to debug (binary, not human-readable)
   • Steeper learning curve
```

### When to use which?
```
External APIs (mobile, frontend, public):  REST
                                            (universal, debug-friendly)

Internal MS-to-MS:                          gRPC
                                            (Google, Uber, Netflix)
                                            
Hybrid (real production):                   REST gateway → gRPC internal
```

---

## 2️⃣ ASYNCHRONOUS — Message Queues

```
Order Service ──publish event──→ [Kafka/RabbitMQ]
                                       ↓
                                  Payment Service ── consume
                                  Email Service   ── consume
                                  Analytics       ── consume

Order Service doesn't wait
Each consumer processes at own pace
```

**Use kab:**
```
Order placed → email send (async, non-critical)
Video uploaded → thumbnail generate (slow)
Multiple consumers (analytics + email + audit)
Spike absorption (queue buffers traffic burst)
Loose coupling (services don't know each other)
```

> Detail: [07_message_queues.md](07_message_queues.md)

---

## Sync vs Async Decision Matrix

```
SYNC use karo jab:
   Response chaiye immediately
   User waiting (login, payment confirm)
   Failure ko handle karna hai right now
   Strong consistency needed

ASYNC use karo jab:
   Background work (email, notification)
   Slow processing (video, ML, reports)
   Multiple consumers
   Loose coupling chahiye
   Spike absorption needed
```

---

## Resilience Patterns (Senior Signal)

### 1. Circuit Breaker
```
Problem:
   User Service down hai?
   → Order Service har request fail karega
   → Threads pile up
   → Order Service bhi crash
   = CASCADE FAILURE

Solution: Circuit Breaker
   3 consecutive failures detected
   ↓
   Circuit "OPEN" → reject requests immediately for 30s
   ↓
   After 30s → Circuit "HALF-OPEN" → try one request
   ↓
   Success? → Close circuit (resume normal)
   Fail?    → Re-open for 30s
```

```
States:
   CLOSED      → All requests pass (normal)
   OPEN        → All requests fail fast (broken)
   HALF-OPEN   → Test one request (recovery check)

Tools: Resilience4j (Spring Boot), Hystrix (legacy Netflix)
```

### 2. Retry + Exponential Backoff
```
1st fail → wait 1s  → retry
2nd fail → wait 2s  → retry
3rd fail → wait 4s  → retry
4th fail → wait 8s  → retry
= Exponential backoff
= Don't hammer a dying service
= Give it room to recover

Add jitter (randomness):
   wait = base * 2^n + random(0, 1000ms)
   = Prevents thundering herd (all clients retry at same instant)
```

### 3. Timeout
```
Default behavior:
   No timeout = thread waits FOREVER
   = Thread pool exhausted
   = Service hangs

Solution: Set explicit timeout
   3 seconds max per call
   = Fail fast
   = Free up thread pool
   = Other requests can proceed

Spring Boot:
   @HttpExchange with timeout
   RestTemplate / WebClient configured
```

### 4. Bulkhead
```
Concept (ship analogy):
   Ship hull divided into compartments
   One compartment leaks ≠ ship sinks
   
In MS:
   Separate thread pools for each downstream service
   User Service threads exhausted ≠ Order Service blocked
```

---

## Service Discovery

### Problem
```
User Service ka IP kya hai?
   Hardcode IP?     = brittle (IP changes)
   Manual config?   = doesn't scale
   DNS?             = slow propagation
```

### Solution: Service Registry
```
1. Service starts → registers self
   "I am 'user-service' at 10.2.3.4:8080"

2. Caller queries registry
   "Where is 'user-service'?"
   ← "10.2.3.4:8080"

3. Service dies → deregisters / health check fails
```

### Tools
```
Eureka       → Spring/Netflix ecosystem
Consul       → HashiCorp
Zookeeper    → Old school (still used)
K8s Service  → BUILT-IN (free with K8s)
```

### K8s ka Magic
```
K8s mein Eureka ki zaroorat NAHI:
   Service name = DNS hostname automatically
   
   Order Service mein:
      RestTemplate.getForObject(
          "http://user-service:8080/api/users/123",
          User.class
      )
   
   "user-service" = K8s Service name
   K8s DNS resolves to right pod IP
   = Built-in service discovery
```

---

## Real Production Pattern

### E-commerce Checkout Flow

```
User clicks "Buy Now":
   ↓
   API Gateway receives request
   ↓
   ┌──────────────────────────────────────┐
   │  CRITICAL PATH (SYNC)                │
   │  User waiting — must complete        │
   ├──────────────────────────────────────┤
   │  → Payment Service (gRPC)            │
   │      validate card, charge           │
   │  → Inventory Service (gRPC)          │
   │      reserve stock                   │
   │  → Order Service (gRPC)              │
   │      create order record             │
   └──────────────────────────────────────┘
   ↓ (all sync done — user gets response)
   ↓
   Return 200 OK to user
   ↓
   ┌──────────────────────────────────────┐
   │  SIDE EFFECTS (ASYNC)                │
   │  User doesn't wait                   │
   ├──────────────────────────────────────┤
   │  → Kafka: "OrderPlaced" event        │
   │      ↓ consumed by:                  │
   │      • Email Service                 │
   │      • SMS Service                   │
   │      • Analytics Service             │
   │      • Recommendation Engine         │
   │      • Audit Log                     │
   └──────────────────────────────────────┘
```

```
Critical path SYNC:
   Must complete before response
   Failure = transaction rollback
   User experience direct impact

Side effects ASYNC:
   Eventual processing fine
   Failure = retry from queue
   Multiple consumers benefit
```

---

## API Gateway (Bonus)

```
Without Gateway:
   Mobile App → User Service
              → Order Service     [10 different URLs/auth/etc.]
              → Product Service
              ...

With Gateway:
   Mobile App → API Gateway (single entry)
                ├─ Auth
                ├─ Rate limit
                ├─ Routing
                ├─ Logging
                ├─ Aggregation (BFF pattern)
                └─→ User/Order/Product services internally

Tools: Kong, AWS API Gateway, Spring Cloud Gateway
```

---

## TRAP — What NOT To Do

```
"Microservices = always async via Kafka"
   = WRONG. User-facing critical path = sync.

"Use REST everywhere internally"
   = Slow at scale. Internal traffic 100K+ rps = gRPC wins.

"Each service has its own DB but call each other directly"
   = Tight coupling reborn. Use events.

"No timeouts because it usually works"
   = Production killer. Always timeout.

"Retry forever until success"
   = Hammers dying service. Use exponential backoff + max retries.

"Hardcode service IPs"
   = Restart breaks everything. Use service discovery.

"No circuit breaker — handle errors with try-catch"
   = Try-catch doesn't prevent thread pool exhaustion.
```

---

## Interview Power Phrases

```
"External APIs REST for universality, internal gRPC for perf"

"Critical path sync, side effects async via Kafka"

"Circuit breaker (Resilience4j) prevents cascade failures"

"Exponential backoff + jitter — avoid thundering herd"

"K8s Service = free service discovery, no Eureka in EKS"

"Bulkhead pattern: per-downstream thread pool isolation"

"API Gateway: single entry, auth + rate-limit + routing"

"Polyglot communication: right protocol per use case"
```

---

## Memory Hooks

```
Phone call vs Email
   = Sync (REST/gRPC) vs Async (Kafka)

Office floors → networks
   = Different process boundaries

Restaurant slips
   = Async queue decoupling

Ship compartments
   = Bulkhead isolation

Electrical fuse
   = Circuit breaker
```

---

## Phase 3 Project Application (July/August)

```
When building full Spring microservices project:

   Service-to-service:
      Internal: gRPC (Spring gRPC)
      External: REST (Spring Web)
   
   Async events:
      Kafka topics for OrderPlaced, PaymentDone
   
   Resilience:
      Resilience4j → Circuit breaker + retry
      Spring Cloud → Timeout config
   
   Discovery:
      K8s Service (since deploying on EKS)
   
   Gateway:
      Spring Cloud Gateway → single entry
   
= Production-grade architecture
= Interview-ready talking points
```

---

## Status

```
Topic 10/10 (HLD foundations) — Done 

Foundations complete:
   01 HLD basics
   02 Capacity estimation
   03 Load balancing
   04 Caching
   05 DB replication
   06 DB sharding
   07 Message queues
   08 CAP theorem
   09 DB types (what when)
   10 MS Communication ← YOU ARE HERE

Next phase:
   System Design Deep Dives
      • URL Shortener
      • Rate Limiter
```

[← HLD README](README.md)
