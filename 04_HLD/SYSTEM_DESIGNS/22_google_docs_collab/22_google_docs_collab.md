# Google Docs — Collaborative Document Editor (HLD)

> 3-Sep MOCK me KHUD derive kiya (novel design, pehle padha nahi tha). Requirements → Bottleneck poora RAIL par chala.
> Ye note = us mock ka lock. 3 cheezein NAYA tool thi (revise pe pakki karni): **per-component CAP · WebSocket+Redis-pubsub · OT/CRDT**.
> Baaki (write-batching, NoSQL edit-log, sharding, queue) KHUD nikaala.

---

## PROBLEM (crux)
Do (ya zyada) log ek hi document EK SAATH edit karein → koi **clash / override na ho**, kisi ka likha **kho na jaaye**, aur sabko ek-doosre ke changes **near real-time** dikhein + aakhir me **sabka doc SAME** ho.

> Ye crux hi poore design ka dil hai — real-time concurrent edit + no-lost-write + convergence.

---

## RAIL — 7 step (jaise mock me chala)

### 1 — Requirements
```
FR:  - multiple users same doc same time edit
     - har user ko baaki ke edits real-time dikhein
     - koi edit clash / override / lost na ho
NFR: - LOW LATENCY (typing instant feel ho)
     - HIGH AVAILABILITY (typing kabhi na ruke, offline bhi chale)
     - CONVERGENCE (sab eventually same doc pe aayein)
```

### 2 — Estimate (rough, atko mat)
```
~100M users · ~30-40M DAU · edits/sec HIGH (write-DOMINATED tool).
=> Point: WRITE-scale massive → buffering + sharding chahiye. (exact number matter nahi.)
```

### 3 — API (resource-based)
```
GET  /documents/{docId}            → doc laao
POST /documents/{docId}/edits      → is doc pe ek EDIT (operation) bhejo
WebSocket /documents/{docId}       → real-time 2-way channel (edits push/receive)
```

### 4 — Data model — "EDIT-OPERATIONS LOG" (NoSQL)
Har edit = ek OPERATION event (snapshot nahi):
```
{ docId, userId, opType (insert/delete), position, char/text, timestamp }
```
- Doc ka current state = us doc ke saare ops ko ORDER me apply karke banta.
- Access pattern: "ek docId ke SAARE edits, TIME-order me" → key=docId, order=timestamp.
- Write-heavy append-log → **NoSQL** (Cassandra: partition=docId, cluster=timestamp = ekdum fit; Mongo bhi ok).
- ★ DB choice DATA-SHAPE se aati, "consistency chahiye" se NAHI. ("relations nahi" = NoSQL ki taraf, SQL ki nahi.)

### 5 — HL boxes (standard blocks — kuch naya nahi)
```
Client ─► CDN (static app) ─► Load Balancer ─► App/Connection servers (WebSocket hold)
                                                     │
                                          Redis (pub/sub + write-buffer)
                                                     │
                                          NoSQL edit-log   +   [SQL/strong-consistent: permissions/ownership]
```

### 6 — DEEP DIVE (asli, unique hissa) — concurrent edit merge
Do log same instant same position pe type karein — winner mat chuno, **MERGE karo, dono bache**:
- Text mat bhejo → **OPERATIONS** bhejo. A:`insert("X",0)`, B:`insert("Y",0)`.
- A ne `"XHELLO"` banaya. B ka op aata → system **TRANSFORM** karta (A pehle pos0 pe daal chuka → B ka pos 0→1) → `"XYHELLO"`.
- B ki taraf bhi A ka op transform → dono end pe **EXACT same** `"XYHELLO"`, dono ke akshar bache.
- Naam = **Operational Transformation (OT)**. Order (XY vs YX) ek **deterministic tie-break** (userId/timestamp) se; par dono survive.
- **CRDT** = alt: har char ko unique id/position → merge apne aap commutative, central transform ki zaroorat nahi.
- Interview line: *"operations bhejta, snapshot nahi; concurrent ops OT/CRDT se transform/merge; sab converge; koi write lost nahi."* (implement nahi karna.)

