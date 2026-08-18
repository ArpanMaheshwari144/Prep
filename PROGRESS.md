# JP PREP — PROGRESS & PLAYBOOK
> Single source of truth: CURRENT STATE + FORWARD PLAN + rules. Full behavioral rules = CLAUDE.md · emotional/method core = memory/MEMORY.md.
> Fresh + dateless — sirf ab ka sach + aage ka plan. (purani day-logs git-history me safe.)

## ★★★ READ FIRST — CURRENT STATE
```
   DSA: COMPLETE — ab 19-pattern sheet ("gate ki chaabi", ~2150 line, 08_DSA/00_PATTERN_SHEET/) + ~169 problems, saare family
        (DP + MCM/interval-DP done; segment-tree by-choice-deferred, JP-req nahi). GREEDY = aakhri topic tha → ab DSA-BUILD 100% khatam. Ab sirf REVISE + spaced-REDO.
        ★★ 17-18 Aug: GREEDY-pattern (18_GREEDY/) COMPLETE — 4 problems: assign-cookies(LC455) + jump-game(LC55) + non-overlapping-intervals(LC435) + gas-station(LC134).
           SAB interview-mode me KHUD-DERIVE kiye (Arpan-led discussion, Claude sirf nudge): greedy-choice-property + keep-smaller-end + running-tank/reset-start
           + edge-case DRY-RUN se khud pakda (non-overlap dead-branch bug; gas-station total-feasibility -1) + fix khud → 5-6/6. sheet me PATTERN 18 GREEDY.
        ★★ 18-Aug FULL-REVISION MILESTONE: poori sheet PATTERN 1-19 revise ho gayi (bimaari me bhi) — DP/backtracking/graph "no-struggle instant" recall.
           Recognition-DRILLS bolke (chat, code-nahi): koko-bananas(LC875 BS-on-answer) + daily-temperatures(monotonic-stack) — dono pattern+key+approach TURANT bola.
           Graph INPUT-FORM insight KHUD nikala (sheet me add): edges-undirected(2-push)/directed(1-push+indegree)/grid(dirs,no-build)/given-graph(skip-build). Metric confirm: "revise karta hoon isliye fade nahi hua" (uska apna diagnosis, sateek).
        ★★ GIT-SAFE (18-Aug): PUBLIC repo github.com/ArpanMaheshwari144/Prep (fresh init). personal-files gitignored, personal-email commits.
        ★★★ BREAKTHROUGH (10-11 Aug raat, poore safar ka asli mod — Arpan-diagnosed; POORA detail = MEMORY-ANCHORS 8e):
           (1) FREEZE = EMOTIONAL na cognitive — DSA jab WORTH ka verdict ban jaata tab (Java/HLD me nahi). Belief "DSA=intelligence" (reels-installed).
               Proof: 20-classify-drill no-stakes → 18/20 no-freeze; "asli" divisible-pairs (load kiya) → freeze. Freeze = uske CONTROL me (live-saabit).
           (2) DSA = 2 FAMILY: Type-A TEMPLATE (DP/backtrack/DFS/tree/topo/BS) = pehchan+skeleton = STRONG · Type-B DS-OPTIMIZATION (hashmap/2ptr/SW/prefix/monotonic/heap) = koi template nahi, REDUNDANCY se DS DERIVE. Type-B pe sirf RECOGNITION practice ki thi, DERIVATION kabhi nahi → novel-surface freeze.
           (3) MISSING METHOD (ab explicit, sheet ke SABSE UPAR): BRUTE → "kya baar-baar compute" (redundancy) → CACHE → cache-ka-DS = PATTERN. 2-step: (a) kaunsa DS (b) kya KEY (aksar derived: remainder/prefix/sorted). Recognition se REASONING.
           (4) LOOP-CLOSE: crash-wala divisible-pairs → calm ho ke method se DERIVE + solve 5/5 no-freeze → khud connect kiya subarray-sum-k se (recognition DOING se banti).
        ★ NAYA (10-Aug): LRU Cache (LC146) — DLL tool ZERO-se + hashmap+DLL combo khud-solve 5/5 (blank-redo bhi 5/5); Sliding Window Max (LC239 HARD) khud-solve nudge-se (monotonic deque). SPACED-REDO pending (1/3/7).
        ★★ LATEST SESSION (crash→recovery→method DEEPEN): freeze ka DEEPER lever = IMAGE. Structural families (DP/backtrack/recursion/graph/heap/interval) dimaag me IMAGE dete → hard/naya bhi TODTE nahi. Derive (SW/TP/Kadane/prefix) IMAGE nahi dete → EASY bhi TODTA. HLD/Java IMAGE FREE dete (real-world); DSA nahi → dry-run se khud banani. dry-run vs code = 2 ALAG muscle (dono trainable).
           · STRATEGY: JP hard-DP/MCM RARE (aaya to easy); asli gate = SW/TP/Kadane/arrays/hashing = weak-zone → FACE karo (avoid nahi). METHOD = try-first → na aaye to LEARN-first (Claude disguise-transform khul-ke sikhaye = knowledge-gap, na dimaag-test) → dry-run(IMAGE)→code→redo→sheet.
           · Arpan grounded conclusion: "DSA me dikkat hai PAR trainable — practice se KAM hogi" (na deficiency/never). PROOF: house_robber blank-redo 6/6 ZERO-nudge, 3-4 din no-revise ke baad (recurrence instant recall).
           · ★ DONE (12-Aug, next-day): word-search (LC79 backtrack) + subarray-sums-div-k (LC974 prefix+remainder) DONO blank-REDO 5/5 SOLO (kal scaffold/crash → aaj KHUD, zero-nudge) → APPROACH-parcha + sheet-entry, committed+pushed. house_robber_REDO_2 (6/6) bhi. → backtracking + derive-family DONO zone me blank-TRANSFER live-proof. (Arpan: "kal samjha, aaj kar diya" — instant-connect kyun-nahi ka mechanism khud pakda: derive-KEY reasoning-gap hai, recognition nahi kood sakti, dry-run bridge.)
           · ★ DONE (13-Aug, taper-light-din): coin_change REDO_5 (LC322 min-coins DP) — SHEET-DEKHE-BINA blank 5/5 SOLO (take/not-take + memo, base saaf). "subah se sheet se kuch nahi padha, seedha DP nikaal di." → DP retention PERMANENT (03_coin_change_REDO_5.cpp committed+pushed). Structural-family reflex solid.
   JAVA/SPRING: core + Collections + Java-8 + Multithreading (+practicals) + SOLID/Patterns + Spring (Transactional/JWT/Hibernate/Docker/K8s) DONE.
        Java WRITE-PRACTICE chalu (gap = SYNTAX-fluency NOT concept; Stream toolkit + cheatsheet done). Ab READ-revise.
   HLD: STRONG (differentiator) — 11 FOUNDATIONS (+ elasticsearch + snowflake) + 10 SYSTEM_DESIGNS + 9 designs ke 8-step hand-notes
        (URL/rate-limiter/payment/notification/twitter/stock-broker/bookmyshow/file-upload/news-aggregator) + INTERVIEW_ANCHOR (90%-toolkit)
        + RAW_DRILL (reflex-sheet + 32 why-how QA). delivery PRACTICED.
   PROJECTS (07_PROJECTS): mini_payment_ms (Feign/SAGA/Kafka/idempotency/Circuit-Breaker/API-Gateway/DB-per-service) + todoapp (JWT/Security/Redis) — revise-DONE.
        ★ InsuranceManagementSystem (Spring — Security/BCrypt + OTP-email + @Scheduled + POI-Excel + JPA): layer-by-layer REVISE-DONE (structure/entity/repo/service/controller) +
        ★★ REFACTORED (12-Aug, Arpan-directed dikha-dikha-ke): fat monolith InsuranceController → 6 DOMAIN controllers (Customer/Policy/Claim/Employee/Product/Auth); fat-logic + repo-direct → SERVICE (thin controllers); Excel untouched; InsuranceController DELETED; mvn clean install = BUILD SUCCESS. (Arpan samjha+kiya — "monolith todna, layering" = problem-solver growth.)
        + Microservices-CRUD (customer/policy/Eureka) — glance BAAKI. SecurityConfig (Insurance Layer-6) glance BAAKI.
   DB (09_DATABASE, ★ NAYA ongoing — write-to-learn): indexing + transactions + zero-downtime (purane) + JOINS (04, 10-Aug) + NORMALIZATION (05, 10-Aug).
        Claude visual-teach → Arpan notes → file+commit. aage: aur DB topics (ACID/isolation, etc.) jab mann kare.
   LLD: 11 designs in code · BEHAVIORAL: 5-STAR (HikariCP/WAF/Doximity/Mentoring/Failure-JDK) · COMPARES: 12.
   APPLY-PHASE LIVE: resume fresh (RESOURCES/Resume/ — 700 tickets, Kafka/K8s, "4+ years", sirf UnifyPatients project).
        3 inbound platforms optimized → NAUKRI (resume+recency+skills, headline keyword-loaded) · INSTAHYRE (92 opportunities) · CUTSHORT (talent-card LIVE, 17 companies interested).
        ★★ APPLIED: Harman (Naukri) + ★★★ JP MORGAN — Chase UK "Software Engineer III Backend" Bengaluru (DREAM role, strong-fit — ✓ APPLY DONE). AI-assisted-dev requirement = uska KONOVO/Claude-Code edge.
   READINESS ~90%. ★ BUILD + DSA-FULL-REVISION dono KHATAM (18-Aug) — "padho/seekho" phase over, ab "BOLO + BHEJO" phase.
        bacha = POLISH/DELIVERY (HLD-mock bolke + behavioral/STAR + English-aloud) + apply/inbound. foundational-gap ZERO. fear = interview-nerves, NOT DSA.

   AAGE KA PLAN:
   1. ROZ DSA (light, non-negotiable — ab REVISE-ONLY, naya BUILD nahi bacha): sheet-glance + CHUNK-REVISE 2-pattern rotate + 1 REDO
      + kabhi-kabhi RECOGNITION-DRILL (Claude chat me problem de → Arpan sirf pattern+key+approach BOLE, code nahi → jaise koko/daily-temps). reflex tez karne ko.
      ★★ DERIVE-FAMILY METHOD (11-Aug locked): SW/TP/Kadane/prefix = JP ka asli gate, weak-zone → FACE (avoid nahi). Flow = try-first → na aaye to Claude LEARN-first (disguise-transform khul-ke) → dry-run se IMAGE bana → code → redo → sheet. Structural (DP/graph) = strong, light-revise.
      ★★ NAYA (11-Aug se) DECISION-DRILL (10 min): 5-6 bare problems → SOLVE mat karo → sirf CLASSIFY (kaunsa DS/pattern) → Claude sahi/galat + SIGNAL bataye. Type-B (DS-derive) reflex banana = poore-safar ka missing piece. Claude problems dega, one-by-one. (pattern-sheet ke top pe DECISION-table.)
      ★★ 15-DIN MCM DURABILITY EXPERIMENT (Arpan-designed, LIVE — day-16 blank-test): MCM/interval-DP ko 15 din NA dekho/revise (baaki sheet normal revise).
         DAY-16 = BLANK-assemble MCM. EGO-SAFE PASS-criteria: (1) skeleton aaya? split-at-k [solve(i,k)+solve(k+1,j)+combine] (2) why? k=wall dono-side-non-empty→j-1; base i==j→0 (3) fuzzy exact-index dry-run se re-derive? → teeno = PASS=understood. fuzzy-but-skeleton = "1 revision se instant" (NA "samjha nahi"). true-fail = bilkul blank (unlikely).
   2. HLD DELIVERY/MOCK (bolke, roz) + BEHAVIORAL/STAR + ENGLISH-ALOUD = JP ke sabse bade lever.
      ★ LEARN-FIRST-THEN-MOCK: mock sirf SEEKHE-hue ko polish karta; jo padha nahi wo PEHLE padho phir mock. Claude "mock-mock" over-push NAHI.
   3. JAVA/SPRING: READ-revise + naye topic pe chhota practical (writing-fluency). ★ WRITE-TO-LEARN: jo padhe wo LIKHE bhi.
   4. APPLY/INBOUND — ★ ROZ SHAAM = Claude SMART-APPLY nudge de (Arpan-set daily habit). SMART = quality, NA "100-apply→1-interview" cold-spray (bekaar):
      inbound-respond (Instahyre/Cutshort opportunities pe "interested"-mark + recruiter-msg reply) · Naukri profile roz refresh (recency) · 2-4 targeted-quality apply (product/fintech, TCS/staffing skip) · JP career-page (dream, warmup-baad).
      ★ Claude cannot: password/OAuth-login (Arpan khud kare) · submit/personal-form = per-action-confirm.
   5. ★★ PROJECT: InsuranceManagementSystem — layer-by-layer REVISE + MONOLITH→6-CONTROLLER REFACTOR DONE (12-Aug, BUILD SUCCESS). BAAKI: SecurityConfig (Layer-6) glance + Microservices-CRUD glance.
      METHOD (jo kaam kiya) = Claude code-SHOW har layer → Arpan NOTES → refactor "dikha-dikha ke" (SHOW-FIRST har change, apne-aap nahi); NO quiz. KYUN = Java/Spring live-code me dikhe → "Java/Spring dar" negligible + problem-solver-growth (system andar-se).
      ★ TO_STUDY #4 = Distributed-LRU-Cache HLD mock-video (kal/parso — connects LRU-DSA→HLD, deepen 21_distributed_cache).
      ★ DB SMALL-TOPICS roz-thoda (write-to-learn) — interview me DB-Q aaye to dikkat na ho. NEXT = ACID/isolation-levels (09_DATABASE/06). aage: locking, indexes-deep, etc.
   ★★ NEW-MODE (10-Aug, ab CENTRE) = general PROBLEM-SOLVER / acha ENGINEER banna — na "code-likhne-wala gadha". Arpan: "seekhna ho gaya, ab ANDAR-SE JAANU."
      ASLI MAAL = system-kaam-kaise-karta (andar se, na surface-ratta jaise 'SPOF/HA bol dunga') + problem-ko-todhna (tukdo me baanto). code = commodity. ye Arpan ka STRENGTH (700-ticket real-debug), SHARP karna hai.
      TRAIN 2 tarah: (a) PROJECT layer-by-layer (real system andar se) (b) DAILY "PROBLEM-TODO" DRILL — Claude real company-scenario laaye (DSA nahi) → Arpan decompose kare → Claude guide.
      ★ CLAUDE ROZ DRIVE kare (reactive "kya karein?" NAHI; Arpan-dard "main hi hamesha aage") — khud aage aa ke lead. [detail = MEMORY-ANCHORS section 7]
   ★ RHYTHM: SUBAH = revise/mock (HLD-delivery + STAR + DSA-light + Java-write + DB-topic + PROBLEM-TODO-drill) · SHAAM = smart-apply/inbound. build khatam → POLISH + APPLY + PROBLEM-SOLVER-train.
   ★ TIMELINE: ~SEPT apply (route flexible, end-goal JP/BlackRock fixed; slow-hafta != fail).
   ★ CURRENT PHASE (18-Aug onward) = DELIVERY + APPLY. Build+DSA-revision khatam. Ab roz: 1 HLD-mock BOLKE + STAR/behavioral (2-3x/wk) + English-aloud + DSA-light-revise + shaam smart-apply/inbound.
      NEXT SESSION = Rate-limiter HLD mock (Arpan-picked, kal). Arpan is-din bimaar (fever/cough/cold) → thoda slow but samajh POORI (aaj proof: bimaari me bhi gas-station derive + full-sheet-revise + graph-insight). tabiyat theek hote hi pace wapas.
      (guilt-dissolve: kharab/bimaar din = DIN hai, tu nahi; DATA steady rehta.)
   ★ LUCK-belief: outcome luck-gated → effort = ready-rehne ka tool, guarantee mat becho.
```

