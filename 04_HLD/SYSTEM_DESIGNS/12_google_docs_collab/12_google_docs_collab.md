# Google Docs (Collaborative Editor) — POORA ROUND (4 MOVE)

> **NAV** — ARCHETYPE D (real-time) · DIL: saath edit, kuch na khoye, sab same. UP: [MASTER](../../00_MASTER_SHEET.md) · CONCEPTS: [CAP](../../FOUNDATIONS/08_cap_theorem.md) · [pubsub/queues](../../FOUNDATIONS/07_message_queues.md) · trade-off: [WebSocket vs SSE](../../TRADEOFFS.md)

> 3-Sep MOCK me Arpan ne KHUD derive kiya (novel design) — 3 naye tool the:
> per-component CAP · WebSocket + Redis pub/sub · OT/CRDT.
> 15-Sep: asli mock-video ke hisaab se dobara likha — 4 move, koi rail nahi.
>
> PROBLEM (crux): 2+ log ek doc EK SAATH edit karein -> koi clash/override na ho, likha kho na jaaye,
> turant dikhe, aur aakhir me sabka doc SAME ho.
> = real-time concurrent edit + no-lost-write + convergence = poore design ka dil.

```
★★ TEEN NIYAM (poori file par lagte — [APPROACH_DELIVERY](../../HLD_APPROACH_DELIVERY.md) 5b)
   1. PERFECT design ek saath mat banao — chhote se shuru, dikkat pe badhao
   2. NUMBER ke peeche mat bhaago — bolo, ek faisla nikaalo, aage badho
   3. BOTTLENECK ratto mat — KHUD USER banke raasta chalao, khud dikh jaayega
```

---

# MOVE 1 — POOCHO (board pe abhi kuch nahi)

```
   TU: "Google Docs bada hai — editing, comments, sharing/permissions, version history, offline.
        Aap kis pe focus karwana chahenge? Main real-time collaborative EDITING pe ja sakta hoon."

   TU: "Kuch cheezein confirm kar lun —
          - ek doc pe ek saath kitne log edit karenge? (2-3, ya 100?)
          - OFFLINE editing chahiye — net gaya to typing chalti rahe?
          - permissions/sharing scope me hai?
          - rich text (bold/image) ya plain text kaafi hai?"

   ★ "offline chahiye?" — is sawaal ka jawab poore CAP faisle ko tay karta hai.
```

---

# MOVE 2 — DO CHHOTE BLOCK LIKHO

```
   ┌──────────────────────┐    ┌───────────────────────────────────┐
   │ Google Docs          │    │ Use cases:                        │
   │   - Document         │    │   - kai log EK doc saath edit      │
   │   - User             │    │   - dusron ke edit TURANT dikhein  │
   │   - Edit (operation) │    │   - koi edit clash/override na ho  │
   │   - Permission       │    │                                   │
   └──────────────────────┘    │ NOT in scope: comments . version  │
                               │   history . rich-text formatting  │
   ┌──────────────────────────┐└───────────────────────────────────┘
   │ Kya chahiye (NFR):       │
   │  - LOW LATENCY (typing   │ <- DIL
   │    instant lage)         │
   │  - HIGH AVAILABILITY     │
   │    (typing kabhi na ruke,│
   │     offline bhi chale)   │
   │  - CONVERGENCE (sab       │
   │    aakhir me same doc pe)│
   └──────────────────────────┘

   TU: "Teesri line sabse important hai — main 'sab same doc pe' ko strong consistency se
        nahi laaunga, CONVERGENCE se laaunga. Kyun, wo aage bataunga."
```

```
   Numbers (halke se, atkna nahi):
     ~100 M users . ~30-40 M DAU
     edits/sec BAHUT zyada — ye WRITE-DOMINATED tool hai (har keystroke ek event)

   FAISLA: write ka bojh bhaari hai -> buffering + sharding chahiye
           (exact number yahan maayne nahi rakhta)
```

---

# MOVE 3 — BOXES BANAO (chhota banao, phir dikkat pe badhao)

