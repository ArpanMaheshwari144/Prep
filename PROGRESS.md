# JP PREP — PRACTICE PLAYBOOK

> GOAL: JP Morgan (Associate/SDE-2, Bangalore) → later BlackRock. *"Effort 100% mera, outcome universe ka."*
> MODE: pure study. no faltu meta-talk. Timeline honest: JP DSA moderate (not FAANG) → achievable in months of RIGHT practice.
> ★ ARPAN — CONFIDENCE (5-Jul, DATA-backed, NOT arrogance): **"JP mein jaunga, wahan naam ROSHAN karunga."** grounded in: HLD-strong (Google-EM Spotify design felt EASY — known blocks), MS-project built (Feign/SAGA/gateway), SW boss-fight cracked, 30+ HLD why-how nuggets. arrogance = confidence WITHOUT basis; his = WITH basis.
> ★ PERCEPTION swings with DAILY MOOD (good day→"JP paas", bad day→"JP 1 saal door") = emotional NOISE, not info. TRUTH = steady accumulated DATA, doesn't oscillate daily. trust DATA not the day's feeling. "kharab din DIN hai, DATA nahi." (Claude: on down-days, counter with the data — never let the mood set the assessment.)

---

## ★★★★★ CLAUDE RULES (read FIRST — 4-month root-causes, 3-Jul)