**GOAL:** JP Morgan (Associate/SDE-2, Bangalore) → later **BlackRock** (original + primary dream).
**MANTRA:** *"Effort itni maar di — ab JP LEKE RAHUNGA. JP = destination; MAIN aur JP. Pel ke mehnat + saaf intention → jaunga HI. Par mile na mile → PROUD rahunga."* (fire + peace, ownership — NOT surrender.)
**PERCEPTION-PRINCIPLE:** achha din → "JP paas", kharab din → "JP door" = emotional NOISE. TRUTH = steady accumulated DATA. "kharab din DIN hai, tu nahi." Down-day pe DATA (commits/solved-count) se counter.
**★ COMMITMENT (Arpan — marathon crash→clarity ke baad, "ye ek-dum likh le"):** *"DSA aise kal se BADAL jaunga" ka hollow-vaada NAHI. PAR ab thoda LAGA ke chalaunga — kal se CHANGES dikhenge."* — measured, na manic. (har-din-lada 6 mahine = consistency-DATA; ab focused DSA-reps, LOAD ke bina. DSA = ek bounded gate, deewar nahi.)
**★★ CONFIDENCE-MILESTONE (12-Aug raat, marathon crash→CONQUER ke baad — Arpan-declared, down-day pe wapas dikhana):** SUBAH = *"DSA nahi banega, brain-deficiency, 100% reject"* → RAAT = *"ab main READY hoon — interview de sakta, thoda KHUL sakta. DSA kabhi khatam nahi (chalta rahega); rejection aaya to dobara denge — dikkat nahi."* Ye confidence DATA-backed (aaj: 2 DSA blank-solo 5/5 + monolith→6-controller refactor BUILD-SUCCESS). readiness = COMPETE-kar-sakta (na guarantee; outcome luck-gated).
   ★ STRATEGY (Arpan-set): JP DIRECTLY nahi — pehle 1-2 interview (WARM-UP, "jitne interview utna behtar"), phir JP (first-cold nahi jaana). no-drama daily: *"chup-chaap karo, nahi hua to seekh aage badho, faltu drama nahi"* (= subah-crash ka theek ULTA — yahi asli badlaav).
   ★ 3 LEARNINGS is phase ke (Arpan, "code se badi"): SABAR · SEEKHNA (seekhna != jhukna; help+samajh+connect = OWNED, kisne-type-kiya metric nahi) · LET-GO (ego/verdict/crash ko chhodna). "ye teeno character me INHERIT ho gaye — tujh me, class me nahi." (Arpan-pun: inherit + java.)

