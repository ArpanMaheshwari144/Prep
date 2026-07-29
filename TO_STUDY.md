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

---
> (done topics -> hata do ya ~strikethrough~)