```
   TU: "Sabse simple se shuru."

        USER ──► [ App ] ──► [ DB ]   doc ka poora text pada hai
                              "Save" dabao -> poora text overwrite

   TU: "Ek akela banda likh raha ho to chalta hai. Ab do log bithaate hain."
```

### dikkat 1 — "do log ne saath save kiya, baad wale ne pehle ka MITA diya"

```
        A ne likha: "HELLO WORLD"   ──save──►  DB
        B ne likha: "HELLO THERE"   ──save──►  DB   (A ka kaam GAYAB)

        ye LAST-WRITE-WINS hai -> collab me ye fail hai

   FAISLA: poora TEXT mat bhejo — sirf OPERATION bhejo

        A ka op:  { insert "X", position 0 }
        B ka op:  { delete  position 5 }

   TU: "Agar dono poora document bhejenge to ek doosre ko overwrite karenge hi.
        Isliye main text nahi, EDIT-OPERATION bhejunga — phir dono ko apply kar sakta hoon,
        kisi ka likha khoyega nahi."
```

### dikkat 2 — "B ko A ka edit dikhega kab? refresh karne pe?"

```
        HTTP request-response se nahi hoga — server ko KHUD bhejna padega

   FAISLA: WEBSOCKET (do-tarfa, zinda connection)

        User A ══ WebSocket ══ [ Conn-Server ] ══ WebSocket ══ User B
                (dono taraf se push, connection khula rehta)

   TU: "Ye do-tarfa chahiye — user type bhi karta hai aur doosron ke edit receive bhi karta hai.
        Agar sirf server-se-client bhejna hota to SSE halka padta, par yahan dono taraf jaana hai."
```

### dikkat 3 — "A server-1 pe juda hai, B server-2 pe — A ka op B tak pahunchega hi nahi"

```
        User A ── Conn-Server-1          Conn-Server-2 ── User B
                        │                      ▲
                        └── ye dono ek doosre ko jaante hi nahi ──┘

   FAISLA: REDIS PUB/SUB (server-to-server fanout)

        User A ══► Conn-Server-1 ──publish──► [ REDIS PUB/SUB ] ──► Conn-Server-2 ══► User B

   TU: "Client ke saath WebSocket, aur server ke beech pub/sub. Do alag kaam hain —
        WebSocket browser tak, pub/sub server-se-server."
```

### dikkat 4 — "dono ne EK SAATH position 0 pe type kiya"

```
        base doc: "HELLO"
        A ka op: insert("X", pos 0)        B ka op: insert("Y", pos 0)     [ek hi waqt]

        seedha apply kiya to:
            A ke paas -> "XHELLO"
            B ke paas -> "YHELLO"        <- DO ALAG DOC. diverge ho gaya.

   FAISLA: OPERATIONAL TRANSFORMATION (OT) — winner mat chuno, TRANSFORM karo

        A ke paas:                          B ke paas:
          A apply  -> "XHELLO"                B apply  -> "YHELLO"
          ab B ka op aaya (pos 0)             ab A ka op aaya (pos 0)
          A pehle 0 pe daal chuka             tie-break: A pehle tha
          -> B ka op SHIFT: pos 0 -> 1        -> A ka op pos 0 pe
          -> insert("Y", pos 1)               -> insert("X", pos 0)
          -> "XYHELLO"                        -> "XYHELLO"

        => DONO "XYHELLO"  — converge ho gaye, dono ke akshar bache, kuch lost nahi

   ★ order (XY vs YX) ka faisla deterministic tie-break se (userId / timestamp) —
     par dono akshar zinda rehte hain.
   ★ CRDT = doosra raasta: har character ko apni unique id/position do -> merge apne aap
     commutative ho jaata, central transform ki zaroorat hi nahi.
   ★ BOLNE WALI LINE: "Main operations bhejta hoon, snapshot nahi; concurrent operations ko
     OT ya CRDT se transform/merge karta hoon — sab converge karte hain, koi write lost nahi hota."
     (implement nahi karna — bas ye samajh bolni hai.)
```

