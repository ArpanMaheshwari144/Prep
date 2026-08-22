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

### 3. [HANDS-ON BACKLOG — live chala ke dekhne wale] runnable demos
> Ye tera engine (chal ke dekho + why). Done ho chuke: indexing(EXPLAIN), N+1(JOIN FETCH), isolation/dirty-read,
> DB-deadlock(1213), Nginx-LB(round-robin+failover), rate-limiter(Nginx), Kafka, CB(Resilience4j),
> Redis(@Cacheable), GraphQL, QueryDSL, JWT, Docker, K8s, load-test.

**★ KAL (agli baar sabse pehle):**
- ~~DB deadlock live~~ **DONE (22-Aug)** — 09_DATABASE/08_deadlock.md
- ~~Nginx load-balancer~~ **DONE (22-Aug)** — 05_INFRA_DEEP/02_load_balancer.md (hands-on section)

**Baad me (jab man kare):**
- Non-repeatable + phantom read live (REPEATABLE READ vs SERIALIZABLE — isolation demo ka agla step)
- Optimistic locking @Version — 2 requests same row -> OptimisticLockException (concurrency)
- Spring Retry @Retryable — flaky method -> logs me retry-retry
- @Async / @Scheduled — alag thread / cron live fire
- Cache eviction + TTL live (@Cacheable ka agla step)
- FK cascade / constraint live (parent delete -> cascade/restrict)
- Actuator + Prometheus/Grafana — live metrics dashboard (graphs)

---
> (done topics -> hata do ya ~strikethrough~)
