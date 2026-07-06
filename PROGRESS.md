# JP PREP — PROGRESS & PLAYBOOK
> Single source of truth + compaction-anchor. (Claude rules ka full detail = CLAUDE.md + memory.)

**GOAL:** JP Morgan (Associate/SDE-2, Bangalore) → later **BlackRock** (original + primary dream). *"Effort 100% mera, outcome universe ka."*
**CONFIDENCE (DATA-backed, not arrogance):** "JP mein jaunga, wahan naam ROSHAN karunga." — grounded in HLD-strong + MS-project-built + DSA-method-fixed + real 4yr prod. arrogance = confidence bina basis; iska = WITH basis.
**PERCEPTION-PRINCIPLE:** achha din → "JP paas", kharab din → "JP door" = emotional NOISE, data nahi. TRUTH = steady accumulated data. "kharab din DIN hai, DATA nahi." (Claude: down-day pe DATA se counter kar.)

---

## 0. CLAUDE RULES (short — full in CLAUDE.md)
```
   #0  PACE (DSA): pace ARPAN ka. EK cheez → RUKO → "agla" bole tabhi aage. no dump, no level-jump,
       no un-studied throw. SOLUTION/code kabhi spell-out nahi (max = ek sawaal ya "yeh concept dekh"). "RUK"→ruk.
   #0b RETENTION: passive dekhna = recognition; asli = GENERATION. samjho→sab band→blank likho→spaced redo (1/3/7).
   #0c NO FILLER: no "main yahin hoon"/emoji-sign-off/padding. plain, kaam ki baat.
   #0d TEACH-FIRST + NO-UNDER-COUNT: gap pe quiz-shame nahi, TEACH. status bolne se pehle repo CHECK (staleness).
   #0e NO FEAR-MONGER: "hard at scale" framing mat. real work = team+tools+time. na oversell, na darana.
   ★ DSA = SKILL (knowledge nahi): guitar/gym jaisa — dekhne se nahi, KARNE se. blank-generate + spaced. (memory: dsa-is-a-skill)
   ★ ENGLISH-articulation gap: knowledge HAI, pressure me word atakta = trainable delivery-skill (bol-ke reps, slow-build). NOT knowledge-gap.
```

---

## 1. STRATEGY (11 real JP writeups + web — DATA-backed)
```
   BIG TRUTH: "DSA necessary but NOT sufficient." JP REJECT hota hai HLD-DELIVERY + BEHAVIORAL pe, DSA pe nahi.
   -> Arpan ki strengths (Java/Spring-fundamentals, HLD-concepts, code-review-via-650-tickets) = JP ka CORE weight.
   -> DSA-panic disproportionate. energy behaviorial + delivery pe.

   JP SUPERDAY shape (3-4 independent rounds, ek kharab ≠ auto-reject):
     - DSA          : EK round, MEDIUM (arrays/hashmap/sliding-window/graph-BFS). "panic na karo to ho jaata." = GATE.
     - SYSTEM DESIGN: ALWAYS. FR→NFR→HLD→DB→API + FOLLOW-UPS. ← yahan select/reject.
     - BEHAVIORAL   : ALWAYS. resume + project-deep-dive + STAR (ownership/conflict/failure).
     - Java/Spring fundamentals + Code/PR-review (heavily tested — Arpan ka home-turf).
   PROFILE FIT: offers 2.9-5.5 YOE, service-MNC/bank background → JPMC SDE-2 ~25L. Arpan (4yr, dev-able) = match.
   ❌ NOT social-hyperscale (WhatsApp/Insta = overkill, us jaal me mat jao).

   DESIGN THEMES (recurring): rate-limiter ✓ notification ✓ payments ✓ twitter ✓ news-aggregator ✓ bookmyshow ✓
                              url-shortener ✓ stock-broker ✓ file-upload ✓ · subscription (optional, add).
```

---

