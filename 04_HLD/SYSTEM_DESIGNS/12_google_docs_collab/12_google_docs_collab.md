# Google Docs — Collaborative Editor — 7-STEP RAIL (single spine, revise top→bottom)

> 3-Sep MOCK me KHUD derive kiya (novel design). 3 NAYA tool the: per-component CAP · WebSocket+Redis-pubsub · OT/CRDT.
> RAIL (04_HLD/HLD_APPROACH_DELIVERY.md). Merged into clean 7-step 7-Sep. WRITE-DOMINATED tool.
> PROBLEM (crux): 2+ log ek doc EK SAATH edit -> koi clash/override na ho, likha kho na jaaye, near-real-time dikhe, aakhir me sabka doc SAME.
>   = real-time concurrent edit + no-lost-write + convergence = poore design ka dil.

---

## STEP 1 — REQUIREMENTS
```
FR:  - multiple users same doc same time edit
     - har user ko baaki ke edits real-time dikhein
     - koi edit clash / override / lost na ho
NFR: - LOW LATENCY (typing instant feel ho)
     - HIGH AVAILABILITY (typing kabhi na ruke, offline bhi chale)
     - CONVERGENCE (sab eventually same doc pe aayein)
```

## STEP 2 — ESTIMATE (rough, atko mat)
```
~100M users · ~30-40M DAU · edits/sec HIGH (write-DOMINATED tool).
=> Point: WRITE-scale massive -> buffering + sharding chahiye. (exact number matter nahi.)
```

## STEP 3 — API (resource-based)
```
GET  /documents/{docId}            -> doc laao
POST /documents/{docId}/edits      -> is doc pe ek EDIT (operation) bhejo
WebSocket /documents/{docId}       -> real-time 2-way channel (edits push/receive)
```

## STEP 4 — DATA MODEL — "EDIT-OPERATIONS LOG" (NoSQL)
```
Har edit = ek OPERATION event (snapshot nahi):
   { docId, userId, opType (insert/delete), position, char/text, timestamp }
- Doc current state = us doc ke saare ops ORDER me apply karke banta.
- Access pattern: "ek docId ke SAARE edits, TIME-order me" -> key=docId, order=timestamp.
- Write-heavy append-log -> NoSQL (Cassandra: partition=docId, cluster=timestamp = ekdum fit; Mongo bhi ok).
- ★ DB choice DATA-SHAPE se aati, "consistency chahiye" se NAHI. ("relations nahi" = NoSQL ki taraf, SQL ki nahi.)
- permissions/ownership -> alag SQL/strong-consistent store (relations + CP).
```

## STEP 5 — HL BOXES (final architecture)
```
   User A                                   User B
     │  WebSocket (persistent 2-way)          │  WebSocket
     ▼                                        ▼
  Conn-Server-1 ── publish op ─► Redis Pub/Sub ─► Conn-Server-2
     │  (buffer edits)                              │
     ▼                                              ▼
  OT/merge (per docId shard)  ───────────► NoSQL edit-log (Cassandra: docId/timestamp)
                                            [permissions → strong-consistent SQL store]
box KYUN:
  CDN -> static app | LB -> traffic (WebSocket consistent-route, connection stateful)
  Conn-Server -> WebSocket hold + OT + edit-buffer | Redis -> pub/sub (server↔server fanout) + write-buffer
  NoSQL edit-log -> ops persist | SQL -> permissions (CP).
```

