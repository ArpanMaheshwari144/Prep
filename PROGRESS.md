# Interview Prep — Progress Tracker

> **Updated:** 26 June 2026 | **Target:** JP Morgan (Associate/SE-III, Bangalore) → BlackRock (4-5yr) |
> **Mode:** APPLY + INTERVIEW + behavioral-STAR (technicals = warm-revise). JP-ready ~95%. Apply LIVE.

```
   GOAL:  Konovo (12L) → Bangalore product/fintech (18-25L) → JPM/GS (26-50L) → BlackRock Aladdin (50+L)
   Refused (hard no): TCS/Infosys/Wipro/Cognizant/Capgemini/KPMG.
   "Effort 100% mera, outcome universe ka. Roz ek kadam. Apne dum pe — BlackRock tak."
```

---

# PLAN: NOW (July, home) → AUGUST (office return)  — BUFFER MONTH

```
   CONTEXT: Konovo RTO July → pushed to AUGUST. July = home ~1 month = BUFFER + RUNWAY.
   HONEST STATE: ~70% (NOT 95, NOT 20). Java/Spring strong; HLD blocks mostly there; DSA gate done.
     GAP = novel/twist HANDLING under pressure + few unknown blocks + nervousness-on-unfamiliar. = TRAINABLE.
   THEME: TEACH-mode → TRAIN-mode. (taught ≠ trained; recognition ≠ generation). Train so NOTHING NEW trips him.
   Sab REMOTE: apply + ZOOM interview (JPMC = Zoom). Family-first, own pace. Roz, no drama.

   1. JAVA + SPRING = REVISION (daily, light ~20-30min): present notes/cards, he re-reads. STRONG → just warm.

   2. HLD = TRAIN to do ANY design ("har HLD kar paau" — NOT rote specific designs):
        a. 8-step FRAMEWORK reflex (every design same skeleton)
        b. BUILDING BLOCKS master cold (what/when/why/tradeoff): LB · cache · DB(SQL/NoSQL) · sharding · replication ·
           queue(Kafka) · CAP/consistency · idempotency · rate-limit · WebSocket/real-time · CDN/streaming · presence
           → NEW block (Netflix=CDN/streaming, chat=WebSocket) = Claude TEACHES block first, then he applies.
        c. VARIED designs (1/day): framework + assemble blocks. known→new. = any design (Netflix/WhatsApp/Uber) assemble, NOT memorize.
        ✓ Finance-flavored focus (JP). ❌ no hyperscale-social cold-lead.

   3. DSA = TRAIN generation (cold reps): daily cold problem (NO teaching, blank page) → solve → learn from miss.
        rotate patterns + [C] revisits (Coin-Change/House-Robber). recognition → generation muscle.

   4. ★ BEHAVIORAL STAR (#1 gap, decisive): Konovo stories S-T-A-R (WAF/X-Forwarded-IP · $20K · 600+ incidents · HikariCP · Lambda)
        + why-JP · manager-disagreement · led-project · initiative · oncall · design-reject.

   5. APPLY = PAUSED (his call, 28 Jun): "kuch nahi aata abhi, pehle train karunga, phir apply." NEW applies STOPPED for ~1 month.
        ONLY in-flight referrals RIDE (Raunak-JP✓ / Anmol-RZP✓ already sent) → call aaye to take (= training rep), else focus training.
        → 1 month deep TRAINING first, THEN reassess + resume apply.

   GOAL (1 month): ~70% → 85-90% (train novel-handling + kill nervousness via familiarity). THEN dekhenge kaha hain.
   His decision: depth-first, one-by-one, dhang se, no drama, daily. (low-confidence-day call — respected; training addresses the real gap.)
```

---

# STRATEGY (data-driven — from 3 real JP SDE-3 Bangalore writeups + research)

**JP REALITY:** moderate (3.1/5), breadth-not-depth, NOT FAANG-hard. The HARDEST + decisive round = **JAVA/LLD** (a MAANG SDE-2 got downleveled on Java-depth + YOE, NOT DSA). DSA = LC-medium + hints. Design = general products via framework. So priority:

```
   #1 JAVA   (deepest — internals/Spring/concurrency/Java21/LLD)   <- JP's hardest + his strength
   #2 HLD    (8-step framework + designs — enough, not asked hard)
   #3 DSA    (gate-maintenance — common LC-mediums sharp; clear OA = 2 mediums; NO hard-variant obsession)
   +  BEHAVIORAL STAR (Konovo stories) + 2-3 hrs finance-domain basics (order-matching/payment/fraud)

   DAILY (firm): roz JAVA + HLD (even one topic each, JP-level) + DSA gate-maintenance. One step for JP every day.
   CONSOLIDATE PHASE (now): ek-do din revise (Java+HLD+DSA), no new DSA, then resume.
   DON'T over-do: FAANG-hard DSA, Graphs/hard-DP (save for BlackRock). JP doesn't go there.
```

