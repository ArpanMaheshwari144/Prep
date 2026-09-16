# Databases — Kya Hai, Kab Use, Real-World Connect

> **NAV** — KYA: kaunsa DB kab (SQL/NoSQL/family). UP: [MASTER](../00_MASTER_SHEET.md) · trade-off: [SQL vs NoSQL](../TRADEOFFS.md) · lagta hai: [payment](../SYSTEM_DESIGNS/07_payment_system/07_payment_system.md) · [bookmyshow](../SYSTEM_DESIGNS/10_bookmyshow/10_bookmyshow_INTERVIEW.md) · [stock-broker](../SYSTEM_DESIGNS/06_stock_broker_trading/06_stock_broker_trading.md)

> **HLD Reference** — 7 DB types with everyday-life analogies
> CAP done — SQL-vs-NoSQL choice + finance/ACID connection at bottom

---

## ★★ SQL vs NoSQL — asli wajah, ratta nahi

```
★ NoSQL bana KYUN? (ye jad hai — "JSON store karna tha" nahi)

  SQL EK MACHINE pe chalne ke liye bana tha.
  ACID + JOIN + strong consistency ka waada SIRF tab nibhaya ja sakta hai jab saara data EK jagah ho.
  Data ko do machine pe baant do -> "do table join karo, beech me transaction chalao"
  ab NETWORK ke aar-paar karna padega -> mehnga + slow.

  NoSQL waalon ne ULTA raasta liya:
     "join denge hi nahi. Strong consistency hamesha nahi denge.
      Iske badle data 100 machine pe baant denge, aur wo sach me chalega."

  => NoSQL ne naye feature JODE nahi — usne feature CHHODE, taaki machine ke paar fail sake.
     (flexible schema/JSON = saath me aaya faayda, jad wajah SCALE thi)
```

```
★ SQL SCALE NAHI HOTA — ye GALAT hai. Ye jaanna zaroori hai, warna interviewer kuredega:
     1. READ REPLICA   -> read baant do (sabse pehla, sasta kadam)
     2. VERTICAL       -> badi machine (aaj ki machines bahut door tak le jaati hain)
     3. PARTITIONING   -> ek hi DB me table ko tukdon me (date/range)
     4. SHARDING       -> alag DB pe baant do (Vitess / Citus) — par cross-shard join/txn ab tumhara sar-dard
     5. NewSQL         -> CockroachDB / Spanner / Aurora (SQL ka roop, andar distributed)

   ★ "Real Bitly ka data ~500 GB hai — ek single Postgres me aa jaata."
     Bina zaroorat NoSQL/sharding bolna = OVER-ENGINEERING (ye sabse common galti hai)
```

```
★ NoSQL ye KAR kaise paata hai (teen cheezein):

  1. PARTITION KEY   har row ke saath key -> hash(key) -> row kis machine pe jaayegi
                     padhna: key do -> seedha us machine pe -> row utha lo
                     -> 100 machine ho ya 1000, EK read ka kharcha wahi
                     (SQL me "WHERE email = x" pe pata hi nahi kis machine pe hai -> sab se poochna padta)

  2. JOIN NAHI DETA  join = do table, do machine, network ke aar-paar milao -> slow
                     NoSQL: "jo saath chahiye, saath hi likh ke rakho" (DENORMALIZE)
                     -> ek read = ek machine = ek jump
                     -> isliye table QUERY ke hisaab se banate hain, query table ke hisaab se nahi

  3. LSM TREE        SQL (B-tree): row ko uski SAHI jagah pe rakhna -> random write (slow)
                     NoSQL (LSM) : jo aaya END me likh do (sequential), baad me background merge
                     -> disk ka sabse tez mode (wahi append-only wali baat jo Kafka me thi)
```

```
★ KEEMAT — NoSQL kab GALAT hai:
  1. ACID / multi-row transaction chahiye        -> paisa, order, booking
  2. NORMALIZATION nahi -> data DUPLICATE hota   -> ek cheez badli to 10 jagah badalni padegi
                                                    (user ne naam badla -> 500 comments me copy pada hai)
  3. QUERY PATTERN BAAD ME BADAL GAYA            -> SQL me naya WHERE/GROUP BY kabhi bhi chala lo;
                                                    NoSQL me table query ke hisaab se bana tha
                                                    -> naya tareeka = poora data DOBARA likho
  4. CHHOTA DATA / shuruaati system              -> 500 GB me SQL aaram se; bina wajah join aur
                                                    consistency khona bewakoofi hai
```

