# Chat / Messaging System — POORA ROUND (4 MOVE, jaise asli me hota hai)

> **21-Sep.** Ye design baaki sab se ek buniyaadi cheez me alag hai, aur wahi is poore page ki jad hai.
> Isi liye ye padh ke nahi, **chala ke** banaya gaya — ek chhota server likh ke, do browser tab khol ke,
> aur teen asli bug khaa ke. Hands-on ka poora hissa neeche hai, aur uska code
> [`07_PROJECTS/chatdemo/`](../../../07_PROJECTS/chatdemo/) me pada hai.

---

## ★★ JAD KI BAAT — chat baaki har design se KYUN alag hai

```
AB TAK HAR DESIGN (url shortener, feed, payment, banking):
      user ne MAANGA  ->  server ne diya  ->  connection BAND

CHAT:
      A ne message bheja  ->  B ne kuch nahi maanga
                          ->  phir bhi B tak TURANT pahunchana hai
```

B baitha hua hai, usne koi request nahi bheji. Server ko **khud** uske paas jaana hai.
HTTP ka poora dhaancha isi ek baat pe toot jaata hai — HTTP me server sirf **jawab** de sakta hai,
**awaaz** nahi de sakta.

Isse do cheezein paida hoti hain jo aur kisi design me nahi thi:

```
1. CONNECTION KHULI RAKHNI PADEGI
      B ka phone server se juda rahe, chahe wo kuch na maange
      (WebSocket -- ek baar judo, dono taraf se baat chalti rahe)

2. EK REGISTER RAKHNA PADEGA -- "kaun kahan juda hai"
      10 chat-server hain, B kis se juda hai?
      A ka message us SAHI server tak pahunchna chahiye
      -> yahi chat design ka DIL hai
```

> ★ **Ek line me:** baaki designs me pehla sawaal hota hai *"data kahan rakhein"*.
> Chat me pehla sawaal hota hai *"banda kahan hai"*.

---
---

# MOVE 1 — POOCHO (board pe abhi kuch nahi)

Chhe sawaal. In me se **do** aise hain jinka jawab badalte hi poora design badal jaata hai.

```
1. 1-to-1 hi, ya GROUP bhi?
      1-to-1  ->  ek message, ek banda
      group   ->  ek message, 500 bande  ->  ek likhai, 500 pahunchai

2. ★ HISTORY kahan rehti hai, aur kitni der?
      WhatsApp jaisa  ->  message PHONE pe rehta, pahunchte hi server se DELETE
                          server sirf ek DAAK-GHAR hai
      Slack jaisa     ->  server pe HAMESHA, 3 saal purana bhi search hota
      ★ ye EK jawab storage ko 100 GB se 100 TB bana deta hai

3. ★ END-TO-END encryption chahiye?
      haan ->  server message PADH hi nahi sakta
               ->  server-side search khatam
               ->  naya banda group me aaye to purane message DIKH hi nahi sakte
               ->  key ka apna poora system chahiye
      ye sabse bada scope-changer hai

4. RECEIPT kya chahiye?
      ek tick (server tak) . do tick (phone tak) . neeli (padh li) . online/last-seen
      har tick apne aap me ek ULTA message hai jo wapas jaata hai
      -> traffic asli message se KAI GUNA ho jaata hai

5. MEDIA bhejni hai?
      haan ->  photo/video message ke saath NAHI jaati
               alag se blob store me jaati hai, message me sirf uska PATA jaata hai

6. OFFLINE banda?
      uska message kahan ruke, kab tak, aur push notification bhejni hai ya nahi
```

**Jo maan ke aage badhna hai (aur ye BOL ke aage badhna hai):**

```
1-to-1 + chhote group (500 tak)
history SERVER pe rehti hai          <- Slack wala model (isme design zyada dikhta hai)
E2E encryption abhi NAHI             <- "scope se bahar rakh raha hoon" bol ke hata do
receipts: sent / delivered / read
```

