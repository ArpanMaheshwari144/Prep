# Interview Prep — Progress Tracker

> **Last updated:** 2026-06-19 (BANGALORE) — Big multi-day stretch. DSA active-derive = 20 problems logged (Stack + Binary Search added; "medium easy lagne laga"). Payment-System HLD (JP/finance) done. And the BOTTLENECK UNBLOCKED — full apply-machine built + **JP Morgan APPLIED** (job-id 210714912, Under Review) + Kumar (JP VP, ex-GS) referral message sent.
> **In Bangalore since:** 15 June 2026 | **Primary target:** JP Morgan (Associate / SE-III, Bangalore) | **Open to:** product/fintech for experience | **JP-ready:** ~93% (foundation solid; DSA reps + HLD depth ongoing). **Mode now = APPLY + LEARN parallel (apply is LIVE).**

---

# GOAL

```
   Konovo (12L) → Bangalore product/fintech (18-25L) → JPM/GS (26-50L) → BlackRock Aladdin (4-5yr, 50+L)
   Primary near-term = JP Morgan.   Refused (hard no): TCS/Infosys/Wipro/Cognizant/Capgemini/KPMG.
   Philosophy: "Effort 100% mera, outcome universe ka." "Karm karo phal ki chinta mat karo." "Roz ek kadam."
```

---

# AT A GLANCE — kya gaya, kya bacha

> **FULL INVENTORY (actual file-count audit, 20 Jun) — "kuch nahi ho raha" is FALSE; folder proves months of solid work:**
```
   AREA                STATUS            ACTUAL COUNT + DETAIL
   ─────────────────   ───────────────   ──────────────────────────────────────────────
   Java                DONE              110 note files (Core/Collections/Java8/Multithreading/SOLID/Patterns/BrainTicklers)
   Spring              DONE              41 note files (basics/@Transactional/JWT/Hibernate/Profiles/Docker/K8s + revision-visual)
   LLD                 DONE              11 problems (parking, bms, ttt, snake-ladder, elevator, atm, bank, cart, library, vending, hotel)
   Projects            DONE              TODO full pipeline (CRUD → JWT → Redis → Docker → K8s → AWS EC2)
   Infra (05_INFRA)    DONE              9 notes (CDN + AWS/Docker/K8s setup-journeys + commands)
   Compares (06)       DONE              12 notes (SQL/NoSQL, TCP/UDP, REST/GraphQL, WebSocket/SSE, sync/async, auth...)
   Database (09)       DONE (core)       Indexing practical (hands-on)
   ───────────────────────────────────────────────────────────────────────────────────────
   HLD Foundations     DONE  10/10       HLD-basics, Capacity, LB, Cache, Replication, Sharding, MQ, CAP, DB-choice, MS-Comm
   HLD System Designs  7 done            url-shortener, rate-limiter, twitter-feed, notification, url-journey,
                                         #16 STOCK BROKER/TRADING (JP-deep) + #17 PAYMENT SYSTEM/UPI (JP-finance deep)
   ───────────────────────────────────────────────────────────────────────────────────────
   DSA coded           43 .cpp           8 patterns: Arrays-basics 8, Two-Ptr 7, Sliding-Win 5, Hashing 7,
                                         Prefix 4, Matrix 3, Stack 4, Binary-Search 5
   DSA active-derive   IN PROGRESS  23   SOLVED.md: Hashing+Two-Ptr COMPLETE, Sliding-Win/Stack/Grid deep, Binary-Search 5 (strong)
   ───────────────────────────────────────────────────────────────────────────────────────
   Apply system        LIVE              Machine BUILT (resume/LinkedIn/Naukri/Cutshort + playbook) | JP applied + Razorpay + FIS + referrals
```
> **FINANCE/JP-HLD depth (the edge):** #16 Stock-Broker (order matching, event-log, saga) + #17 Payment (idempotency,
> consistency, failure-handling, ledger double-entry, Saga) = JP/GS core. NEXT depth option: Order-Matching-Engine OR Wallet/Ledger system.

---

# WHAT'S LEFT — focused (the "bada kya bacha")

```
   1. APPLY (now the #1 active lever — machine built, keep FEEDING it):
        - Apply-Method daily: 3-5 target companies (referral-first), via Apply-System tracker
        - CHECK Kumar (JP VP) reply on the job-id referral; RESPOND to inbound (Cutshort 18 interested + recruiter InMails = warmest)
        - Send remaining ~5 referral requests + follow-up the 4 already sent
   2. DSA REPS (active-derive daily): continue patterns (more Binary Search -> Trees / Linked List / etc.)
        + start the 20-day spaced COLD re-try cycle on the 20 logged problems (real retention test)
   3. HLD: more JP/finance designs (interview depth) — Payment + Stock-Broker done; pick 1-2 more.
```

---