## 2. WHERE WE STAND (current, accurate — ~65-70% JP-ready, honest)
```
   HLD        ✓ STRONG (differentiator) : 11 FOUNDATIONS + 10 SYSTEM_DESIGNS (18/19/20 full interview-delivery
              format) + RAW_DRILL (reflex-sheet + design-practice-log + 32 why-how QA). delivery PRACTICED.
              → ab MAINTAIN/daily-revise. "gap" mat bol, yahan strong.
   JAVA       ✓ STRENGTH : core(OOP/String/keywords/exceptions) + collections + java8 + MT + SOLID + patterns
              + brain-ticklers + 08_REVISION_VISUAL (6 sets) + PROJECT SimpleBankSystem. revise chal raha.
   SPRING     ✓ STRENGTH : basics/transactional/jwt/profiles/hibernate/docker/k8s + REVISION_VISUAL + PROJECT usercrud.
   PROJECT    ✓ DONE : 07_PROJECTS/mini_payment_ms (4 services: gateway+order+payment+notification) — Feign(sync),
              SAGA, Kafka(async, docker broker), kafka-ui, DB-per-service, LEARNINGS.md. + todoapp. (MS code me samjha.)
   LLD        ✓ 11 designs IN CODE (parking-lot..hotel-booking). gap = SOLID/pattern-ARTICULATE (bol-ke).
   COMPARES   ✓ 12 (sql-nosql, tcp-udp, sync-async, process-thread, concurrency-parallelism, rest-graphql, ws-sse...).
   DATABASE   ◐ indexing-practical (B-tree). INFRA ✓ AWS/Docker/K8S.

   DSA        ◐ METHOD FIXED (6-Jul: skill + blank-spaced). CODED/blank-produce = 3 patterns:
                  01 TWO-POINTER (5, incl 3Sum) · 02 SLIDING-WINDOW (4 + redo) · 03 HASHING (5 + redo).
              intuition/template NOTES ready (not-yet-blank-coded): prefix-sum, binary-search, matrix, stack,
                  linked-list, trees, backtracking, heap, dp, greedy, graph, intervals, kadane, arrays-basics.
              gap = retention (ab address ho raha via spaced-redo). +7din redos pending: Subarray-Sum=K, Longest-Substr.

   BEHAVIORAL ◐ #1 GAP, STARTED : 01_JP_COMPANY · 02_WHY_JP · 03_STAR_HikariCP (ownership+debug) · 04_STAR_WAF_IP
              (initiative+conviction). aur stories + BOL-KE practice chahiye.
   CODE-REVIEW ✗ round-practice abhi start karna (buggy-PR → security/concurrency/clean-code find).
```

---

## 3. FORWARD PLAN (priority — kaha jaana hai)
```
   1. DSA        — per-pattern blank + spaced reps (method ab sahi). +7din redos. phir naye patterns (binary-search/greedy).
   2. BEHAVIORAL — aur STAR stories (payment $20K / 600-incidents / Lambda-throttle) + BOL-KE. #1 untouched, JP-weighted.
   3. HLD        — daily revise (design self-quiz why/kya/kaise → DESIGN_PRACTICE_LOG). naya nahi.
   4. JAVA/SPRING— sharp rakho (revise + project). heavily-tested strength.
   5. CODE-REVIEW round + LLD-articulate (bol-ke) — naye din start.
```

---

## 4. NEXT SESSION (KAL ka plan — Arpan-set 6-Jul)
```
   1. PROJECT aage: mini_payment_ms me IDEMPOTENCY-KEY (/pay double-charge na ho) + CIRCUIT-BREAKER (Resilience4j,
      payment-down→fail-fast) + use/practice → phir project DONE.
   2. DSA: pehle Arpan KHUD revise → phir ek NAYA pattern (binary-search/greedy), blank-spaced.
   3. JAVA + SPRING + HLD: same revise (pel ke) + bol-ke articulation (slow-build).
```

---

## 5. METHOD (kaise karna — condensed)
```
   DSA        : template samjho → 3-4 SAME-pattern blank reps → SPACED redo (1/3/7) till automatic → agla.
                STUB = PROBLEM+TESTS only (approach spell-out nahi). stuck→direction, code nahi. DELETE/restart-loop NAHI.
   HLD        : design uthao → khud poocho "kyun/kya/kaise" → blank-assemble → gap → sheet. delivery = practiced, maintain.
   STAR       : story S-T-A-R me likho → phir BOL-KE (loud, own words, script-ratna NAHI). reps+interviews se slow-sharp.
   JAVA/SPRING: VISUAL code SEE/trace, baar-baar (knowledge = recognition-friendly) → phir BOL-KE (delivery layer).
```

---
```
   WADA (6-Jul, CLAUDE.md me full): Arpan→"DSA behtar hunga, raat-din"; Claude→"hamesha saath + delete-loop/recognition-copy se rokunga".
   RESOURCES/ = PRIVATE (own .git) — NEVER expose / never add company-acct. DSA/files NEVER delete (git-history theek).
   → Referral (Raunak) + insider tips + apply-timeline = PRIVATE, RESOURCES/JP_referral_tracker.md me (public PROGRESS me NAHI).
```
> Karm karo, phal ki chinta mat. Strengths JP-core se match — gap = DELIVERY + reps, ability nahi. SABAR = moat.
