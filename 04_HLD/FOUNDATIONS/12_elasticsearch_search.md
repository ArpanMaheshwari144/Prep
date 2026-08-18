# Elasticsearch (Full-Text Search)

> **HLD Topic 12 — Search weapon**

---

## STORY — Book ka Index Page

> Ek 500-page book me "recursion" dhundhna hai.
>
> **Without index:** page-1 se shuru, har page padho → "recursion" dhundho. 500 page scan. Slow.
>
> **With index (book ke peeche):** index page me `recursion → page 42, 87, 210`. Seedha jump. Fast.
>
> **Elasticsearch = book ka index page** — par har WORD ka (`word → kaunse docs me`).
> DB `LIKE '%x%'` = har page scan (slow). ES = index dekho, seedha jump.

---

## Problem — DB search kyun weak

```
DB me 1 crore products. User: "blue runing shose" (typo)

WHERE name LIKE '%blue%'
   ✗ SLOW   → har row scan (%x% pe index kaam nahi karta) = O(n)
   ✗ WEAK   → "runing" (typo) → 0 result
             → "shoes" vs "shoe" → match nahi
             → ranking nahi (kaunsa relevant, pata nahi)
```

→ dedicated **search engine = Elasticsearch (ES)**.

**ES deta:** fast full-text · typo-tolerance · stemming (shoes=shoe) · synonyms · relevance-ranking · filters/facets.

---

## CORE — Inverted Index

Normal DB: `row → words`. ES **ULTA: `word → docs`** (reverse map = HashMap).

```
doc1: "blue running shoes"
doc2: "blue shirt"
doc3: "running shorts"

INVERTED INDEX:
   blue     → [doc1, doc2]
   running  → [doc1, doc3]
   shoes    → [doc1]
   shirt    → [doc2]
```

**FAST kyun (yahi dil hai):**
```
search "blue" → map.get("blue") → [doc1, doc2]     ← O(1)/O(log n)
   • poore data pe LOOP/scan NAHI — ek lookup
   • = DSA HashMap: map["blue"] -> list, get() seedha
   • index WRITE-time pe banta (ek baar) → search-time sirf lookup → sasta
```

**= DB-indexing ka hi text-version:**
| | Index kya | Kya fast |
|---|---|---|
| DB index (B-tree) | column pe sorted | `WHERE id=5` — scan skip |
| Inverted index | word → docs map | `word=blue` — scan skip |

Dono essence: **"pre-built lookup banao → full-scan bachao."**

---

## Poora ES = 4 layer (core + 3)

```
text ─[1. ANALYZER: tokenize + lowercase + stem(shoes→shoe) + stop-words hatao]→ clean words
     ─→ [2. INVERTED INDEX: word → docs]         (CORE = HashMap)
search ─→ lookup ─→ [3. BM25 scoring: RANK]      → best-first results
          (4. DISTRIBUTED: shards[scale] + replicas[reliability])
```

1. **Analyzer** (write pe): text → tokens + stemming. **Typo/synonym/stemming ka jaadu yahin** (index banane se pehle).
2. **Inverted Index** (core): word → docs.
3. **BM25 / TF-IDF** (search pe): 1000 docs mile to kaunsa UPAR? score (word-frequency + rarity) → **ranking**.
4. **Distributed**: sharding (scale — data bada to alag machines) + replication (reliability). [inverted-index ≠ sharding: index = STRUCTURE/fast, shard = SPLIT/scale — alag layer.]

---

## DB ke saath — ES kabhi source of truth NAHI

```
   Write ──→ [ DB = SOURCE OF TRUTH ]   (asli data, ACID, transactions)
                    │  async sync: CDC / event / dual-write
                    ▼
             [ ELASTICSEARCH = search index ]   (searchable copy)
   Search ←──────────┘
```

- **DB = source of truth** (asli data yahin). **ES = search-index** (copy, sirf dhundhne ke liye).
- Async sync → ES **eventually-consistent** (thoda laggy) — search ke liye acceptable.

**ES kab NAHI:**
- Primary datastore ki tarah (transactions/ACID chahiye → DB). ES search-LAYER hai, source-of-truth nahi.
- Simple exact-lookup / chhota data → DB-index kaafi, ES overkill.

---

## Interview Talking Points

**Q: "Search feature kaise design karoge?"**
> *"Elasticsearch — DB source-of-truth rahega, ES search-index. Inverted-index se full-text + typo-tolerance + ranking. DB↔ES async sync (CDC/event), ES eventually-consistent — search ke liye acceptable."*

**Q: "ES fast kaise?"**
> *"Inverted-index = word→docs (reverse HashMap). Search = key lookup O(1), full-scan nahi. Index write-time pe banta. = DB B-tree index ka text-version."*

**Q: "ES ke andar kya-kya?"**
> *"Analyzer (tokenize + stemming), inverted-index (core), BM25 relevance-scoring (ranking), distributed shards+replicas."*

**Q: "ES ko primary DB bana sakte?"**
> *"Nahi — ES search-layer, source-of-truth nahi. Transactions/ACID DB pe. ES = DB ka searchable eventually-consistent copy."*

---

## Power Phrase

> **"ES = Analyzer (tokenize/stem) + Inverted-Index (word→docs HashMap, core) + BM25 (ranking) + Distributed (shards/replicas). DB source-of-truth, ES search-index, async sync (eventually-consistent). Fast = scan ki jagah HashMap-lookup — DB-index ka text-version."**

---

## Memory Hook

```
ES = "book ka index page, har word ka"

Core:   Inverted Index = word → docs (reverse HashMap)
Fast:   get(word) O(1), scan NAHI = DB-index ka text-version

4 layer:
   Analyzer   → tokenize + stem (typo/synonym yahin)
   Inverted   → word → docs (CORE)
   BM25       → relevance ranking
   Distributed→ shard (scale) + replica (reliability)

DB ke saath:
   DB = source of truth | ES = search-index (copy)
   async sync → eventually-consistent
```

---

## Trap Box

```
Trap 1: "ES ko primary DB banao"
         ES source-of-truth NAHI → transactions/ACID DB pe
Trap 2: "inverted-index = sharding"
         NAHI → index = structure(fast); shard = split(scale). Alag layer.
Trap 3: "ES real-time consistent"
         async sync → eventually-consistent (thoda laggy, search ke liye ok)
Trap 4: "har cheez ES me search karo"
         simple exact-lookup → DB-index kaafi, ES overkill
```

---

## Deep-dive Q&A

```
Q: reverse karke FAST kaise hota?
   → "blue" ab KEY hai → map.get("blue") = O(1), scan nahi. = DSA HashMap. (yahi click-point)

Q: ye to DB-indexing jaisa hi hai? → HAAN
   → dono "pre-built lookup → full-scan bachao". DB-index = column(id); inverted = word→docs (+typo/rank).

Q: ES ne SHARD kar diya, usme dhundhta? → NAHI, do alag cheez
   → inverted-index = STRUCTURE (fast, reverse-map, split NAHI) = book index-page
   → sharding = SCALE (data bada → alag machines) = library ko buildings me baantna

Q: bas inverted-index + kuch aur nahi? → core haan, par 4 layer
   → analyzer(stem) + inverted(core) + BM25(rank) + distributed(scale)
```