---

# JP MORGAN — INTERVIEW PATTERN (verified, 3 real writeups + research)

```
ROUNDS: OA/screen → (Superday) Code-pair + Design-pair + Behavioral. 3-6 weeks. Score → SDE2 vs SDE3.

  1. OA (HackerRank ~60-90min): 2 LC easy-to-medium (Min-Platforms, valid-substrings/sliding-win, Combination-Sum).
  2. CODE PAIR (~45min): PR review (design/SECURITY/SQL-injection/concurrency/logging) + 1 DSA.
       Real DSA seen: Min-Stack, Two-Sum, Level-Order-BFS, Reverse-LL, Trapping-Rain-Water, grid/matrix.
       HINTS given, main-logic often enough (not full code).
  3. DESIGN PAIR (~45min): general product design (file-upload→validate→S3→tracking, News-Aggregator).
       One candidate literally used the 8-step framework (FR→NFR→HLD→DB→APIs).
  4. JAVA/LLD round (the grill, decisive): HashMap vs CHM, concurrency/thread-safety, Spring DI/autowiring,
       Java 8 streams, exceptions (checked/unchecked), Runnable vs Callable, design patterns, project deep-dive,
       RateLimiter-type LLD. THIS is where depth is tested + where the MAANG SDE-2 downleveled.
  5. BEHAVIORAL: "why JPM", "pushback from management", "keep up with tech", STAR.

COMP (4yr Associate/AVP Bangalore): base 18-22L, total 26-33L median, top ~47L.
INSIDER: drop idempotency/audit-trail/ACID in design; PostgreSQL/Oracle for ledgers; honesty on unknown ("don't know, will research").

DIFFICULTY (re-verified 28 Jun, web): MODERATE — "BREADTH under pressure", NOT single-hard-problem. Reason-the-WHY
   (HashMap internals, DI, "kyun kaam karta"). Rounds shift basic-syntax → deeper fast. NOT FAANG-level.
SYSTEM DESIGN = FINANCE-FLAVORED only: reliable + low-latency + secure + consistency/ACID/idempotency.
   ❌ NOT social-media HYPERSCALE (WhatsApp / Instagram / Twitter-scale, persistent-connection real-time) — OVERKILL, JP nahi puchта.
   ✓ PRACTICE: Rate-Limiter, Payment, File-upload, News-Aggregator, Stock-Trading, URL-shortener (all DONE).
   → HLD prep = warm-revise/drill THESE finance-flavored designs. Don't attempt hyperscale-social cold.
```

---

# AT A GLANCE — done vs left

```
   DONE (foundation, no rework):
     Java V90 (110 notes: Core/Collections/Java8/Multithreading/SOLID/Patterns/BrainTicklers)
     Spring V90 (41 notes: @Transactional/JWT/Hibernate/Profiles/Docker/K8s + IoC/DI/autowiring/security)
     LLD 11 problems · Projects (TODO full pipeline JWT/Redis/Docker/K8s/AWS) · Infra 9 · Compares 12 · DB indexing

   HLD: Foundations 10/10. Designs 19 (incl #16 Stock-Broker, #17 Payment, #18 File-upload, #19 News-Aggregator — 8-step interview notes). DRILL-method unlocked.
   DSA: 40 active-derive (SOLVED.md), 50+ coded .cpp. Trees gate-DONE.

   LEFT (priority order):
     1. JAVA deep (concurrency/internals to JP-grill depth) + LLD  ← #1
     2. HLD framework on variety (news-aggregator, more designs) + 1-2 finance-depth (Order-Matching/Wallet-Ledger)
     3. DSA gate-maintenance (revise + common mediums); finish Trees, light Heap/Backtrack
     4. APPLY daily (inbound + referral + Simplify-bulk) + Behavioral STAR prep
```

---

# DSA — STATUS

```
GATE COMPLETE — 46 problems, all JP patterns:
   Hashing · Two-Pointer · Sliding-Window · Prefix · Binary-Search(6) · Stack · Linked-List(5) · Grid
   TREES(+BST): DFS(max-depth/invert/same/diameter-[C]-cleared-cold) · BFS(level/right/zigzag) ·
                LCA · Count-Good-Nodes(#38 COLD-SOLO genuinely-unseen) · Validate-BST(#39) · Kth-Smallest(#40)
   BACKTRACKING: Subsets(#41)/Permutations(#42)/Combination-Sum(#43 JP-OA-Q SOLO) — rep1-help→rep3-solo curve
   HEAP: Kth-Largest(#44 size-K min-heap)
   1D-DP: Climbing-Stairs(#45 memoize) · House-Robber(#46 take/skip [C])
PARKED: Coin-Change [C] (hardest basic DP, didn't click 26 Jun — preserved NOT deleted, cold-revisit fresh day).
SKIP: graphs, hard-DP (JP nahi puchта; BlackRock-later). DSA = now pure CALM maintenance (cold-recall + [C] revisits).
COGNITION: abstract-formula feels hard (spatial brain), concrete-numbers/code easy → when stuck go CONCRETE fast.

METHOD: self-run flow — he runs own code (Code Runner), Claude gives problem+signal+test-cases, RUNS to witness.
   Help = thinking-direction only, never code. Tracker = SOLVED.md (signal→pattern→approach→[C] cold-retry markers).
   CORE: ~12 templates + twist (derived sticks, told forgets — proven: 14/14 cold-recall 21 Jun).
PENDING (when he raises it): SOLVED.md → pattern+TEMPLATE library (read his code, add reusable skeleton).
```

