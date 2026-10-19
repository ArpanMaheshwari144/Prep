# Google Docs (Collaborative Editor)

> 2 ya zyada log ek doc ek saath edit karein. Kisi ka likha na mite, kuch kho na jaaye, turant dikhe.
> Dil: ek saath edit, koi likha khoye nahi, aur aakhir me sabke paas doc same (convergence).
>
> Har card: kya toota -> kya kiya -> yahi kyun -> doosra kyun nahi. Kram me chalo. "Pooche" wali line = interviewer aage poochhe to.

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c8_2-a2085e82.svg)

---

## Shuru

Sabse simple: doc ka poora text DB me. "Save" dabao to poora text upar likh do (overwrite).

Main dhyaan ek saath EDITING pe. Comments, version history, formatting scope ke bahar. Do sawaal pehle poochho: ek
doc pe ek saath kitne log (2-3 ya 100), aur offline chahiye ya nahi. Offline ka jawab hi poora CAP faisla tay karta.

Kya chahiye: kai log ek doc saath edit karein, doosron ka likha turant dikhe, kisi ka likha na mite. Kaisa chale:
typing turant ho aur kabhi na ruke (offline bhi). Aakhir me sabka doc same ho. "Turant sab same" (strong) nahi,
"aakhir me same" (convergence).

Numbers: ~10 crore user, ~3-4 crore roz. Har akshar ek event, to likhna bahut zyada. Isliye aage beech me buffer aur
baantna (sharding) lagega. Pakka number yahan maayne nahi rakhta.

---

*ab tootega kahan? do log ek hi doc save karein to*

## 1. Ravi ne baad me save kiya, mera "WORLD" mit gaya

Maine 10:05 pe "HELLO WORLD" save kiya, Ravi ne 10:07 pe "HELLO THERE". Baad wala jeeta, mera WORLD gayab.

Isliye ab poora text nahi bhejte, sirf ek badlaav bhejte hain: "pos 5 pe ' WORLD'", aur saath me ki kis version ko
dekh ke bana, v7 (operation + baseVersion). Server use agla version de ke lagata aur Ravi ko bhej deta.

Ye kyun chalta? Server ke paas text nahi, badlaav aate hain. Mera op aur Ravi ka op dono v7 pe bane, to server samajh
jaata ki dono saath bane, aur dono ko khiska ke lagata: "HELLO WORLD THERE". Kisi ka likha nahi mitta.

Lock kyun nahi? User ko lock ya intezaar nahi chahiye. Purane version pe bana op bhi khisak ke lag jaata.

**Pooche: maine mitaya, aur Ravi ne wahin likha?** Mitana bhi ek op hai. Server dono ko khiska ke lagata, Ravi ka
akshar sahi jagah.

**Pooche: abhi ka doc kya hai?** Us doc ke saare op kram se, pehle se aakhri tak laga do. Doc = op ka jod.

---

*ab badlaav bhej to diye, par Ravi ko dikhe kaise?*

## 2. Ravi ko mera likha refresh karne pe hi dikha

HTTP me server khud kuch nahi bhej sakta. Maine X likha, server ke paas aaya, par Ravi ko refresh tak kuch nahi.

Isliye connection khula rehta hai, dono taraf (WebSocket). Server khud bhejta, Ravi ko ~200 ms me X dikh jaata.

WebSocket hi kyun? Ek khula connection dono taraf chalta. Server khud bhejta, user ko refresh nahi karna padta.

**Pooche: connection chupchaap mar gaya to?** Har ~30 second ping / pong. Jawab nahi aaya to connection band, jagah khaali.

**Pooche: connection toot gaya to?** Main likhta rahun. Judte hi bolo "main v12 pe tha", server v13 se aage sab bhej deta.

---

*ab dono ek hi jagah ek saath likhein to?*

## 3. 11 baje meeting me dono ne sabse upar ek saath type kiya

Maine X likha, Ravi ne Y, dono sabse upar. Jo aaya wahi laga diya, to meri screen pe "YX..." aur Ravi ki pe "XY...".
Do alag doc.

Isliye kram server tay karta hai, aur doosre ka op khiskata hai (OT). Server ne pehle mera X liya (v8), phir Ravi ka
Y (v9). Y ko 0 se 1 pe khiska diya. Dono ke paas "XYHELLO".

Ye kyun chalta? Kram sirf server ka, har op ko agla version. Phone ki ghadi kabhi nahi. Jo op aaya wo us kram se
khisakta, to sabki screen same. Mujhe apna likha turant dikhe, baaki ka khisak ke aaye.

Kram timestamp se kyun nahi? Ek ms pe do op aaye to same key, ek ne doosra chupchaap mita diya. Isliye version.

CRDT kyun nahi, OT kyun? Beech me server hai, har op usse guzarta hai. OT yahi maangta hai (Docs bhi OT).

**Pooche: CRDT hota kya hai?** Har akshar ki apni unique id. Kisi bhi kram me jodo, nateeja same. Offline chalta,
par memory zyada. OT me op bhejo aur ek saath wale ko khiskao, CRDT me id se apne aap jud jaata.

**Pooche: Ravi ka op ~200 ms baad aaya, mera beech me likha tha?** App apne pending op khiska ke uska lagata.

**Pooche: dono ne pos 0 pe likha, tie?** Server pe jo pehle aaya wo 0 pe, doosra 1 pe khisakta. Dono ke paas "XYHELLO".

---

*ab OT ek server pe hota hai, par connection itne saare*

## 4. 50 lakh connection khule, ek connection server nahi jhelta

Ek server ~1 lakh connection hi jhelta. Kai server lagaye, par mere doc ke editor alag server pe: main server-1 pe,
Ravi server-3 pe. OT kaun kare? Do jagah do version, doc toota.

