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

### 2. [chhota glance — CS-fundamental gap-fill] Computer Networks basics
- **Kaha aaya:** JP recruitment skill-list (GfG) me CN listed — Arpan ne dedicated nahi padha (baaki sab — DSA/DBMS/OS-multithreading/Spring/microservices/Docker/K8s — done).
- **Kyun (dara nahi, gap-fill):** lateral backend ke liye CN deep-theory GATE nahi (Raunak = 3 round: DSA + SD + behavioral, no MCQ-exam). PAR "interview me aa jaaye to dikkat na ho" — chhota glance kaafi.
- **Kya padhna (halka, interview-level — ratta nahi, samajh):** TCP vs UDP (kab kya) · HTTP/HTTPS + status codes (200/4xx/5xx) · DNS (naam→IP kaise) · sockets basics · TLS/handshake ek-line · REST already-pata. (backend-relevant hi, OSI-7-layer ki ratта NAHI.)
- **Kaise:** ek chhoti visual note → 09_DATABASE-jaisa folder (ya 05_INFRA_DEEP) me daal ke ek baar padho. connect: HTTP↔REST↔load-balancer↔caching (already-pata cheezon se jodo).


**Baad me (jab man kare):**
- Non-repeatable + phantom read live (REPEATABLE READ vs SERIALIZABLE — isolation demo ka agla step)
- Spring Retry @Retryable — flaky method -> logs me retry-retry
- @Async / @Scheduled — alag thread / cron live fire
- FK cascade / constraint live (parent delete -> cascade/restrict)

**DONE (hands-on ho gaye — 22-Aug):**
- ~~DB indexing live EXPLAIN~~ (09_DATABASE/01)
- ~~N+1 problem live~~ (02_SPRING/05_HIBERNATE/02 + usercrud)
- ~~Isolation / dirty-read~~ (09_DATABASE/07)
- ~~DB Deadlock (1213)~~ (09_DATABASE/08)
- ~~Nginx Load Balancer (round-robin + failover)~~ (05_INFRA_DEEP/02)
- ~~Optimistic Locking @Version~~ (02_SPRING/05_HIBERNATE/08 + usercrud)
- ~~Actuator + Prometheus/Grafana live dashboard~~ (05_INFRA_DEEP/05)

---
> (done topics -> hata do ya ~strikethrough~)