**Ginti:**

```
50 crore user, 10 crore roz ke
ek user 40 message/din      ->  ~400 crore message/din
online EK WAQT me           ->  ~2 crore connection        <- ★ SABSE AHEM NUMBER
```

> ★ Wahi aakhri line chat ka asli paimana hai. Baaki designs me sawaal tha *"kitni request per second"*.
> Yahan sawaal hai **"ek waqt me kitni connection KHULI rahengi"** — kyunki har khuli connection
> server ki memory kha rahi hai, chahe uspe ek bhi message na aaye.

---
---

# MOVE 2 — DO CHHOTE BLOCK LIKHO

```
 ┌── FR (kya karega) ────────────────┐   ┌── NFR (kaisa hona chahiye) ──────────┐
 │  - 1-to-1 message bhejo           │   │  - TURANT pahunche (<1 sec)          │
 │  - chhota group (500 tak)         │   │  - message KABHI na khoye            │
 │  - offline banda: baad me mile     │   │  - KRAM na bigde (ek chat ke andar)  │
 │  - purani history padho           │   │  - DUPLICATE na dikhe (retry pe bhi) │
 │  - tick: sent/delivered/read      │   │  - 2 crore EK SAATH jude rahein      │
 │  - online / last-seen             │   │  - server gire to dusra sambhal le   │
 └────────────────────────────────┘   └───────────────────────────────────┘

 SCOPE SE BAHAR (bol ke hatao): E2E encryption . voice/video call . bade broadcast group
```

**DO BLOCK JO POORA DESIGN CHALATE HAIN:**

```
BLOCK 1 -- CONNECTION (khuli rehti hai)
   har online user ka ek taar server se juda rehta hai
   ye taar hi wo raasta hai jisse server user tak pahunchega

BLOCK 2 -- REGISTER (kaun kis taar ke peeche)
   user -> us user tak likhne ka raasta
   ye MEMORY me hai, DB me nahi -- kyunki taar khud memory me hai
```

Baaki sab (offline, history, group, tick) **inhi do ke upar** khada hota hai.

---

**NUMBER (bolo, faisla nikaalo, aage badho):**

```
   50 crore user, 10 crore roz ke
   ek user 40 message/din      ->  400 crore message/din

   WRITE   4 x 10^9 / 86400        ~=  46,000 write/sec     (peak 2-3x = ~1.5 lakh)
   READ    catch-up + history      ->  write se kam, par tick/receipt ise GUNA karte hain
   ONLINE  ek waqt me              ~=  2 crore KHULI CONNECTION      <- ★ asli paimana

   STORAGE  4 x 10^9 x ~300 byte   ~=  1.2 TB / din
            1 saal                 ~=  ~440 TB       (Slack model, sab rakho)
            WhatsApp model         ~=  lagbhag ZERO  (pahunchte hi delete)
```

**TEEN FAISLE JO IN NUMBERS SE SEEDHE NIKALTE HAIN:**

```
1. CONNECTION ka apna TIER chahiye
      2 crore connection / ~1 lakh per box  =  ~200 CHAT SERVER
      ye server kuch "kaam" nahi kar rahe -- sirf taar pakde baithe hain
      -> inhe API/business wale server se ALAG rakho
      -> aur inpe event-loop (Netty type), "ek connection = ek thread" chalega hi nahi

2. SHARDING LAZMI HAI (yahan banking se ULTA faisla)
      banking = 120 write/sec  ->  ek Postgres ka 10-va hissa  ->  sharding ki zaroorat NAHI
      chat    = 46,000 write/sec + 1.2 TB roz  ->  ek box ka sawaal hi nahi
      ★ yahi wajah hai ki DB ka chunav bhi ULTA jaata hai

3. HISTORY ka MODEL storage ka dhaancha tay karta hai
      MOVE 1 ka sawaal #2 yahan cash hota hai:
         WhatsApp model -> server lagbhag storage-free, DB chhota
         Slack model    -> 440 TB/saal + cold storage + archival
      is design me SLACK model maan ke chal rahe hain
```

