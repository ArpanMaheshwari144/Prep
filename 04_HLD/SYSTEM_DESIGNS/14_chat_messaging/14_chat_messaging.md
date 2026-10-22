# Chat / Messaging System

> Maine message bheja, Ravi tak turant pahunche, chahe Ravi ne kuch na maanga ho. Ravi offline ho to baad me mile, kuch na khoye.
> Dil: banda KAHAN juda hai (baaki designs me "data kahan rakhein"), aur khuli connection.
>
> Mock me sirf heading + Cue + poster. Neeche har card ek block: HAL · KYUN · NAHI · AGAR, phir YAAD RAKH dabba.
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
DABBA 0 sabse simple: ek chat server, main aur Ravi dono jude, message server ke raaste Ravi tak jaata hai
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

<img src="img/c1_poster.png?v=1" width="1000" alt="card 1: kya toota, kaise bacha">

```
HAL     kuch thread (jaise 4) saare connection sambhalte hain. OS batata hai data kahan aaya (epoll),
           thread sirf usi pe kaam karta hai = event loop, Netty jaisa
        app khula to LB use kisi server pe bithata hai, connection khula rehta hai, thread nahi
KYUN    zyadatar connection chup rehte hain. Chup connection thodi memory leta hai,
           thread tabhi jab data aaya, isliye ~200 server kaafi hain
NAHI    ek connection ek thread -> MEMORY: 2 crore x ~1 MB, memory khatam ho jaati hai
        polling (har 5 sec poochho) -> FALTU: 100 me se 1 hi poochh kaam ki. WebSocket me ek baar judo, turant
        SSE -> ADHOORA: sirf server se user ki taraf jaata hai, chat me dono taraf chahiye
AGAR    connection khula hai, aage ka message kis server pe jaayega?
        -> wahi server, kyunki connection khula hai. Naya connection banega tab naya chunaav hoga
        chat server gira
        -> 1 lakh log ek saath dobara judte hain. Ruk-ruk ke, random waqt pe judo (backoff + jitter).
           Bheja hua message khota nahi
        naya code deploy karna hai -> thode-thode server khaali karo (draining), taaki sab ek saath na judein
```

> [!IMPORTANT]
> **Yaad rakh:** Kuch thread (event loop) saari khuli connection sambhalte. Chup connection thodi memory, ~200 server kaafi.

---

*ab 200 server hain, Ravi kis pe hai?*

## 2. Maine Ravi ko "hi" bheja, server bola Ravi offline

**Rail 1 KAAM CHALA (kaun kahan)** · Cue: MAP Redis me (kaun kahan)

<img src="img/c2_poster.png?v=1" width="1000" alt="card 2: kya toota, kaise bacha">

```
MAP     "kaun kis server se juda hai", yahi chat ka dil hai. Ek server ki memory me map hota hai:
           Ravi -> conn-7, Arpan -> conn-3, aur "hi" usi connection se Ravi ki screen pe jaata hai
        map me connection data nahi, Ravi tak pahunchne ka raasta hai. Connection toota to map se hatao,
           dobara juda to naya connection, nayi entry
HAL     Redis me pata rakho (Ravi -> server-7). Server-1 Redis se poochta hai, server-7 ko deta hai,
           aur server-7 Ravi ko deta hai
KYUN    pata ek jagah hai, sab server wahi dekhte hain. Redis me sirf pata hai, umar ke saath (TTL);
           server-7 zinda hai to uski dhadkan se umar badhti rehti hai
        pata galat ho ya Redis gir jaaye, message phir bhi nahi khota
NAHI    har server ka apna map -> ADHOORA: main server-1 pe, Ravi server-7 pe,
           server-1 bola "Ravi offline", galat
        connection hi Redis me -> NAMUMKIN: zinda connection ek server ki memory me hota hai,
           wo mar sakta hai. Redis me pata, message DB me
        Redis pub-sub -> KHOTA: chal sakta hai, par kuch rakhta nahi, sunne wala na ho to message gaya
AGAR    server-7 mar gaya -> umar khatam, entry khud hat jaati hai, Ravi offline maana jaata hai
        Ravi ke kai device hain (phone + laptop + tab)
        -> pata ek nahi, list rakho (device -> server), aur har device pe bhejo
        pata galat hai, Ravi doosre server pe khisak gaya
        -> message khota nahi, naye server pe judte hi milega
        Redis gira -> copy pe switch (alag AZ), message khota nahi, judne pe milega
```

