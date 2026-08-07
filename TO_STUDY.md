# TO STUDY — backlog (jo topics dekhe par padhe nahi; baad me padhna)

> Running list. Jo bhi naya topic saamne aaye aur abhi na padha ho -> yahan daalo -> baad me padho -> phir tick/hata do.

---

## Pending

### 1. Elasticsearch (HLD — search box / full-text search)
- **Kaha aaya:** system design me "search box / search feature" -> Elasticsearch use hota.
- **Kya hai (short):** full-text SEARCH ENGINE. normal DB `WHERE name LIKE '%x%'` slow + weak; Elasticsearch fast full-text search, ranking, typo-tolerance, filters deta. (inverted-index pe based.)
- **Padhna:** kya hai · kab use (search-heavy feature) · DB ke saath kaise (DB = source of truth, ES = search-index) · inverted index basic.

### 2. Distributed ID generation (auto-increment vs UUID vs Snowflake)
- **Kaha aaya:** DB me `AUTO_INCREMENT` -- distributed/multi-node me problem.
- **Kyun problem:** auto-increment SINGLE DB pe theek. multiple DB/shards -> do node same ID de sakte (clash) + coordination bottleneck.
- **Alternatives:** UUID (128-bit random, globally unique, par bada + un-ordered) · Snowflake ID (Twitter -- timestamp+machine-id+sequence, unique + roughly time-ordered) · DB-ticket-server / sequence.
- **Padhna:** auto-increment kyun distributed me toota · UUID pros/cons · Snowflake kaise kaam karta · kab kya chuno.

### 3. [POST-SEPT, tool — abhi mat chhed] MemPalace — AI memory system setup
- **Link:** https://github.com/mempalace/mempalace  (MIT, local-first, free)
- **Kya hai:** open-source AI memory — conversation verbatim store + semantic search + Claude Code auto-save hooks + MCP (36 tools). = MEMORY.md + guard-hook ka AUTOMATED version (mujhe manually memory padhni na pade, auto-retrieve ho).
- **Stack:** Python 3.9+ · ChromaDB · Docker. sab LOCAL (kuch bahar nahi jaata).
- **Kyun WAIT:** setup ek chhota project hai (Python/ChromaDB/embeddings/Docker) -> ABHI 25-din JP-schedule se dhyan bhatkega (rabbit-hole). current setup (markdown + hook) simple + kaam kar raha.
- **Kab:** SEPTEMBER apply ke BAAD, time ho tab. + soch: personal/emotional baatein vector-DB me verbatim jaayengi (local hai to risk kam).

### 4. GitHub Stacked PRs (dev-workflow — market me trending)
- **Kaha aaya:** Arpan ne market/GitHub me suna — stacked PRs kaafi use ho raha.
- **Kya hai (short, net-confirm karna):** bade change ko chhote **DEPENDENT PRs ki chain** me todo — har PR pichhle PR ke branch pe "stack" hoti (PR2 ka base = PR1, na ki main). Faayda: PRs chhote + reviewable, dependent kaam ke liye har PR ka merge-wait nahi.
- **Tools:** Graphite · ghstack · spr · git-branchless.
- **Padhna:** kya + kyun (review-velocity, chhote PR) · kaise banate/manage · konsa tool. (net se detail nikaal ke.)

### 5. REST — nayi query/design method (market me trending)
- **Kaha aaya:** Arpan ne market me suna — REST query karne ka koi naya/flexible tareeka.
- **Kya ho sakta (net-confirm — exact cheez PIN karni hai):** GraphQL-style flexible querying · ya OData / JSON:API (query-params se filter/sort/sparse-fields) · ya cursor-based query pattern. exact "new method" net se confirm.
- **Padhna:** PEHLE net se exact cheez pin karo (kaunsi "new query method") -> phir kya/kyun/kaise.

---
> (done topics -> hata do ya ~strikethrough~)
