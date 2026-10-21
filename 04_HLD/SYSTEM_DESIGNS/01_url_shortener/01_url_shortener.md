# URL Shortener

> Lamba link do, chhota 7 akshar ka code milta hai. Koi chhote link pe click kare to asli link pe bhej do (302).
> Dil do cheezein: har link ka code alag, aur click pe redirect bahut tez. Log link banate kam, kholte zyada (100 me 99 baar kholna).
>
> Mock me sirf heading + Cue + kahani ki tasveer. Neeche har card ek block: HAL (↳ agar = cross-Q) · KYUN · NAHI (doosra kyun nahi).
> Cross-Q: hal ke har bade shabd pe socho -> gira? · dheema / bheed? · do ek saath? · galat hua? · kaise / kitna?

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c8_2-f3763409.svg)

---

## Shuru

```
FR      lamba link do, 7 akshar ka chhota code milta hai.
        Chhote link pe click kiya to asli link pe bhejo (302)
NFR     har link ka code alag ho. Click pe redirect bahut tez ho. 100 me 99 baar kholna hi hota hai
NUMBERS roz 10 crore naye link (~1,000 / sec). Har link ~100 baar khulta hai (~1 lakh click / sec).
        Ek link ~500 byte, 5 saal me ~180 arab link, ~90 TB. 7 akshar = ~3.5 lakh crore code, ~100 saal.
        Click 100 guna hain, isliye aage cache. 90 TB ek machine me nahi aata, isliye aage shard
PEHLE   sabse simple: App code banata, DB me (code, lamba link) rakhta. Click pe App DB me code dhoondh ke
        asli link pe bhejta
```

> [!IMPORTANT]
> **RAIL: har design isi kram me chalta (8 dabbe, sab file me same)**
> kaam chala → bheed aayi → machine giri → do log ek saath → kuch dheema → data bada → door ke log → bahar ka hamla
>
> | rail | is design me | card |
> |---|---|---|
> | 1 kaam chala | lamba link → 7 akshar code, click pe 302 | Shuru |
> | 2 bheed aayi | sale link viral, har click DB pe | 1 |
> | 3 machine giri | App giri, site band | 2 |
> | 4 do log ek saath | maine aur Ravi ko ek hi code (**is design ka khaas**) | 3 |
> | 5 kuch dheema | click ginti se redirect dheema | 4 |
> | 6 data bada | 90 TB ek machine me nahi · naya link 404 | 5, 6 |
> | 7 door ke log / darwaza | LB gira, region gaya | 7 |
> | 8 bahar ka hamla | spammer 5,000 link / sec | 8 |

---

*ab tootega kahan? sabse pehle padhne pe, kyunki click 100 guna hain*

## 1. Mera sale wala link viral hua, har click DB pe, DB baith gaya

**Rail 2 BHEED AAYI** · Cue: REDIS CACHE (cache-aside)

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c1_1.png?v=4" width="900">

<img src="img/c1_2.png?v=4" width="900">

<img src="img/c1_3.png?v=4" width="900">

</details>

```
HAL     beech me Redis: click pe pehle Redis dekho, mila to wahin se, nahi to DB se laao aur Redis me rakho
        (cache-aside). Naya link banate hi Redis me bhi daal do
        ↳ agar Redis bhar gaya? -> jo link der se nahi khule, unhe nikaalo (LRU)
          Redis me sirf garam link, 180 arab nahi
        ↳ agar link badla, Redis me purana? -> pehle DB badlo, phir Redis se hatao
          link badla ya hataya, dono me yahi. Ulta kiya to beech ka click purana link wapas Redis me
          bhar deta
        ↳ agar Redis hi gir gaya? -> copy (cluster + replica) sambhalti hai
          warna 95% click DB pe, DB phir baith jaata
        ↳ agar Redis ki copy bhi gayi? -> 1000 click me sirf 1 DB tak jaata
          baaki usi ka jawab lete. Tab bhi bojh ho to kuch click mana karo, DB bachao (load shedding)
        ↳ agar link ki expiry hai, Redis me uska kya?
          -> Redis ki umar (TTL) = link ki expiry, dono saath mitte hain
        ↳ agar har App apna cache rakhe, Redis kyun?
          -> link badla to har App ki copy purani reh jaati. Isliye ek shared Redis
        ↳ agar ek link viral hua, uski DB machine garam ho gayi?
          -> wo link pehle hi Redis me mil jaata hai, DB tak kam aata
        ↳ agar bot fake code (/xyz999) pe lakhon click maar raha, sab DB pe jaayenge?
          -> nahi. Code = counter ka number. Itna number diya hi nahi, to App wahin 404, DB tak nahi.
          Deleted / expired link ke liye Redis me "nahi hai" 1 min rakho
KYUN    Redis RAM se ~0.1 ms, DB disk se ~10 ms. 95% click Redis se hi nikal jaate hain.
        Sach ek hi jagah hai, DB me. Redis sirf garam copy hai, khoyi to DB se wapas ban jaati
NAHI    DB ki copy (replica) -> DHEEMA: wo bhi disk se padhti hai, dheemi hi rehti
        Memcached -> GIRA: usme copy nahi, gira to saari bheed seedha DB pe. Redis gira to uski copy sambhalti
```

