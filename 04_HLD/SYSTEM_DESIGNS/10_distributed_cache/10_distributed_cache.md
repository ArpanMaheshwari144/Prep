# Distributed Cache (Redis-jaisa)

> Tez RAM wala key-value store, jo ek machine se bada ho aur ek machine mare to bhi chale. Concept detail = FOUNDATIONS/04_caching.md
> Dil do cheezein: tez (<1 ms), aur machine mare to bhi chale. BookMyShow ka ulta: wahan sahi hona dil tha, yahan tezi.
>
> Mock me sirf heading + Cue + kahani ki tasveer. Neeche har card: HAL · KYUN · NAHI, phir YAAD RAKH dabba + CROSS-Q dabba (sawaal → cue, poora jawab).
> Cross-Q: hal ke har bade shabd pe socho -> gira? · dheema / bheed? · do ek saath? · galat hua? · kaise / kitna?

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c6_2-4533d6f5.svg)

---


## Shuru

```
FR      get(key), put(key, value, ttl), delete(key). Disk pe bachana, pub/sub aur transactions scope se bahar
NFR     <1 ms (yahi dil hai), ek machine mare to bhi chale, TB bhar data. "Thodi der baad sahi" (eventual)
        chalega, asli sach DB me hai
NUMBERS ~1 TB data, ~10 lakh read / sec, padhai zyada. DB se ~10 ms lagta hai (disk), 10 lakh / sec pe DB
        ghutno pe aa jaata hai, isliye aage RAM cache
        1 TB ek machine ki RAM (64 GB) me nahi aata, to baanto (sharding)
        10 lakh / sec ek machine nahi deti, to machines me baanto
        machine mari to saara bojh DB pe, isliye har machine ki copy (replication)
        teeno ne "ek machine kaafi" khaarij kar diya, isliye distributed
PEHLE   sabse simple: ek machine, uski RAM me ek HashMap, get / put O(1)
```

> [!IMPORTANT]
> **RAIL: har design isi kram me chalta (8 dabbe, sab file me same)**
> kaam chala → bheed aayi → machine giri → do log ek saath → kuch dheema → data bada → door ke log → bahar ka hamla
>
> | rail | is design me | card |
> |---|---|---|
> | 1 kaam chala | get / put, RAM bhar gayi, kya hatayein | 1 |
> | 2 bheed aayi | 7 baje home key khatam, 1000 log DB pe · IPL final, ek key pe crore log | 5, 6 |
> | 3 machine giri | machine B mari, uski key DB pe (**is design ka khaas: machine mare to bhi chale**) | 3 |
> | 4 do log ek saath | Ravi ne naam badla, DB me naya, cache me purana · delete ke beech purana wapas bhara (card 4 ka cross-Q) | 4 |
> | 5 kuch dheema | alag nahi: DB ~10 ms, isliye aage cache (Shuru) | - |
> | 6 data bada | 1 TB, "user:42" kis machine pe | 2 |
> | 7 door ke log | nahi aata | - |
> | 8 bahar ka hamla | jo key DB me hai hi nahi, baar-baar maango (card 5 ka cross-Q) | 5 |


> [!NOTE]
> **AWS pe kaunsa DB (interview me naam bolna ho to)** · SQL wala = RDS, Cassandra RDS pe nahi chalta
>
> - Ye khud cache hai → **ElastiCache (Redis)**: AWS pe Redis jaisa managed cache yahi

---

*ab tootega kahan? sabse pehle RAM pe, kyunki RAM chhoti hai*

## 1. Subah cache machine ki RAM bhar gayi, kya hatayein

**Rail 1 KAAM CHALA** · Cue: LRU + TTL (HashMap + DLL)

<img src="img/c1_poster.png?v=1" width="1000" alt="card 1: kya toota, kaise bacha">


```
HAL     jo sabse der se chhua gaya wo bahar (LRU), aur har key ki umar (TTL). RAM bhari to sabse der se
        chhua hatao. Wahi LRU jo LC 146 me hai
        cache-aside: App pehle cache se poochhta hai. GET profile:42 mili (HIT) to <1 ms, DB tak nahi.
        Nahi mili (MISS) to DB se (~10 ms), cache me TTL ke saath rakho, agli baar HIT
KYUN    RAM bhari to sabse purana chhua pehle jaata hai, umar khatam to key bekaar. Hati key sirf ek miss
        hai, App DB se laake wapas rakh deta hai
        sach DB me hai, cache sirf tez copy. Cache khaali ho tab bhi sahi data DB se milta hai,
        App ko farak sirf tezi ka padta hai
NAHI    LFU (kitni baar chhua)
        -> ATKA: kal ki famous key aaj bhi baithi rehti hai, purani ginti nahi ghatti. Default LRU
        hatana hi band (noeviction)
        -> ERROR: naya likhna error deta hai. Cache me allkeys-lru rakho
```

> [!IMPORTANT]
> **Yaad rakh:** RAM bhari to sabse der se chhua bahar (LRU), har key ki umar (TTL). HashMap + list = O(1). Sach DB me.


---

*ab ek machine me jagah ka hisaab ho gaya, par data hi ek machine se bada*

## 2. Subah 11 tak data 1 TB: "user:42" kis machine pe?

**Rail 6 DATA BADA** · Cue: CONSISTENT HASHING + virtual nodes

<img src="img/c2_poster.png?v=1" width="1000" alt="card 2: kya toota, kaise bacha">


