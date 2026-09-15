# HLD TRADE-OFFS — 15 jode jo GHOOM-GHOOM ke aate hain

> **NAV** — KYA: 15 A-vs-B jode + wajah + bolne-wali line. UP: [MASTER](00_MASTER_SHEET.md) · saancha: [APPROACH_DELIVERY](HLD_APPROACH_DELIVERY.md) · detail: [FOUNDATIONS](FOUNDATIONS)

> KYUN ye file: trade-off koi alag "hoshiyari" nahi hai. Trade-off tabhi bol paate ho jab
> ek slot pe DO option pata ho + ek wajah. Agar sirf ek option pata hai to trade-off aa hi
> nahi sakta — ye knowledge ka sawaal hai, dimaag ka nahi.
> Achhi baat: ye list FINITE hai. Poore HLD me yahi ~15 jode ghoomte hain. Har design inhi
> me se 3-4 use karta hai.
>
> KAISE PADHNA: ratna nahi. Har jode ka DHAANCHA ek hi hai —
> **"A ya B? Maine ___ chuna, kyunki ___. Iski keemat ye hai ki ___."**
> Teesra hissa (keemat) sabse important hai — wahi batata hai ki tu samajh ke bol raha hai.
>
> HAR ENTRY ME: kab A · kab B · ek-line wajah · BOLNE WALI LINE (English) · kis design me aata.

---

## ★ SAANCHA (ye 3 line har trade-off pe fit hoti hain — ratna sirf ye hai)

```
   "There are two options here -- A and B.
    I'd go with A, because <requirement jo Step-1 me bola tha>.
    The trade-off is that we give up <B ka faayda>."
```
```
   Hinglish version (soch isi me, bolna English me):
   "Do raste hain -- A aur B. Main A lunga kyunki mera requirement ___ hai.
    Keemat ye hai ki ___ chhodna padega."
```

---

## 1. SQL vs NoSQL

```
   SQL    -> data ke RISHTE hain (user-order-payment), transaction/ACID chahiye,
             query flexible chahiye (join, filter, report)
   NoSQL  -> data bahut zyada + flat, access-pattern FIX hai, horizontal scale chahiye
             (key se uthana, timeline, logs, chat messages)

   WAJAH ek line: "kya mujhe POOCHNA hai (query) ya sirf UTHANA hai (key se)?"
   KEEMAT: SQL -> scale pe sharding ka dard . NoSQL -> join/transaction khud handle karo

   BOLNE WALI LINE:
     "The access pattern here is a simple key lookup at very high volume, so I'd use a
      NoSQL store. If we needed multi-table transactions, I'd flip to a relational DB."

   KAHAN AATA: url-shortener . twitter-feed . payment (yahan SQL) . bookmyshow (SQL)
```

## 2. SYNC vs ASYNC (REST/gRPC vs Queue)

```
   SYNC   -> caller ko JAWAB chahiye abhi (login, payment authorize, seat availability)
   ASYNC  -> caller ko jawab ka intezaar nahi (email, analytics, notification, report)

   WAJAH: "user screen pe ruka rahega ya nahi?"
   KEEMAT: async -> system eventually-consistent, duplicate handle karna padta, debug mushkil

   BOLNE WALI LINE:
     "The user doesn't need to wait for the email to be sent, so I'd push that to a queue
      and return immediately. The cost is that the system becomes eventually consistent."

   KAHAN AATA: message-queue . notification . payment . file-upload
```

## 3. PUSH vs PULL

```
   PUSH  -> server data bhejta (WebSocket/SSE) -> real-time chahiye, kam clients
   PULL  -> client maangta (polling / poll()) -> client apni raftaar se, slow client marta nahi

   WAJAH: "real-time kitna zaroori hai vs consumer ko dabne se bachana kitna zaroori hai"
   KEEMAT: push -> slow consumer overload ho sakta, connection maintain karni padti
           pull -> thodi latency (agle poll tak wait)

   BOLNE WALI LINE:
     "Kafka is pull-based -- consumers poll at their own pace, so a slow consumer never
      gets overwhelmed. The trade-off is a little extra latency."

   KAHAN AATA: message-queue . notification . twitter-feed (fanout-on-write vs read)
```

## 4. FANOUT-ON-WRITE vs FANOUT-ON-READ (push vs pull ka feed-version)

```
   ON-WRITE -> post karte hi sab followers ki timeline me daal do
               -> READ super-fast . normal user ke liye best
   ON-READ  -> timeline maangne pe live jodo
               -> celebrity (10M follower) ke liye, warna 10M writes ek post pe

   WAJAH: "read zyada hain ya write? aur kya koi user asaadharan bada hai?"
   KEEMAT: on-write -> celebrity pe write-storm . on-read -> har read mehnga
   ASLI JAWAB: HYBRID -- normal user on-write, celebrity on-read

   BOLNE WALI LINE:
     "I'd fan out on write for regular users, but for celebrities that would mean millions
      of writes per post, so for them I'd merge at read time -- a hybrid."

   KAHAN AATA: twitter-feed . news-aggregator
```

