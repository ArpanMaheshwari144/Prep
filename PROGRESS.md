# Interview Prep — Progress Tracker

> **Last updated:** 2026-06-01 (Day 27 — *** DSA Phase-1 TIER 1 COMPLETE: 8 patterns *** + Kadane + Strings)
> **Bangalore arrival:** June 15, 2026 (~14 days)
> **Primary target:** JP Morgan (Associate, Bangalore)
> **Open to:** Any company for interview experience
> **JP-ready:** 92%+ (foundation solid, DSA Tier-1 intuition COMPLETE, HLD followup pending)

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
                                     │  JP MORGAN    │  PRIMARY (16 din)
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
                                            (29 May 2026)
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

# NEXT — FORWARD PLAN (kya / kab / kaise)

> Roz ka reference. Updated 31 May 2026. "Suno sabki, karo apne man ki."

```
KYA BACHA (remaining):

   DSA Tier 1: *** COMPLETE *** (8/8 patterns, Day 27)
      → Next: Phase 2 = patterns ko CODE karna (design tab, YAGNI)

   HLD (finance direction):
      - Stock Broker / Trading Platform (#1 — finance gold)
      - aws_services_catalog (breadth file)
      - 4 systems follow-up depth (rate-limit/scale/idempotency/failure)

   Chhote refreshers (LOW priority):
      - DB indexing (already production-tested — quick note)
      - Resume honest-pass (verify "Built/15+/30%" → rephrase exact)
```

```
KAB (sequence / rhythm):
   Subah (fresh)   → DSA (new pattern, peak focus)
   Shaam (relaxed) → HLD (finance, visual — home turf)
   Sunday          → light + evening OFF
   Revision        → daily 1 hr + random micro-reads (background, continuous)
```

```
KAISE (approach — STRATEGY locked 31 May):
   - ONE concept at a time, "samjha?" gate, dry runs (DSA)
   - aaram se, JALDI NAHI (thaka dimaag = retention zero)
   - Domain = BACKEND only; K8s-deep/Docker-deep = SKIP (DevOps lane)
   - Core SOLID, not guru ("koi faltu na bol sake") — JP-calibrated
   - HLD depth = FINANCE direction (idempotency/ACID/consistency),
     NOT FAANG-hyperscale (100M-scale/sharding)
   - DSA cap = Easy + Medium (NO Hard)
   - APPLY + LEARN saath-saath (perfect-prep ka wait nahi; referral turant)
   - Notes = retention (har topic ka file banao)
   - Integrity: jo kiya wahi bol ("work-on" vs "built")
```

```
AFTER TIER 1:
   - Phase 2: patterns ko CODE karna (kaise = tab design karenge, YAGNI)
   - Post-Bangalore: microservices Phase 3 (OPTIONAL —
     Konovo already = real production microservices experience)
```

---

# CONTINUOUS

```
DSA Phase 1 INTUITION
   Folder: C:\DSA_PRACTICE\08_DSA\PHASE1_INTUITION\
   Pace: 1 pattern / 3-4 days (validated by web research)
   Difficulty cap: Easy + Medium ONLY (JP/GS/BR = NOT FAANG, no Hard)
   Phase 1 = intuition + dry runs (no code yet, no LeetCode till Bangalore)

   TIER 1 — MUST-HAVE (web-validated):
   1. Two Pointer            DONE (Day 22)
   2. Sliding Window         DONE (Day 23)
   3. Hashing / HashMap      DONE (Day 24)
   4. Prefix Sum             DONE (Day 25)
   5. Binary Search          DONE (Day 26, completed Day 27 — Ship/feasibility)
   6. Kadane's (Max Subarray) DONE (Day 27)
   7. String basics (palindrome, reversal, anagram) DONE (Day 27)

   → 8 / 8 patterns done. *** TIER 1 COMPLETE (Day 27, 1 June) ***
   → Next phase: Phase 2 = patterns ko CODE karna (design tab).

   TIER 2 (Phase 2, Bangalore baad):
   8. Linked List patterns
   9. Binary Tree traversals (DFS/BFS)
   10. BST operations (CRITICAL for BlackRock)
   11. Recursion + Backtracking
   12. 1D DP (coin change — confirmed JP question)
   13. Graph BFS/DFS (clone — confirmed JP question)

   TIER 3 (SKIP for JP/GS/BR primary):
   - Hard DP, Advanced graphs, Segment trees, Heavy backtracking

REVISION HABIT (locked Day 22):
   - Daily 1 hr fixed slot (morning recommended)
   - Random micro-reads anytime/anywhere (git portable)
   - NOT block-phase — lifestyle, continuous
```