> [!IMPORTANT]
> **Yaad rakh:** Sach DB me. Redis sirf garam copy: click pehle Redis, miss pe DB se laao aur Redis me rakho.

> [!TIP]
> **Cross-Q: sawaal → cue (bas itna yaad)**
> - Redis bhar gaya? → **LRU, sirf garam link**
> - Link badla, Redis me purana? → **pehle DB, phir Redis se hatao**
> - Redis gira? → **copy (cluster + replica)**
> - Copy bhi gayi, saari bheed DB pe? → **single flight (1 jaaye, baaki uska jawab) + load shedding**
> - Link expiry? → **Redis TTL = link ki expiry**
> - Har App apna cache kyun nahi? → **link badla to purana reh jaata, isliye shared Redis**
> - Bot fake code pe lakhon click? → **itna number diya hi nahi, App wahin 404**
> - Memcached kyun nahi? → **copy nahi, gira to bheed DB pe**


---

*ab App hi ek hai, wo gir gaya to?*

## 2. Usi bheed me ek App machine giri, poori site band

**Rail 3 MACHINE GIRI** · Cue: STATELESS App x N + LOAD BALANCER

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c2_1.png?v=4" width="900">

<img src="img/c2_2.png?v=4" width="900">

<img src="img/c2_3.png?v=4" width="900">

</details>

```
HAL     kai App aur aage Load Balancer (LB), jo baari-baari kisi bhi App ko bhejta (round-robin).
        App kuch yaad nahi rakhta (stateless), sab Redis / DB me, to koi bhi App koi bhi click le sakti
        ↳ agar App giri nahi, bas dheemi? -> jiske kam connection khule, use bhejo (least-conn)
        ↳ agar giri hui App theek ho gayi? -> /health phir haan bolta, LB use wapas list me le leta
KYUN    App giri to /health ka jawab nahi aata, LB use list se hata deta, agla click doosri App pe
NAHI    ek badi machine -> GIRA: kitni bhi badi ho, ek hi hai. Giri to poori site band
        login App me hi yaad -> GIRA: user usi App se bandh jaata, wo giri to logout.
           Isliye session bahar, Redis / DB me
```

> [!IMPORTANT]
> **Yaad rakh:** App kuch yaad nahi rakhti (stateless). LB /health poochta, chup App list se bahar.

> [!TIP]
> **Cross-Q: sawaal → cue (bas itna yaad)**
> - App giri nahi, bas dheemi? → **least-conn**
> - Giri App theek ho gayi? → **/health haan, wapas list me**
> - Login App me yaad rakhein? → **nahi, session Redis me**


---

*ab kai App hain, sab code bana rahe, takkar?*

## 3. Maine aur Ravi ne link banaya, dono ko ek hi code mila

**Rail 4 DO LOG EK SAATH (khaas)** · Cue: COUNTER RANGE + base62

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c3_1.png?v=4" width="900">

<img src="img/c3_2.png?v=4" width="900">

<img src="img/c3_3.png?v=4" width="900">

</details>