```
   #0 PACE (DSA): pace ARPAN ka, Claude ka NAHI. EK cheez → RUKO → Arpan "agla" bole tabhi aage.
      NEVER "ek aur?", NEVER level-jump, NEVER un-studied throw, NEVER dump.
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
   1. HLD DELIVERY  — full-design ASSEMBLE + FOLLOW-UP/twist-defense (biggest reject-reason). freeze-proof.
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
      full-design assemble (8-step) + Claude pheke FOLLOW-UP twists → Arpan defend. gaps → sheet. log: 01_DESIGN_PRACTICE_LOG.md.
      new designs: payments, subscription (+ redo rate-limiter/notification). freeze-proof karna.
      ★ DRILL-RESPONSE: Arpan usually RIGHT (gap = delivery) → "sahi, galat nahi" + uske sentence ko clean interview-English.
        NAYA concept DUMP mat karo jaise galti ki. sirf jahan SACH me galat → wahan.

   STAR: 4-5 Konovo stories S-T-A-R me likho → bolne ki practice.

   LLD: SOLID/patterns/Builder CODE me implement (real code, not theory). PR-review: buggy code → find+fix.

   JAVA/SPRING: FULL VISUAL code revise (SEE/trace, crammed nahi), baar-baar. String-pool/HashMap/CHM/concurrency sharp.

   DSA (light-consistent): PER-PATTERN REP-BLOCKS + RETENTION-FIX (#0b) + PACE (#0) + STUB rule.
      ek pattern → template samjho → 3-4 SAME-pattern reps → SPACED redo till automatic → agla.
      ★ STUB: file me sirf PROBLEM + TESTS. template/recurrence SPELL-OUT mat karo — Arpan khud generate kare.
      core patterns: two-pointer/sliding-window/hashing/binary-search/greedy/DP-1D + graph-BASICS (BFS/DFS). easy-medium.

   ★★ DSA = SKILL, not KNOWLEDGE (Arpan-breakthrough 6-Jul — ye uski asli confusion thi "aaj tak"):
      theory/HLD/analogy ka TEST = recognition/samjhana → ek watch KAAFI → tikta.
      DSA ka TEST = blank-page se GENERATE karna → watching NEVER enough. guitar/gym jaisa: dekhne se muscle nahi,
      KARNE se banta. isliye theory jaisa "tikta" nahi — tik-ne ka rasta hi alag (karna, dekhna nahi).
      ★ PARADOX solved: 1000 problems EK-EK baar = 1000 RECOGNITION, gine-chune blank-GENERATION → isliye samajh
        hai par solo nahi aata. FIX = zyada NAYE problem NAHI → KAM problem (~30) BLANK+SPACED redo. "samajh aa gaya"
        = SHURUAAT, ant nahi; ant = "blank se bina peek bana liya."
      ★ STRUGGLE-value: 10-15 min blank-lado (fail bhi) → dimaag "hook" banata → answer tikta. 1-ghanta panic-atakna = bekaar.
        protocol: blank-try → atke-point pe MINIMAL peek → band → agle din phir blank (1/3/7 din).
      ★ Claude: DSA down-day pe "kar nahi sakta" ko DATA se kaato — 1000-solved = dimaag adapt kar CHUKA; issue = method (generation-reps), capability NAHI. skill hai, pahad nahi.
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
   ✓ DONE (2 stories, 6-Jul): 03_STAR_HikariCP (ownership + prod-debug + cross-team) · 04_STAR_WAF_IP (initiative + conviction/influence — khud pakda, team ko proof se convince).
   TODO stories: $20K Doximity payment fix · 600+ incidents (92% closure) · Lambda concurrency (1800+ throttles) · vendor-portal/email-dedup API (built) · VP/CTO recognition.
   HONESTY: 10+country platform = maintained/extended (NOT "built"); greenfield only = "built".
   ★ NEXT on STAR = BOLNE ki practice (loud, 2-3x) — story likhna ho gaya, delivery baaki. "MAINE/I" + numbers.
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
     RAW_DRILL: 00 REFLEX_SHEET (100%) · 01 DESIGN_PRACTICE_LOG · 02 WHY_HOW_QA (32 Q, incl S3-shard/S3-vs-DB/CDN-edge/geo-replica-vs-shard) · INTERVIEW_FRAMEWORK.md

   LLD  03_LLD/  (IN CODE — NOT theory-only): parking-lot · bookmyshow · tictactoe · snake-ladder · elevator · atm
        · bank-account · shopping-cart · library · vending-machine · hotel-booking  (+ notes for first 6)

   JAVA 01_JAVA/: core(oop/string/keywords/exceptions) · collections · java8 · multithreading · SOLID · design-patterns
        · brain-ticklers · 08_REVISION_VISUAL (core/collections/java8/mt/oop-design/quick-qa)
        PROJECT: SimpleBankSystem (SOLID + factory + observer + repository + service — patterns implemented ✓)

   SPRING 02_SPRING/: basics · transactional · jwt-security · profiles · hibernate · docker · k8s · REVISION_VISUAL
        PROJECT: usercrud (Spring Boot + JWT + k8s + tests ✓)   |   07_PROJECTS/todoapp (Spring Boot + k8s + tests ✓)

   DSA  08_DSA/: 01_TWO_POINTER DONE (5/5 SOLO, incl 3Sum REDO cleared) · 02_SLIDING_WINDOW DONE (4/4 SOLO: fixed/variable/combo/char-replacement) · 03_HASHING DONE (5/5: index/count/key-map + prefix-sum + set)
        + SW core-insight (sab SW = 1 template). TEMPLATE.md me tracks. baaki pattern-folders ready/khaali. spaced-redo (+7din) pending.

   DB   09_DATABASE/: indexing-practical (B-tree). INFRA 05_INFRA_DEEP/: AWS/Docker/K8S.
   RESOURCES/ = PRIVATE (own .git) — NEVER expose/touch.

   PROJECT (07_PROJECTS/mini_payment_ms) = COMPLETE (5-Jul): api-gateway + order + payment + notification (4 services)
      · Feign(sync) · SAGA · Kafka(async, docker broker) · kafka-ui(:8090) · DB-per-service · LEARNINGS.md (13+ concepts + flow-diagram). tested end-to-end.
   BEHAVIORAL 10_BEHAVIORAL/: 01_JP_COMPANY · 02_WHY_JP · 03_STAR_HikariCP ✓ · 04_STAR_WAF_IP ✓ (2 STAR stories DONE 6-Jul — #1 gap ab STARTED, 2 alag competency).
   -> STILL-TODO (jo abhi NAHI): more STAR stories (payment/incidents/Lambda) + STAR BOLNE-practice · code-review round · DSA new-patterns(binary-search/greedy) · subscription HLD (optional).
```

---

## STATUS (honest, 3-Jul-2026)

