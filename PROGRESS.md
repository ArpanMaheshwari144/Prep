# JP PREP — PRACTICE PLAYBOOK

> GOAL: JP Morgan (Associate/SDE-2, Bangalore) → later BlackRock. *"Effort 100% mera, outcome universe ka."*
> MODE: pure study. no faltu meta-talk. Timeline honest: JP DSA moderate (not FAANG) → achievable in months of RIGHT practice.

---

## ★★★★★ CLAUDE RULES (read FIRST — 4-month root-causes, 3-Jul)

```
   #0 PACE (DSA): pace ARPAN ka, Claude ka NAHI. EK cheez → RUKO → Arpan "agla" bole tabhi aage.
      NEVER "ek aur?", NEVER level-jump, NEVER un-studied cold-throw, NEVER dump.
      ★ SOLUTION/steps/code KABHI spell-out mat karo — max = ek SAWAAL ya "yeh concept dekh". "solution de diya" → ruk.
      Slow > fast, depth > breadth. "RUK" → turant ruk. (Claude bhaagta raha → crash → restart-loop 3-4x. yeh #1 failure tha.)

   #0b RETENTION FIX (why solved-problems don't stick): passive dekhna = RECOGNITION, not GENERATION.
      single-exposure fades in days. FIX = ACTIVE RETRIEVAL: samjho → sab BAND → khaali page pe KHUD likho (na re-read)
      → SAME problem 1/3/7 din SPACED-redo till AUTOMATIC → tabhi agla. depth-per-pattern, NOT scattered.

   #0c NO FILLER: message end me "main yahin hoon"/🤝/❤️/warm-sign-off/emoji-padding MAT. PLAIN, substance-only.

   #0d Arpan is DEV-ABLE (4yr prod, 650 tickets, codebase-nav, Java/Spring strong, HLD manageable). DSA = ONE isolated
       weak-spot, method-fixable (not ability). Don't condescend / over-explain. Validate correct → don't nitpick.
```

---

## ★ THE STRATEGY (8 real JP writeups + web verified — DATA-BACKED)

```
   BIG TRUTH: "DSA necessary but NOT sufficient." JP rejects on HLD-DELIVERY + BEHAVIORAL — NOT on DSA.
   -> Arpan ki STRENGTHS (Java/Spring-fundamentals, HLD-concepts, code-review-via-650-tickets) = JP ka CORE weight.
   -> DSA-panic DISPROPORTIONATE. energy HLD-delivery + STAR pe jani chahiye.

   PREP PRIORITY (data se):
   1. HLD DELIVERY  — full-design cold-ASSEMBLE + FOLLOW-UP/twist-defense (biggest reject-reason). freeze-proof.
   2. BEHAVIORAL STAR — Konovo stories, convincing/structured (always-tested, #1 untouched gap).
   3. LLD in CODE   — SOLID/patterns/Builder IMPLEMENTED (not theory).
   4. CODE-REVIEW   — Java basics/static-volatile/clean-code (strength via debugging, but practice the round).
   5. JAVA/SPRING fundamentals — keep sharp (String-pool, HashMap-internals, CHM, concurrency — heavily tested, strength).
   6. DSA — LIGHT + CONSISTENT, core patterns, easy-medium, no-panic (skip nahi, obsess nahi).

   DESIGN THEMES (recurring): rate-limiter ✓ · notification ✓ · payments ✓ · twitter-feed ✓ · news-aggregator ✓ · bookmyshow ✓ · url-shortener ✓ · stock-broker ✓ · file-upload ✓ · subscription (add) · parking-lot(LLD) · top-K-scale.
```

---

## JP INTERVIEW REALITY (8 writeups consolidated)