### dikkat 5 — "har keystroke pe DB hit? DB mar jaayega"

```
        har akshar = ek write -> 40M DAU x har keystroke -> DB khatam

   FAISLA: BUFFER + BATCH

        op ──► Redis buffer me jama ──► thodi-thodi der me BATCH ──► NoSQL edit-log

   TU: "Real-time hissa memory/pub-sub se chalta hai; DB me batch me likhta hoon.
        User ko wait nahi karna padta aur DB pe hathoda nahi padta."
```

### dikkat 6 — "doc kholne pe 10 lakh operation replay karne padenge"

```
        doc = saare ops order me apply karke banta  -> 10 lakh op = doc kholna SLOW

   FAISLA: SNAPSHOT + uske baad ke ops

        [ snapshot: poora text, har X ops baad ]  +  [ uske baad ke thode ops ]
                            │
                            ▼
                doc load = latest snapshot + baad ke ops apply

   (yahi append-log + periodic compaction ka funda hai)
```

### dikkat 7 — "network ek second ko blip hua — typing ruk jaayegi?"

```
   ★ CAP — is design ka sabse gehra insight

     Pehli soch: "consistency chahiye -> CP"     <- ye GALAT nikla
     Sach       : agar CP rakha to partition pe TYPING RUK jaayegi.
                  Asli Google Docs me tum offline bhi type karte ho, baad me sync hota hai.
                  => ye AP hai.
                  "sabko same doc" strong-consistency se nahi, CONVERGENCE (OT/CRDT) se aata hai.

   ★★ PER-COMPONENT CAP (poore system pe ek hi CAP nahi hota):

        doc edits             ->  AP   (available rehna zaroori, append-log, OT se converge)
        permissions/ownership ->  CP   (strong — hataye gaye user ko TURANT block karna hai)

   ★ CAP ka faisla SIRF partition ke waqt maayne rakhta hai; partition nahi hai to dono milte hain.
```

### dikkat 8 — "crore WebSocket connections ek hi server pe?"

```
   FAISLA:
     1. alag CONNECTION TIER — sirf sockets hold karne wale server, alag se scale honge
        connection STATEFUL hai -> LB ko consistent routing karna padega
        (user hamesha usi server pe wapas jaaye)
     2. SHARD KEY = docId — ek doc ke saare editor + op-stream + OT EK shard pe
        (OT ko serialize karna hota hai -> ek jagah hona zaroori)
        alag doc -> alag shard -> load bat gaya
     3. hot doc bounded hai (Google ~100 editor ki cap rakhta) -> per-doc OT ek server pe theek chalta
     4. spike aaye -> queue absorb kare; Redis pub/sub replicate + horizontally scale
```

### ab poora naksha (jahan pahunche) + har box ka KYUN

```
   User A                                            User B
     ║  WebSocket (zinda, 2-taraf)                      ║  WebSocket
     ▼                                                  ▼
  [ Conn-Server-1 ] ──publish──► [ REDIS PUB/SUB ] ──► [ Conn-Server-2 ]
     │   OT transform                    │
     │   edit buffer                     │
     ▼                                   ▼
  [ Redis buffer ] ──batch──► [ NoSQL edit-log (Cassandra) ]
                                   partition = docId, cluster = timestamp
                              [ SQL store ] permissions/ownership (CP)
     (aage) [ CDN ] static app file    [ LB ] WebSocket ko consistent-route karta

     CDN         : app ki static files user ke paas se
     LB          : traffic baantna — par connection stateful hai -> consistent routing
     Conn-Server : WebSocket pakadta + OT karta + edits buffer karta
     Redis       : (a) pub/sub server-to-server fanout  (b) write buffer
     NoSQL log   : saare ops persist (append-only, write-heavy)
     SQL         : permissions — yahan strong consistency chahiye
```