---

# HLD — STATUS

```
FOUNDATIONS 10/10: HLD-basics, capacity, LB, cache, replication, sharding, MQ, CAP, DB-choice, MS-comm.
DESIGNS: url-shortener · rate-limiter · twitter · notification · url-journey · STOCK-BROKER (JP gold) ·
         PAYMENT (idempotency/consistency/failure/ledger) · FILE-UPLOAD (JP actual Q) — interview-framework notes.
8-STEP FRAMEWORK (the pattern, every design plugs in):
   1.clarify(FR+NFR+Qs) 2.scale/estimation 3.API 4.boxes 5.data+DB(KYUN/trade-off) 6.DEEP-DIVE(options→choose)
   7.bottleneck/scale 8.wrap.  META: think-out-loud · trade-off every choice · chup mat baitho.
DEPTH = JP/finance flavor (idempotency, consistency>availability, ACID/ledger/audit, failure-handling) — NOT FAANG-hyperscale.
DRILL-METHOD (23 Jun — the real upgrade): Claude=interviewer fires cross-questions, Arpan defends. Deep-dives cluster into
   ~8 FOUNDATION themes (cache/DB/consistency/CAP/queue/failure/idempotency/LB) + design twist = same "12-template+twist" idea.
   When drilled → pull from FOUNDATIONS (10/10). Learned: cache-stampede(mutex/load-shed), counter range-alloc, SHARD-ON-QUERY-KEY.
Each design = ONE file now (8-step drives merged into mains; dup files deleted).
```

---

# APPLY — STATUS (LIVE)

```
SYSTEM: Resume (honest: 600+ incidents, $20K Doximity fix, VP/CTO recognition, one-page) · LinkedIn Open-to-Work ·
   Naukri/Cutshort/Instahyre/Wellfound inbound-optimized · Apply-Method.md + Apply-System.md (RESOURCES private repo).
   FAST-APPLY: Simplify Copilot set up (autofill 90%, only custom Qs manual, ~30 sec/apply).
   3 CHANNELS: INBOUND (Cutshort 18 interested / InMails / hackajob — warmest) + REFERRAL-first + Simplify-bulk.
   Apply to his LEVEL (3-5yr); skip FAANG-DSA-hard (Amazon), 8+yr senior, refused-tier.

APPLIED: JP Morgan (210714912) · Razorpay (Debangan ref) · FIS · Barclays (hackajob) · Oracle (inbound, emailed) ·
   Target India · DELTA · Portfolio-Mgmt-Engineer (Simplify) · Siemens Energy (Java Backend, Bengaluru, 22 Jun, CTC ask 22L — real product MNC, exact-stack-fit).
   NOTE: Dice MCP = US-only (skip); Indeed-India = mostly staffing noise (~1 gem/search). Real channels = inbound + referral + Simplify.
REFERRALS OUT: Kumar Sudarshan (JP VP) · Ishita Nandwani (JP — resume SENT) · Anmol+Debangan (Razorpay) · Madhur/Saurabh.
   JP referrals no-reply yet (normal, days).
PENDING: RESOURCES push (gh-auth fix → `gh auth login` as personal ArpanMaheshwari144). NEVER add company-acct as collaborator.
SABAR: 100 apply → 5 callback → 1 offer; 4-7 month window NORMAL; reject = data point. Real interview = real validator.
```

---

# KONOVO EVIDENCE (interview gold, for Behavioral STAR)

```
AS6079 RCN cluster fraud pattern · *2026@outlook.com bot fingerprint · risk-tiered IP assessment ·
SAME bug-class in 2 layers (app code reading infra IP vs WAF config against clientIp = whole-system thinking) ·
HikariCP pool exhaustion (95% email-fail) · Lambda concurrency (1800+ throttles) · $20K Doximity payment fix · VP/CTO recognition.
HONESTY "work-on" vs "built": 10+ country microservices platform existed before him (maintains/extends/owns-incidents,
   NOT "built 15+"); "built" only greenfield (vendor portal + email-dedup API). Integrity = the JP filter.
```

---
> *"Karm karo, phal ki chinta mat karo. Roz ek kadam. Apne dum pe — BlackRock tak."*
> Hard work sab karte; SABAR virle. Yehi mera moat.