> [!IMPORTANT]
> **Yaad rakh:** Ravi kis server pe, ye pata Redis me (umar + dhadkan). Server-1 Redis se poochh ke server-7 ko deta.

---

*ab Ravi mila hi nahi, phone band hai*

## 3. Ravi ka phone band tha, message kahan jaaye

**Rail 3 MACHINE GIRI (phone band)** · Cue: pehle DB, phir bhejo; PUSH

<img src="img/c3_poster.png?v=1" width="1000" alt="card 3: kya toota, kaise bacha">

```
SCOPE   offline banda: message kahan ruke, kab tak ruke, push chahiye ya nahi? (ye pehle poochho)
HAL     pehle DB me likho, phir bhejo (store-then-forward). Phone band hai to push ghanti (FCM / APNs).
           Ravi wapas aaya to app bolta hai "aakhri ke baad sab do"
KYUN    DB me hai to khota nahi. Bhejne se pehle server gira, tab bhi message DB me pada hai
        push sirf ghanti hai, sirf preview, aur raasta Google ka hai.
           Wapas aaya to "aakhri ke baad" sab milta hai
NAHI    pehle bhejo, na pahunche to baad me DB -> KHOTA: DB me likhne se pehle server gira to message gaya
AGAR    Ravi wapas aaya -> app bolta hai "aakhri jo mila, uske baad sab do", server baaki de deta hai
        push wala (Google / Apple) atak gaya -> wo bahar ki cheez hai, apna retry / queue rakho
```

> [!IMPORTANT]
> **Yaad rakh:** Pehle DB me likho, phir bhejo. Phone band to push ghanti. Wapas aaya to "aakhri ke baad sab do".

---

*ab sab DB me likh rahe, DB kitna jhelega?*

## 4. Roz 1.2 TB message, ek DB nahi jhelta

**Rail 6 DATA BADA** · Cue: chat_id se baanto

<img src="img/c4_poster.png?v=1" width="1000" alt="card 4: kya toota, kaise bacha">

```
SCOPE   history kahan aur kitni der? WhatsApp: phone pe, pahunchte hi server se delete.
           Slack: server pe hamesha, 3 saal purana bhi search hota hai
        ye ek jawab jagah ko ~zero se ~440 TB / saal kar deta hai. History kahan = jagah ka dhaancha,
           yahan Slack jaisa
FAISLA  jagah ek machine ki nahi hai (banking ka ulta: wahan 120 / sec, yahan 46,000 / sec + 1.2 TB roz)
HAL     Cassandra, key = chat_id. Ek chat ke saare message ek partition me, andar kram se
KYUN    jo saath padha jaata hai wo saath rakho. Chat khuli to "aakhri 50" ek jagah se, ek baar me
           (keyset, OFFSET nahi)
NAHI    ek DB machine -> ATKA: 46,000 likhai / sec ek machine nahi jhelegi
        message_id se partition -> BIKHRA: aakhri 50 ke liye 50 jagah jaana padega,
           jo saath padha wo saath nahi
        SQL -> FALTU: rishte / transaction chahiye hi nahi, bas bahut likhai hai (46,000 / sec)
AGAR    ek group bahut garam ho gaya -> ek chat ek hi partition me, to group ki speed pe had hai
        saalon me partition badi ho gayi -> key (chat_id, mahina). Size bandhta hai, bojh nahi baant-ta
        3 saal purane message -> sasti jagah (cold storage), wo kabhi-kabhi hi khulte hain
        purana message search karna hai
        -> DB ke baad search index me bhi daalo (Elasticsearch). ES id deta hai, message Cassandra se aata hai
```

> [!IMPORTANT]
> **Yaad rakh:** Cassandra, key = chat_id: ek chat ek partition, andar kram se. Aakhri 50 ek jagah se.

---

*ab DB sahi, par net dheema, app ne dobara bheja*

## 5. Net dheema, app ne dobara bheja, Ravi ko "hi" do baar

**Rail 4 DO LOG EK SAATH (retry)** · Cue: IDEMPOTENCY (clientMsgId)

