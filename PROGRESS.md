# Interview Prep — Progress Tracker

> **Last updated:** 2026-06-06 (Day 31) — DSA Prefix Sum done; HLD CAP deepened; DSA method breakthrough (investigation/navigator style); memory consolidated to single file.
> **Bangalore arrival:** 15-17 June 2026 (~9 days) | **Primary target:** JP Morgan (Associate, Bangalore) | **Open to:** any company for experience | **JP-ready:** ~92% (foundation solid; DSA-coding + HLD-depth = remaining)

---

# GOAL

```
   Konovo (12L) → Bangalore product/fintech (18-25L) → JPM/GS (26-50L) → BlackRock Aladdin (4-5yr, 50+L)
   Primary near-term = JP Morgan.   Refused (hard no): TCS/Infosys/Wipro/Cognizant/Capgemini/KPMG.
   Philosophy: "Effort 100% mera, outcome universe ka." "Karm karo phal ki chinta mat karo." "Roz ek kadam."
```

---

# FINAL STUDY WEEK — PLAN (8-13 June, before Bangalore)

> Theme: Tier-1 DSA coding WRAP (new navigator/investigation method) + core HLD foundations finish + daily revision. No big new thing — CONSOLIDATE, then guilt-free move. Energy-matched (morning DSA / evening HLD). Flexibility: jaldi nahi, adjust if over-run, rest if tired. Gate-focus, quality over volume.

```
   MON 8:   SUBAH  DSA → build NAVIGATOR (signal->pattern, together) = new method launch
            SHAAM  HLD → Message Queues (#07) — async/idempotency/DLQ/retry

   TUE 9:   SUBAH  DSA → Binary Search (investigation-style) + BS-on-answers (Koko/Ship)
            SHAAM  HLD → Databases what/when (#09) — SQL vs NoSQL choice (JP)

   WED 10:  SUBAH  DSA → Kadane (quick) + Strings start
            SHAAM  HLD → MS Communication (#10) — sync/async, REST/gRPC/queue

   THU 11:  SUBAH  DSA → Strings finish (palindrome/anagram, 2-3)
            SHAAM  HLD → Stock Broker/Trading follow-ups (JP gold) or Capacity Estimation

   FRI 12:  SUBAH  DSA → BUFFER: shaky patterns solo redo (Product-Except-Self etc.) + navigator refine
            SHAAM  HLD → BUFFER: pending HLD finish, or basics revision

   SAT 13:  (2-slot) SUBAH  REVISION consolidation — navigator recognition-drill (signal->pattern) + HLD re-glance
                     SHAAM  Java/basics trap-drill (interview fundamentals) + Bangalore mindset

   ROZ:     1 hr solo revision + random micro-reads; basics sharpen woven in.
   SUN 14:  REST (buffer)   |   15-17: BANGALORE MOVE (guilt-free)   |   17+: build apply-system + apply + learn-on-go
```

```
   Week-end target: DSA Tier-1 coding ~DONE (7 patterns) + NAVIGATOR ready;
   HLD deep = 8 foundations (LB/Cache/Shard/Replication/CAP/MQ/DB-choice/MS-comm). On schedule.
```

---

# DSA — STATUS

```
PHASE 1 INTUITION — COMPLETE (8 patterns): Arrays, Two Pointer, Sliding Window, Hashing,
   Prefix Sum, Binary Search (+BS-on-answers), Kadane, Strings.

PHASE 2 CODING (C++, LeetCode-style) — in progress:
   DONE:    Two Pointer | Sliding Window | Hashing | Prefix Sum   (4/7, ~13 problems, solo)
   PENDING: Binary Search | Kadane | Strings
   (Subarray-Sum-K = Hashing+Prefix bridge, solo-redone. Product-Except-Self pending — knows approach.)

NEW METHOD (Day 31 breakthrough): do DSA AS investigation (his Konovo style) —
   SIGNAL → PATTERN → WHY → implement → GENERALIZE. Build a DSA NAVIGATOR (signal→pattern→template,
   mirroring his Konovo .claude-context). He is ALREADY a problem-solver; DSA = the interview GATE only.
   Claude = medium/Socratic, he drives. (Launch Monday, together.)

TIER 2 (post-Bangalore / as-needed): Linked List, Trees (DFS/BFS), BST (BlackRock priority),
   Recursion+Backtracking, 1D DP (coin change = JP), Graph BFS/DFS (clone = JP).
CAP: Easy + Medium only — NO Hard (JP/GS/BlackRock ≠ FAANG). Quality over volume (2-3/pattern).
```

---

# HLD — STATUS

```
FOUNDATIONS deepened: Load Balancing | Caching | Sharding (consistent hashing) |
   Replication (failover/split-brain) | CAP   — 5 DONE.
   PENDING: Message Queues (#07), Databases what/when (#09), MS Communication (#10), Capacity Estimation.
SYSTEM DESIGNS: URL browser-journey + Stock Broker/Trading DONE; 4-systems follow-up depth pending.

DEPTH DIRECTION = JP/FINANCE flavor (HIGH invest): idempotency, consistency>availability,
   ACID/ledger/double-entry/audit-trail, failure-handling (circuit breaker/DLQ/retry), trade-off articulation.
   LOW invest (skip): FAANG-hyperscale — 100M-scale, global sharding, CDN-edge, celebrity-fanout.
   Insight: "ek HLD core strong, doosra easy" (blocks repeat: LB/cache/DB/queue/sharding = 80% same).
```

---

# FOUNDATION — DONE (done-state, no rework)

```
Java (V90: Core/Collections/Java8/Multithreading/SOLID/Patterns/BrainTicklers) | Spring (REST/@Transactional/
   JWT/Hibernate gotchas) | LLD (12 problems) | DB indexing (hands-on practical).
Projects: SimpleBankSystem, UserCRUD, TODO full pipeline (CRUD → JWT → Redis → Docker → K8s → AWS EC2).
Docker / K8s / AWS — practical, done. Interview ritual doc.
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
SABAR for the no-offer window: 100 apply → 5 callback → 1 offer; 4-7 month window NORMAL; reject = data point.
Cricket truth: "Practice mein har koi 6 marta, asli match mein pata." Real interview = real validator.
```

---

# JP MORGAN — INTERVIEW PATTERN (reference)

```
ROUNDS (3-4):
   1. HackerRank OA — 90 min (aptitude + 2 easy-med DSA)
   2. Coding/Tech (VP) — 50-60 min (2 LC-mediums)
   3. System Design + Java — 50-60 min (LLD/HLD + internals)
   4. Behavioral (ED) — 45-60 min  ← REAL GATE
QUESTIONS: DSA (combination-sum/recursion/streams); System (trading platform / price feed / ledger);
   Java (8/11/17, Runnable vs Callable, Spring autowiring); Behavioral ("why JPM", conflict, STAR).
INSIDER: drop "idempotency" + "audit trail" in design; PostgreSQL/Oracle over NoSQL for ledgers; ACID mastery.
COMP (4yr Associate Bangalore): base 18-22L, total 26-33L median, top ~47L (Senior Associate).
```

---

# COUNTDOWN

```
   Aaj: 6 June 2026 (Saturday)   |   Move: 15 June   |   Days: ~9 baki
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
