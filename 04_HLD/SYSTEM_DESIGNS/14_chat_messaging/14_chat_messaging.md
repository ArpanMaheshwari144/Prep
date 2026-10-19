# Chat / Messaging System

> Maine message bheja, Ravi tak turant pahunche, chahe Ravi ne kuch na maanga ho. Ravi offline ho to baad me mile, kuch na khoye.
> Dil: banda KAHAN juda hai (baaki designs me "data kahan rakhein"), aur khuli connection.
>
> Har card: kya toota -> kya kiya -> yahi kyun -> doosra kyun nahi. Kram me chalo. "Pooche" wali line = interviewer aage poochhe to.

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c10_2-8d12e6ec.svg)

---

## Mock cue (mock me sirf ye dekho: kya hua -> ilaaj -> kaunsa dabba juda)

```
    KYA HUA                                   ILAAJ                                DABBA JUDA
1   subah 2 crore jude, memory khatam      -> EVENT LOOP, kuch thread, lakhon conn   Chat Server x200 + LB
2   Ravi doosre server pe, "offline" bola  -> MAP Redis me (kaun kahan)            Redis
3   Ravi ka phone band                     -> pehle DB, phir bhejo; PUSH           Message DB + Push
4   roz 1.2 TB message                     -> chat_id se baanto                    Cassandra
5   net dheema, "hi" do baar               -> IDEMPOTENCY (clientMsgId)            Idempotency check
6   meri ghadi 5 min aage                  -> SEQ, server ka +1                    Seq (Redis INCR)
7   College Gang (500) me message          -> group ka EK copy                     (Cassandra me group chat)
8   Ravi ne 50 padhe: unread / tick        -> NISHAAN read_upto                    Nishaan store
9   200 MB shaadi ka video                 -> S3 PRESIGNED URL, message me pata    S3
10  raat ko online / last seen             -> UMAR 30 sec (TTL)                    Presence

POORA DESIGN (upar ke dabbe kram se):
   User -> LB -> Chat Server (event loop) -> Idempotency -> Cassandra (chat_id, seq)
   Redis: kaun kis server · Seq: +1 · Push: offline ghanti · Nishaan: read_upto · S3: video · Presence: TTL
```

---

## Shuru

Chat baaki designs se alag kyun? Ab tak (url shortener, feed, payment, banking) user ne maanga, server ne diya,
connection band. Chat me maine bheja, Ravi ne kuch nahi maanga, phir bhi Ravi tak turant. HTTP me server sirf jawab
de sakta, khud awaaz nahi laga sakta. Isliye do nayi cheez: connection khuli rakhni, aur ek map "kaun kis server se
juda hai". Yahi chat ka dil.

Sabse simple: ek chat server. Main aur Ravi dono usse jude, message server ke raaste Ravi tak.

Ravi ek baar /connect kholta, connection band nahi hota, dono taraf baat (WebSocket). Server ki memory me map:
Ravi -> conn-7, Arpan -> conn-3. Main "hi" bheju to map me Ravi -> conn-7, usi connection se turant Ravi ki screen
pe. Connection data nahi hai, Ravi tak pahunchne ka raasta hai. Toota to map se hatao, dobara juda to naya
connection, map me nayi entry.

Pehle 6 sawaal poocho, inme 2 poora design badalte. 1-to-1 ya group (group = ek message, 500 jagah)? History kahan,
kitni der: WhatsApp me phone pe, pahunchte hi server se delete; Slack me server pe hamesha, 3 saal purana bhi search.
Ye ek jawab jagah ~zero se ~440 TB / saal kar deta. End-to-end encryption? Haan to server message padh hi nahi
sakta: server pe search khatam, naya member purane message nahi dekh sakta, chaabi ka poora system. Ye sabse bada
scope badalne wala. Tick? Ek tick (server tak), do tick (phone tak), neeli (padh li), online / last seen. Har tick
ek ulta message, traffic kai guna. Media? Message ke saath nahi, alag S3 me, message me sirf pata. Offline banda?
Message kahan ruke, kab tak, push?

Maan ke chalo (bol ke): 1-to-1 + chhote group (500 tak), history server pe (Slack jaisa, design zyada dikhta), E2E
abhi nahi ("scope se bahar"), tick: bheja / pahuncha / padha. Bahar: E2E, voice / video, bade broadcast group.

Kaisa chale: turant (<1 sec), kabhi na khoye, ek chat ke andar kram na bigde, do baar na dikhe, 2 crore ek saath
jude, server gire to doosra sambhale.

