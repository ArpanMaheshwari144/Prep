# Chat / Messaging System

> Maine message bheja, Ravi tak turant pahunche, chahe Ravi ne kuch na maanga ho. Ravi offline ho to baad me mile, kuch na khoye.
> Dil: banda KAHAN juda hai (baaki designs me "data kahan rakhein"), aur khuli connection.
>
> Mock me sirf heading + Cue + GIF. Neeche har card: **hal** · **kyun** · **nahi** (doosra kyun nahi) · **Pooche**.

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c10_2-8d12e6ec.svg)

---


## Shuru

- **FR:** 1-to-1 + chhote group (500 tak) · history server pe (Slack jaisa, design zyada dikhta) · tick: bheja / pahuncha / padha · offline ko baad me mile · bahar: E2E, voice / video, bade broadcast group
- **NFR:** turant (<1 sec) · kabhi na khoye · ek chat ke andar kram na bigde · do baar na dikhe · 2 crore ek saath jude · server gire to doosra sambhale
- **numbers:** 50 crore user, 10 crore roz x 40 message = ~400 crore / din = ~46,000 likhai / sec, bheed me 2-3 guna ~1.5 lakh · ek waqt ~2 crore khuli connection = asli paimana ("kitni request / sec" nahi, "kitni connection khuli") · jagah 400 crore x ~300 byte ~ 1.2 TB roz, ~440 TB / saal

---

*ab tootega kahan? sabse pehle connection ki ginti pe, kyunki 2 crore khuli hain*

## 1. Subah 2 crore log jude, server ki memory khatam

**Cue:** EVENT LOOP, kuch thread, lakhon conn · dabba: Chat Server x200 + LB

- **chat alag kyun:** ab tak (url shortener, feed, payment, banking) user maanga, server diya, connection band. Chat me Ravi ne kuch nahi maanga, phir bhi turant.
- **chat alag kyun:** HTTP me server sirf jawab de sakta, khud awaaz nahi laga sakta -> connection khuli rakhni
- **dabba 0:** sabse simple, ek chat server, main aur Ravi dono jude, message server ke raaste Ravi tak
- **WebSocket:** Ravi ek baar /connect kholta, connection band nahi hota, dono taraf baat
- **paimana:** har khuli connection memory khaati, message aaye ya na aaye
- **faisla:** connection ki apni line: 2 crore / ~1 lakh har machine = ~200 chat server, sirf connection pakadte, API / business server se alag
- **toota:** ek connection = ek thread (~1 MB), 2 crore thread -> memory khatam
- **hal:** kuch thread (jaise 4) saare connection. OS batata data kahan aaya (epoll), thread sirf usi pe (event loop, Netty jaisa).
- **hal:** app khula to LB kisi server pe bithata, connection khula, thread nahi
- **kyun:** zyadatar connection chup. Chup connection thodi memory, thread tabhi jab data aaya -> ~200 server kaafi.
- **nahi:** ek connection ek thread -> 2 crore x ~1 MB, memory khatam
- **nahi:** polling (har 5 sec poochho) -> 100 me 1 hi poochh kaam ki. WebSocket me ek baar judo, turant.
- **nahi:** SSE -> sirf server se user, chat me dono taraf chahiye
- **Pooche:** wahi connection aage kis server pe? -> wahi, connection khula. Naya connection = naya chunaav.
- **Pooche:** chat server gira? -> 1 lakh log ek saath dobara judte. Ruk-ruk ke, random waqt (backoff + jitter). Bheja hua khota nahi.
- **Pooche:** naya code deploy? -> thode-thode server khaali karo (draining), sab ek saath na judein

---

*ab 200 server hain, Ravi kis pe hai?*

## 2. Maine Ravi ko "hi" bheja, server bola Ravi offline

**Cue:** MAP Redis me (kaun kahan) · dabba: Redis

- **map:** "kaun kis server se juda", yahi chat ka dil. Ek server me memory map: Ravi -> conn-7, Arpan -> conn-3, "hi" usi connection se Ravi ki screen pe.
- **map:** connection data nahi, Ravi tak pahunchne ka raasta. Toota to map se hatao, dobara juda to naya connection, nayi entry.
- **toota:** har server ke map me sirf apne log. Main server-1, Ravi server-7 -> server-1 bola "Ravi offline", galat.
- **hal:** Redis me pata: Ravi -> server-7 (map). Server-1 Redis se poochta, server-7 ko deta, server-7 Ravi ko.
- **kyun:** pata ek jagah, sab server dekhte. Redis me sirf pata, umar ke saath (TTL), server-7 zinda to dhadkan se umar badhti.
- **kyun:** pata galat ho ya Redis gire, message phir bhi nahi khota
- **nahi:** connection hi Redis me -> ho hi nahi sakta, zinda connection ek server ki memory me, mar sakti. Redis me pata, message DB me.
- **nahi:** Redis pub-sub -> chal sakta, par kuch rakhta nahi, sunne wala na ho to message gaya
- **Pooche:** server-7 mar gaya? -> umar khatam, entry khud hat-ti, Ravi offline maana
- **Pooche:** Ravi ke kai device (phone + laptop + tab)? -> pata ek nahi, list (device -> server), har device pe bhejo
- **Pooche:** pata galat, Ravi khisak gaya? -> message khota nahi, naye server pe judte hi milega
- **Pooche:** Redis gira? -> copy pe switch (alag AZ), message khota nahi, judne pe milega

