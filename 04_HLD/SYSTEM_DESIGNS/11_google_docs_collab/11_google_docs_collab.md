# Google Docs (Collaborative Editor)

> 2 ya zyada log ek doc EK SAATH edit karein -> kisi ka likha na mite, kuch kho na jaaye, turant dikhe, aakhir me sabke paas doc SAME.
> Is design ka dil: **ek saath edit + koi likha khoye nahi + aakhir me sab same (CONVERGENCE)**.
> 3 khaas cheez: har hisse ka alag CAP · WebSocket + Redis pub/sub · OT / CRDT.

---

## TASVEER (ByteByteGo / Alex Xu · CC BY-NC-ND 4.0)

![How to Design Google Docs](https://assets.bytebytego.com/diagrams/0206-google-doc.png)
Source: [How to Design Google Docs](https://bytebytego.com/guides/how-to-design-google-docs/)
(WebSocket + collaboration service + OT / CRDT + ops log — poora design ek tasveer me)

---

## SHURU: pehle poocho, phir numbers

```
POOCHO:  "Editing, comments, sharing / permission, version history, offline — main ek saath EDITING pe."
         ek doc pe ek saath kitne log? (2-3 ya 100) · OFFLINE
         chahiye?  <- iska jawab poora CAP faisla tay karta
         permission scope me? · rich text ya plain?

KYA KARE (FR):    kai log EK doc saath edit · doosron ka likha TURANT dikhe · kisi ka likha na mite
                  scope bahar: comments · version history · formatting
KAISA CHALE (NFR): typing TURANT (dil) · typing KABHI na ruke (offline bhi)
                   aakhir me sabka doc same — "turant sab same"
                   (strong) se NAHI, "aakhir me same" (convergence) se

NUMBERS (andaaza):
   ~10 crore user · ~3-4 crore roz · har akshar ek event -> LIKHNA bahut zyada
   -> beech me buffer + baantna (sharding) chahiye (pakka number yahan maayne nahi rakhta)
```

---

## ════ MOCK SE PEHLE: SIRF YE CARD PADHO ════

> Har card: DIKKAT 1 (kya toota) -> HAL (kya kiya) -> diagram -> DIKKAT 2, 3 ... (interviewer ke
> agle sawaal) har ek ke neeche uska HAL. Revise: sirf baayein column padho.

#### DABBA 0: SABSE SIMPLE (yahan se shuru, phir "kahan tootega")
```
Doc ka poora text DB me. "Save" dabao -> poora text upar likh do (overwrite).
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_App["App"]
    n_DB["DB"]
    n_USER --> n_App
    n_App --> n_DB
```

#### 1 DONO NE SAATH SAVE KIYA, BAAD WALE NE PEHLE KA MITA DIYA
```
DIKKAT 1   Doc me "HELLO" tha. Arpan ne "HELLO WORLD" karke save kiya, usi waqt Ravi ne "HELLO THERE".
           Ravi ka save baad me pahuncha, usne poora text upar likh diya -> Arpan ka "WORLD" gayab (jo baad
           me aaya wo jeeta = LAST-WRITE-WINS).
HAL        Poora doc bhej ke save karo to baad wala pehle wale ka likha mita deta.
           Isliye sirf ye bhejo ki kya badla: 'position 5 pe WORLD daala'. Saath me 'maine
           doc ka version 7 dekh ke likha'.
           Arpan aur Ravi dono ke badlaav lag jaate, kisi ka likha nahi mitta.
           op = { docId, userId, type (insert / delete), position, char, baseVersion }
           baseVersion = Arpan ne doc ka kaunsa version dekh ke ye op banaya.
           Arpan aur Ravi dono ne v7 dekh ke op banaya -> server ko pata ye do op ek saath
           (concurrent) bane -> OT khiska ke dono lagata.
```
```mermaid
flowchart TD
    n_USER["USER"]
    n_App["App<br/>operation bhejo, text nahi"]
    n_DB["DB"]
    n_USER --> n_App
    n_App --> n_DB
```
```
DIKKAT 2   Arpan ne "L" mitaya, Ravi ne usi jagah type kiya?
HAL        Mitana bhi ek op hai. OT dono ko adjust karta: Ravi ka akshar sahi jagah khisakta,
           Arpan sahi "L" mitata.
```

#### 2 RAVI KO ARPAN KA LIKHA KAB DIKHEGA?
```
DIKKAT 1   Normal HTTP me server khud kuch nahi bhej sakta, sirf jawab de sakta. To Ravi refresh kare
           tab Arpan ka likha dikhe? Bekaar.
HAL        Har user ka server se ek connection khula rehta (WebSocket). Arpan likhe to turant server tak,
           aur Ravi ka likha turant Arpan tak.
           Connection toota aur wapas juda to app bolta 'main version 120 tak tha', server 121 se
           aage ke saare badlaav bhej deta.
           Pehle ek HTTP request "Upgrade: websocket" -> server "101
           Switching Protocols" -> wahi connection khula rehta, ab dono taraf chhote-chhote message.
```
```mermaid
flowchart TD
    n_USER_A_B["USER A / B"]
    n_Conn_Server["Conn-Server"]
    n_DB["DB"]
    n_USER_A_B --> n_Conn_Server
    n_Conn_Server --> n_DB
```
```
DIKKAT 2   Connection beech me toot gaya?
HAL        Dobara judo (ruk-ruk ke) + bolo "main v120 tak tha" -> server v121 se aage ke saare
           op bhej deta. Kuch nahi chhoota.

DIKKAT 3   Arpan abhi bhi hai, kaise pata?
HAL        Har ~30 sec ping / pong. Jawab nahi = connection band, jagah khaali.

DIKKAT 4   Baar-baar poochhna (polling) ya SSE kyun nahi?
HAL        Polling = crore log x har second bekaar sawaal + 1 sec der. SSE sirf server ->
           user, yahan dono taraf chahiye.
```

#### 3 ARPAN SERVER-1 PE, RAVI SERVER-2 PE
```
DIKKAT 1   Ek connection server itne connection nahi jhelta, to kai lagaye.
           Arpan server-1 se juda, Ravi server-2 se, dono ek hi doc pe.
           Arpan ne type kiya -> server-1 ko pata, par server-2 ko kaise pata chale?
           Server ek doosre ko jaante hi nahi.
HAL        Arpan server-1 pe, Ravi server-2 pe. Server-1 Arpan ka badlaav Redis pe 'doc:123' channel me daal
           deta, server-2 us channel ko sun raha hai, to Ravi tak pahuncha deta.
           Redis ye kahin save nahi karta, sirf aage bhejta. Kuch chhoot gaya to
           dobara judne pe log se mil jaata.
           Dhyaan: badlaav ka KRAM aur khiskana (OT) sirf doc ke EK owner server pe hota
           (docId se sab editor usi server pe).
           Pub/sub sirf aage pahunchata hai, faisla nahi karta.
           Har doc ek channel (doc:123). Jis server pe doc:123 ka koi user hai, wo SUBSCRIBE karta.
           Server-1 --publish doc:123--> Redis --> Server-2 --> Ravi
           Redis pub/sub kuch STORE nahi karta (bheja aur bhool gaya). Iska kaam sirf
           turant dikhana, data bachana nahi.
```
```mermaid
flowchart TD
    n_USER_A_B["USER A / B"]
    n_LB["LB"]
    n_Conn_Server_x_N_1["Conn-Server 1"]
    n_Conn_Server_x_N_2["Conn-Server 2"]
    n_Redis_pub_sub["Redis pub/sub"]
    n_DB["DB"]
    n_USER_A_B --> n_LB
    n_LB --> n_Conn_Server_x_N_1
    n_LB --> n_Conn_Server_x_N_2
    n_Conn_Server_x_N_1 --> n_Redis_pub_sub
    n_Conn_Server_x_N_2 --> n_Redis_pub_sub
    n_Conn_Server_x_N_1 --> n_DB
    n_Conn_Server_x_N_2 --> n_DB
```
```
DIKKAT 2   Us waqt server-2 restart ho raha tha, sun hi nahi raha?
HAL        Pub/sub ka message gaya. Par op log me likha hai: Ravi dobara jude to version bata ke baaki le le.

DIKKAT 3   Redis pub/sub hi gir gaya?
HAL        Copy pe switch (Sentinel). Kuch second live dikhna ruka, likha kuch nahi khoya (log me hai).

DIKKAT 4   Kafka kyun nahi?
HAL        Kafka store karta + thoda dheema. Live typing ko dobara padhna nahi chahiye, tezi chahiye.
```

#### 4 DONO NE EK SAATH POSITION 0 PE TYPE KIYA
```
DIKKAT 1   Doc "HELLO". Arpan ne position 0 pe "X" likha, usi pal Ravi ne position 0 pe "Y".
           Seedha lagaya -> Arpan ki screen pe "XHELLO" phir Y ->
           "YXHELLO"; Ravi ki pe "YHELLO" phir X -> "XYHELLO".
           Do alag doc ban gaye (diverge).
HAL        OT (Operational Transformation): doosre ka op aane pe usko apne op ke hisaab se khiska do.
           Dono taraf ek hi niyam -> dono ka doc same.
           (CRDT doosra raasta: har akshar ki apni unique id, kisi bhi kram me jodo, nateeja same.)
           Niyam (tie-break): same jagah pe dono ne likha to Arpan ka pehle.
           Arpan ki screen: XHELLO. Ravi ka "Y at 0" aaya -> Arpan ka X
                            pehle hai -> Y ko 1 pe khiskao -> "XYHELLO"
           Ravi ki screen:  YHELLO. Arpan ka "X at 0" aaya -> Arpan pehle -> X 0 pe hi -> "XYHELLO"
           Dono ke paas "XYHELLO". Same.
```
```mermaid
flowchart TD
    n_USER_A_B["USER A / B"]
    n_LB["LB"]
    n_Conn_Server_x_N_1["Conn-Server 1<br/>doc:123 ka owner + OT"]
    n_Conn_Server_x_N_2["Conn-Server 2"]
    n_Redis_pub_sub["Redis pub/sub"]
    n_DB["DB"]
    n_USER_A_B --> n_LB
    n_LB --> n_Conn_Server_x_N_1
    n_LB --> n_Conn_Server_x_N_2
    n_Conn_Server_x_N_1 --> n_Redis_pub_sub
    n_Conn_Server_x_N_2 --> n_Redis_pub_sub
    n_Conn_Server_x_N_1 --> n_DB
    n_Conn_Server_x_N_2 --> n_DB
```
```
DIKKAT 2   Khiskana kaun karta, app ya server?
HAL        Dono. Server KRAM tay karta (har op ko agla version number) + aaye op ko
           khiskata. App apne pending op ko.

DIKKAT 3   OT kyun, CRDT kyun nahi?
HAL        Beech me server hai (har op usi se guzarta) -> OT seedha baithta, Google Docs bhi OT.
           CRDT = bina server (offline-first, phone se phone). Keemat: har akshar ki id = memory zyada.

DIKKAT 4   Ek doc ka OT do server pe chala?
HAL        Kram toot gaya. Isliye ek doc ke saare op EK server pe (docId se chunna).
```

#### 5 HAR AKSHAR PE DB ME LIKHNA
```
DIKKAT 1   Crore log type kar rahe, har akshar pe ek DB write -> DB khatam.
HAL        Har akshar pe DB me likhna mehnga. Live dikhana to Redis se ho hi raha hai.
           Isliye badlaav pehle Redis me jama karo, har 1 second ek saath Cassandra me likh do. Ek doc ke
           saare badlaav ek jagah, server ke diye number ke kram me.
           Edit log: partition key = docId, uske andar kram = version (SERVER ka number).
           Timestamp se kram NAHI: ek hi millisecond pe do op = same key = ek
                                   ne doosre ko chupchaap mita diya, op gaya.
           Cassandra likhne me tez: pehle commit log (sirf neeche jodna) + RAM (memtable).
```
```mermaid
flowchart TD
    n_USER_A_B["USER A / B"]
    n_LB["LB"]
    n_Conn_Server_x_N_1["Conn-Server 1"]
    n_Conn_Server_x_N_2["Conn-Server 2"]
    n_Redis_pub_sub["Redis pub/sub"]
    n_Redis_buffer["Redis buffer"]
    n_Cassandra_edit_log["Cassandra edit log"]
    n_USER_A_B --> n_LB
    n_LB --> n_Conn_Server_x_N_1
    n_LB --> n_Conn_Server_x_N_2
    n_Conn_Server_x_N_1 --> n_Redis_pub_sub
    n_Conn_Server_x_N_2 --> n_Redis_pub_sub
    n_Conn_Server_x_N_1 --> n_Redis_buffer
    n_Conn_Server_x_N_2 --> n_Redis_buffer
    n_Redis_buffer --> n_Cassandra_edit_log
```
```
DIKKAT 2   Arpan ka op Redis buffer me tha, Redis gir gaya -> op gaya?
HAL        Redis ki copy async thi, poora nahi bachati. Ilaaj: Arpan ka app op apne paas rakhe jab tak
           server "mil gaya" (ACK) na bole, dobara jude to phir bheje (opId ke saath, server
           ke paas pehle se hai to skip).
           Server ACK tab deta jab op Redis buffer me + uski copy me likh gaya (Redis WAIT 1, ~1 ms).
           Cassandra ka 1 second wala batch ACK ke raaste me nahi -> typing turant.

DIKKAT 3   Redis gira, ACK ho chuka tha?
HAL        Op copy me bhi likha tha, copy main ban gayi, op bacha. ACK se pehle gira -> app dobara bhejta.

DIKKAT 4   "Sahi hona chahiye to SQL lo"?
HAL        Nahi. DB data ki shakal se chuno: rishte (join) nahi, bas bahut saara jodna -> Cassandra.
```

#### 6 DOC KHOLA, 10 LAKH OP SHURU SE CHALANE PADE
```
DIKKAT 1   Arpan ne 2 saal purana doc khola. Server ne pehle op se 10 lakh op dobara chalaye tab
           doc bana -> kholne me bahut der.
HAL        2 saal purana doc kholne pe 10 lakh badlaav shuru se chalana dheema.
           Isliye har 1000 badlaav pe doc ki photo le lo. Kholte waqt aakhri
           photo + uske baad ke thode badlaav.
           Snapshot table: (docId, version 5000, poora text) -> S3 / Cassandra me.
           Arpan ne doc khola -> aakhri photo (v5000) + edit log se "version > 5000" wale op (thode se)
           -> unhe kram se lagao -> doc taiyaar. 10 lakh ki jagah ~1000 op.
```
```mermaid
flowchart TD
    n_USER_A_B["USER A / B"]
    n_LB["LB"]
    n_Conn_Server_x_N_1["Conn-Server 1"]
    n_Conn_Server_x_N_2["Conn-Server 2"]
    n_Redis_pub_sub["Redis pub/sub"]
    n_Redis_buffer["Redis buffer"]
    n_Cassandra_edit_log["Cassandra edit log<br/>+ snapshot"]
    n_USER_A_B --> n_LB
    n_LB --> n_Conn_Server_x_N_1
    n_LB --> n_Conn_Server_x_N_2
    n_Conn_Server_x_N_1 --> n_Redis_pub_sub
    n_Conn_Server_x_N_2 --> n_Redis_pub_sub
    n_Conn_Server_x_N_1 --> n_Redis_buffer
    n_Conn_Server_x_N_2 --> n_Redis_buffer
    n_Redis_buffer --> n_Cassandra_edit_log
```
```
DIKKAT 2   Photo lete waqt koi type kar raha?
HAL        Photo ek version tak (v1000). Kholna = v1000 ki photo + v1001 se aage ke op. Kuch nahi rukta.

DIKKAT 3   Kitni baar photo?
HAL        Har 1000 op ya har kuch minute, jo pehle ho.
```

#### 7 NETWORK EK SECOND GAYA, TYPING RUKEGI? (CAP)
```
DIKKAT 1   Arpan ka network 1 second ke liye gaya. Typing ruk jaaye, ya wo type karta rahe?
HAL        Likhne ke liye: network jaaye to bhi type karte raho, baad me sab mil
           jaayega aur doc same ho jaayega.
           Permission ke liye ulta: kisi ka access hataya to turant rokna hai, chahe kuch der system ruke.
           "Doc sahi chahiye -> CP" bolna galat: CP me network jaate hi typing ruk jaati.
           Yahan sahi hona "aakhir me sab same" se aata, typing rok ke nahi.
```
```mermaid
flowchart TD
    n_USER_A_B["USER A / B"]
    n_LB["LB"]
    n_Conn_Server_x_N_1["Conn-Server 1"]
    n_Conn_Server_x_N_2["Conn-Server 2"]
    n_Redis_pub_sub["Redis pub/sub"]
    n_SQL_permissions["SQL permissions"]
    n_Redis_buffer["Redis buffer"]
    n_Cassandra_edit_log["Cassandra edit log"]
    n_USER_A_B --> n_LB
    n_LB --> n_Conn_Server_x_N_1
    n_LB --> n_Conn_Server_x_N_2
    n_Conn_Server_x_N_1 --> n_Redis_pub_sub
    n_Conn_Server_x_N_2 --> n_Redis_pub_sub
    n_Conn_Server_x_N_1 --> n_Redis_buffer
    n_Conn_Server_x_N_2 --> n_Redis_buffer
    n_Conn_Server_x_N_1 --> n_SQL_permissions
    n_Conn_Server_x_N_2 --> n_SQL_permissions
    n_Redis_buffer --> n_Cassandra_edit_log
```
```
DIKKAT 2   Arpan ne 1 ghanta offline type kiya?
HAL        App ke paas pending op + aakhri version. Wapas aate hi bhejo, server khiska ke lagaye, sab same.

DIKKAT 3   Arpan ka access hata diya, aur wo offline type kar raha tha?
HAL        Dobara jude to pehle permission check (SQL, CP). Access nahi -> pending op reject.
```

#### 8 CRORE WEBSOCKET CONNECTION
```
DIKKAT 1   Crore connection khule. Ek doc ke 5 editor alag-alag 5 server pe bikhre -> har server
           apna OT chala raha -> kram toota.
HAL        Alag CONNECTION server ki line. docId se baanto: ek doc ke saare editor + uska OT EK hi server pe
           (wo us doc ka OWNER). Wahi har op ko version number deta, to kram kabhi do jagah tay nahi hota.
           Sirf dekhne wale (likh nahi rahe) doosre server pe ho sakte, unhe Redis pub/sub se badlaav.
           URL me docId -> LB consistent hashing RING se server chunta. Server juda / gira
           -> sirf uske hisse ke doc hilte.
           (hash(docId) % N NAHI: N badla to lagbhag har doc ka server badla = sab dobara jude.)
           Ek doc pe ~100 editor tak ek server jhel leta. App ki static files CDN se.
```
```mermaid
flowchart TD
    n_USER_A_B["USER A / B"]
    n_CDN["CDN"]
    n_LB["LB"]
    n_Conn_Server_x_N_1["Conn-Server 1"]
    n_Conn_Server_x_N_2["Conn-Server 2"]
    n_Redis_pub_sub["Redis pub/sub"]
    n_SQL_permissions["SQL permissions"]
    n_Redis_buffer["Redis buffer"]
    n_Cassandra_edit_log["Cassandra edit log"]
    n_USER_A_B --> n_CDN
    n_USER_A_B --> n_LB
    n_LB --> n_Conn_Server_x_N_1
    n_LB --> n_Conn_Server_x_N_2
    n_Conn_Server_x_N_1 --> n_Redis_pub_sub
    n_Conn_Server_x_N_2 --> n_Redis_pub_sub
    n_Conn_Server_x_N_1 --> n_Redis_buffer
    n_Conn_Server_x_N_2 --> n_Redis_buffer
    n_Conn_Server_x_N_1 --> n_SQL_permissions
    n_Conn_Server_x_N_2 --> n_SQL_permissions
    n_Redis_buffer --> n_Cassandra_edit_log
```
```
DIKKAT 2   Doc wala server gir gaya?
HAL        Ring pe agla server le leta. Sab dobara jude, photo + op se doc bana, version bata ke baaki op.

DIKKAT 3   Likhe hue ka kram kaun tay kare?
HAL        SERVER: har op ko doc ka agla version number. App ki ghadi kabhi nahi.

DIKKAT 4   Ek doc pe 1000 log?
HAL        Jo sirf dekh rahe (likh nahi rahe) unhe alag rakho, pub/sub se updates.
```

```
AAKHRI DABBE (sab dikkat ke baad):
  CDN (app ki files) -> LB (docId se same server = doc ka owner) -> Conn-Server (WebSocket + OT + version)
  -> editors ko seedha PUSH · Redis pub/sub sirf un dekhne walon tak jo doosre server pe hain
  -> Redis buffer -> BATCH -> Cassandra op log (docId, version) + snapshot · permission = SQL

DB:  op log = Cassandra (bahut saara jodna; Mongo bhi chalega) · snapshot taaki doc jaldi khule
     permission = alag SQL (rishte + CP) · edit AP (typing kabhi na ruke)

END ME POOCHEGA:
  "Do log ek jagah type?" -> OT: ek saath bane op ko khiskao, sab same · (CRDT = har akshar ki unique id)
  "Kram kaun tay kare?"   -> server version deta; timestamp sirf dikhane ke liye
  "10x?"            -> alag connection line · Conn-Server docId se baanto · Redis copy
                       · DB buffer + batch · doc kholna = snapshot + baad ke op
  "SPOF?"           -> Conn-Server (doosre pe dobara judo), Redis (copy)
  "Chal raha, kaise pata?" -> likha doosre tak kitni der me
  pahuncha (p99) · WebSocket toot-ne ki dar · buffer lag · alert
```

AAKHRI DABBE ki tasveer:

```mermaid
flowchart TD
    n_USER_A_B["USER A / B"]
    n_CDN["CDN"]
    n_LB["LB"]
    n_Conn_Server_x_N_1["Conn-Server 1"]
    n_Conn_Server_x_N_2["Conn-Server 2"]
    n_Redis_pub_sub["Redis pub/sub"]
    n_SQL_permissions["SQL permissions"]
    n_Redis_buffer["Redis buffer"]
    n_Cassandra_edit_log["Cassandra edit log"]
    n_USER_A_B --> n_CDN
    n_USER_A_B --> n_LB
    n_LB --> n_Conn_Server_x_N_1
    n_LB --> n_Conn_Server_x_N_2
    n_Conn_Server_x_N_1 --> n_Redis_pub_sub
    n_Conn_Server_x_N_2 --> n_Redis_pub_sub
    n_Conn_Server_x_N_1 --> n_Redis_buffer
    n_Conn_Server_x_N_2 --> n_Redis_buffer
    n_Conn_Server_x_N_1 --> n_SQL_permissions
    n_Conn_Server_x_N_2 --> n_SQL_permissions
    n_Redis_buffer --> n_Cassandra_edit_log
```

---

## 10 GUNA TRAFFIC — har dabba alag se

```
WebSocket      -> crore connection -> alag CONNECTION LINE + docId se same server
Conn-Server    -> OT kahan? -> docId se baanto (ek doc = ek jagah)
Redis pub/sub  -> bheed -> copy + aur machine
DB likhna      -> har akshar? -> buffer + batch, docId se baanto
doc kholna     -> 10 lakh op? -> SNAPSHOT + baad ke op

POOCHEGA: "How would you scale this to 10x?"      -> user ka raasta chalo, pehle jo toote
POOCHEGA: "What's the single point of failure?"   -> Conn-Server (doosre pe dobara judo), Redis (copy)
POOCHEGA: "How do you know it's working?"         -> likha pahunchne
          ka p99 · WebSocket toot-ne ki dar · buffer lag · alert
```

---

## POOCHE TO (gehra sawaal aaye tab)

```
API:      GET /documents/{docId} -> snapshot + baad ke op · POST /documents/{docId}/edits -> ek op
          WebSocket /documents/{docId} -> live, dono taraf

DATA:     har badlaav = OPERATION: { docId, userId, opType
          (insert / delete), position, char / text, baseVersion }
          server har op ko agla version deta (kram); timestamp sirf dikhane ke liye
          abhi ka doc = us doc ke saare op KRAM se lagao
          Cassandra theek (bahut saara jodna) · Mongo bhi chalega · permission = alag SQL (rishte + CP)

EK AKSHAR KA SAFAR:
          1. Arpan ne akshar likha -> op { docId, userId, insert, pos, char, baseVersion }
          2. WebSocket se doc ke owner Conn-Server tak (Ravi bhi usi pe)
          3. OT: ek saath bane op ke hisaab se khiskao -> agla version number -> lagao
          4. Ravi ko seedha PUSH · doosre server ke dekhne walon ko Redis pub/sub se
          5. Redis buffer -> BATCH -> Cassandra edit log
          6. Ravi ke app ne op liya -> apni taraf khiskaya -> screen update

OT vs CRDT: OT = op bhejo, ek saath wale ko KHISKAO, sab same · CRDT
            = har akshar ki unique id, kisi bhi kram me jodo same
```

---

## AAKHRI DABBA + WRAP

```
CDN = app ki files · LB = docId se same server · Conn-Server = WebSocket + OT + buffer
Redis pub/sub = server se server · Redis buffer = batch
likhna · Cassandra = op log (docId, version) + snapshot
SQL = permission (CP)
```
```
JAWAB:    Har user doc ke hisaab se ek server se juda (ek doc ke sab log ek server pe). Har akshar ek
          badlaav.
          Server ek saath wale badlaav khiska ke lagata, kisi ka likha nahi khota, sabka doc same.
          Doosre servers tak Redis se.
          Badlaav jama karke Cassandra me, photo ke saath taaki doc jaldi khule. Likhna kabhi na ruke,
          permission turant lage (SQL).
```
```mermaid
flowchart TD
    n_USER_A_B["USER A / B"]
    n_CDN["CDN"]
    n_LB["LB"]
    n_Conn_Server_x_N_1["Conn-Server 1"]
    n_Conn_Server_x_N_2["Conn-Server 2"]
    n_Redis_pub_sub["Redis pub/sub"]
    n_SQL_permissions["SQL permissions"]
    n_Redis_buffer["Redis buffer"]
    n_Cassandra_edit_log["Cassandra edit log"]
    n_USER_A_B --> n_CDN
    n_USER_A_B --> n_LB
    n_LB --> n_Conn_Server_x_N_1
    n_LB --> n_Conn_Server_x_N_2
    n_Conn_Server_x_N_1 --> n_Redis_pub_sub
    n_Conn_Server_x_N_2 --> n_Redis_pub_sub
    n_Conn_Server_x_N_1 --> n_Redis_buffer
    n_Conn_Server_x_N_2 --> n_Redis_buffer
    n_Conn_Server_x_N_1 --> n_SQL_permissions
    n_Conn_Server_x_N_2 --> n_SQL_permissions
    n_Redis_buffer --> n_Cassandra_edit_log
```

ARCHETYPE D (real-time) · CONCEPTS: [CAP](../../FOUNDATIONS/08_cap_theorem.md) · [pubsub/queues](../../FOUNDATIONS/07_message_queues.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
