# Interview Prep — Progress Tracker

> **Last updated:** 2026-05-23 (Day 20 — TODO App deployed on AWS EC2 + production lesson learned)
> **Bangalore arrival:** June 15, 2026 (~23 days)
> **Primary target:** JP Morgan (Associate, Bangalore)
> **Open to:** Any company for interview experience
> **JP-ready:** 87%+ (project pipeline 100% complete, HLD followup still pending)

---

# DREAM — DAILY MANIFEST

```
        BLACKROCK ALADDIN 
        ═══════════════════════
        Java Backend Engineer
        Mera ASLI sapna
        Mera ASLI destination

        "Mile to acha. Na mile to aur bhi acha.
         Karm karo, phal ki chinta mat karo."
```

---

# PRIMARY TARGET — JP MORGAN

```
WHY JP MORGAN:
   Easier crack than GS (3-4 rounds vs 5-6)
   DSA = LC easy-medium (not hard)
   Java/Spring/HLD = matches tera prep
   Konovo fraud-detection = natural banking fit
   idempotency + audit trail = your terms already
   Same destination — BlackRock trajectory
   ~₹26-33L expected (2-3x Konovo jump)
```

```
JP MORGAN — REAL INTERVIEW PATTERN (researched 18 May):

ROUNDS (3-4):
   1. HackerRank OA — 90 min (aptitude + 2 easy-med DSA)
   2. Coding/Tech (VP-led) — 50-60 min (2 LC-mediums)
   3. System Design + Java — 50-60 min (LLD/HLD + internals)
   4. Behavioral (ED-led) — 45-60 min ← REAL GATE

QUESTIONS PATTERN:
   • DSA: Combination Sum II, recursion, Streams transformations
   • System: Trading platform, real-time price feed, ledger
   • Java: Java 8/11/17, Runnable vs Callable, Spring autowiring
   • Behavioral: "Why JPM banking", conflict mgmt, STAR stories

INSIDER TIPS:
   Drop "idempotency" + "audit trail" in system design
   Prefer PostgreSQL/Oracle over NoSQL for ledgers
   ACID > eventual consistency mastery
   Behavioral = REAL gate (ED gates it)

EXPECTED COMP (4yr Associate Bangalore):
   Base: ₹18-22L
   Total: ₹26-33L median
   Top: ₹47L (Senior Associate)
```

---

# PATH (opportunistic)

```
                                              
                                     ┌─────────────────┐
                                     │  BLACKROCK      │  DREAM
                                     │  Aladdin         │     3-5 saal
                                     │  Java Backend    │     50+ LPA
                                     └────────▲────────┘
                                              │
                                     ┌────────┴────────┐
                                     │  JP MORGAN    │  PRIMARY (28 din)
                                     │  Associate       │  ₹26-33L
                                     │  Bangalore       │
                                     └────────▲────────┘
                                              │
                                     ┌────────┴────────┐
                                     │  Open Parallel   │  EXPERIENCE
                                     │  Razorpay/Cred/  │  ₹18-25L
                                     │  Swiggy/Anywhere │
                                     └────────▲────────┘
                                              │
                                     ┌────────┴────────┐
                                     │  KONOVO          │  CURRENT
                                     │  12 LPA          │  Muzaffarnagar
                                     │  Java Backend    │  4+ yrs
                                     └─────────────────┘
                                              ↑
                                            HUM YAHAN
                                            (18 May 2026)
```

```
REFUSED:
   TCS, Infosys, Wipro, Cognizant, Capgemini, KPMG
   "TCS mein marna pasand karunga"
```

---

# DONE — FOUNDATION (Day 15 night)

## JAVA
- 22 Core Java topics (V90 deep)
- 11 Collections topics
- 5 Java 8 topics
- 10 Multithreading topics
- 13 Brain Tickler interview classics
- 20 Quick Java Q&A traps
- SOLID (5 principles split)
- Design Patterns (Builder, Singleton, Factory)
- 25 visual revision files
- SimpleBankSystem runnable project + revision

## SPRING
- Spring basics, REST, @Transactional, JWT theory, Profiles
- 7 Hibernate Gotcha files
- UserCRUD runnable project + revision
- 5 visual revision files

## LLD (12 problems)

## HLD
- 10 Foundations (CAP, sharding, replication, etc.)
- 4 System Designs (URL/Rate/Twitter/Notification)
- 4 Infra Deep (CDN/LB/DNS/WAF)
- 12 Rapid-Fire Compares