```
HAL     Counter har App ko alag range deta: App-1 ko 1-1000, App-2 ko 1001-2000. App apni range se agla
        number leta. Ek range kabhi do App ko nahi, to code kabhi same nahi
        ↳ agar do log ek saath same code maangein? -> DB ka UNIQUE ek ko rokta, 409
          user apna code maange: lambai dekho, admin jaise reserved naam nahi, pehle se na ho
        ↳ agar App-1 range me 400 pe crash? -> 401 se 1000 bekaar, jaan-boojh ke
          3.5 lakh crore me kuch hazaar se farak nahi
        ↳ agar Counter hi gir gaya? -> App ki range chalti rehti hai
          tab tak Counter ki copy uth jaati
        ↳ agar range khatam ho gayi? -> tabhi Counter (ZooKeeper / DB) se agli range lo
        ↳ agar MD5 ya random code lagate?
          -> do link ka code same ho sakta hai, har baar "pehle se hai?" dekhna padta. Counter me nahi
        ↳ agar counter se agla code andaza ho jaaye?
          -> haan, ye keemat hai. Chahiye to number pehle shuffle / XOR karo, phir base62. Unique phir bhi
        ↳ agar do log same lamba link dein?
          -> dono ko alag code, har ek ki apni ginti, apni expiry. Ek hi chahiye to long_url ke hash pe
          pehle dekh lo
        ↳ agar Redis INCR se number lete?
          -> chal jaata, par har link pe ek call. Range me 1000 link pe ek call
        ↳ agar har number bachana ho, ek bhi bekaar na jaaye?
          -> har link pe Counter se poochho. Phir range ka fayda khatam
BASE62  number ko 62 akshar (0-9, a-z, A-Z) me likho. 125 = "21", 100 crore = "15FTGg"
KYUN    counter kabhi dohraata nahi, to code bhi nahi dohraata, check ki zaroorat hi nahi.
        Range ke andar App khud number deta, har link pe Counter tak nahi jaata. 1000 link pe ek call
NAHI    UUID -> LAMBA: 36 akshar, link chhota karna hi to kaam tha
        lambe link ka hash, pehle 7 akshar -> TAKKAR: do link ke same aa sakte, har baar DB me dekho.
           Counter me takkar hoti hi nahi
```

> [!IMPORTANT]
> **Yaad rakh:** Counter har App ko range deta, App range se agla number, phir base62. Takkar kabhi nahi.

> [!TIP]
> **Cross-Q: sawaal → cue (bas itna yaad)**
> - Do log same custom code maangein? → **DB UNIQUE, 409**
> - App range ke beech crash? → **bache number bekaar, chalta hai**
> - Counter gira? → **App ki range chalti + Counter ki copy**
> - MD5 / random kyun nahi? → **takkar, har baar DB check**
> - Agla code andaza ho jaaye? → **shuffle / XOR, phir base62**
> - UUID kyun nahi? → **36 akshar, lamba**


---

*ab click gin-ne gaye to redirect dheema*

## 4. Maine click ginti chahi, redirect hi dheema ho gaya

**Rail 5 KUCH DHEEMA** · Cue: KAFKA, ginti peeche

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c4_1.png?v=4" width="900">

<img src="img/c4_2.png?v=4" width="900">

<img src="img/c4_3.png?v=4" width="900">

</details>

```
HAL     redirect turant (302), "ek click hua" ki khabar Kafka me daal ke aage. Analytics service apni speed
        se Kafka se padh ke ginti karti
        ↳ agar 302 kyun, 301 kyun nahi? -> 301 pe browser yaad rakhta, ginti band
          301 pe browser link yaad rakh leta, agli baar server tak aata hi nahi.
          302 pe har click server tak aata, ginti aur expiry chalti
        ↳ agar Analytics dheema ho gaya? -> khabar Kafka me padi rehti
          wo apni speed se padhta, kahan tak padha yaad rakhta (offset)
        ↳ agar wahi click do baar aaya? -> har click ki apni id, dobara to ginti nahi
        ↳ agar Kafka me koi khabar kharab nikli? -> use alag line me daalo (DLQ), baaki ginti chalti rahe
        ↳ agar Kafka hi band ho gaya? -> redirect phir bhi chalta
          bas kuch click ginti me nahi aate
        ↳ agar naya report chahiye? -> Kafka me pade purane click dobara padh lo (replay)
        ↳ agar Kafka kyun, SQS kyun nahi?
          -> SQS padhte hi message hata deta. Kafka rakhta hai, naya report = dobara padho
KYUN    redirect kisi ka intezaar nahi karta. Ginti peeche hoti, disk pe pakki. Ginti wala dheema / band ho,
        to bhi link khulta hai, ginti thodi der se aati
NAHI    click seedha DB me -> DHEEMA: har redirect DB likhai ka intezaar karta, wahi dheemapan
        sirf Redis me INCR -> ADHOORA: sirf ginti milti, kaun, kab, kahan se click hua, wo nahi
```

