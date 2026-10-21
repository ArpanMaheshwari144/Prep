# Google Docs (Collaborative Editor)

> 2 ya zyada log ek doc ek saath edit karein. Kisi ka likha na mite, kuch kho na jaaye, turant dikhe.
> Dil: ek saath edit, koi likha khoye nahi, aur aakhir me sabke paas doc same (convergence).
>
> Mock me sirf heading + Cue + kahani ki tasveer. Neeche har card: HAL · KYUN · NAHI, phir YAAD RAKH dabba + CROSS-Q dabba (sawaal → cue, poora jawab).
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

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c1_1.png?v=1" width="900">

<img src="img/c1_2.png?v=1" width="900">

<img src="img/c1_3.png?v=1" width="900">

</details>

```
HAL     sirf badlaav (op) bhejo, kis version pe bana (baseVersion v7). Server khiska ke lagata
KYUN    server ke paas text nahi aata, badlaav aate hain. Server op ko agla version deta, lagata,
        aur Ravi ko bhej deta. Dono op v7 pe bane, to server samjhta ki saath bane,
        aur dono khiska ke lagata hai: "HELLO WORLD THERE"
NAHI    lock lagao -> RUKNA: user ko lock ya intezaar nahi chahiye.
        Purane version pe bana op bhi khisak ke lag jaata hai
```

> [!IMPORTANT]
> **Yaad rakh:** Text nahi, sirf badlaav (op) + baseVersion. Server agla version deta, purane pe bana op khiska ke lagata.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - maine op se mitaya, aur Ravi ne wahin likha → **mitana bhi ek op hai**: Server dono khiska ke lagata, Ravi ka akshar sahi jagah aata
> - sirf op aate hain, to abhi ka doc kya hai? → **doc = op ka jod**: us doc ke saare op kram se, pehle se aakhri tak laga do. Doc = op ka jod


---

*ab badlaav bhej to diye, par Ravi ko dikhe kaise?*

## 2. Ravi ko mera likha refresh karne pe hi dikha

**Rail 1 KAAM CHALA** · Cue: WEBSOCKET (khula, dono taraf)

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c2_1.png?v=1" width="900">

<img src="img/c2_2.png?v=1" width="900">

<img src="img/c2_3.png?v=1" width="900">

</details>

```
HAL     connection khula rehta, dono taraf (WebSocket). Server khud bhejta, ~200 ms me dikhta
KYUN    ek hi khula connection dono taraf chalta hai. Server khud bhej deta, user ko refresh nahi karna padta
NAHI    HTTP -> EKTARFA: server khud kuch nahi bhej sakta.
        X server tak aaya, par Ravi ko refresh tak kuch nahi
```

> [!IMPORTANT]
> **Yaad rakh:** Khula connection, dono taraf (WebSocket). Server khud bhejta ~200 ms. Toota to "main v12 pe tha", baaki op.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - WebSocket connection chupchaap mar gaya → **ping / pong ~30 sec**: har ~30 second ping / pong hota hai. Jawab nahi aaya to connection band, jagah khaali
> - WebSocket connection toot gaya → **"main v12 pe tha"**: main likhta rehta hoon. Judte hi bolta "main v12 pe tha", server v13 se aage sab bhej deta


---

*ab dono ek hi jagah ek saath likhein to?*

## 3. 11 baje meeting me dono ne sabse upar ek saath type kiya

**Rail 4 DO LOG EK SAATH (khaas)** · Cue: OT (server kram, op khiskao)

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c3_1.png?v=1" width="900">

<img src="img/c3_2.png?v=1" width="900">

<img src="img/c3_3.png?v=1" width="900">

</details>

```
HAL     kram server tay karta (OT): X pehle v8, Y v9 aur 0 se 1 pe khiska. Dono ke paas XYHELLO
KYUN    kram sirf server ka hai, har op ko agla version milta, phone ki ghadi kabhi nahi.
        Aaya hua op us kram se khisakta hai, to sabki screen same rehti hai.
        Mujhe apna likha turant dikhta hai, baaki ka khisak ke aata hai
NAHI    jo aaya wahi laga do -> ALAG: meri screen "YX...", Ravi ki "XY...", do alag doc ban gaye
        kram timestamp se    -> TAKKAR: ek ms pe do op = same key, ek ne doosra chupchaap mita diya.
                                Isliye version
        CRDT                 -> FALTU: beech me server hai hi, har op usse guzarta.
                                OT yahi maangta (Docs bhi OT)
```

