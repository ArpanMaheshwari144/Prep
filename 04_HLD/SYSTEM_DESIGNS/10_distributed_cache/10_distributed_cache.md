# Distributed Cache (Redis-jaisa)

> Tez RAM wala key-value store, jo ek machine se bada ho aur ek machine mare to bhi chale. Concept detail = FOUNDATIONS/04_caching.md
> Dil do cheezein: tez (<1 ms), aur machine mare to bhi chale. BookMyShow ka ulta: wahan sahi hona dil tha, yahan tezi.
>
> Mock me sirf heading + Cue + GIF. Neeche har card: **hal** · **kyun** · **nahi** (doosra kyun nahi) · **Pooche**.

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c6_2-4533d6f5.svg)

---


## Shuru

- **FR:** get(key), put(key, value, ttl), delete(key) · disk pe bachana, pub/sub, transactions scope se bahar
- **NFR:** <1 ms (yahi dil) · ek machine mare to chale · TB bhar data · "thodi der baad sahi" (eventual) chalega, asli sach DB me
- **numbers:** ~1 TB data, ~10 lakh read / sec, padhai zyada. DB se ~10 ms (disk), 10 lakh / sec pe DB ghutno pe -> aage RAM cache. 1 TB ek machine ki RAM (64 GB) me nahi -> baanto (sharding) · 10 lakh / sec ek machine nahi deti -> machines me baanto · machine mari to saara bojh DB pe -> har machine ki copy (replication). Teeno = "ek machine kaafi" khaarij -> distributed

---

*ab tootega kahan? sabse pehle RAM pe, kyunki RAM chhoti hai*

## 1. Subah cache machine ki RAM bhar gayi, kya hatayein

**Cue:** LRU + TTL (HashMap + DLL) · dabba: Cache (LRU + TTL)

- **pehle (sabse simple):** ek machine, uski RAM me ek HashMap, get / put O(1)
- **cache-aside:** App pehle cache se: `GET profile:42` mili (HIT) <1 ms, DB tak nahi. Nahi mili (MISS) -> DB se (~10 ms), cache me TTL ke saath, agli baar HIT.
- **hal:** jo sabse der se chhua wo bahar (LRU) + har key ki umar (TTL). RAM bhari to sabse der se chhua hatao. Wahi LRU jo LC 146 me.
- **kyun:** RAM bhari to sabse purana chhua pehle jaata, umar khatam = key bekaar. Hati key = sirf ek miss, App DB se laake wapas rakh deta.
- **kyun:** sach DB me, cache sirf tez copy. Cache khaali ho tab bhi sahi data DB se, App ko farak sirf tezi ka.
- **nahi:** LFU (kitni baar chhua) -> kal ki famous key aaj bhi baithi, purani ginti nahi ghatti. Default LRU.
- **nahi:** hatana hi band (noeviction) -> naya likhna error deta. Cache me allkeys-lru rakho.
- **Pooche:** sabse purana har baar dhoondhoge? -> nahi. HashMap + doubly linked list: chhua to aage, peeche wala hatao, O(1).
- **Pooche:** user:9 phir padha? -> utha ke aage. Ab sabse baad me nikalega, chhua = taaza.
- **Pooche:** Redis sach me poori LRU list rakhta? -> nahi, andaaze wala LRU: kuch random key me sabse purani. Poori list = har key pe memory.
- **Pooche:** umar (TTL) kitni rakhoge? -> jitna purana jhel sake (5 min chalega?). Profile = minute, price / stock = second.
- **Pooche:** umar (TTL) khatam ho gayi? -> agla padhne wala MISS, DB se taaza. Purana zyada der nahi tikta.

---

*ab ek machine me jagah ka hisaab ho gaya, par data hi ek machine se bada*

## 2. Subah 11 tak data 1 TB: "user:42" kis machine pe?

**Cue:** CONSISTENT HASHING + virtual nodes · dabba: Cache client + Nodes

- **hal:** kai machine, App ke paas cache client jo batata key kis machine pe (consistent hashing). Key ring pe aage chalti, pehli machine = uska ghar. Aakhir ke baad ghoom ke pehli.
- **kyun:** machine jude / jaaye to sirf pados ki ~K/N key hilti (K = saari key, N = machine). Sirf us hisse ki ek baar miss, baaki hit.
- **kyun:** virtual nodes = pados kai machine, bojh ek pe nahi
- **nahi:** seedha hash % N -> N badla to har key ka % N badla. 4 se 5 machine = ~80% key hilti. Ring me N hai hi nahi.
- **nahi:** A-M ek pe, N-Z doosre pe -> ek range garam (sab "S" naam), ek machine pe bojh. Hash se key bikhar jaati.
- **Pooche:** machine D judi? -> sirf pados ki key khiski (201..250, C se D pe). Baaki sab hit.
- **Pooche:** machine A giri? -> uski key aage wali machine pe. Sirf wahi ek baar miss.
- **Pooche:** A ka saara bojh B pe double? -> har machine ring pe 100-200 jagah (virtual nodes). Bojh kai pados pe bat-ta.
- **Pooche:** Redis Cluster bhi ring hai? -> nahi. `CRC16(key) % 16384` slot, slot se machine. Machine badhe to sirf kuch slot khiskao.