Numbers: 50 crore user, 10 crore roz, har ek 40 message roz, to ~400 crore message roz. 400 crore / 86,400 second
~ 46,000 likhai har second, bheed me 2-3 guna ~1.5 lakh. Ek waqt ~2 crore khuli connection, yahi asli paimana:
"kitni request / second" nahi, "kitni connection khuli". Har khuli connection memory khaati, message aaye ya na aaye.
Jagah: 400 crore x ~300 byte ~ 1.2 TB roz, 1 saal ~440 TB (Slack jaisa). WhatsApp jaisa ~zero.

Teen faisle: connection ki apni line, 2 crore / ~1 lakh har machine = ~200 chat server jo sirf connection pakadte,
API / business server se alag. Jagah ek machine ki nahi (banking ka ulta: wahan 120 / sec, yahan 46,000 / sec + 1.2
TB roz). Aur history kahan, usi se jagah ka dhaancha tay, yahan Slack jaisa.

**Pooche: polling kyun nahi, har 5 sec poochho?** 100 me 1 hi poochh kaam ki. WebSocket me ek baar judo, turant.

**Pooche: SSE kyun nahi?** SSE sirf server se user. Chat me dono taraf chahiye.

---

*ab tootega kahan? sabse pehle connection ki ginti pe, kyunki 2 crore khuli hain*

## 1. Subah 2 crore log jude, server ki memory khatam

Pehle ek connection = ek thread (~1 MB). 2 crore connection, har ek ka thread. Memory khatam.

Isliye kuch thread (jaise 4) saare connection sambhalte hain. Zyadatar connection chup rehte. OS batata kahan data
aaya (epoll), thread sirf usi pe kaam karta (event loop, Netty jaisa). App khula to LB use kisi server pe bithata,
connection khula, thread nahi.

Ye kyun chalta? Connection kholo, thread mat do. Chup connection thodi si memory leta, thread tabhi jab data aaya.
Isliye ~200 server kaafi.

Ek connection ek thread kyun nahi? Wahi hua: 2 crore thread, har ek ~1 MB, memory khatam.

**Pooche: wahi connection aage kis server pe?** Wahi server, connection khula hai. Naya connection = naya chunaav.

**Pooche: chat server gira?** Ek server gira = 1 lakh log ek saath dobara judte. Isliye ruk-ruk ke, random waqt pe
judo (backoff + jitter). Bheja hua khota nahi.

**Pooche: naya code deploy?** Thode-thode server khaali karo (draining), taaki sab ek saath na judein.

---

*ab 200 server hain, Ravi kis pe hai?*

## 2. Maine Ravi ko "hi" bheja, server bola Ravi offline

Pehle har server ke map me sirf apne log. Main server-1 pe, Ravi server-7 pe. Server-1 ke map me Ravi nahi, to bola
"Ravi offline". Galat.

Isliye Redis me pata: Ravi -> server-7 (map). Server-1 Redis se poochta, server-7 ko deta, server-7 Ravi ko.

Ye kyun chalta? Pata ek jagah hai jo sab server dekh sakte. Redis me sirf pata, umar ke saath (TTL). Server-7 zinda
hai to dhadkan se entry ki umar badhti rehti. Pata galat ho ya Redis gire, message phir bhi nahi khota.

Connection hi Redis me rakh do? Ho hi nahi sakta. Zinda connection ek server ki memory me hai, mar sakti. Redis me
sirf pata. Pakka maal (message) DB me.

Redis pub-sub se kyun nahi? Chal sakta, par wo kuch rakhta nahi. Sunne wala na ho to message gaya.

**Pooche: server-7 mar gaya?** Umar khatam, entry khud hat jaati. Ravi offline maana.

**Pooche: Ravi ke kai device (phone + laptop + tab)?** Pata ek nahi, list (device -> server). Har device pe bhejo.

**Pooche: pata galat nikla, Ravi khisak gaya?** Message khota nahi. Naye server pe judte hi milega.

**Pooche: Redis hi gir gaya?** Copy pe switch (alag AZ). Message khota nahi, judne pe milega.

---

*ab Ravi mila hi nahi, phone band hai*

## 3. Ravi ka phone band tha, message kahan jaaye

Pehle bhejte the, na pahunche to DB me. Server DB me likhne se pehle gira, message gaya.

Isliye pehle DB me likho, phir bhejne ki koshish (store-then-forward). Phone band hai to push ghanti (FCM / APNs).

