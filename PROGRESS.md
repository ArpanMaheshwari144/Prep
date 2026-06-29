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

   2. DSA = TEACH-FIRST PRACTICE  (medium, JP→BlackRock level — FAANG-hard NAHI)
      ★ LEARNING PHASE (his call 30-Jun): spoiler se farak NAHI padta abhi. flow:
        STEP 1. Claude PEHLE problem SAMJHAYE (kya poocha + kaise sochna) — teach, na ki seedha cold-throw.
        STEP 2. Claude FILE banaye (stub: problem + tests + // expected + empty function + main).
        STEP 3. Arpan code kare (run kare).
        STEP 4. na hua  -> Claude HINT de (thinking-direction).
        STEP 5. phir bhi na hua -> Claude SOLUTION/code bataye -> Arpan likhe -> [C] mark (cold-redo later).
      - pehle SAMJHANA phir KARWANA. (NOT pure "ja cold kar" pheko.)
      - target: pattern samajh + novel handle. (pressure-gap baad me mocks se.)

   3. HLD = ★ RAW TRADEOFF DRILL — DESIGN-LESS (30-Jun lock, Arpan clarified) ★
      - sab designs padh liye, formula aa gaya (block-kab har design pe SAME). NAYA design NAHI, BookMyShow jaisा known bhi NAHI.
      - RAW = KOI design ki story nahi (na fresh, na rehearsed). SEEDHA tradeoff/concept rapid-fire, COLD:
        Claude pheke isolated sawaal → Arpan kab/kya/kyun/kaise bole. e.g.:
          • "kab SQL, kab NoSQL?"
          • "10M user aaye → kaise scale? kab SHARD? kis KEY pe shard? kyun?"
          • "kab REPLICA? kab CACHE? kab QUEUE?"
          • "read-heavy hua to? write-spike hua to? consistency-critical hua to?"
        → ye answers har design me SAME (sharding/replica/cache/queue universal) → isliye design ke BINA reflex-drill.
      - KYUN raw/design-less: design-wrapper "rehearsed/checked" lagता hai; Arpan ko GENUINE cold-practice chahiye (jaise DSA).
      - master "kaunsa BLOCK kab": cache · queue(Kafka) · shard(+key) · replica · LB · CAP/consistency ·
        idempotency-vs-atomic · rate-limit · CDN · WebSocket. NOT ratta — kyun/kab samajh ke. (DSA bhi practice-mode.)
```

```
   DAILY SCHEDULE (pel ke — idle nahi, enjoy + grind, roz):
     SUBAH (FRESH mind → HARD/gap areas):
       1. HLD  → ~1 hr → RAW TRADEOFF DRILL (design-less rapid-fire: SQL-vs-NoSQL, 10M→shard/replica/cache kab-kyun-kaise). NO design story.
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