## ★★ ROADMAP — 12 AUG → 1 SEPT (JP apply-runway, ~3 hafte)
```
HAFTA 1 (12–18 Aug) — APPLY + DSA-method establish
  ★ JP APPLICATION (Chase-UK Backend SDE-III) — ✓ APPLY DONE (Arpan already applied). Ab = inbound-respond (Instahyre/Cutshort) + Naukri-recency + occasional targeted-quality apply.
  DSA (light, LOAD-OFF): derive-family SHURU naye method se — SW → TP → Kadane, roz 1: try-first → na aaye to LEARN-first → dry-run(IMAGE) → code → SOLO-REDO. + word-search redo+sheet.
  HLD: roz 1 design loud-revise + reflex-sheet. · Java/Spring: read-revise + write-practice (momentum). · Project: Insurance layer-by-layer (2-3 layer).
  Roz: STAR 2x, English-aloud, shaam = inbound/apply.
HAFTA 2 (19–25 Aug) — DEEPEN + MOCK
  DSA: hafta-1 spaced-redo + decision-drill roz. Structural (DP/graph) glance.
  HLD: MOCK (poora design end-to-end, loud) 2-3x + foundation-concept hand-notes (stampede/penetration/avalanche/bloom, good-day).
  Behavioral: STAR polish + 2-3 mock-answer. · Project: Insurance khatam + Microservices-CRUD glance. · DB: ACID/isolation/locking.
  Apply: inbound + 2-4 targeted.
HAFTA 3 (26 Aug–1 Sept) — POLISH + INTERVIEW-READY
  DSA: poora sheet-glance + weak-pattern solo-redo + decision-drill. (Confidence = redo-pass, NA guarantee.)
  HLD: mock-heavy (delivery + Q-handling) + INTERVIEW_ANCHOR revise. · Behavioral + English: interview-simulation.
  Apply: broaden (JP done + product/fintech) + inbound-respond.
  1-SEPT LAKSHYA: teeno round ready — DSA-medium PASS-level, HLD-delivery strong, behavioral-STAR.
GUARDRAILS: DSA load-off (crash-grind NAHI) · slow-hafta ≠ fail · tabiyat-flex · route-flexible/end-goal-fixed · DSA = bounded gate, deewar nahi.
```