Ye kyun chalta? DB me hai to khota nahi. Bhejne se pehle server gira tab bhi DB me pehle se. Push sirf ghanti hai,
sirf preview, raasta Google ka. Ravi wapas aaya to "aakhri ke baad" sab milta.

Pehle bhejo, baad me DB? Wahi hua: beech me server gira, message gaya.

**Pooche: Ravi wapas aaya?** App bolta "aakhri jo mila, uske baad sab do". Server baaki de deta.

**Pooche: push wala (Google / Apple) hi atka?** Wo bahar ki cheez hai. Apna retry / queue rakho.

---

*ab sab DB me likh rahe, DB kitna jhelega?*

## 4. Roz 1.2 TB message, ek DB nahi jhelta

Pehle ek DB machine. 46,000 likhai / second, nahi jhelega.

Isliye Cassandra, aur ek chat ke saare message ek partition me (key = chat_id), andar kram se.

Ye kyun chalta? Jo saath padha jaata wo saath rakho. Chat khuli to "is chat ke aakhri 50" ek jagah, ek baar
(keyset, OFFSET nahi).

message_id se partition kyun nahi? Aakhri 50 laane 50 jagah jaana padta. Jo saath padha jaata wo saath nahi rehta.

SQL kyun nahi? Rishte / transaction chahiye hi nahi, bas bahut likhai (46,000 / sec).

**Pooche: ek group bahut garam?** Ek chat ek partition hi rahega. Group ki speed pe had hai.

**Pooche: saalon me partition bahut badi?** Key (chat_id, mahina). Size bandh jaata, par bojh nahi baant-ta.

**Pooche: 3 saal purane message?** Sasti jagah (cold storage). Kabhi-kabhi hi khulte.

**Pooche: purana message search?** DB ke baad search index me bhi likho (Elasticsearch). ES id deta, message
Cassandra se.

---

*ab DB sahi, par net dheema, app ne dobara bheja*

## 5. Net dheema, app ne dobara bheja, Ravi ko "hi" do baar

Pehle har bhejna naya message. Hi, timeout, phir hi. Ravi ki chat me do baar.

Isliye app har message ki apni id banata (m-9), dobara bheje to wahi id (clientMsgId). Server dekhta "pehle aaya?",
haan to skip, pehli wali id lautao.

Ye kyun chalta? Id app banata, server sirf "pehle dekha?" poochta, ek kadam. Dobara bhejna safe: wahi id = wahi
message.

**Pooche: dono ek saath pahunche?** Dekhna + daalna ek kadam me (Cassandra LWT / Redis SET NX).

**Pooche: do user ki id same nikli?** Check jodi pe (sender_id, clientMsgId). Alag log = alag message.

**Pooche: clientMsgId kab tak yaad rakhein?** Jitni der me dobara aata. Retry minute me, 1 ghanta kaafi.

---

*ab ek baar hi aaya, par kram galat*

## 6. Meri ghadi 5 min aage, mere message upar chadh gaye

Pehle kram phone ki ghadi se. Meri ghadi 10:05 (asal 10:00), mera message Ravi se upar chadh gaya. Kram ulta.

Isliye har chat ka server pe +1 number (Redis INCR seq:chat123). Chat ka kram isi se (seq), har chat alag.

Ye kyun chalta? Kram server ka number tay karta, ghadi nahi. Meri ghadi 5 min aage ho, farak nahi. Aur ginti me
chhed dikhe to pata chalta kuch chhoota, wo maang lo.

Phone ki ghadi? Wahi hua, kram ulta.

Snowflake id se kram? Wo sirf pehchaan (message_id). Kram nahi, do server ki ghadi me ms ka farak.

**Pooche: app ko 4415 mila, phir 4417?** 4416 chhoota (chhed). Thoda ruko, phir maango "4415 ke baad ke do".

**Pooche: seq wala Redis restart ho gaya?** Cassandra me har message ka seq hai. Sabse bade se aage.

**Pooche: Ravi wapas aaya?** "Meri aakhri 4417", to 4418 se aage sab.

---

*ab 1-to-1 theek, group me 500 log*

## 7. College Gang (500) me ek message, 500 baar likhein?

Pehle har member ke inbox me copy. Ek message, 500 likhai.

Isliye group ka message ek baar likho, group chat ke neeche. Sab wahi padhte. Bhejna server-wise: online kahan hain
Redis se dekho, har server ko ek call (server-2 pe 120 member, server-5 pe 80).