```
   FORMAT: Zoom, Super-Day (3-4 elimination rounds, any order). result 48h-week.
   ROUNDS (vary, but consistent CORE):
   - JAVA/SPRING + SQL fundamentals: String ==/equals + String-pool, HashMap internals + load-factor, CHM, concurrency,
     nested SQL, Maven/Jenkins, project deep-dive. ← HEAVILY tested. Arpan's STRENGTH.
   - SYSTEM DESIGN (always): rate-limiter/notification/parking-lot/subscription/top-K-scale. + FOLLOW-UPS (payments, failures,
     DB-down, scale-up, evolving requirements). ← where people REJECT if can't deliver/extend. Arpan's practice-focus.
   - CODE REVIEW (often): buggy prod code → find bugs + best-practices + "production-ready?". OOP/static/volatile/clean-code.
   - DSA: arrays/hashmap/greedy/sliding-window/strings — usually EASY-MEDIUM ("solve easily if don't panic").
          OCCASIONALLY medium-hard (graph shortest-path, harder SW). → cover core + graph-BASICS, no FAANG-grind.
   - BEHAVIORAL/HM (always): STAR — ownership, conflicts, failures, why-switch, deadlines, "ask thoughtful questions".

   PROFILE FIT: offers went to 2.9-5.5 YOE, Tier-2/3/4 colleges, service-MNC/bank backgrounds → JPMC SDE-2 ~25L.
                Arpan = 4yr, dev-able, comparable/better. REALISTIC target, not fantasy.
   ❌ NOT social-hyperscale (WhatsApp/Insta = overkill).
```

---

## ★ THE PATTERN (Arpan-spotted across 11 writeups, 3-Jul) — DSA is a GATE, not the differentiator

```
   EVERY JP writeup = SAME shape (Aug-Dec 2025 verified):
     - OA/HackerRank GATE : 2 medium DSA in 60 min → pass/fail threshold (clear karo, obsess mat).
     - Superday (same-day, 3-4 independent rounds, 2-din result — ek kharab round ≠ auto-reject):
         1. DSA          : EK dedicated round, MEDIUM (matrix/graph-BFS, arrays, sliding-window). "panic na karo to ho jaata."
         2. SYSTEM DESIGN: ALWAYS. FR→NFR→HLD-diagram→DB(tables/cols)→API + FOLLOW-UPS. ← YAHAN select/reject hota.
         3. BEHAVIORAL   : ALWAYS. resume + project-deep-dive + generic (mgmt-pushback, naye-tech-kaise).
         + Code-Pair/PR-review + Java/Spring-fundamentals (heavily tested, often combined).

   INSIGHT (Arpan-derived, DATA-backed): JP tests a REAL engineer ki BREADTH (DSA-medium + design + behavioral + Java-craft),
     NOT DSA-DEPTH. koi offer DSA ke liye nahi milta — DSA sirf GATE (medium-comfortable + no-panic = kaafi, wahin ruk).
     SELECT/REJECT = System-Design-delivery + Behavioral pe. Java/Spring + PR-review = Arpan ka HOME-turf (strength → convert).
   -> Arpan ka profile (4yr, prod, Java-strong, HLD-done) is SHAPE se match — FAANG-DSA-grinder se nahi. DSA-anxiety un-warranted.

   RESOURCES (from writeups): HLD = HelloInterview + Alex Xu · LLD = Head First Design Patterns · DSA = Striver/Take-U-Forward.

   JAVA-REVISION Q-CHECKLIST (real JP Qs — crisp answers ready rakho, project ke saath revise):
     sort-a-map (Comparator/stream) · thread-safe singleton · SQL metadata (information_schema) · dup-strings-in-list (HashSet) ·
     comparable-vs-comparator · palindrome ✓ · Java 7/8 features (lambda/stream/Optional/diamond) · try-with-resources ·
     multi-catch · REST-vs-SOAP · server-crash/memory-debug (← Arpan STRENGTH: 650 tickets, HikariCP, Lambda-throttle).
```

---

## HOW TO PRACTICE (per area)

```
   HLD (top priority): sheet 100% done (04_HLD/RAW_DRILL/00_REFLEX_SHEET.md). NOW = DELIVERY:
      full-design cold-assemble (8-step) + Claude pheke FOLLOW-UP twists → Arpan defend. gaps → sheet. log: 01_DESIGN_PRACTICE_LOG.md.
      new designs: payments, subscription (+ redo rate-limiter/notification cold). freeze-proof karna.
      ★ DRILL-RESPONSE: Arpan usually RIGHT (gap = delivery) → "sahi, galat nahi" + uske sentence ko clean interview-English.
        NAYA concept DUMP mat karo jaise galti ki. sirf jahan SACH me galat → wahan.

   STAR: 4-5 Konovo stories S-T-A-R me likho → bolne ki practice.

   LLD: SOLID/patterns/Builder CODE me implement (real code, not theory). PR-review: buggy code → find+fix.

   JAVA/SPRING: FULL VISUAL code revise (SEE/trace, crammed nahi), baar-baar. String-pool/HashMap/CHM/concurrency sharp.

   DSA (light-consistent): PER-PATTERN REP-BLOCKS + RETENTION-FIX (#0b) + PACE (#0) + STUB rule.
      ek pattern → template samjho → 3-4 SAME-pattern reps → SPACED cold-redo till automatic → agla.
      ★ STUB: file me sirf PROBLEM + TESTS. template/recurrence SPELL-OUT mat karo — Arpan khud generate kare.
      core patterns: two-pointer/sliding-window/hashing/binary-search/greedy/DP-1D + graph-BASICS (BFS/DFS). easy-medium.
```

