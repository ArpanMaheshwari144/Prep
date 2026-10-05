# Chat / Messaging System

> A ne message bheja -> B tak TURANT, chahe B ne kuch na maanga ho · offline ho to baad me mile · kuch na khoye.
> Is design ka dil: **"banda KAHAN hai"** (baaki designs me "data kahan rakhein") + **khuli connection**.
> 21-Sep: ye padh ke nahi bana — ek chhota server aur do browser tab chala ke bana. Jahan aankh se dekha, wahan **★ DEKHA:**

```
JAD KI BAAT — chat baaki har design se KYUN alag:
   ab tak (url shortener, feed, payment, banking): user ne MAANGA -> server ne diya -> connection BAND
   chat: A ne bheja -> B ne kuch nahi maanga -> phir bhi B tak TURANT
   HTTP me server sirf JAWAB de sakta, AWAAZ nahi -> do nayi cheez:
   1. CONNECTION KHULI rakhni (WebSocket — ek baar judo, dono taraf baat)
   2. REGISTER "kaun kahan juda hai" (10 chat server, B kis se?) -> yahi chat ka DIL
```

---

## TASVEER (ByteByteGo / Alex Xu · CC BY-NC-ND 4.0)

![What is the Journey of a Slack Message?](https://assets.bytebytego.com/diagrams/0338-slack-message-journey.jpg)
Source: [What is the Journey of a Slack Message?](https://bytebytego.com/guides/what-is-the-journey-of-a-slack-message/)
(ek message sender se receiver tak — WebSocket, server, fan-out)

![Short/long polling, SSE, WebSocket](https://assets.bytebytego.com/diagrams/0337-short-long-polling-sse-websocket.jpeg)
Source: [Short/long polling, SSE, WebSocket](https://bytebytego.com/guides/shortlong-polling-sse-websocket/)
(chat me WebSocket kyun — polling / long polling / SSE se farak)

---

## SHURU — poocho + numbers

```
POOCHO (6, inme 2 poora design badalte):
   1. 1-to-1 ya GROUP? (group = ek message, 500 pahunchai)
   2. ★ HISTORY kahan, kitni der? WhatsApp = phone pe, pahunchte hi server se DELETE (server = DAAK-GHAR)
                                  Slack = server pe HAMESHA, 3 saal purana bhi search
                                  -> ye EK jawab storage 100 GB se 100 TB banata
   3. ★ END-TO-END encryption? haan -> server PADH hi nahi sakta -> server search khatam · naya member
        purane message nahi dekh sakta · key ka poora system — sabse bada scope-changer
   4. RECEIPT? ek tick (server) · do tick (phone) · neeli (padh li) · online / last-seen
      har tick ek ULTA message -> traffic KAI GUNA
   5. MEDIA? message ke saath nahi, blob store me, message me sirf PATA
   6. OFFLINE banda? message kahan ruke, kab tak, push notification?

MAAN KE CHALO (bol ke): 1-to-1 + chhote group (500 tak) · history SERVER pe (Slack model, design zyada dikhta)
                        · E2E abhi NAHI ("scope se bahar") · receipts sent / delivered / read

FR:      1-to-1 · group 500 · offline ko baad me · purani history · tick sent / delivered / read · online / last-seen
         scope bahar: E2E · voice / video · bade broadcast group
NFR:     TURANT (<1 sec) · KABHI na khoye · KRAM na bigde (ek chat ke andar) · DUPLICATE na dikhe
         · 2 crore EK SAATH jude · server gire to doosra sambhale

NUMBERS: 50 crore user, 10 crore roz · 40 msg / din -> ~400 crore / din
         WRITE 4 x 10^9 / 86400 ~ 46,000 / sec (peak 2-3x ~1.5 lakh) · READ catch-up + history, tick guna karte
         ONLINE ek waqt ~2 CRORE KHULI CONNECTION  <- ★ asli paimana ("kitni request / sec" nahi,
                "kitni connection KHULI" — har khuli connection memory khaati, message aaye ya na aaye)
         STORAGE 4 x 10^9 x ~300 B ~ 1.2 TB / din -> 1 saal ~440 TB (Slack) · WhatsApp model ~ZERO

TEEN FAISLE:
   1. CONNECTION ka apna TIER: 2 crore / ~1 lakh per box = ~200 CHAT SERVER (sirf taar pakde)
      -> API / business server se ALAG · event loop (Netty type), "ek connection = ek thread" nahi
   2. STORAGE ek box ka NAHI (banking ka ULTA: 120 / sec vs 46,000 / sec + 1.2 TB roz) — DB dikkat 4 me
   3. HISTORY MODEL storage ka dhaancha tay karta (sawaal 2) — yahan SLACK model

DO BLOCK JO DESIGN CHALATE:
   CONNECTION (khuli) = har online user ka taar, isi raaste server user tak
   REGISTER (kaun kis taar ke peeche) = MEMORY me, DB me nahi (taar khud memory me)
```

---

## DABBA 0 — sabse simple: EK server, do user

```
SOLUTION: A /connect?user=A (BAND NAHI hoti) -> register A -> penA · B /connect -> B -> penB
          A /send?to=B&text=hi (normal request) -> register.get("B") -> penB me likho -> B ko "A: hi"
          `pen` DATA nahi, B tak PAHUNCHNE KA RAASTA · message B ke PURANE khule taar se gaya · B ne kabhi nahi poocha
★ DEKHA:  B ki screen pe message aa gaya, B ne kuch maanga hi nahi. Screen pe "JUD GAYA — ye connection ab khuli padi hai",
          console pe `[JUDA] B   register ab = [A, B]`
```
```
  USER A / B
    │
    ▼
  [ Chat Server ]
```

---

## DIKKAT 1 — 2 crore connection KHULI, har ek memory kha rahi

```
DIKKAT:   har connection: file descriptor (OS khaata, ginti limited) + socket ke DO buffer (kernel) + app object
          ~10-50 KB · 1 lakh x ~30 KB = ~3 GB sirf HAATH PAKADNE me · 2 crore / 1 lakh = ~200 server

SOLUTION: (a) THREAD model badlo: event loop (Netty type), kuch thread, laakhon connection
          (b) SERVER GIRA -> 1 lakh ek saath wapas -> baaki pe jhatka + register dobara
              -> client reconnect me BACKOFF + JITTER (recovery spike: FOUNDATIONS/14_jab_ilaaj_hi_bimari_bane.md)
          (c) DEPLOY dard: REST restart = kuch retry · chat restart = 1 lakh disconnect
              -> thode-thode server, connection pehle hatao (DRAINING)
          aage LB connection ko kisi chat server pe bithaye
★ DEKHA:  server ki ginti jaan-boojh ke TEEN rakhi. Do khuli connection ne do jagah pakdi, teesri bhi bhari —
          send request server tak PAHUNCHI HI NAHI. Console pe `[JUDA]` teen, `[SEND]` EK BHI nahi.
          Na error, na crash, CPU khaali. Bas jagah khatam. Khuli connection bina kuch kiye jagah gherti — dekha.

NAYA:     LB
BADLA:    Chat Server -> Chat Server x ~200 (event loop)
```
```
  USER A / B
    │
    ▼
  [ LB ]
    │
    ▼
  [ Chat Server x ~200 ]
```
```
POOCHEGA: "What happens if a chat server goes down?"
BOL:      "Its connections drop and clients reconnect to another server with backoff and jitter, so they don't
           stampede. No message is lost — it was written to the database before delivery — and on reconnect the
           client asks for everything after its last message id."
```

---

## DIKKAT 2 — A server-1 pe, B server-7 pe: server-1 ki diary me B hai hi nahi

```
DIKKAT:   server-1 diary {A -> penA} · server-7 {B -> penB} · A ka message server-1 -> "B OFFLINE"
          jabki B ONLINE hai, bas doosre server pe · har server ko sirf apne judne walon ka pata
★ DEKHA:  wahi program do port pe. A pehle pe, B doosre pe. A ki screen `(server bola: B OFFLINE hai)` —
          B us waqt bilkul online, connection khuli. Ek diary me `[A]`, doosri me `[B]`.

SOLUTION: teen raaste — ek bekaar, DO asli:
          1. SAB SE POOCHHO (har message pe 199 sawaal) -> KHARIJ (bolo aur khud kharij karo — dikhta hai socha)
          2. SAANJHI DIARY (Redis presence / routing): B -> server-7 (sirf PATA, pen NAHI)
             server-1 Redis se "B kahan?" -> server-7 -> seedhi call -> server-7 local diary se penB
             pen KABHI Redis me nahi (zinda taar, sirf usi server ki memory)
             server-7 mara, entry padi -> TTL + dhadkan
          3. PUB-SUB (Redis pub-sub / Kafka): server-7 channel "user-B" sunta · server-1 us channel pe daalta
             faayda: bhejne wale ko pata hi nahi chahiye B kahan · nuksaan: Redis pub-sub bhej ke BHOOLTA
             (koi na sun raha = gaya) -> OFFLINE iske bharose nahi
          ASLI = MEL: kaun kahan -> Redis routing · server se server -> seedhi call / pub-sub · offline -> DB + push

NAYA:     Redis (presence / routing + pub-sub)
```
```
  USER A / B
    │
    ▼
  [ LB ]
    │
    ▼
  [ Chat Server x ~200 ] ──► [ Redis ]
```

---

## DIKKAT 3 — B offline: pen hi nahi, message jaaye kahan?

```
DIKKAT:   GALAT soch: pehle B ko bhejo, na pahunche to DB
          pen hona sanyog hai, message khona chalega hi nahi

SOLUTION: SAHI: pehle DB me LIKHO, PHIR bhejne ki koshish
          /send: 1. ID do + DB me LIKHO (yahan tak PAKKA)
                 2. B juda -> us server tak, pen me · nahi juda -> DB me pada + phone pe GHANTI (push notification)
          offline me "kho gaya" hota hi nahi — likha ja chuka, uthaya nahi gaya
          B WAPAS: "mere paas aakhri id 4417, uske BAAD ka do" -> 4418 se aage · NISHAAN CLIENT rakhta
                   (Kafka hands-on wala offset — "padhne wala apna nishaan khud rakhta")
          B poochhta kab: JUDTE waqt EK BAAR (catch-up) · judne ke baad CHUP, server taar me daalta
                   polling: har 5 sec "kuch aaya?" = 100 request, 1 kaam ki · chat: 1 request, phir jitne message
          PUSH ALAG RAASTA: pen = tera server -> app (app khula ho) · ghanti = tera server -> Google / Apple -> phone OS
                   app band, phone jeb me -> ghanti phir bhi (OS se jaata)

NAYA:     Message store · Push (Google / Apple)
```
```
  USER A / B
    │
    ▼
  [ LB ]
    │
    ▼
  [ Chat Server x ~200 ] ──► [ Redis ]
    │         │
    │         └──► [ Push (Google / Apple) ]
    ▼
  [ Message store ]
```
```
POOCHEGA: "How do you make sure no message is lost?"
DHYAAN:   beech me Kafka ho: producer acks=all · offset kaam ke BAAD · message id se idempotent · fail -> DLQ
BOL:      "I write the message to the database first and only then try to deliver it. If the receiver is offline
           it simply waits there and a push notification goes out; when they reconnect they ask for everything
           after their last message id."
```

---

## DIKKAT 4 — 1.2 TB roz aur 46,000 write / sec: ek DB box nahi

```
DIKKAT:   kis cheez se baantein?

SOLUTION: padhne ka tareeka batata: chat me sirf "is chat ke aakhri 50 do" / "uske pehle 50" — join, report, search nahi
          PARTITION KEY = chat_id (kis dabbe) · SORT KEY = message_id (dabbe ke andar kram) -> "aakhri 50" EK disk read
          user_id se baante to ek baatcheet A aur B DO jagah — chat dono ki saanjhi, chat ka apna dabba
          MESSAGE ID: auto-increment = ek counter, 200 server, bottleneck · SNOWFLAKE = har server khud, time se bada
                      (FOUNDATIONS/13_distributed_id_snowflake.md) · ek ID = kram + cursor
                      WHERE chat_id = ? AND id < 4417 ORDER BY id DESC LIMIT 50 (keyset; OFFSET 200000 ghatak, banking wali baat)
          KAUNSA DB: likhai bahut, padhai saadi · partition key pe SORTED · node jodo -> likhai jhele -> WIDE-COLUMN (Cassandra / Scylla)
                     relational kyun NAHI (banking me kyun THA): banking = multi-row atomicity + balance >= 0 constraint
                     chat = ek message ek row, kisi se lena-dena nahi, constraint nahi · 1.2 TB roz table ka index har insert pe
          PURANA: WhatsApp model = pahunchte hi DELETE (storage ~zero) · Slack = hamesha, COLD storage me khiskao
                  (isliye ye sawaal SHURU me) · retention != sharding (size vs abhi ka write load)

BADLA:    Message store -> Cassandra (chat_id partition, message_id sort)
NAYA:     Cold storage
```
```
  USER A / B
    │
    ▼
  [ LB ]
    │
    ▼
  [ Chat Server x ~200 ] ──► [ Redis ]
    │         │
    │         └──► [ Push (Google / Apple) ]
    ▼
  [ Cassandra messages ] ──► [ Cold storage ]
```
```
POOCHEGA: "The database is too big / takes too many writes. What do you do?"
BOL:      "Wide-column store partitioned by chat id and sorted by a snowflake message id, so the last fifty messages
           of a chat are one read and writes spread across nodes. Old data moves to cold storage."

POOCHEGA: "What about a hot partition — one very active group?"
DHYAAN:   chat_id + MAHINA sirf partition ka SIZE baandhta, abhi ka BOJH nahi (is mahine sab ek dabbe me)
          consistent hashing ek hot key ko nahi bachata
BOL:      "I'd add a random bucket to the key, chat id plus 0 to 9, and merge on read, or cap group size and rate."

POOCHEGA: "Data keeps growing — what happens in 3 years?"
BOL:      "It depends on the model we picked: WhatsApp-style deletes after delivery; Slack-style keeps everything,
           so older months move to cold storage."
```

---

## DIKKAT 5 — 500 ka group: ek message, likhai 500 baar?

```
DIKKAT:   har member ke inbox me copy?

SOLUTION: A. HAR MEMBER KE INBOX ME COPY (fan-out on write) -> 500 row, padhna sasta = TWITTER wala tareeka
          B. EK HI COPY chat_id ke neeche (fan-out on read) -> 1 row, sab wahi padhein   <- CHAT ME B
          twitter se ULTA kyun: (1) twitter feed = 200 logon ki MILAWAT, pehle banani padti · chat ka group sabke liye
                                 EK JAISA, copy se kuch nahi milta · (2) twitter 10 crore follower, chat 500
          MESSAGE ek baar likho · BHEJNA = register me har member, uske raaste me (500 socket write saste, DB write EK)

NAYA:     koi dabba nahi
```

---

## DIKKAT 6 — 500 member: kiske kitne unread, kahan rakhoge?

```
DIKKAT:   har member ki apni haalat

SOLUTION: A. HAR MESSAGE x HAR MEMBER record ("4417 -> Arpan delivered, Suresh read ...")
             500 x 50 = 25,000 record = 500-guna likhai PEECHE DARWAZE se wapas
          B. HAR MEMBER EK NISHAAN (cursor): "Arpan is group me 4417 tak padh chuka" -> 500 row
             unread = 4417 ke BAAD kitne (sasta, message id ke kram me)   <- YAHI
          offline wali baat hi: padhne wala apna nishaan khud rakhta
          ★ tick ka maamla yahin: "sabko mila" / "sabne padha" ke liye A wala (per message per member) chahiye

NAYA:     Cursor store
```
```
  USER A / B
    │
    ▼
  [ LB ]
    │
    ▼
  [ Chat Server x ~200 ] ──► [ Redis ]
    │         │
    │         ├──► [ Cursor store ]
    │         └──► [ Push (Google / Apple) ]
    ▼
  [ Cassandra messages ] ──► [ Cold storage ]
```

---

## DIKKAT 7 — do tick aur neeli tick: server ko pata kaise?

```
DIKKAT:   ek tick "server tak" = /send ka JAWAB, sasta
          do tick "B ke PHONE tak" = server ko KHUD kabhi pata nahi -> B ka app bole "mil gaya"
          neeli "B ne PADHA" = app tab bole jab chat SCREEN pe khuli
          EK message = CHHE kaam: A->server · server->B · B->server "mil gaya" · server->A "delivered"
                                  · B->server "padh liya" · server->A "read" (receipt traffic kai guna)

SOLUTION: wahi CURSOR: GALAT = har message ka delivered / read flag (50 aaye = 50 ack, 50 push)
          SAHI = per chat per banda DO number: delivered_upto = 4417 · read_upto = 4410
          B ne chat kholi, 50 padhe -> EK baat "read_upto 4467" -> A ko EK push -> 50 neeli ek saath
          ★ teen tick + group unread + offline catch-up = EK hi cheez: ek nishaan aage khiskta

NAYA:     koi dabba nahi — Cursor store
```

---

## DIKKAT 8 — 10 lakh ka broadcast group: tick ka kya?

```
DIKKAT:   per-member record 500 pe chalta, 10 lakh pe nahi

SOLUTION: WhatsApp: group me tick, par size BANDHA (~1000) · do tick = sabko mila, neeli = sabne padha
                    (bade group me neeli dikhti hi nahi) · "Info" me har banda alag = mehnga record SACH me rakhte
          Channel / broadcast: tick BAND
          Slack: per-message receipt HAI HI NAHI, sirf per-channel unread (ek cursor)
          ★ jawab chaturai nahi: feature utna rakho jitna scale jhele, scale badhe to feature HATAO (SEEMA lagao)

NAYA:     koi dabba nahi
```

---

## DIKKAT 9 — A ka net slow, app ne dobara bheja, B ko EK baat DO baar

```
DIKKAT:   5 sec timeout -> DOBARA bheja, pehla pahunch chuka -> do message, do id -> duplicate
          wahi shakal jo payment me (06_payment_system dikkat 2)

SOLUTION: IDEMPOTENCY KEY = chat me clientMsgId: { chatId, text, clientMsgId: "a7f3-91" }
          server: pehle aayi? haan -> naya MAT, purana wapas · nahi -> naya
          ★ ID CLIENT KYUN BANATA: RETRY bhi client karta. server banata to har retry pe NAYI id -> duplicate rukta hi nahi
            "jo dobara bhej raha, usi ko pehchaan deni hogi"
          "pehle aayi?" + insert ek ATOMIC step (UNIQUE constraint / Redis SET key NX EX)

NAYA:     Idempotency check
```
```
  USER A / B
    │
    ▼
  [ LB ]
    │
    ▼
  [ Chat Server x ~200 ] ──► [ Redis ]
    │         │
    │         ├──► [ Cursor store ]
    │         └──► [ Push (Google / Apple) ]
    ▼
  [ Idempotency check ]
    │
    ▼
  [ Cassandra messages ] ──► [ Cold storage ]
```
```
POOCHEGA: "What if the client retries and sends the same message twice?"
BOL:      "The client generates a clientMsgId and reuses it on retry; the server claims it atomically and returns
           the existing message instead of creating a second one."
```

---

## DIKKAT 10 — kram kis se tay hoga, kiska time maanoge?

```
DIKKAT:   DO alag sawaal (log milate): (1) EK chat ke andar kram (A ke teen, isi kram me)
                                        (2) DO logon ke beech (A aur B saath — kiska pehle?)
          (2) asal me sawaal hi nahi: jo server pe pehle pahuncha — manmaana, par sabko EK dikhta, itna kaafi

SOLUTION: (1) pehle se hal (dikkat 4): snowflake id time se badhti + ek chat ek partition -> ek jagah, ek kram
          ★ JAAL: kram CLIENT ke time se MAT: phone ki ghadi galat · koi time aage kare -> hamesha UPAR chipke
             kram SERVER ki ID se · client time sirf DIKHANE ("10:42 AM")
          ULTE KRAM me pahunche (retry se teesra pehle) -> B ka app ID se lagaye, aane se nahi
          beech ka GAYAB pakadna -> snowflake se nahi (gap normal) -> har chat ka SEQ (+1): 15, 17 aaya, 16 nahi -> catch-up
          beech me Kafka -> KEY = chatId (ek chat ek partition, order pakka; alag chats parallel; global = ek partition)
          ★ "Ye to WhatsApp me hota hai" (Arpan ne dekha: doosra pehle chala gaya) — jaan-boojh ke:
             strict kram = pehla atke to doosra bhi roko = HEAD-OF-LINE BLOCKING (Kafka wali)
             chuna: kram thoda idhar-udhar, par message ruke nahi (1-2 sec insaan ko chalta)

NAYA:     koi dabba nahi
```
```
POOCHEGA: "How do you keep messages in order?"
BOL:      "Order comes from a server-assigned id, never the client clock. One chat lives in one partition, and with
           Kafka I key by chat id, so a chat stays in order while different chats run in parallel. A per-chat
           sequence number lets the client spot a gap and catch up."
```

---

## DIKKAT 11 — 5 MB ka video bhi isi raaste se?

```
DIKKAT:   chat server ke through -> connection + thread block · 200 server ka tier jo CHHOTE message ke liye, BYTES dho raha

SOLUTION: client PEHLE blob store (S3) me, phir message me sirf PATA: { type: image, url, size, thumbnail }
          upload = 07_file_upload jaisa PRE-SIGNED URL (bytes app server ko chhute nahi)
          download = CHHOTI UMAR ka pre-signed URL (link aage bhejne se kaam na chale — 07_file_upload dikkat 8)
          THUMBNAIL client banata, kuch KB, message ke SAATH -> B ko TURANT kuch dikhe
          GROUP: file EK baar, 500 ko wahi EK pata (ek copy, kai pahunchai)

NAYA:     Blob store (S3)
```
```
  USER A / B
    │
    ├──► [ Blob store (S3) ]
    │
    ▼
  [ LB ]
    │
    ▼
  [ Chat Server x ~200 ] ──► [ Redis ]
    │         │
    │         ├──► [ Cursor store ]
    │         └──► [ Push (Google / Apple) ]
    ▼
  [ Idempotency check ]
    │
    ▼
  [ Cassandra messages ] ──► [ Cold storage ]
```

---

## DIKKAT 12 — online / last-seen: sabko sabka status

```
DIKKAT:   sabse mehnga "chhota" feature: message kabhi-kabhi, status HAR WAQT badalta, aur sabke contacts ko
          NAIVE: har connect / disconnect pe saare contacts ko -> 2 crore x 500 = bakwaas

SOLUTION: 1. TTL SE APNE AAP MARNA: Redis presence:B = online, TTL 30 sec · app har 15 sec heartbeat -> refresh
             app band / net gaya -> heartbeat ruka -> TTL khatam -> APNE AAP offline
             "offline" ka message bhejna hi NAHI — CHUP ho jaana hi signal (design ka sabse saaf hissa)
          2. POOCHO, BATAO MAT (pull): A ko B ka status tabhi jab B ki chat KHULI -> sirf unhi ka maange
          LAST SEEN = wahi key ka aakhri update time, alag rakha
          last-seen CHHUPANE ka option = SETTING: chhupaya hai to data hote hue bhi nahi dikhana (privacy alag layer)

NAYA:     koi dabba nahi — Redis presence
```

---

## 10x SCALE — har dabba alag

```
Chat Server   -> 2 crore khuli = ~200 box sirf haath pakadne · event loop, warna thread khatam
REGISTER      -> memory me, server gira to gaya -> Redis TTL entry + dhadkan
RECONNECT     -> ek server gira = 1 lakh ek saath -> backoff + jitter · deploy pe draining
Cassandra     -> chat_id partition · hot group -> bucket (chat_id + 0..9) / had · purana cold storage
Push          -> Google / Apple bahar ki cheez -> apna retry / queue
Redis         -> replica, ALAG AZ · message store bhi replica, alag AZ

POOCHEGA: "How would you scale this to 10x?"      -> user ka raasta chalo, pehle jo toote
POOCHEGA: "What's the single point of failure?"   -> chat server (reconnect), Redis (replica)
POOCHEGA: "How do you know it's working?"         -> delivery p99 · open connections per box · reconnect rate · push failures · alert
```

---

## POOCHE TO (deep-dive)

```
API:      GET /connect (BAND nahi hoti — WebSocket / SSE) · POST /messages { chatId, text, clientMsgId }
          GET /messages?chatId=&before= (catch-up + history, cursor) · POST /messages/{id}/read (receipt)

DO DUNIYA:  ZINDA TAAR (memory, ek server ki, mar sakti) = register / pen
            PAKKA MAAL (DB, sab jagah se dikhta) = message
            jo pen ko Redis me rakhne ki koshish kare — ho hi nahi sakta
            ek user -> KAI pen (phone + laptop + tab) — ★ dekha
```

---

## AAKHRI DABBA + WRAP

```
LB = connection ko chat server pe bithaye · Chat Server = khuli connection + local register (pen), event loop
Redis = kaun kis server (TTL + dhadkan) + pub-sub + presence · Push = app band pe ghanti (phone OS se)
Idempotency = clientMsgId · Cassandra = chat_id / message_id, pehle LIKHO phir bhejo · Cold storage = purana
Cursor store = delivered_upto / read_upto · Blob store = media, message me sirf pata
```
```
  USER A / B
    │
    ├──► [ Blob store (S3) ]
    │
    ▼
  [ LB ]
    │
    ▼
  [ Chat Server x ~200 ] ──► [ Redis ]
    │         │
    │         ├──► [ Cursor store ]
    │         └──► [ Push (Google / Apple) ]
    ▼
  [ Idempotency check ]
    │
    ▼
  [ Cassandra messages ] ──► [ Cold storage ]
```
```
BOL: "Each user holds an open WebSocket to one of ~200 chat servers, and Redis records which server each user is on.
      A message is deduped on clientMsgId, written first to Cassandra partitioned by chat id with a snowflake id,
      then routed to the receiver's server; if they're offline it waits and a push notification goes out, and on
      reconnect they ask for everything after their last id. Receipts and unread counts are just cursors per user per
      chat, presence is a TTL key kept alive by heartbeats, and media goes to S3 with only a link in the message."
```

[← SYSTEM_DESIGNS](..) · [← Home README](../../../README.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
