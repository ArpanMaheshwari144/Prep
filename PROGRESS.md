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
   ★ COPY-PEN = the "struggle" (8-Jul, Arpan-derived): DSA formula = PATTERN (jaano) + COPY-PEN (example haath se paper pe trace → code → run).
       dono chahiye — pattern bina copy-pen "trace kya"; copy-pen se sahi-apply + apna bug khud pakadta. copy-pen PEHLE, code baad me.
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

   DSA        ◐ METHOD = blank-produce + COPY-PEN (paper trace pehle) + spaced-redo. CODED = 5 patterns:
                  01 TWO-POINTER (5+2redo) · 02 SLIDING-WINDOW (4+2redo) · 03 HASHING (5+2redo)
                  · 07 BINARY-SEARCH (6: basic/insert/first-last/rotated/find-min/koko) · 04 PREFIX-SUM (pivot/altitude/product).
                  · 05 MATRIX (8-Jul: spiral + set-zeros + transpose — ALL SOLO via copy-pen; spiral crashed then conquered same-day). baaki first-pass: stack, linked-list.
              advanced PARKED (08_DSA/LATER_TOPICS): trees/backtracking/heap/dp/greedy/graph/intervals/kadane.
              ROOT-CAUSE (Arpan-owned 8-Jul): 1000-problems = video-based RECOGNITION, not independent generation.
                  FIX = khud-blank-solve (no video) + spaced-redo + PEEK->SIMILAR-PROBLEM-TEST (verify pattern set, not ratta).
              ★ DSA = GATE (medium, pass-karna), not mastery. strengths (HLD/Java) = where JP decides. spaced-redo pending: Rotated(peeked)/Koko/Subarray-K/Longest-Substr.

   BEHAVIORAL ◐ #1 GAP, 3 STAR stories done : 01_JP_COMPANY · 02_WHY_JP · 03_STAR_HikariCP (ownership+debug) ·
              04_STAR_WAF_IP (initiative+conviction) · 05_STAR_Doximity (IMPACT $20K + initiative + business-sense).
              teeno alag competency. NEXT: BOL-KE practice (delivery) + maybe a FAILURE story (common ask, still missing).
   CODE-REVIEW ✗ round-practice abhi start karna (buggy-PR → security/concurrency/clean-code find).
```

---

## 3. FORWARD PLAN (priority — kaha jaana hai)
```
   1. DSA        — STRATEGY (Arpan, 7-Jul): SCOPE NARROWED to 8 CORE patterns (advanced DP/graph/trees/etc PARKED in 08_DSA/LATER_TOPICS). FOCUS-8: two-pointer · sliding-window · hashing · prefix-sum · matrix · stack · binary-search · linked-list.
                  METHOD: roz REVISE + same-topic ke NAYE question (pel ke) -> reps se "wire"/muscle (Koko-proof: reps-heavy = automatic, pressure me bhi chalta; reps-kam = atakta. capacity nahi, reps ki baat).
                  STRONG (Arpan-self, anchor): two-ptr, SW, hashing, BS. FIRST-PASS baaki: prefix-sum, matrix, stack, linked-list. ★ Claude spaced-redo (1/3/7) track+remind.
   2. BEHAVIORAL — aur STAR stories (payment $20K / 600-incidents / Lambda-throttle) + BOL-KE. #1 untouched, JP-weighted.
   3. HLD        — daily revise (design self-quiz why/kya/kaise → DESIGN_PRACTICE_LOG). naya nahi.
   4. JAVA/SPRING— sharp rakho (revise + project). heavily-tested strength.
   5. CODE-REVIEW round + LLD-articulate (bol-ke) — naye din start.
```

---

## RECENT DONE (7-8 Jul, factual)
```
   - PROJECT mini_payment_ms FULLY DONE: idempotency + circuit-breaker (Resilience4j, tested CLOSED->OPEN->HALF-OPEN).
   - DSA: binary-search (6) + prefix-sum (3) coded; 4 spaced-redos (copy-pen); matrix DONE (spiral+set-zeros+transpose, all SOLO copy-pen); stack STARTED (valid-parentheses SOLO, found own hidden bug via edge-test).
   - ★ DSA BREAKTHROUGH (8-Jul, Arpan's own synthesis): "struggle = copy-pen"; formula = PATTERN + COPY-PEN; had pattern, copy-pen was the missing piece → DSA now flows ("aa raha hai"). (see rules block + memory dsa-is-a-skill.)
   - HLD: rate-limiter full deep-dive (discussion mode: why/algo/distributed/fail-open/placement) — notes already comprehensive, added reject-early reasoning. HLD = STRONG (glance-recall).
   - JAVA/SPRING revise (code-read mode, 10+ topics): String/StringBuilder · SOLID · IoC-DI · @Transactional · Streams/Lambda · Comparable-Comparator · Exceptions · Singleton/Factory · JPA. (all glance-recall = mastery.)
   - BEHAVIORAL: 3 STAR stories (HikariCP/WAF/Doximity).
   NEXT: matrix first-pass (fresh, copy-pen) + stack/LL · spaced-redos · behavioral bol-ke · Java/HLD daily-revise.
   ★★ SUNDAY 12-JUL-2026 REMINDER (Arpan, 8-Jul): PATTERN-SHEET banana -> har solved DSA problem ko ek short PATTERN-summary me convert (code NAHI), sab ek FILE me. revision us pattern-file se hoga (code nahi kholenge). "same/similar Q aaye -> pattern dekho, code nahi." Claude: is Sunday ye raise karna + Arpan ke saath sheet banana/improve karna. (Claude ko Sunday ye YAAD dilana hai.)
```

## 4. DAILY ROUTINE (Arpan-set 7-Jul — project DONE, ab steady rhythm)
```
   ROZ, isi order me (same pace):
   1. DSA (PRIORITY) — Arpan-method (7-Jul): (a) roz SPACED-REDO purane problems (+3/+4/+7 din, blank, copy-pen) — Claude track+remind.
                       (b) DEPTH: jab ek topic SMOOTH chale -> us SAME topic ka ek aur naya Q (Claude gauge kare). (c) jab SOLID -> NEW topic utha.
                       focus-8: two-ptr/SW/hashing/BS/prefix-sum DONE first-pass. BAAKI first-pass: matrix (light, index-nav: spiral/rotate/set-zeroes), stack, linked-list.
                       ★ KAL (Arpan): MATRIX finish first (spiral/rotate/set-zeroes — no big trick, careful index-nav; copy-pen pe grid bana ke, index/off-by-one galti aasan) -> phir LINKED-LIST + STACK.
   2. HLD REVISE     — ek design self-quiz (why/kya/kaise) + reflex. (maintain, strong zone.)
   3. JAVA/SPRING revise — bol-ke (articulation).
   (Behavioral STAR + bol-ke = beech-beech; code-review/LLD = naye din.)
   ★ Claude: roz spaced-redo (1/3/7) remind + revise-reminder. project = DONE, dobara nahi.
```

---

## 5. METHOD (kaise karna — condensed)
```
   ★ REVISION = HABIT (Arpan, 6-Jul): jo padha/banaya (project bhi) wo ~10 din me FADE -> revisit ZAROORI.
     fresh-aaj ≠ yaad-baad-me (same spaced-retention principle, macro/schedule level).
     -> AUGUST (apply-phase) se ~2 HRS DAILY sirf REVISE (DSA + Java + HLD + project, pel ke); abhi bhi roz thoda.
     ★ Claude: roz REVISE-REMINDER dena (ye mera kaam).
```
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