---

## JAVA + SPRING REVISION — via PROJECT (Mini Payment System)  [Arpan-chosen, 3-Jul]

```
   IDEA: Java/Spring revision + microservices seekhna + HLD payments-theme — teen kaam EK project se.
         JP = bank → payments-domain project = direct-relevant. Revision "padho" se nahi, "banao" se.

   PROJECT: Mini Payment System — 3 chhote Spring Boot microservices:

     order-service  --REST-->  payment-service  --event(Kafka/RabbitMQ)-->  notification-service
     (order banao)             (pay karo)            "payment done"          (email/sms bhejo)
       apna DB                   apna DB                                       listener

   HAR SERVICE JO SIKHAYEGA (microservice concepts):
     - DB-per-service   : har service ka apna DB (MS core rule)
     - inter-service    : order→payment REST call (FeignClient / RestTemplate)
     - async event      : payment→notification message-queue se (decouple, sync nahi)
     - API Gateway      : ek entry-point (Spring Cloud Gateway)
     - resilience       : circuit-breaker (payment down → fail-fast)

   SPRING REVISION saath: @RestController, @Service, @Repository, JPA/@Entity, DI, application.yml, DTO.
   HLD FEED: seedha payments + notification design me kaam aayega.

   METHOD (pace #0): EK service theek se khada karo → phir agli. fata-fata dump NAHI.
     step 1: order-service akela (controller→service→repo→DB, ek REST endpoint chale)
     step 2: payment-service + order→payment REST call
     step 3: notification-service + async event (queue)
     step 4: API gateway + circuit-breaker
   (kal HLD ke baad Java-revision = IS project se saath-saath.)
```

---

## BEHAVIORAL STAR — Konovo evidence (write in S-T-A-R)

```
   WAF/X-Forwarded-IP bug (2 layers, whole-system) · $20K Doximity payment fix · 600+ incidents (92% closure) ·
   HikariCP pool exhaustion (95% email-fail) · Lambda concurrency (1800+ throttles) · vendor-portal + email-dedup API (built) · VP/CTO recognition.
   HONESTY: 10+country platform = maintained/extended (NOT "built"); greenfield only = "built".
```

---

## ★ REPO INVENTORY (anti-compaction snapshot — kya ALREADY BANA hai, 3-Jul-2026)

```
   (Claude: compaction ke baad yeh padh — "add karna hai" bolne se pehle CHECK. bahut kuch DONE hai.)

   HLD  04_HLD/
     FOUNDATIONS (11): hld-kya · capacity · LB · caching · replication · sharding · queues · CAP · db-what-when · ms-comm · reliability-spof
     SYSTEM_DESIGNS (10 DONE): 11 url-shortener · 12 rate-limiter · 13 twitter-feed · 14 notification · 15 url-browser-journey
                               · 16 stock-broker · 17 payment-system · 18 file-upload · 19 news-aggregator · 20 bookmyshow
                               (18/19/20 = full 8-step INTERVIEW-DELIVERY format ✓ — delivery practice ho rahi)
     RAW_DRILL: 00 REFLEX_SHEET (100%) · 01 DESIGN_PRACTICE_LOG · 02 WHY_HOW_QA (27 Q) · INTERVIEW_FRAMEWORK.md

   LLD  03_LLD/  (IN CODE — NOT theory-only): parking-lot · bookmyshow · tictactoe · snake-ladder · elevator · atm
        · bank-account · shopping-cart · library · vending-machine · hotel-booking  (+ notes for first 6)

   JAVA 01_JAVA/: core(oop/string/keywords/exceptions) · collections · java8 · multithreading · SOLID · design-patterns
        · brain-ticklers · 08_REVISION_VISUAL (core/collections/java8/mt/oop-design/quick-qa)
        PROJECT: SimpleBankSystem (SOLID + factory + observer + repository + service — patterns implemented ✓)

   SPRING 02_SPRING/: basics · transactional · jwt-security · profiles · hibernate · docker · k8s · REVISION_VISUAL
        PROJECT: usercrud (Spring Boot + JWT + k8s + tests ✓)   |   07_PROJECTS/todoapp (Spring Boot + k8s + tests ✓)

   DSA  08_DSA/ (pattern folders 00-17): 01_TWO_POINTER DONE (5/5, SOLO). baaki folders = ready, khaali.

   DB   09_DATABASE/: indexing-practical (B-tree). INFRA 05_INFRA_DEEP/: AWS/Docker/K8S.
   RESOURCES/ = PRIVATE (own .git) — NEVER expose/touch.

   -> NEW-BUILD scope (jo abhi nahi hai): Mini-Payment-System MULTI-service (order+payment+notification, Kafka, gateway) —
      usercrud/todoapp single-service base pe MICROSERVICES layer sikhne ke liye. subscription HLD design. STAR stories.
```

