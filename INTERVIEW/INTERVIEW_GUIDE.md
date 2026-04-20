# Interview Guide v2 — Arpan Maheshwari
### Resume-aligned | 30-min Pre-Interview Refresh | Buzzwords MUH-PE

> **"Knowledge + Authenticity = Real Engineer Signal. Over-polish = Rehearsed = Red Flag."**
> **"Don't memorize. Skim before interview. Key buzzwords tongue pe."**

---

# TABLE OF CONTENTS

1. [2-Min Project Pitch](#1-project-pitch)
2. [Resume Bullets → Expected Q&A](#2-resume-bullets--expected-qa) ⭐ **MAIN SECTION**
3. [30-Min Buzzword MUH-PE Reference](#3-buzzword-muh-pe) ⭐ **PRE-INTERVIEW**
4. [Behavioral STAR Stories](#4-behavioral-star-stories)
5. [Honest Defense Patterns](#5-honest-defense-patterns) ⭐ **WHEN STUCK**
6. [System Design Buzzwords](#6-system-design-buzzwords)
7. [Why Hire You Pitch](#7-why-hire-you)
8. [Questions to Ask Interviewer](#8-questions-to-ask)
9. [Common Traps](#9-common-traps)

---

# 1. PROJECT PITCH

## 2-Min Opening (Open Any Interview With This)

```
"I'm a Backend Software Engineer at Konovo (formerly GlocalMind) — 
an AI-powered healthcare market research platform serving 10+ countries.

I build Java/Spring Boot microservices with MySQL, MongoDB, Redis, 
Docker, and AWS. My biggest ownership: a vendor-facing portal handling 
integrations with Doximity, M3, and other partners — REST APIs, data 
models, integration pipelines, survey delivery, completion tracking.

On top of feature work, I own production incidents end-to-end — 
500+ resolved, 92% closure rate, including 14 Critical/Blocker and 
110 Major. I investigate through log correlation using CloudWatch, 
Papertrail, and New Relic, then fix the root cause in code or 
database schema.

I've also built an email deduplication API that killed 95% of dupes, 
migrated the platform from JDK 8 to JDK 11 with zero downtime, 
expanded JUnit test coverage with Jenkins auto-reports, and mentored 
a junior engineer. I've been recognized 3 times by VP and CTO for 
critical production investigations.

I'm deeply hands-on and love owning things end-to-end. Open to relocation."
```

**Delivery tip:** Don't rush. 2 minutes feels longer than you think.

---

## Project Architecture Visual

```
  ┌────────────────────────────────────────────────────────────┐
  │         KONOVO — HEALTHCARE MARKET RESEARCH PLATFORM        │
  │                                                            │
  │  🌐 External Vendors (Doximity, M3)                        │
  │   │                                                        │
  │   ↓                                                        │
  │  🔐 Vendor Portal — REST APIs + Integration Pipelines      │
  │   │                                                        │
  │   ↓                                                        │
  │  🧠 Business Services ──→ 🏪 Repository ──→ 💾 MySQL       │
  │       │                        │                           │
  │       ↓                        ↓                           │
  │       📨 Email Service    ⚡ Redis Cache                    │
  │       📊 Survey Tracking  🗄️ MongoDB (specific data)       │
  │                                                            │
  │  📈 Observability: CloudWatch, Papertrail, New Relic APM   │
  │  🐳 Deployment: Docker, Jenkins CI/CD                      │
  └────────────────────────────────────────────────────────────┘
```

---

# 2. RESUME BULLETS → EXPECTED Q&A

**Read this section 30 min before interview. Every resume bullet = possible deep-dive question.**

---

## 🧰 BULLET 1: Vendor Portal (Doximity, M3)

**Resume line:** *"Built and owned the vendor-facing portal end-to-end — REST APIs, data models, integration pipelines for Doximity, M3 — survey delivery, completion tracking, vendor API reconciliation."*

### Q: "Walk me through the vendor portal architecture."

**Buzzword answer (1-line):**
> "Spring Boot REST APIs exposing survey delivery endpoints, MySQL persistence, integration pipelines pulling vendor user data and pushing survey completion events back to vendor APIs."

**Full answer (2 min):**
> "Vendors like Doximity and M3 have physician networks. They send users to our platform to take surveys. Flow:
> - Vendor user lands on our portal via vendor API handshake
> - We capture vendor metadata (vendor ID, user context)
> - User takes survey, we save responses to MySQL
> - On completion, our system sends event back to vendor API for reconciliation and payout tracking
> - I designed the REST endpoints, data models, and integration retry logic"

### Follow-up Q: "How do vendors authenticate?"
**Safe answer:** "API key based — each vendor gets unique credentials, validated at gateway before reaching our APIs. For sensitive operations we also verify signature."

### Follow-up Q: "What if vendor API is down?"
**Safe answer:** "Retry logic with exponential backoff. Events queued for replay. Logged to Papertrail for investigation."

### Follow-up Q: "What about rate limiting per vendor?"
**Safe answer:** "Yes — different quotas per vendor SLA, enforced at gateway or in-app. We track request counts and reject with 429 when exceeded."

### "I DON'T KNOW" fallback:
> "Honestly not sure about that specific config — my work was more on the API design and data flow side. I'd check with the platform team or dig into our gateway config if I needed to answer that."

---

## 📧 BULLET 2: Email Deduplication API

**Resume line:** *"Built an email deduplication API using normalization and fuzzy matching — eliminated 95% of duplicate physician records."*

### Q: "Walk me through the dedup algorithm."

**Buzzword answer:**
> "Normalize email (lowercase, trim, strip subaddresses), then Levenshtein-based fuzzy matching with a similarity threshold for near-duplicates."

**Full answer:**
> "Two layers:
> 1. **Normalization** — lowercase, trim whitespace, strip Gmail dots, remove `+alias` subaddresses (arpan+test@gmail.com → arpan@gmail.com)
> 2. **Fuzzy match** — for names/emails close but not identical, I used edit distance (Levenshtein) with a threshold around 85% similarity
> 3. Before inserting new records, I check against existing ones — if match → merge, else insert"

### Follow-up Q: "Why not just UNIQUE constraint on email?"
**Answer:** "UNIQUE catches only EXACT match. Dr. Smith registered as 'john.smith@gmail.com' and later 'John.Smith@gmail.com' — UNIQUE wouldn't catch. Normalization + fuzzy handles real-world dirty data."

### "I DON'T KNOW" fallback:
> "I don't remember the exact Levenshtein threshold we tuned to — it was iterative based on false positive rate. Happy to look up the code if we continue."

---

## 🔁 BULLET 3: JDK 8 → 11 Migration

**Resume line:** *"Migrated GlocalMind platform from JDK 8 to JDK 11 with zero downtime — resolved dependency conflicts, 15% throughput improvement."*

### Q: "What broke during the JDK 8 → 11 migration?"

**Buzzword answer:**
> "Removed javax.* packages (Java EE modules like JAXB, JAX-WS, CORBA), classpath vs module path changes, some Hibernate/Spring versions needed updates."

**Full answer:**
> "Main issues:
> 1. **javax.* modules removed** — JAXB, JAX-WS, activation, CORBA — had to add as explicit dependencies
> 2. **Reflection warnings** — Spring and Hibernate reflecting into JDK internals; suppressed with `--add-opens` JVM flags
> 3. **String methods** — `String.isBlank()`, `String.strip()` are JDK 11+, opportunity to use
> 4. **Gradle / Maven** version bumps for JDK 11 support
> 5. Throughput improvement from **G1 GC becoming default** in JDK 9+ and JIT improvements
>
> We rolled out canary → 10% → 50% → 100% over a week, monitored via New Relic. No downtime because we used rolling deploys."

### Follow-up Q: "What was the trickiest bug?"
**Safe answer:** "A library we used depended on `sun.misc.Unsafe` which is stricter in JDK 11. Had to update the library version that used the VarHandle API instead."

### "I DON'T KNOW" fallback:
> "Honestly I handled certain parts — our DevOps lead owned the JVM flags and deployment tuning. I focused on the code-side fixes."

---

## ⚡ BULLET 4: Profiled 15+ Microservices (30% gain)

**Resume line:** *"Profiled 15+ Spring Boot microservices — cut query execution time by 30% through targeted index and pagination fixes."*

### Q: "What profiling tool did you use?"

**Answer:** "New Relic APM for end-to-end request traces, MySQL EXPLAIN for query plans, and application logs for slow query identification."

### Q: "Give me a specific example of an index you added."

**Answer:**
> "One case — survey responses query filtered by `user_id` and `status`. Table had ~5 million rows. Query took ~2 seconds doing full scan. I added a **composite index on (user_id, status)** — reduced to 50ms. EXPLAIN confirmed the index was used."

### Q: "How did pagination help?"
**Answer:**
> "We had endpoints returning all records without limit — some hit 50k rows. Added `LIMIT/OFFSET` with default page size 20, max 100. Also for deep pagination switched to **keyset pagination** (WHERE id > last_id) since OFFSET gets slow for page 100+."

### "I DON'T KNOW" fallback:
> "I don't remember the exact query on that one — but pattern was: profile → find slow query → EXPLAIN → add index or add LIMIT. Measured via New Relic transaction traces."

---

## 🧪 BULLET 5: JUnit + Jenkins

**Resume line:** *"Expanded JUnit test coverage across Spring Boot microservices and configured Jenkins to email automated test reports."*

### Q: "What's your testing approach?"

**Answer:** "Unit tests with JUnit 5 + Mockito for mocking dependencies, integration tests for DB and REST layers. Aim for meaningful coverage — not chasing %, focusing on business logic."

### Q: "What does Jenkins do?"
**Answer:** "Every push triggers build → compile → tests → if fail, email notification to team. Prevents broken code reaching main branch."

### "I DON'T KNOW" fallback:
> "I don't own the entire CI pipeline — DevOps team manages infra. I added test coverage and hooked into the existing Jenkins job."

---

## 🔍 BULLET 6: AWS API Gateway X-Forwarded-For

**Resume line:** *"Discovered AWS API Gateway silently mis-parsing X-Forwarded-For headers — flagging valid IPs as duplicates across 300+ vendor integrations."*

### Q: "Walk me through this bug."

**Answer:**
> "We had a rate limiter using client IP (from X-Forwarded-For header). Started seeing legitimate vendor traffic rate-limited. Team dismissed it as 'vendors sending bad IPs'. I dug in:
> - Captured raw headers from AWS API Gateway logs
> - Found Gateway was appending its internal IP to X-Forwarded-For, so we got `vendor_IP, gateway_IP`
> - Our parser took the LAST IP instead of FIRST → all requests rate-limited by gateway IP
> - Fix: take FIRST (leftmost) IP in chain — that's the original client
> - Shipped fix, 300+ integrations restored
> - Recognized by VP and CTO"

### Q: "What's the standard for X-Forwarded-For parsing?"
**Answer:** "Leftmost IP is the original client. Each proxy appends its IP. RFC 7239 (Forwarded header) clarifies this officially."

### "I DON'T KNOW" fallback:
> "Exact RFC number I don't remember — but the pattern is leftmost = original."

---

## 🔌 BULLET 7: MySQL HikariCP Pool Exhaustion ⭐ KEY STORY

**Resume line:** *"Diagnosed a MySQL HikariCP connection pool exhaustion causing 95% email delivery failure (959/1009 dropped) — isolated blocking batch query, shipped targeted fix."*

### Q: "Walk me through this — how did you diagnose it?"

**Buzzword answer:**
> "HikariCP logs threw 'Connection is not available, request timed out after 30000ms' — pool at maxSize, waiting threads building up. Pivoted to DB, found a blocking UPDATE query from an external tool locking shared tables."

**Full answer (STAR):**
> **Situation:** 959 out of 1009 emails dropped in one batch. Critical — clients waiting for survey emails.
> 
> **Task:** Root cause and restore.
> 
> **Action:**
> 1. Checked app logs — HikariCP signature error: *'Connection is not available, request timed out after 30000ms'*
> 2. Knew this meant **pool exhaustion** — all connections held, new requests waiting >30s, timing out
> 3. Normal baseline was ~2-3 active connections, pool max is 10. Suddenly pegged at max with 50+ threads waiting.
> 4. Connections were HELD — not returned to pool. So something in DB was blocking.
> 5. Went to RDS — **top waits** showed a long-running UPDATE holding table locks. 15 minutes elapsed.
> 6. Traced — external reporting tool ran batch UPDATE that locked the shared survey table.
> 7. Our app connections were stuck waiting for locks → pool filled → new requests timed out → emails failed.
> 
> **Result:** Immediate fix — waited for UPDATE to finish, restarted affected pods. Longer term suggested **separating reporting DB from transactional DB** and adding **query timeout on batch operations**. 

### Q: "What is HikariCP?"
**Answer:** "Java connection pool library — Spring Boot default. Manages reusable DB connections. Default maxPoolSize 10, connectionTimeout 30s."

### Q: "What are the key HikariCP configs?"
```properties
spring.datasource.hikari.maximum-pool-size=10      # Max connections
spring.datasource.hikari.connection-timeout=30000  # Max wait for connection (30s)
spring.datasource.hikari.idle-timeout=600000       # Unused connection timeout
spring.datasource.hikari.max-lifetime=1800000      # Force recycle connection
spring.datasource.hikari.minimum-idle=2            # Always keep this many ready
```

### Q: "How would you have prevented this?"
**Answer:** "Multiple angles:
- **Alerting** on pool utilization >80% → investigate before exhaustion
- **Query timeout** on batch operations so locks auto-release
- **Separate pools or DBs** for reporting/analytics vs transactional
- **Read replica** for reporting to offload from main DB"

### "I DON'T KNOW" fallback:
> "I didn't know HikariCP deeply before this incident — learned from the logs. I now understand the pool concept, but I wouldn't call myself a HikariCP expert. The investigation was more about following the error to the DB issue."

---

## 🔄 BULLET 8: MongoDB-MySQL Sync Gap

**Resume line:** *"Uncovered a MongoDB-MySQL sync gap silently blocking survey launches — traced the full API call chain, patched 7 affected surveys."*

### Q: "Why both MongoDB AND MySQL?"
**Answer:** "MySQL for transactional data (users, payments, relational structures). MongoDB for flexible survey responses (nested JSON, dynamic schema per survey)."

### Q: "Walk me through the sync gap."
**Answer:**
> "Survey creation wrote metadata to MySQL and survey schema to MongoDB. For certain branch paths in code, the MongoDB write succeeded but the MySQL `is_launched` flag wasn't updated. Surveys appeared broken to users — MongoDB had the data, MySQL thought it wasn't launched.
> 
> I traced the entire API call chain — found one conditional path that skipped the MySQL update. Fixed the code and patched the 7 already-affected surveys manually with a SQL UPDATE."

### Q: "How do you prevent sync gaps in general?"
**Answer:** "Ideal is **distributed transaction** or **event-driven** (write to MySQL, publish event, consumer updates MongoDB). Realistic — add integration tests covering all code paths, and have reconciliation jobs detect mismatches."

---

## ⚠️ BULLET 9: AWS Lambda Concurrency

**Resume line:** *"Correlated CloudWatch, CloudTrail, and New Relic APM to isolate an AWS Lambda concurrency misconfiguration causing 1,800+ throttles and 8x page load spike."*

### Q: "What is Lambda concurrency?"
**Answer:** "Max simultaneous Lambda executions. Default account limit 1000 per region. **Reserved concurrency** locks a specific amount for a function. **Provisioned concurrency** keeps warm instances ready to avoid cold starts."

### Q: "What was the misconfig?"
**Answer:** "Our function had reserved concurrency set too low (say 10) while traffic spiked. Excess requests hit the concurrency limit and Lambda threw 429 Throttle. Upstream saw failures and retried, amplifying load. Page load spiked 8x because retries cascaded."

### Q: "How did you correlate across tools?"
**Answer:**
> "**CloudWatch** showed Lambda throttle metrics (1800+ throttles). 
> **CloudTrail** showed the config change timestamp — someone reduced reserved concurrency in a deploy.
> **New Relic APM** showed page load spike correlating with the throttle timeline.
> Triangulated the config change → throttles → page load. Fix: adjust reserved concurrency to handle peak traffic."

### "I DON'T KNOW" fallback:
> "I'm not a Lambda expert at the infrastructure-design level — I understood enough of concurrency to identify this. A Lambda specialist could design the exact concurrency strategy better."

---

## 💳 BULLET 10: Doximity Payment Flow

**Resume line:** *"Debugged a broken Doximity automated payment flow forcing untracked manual payments — fixed root cause in payment logic."*

### Q: "What broke?"
**Answer:**
> "Doximity vendor payouts were supposed to auto-trigger after survey completion. Flow broke silently — survey complete → payment event not firing. Team was doing manual payments (risky, untracked).
> 
> I traced the event chain — found a missing condition check in payment controller. Fixed the logic, redeployed, payments resumed auto. Recovered untracked payments via audit log."

### Q: "How are payments idempotent?"
**Answer:** "Each payment request has a unique idempotency key. If same key seen again, we return the earlier result instead of re-executing. Prevents double-payment on retries."

---

## 💰 BULLET 11: PayPal Redemption Failures

**Resume line:** *"Resolved multiple PayPal redemption failures — traced orphaned credit adjustments causing negative balance, fixed credit_status filtering bug."*

### Q: "What's an 'orphaned credit adjustment'?"
**Answer:**
> "A credit adjustment record that has no matching parent transaction. Some old records had their parent transactions deleted/expired, but the adjustments remained. Our balance calculation summed all adjustments, including orphans — producing negative balances.
> 
> Fix: filter by `credit_status = 'ACTIVE'` in the payment controller to exclude orphaned/voided records. Retroactively cleaned up data."

---

## 🏗️ BULLET 12: 500+ Production Incidents

**Resume line:** *"Owned 500+ production incidents across Java, AWS, MySQL, MongoDB (92% closure, 14 Critical/Blocker, 110 Major) — log correlation, code-level analysis, fixes in code and schema, mentored junior, led PR reviews."*

### Q: "What's your typical incident response playbook?"

**Answer (structure this):**
> **1. Alert received** — check severity, scope
> **2. Logs first** — CloudWatch/Papertrail/New Relic — identify error pattern, timeline
> **3. Reproduce if possible** — in staging with similar data
> **4. Isolate** — is it code, DB, infra, 3rd-party?
> **5. Root cause** — not just symptom — WHY did this happen?
> **6. Fix** — code patch, schema change, config update — with tests
> **7. Verify in prod** — monitor for regression
> **8. Post-mortem** — document, prevention plan

### Q: "What's the hardest incident you resolved?"
**Pick one:** HikariCP pool exhaustion OR AWS Lambda concurrency — both strong.

### Q: "How do you mentor juniors?"
**Answer:** "Pair debugging — share my screen, think aloud through logs, let them drive next time. For PR reviews — focus on patterns (not nitpicks), ask 'what does this break?', teach them to ask the same."

---

# 3. BUZZWORD MUH-PE

**⏰ Read this 30 min before any interview. Buzzwords should roll off tongue, not be memorized exactly.**

## Java / Spring Boot

| Buzzword | 1-line definition | When to drop |
|----------|-------------------|--------------|
| **HikariCP** | Spring Boot's default Java connection pool library — manages reusable DB connections | DB performance, connection issues |
| **Connection Pool** | Reusable bucket of DB connections — avoids expensive TCP handshake per query | Any DB discussion |
| **maxPoolSize** | HikariCP default 10 connections | Deep DB questions |
| **connectionTimeout** | HikariCP default 30s — max wait for a connection | Pool exhaustion stories |
| **@Transactional** | Spring's declarative transaction — auto commit on success, rollback on exception | Transaction questions |
| **@Cacheable** | Spring annotation — method result cached in Redis/in-memory | Caching questions |
| **@Autowired** | Spring Dependency Injection | Basic Spring |
| **Spring Security Filter Chain** | Ordered list of filters — CORS → JWT Auth → Authorization | Security questions |
| **JPA / Hibernate** | Java Persistence API standard / its implementation | ORM questions |
| **Lazy vs Eager loading** | Lazy = fetch on access, Eager = fetch immediately; many = lazy, one = eager | ORM deep-dive |
| **N+1 problem** | 1 query for parents + N queries for each child = N+1 = slow; fix with JOIN FETCH | ORM optimization |
| **L1 / L2 Cache** | L1 = session-scoped (always on), L2 = application-scoped (opt-in) | Hibernate caching |

## REST / APIs

| Buzzword | Definition | When to drop |
|----------|-----------|--------------|
| **Idempotency** | Same request N times = same result | Payment, PUT/DELETE |
| **Statelessness** | Server doesn't store session — each request self-contained | REST, JWT |
| **Rate Limiting** | Cap requests per client per time window | High-traffic APIs |
| **HATEOAS** | REST responses include navigation links (most don't implement fully) | REST purist question |
| **CORS** | Cross-Origin Resource Sharing — browser security | Frontend integration |
| **REST vs gRPC** | REST = HTTP + JSON, human-readable; gRPC = Protobuf + HTTP/2, fast, strict schema | Architecture |

## Security / JWT

| Buzzword | Definition | When to drop |
|----------|-----------|--------------|
| **JWT** | JSON Web Token — signed stateless token (header.payload.signature) | Auth flows |
| **HMAC-SHA256** | JWT signing algorithm using shared secret | JWT internals |
| **BCrypt** | Slow password hashing with salt — intentionally slow to resist brute force | Password storage |
| **Salt** | Random bytes added to password before hashing — prevents rainbow table attacks | Hashing |
| **OAuth 2.0** | Authorization framework — access tokens, scopes, grants | Third-party auth |
| **CSRF** | Cross-Site Request Forgery — disabled for JWT (header-based, not cookie) | Security config |
| **401 vs 403** | 401 = not authenticated (kaun ho?), 403 = authenticated but no permission (access nahi) | Status codes |

## Database

| Buzzword | Definition | When to drop |
|----------|-----------|--------------|
| **ACID** | Atomicity, Consistency, Isolation, Durability — transaction guarantees | Transactions |
| **Index** | B-tree data structure on columns — fast lookup O(log n) vs scan O(n) | Query optimization |
| **Composite Index** | Index on multiple columns — order matters, leftmost prefix rule | Deep DB |
| **EXPLAIN** | MySQL query plan output — shows if index used | Query debugging |
| **Deadlock** | Two transactions waiting for each other's locks | Concurrency |
| **Isolation Levels** | READ_UNCOMMITTED, READ_COMMITTED (default), REPEATABLE_READ, SERIALIZABLE | Transactions deep |
| **Sharding** | Split large table across multiple DBs by key (e.g., region) | Scaling |
| **Read Replica** | Secondary DB syncs from master, handles read traffic | Scaling |

## Caching / Redis

| Buzzword | Definition | When to drop |
|----------|-----------|--------------|
| **Redis** | In-memory key-value store — blazing fast, supports TTL | Caching |
| **TTL** | Time-to-live — cache entry auto-expires after N seconds | Cache config |
| **Cache Aside** | App checks cache, misses → load from DB + populate cache | Cache pattern |
| **Write-Through** | Every write updates cache AND DB together | Cache pattern |
| **LRU** | Least Recently Used — eviction policy | Cache internals |

## AWS / DevOps

| Buzzword | Definition | When to drop |
|----------|-----------|--------------|
| **Lambda** | AWS serverless compute — pay per execution, auto-scale | Serverless |
| **API Gateway** | AWS managed API fronting — routing, auth, rate limit | Cloud architecture |
| **CloudWatch** | AWS logging + metrics + alarms | Observability |
| **CloudTrail** | AWS account activity audit log | Security / audit |
| **RDS** | AWS managed relational DB (MySQL, PostgreSQL, etc.) | Cloud DB |
| **S3** | AWS object storage | Files, backups |
| **Auto-scaling** | Instances scale based on CPU/memory/custom metrics | Scaling |
| **Blue-Green** | Two environments, switch traffic — zero downtime deploys | Deployment |

## Containers / Docker

| Buzzword | Definition | When to drop |
|----------|-----------|--------------|
| **Docker** | Container runtime — package app + deps + config | Deployment |
| **Dockerfile** | Recipe to build image | Build process |
| **docker-compose** | Run multiple containers (app + DB + cache) together | Local dev |
| **Kubernetes** | Container orchestrator — deploys, scales, heals pods | Production scale |
| **Pod** | Smallest K8s unit — usually one container | K8s basics |
| **Service** | K8s networking abstraction — stable endpoint for pods | K8s networking |

## Microservices

| Buzzword | Definition | When to drop |
|----------|-----------|--------------|
| **Microservice** | Small independent service, own DB, communicates via API/events | Architecture |
| **Circuit Breaker** | Stops calls to failing service — like an electrical fuse | Resilience |
| **Service Discovery** | How services find each other (Eureka, Consul) | Microservices |
| **Event-Driven** | Services communicate via events (Kafka, RabbitMQ) | Decoupling |
| **Saga Pattern** | Long-running distributed transaction via compensating actions | Data consistency |

## Observability

| Buzzword | Definition | When to drop |
|----------|-----------|--------------|
| **APM** | Application Performance Monitoring (New Relic, Datadog) | Performance |
| **Distributed Tracing** | Trace a request across multiple services (e.g., Zipkin) | Microservices debug |
| **Structured Logging** | Logs in JSON with fields — queryable | Logging |
| **Correlation ID** | Unique ID per request — track across services | Debugging |

---

# 4. BEHAVIORAL STAR STORIES

## Story 1: The HikariCP Production Incident (⭐ Strongest Technical)

```
  ┌──────────────────────────────────────────────────────────┐
  │ S: 959 out of 1009 emails dropped. Critical production.   │
  │                                                           │
  │ T: Root cause and restore service.                        │
  │                                                           │
  │ A: → HikariCP logs: 'connection timeout after 30s'        │
  │    → Pool metrics: active=10/10, waiting=50+              │
  │    → Connections HELD — went to DB                        │
  │    → RDS top waits: blocking UPDATE from external tool    │
  │    → External tool locked tables 15 min                   │
  │    → Our app connections stuck waiting for locks          │
  │    → Pool exhausted → new requests timed out → emails    │
  │      fail                                                 │
  │                                                           │
  │ R: Restarted affected pods, UPDATE finished, service     │
  │    restored. Suggested: separate reporting DB + query     │
  │    timeout on batch operations. Recognized by VP/CTO.     │
  └──────────────────────────────────────────────────────────┘
```

## Story 2: AWS API Gateway X-Forwarded-For

```
  ┌──────────────────────────────────────────────────────────┐
  │ S: 300+ vendor integrations rate-limited incorrectly.     │
  │    Team dismissed as vendor issue.                        │
  │                                                           │
  │ T: Prove it's our bug, find and fix.                      │
  │                                                           │
  │ A: → Captured raw headers from API Gateway logs           │
  │    → X-Forwarded-For had 'vendor_IP, gateway_IP'          │
  │    → Our parser took LAST (gateway_IP)                    │
  │    → All vendors appeared as same IP → rate limit hit     │
  │    → Fix: take FIRST (leftmost) IP — standard per RFC     │
  │                                                           │
  │ R: 300+ integrations restored. Recognized by VP and CTO. │
  └──────────────────────────────────────────────────────────┘
```

## Story 3: AWS Lambda Concurrency

```
  ┌──────────────────────────────────────────────────────────┐
  │ S: 1800+ throttles, 8x page load spike. Users angry.     │
  │                                                           │
  │ T: Diagnose + fix fast.                                   │
  │                                                           │
  │ A: → CloudWatch metrics: throttle spike at 14:23          │
  │    → CloudTrail: config change at 14:20 — reserved        │
  │      concurrency reduced in deploy                        │
  │    → New Relic APM: page load spike matches throttles     │
  │    → Root cause: concurrency too low for traffic          │
  │                                                           │
  │ R: Increased reserved concurrency, implemented auto-scale │
  │    policy. Recognized by VP and CTO.                      │
  └──────────────────────────────────────────────────────────┘
```

## Story 4: Vendor Portal End-to-End (Ownership)

```
  ┌──────────────────────────────────────────────────────────┐
  │ S: Need to onboard Doximity, M3, and other healthcare     │
  │    networks for survey distribution.                       │
  │                                                           │
  │ T: Build vendor-facing portal from scratch — REST APIs,   │
  │    integrations, data reconciliation.                     │
  │                                                           │
  │ A: → Designed REST endpoints (vendor handshake, survey    │
  │      delivery, completion events)                         │
  │    → MySQL data model for vendor metadata, response data  │
  │    → Integration pipelines for vendor API push/pull       │
  │    → Retry logic, error handling, observability           │
  │                                                           │
  │ R: Multiple vendors integrated, thousands of surveys      │
  │    delivered, automated reconciliation replacing manual.  │
  └──────────────────────────────────────────────────────────┘
```

## Story 5: Mentoring + PR Reviews

```
  ┌──────────────────────────────────────────────────────────┐
  │ S: Junior engineer joined team, struggling with prod      │
  │    debugging and code quality.                            │
  │                                                           │
  │ T: Ramp them up — own their growth.                       │
  │                                                           │
  │ A: → Pair-debugging sessions — shared screen, think aloud │
  │    → PR reviews focused on patterns (not nitpicks)        │
  │    → Encouraged 'why does this work?' questions           │
  │    → Rotated incident handling — they drove, I backed     │
  │                                                           │
  │ R: Independent incident handler within 3 months,          │
  │    contributing meaningful PRs.                           │
  └──────────────────────────────────────────────────────────┘
```

---

# 5. HONEST DEFENSE PATTERNS

**When you don't know something — USE THESE. Don't bluff.**

## Pattern 1: Honest "I don't know" + "but I'd approach it..."

> *"Honestly I haven't worked with X directly. My approach if I hit this problem — I'd [step 1], then [step 2], then [step 3]. And I'd consult docs or a specialist if I got stuck on [specific part]."*

**Why this works:** Interviewer sees reasoning, not memorization.

## Pattern 2: "I did X specifically — Y generally I'd defer to..."

> *"For this incident specifically, I [did X]. The broader question of how to design Y across an entire system — that's more architect territory. I'd collaborate with our platform team on that."*

**Why this works:** Shows you know your scope.

## Pattern 3: "Not sure if this is still current, but..."

> *"Last time I touched this was during [context]. Not sure if this is still the current best practice, but my understanding was [explanation]."*

**Why this works:** Honest about recency, still demonstrates thinking.

## Pattern 4: "Let me think aloud..."

> *"Let me think through this. If I had [component X] that needed [property Y], first constraint is [A]... then I'd need [B]... actually, does [edge case] matter here?"*

**Why this works:** They want to see your thought process, not a perfect answer.

## Pattern 5: The "Correct Me If Wrong" opener

> *"Let me try — correct me if I go wrong. HikariCP manages connections, right? So pool exhaustion would mean..."*

**Why this works:** Invites collaboration, shows humility.

## ⚠️ What NOT to say:

- ❌ Silent long pauses (looks frozen — pause but talk through it)
- ❌ "I have no idea" (hard stop)
- ❌ Making up incorrect answers (bluffing = fatal)
- ❌ "My team handled that" (looks like you don't own)
- ❌ Perfect-sounding memorized answers (sounds rehearsed)

## ✅ What TO say:

- ✅ "Hm let me think..." (give yourself time)
- ✅ "I don't know the exact term but the concept I'd use is..."
- ✅ "Actually I'm not sure — what I DO know is [related thing]"
- ✅ "That's outside my direct work — can I get context on what scale you're asking about?"

---

# 6. SYSTEM DESIGN BUZZWORDS

```
  ┌──────────────────────────────────────────────────────────┐
  │              SYSTEM DESIGN KA TOOLBOX                    │
  │                                                          │
  │  🚦 Load Balancer    — Traffic distribute karta          │
  │  ⚡ Cache (Redis)    — Fridge (fast) vs Market (DB slow) │
  │  📖 Read Replicas    — Master writes, replicas read      │
  │  📢 Message Queue    — Loudspeaker (Kafka) broadcast     │
  │  🔀 API Gateway      — Building security gate            │
  │  📦 Sharding         — Library sections (A-M rack 1)     │
  │  ↔️ Horizontal Scale — Team badhao (add servers)         │
  │  ↕️ Vertical Scale   — Same server bada karo (limit!)    │
  │  🔄 Circuit Breaker  — Fuse (overload band, reset hoga) │
  │  🎯 Rate Limiting    — "Ek din 100 requests bas"         │
  │  🔑 Idempotency      — 2 baar button dabao = 1 result    │
  │  ⚖️ CAP Theorem      — 3 mein se 2 chuno (C, A, P)       │
  │  📡 Event-Driven     — Services talk via events (Kafka)  │
  │  🗄️ Eventual Consistency — Sab sync hoga, time lagega   │
  └──────────────────────────────────────────────────────────┘
```

---

# 7. WHY HIRE YOU

```
  ┌────────────────────────────────────────────────────────┐
  │                  WHY ARPAN?                             │
  │                                                         │
  │  1. 🔧 PRODUCTION OWNERSHIP                            │
  │     500+ incidents, 92% closure. I own what I build.   │
  │                                                         │
  │  2. 🏗️ END-TO-END BUILDER                              │
  │     Vendor portal, integrations, APIs, data models —   │
  │     full stack (code + DB).                            │
  │                                                         │
  │  3. 🏆 RECOGNIZED IMPACT                               │
  │     VP + CTO 3x for finding bugs others missed.        │
  │                                                         │
  │  4. 📚 LEARN-TO-SOLVE MINDSET                          │
  │     1000+ LeetCode streak. Didn't know HikariCP before │
  │     the incident — learned on the job, fixed it.       │
  │                                                         │
  │  = Hands-on + Ownership + Growth mindset               │
  └────────────────────────────────────────────────────────┘
```

---

# 8. QUESTIONS TO ASK

```
  Ye poochega toh interviewer impressed:

  1. "On-call rotation kaise hai?"        → Production mindset
  2. "CI/CD pipeline kya use karte?"      → DevOps aware
  3. "Recent technical challenge?"         → Genuine interest  
  4. "Tech debt vs features balance?"      → Engineering maturity
  5. "First 90 days kaise dikhte?"         → Planning mindset
  6. "Incident post-mortem culture?"       → Quality focus
  7. "How do you decide tech stack?"       → Architecture thinking
```

---

# 9. COMMON TRAPS

```
  ✗ GALAT                          ✓ SAHI
  ┌────────────────────────┐      ┌──────────────────────────────┐
  │"I know everything"     │  →   │"Strong hands-on + always     │
  │                        │      │ learning"                    │
  ├────────────────────────┤      ├──────────────────────────────┤
  │"Company bad hai"       │  →   │"Looking for bigger scale     │
  │                        │      │ challenges and growth"       │
  ├────────────────────────┤      ├──────────────────────────────┤
  │"I don't know X"        │  →   │"Haven't used X in production │
  │                        │      │ but I understand the concept │
  │                        │      │ — my approach would be..."   │
  ├────────────────────────┤      ├──────────────────────────────┤
  │ *silence*              │  →   │"Let me think aloud..."       │
  ├────────────────────────┤      ├──────────────────────────────┤
  │"My team handled that"  │  →   │"I contributed on X side, the │
  │                        │      │ infra side was team Y"       │
  ├────────────────────────┤      ├──────────────────────────────┤
  │ Memorized perfect      │  →   │"Honestly that was rough —    │
  │ answer                 │      │ here's what I actually did"  │
  └────────────────────────┘      └──────────────────────────────┘
```

---

# 🎯 30-MIN PRE-INTERVIEW CHECKLIST

```
15 min: Skim Section 3 (Buzzwords MUH-PE)
10 min: Skim Section 2 (Resume Bullets Q&A) — focus on HikariCP + Vendor Portal
 5 min: Glance Section 5 (Honest Defense Patterns)
```

---

*Interview Guide v2 — by Arpan Maheshwari*
*"Knowledge hai, experience hai, stories hain. Ab confidently + authentically bol. Tu ready hai."*
*"Don't rata maaro. Samjho. Muh pe buzzword aaega natural."*