## DOCKER + K8S (fully wrapped + practical)

## INTERVIEW RESOURCES
- before_interview_ritual.md (game-day prep)

## KONOVO PRODUCTION EVIDENCE
- AS6079 RCN cluster fraud pattern
- *2026@outlook.com bot fingerprint
- Risk-tiered IP assessment
- WAF/CloudFront diagnosis (May 17)
- "Mere ander dum hai" validation

## TODO APP — Day 1 + Day 2 DONE (May 18-19)

### Day 1 BASE (May 18)
- Spring Boot 4.0.6 + Java 21
- 8 dependencies (Web/JPA/MySQL/Security/Validation/Lombok/Redis/DevTools)
- application.properties (MySQL + auto-create DB)
- model/Todo + User
- repository/TodoRepository + UserRepository
- service/TodoService + UserService
- controller/TodoController + UserController
- App boots, DB + tables auto-create, GET /todos = 200 OK

### Day 2 JWT AUTH LAYER (May 19) 
- pom.xml: jjwt-api/impl/jackson 0.12.6
- security/JwtService.java (6 methods)
- security/CustomUserDetailsService.java (Spring bridge)
- security/JwtFilter.java (OncePerRequestFilter — request scanner)
- security/SecurityConfig.java (3 beans: FilterChain/PasswordEncoder/AuthManager)
- controller/AuthController.java (register/login)
- dto/LoginRequest, RegisterRequest, TokenResponse
- service/UserService.java updated (BCrypt password hashing)
- application.properties: jwt.secret + jwt.expiration

### Verified END-TO-END via Postman:
- POST /auth/register → user saved with BCrypt hash ($2a$10$...)
- POST /auth/login → JWT issued (access + refresh)
- POST /todos with Bearer token → 200 OK
- DB tables confirmed in MySQL Workbench (users + todos)