**OT transform — step-by-step (dono side same pe converge):**
```
Base doc: "HELLO"
A ka op: insert("X", pos 0)        B ka op: insert("Y", pos 0)    [same waqt]

  A ke paas:                         B ke paas:
    apply A  -> "XHELLO"               apply B  -> "YHELLO"
    B aaya (pos 0); par A pehle        A aaya (pos 0); tie-break
    0 pe daal chuka -> SHIFT           (A pehle) -> pos 0 pe
    -> insert("Y", pos 1)             -> insert("X", pos 0)
    -> "XYHELLO"                      -> "XYHELLO"

  => dono = "XYHELLO"  (CONVERGE, dono ke akshar bache, koi lost nahi)
```

### 7 — Bottleneck
```
1. Crore WebSocket connections → dedicated CONNECTION-TIER (sockets hold), alag scale;
   connection stateful → LB consistent-route.
2. SHARD KEY = docId → ek doc ke saare editors + op-stream + OT ek hi shard pe (OT serialize).
   alag docs → alag shards. load bat-ta + OT clean.
3. Hot doc bounded hai (Google ~100 editor cap) → per-doc OT ek server pe theek.
4. Spike → message QUEUE se absorb. Redis pub/sub → replication + horizontal scale.
```

---

## WRITE PATH — ek edit ka safar (end-to-end)
```
1. User A ek char type karta -> op ban-ta { docId, userId, insert, pos, char, ts }
2. op WebSocket se A ke Conn-Server pe jaata
3. server: OT transform (concurrent ops ke against) -> apply
4. op Redis PUB/SUB pe publish -> baaki Conn-Servers -> unke WebSocket clients (User B) ko PUSH
5. op Redis BUFFER me jama -> periodically BATCH -> NoSQL edit-log me persist (har keystroke DB nahi)
6. B ka client op receive -> apna OT transform -> screen update
```

## SNAPSHOT + OPS — read optimization
```
Dikkat: doc kholte waqt 10-lakh ops replay karna = slow.
Fix:   periodically doc ka SNAPSHOT (poora current text) save karo + uske baad ke ops.
       doc-load = latest snapshot + baad ke thode ops apply. (append-log + periodic compaction.)
```

---

## ★ CAP — is design ka sabse gehra insight (NAYA tool)
Mock me pehle laga "CP chahiye (consistency har keemat pe)". **Galat** — actual = **AP**, par correctness convergence se milti:
- Agar CP hota → network blip pe typing **ruk** jaati. Reality: **offline bhi type karo, sync baad me** = AP behaviour.
- "Sabko same doc" strong-consistency se nahi, **eventual/convergent (OT/CRDT)** se aati.

★★ **PER-COMPONENT CAP** (poore system pe ek CAP nahi — har data ka apna):
```
doc-edits            → AP  (available, append-log, converge via OT)
permissions/ownership→ CP  (strong-consistent: nikaale gaye user ko turant block)
```
CAP ka faisla SIRF **partition ke waqt** matter karta; no-partition = dono milte.

---

## FINAL ARCHITECTURE
```
   User A                                   User B
     │  WebSocket (persistent 2-way)          │  WebSocket
     ▼                                        ▼
  Conn-Server-1 ── publish op ─► Redis Pub/Sub ─► Conn-Server-2
     │  (buffer edits)                              │
     ▼                                              ▼
  OT/merge (per docId shard)  ───────────► NoSQL edit-log (Cassandra: docId/timestamp)
                                            [permissions → strong-consistent store]
```

---

## TRAP BOX (ye galtiyan mock me aayi thi)
```
GALAT: Last-Write-Wins    -> kisi ka likha KHO jaata; collab me chalta nahi. OT/CRDT karo.
GALAT: "consistency = CP"  -> actual AP (offline-type + converge). CP hota to typing rukti.
GALAT: "consistency = SQL" -> DB data-SHAPE se aati; edit-log = NoSQL. "relations nahi" = NoSQL ki taraf.
GALAT: har keystroke DB hit -> nahi; buffer + batch persist.
GALAT: ek CAP poore system pe -> per-component (edits AP, permissions CP).
```

---

## 1-LINE RECALL
**Real-time collab = WebSocket (browser↔server push) + Redis pub/sub (server↔server fan-out) + edit-op-log in NoSQL + OT/CRDT (merge concurrent edits, no lost write, converge). CAP = AP (offline-type + converge), per-component (edits AP / permissions CP). Bottleneck = connection-tier + shard-by-docId.**