---

*ab data baant diya, par ek machine mari to?*

## 3. Dopahar machine B mar gayi, uski key ka bojh DB pe

**Cue:** REPLICA (async) + quorum failover · dabba: Node + replica

- **hal:** har machine ki ek copy (replica, async). B mari to B ki copy B ban jaati. DB bacha.
- **kyun:** cache me tezi pehle. Copy thodi der baad likhi jaati, aakhri likhai khoye to chalta, sach DB me.
- **kyun:** giri to copy main, App ko us hisse me thodi der miss. Asar sirf B ki key pe, har machine ka apna hissa.
- **nahi:** copy na rakho -> pehle yahi tha: B mari, B ki saari key (user:42 ...) miss, DB pe achanak bojh, DB dheema
- **Pooche:** copy ko main banane me kitna? -> ~10-30 sec (pakka B mari?). Us beech B ki key DB se, dheema.
- **Pooche:** copy peeche thi? -> aakhri kuch likhai kho sakti. Cache me chalta, sach DB me.
- **Pooche:** network toota, do main ban gaye? -> zyadatar machines (quorum) tay karti kaun main. Akeli wali likhna band (split brain).
- **Pooche:** copy se padh bhi lo? -> haan, padhai bat-ti. Thoda purana mil sakta, copy peeche chalti.
- **Pooche:** poora cache hi gir gaya? -> saari padhai DB pe. DB ke aage rate limit / load shedding, warna DB bhi gira.

---

*ab machine marne se nahi girta, par data purana dikh raha*

## 4. Ravi ne naam badla, sabko purana dikh raha

**Cue:** DB me likho, cache key DELETE · dabba: (App: likhne pe delete)

- **hal:** pehle DB update, phir cache ki key DELETE (delete on write). Agla padhne wala DB se naya laata, cache me rakhta.
- **kyun:** likho DB me, cache se hatao. Kuch bhi pahunche, key khaali, agla DB se naya. Umar = suraksha jaal: delete chhoota to utni der hi purana.
- **nahi:** sirf DB update -> DB me "Ravi K", cache me purana "Ravi", sab "Ravi" dekhte
- **nahi:** write-through (DB + cache saath likho) -> taaza rehta, par likhna dheema
- **nahi:** write-back (pehle sirf cache, DB baad me) -> tez, par cache mara to data gaya. Paise pe kabhi nahi.
- **Pooche:** delete fail ho gaya? -> umar (TTL) bachati, utni der hi purana. Pakka chahiye to DB ke log se delete (CDC).
- **Pooche:** purana beech me wapas bhar gaya? -> thodi der baad phir delete (double delete), ya version se likho
- **Pooche:** Ravi ko apna hi purana naam dikha? -> copy peeche hai. Likhne wale ko kuch der primary se padhao.

---

*ab data taaza, par key khatam hote hi bheed*

## 5. 7 baje "home page" key khatam, 1000 log ek saath DB pe

**Cue:** LOCK: SET NX EX, sirf pehla DB jaaye · dabba: (App: SET NX lock)

- **hal:** ek jaaye, DB se laaye, cache bhare, baaki ruk ke cache se (lock). Pehla lock leta (`SET lock:home NX EX 5`), wahi DB se. Baaki 999 cache se.
- **kyun:** khaali key pe ek hi DB jaata, baaki rukte ya purani value lete. DB pe 1000 ek saath kabhi nahi.
- **kyun:** lock ki umar, to lene wala mare to khud khulta. App ko bas thoda dheema ya purana.
- **nahi:** sabko DB jaane do -> sabko miss, 1000 ek saath (stampede), home page dheema
- **Pooche:** purani de sakte, to key khatam hone hi kyun di? -> umar khatam se thoda pehle taaza karo (soft-TTL), key khaali hi na ho
- **Pooche:** 999 kitna ruke? -> 50-100 ms, ya purani value do. Thodi purani home page chalti.
- **Pooche:** lock lene wala hi mar gaya? -> lock ki umar (EX 5). Khud khulta, agla le leta. 999 hamesha nahi ruke.
- **Pooche:** saari key ek saath khatam? -> umar me random daalo (60 min +- 5). Ek saath nahi girti.
- **Pooche:** key DB me thi hi nahi? -> "nahi hai" bhi cache karo, chhoti umar ke saath. Warna har baar DB (penetration).
- **Pooche:** sale 12 baje, pata tha na? -> pehle hi cache bhar do + machine badhao. Waqt pata = tayyari pehle.

