# Distributed Cache (Redis-jaisa)

> Tez RAM wala key-value store, jo ek machine se bada ho aur ek machine mare to bhi chale. Concept detail = FOUNDATIONS/04_caching.md
> Dil do cheezein: tez (<1 ms), aur machine mare to bhi chale. BookMyShow ka ulta: wahan sahi hona dil tha, yahan tezi.
>
> Har card: kya toota -> kya kiya -> yahi kyun -> doosra kyun nahi. Kram me chalo. "Pooche" wali line = interviewer aage poochhe to.

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c6_2-4533d6f5.svg)

---

## Shuru

Sabse simple: ek machine, uski RAM me ek HashMap, get / put O(1). App pehle cache se poochhta: mila to wahi do, nahi
mila to DB se laao, cache me daalo, phir do (cache-aside).

Kyun chahiye? Meri profile DB se padhne me ~10 ms lagte (disk). 10 lakh log har second profile khol rahe, DB ghutno pe.
Isliye DB ke aage RAM cache. `GET profile:42`: cache me mili (HIT) to <1 ms, DB tak gaya hi nahi. Nahi mili (MISS) to
DB se (~10 ms), cache me daalo TTL ke saath, agli baar HIT. Naam badla to DB me likho aur cache ki key DELETE, agla
padhne wala DB se naya laata.

Ye kyun chalta? Sach DB me, cache sirf tez copy. Padho cache se, na mile to DB se bharo. Likho DB me, key hatao. App ko
farak sirf tezi ka, cache khaali ho tab bhi sahi data DB se milta.

Pehle poochho: ek machine ka cache (kya hatayein, TTL) ya kai machine wala (baantna, copy), kis pe focus? Kaisa data,
padhai zyada? Kitna purana chalega, 5 min? Isi se tay hota cache kab saaf karna. Hatane ka tareeka kya (LRU / LFU)?
Cache DB ke aage hai ya khud asli store?

Kaam (FR): get(key), put(key, value, ttl), delete(key). Disk pe bachana, pub/sub, transactions scope se bahar.
Kaisa chale (NFR): <1 ms (yahi dil), ek machine mare to chale, TB bhar data. "Thodi der baad sahi" (eventual) chalega,
asli sach DB me.

Numbers: ~1 TB data, ~10 lakh read har second, padhai zyada. 1 TB ek machine ki RAM (64 GB) me nahi aata, isliye
baanto (sharding). 10 lakh har second ek machine nahi deti, isliye machines me baanto. Machine mari to saara bojh DB
pe, isliye har machine ki copy (replication). Teeno milke "ek machine kaafi" khaarij karte, isliye distributed.

**Pooche: umar (TTL) khatam ho gayi?** Agla padhne wala MISS, DB se taaza. Purana zyada der nahi tikta.

**Pooche: poora cache hi gir gaya?** Saari padhai DB pe. DB ke aage rate limit / load shedding, warna DB bhi gira.

---

*ab tootega kahan? sabse pehle RAM pe, kyunki RAM chhoti hai*

## 1. Subah cache machine ki RAM bhar gayi, kya hatayein

Isliye jo sabse der se chhua, wo bahar (LRU). Aur har key ki umar bhi (TTL). Key rakho umar ke saath, padho to RAM se.
RAM bhar gayi to sabse der se chhua hatao. Wahi LRU jo LC 146 me.

Ye kyun chalta? RAM bhari to sabse purana chhua pehle jaata, umar khatam = key bekaar. Hati key = sirf ek miss, App DB
se laake wapas rakh deta. Sach DB me, cache sirf tez copy.

LFU (kitni baar chhua) kyun nahi? Kal ki famous key aaj bhi baithi rehti, kyunki purani ginti nahi ghatti. Default LRU.

Hatana hi band kar diya to? Naya likhna error deta (noeviction). Cache me allkeys-lru rakho.

**Pooche: sabse purana har baar dhoondhoge?** Nahi. HashMap + doubly linked list: chhua to aage, peeche wala hatao,
O(1). Wahi LRU jo LC 146 me.