```
★★ FAISLA — do sawaal, bas:
     1. "JOIN aur multi-row TRANSACTION chahiye?"        haan -> SQL
     2. "Access-pattern PAKKA key-lookup hai, aur data itna ki ek machine me na aaye?"  haan -> NoSQL
     warna -> SQL hi rakho (replica/partition se kaam chal jaayega)

★ BOLNE WALI LINE:
  "SQL scale hota hai — replica, partitioning, ab NewSQL bhi. Main NoSQL isliye le raha hoon
   kyunki mera access pattern pure key-lookup hai aur mujhe join chahiye hi nahi —
   na ki isliye ki SQL scale nahi karta."

★ EK LINE KA NIYAM:
     SQL   -> "nahi pata kal kya poochunga" + "sahi hona zaroori hai"
     NoSQL -> "hamesha KEY se uthaunga" + "itna data ki ek machine me nahi aayega"
```

```
   DESIGN            CHUNA           WAJAH
   ──────────────────────────────────────────────────────────────────────
   payment           SQL             debit + credit dono ya koi nahi (ACID)
   bookmyshow        SQL             ek seat do ko nahi -> row lock + atomic update
   stock-broker      SQL + ledger    paisa aur share ek saath hile
                     (+ RAM)         order-book RAM me — wo speed ke liye, alag cheez
   file-upload       SQL             data CHHOTA hai, par status galat nahi ho sakta
   url-shortener     NoSQL           sirf key se uthana, join nahi, ~90 TB
   twitter-feed      NoSQL           crore tweets, simple shape, write-heavy
   news-aggregator   NoSQL           crore article, simple, ACID ki zaroorat nahi
   google-docs       NoSQL edit-log  + permissions ALAG SQL me
   message-queue     append-only file + metadata strongly-consistent store (ZK/KRaft)

   ★ DO SEEKH:
     1. file-upload me data chhota tha phir bhi SQL -> faisla SIZE se nahi, ZARURAT se hota hai
     2. google-docs aur message-queue me DONO hain:
          bhaari + baar-baar aane wala   -> NoSQL / file
          chhota + 100% sahi rehna wala  -> SQL / strongly-consistent store
        -> ek design me ek hi DB hona zaroori nahi (ye bolna achha lagta hai)
```

---

## 7 Major Types

```
1. RELATIONAL (SQL)
2. DOCUMENT (NoSQL)
3. KEY-VALUE
4. WIDE-COLUMN
5. GRAPH
6. SEARCH ENGINE
7. TIME-SERIES
```

---

## 1 RELATIONAL (SQL) — "Excel Sheet"

```
Examples: PostgreSQL, MySQL, MariaDB, Oracle

Kya hai:
   • Tables = Excel sheets
   • Rows + Columns (rigid)
   • Tables ke beech relationships (foreign keys)
   • SQL queries
```

### Real-world Connect:
```
Soch — bank ki passbook
   • Har row = ek transaction
   • Columns fixed: date, type, amount, balance
   • Sab transactions strictly recorded
   • Data ka type pakka (date is date, amount is number)

Yahi relational DB hai.

Jab use:
   Bank account (HDFC, ICICI internal — Postgres/Oracle)
   Amazon order tracking (orders table)
   Tera UserCRUD project! (MySQL — same pattern)
   Anywhere strict structure + transactions chahiye
```

---

## 2 DOCUMENT (NoSQL) — "JSON Folder"

```
Examples: MongoDB, CouchDB

Kya hai:
   • Data = JSON-like documents
   • Har document alag fields ho sakte (flexible)
   • Tables ki jagah "collections"
```

### Real-world Connect:
```
Soch — Resume PDFs ka folder
   • Mera resume: skills, projects, certifications
   • Mohit ka resume: skills, hobbies, awards
   • Different fields, both still resumes

Yahi document DB hai — flexible structure.

Example:
   Amazon ke products:
      • Laptop:  RAM, processor, screen size
      • T-shirt: size, color, fabric
      • Book:    author, pages, ISBN

   Har product alag attributes — DocumentDB perfect.

Jab use:
   E-commerce product catalog (Amazon, Flipkart)
   Blog/CMS posts (varying structure)
   User profiles with flexible fields
   Game player stats (game-specific data)
```

