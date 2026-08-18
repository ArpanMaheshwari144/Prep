# TO STUDY — backlog (jo topics dekhe par padhe nahi; baad me padhna)

> Running list. Jo bhi naya topic saamne aaye aur abhi na padha ho -> yahan daalo -> baad me padho -> phir tick/hata do.

---

## Pending

### 1. [POST-SEPT, tool — abhi mat chhed] MemPalace — AI memory system setup
- **Link:** https://github.com/mempalace/mempalace  (MIT, local-first, free)
- **Kya hai:** open-source AI memory — conversation verbatim store + semantic search + Claude Code auto-save hooks + MCP (36 tools). = MEMORY.md + guard-hook ka AUTOMATED version (mujhe manually memory padhni na pade, auto-retrieve ho).
- **Stack:** Python 3.9+ · ChromaDB · Docker. sab LOCAL (kuch bahar nahi jaata).
- **Kyun WAIT:** setup ek chhota project hai (Python/ChromaDB/embeddings/Docker) -> ABHI 25-din JP-schedule se dhyan bhatkega (rabbit-hole). current setup (markdown + hook) simple + kaam kar raha.
- **Kab:** SEPTEMBER apply ke BAAD, time ho tab. + soch: personal/emotional baatein vector-DB me verbatim jaayengi (local hai to risk kam).

### 2. GitHub Stacked PRs (dev-workflow — market me trending)
- **Kaha aaya:** Arpan ne market/GitHub me suna — stacked PRs kaafi use ho raha.
- **Kya hai (short, net-confirm karna):** bade change ko chhote **DEPENDENT PRs ki chain** me todo — har PR pichhle PR ke branch pe "stack" hoti (PR2 ka base = PR1, na ki main). Faayda: PRs chhote + reviewable, dependent kaam ke liye har PR ka merge-wait nahi.
- **Tools:** Graphite · ghstack · spr · git-branchless.
- **Padhna:** kya + kyun (review-velocity, chhote PR) · kaise banate/manage · konsa tool. (net se detail nikaal ke.)

### 3. REST — nayi query/design method (market me trending)
- **Kaha aaya:** Arpan ne market me suna — REST query karne ka koi naya/flexible tareeka.
- **Kya ho sakta (net-confirm — exact cheez PIN karni hai):** GraphQL-style flexible querying · ya OData / JSON:API (query-params se filter/sort/sparse-fields) · ya cursor-based query pattern. exact "new method" net se confirm.
- **Padhna:** PEHLE net se exact cheez pin karo (kaunsi "new query method") -> phir kya/kyun/kaise.

### 4. [kal/parso — HLD] Distributed LRU Cache (YouTube mock interview)
- **Kya:** "System Design Interview — Design a Distributed LRU Cache (Full mock with Sr. MAANG SWE)" (YouTube pe search).
- **Kyun ye achha:** kal humne LRU **single-machine (DSA/LLD)** khud banaya (08_DSA/09_DESIGN/03_lru_cache.cpp) -> ye uska **DISTRIBUTED (HLD)** version = natural agla step (single -> distributed).
- **Chapters:** functional/NFR -> API design (single-machine) -> API design (cache + linked-list) -> managing cache across nodes -> retrieval/replacement.
- **Kaise:** dekho + NOTES likho (write-to-learn) -> 04_HLD me daal denge. connect: LRU (DSA) <-> distributed-cache (HLD).

### 5. [chhota glance — CS-fundamental gap-fill] Computer Networks basics
- **Kaha aaya:** JP recruitment skill-list (GfG) me CN listed — Arpan ne dedicated nahi padha (baaki sab — DSA/DBMS/OS-multithreading/Spring/microservices/Docker/K8s — done).
- **Kyun (dara nahi, gap-fill):** lateral backend ke liye CN deep-theory GATE nahi (Raunak = 3 round: DSA + SD + behavioral, no MCQ-exam). PAR "interview me aa jaaye to dikkat na ho" — chhota glance kaafi.
- **Kya padhna (halka, interview-level — ratta nahi, samajh):** TCP vs UDP (kab kya) · HTTP/HTTPS + status codes (200/4xx/5xx) · DNS (naam→IP kaise) · sockets basics · TLS/handshake ek-line · REST already-pata. (backend-relevant hi, OSI-7-layer ki ratта NAHI.)
- **Kaise:** ek chhoti visual note → 09_DATABASE-jaisa folder (ya 05_INFRA_DEEP) me daal ke ek baar padho. connect: HTTP↔REST↔load-balancer↔caching (already-pata cheezon se jodo).

---
> (done topics -> hata do ya ~strikethrough~)