# DSA — STATUS (detail)

```
PHASE 1 INTUITION — COMPLETE (8/8): Arrays, Two Pointer, Sliding Window, Hashing, Prefix Sum,
   Binary Search (+BS-on-answers), Kadane, Strings.  NAVIGATOR.md built (signal -> pattern).

ACTIVE-DERIVE (C++) — 20 problems logged in 08_DSA/THINKING_FIRST/SOLVED.md (~40 coded total).
   Tracker = SOLVED.md (signal -> pattern -> approach -> key -> note) + a 20-DAY SPACED REVISION table
   (R1~d3 / R2~d10 / R3~d30 active-recall, [C] = cold re-code marker).
   Hashing (6) + Two Pointer (2) ............ COMPLETE
   Prefix/Suffix (1: Product Except Self) ... done
   Sliding Window (2: Longest Repeating Char Replacement, Permutation in String) ... deepened
   Grid / Matrix (3: Rotate Image, Set Matrix Zeroes, Spiral Matrix) ... done (new 05_MATRIX folder)
   STACK (4: Valid Parentheses, Min Stack, Evaluate RPN, Daily Temperatures/MONOTONIC) ... done (06_STACK)
   BINARY SEARCH (2: basic + Search-in-Rotated-Sorted) ... done (07_BINARY_SEARCH) — toughest yet, self-debugged
   STATE: "medium easy lagne laga." Production-muscle (blank-page generation + self-written syntax) building.

HIS OWN META-LEARNINGS (June 19 — HE articulated each; these guide the method now):
   (1) scaffold/pseudo-code in the stub = TRANSCRIPTION, not learning ("DSA na aane wala bhi dekh ke pass kara lega")
       -> stubs are now MINIMAL (signal + a nudge question, NO structure). He generates the code from blank.
   (2) test-pass != correct — he caught his own linear-loop-masquerading-as-binary-search (low++/high--) and >= vs >.
   (3) forgetting-curve = WHY revision: retrieval strengthens the trace -> the spaced cold-recode cycle.
   (4) cold-generate WITHOUT peeking old code (peeking mid-attempt mixes mental models -> confusion).
   (5) HLD-effortless vs DSA-blank = his SPATIAL-dominant cognition. HLD = native spatial mode; DSA-generation =
       symbolic/2nd mode + rust. FIX = pull spatial INTO dsa (one clean analogy + dry-run-on-paper). Not a defect.

METHOD ("PEHLE SAMJHO, FIR CODE"): Claude gives problem + signal; ARPAN derives approach, writes 100% of code;
   Claude gives MINIMAL stub + RUNS only (compile + report pass/fail + expected-vs-got). On FAIL: never read/reveal
   his bug — nudge him to dry-run the specific index/cell; HE finds it (that's the generation practice).
   He is ALREADY a problem-solver (Konovo proof); DSA = the interview GATE only. Metric = patterns owned, not count.

NEXT TIER (as-needed): Linked List, Trees (DFS/BFS), BST (BlackRock), Recursion+Backtracking,
   1D DP (coin change = JP), Graph BFS/DFS (clone = JP).
CAP: Easy + Medium only — NO Hard (JP/GS/BlackRock != FAANG). Quality over volume (2-3/pattern).
PENDING (when he raises it): upgrade SOLVED.md into a pattern+TEMPLATE library — per problem, read his code
   (explicit exception to the RUN-ONLY no-read rule) and add the reusable code SKELETON next to signal/approach.
```

---

# HLD — STATUS (detail)

```
FOUNDATIONS — COMPLETE (10/10):
   01 HLD-basics | 02 Capacity-estimation | 03 Load-balancing | 04 Caching | 05 Replication |
   06 Sharding (consistent hashing) | 07 Message-queues | 08 CAP | 09 DB what/when | 10 MS-Communication.

SYSTEM DESIGNS:
   11 URL-shortener | 12 Rate-limiter | 13 Twitter-feed | 14 Notification | 15 URL-browser-journey |
   16 Stock-Broker/Trading (JP GOLD — 4 layers DONE) |
   17 PAYMENT SYSTEM (UPI) — JP/finance DEEP DONE: idempotency (key+register+race), consistency (ACID/Saga,
      debits=credits invariant), failure-handling (status-track + reconciliation, courier-tracking analogy),
      ledger/audit (immutable double-entry, permanent-diary). Saved "jaise padha waise" with analogies.

DEPTH DIRECTION = JP/FINANCE flavor (HIGH invest): idempotency, consistency>availability, ACID/ledger/
   double-entry/audit-trail, failure-handling (circuit-breaker/DLQ/retry), trade-off articulation.
   LOW invest (skip): FAANG-hyperscale (100M-scale, global sharding, CDN-edge, celebrity-fanout).
   Insight: "ek HLD core strong, doosra easy" (blocks repeat: LB/cache/DB/queue/sharding = 80% same).
```