## 5. CACHE-ASIDE vs WRITE-THROUGH vs WRITE-BACK

```
   CACHE-ASIDE   -> app dekhta: cache me hai? nahi -> DB -> cache me daal do
                    (sabse common default)
   WRITE-THROUGH -> likho to cache aur DB DONO me ek saath -> cache hamesha fresh, write slow
   WRITE-BACK    -> pehle cache, DB baad me (async) -> write fast, crash pe DATA LOSS

   WAJAH: "stale data chalega? ya write ki speed zyada zaroori hai?"
   KEEMAT: aside -> pehla request slow (miss) . through -> har write slow
           back -> sabse tez par sabse risky

   BOLNE WALI LINE:
     "I'd use cache-aside as the default. If reads must never be stale, write-through.
      I'd avoid write-back here because a crash would lose data."

   KAHAN AATA: distributed-cache . url-shortener . twitter-feed
```

## 6. STRONG vs EVENTUAL CONSISTENCY

```
   STRONG   -> paisa, seat-booking, inventory (do log ek hi seat na le lein)
   EVENTUAL -> like-count, feed, analytics, cache, search-index

   WAJAH: "galat/purana data 2 second ke liye dikh jaaye to kya bigdega?"
   KEEMAT: strong -> slow + availability girti (CAP) . eventual -> user ko purana dikh sakta

   BOLNE WALI LINE:
     "For the seat booking itself I need strong consistency -- two users can't get the same
      seat. But the 'seats available' count on the listing page can be eventually consistent."

   KAHAN AATA: bookmyshow . payment . stock-broker . twitter-feed (eventual)
```

## 7. CAP — CP vs AP (ye upar wale ka distributed-version)

```
   network toot-ta hai (P hamesha maanna padta) -> bachte do: C ya A
   CP -> partition me REJECT kar do, galat data mat do (payment, booking, DB-leader)
   AP -> partition me bhi jawab do, baad me sudhar lo (feed, cache, DNS, shopping-cart)

   BOLNE WALI LINE:
     "During a network partition I'd rather reject the write than double-book a seat --
      so this component is CP. The browse/search path can stay AP."

   KAHAN AATA: har design ke deep-dive me poocha ja sakta
```

## 8. L4 vs L7 LOAD BALANCER

```
   L4 (TCP)  -> sirf IP/port dekh ke aage phenk deta -> bahut TEZ, kuch samajhta nahi
   L7 (HTTP) -> URL/header/cookie dekh sakta -> path-based routing, sticky session, TLS-terminate
                -> thoda slow, par SAMAJHDAR

   WAJAH: "routing ka faisla content ke aadhar pe lena hai ya nahi?"
   BOLNE WALI LINE:
     "I'd use an L7 load balancer because I want to route /api and /static differently
      and terminate TLS there. L4 would be faster but can't see the request."

   KAHAN AATA: har design ke HL-boxes me (LB ka box)
```

## 9. HORIZONTAL vs VERTICAL SCALING

```
   VERTICAL   -> badi machine le lo -> simple, no code-change -> par CEILING hai + SPOF
   HORIZONTAL -> zyada machine -> unlimited-ish + HA -> par state/sharding/coordination ka dard

   BOLNE WALI LINE:
     "Vertical scaling is simpler and I'd use it early, but it has a ceiling and it's a single
      point of failure -- so past that point I'd scale horizontally behind a load balancer."

   KAHAN AATA: estimation ke baad har design me
```

## 10. REPLICATION vs SHARDING (dono alag problem solve karte — ye confuse hota hai)

```
   REPLICATION -> ek hi data ki KAI COPY  -> READ scale + HA (machine mare to bacha rahe)
   SHARDING    -> data ko TUKDON me baant -> WRITE scale + storage (ek machine me fit nahi)

   WAJAH: "read zyada ho rahe (replica) ya data/write hi nahi sama raha (shard)?"
   KEEMAT: replication -> replica-lag (stale read) . sharding -> cross-shard query/join mushkil,
           re-shard karna dard, hot-shard ban sakta

   BOLNE WALI LINE:
     "Reads are the bottleneck here, so read replicas first. If the write volume itself
      outgrows one machine, then I'd shard by userId."

   KAHAN AATA: url-shortener . twitter-feed . payment . message-queue (partition = sharding)
```

## 11. NORMALIZE vs DENORMALIZE

```
   NORMALIZE   -> data ek jagah, duplicate nahi -> update aasan, par JOIN karna padta
   DENORMALIZE -> data copy karke saath rakh do -> read FAST (join nahi), par update kai jagah

   WAJAH: "read-heavy hai ya write/update-heavy?"
   BOLNE WALI LINE:
     "This is read-heavy and the join was the bottleneck, so I'd denormalize and store the
      author's name alongside the post. The cost is updating it in two places."

   KAHAN AATA: twitter-feed . news-aggregator . bookmyshow
```