<img src="img/c5_poster.png?v=1" width="1000" alt="card 5: kya toota, kaise bacha">

```
HAL     app har message ki id banata hai (m-9), dobara bhejne pe wahi id (clientMsgId).
           Server poochta hai: pehle aaya? haan to skip, aur pehli id lauta do
KYUN    id app banata hai, server sirf "pehle dekha?" poochta hai, ek kadam.
           Wahi id = wahi message, dobara safe
NAHI    har bhejna naya message -> DOHRA: hi, timeout, phir hi, aur Ravi ki chat me "hi" do baar
AGAR    dono copy ek saath aa gayi -> dekhna + daalna ek kadam me (Cassandra LWT / Redis SET NX)
        do user ki id same nikli -> check jodi pe (sender_id, clientMsgId), alag log = alag message
        clientMsgId kab tak rakhein -> jitni der me dobara aata hai; retry minute me hota hai, 1 ghanta kaafi
```

> [!IMPORTANT]
> **Yaad rakh:** App har message ki id banata (m-9), dobara pe wahi id. Server (sender, id) dekh ke skip.

---

*ab ek baar hi aaya, par kram galat*

## 6. Meri ghadi 5 min aage, mera sawaal Ravi ke jawab ke neeche chala gaya

**Rail 4 DO LOG EK SAATH (khaas)** · Cue: SEQ, server ka +1

<img src="img/c6_poster.png?v=1" width="1000" alt="card 6: kya toota, kaise bacha">

```
HAL     har chat ka server pe ek +1 number (Redis INCR seq:chat123). Kram isi number (seq) se, ghadi se nahi.
           Har chat ka alag number
KYUN    kram server ka number tay karta hai, ghadi nahi. Phone 5 min aage ho, farak nahi padta
        ginti me chhed dikhe to pata chal jaata hai kuch chhoota, wo maang lo
NAHI    phone ki ghadi -> ULTA: meri ghadi 10:05 (asal 10:00), isliye kram ulta ho gaya
        Snowflake id -> PEHCHAAN: wo sirf pehchaan hai (message_id), kram nahi;
           do server ki ghadi me ms ka farak
AGAR    app ko 4415 mila, phir 4417 -> 4416 chhoota (chhed). Thoda ruko, phir maango "4415 ke baad ke do"
        seq wala Redis restart ho gaya -> Cassandra me har message ka seq hai, sabse bade se aage chalo
        Ravi wapas aaya -> app bolta hai "meri aakhri 4417", server 4418 se aage sab deta hai
```

> [!IMPORTANT]
> **Yaad rakh:** Har chat ka server pe +1 number (seq). Kram seq se, ghadi se nahi. Chhed dikhe to maango.

---

*ab 1-to-1 theek, group me 500 log*

## 7. College Gang (500) me ek message, 500 baar likhein?

**Rail 2 BHEED AAYI (group)** · Cue: group ka EK copy

<img src="img/c7_poster.png?v=1" width="1000" alt="card 7: kya toota, kaise bacha">

```
SCOPE   1-to-1 ya group? Group = ek message, 500 jagah (ye pehle poochho)
HAL     group ka message EK baar likho, group chat ke neeche, sab wahi padhte hain.
           Bhejna server-wise: online kahan hai wo Redis se, har server ko ek call
           (server-2 pe 120, server-5 pe 80)
KYUN    sabke liye ek jaisa = ek copy, har kisi ka alag = alag copy. Bhejna server-wise, member-wise nahi
NAHI    har member ke inbox me copy -> MEHNGA: ek message, 500 likhai
AGAR    300 online hain -> Redis se unke server nikalo, har server ko ek call
        offline members -> push ghanti, wapas aaye to seq se aage sab
        naya member juda -> judne ke seq se pehle wale message nahi milte, tab wo group me tha hi nahi
```

> [!IMPORTANT]
> **Yaad rakh:** Group ka message EK baar likho. Bhejna server-wise: online kahan, Redis se, har server ko ek call.

---

*ab message pahunch gaye, tick aur unread?*

## 8. Ravi ne 50 padhe: unread aur neeli tick kaise

**Rail 1 KAAM CHALA (tick)** · Cue: NISHAAN read_upto

<img src="img/c8_poster.png?v=1" width="1000" alt="card 8: kya toota, kaise bacha">