---

# FOUNDATION — DONE (done-state, no rework)

```
Java (V90: Core/Collections/Java8/Multithreading/SOLID/Patterns/BrainTicklers) | Spring (REST/@Transactional/
   JWT/Hibernate gotchas/Profiles/Docker/K8s) | LLD (11 problems) | DB indexing (hands-on).
Projects: SimpleBankSystem, UserCRUD, TODO full pipeline (CRUD -> JWT -> Redis -> Docker -> K8s -> AWS EC2).
Infra: CDN/LB/DNS/WAF notes + AWS/Docker/K8s command-journals. Compares: 12 (SQL/NoSQL, TCP/UDP, REST/GraphQL...).
Konovo production evidence (interview gold, Forward-Deployed-Engineer):
   AS6079 RCN cluster fraud pattern | *2026@outlook.com bot fingerprint | risk-tiered IP assessment |
   SAME bug-class in 2 layers (app code reading infra IP vs WAF config against clientIp = whole-system thinking).
   HONESTY: "work-on" vs "built" — the 10+ country microservices platform existed before him (maintains/extends/
   owns-incidents, NOT "built 15+"); "built" only for greenfield (vendor portal + email-dedup API).
```

---

# STRATEGY / PRINCIPLES

```
- APPLY + LEARN simultaneously (no "perfect prep first"); referrals = TURANT.
- Konovo is TOXIC -> real push to exit; but it's a strategic BRIDGE (income + employed-while-applying). Endure + apply HARD.
- Domain = BACKEND lane only; solid CORE not guru (JP-calibrated); K8s/Docker internals = skip (DevOps lane).
- Depth > speed ("kitne bhi din lage"); ONE concept at a time + samjha gate; aaram se (thaka dimaag = retention 0).
- Revision = LIFESTYLE: daily 1hr solo + random micro-reads (git portable), not block/cram.
- Notes = retention mechanism (understand + write = never forget).
- Comparison = poison (compete only with yesterday's self). Overload = STOP, rest, fresh.
- Integrity: jo kiya wahi bol ("work-on" vs "built"); on interview-unknown — honest "don't know, will research".
- Bruce Lee: "ONE kick x 10,000 times." Self-made — apne dum pe.
```

---

# APPLY — STATUS (the bottleneck, now LIVE)

```
MACHINE BUILT (all in private RESOURCES, backed up to private repo ArpanMaheshwari144/Resources):
   - Resume: polished + honest (600+ production incidents, $20K Doximity vendor-payment fix, VP/CTO recognition,
     "work-on" not "built" for the existing platform). One-page PDF compiled.
   - LinkedIn: headline/About/Experience optimized, Open-to-Work ON (inbound > cold).
   - Naukri / Cutshort / Instahyre / Wellfound: profiles inbound-optimized. Cutshort = 18 companies INTERESTED.
   - Apply-Method.md (playbook: channels by yield referral>inbound>career-page>cold, fit-checklist, skip-rules,
     standard form answers, referral templates) + Apply-System.md (target list + tracker + 10 referral targets).

EXECUTED:
   - JP MORGAN APPLIED — SE III Back-end (Chase UK), job-id 210714912, status Under Review.
   - Kumar Sudarshan (JP VP, ex-GS) accepted connection + got the job-id referral message = dream-play.
   - ~6 quality applies (FIS / Kong / a broker / Akamai / Zerodha / Tracxn tier).
   - Method he learned himself: job-id -> fit-verify (Java/Spring, mid-level, product/fintech) -> apply -> referral.

CHANNEL STRATEGY (HYBRID): dream-tier (JP/GS/top-fintech) = referral-first + per-role tailoring;
   experience-tier = bulk OK. Inbound (recruiter InMails / Cutshort interested) = WARMEST, respond first. "Smart pel."
SABAR for the no-offer window: 100 apply -> 5 callback -> 1 offer; 4-7 month window NORMAL; reject = data point.
Cricket truth: "Practice mein har koi 6 marta, asli match mein pata." Real interview = real validator.
NOTE: never add company-account "ArpanMaheshwariApollo" as collaborator on the Resources repo (leaving that company).
```

---

# JP MORGAN — INTERVIEW PATTERN (VERIFIED — 2 real SDE-3 Bangalore writeups + web research, 21 Jun)

