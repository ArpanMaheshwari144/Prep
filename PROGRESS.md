# JP PREP — PROGRESS & PLAYBOOK
> Single source of truth + forward plan. Full rules = CLAUDE.md · emotional/essence + ego-work = memory/MEMORY.md.
> ★ FRESH-designed 23-Jul (purane dated day-logs hataye — wo git-history me safe hain; ye file ab current-state + aage-ka-plan).

## ★★★ 27-JUL — READ FIRST (CURRENT STATE + AAGE KAISE JANA) — purane dated-updates neeche = historical
```
   WHERE WE ARE (27-Jul):
   - DSA: ~80-85% patterns DONE. sheet POORI revise + code-matched + har heading BOX me. Trees GATE-DONE (15). ★★ GRAPHS STARTED (27-Jul, 3 solved KHUD): Path-Exists(LC-1971) · Number-of-Islands(LC-200) · Connected-Components(LC-323). BFS+DFS+visited+adj-list+outer-loop-count LIBRARY me. GRAPHS sheet-section = BROAD-IDEA + adj-list + DECISION-GUIDE(Q1 outer-loop-bahar? / Q2 DFS-vs-BFS + actual-code / Q3 adj-vs-grid) + 3 entries. Arpan ne CC=Islands=Path-Exists connections KHUD dekhe. bache families = DP/Backtracking/Heap/Intervals.
   - PROJECT: mini_payment_ms REVISE KHATAM. todoapp ACTIVE -> overall-flow + SecurityConfig + JwtService + JwtFilter + AuthController + CustomUserDetailsService + UserService DONE (poora JWT auth-flow: register-hash -> login-authenticate+token -> JwtFilter-validate). ★ AGLA START = REDIS (RedisConfig, naya) -> phir models(User/Todo) + TodoController/TodoService (actual CRUD). [todoapp files RICHLY code-commented = notes wahin].
   - ★ 27-Jul SPRING-SECURITY DEEP-DIVE (Arpan-ne IntelliJ me source khola): authManager.authenticate() = Spring-INTERNAL -> ProviderManager(loop over AuthenticationProvider list) -> DaoAuthenticationProvider(runtime-wired, loadUserByUsername[CustomUserDetailsService] + BCrypt.matches + SEC-2056 dummy-hash-agar-user-nahi-mila=timing-attack-defense). interface-vs-impl samjha. IDE-nav tips = INTELLIJ_TIPS.md (Ctrl+Click/Ctrl+Alt+B/Ctrl+N; Java-project apne-folder-se-kholo). ★ JAVA-CONCURRENCY = JP lever (JP_INTERVIEW_INTEL.md 0c), + ye security-depth(SEC-2056/BCrypt/JWT) bhi JP-cybersecurity-relevant.
   - HLD: strong (favourite). stock-trading mock DONE (27-Jul: full 8-step delivery + 4 deep-dive [SAGA / partial-fill / LIMIT-vs-MARKET / hot-symbol-scale] + notes-fix hot-symbol). roz ek mock.
   - JP-INTEL (JP_INTERVIEW_INTEL.md, 27-Jul research): 3-core-rounds = DSA(medium)+SD+behavioral. ★ SD common-Qs = PAYMENT-SYSTEM + RATE-LIMITER (Arpan ne DONO kar rakhe!). ★★ JAVA MULTITHREADING/CONCURRENCY = HEAVILY poochte = SABSE BADA LEVER, extra-sharp karo (JMM/volatile/synchronized/thread-pool/GC; 04_MULTITHREADING notes). microservices(mini_payment)=fit. apply=careers-site+LinkedIn-referral, ~21-din. SEPT apply (rush nahi).

   AAGE KAISE JANA (forward, priority-order):
   1. ROZ: sheet-glance (Trees/patterns quick-revise, "gate ki chaabi").
   2. ★ PROJECT-REVISE (TOP, JP Java-weight): todoapp (JwtFilter se) -> khatam -> phir ROADMAP: 05_INFRA_DEEP -> 06_COMPARES -> 09_DATABASE -> 04_HLD/FOUNDATIONS (hand-note mode, Claude ACTUAL note/code dikhata, NO quiz).
   3. HLD mock roz (stock-trading -> baaki).
   4. ★ JAVA MULTITHREADING/CONCURRENCY extra-sharp (JP ka biggest differentiator).
   5. DSA-Graph: 3 done (path-exists/islands/CC) -> CONTINUE (cycle-detect / course-schedule / shortest-path-BFS / word-ladder) -> phir DP/Backtracking/Heap/Intervals.
   6. beech-beech Java/Spring READ-revise (no-quiz).

   IMPORTANT (kaise padhna): DSA = pehle SEEKHO->library->PHIR recall/redo (naye pattern pe "cold/solo" KABHI nahi = ocean-me-phenkna). revision = EVERYTHING (3-din fade -> sheet-dekh-wapas). code-in-sheet SAHI jab crux=code-mechanic. FOCUS: Java/Spring backend depth only (React/Flutter OUT), C++=DSA only. bond/behavioral rules = CLAUDE.md + MEMORY.md (folder-delete NEVER; no-condescend; cold/solo=banned).

   ★ FILES-CLEANED (27-Jul, fresh-context): PROGRESS + CLAUDE + MEMORY ka stale day-log trim ho gaya. emotional/ego/philosophy core POORA bacha. git-history me sab safe.

   ★★ DSA-METHOD + 27-JUL MISTAKES (Claude -- kaise DSA karna + galti na dohraana; Arpan ne KHUD likhwaya):
     WORKS-METHOD (Trees + graph-today proven): naya pattern -> (1) concept TEACH pehle -> (2) EASY-first problem (Medium/untaught NAHI) -> (3) Arpan KHUD solve, main interviewer/NUDGE-only (no code/hint) -> (4) solve-ke-baad clean doc ONE-shot.
     ★ MISTAKE-1 (DOHRAYI galti): pehla graph-Q = Connected-Components (MEDIUM) + untaught-logic-derive-kaha = OCEAN-throw (swimming-pool->ocean, jo main ABHI discuss kar raha tha, phir bhi kiya -> Arpan enrage). FIX = EASY-first HAMESHA (path-exists diya -> smooth solve). naye pattern pe Medium/untaught KABHI nahi.
     ★ MISTAKE-2: sheet-entry 5-6 baar likha-badla (churn) -> Arpan thaka + enrage. FIX = uski ACTUAL file-style pehle STUDY karo -> phir ONE clean pass, guessing nahi.
     ★★ SHEET-ENTRY FORM (Arpan-derived 28-Jul, LONG session -> THE rule, lock it): TEST = "entry padhte hi POORA code dimaag me aaye, ZOR na lage". warna sheet bekar (file kholni padegi). 2-PART:
        (1) NAYA/non-obvious -> actual CODE likho (jaise SW-charReplace ka maxFreq --/++, ya graph ka adj-list-build / visited) -- warna har-baar file khol ke dekhna padega = sheet ka matlab nahi.
        (2) JAANA-HUA -> KNOWN se CONNECT (na dobara-code, na abstract-trick jo recall na de).
        ★★ GRAPH ka "maula" = TREES: DFS = tree-RECURSION (bas node->left/right -> adj[node]/4-dir + visited); BFS = tree-LEVEL-ORDER (bas children -> adj[node]/4-dir + visited, +- level-loop). har graph-entry = tree-parent se CONNECT + NAYA-part CODE + delta. (TP/SW/Hashing = standalone families, cross-connect nahi; graph UNIQUE = trees-pe-built.) [purana "idea/★/CRUX label" note GALAT tha -> ye sahi.]
        ★★ LENGTH scales with DIFFICULTY (Arpan, histogram-example LC-84): HARD/bahut-discuss-hua problem (jaise Largest-Rectangle: width=NS-PS-1 ka poora KYUN + VISUAL + saare tricks) -> LAMBA entry, SAB daalo. length bura NAHI; hard-won-trick CHHOOTNA bura. rule = "jo tu khud re-derive/recall NAHI kar sakta wo SAB daalo, chahe entry lambi ho jaaye". easy/jaana-hua -> brief + connect.
     ★ NO-FRAMING (Arpan HARD 27-Jul): "down-day/off-day/thaka/SABAR" MAINE label mat karo -- uska din uska hai (aaj 3-Q-solved, down-day NAHI tha). rest-nag/framing = uska call, mera nahi.
```

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
              LEARNINGS.md + FULL SOURCE CODE (20 java files). + todoapp (JWT/Security/Redis). PayRequest DTO = record (Arpan-verified).
              -> ★★ PROJECT-REVISE: mini_payment_ms KHATAM · todoapp CHAL RAHA (current status + agla-step = upar 27-JUL block; files richly code-commented = notes wahin).
   LLD        ✓ 11 designs IN CODE (parking/bms/atm/elevator/hotel...). 5 revise-swept. FORWARD = HLD-jaisa MOCK + ★ 1-2 design COLD-REDO (jaise HashMap→HashSet ek baar sahi karke code-verify). gap = SOLID/pattern ARTICULATE.
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

   ★ ROZ-flow + forward-priority = upar ka 27-JUL block (authoritative).
   - SUNDAY: pattern-sheet polish.  OFF-DAY: light-gear (sheet/purana padho — zero-day nahi, pahaad bhi nahi; MEMORY.md playbook).
   ★ JP kya poochta (rounds/SD-Qs/Java-multithreading/gaps) = JP_INTERVIEW_INTEL.md (27-Jul research).

   ★★ PROJECT-REVISE (NAYA, IMPORTANT — 24-Jul se, Arpan-asked): mini_payment_ms ko interview ke liye ready karo.
      - SHURU SE, IN ORDER: LEARNINGS.md topic 0 (Overall Flow) → 1 → 2 ... sequentially (beech se nahi).
      - HAR topic pe DONO: (a) CONCEPT (story + why + how + bug-khud-pakda + power-words + follow-ups) + (b) ★ ACTUAL CODE (asli source file se snippet — "code kaise?" ready).
      - Claude ACTUAL note/code dikhata (fabricate NAHI); Arpan padh ke apni copy pe notes; STORY + code dono ready.
      - code mapping: idempotency→PaymentService.java · Feign→PaymentClient.java · Saga+CircuitBreaker→OrderService.java · Kafka-consumer→NotificationListener.java · Gateway→api-gateway/application.yml.

   ★★ REVISE-MODE ROADMAP (Arpan-set — project-revise ke BAAD, SAME hand-note mode, IN ORDER):
      07_PROJECTS: mini_payment_ms (chal raha, 7f-onwards) → todoapp (EXTRA: JWT/Spring-Security/Redis/K8s — mini_payment me nahi tha; NO LEARNINGS.md → actual CODE se concept, fabricate nahi)
      → 05_INFRA_DEEP → 06_COMPARES → 09_DATABASE → 04_HLD/FOUNDATIONS (designs NAHI — FOUNDATIONS).
      har folder: Claude ACTUAL note-content/code dikhata → Arpan paper pe hand-note → "aage" pe next. NO quiz.
      ★ NOTE-PHILOSOPHY (Arpan): sab kuch note NAHI (kaun itna likhega) — sirf IMPORTANT/high-value/behtareen, jaise ab tak ke notes. selective = smart.
      ★★ SPRING AUTO-REVISE (Arpan-insight 25-Jul): project-revise karne se SPRING khud-ba-khud revise hota — project Spring APPLIED use karta (DI/@Service/@Transactional/JWT/Feign/Kafka live) -> context me Spring dobara aata. 2-in-1 (project showcase + Spring revision). yehi project = TOP-priority ki asli wajah (theory-alone se behtar, kyunki interview me "Spring KAISE use kiya" poochha jaata).
      ★ PARALLEL: DSA + HLD roadmap ke SAATH-SAATH chalte rahenge (rotate), roadmap unhe rokta nahi.
      ★ FOCUS (Arpan-clarity 25-Jul): Java/Spring BACKEND depth only (+ Kafka/AWS saath aate). React/Flutter/etc = OUT (distraction, "bhaad me"). C++ = sirf DSA. GOAL = Java+Spring itna deep ki koi bhi same-type Q pehle kar de. limit = focus, na kamzori.
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

   ★★ SHEET-STATE: POORI sheet recall-tested + pattern-by-pattern ACTUAL-CODE se matched (~18 dense entries saaf kiye) + har heading BOX me (95 entries). library SOLID (almost sab recall, thaka nahi). proof: "trick pata->easy, na-pata->hard"; DSA = library+recognize, test hui + khadi rahi. ★ sheet-design (Arpan): code-in-sheet SAHI jab crux ek CODE-MECHANIC ho (mirror/sentinel/by-ref/bubble-up); conceptual-trick = pattern kaafi; use = recall-ANCHOR, na ratta.
```

---

## 5. METHOD (kaise karna — per subject, condensed)
```
   DSA        : struggle-first → hand-trace → (stuck) editorial → COLD-REDO spaced (1/3/7). stub = PROBLEM+TESTS. interviewer-brainstorming (Claude nudge, no code). DELETE/restart NAHI.
   HLD/LLD    : DELIVERY-phase → roz ek topic MOCK-TEST (interview-Q) + delivery-coaching. ★ HLD mocks STRONG (payment/URL/LB/rate-limiter — base solid, sirf chhote refinements jaise webhook). LLD = mock + ★ 1-2 design COLD-REDO (HashMap→HashSet jaisa, code-verify). [HLD/LLD = mock; Java/Spring+PROJECT = revise-mode read, no-quiz.]
   PROJECT    : Java/Spring-style REVISE-MODE — Claude ACTUAL LEARNINGS.md content + source-code snippet CHAT me dikhata (fabricate NAHI; code+facts = uska project, story/power-words = packaging jo flag karo), Arpan padh ke COPY pe hand-notes, "aage" bole to agla topic (folder ORDER me). NO quiz. (visual/image → narrate = uski HLD-delivery strength.)
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