## STEP 6 — DEEP DIVE (asli, unique hissa): concurrent edit MERGE + write-path
```
Do log same instant same position type karein -> winner mat chuno, MERGE karo, DONO bache:
- Text mat bhejo -> OPERATIONS bhejo. A:insert("X",0), B:insert("Y",0).
- A ne "XHELLO" banaya. B ka op aata -> system TRANSFORM (A pehle pos0 pe daal chuka -> B ka pos 0→1) -> "XYHELLO".
- B ki taraf bhi A ka op transform -> dono end pe EXACT same "XYHELLO", dono ke akshar bache.
- Naam = OPERATIONAL TRANSFORMATION (OT). Order (XY vs YX) deterministic tie-break (userId/timestamp); dono survive.
- CRDT = alt: har char ko unique id/position -> merge apne-aap commutative, central transform ki zaroorat nahi.
- LINE: "operations bhejta, snapshot nahi; concurrent ops OT/CRDT se transform/merge; sab converge; koi write lost nahi." (implement nahi karna.)

OT transform — step-by-step (dono side same pe converge):
   Base doc: "HELLO"
   A ka op: insert("X", pos 0)        B ka op: insert("Y", pos 0)    [same waqt]
     A ke paas:                         B ke paas:
       apply A  -> "XHELLO"               apply B  -> "YHELLO"
       B aaya (pos 0); A pehle            A aaya (pos 0); tie-break
       0 pe daal chuka -> SHIFT           (A pehle) -> pos 0 pe
       -> insert("Y", pos 1)             -> insert("X", pos 0)
       -> "XYHELLO"                      -> "XYHELLO"
   => dono = "XYHELLO"  (CONVERGE, dono ke akshar bache, koi lost nahi)

WRITE PATH — ek edit ka safar (end-to-end):
   1. User A char type -> op ban-ta { docId, userId, insert, pos, char, ts }
   2. op WebSocket se A ke Conn-Server pe
   3. server: OT transform (concurrent ops ke against) -> apply
   4. op Redis PUB/SUB pe publish -> baaki Conn-Servers -> unke WebSocket clients (User B) ko PUSH
   5. op Redis BUFFER me jama -> periodically BATCH -> NoSQL edit-log persist (har keystroke DB nahi)
   6. B ka client op receive -> apna OT transform -> screen update

SNAPSHOT + OPS — read optimization:
   Dikkat: doc kholte 10-lakh ops replay = slow.
   Fix: periodically doc ka SNAPSHOT (poora current text) + uske baad ke ops.
        doc-load = latest snapshot + baad ke thode ops apply. (append-log + periodic compaction.)

★ CAP — is design ka sabse gehra insight (per-component):
   Pehle laga "CP chahiye". GALAT -> actual AP, correctness convergence se:
     CP hota -> network blip pe typing RUK jaati. Reality: offline bhi type, sync baad = AP.
     "Sabko same doc" strong-consistency se nahi, EVENTUAL/convergent (OT/CRDT) se.
   ★★ PER-COMPONENT CAP (poore system pe ek CAP nahi):
     doc-edits             -> AP  (available, append-log, converge via OT)
     permissions/ownership -> CP  (strong: nikaale-gaye user ko turant block)
   CAP faisla SIRF partition ke waqt matter; no-partition = dono milte.
```

## STEP 7 — BOTTLENECK / SCALE
```
1. Crore WebSocket connections -> dedicated CONNECTION-TIER (sockets hold), alag scale;
   connection stateful -> LB consistent-route.
2. SHARD KEY = docId -> ek doc ke saare editors + op-stream + OT ek hi shard pe (OT serialize).
   alag docs -> alag shards. load bat-ta + OT clean.
3. Hot doc bounded (Google ~100 editor cap) -> per-doc OT ek server pe theek.
4. Spike -> message QUEUE absorb. Redis pub/sub -> replication + horizontal scale.
```

---

## ═══ APPENDIX — TRAP BOX + RECALL ═══
```
TRAP BOX (mock me aayi galtiyan):
   GALAT: Last-Write-Wins    -> kisi ka likha KHO jaata; collab me nahi chalta. OT/CRDT karo.
   GALAT: "consistency = CP"  -> actual AP (offline-type + converge). CP hota to typing rukti.
   GALAT: "consistency = SQL" -> DB data-SHAPE se; edit-log = NoSQL. "relations nahi" = NoSQL ki taraf.
   GALAT: har keystroke DB hit -> nahi; buffer + batch persist.
   GALAT: ek CAP poore system pe -> per-component (edits AP, permissions CP).
```
**1-LINE RECALL:** Real-time collab = WebSocket (browser↔server push) + Redis pub/sub (server↔server fan-out) + edit-op-log in NoSQL + OT/CRDT (merge concurrent edits, no lost write, converge). CAP = AP (offline-type + converge), per-component (edits AP / permissions CP). Bottleneck = connection-tier + shard-by-docId.

---

[← HLD README](../README.md)
