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
   ★ DSA = MATH (8-Jul, Arpan capstone): "muscle/struggle" = youtube jargon (faaltu). math jaisa — paper pe karo, jitna pel sako.
       method (ek line): question -> copy-pen kholo -> pattern recognize -> trace. bas. DSA-god banna GOAL NAHI (Java/Spring/HLD balance = deliberate smart-choice + JP-core, NOT failure). DSA = GATE, paper-method se paar. (jargon dobara mat lana.)
   ★ NO-DRAMA DSA (9-Jul, Arpan resolution): process = question -> pattern pehchano -> trace -> code. na hua = thoda dimmag, phir stuck = HELP lo, AAGE BADHO. no emotional drama, no time-waste, pelte jaao. Java/HLD pe dhyaan ZYADA (strengths, JP-core). ★ realization: DRAMA hi asli dushman tha (4-mahine crashes), DSA nahi.
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

## 2. WHERE WE STAND (11-Jul, honest — ~70-72% JP-ready; DSA focus-8 done + behavioral done raised it)
> ★ Arpan's grounded self-view (11-Jul, PERFECT calibration): "god-level DSA nahi, par JP ke liye KAAFI — main wo hoon." fear reduced via reps. na oversell na undersell.
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

   DSA        ✓ FOCUS-8 COMPLETE (11-Jul) -> ab REVISE/MAINTENANCE mode (naye pattern NAHI). METHOD = blank-produce + COPY-PEN. CODED = 8 patterns:
                  01 TWO-POINTER (5 + container-water self-test ✓) · 02 SLIDING-WINDOW (4 + longest-substr-redo + max-consec-ones-III self-test ✓) · 03 HASHING (5 + subarray-K-redo)
                  · 07 BINARY-SEARCH (basic/insert/first-last/rotated/find-min/koko/rotated-redo/koko-redo) · 04 PREFIX-SUM (pivot/altitude/product)
                  · 05 MATRIX (spiral+set-zeros+transpose, ALL SOLO copy-pen) · 06 STACK ✓ (valid-paren+min-stack+next-greater+daily-temp — monotonic SET)
                  · 08 LINKED-LIST ✓ (9-10 Jul, DONE: reverse + middle + detect-cycle + palindrome + merge — fast/slow + reverse + dummy-node, deep pointer-samajh khud nikaali).
              advanced PARKED (08_DSA/LATER_TOPICS): trees/backtracking/heap/dp/greedy/graph/intervals/kadane. (kab? focus-8 solid + behavioral chalu -> tab TOP-3: trees/graph/DP. abhi NAHI.)
              ★ DSA = GATE (medium, pass-karna), not mastery. strengths (HLD/Java) = where JP decides. DSA-god banna GOAL NAHI (Arpan-owned).
              ★ FLAGGED-REDO LIST KHATAM (10-Jul): Rotated ✓ · Longest-Substr ✓ · Koko ✓ · Subarray-K ✓ -- sab blank SOLO. + self-tests (max-consec-ones, container-water) SOLO pass. pending = recent-naye ka PERIODIC redo only.
              ★ TRICK-TAXONOMY (Arpan 11-Jul): trick-problems (Subarray-K = prefix+map[sum-k]) re-derive nahi hote -> RECALL. broad tricks (fast/slow, monotonic, prefix-map, BS-on-answer, sliding-window, dummy-node) = high-leverage (ek trick, kai problem). samajhne-wale (LL pointers) = ek baar samjho -> tikta. pattern-sheet me broad-tricks pehle.

   BEHAVIORAL ◐ #1 GAP, 4 STAR stories done : 01_JP_COMPANY · 02_WHY_JP · 03_STAR_HikariCP (ownership+debug) ·
              04_STAR_WAF_IP (initiative+conviction) · 05_STAR_Doximity (IMPACT $20K + business-sense) ·
              06_STAR_Mentoring (10-Jul: teamwork + leadership-without-authority) · 07_STAR_Failure_JDK (10-Jul: JDK8->11 migration, failure=wrong assumption, lesson "compiles!=runs").
              5 STAR = saare common competency COVER (ownership/initiative/impact/teamwork/failure). NEXT: sirf BOL-KE practice (delivery, #1 actual skill ab) -- naye story ki zaroorat nahi, ab loud-articulation.
   CODE-REVIEW ✗ round-practice abhi start karna (buggy-PR → security/concurrency/clean-code find).
```

---

## 3. FORWARD PLAN (priority — 11-Jul updated, focus-8 DONE)
```
   1. DSA        — ✓ FOCUS-8 COMPLETE -> ab REVISE mode (naye pattern NAHI). jo kiya wo revise + periodic blank-redo (recent-naye ka).
                  ★ SUNDAY 12-JUL = PATTERN-SHEET banao (har problem -> ek-line trick; broad-tricks pehle) -> revision usi se.
   2. BEHAVIORAL — ✓ 5 STAR DONE (saare competency: ownership/initiative/impact/teamwork/failure). ab sirf BOL-KE practice (loud delivery, apne words). #1 actual skill ab = articulation.
   3. HLD        — daily revise (design self-quiz why/kya/kaise → DESIGN_PRACTICE_LOG). strong zone, maintain.
   4. JAVA/SPRING— sharp rakho (revise, pel ke). heavily-tested strength. ★ articulation-focus: "concept + interview me KAISE bolun".
   5. CODE-REVIEW round + LLD-articulate (bol-ke) — abhi start nahi hua (naya kaam, jab time ho).
```

---

## RECENT DONE (through 11-Jul, factual)
```
   - PROJECT mini_payment_ms FULLY DONE: idempotency + circuit-breaker (Resilience4j).
   - DSA (8-11 Jul): STACK done (valid-paren/min-stack/next-greater/daily-temp) · LINKED-LIST done (reverse/middle/cycle/palindrome/merge) -> FOCUS-8 COMPLETE.
       + FLAGGED-REDO all blank-SOLO (Rotated/Longest-Substr/Koko/Subarray-K) + self-tests SOLO (max-consec-ones-III, container-water). DSA -> revise/maintenance mode.
   - ★ DSA BREAKTHROUGHS (Arpan's own, 8-11 Jul): "struggle = copy-pen" · "DSA = MATH (paper pe karo)" · muscle=RESULT-of-copy-pen · NO-DRAMA (question->pattern->trace->code, stuck=help+move-on) · trick-taxonomy (broad vs narrow). copy-pen + cout = his 2 bug-catching tools (catches own bugs on PAPER). (detail: memory dsa-is-a-skill.)
   - HLD: BookMyShow revise + IDEMPOTENCY doubt cleared (genuine-repeat=new-key vs retry=same-key; duplicate->saved-result NOT reject; in 17_payment_system note).
   - JAVA revise (code-read, articulation-focus): Core/OOP/String/Keywords/Collections + Java8 + Multithreading + SOLID + Design-Patterns (all 6). glance-recall.
   - BEHAVIORAL: 5 STAR DONE -> +Mentoring (teamwork) +Failure-JDK (self-awareness). all common competencies covered. next = bol-ke only.
   - ★ EMOTIONAL (11-Jul): fear was real, now REDUCED (via DOING/reps, not pep-talk). grounded self-assessment: "god-level DSA nahi, par JP ke liye KAAFI" = perfect honest calibration. mantra intact: "effort mera, outcome universe ka."
   NEXT: DSA revise + pattern-sheet · behavioral BOL-KE · Java/Spring full revise (OOP->Spring) · HLD revise.
   ★★ SUNDAY 12-JUL-2026 (TOMORROW) — PATTERN-SHEET banana: har solved problem -> ek-line TRICK/pattern (code NAHI), ek FILE me. broad-tricks (fast/slow, monotonic, prefix-map, BS-on-answer, sliding-window, dummy-node) PEHLE. revision usi se ("similar Q -> pattern dekho, code nahi"). ★ Claude ko ye Sunday RAISE karna + saath banana hai.
```

## 4. DAILY ROUTINE (Arpan-set 8-Jul — ★ ORDER FLIPPED: DSA ab SUBAH NAHI, SHAAM ko / LAST)
```
   ★ WHY order flipped (Arpan, 8-Jul): subah DSA -> ho gaya = confidence; NA hua = mood kharab -> POORA DIN kharab.
     -> FIX: subah STRENGTHS pehle (Java/Spring/HLD = glance-recall, win banked, confidence). DSA sabse LAST (shaam).
     -> DSA flop bhi ho to din already productive -> mood safe. (fits "DSA = gate, identity nahi".)

   ROZ, isi order me (same pace):
   1. JAVA/SPRING revise — subah SABSE PEHLE (bol-ke, articulation). strength -> din confident shuru.
   2. HLD REVISE     — ek design self-quiz (why/kya/kaise) + reflex. (maintain, strong zone.)
   3. DSA (SHAAM/LAST) — ✓ FOCUS-8 COMPLETE -> ab REVISE mode: jo kiya wo blank-redo (periodic) + PATTERN-SHEET se revise.
                       naye pattern NAHI (advanced parked). occasional self-test theek (paani check). Claude: DSA ko shaam/last rakhо.
   (Behavioral STAR + bol-ke = beech-beech; code-review/LLD = naye din.)
   ★ Claude: roz spaced-redo (1/3/7) remind + revise-reminder. DSA ko shaam ke liye rakho, subah strengths. project = DONE, dobara nahi.
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