**DB CHUNAV — WIDE-COLUMN (Cassandra / Scylla type). Teen wajah:**

```
   1. LIKHAI bahut, PADHAI bahut SAADI
         "is chat ke aakhri 50 message" -- bas yahi ek sawaal, baar-baar
         koi join nahi . koi report nahi . koi search nahi (E2E scope se bahar)

   2. PARTITION KEY pe data SORTED pada rehta
         chat_id ke andar message_id ke kram me
         -> "aakhri 50" EK disk read me

   3. LIKHAI me tez aur AAGE-BADHAANE layak
         node jodo -> aur likhai jhel lega (write path pe lock/constraint ka bojh nahi)
```

★ **Relational yahan kyun NAHI** (aur banking me kyun THA): banking me multi-row atomicity aur
`balance >= 0` jaisa constraint chahiye tha -- wo relational ka kaam hai. Chat me ek message ek
row hai, kisi doosri row se uska lena-dena nahi, aur koi constraint nahi -- sirf **bahut saari
likhai**. Jo cheez banking me relational ko zaroori banati thi, wo yahan hai hi nahi.

★ **Aur ek cheez jo relational me maar deti:** 1.2 TB roz ka ek hi `messages` table, jiska index
har insert pe update hota. Wide-column me likhai append jaisi hai.

---
---

# MOVE 3 — BOXES BANAO (chhota banao, phir dikkat pe badhao)

## Kadam 1 — EK server, do user (yahi chala ke dekha)

```
   A ka browser                SERVER                 B ka browser
        |                                                  |
        |---- /connect?user=A --------->  [ register ]      |
        |     (ye request BAND NAHI HOTI)   A -> penA       |
        |                                   B -> penB  <----| /connect?user=B
        |                                                  |
        |---- /send?to=B&text=hi ------>  register.get("B") |
        |     (ye NORMAL request hai,      penB me likh do  |
        |      jawab de ke band)                ----------->|  "A: hi"
```

**Dhyaan dene wali teen baatein:**

```
1. `pen` koi DATA nahi hai -- wo B TAK PAHUNCHNE KA RAASTA hai
2. A ka message B ki apni request se NAHI gaya --
   wo B ke us PURANE taar se gaya jo pehle se khula pada tha
3. B ne kabhi nahi poocha "mere liye kuch aaya?"
```

## Kadam 2 — dikkat: connection MUFT NAHI hoti

```
har ek khuli connection ke liye server ke paas hona chahiye:
   ek file descriptor      (OS ka khaata, ginti limited)
   socket ke DO buffer     (bhejne ka + aane ka, kernel memory)
   app ka apna object      (ye user kaun, kis chat me, auth)

motamoti ~10 se 50 KB per connection

1 lakh connection x ~30 KB = ~3 GB     <- sirf HAATH PAKDE RAKHNE me
2 crore online / 1 lakh    = ~200 server  <- jo kuch KAAM nahi kar rahe
```

Isse teen cheezein nikalti hain:

```
a. THREAD ka model badalna padta hai
      "ek connection = ek thread" yahan chalta hi nahi (1 lakh thread nahi banenge)
      -> event-loop (Netty type): kuch hi thread, laakhon connection pe nazar

b. SERVER GIRA to sab EK SAATH wapas aayenge
      1 lakh connection tooti -> 1 lakh app ne turant dobara judne ki koshish ki
      -> baaki servers pe jhatka, aur register dobara likhna pada
      -> ILAAJ: client ke reconnect me BACKOFF + JITTER
      (yahi recovery-spike hai -- FOUNDATIONS/14_jab_ilaaj_hi_bimari_bane.md)

c. DEPLOY dard ban jaata hai
      REST server restart = kuch retry, kisi ko pata nahi chalta
      chat server restart = 1 lakh log disconnect
      -> thode-thode server, aur connection pehle se hataao (draining)
```