Isliye docId se ring pe server chunte hain. Ek doc ke saare editor ek hi server pe, wo us doc ka owner. doc:123 ring
pe server-1 ka, to OT wahin.

Ye kyun chalta? Ek doc = ek owner server, OT ek hi jagah. Editor docId se wahin aata, viewer kahin bhi.

Editor alag-alag server pe kyun nahi? Do server = do version. OT ek jagah hi ho sakta.

**Pooche: 1000 log ek doc pe?** 100 editor owner pe (OT sirf likhne walon ka), 900 viewer kahin bhi.

**Pooche: server juda ya gira?** Ring pe sirf uske pados ke doc khiskte, wo log dobara judte.

**Pooche: owner gira, doc kahan gaya?** Doc server pe nahi, DB me hai. Ring agla server deta, naya owner DB se doc
banata. Editor dobara jude, version bata ke baaki op le. Doc nahi khota.

---

*ab viewer kahin bhi hai, use op pahunche kaise?*

## 5. Ravi ab sirf padh raha, wo doosre server pe

Maine server-1 pe likha. Ravi server-2 pe hai, server-2 ko kuch pata nahi. Ravi ke paas purana doc.

Isliye har doc ka ek channel (Redis pub/sub). Server-1 OT karke doc:123 channel pe op daalta. Jis server pe us doc ka
user hai wo channel sunta (subscriber), server-2 Ravi ko bhej deta.

Pub/sub hi kyun? Iska kaam sirf turant pahunchana hai, kuch store nahi karta (bheja aur bhool gaya). Bachana DB ka
kaam hai, channel ka nahi. Viewer ka kuch chhoota to version bata ke DB se le lo.

**Pooche: editor same server pe hai to?** Seedha push. Pub/sub ki zaroorat nahi.

**Pooche: Ravi ka server restart ho gaya?** Op channel se gaya (pub/sub store nahi karta), par DB me hai. Ravi jude,
version bata ke baaki le le.

**Pooche: Redis pub/sub hi gir gaya?** Copy main ban jaati (Sentinel). Live kuch second ruka, khoya kuch nahi.

---

*ab sab live chal raha, par har akshar DB tak?*

## 6. 3 baje DB alert: har akshar pe DB me likha, DB baith gaya

20 lakh log, har second 3 akshar. Yaani 60 lakh write har second seedha DB pe. DB baith gaya.

Isliye op pehle Redis me jama hote hain, aur har second ek saath DB me (buffer + batch). Op Redis buffer aur uski copy
dono me gaya (WAIT 1, ~1 ms), tab user ko ACK "saved". Har 1 second batch Cassandra me.

Ye kyun chalta? ACK tabhi jab op Redis buffer aur uski copy dono me. Cassandra ka batch ACK ke raaste me hai hi nahi.
User ko turant "saved", aur DB pe kam likhai.

**Pooche: ACK se pehle Redis gira?** App op rakhta hai, bina ACK pakka nahi. Jude to opId ke saath phir bhejo. Pehle
se hai to skip.

**Pooche: ACK ke baad Redis gira?** Copy main ban gayi. ACK se pehle copy me likh chuke the, to kuch nahi khoya.

**Pooche: batch dheema ho gaya?** Typing nahi rukti, batch ACK ke raaste me nahi. Buffer badhta hai, to alert.

**Pooche: batch dobara likh diya?** (docId, version) same, to ek hi row. Kuch double nahi.

**Pooche: Cassandra kyun?** Sirf neeche jodta (commit log + RAM), likhai tez. Join chahiye nahi. Mongo bhi chalega.

---

*ab net hi chala gaya to?*

## 7. Metro me mera net 1 second gaya, typing rukegi?

Net gaya to sab ruk jaata, kaam ruka. Ye nahi chahiye.

Isliye likhna chalta rehta hai (AP), aur permission turant rokti hai (CP). Net gaya to main likhta rahun, op phone me
pending jama. Net wapas aaya to server pehle permission check karta, phir khiska ke lagata.

Ye kyun chalta? Typing kabhi na ruke (AP): judte hi op bhejo, aakhir me sab same. Par access kabhi galat na ho (CP):
permission hatate hi roko, chahe thodi der ruke. Isliye permission alag SQL me.

Sab kuch "turant sab same" (strong) kyun nahi? Tab net jaate hi typing ruk jaati. Humein "aakhir me same" chahiye.

**Pooche: 1 ghanta offline raha?** Pending op aur aakhri version rakho. Wapas aate hi bhejo, server khiska ke lagata,
sab same.

**Pooche: offline me mera access hata diya gaya?** Pehle permission (CP). Jude to SQL check, nahi hai to pending op reject.

---

*ab doc purana aur lamba ho gaya*

## 8. Raat ko 2 saal purana doc khola, 10 lakh op shuru se chale

Har baar shuru se saare op ek-ek chalate, to minuton khaali screen.

Isliye har 1000 op pe ek photo (snapshot). Background job leta, user ko nahi dikhta. Doc kholna = aakhri photo +
uske baad ke ~1000 op.

Ye kyun chalta? Photo ek version tak, kholna = photo + uske baad ke op. Shuru se kabhi nahi.

**Pooche: photo lete waqt kisi ne likha?** Photo ek version tak (v9,99,000), baad ke op alag. Kuch nahi rukta.

**Pooche: kal ka purana version v5,00,500 maanga?** v5,00,000 ki photo + 500 op. Koi bhi purana version = us se
pehli photo + beech ke op.

**Pooche: photo kitni baar lein?** Kam li to kholte waqt utne zyada op chalane padte. Har 1000 op theek.

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