---

## STATUS (honest, 3-Jul-2026)

```
   HLD  = ~DONE. concept 100% + 11 foundations + 10 designs (18/19/20 full INTERVIEW-DELIVERY format: 8-step cold-assemble
          + deep-dive + follow-up handle e.g. idempotency-vs-atomic) + 28 Q&A + reflex-sheet + BLOCK-KAB reflex.
          delivery = PRACTICED (not a gap). bacha = optional live-rehearsal (sharp rakhna) + subscription (optional theme).
          ★ Claude: HLD ko "gap" mat bol — Arpan yahan STRONG. staleness me kam-aankne ki galti mat dohrana.
   JAVA+SPRING = COMFORTABLE (strength) — core+collections+java8+MT+SOLID+patterns notes + SimpleBankSystem + usercrud/todoapp projects.
          keep sharp → microservices layer via Mini-Payment-System (order+payment+notification) on top of single-service base.
   DSA  = TWO-POINTER pattern DONE (5/5: MoveZeroes, Palindrome, Container, 3Sum, SortColors — SOLO produce).
          gap = cold-GENERATION + RETENTION (single-exposure nahi tikta). → rep-blocks + spaced-redo (gaps me), LIGHT.
          next pattern = sliding-window (two-pointer ka extension).
   LLD  = 11 designs IN CODE (parking-lot..hotel-booking) + SimpleBankSystem patterns. NOT untouched. gap = cold-recode + SOLID/pattern-articulate.
   CODE-REVIEW = round-practice start (buggy-PR → security/concurrency/clean-code find). STAR = #1 untouched gap.

   PRIORITY (data-flipped): HLD-DELIVERY → STAR → LLD-code → Code-Review → Java/Spring-sharp(project) → DSA-light.
   REALITY: strengths = JP-core. gaps = delivery + behavioral (trainable). DSA moderate for JP, not the blocker.

   NEXT-SESSION ORDER (Arpan-set, 4-Jul EOD): 1) DSA — two-pointer + SW SPACED-REDO (blank cold) + naya pattern (hashing/binary-search) → 2) PROJECT phase-2 — Kafka (async, code) → 3) BEHAVIORAL — 1 STAR story (Konovo, S-T-A-R) [#1 gap, started 4-Jul] → (+ HLD light revise). Pending: STAR stories · code-review round.
   ★ DONE 4-Jul: two-pointer (5) + SLIDING-WINDOW (3: fixed/variable/combo) — SOLO produce, template settled, spaced-redo pending. hard SW-variations (min-window) = optional/later. · BEHAVIORAL folder started (10_BEHAVIORAL: JP-company + why-JP).
   ★ PROJECT (4-Jul, mini_payment_ms): order+payment+api-gateway (3 services) · Feign inter-service · SAGA compensating · API-Gateway routing (8080) · DB-per-service — sab BANAYA/DEBUG/SAMJHA khud, tested. LEARNINGS.md me ~10 concepts (HikariCP, boilerplate, Feign, SAGA, gateway-namespace-gotcha, gitignore...). "bol sakta -> bana sakta" for MS achieved. BACHA (phase-2): Kafka (async, code me) — kal.
```

```
   REFERRALS: Raunak (JP — sent ✓) · Anmol (Razorpay — no fit). RESOURCES = PRIVATE repo (never expose / never add company-acct).
```

---
> Karm karo, phal ki chinta mat karo. Strengths JP-core se match — gap DELIVERY hai, ability nahi. SABAR = moat.