Ye kyun chalta? Sabke liye ek jaisa = ek copy. Har kisi ka alag = alag copy. Aur bhejna server-wise, member-wise nahi.

Har member ke inbox me copy? Wahi hua, 500 likhai.

**Pooche: 300 online hain?** Redis se unke server, har server ko ek call.

**Pooche: offline members?** Push ghanti. Wapas aaye to seq se aage padh lenge.

**Pooche: naya member juda?** Judne ke seq se pehle wale nahi milte. Tab wo group me tha hi nahi.

---

*ab message pahunch gaye, tick aur unread?*

## 8. Ravi ne 50 padhe: unread aur neeli tick kaise

Pehle har message x har member ka "padha" record. 50 message x 500 member = 25,000 likhai. Bhaari.

Isliye har member ka ek number: kahan tak padha (read_upto). Ravi ne 50 padhe, nishaan khiska: read_upto = 4467.
Ek likhai.

Ye kyun chalta? Nishaan khisakta hai, ginti nahi hoti. Unread, tick, offline ke baad lena, sab isi ek number se.
Khoya to agli baar.

Har message pe "padha" record? Wahi 25,000 likhai.

**Pooche: unread kaise?** Aakhri seq - nishaan. Ghatao, gino nahi.

**Pooche: neeli tick?** Mujhe ek push "read_upto 4467", 50 ek saath neeli.

**Pooche: do tick (mila)?** "Kahan tak mila" (delivered_upto), Ravi ka phone bhejta.

**Pooche: wo "mila" raaste me kho gaya?** Agli baar judte hi phone dobara bhejta. Nishaan sirf aage badhta.

**Pooche: group me do tick?** Sabse chhota nishaan: sab ka delivered_upto >= seq.

**Pooche: 10 lakh ka broadcast group?** Tick band, sirf unread. 10 lakh nishaan milana mehnga.

---

*ab text theek, par video bhaari*

## 9. Shaadi ka 200 MB video chat ke raaste

Pehle video chat server se. 200 MB pe server 2 min atka, aur group me 500 x 200 MB = 100 GB.

Isliye app video seedha S3 me daalta, 10 min ka paas leke (presigned URL). Message me sirf S3 key + thumbnail.

Ye kyun chalta? Bhaari maal S3 me, message halka. Ek upload, group me 500 ko wahi key. Message me key, URL nahi,
kyunki URL marta hai.

Chat server ke raaste? Wahi hua: server atka, 100 GB.

**Pooche: 200 MB me net chala gaya?** Tukdon me upload (multipart). Bacha hissa hi dobara.

**Pooche: Ravi ne khola?** Key se naya chhota URL. URL marta hai, key nahi.

**Pooche: wahi video 1000 baar forward hua?** File ka hash. Pehle se hai to wahi file, upload nahi.

**Pooche: E2E hai to thumbnail?** App khud encrypt kare, chaabi message ke saath. Server dekh nahi sakta.

---

*ab aakhri: kaun online hai*

## 10. Raat ko online / last seen sabko kaise

Pehle har aana-jaana saare contacts ko batate the. 500 contacts x crore log, bahut message.

Isliye online = Redis key + umar (30 sec). App har 15 sec dhadkan deta "hoon". Status dekho tabhi jab chat khule.

Ye kyun chalta? Status maango tab dekho, aur key khud mit jaati. Thoda purana status chalta, paisa nahi hai.

Sabko har badlav batao? Wahi hua, bahut message.

**Pooche: chat khuli hai aur main chala gaya?** Khuli chat = subscribe (live badlav). Baaki chat pull.

**Pooche: crore dhadkan har second?** Chat server ikattha karke Redis me (batch).

**Pooche: app band?** Dhadkan band, umar khatam = offline. Server us waqt last seen alag likhta.

**Pooche: Ravi ne last seen chhupaya?** Mat dikhao.

**Pooche: "typing..." kaise?** Sirf khuli chat me, store nahi. 2 sec ki khabar.

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

---

ARCHETYPE D (real-time) · CONCEPTS: [message-queues](../../FOUNDATIONS/07_message_queues.md) · [sharding](../../FOUNDATIONS/06_database_sharding.md) · [Snowflake id](../../FOUNDATIONS/13_distributed_id_snowflake.md) · [CAP](../../FOUNDATIONS/08_cap_theorem.md) · [← SYSTEM_DESIGNS](..) · [← Home README](../../../README.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