---

*ab Ravi mila hi nahi, phone band hai*

## 3. Ravi ka phone band tha, message kahan jaaye

**Cue:** pehle DB, phir bhejo; PUSH · dabba: Message DB + Push

- **scope:** offline banda: message kahan ruke, kab tak, push? (pehle poochho)
- **toota:** pehle bhejte, na pahunche to DB. Server DB me likhne se pehle gira -> message gaya.
- **hal:** pehle DB me likho, phir bhejo (store-then-forward). Phone band to push ghanti (FCM / APNs).
- **kyun:** DB me hai to khota nahi, bhejne se pehle server gira tab bhi DB me
- **kyun:** push sirf ghanti, sirf preview, raasta Google ka. Wapas aaya to "aakhri ke baad" sab.
- **nahi:** pehle bhejo, baad me DB -> beech me server gira, message gaya
- **Pooche:** Ravi wapas aaya? -> app bolta "aakhri jo mila, uske baad sab do", server baaki deta
- **Pooche:** push wala (Google / Apple) atka? -> bahar ki cheez, apna retry / queue

---

*ab sab DB me likh rahe, DB kitna jhelega?*

## 4. Roz 1.2 TB message, ek DB nahi jhelta

**Cue:** chat_id se baanto · dabba: Cassandra

- **scope:** history kahan, kitni der? WhatsApp: phone pe, pahunchte hi server se delete. Slack: server pe hamesha, 3 saal purana bhi search.
- **scope:** ye ek jawab jagah ~zero se ~440 TB / saal kar deta. History kahan = jagah ka dhaancha, yahan Slack jaisa.
- **faisla:** jagah ek machine ki nahi (banking ka ulta: wahan 120 / sec, yahan 46,000 / sec + 1.2 TB roz)
- **toota:** ek DB machine, 46,000 likhai / sec nahi jhelegi
- **hal:** Cassandra, ek chat ke saare message ek partition (key = chat_id), andar kram se
- **kyun:** jo saath padha jaata wo saath rakho. Chat khuli to "aakhri 50" ek jagah, ek baar (keyset, OFFSET nahi).
- **nahi:** message_id se partition -> aakhri 50 ke liye 50 jagah, saath padha saath nahi
- **nahi:** SQL -> rishte / transaction chahiye hi nahi, bas bahut likhai (46,000 / sec)
- **Pooche:** ek group bahut garam? -> ek chat ek partition hi, group ki speed pe had
- **Pooche:** saalon me partition badi? -> key (chat_id, mahina). Size bandhta, bojh nahi baant-ta.
- **Pooche:** 3 saal purane? -> sasti jagah (cold storage), kabhi-kabhi khulte
- **Pooche:** purana message search? -> DB ke baad search index me bhi (Elasticsearch). ES id deta, message Cassandra se.

---

*ab DB sahi, par net dheema, app ne dobara bheja*

## 5. Net dheema, app ne dobara bheja, Ravi ko "hi" do baar

**Cue:** IDEMPOTENCY (clientMsgId) · dabba: Idempotency check

- **toota:** har bhejna naya message: hi, timeout, phir hi -> Ravi ki chat me do baar
- **hal:** app har message ki id banata (m-9), dobara pe wahi id (clientMsgId). Server: pehle aaya? haan to skip, pehli id lautao.
- **kyun:** id app banata, server sirf "pehle dekha?" poochta, ek kadam. Wahi id = wahi message, dobara safe.
- **Pooche:** dono ek saath? -> dekhna + daalna ek kadam (Cassandra LWT / Redis SET NX)
- **Pooche:** do user ki id same? -> check jodi pe (sender_id, clientMsgId), alag log = alag message
- **Pooche:** clientMsgId kab tak? -> jitni der me dobara aata, retry minute me, 1 ghanta kaafi

---

*ab ek baar hi aaya, par kram galat*

## 6. Meri ghadi 5 min aage, mere message upar chadh gaye

**Cue:** SEQ, server ka +1 · dabba: Seq (Redis INCR)

- **toota:** kram phone ki ghadi se, meri 10:05 (asal 10:00), mera message Ravi se upar, kram ulta
- **hal:** har chat ka server pe +1 number (Redis INCR seq:chat123), kram isi se (seq), har chat alag
- **kyun:** kram server ka number tay karta, ghadi nahi. 5 min aage ho, farak nahi.
- **kyun:** ginti me chhed dikhe to pata chalta kuch chhoota, maang lo
- **nahi:** phone ki ghadi -> kram ulta
- **nahi:** Snowflake id -> sirf pehchaan (message_id), kram nahi, do server ki ghadi me ms ka farak
- **Pooche:** app ko 4415, phir 4417? -> 4416 chhoota (chhed). Thoda ruko, phir maango "4415 ke baad ke do".
- **Pooche:** seq wala Redis restart? -> Cassandra me har message ka seq, sabse bade se aage
- **Pooche:** Ravi wapas aaya? -> "meri aakhri 4417" -> 4418 se aage sab

