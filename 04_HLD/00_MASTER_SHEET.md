# HLD MASTER SHEET — koi bhi design ASSEMBLE karne ka tareeka

<a id="index"></a>

## INDEX — click karo, seedha wahan

**Kaise use karo:** doubt aaya -> neeche **DOUBT DHOONDHO** me shabd dhoondho -> click. Har PART ke upar `upar INDEX` link se wapas yahan. Concept ki detail / poora design / chala ke dekha = doosri file (neeche **DOOSRI FILE**).

| PART | andar kya |
|---|---|
| [1 NAKSHA + NIYAM](#part1) | [kaunsi file kab](#naksha) · [teen baatein](#teen-baatein) |
| [2 CROSS-QUESTION](#part2) | [6 sawaal](#chhe-sawaal) · [8 dabbe](#dabbe) · [jawab se pehle 5 sec](#jawab-se-pehle) · [Q1-Q22 ab design file me](#bank) |
| [3 DESIGN KHADA KARNA](#part3) | [teen line](#teen-line) · [6 archetype](#archetype) · [rail (7 step)](#rail) · [block menu](#block-menu) · [drill wali rows + jode](#drill-rows) · [A ya B trade-off](#trade-off) · [14 design ka DIL](#designs) |
| [4 ATKE TO / ROUND DIN](#part4) | [anjaan design](#anjaan) · [LinkedIn misaal](#linkedin) · [kuch na pata](#na-pata) · [checklist](#checklist) |
| [5 BOLNA](#part5) | [kaise bolna](#kaise-bolna) · [SHABD](#shabd) |

### DOUBT DHOONDHO (doubt -> jawab kahan)

| doubt / shabd | jao |
|---|---|
| server / node / DB gira · failover | [REPLICA](#dabba-replica) · [LB](#dabba-lb) |
| provider slow · timeout · circuit breaker | [CIRCUIT BREAKER](#dabba-cb) |
| cache gira · stampede · hot key expire | [CACHE](#dabba-cache) |
| region / data centre gira · multi-AZ | [url shortener](SYSTEM_DESIGNS/01_url_shortener/01_url_shortener.md) dikkat 5 |
| request do baar · retry · double click · idempotency | [IDEMPOTENCY KEY](#dabba-idem) |
| key lagi par bhejna fail | [notification](SYSTEM_DESIGNS/04_notification_system/04_notification_system.md) dikkat 4b |
| do user ek saath · race · ek seat · lock | [DB TRANSACTION](#dabba-txn) · [bookmyshow](#d09) |
| achanak spike · flash sale · IPL · pre-warm | [QUEUE](#dabba-queue) |
| 10x users · scale kaise | [rail step 7](#rail) |
| DB bada · writes zyada · shard key · consistent hashing | [SHARD](#dabba-shard) · [TO9](#to9) |
| hot key · celebrity · hot partition | [twitter](#d03) · [cache design](#d10) |
| purana data · stale · replica lag · read-your-own-writes | [REPLICA](#dabba-replica) |
| order / kram · partition key | [kafka](SYSTEM_DESIGNS/12_message_queue_kafka/12_message_queue_kafka.md) dikkat 3 · [chat](SYSTEM_DESIGNS/14_chat_messaging/14_chat_messaging.md) dikkat 10 |
| CAP · consistency ya availability · network toota | [TO6](#to6) |
| event / message kho gaya · offset · DLQ · outbox | [QUEUE](#dabba-queue) |
| crash beech me · aadha kaam · PENDING · reconciliation · saga | [DB TRANSACTION](#dabba-txn) |
| 429 · throttle · backoff · jitter | [notification](SYSTEM_DESIGNS/04_notification_system/04_notification_system.md) dikkat 8 |
| OTP pehle · priority | [notification](SYSTEM_DESIGNS/04_notification_system/04_notification_system.md) dikkat 7 |
| monitoring · alert · pata kaise chale | [FOUNDATIONS/18](FOUNDATIONS/18_monitoring.md) |
| security · abuse · owner check · WAF | [file upload](SYSTEM_DESIGNS/07_file_upload_validate_system/07_file_upload_INTERVIEW.md) dikkat 9 · [FOUNDATIONS/17](FOUNDATIONS/17_waf.md) |
| data badhta jaaye · retention · archive | [banking](SYSTEM_DESIGNS/13_banking_system/13_banking_system.md) "5 saal purana data" |
| SPOF · single point of failure | [FOUNDATIONS/11](FOUNDATIONS/11_reliability_spof_cloud.md) |
| SQL ya NoSQL | [TO1](#to1) |
| sync ya async | [TO2](#to2) |
| push ya pull | [TO3](#to3) |
| fanout on write / read | [TO4](#to4) |
| cache-aside / write-through / write-back | [TO5](#to5) · [jode](#drill-rows) |
| L4 ya L7 · LB algo | [TO7](#to7) · [drill rows](#drill-rows) |
| vertical ya horizontal | [TO8](#to8) |
| replica ya shard | [TO9](#to9) |
| normalize / denormalize | [TO10](#to10) |
| WebSocket / SSE / long-poll | [TO11](#to11) |
| exactly-once | [TO12](#to12) |
| batch ya stream | [TO13](#to13) |
| leader / leaderless · quorum W+R>N | [TO14](#to14) |
| LRU/LFU · session/JWT · monolith/MS · WebSocket/Kafka | [jode](#drill-rows) |
| health check · connection pool · CORS · estimate formula | [drill rows](#drill-rows) |
| design kaise shuru karun | [teen line](#teen-line) · [archetype](#archetype) · [rail](#rail) |
| ye need -> kaunsa block | [block menu](#block-menu) |
| anjaan design aa gaya | [recipe](#anjaan) · [LinkedIn misaal](#linkedin) |
| kuch nahi pata · atak gaya | [4 line](#na-pata) · [kaise bolna](#kaise-bolna) |
| estimate · QPS · storage | [rail step 2](#rail) · [drill rows](#drill-rows) |
| English word atka | [SHABD](#shabd) |
| interview ka din | [checklist](#checklist) |

### 8 DABBE · A ya B · 14 DESIGN

- **8 dabbe**: [REPLICA](#dabba-replica) · [CACHE](#dabba-cache) · [SHARD](#dabba-shard) · [QUEUE / KAFKA](#dabba-queue) · [LOAD BALANCER](#dabba-lb) · [IDEMPOTENCY KEY](#dabba-idem) · [CIRCUIT BREAKER](#dabba-cb) · [DB TRANSACTION](#dabba-txn)
- **A ya B**: [1 SQL vs NoSQL](#to1) · [2 sync vs async](#to2) · [3 push vs pull](#to3) · [4 fanout write/read](#to4) · [5 write-back](#to5) · [6 strong vs eventual (CAP)](#to6) · [7 L4 vs L7](#to7) · [8 vertical vs horizontal](#to8) · [9 replica vs shard](#to9) · [10 normalize vs denormalize](#to10) · [11 long-poll / WS / SSE](#to11) · [12 at-least vs exactly-once](#to12) · [13 batch vs stream](#to13) · [14 leader / leaderless / quorum](#to14)
- **14 design**: [01 url shortener](#d01) · [02 rate limiter](#d02) · [03 twitter feed](#d03) · [04 notification](#d04) · [05 stock broker](#d05) · [06 payment](#d06) · [07 file upload](#d07) · [08 news aggregator](#d08) · [09 bookmyshow](#d09) · [10 distributed cache](#d10) · [11 google docs](#d11) · [12 kafka / MQ](#d12) · [13 banking](#d13) · [14 chat](#d14)
- **SHABD**: [verbs](#shabd-0) · [rate limiter](#shabd-1) · [caching](#shabd-2) · [LB](#shabd-3) · [replica/shard](#shabd-4) · [queue](#shabd-5) · [CAP](#shabd-6) · [reliability](#shabd-7) · [drill](#shabd-drill)

### DOOSRI FILE (gehrai chahiye to)

- **FOUNDATIONS**: [01 hld kya hai](FOUNDATIONS/01_hld_kya_hai.md) · [02 capacity estimation](FOUNDATIONS/02_capacity_estimation.md) · [03 load balancing](FOUNDATIONS/03_load_balancing.md) · [04 caching](FOUNDATIONS/04_caching.md) · [05 database replication](FOUNDATIONS/05_database_replication.md) · [06 database sharding](FOUNDATIONS/06_database_sharding.md) · [07 message queues](FOUNDATIONS/07_message_queues.md) · [08 cap theorem](FOUNDATIONS/08_cap_theorem.md) · [09 databases what when](FOUNDATIONS/09_databases_what_when.md) · [10 ms communication](FOUNDATIONS/10_ms_communication.md) · [11 reliability spof cloud](FOUNDATIONS/11_reliability_spof_cloud.md) · [12 elasticsearch search](FOUNDATIONS/12_elasticsearch_search.md) · [13 distributed id snowflake](FOUNDATIONS/13_distributed_id_snowflake.md) · [14 jab ilaaj hi bimari bane](FOUNDATIONS/14_jab_ilaaj_hi_bimari_bane.md) · [15 cdn](FOUNDATIONS/15_cdn.md) · [16 dns](FOUNDATIONS/16_dns.md) · [17 waf](FOUNDATIONS/17_waf.md) · [18 monitoring](FOUNDATIONS/18_monitoring.md) · [19 url browser journey (URL -> DNS -> TCP -> TLS -> HTTP -> render)](FOUNDATIONS/19_url_browser_journey.md)
- **14 DESIGN (poori)**: [01 url shortener](SYSTEM_DESIGNS/01_url_shortener/01_url_shortener.md) · [02 rate limiter](SYSTEM_DESIGNS/02_rate_limiter/02_rate_limiter.md) · [03 twitter feed](SYSTEM_DESIGNS/03_twitter_feed/03_twitter_feed.md) · [04 notification](SYSTEM_DESIGNS/04_notification_system/04_notification_system.md) · [05 stock broker](SYSTEM_DESIGNS/05_stock_broker_trading/05_stock_broker_trading.md) · [06 payment](SYSTEM_DESIGNS/06_payment_system/06_payment_system.md) · [07 file upload](SYSTEM_DESIGNS/07_file_upload_validate_system/07_file_upload_INTERVIEW.md) · [08 news aggregator](SYSTEM_DESIGNS/08_news_aggregator/08_news_aggregator_INTERVIEW.md) · [09 bookmyshow](SYSTEM_DESIGNS/09_bookmyshow/09_bookmyshow_INTERVIEW.md) · [10 distributed cache](SYSTEM_DESIGNS/10_distributed_cache/10_distributed_cache.md) · [11 google docs](SYSTEM_DESIGNS/11_google_docs_collab/11_google_docs_collab.md) · [12 kafka / MQ](SYSTEM_DESIGNS/12_message_queue_kafka/12_message_queue_kafka.md) · [13 banking](SYSTEM_DESIGNS/13_banking_system/13_banking_system.md) · [14 chat](SYSTEM_DESIGNS/14_chat_messaging/14_chat_messaging.md)
- **HANDS_ON**: [01 rate limiter Redis](HANDS_ON/01_rate_limiter_redis) · [02 INCR+EXPIRE crash](HANDS_ON/02_incr_expire_crash) · [03 provider 429](HANDS_ON/03_provider_429) · [04 event loss](HANDS_ON/04_event_loss) (code yahan; asli output + nichod = design file ka HANDS-ON section)
- **LOG**: [HLD_PRACTICE_LOG](HLD_PRACTICE_LOG.md) · **farak batao**: [06_COMPARES](../06_COMPARES)

### ALEX XU (Vol 1) ka chapter -> is repo me kahan

| chapter | yahan |
|---|---|
| 1 Scale from zero to millions | [block menu](#block-menu) · [replica](#dabba-replica) · [cache](#dabba-cache) · [LB](#dabba-lb) |
| 2 Back-of-the-envelope | [rail step 2](#rail) · [FOUNDATIONS/02](FOUNDATIONS/02_capacity_estimation.md) |
| 3 Interview framework | [rail](#rail) · [checklist](#checklist) |
| 4 Rate limiter | [02](#d02) |
| 5 Consistent hashing | [10 distributed cache](#d10) · [SHARD](#dabba-shard) |
| 6 Key-value store | [10](#d10) · [TO14 quorum](#to14) · [TO6 CAP](#to6) |
| 7 Unique ID | [FOUNDATIONS/13 snowflake](FOUNDATIONS/13_distributed_id_snowflake.md) |
| 8 URL shortener | [01](#d01) |
| 9 Web crawler | [08 news aggregator](#d08) (crawl hissa; poora crawler design nahi padha) |
| 10 Notification | [04](#d04) |
| 11 News feed | [03 twitter feed](#d03) |
| 12 Chat | [14](#d14) |
| 13 Search autocomplete | [archetype E (trie)](#archetype) (alag design file nahi) |
| 14 YouTube | [07 upload](#d07) + CDN (video transcoding padha nahi) |
| 15 Google Drive | [07 upload](#d07) + [11 google docs](#d11) (sync hissa padha nahi) |

---


> EK file. Interview se pehle sirf YE. (detail chahiye to hi SYSTEM_DESIGNS/* kholo.)
>
> IDEA: har design naya nahi hota. Har design **6 archetype** me se kisi ek (ya do ke mel) me girta hai.
> Archetype pehchano -> uska DIL pata -> default blocks lagao -> RAIL pe bol do.
>
> ★ HONEST HAD: ye sheet kisi bhi design ka HIGH-LEVEL khada kar degi (boxes + kyun + trade-off).
>   Jo cheez tune PADHI hi nahi uska DEEP-DIVE ye sheet nahi degi -- wahan section 7 wali
>   honest line kaam aati hai. Ye kami nahi, ye tareeka hai.

---
---

[upar INDEX](#index)

<a id="part1"></a>

# PART 1 — NAKSHA + NIYAM

<a id="naksha"></a>

## 0. KAUNSI FILE KAB — poore 04_HLD ka naksha (26-Sep saaf kiya: har kaam ki EK file)

```
   INTERVIEW-DIN / REVISE
     00_MASTER_SHEET.md    <- YE. KYA bolna: 6 sawaal · CROSS-QUESTION BANK · archetype · rail ·
                              blocks · A ya B jode · 14 design ka DIL · KAISE bolna · SHABD
                              (29-Sep: 01_DELIVERY + 02_TRADEOFFS isi me mile)

   PADHNE / DEPTH
     FOUNDATIONS/01..18    <- ek concept = ek file
        01 hld · 02 capacity · 03 LB · 04 cache · 05 replication · 06 sharding · 07 queue
        08 CAP · 09 db kaunsa · 10 microservice baat-cheet · 11 SPOF · 12 search · 13 id
        14 HUB (bachane wala hi girane wala) · 15 CDN · 16 DNS · 17 WAF · 18 monitoring
     SYSTEM_DESIGNS/01..14 <- 14 poore design (browser journey = FOUNDATIONS/19, concept)
     HANDS_ON/01..04       <- chala ke dekha: rate limiter Redis · INCR+EXPIRE crash · provider 429 ·
                              event kahan khota (Java demo, asli output)

   DRILL / ABHYAS
     HLD_PRACTICE_LOG.md                  <- bolke kiye design ka log (naya yahin)

   BAHAR (zaroorat pe)
     ../06_COMPARES/       <- gehre "farak batao" explainer (TCP/UDP, HTTP versions, JWT/OAuth...)
     ../05_INFRA_DEEP/     <- sirf DEVOPS commands + hands-on demo folder (LB / monitoring / load-test)
```
```
   ★ CONFUSION-PROOF NIYAM:
       "kya bolna"         -> ye file
       "what if X fails?"  -> ye file, CROSS-QUESTION BANK
       "kaise bolna"       -> ye file, KAISE BOLNA + SHABD (aakhir me)
       "A ya B?"           -> ye file, A ya B — TRADE-OFF JODE (section 4 ke baad)
       "ye cheez hai kya?" -> FOUNDATIONS
       "poora design"      -> SYSTEM_DESIGNS
       "khud ko test"      -> HLD_PRACTICE_LOG (bolke kiye round)
     Har file ke UPAR ek NAV line hai -> wahan se seedha jump.
```

---

<a id="teen-baatein"></a>

## ★★ TEEN BAATEIN JO SAB PE BHAARI (Arpan ka apna nichod, 15-Sep — asli mock videos se)

```
1. PERFECT DESIGN HOTA HI NAHI -- chhote se shuru, phir scale ke baare me socho.
   sab ek saath kaise soch sakta koi? IMPOSSIBLE hai. pehle chhota, phir bada.
   -> STEP 5 = sirf kaam-chalau boxes. cache/CDN/queue/shard = STEP 6-7, dikkat ke jawab me.

2. NUMBER KE PEECHE MAT BHAAG -- wo bekaar hai. bolo aur aage badho.
   number bol ke tu kuch SPECIAL nahi kar deta.

3. BOTTLENECK RATTO MAT -- bolte waqt KHUD USER BANKE dekho:
   wo kya soch raha, kahan cheez phat rahi. Bottleneck khud dikh jaayega.
```
> Misaal (Spotify / Bitly) = neeche "KAISE BOLNA" section.

---
---

[upar INDEX](#index)

<a id="part2"></a>

# PART 2 — CROSS-QUESTION (roz grill yahin se)

<a id="chhe-sawaal"></a>

## ★★ HAR DESIGN PE 6 SAWAAL — sirf YAAD DILAANE ki list, rail nahi (28-Sep sudhaar)

> 28-Sep (rate limiter ke baad, Arpan): sawaal bhi khud poochna, jawab bhi khud dena — aur JAWAB
> padhne se aata. Ye 6 koi naya design nahi banwate; sirf PADHE hue dabbe pe dikkat yaad dilaate.
> Har design ki APNI khaas dikkatein YAAD rakhni padti hain = section 5 (DIL + KHAAS hissa).

```
                              PAYMENT                              GOOGLE DOCS
1. ye GIRA to?                beech me crash -> transaction,       server gira -> ops log/buffer,
                              PENDING pehle                        snapshot + ops
2. DOBARA aaya to?            retry -> idempotency key             same op do baar -> op-id, dobara chhodo
3. do EK SAATH aaye to?       race -> UNIQUE, WHERE balance >= x   ek jagah do log type -> OT / CRDT
4. bahar wala SLOW / band?    PSP -> status + timeout + recon      user offline -> likhne do, baad me merge
5. BAHUT zyada ho gaya?       LB, replica, shard by account_id     crore WebSocket -> conn tier, shard docId
6. kisi ko PURANA dikha?      cache / replica lag -> balance       edit der se -> WebSocket + pub/sub push
                              PRIMARY se
```

```
★ har design ka ek sawaal sabse BHAARI = uska DIL:
     payment 2 + 3 (retry, race) · google docs 3 (saath edit) · chat 1 + 6 (offline, der)
     bookmyshow 3 (ek seat do log) · rate limiter 3 + 5 · feed 5 + 6
★ jawab kahan se: common dabbe (LB/replica/shard/cache) aate hain · KHAAS wala = neeche section 5
★ round me bhool gaya -> ye 6 mann me ghumao. "ek second, sochta hoon" bolna normal.
   45 min me 2-3 dikkat GEHRAI se = kaafi. sab ek saath koi nahi bolta.
```

---

<a id="bank"></a>

## ★★★ CROSS-QUESTION — jo har design me ghoom ke aate hain

> 4-Oct se Q1-Q22 yahan nahi, har DESIGN FILE me us dikkat ke neeche ("INTERVIEWER AISE POOCHEGA").
> Roz ek design ka mock -> end me usi file ke sawaal. Yahan bache: 8 dabbe x 4 line + jawab se pehle 5 sec.

<a id="dabbe"></a>

### ★★★ 8 DABBE x 4 LINE (29-Sep, Sonnet ka saancha — cross-question yahin se aate)

> Rail pakka hai (sawaal -> requirement -> number -> chhote se shuru). Fisalna CROSS-QUESTION pe hai,
> kyunki "KAB lagana" pata hai, "LAGANE KE BAAD KYA TOOTTA" kam pada. Har dabba = 4 line:
> 1 kya solve · 2 kya NAYI dikkat laata (<- cross-question) · 3 uska fix · 4 iski jagah kya, wo kyun nahi.
> Atke to: pehle PADHO / chala ke DEKHO (jaise Redis ko maar ke dekha, HANDS_ON/), phir wahi sawaal
> 1/3/7 din baad dobara. Dry-run sirf jaani cheez ka hota hai, anjaan pe nahi.


<a id="dabba-replica"></a>

#### DABBA: REPLICA

```
 1 padhne ka load baant-ta · primary gire to standby
 2 LAG: copy thodi der baad pahunchti (async) -> apna likha turant purana dikhe ·
   failover pe aakhri writes kho sakti (double booking / paisa)
 3 apna likha PRIMARY se padho (read-your-own-writes) · paise/seat pe SYNC / semi-sync standby, alag AZ
 4 cache? (taaza data chahiye tha) · shard? (masla likhna nahi, padhna tha)

```

<a id="dabba-cache"></a>

#### DABBA: CACHE (Redis)

```
 1 baar-baar ka read tez, DB ka bojh kam
 2 STALE (purana dikhe) · cache gira -> saara load DB pe · hot key expire -> STAMPEDE (1000 miss ek saath)
 3 write pe DELETE + TTL (race kam, khatam nahi) · replica/cluster · mutex / soft-TTL · load shedding
 4 read replica? (har read ab bhi DB tak jaata) · paisa / balance kabhi cache se nahi

```

<a id="dabba-shard"></a>

#### DABBA: SHARD

```
 1 data ya WRITES ek machine se bade -> baant do
 2 HOT SHARD (celebrity / ek key pe bheed) · CROSS-SHARD query / transaction mushkil · RESHARD dard
 3 shard key soch ke (userId / chatId: jo query me hamesha ho) · hot key tod do (key+0..9) ·
   consistent hashing (kam data khiske) · jo saath chahiye wo ek hi shard pe
 4 pehle: vertical scaling · read replica (sirf read zyada ho) · purana data archive (size) —
   sharding AAKHRI hathiyar

```

<a id="dabba-queue"></a>

#### DABBA: QUEUE / KAFKA

```
 1 bhejne wala aur karne wala alag (async) · jhatka sokh le (spike) · ek event -> kai consumer
 2 AT-LEAST-ONCE: duplicate aayega · ORDER sirf ek partition ke andar · poison message line rok de ·
   consumer peeche (lag)
 3 idempotent consumer (message-id dedup) · same key -> same partition · retry + DLQ ·
   lag pe alert, consumer badhao (max = partition ki ginti) · Kafka me priority nahi -> alag topic
 4 seedha REST call? (turant jawab chahiye tab) · RabbitMQ? (routing / per-message ack; replay nahi)

```

<a id="dabba-lb"></a>

#### DABBA: LOAD BALANCER

```
 1 traffic kai server me baanto · mara server bahar (health check)
 2 LB khud SPOF · STICKY session = ek server pe bojh, wo mare to session gaya · rokta nahi, sirf baant-ta
 3 LB ki jodi (active-passive / managed) · server STATELESS rakho (session Redis / JWT) ·
   bheed rokni ho to queue / rate limit
 4 DNS round-robin? (health check dheema, cache ki wajah se) · L4 vs L7: L7 URL / header dekh sakta

```

<a id="dabba-idem"></a>

#### DABBA: IDEMPOTENCY KEY

```
 1 retry / double-click pe kaam do baar na ho (paisa, email)
 2 key laga di aur kaam FAIL -> retry pe skip -> kaam kabhi nahi hua ·
   check aur set alag-alag = race (do request dono andar)
 3 SET NX (check + set ek atomic step) · "sending" chhota TTL -> success pe "sent" ·
   DB me UNIQUE constraint · duplicate pe WAHI purana jawab lautao
 4 lock? (bhaari, deadlock) · "exactly-once" ka waada? (distributed me milta nahi — at-least-once + dedup)

```

<a id="dabba-cb"></a>

#### DABBA: CIRCUIT BREAKER

```
 1 mara / dheema provider ko call hi band -> worker timeout me na phase
 2 galat threshold -> theek provider bhi band ho jaaye · OPEN ke waqt kaam kahan jaaye?
 3 N fail -> OPEN (call band) -> thodi der -> HALF-OPEN (ek test) -> theek to CLOSED ·
   FALLBACK: doosra provider / queue me rakho / cached jawab
   (naam yaad: OPEN = taar toota = current nahi = call nahi)
 4 sirf timeout? (har call phir bhi intezaar karti) · sirf retry? (marte provider pe aur hathoda)

```

<a id="dabba-txn"></a>

#### DABBA: DB TRANSACTION

```
 1 kai kaam ek saath — ya sab, ya kuch nahi (atomic) · seat / balance sahi
 2 do alag service / DB pe transaction nahi chalti · lambi transaction = lock = dheema / deadlock ·
   transaction ke andar bahar ka call (PSP) = rollback se wapas nahi aata
 3 ek DB me: ek transaction · alag service: PENDING -> kaam -> DONE + RECONCILIATION job /
   SAGA (fail pe ulta kaam: refund) · event ke liye OUTBOX · race pe UPDATE ... WHERE seats >= 1
 4 2PC (two-phase commit)? (dheema, coordinator SPOF — isliye saga)
```

<a id="jawab-se-pehle"></a>

#### JAWAB SE PEHLE 5 SECOND

```
★★ JAWAB SE PEHLE 5 SECOND (29-Sep, grill 2.5/5 ke baad nikla)
   Aadat thi: sawaal ka SHABD suna -> pehla jaana-pehchaana dabba bola -> ruk gaya.
   ("data badhega" -> shard · "chal raha hai?" -> logs · "crash" -> logs)  — DSA me "sum" -> prefix jaisa.

   1. PEHLE PUCHHO: "asli me kya toot raha hai?"
        SIZE (data bada) · LOAD (bheed) · GIRA (dabba mara) · SLOW · DOBARA (duplicate) ·
        EK SAATH (race) · AADHA KAAM (crash beech me) · PATA KAISE CHALE (monitoring)
   2. PHIR 3 HISSE:   dikkat  ->  ilaaj  ->  BARIKHI ("par dhyan: ...")   <- cross-question yahin aata
   3. HO SAKE TO:     "At work I ..."  (HikariCP alert · WAF IP block · $0 duplicate write · Fargate IP)
```


> Q1-Q22 ka poora jawab (4-Oct se) har DESIGN FILE me, us dikkat ke neeche jahan wo baat hai:
> "INTERVIEWER AISE POOCHEGA" + "YAHI SAWAAL DOOSRE DESIGN ME BHI" (connect) + "MASTER SHEET SE JODA".
> Purana bank git history me (commit 9f0fe5d tak).

---

[upar INDEX](#index)

<a id="part3"></a>

# PART 3 — DESIGN KHADA KARNA

<a id="teen-line"></a>

## 1. TEEN LINE KA METHOD (poora khel isi me hai)

```
   1. Ye design kis ARCHETYPE ka hai?        -> section 2
   2. Us archetype ka DIL kya hai?           -> wahi Step-1 me bolna, wahi deep-dive banega
   3. Dil ke hisaab se BLOCKS uthao          -> section 4 ka menu
   -> RAIL pe rakh ke bol do                 -> section 3
```

---

<a id="archetype"></a>

## 2. ★★ 6 ARCHETYPE — koi bhi design in me se hai (ye sabse zaroori table)

```
┌───────────────────────────────────────────────────────────────────────────────────────┐
│ A. READ-HEAVY / FEED          DIL = read SASTA ho                                     │
│    misaal: twitter-feed, news-aggregator, youtube-home, LinkedIn-feed, instagram      │
│    default blocks: CACHE (aggressive) . read-REPLICA . DENORMALIZE . CDN              │
│                    precompute/FANOUT-on-write . pagination (cursor)                   │
│    signature Q   : celebrity/hot-key? -> hybrid fanout (normal=write, celeb=read)     │
│    consistency   : EVENTUAL (2 sec purana feed chalega)                                │
├───────────────────────────────────────────────────────────────────────────────────────┤
│ B. WRITE-HEAVY / INGEST       DIL = write GIRE nahi + peeche kaam chalta rahe          │
│    misaal: logging, analytics, clickstream, IoT, message-queue, notification-ingest    │
│    default blocks: QUEUE (Kafka) . partition/shard by key . batch-write . async worker │
│                    append-only log . retention                                         │
│    signature Q   : ordering? -> per-key partition . duplicate? -> idempotent consumer  │
│    consistency   : EVENTUAL + at-least-once                                            │
├───────────────────────────────────────────────────────────────────────────────────────┤
│ C. TRANSACTIONAL / MONEY      DIL = galat data KABHI nahi (ek hi baar, ek hi seat)      │
│    misaal: payment, booking (bookmyshow), trading, wallet, inventory, ticket           │
│    default blocks: SQL + ACID . LOCK ya unique-constraint . IDEMPOTENCY-key            │
│                    LEDGER (append-only, audit) . SAGA (multi-service) . CP side of CAP │
│    signature Q   : do user ek saath? -> atomic check+mark . retry pe double? -> idem-key│
│    consistency   : STRONG (yahan koi compromise nahi)                                  │
├───────────────────────────────────────────────────────────────────────────────────────┤
│ D. REAL-TIME / PUSH           DIL = turant pahunche + connection zinda rahe             │
│    misaal: chat/WhatsApp, google-docs-collab, live-price, live-score, presence          │
│    default blocks: WEBSOCKET . connection-registry (kaun kis server pe)                 │
│                    PUB/SUB (Redis/Kafka) server-to-server . offline -> push (FCM/APNS)  │
│                    message-store (Cassandra) . ordering per-conversation                │
│    signature Q   : user offline? -> store + push . 2 log ek saath likhein? -> OT/CRDT   │
│    consistency   : per-conversation order + eventual convergence                        │
├───────────────────────────────────────────────────────────────────────────────────────┤
│ E. SEARCH / LOOKUP            DIL = sahi cheez turant dhoondh ke do                     │
│    misaal: typeahead, search, news-search, product-search, nearby/geo                   │
│    default blocks: INVERTED INDEX (Elasticsearch) . TRIE (prefix) . top-K cache         │
│                    async indexer (DB -> index) . ranking . geohash (nearby ke liye)     │
│    signature Q   : index kab update? -> async, thoda stale chalega                      │
│    consistency   : EVENTUAL (naya item 1 min baad dikhe to chalega)                     │
├───────────────────────────────────────────────────────────────────────────────────────┤
│ F. INFRA / COMPONENT          DIL = ek machine se bada + machine mare to chale           │
│    misaal: distributed-cache, rate-limiter, URL-shortener(ID-gen), unique-ID, LB, MQ    │
│    default blocks: CONSISTENT HASHING . SHARD + REPLICA . leader/follower                │
│                    in-memory DS . TTL/eviction . coordinator (ZK/KRaft/etcd)             │
│    signature Q   : node add/remove? -> consistent hashing . node mare? -> replica/failover│
│    consistency   : mostly eventual, metadata strong                                      │
└───────────────────────────────────────────────────────────────────────────────────────┘

★ BADE PRODUCT = 2-3 ARCHETYPE KA MEL. Yahi "design LinkedIn/Instagram/Uber" ka raaz hai:
     LinkedIn  = A (feed) + E (search/people) + D (messaging) + B (activity-ingest)
     Instagram = A (feed) + F (media/blob+CDN) + E (explore/search)
     Uber      = D (live location) + E (geo-match) + C (payment)
     WhatsApp  = D (real-time) + B (ingest) + F (storage)
     Zomato    = E (search) + C (order/payment) + D (live tracking)
  -> interview me BOL do: "this really breaks into three sub-systems" -> phir ek-ek karke chalo.
     Ye move akela hi tujhe structured dikha deta hai.
```

---

<a id="rail"></a>

## 3. UNIVERSAL RAIL — har design pe yahi bolna (design ka naam bas badalta hai)

```
STEP 1  REQUIREMENTS (2 min)
   ★★ PEHLA MOVE = SCOPE KAATO (15-Sep, asli mock video se -- ex-Google EM ka pehla kaam):
      "This is a big system -- I'll scope it to X and Y and go deep there. Sound good?"
        Spotify wale round me usne turant bola: "sirf finding aur playing."
        -> baaki 40 minute sirf 2 cheez pe gaye, design bikhra nahi.
      Bada product (LinkedIn/Uber/Zomato) mile -> 2-3 sub-system bolo, phir EK chuno.

   . 2-3 clarifying Q poochho: kitne user? read-heavy ya write-heavy? real-time chahiye?
     kya scope me NAHI hai?
   . FR = 4-5 line, user ki bhasha me ("user post kare", "user feed dekhe")
   . NFR = scale . latency . availability . consistency (STRONG ya EVENTUAL -- ye bolna ZAROORI)
   . ★ AAKHIR ME DIL BOLO: "the heart of this problem is ___"   <- yahi poore interview ka anchor

STEP 1b CORE ENTITIES (20 second -- ek line, bas)
   3-5 NAAM bol do, poora schema nahi:
        Kafka  -> "Topic, Partition, Message, Consumer Group, Offset"
        Spotify-> "User, Song, Artist, Playlist"
        Bitly  -> "User, Link, Click"
   KYUN: iske baad API aur data-model apne aap nikal aate hain (dono me yahi naam aayenge).
   (Hello-Interview framework isko alag step kehta hai; asli round me ye API ke saath hi
    ek line me bol diya jaata -- bada step mat banao.)

STEP 2  ESTIMATE (2 min) -- number se DECISION nikaalo, warna number bekaar
   QPS      = daily requests / 100,000      (approx: 86,400 ~ 10^5)
   peak     = 2-3x
   storage  = per-record size x records x days
   ★ phir turant bolo: "iska matlab ek machine kaafi nahi -> shard/replica chahiye"

STEP 3  API (1-2 min) -- 3-5 endpoint, bas
   POST /resource  . GET /resource/{id}  . GET /feed?cursor=...
   . auth ek line . pagination = CURSOR (offset nahi, deep-page slow)

STEP 4  DATA MODEL (2-3 min)
   . main tables/collections + KEY fields
   . SQL ya NoSQL -> KYUN (rishte+transaction = SQL . flat+huge+fixed-access = NoSQL)
   . shard key kya hoga
   . bade file/media -> S3/blob, DB me sirf URL

STEP 5  HL BOXES (5 min) -- yahan sabse zyada number milte
   client -> CDN(static) -> LB -> API gateway -> service(s) -> cache -> DB
                                                      |
                                                      +-> QUEUE -> workers -> (email/index/analytics)
   ★ har box ke saath ek "kyun" -- box bina kyun ke = ratta

STEP 6  DEEP-DIVE (8-10 min) -- 2-3 cheez, POORA nahi
   ★ kaunsi chuno? -> jo DIL hai wahi (Step 1 me jo bola). Archetype ka "signature Q" = yahi.

STEP 7  BOTTLENECK / SCALE-10x (3 min)
   . kya PEHLE tootega? -> DB write . single hot key . ek queue . ek service
   . fix: shard . cache . replica . partition badhao . async karo . rate-limit
   . monitoring: p99 latency, error-rate, queue-lag, cache hit-rate
```

---

<a id="block-menu"></a>

## 4. BLOCK MENU (need -> block -> ek line kyun)

> Ye compact version interview-din ke liye.

```
   read baar-baar          -> CACHE (Redis, cache-aside + TTL)   RAM, disk se 100x tez
   read phir bhi DB pe     -> READ REPLICA                       reads baant do
   write/storage nahi sama -> SHARDING (+ shard key)             data tukdon me
   spike / burst           -> QUEUE                              requests HOLD karo (LB nahi, REPLICA bhi
                                                                 nahi — replica read-scale hai, write-spike nahi)
   slow kaam, decouple     -> QUEUE + WORKER                     user block na ho
   traffic baantna         -> LOAD BALANCER (L7)                 content-based routing + TLS
   kai service, ek darwaza -> API GATEWAY                        auth+routing+rate-limit ek jagah
   bade file / stream / DL -> S3 + PRESIGNED URL                 bytes client<->S3 seedha (server sirf link de,
                                                                 warna bandwidth marta) · DB me sirf link
   machine mare            -> REPLICATION (leader/follower)      copy se kaam chale
   paisa / seat            -> SQL + LOCK/unique + CP             galat data NEVER
   consistency vs availab. -> CAP choice                         paisa/booking = CP, social feed = AP
   retry pe double-effect  -> IDEMPOTENCY KEY                    ek hi baar asar
   2 user ek resource      -> ATOMIC check+mark (ya lock)        race khatam
   abuse / flood           -> RATE LIMITER (token bucket, 429)   Redis counter
   static global slow      -> CDN                                edge user ke paas
   server->user turant     -> WEBSOCKET (ya SSE agar ek-tarfa)   zinda connection
   slow query              -> INDEX (B-tree)                     O(log n)
   join slow, read-heavy   -> DENORMALIZE                        jodke rakh do
   node add/remove         -> CONSISTENT HASHING                 sirf thodi key ghoome
   cache full              -> EVICTION (LRU/LFU/TTL)             jagah banao
   badi list               -> CURSOR PAGINATION                  "last id ke baad"
   text/prefix dhoondhna   -> ELASTICSEARCH / TRIE               inverted index / prefix tree
   multi-service txn       -> SAGA + compensating                distributed rollback
   dead service hammer     -> CIRCUIT BREAKER                    fail-fast, cascade roko
   unique id at scale      -> SNOWFLAKE / range-allocation       DB sequence bottleneck
   message kitni baar      -> at-least-once + IDEMPOTENT         exactly-once ka effect
```

NIYAM: pehle NEED dekho, phir block. Har block ka apna kaam — galat jagah mat lagao.

TRADE-OFF bolna ho -> neeche "A ya B — TRADE-OFF JODE" section (jode + ready English lines).

<a id="drill-rows"></a>

### ★ (drill se) — menu me jo rows nahi thi

```
   abhi likha, turant padha,  -> READ-YOUR-OWN-WRITES               likhne wale ko thodi der
   purana mila (replica lag)                                        PRIMARY se padhao
   update pe cache stale      -> cache key DELETE (update nahi)     do write ulte kram me = galat value;
                                                                    delete -> agla read DB se bharega
   request kis server pe      -> LB ALGO: round-robin (barabar) · weighted (badi machine) ·
                                 least-conn (request lambi-chhoti) · IP-hash (sticky session)
   bada app, alag scale       -> MICROSERVICES                      alag scale + deploy, ek gira baaki chalu
                                                                    (chhota app = monolith, MS over-engineer)
   login state, kai server    -> JWT (stateless) / session (Redis)  JWT = signature verify, koi store nahi
   server mara, LB ko pata?   -> HEALTH CHECK (LB pull, /health)    2-3 lagataar fail = hatao, 2-3 pass = wapas
                                 HEARTBEAT (server push "zinda")    signal band = mara
   har request naya DB conn   -> CONNECTION POOL (HikariCP)         borrow -> use -> return; size fix -> exhaust
   "1M user -> kitna server?" -> ESTIMATE                           QPS = req/day / 10^5 · peak x2-3 ·
                                                                    storage = req/day x size x (saal x 400)
   frontend alag origin       -> CORS header                        server Access-Control-Allow-Origin
                                                                    (specific origin, prod me "*" nahi)
```

```
   ★ jode, ek line (A ya B):
     LRU vs LFU          -> LRU = kab aakhri baar use (TIME) · LFU = kitni baar use (COUNT)
                            A: 100 baar use, aakhri 1 ghanta pehle · B: 2 baar, aakhri 1 sec pehle
                            -> LRU A hataata, LFU B hataata
     cache-aside vs W-through -> aside = DB update + cache DELETE · through = cache + DB saath likho
     session vs JWT      -> session = har request Redis lookup (stateful) · JWT = token khud saboot
     monolith vs MS      -> chhota / ek team = monolith · alag scale + isolation = MS
     WebSocket vs Kafka  -> WebSocket = server se USER ke browser tak live · Kafka = backend services ke beech
```

```
   ★ design ke waqt 3 reflex (rate-limiter drill se, har shared component pe lagte):
     A. component HAR request pe baitha (middleware)  -> LATENCY sabse zaroori -> in-memory (Redis)
     B. shared state, kai server                      -> EK central store; per-server count = limit toot-ti
     C. wo component mara                             -> default FAIL-OPEN (allow);
                                                         FAIL-CLOSED sirf payment / auth jaise
   ★ TRAP: "load baantna" = SHARDING, CDN nahi. CDN sirf static file (img/video/css), counter nahi.
```

---

<a id="trade-off"></a>

## ★ A ya B — TRADE-OFF JODE (02_TRADEOFFS yahan mila, 29-Sep)

> Trade-off koi alag hoshiyari nahi: ek slot pe DO option pata ho + ek wajah. Sirf ek option pata =
> trade-off aa hi nahi sakta (knowledge ka sawaal, dimaag ka nahi). List FINITE hai — har design inhi
> me se 3-4 use karta. Teesra hissa (KEEMAT) sabse zaroori — wahi dikhata ki samajh ke bol raha hai.
> (section 4 wale jode yahan dobara nahi: LRU/LFU · cache-aside/write-through · session/JWT ·
>  monolith/MS · WebSocket/Kafka. Queue vs LB, 301/302, rate-limit algo, modulo vs consistent hashing,
>  offset vs cursor, lock vs idempotency bhi upar section 4/5 me hain.)

```
★ SAANCHA (ratna sirf ye):
   "There are two options here -- A and B.
    I'd go with A, because <requirement jo Step-1 me bola tha>.
    The trade-off is that we give up <B ka faayda>."
   Hinglish soch: "Do raste hain -- A aur B. Main A lunga kyunki requirement ___ hai. Keemat ye ki ___ chhodna padega."
   misaal: "SQL vs Cassandra -- SQL simple par 180B rows pe scale nahi; Cassandra lunga kyunki
            key-value + horizontal scale + HA."
```


<a id="to1"></a>

#### TO1 — SQL vs NoSQL

```
 1 SQL vs NoSQL       SQL = RISHTE + ACID + flexible query (join/report) · NoSQL = bahut data, flat,
                      access FIX (key se uthana: timeline/logs/chat). wajah: POOCHNA hai ya sirf UTHANA?
                      keemat: SQL -> sharding ka dard · NoSQL -> join/txn khud sambhalo
   BOL: "The access pattern is a simple key lookup at very high volume, so NoSQL. If we needed
         multi-table transactions, I'd flip to a relational DB."            (01 · 03 · 07/10 = SQL)

```

<a id="to2"></a>

#### TO2 — SYNC vs ASYNC

```
 2 SYNC vs ASYNC      sync = user ko jawab ABHI chahiye (login, payment authorize, seat check) ·
                      async = intezaar nahi (email, analytics, report). wajah: user screen pe rukega?
                      keemat: async -> eventually consistent, duplicate sambhalo, debug mushkil
   BOL: "The user doesn't need to wait for the email, so I'd push it to a queue and return
         immediately. The cost is that the system becomes eventually consistent."  (13 · 04 · 07 · 08)

```

<a id="to3"></a>

#### TO3 — PUSH vs PULL

```
 3 PUSH vs PULL       push = server bheje (WebSocket/SSE), real-time · pull = client apni raftaar se
                      maange (poll), slow client dabta nahi. keemat: push -> slow consumer overload +
                      connection sambhalo · pull -> agle poll tak thodi der
   BOL: "Kafka is pull-based -- consumers poll at their own pace, so a slow consumer never gets
         overwhelmed. The trade-off is a little extra latency."             (13 · 04 · 03)

```

<a id="to4"></a>

#### TO4 — FANOUT WRITE/READ

```
 4 FANOUT WRITE/READ  on-write = post pe sab inbox me daalo, read tez · on-read = maangne pe jodo.
                      keemat: on-write -> celeb pe write-storm · on-read -> har read mehnga. JAWAB = HYBRID
   BOL: "I'd fan out on write for regular users, but for celebrities that means millions of writes
         per post, so for them I'd merge at read time -- a hybrid."          (03 · 09)

```

<a id="to5"></a>

#### TO5 — WRITE-BACK

```
 5 WRITE-BACK         (aside/through section 4 me) write-back = pehle cache, DB baad me (async) ->
                      write sabse tez, crash pe DATA LOSS. keemat: aside -> pehla read slow (miss) ·
                      through -> har write slow · back -> sabse risky
   BOL: "Cache-aside as the default. If reads must never be stale, write-through. I'd avoid
         write-back here because a crash would lose data."                   (11 · 01 · 03)

```

<a id="to6"></a>

#### TO6 — STRONG vs EVENTUAL = CAP CP vs AP

```
 6 STRONG vs EVENTUAL = CAP CP vs AP   (Q14 bank me bhi)
                      strong/CP = paisa, seat, inventory, DB-leader -> partition me REJECT ·
                      eventual/AP = like-count, feed, cache, search-index, DNS, cart -> jawab do, baad me sudhaaro
                      wajah: "2 sec purana dikhe to kya bigdega?" keemat: strong -> slow + availability girti
   BOL: "For the seat booking itself I need strong consistency -- two users can't get the same
         seat. But the 'seats available' count on the listing can be eventually consistent."
   BOL: "During a network partition I'd rather reject the write than double-book a seat -- so
         this component is CP. The browse/search path can stay AP."          (10 · 07 · 06 · 03)

```

<a id="to7"></a>

#### TO7 — L4 vs L7 LB

```
 7 L4 vs L7 LB        L4 = sirf IP/port, bahut TEZ, andar nahi dekhta · L7 = URL/header/cookie ->
                      path routing, sticky session, TLS terminate; thoda slow par samajhdar
   BOL: "I'd use an L7 load balancer because I want to route /api and /static differently and
         terminate TLS there. L4 would be faster but can't see the request."  (har design)

```

<a id="to8"></a>

#### TO8 — VERTICAL vs HORIZ. vertical = badi machine, simple, par CEILING + SPOF · horizontal = zyada machine,

```
 8 VERTICAL vs HORIZ. vertical = badi machine, simple, par CEILING + SPOF · horizontal = zyada machine,
                      HA, par state/shard/coordination ka dard
   BOL: "Vertical scaling is simpler and I'd use it early, but it has a ceiling and it's a single
         point of failure -- past that I'd scale horizontally behind a load balancer."

```

<a id="to9"></a>

#### TO9 — REPLICATION vs SHARDING

```
 9 REPLICATION vs SHARDING   replica = ek data KAI copy -> READ scale + HA · shard = TUKDE -> WRITE
                      scale + storage. keemat: replica -> lag (stale read) · shard -> cross-shard join,
                      re-shard dard, hot shard
   BOL: "Reads are the bottleneck, so read replicas first. If the write volume itself outgrows one
         machine, then I'd shard by userId."                                 (01 · 03 · 07 · 13)

```

<a id="to10"></a>

#### TO10 — NORMALIZE vs DENORM.

```
10 NORMALIZE vs DENORM.  normalize = ek jagah, update aasan, JOIN chahiye · denormalize = copy saath
                      rakho, read tez, update kai jagah. wajah: read-heavy ya update-heavy?
   BOL: "This is read-heavy and the join was the bottleneck, so I'd denormalize and store the
         author's name with the post. The cost is updating it in two places." (03 · 09 · 10)

```

<a id="to11"></a>

#### TO11 — LONG-POLL vs WS vs SSE

```
11 LONG-POLL vs WS vs SSE   long-poll = server request rok ke rakhe, simple par mehnga · WebSocket =
                      dono taraf (chat, trading, collab) · SSE = sirf server->client, halka, HTTP pe.
                      keemat: WS -> har connection zinda rakho (memory + sticky LB)
   BOL: "Chat is bidirectional, so WebSocket. If it were only server-to-client updates, SSE
         would be lighter."                                                  (15 · 12 · 06 · 04)

```

<a id="to12"></a>

#### TO12 — AT-LEAST vs EXACTLY-ONCE

```
12 AT-LEAST vs EXACTLY-ONCE  at-least = kaam pehle, commit baad, duplicate aa sakta (default) ·
                      exactly = mehnga, har jagah possible nahi. JAWAB = at-least-once + idempotent consumer
   BOL: "I'd take at-least-once delivery and make the consumer idempotent using an event id,
         rather than paying for true exactly-once."                          (13 · 07 · 04)

```

<a id="to13"></a>

#### TO13 — BATCH vs STREAM

```
13 BATCH vs STREAM    batch = raat ko ek saath (report, billing, reconciliation), sasta, DER se ·
                      stream = event aate hi (fraud, live dashboard, alert), turant par mehnga+complex
   BOL: "Daily reconciliation can be a batch job. Fraud detection can't wait, so that one has
         to be a stream."                                                    (07 · 09)

```

<a id="to14"></a>

#### TO14 — SINGLE / MULTI-LEADER / LEADERLESS

```
14 SINGLE / MULTI-LEADER / LEADERLESS   single = ek jagah write, conflict nahi, par leader bottleneck
                      + SPOF · multi = kai region me local write tez, par CONFLICT · leaderless = kisi
                      ko bhi likho, quorum se padho (Dynamo/Cassandra), HA par version conflict.
                      keemat: conflict resolution likhna (last-write-wins / CRDT)
                      QUORUM: N copy · W copy pe likho · R copy se padho · W + R > N = padhte waqt
                      kam se kam EK copy taaza milegi (N=3, W=2, R=2)
   BOL: "I'd keep a single leader for writes so there are no conflicts. If we needed low write
         latency in multiple regions, we'd go multi-leader and pay for conflict resolution."
                                                                             (07 · 11 · 12)
```

```
★ CHHOTE JODE (ek line):
   CDN pull vs push -> pull = pehli request pe origin se le aata (simple) · push = pehle se bhar do
                       (bada file / launch)
   Bloom filter     -> "pakka NAHI hai" ka sasta check, DB hit bachata (false-positive ok,
                       false-negative kabhi nahi)
   gehra "farak batao" (SQL/NoSQL, WS/SSE, Kafka/RabbitMQ...) = ../06_COMPARES

★ KAISE USE: roz 3 jode (hafte me poori list) · design revise karte rok ke poochho "is slot pe
  DOOSRA option kya tha?" -- jodne se yaad rehta, alag ratne se nahi · mock me 2 trade-off bolo
  (section 8) · kuch na yaad aaye -> section 7 ki SAFE TRADE-OFF LINE.
```

---

<a id="designs"></a>

## 5. 14 PADHE HUE DESIGN — DIL + KHAAS HISSA (jo PADHNA padta hai)

> Common dabbe (LB · replica · cache · shard · queue · CDN · S3) har design me wahi — wo derive ho jaate.
> Neeche sirf wo jo us design ka APNA hai, jise bina padhe bol nahi sakte (Arpan ka nichod, 25-Sep).
> Detail = SYSTEM_DESIGNS/<naam>. Revise karte waqt bas ye section.


<a id="d01"></a>

#### 01 URL-SHORTENER  ·  [poori file](SYSTEM_DESIGNS/01_url_shortener/01_url_shortener.md)

```
01 URL-SHORTENER   DIL: chhota unique code + tez redirect
   . COUNTER + BASE62: counter kabhi repeat nahi -> collision nahi; /62 ke remainder ulte = code; 7 char = 3.5 trillion
   . RANGE ALLOCATION: coordinator har server ko block (1..1000) de, server local chalaye; crash = baaki range waste (manzoor)
   . 302 (301 nahi, browser cache kar leta -> click gine nahi jaate) · custom alias = UNIQUE constraint -> 409

```

<a id="d02"></a>

#### 02 RATE-LIMITER  ·  [poori file](SYSTEM_DESIGNS/02_rate_limiter/02_rate_limiter.md)

```
02 RATE-LIMITER    DIL: over-limit reject, legit allow
   . 4 ALGO: TOKEN bucket (refill, burst OK) · LEAKY bucket (fixed rate nikle) ·
             FIXED window (edge pe 2x ka bug) · SLIDING window LOG (har request ka time, sahi, par
             memory zyada) · SLIDING window COUNTER (pichhli + abhi ki window ka jod, sasta andaaza)
   . Redis INCR atomic; kai step = Lua; EXPIRE sirf PEHLI baar (count==1), warna key kabhi reset nahi
   . 429 + Retry-After · fail-OPEN vs fail-CLOSED · per-user limit bheed se nahi bachata (load shedding alag)

```

<a id="d03"></a>

#### 03 TWITTER-FEED  ·  [poori file](SYSTEM_DESIGNS/03_twitter_feed/03_twitter_feed.md)

```
03 TWITTER-FEED    DIL: read sasta ho
   . FANOUT ON WRITE: post pe follower ke inbox me sirf tweet_id (LPUSH), LTRIM 800 se capped
   . HYBRID: ~10K se kam follower = push, celeb = pull; read = inbox + celeb tweets -> merge -> sort -> top 50
   . fanout async (Kafka) -> "tweet bana" turant, "sab tak pahuncha" baad me

```

<a id="d04"></a>

#### 04 NOTIFICATION  ·  [poori file](SYSTEM_DESIGNS/04_notification_system/04_notification_system.md)

```
04 NOTIFICATION    DIL: ek event -> sahi channel / user / time
   . brain: preference (channel, quiet hours) + template -> per-CHANNEL queue + worker
   . idempotent worker (SET id NX EX) + backoff x 2^n + jitter -> retry queue -> DLQ + circuit breaker
   . PRIORITY lane (OTP alag, promo ke peeche nahi) · "accepted" != "delivered" (webhook)

```

<a id="d05"></a>

#### 05 STOCK-BROKER  ·  [poori file](SYSTEM_DESIGNS/05_stock_broker_trading/05_stock_broker_trading.md)

```
05 STOCK-BROKER    DIL: order MATCH + paisa/share sahi
   . MATCHING ENGINE: har symbol ka EK thread, lock nahi; asks sasta-pehle, bids mehnga-pehle;
     best-bid >= best-ask -> match; price-time priority; scale SYMBOL se, ek symbol ke andar kabhi nahi
   . order pe paisa BLOCK, match pe debit · settlement ACID + double-entry
   . EVENT LOG / SEQUENCER: pehle log, phir book; crash = replay (WAL wali soch) · live price = pub/sub

```

<a id="d06"></a>

#### 06 PAYMENT  ·  [poori file](SYSTEM_DESIGNS/06_payment_system/06_payment_system.md)

```
06 PAYMENT         DIL: paisa DO BAAR na kate
   . IDEMPOTENCY KEY: client har tap pe UUID, retry pe same; server STORED RESULT lautaye (reject nahi)
     claim = UNIQUE constraint / SETNX, IN_PROGRESS -> DONE, key ~24h
   . PENDING pehle likho, phir PSP call -> webhook (push) + reconciliation (pull) dono
   . LEDGER double-entry, immutable (galti = nayi correction entry) · SAGA (compensate) vs 2PC (lock, coordinator atke)

```

<a id="d07"></a>

#### 07 FILE-UPLOAD  ·  [poori file](SYSTEM_DESIGNS/07_file_upload_validate_system/07_file_upload_INTERVIEW.md)

```
07 FILE-UPLOAD     DIL: upload -> validate -> track
   . PRESIGNED URL: bytes client <-> S3 seedha, server sirf link (GET bhi, 5-15 min)
   . MULTIPART: 5 MB tukde, sirf fail tukda dobara · tmp/ + lifecycle (DINO me) + abort-incomplete
   . trackingId + status UPLOADING -> VALIDATING -> DONE/FAILED · ownerId authz · MAGIC BYTES (naam pe bharosa nahi)

```

<a id="d08"></a>

#### 08 NEWS-AGGREGATOR  ·  [poori file](SYSTEM_DESIGNS/08_news_aggregator/08_news_aggregator_INTERVIEW.md)

```
08 NEWS-AGGREGATOR DIL: kai source -> ek feed
   . WRITE path (crawl) aur READ path (feed) ALAG, ek doosre ko dheema na karein
   . worker me CLEAN + DEDUPE (ek khabar 5 source) + CATEGORY · har source alag timeout/retry/skip
   . category-wise cache merge (10 lakh fanout se bache) · RETENTION != sharding

```

<a id="d09"></a>

#### 09 BOOKMYSHOW  ·  [poori file](SYSTEM_DESIGNS/09_bookmyshow/09_bookmyshow_INTERVIEW.md)

```
09 BOOKMYSHOW      DIL: do log EK seat na lein
   . ATOMIC: UPDATE seats SET status='booked' WHERE seat_id=? AND status='available' -> 1 row jeeta, 0 = gayi
   . HOLD: status='held' + held_until (5 min); pay -> booked · SQL me TTL NAHI: booking UPDATE expired hold
     (held_until < now) ko khali maane + sweeper job saaf kare
   . 2 user ek seat = atomic mark · 1 user double click = idempotency · spike = queue + per-show worker

```

<a id="d10"></a>

#### 10 DISTRIBUTED-CACHE  ·  [poori file](SYSTEM_DESIGNS/10_distributed_cache/10_distributed_cache.md)

```
10 DISTRIBUTED-CACHE DIL: speed + node mare to chale
   . CONSISTENT HASHING: ring, key clockwise agle node pe; node add/remove pe sirf ~K/N keys hilti; VIRTUAL nodes
     (hash % N pe lagbhag sab keys shift)
   . LRU = HashMap + doubly linked list, dono O(1) · stampede: mutex / soft-TTL · hot key: replicate + L1 local

```

<a id="d11"></a>

#### 11 GOOGLE-DOCS  ·  [poori file](SYSTEM_DESIGNS/11_google_docs_collab/11_google_docs_collab.md)

```
11 GOOGLE-DOCS     DIL: saath edit, kuch na khoye, sab same
   . TEXT nahi, OPERATION bhejo ({insert "X", pos 0})
   . OT: winner mat chuno, TRANSFORM karo (baad wale ki position shift), tie-break deterministic
     CRDT: har char ki unique id, merge apne aap, central server nahi chahiye
   . snapshot + baad ke ops · shard by docId (OT ek jagah serialize) · edits AP, permissions CP

```

<a id="d12"></a>

#### 12 MESSAGE-QUEUE  ·  [poori file](SYSTEM_DESIGNS/12_message_queue_kafka/12_message_queue_kafka.md)

```
12 MESSAGE-QUEUE   DIL: kisi ko roko mat, kuch kho na jaaye
   . APPEND-ONLY LOG + OFFSET (kram-number): padh ke delete nahi, consumer apna offset rakhe -> replay
   . key -> partition = ek key ka ORDER · CONSUMER GROUP: group me baanto, alag group = sabko poora
     parallelism = partition count
   . ISR + acks (0/1/all) · at-least-once + idempotent consumer (eventId) · partition badhane pe order toot sakta

```

<a id="d13"></a>

#### 13 BANKING  ·  [poori file](SYSTEM_DESIGNS/13_banking_system/13_banking_system.md)

```
13 BANKING         DIL: paisa na bane na mare
   . ek DB = @Transactional (saga NAHI); cross-bank = saga
   . DOUBLE-ENTRY LEDGER (jod hamesha 0) · BALANCE = derived, USI txn me update · raat ko reconciliation (ledger jeete)
   . overdraft: UPDATE ... WHERE balance >= x (0 rows = mana) · deadlock: account-id ke kram me lock · outbox

```

<a id="d14"></a>

#### 14 CHAT  ·  [poori file](SYSTEM_DESIGNS/14_chat_messaging/14_chat_messaging.md)

```
14 CHAT            DIL: turant pahunche, offline pe na khoye
   . connection + register: kaun kis server pe (Redis me sirf PATA) · pub-sub server-to-server
   . PEHLE DB me likho, PHIR bhejo · catch-up "id X ke baad ka do"
   . CURSOR (read_upto / delivered_upto) = unread + ticks ek hi idea · clientMsgId (retry dedup)
   . order SERVER id se (client time nahi) · presence = TTL + heartbeat · group = fan-out on READ

★ USE: design ka naam suno -> yahan se DIL + KHAAS hissa -> baaki common dabbe dikkat pe lagao.
```

---

---

[upar INDEX](#index)

<a id="part4"></a>

# PART 4 — ATKE TO / ROUND KA DIN

<a id="anjaan"></a>

## 6. ★ ANJAAN DESIGN AA JAAYE TO — 5 MINUTE RECIPE

```
   1. ARCHETYPE pehchano (section 2). Bade product ho to 2-3 sub-system me TODO -- aur ye BOLO.
   2. DIL bolo: "the heart of this is ___"
   3. FR 4 line + NFR (read/write-heavy, consistency strong ya eventual)
   4. Estimate -> ek line: "iska matlab shard/cache/queue chahiye"
   5. Archetype ke DEFAULT BLOCKS utha ke boxes bana do
   6. Deep-dive = us archetype ka SIGNATURE Q
   7. Bottleneck = hot-key / DB-write / ek queue
```

<a id="linkedin"></a>

### WORKED EXAMPLE — "Design LinkedIn" (bilkul aise bolna)

```
   "LinkedIn is really three systems, so let me split it and then go deep on one:
      (a) the FEED -- read-heavy
      (b) SEARCH -- people and jobs
      (c) MESSAGING -- real-time
    Which one would you like me to focus on? I'll assume the feed."

   FEED = archetype A -> DIL = read sasta ho
      FR   : post banao . connections ki feed dekho . like/comment
      NFR  : read:write ~ 100:1 . feed <200ms . eventual consistency theek
      EST  : 1M DAU x 10 feed-open/day = 10M reads/day (~120/sec, peak 400/sec); posts 100k/day
      DATA : posts (postId, authorId, text, mediaUrl, ts)  [NoSQL, flat + huge]
             connections (userId, connId)                  [graph-ish; SQL ya graph-DB]
             timeline-cache (userId -> [postId...])        [Redis list]
      BOXES: client -> CDN(media) -> LB -> feed-service -> Redis timeline -> posts-DB
                                          |
                                          +-> Kafka -> fanout-worker -> followers ki timeline me daalo
      DEEP : fanout-on-write default; ek banda jiske 5 lakh follower hain ->
             uske liye fanout-on-read (hybrid). Media S3+CDN pe.
      BOTTLE: hot post / big account -> write-storm -> hybrid + cache
              10x -> shard posts by postId, timeline-cache shard by userId

   SEARCH  (agar wo poochein) = archetype E -> Elasticsearch + async indexer + top-K cache
   MESSAGING (agar wo poochein) = archetype D -> WebSocket + connection-registry + pub/sub +
              offline push + Cassandra message-store
```

---

<a id="na-pata"></a>

## 7. JAB KUCH NA PATA HO — 4 LINE (ratni hain)

```
   1. "I haven't worked with that specific piece, but here's how I'd reason about it..."
   2. Fundamentals se todo, LOUD: "it's read-heavy, so cache first; writes are the risk, so a queue..."
   3. Clarifying question poochho -> time milta hai aur interviewer khud nudge deta hai
   4. Jaani-hui cheez se jodo: "this looks like X, where I did ___"

   ★ MAT karna: bluff. Galat-confident jawab follow-up me turant pakda jaata --
     aur wahi asli nuksaan karta hai, "pata nahi" se zyada.

   ★ SAFE TRADE-OFF LINE (jab jodi yaad na aaye):
     "There's a trade-off here -- the other option would be X, but given the requirement
      I stated earlier, I'd stay with this one."
```

---

<a id="checklist"></a>

## 8. INTERVIEW-DIN CHECKLIST (10 line, bas itna)

```
   [ ] shuru me RAIL announce karo ("I'll go requirements -> estimate -> API -> data model ->
       high-level -> deep-dive -> bottlenecks")
   [ ] 2-3 clarifying question (scope bhi poochho: "kya ye scope me nahi hai?")
   [ ] DIL ek line me bolo
   [ ] numbers bolo (QPS + storage) aur turant unse ek DECISION nikaalo
   [ ] har box ke saath "kyun"
   [ ] kam-se-kam 2 TRADE-OFF (dono taraf -- "A chuna, B ki keemat ye")
   [ ] deep-dive 2-3 cheez, poora nahi
   [ ] "scale 10x" ka jawab ready (shard/cache/replica/async)
   [ ] chup mat raho -- soch LOUD bolo
   [ ] na aaye to honest + reason (section 7)
```

---

---

[upar INDEX](#index)

<a id="part5"></a>

# PART 5 — BOLNA

<a id="kaise-bolna"></a>

## ★ KAISE BOLNA (01_DELIVERY PART 1 yahan mila, 29-Sep)

> Rail (section 3), scope kaato, core entities, har box naam + ek "kyun", teen niyam (upar),
> na-pata-ho 4 line (section 7), checklist (section 8) pehle se is file me hain. Neeche sirf jo baaki tha.
> Common follow-ups ("scale 10x", "gira to", CAP, race, hot key, data lost, monitoring) = CROSS-QUESTION BANK.

```
HLD ALAG KYUN LAGTA: DSA = binary (pattern hai/nahi) · Java = fixed sawaal-jawab · HLD = OPEN game,
   answer-key nahi, interviewer SAATH chalata. Grade = soch + communication + trade-off + ambiguity me
   aage badhna. "Perfect complete answer" grade hi nahi hota. Khula != blank: rail hamesha hai,
   aur rail TU lead karta hai, interviewer wait nahi karta.

ESTIMATE: 30 second, scale justify karo ("read-heavy, billions -> cache + shard"), aage badho.
   1500 vs 2000 RPS design nahi badalta. PAR poora SKIP bhi nahi -- ek banda Zomato me reject hua,
   BOTE chhod ke seedha distributed pe kood gaya tha.

TEEN NIYAM ki misaal (15-Sep, asli mock videos):
   chhote se shuru: Spotify / Bitly dono me pehle 4 box (app / LB / server / DB), phir jahan toota
      wahan CDN / cache / S3 / shard joda.
   USER banke chalo: app kholi -> search -> har search DB pe?       -> cache
                     play dabaya -> 5 MB stream -> gaana viral?      -> CDN
   number: 1000 ya 1200, farak nahi. mota andaaza, ek decision, aage.
```

```
INTERVIEWER KYA DEKHTA HAI (16-Sep, ex-Google EM "10 signals"):
   ★ "Domain knowledge is NOT a quiz -- it's the art of APPLYING components."
     component ka lecture nahi; SAHI JAGAH lagao + wajah bolo.
   1 BOILERPLATE me mat ulajho: LB / gateway / CDN ek line me, waqt DIL pe (matching, fanout,
     dedupe, seat-lock). box ginne se score nahi milta.
   2 PARKING LOT: "There's an optimization here -- caching. Let me park it and come back once
     the core flow is right."  (dikh bhi gaya, design saaf bhi raha)
   3 LISTENING: unki baat apne shabdon me dohrao ("so you want me to focus only on playback --
     right?") + beech me chhota pause.
   4 CONCISE: board pe chhote label, poora vaakya muh se.
   5 SHARING: soch bolo ("I'm thinking this because..."), sirf nateeja nahi.
   6 FLEXIBLE: requirement badli / feedback aaya -> approach badlo, zidd nahi.
   7 TEST your design: apna flow chala ke edge case pakdo (= user banke chalo).
   8 CHOICES: har faisle ka "kyun", ittefaq se kuch nahi.
   9 SCALING: BOTE dikhao, usi me atko mat.
   ⚠ RED FLAGS: defensive hona / feedback pe bahas · requirements me 5-8 min se zyada ·
     har optimization turant thokna (parking lot use karo).

★ JP / FINANCE: deep-dive me ye teen shabd DROP karo: IDEMPOTENCY · AUDIT-TRAIL / LEDGER · ACID jahan paisa.
```

```
NA PATA HO -- section 7 ke upar jo baaki tha:
   COOL-DOWN: panic ki jad = "mujhe pata HONA chahiye". Replace: "main ise REASON karunga."
   5th escape: ASSUME + MOVE -> "I'll assume X and move on."   (kabhi FREEZE / chup nahi)
   RATTI LINE: "I haven't worked with X directly, but I'd approach it by ___. Let me note it and continue."
   per subject: DSA  -> brute force se shuru, loud ("naive O(n^2) is this, now optimize")
                JAVA -> "I don't remember the exact API, but my understanding is..." + reason
                HLD  -> block menu se reason; single right answer hota hi nahi, REASONING hi jawab

   ANJAAN DESIGN: design yaad nahi karte -- wahi ~15 BLOCK ki nayi jodi. Rail pe DERIVE karo:
      Google Docs -> "concurrent edits clash -> I'd sequence operations per doc" (OT/CRDT naam na
      aaye tab bhi reason). JP backend me Google-scale kam; rate-limiter / payment / notification /
      url type tere paas hain.

   "KUCH NAHI AAYA" ka darr:
      1-3 cheez na aana har candidate ke saath GUARANTEED. grade = na-aane pe kaise sambhala.
      ek unknown se round bekaar nahi; poora round grade hota, JP = 2/3 round tera zone.
      asli khatra = unknown nahi, SPIRAL (fail lag raha -> panic -> baaki bikhra). unknown ko spiral se alag rakho.
      "nahi aata par aise nikaalunga" = POSITIVE signal, bluff se zyada izzat.
      ANCHOR: 4 saal + 700 prod ticket, "2am prod down, pata nahi kya toota" -- ye usse aasaan.

META-SACH: perfect answer koi nahi deta · nerves + kuch chhootna normal · HLD = DIALOGUE, interviewer
   nudge karta · bolna TRAINABLE hai (mock se aata) · pehla interview = pehla rep, final nahi.
MOCK me coach kya dekhta: rail pe chala? · har box kyun? · trade-off saanche me? · atakne pe navigate?
   · pichhli baar se behtar? (poora-hai-ki-nahi NAHI)

1-LINE RECALL: RAIL pakdo -> har box naam + why -> trade-off saanche me -> atko to escape (kabhi chup
   nahi). Perfect nahi, NAVIGATE. Anjaan = blocks se derive. "Nahi aata" = ratti line + reason, spiral se alag.
```

---

<a id="shabd"></a>

## ★ SHABD — word atke to (01_DELIVERY PART 2 yahan mila, 29-Sep — poora rakha)

> **Kyun:** concept 100% aata (Hindi me round faad de). Sirf interview me English **word tongue pe**
> nahi aata -> us line pe freeze. Ye gap CHHOTA + FINITE hai. Yahan wo word + EXACT line.
> **KAISE:** padho mat — **LOUD BOLO**. Har topic ki "one breath" line 2-3 baar zabaan se nikaalo.
> **Yaad rakh:** soch tere paas HAI, ye sirf word-swap hai (bucket khatam -> "bucket is empty").
> Native banna zaroori nahi — clear soch + sahi word kaafi. JP-Bangalore Hinglish-ok.

<a id="shabd-0"></a>

### 0. UNIVERSAL HLD VERBS (har design me — connective tissue)

| bolna hai (Hindi) | English word | line |
|---|---|---|
| load sambhalna | **handle** | "the system should *handle* millions of requests per second" |
| bada karna | **scale** | "we *scale horizontally* by adding more servers" |
| baant dena | **distribute** | "requests are *distributed* across nodes" |
| pakka karna | **ensure** | "to *ensure* consistency, we ..." |
| kam karna (load/delay) | **reduce / offload** | "caching *reduces* load on the database" |
| fail hone pe bhi chale | **fault-tolerant / failover** | "if a node fails, we *failover* to a replica" |
| bottleneck / adchan | **bottleneck** | "the database becomes the *bottleneck* at scale" |
| trade-off | **trade-off** | "there's a *trade-off* between consistency and latency" |

<a id="shabd-1"></a>

### 1. RATE LIMITER (token bucket)

| tera Hindi | English word | line |
|---|---|---|
| bucket se token liya | **consume** (borrow nahi — token wapas nahi jaata) | "each request *consumes* a token from the bucket" |
| bucket bharta rehta | **refill / replenish** | "the bucket *refills* at a steady rate, say 10 tokens/sec" |
| bucket ki size | **capacity** | "the bucket has a fixed *capacity*" |
| ek saath thode zyada allow | **burst** | "this allows short *bursts* up to the bucket size" |
| bucket khatam / saare token use | **empty / exhausted** | "once the bucket is *empty*..." |
| request rok do | **throttle** | "extra requests are *throttled*" |
| mana kar do | **reject** | "the request is *rejected* with a 429 Too Many Requests" |
| ek jaisi speed | **steady rate** | "it smooths traffic to a *steady rate*" |

**ONE BREATH:** *"A token bucket has a fixed capacity and refills at a steady rate. Each request consumes a token. If tokens are available it's allowed, otherwise it's throttled and rejected with a 429 — this handles short bursts while keeping a steady average rate."*

<a id="shabd-2"></a>

### 2. CACHING

| tera Hindi | English word | line |
|---|---|---|
| data mila cache me | **cache hit** | "if it's a *cache hit*, we return immediately" |
| cache me nahi mila | **cache miss** | "on a *cache miss*, we go to the database" |
| purana data hata do | **evict** | "the least recently used entry is *evicted*" |
| expire time | **TTL (time to live)** | "each entry has a *TTL* after which it expires" |
| purana/basi data | **stale** | "the cache may serve *stale* data for a short time" |
| cache saaf karna | **invalidate** | "when data changes, we *invalidate* the cache entry" |
| load ghatana | **offload** | "caching *offloads* read traffic from the DB" |

**ONE BREATH:** *"We add a cache in front of the database. On a cache hit we return fast; on a miss we read from the DB and populate the cache. Entries have a TTL and are evicted when full. On writes we invalidate the entry to avoid stale data."*

<a id="shabd-3"></a>

### 3. LOAD BALANCING

| tera Hindi | English word | line |
|---|---|---|
| load baant do | **distribute** | "the load balancer *distributes* requests across servers" |
| baari-baari | **round robin** | "a simple strategy is *round robin*" |
| server zinda hai? check | **health check** | "it does periodic *health checks* on each server" |
| kharab server hata do | **remove from pool** | "an unhealthy server is *removed from the pool*" |
| ek user ek server pe | **sticky session** | "*sticky sessions* pin a user to one server" |

**ONE BREATH:** *"A load balancer sits in front and distributes incoming requests across servers, using strategies like round robin. It runs health checks and removes unhealthy servers from the pool, so traffic only goes to healthy nodes."*

<a id="shabd-4"></a>

### 4. DATABASE — REPLICATION & SHARDING

| tera Hindi | English word | line |
|---|---|---|
| data ki copy | **replica** | "we keep read *replicas* of the database" |
| likhne wala main DB | **primary / leader** | "writes go to the *primary*, reads to the replicas" |
| copy update hone me delay | **replication lag** | "there can be some *replication lag*" |
| data tukdo me baant do | **shard / partition** | "we *shard* the data across multiple databases" |
| kis shard me jaye | **shard key** | "we pick a *shard key*, like user id" |
| ek shard pe zyada load | **hot partition** | "a bad shard key can cause a *hot partition*" |

**ONE BREATH:** *"We use a primary for writes and read replicas for reads to scale reads, accepting some replication lag. For write scale we shard the data across databases using a shard key like user id, being careful to avoid hot partitions."*

<a id="shabd-5"></a>

### 5. ASYNC / MESSAGE QUEUES

| tera Hindi | English word | line |
|---|---|---|
| kaam baad me karo | **asynchronous** | "we process it *asynchronously*" |
| beech me queue | **queue / broker** | "requests go into a *message queue* like Kafka" |
| daalne wala | **producer** | "the *producer* publishes the event" |
| uthane wala | **consumer** | "a *consumer* picks it up and processes it" |
| load ka jhatka jhelo | **buffer / absorb spikes** | "the queue *buffers* traffic and absorbs spikes" |
| ek hi baar effect ho | **idempotent** | "processing is *idempotent*, so retries are safe" |

**ONE BREATH:** *"Instead of doing it inline, we push the work to a message queue. The producer publishes an event and a consumer processes it asynchronously. This decouples the services and lets the queue absorb traffic spikes. We make processing idempotent so retries are safe."*

<a id="shabd-6"></a>

### 6. CONSISTENCY / CAP

| tera Hindi | English word | line |
|---|---|---|
| har jagah same data | **strong consistency** | "banking needs *strong consistency*" |
| thodi der me sab same | **eventual consistency** | "for feeds, *eventual consistency* is fine" |
| network toot gaya | **network partition** | "during a *network partition*, we must choose" |
| response time | **latency** | "this reduces *latency* for the user" |
| ek saath kitne handle | **throughput** | "it increases the system's *throughput*" |
| do log ek seat, ek hi jeete | **conditional update** (check WHERE me) | "only one succeeds, because the check is *inside the WHERE*: `UPDATE seats SET status='booked' WHERE seat_id='A1' AND status='available'`" |

★ AADAT (27-Sep, BookMyShow): "atomic" / "race nahi hogi" bole to SAATH me query bhi bolo — haath roz WHERE lagata hai, muh se bhi nikle. `save()` sirf id pe UPDATE karta, status check nahi -> isliye ye line zaroori.

**ONE BREATH:** *"By CAP, during a network partition we choose between consistency and availability. Payments need strong consistency, but for something like a news feed, eventual consistency is acceptable to keep latency low and availability high."*

<a id="shabd-7"></a>

### 7. RELIABILITY / FAILURE

| tera Hindi | English word | line |
|---|---|---|
| ek point fail = sab fail | **single point of failure (SPOF)** | "we remove any *single point of failure*" |
| backup pe switch | **failover** | "on failure we *failover* to a standby" |
| dobara koshish | **retry with backoff** | "the client *retries with exponential backoff*" |
| girta hua system bacha lo | **circuit breaker** | "a *circuit breaker* stops calling a failing service" |
| thoda-thoda kaam karta rahe | **graceful degradation** | "the system *degrades gracefully* instead of crashing" |

**ONE BREATH:** *"We avoid single points of failure by replicating components and using failover to standbys. Clients retry with backoff, and a circuit breaker protects against a failing downstream service, so the system degrades gracefully instead of going fully down."*

<a id="shabd-drill"></a>

### HOW TO DRILL (roz 5 min)
1. Ek topic uthao -> **one-breath line LOUD bolo** 2-3 baar.
2. Phir file band karke wahi concept **apne words me English me bolo** (Hindi soch -> English word).
3. Atka? -> word table dekh lo -> dobara bolo. (jaise DSA nudge.)
4. Roz 1-2 topic. Ghis-ghis ke tongue pe chadhega. **Bolna hai, padhna nahi.**
