# Chat / Messaging System

> Maine message bheja, Ravi tak turant pahunche, chahe Ravi ne kuch na maanga ho. Ravi offline ho to baad me mile, kuch na khoye.
> Dil: banda KAHAN juda hai (baaki designs me "data kahan rakhein"), aur khuli connection.
>
> Mock me sirf heading + Cue + poster. Poster me sab: upar "Pehle aisa tha", tasveer (ilaaj), hara "Kyun chalta", laal number = kya toot sakta + kaise bacha, peela = doosra tareeka kyun nahi, grey = baaki.
> Cross-Q: hal ke har bade shabd pe socho -> gira? · dheema / bheed? · do ek saath? · galat hua? · kaise / kitna?

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c10_2-8d12e6ec.svg)

---


## Shuru

```
FR      1-to-1 aur chhote group (500 tak). History server pe rahegi (Slack jaisa, design zyada dikhta hai)
        tick chahiye: bheja / pahuncha / padha. Offline bande ko message baad me mile
        bahar rakha: E2E, voice / video, bade broadcast group
NFR     message turant pahunche (<1 sec), kabhi na khoye, ek chat ke andar kram na bigde, do baar na dikhe
        2 crore log ek saath jude rahein, server gire to doosra sambhale
NUMBERS 50 crore user, roz 10 crore log x 40 message = ~400 crore / din = ~46,000 likhai / sec
           bheed me 2-3 guna, ~1.5 lakh / sec
        ek waqt ~2 crore khuli connection = asli paimana
           ("kitni request / sec" nahi, "kitni connection khuli hain")
        jagah: 400 crore x ~300 byte ~ 1.2 TB roz, ~440 TB / saal
ALAG    ab tak (url shortener, feed, payment, banking) user maangta tha, server deta tha, connection band.
           Chat me Ravi ne kuch nahi maanga, phir bhi message turant chahiye
        HTTP me server sirf jawab de sakta hai, khud awaaz nahi laga sakta, isliye connection khuli rakhni hai
DABBA 0 sabse simple: ek chat server, Arpan aur Ravi dono jude, message server ke raaste Ravi tak jaata hai
SOCKET  WebSocket: Ravi ek baar /connect kholta hai, connection band nahi hota, dono taraf baat hoti hai
PAIMANA har khuli connection memory khaati hai, message aaye ya na aaye
FAISLA  connection ki apni line: 2 crore / ~1 lakh har machine = ~200 chat server.
           Ye sirf connection pakadte hain, API / business server se alag
```


> [!IMPORTANT]
> **RAIL: har design isi kram me chalta (8 dabbe, sab file me same)**
> kaam chala → bheed aayi → machine giri → do log ek saath → kuch dheema → data bada → door ke log → bahar ka hamla
>
> | rail | is design me | card |
> |---|---|---|
> | 1 kaam chala | Ravi kis server pe (map) · offline ko baad me · tick | 2, 8 |
> | 2 bheed aayi | 2 crore khuli connection · group 500 · online sabko | 1, 7, 10 |
> | 3 machine giri | bhejne se pehle server gira · Ravi ka phone band | 3 |
> | 4 do log ek saath | app ne dobara bheja · ghadi aage, kram ulta (**is design ka khaas**) | 5, 6 |
> | 5 kuch dheema | 200 MB video chat ke raaste | 9 |
> | 6 data bada | roz 1.2 TB message | 4 |
> | 7 door ke log | nahi aata | - |
> | 8 bahar ka hamla | is file me nahi: rate limit = 02_rate_limiter | - |

> [!NOTE]
> **AWS pe kaunsa DB (interview me naam bolna ho to)** · SQL wala = RDS, Cassandra RDS pe nahi chalta
>
> - Message (Cassandra) → **Amazon Keyspaces** (ya EC2 pe khud): key = chat_id, ek chat ek partition, likhai bahut
> - seq / online (Redis) → **ElastiCache (Redis)**: tez ginti aur haal
> - Search (Elasticsearch) → **Amazon OpenSearch**: purane message me shabd dhoondhna

---

*ab tootega kahan? sabse pehle connection ki ginti pe, kyunki 2 crore khuli hain*

## 1. Subah 2 crore log jude, server ki memory khatam

**Rail 2 BHEED AAYI (connection)** · Cue: EVENT LOOP, kuch thread, lakhon conn

<img src="img/c1_poster.png?v=3" width="1000" alt="card 1: kya toota, kaise bacha">


---

*ab 200 server hain, Ravi kis pe hai?*

## 2. Maine Ravi ko "hi" bheja, server bola Ravi offline

**Rail 1 KAAM CHALA (kaun kahan)** · Cue: MAP Redis me (kaun kahan)

<img src="img/c2_poster.png?v=3" width="1000" alt="card 2: kya toota, kaise bacha">


---

*ab Ravi mila hi nahi, phone band hai*

## 3. Ravi ka phone band tha, message kahan jaaye

**Rail 3 MACHINE GIRI (phone band)** · Cue: pehle DB, phir bhejo; PUSH

<img src="img/c3_poster.png?v=3" width="1000" alt="card 3: kya toota, kaise bacha">


---

*ab sab DB me likh rahe, DB kitna jhelega?*

## 4. Roz 1.2 TB message, ek DB nahi jhelta

**Rail 6 DATA BADA** · Cue: chat_id se baanto

<img src="img/c4_poster.png?v=3" width="1000" alt="card 4: kya toota, kaise bacha">


---