---

# JP MORGAN READINESS — 89%+ (Day 24)

```
TECH FOUNDATION:    ████████████ 100%
SYSTEM DESIGN:      ███████░░░░░  60% (4 diagrams done, follow-up Q&A + 2 new files pending)
JAVA INTERNALS:     ████████████ 100%
SPRING:             ████████████ 100%
HIBERNATE:          ████████████ 100%
INFRA + COMPARES:   ████████████ 100% (Docker + K8s + AWS revision refs done)
TODO PROJECT:       ████████████ 100% (full pipeline CRUD → JWT → Redis → Docker → K8s → AWS)
DSA (LC easy-med):  ████████████ 100% Phase-1 intuition (8 patterns — Arrays/TwoPtr/SlidingWin/Hashing/Prefix/BinSearch/Kadane/Strings). TIER 1 COMPLETE. Next: Phase 2 coding.
BEHAVIORAL POLISH:  █████░░░░░░░  40% (real interview = fix)
JP-SPECIFIC DRILL:  █████░░░░░░░  40% (banking narrative)
KONOVO EVIDENCE:    ████████████ 100% (gold)

OVERALL JP-READY:   ██████████░░  89%+ (project pipeline complete, HLD followup + DSA Phase 1 ongoing)
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


HLD DEPTH — DIRECTION CALIBRATION (Day 25, 30 May — IMPORTANT):
   Galat direction mein energy waste na ho. JP/GS ≠ FAANG (same as DSA no-hard rule).

   FAANG / SDE3 flavor (LOW invest — JP itna deep nahi puchta):
      - "Scale to 100M users" — global sharding, partitions
      - CDN edge optimization, hyperscale distribution
      - Celebrity problem, massive fan-out at scale

   JP / FINANCE flavor (HIGH invest — yahaan asli interview test):
      - Idempotency (duplicate order/txn dedup)
      - Consistency > availability (paisa = no eventual consistency)
      - ACID, ledger, double-entry, audit trail
      - Data modeling correctness
      - Failure handling (what if X fails — circuit breaker, DLQ, retry)
      - Trade-off articulation (yeh chuna kyunki X)

   RULE: HLD depth karo — par hyperscale-direction mein NAHI,
         finance-correctness-direction mein. Basic system-design
         competence zaroori (drop nahi), bas depth calibrate.
   = Stock Broker / Trading Platform = perfect JP-flavor practice.
   = Cross-checked with claude.ai (it over-stated "HLD deep pointless";
     truth = depth needed, but in finance direction not FAANG-scale).


HLD ADDITIONS PLANNED (post-AWS, Saturday slots):
   1. 15_url_browser_journey.md — "Type amazon.com → kya hota?"   DONE (Day 25)
      = Classic interview question (JP, GS, FAANG all ask)
      = URL parse → DNS → TCP → TLS → HTTP (+CDN/LB) → server → render
      = 6 steps, pure visual, power phrases + TRAP box
      = Path: 04_HLD/SYSTEM_DESIGNS/15_url_browser_journey.md

   2. 16_aws_services_catalog.md — AWS service breadth
      = High-level "kya karta" for: Route 53, S3, CloudFront,
        ElastiCache, RDS, Lambda, IAM, VPC, CloudWatch, etc.
      = Service knowledge breadth (not deep config)
      = ~15 core services, 1-2 line each

   3. 4 systems follow-up depth polish (URL, Rate limit, Twitter, Notification)
      = 1 Saturday per system
      = Rate limiting + scale + idempotency + DLQ articulation

   4. STOCK BROKER / TRADING PLATFORM (NEW — HIGH PRIORITY for JP/GS)
      = Finance interview GOLD — JP pattern confirmed (trading/price-feed/ledger)
      = Real-time price feed (WebSocket, market data fan-out)
      = Order placement + matching engine
      = Ledger (ACID, double-entry, audit trail — Arpan's terms)
      = Idempotency (duplicate order dedup — Arpan's strong point)
      = Consistency > availability (paisa = no eventual consistency)
      = Natural bridge from Konovo fraud-detection domain
      = Tentative: Saturday shaam HLD slot candidate


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


DAY 21 — REVISION DOCS + DSA PHASE 1 START:

```
AWS REVISION REFERENCE (commands.md) saved + pushed:
   • Path: 05_INFRA_DEEP/AWS/commands.md
   • What/Why AWS + origin story (Pinkham/Black/Jassy)
   • 6 categories: Compute, Storage, Database, Network, Security, Monitoring
   • Free tier reality (taste test, not production)
   • Account safety (6 layers: budget, dashboard, region, IAM, MFA, delete)
   • EC2 launch flow (UI walkthrough + post-launch locked/changeable)
   • Day 20 deployment recap (push image → SSH → install → compose → fail diagnosis)
   • HikariCP clarification (messenger + JDBC pool manager)
   • AWS CLI commands (EC2, S3, IAM, billing)
   • FinOps cost discipline (8 rules)