```
HAL     kai machine hain, App ke paas cache client jo batata hai key kis machine pe hai (consistent hashing).
        Key ring pe aage chalti hai, pehli machine uska ghar. Aakhir ke baad ghoom ke pehli
KYUN    machine jude ya jaaye to sirf pados ki ~K/N key hilti hain (K = saari key, N = machine).
        Sirf us hisse ki ek baar miss, baaki hit
        virtual nodes se pados kai machine ban jaati hain, bojh ek pe nahi padta
NAHI    seedha hash % N
        -> HILTA: N badla to har key ka % N badla. 4 se 5 machine = ~80% key hilti. Ring me N hai hi nahi
        A-M ek pe, N-Z doosre pe
        -> GARAM: ek range garam (sab "S" naam), ek machine pe bojh. Hash se key bikhar jaati hai
```

> [!IMPORTANT]
> **Yaad rakh:** Key ka hash ring pe, aage chalo, pehli machine ghar. Machine jude / jaaye to sirf pados ki key hilti.


---

*ab data baant diya, par ek machine mari to?*

## 3. Dopahar machine B mar gayi, uski key ka bojh DB pe

**Rail 3 MACHINE GIRI (khaas)** · Cue: REPLICA (async) + quorum failover

<img src="img/c3_poster.png?v=1" width="1000" alt="card 3: kya toota, kaise bacha">


```
HAL     har machine ki ek copy hai (replica, async). B mari to B ki copy B ban jaati hai. DB bach gaya
KYUN    cache me tezi pehle hai. Copy thodi der baad likhi jaati hai, aakhri likhai khoye to chalta hai,
        sach DB me hai
        machine giri to copy main ban jaati hai, App ko us hisse me thodi der miss. Asar sirf B ki key pe,
        har machine ka apna hissa hai
NAHI    copy na rakho
        -> BOJH: pehle yahi tha, B mari, B ki saari key (user:42 ...) miss, DB pe achanak bojh, DB dheema
```

> [!IMPORTANT]
> **Yaad rakh:** Har machine ki async copy. B mari to ~10-30 sec me copy main. Network toota to zyadatar (quorum) chunte.


---

*ab machine marne se nahi girta, par data purana dikh raha*

## 4. Ravi ne naam badla, sabko purana dikh raha

**Rail 4 DO LOG EK SAATH** · Cue: DB me likho, cache key DELETE

<img src="img/c4_poster.png?v=1" width="1000" alt="card 4: kya toota, kaise bacha">


```
HAL     pehle DB update, phir cache ki key DELETE (delete on write). Agla padhne wala DB se naya laata hai,
        cache me rakhta hai
KYUN    likho DB me, cache se hatao. Kuch bhi pahunche, key khaali hai, to agla DB se naya laayega.
        Umar suraksha jaal hai: delete chhoot gaya to bhi utni der hi purana dikhega
NAHI    sirf DB update karo
        -> PURANA: DB me "Ravi K", cache me purana "Ravi", sab "Ravi" dekhte hain
        write-through (DB + cache saath likho)
        -> DHEEMA: taaza rehta hai, par likhna dheema
        write-back (pehle sirf cache, DB baad me)
        -> KHOYA: tez hai, par cache mara to data gaya. Paise pe kabhi nahi
```

> [!IMPORTANT]
> **Yaad rakh:** Pehle DB update, phir cache key DELETE. Delete fail ho to umar (TTL) bachati, utni der hi purana.


---

*ab data taaza, par key khatam hote hi bheed*

## 5. 7 baje "home page" key khatam, 1000 log ek saath DB pe

**Rail 2 BHEED AAYI (key khaali)** · Cue: LOCK: SET NX EX, sirf pehla DB jaaye

<img src="img/c5_poster.png?v=1" width="1000" alt="card 5: kya toota, kaise bacha">


```
HAL     ek jaaye, DB se laaye, cache bhare, baaki ruk ke cache se padhein (lock). Pehla lock leta hai
        (SET lock:home NX EX 5), wahi DB jaata hai. Baaki 999 cache se
KYUN    khaali key pe ek hi DB jaata hai, baaki rukte hain ya purani value lete hain.
        DB pe 1000 ek saath kabhi nahi
        lock ki umar hai, to lene wala mare to lock khud khulta hai. App ko bas thoda dheema ya purana
        milta hai
NAHI    sabko DB jaane do
        -> BHEED: sabko miss, 1000 ek saath (stampede), home page dheema
```

> [!IMPORTANT]
> **Yaad rakh:** Khaali key pe pehla lock leta (SET NX EX 5), wahi DB jaata, baaki ruk ke cache se. Lock khud khulta.


---

*ab key khaali hone ki bheed sambhal li, par ek key pe hi crore log*

## 6. Raat IPL final, "live:score" key ki machine akeli mar rahi

**Rail 2 BHEED AAYI (garam key)** · Cue: L1 LOCAL CACHE + key ki copies

<img src="img/c6_poster.png?v=1" width="1000" alt="card 6: kya toota, kaise bacha">


```
HAL     garam key ki thodi der ki copy har App ki memory me (L1 cache). App 1-2 sec me ek baar Machine A
        se leta hai, A aaram se. Ya key ki kai copy (score#1..#10)
KYUN    garam key ko faila do. Ek key = ek machine, crore read / sec pe A ka CPU full, B aur C khaali.
        Isliye bheed baantni padti hai
        score 1-2 sec purana chalta hai, paisa nahi hai
NAHI    consistent hashing se bheed bat jaayegi
        -> GALAT: ring key baant-ta hai, ek key ki bheed nahi. Ek key = ek machine
```

> [!IMPORTANT]
> **Yaad rakh:** Garam key ki 1-2 sec copy har App ki RAM me (L1). Likhna ho to tukde like#0..#9, padhte waqt jodo.


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