---

## 3 KEY-VALUE — "HashMap (Java jaise)"

```
Examples: Redis, Memcached, DynamoDB

Kya hai:
   • EXACTLY HashMap jaise (Java mein jo tu use karta)
   • Bus key → value
   • No queries, sirf key se lookup
   • In-memory = SUPER FAST (sub-millisecond)
```

### Real-world Connect:
```
Soch — Phone contacts:
   "Mom"    → "+91-9876543210"
   "Boss"   → "+91-9876511111"

Naam (key) se number (value) instantly mil jata.
Yahi Key-Value DB.

Real production:
   Login → Redis: "session:user123" → "active_token_xyz"
   Login check ke time bus key se value mil jata

   Same pattern: Java HashMap
   Same speed: ek line code = milliseconds

Jab use:
   Cache (DB ke saamne, fast access)
   Login sessions (web apps)
   Rate limiting (kitne requests per user)
   Gaming leaderboards (Top 10 scores)
   Shopping cart (temporary storage)

Real example:
   Twitter feed cache = Redis
   Tera UserCRUD session = Redis (production mein)
```

---

## 4 WIDE-COLUMN — "Excel Pe Steroids"

```
Examples: Cassandra, HBase, Bigtable, ScyllaDB

Kya hai:
   • Tables jaise but har row alag columns ho sakte
   • Massive horizontal scale (1000s of nodes)
   • Write throughput = millions per second
```

### Real-world Connect:
```
Soch — Twitter tweets database:
   • Daily 500 million tweets
   • Har tweet ka row
   • Storage scale = petabytes

   Relational DB (PostgreSQL) yeh handle nahi kar paayega.
   Cassandra-type DB chahiye = scale-out

Real-world examples:
   • Twitter tweets storage (Cassandra)
   • Netflix viewing history (Cassandra)
   • Discord messages (Cassandra)
   • IoT sensor data (millions writes/sec)

Jab use:
   Massive write volume (logs, tweets, messages)
   Time-series data at scale
   Multi-region replication needed
```

---

## 5 GRAPH — "LinkedList of Relationships"

```
Examples: Neo4j, ArangoDB, Amazon Neptune

Kya hai:
   • Data = NODES + EDGES (connections)
   • EXACTLY LinkedList jaise (Java mein jo tu use karta)
   • Har node ke aage wale node ka pata
   • Traversal queries (jump from one to another)
```

### Real-world Connect (KEY POINT):
```
Soch — LinkedList:
   Node1 → Node2 → Node3 → Node4
   Har node ka aage wale ka reference

Graph DB exactly yahi:
   Arpan ──"works at"──► Konovo
   Arpan ──"friend of"──► Mohit
   Mohit ──"works at"──► Google

   Query: "Mere doston mein kaun Google mein hai?"
   = Edges traverse karke find

PRODUCTION USE — OWN EXPERIENCE:

YouTube "Next video" / "You might like":
   Tu ne video X dekha
   Graph DB query: "X dekhne wale ne aur kya dekha?"
   → Y, Z videos suggest

Instagram "You might know":
   Tera dost A
   A ka dost B
   B aur tujhe suggest hota
   = Friend-of-friend graph traversal

Netflix recommendations:
   "Because you watched Stranger Things, here's Dark"
   = Content graph + user behavior graph

LinkedIn connections:
   "Mohit is 2nd degree connection"
   = 2 hops graph traversal

Bank fraud detection:
   Account A → B → C → D (suspicious pattern)
   Graph traversal flags it
```

```
Jab use:
   Social networks (LinkedIn, Facebook)
   Recommendations (Netflix, YouTube, Insta)
   Fraud detection (banks)
   Knowledge graphs (Wikipedia, Google)
   Supply chain optimization
```

---

## 6 SEARCH ENGINE — "Google Search Box"

```
Examples: Elasticsearch, Solr, Algolia

Kya hai:
   • Full-text search optimized
   • Fuzzy matching ("apple" → "appl" bhi match)
   • Autocomplete suggestions
   • Filter combinations (faceted search)
```