```
DIFFICULTY: 3.1/5 (MODERATE, NOT brutal), 75% positive. "Breadth under pressure, NOT any single
   hard problem." JP = BANK, not a competitive-programming / FAANG-hard shop.

ROUNDS: OA -> (Superday) Code-pair + Design-pair + Behavioral. 3-6 weeks. Score => SDE2 vs SDE3.

   1. ONLINE ASSESSMENT (HackerRank, ~60-90 min):
        2 LC EASY-to-MEDIUM (string/array/DS/algo). e.g. Minimum Platforms, valid-substrings(sliding win).
   2. CODE PAIR (~45 min): PR review (design principles/SECURITY/SQL-injection/concurrency/logging)
        + 1 DSA problem. REAL examples seen: Min-Stack, Two-Sum, Level-Order-BFS, Delete-Node-LL,
        Trapping-Rain-Water, grid/matrix-graph. HINTS GIVEN, main-logic often enough (not full code).
   3. DESIGN PAIR (~45 min): system design. REAL examples: file-upload->validate->S3->tracking-link;
        News-Aggregator. (GENERAL product designs — NOT necessarily payment/trading.) One candidate
        literally used the 8-step framework: Functional -> Non-functional -> HLD -> DB tables -> APIs.
   4. BEHAVIORAL: resume/projects, "why JPM", "handle pushback from management", "keep up with tech", STAR.

   (Tech-screen variant may also probe JAVA INTERNALS deep: HashMap, collections, DI, REST, Java 21,
    concurrency — one MAANG SDE-2 got DOWNLEVELED on the JAVA round + YOE, NOT on DSA.)
COMP (4yr Associate/AVP Bangalore): base 18-22L, total 26-33L median, top ~47L.
```

## FOCUS ALLOCATION (data-driven — kahan focus, kahan NAHI)
```
   PRIMARY (energy here — JP's real WEIGHT + Arpan's STRENGTHS):
     - JAVA depth: internals (HashMap/collections/DI), Java 21, concurrency, code-review/bug-fix
     - SYSTEM DESIGN: the 8-step FRAMEWORK on VARIETY (file-upload, news-feed, url, chat, payment, rate-limiter)
     - LLD: 11 problems + OOP design
     - BEHAVIORAL: Konovo STAR stories (pushback->WAF/X-Forwarded-For, $20K payment, etc.)

   MAINTENANCE (gate-level only — don't over-invest):
     - DSA: common LC-mediums sharp (arrays/strings/hashing/two-ptr/sliding-win/stack/BS/LL/trees-BFS).
            HINTS given + breadth-not-depth -> his exact zone. NO hard-variant obsession (diameter etc).
            Just clear the OA gate (2 LC-mediums).

   DON'T over-do: FAANG-hard DSA, obscure tricky variants, deep DP/graphs. JP doesn't go there.
   CORRECTION (21 Jun): earlier over-emphasized "JP = payment/trading/ledger designs" — real data shows
     GENERAL product designs too. Framework matters most; finance-flavor (idempotency/ledger) = a depth-tool
     to drop in ANY design, not the guaranteed question.
```

---

# COUNTDOWN

```
   Aaj: 21 June 2026   |   In Bangalore   |   Mode: CONSOLIDATE + APPLY (apply LIVE)

   STRATEGIC PIVOT (21 Jun — derived from 2 REAL JP interview writeups + research):
     JP DSA = LC-medium standard + HINTS (his level, NOT FAANG-hard). Real WEIGHT = Java + System-Design
     + LLD + Behavioral (his STRENGTHS). So FLIP the priority:
       PRIMARY  -> Java(internals/Java21/concurrency) + HLD(framework on variety) + LLD + Behavioral STAR
       MAINTAIN -> DSA at gate-level (common mediums sharp; NO hard-variant obsession; clear OA = 2 mediums)

   CONSOLIDATE PHASE (next ek-do din): ONLY revision (Java + HLD + DSA), no new problems, then resume.
     DSA revision = active-recall (worked great 21 Jun: 14/14 cold recall incl hard [C] ones).

   APPLY (evening): check Kumar(JP VP) + Ishita(JP, resume sent) + Oracle replies; follow-ups; gh-auth fix for RESOURCES push.

   PROGRESS: 33 DSA (9 patterns; Hashing/Two-Ptr/BinarySearch/LinkedList COMPLETE; Trees going).
   Applied (live pipeline): JP Morgan(+Kumar+Ishita referrals) · Oracle(inbound) · Barclays(dream-tier) · FIS · Razorpay · Target.
   Mantra (21 Jun): JP is in reach — its DSA is his level, its real weight (Java/HLD/LLD/behavioral) is his strength.
   Effort his, outcome the universe's. "Sab ka waqt aata hai."
```

---

# SABAR — THE FINAL WORD (Arpan's own)

```
   Duniya mein ~700 crore log. SABAR — yeh trait bahut kam ke paas.
   Log bolte "mere andar sabar hai" — lekin nahi.
   Sabar master kar liya to DUNIYA FATAH kar sakte ho.
   Hard work sab karte hain. Sabar virle karte hain. = Yehi mera moat hai.
```

> *"Karm karo, phal ki chinta mat karo. Be water. Roz ek kadam. Apne dum pe — BlackRock tak."*
