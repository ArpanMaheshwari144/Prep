# SQL vs NoSQL

---

## 1-Line Analogy

```
SQL    = EXCEL spreadsheet  (rows + columns, fixed schema)
NoSQL  = JSON document file (flexible, anything goes)
```

---

## Side by Side

```
┌──────────────────┬──────────────────┬──────────────────┐
│  Feature         │  SQL              │  NoSQL            │
├──────────────────┼──────────────────┼──────────────────┤
│ Schema           │ FIXED, predefined │ FLEXIBLE, dynamic│
│                  │ ALTER TABLE        │ Add field anytime │
├──────────────────┼──────────────────┼──────────────────┤
│ Structure        │ Tables + rows     │ Documents/KV/    │
│                  │ Foreign keys      │ Wide-column/Graph│
├──────────────────┼──────────────────┼──────────────────┤
│ Query language   │ SQL (standard)    │ Varies per DB     │
│                  │ JOIN, GROUP BY    │ MongoDB query,    │
│                  │                    │ DynamoDB API     │
├──────────────────┼──────────────────┼──────────────────┤
│ ACID              │ Strong          │ Often eventual│
│                  │ Transactions       │ BASE (newer ones)│
├──────────────────┼──────────────────┼──────────────────┤
│ Scaling          │ VERTICAL (mostly) │ HORIZONTAL native │
│                  │ Bigger server      │ Sharding built-in │
├──────────────────┼──────────────────┼──────────────────┤
│ Relationships    │ Native (JOIN)      │ Denormalize /     │
│                  │                    │ embed documents   │
├──────────────────┼──────────────────┼──────────────────┤
│ Examples         │ MySQL, PostgreSQL,│ MongoDB (doc)     │
│                  │ Oracle, SQL Server│ Redis (KV)        │
│                  │                    │ Cassandra (wide)  │
│                  │                    │ Neo4j (graph)     │
│                  │                    │ DynamoDB (KV/doc) │
└──────────────────┴──────────────────┴──────────────────┘
```

---

## When to Use What

```
SQL CHOOSE KARO when:
   • Strong relationships (User → Order → Items)
   • Need ACID (banking, finance)
   • Complex queries with JOINs
   • Schema stable / well-defined
   • Reporting + analytics
   • Compliance / audit required

   Examples: UserCRUD, banking, ERP

NoSQL CHOOSE KARO when:
   • Schema changes frequently
   • Massive scale (millions/billions records)
   • Unstructured data (logs, social posts)
   • Specific access pattern (key-value lookups)
   • Eventual consistency acceptable

   Examples:
   • Cache (Redis)
   • Social feed (Cassandra)
   • Catalog (MongoDB)
   • Session storage (Redis)
   • Time-series (InfluxDB)
```

---

## NoSQL Types Visual

```
┌──────────────┬──────────────────────┬──────────────────┐
│  Type        │  Structure            │  Use Case         │
├──────────────┼──────────────────────┼──────────────────┤
│ Document     │ JSON-like docs        │ Catalogs, profiles│
│ (MongoDB)    │ Nested fields         │ CMS, blogs        │
├──────────────┼──────────────────────┼──────────────────┤
│ Key-Value    │ Simple key → value     │ Cache, sessions   │
│ (Redis)      │ Super fast            │ Real-time data    │
├──────────────┼──────────────────────┼──────────────────┤
│ Wide-Column  │ Rows with many cols   │ Time-series        │
│ (Cassandra)  │ Distributed           │ Analytics          │
│              │                        │ Twitter feed       │
├──────────────┼──────────────────────┼──────────────────┤
│ Graph        │ Nodes + edges          │ Social networks    │
│ (Neo4j)      │ Relationships first   │ Fraud detection    │
│              │                        │ Recommendations    │
└──────────────┴──────────────────────┴──────────────────┘
```

---

## INTERVIEW LINE

```
"SQL when you need relationships, ACID, complex queries.
 NoSQL when you need horizontal scale, flexible schema,
 or specific access patterns.

 Don't fight your data model. If it's tabular and related,
 SQL. If it's documents, hierarchical, or eventually
 consistent, NoSQL.

 Many real systems use BOTH — SQL for transactional core,
 NoSQL for cache/feed/analytics."
```

---

## Memory Hook

```
SQL    = Excel spreadsheet (structured)
NoSQL  = JSON files folder (flexible)

SQL = JOIN king
NoSQL = SHARD king

Real apps: HYBRID (SQL + Redis + Cassandra commonly)
```

[← HLD README](../README.md)