### Real-world Connect:
```
Soch — Amazon search box:
   Tu type karta "ipone" (typo)
   Amazon dikha deta "iPhone" results
   = fuzzy match + autocomplete

Yahi Elasticsearch karta.

Real-world:
   • Amazon product search
   • GitHub code search
   • Netflix content search
   • Tera Konovo logs (likely Elasticsearch + Kibana)

Jab use:
   Site search (anywhere search box dikha)
   Log analytics (debugging production)
   Autocomplete (Google search bar)
```

---

## 7 TIME-SERIES — "Stock Chart / Fitbit"

```
Examples: InfluxDB, TimescaleDB, Prometheus

Kya hai:
   • Timestamp + value = data point
   • Compression for time-ordered data
   • Aggregations over windows (avg/min/max)
```

### Real-world Connect:
```
Soch — Fitbit / Apple Watch:
   10:00 AM   85 BPM
   10:01 AM   88 BPM
   10:02 AM   90 BPM
   ... continuous stream

Stock chart (Zerodha/Groww):
   Stock price har second update
   = millions of timestamped data points

Real-world:
   • Server monitoring (CPU, memory graphs in Grafana)
   • IoT sensors (temperature, GPS)
   • Trading systems (price ticks)
   • Fitness apps (steps, heart rate)

Jab use:
   Application metrics (Prometheus + Grafana)
   IoT sensor data
   Financial market data
   Monitoring dashboards
```

---

## Quick Summary

```
TYPE              REAL-WORLD ANALOGY            EXAMPLE         FAMOUS USE
────              ───────────────────           ───────         ──────────
Relational        Bank passbook                  PostgreSQL      Banking, orders
Document          Resume folder                  MongoDB         Amazon catalog
Key-Value         HashMap / phone contacts       Redis           Cache, sessions
Wide-Column       Twitter tweets storage          Cassandra       Tweets, logs
Graph             LinkedList + suggestions       Neo4j           LinkedIn, Netflix
Search            Google search box              Elasticsearch   Amazon search
Time-Series       Fitbit / stock chart           InfluxDB        Monitoring, IoT
```

---

## Polyglot Reality (Multiple DBs in 1 App)

```
Real apps = MULTIPLE DBs together (each for what it's best at)

Tera Amazon shopping experience:
   • Search box      → Elasticsearch
   • Product details → MongoDB
   • Order placement → PostgreSQL
   • Cart            → Redis
   • Recommendations → Graph DB (Neo4j-style)
   • User session    → Redis
   • Activity logs   → Cassandra

   = 7 DBs in 1 app
   = Each used optimally
   = "Polyglot persistence" pattern
```

---

## Quick Decision Logic

```
"Bank-style transactions chahiye?"
   → Relational (Postgres, MySQL)

"Flexible JSON-like data?"
   → Document (MongoDB)

"Cache jaisa fast lookup?"
   → Key-Value (Redis)

"Twitter-scale write volume?"
   → Wide-Column (Cassandra)

"Friend-of-friend / suggestions?"
   → Graph (Neo4j)

"Search box / autocomplete?"
   → Search (Elasticsearch)

"Time-stamped sensor data?"
   → Time-Series (InfluxDB)
```

---

## Connect to CAP + FINANCE (CAP done — ab connection)

```
SQL vs NoSQL ka choice CAP se SEEDHA judta:
   SQL (PostgreSQL/Oracle)        → CP-leaning (consistency, ACID)  → money/ledger
   NoSQL scale (Cassandra/Dynamo) → AP-leaning (availability/scale) → high-scale/flexible
   Redis (key-value)              → tunable
```

```
ARPAN'S CASE (finance/JP) — THE takeaway (his own):
   "Mere case mein ACID ka khayal → SQL."

   Ledger / account balance / transaction  =  SQL (ACID + strong consistency)
   = galat balance / double-spend KABHI nahi.
   JP insider-tip: "PostgreSQL/Oracle over NoSQL for ledgers; ACID > eventual."
```

```
PER-CRITICALITY (CAP se jod ke) — ek hi app mein dono zones:
   money-path (ledger/txn)              → SQL / CP
   non-critical high-scale (logs/catalog/feed/recos) → NoSQL / AP

   = "polyglot persistence" + CAP per-criticality = same insight.
     (replication "money-path sync, baaki async" ki bhi yahi soch.)
```