> [!IMPORTANT]
> **Yaad rakh:** Redirect turant (302), "click hua" Kafka me, ginti peeche Analytics apni speed se.

> [!TIP]
> **Cross-Q: sawaal → cue (bas itna yaad)**
> - 301 kyun nahi? → **browser yaad rakhta, ginti band**
> - Analytics dheema? → **Kafka me padi, offset se aage**
> - Wahi click do baar? → **click id, ginti ek**
> - Kharab khabar? → **DLQ**
> - Kafka hi band? → **redirect chalta, bas ginti kam**
> - SQS kyun nahi? → **padhte hi mita deta, replay nahi**


---

*ab data hi itna ho gaya*

## 5. DB team boli: 5 saal me 90 TB, ek machine me nahi

**Rail 6 DATA BADA** · Cue: SHARD by code (RING)

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c5_1.png?v=5" width="900">

<img src="img/c5_2.png?v=5" width="900">

<img src="img/c5_3.png?v=5" width="900">

</details>

```
HAL     data kai machine me baanto (shard), link ke code se. Code ring pe aage chalta, jo pehli machine mile
        wo uski (consistent hashing). Har tukde ki 3 copy
        ↳ agar data sirf ~500 GB hota? -> ek MySQL kaafi. Shard tab, jab ek machine me na aaye
        ↳ agar ek machine mar gayi? -> baaki 2 copy se milta, link nahi gaye
        ↳ agar ek machine pe zyada bojh? -> ring pe 100-200 jagah (virtual nodes)
          har machine ring pe 100-200 jagah baithti, bojh barabar
        ↳ agar Cassandra kyun, MySQL kyun nahi?
          -> join aur transaction chahiye hi nahi, bas code se link, aur 90 TB.
          Wajah kaam ka dhaancha + size, "NoSQL powerful" nahi
        ↳ agar Cassandra me main copy kaunsi hai? -> koi nahi. Teeno barabar, koi bhi likh / padh sakti
        ↳ agar AWS pe Cassandra kahan? RDS pe? -> NAHI, RDS sirf SQL (MySQL / Postgres).
          Cassandra = Amazon Keyspaces ya EC2 pe khud; ya seedha DynamoDB (key-value, AWS ka apna)
        ↳ agar copy badha do to likhna tez hoga?
          -> nahi, har copy pe likhna padta. Likhne ka bojh = shard, copy = bachav
KYUN    click hamesha code ke saath aata hai, to har click seedha ek machine pe jaata
NAHI    country se baanto -> BIKHRA: code dekh ke pata nahi kis country ka, saari machine poochni padti
        ek badi machine -> CHHOTI: 90 TB ek machine me aata hi nahi
        seedha code % machine -> HILTA: machine judi to lagbhag saari keys hil jaati (3 se 4 = sab badle).
           Ring me sirf pados ki keys hilti
```

> [!IMPORTANT]
> **Yaad rakh:** Code ka hash ring pe, pehli machine uski, agli 2 machine pe copy (Cassandra).

> [!TIP]
> **Cross-Q: sawaal → cue (bas itna yaad)**
> - Data sirf 500 GB? → **ek MySQL kaafi**
> - Ek machine mari? → **ring ke padosi pe copy**
> - Ek machine pe zyada bojh? → **virtual nodes**
> - Cassandra kyun, MySQL kyun nahi? → **join / transaction nahi + 90 TB**
> - Main copy kaunsi? → **koi nahi, teeno barabar**
> - Copy badhao to likhna tez? → **nahi, likhne ka bojh = shard**
> - code % N kyun nahi? → **machine judi to sab keys hilti**

> [!CAUTION]
> **AWS pe Cassandra kahan? RDS pe? → NAHI.** RDS sirf SQL (MySQL · Postgres · MariaDB · Oracle · SQL Server).
> Cassandra → **Amazon Keyspaces** ya EC2 pe khud · key-value → **DynamoDB** · Mongo → **DocumentDB** · Redis → **ElastiCache**


---

*ab har tukde ki 3 copy, par naya link 404?*

## 6. Maine naya link banaya, turant khola, 404 aaya