---

*ab key khaali hone ki bheed sambhal li, par ek key pe hi crore log*

## 6. Raat IPL final, "live:score" key ki machine akeli mar rahi

**Cue:** L1 LOCAL CACHE + key ki copies · dabba: App + L1 local cache

- **hal:** garam key ki thodi der ki copy har App ki memory me (L1 cache). App 1-2 sec me ek baar Machine A se leta, A aaram se. Ya key ki kai copy (score#1..#10).
- **kyun:** garam key ko faila do. Ek key = ek machine, crore read / sec -> A ka CPU full, B aur C khaali. Isliye bheed baantni padti.
- **kyun:** score 1-2 sec purana chalta, paisa nahi
- **nahi:** consistent hashing se bheed bat jaayegi -> nahi, ring key baant-ta, ek key ki bheed nahi. Ek key = ek machine.
- **Pooche:** garam key pakdoge kaise? -> pehle pakdo: App me ginti + machine ka metric achanak upar
- **Pooche:** L1 me score kitna purana? -> 1-2 sec. Score chalta, paisa / stock nahi.
- **Pooche:** us garam key pe likhna (like)? -> key ke tukde like#0..#9. Kisi ek me likho, padhte waqt jodo.
- **Pooche:** kitni parat hui? -> L1 (App RAM) -> L2 Redis -> DB. Neeche wali dheemi, isliye upar se pehle.
- **Pooche:** stampede aur hot key me farak? -> stampede = key khaali (lock). Hot key = key bhari, ek machine pe bheed (L1).

---

## Poora design ek saans me

```
App + L1 local cache (nano-sec, hot key ka pehla ilaaj) -> Cache client (consistent hashing: key -> machine)
-> Machine (HashMap + DLL = LRU, + TTL) primary + copy (alag AZ) -> miss pe DB (asli sach)
PADHNA = cache-aside (~90% hit, DB pe bojh 10 guna kam) · PURANA = TTL + likhne pe delete
STAMPEDE = lock / soft-TTL · HOT KEY = copies / L1 local
```

Bolne ke liye: cache = RAM me rakhi key-value, kai machines me baanti (ring se), har machine ki copy. App pehle cache
se poochhta, na mile to DB se laake cache me rakhta. RAM bhare to purani key nikalti, har key ki umar hoti. Do dikkat
dhyaan rakhni: purana data (badalne pe key hatao + umar), aur key khatam hote hi bheed (ek laaye, baaki ruke). Bahut
garam key: har app server apne paas 1-2 second rakhe.

---

## 10 guna traffic

- **Cache client**: machine judi / gayi to consistent hashing + virtual nodes.
- **Machine**: RAM bhari to LRU + TTL. Mari to copy promote. Data badha to machine jodo.
- **Hot key**: copies + L1 local cache.
- **Umar khatam**: lock, soft-TTL, kabhi khatam nahi, ya pehle se bharo.
- **DB**: purana data, to likhne pe delete + TTL.
- **Aage**: write-back (likhna bahut ho to), kai region me cache.

**Pooche: 10 guna kaise?** Request ka raasta chalo.

**Pooche: kahan se sab gir sakta?** Machine (copy hai), cache client ka config (sab App me copy).

**Pooche: chal raha hai, kaise pata?** Hit ratio sabse zaroori (achanak gira = kuch toota), p99, evictions, alert.

---

## Pooche to (kisi card me nahi baitha)

API: GET key, PUT key value ttl, DELETE key. Jaan-boojh ke chhoti: query nahi, sirf key se, to index / planner nahi,
isiliye tez.

Machine ke andar: HashMap key -> value (O(1)). LRU = HashMap + DLL. TTL lazy + active dono.

Kaunsa store: RAM wala key-value (Redis-jaisa), DB peeche asli sach. <1 ms chahiye to RAM (disk nahi). Join nahi
chahiye, to key-value kaafi.

Trade-off: tezi vs sahi hona. Cache AP ki taraf, "thodi der baad sahi" chalta (CAP).

- **Pehle poochho:** ek machine ka cache (kya hatayein, TTL) ya kai machine wala (baantna, copy), kis pe focus?
- **Pehle poochho:** kaisa data, padhai zyada?
- **Pehle poochho:** kitna purana chalega, 5 min? -> isi se tay hota cache kab saaf karna
- **Pehle poochho:** hatane ka tareeka kya (LRU / LFU)?
- **Pehle poochho:** cache DB ke aage hai ya khud asli store?

---

ARCHETYPE F · CONCEPTS: [caching](../../FOUNDATIONS/04_caching.md) · [sharding](../../FOUNDATIONS/06_database_sharding.md) · [replication](../../FOUNDATIONS/05_database_replication.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