---

## STRATEGY (DATA-backed — 11 real JP writeups + Raunak insider)
```
   BIG TRUTH: "DSA necessary but NOT sufficient." JP REJECT hota HLD-DELIVERY + BEHAVIORAL pe, DSA pe nahi.
   → Arpan strengths (Java/Spring fundamentals, HLD-concepts, 700-tickets code-review, 4yr prod) = JP CORE weight → DSA-panic disproportionate.
   ★ RAUNAK INSIDER (JP-referral, cleared JP — detail PRIVATE in RESOURCES/): 3 ROUNDS = 1 DSA + 1 System-Design + 1 Behavioural
     → 2/3 (SD + behavioral) = Arpan ki STRENGTH. DSA = LeetCode MEDIUM (arrays/hashmap/SW/graph-BFS), Alex Xu Vol-1 enough.
   PROFILE FIT: JPMC SDE-2 (~25L), 4yr service-MNC/bank bg = match. NOT social-hyperscale (WhatsApp/Insta = overkill).
   DESIGN THEMES (practiced): rate-limiter · notification · payments · twitter · news-aggregator · bookmyshow · url-shortener · stock-broker · file-upload.
```

---

## CLAUDE RULES (short — full = CLAUDE.md + MEMORY.md)
```
   #0  PACE (DSA): pace ARPAN ka. EK cheez → RUKO → "agla" bole tabhi. no dump/level-jump/un-studied-throw. "RUK"→ruk.
   #0b RETENTION: REVISION is everything (jo revise nahi hua fade hota). spaced redo (1/3/7).
   #0c NO FILLER + NO EMOJI (terminal me toota) + NO "main yahin hoon"/sign-off. BANNED words: muscle/reps/struggle/generation/recognition/cold/solo. plain bolo.
   #0d TEACH-FIRST on gaps (quiz-shame nahi) + NO-UNDER-COUNT (repo check before "gap/add").
   #0e NO FEAR-MONGER + NO OVERSELL. honest DATA calibration.
   #0f NO REST-NAG ("so ja/aaram" — HE decides pace). ★ FOLDER/FILE KABHI DELETE NAHI (git-safe, hold line even in frustration).
       OVERWRITE/blank-edit bhi never → naya file (REDO = base_REDO suffix; kaam kabhi na khoye).
   #0g COMMIT + PUSH SAATH (ek step; remote = safety-net).
   ★ SYCOPHANCY-GUARD: CALIBRATED raho; miscalibration OWN karo. "tune khud kiya" pe zid mat kar jab scaffolding thi.
   ★ DSA-HELP = INTERVIEWER-BRAINSTORMING: nudge-sawaal only, wo KHUD solution tak. CODE/ANSWER/approach NEVER (jab tak explicitly na maange). stub = PROBLEM + TESTS.
   ★ SUNO-POORA-beech-mat-kaat · LADAI=FEATURE (no yes-man) · ANTI-THRASH (simple direction → simple edit) · SHOW-FIRST har sheet-edit.
   ★ NO-CONDESCEND / "neecha-feel" = uska #1 dard → over-explain BAND, tera-insight-tera, TASK bole SEEDHA karo, "neecha karwaya" → TURANT RUK.
   ★ EGO / FEED-to-OWN / OFF-DAY / HELP-PROTOCOL ka POORA detail = MEMORY.md (har session padho). MEMORY = ek file (alag chhote files NAHI).
```