> [!IMPORTANT]
> **Yaad rakh:** Kram sirf server ka (OT): har op ko agla version, aaya op khisak ke lagta. Sabki screen same.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - CRDT hota kya hai? → **unique id (CRDT)**: har akshar ki unique id hoti, kisi bhi kram me jodo to nateeja same. Offline chalta, memory zyada. OT op ko khiskata hai, CRDT id se apne aap judta hai
> - OT ke beech Ravi ka op ~200 ms baad aaya, aur maine beech me likh diya tha → **pending op khiskao**: mera app apne pending op khiska ke uska op lagata hai
> - dono ne pos 0 pe likha, tie ho gaya → **jo pehle aaya wo 0 pe**: server pe jo pehle aaya wo 0 pe, doosra 1 pe khisakta. Dono ke paas "XYHELLO"


---

*ab OT ek server pe hota hai, par connection itne saare*

## 4. 50 lakh connection khule, ek connection server nahi jhelta

**Rail 2 BHEED AAYI (connection)** · Cue: RING: docId se ek OWNER server

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c4_1.png?v=1" width="900">

<img src="img/c4_2.png?v=1" width="900">

<img src="img/c4_3.png?v=1" width="900">

</details>

```
HAL     docId se ring pe server chuno. doc:123 ka owner server-1, dono editor wahin, OT ek jagah
KYUN    ek server ~1 lakh connection hi jhelta hai, isliye kai server chahiye.
        Ek doc = ek owner server, to OT ek hi jagah hota hai.
        Editor docId se owner pe aata hai, viewer kahin bhi jud sakta hai
NAHI    editor alag-alag server pe (main server-1, Ravi server-3) -> TOOTA: OT kaun kare?
        do server = do version, doc toot gaya
PEHLE   pehle poochho: ek doc pe ek saath kitne log hain, 2-3 ya 100?
```

> [!IMPORTANT]
> **Yaad rakh:** docId se ring pe owner server, ek doc ke sab editor wahin, OT ek jagah. Doc DB me, server gira to agla.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - ek doc pe 1000 log aa gaye → **editor owner pe, viewer kahin bhi**: 100 editor owner pe (OT sirf likhne walon ka), 900 viewer kahin bhi
> - ring me server juda ya gira → **sirf pados ke doc**: ring pe sirf uske pados ke doc khiskte hain, wo log dobara judte hain
> - owner server gira, to doc kahan gaya? → **doc DB me hai**: doc server pe nahi, DB me hai. Ring agla server deta, naya owner DB se doc banata. Editor dobara judte, version bata ke baaki op le lete. Doc nahi khota


---

*ab viewer kahin bhi hai, use op pahunche kaise?*

## 5. Ravi ab sirf padh raha, wo doosre server pe

**Rail 2 BHEED AAYI (viewer)** · Cue: REDIS PUB/SUB (doc ek channel)

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c5_1.png?v=1" width="900">

<img src="img/c5_2.png?v=1" width="900">

<img src="img/c5_3.png?v=1" width="900">

</details>

```
HAL     har doc ka ek channel (Redis pub/sub). Server-1 op daalta, server-2 sunta, Ravi ko bhejta
KYUN    jis server pe us doc ka user hai, wo channel sunta hai (subscriber).
        Iska kaam sirf turant pahunchana hai, kuch store nahi karta (bheja aur bhool gaya).
        Bachana DB ka kaam hai, channel ka nahi. Viewer ka kuch chhoot gaya to version bata ke DB se le lo
```

> [!IMPORTANT]
> **Yaad rakh:** Har doc ek Redis channel: owner daalta, doosre server sunte. Store nahi, chhoota to version bata ke DB se.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - editor same server pe hai → **seedha push**: pub/sub ki zaroorat hi nahi
> - Ravi ka server restart ho gaya → **store nahi, DB me hai**: op channel se chala gaya (store nahi), par DB me hai. Ravi judta, version bata ke baaki le leta
> - Redis pub/sub hi gir gaya → **copy main ban jaati hai (Sentinel)**: Live kuch second ruka, khoya kuch nahi


---

*ab sab live chal raha, par har akshar DB tak?*

## 6. 3 baje DB alert: har akshar pe DB me likha, DB baith gaya

**Rail 6 DATA BADA (likhai)** · Cue: REDIS BUFFER + BATCH, ACK = WAIT 1

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c6_1.png?v=1" width="900">

<img src="img/c6_2.png?v=1" width="900">

<img src="img/c6_3.png?v=1" width="900">

</details>

