# Google Docs (Collaborative Editor)

> 2 ya zyada log ek doc ek saath edit karein. Kisi ka likha na mite, kuch kho na jaaye, turant dikhe.
> Dil: ek saath edit, koi likha khoye nahi, aur aakhir me sabke paas doc same (convergence).
>
> Mock me sirf heading + Cue + GIF. Neeche har card: **hal** · **kyun** · **nahi** (doosra kyun nahi) · **Pooche**.

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c8_2-a2085e82.svg)

---


## Shuru

- **FR:** kai log ek doc saath edit karein, doosron ka likha turant dikhe, kisi ka likha na mite · dhyaan sirf ek saath EDITING pe (comments, version history, formatting scope ke bahar)
- **NFR:** typing turant ho aur kabhi na ruke (offline bhi) · aakhir me sabka doc same (convergence), "turant sab same" (strong) nahi
- **numbers:** ~10 crore user, ~3-4 crore roz. Har akshar ek event -> likhna bahut zyada -> buffer + baantna (sharding). Pakka number yahan maayne nahi rakhta.

---

*ab tootega kahan? do log ek hi doc save karein to*

## 1. Ravi ne baad me save kiya, mera "WORLD" mit gaya

**Cue:** OPERATION + baseVersion (text nahi) · dabba: App (op bhejo)

- **pehle:** sabse simple = doc ka poora text DB me, "Save" pe poora text upar likh do (overwrite)
- **toota:** 10:05 mera "HELLO WORLD", 10:07 Ravi ka "HELLO THERE" -> baad wala jeeta, mera WORLD gayab
- **hal:** poora text nahi, sirf badlaav bhejo: "pos 5 pe ' WORLD'" + kis version pe bana, v7 (operation + baseVersion)
- **hal:** server use agla version de ke lagata, Ravi ko bhej deta
- **kyun:** server ke paas text nahi, badlaav aate. Dono op v7 pe bane -> server samjhe saath bane, dono khiska ke lagata: "HELLO WORLD THERE"
- **nahi:** lock -> user ko lock ya intezaar nahi chahiye. Purane version pe bana op bhi khisak ke lag jaata.
- **Pooche:** maine mitaya, aur Ravi ne wahin likha? -> mitana bhi ek op. Server dono khiska ke lagata, Ravi ka akshar sahi jagah.
- **Pooche:** abhi ka doc kya hai? -> us doc ke saare op kram se, pehle se aakhri tak laga do. Doc = op ka jod.

---

*ab badlaav bhej to diye, par Ravi ko dikhe kaise?*

## 2. Ravi ko mera likha refresh karne pe hi dikha

**Cue:** WEBSOCKET (khula, dono taraf) · dabba: Conn-Server

- **hal:** connection khula rehta, dono taraf (WebSocket). Server khud bhejta, Ravi ko ~200 ms me X dikh jaata.
- **kyun:** ek khula connection dono taraf chalta. Server khud bhejta, user ko refresh nahi karna padta.
- **nahi:** HTTP -> server khud kuch nahi bhej sakta. X server tak aaya, par Ravi ko refresh tak kuch nahi.
- **Pooche:** connection chupchaap mar gaya? -> har ~30 sec ping / pong. Jawab nahi to connection band, jagah khaali.
- **Pooche:** connection toot gaya? -> main likhta rahun. Judte hi "main v12 pe tha", server v13 se aage sab bhej deta.

---

*ab dono ek hi jagah ek saath likhein to?*

## 3. 11 baje meeting me dono ne sabse upar ek saath type kiya

**Cue:** OT (server kram, op khiskao) · dabba: Conn-Server (OT)

- **hal:** kram server tay kare, doosre ka op khiskao (OT). Mera X pehle (v8), Ravi ka Y (v9), Y 0 se 1 pe -> dono ke paas "XYHELLO"
- **kyun:** kram sirf server ka, har op ko agla version, phone ki ghadi kabhi nahi. Aaya op us kram se khisakta -> sabki screen same.
- **kyun:** mujhe apna likha turant dikhe, baaki ka khisak ke aaye
- **nahi:** jo aaya wahi laga do -> meri screen "YX...", Ravi ki "XY...", do alag doc
- **nahi:** kram timestamp se -> ek ms pe do op = same key, ek ne doosra chupchaap mita diya. Isliye version.
- **nahi:** CRDT -> beech me server hai, har op usse guzarta. OT yahi maangta (Docs bhi OT).
- **Pooche:** CRDT hota kya hai? -> har akshar ki unique id, kisi bhi kram me jodo nateeja same. Offline chalta, memory zyada. OT op khiskata, CRDT id se apne aap judta.
- **Pooche:** Ravi ka op ~200 ms baad aaya, mera beech me likha tha? -> app apne pending op khiska ke uska lagata
- **Pooche:** dono ne pos 0 pe likha, tie? -> server pe jo pehle aaya wo 0 pe, doosra 1 pe khisakta. Dono ke paas "XYHELLO".

---

*ab OT ek server pe hota hai, par connection itne saare*

## 4. 50 lakh connection khule, ek connection server nahi jhelta

**Cue:** RING: docId se ek OWNER server · dabba: LB + Conn-Server (owner)