**Pooche: user:9 phir padha?** Utha ke aage. Ab sabse baad me nikalega, chhua = taaza.

**Pooche: Redis sach me poori LRU list rakhta?** Nahi, andaaze wala LRU: kuch random key me sabse purani. Poori list =
har key pe memory.

**Pooche: umar (TTL) kitni rakhoge?** Jitna purana jhel sake. Profile = minute, price / stock = second.

---

*ab ek machine me jagah ka hisaab ho gaya, par data hi ek machine se bada*

## 2. Subah 11 tak data 1 TB: "user:42" kis machine pe?

Isliye kai machine, aur App ke paas cache client jo batata key kis machine pe (consistent hashing). Key ring pe aage
chalti, pehli machine jo mile wo uska ghar. Aakhir ke baad ghoom ke pehli.

Ye kyun chalta? Machine jude ya jaaye to sirf pados ki ~K/N key hilti hain (K = saari key, N = machine). Sirf us hisse
ki ek baar miss, baaki sab hit. Virtual nodes = pados kai machine, bojh ek pe nahi.

Seedha hash % N kyun nahi? N badla to har key ka % N badal jaata. 4 se 5 machine = ~80% key hilti. Ring me N hai hi nahi.

A-M ek pe, N-Z doosre pe kyun nahi? Ek range garam ho jaati (sab "S" naam), ek machine pe bojh. Hash se key bikhar jaati.

**Pooche: machine D judi?** Sirf pados ki key khiski (201..250, C se D pe). Baaki sab hit.

**Pooche: machine A giri?** Uski key aage wali machine pe. Sirf wahi ek baar miss.

**Pooche: A ka saara bojh B pe double?** Har machine ring pe 100-200 jagah baithti (virtual nodes). Bojh kai pados pe bat-ta.

**Pooche: Redis Cluster bhi ring hai?** Nahi. `CRC16(key) % 16384` slot, slot se machine. Machine badhe to sirf kuch slot khiskao.

---

*ab data baant diya, par ek machine mari to?*

## 3. Dopahar machine B mar gayi, uski key ka bojh DB pe

Har machine akeli thi, copy nahi. B mari, B ki saari key (user:42 ...) miss, DB pe achanak bojh, DB dheema.

Isliye har machine ki ek copy (replica, async). B mari to B ki copy B ban jaati. DB bacha.

Ye kyun chalta? Cache me tezi pehle. Copy thodi der baad likhi jaati, aakhri likhai kho jaaye to chalta, kyunki sach
DB me. Machine giri to copy main banti, App ko us hisse me thodi der miss. Aur asar sirf B ki key pe, kyunki har
machine ka apna hissa.

Copy na rakho to? Wahi pehle wala haal, B ki saari key DB pe.

**Pooche: copy ko main banane me kitna?** ~10-30 sec (pakka B mari?). Us beech B ki key DB se, dheema.

**Pooche: copy peeche thi?** Aakhri kuch likhai kho sakti. Cache me chalta, sach DB me.

**Pooche: network toota, do main ban gaye?** Zyadatar machines (quorum) tay karti kaun main. Akeli wali likhna band
(split brain).

**Pooche: copy se padh bhi lo?** Haan, padhai bat-ti. Thoda purana mil sakta, copy peeche chalti.

---

*ab machine marne se nahi girta, par data purana dikh raha*

## 4. Ravi ne naam badla, sabko purana dikh raha

DB me naya "Ravi K", cache me purana "Ravi". Sab log padhte to "Ravi" dekhte.

Isliye pehle DB update, phir cache ki key DELETE (delete on write). Agla padhne wala DB se naya laata, cache me rakhta.

Ye kyun chalta? Likho DB me, cache se hatao. Kuch bhi pahunche, key khaali hi hogi, to agla padhne wala DB se naya
laata. Umar suraksha jaal hai: delete chhoota to bhi utni der hi purana.