*ab DB sahi, par net dheema, app ne dobara bheja*

## 5. Net dheema, app ne dobara bheja, Ravi ko "hi" do baar

**Rail 4 DO LOG EK SAATH (retry)** · Cue: IDEMPOTENCY (clientMsgId)

<img src="img/c5_poster.png?v=3" width="1000" alt="card 5: kya toota, kaise bacha">


---

*ab ek baar hi aaya, par kram galat*

## 6. Meri ghadi 5 min aage, mera sawaal Ravi ke jawab ke neeche chala gaya

**Rail 4 DO LOG EK SAATH (khaas)** · Cue: SEQ, server ka +1

<img src="img/c6_poster.png?v=3" width="1000" alt="card 6: kya toota, kaise bacha">


---

*ab 1-to-1 theek, group me 500 log*

## 7. College Gang (500) me ek message, 500 baar likhein?

**Rail 2 BHEED AAYI (group)** · Cue: group ka EK copy

<img src="img/c7_poster.png?v=3" width="1000" alt="card 7: kya toota, kaise bacha">


---

*ab message pahunch gaye, tick aur unread?*

## 8. Ravi ne 50 padhe: unread aur neeli tick kaise

**Rail 1 KAAM CHALA (tick)** · Cue: NISHAAN read_upto

<img src="img/c8_poster.png?v=3" width="1000" alt="card 8: kya toota, kaise bacha">


---

*ab text theek, par video bhaari*

## 9. Shaadi ka 200 MB video chat ke raaste

**Rail 5 KUCH DHEEMA (video)** · Cue: S3 PRESIGNED URL, message me pata

<img src="img/c9_poster.png?v=3" width="1000" alt="card 9: kya toota, kaise bacha">


---

*ab aakhri: kaun online hai*

## 10. Raat ko online / last seen sabko kaise

**Rail 2 BHEED AAYI (online)** · Cue: UMAR 30 sec (TTL)

<img src="img/c10_poster.png?v=3" width="1000" alt="card 10: kya toota, kaise bacha">


---

## Poora design ek saans me

```
User -> LB -> Chat Server (khuli WebSocket, event loop)
     -> Idempotency (clientMsgId) -> Cassandra (chat_id partition, andar seq; pehle LIKHO phir bhejo)
     -> Redis (kaun kis server: umar + dhadkan) -> lene wale ka Chat Server -> lene wala
offline -> Push · wapas jude to "aakhri seq ke baad sab" · nishaan = delivered_upto / read_upto
media -> S3, message me sirf pata · online = Presence (TTL) · purana -> sasti jagah
DB: message = Cassandra (partition chat_id, andar seq; message_id = Snowflake, sirf pehchaan)
    zinda connection = server ki memory · pakka maal (message) = DB
```

Bolne ke liye: har user ~200 me se ek server se juda, Redis me likha kaun kahan. Message ki id se do baar nahi, pehle
DB me (chat ke hisaab se baanta), phir lene wale ke server tak. Offline ho to DB me pada + push, jude to "aakhri seq
ke baad". Tick aur unread har user ka nishaan, online umar wali Redis key, video S3 me, message me sirf pata.

---

## 10 guna traffic

- **Chat Server**: 2 crore khuli = ~200 machine sirf connection pakadne. Event loop, warna thread khatam.
- **Map**: memory me ho to server gira to gaya. Isliye Redis me umar wali entry + dhadkan.
- **Dobara judna**: ek server gira = 1 lakh ek saath. Ruk-ruk ke + random (backoff + jitter). Deploy pe draining.
- **Cassandra**: chat_id partition. Garam group pe speed ki had. Purana sasti jagah.
- **Push**: Google / Apple bahar ki cheez, apna retry / queue.
- **Redis**: copy, alag AZ. Message store ki bhi copy, alag AZ.

**Pooche: 10 guna kaise?** User ka raasta chalo, pehle jo toote.

**Pooche: single point of failure?** Chat server (dobara judo), Redis (copy).

**Pooche: chal raha hai, kaise pata?** Pahunchne ka p99, har machine pe khuli connection, dobara judne ki dar, push
fail, alert.

---

## Pooche to (kisi card me nahi baitha)

API: GET /connect (band nahi hoti, WebSocket) · POST /messages { chatId, text, clientMsgId } · GET
/messages?chatId=&before= (wapas lena + history, cursor) · POST /chats/{chatId}/read { read_upto } (tick).

Do duniya: zinda connection (memory, ek server ki, mar sakti) = map. Pakka maal (DB, sab jagah se dikhta) = message.
Ek user ke kai connection ho sakte (phone + laptop + tab).

Hands-on: ye design ek chhota server aur do browser tab chala ke bana (21-Sep). Demo ka code repo me nahi rakha.

- **Shuru ke 6 sawaal:** 1-to-1 / group, history kahan, E2E, tick, media, offline. Inme 2 poora design badalte.
- **E2E?** -> haan to server message padh hi nahi sakta: server pe search khatam, naya member purane message nahi dekh sakta, chaabi ka poora system. Sabse bada scope badalne wala.

---

ARCHETYPE D (real-time) · CONCEPTS: [message-queues](../../FOUNDATIONS/07_message_queues.md) · [sharding](../../FOUNDATIONS/06_database_sharding.md) · [Snowflake id](../../FOUNDATIONS/13_distributed_id_snowflake.md) · [CAP](../../FOUNDATIONS/08_cap_theorem.md) · [← SYSTEM_DESIGNS](..) · [← Home README](../../../README.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