- **hal:** docId se ring pe server chuno. Ek doc ke saare editor ek hi server pe = us doc ka owner. doc:123 ring pe server-1 ka, OT wahin.
- **kyun:** ek server ~1 lakh connection hi jhelta, isliye kai server. Ek doc = ek owner, OT ek hi jagah.
- **kyun:** editor docId se wahin aata, viewer kahin bhi
- **nahi:** editor alag-alag server pe (main server-1, Ravi server-3) -> OT kaun kare? do server = do version, doc toota
- **pehle poochho:** ek doc pe ek saath kitne log (2-3 ya 100)?
- **Pooche:** 1000 log ek doc pe? -> 100 editor owner pe (OT sirf likhne walon ka), 900 viewer kahin bhi
- **Pooche:** server juda ya gira? -> ring pe sirf uske pados ke doc khiskte, wo log dobara judte
- **Pooche:** owner gira, doc kahan gaya? -> doc server pe nahi, DB me. Ring agla server, naya owner DB se doc banata. Editor dobara jude, version bata ke baaki op le. Doc nahi khota.

---

*ab viewer kahin bhi hai, use op pahunche kaise?*

## 5. Ravi ab sirf padh raha, wo doosre server pe

**Cue:** REDIS PUB/SUB (doc ek channel) · dabba: Redis pub/sub

- **toota:** maine server-1 pe likha, server-2 ko kuch pata nahi -> Ravi ke paas purana doc
- **hal:** har doc ka ek channel (Redis pub/sub). Server-1 OT karke doc:123 channel pe op daalta.
- **hal:** jis server pe us doc ka user, wo channel sunta (subscriber) -> server-2 Ravi ko bhej deta
- **kyun:** kaam sirf turant pahunchana, kuch store nahi (bheja aur bhool gaya). Bachana DB ka kaam, channel ka nahi.
- **kyun:** viewer ka kuch chhoota to version bata ke DB se le lo
- **Pooche:** editor same server pe? -> seedha push, pub/sub ki zaroorat nahi
- **Pooche:** Ravi ka server restart ho gaya? -> op channel se gaya (store nahi), par DB me. Ravi jude, version bata ke baaki le.
- **Pooche:** Redis pub/sub hi gir gaya? -> copy main ban jaati (Sentinel). Live kuch second ruka, khoya kuch nahi.

---

*ab sab live chal raha, par har akshar DB tak?*

## 6. 3 baje DB alert: har akshar pe DB me likha, DB baith gaya

**Cue:** REDIS BUFFER + BATCH, ACK = WAIT 1 · dabba: Redis buffer + Cassandra

- **toota:** 20 lakh log, har second 3 akshar = 60 lakh write / sec seedha DB pe
- **hal:** op pehle Redis me jama, har 1 second ek saath Cassandra me (buffer + batch)
- **hal:** op Redis buffer + uski copy dono me gaya (WAIT 1, ~1 ms), tab user ko ACK "saved"
- **kyun:** ACK tabhi jab op buffer + copy dono me. Cassandra batch ACK ke raaste me hai hi nahi -> turant "saved", DB pe kam likhai.
- **Pooche:** ACK se pehle Redis gira? -> app op rakhta, bina ACK pakka nahi. Jude to opId ke saath phir bhejo, pehle se hai to skip.
- **Pooche:** ACK ke baad Redis gira? -> copy main ban gayi. ACK se pehle copy me likh chuke, kuch nahi khoya.
- **Pooche:** batch dheema ho gaya? -> typing nahi rukti, batch ACK ke raaste me nahi. Buffer badhta, to alert.
- **Pooche:** batch dobara likh diya? -> (docId, version) same, to ek hi row. Kuch double nahi.
- **Pooche:** Cassandra kyun? -> sirf neeche jodta (commit log + RAM), likhai tez. Join chahiye nahi. Mongo bhi chalega.

---

*ab net hi chala gaya to?*

## 7. Metro me mera net 1 second gaya, typing rukegi?

**Cue:** likhna AP, permission CP · dabba: SQL permissions

- **hal:** likhna chalta rehta (AP), permission turant rokti (CP). Net gaya -> likhta rahun, op phone me pending jama.
- **hal:** net wapas -> server pehle permission check, phir khiska ke lagata
- **kyun:** typing kabhi na ruke (AP): judte hi op bhejo, aakhir me sab same
- **kyun:** access kabhi galat na ho (CP): permission hatate hi roko, chahe thodi der ruke. Isliye permission alag SQL me.
- **nahi:** net gaya to sab ruk jaaye -> kaam ruka, ye nahi chahiye
- **nahi:** sab kuch "turant sab same" (strong) -> net jaate hi typing ruk jaati. Humein "aakhir me same" chahiye.
- **pehle poochho:** offline chahiye ya nahi? -> isi ka jawab poora CAP faisla tay karta
- **Pooche:** 1 ghanta offline raha? -> pending op + aakhri version rakho. Wapas aate hi bhejo, server khiska ke lagata, sab same.
- **Pooche:** offline me mera access hata diya? -> pehle permission (CP). Jude to SQL check, nahi hai to pending op reject.

---

*ab doc purana aur lamba ho gaya*

## 8. Raat ko 2 saal purana doc khola, 10 lakh op shuru se chale

**Cue:** SNAPSHOT + uske baad ke op · dabba: Cassandra (+ snapshot)

- **hal:** har 1000 op pe ek photo (snapshot), background job leta, user ko nahi dikhta
- **hal:** doc kholna = aakhri photo + uske baad ke ~1000 op
- **kyun:** photo ek version tak, kholna = photo + uske baad ke op. Shuru se kabhi nahi.
- **nahi:** har baar shuru se saare op ek-ek chalao -> minuton khaali screen
- **Pooche:** photo lete waqt kisi ne likha? -> photo ek version tak (v9,99,000), baad ke op alag. Kuch nahi rukta.
- **Pooche:** kal ka purana version v5,00,500 maanga? -> v5,00,000 ki photo + 500 op. Koi bhi purana version = us se pehli photo + beech ke op.
- **Pooche:** photo kitni baar lein? -> kam li to kholte waqt utne zyada op chalane padte. Har 1000 op theek.

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