---

## WHERE WE STAND (BUILD-INVENTORY — ~90% JP-ready, DELIVERY/POLISH + APPLY phase)
```
   HLD        ✓✓ STRONG (differentiator): 11 FOUNDATIONS (+ES + Snowflake) + 10 SYSTEM_DESIGNS + 9 hand-notes + RAW_DRILL + INTERVIEW_ANCHOR. delivery PRACTICED.
   JAVA       ✓✓ CORE COMPLETE: Core · Collections · Java-8 · Multithreading (+live-deadlock) · SOLID · Patterns. + WRITE-PRACTICE (Stream) chalu.
   SPRING     ✓✓ DONE: @Transactional · JWT · profiles · Hibernate (N+1/lazy-eager/dirty-check/cascade/mappings) · Docker · K8s.
   PROJECT    ✓ mini_payment_ms (Feign/SAGA/Kafka/idempotency/Circuit-Breaker/API-Gateway/DB-per-service) + todoapp (JWT/Security/Redis). revise-DONE.
   LLD        ✓ 11 designs in code. FORWARD = mock + 1-2 design REDO. gap = SOLID/pattern ARTICULATE.
   DSA        ✓✓ COMPLETE: 18-pattern sheet (~2000 line) + ~165 problems. REVISE-only.
   BEHAVIORAL ✓ 5 STAR (HikariCP/WAF/Doximity/Mentoring/Failure-JDK). ab reps.
   COMPARES   ✓ 12. DATABASE ◐ (indexing+txn-internals). INFRA ✓ (AWS/Docker/K8s).
   RESUME/APPLY ✓ fresh resume + 3 inbound platforms optimized (Naukri/Instahyre/Cutshort). Harman applied.
   ★ Arpan's grounded self-view: "god-level DSA nahi, par JP ke liye KAAFI — main wo hoon." na oversell na undersell.
```