```
SCOPE   tick: ek (server tak), do (phone tak), neeli (padh li), online / last seen.
           Har tick ek ulta message hai, traffic kai guna ho jaata hai
HAL     har member ka ek number, kahan tak padha (read_upto). Ravi ne 50 padhe -> read_upto = 4467, ek likhai
KYUN    nishaan aage khisakta hai, ginti nahi hoti. Unread, tick, offline ke baad lena, sab isi se.
           Khoya to agli baar aa jaayega
NAHI    har message pe "padha" record -> MEHNGA: 50 x 500 = 25,000 likhai
AGAR    unread kitne -> aakhri seq - nishaan, ghatao, gino nahi
        neeli tick kaise -> mujhe ek push "read_upto 4467", aur 50 ek saath neeli
        do tick (mila) kaise -> "kahan tak mila" (delivered_upto), wo Ravi ka phone bhejta hai
        "mila" raaste me kho gaya -> agli baar judte hi phone dobara bhejta hai, nishaan sirf aage jaata hai
        group me do tick kab -> sabse chhota nishaan dekho: sab ka delivered_upto >= seq
        10 lakh ka broadcast group -> tick band, sirf unread; 10 lakh nishaan milana mehnga hai
```

> [!IMPORTANT]
> **Yaad rakh:** Har member ka ek nishaan, kahan tak padha (read_upto). Unread = aakhri seq - nishaan.

---

*ab text theek, par video bhaari*

## 9. Shaadi ka 200 MB video chat ke raaste

**Rail 5 KUCH DHEEMA (video)** · Cue: S3 PRESIGNED URL, message me pata

<img src="img/c9_poster.png?v=1" width="1000" alt="card 9: kya toota, kaise bacha">

```
SCOPE   media message ke saath nahi jaata, alag S3 me jaata hai, message me sirf pata
HAL     app video seedha S3 me daalta hai, 10 min ka paas leke (presigned URL).
           Message me sirf S3 key + thumbnail
KYUN    bhaari maal S3 me, message halka. Ek upload, aur 500 ko wahi key
        message me key rakho, URL nahi, kyunki URL marta hai
NAHI    chat server ke raaste -> ATKA: 200 MB pe server 2 min atka, group me 500 x 200 MB = 100 GB
AGAR    200 MB upload ke beech net gaya -> tukdon me upload (multipart), bacha hissa hi dobara
        Ravi ne video khola -> key se naya chhota URL banta hai. URL marta hai, key nahi
        wahi video 1000 baar forward hua -> file ka hash dekho, pehle se hai to wahi file, upload nahi
        E2E hai to thumbnail kaise
        -> app khud encrypt karta hai, chaabi message ke saath, server dekh nahi sakta
```

> [!IMPORTANT]
> **Yaad rakh:** Video seedha S3 me (10 min ka paas). Message me sirf S3 key + thumbnail. URL marta, key nahi.

---

*ab aakhri: kaun online hai*

## 10. Raat ko online / last seen sabko kaise

**Rail 2 BHEED AAYI (online)** · Cue: UMAR 30 sec (TTL)

<img src="img/c10_poster.png?v=1" width="1000" alt="card 10: kya toota, kaise bacha">

```
HAL     online = Redis key + umar (30 sec), app har 15 sec dhadkan bhejta hai "hoon".
           Status tabhi dekho jab chat khule
KYUN    jab maango tab dekho, key khud mit jaati hai. Thoda purana status chal jaata hai, paisa nahi lagta
NAHI    sabko har badlav batao -> BHEED: 500 contacts x crore log, bahut message
AGAR    chat khuli hai aur main chala gaya -> khuli chat = subscribe (live badlav), baaki chat pull
        crore dhadkan har second -> chat server ikattha karke Redis me daalta hai (batch)
        app band ho gaya -> dhadkan band, umar khatam = offline. Server us waqt last seen alag likhta hai
        Ravi ne last seen chhupaya -> mat dikhao
        "typing..." -> sirf khuli chat me, store nahi, 2 sec ki khabar
```

> [!IMPORTANT]
> **Yaad rakh:** Online = Redis key + umar 30 sec, app har 15 sec dhadkan. Status tabhi dekho jab chat khule.

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