```
HAL     op Redis buffer + copy dono me (WAIT 1, ~1 ms), tab ACK. Har 1 sec ek batch Cassandra me
KYUN    ACK tabhi jab op buffer aur copy dono me pahunch gaya.
        Cassandra ka batch ACK ke raaste me hai hi nahi.
        Isliye turant "saved" milta hai aur DB pe likhai kam hoti hai
```

> [!IMPORTANT]
> **Yaad rakh:** Op Redis buffer + copy (WAIT 1), tab ACK. Har 1 sec batch Cassandra me. Batch ACK ke raaste me nahi.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - ACK se pehle Redis gira → **app op rakhta, opId**: app op rakhta hai, bina ACK pakka nahi. Judte hi opId ke saath phir bhejo, pehle se hai to skip
> - ACK ke baad Redis gira → **copy main ban gayi**: ACK se pehle copy me likh chuke the, kuch nahi khoya
> - batch dheema ho gaya → **typing nahi rukti**: kyunki batch ACK ke raaste me nahi. Buffer badhta hai, to alert
> - batch dobara likh diya → **(docId, version) ek row**: (docId, version) same hai, to ek hi row banti hai. Kuch double nahi
> - Cassandra hi kyun? → **sirf neeche jodta**: wo sirf neeche jodta hai (commit log + RAM), likhai tez. Join chahiye nahi. Mongo bhi chalega


---

*ab net hi chala gaya to?*

## 7. Metro me mera net 1 second gaya, typing rukegi?

**Rail 3 MACHINE GIRI (net gaya)** · Cue: likhna AP, permission CP

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c7_1.png?v=1" width="900">

<img src="img/c7_2.png?v=1" width="900">

<img src="img/c7_3.png?v=1" width="900">

</details>

```
HAL     likhna AP: typing chalti, op phone me pending. Permission CP: net aate hi SQL check
KYUN    typing kabhi na ruke (AP): net wapas aate hi op bhejo, server permission check karke khiska ke
        lagata, aakhir me sab same. Access kabhi galat na ho (CP): permission hatate hi roko,
        chahe thodi der ruke. Isliye permission alag SQL me
NAHI    net gaya to sab ruk jaaye -> RUKA: kaam ruk gaya, ye nahi chahiye
        sab kuch "turant sab same" (strong) -> RUKA: net jaate hi typing ruk jaati.
        Humein "aakhir me same" chahiye
PEHLE   pehle poochho: offline chahiye ya nahi? Isi ke jawab se poora CAP faisla tay hota hai
```

> [!IMPORTANT]
> **Yaad rakh:** Likhna AP: op phone me pending, typing kabhi nahi rukti. Permission CP: judte hi SQL check, nahi to reject.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - pending op ke saath 1 ghanta offline raha → **pending op + version**: pending op aur aakhri version rakho. Wapas aate hi bhejo, server khiska ke lagata, sab same
> - offline me mera access hata diya → **pehle permission (CP)**: Judte hi SQL check, access nahi hai to pending op reject


---

*ab doc purana aur lamba ho gaya*

## 8. Raat ko 2 saal purana doc khola, 10 lakh op shuru se chale

**Rail 5 KUCH DHEEMA (purana doc)** · Cue: SNAPSHOT + uske baad ke op

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c8_1.png?v=1" width="900">

<img src="img/c8_2.png?v=1" width="900">

<img src="img/c8_3.png?v=1" width="900">

</details>

```
HAL     har 1000 op pe photo (snapshot), background job leta. Kholna = aakhri photo + baad ke op
KYUN    photo ek version tak ki hoti hai, to kholna = photo + uske baad ke ~1000 op. Shuru se kabhi nahi.
        Photo background job leta hai, user ko dikhta hi nahi
NAHI    har baar shuru se saare op ek-ek chalao -> DHEEMA: minuton tak khaali screen
```

> [!IMPORTANT]
> **Yaad rakh:** Har 1000 op pe photo (snapshot), job leta. Kholna = aakhri photo + baad ke op. Koi bhi version aise hi.

> [!TIP]
> **Cross-Q: sawaal → cue, phir poora jawab**
> - photo lete waqt kisi ne likha → **photo ek version tak**: photo ek version tak hai (v9,99,000), baad ke op alag. Kuch nahi rukta
> - kal ka purana version v5,00,500 maanga → **pehli photo + beech ke op**: v5,00,000 ki photo + 500 op. Koi bhi purana version = us se pehli photo + beech ke op
> - photo kitni baar lein? → **har 1000 op**: kam li to kholte waqt utne zyada op chalane padte. Har 1000 op pe theek hai


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