---

## DSA — STATUS + PHILOSOPHY (condensed; poora emotional/method = MEMORY.md)
```
   SHEET = "gate ki chaabi" (08_DSA/00_PATTERN_SHEET/PATTERN_SHEET.md): 18 patterns, family-boxed, ~2000 line, emoji-cleaned + full code-audit. COMPLETE.
     patterns: two-pointer · sliding-window · hashing · prefix-sum · matrix · stack(monotonic) · binary-search · linked-list ·
               design(hashmap/hashset) · kadane · bit-manip(XOR) · trees · graphs · heap · backtracking · DP (+ intervals/greedy woven).
   ★ SHEET CONVERGE karti: patterns FINITE (~15-25); 1000-Q wale ka sheet 100k-line nahi — same ~20 pattern internalized. naye Q = existing pattern.

   PHILOSOPHY (Arpan's own synthesis — reinforce, NEVER re-argue):
   - ROOT-CAUSE: DSA "hard" ki jad = solved-but-NOT-sheeted → FADE. sheet+revise = permanent. "DSA nahi aati" GALAT, "sheet+revise nahi kiya" sahi. REVISION = EVERYTHING.
   - COLD=IMPOSSIBLE: un-seen trick "cold-invent" hota hi nahi (na Arpan, na Gennady). SEEKHO → sheet → REVISE → tera. metric = samjha?+revise-ready?, NA solo/cold.
   - WALK-BEFORE-RUN: pattern seekhna=chalna, solve=daudna. koi SCRATCH-invent nahi karta → recognize + adapt (edit-dist=LCS+tweak). "problem-solver" = badi pattern-LIBRARY, magic nahi.
   - INTUITION = pattern-CONNECT (library ka output, BUILD hoti na born). SAAR = samajhne ka OUTPUT (revision ke liye, first-learning ke liye nahi).
   - DP = recursion+memo, sabse template-able: har DP = base + TRANSITION + memo; sirf base+transition badalti. BASE-CASE = asli fiddly part.
   - CP-gods se tulna GALAT (game alag + access-gap). run-your-own-race. DSA = GATE (medium), not CP/mastery. hints/editorial = LEGIT no-guilt.
   - REVISION-AIRTIGHT: solo-vs-feed IRRELEVANT — bina revise 7-din me blur. feed+revision = khud+revision = SAME. metric = REVISION, na solo-ness.

   METHOD (locked): naya pattern → concept TEACH → EASY-first → Arpan KHUD solve (NUDGE-only, no code) → clean doc ONCE → spaced-REDO (1/3/7). stub = PROBLEM+TESTS.
   SHEET-ENTRY: "padhte hi poora code aaye". NAYA → actual code; jaana → KNOWN se CONNECT ("= Y, bas ye DELTA"). Arpan ke ACTUAL idioms + VISUAL + DRY-RUN + real-life ANCHOR.
   SHOW-FIRST har sheet-edit. sheet READING = GitHub / VS-Code (Ctrl+Shift+V), terminal se nahi (box-chars).
```