### Architecture proven:
- HMAC-SHA256 signing
- Stateless session (no server-side session)
- CSRF disabled (JWT immune)
- /auth/** permitAll, anything else authenticated
- BCrypt password hashing (never plain text)

---

# DAY 15 NIGHT ADDITIONS (May 18)

## JWT THEORY DEEP-DIVE
- Hotel keycard analogy locked
- 3-part token structure (header.payload.signature)
- HMAC-SHA256 algorithm understanding
- Stateless auth flow
- "Code anyone can write, story behind code" thesis

## COGNITIVE PROFILE MAPPED
- Spatial-architectural dominant (system observation natural)
- Form/structure imagery: INSTANT, AUTO
- Sound/texture: ON-DEMAND
- Color: ABSENT (no color rendering — rare)
- Dual coding + schema-driven learning
- "File open = instant recall" PROVEN
- Rare engineer cognition (Jeff Dean / Vint Cerf tribe)

## 4 ACTIVE RECALL TESTS PASSED
- PriorityQueue (min-heap, top-K) — essence 
- Volatile (visibility, cache, whiteboard analogy) — essence 
- N+1 problem (JOIN FETCH) — essence (with file refresh)
- CAP theorem (3 letters, trade-off) — essence 

## PRINCIPLES LOCKED IN MEMORY.md
- YAGNI / Need-based addition (no forced "Day X" schedules)
- Modern engineering era ("code is commodity, story = value")
- Overload intervention (Claude must intervene on overload signals)
- Cognitive architecture detail (spatial dominance, no color)
- JP Morgan + Goldman Sachs interview data
- JP Morgan = primary target

## JP MORGAN RESEARCH (real interview data)
- 3-4 rounds, LC easy-medium DSA, ₹26-33L
- Behavioral = REAL gate (ED-led)
- Financial domain bias in system design
- Konovo fraud-detection = natural fit

---

# CONTINUOUS

```
DSA Phase 1 INTUITION
   Pace: 1 pattern/week
   Target June 15: 8-9 array/string patterns
```

---

# JP MORGAN READINESS — 87%+ (Day 20)

```
TECH FOUNDATION:    ████████████ 100%
SYSTEM DESIGN:      ███████░░░░░  60% (diagrams done, follow-up Q&A gap)
JAVA INTERNALS:     ████████████ 100%
SPRING:             ████████████ 100%
HIBERNATE:          ████████████ 100%
INFRA + COMPARES:   ████████████ 100% (bonus)
TODO PROJECT:       ████████████ 100% (full pipeline CRUD → JWT → Redis → Docker → K8s → AWS)
DSA (LC easy-med):  ██████░░░░░░  50% (Phase 1 ongoing — separate track)
BEHAVIORAL POLISH:  █████░░░░░░░  40% (real interview = fix)
JP-SPECIFIC DRILL:  █████░░░░░░░  40% (banking narrative)
KONOVO EVIDENCE:    ████████████ 100% (gold)

OVERALL JP-READY:   ██████████░░  87%+ (project pipeline complete, HLD followup pending)
```

---

# PROJECT EXTENSION PATH (NEED-BASED)

```
TODO App = FULL PIPELINE DONE (Day 20 milestone)

Path complete:
   CRUD Base       (Day 15)
   JWT Auth        (Day 16)
   Redis caching   (Day 17)
   Docker stack    (Day 18)
   K8s local       (Day 19)
   AWS EC2 deploy  (Day 20) aaj

Bangalore tak (23 din): comfortable margin

AWS DAY 20 — KEY LEARNINGS:
   • Free tier reality: marketing taste, not production
   • EC2 t3.micro = 1 GB RAM + 2 vCPU BURSTABLE
   • Burstable CPU credits: idle accumulate, busy burn, 0 = throttled
   • SSH key Windows permissions (icacls fix)
   • Docker + Compose install on Amazon Linux 2023
   • Docker Hub push/pull for image distribution
   • Nano paste gotcha (auto-indent + YAML conflict)
   • CPU starvation diagnosis (HikariPool "thread starvation" log)
   • OOM vs CPU starvation distinction (free -h proves)
   • Production: t3.medium+ or RDS/ElastiCache offload
   • Budget alert + GPay autopay cancel (cost discipline)
   • Cleanup discipline: terminate, verify $0 billing

K8S DAY 19 — KEY LEARNINGS:
   • kind = lightweight local K8s (cluster inside Docker container)
   • kubectl: get pods/svc/nodes, apply, logs, describe, port-forward
   • Manifest types: Deployment, Service, PersistentVolumeClaim
   • Service types: ClusterIP (internal), NodePort (external)
   • imagePullPolicy: Never = use local image (kind load required first)
   • Service-name DNS within cluster (mysql, redis hostnames)
   • Race condition: app pod restart 1-2 times before MySQL ready
   • K8s auto-healing = pod crash → restart automatic
   • port-forward = test cluster services from localhost
   • Stack Overflow validated patterns = 95% boilerplate

HLD GAP DISCOVERED (Day 18, claude.ai cross-validation):
   • Diagrams banaye 4 systems ke = visual mastery
   • BUT follow-up question prep MISSING:
     - "Add rate limiting" — kahaan, kaise, trade-offs
     - "Scale to 100M users" — sharding, partitions
     - "Idempotency strategy" — event ID, dedup
     - "What if X fails" — circuit breaker, DLQ
     - "Trade-off articulation" — yeh chuna kyunki X
   • Real HLD readiness ~50% (not 100% as earlier inflated)
   • Better caught NOW than in interview
   • Fix in viraam phase (post-AWS)


DOCKER DAY 18 — KEY LEARNINGS:
   • Dockerfile (JRE-only base, COPY jar, ENTRYPOINT pattern)
   • docker-compose.yml — 3-service orchestration (app + mysql + redis)
   • Named volumes for stateful service data persistence
   • healthcheck + depends_on:service_healthy (race condition fix)
   • Service-name DNS in Docker network (mysql/redis as hostnames)
   • Stack-vs-container distinction (compose grouping, not nesting)
   • Trap: depends_on alone = start-order only, not ready-status
   • Stack Overflow validated same pattern = 90% boilerplate confirmed

REDIS DAY 17 — KEY LEARNINGS:
   • Cache-aside pattern (read fill, write invalidate)
   • RedisCacheManager with JSON serializer (5-min TTL)
   • @Cacheable / @CacheEvict / @Caching annotation usage
   • Hidden Windows native Redis = port conflict trap (debugged + removed)
   • Observability-first lesson learned (debug logs upfront, not after fail)
```

# DSA — SEPARATE TRACK

```
DSA = independent track from project work
Don't mix with this conversation flow
Tu decide kab DSA day

JP Morgan ke liye:
   • DSA LC easy-medium expected
   • Not hard (FAANG level nahi)
   • Phase 1 INTUITION ongoing
   • June 15 target: 8-9 patterns

Tu bole "chal DSA" — tab attack
Otherwise = continuous slow practice
```

---

# BANGALORE PHASE (15 June onwards)

```
APPLY MODE — PARALLEL:
   JP Morgan PRIMARY (referral = turant)
   Goldman Sachs (stretch — DSA hard prep needed)
   Razorpay/Cred/Swiggy parallel (any company for experience)
   Mock interviews jab man
   Articulation = real-interview phase (4-5 real = fix)
   
   = "Practice mein har koi 6 marta, asli match mein pata"
   = Cricket truth — real exposure = real validator
```

---

# DAILY PRINCIPLES

```
SUBAH    → New topic / deep work (peak focus)
SHAAM    → Visual revision (file open + active recall)
SHAAM    → Apply mode parallel (Bangalore-targeted)

ROZ      → Ek kadam aage
NEVER    → 100% perfection chase
ALWAYS   → Real interview = real validator
FOCUS    → Effort (control), Outcome (universe)
OVERLOAD → STOP, sleep, fresh mind tomorrow
```

---

# MINDSET ANCHORS

- **DEPTH > SPEED** — "kitne bhi din lage, koi dikkat nahi"
- **Comparison = Poison** — yesterday's self only competition
- **Pattern recognition** — 80% HLD = same flow, 20% = decisions
- **Visualization superpower** — diagrams + analogies = auto-image brain
- **Argue when wrong** — senior position ≠ correct
- **Real interview = real validator** — mocks limited
- **Bruce Lee** — *"ONE kick × 10,000 times > 10,000 kicks × ONE time"*
- **Effort + direction in control → universe writes outcomes**
- **Self-made > inherited** — apne dum pe
- **Prep never 100%** — 80% solid + real interview = validation
- **YAGNI** — features need-based, not plan-based
- **Modern era** — "Code is commodity, story behind code = value"
- **Overload = STOP** — wrong decisions in tired state

---

# POWER PHRASES — DAILY READ

> *"Goal wahi hai. Journey kismat decide karegi.
>    Roz padhna — phase nahi, habit.
>    Effort control mein, outcome universe ka."*

> *"Mile to acha. Na mile to aur bhi acha.
>    Karm karo, phal ki chinta mat karo."*

> *"Apne dum pe — kyunki khud ka naam apna hi banta hai."*

> *"Code koi bhi likh sakta hai, lekin uske piche ki kahani —
>    jisne samjha hoga, wahi bolega."*

> *"Practice mein har koi 6 marta hai. Asli match mein pata chalta hai."*

> *"Roz ek kadam. Aaj kal se behtar."*

> *"Dum hai bhai — Konovo WAF diagnosis = proof."*

---

# BANGALORE COUNTDOWN

```
   Aaj:      19 May 2026 (Tuesday)
   Target:   15 June 2026
   ────────────────────────────
   Days:     27 baki
```

---

# META

```
   Foundation = ROCK SOLID
   TODO App base = WORKING
   JP Morgan = PRIMARY TARGET (locked)
   80% JP-ready (28 din mein 95%+)
   DSA Phase 1 = continuous
   JWT implementation = kal subah
   Apply mode = 15 June onwards
   
   = "Effort 100% mera. Outcome universe ka."
   = "Apne dum pe — BlackRock tak."
```

---

# FINAL VISUAL — MERA PATH

```
┌────────────────────────────────────────────────────┐
│                                                      │
│    AAJ (18 May 2026)                                 │
│    Konovo, 12 LPA, PREP MODE                         │
│           │                                          │
│           │  28 din — TODO + revision + DSA          │
│           ▼                                          │
│    15 JUNE — Bangalore landing                       │
│           │                                          │
│           │  APPLY MODE — JP Morgan PRIMARY          │
│           ▼                                          │
│      ┌─────────────────────────────────┐            │
│      │  PARALLEL APPLY:                 │            │
│      │                                   │            │
│      │  JP MORGAN (26-33L)        │            │
│      │  Goldman Sachs (38-43L)       │            │
│      │  Razorpay/Cred/Swiggy (18-25L)│            │
│      │  Any company (experience)     │            │
│      │                                   │            │
│      │  JP referral = TURANT             │            │
│      │  Any offer = grab + grow          │            │
│      └─────────────────────────────────┘            │
│           │                                          │
│           ▼                                          │
│    BLACKROCK ALADDIN                            │
│    (3-5 saal horizon — kabhi bhi)                    │
│                                                      │
│              "Effort 100% mera                       │
│               Outcome universe ka"                   │
└────────────────────────────────────────────────────┘
```