Write-through kyun nahi? DB + cache saath likho. Taaza rehta, par likhna dheema.

Write-back kyun nahi? Pehle sirf cache me, DB baad me. Tez hai, par cache mara to data gaya. Paise pe kabhi nahi.

**Pooche: delete fail ho gaya?** Umar (TTL) bachati, utni der hi purana. Pakka chahiye to DB ke log se delete (CDC).

**Pooche: purana beech me wapas bhar gaya?** Thodi der baad phir delete (double delete), ya version se likho.

**Pooche: Ravi ko apna hi purana naam dikha?** Copy peeche hai. Likhne wale ko kuch der primary se padhao.

---

*ab data taaza, par key khatam hote hi bheed*

## 5. 7 baje "home page" key khatam, 1000 log ek saath DB pe

Sabko miss, sab DB pe, 1000 ek saath (stampede). Home page dheema.

Isliye ek jaaye, DB se laaye, cache bhare. Baaki ruk ke cache se (lock). Pehla lock leta (`SET lock:home NX EX 5`),
wahi DB se laata. Baaki 999 yahin cache se.

Ye kyun chalta? Khaali key pe ek hi DB jaata, baaki rukte ya purani value lete. DB pe 1000 ek saath kabhi nahi. Lock
ki umar hai, to lene wala mare to khud khulta. App ko bas thoda dheema ya purana.

Sabko DB jaane do? Wahi pehle wala haal, 1000 ek saath, home page dheema.

Purani de sakte, to key khatam hone hi kyun di? Umar khatam se thoda pehle taaza karo (soft-TTL), key khaali hi na ho.

**Pooche: 999 kitna ruke?** 50-100 ms, ya purani value do. Thodi purani home page chalti.

**Pooche: lock lene wala hi mar gaya?** Lock ki umar (EX 5). Khud khulta, agla le leta. 999 hamesha nahi ruke.

**Pooche: saari key ek saath khatam?** Umar me random daalo (60 min +- 5). Ek saath nahi girti.

**Pooche: key DB me thi hi nahi?** "Nahi hai" bhi cache karo, chhoti umar ke saath. Warna har baar DB (penetration).

**Pooche: sale 12 baje, pata tha na?** Pehle hi cache bhar do + machine badhao. Waqt pata = tayyari pehle.

---

*ab key khaali hone ki bheed sambhal li, par ek key pe hi crore log*

## 6. Raat IPL final, "live:score" key ki machine akeli mar rahi

Ek key, ek machine, crore read har second. Machine A ka CPU full, B aur C khaali. Score atka.

Isliye garam key ki thodi der ki copy har App ki memory me (L1 cache). App 1-2 sec me ek baar Machine A se leta,
Machine A aaram se. Ya key ki kai copy (score#1..#10).

Ye kyun chalta? Garam key ko faila do. Ek key = ek machine, isliye bheed baantni padti. Score 1-2 sec purana chalta,
paisa nahi.

Consistent hashing se bheed bat nahi jaati? Nahi. Ring key baant-ta hai, ek key ki bheed nahi. Ek key = ek machine.

**Pooche: garam key pakdoge kaise?** Pehle pakdo: App me ginti + machine ka metric achanak upar.

**Pooche: L1 me score kitna purana?** 1-2 sec. Score chalta, paisa / stock nahi.

**Pooche: us garam key pe likhna (like)?** Key ke tukde like#0..#9. Kisi ek me likho, padhte waqt jodo.

**Pooche: kitni parat hui?** L1 (App RAM) -> L2 Redis -> DB. Neeche wali dheemi, isliye upar se pehle.

**Pooche: stampede aur hot key me farak?** Stampede = key khaali (lock). Hot key = key bhari, ek machine pe bheed (L1).

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

---

ARCHETYPE F · CONCEPTS: [caching](../../FOUNDATIONS/04_caching.md) · [sharding](../../FOUNDATIONS/06_database_sharding.md) · [replication](../../FOUNDATIONS/05_database_replication.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
