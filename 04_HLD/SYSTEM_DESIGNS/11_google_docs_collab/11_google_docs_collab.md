# Google Docs (Collaborative Editor)

> 2 ya zyada log ek doc ek saath edit karein. Kisi ka likha na mite, kuch kho na jaaye, turant dikhe.
> Dil: ek saath edit, koi likha khoye nahi, aur aakhir me sabke paas doc same (convergence).
>
> Mock me sirf heading + Cue + poster. Poster me sab: upar "Pehle aisa tha", tasveer (ilaaj), hara "Kyun chalta", laal number = kya toot sakta + kaise bacha, peela = doosra tareeka kyun nahi, grey = baaki.
> Cross-Q: hal ke har bade shabd pe socho -> gira? · dheema / bheed? · do ek saath? · galat hua? · kaise / kitna?

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c8_2-a2085e82.svg)

---


## Shuru

```
FR      kai log ek doc saath edit karein, doosron ka likha turant dikhe, kisi ka likha na mite.
        Dhyaan sirf ek saath EDITING pe (comments, version history, formatting scope ke bahar)
NFR     typing turant ho aur kabhi na ruke (offline bhi). Aakhir me sabka doc same (convergence),
        "turant sab same" (strong) nahi
NUMBERS ~10 crore user, ~3-4 crore roz. Har akshar ek event, to likhna bahut zyada hai,
        isliye buffer + baantna (sharding). Pakka number yahan maayne nahi rakhta
PEHLE   sabse simple tha: poora text DB me rakho, aur Save pe poora text upar likh do (overwrite)
```

> [!IMPORTANT]
> **RAIL: har design isi kram me chalta (8 dabbe, sab file me same)**
> kaam chala → bheed aayi → machine giri → do log ek saath → kuch dheema → data bada → door ke log → bahar ka hamla
>
> | rail | is design me | card |
> |---|---|---|
> | 1 kaam chala | kai log saath edit, likha turant dikhe · Ravi ko refresh pe hi dikha | 2 |
> | 2 bheed aayi | 50 lakh connection, ek server nahi jhelta · Ravi viewer, doosre server pe | 4, 5 |
> | 3 machine giri | owner server gira (card 4 ka cross-Q) · metro me net gaya, typing rukegi? | 4, 7 |
> | 4 do log ek saath | Ravi ne baad me save kiya, mera WORLD mita · dono ne sabse upar ek saath type kiya (**is design ka khaas: ek saath edit, sabka doc same**) | 1, 3 |
> | 5 kuch dheema | 2 saal purana doc, 10 lakh op shuru se | 8 |
> | 6 data bada | har akshar ek op, har op pe DB likhai | 6 |
> | 7 door ke log | nahi aata | - |
> | 8 bahar ka hamla | offline me access hata diya (card 7 ka cross-Q) | 7 |


> [!NOTE]
> **AWS pe kaunsa DB (interview me naam bolna ho to)** · SQL wala = RDS, Cassandra RDS pe nahi chalta
>
> - Op log (Cassandra) → **Amazon Keyspaces** (ya EC2 pe khud): har akshar ek op, likhai bahut, (docId, version)
> - Permission (SQL) → **AWS RDS (Postgres / MySQL)**: access kabhi galat na ho (CP)
> - Buffer / pub-sub (Redis) → **ElastiCache (Redis)**: ACK tez, doosre server tak op

---

*ab tootega kahan? do log ek hi doc save karein to*

## 1. Ravi ne baad me save kiya, mera "WORLD" mit gaya

**Rail 4 DO LOG EK SAATH (save)** · Cue: OPERATION + baseVersion (text nahi)

Jod: Jira / Confluence comment me bhi version; wahan purana = save MANA, Docs khiska ke lagata

<img src="img/c1_poster.png?v=4" width="1000" alt="card 1: andar + kya gira, kaise bacha">


---

*ab badlaav bhej to diye, par Ravi ko dikhe kaise?*

## 2. Ravi ko mera likha refresh karne pe hi dikha

**Rail 1 KAAM CHALA** · Cue: WEBSOCKET (khula, dono taraf)

Jod: WhatsApp (net aate hi saare message ek saath) · Stock ka live price bhi WebSocket push (05 card 8)

<img src="img/c2_poster.png?v=4" width="1000" alt="card 2: andar + kya toota, kaise bacha">


---

*ab dono ek hi jagah ek saath likhein to?*

## 3. Arpan aur Ravi ne sabse upar ek saath type kiya

**Rail 4 DO LOG EK SAATH (khaas)** · Cue: OT (server kram, op khiskao)

Jod: Chat me bhi kram server ka +1 number (seq), phone ki ghadi nahi (14 card 6)

<img src="img/c3_poster.png?v=4" width="1000" alt="card 3: andar + kya toota, kaise bacha">


---

*ab OT ek server pe hota hai, par connection itne saare*

## 4. 50 lakh connection khule, ek connection server nahi jhelta

**Rail 2 BHEED AAYI (connection)** · Cue: RING: docId se ek OWNER server

