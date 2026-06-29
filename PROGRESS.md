# JP PREP — PRACTICE PLAYBOOK

> GOAL: JP Morgan (Associate/SE-III, Bangalore) → later BlackRock. *"Effort 100% mera, outcome universe ka."*
> **MODE (his call, 29 Jun): 2 MONTHS PURE STUDY (July + Aug) — no mocks, no referral-chasing, no new applies, no faltu meta-talk. Just PRACTICE. Reassess after Aug (kaha hain + kitna aur time).**
> STATE: ~60-65% ready. Gap = novel-handling under pressure + nervousness + behavioral. Pressure-gap closes ONLY via mocks/real-interviews (LATER). Now = build knowledge + train.
> Claude: bolna kam, KARANA zyada. (in-flight referrals Raunak-JP[submitted] / Anmol-RZP[no-role-yet] sit naturally — call aaye to decide then.)

---

## THE 3 LOCKS (how to practice — daily)

```
   1. JAVA + SPRING = READ / REVISE  (reading kaam — baar-baar padho → dimaag me FIX)
      - Claude notes + code present kare, he RE-READS, repeat. NOT active-practice.
      - goal: interview me KOI answer pe na atke. (proven method for him.)

   2. DSA = COLD SOLVE  (medium, JP→BlackRock level — FAANG-hard NAHI)
      - Claude gives ONLY question + test-cases. NO hint, NO code, NO signal first.
      - he codes COLD. fasa → HINT (thinking-direction). phir bhi fasa → Claude TEACH (kahan galat) → he REDOES cold.
      - target: novel problem handle under pressure (generation muscle).

   3. HLD = ★ NEW DESIGN PADHNA BAND (30-Jun lock) → ab TRADEOFF-DRILL PRACTICE ★
      - sab designs padh liye, pattern aa gaya (8-step + block-kab = ek formula har design pe). NAYA design NAHI.
      - ab PRACTICE = RAW TRADEOFF drill (design "padhna/samajhna" NAHI):
        Claude ek design pe TWIST/follow-up phekta → Arpan DEFEND kare. Arpan ki khud ki line:
        "interview me questions LAGBHAG SAME — har design pe wahi twists" → reflex banana.
        common twists: "10x traffic?" · "cache stale?" · "queue full?" · "SQL slow → NoSQL kyun nahi?" ·
        "payment timeout/fail?" · "2-region?" · "double-X race?" → kab/kya/kyun/kaise REFLEX.
      - master "kaunsa BLOCK kab": CDN · WebSocket · cache · queue(Kafka) · shard · replica · LB ·
        CAP/consistency · idempotency-vs-atomic · rate-limit. finance/general, hyperscale NAHI.
      - NOT ratta — formula samjha hua hai, sirf tradeoff-handling cold-practice. (DSA bhi same: practice mode.)
```

```
   DAILY SCHEDULE (pel ke — idle nahi, enjoy + grind, roz):
     SUBAH (FRESH mind → HARD/gap areas):
       1. HLD  → ~1 hr → TRADEOFF-DRILL (Claude pheke twist on a design → Arpan defends). NO new design padhna.
       2. DSA  → ~1 hr → COLD solve (medium, JP-level; only question+tests → he codes → na hua → hint → teach → redo)
       2b. LLD + PR-REVIEW (with CODE — he's only done THEORY, needs to SEE+do code):
         - LLD: SOLID violations→fixes, design-patterns IMPLEMENTED, Builder — show REAL code, work through it (not just theory)
         - PR-REVIEW round practice: Claude shows production code w/ bugs+bad-practices → he finds bugs + improves (OOP/patterns/clean-code)
   DOPAHAR (easier, revise lane):
       3. JAVA + SPRING → ~2 hr → visual-code revise (FULL clear code he can SEE/trace, NOT crammed snippets), topics rotate
     + BEHAVIORAL STAR → 2-3x/week (Konovo stories)
   LOGIC: hard/novel (HLD+DSA = the gap) ko FRESH-mind morning; Java/Spring (strong, revise) afternoon.
   *** CODE-VISIBILITY RULE: show LOTS of CODE (he's spatial — jitna code dekhega utna yaad + round-practice). Java/Spring/LLD/PR-review = CODE-heavy, not theory. ***
   Claude rule: bolna kam, KARANA zyada. JP-focused only — NEVER off-target/above-JP/unstudied design.
```