**Rail 6 DATA BADA (copy ka AGAR)** · Cue: QUORUM (2 me likho, 2 se padho)

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c6_1.png?v=5" width="900">

<img src="img/c6_2.png?v=5" width="900">

<img src="img/c6_3.png?v=5" width="900">

</details>

```
HAL     likhna 2 copy me, padhna bhi 2 copy se (quorum, 3 me se 2). Har jodi me ek taaza copy zaroor hoti
        ↳ agar likhte waqt copy C gir gayi? -> 2 me ho gaya to save
          C uthi to padosi ke paas rakha note use mil jaata (hinted handoff)
        ↳ agar padhte waqt A aur C me farak nikla? -> wahin C ko theek kar do
          (read repair)
        ↳ agar A aur B dono gir gayi, sirf 1 bachi? -> likhna fail, user ko error
          (ya 1 pe likho, risk ke saath). Par jo "done" bola, wo kabhi khota nahi
        ↳ agar 2 copy se padhna dheema nahi? -> thoda. Isliye pehla ilaaj Redis hai, DB tak kam aata
        ↳ agar 5 copy hon to quorum kitna?
          -> likhne + padhne ki ginti copy se zyada ho (W + R > N): 3 + 3 > 5
        ↳ agar data bachane ko Kafka laga dein? -> nahi. DB ka apna log ye khud karta, extra dabba nahi
MISAAL  A, B me likha, A + C se padha, to A me mil gaya
KYUN    2 + 2 = 4, copy sirf 3. Padhne wali jodi me likhne wali ek copy aani hi hai. Banane ke baad 404 nahi
NAHI    teeno me likho -> RUKA: ek copy dheemi ya giri to har likhai ruk jaati
        ek se padho -> PURANA: wahi copy mil sakti jahan abhi pahuncha nahi, wahi 404
```

> [!IMPORTANT]
> **Yaad rakh:** 3 copy: 2 me likho, 2 se padho (W + R > N). Har jodi me ek taaza copy.

> [!TIP]
> **Cross-Q: sawaal → cue (bas itna yaad)**
> - Likhte waqt copy C giri? → **2 kaafi, C ke liye note (hinted handoff)**
> - Padhte waqt A, C me farak? → **read repair**
> - 2 copy gir gayi? → **likhna fail, error**
> - 5 copy ho to? → **3 + 3 > 5**
> - Teeno me likho? → **ek giri to likhai ruki**


---

*ab LB hi ek hai*

## 7. Shaam ko LB hi gir gaya, koi request andar nahi

**Rail 7 DARWAZA / DOOR KE LOG** · Cue: ROUTE 53 + 2 LB (health check)

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c7_1.png?v=4" width="900">

<img src="img/c7_2.png?v=4" width="900">

<img src="img/c7_3.png?v=4" width="900">

</details>

```
HAL     do LB, aage Route 53. Route 53 har ~30 sec LB se poochta hai (health check). Jo 3 baar chup raha,
        wo list se bahar, sab LB-2 pe (DNS failover)
        ↳ agar phone ne purana pata yaad rakha? -> ~60 sec kuch request fail
          (DNS TTL). Isliye LB khud kai zone me (ALB), aur har jagah 2+
        ↳ agar poora region hi chala gaya? -> Route 53 doosre region pe bhejta
          naye link ke aakhri kuch second kho sakte
KYUN    phone sabse pehle Route 53 se hi poochta hai "kahan jaun?". Wahin se zinda LB ka pata mil jaata
NAHI    ek hi LB -> GIRA: wo gira to App kitne bhi hon, koi andar nahi aa sakta
```

> [!IMPORTANT]
> **Yaad rakh:** 2 LB, aage Route 53. Health check pe 3 baar chup = bahar.

> [!TIP]
> **Cross-Q: sawaal → cue (bas itna yaad)**
> - Phone ne purana pata yaad rakha? → **DNS TTL ~60 sec, LB kai zone me**
> - Poora region gaya? → **Route 53 doosre region pe**


---

*ab koi galat istemal kare to?*

## 8. Raat ko spammer ki script, har second 5,000 link

**Rail 8 BAHAR KA HAMLA** · Cue: RATE LIMIT darwaze pe + WAF

<details><summary>kahani dekho (3 scene)</summary>

<img src="img/c8_1.png?v=4" width="900">

<img src="img/c8_2.png?v=4" width="900">