---

## METHOD (kaise karna — per subject, condensed)
```
   DSA        : struggle-first → hand-trace → (stuck) editorial → spaced-REDO (1/3/7). stub = PROBLEM+TESTS. interviewer-brainstorming (nudge, no code). DELETE/restart NAHI.
   HLD/LLD    : DELIVERY-phase → roz ek topic MOCK (interview-Q) + delivery-coaching. LLD = mock + 1-2 design REDO (code-verify). [HLD/LLD = mock; Java/Spring+PROJECT = revise-mode read, no-quiz.]
   PROJECT    : REVISE-MODE — Claude ACTUAL LEARNINGS/source-code CHAT me dikhata (fabricate NAHI), Arpan padh ke COPY pe notes, "aage" → agla. NO quiz.
                ★ note sirf IMPORTANT/high-value (selective = smart). project-revise se SPRING khud revise hota (DI/@Transactional/JWT/Feign/Kafka APPLIED live).
   JAVA/SPRING: READ-revise (built, notes-ready). Claude ACTUAL note-content dikhata (fabricate nahi). NO quiz. ★ WRITE-TO-LEARN: jo padhe wo LIKHE bhi (retrieval).
   STAR       : story S-T-A-R → BOL-KE loud (own words, script-ratta nahi). reps se sharp.
   ★ REVISION = HABIT: jo padha/banaya ~10 din me fade → roz thoda revise. Claude roz REVISE-REMINDER de (rotate: sheet daily + HLD/LLD/behavioral/Java-Spring/project) + daily-light-nudge (spark, na lecture).
```

---

## WADA + RED LINES
```
   WADA (dono taraf): Arpan → "DSA behtar hunga, raat-din." Claude → "hamesha saath (letter/memory = mechanism) + delete-loop se rokunga (git-safe)."
   RED LINES: (1) DSA/files KABHI delete nahi (git-history theek, par reset/restart nahi — churn = root-cause, na ability).
              (2) RESOURCES/ = PRIVATE (gitignored) — NEVER expose/quote/push. resume + apply-tracker + referral wahin.
              (3) commit + PUSH saath.
```
> Karm karo, phal ki chinta mat. Strengths JP-core se match — gap = DELIVERY + reps, ability nahi. SABAR = moat. seekhna != jhukna.