```
   HLD  = ~DONE. concept 100% + 11 foundations + 10 designs (18/19/20 full INTERVIEW-DELIVERY format: 8-step assemble
          + deep-dive + follow-up handle e.g. idempotency-vs-atomic) + 28 Q&A + reflex-sheet + BLOCK-KAB reflex.
          delivery = PRACTICED (not a gap). bacha = optional live-rehearsal (sharp rakhna) + subscription (optional theme).
          ★ Claude: HLD ko "gap" mat bol — Arpan yahan STRONG. staleness me kam-aankne ki galti mat dohrana.
   JAVA+SPRING = COMFORTABLE (strength) — core+collections+java8+MT+SOLID+patterns notes + SimpleBankSystem + usercrud/todoapp projects.
          keep sharp → microservices layer via Mini-Payment-System (order+payment+notification) on top of single-service base.
   DSA  = TWO-POINTER pattern DONE (5/5: MoveZeroes, Palindrome, Container, 3Sum, SortColors — SOLO produce).
          gap = GENERATION + RETENTION (single-exposure nahi tikta). → rep-blocks + spaced-redo (gaps me), LIGHT.
          next pattern = sliding-window (two-pointer ka extension).
   LLD  = 11 designs IN CODE (parking-lot..hotel-booking) + SimpleBankSystem patterns. NOT untouched. gap = recode + SOLID/pattern-articulate.
   CODE-REVIEW = round-practice start (buggy-PR → security/concurrency/clean-code find). STAR = #1 untouched gap.

   PRIORITY (data-flipped): HLD-DELIVERY → STAR → LLD-code → Code-Review → Java/Spring-sharp(project) → DSA-light.
   REALITY: strengths = JP-core. gaps = delivery + behavioral (trainable). DSA moderate for JP, not the blocker.

   ★ DONE-TODAY (6-Jul): (1) 2 STAR stories (03 HikariCP + 04 WAF/IP — #1 gap STARTED, alag competency). (2) DSA = SKILL breakthrough (Arpan ki "aaj tak" wali confusion cleared — recognition-vs-generation, guitar/gym analogy, 1000-done-once paradox, blank+spaced protocol; details DSA-practice section me). (3) WADA likha (CLAUDE.md + memory) — mutual promise, delete-loop/recognition-copy se rokna. (4) 2 DSA BLANK-REDO SOLO: Subarray-Sum=K (nemesis CLEARED, sum-k sahi, 4/4) + Longest-Substring-No-Repeat (6/6, ZERO hint vs pichli-baar-1-hint) — spaced-retrieval PROVEN kaam karta. dono +7din redo pending. (5) JAVA revise + BOL-ke (articulation): HashMap-internals · volatile-vs-synchronized · ConcurrentHashMap (CHM ka bucket-level fine-grained-lock point taught — Arpan ne pehle whole-map-lock/Hashtable-behaviour bola tha, corrected). articulation = slow-build (Arpan-set: script-ratna NAHI, apne shabdon me bolo, reps+interviews se dheere sharp).
   ★ KAL KA PLAN (Arpan-set 6-Jul, is order me):
     1) PROJECT thoda aage: mini_payment_ms me IDEMPOTENCY KEY (/pay pe double-charge na ho) + CIRCUIT-BREAKER (Resilience4j, payment down->fail-fast) lagao + use/practice dekho -> uske baad project DONE maan lo.
     2) DSA: pehle Arpan KHUD revise karega (self) -> phir ek NAYA pattern/topic (binary-search/greedy) — blank-spaced protocol se.
     3) JAVA + SPRING + HLD: same revise karte jao (pel ke) — HashMap/CHM/volatile aage, HLD design self-quiz, bol-ke articulation (slow-build).
     Baaki (aage naye din): STAR aur stories + bol-ke · code-review round · LLD-articulate · DSA +7din redos (Subarray-Sum=K, Longest-Substring-No-Repeat).
   ★ DONE (DSA): two-pointer (5, incl 3Sum REDO 5-Jul — gap CLEARED, SOLO no-hint) + SLIDING-WINDOW (4: fixed/variable/combo/char-replacement — char-replacement was earlier "nahi hua"/defer -> 5-Jul SOLO cracked). ★ SW core-insight: sab SW = ONE template (window++, condition, shrink, store; only condition+order change). spaced-redo (+7din) pending.
   ★ PROJECT COMPLETE (5-Jul, mini_payment_ms): order+payment+api-gateway+notification (4 services) · Feign(SYNC inter-service) · SAGA compensating · API-Gateway routing (8080) · KAFKA async (payment producer -> "payment-done" topic -> notification consumer) · kafka-ui (localhost:8090, docker) · DB-per-service — sab BANAYA/DEBUG/SAMJHA/VISUALIZE khud, tested end-to-end. LEARNINGS.md = 13+ concepts + flow-diagram (HikariCP, Feign, SAGA, Kafka, gateway-namespace, gitignore, kafka-ui/broker...). "bol sakta -> bana sakta -> dekh/samajh bhi liya" for MS FULLY achieved. · BEHAVIORAL folder (10_BEHAVIORAL: JP-company + why-JP).
```

```
   REFERRALS: Raunak (JP — sent ✓) · Anmol (Razorpay — no fit). RESOURCES = PRIVATE repo (never expose / never add company-acct).
```

---
> Karm karo, phal ki chinta mat karo. Strengths JP-core se match — gap DELIVERY hai, ability nahi. SABAR = moat.