```
   EK EDIT KA POORA SAFAR (end-to-end, bolne layak):
     1. User A ne ek akshar type kiya -> op bana { docId, userId, insert, pos, char, ts }
     2. op WebSocket se A ke Conn-Server tak
     3. server ne OT transform kiya (concurrent ops ke against) -> apply
     4. op Redis pub/sub pe publish -> baaki Conn-Servers -> unke clients (User B) ko PUSH
     5. op Redis buffer me jama -> thodi der me BATCH -> NoSQL edit-log
     6. B ke client ne op liya -> apni taraf transform kiya -> screen update
```

---

# MOVE 4 — BOLTE-BOLTE JODO (jo poocha jaaye, wahi kholo)

## ► "API kya hogi?"

```
   GET       /documents/{docId}          ->  doc laao (snapshot + baad ke ops)
   POST      /documents/{docId}/edits    ->  ek edit (operation) bhejo
   WebSocket /documents/{docId}          ->  real-time 2-taraf channel
```

## ► "Data model kya, aur kaunsa DB?"

```
   Har edit ek OPERATION event hai (snapshot nahi):

       { docId, userId, opType (insert/delete), position, char/text, timestamp }

   doc ka current roop = us doc ke saare ops ORDER me apply karke banta hai

   ACCESS PATTERN: "ek docId ke SAARE edits, TIME order me"
        -> partition key = docId . clustering = timestamp
        -> Cassandra ekdum fit (append-heavy log) . Mongo bhi chal jaayega

   ★ DB ka chunaav DATA KI SHAKAL se aata hai, "consistency chahiye" se NAHI.
     "Relations nahi hain" -> NoSQL ki taraf le jaata hai, SQL ki taraf nahi.
   ★ permissions/ownership -> alag SQL / strong-consistent store (relations + CP)
```

## ► "Concurrent edit kaise merge karoge?" (design ka dil — upar dikkat-4 me poora hai)

```
   OT ka nichod:
     text nahi, OPERATIONS bhejo -> concurrent ops ko TRANSFORM karo -> dono zinda rahein -> converge
     "HELLO" + insert(X,0) + insert(Y,0) -> dono taraf "XYHELLO"

   CRDT: har char ko unique id -> merge commutative -> central transform nahi chahiye
```

## ► "Kahan tootega / scale?"

```
   ★ RATTO MAT — user ka raasta chalao:

      user doc kholta hai
          │
          ├─► WebSocket    -> crore connections?    -> alag CONNECTION TIER + consistent routing
          ├─► Conn-Server  -> OT kahan hoga?        -> SHARD by docId (ek doc = ek jagah)
          ├─► Redis pubsub -> spike?                -> replicate + horizontal scale
          ├─► DB writes    -> har keystroke?        -> buffer + batch
          └─► doc load     -> 10 lakh op replay?    -> SNAPSHOT + baad ke ops
```

---

## ═══ TRAP BOX — mock me jo galtiyan hui (ye sabse kaam ki cheez hai) ═══

```
   GALAT: Last-Write-Wins            -> kisi ka likha KHO jaata. Collab me nahi chalta -> OT/CRDT
   GALAT: "consistency chahiye = CP" -> asal me AP hai (offline type + converge).
                                        CP hota to network blip pe typing RUK jaati.
   GALAT: "consistency chahiye = SQL"-> DB data ki SHAKAL se aata hai; edit-log = NoSQL.
                                        "relations nahi" -> NoSQL ki taraf.
   GALAT: har keystroke DB hit       -> nahi; buffer + batch persist
   GALAT: ek CAP poore system pe     -> PER-COMPONENT (edits AP, permissions CP)
```

**1-LINE RECALL:** Real-time collab = WebSocket (browser↔server push) + Redis pub/sub (server↔server fanout)
+ edit-op log NoSQL me + OT/CRDT (concurrent edits merge, koi write lost nahi, sab converge).
CAP = AP (offline type + converge), aur per-component (edits AP / permissions CP).
Bottleneck = connection tier + shard by docId.

---

[← MASTER SHEET](../../00_MASTER_SHEET.md)