## Kadam 3 — dikkat: DO SERVER ho gaye (yahi asli sawaal hai)

```
server-1 ki diary  =  { A -> penA }
server-7 ki diary  =  { B -> penB }

A ka message  ->  server-1 pe gira
                  server-1 ne APNI diary kholi -> usme B hai hi nahi
                  -> "B OFFLINE hai"

...jabki B bilkul ONLINE hai, bas KISI AUR server se juda hai
```

Server-1 jhooth nahi bol raha. **Har server ko sirf apne judne walon ka pata hai.**

### Iske TEEN raste — ek bekaar, DO asli

```
1. SAB SE POOCHH LO                                        <- KHARIJ
      server-1 baaki 199 se poochhe "B tumhare paas hai?"
      har message pe 199 sawaal -> bakwaas
      ★ interview me ye BOLO aur KHUD kharij karo -- dikhta hai ki socha hai

2. SAANJHI DIARY (Redis) -- routing / presence            <- CHALTA HAI
      Redis me:   B -> server-7        (sirf PATA, pen NAHI)
      A ka message server-1 pe aaya
         -> Redis se poochha "B kahan hai?"  -> "server-7"
         -> server-1 ne server-7 ko seedhi call ki
         -> server-7 ne apni local diary se penB uthaya aur likh diya

      ★ pen KABHI Redis me nahi jaata -- wo ek ZINDA taar hai,
        sirf usi server ki memory me reh sakta hai
      dikkat: server-7 mar gaya par Redis me entry padi rah gayi
              -> entry pe TTL + server se dhadkan

3. PUB-SUB (Redis pub-sub / Kafka)                         <- CHALTA HAI
      B server-7 se juda  ->  server-7 ne channel "user-B" SUNNA shuru kiya
      A ka message aaya   ->  server-1 ne channel "user-B" pe DAAL diya
                          ->  jo sun raha hai (server-7) use mil gaya

      faayda : bhejne wale ko jaanne ki zaroorat HI NAHI ki B kahan hai
      nuksaan: Redis pub-sub bhej ke BHOOL jaata hai -- us waqt koi sun na raha ho
               to message gaya. Isliye OFFLINE ka kaam iske bharose nahi.
```

**Asli system dono ka MEL hota hai:**

```
kaun kahan juda hai      ->  Redis (presence / routing table)
ek server se dusre tak   ->  seedhi call YA pub-sub channel
banda OFFLINE hai        ->  message DB/queue me + phone pe push notification
```

## Kadam 4 — OFFLINE banda (raasta hamesha ALAG hota hai)

Poora khel ek ulte-lagne wale faisle pe tika hai:

```
GALAT soch :  pehle B ko bhejo. Na pahunche to phir DB me rakho.
SAHI soch  :  pehle DB me LIKHO. PHIR bhejne ki koshish karo.
```

Kyunki pen hona ya na hona **sanyog** hai, par message kho jaana chalega hi nahi.

```
/send aaya
   1. message ko ID do aur DB me LIKH do          <- yahan tak PAKKA
   2. ab dekho B kahan hai
        juda hai    ->  us server tak bhejo, pen me likho
        juda nahi   ->  kuch mat karo, DB me pada hai
                        + uske phone pe GHANTI bajao (push notification)
```

Offline case me "message kho gaya" hota hi nahi. Wo likha ja chuka hai, bas uthaya nahi gaya.

### B wapas aaya — wo kya poochhta hai

```
B:  "mere paas aakhri id 4417 tak hai. Uske BAAD ka sab do."
```

Server DB se 4418 se aage sab de deta hai. **Nishaan client rakhta hai, server nahi.**

> ★ Ye bilkul wahi cheez hai jo Kafka hands-on me khud chala ke dekhi thi — **parchi kahan tak
> khiski hai**. Wahan offset, yahan last-message-id. Soch ek hi:
> *"padhne wala apna nishaan khud rakhta hai."*

