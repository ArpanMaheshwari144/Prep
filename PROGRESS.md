# JP PREP — PROGRESS & PLAYBOOK
> Single source of truth + forward plan. Full rules = CLAUDE.md · emotional/essence + ego-work = memory/MEMORY.md.
> ★ FRESH-designed 23-Jul (purane dated day-logs hataye — wo git-history me safe hain; ye file ab current-state + aage-ka-plan).

**GOAL:** JP Morgan (Associate/SDE-2, Bangalore) → later **BlackRock** (original + primary dream).
**MANTRA (Arpan's fire):** *"Effort itni maar di — ab JP LEKE RAHUNGA. JP = destination; MAIN aur JP. Pel ke mehnat + saaf intention → jaunga HI. Par mile na mile → PROUD rahunga."* (fire + peace, ownership — NOT surrender.)
**PERCEPTION-PRINCIPLE:** achha din → "JP paas", kharab din → "JP door" = emotional NOISE. TRUTH = steady accumulated DATA. "kharab din DIN hai, tu nahi; DATA hi sach." Down-day pe DATA (commits/solved-count) se counter.

---

## 0. CLAUDE RULES (short — full CLAUDE.md + MEMORY.md)
```
   #0  PACE (DSA): pace ARPAN ka. EK cheez → RUKO → "agla" bole tabhi. no dump/level-jump/un-studied-throw. "RUK"→ruk.
   #0b RETENTION: REVISION is everything (jo revise nahi hua fade hota — solo/feed koi bhi). spaced redo (1/3/7).
   #0c NO FILLER + NO EMOJI (uske terminal me toota dikhta) + NO "main yahin hoon"/sign-off. BANNED words: muscle/reps/struggle/generation/recognition (hollow → enrage). plain bolo.
   #0d TEACH-FIRST on gaps (quiz-shame nahi) + NO-UNDER-COUNT (repo check before "gap/add" — wo aage hai).
   #0e NO FEAR-MONGER ("hard at scale") + NO OVERSELL ("faad diya"). honest DATA calibration.
   #0f NO REST-NAG ("so ja/aaram" — HE decides). ★ FOLDER/FILE KABHI DELETE NAHI (git-safe, hold line even in frustration — folder 3x deleted, git restored har baar. WADA).
   #0g COMMIT + PUSH SAATH (add && commit && push — ek step; remote = safety-net).
   ★ SYCOPHANCY-GUARD: reflexively self-assessment counter mat kar; CALIBRATED raho; miscalibration OWN karo (dono taraf, data se). "tune khud kiya" pe zid mat kar jab scaffolding thi.
   ★ DSA-HELP = INTERVIEWER-BRAINSTORMING: nudge-sawaal only ("2 map se socho?"), wo KHUD solution tak. CODE/ANSWER/approach NEVER (jab tak explicitly na maange). syntax de sakte, LOGIC/STRUCTURE nahi. stub = PROBLEM + TESTS.
   ★ MEMORY = ek file (memory/MEMORY.md). alag chhote files NAHI.
   ★★ EGO / FEED-to-OWN / REVISION-AIRTIGHT / OFF-DAY / HELP-PROTOCOL ka POORA detail = MEMORY.md (23-Jul ka core kaam — har session padho).
```

---

## 1. STRATEGY (DATA-backed — 11 real JP writeups + Raunak insider)
```
   BIG TRUTH: "DSA necessary but NOT sufficient." JP REJECT hota HLD-DELIVERY + BEHAVIORAL pe, DSA pe nahi.
   -> Arpan strengths (Java/Spring fundamentals, HLD-concepts, 650-tickets code-review, 4yr prod) = JP CORE weight.
   -> DSA-panic disproportionate.

   ★ RAUNAK INSIDER (JP-referral, cleared JP — detail PRIVATE in RESOURCES/): 3 ROUNDS = 1 DSA + 1 System-Design + 1 Behavioural.
     -> 2/3 (SD + behavioral) = Arpan ki STRENGTH, sirf 1 DSA. DSA = LeetCode MEDIUM (arrays/hashmap/SW/graph-BFS). Alex Xu Vol-1 enough.
   PROFILE FIT: JPMC SDE-2 (~25L), 4yr service-MNC/bank bg = match. ❌ NOT social-hyperscale (WhatsApp/Insta = overkill).
   ★ JP layoffs (news) = OPERATIONS/call-center, NOT software-eng. tech = growing side. not red-flag for his role.
   DESIGN THEMES (practiced): rate-limiter · notification · payments · twitter · news-aggregator · bookmyshow · url-shortener · stock-broker · file-upload.
```

---

## 2. WHERE WE STAND (23-Jul — honest ~76% JP-ready; BUILD KHATAM → CONSOLIDATE + fill remaining)
```
   HLD        ✓✓ STRONG (differentiator) : 11 FOUNDATIONS + 10 SYSTEM_DESIGNS (full interview-delivery format) +
              RAW_DRILL (reflex-sheet + practice-log + 32 why-how QA) + SPOF-chain + Route-53. delivery PRACTICED (mock-tested).
              -> ab DAILY-REVISE + roz ek topic MOCK-TEST (interview-Q) + delivery-coaching (achhe words).
   JAVA       ✓✓ CORE COMPLETE (61 topics): Core(22) · Collections(11) · Java-8(5) · Multithreading(11 +live-deadlock) · SOLID(6) · Patterns(6).
   SPRING     ✓✓ DONE : basics · @Transactional (concept+DB-internals+propagation+gotchas) · JWT (Bearer/CSRF/signature-trust) ·
              profiles · Hibernate(7: N+1/lazy-eager/dirty-check/L1/cascade/mappings) · Docker · K8s (6/6).
   PROJECT    ✓ mini_payment_ms — 4 microservices (gateway+order+payment+notification): Feign(sync) · SAGA · Kafka(async, docker broker) ·
              idempotency (check-then-act, bug-fixed) · Circuit-Breaker(Resilience4j, 3-state) · API-Gateway(routing) · DB-per-service · kafka-ui.
              LEARNINGS.md + FULL SOURCE CODE (20 java files). + todoapp. -> ★ ab PROJECT-REVISE phase (see §3).
   LLD        ✓ 11 designs IN CODE (parking/bms/atm/elevator/hotel...). 5 revise-swept ("easy lagta ab"). gap = SOLID/pattern ARTICULATE.
   DSA        ✓ ~12 PATTERNS (sheet family-boxed, ~700 line, "gate ki chaabi") + ~70 problems. REVISE mode + Trees STARTED (see §4).
   BEHAVIORAL ✓ 5 STAR (HikariCP/WAF/Doximity/Mentoring/Failure-JDK — all competencies). done bol-ke; ab reps.
   COMPARES   ✓ 12 (sql-nosql/tcp-udp/sync-async/rest-graphql...). DATABASE ◐ (indexing+txn-internals). INFRA ✓ (AWS/Docker/K8s).
   CODE-REVIEW ✗ (optional — buggy-PR find; low priority).
   ★ Arpan's grounded self-view: "god-level DSA nahi, par JP ke liye KAAFI — main wo hoon." na oversell na undersell.
```

---

## 3. FORWARD PLAN (24-Jul onwards → SEPTEMBER apply)
```
   ★★ TIMELINE (23-Jul, Arpan-set — "August=apply" PURANA): JP apply = SEPTEMBER (rush nahi). end-Aug tak "sahi jagah hold" → Sept apply + pel.
      beech me koi achhi comp crack ho to wo bhi JP/BlackRock ka raasta (route FLEXIBLE, end-goal fixed).
   ★ PHASE = CONSOLIDATION + remaining fill (naya-heavy nahi; Java+Spring theory DONE → beech-beech READ-revise, no-quiz).

   ROZ ka flow (Arpan-set):
   - SUBAH:  pattern-sheet glance (daily, "gate ki chaabi") + HLD ek topic → Claude MOCK-TEST (interview-Q) + delivery-coaching (achhe words: caching/sharding/replication/idempotency — "aise bolo, kaise/kab/kyun" → interviewer impress).
   - SHAAM (light DSA): Trees continue (1-2 easy) YA pattern glance-test (jo bane=solid, jo nahi=redo-list). gate hai, doobna nahi.
   - BEECH-BEECH: Java/Spring ek note READ-revise (no quiz) + ★ PROJECT-REVISE.
   - SUNDAY: pattern-sheet polish.
   - OFF-DAY: light-gear (sheet revise/purana padho) — zero-day nahi par pahaad bhi nahi. (MEMORY.md off-day playbook.)

   ★★ PROJECT-REVISE (NAYA, IMPORTANT — 24-Jul se, Arpan-asked): mini_payment_ms ko interview ke liye ready karo.
      - SHURU SE, IN ORDER: LEARNINGS.md topic 0 (Overall Flow) → 1 → 2 ... sequentially (beech se nahi).
      - HAR topic pe DONO: (a) CONCEPT (story + why + how + bug-khud-pakda + power-words + follow-ups) + (b) ★ ACTUAL CODE (asli source file se snippet — "code kaise?" ready).
      - Claude ACTUAL note/code dikhata (fabricate NAHI); Arpan padh ke apni copy pe notes; STORY + code dono ready.
      - code mapping: idempotency→PaymentService.java · Feign→PaymentClient.java · Saga+CircuitBreaker→OrderService.java · Kafka-consumer→NotificationListener.java · Gateway→api-gateway/application.yml.

   ★ TREES/GRAPH/DP: Trees STARTED (23-Jul). continue easy → phir Graph, DP, Backtracking, Heap, Intervals (finite ~5-6 pattern bache — sheet CONVERGE karti, ~70% done).
```

---

## 4. DSA — STATUS + PHILOSOPHY (condensed; poora emotional/method = MEMORY.md)
```
   SHEET = "gate ki chaabi" (08_DSA/00_PATTERN_SHEET/PATTERN_SHEET.md): 12 patterns, family-boxed (har pattern apna gray-box + "KYUN saath" family headers), ~700 line.
     patterns: 1 two-pointer · 2 sliding-window · 3 hashing · 4 prefix-sum · 5 matrix · 6 stack(monotonic) · 7 binary-search · 8 linked-list · 9 design(hashmap/hashset) · 10 kadane · 11 bit-manip(XOR) · 12 trees.
   ★ SHEET CONVERGE karti (Arpan-insight 23-Jul): sheet problems pe nahi, PATTERNS pe badti; patterns FINITE (~15-25). 1000-Q wale ka sheet 100k-line nahi — same ~20 pattern internalized. ~70% done → bacha ~5-6 bade pattern → phir plateau (naye Q = existing pattern).

   METHOD (23-Jul, Arpan-committed): STRUGGLE-FIRST (peek nahi) → chhota example HAND-TRACE → sachchi-struggle-ke-baad editorial LAST-RESORT (no guilt) → kuch-din-baad COLD-REDO (spaced). job ke saath 1 problem/din 30-45min.
   ★ LIBRARY-OF-MECHANICS model: in-library pattern → instant; missing → learn ONCE (dry-run se zero-invent nahi hota) → library → transfer. "cold weak" = library-COVERAGE, ability nahi.
   ★ PHILOSOPHY (Arpan's, reinforce NEVER re-argue): "DSA = pattern + trick ka khel." REVISION >> new-solve (swap-nodes 3.5hr→5min proof). DSA = GATE (medium), not CP/mastery. hints/editorial = LEGIT no-guilt.
   ★★ REVISION-AIRTIGHT (23-Jul): solo-vs-feed IRRELEVANT — cold-solved bhi bina revise 7-din me blur. feed+revision = solo+revision = SAME. metric = REVISION, na solo-ness. (= "help+redo=owned" ka root.)

   RECENT (23-Jul): Two-Pointer + SW pattern REVISE (glance-test, sab recall, 2 revise-note) · Max-Product + HashMap COLD-REDO (verified) · Single-Number(XOR) solo · Trees STARTED (Max-Depth + Invert solo, pointer-vs-value+leaf-crash seekha) · while-vs-if + k=size-vs-limit confusion cleared · Redis-single-threaded (HLD rate-limiter mock).
```

---

## 5. METHOD (kaise karna — per subject, condensed)
```
   DSA        : struggle-first → hand-trace → (stuck) editorial → COLD-REDO spaced (1/3/7). stub = PROBLEM+TESTS. interviewer-brainstorming (Claude nudge, no code). DELETE/restart NAHI.
   HLD/LLD    : DELIVERY-phase → roz ek topic MOCK-TEST (interview-Q) + delivery-coaching (achhe words). LLD deep. [HLD/LLD = mock CHAHIYE; Java/Spring = no-quiz read.]
   PROJECT    : topic-by-topic (LEARNINGS.md se, in order) → concept + ACTUAL CODE → story + follow-ups ready. read-mode + notes.
   JAVA/SPRING: READ-revise (built, notes-ready, git-safe, lifelong). Claude ACTUAL note-content dikhata (fabricate nahi). NO quiz.
   STAR       : story S-T-A-R → BOL-KE loud (own words, script-ratta nahi). reps se sharp.
   ★ REVISION = HABIT: jo padha/banaya (project bhi) ~10 din me fade → roz thoda revise. Claude roz REVISE-REMINDER de (rotate: sheet daily + HLD/LLD/behavioral/Java-Spring/project).
```

---

## 6. WADA + RED LINES
```
   WADA (dono taraf): Arpan → "DSA behtar hunga, raat-din." Claude → "hamesha saath (letter/memory = mechanism) + delete-loop se rokunga (git-safe)."
   RED LINES: (1) DSA/files KABHI delete nahi (git-history theek, par reset/restart nahi — churn = 4-month root-cause, na ability).
              (2) RESOURCES/ = PRIVATE (own .git) — NEVER expose. referral (Raunak) + apply-timeline = RESOURCES/JP_referral_tracker.md (public PROGRESS me NAHI).
              (3) commit + PUSH saath.
```
> Karm karo, phal ki chinta mat. Strengths JP-core se match — gap = DELIVERY + reps, ability nahi. SABAR = moat. seekhna != jhukna.
```