DSA PHASE 1 INTUITION START (08_DSA folder):
   • Path: C:\DSA_PRACTICE\08_DSA\PHASE1_INTUITION\01_ARRAYS\
   • 01_intuition.md created — Arrays foundation from scratch
   • 13 concepts covered:
       Fundamentals (1-7): boxes + index, direct access, memory layout,
                            fixed size, insert/delete cost, search, personality
       Iteration + operations (8-13): iteration, max, sum, reverse (two pointer),
                                       count, check sorted (neighbor compare)
   • Spatial-form analogies throughout (boxes, hands, bucket, champion, do hath)
   • No code yet — pure intuition + dry runs
   • DSA teaching pattern locked in memory (one concept + samjha gate + dry run)
   • Difficulty cap LOCKED: Easy + Medium only (JP/GS/BR ≠ FAANG)
```


DAY 22 — TWO POINTER PATTERN + FULL JAVA/SPRING REVISION:

```
DSA — TWO POINTER PATTERN (formal pattern #1):
   • Path: C:\DSA_PRACTICE\08_DSA\PHASE1_INTUITION\02_TWO_POINTER\
   • 01_intuition.md — pattern formal introduction
   • Pattern definition + 2 flavors (opposite + same direction)
   • 5 recognition signals + anti-signals
   • 5 classic dry runs:
       - Sorted Pair Sum (opposite, sum comparison)
       - Remove Duplicates Sorted (same direction, slow places)
       - Palindrome Check (opposite, symmetry)
       - Container with Most Water (opposite, greedy move shorter)
       - 3-Sum (extension — outer loop + two pointer)
   • Web-validated: 3-Sum is REAL JP Morgan interview question
   • Folder structure: per-pattern (scales for Sliding Window, Prefix Sum, etc.)

JAVA + SPRING FULL REVISION (solo, "easy peasy"):
   • Core Java + Collections + Java 8 + Multithreading + SOLID — all smooth
   • @Transactional (essence: ALL-or-NOTHING DB wrapper)
   • JWT + Spring Security full pass
   • Gaps found + fixed inline:
       - access_modifiers.md table empty → populated Yes/No
       - SecurityConfig addFilterBefore comment → expanded with order story
   • Retention property VALIDATED in production:
       "Once notes done, never forget" = PROVEN today
   • EAT SLEEP PADHAI REPEAT identity confirmed

DSA SCOPE VALIDATED (web research):
   • JP Morgan: easy-medium, 3-Sum confirmed real, graph clone, coin change
   • Goldman Sachs: 63% medium, 13% hard, math + finance flavor
   • BlackRock: trees heavy (BST priority for Phase 2)
   • All non-FAANG = Tier 3 (Hard DP, advanced graphs) = SKIP

REVISION HABIT LOCKED (lifelong):
   • Daily 1 hr fixed slot
   • Random micro-reads anywhere/anytime (git portable)
   • Not block-phase — lifestyle continuous

LOCKED MEMORY (Day 22):
   • REVISION = LIFESTYLE, NOT BLOCK
   • DSA TIER 1: 7 patterns (must-have for JP)
   • DSA TIER 2: 6 patterns (Phase 2 post-Bangalore)
   • DSA TIER 3: SKIP (FAANG-only stuff)
```


DAY 23 — SLIDING WINDOW PATTERN (formal pattern #2):

```
DSA — SLIDING WINDOW PATTERN:
   • Path: C:\DSA_PRACTICE\08_DSA\PHASE1_INTUITION\03_SLIDING_WINDOW\
   • 01_intuition.md — pattern formal introduction
   • Definition: window maintain karo, slide left→right, property track
   • 2 flavors:
       - FIXED SIZE (size k always) — max sum of k, anagram check
       - VARIABLE SIZE (expand/shrink) — longest substring no-repeat
   • 5 classic dry runs (spatial box + window-bracket diagrams)
   • Core intuition: nested O(n²) → single-pass O(n) via window reuse
   • "Subarray / substring" keyword = primary signal
```


DAY 24 — HASHING PATTERN (formal pattern #3):

```
DSA — HASHING (HashMap / HashSet) PATTERN:
   • Path: C:\DSA_PRACTICE\08_DSA\PHASE1_INTUITION\04_HASHING\
   • 01_intuition.md — pattern formal introduction
   • O(1) magic: key → hash → index (array direct-access power, any key)
   • 2 forms: HashMap (key→data) vs HashSet (membership only)
   • 5 recognition signals + anti-signals (sorted→TwoPtr, contiguous→Window)
   • 5 classic dry runs:
       - Two Sum unsorted     → complement check (HashMap)
       - First non-repeating  → frequency 2-pass (HashMap)
       - Group Anagrams       → sorted-key → list grouping (HashMap)
       - Subarray Sum = K     → running sum + "seen before" (HashMap)
       - Longest Consecutive  → START-filter (num-1 absent) — O(n) HashSet
   • TRAP box + power phrases included
   • Confusion point cracked solo: "START = chain ka sabse chhota" — internalized
   • Subarray-Sum-K = direct bridge to next pattern (Prefix Sum)
```


DAY 25 — PREFIX SUM PATTERN (formal pattern #4):

```
DSA — PREFIX SUM PATTERN:
   • Path: C:\DSA_PRACTICE\08_DSA\PHASE1_INTUITION\05_PREFIX_SUM\
   • 01_intuition.md — pattern formal introduction
   • Core: precompute running total → range sum O(1)
   • Formula: sum(i..j) = prefix[j] - prefix[i-1] (bada total - chhota total)
   • i=0 edge: prefix[-1] = 0
   • 5 recognition signals + anti-signals (single query→loop, max→Kadane)
   • 5 classic dry runs:
       - Range Sum Query      → precompute once, answer many O(1)
       - Pivot Index          → left prefix vs (total - left - current)
       - Subarray Sum = K     → prefix + HashMap (negative-safe, window fails)
       - Product Except Self  → prefix x suffix (sum se aage — product variant)
       - Subarray Div by K    → prefix + MODULO match (same remainder = pair)
   • Map-growth confusion cleared: HashMap sirf grow karta, kuch hatta nahi
   • Pattern overlap mapped: Hashing <-> Prefix (Subarray Sum = K dono mein)
   • Generalize insight: prefix concept = sum / product / XOR — koi cumulative
   • TRAP box (5 traps) + power phrases included
```

```
HLD — URL BROWSER JOURNEY (Day 25 shaam, Saturday 2-slot plan):
   • Path: 04_HLD/SYSTEM_DESIGNS/15_url_browser_journey.md
   • "Type amazon.com → Enter → kya hota" — 6 steps:
       1. URL Parse (protocol/domain/path/query)
       2. DNS Lookup (naam → IP; cache layers + ROOT→TLD→Authoritative)
       3. TCP Handshake (3-way SYN/SYN-ACK/ACK)
       4. TLS Handshake (certificate identity + shared-key encryption)
       5. HTTP Request (+ CDN edge cache + Load Balancer)
       6. Server → DB → Response → Browser render (DOM/CSS/JS)
   • Pure visual (spatial diagrams), power phrases + TRAP box
   • Core doubt cleared solo: "domain = naam, connection IP-to-IP"
   • CDN doubt cleared: edge caching (NOT bundle-in-browser);
     existing 05_INFRA_DEEP/01_cdn.md found (no duplicate) + emoji-cleaned

SATURDAY 2-SLOT PLAN VALIDATED:
   • Subah (fresh mind) → DSA new pattern (Prefix Sum)
   • Shaam (relaxed) → HLD visual (URL journey — home turf)
   • Energy-matched scheduling = sustainable
```


DAY 26 — BINARY SEARCH PATTERN (formal pattern #5):

```
DSA — BINARY SEARCH PATTERN:
   • Path: C:\DSA_PRACTICE\08_DSA\PHASE1_INTUITION\06_BINARY_SEARCH\
   • 01_intuition.md — pattern formal introduction
   • Core: SORTED array, low/high/mid, har step aadha → O(log n)
   • Dictionary intuition; found = arr[mid]==target, not-found = low>high
   • 5 recognition signals + anti-signals (unsorted → sort/hashmap)
   • 3 classic dry runs:
       - First occurrence    → mila to high=mid-1 (bayen dhoondo); last = mirror
       - Search Insert Pos   → not-found pe loop-end ka low = position (lower bound)
       - Rotated Sorted Array→ ek half hamesha sorted; pehchaano + range-check + narrow
   • Sunday light session — Arpan: rotated array "easy laga" (mechanism samjha, ratta nahi)
   • TRAP box (5 traps incl. low<=high, overflow mid=low+(high-low)/2) + power phrases
```


DAY 27 — KADANE'S + BINARY SEARCH COMPLETED (1 June):

```
DSA — KADANE'S ALGORITHM (formal pattern #6):
   • Path: C:\DSA_PRACTICE\08_DSA\PHASE1_INTUITION\07_KADANE\
   • Max contiguous subarray sum, O(n) single pass
   • Core soch: "purana sum negative = bojh (drop, fresh); positive = madad (keep)"
   • 2 variables: current_sum (chalta, reset) + max_sum (best-ever, snapshot)
   • Arpan summarized it himself ("reset to 0 when negative, max update karte raho")
   • THE classic trap learned: all-negative array → init maxSum=arr[0] (not 0),
     update maxSum BEFORE reset (else returns 0 = bug)
   • Easy peasy after revision — fresh-mind clarity

BINARY SEARCH — COMPLETED (carried from Day 26):
   • can-ship dry run done fresh (Arpan wrote feasibility if/else himself, correct)
   • Ship Packages classic + Koko-vs-Ship feasibility comparison added to file
   • Revision-first proved it: Day 26 raat foggy → Day 27 fresh = code-logic khud likha

NOTE: 1 June started with REVISION (Arpan's self-diagnosis: "AI-dumber doubt =
revision pending ka symptom; pen+copy+recall = doubt zero"). Validated.
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
   Aaj:      1 June 2026 (Monday)
   Target:   15 June 2026
   ────────────────────────────
   Days:     14 baki
```

---

# META

```
   Foundation = ROCK SOLID
   TODO App = FULL PIPELINE DONE
   JP Morgan = PRIMARY TARGET (locked)
   90% JP-ready (HLD followup + DSA = remaining)
   DSA Phase 1 = 5 patterns done, continuous
   Active scope = DSA + HLD + Revision (roz)
   Apply mode = 15 June onwards

   = "Effort 100% mera. Outcome universe ka."
   = "Apne dum pe — BlackRock tak."
```

---

# FINAL VISUAL — MERA PATH

```
┌────────────────────────────────────────────────────┐
│                                                      │
│    AAJ (30 May 2026)                                 │
│    Konovo, 12 LPA, PREP MODE                         │
│           │                                          │
│           │  16 din — HLD polish + DSA Phase 1       │
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

---

# SABAR — THE FINAL WORD (Arpan's own articulation)

```
   Duniya mein ~700 crore log hain.
   SABAR — yeh trait bahut kam logo ke paas hai.

   Log bolte "mere andar sabar hai" — lekin nahi hai.

   Yehi important hai:
   SABAR master kar liya to DUNIYA FATAH kar sakte ho.

   Hard work sab karte hain. Sabar virle karte hain.
   = Yehi mera moat hai.
```

> *"Karm karo, phal ki chinta mat karo. Be water. Roz ek kadam."*