---

*ab 1-to-1 theek, group me 500 log*

## 7. College Gang (500) me ek message, 500 baar likhein?

**Cue:** group ka EK copy · dabba: (Cassandra me group chat)

- **scope:** 1-to-1 ya group? group = ek message, 500 jagah (pehle poochho)
- **toota:** har member ke inbox me copy -> ek message, 500 likhai
- **hal:** group ka message ek baar, group chat ke neeche, sab wahi padhte
- **hal:** bhejna server-wise: online kahan Redis se, har server ko ek call (server-2 pe 120, server-5 pe 80)
- **kyun:** sabke liye ek jaisa = ek copy, har kisi ka alag = alag copy. Bhejna server-wise, member-wise nahi.
- **nahi:** har member ke inbox me copy -> 500 likhai
- **Pooche:** 300 online? -> Redis se unke server, har server ko ek call
- **Pooche:** offline members? -> push ghanti, wapas aaye to seq se aage
- **Pooche:** naya member juda? -> judne ke seq se pehle wale nahi, tab group me tha hi nahi

---

*ab message pahunch gaye, tick aur unread?*

## 8. Ravi ne 50 padhe: unread aur neeli tick kaise

**Cue:** NISHAAN read_upto · dabba: Nishaan store

- **scope:** tick: ek (server tak), do (phone tak), neeli (padh li), online / last seen. Har tick ek ulta message, traffic kai guna.
- **toota:** har message x har member ka "padha" record: 50 x 500 = 25,000 likhai
- **hal:** har member ka ek number, kahan tak padha (read_upto). Ravi ne 50 padhe -> read_upto = 4467, ek likhai.
- **kyun:** nishaan khisakta, ginti nahi. Unread, tick, offline ke baad lena, sab isi se. Khoya to agli baar.
- **nahi:** har message pe "padha" record -> 25,000 likhai
- **Pooche:** unread? -> aakhri seq - nishaan, ghatao, gino nahi
- **Pooche:** neeli tick? -> mujhe ek push "read_upto 4467", 50 ek saath neeli
- **Pooche:** do tick (mila)? -> "kahan tak mila" (delivered_upto), Ravi ka phone bhejta
- **Pooche:** "mila" raaste me kho gaya? -> agli baar judte hi phone dobara bhejta, nishaan sirf aage
- **Pooche:** group me do tick? -> sabse chhota nishaan: sab ka delivered_upto >= seq
- **Pooche:** 10 lakh ka broadcast group? -> tick band, sirf unread, 10 lakh nishaan milana mehnga

---

*ab text theek, par video bhaari*

## 9. Shaadi ka 200 MB video chat ke raaste

**Cue:** S3 PRESIGNED URL, message me pata · dabba: S3

- **scope:** media message ke saath nahi, alag S3 me, message me sirf pata
- **toota:** video chat server se, 200 MB pe server 2 min atka, group me 500 x 200 MB = 100 GB
- **hal:** app video seedha S3 me, 10 min ka paas (presigned URL). Message me sirf S3 key + thumbnail.
- **kyun:** bhaari maal S3, message halka. Ek upload, 500 ko wahi key.
- **kyun:** message me key, URL nahi, kyunki URL marta hai
- **nahi:** chat server ke raaste -> server atka, 100 GB
- **Pooche:** 200 MB me net gaya? -> tukdon me upload (multipart), bacha hissa hi dobara
- **Pooche:** Ravi ne khola? -> key se naya chhota URL. URL marta, key nahi.
- **Pooche:** wahi video 1000 baar forward? -> file ka hash, pehle se hai to wahi file, upload nahi
- **Pooche:** E2E hai to thumbnail? -> app khud encrypt, chaabi message ke saath, server dekh nahi sakta

---

*ab aakhri: kaun online hai*

## 10. Raat ko online / last seen sabko kaise

**Cue:** UMAR 30 sec (TTL) · dabba: Presence

- **toota:** har aana-jaana saare contacts ko: 500 contacts x crore log, bahut message
- **hal:** online = Redis key + umar (30 sec), app har 15 sec dhadkan "hoon". Status tabhi dekho jab chat khule.
- **kyun:** maango tab dekho, key khud mitti. Thoda purana status chalta, paisa nahi.
- **nahi:** sabko har badlav batao -> bahut message
- **Pooche:** chat khuli, main chala gaya? -> khuli chat = subscribe (live badlav), baaki chat pull
- **Pooche:** crore dhadkan har second? -> chat server ikattha karke Redis me (batch)
- **Pooche:** app band? -> dhadkan band, umar khatam = offline. Server us waqt last seen alag likhta.
- **Pooche:** Ravi ne last seen chhupaya? -> mat dikhao
- **Pooche:** "typing..."? -> sirf khuli chat me, store nahi, 2 sec ki khabar

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