## 12. LONG-POLLING vs WEBSOCKET vs SSE

```
   LONG-POLLING -> request bhejo, server rok ke rakhe, data aaye to jawab -> simple, purana, mehnga
   WEBSOCKET    -> dono taraf se baat (full-duplex) -> chat, live-trading, collab
   SSE          -> sirf server->client stream -> notification/feed-update (halka, HTTP pe hi)

   WAJAH: "client ko bhi bhejna hai (WS) ya sirf sunna hai (SSE)?"
   KEEMAT: WebSocket -> har client ki connection zinda rakhni padti (memory + LB sticky)

   BOLNE WALI LINE:
     "Chat is bidirectional, so WebSocket. If it were only server-to-client updates,
      SSE would be lighter."

   KAHAN AATA: chat/messaging . google-docs-collab . stock-broker . notification
```

## 13. AT-LEAST-ONCE vs EXACTLY-ONCE (delivery)

```
   AT-LEAST-ONCE -> kaam pehle, commit baad me -> kuch khoyega nahi, DUPLICATE aa sakta (default)
   EXACTLY-ONCE  -> na loss na duplicate -> mehnga, slow, har jagah possible nahi

   ASLI JAWAB: at-least-once + IDEMPOTENT consumer (eventId dedup) = exactly-once EFFECT
   BOLNE WALI LINE:
     "I'd take at-least-once delivery and make the consumer idempotent using an event id,
      rather than paying the cost of true exactly-once."

   KAHAN AATA: message-queue . payment . notification
```

## 14. BATCH vs STREAM processing

```
   BATCH  -> raat ko ek saath (daily report, billing, reconciliation) -> simple, sasta, DER se
   STREAM -> event aate hi turant (fraud-detect, live dashboard, alerting) -> turant, par mehnga+complex

   WAJAH: "jawab ab chahiye ya kal subah chalega?"
   BOLNE WALI LINE:
     "Daily reconciliation can be a batch job. Fraud detection can't wait, so that one
      has to be a stream."

   KAHAN AATA: payment . news-aggregator . analytics wale hisse
```

## 15. SINGLE-LEADER vs MULTI-LEADER / LEADERLESS

```
   SINGLE-LEADER -> ek hi jagah write -> conflict HI nahi -> par leader = bottleneck + SPOF
   MULTI-LEADER  -> kai jagah write (multi-region) -> fast local write -> par CONFLICT aayega
   LEADERLESS    -> kisi ko bhi likho, quorum se padho (Dynamo/Cassandra) -> HA, par version-conflict

   WAJAH: "ek jagah likhna kaafi hai, ya multi-region latency kaatni hai?"
   KEEMAT: multi-leader/leaderless -> conflict-resolution (last-write-wins / CRDT) likhna padta

   BOLNE WALI LINE:
     "I'd keep a single leader for writes so there are no conflicts. If we needed low write
      latency in multiple regions, we'd go multi-leader and pay for conflict resolution."

   KAHAN AATA: payment . distributed-cache . google-docs-collab (CRDT/OT) . DB-layer har design me
```

---

## ★ BONUS — chhote jode jo ek-line me bol dete hain

```
   301 vs 302          -> 301 permanent (browser cache karega, analytics chali gayi)
                          302 temporary (har baar server aayega -> click-count milega)
                          "I'd use 302 so we keep click analytics."

   CDN: pull vs push   -> pull = pehli request pe origin se le aata (simple)
                          push = pehle se bhar do (bade file/launch ke liye)

   Rate-limit algo     -> token-bucket = burst allow karta (user-friendly)
                          fixed-window = simple par boundary pe double-burst
                          sliding-window = sahi par thoda mehnga

   Idempotency key     -> "same request dobara aaye to dobara paisa na kate"

   Bloom filter        -> "definitely-not-present" sasta check -> DB hit bachata
                          (false-positive ok, false-negative kabhi nahi)

   Hashing: modulo vs consistent
                       -> modulo -> node add/remove pe SAB key ghoom jaati (cache poori thandi)
                          consistent hashing -> sirf thodi key ghoomti
                          "That's why distributed caches use consistent hashing."
```

---

## ★ KAISE USE KARNA (padhne ka tareeka)

```
   1. Har din 3 jode padho (5 min) -> hafte me poori list ek baar ghoom jaayegi.
   2. Kisi bhi design ko revise karte waqt rok ke poochho: "yahan is slot pe DOOSRA option kya tha?"
      -> answer isi file me hai. Jodne se yaad rehta, alag se ratne se nahi.
   3. Mock me kam-se-kam 2 trade-off BOLO. Dono taraf bolna hi asli cheez hai --
      "A chuna, B ki keemat ye thi."
   4. Ek bhi na yaad aaye to ye line kaafi hai (honest + safe):
      "There's a trade-off here -- the other option would be X, but given the requirement
       I stated earlier, I'd stay with this one."
```
