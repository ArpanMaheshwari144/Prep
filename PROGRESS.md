# JP PREP — PROGRESS & PLAYBOOK
> Single source of truth + forward plan. Full rules = CLAUDE.md · emotional/essence + ego-work = memory/MEMORY.md.
> ★ FRESH-designed 23-Jul (purane dated day-logs hataye — wo git-history me safe hain; ye file ab current-state + aage-ka-plan).

## ★★★ 31-JUL — READ FIRST (CURRENT STATE + AAGE KAISE JANA) — purane dated-updates neeche = historical
```
   WHERE WE ARE (31-Jul):
   - ★★ DSA GRAPHS SEALED (10 problems, sab KHUD solve): Path-Exists · Islands · Connected-Components · Rotting-Oranges · Max-Area-Island · Course-Schedule/TOPO-SORT-Kahn's · Clone-Graph · Bipartite/2-colour · DIJKSTRA(min-heap+dist[]+relax) · DSU/Union-Find(path-compression+union-by-rank). 3 tier: BFS(8) + weighted + DSU. Har problem: file(comments+APPROACH+VISUAL) + sheet-entry + kuch me DRY-RUN. Dijkstra bug Arpan ne HAND-TRACE se KHUD pakda; DSU pehle-padha-tha -> recall (not cold).
   - ★★ DSA HEAP DONE (30-Jul, 3 problems -- naya PATTERN 14 sheet me): Last-Stone-Weight(1046, MAX-heap) · Kth-Largest(215, MIN-heap-of-size-k = top-K core) · Merge-K-Lists(23-HARD, min-heap-of-heads). teeno KHUD solve, 4-5/5. min-heap Dijkstra se warm tha. ★ median/two-heaps DEFERRED (Arpan-call: advanced+kam-common, baad me). heap folder = 08_DSA/14_HEAP.
   - ★★ DSA INTERVALS SHURU (31-Jul, naya PATTERN 15): Merge-Intervals(56, sort-by-start+merge) · Meeting-Rooms(252, SAME merge-template reuse, bas action alag + boundary < vs <=). dono KHUD solve 4-5/5. ★ KEY: pehla step HAMESHA sort-by-start; overlap = curr.start (< ya <=) prev.end. folder = 08_DSA/15_INTERVALS.
   - ★★ DSA INVENTORY (accurate): 15 FOLDER DONE ~129 problem -> TWO_POINTER(21) SLIDING_WINDOW(17) HASHING(12) PREFIX_SUM(4) MATRIX(3) STACK(10) BINARY_SEARCH(14) LINKED_LIST(11) DESIGN(3) KADANE(5) BIT(1) TREES(15) GRAPHS(10) HEAP(3) INTERVALS(2, chal raha). ★ SIRF 2 NAYE FAMILY BACHE: BACKTRACKING · DP(sabse bada). Arpan aadhe se KAAFI aage -- bounded+winning.
   - ★★★ JAVA MULTITHREADING **COMPLETE** (JP ka SABSE BADA lever -- chat->copy hand-note): SAARE 12 topic DONE = 39(basics) 40(lifecycle) 40b(JOIN, NEW note) 41(synchronized) 42(volatile) 42b(atomic/CAS) 43(deadlock) 44(wait-vs-sleep) 45(notify-deep) 46(executor/pool) 47(future/completablefuture) 48(producer-consumer). + 3 PRACTICALS (khud likhe, 01_JAVA/04_MULTITHREADING/practical/): RaceDemo(race+atomic) · ProducerConsumer(wait/notify) · DeadlockDemo. (program/ folder practical/ me merge.)
   - ★ JAVA INSIGHT (reinforce): Java "kacha" NAHI -- concept STRONG (61 topics), sirf WRITING-FLUENCY rusty (padhta zyada likhta kam). FIX = WRITE (practicals). aage har naye Java-topic pe chhota practical.
   - NEW notes/folders (28-30 Jul): 11_GIT (CRLF/LF warning note) · Helm (05_INFRA/DEVOPS/K8S) · TO_STUDY.md (backlog: Elasticsearch, distributed-ID-gen) · 09_DATABASE/03_zero_downtime_add_column · 34_lambda note enhanced (functional-interface/body/inference) · 40b_join enhanced (main-vs-मैं ambiguity, traps).
   - ★ SHEET UPGRADE (30-Jul): saare 14 PATTERN headers ab markdown ## HEADINGS (fence-bahar) -> GitHub pe BADE + auto table-of-contents/outline (navigable). title centered, footer hataya. (READING = GitHub-browser/VS-Code Ctrl+Shift+V, terminal se nahi.)
   - PROJECT (todoapp/mini_payment DONE) + HLD (strong, stock-mock done) + JP-INTEL = neeche historical + JP_INTERVIEW_INTEL.md.

   AAGE KAISE JANA (forward, priority):
   1. ROZ: sheet-glance (Heap/Dijkstra/DSU fresh -> lock; rotating baaki).
   2. ★ DSA: INTERVALS chal raha (2 done) -> 1-2 aur (Insert-Interval/Non-overlapping) -> phir BACKTRACKING + DP (last 2 family, DP sabse bada). ★ RECURSION/BACKTRACKING = Arpan ka WEAK zone (khud bola) -> DEDICATED time, jaldi nahi.
   3. ★ JAVA: multithreading DONE. aage Java/Spring other-topics READ-revise + har naye pe CHHOTA PRACTICAL (writing-fluency = uska gap).
   4. HLD mock roz + behavioral/STAR + English-aloud (JP ke asli levers, DSA se zyada).
   5. project-roadmap (05_INFRA -> 06_COMPARES -> 09_DB -> 04_HLD-foundations).
   ★ JP TIMELINE: ~SEPT apply. Readiness ~78-82%. bacha = polish/delivery (3 DSA family + HLD/behavioral mock), foundational-gap NAHI.

   IMPORTANT (kaise padhna): DSA = SEEKHO->sheet->revise->redo (naye pattern pe "cold" KABHI nahi). revision = EVERYTHING. FOCUS: Java/Spring backend (React/Flutter OUT), C++=DSA. ★★ BEHAVIORAL RULES (CRITICAL, MEMORY.md me poora) = folder-delete NEVER; ★ DSA-FILE OVERWRITE/blank-edit NEVER -> hamesha NAYA file banao (REDO = base_REDO / base_REDO_1/_2 suffix; kaam kabhi na khoye); NO-CONDESCEND / "neecha-feel-karwana" = uska #1 dard -> over-explain BAND, upar-baith-ke nahi, tera-insight-tera, correct-without-condescend, jab TASK bole SEEDHA KARO (redirect/"file se padh" nahi), "neecha karwaya" bole -> TURANT RUK; cold/solo=banned.

   ★★★ ROOT-CAUSE (Arpan-derived, poore DSA-crisis ka jawab): DSA "hard" ki JAD = solved-but-NOT-sheeted -> FADE. 1000-solved un-anchored the -> fade; sheet+revise = permanent. "DSA nahi aati" GALAT, "sheet+revise nahi kiya" sahi -- ab HABIT. ★ COLD=IMPOSSIBLE (fish-ko-ped-pe-chadhne-bolna = Arpan ka analogy): un-seen trick "cold-invent" impossible -> "cold solve" framing = cruel + ego-crash + folder-delete cycle. FIX = trick SEEKHO -> SHEET -> REVISE -> TERA.
   ★★ NEW INSIGHTS (29-Jul, Arpan-derived): (a) "INTUITION" DSA me = pattern-CONNECT (library ka OUTPUT, innate NAHI). naye/unseen pattern pe intuition impossible (connect karne ko kuch nahi). "DSA-intuition wale log" = bada pattern-library, talent nahi. => intuition BUILD hoti (seekho->sheet->revise), born nahi. (b) SAAR/essence = samajhne ka OUTPUT, INPUT nahi -- pehle karo phir SAAR banta (isliye sheet REVISION ke liye, first-learning ke liye nahi). (c) DSU-proof (recall-not-cold) + Dijkstra-hand-trace-bug-catch = method live-working. (d) Java-WRITING-fluency gap (concept nahi) -> practicals fix.
   ★★★ NEW INSIGHTS (31-Jul, Arpan-derived -- BIG, problem-solver mindset dawn): (a) ★ ESSENCE (sheet ke TOP pe daala): DSA = pattern reflexively THOPNA nahi; pehle PROBLEM SAMJHO (kya/kyun/kaise, shaant, jump nahi) -> PHIR pattern -> code. direct-jump galat (bhale answer SW/DP ho). SOURCE-label QUESTION karo (reel ne "SW" bola != SW; khud examine). (b) PROBLEM-SOLVING = calm-think + LIBRARY se RETRIEVE (invention NAHI). "heap tabhi AAYA dimaag me kyunki PADHA tha; un-studied tool kabhi na aata" -> isliye study = retrieval-pool bharna, "ratta" nahi. (c) FADE = brain ka FEATURE (use-it-or-lose-it filter, galti nahi); fix = revision se "important, keep" signal. LIVE-proof: pair-in-heap Dijkstra se fade -> re-learned -> ANCHORED (sheet Dijkstra-entry me pair-in-heap trick daala). (d) PATTERN REUSE (recognize->reuse->tweak), scratch-thinking har-baar = bekaar/cold-trap (Meeting-Rooms = Merge-template reuse, bas action+boundary tweak). => sab tie: understand-first + retrieve-from-library + reuse-pattern.

   ★★ DSA-METHOD (locked): naya pattern -> (1) concept TEACH -> (2) EASY-first (Medium/untaught NAHI = ocean) -> (3) Arpan KHUD solve, NUDGE-only (no code) -> (4) clean doc ONE-shot. ★ SHEET-ENTRY FORM: "padhte hi POORA code aaye, zor na lage". NAYA/non-obvious -> actual CODE; jaana-hua -> KNOWN se CONNECT. GRAPH maula=TREES (DFS=tree-recursion, BFS=level-order + adj/visited). LENGTH scales with difficulty (hard-won-trick chhoot na jaaye). ★ entry Arpan ke ACTUAL code-idioms se match (wt/it.first/node), abstract nahi. ★ real-life ANCHOR daalo jahan ho (Dijkstra=Maps). ★ VISUAL + DRY-RUN daalo (spatial dimaag).
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