### "B poochhta hai ya nahi" — isme uljhan hoti hai

```
JUDTE WAQT -- EK BAAR      B: "4417 ke baad ka do"   (catch-up, normal request)
JUDNE KE BAAD              B CHUP. server jo aaye wo taar me daalta rehta hai.
```

Farak polling se samjho:

```
polling   B har 5 sec poochhta "kuch aaya?" -> 99 baar "nahi", 1 baar "haan"
          = 100 request, 1 kaam ki

chat      B ek baar poochhta (catch-up), phir chup
          = 1 request, aur uske baad jitne message utne hi
```

★ Sahi line: **B sirf shuru me EK BAAR poochhta hai, baar-baar nahi.**

### Push notification ek ALAG raasta hai

```
pen wala raasta  ->  tera server  ->  seedha app tak          (app khula ho tabhi)
ghanti wala      ->  tera server  ->  Google/Apple  ->  phone ka OS  ->  GHANTI
```

App band ho, phone jeb me ho — ghanti phir bhi bajti hai, kyunki wo raasta tere server se nahi,
**phone ke OS** se jaata hai. Isi liye har chat system me ye dono raaste alag banaye jaate hain.

## Kadam 5 — HISTORY / STORAGE

```
400 crore message/din  x  ~300 byte  =  ~1.2 TB roz
```

Ek machine ka kaam nahi. Sawaal: **kis cheez ke hisaab se baantein?**

**Jawab padhne ke tareeke se nikalta hai.** Chat me sirf ek hi sawaal poochha jaata hai:

```
"is chat ke aakhri 50 message do"
"aur uske pehle wale 50 do"

koi join nahi . koi report nahi . koi search nahi (E2E scope se bahar rakha)
```

```
partition key  =  chat_id         <- kis dabbe me
sort key       =  message_id      <- us dabbe ke andar kis KRAM me

-> EK chat ke saare message EK jagah, time ke kram me
-> "aakhri 50" EK disk read me
```

user_id se baantte to ek hi baatcheet DO jagah tuk jaati — A ke dabbe me bhi, B ke bhi.
**Chat dono ki saanjhi hai, isliye chat ka apna dabba.**

### Message ID

```
auto-increment  ->  ek hi jagah se number  ->  200 server, ek kaunter  ->  bottleneck
Snowflake type  ->  har server KHUD bana le, phir bhi TIME ke hisaab se bada
                    (FOUNDATIONS/13_distributed_id_snowflake.md)
```

Is ek ID se do kaam hote hain — **kram** bhi aur **cursor** bhi:

```
WHERE chat_id = ? AND id < 4417 ORDER BY id DESC LIMIT 50      <- keyset
OFFSET 200000                                                   <- yahan bhi utna hi ghatak
                                                                   (banking design wali baat)
```

### Kaunsa DB

Wide-column (Cassandra / Scylla type) — **wajah MOVE 2 me likhi hai**, aur wo wajah in numbers se
nikli thi, kisi "NoSQL modern hai" wali baat se nahi.

### Hot partition

Ek bahut active group ka poora bojh ek hi partition pe. **Wahi hot-key wali baat jo cache me thi.**

```
partition key = chat_id + mahina      ->  ek chat bhi kai dabbon me bat gayi
```

### Purana data — yahan MOVE 1 ka sawaal wapas aata hai

```
WhatsApp model   pahunchte hi server se DELETE  ->  server storage lagbhag ZERO
                 (history phone pe, server = sirf daak-ghar)

Slack model      hamesha rakho, 3 saal purana bhi search
                 ->  1.2 TB roz wala hisaab
                 ->  purana data COLD storage me khiskao
```

★ Isi liye wo sawaal **shuru me** poochha jaata hai. Ek jawab se storage ka dhaancha badal jaata hai.