---

## JP INTERVIEW REALITY (3 real writeups + web, verified)

```
   DIFFICULTY: MODERATE — "breadth under pressure", NOT single-hard, NOT FAANG. Reason-the-WHY.
   ROUNDS (~4, real writeups incl Pramod Jan-2026): (1) DSA + LLD (greedy/medium problem + SOLID/patterns/Builder) ·
     (2) System-Design + Spring-Boot (project architecture + a design e.g. notification) · (3) PR-REVIEW (buggy prod code → find+fix, OOP/patterns/clean-code) · (4) HR/Behavioral STAR.
     DSA = LC easy-MEDIUM + HINTS (greedy/combo-sum/circular-dial — NOT hard-DP/graphs). Balance DSA+LLD+HLD; know your project architecture.
   - CODING = his zone (Combination-Sum, Streams, Min-Stack, Level-Order seen). hints + main-logic enough.
   - JAVA/SPRING GRILL = HEAVIEST/decisive (HashMap-vs-CHM, concurrency/thread-safety, Spring-Boot internals, DI/autowiring,
     Java8 streams, exceptions, Runnable-vs-Callable, design-patterns, project deep-dive). ← his strength, keep revising.
   - SYSTEM DESIGN = FINANCE-FLAVORED (reliable/low-latency/secure/ACID/idempotency): Rate-Limiter, Payment, File-upload,
     News-Agg, Trading, URL. ❌ NOT social-media hyperscale (WhatsApp/Insta — OVERKILL, JP nahi puchta).
   - BEHAVIORAL = STAR (why-JP, manager-disagreement, led-project, initiative, oncall, design-reject). #1 GAP currently.
   COMP (4yr Associate Bangalore): base ~18-22L, total ~26-33L. CIBIL 775 (clears bank bg-check).
   INTERVIEW = Zoom (works from home). reject = data point (selected-candidate nailed STAR; rejected-one failed behavioral/sysdesign).
```

---

## BEHAVIORAL STAR — Konovo gold (write in S-T-A-R)

```
   WAF / X-Forwarded-IP bug (2 layers, whole-system thinking) · $20K Doximity payment fix · 600+ incidents (92% closure) ·
   HikariCP pool exhaustion (95% email-fail) · Lambda concurrency (1800+ throttles) · vendor-portal + email-dedup API (built) ·
   VP/CTO recognition. HONESTY: 10+country platform = maintained/extended (NOT "built"), greenfield only = "built".
```

---

## STATUS SNAPSHOT (for context, not tracking)

```
   COVERAGE DONE: Java 110 notes, Spring 41, HLD 19 designs + foundations 10/10, DSA 46 problems (all patterns), LLD 11.
   DSA patterns: Hashing/Two-Ptr/Sliding-Win/Prefix/Binary-Search/Stack/LinkedList/Grid/Trees(+BST)/Backtracking/Heap/1D-DP.
     parked [C]: Coin-Change (hardest, cold-revisit). SKIP: graphs/hard-DP (BlackRock-later).
   REFERRALS (ride, in-flight): Raunak Narayan (JP — email+phone sent ✓) · Anmol Srivastava (Razorpay — role-link pending, no fit yet).
   APPLIES: PAUSED (his call) — resume after training. (~16 applied earlier: eBay/Nutanix/DocuSign/Visa/CME/Siemens etc.)
   RESOURCES (resume/apply-tracker) = PRIVATE repo ArpanMaheshwari144/Resources (push pending gh-auth; never expose / never add company-acct).
```

---
> Karm karo, phal ki chinta mat karo. Roz ek kadam. Apne dum pe — BlackRock tak.
