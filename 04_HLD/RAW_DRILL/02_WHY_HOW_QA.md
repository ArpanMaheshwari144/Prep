# HLD — WHY / HOW Q&A DRILL (reasoning + follow-ups)

> JP follow-up style: "kyu? kaise? kab?" reasoning questions. Jaise-jaise karenge, pattern jama hoga.
> Format: Q → crisp answer (Arpan-language) + interview-line (English). Arpan ne mostly khud diye (3-Jul).

---

## 3-Jul batch

**Q1. CDN STATIC files ke liye — DYNAMIC data (live balance) CDN pe kyu nahi?**
```
   dynamic data HAR USER ka ALAG + constantly BADALTA. CDN edge pe EK copy cache karke SABKO deta →
   dynamic cache karo to user ko STALE/GALAT/kisi-aur-ka data mile. static = sabke liye same + badalta nahi → safe.
   interview: "CDN caches one edge copy served to everyone — fine for static (same for all, rarely changes);
   dynamic/personalized would serve stale/wrong data, so it stays on origin."
```

**Q2. Rate-limiter API-GATEWAY pe kyu (app-server pe nahi)?**
```
   gateway = FRONT gate → invalid request EARLY reject → app-server ka resource waste na ho.
   (Arpan analogy: chor andar aa gaya, alarm baad me baja = bekaar. rok DARWAZE pe.)
   interview: "Rate limiter sits at the entry so bad requests are rejected early, before consuming app-server resources."
```

**Q3. Cache-aside: update pe cache UPDATE nahi, DELETE kyu?**
```
   failed/out-of-order cache-write galat data chhod sakta. DELETE → next read DB (source of truth) se repopulate → safe.
   interview: "On write I invalidate (delete) the cache — a failed or racy cache-write could leave wrong data; delete lets
   the next read repopulate from the DB."
```

**Q4. Spike pe QUEUE, LB kyu nahi?**
```
   LB load DISTRIBUTE karta, BUFFER nahi → load > capacity → phir bhi crash. QUEUE spike ABSORB karta (hold → one-by-one process).
   interview: "LB distributes but doesn't buffer; if load exceeds capacity it still fails. A queue absorbs the spike and the
   backend consumes at its own pace."
```

**Q5. Read-replica READ scale deta, WRITE scale kyu nahi?**
```
   write har replica pe apply + propagate karna padta → write-volume ghatata nahi. write scale = SHARD (data baant, har shard apne writes).
   interview: "Replicas scale reads, not writes — every write still hits master + all replicas. To scale writes you shard."
```

**Q6. SQL ACID deta, NoSQL scale ke liye kya TRADE karta?**
```
   SQL = ACID + consistency + joins, par scale mushkil. NoSQL TRADES strong-consistency (→ eventual) + joins FOR horizontal-scale + flexible-schema.
   interview: "NoSQL trades strong consistency and joins for easy horizontal scale and flexible schema. Pick by need."
```

**Q7. Idempotency SAME-user duplicate rokta — 2 ALAG user same-seat race pe kyu FAIL, kya chahiye?**
```
   2 alag user = 2 alag KEY → idempotency dono ko "alag legit" samajh ke PASS → double-book. wahan ATOMIC (conditional-update/lock) → sirf EK jeete.
   interview: "Idempotency dedupes one user's retries via a key; two different users have different keys → both pass → double-book.
   A race between different users needs an atomic check-and-set/lock so only one succeeds."
```

**Q8. CAP: paisa/booking me CP kyu (availability chhoda)?**
```
   PARTITION pe: stale/galat balance DIKHAO (available) ya REJECT karo. paisa me galat-balance/double-spend = catastrophe → REJECT > wrong. "sahi ya kuch nahi."
   interview: "During a partition, money chooses consistency — better to reject than show a stale balance or allow a double-spend.
   Brief unavailability beats being wrong about money." (note: OTP/message-timing = reliability, CAP-core NAHI.)
```

**Q9. Health-check: 1 fail pe down kyu nahi, 2-3 threshold kyu?**
```
   1 fail = transient blip ho sakta (network glitch / dropped response), real outage nahi. N CONSECUTIVE fail → dead, false-positive se bachne.
   (Arpan analogy: ek scratch pe shoe nahi fenkte.)
   interview: "One failed check could be a transient blip, not a real outage — so we need N consecutive failures before marking dead."
```

**Q10. Circuit-breaker OPEN me FAIL-FAST kyu (wait karne se behtar)?**
```
   OPEN me dead/slow service ko call → har call TIMEOUT tak HANG → threads bhar jaati → tera service bhi choke (cascade).
   fail-FAST = turant error/fallback → threads free → no cascade.
   interview: "When open, calling the failing service hangs every call until timeout, tying up threads until your service chokes.
   Failing fast returns an instant error/fallback, frees resources, stops the cascade."
```

**Q11. Shard-KEY galat (country, 90% India) → dikkat? achhi key kaise?**
```
   skewed key (country) → 90% users EK shard → HOT SHARD (overload), baaki khaali → benefit khatam.
   good key = (1) QUERY-key (queries me condition, e.g. userId) (2) EVENLY-distributed/high-cardinality. userId dono; country #2 fail.
   interview: "Shard on the query key so lookups hit one shard — but it must be high-cardinality and evenly distributed.
   A skewed key like country puts 90% on one shard (hot shard), killing the benefit."
```

**Q12. at-least-once me DUPLICATE → kaise handle?**
```
   at-least-once = pakka pahunche, par ack-lost pe resend → duplicate. IDEMPOTENCY-key → consumer check kare "processed?" → skip → effect once.
   interview: "The consumer uses an idempotency key — checks if the message-id was already processed and skips it — so the effect happens once."
```

**Q13. SAGA me rollback nahi COMPENSATING kyu?**
```
   normal rollback (@Transactional) sirf EK DB ka UNCOMMITTED work undo karta. SAGA me har service ALREADY COMMIT kar chuki apne DB me →
   committed cheez rollback nahi hoti → COMPENSATING action (naya reverse op, e.g. "cancel order") se undo.
   interview: "A rollback only undoes uncommitted work in one DB. In a saga each service already committed locally, so you run a
   compensating action — an explicit reverse operation — to undo the committed effect."
```

**Q14. DB index O(log n) fast — har column pe kyu nahi?**
```
   index = B-tree (sorted, O(log n) read). PAR har WRITE pe tree REORDER karna padta → writes SLOW + extra storage.
   isliye sirf query-condition wale columns pe index. (EXPLAIN se dekho query index use kar rahi ya nahi.)
   interview: "An index is a B-tree giving O(log n) reads, but every write must update the tree — so indexes slow writes and
   add storage. Index only columns used in query filters, not everything."
```

---
> aage: replication-lag, connection-pooling, blob/S3, websocket-vs-kafka, cache-eviction, pagination · aur.