---
---

# ═══ HANDS-ON — CHAT KHUD CHALA KE DEKHA (21-Sep) ═══

> Code: [`07_PROJECTS/chatdemo/`](../../../07_PROJECTS/chatdemo/) —
> [`ChatServer.java`](../../../07_PROJECTS/chatdemo/ChatServer.java) (~150 line, koi library nahi)
> + [`index.html`](../../../07_PROJECTS/chatdemo/index.html) (do browser tab = A aur B)
>
> Chalana: `java ChatServer.java 8080` phir `localhost:8080/?me=A` aur `?me=B`

## Poora dil — teen tukde

```java
// 1. REGISTER
static ConcurrentHashMap<String, PrintWriter> register = new ConcurrentHashMap<>();

// 2. CONNECT -- connection kholo aur PAKDE RAHO
ex.getResponseHeaders().set("Content-Type", "text/event-stream");
ex.sendResponseHeaders(200, 0);                       // 0 = length pata nahi, band mat karo
PrintWriter pen = new PrintWriter(ex.getResponseBody(), true);

register.put(user, pen);
System.out.println("[JUDA] " + user + "   register ab = " + register.keySet());

while (true) {                                        // ye loop connection ZINDA rakhta hai
    Thread.sleep(15000);
    pen.print(": ping\n\n");
    pen.flush();
    if (pen.checkError()) throw new IOException("connection tooti");
}

// 3. SEND -- register me dhoondo aur THOOS do
PrintWriter pen = register.get(to);
if (pen != null) {
    pen.print("data: " + from + ": " + text + "\n\n");
    pen.flush();
    jawab = "bhej diya";
} else {
    jawab = to + " OFFLINE hai";
}
```

## ★ TEEN ASLI BUG JO IS DEMO ME MILE

### BUG 1 — thread khatam, aur server CHUP ho gaya

Thread pool jaan-boojh ke **3** rakha tha. Do khuli connection = do thread pakde hue.

```
log me:
[JUDA] A   register ab = [A]
[JUDA] A   register ab = [A]
[JUDA] B   register ab = [A, B]
                                 <- [SEND] ki EK BHI line nahi
```

`/send` request server tak **pahunchi hi nahi** — uske liye ek bhi thread khaali nahi bacha.
Server zinda, koi error nahi, CPU khaali. Bas thread khatam.

```
THREADS 3 -> 50 kiya, aur turant dono taraf chalne laga
```

> ★ "Connection muft nahi hoti" — ye padha nahi, **dekha** gaya.

### BUG 2 — marte hue ne ZINDA wale ko uda diya

```java
register.remove(user);      // <- pehli koshish
```

```
log me:
[JUDA] A   register ab = [A, B]
[JUDA] A   register ab = [A, B]     <- A ki NAYI connection ne apna pen rakha
[TOOTI] A  register ab = [B]        <- PURANI (mar chuki) connection ne "A" HATA diya
```

`remove(user)` ye dekhta hi nahi ki jo pada hai wo **mera** pen hai ya kisi aur ka.
Marte hue purane ne zinde naye ko maar diya, aur uske baad sab "OFFLINE" jaane laga.

```java
register.remove(user, pen);    // <- ilaaj: "tabhi hatao jab value abhi bhi MERA pen ho"
```

> ★ Do-value wala `remove` poora faisla map ke **andar, ek hi call me** karta hai.
> Ye bilkul wahi bimari thi jo ConcurrentHashMap hands-on me mili thi — **do alag call jahan
> ek honi chahiye thi**. (`01_JAVA/02_COLLECTIONS/02_hashmap_vs_concurrenthashmap.md`)

### BUG 3 — client ne "JUD GAYA" dikhaya, server ke register me naam tha hi nahi

A ki tab upar likh rahi thi *"JUD GAYA — ye connection ab khuli padi hai"*, jabki server ke
register me A kab ka ja chuka tha. **Client ko lagta hai juda hoon, server kehta hai koi nahi hai.**