Jod: URL shortener aur Cache me bhi ring (01 card 5, 10 card 2)

<img src="img/c4_poster.png?v=4" width="1000" alt="card 4: andar + kya toota, kaise bacha">


---

*ab viewer kahin bhi hai, use op pahunche kaise?*

## 5. Ravi ab sirf padh raha, wo doosre server pe

**Rail 2 BHEED AAYI (viewer)** · Cue: REDIS PUB/SUB (doc ek channel)

Jod: Stock ka price bhi pub/sub se har WebSocket server tak (05 card 8)

<img src="img/c5_poster.png?v=4" width="1000" alt="card 5: andar + kya toota, kaise bacha">


---

*ab sab live chal raha, par har akshar DB tak?*

## 6. 3 baje DB alert: har akshar pe DB me likha, DB baith gaya

**Rail 6 DATA BADA (likhai)** · Cue: REDIS BUFFER + BATCH, ACK copy ke baad

Jod: Cassandra = sirf aage jodna (09 databases) · URL me bhi ginti Kafka se, raaste se bahar (01 card 4)

<img src="img/c6_poster.png?v=4" width="1000" alt="card 6: andar + kya toota, kaise bacha">


---

*ab net hi chala gaya to?*

## 7. Metro me mera net 1 second gaya, typing rukegi?

**Rail 3 MACHINE GIRI (net gaya)** · Cue: likhna AP, permission CP

Jod: WhatsApp bhi offline me type karne deta (AP) · Banking me transfer / balance CP (13)

<img src="img/c7_poster.png?v=4" width="1000" alt="card 7: andar + kya toota, kaise bacha">


---

*ab doc purana aur lamba ho gaya*

## 8. Raat ko 2 saal purana doc khola, 10 lakh op shuru se chale

**Rail 5 KUCH DHEEMA (purana doc)** · Cue: SNAPSHOT + uske baad ke op

Jod: Banking ka balance bhi = mahine ka snapshot + baad ki line (13 card 11)

<img src="img/c8_poster.png?v=4" width="1000" alt="card 8: andar + kya toota, kaise bacha">


---

## Poora design ek saans me

```
CDN (app ki files) -> LB (editor: docId se owner server · viewer: koi bhi) -> Conn-Server (WebSocket + OT)
-> editors ko seedha PUSH · Redis pub/sub sirf un dekhne walon tak jo doosre server pe hain
-> Redis buffer (+ copy, phir ACK) -> BATCH -> Cassandra op log (docId, version) + snapshot
permission = alag SQL (CP) · edit AP (typing kabhi na ruke)
```

Bolne ke liye: har user doc ke hisaab se ek server se juda, ek doc ke sab editor ek server pe. Har akshar ek
badlaav. Server ek saath wale badlaav khiska ke lagata, kisi ka likha nahi khota, sabka doc same. Doosre servers
tak Redis se. Badlaav jama karke Cassandra me, photo ke saath taaki doc jaldi khule. Likhna kabhi na ruke,
permission turant lage (SQL).

---

## 10 guna traffic

- **WebSocket**: ~50 lakh connection. Alag connection line, aur docId se same server.
- **Conn-Server**: OT kahan? docId se baanto, ek doc = ek jagah.
- **Redis pub/sub**: bheed badhi to copy + aur machine.
- **DB likhna**: har akshar? Buffer + batch, docId se baanto.
- **Doc kholna**: 10 lakh op? Snapshot + baad ke op.

**Pooche: 10 guna kaise?** User ka raasta chalo, pehle jo toote wahi theek karo.

**Pooche: kahan se sab gir sakta?** Conn-Server (doosre pe dobara judo), Redis (copy).

**Pooche: chal raha hai, kaise pata?** Likha pahunchne ka p99, WebSocket toot-ne ki dar, buffer lag, alert.

---

## Pooche to (kisi card me nahi baitha)

API: GET /documents/{docId} -> snapshot + baad ke op. POST /documents/{docId}/edits -> ek op.
WebSocket /documents/{docId} -> live, dono taraf.

Op: { docId, userId, opType (insert / delete), position, char / text, baseVersion }. Server har op ko agla version
deta (kram). Timestamp sirf dikhane ke liye.

DB: op log Cassandra (bahut saara jodna, Mongo bhi chalega), snapshot taaki doc jaldi khule. Permission alag SQL
(rishte + CP).

Ek akshar ka safar: maine akshar likha, op bana. WebSocket se doc ke owner Conn-Server tak (Ravi bhi usi pe). OT ne
ek saath bane op ke hisaab se khiskaya, agla version diya, lagaya. Ravi ko seedha push, doosre server ke dekhne walon
ko Redis pub/sub se. Phir Redis buffer, batch, Cassandra edit log. Ravi ke app ne op liya, apni taraf khiskaya,
screen update.

App ki static files? CDN. JS / CSS sabke liye same, paas se aaye.

---

ARCHETYPE D (real-time) · CONCEPTS: [CAP](../../FOUNDATIONS/08_cap_theorem.md) · [pubsub/queues](../../FOUNDATIONS/07_message_queues.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