<img src="img/c8_3.png?v=4" width="900">

</details>

```
HAL     gateway pe hi ginti: har API key ki ek shared ginti (Redis me). Had paar to 429,
        request andar aati hi nahi (rate limit)
        ↳ agar darwaze pe pehchaana hua hamla aaya? -> WAF darwaze pe hi rok deta
KYUN    rokna darwaze pe, sabki ek ginti se. Asli users ki speed bachi rehti
NAHI    har App apni ginti -> DHEELA: 10 App = had 10 guna, aur bheed App tak aa chuki hoti
        sirf IP pe had -> BADLA: bot naya IP le aata. Isliye ginti API key / user_id pe, jo bot badal na sake
```

> [!IMPORTANT]
> **Yaad rakh:** Gateway pe sabki ek ginti (Redis), had paar to 429, andar aaya hi nahi. Pehchaana hamla WAF pe.

> [!TIP]
> **Cross-Q: sawaal → cue (bas itna yaad)**
> - Har App apni ginti? → **had N guna dheeli**
> - Ginti IP pe? (college ek IP) → **nahi, user_id / API key**
> - Ban kitni der? → **TTL (jaise 24 ghante), permanent nahi**


Poora rate limit = [02_rate_limiter](../02_rate_limiter/02_rate_limiter.md).

---

## Poora design ek saans me

```
User -> Route 53 -> 2 LB -> WAF -> API Gateway (rate limit + login) -> App (stateless)
     -> Redis (95% click) -> Cassandra (code se baanta, 3 copy, 2 me likho 2 se padho)
NAYA LINK: App -> range se agla number -> base62 -> Redis + Cassandra -> chhota link
CLICK:     Redis me mila? -> 302 · nahi -> Cassandra -> Redis me rakho -> 302
           click ki khabar -> Kafka -> Analytics (kharab -> DLQ), redirect ruke bina
```

Bolne ke liye: short code ek badhta number hai, 62 akshar me likha. Har server ko numbers ki apni range milti, to do
server kabhi same code nahi banate. 100 me 99 baar link khulta, isliye khulna zyadatar Redis se, baaki Cassandra se.
Redirect turant, ginti peeche Kafka se. Do LB + Route 53, taaki ek gire to sab na gire. Darwaze pe had.

---

## 10 guna traffic

- **Route 53**: user ko uske paas wale region pe bhejo (geo-routing).
- **App**: stateless hai, machine badhao. Padhne wale aur likhne wale App alag (padhne wale ~20, likhne wale 2).
- **Redis**: saare link nahi samaate. Garam rakho, thande nikaalo (LRU + expiry). Thanda link khula to Cassandra se wapas.
- **Cassandra**: code se baanto, har tukde ki 3 copy. Padhne ka bojh badha to aur machine / copy.
- **Counter**: 2 copy, ek chalu ek taiyaar. Range waise bhi thodi der chala leti.
- **Kafka**: partition badhao.

**Pooche: kahan se sab gir sakta?** LB (2 copy), Counter (2 copy), Route 53 health check.

**Pooche: chal raha hai, kaise pata?** Redirect ka time (p99), galtiyon ki ginti, Kafka me kitna pending, alert.

---

## Pooche to (kisi card me nahi baitha)

API: link banana = POST /api/shorten { long_url, custom_code? } -> { short_url, expires_at }.
Link kholna = GET /abc123 -> 302 + Location. Banana POST, kholna GET.

Row: short_code (isi se machine tay) · long_url · created_at · expires_at.

Kaunsa DB kitne tak: MySQL ~1 arab row tak aaram se, Mongo ~10 arab tak, Cassandra / DynamoDB usse bahut aage.

Shabd bhool jao to seedhi baat bolo: "MD5" = lambe link ka hash, pehle 7 akshar. "Base62" = 62 akshar me number likh do.
"Counter" = ek badhta number, har link ko agla. Interviewer ko samajh chahiye, naam wo khud bol dega.

Koi phishing ka lamba link chhota kare? Chhota karne se pehle check karo (virus / dhokhe ki site),
taaki hamara domain spam na bane.

---

ARCHETYPE F (infra/component) · CONCEPTS: [ID-gen](../../FOUNDATIONS/13_distributed_id_snowflake.md) · [caching](../../FOUNDATIONS/04_caching.md) · [sharding](../../FOUNDATIONS/06_database_sharding.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
