# JP MORGAN — INTERVIEW INTEL (online research, 24-Jul)
> Kya poochte hain — DSA/HLD/LLD/Java/behavioral + structure. Real interview-experiences + guides se (Bangalore SDE-2/Associate target).
> ★ Use: prep ko isse ALIGN karo. Sources neeche. (public info — sensitive nahi.)

---

## 0. STRUCTURE (Bangalore SDE-2 / Associate)
```
   - Online assessment (HackerRank): 2 easy-medium DSA
   - Phir 3 ROUNDS x ~50 min -- ★ SAARE 3 hote hain (ek kharab != auto-reject)
   - 3 rounds = SYSTEM-DESIGN + DSA + BEHAVIORAL/CODE-REVIEW
     -> ye Raunak-insider se MATCH (1 DSA + 1 SD + 1 behavioural). 2/3 = Arpan strength.
   - Selective (bahut fail hote) -- par structure Arpan-favor.
   - No centralized interviewing -> department-specific variation.
```

## 1. DSA
```
   DIFFICULTY: LeetCode EASY-MEDIUM (Hard kam). BREADTH > depth -- 1 ghante me 1-2 medium + complexity/edge-case discuss.
   MOST-COMMON PATTERNS (Arpan ke paas SAB):
     - Arrays/Strings: TWO-POINTER · SLIDING-WINDOW · PREFIX-SUM · FREQUENCY-MAP(HashMap)  [almost every round]
     - HashMap/Set (O(1) lookup, grouping, frequency) · LINKED-LIST · TREES · thoda GRAPH-BFS
   SPECIFIC NAMES jo aate: Two-Sum · Valid-Parentheses · Product-of-Array-Except-Self · Group-Anagrams ·
     Top-K-Frequent · Longest-Substring-No-Repeat · Non-overlapping-Intervals · (business-context: trade-matching/txn-validation)
   ★ ARPAN GAPS (add karne hain): Top-K-Frequent (heap/bucket) · Non-overlapping-Intervals · Graph-BFS. (Trees shuru ho gaya.)
   ★ baaki sab sheet me hai. DSA-panic disproportionate (medium, 1 round).
```

## 2. SYSTEM DESIGN (HLD) -- Arpan STRONG zone
```
   QUESTIONS: "Monolith ko 50x traffic scale karo" · "Google-Drive jaisa (skeleton diya: LB/file-service/S3 -> optimize)" ·
              "Rate Limiter" (Arpan ne mock kiya) · real-world FINANCE context (security/scalability/compliance).
   FORMAT: 20 min block-diagram + 25 min discuss.
   ★ FRAMING thoda naya (scale-existing / optimize-skeleton, na ki design-from-scratch) -- par CONTENT = existing toolkit
     (LB · horizontal-scale · caching/Redis · DB replication+sharding · async-queue/Kafka · CDN · SPOF-chain). naya seekhna nahi.
   ★ practice: HLD-mock me "monolith 50x scale" + "optimize-skeleton" framings (roz ek).
```

## 3. LLD
```
   OOP 4 pillars + DESIGN PATTERNS (kuch classic aane chahiye) · real-time TRADING platform (low-latency) · FRAUD-detection (payments).
   ★ Arpan: 11 LLD designs + patterns cover karte -> trading/fraud FINANCE-flavor note kar.
```

## 4. JAVA / SPRING -- Arpan ka stuff
```
   ACTUAL Qs: HashMap vs Hashtable vs ConcurrentHashMap (segment/bucket locking) · equals() vs hashCode() ·
     class IMMUTABILITY · Callable vs Runnable · CACHING/distributed-cache · Docker for Spring Boot · Microservices ·
     Core-Java + SQL (e.g. 4th-highest-salary, remove-duplicates).
   ★ Arpan: ye SAB padha (ConcurrentHashMap/equals-hashCode/immutable/threading/caching/Docker/microservices).
     project mini_payment_ms = microservices+caching+idempotency LIVE.
```

## 5. CODE REVIEW (ek round me)
```
   Buggy/production code dikha ke: "kya galat? inefficient kahan? kaise improve?"
   ★ Arpan 650-tickets background = DIRECT strength (roz karta aaya).
```

## 6. BEHAVIORAL
```
   Resume + PROJECT deep-dive + decision-making + STAR (ownership/conflict/failure).
   ★ Arpan: 5 STAR done + project-revise (mini_payment_ms) -> ready.
```

---

## ★ BOTTOM LINE
```
   JP jo poochta ~80% Arpan ne prepare kiya. Structure favor: SD + code-review + Java + behavioral = STRONG (3 me 2+ rounds).
   DSA sirf 1 round, MEDIUM, exact-patterns-in-sheet.
   TODO (chhote gaps): DSA -> Top-K-Frequent · Non-overlapping-Intervals · Graph-BFS · (Trees continue).
               HLD -> "50x-scale" + "optimize-skeleton" framings mock. LLD -> trading/fraud finance-flavor.
   => DSA-panic disproportionate; profile = achha JP fit.
```

## SOURCES
```
   - interviewquery.com/interview-guides/jp-morgan-chase-software-engineer
   - codejeet.com/blog/jpmorgan-hash-table-interview-questions
   - leetcode.com/discuss -> "JP Morgan Associate(SDE-2) Bangalore Offer" · "JPMC Java Dev SDE2 Bangalore Offer"
   - educative.io/blog/jp-morgan-system-design-interview-questions
   - systemdesignhandbook.com/guides/jp-morgan-system-design-interview
   - medium (kaurharjeet122) -- JP Java/Spring/Microservices Qs
```