> ★ Isi liye asli app me dhadkan **dono taraf** se chalti hai — server bhi poochhta rehta hai
> "zinda ho?" aur client bhi. Ek taraf ki khamoshi pakadni padti hai, warna user baitha rehta hai
> aur uske message kahin nahi jaate.

## ★ DO SERVER WALA PRAYOG — poori dikkat saamne

Ek hi program, do baar, do port pe. A ko 8080 pe rakha, B ko 8081 pe.

```
server-1 (8080)   register = [A]
                  [SEND] A -> B : Hello         <- message yahin aaya

server-2 (8081)   register = [B]                <- B yahan baitha hai
                                                   iske paas message aaya hi nahi
```

A ki screen pe: `(server bola: B OFFLINE hai)` — **aur B us waqt bilkul online tha.**

> ★ Yahi wo ek tasveer hai jo poore chat design ko samjha deti hai. Iske baad "Redis me routing"
> ya "pub-sub" ratt-ne wali baat nahi rehti — wo is dikkat ka seedha jawab ban jaate hain.

---
---

# MOVE 4 — BOLTE-BOLTE JODO

## ► "API kya hogi?"

```
GET   /connect                      <- ye BAND NAHI hoti (WebSocket / SSE)
POST  /messages                     { chatId, text, clientMsgId }
GET   /messages?chatId=&before=     <- catch-up + purani history (cursor)
POST  /messages/{id}/read           <- receipt
```

★ `clientMsgId` client banata hai — wahi duplicate rokta hai (network retry pe do baar na jaaye).

## ► "Kahan tootega / 10x traffic pe?"

```
CONNECTION ka bojh    2 crore khuli connection = ~200 server sirf haath pakadne ko
                      -> event-loop, warna thread khatam (ye demo me DEKHA)
REGISTER              memory me hai -> server gira to uska poora register gaya
                      -> Redis me TTL wali entry + dhadkan
RECONNECT ka toofan   ek server gira -> 1 lakh ek saath wapas
                      -> backoff + jitter (warna nayi laher)
HOT PARTITION         ek viral group ek hi partition pe -> chat_id + mahina
PUSH ka raasta        Google/Apple bahar ki cheez hai -> uska apna retry/queue
```

## ► WRAP (ek line har problem ki)

```
"User -> LB -> CHAT SERVER (khuli connection + local register)
 -> REDIS (kaun kis server pe) -> dusre server tak seedhi call ya pub-sub channel
 -> message pehle DB me (wide-column, chat_id ke hisaab se baanta, snowflake id)
 -> offline hua to DB me pada rehta hai + phone pe push notification
 -> wapas aaya to 'mere aakhri id ke baad ka do'."
```

---

## ★ IS DESIGN KA IMAANDAR HISAAB (21-Sep)

```
JO HO GAYA (chala ke, sirf padh ke nahi):
   push ka poora mechanism      khuli connection + register + pen
   connection ka kharcha        thread khatam hone tak dekha
   do server wali dikkat        do port pe chala ke saamne laayi
   uske teen raste              1 kharij + Redis routing + pub-sub
   offline ka raasta            pehle likho phir bhejo, catch-up, push notification
   storage                      chat_id partition, snowflake id, cursor, wide-column, cold

JO ABHI BAKI HAI:
   group fan-out                ek message 500 logon tak -- ek likhai ya 500
   tick                         sent / delivered / read -- teeno ka apna raasta
   kram aur duplicate           ek hi message do baar na dikhe
   media                        photo/video ka alag rasta (blob + pata)
   presence                     online / last seen
```

★ Imaandari: is design ka **teen-chauthai** hua hai. Jo bacha hai wo upar wale dhaanche ke **upar**
baithta hai — buniyaad khadi ho chuki hai.

---

[← SYSTEM_DESIGNS](..) · [← Home README](../../../README.md)
