# Interview Prep — Progress Tracker

> **Last updated:** 2026-06-09 (Day ~34) — HLD Foundations 10/10 DONE (MS Communication finished); DSA Phase 2 coding at 5 patterns; Stock Broker design deep-dive started.
> **Bangalore arrival:** 15-17 June 2026 (~6-8 days) | **Primary target:** JP Morgan (Associate, Bangalore) | **Open to:** any company for experience | **JP-ready:** ~93% (foundation solid; DSA-coding 3 patterns + HLD design-depth = remaining)

---

# GOAL

```
   Konovo (12L) → Bangalore product/fintech (18-25L) → JPM/GS (26-50L) → BlackRock Aladdin (4-5yr, 50+L)
   Primary near-term = JP Morgan.   Refused (hard no): TCS/Infosys/Wipro/Cognizant/Capgemini/KPMG.
   Philosophy: "Effort 100% mera, outcome universe ka." "Karm karo phal ki chinta mat karo." "Roz ek kadam."
```

---

# AT A GLANCE — kya gaya, kya bacha

```
   AREA                STATUS            DETAIL
   ─────────────────   ───────────────   ──────────────────────────────────────────────
   Java                DONE              Collections, Java8, Multithreading, SOLID, Patterns, BrainTicklers
   Spring              DONE              Basics, @Transactional, JWT, Hibernate, Profiles, Docker, K8s, Revision-visual
   LLD                 DONE              11 problems (parking, bms, ttt, snake, elevator, atm, bank, cart, library, vending, hotel)
   Projects            DONE              TODO full pipeline (CRUD → JWT → Redis → Docker → K8s → AWS EC2)
   Infra / Compares    DONE              CDN/LB/DNS/WAF + AWS/Docker/K8s commands; 12 compare-notes
   Database            DONE (core)       Indexing practical (hands-on)
   ───────────────────────────────────────────────────────────────────────────────────────
   HLD Foundations     DONE  10/10       HLD-basics, Capacity, LB, Cache, Replication, Sharding, MQ, CAP, DB-choice, MS-Comm
   HLD System Designs  IN PROGRESS       6 drafted; Stock Broker = JP-deep DONE (4 layers: single-thread/order-types/event-log/saga)
   ───────────────────────────────────────────────────────────────────────────────────────
   DSA Phase 1 (intuition) DONE  8/8     Arrays, Two-Ptr, Sliding-Win, Hashing, Prefix, Binary-Search, Kadane, Strings + NAVIGATOR
   DSA Phase 2 (coding)    IN PROGRESS   5 patterns coded (~16 problems); 3 patterns LEFT
   ───────────────────────────────────────────────────────────────────────────────────────
   Apply system        NOT STARTED       Bangalore DAY-1 task (referral-first)
```

---

# WHAT'S LEFT — focused (the "bada kya bacha")

```
   1. DSA CODING (3 patterns):    Binary Search (+ BS-on-answers, rotated) | Kadane | Strings
   2. HLD STOCK BROKER deep-dive (4 layers — ALL DONE):
         [x] Matching engine SINGLE-THREADED per symbol (concurrency)   DONE
         [x] Order TYPES (market vs limit) + PARTIAL fills   DONE
         [x] EVENT LOG / sequencer (crash → rebuild, audit replay)   DONE
         [x] SETTLEMENT across services → SAGA (when one @Transactional isn't enough)   DONE
   3. HLD OTHER DESIGNS (optional depth pass): url-shortener, rate-limiter, twitter-feed, notification
   4. APPLY SYSTEM (Bangalore day-1): platform list + referral targets + resume-tailor + tracker
```

---

# DSA — STATUS (detail)

```
PHASE 1 INTUITION — COMPLETE (8/8): Arrays, Two Pointer, Sliding Window, Hashing, Prefix Sum,
   Binary Search (+BS-on-answers), Kadane, Strings.  NAVIGATOR.md built (signal -> pattern).

PHASE 2 CODING (C++, his method: pure-intuition -> he writes ALL code) — in progress:
   DONE (5):  Arrays-basics(5) | Two Pointer(3) | Sliding Window(3) | Hashing(3) | Prefix Sum(2)   = ~16 problems, solo
   LEFT (3):  Binary Search | Kadane | Strings
   (Subarray-Sum-K = Hashing+Prefix bridge — conquered via gullak analogy, solo-coded incl map[0]=1 edge.)

METHOD (locked): give ONE simple real-life analogy = pure-words intuition (no code/syntax/pseudo),
   then HE writes all code (production rep). Intuition FIRST in chat, file SECOND on his go.
   He is ALREADY a problem-solver (Konovo proof); DSA = the interview GATE only. Metric = patterns covered, not count.

TIER 2 (post-Bangalore / as-needed): Linked List, Trees (DFS/BFS), BST (BlackRock), Recursion+Backtracking,
   1D DP (coin change = JP), Graph BFS/DFS (clone = JP).
CAP: Easy + Medium only — NO Hard (JP/GS/BlackRock != FAANG). Quality over volume (2-3/pattern).
```

---

# HLD — STATUS (detail)

```
FOUNDATIONS — COMPLETE (10/10):
   01 HLD-basics | 02 Capacity-estimation | 03 Load-balancing | 04 Caching | 05 Replication |
   06 Sharding (consistent hashing) | 07 Message-queues | 08 CAP | 09 DB what/when | 10 MS-Communication.

SYSTEM DESIGNS — 6 drafted:
   11 URL-shortener | 12 Rate-limiter | 13 Twitter-feed | 14 Notification | 15 URL-browser-journey |
   16 Stock-Broker/Trading (JP GOLD — base strong; deep-dive 4 layers in progress).

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

# BANGALORE PHASE (15+ June)

```
MOVE 15-17 = guilt-free (travel + settle, no study). Then APPLY mode (parallel):
   JP Morgan PRIMARY (referral turant) | GS (stretch) | Razorpay/Cred/Swiggy/any (experience).
DAY-1 task: build apply-system — platform list (LinkedIn/Naukri/Dice/Indeed), referral targets
   (LinkedIn + ex-colleagues + Konovo network), resume-tailoring template, tracker.
   HYBRID: dream-tier = referral-first + per-role tailoring; experience-tier = bulk OK. "Smart pel."
SABAR for the no-offer window: 100 apply -> 5 callback -> 1 offer; 4-7 month window NORMAL; reject = data point.
Cricket truth: "Practice mein har koi 6 marta, asli match mein pata." Real interview = real validator.
```

---

# JP MORGAN — INTERVIEW PATTERN (reference)

```
ROUNDS (3-4):
   1. HackerRank OA — 90 min (aptitude + 2 easy-med DSA)
   2. Coding/Tech (VP) — 50-60 min (2 LC-mediums)
   3. System Design + Java — 50-60 min (LLD/HLD + internals)
   4. Behavioral (ED) — 45-60 min  <- REAL GATE
QUESTIONS: DSA (combination-sum/recursion/streams); System (trading platform / price feed / ledger);
   Java (8/11/17, Runnable vs Callable, Spring autowiring); Behavioral ("why JPM", conflict, STAR).
INSIDER: drop "idempotency" + "audit trail" in design; PostgreSQL/Oracle over NoSQL for ledgers; ACID mastery.
COMP (4yr Associate Bangalore): base 18-22L, total 26-33L median, top ~47L (Senior Associate).
```

---

# COUNTDOWN

```
   Aaj: 9 June 2026   |   Move: 15-17 June   |   Days: ~6-8 baki
   Pre-move focus: DSA coding (Binary Search next) + Stock Broker deep-dive + daily revision. CONSOLIDATE, then guilt-free move.
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
