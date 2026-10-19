# URL Shortener

> Lamba link do, chhota 7 akshar ka code milta hai. Koi chhote link pe click kare to asli link pe bhej do (302).
> Dil do cheezein: har link ka code alag, aur click pe redirect bahut tez. Log link banate kam, kholte zyada (100 me 99 baar kholna).
>
> Har card: kya toota -> kya kiya -> yahi kyun -> doosra kyun nahi. Kram me chalo. "Pooche" wali line = interviewer aage poochhe to.

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c8_2-f3763409.svg)

---

## Mock cue (mock me sirf ye dekho: kya hua -> ilaaj -> kaunsa dabba juda)

```
   KYA HUA                                     ILAAJ                                DABBA JUDA
1  mera sale link viral, DB baith gaya      -> REDIS CACHE (cache-aside)            Redis
2  usi bheed me ek App giri, site band      -> STATELESS App x N + LOAD BALANCER    LB + App x N
3  main aur Ravi, dono ko ek hi code        -> COUNTER RANGE + base62               Counter
4  click ginti likhi, redirect dheema       -> KAFKA, ginti peeche                  Kafka + Analytics + DLQ
5  DB team: 5 saal me 90 TB                 -> SHARD by code (RING)                 Cassandra
6  naya link banaya, turant khola, 404      -> QUORUM (2 me likho, 2 se padho)      (Cassandra quorum)
7  shaam ko LB hi gira                      -> ROUTE 53 + 2 LB (health check)       Route 53 + LB x2
8  raat ko spammer, 5,000 link / sec        -> RATE LIMIT darwaze pe + WAF          API Gateway + WAF

POORA DESIGN (upar ke dabbe kram se):
   User -> Route 53 -> LB x2 -> WAF -> API Gateway -> App x N (Counter se range) -> Redis -> Cassandra
   click ki khabar: App -> Kafka -> Analytics (fail -> DLQ)
```

---

## Shuru

Sabse simple: User link deta, App ek code banata, DB me (code, lamba link) rakh deta. Click aaya to App DB me code
dhoondhta aur asli link pe bhej deta.

Code kaise bane? Ek ginti (counter) 1, 2, 3... Har naye link ko agla number, aur us number ko 62 akshar
(0-9, a-z, A-Z) me likh do (base62). 125 = "21", 100 crore = "15FTGg". Counter kabhi dohraata nahi, to code bhi
kabhi dohraata nahi, check ki zaroorat hi nahi.

Numbers: roz 10 crore naye link, yaani ~1,000 har second. Har link ~100 baar khulta, to ~1 lakh click har second.
Ek link ~500 byte, 5 saal me ~180 arab link, ~90 TB. 7 akshar se ~3.5 lakh crore code bante, ~100 saal chalega.
Click naye link se 100 guna, isliye aage cache lagega. 90 TB ek machine me nahi aata, isliye aage shard lagega.

**Pooche: 302 kyun, 301 kyun nahi?** 301 pe browser link yaad rakh leta, agli baar server tak aata hi nahi, ginti band.
302 pe har click server tak aata, ginti aur expiry chalti.

**Pooche: MD5 ya random code kyun nahi?** Do link ka code same ho sakta, har baar "pehle se hai?" dekhna padta. Counter me nahi.

**Pooche: counter se agla code andaza nahi ho jaayega?** Haan, ye keemat hai. Chahiye to number ko pehle shuffle / XOR
karo, phir base62. Unique phir bhi rehta.

**Pooche: user apna code maange to?** Lambai dekho, admin jaise reserved naam nahi, pehle se na ho. Do log ek saath
maange to DB ka UNIQUE ek ko rokta, use 409.

**Pooche: data ~500 GB hota to?** Ek MySQL kaafi. Shard tab jab ek machine me na aaye.

**Pooche: do log same lamba link de to?** Alag code do, har ek ki apni ginti, apni expiry. Ek hi chahiye to
long_url ke hash pe pehle dekh lo.

---

*ab tootega kahan? sabse pehle padhne pe, kyunki click 100 guna hain*

## 1. Mera sale wala link viral hua, har click DB pe, DB baith gaya

Isliye beech me Redis lagaya. Click aata hai to pehle Redis me dekhte hain. Mil gaya to wahin se jawab, nahi mila to
DB se laate hain aur Redis me rakh dete hain (cache-aside). Naya link banate hi Redis me bhi daal dete hain.

Redis hi kyun? Redis RAM se padhta hai, ~0.1 ms. DB disk se, ~10 ms. Aur 95% click Redis se hi nikal jaate hain.

DB ki copy (replica) kyun nahi laga di? Copy bhi disk se hi padhti hai, to dheemi hi rehti.

Memcached bhi to cache hai, wo kyun nahi? Usme copy nahi hoti. Redis gira to uski copy kaam sambhal leti hai,
Memcached gira to saari bheed seedha DB pe.

Sach ek hi jagah hai, DB me. Redis sirf garam copy hai, khoyi bhi to DB se wapas ban jaati.

**Pooche: Redis bhar gaya to?** Jo link der se nahi khule, unhe nikaal do (LRU). Redis me sirf garam link, 180 arab nahi.

**Pooche: link ki expiry?** Redis ki umar (TTL) = link ki expiry. Dono saath mitte hain.

**Pooche: link badla ya hataya to?** Pehle DB, phir Redis se hatao. Ulta kiya to beech ka click purana link wapas Redis me bhar deta.

**Pooche: Redis hi gir gaya to?** Uski copy (cluster + replica) sambhalti hai. Warna 95% click DB pe, DB phir baith jaata.

**Pooche: copy bhi gayi to?** Ek link ke 1000 click me sirf 1 DB tak jaata, baaki usi ka jawab lete. Tab bhi bojh ho
to kuch click mana karo, DB bachao (load shedding).

**Pooche: har App apna cache kyun nahi rakhta?** Link badla to har App ki copy purani reh jaati. Isliye ek shared Redis.

**Pooche: ek link viral, uski DB machine garam?** Wo pehle hi Redis me mil jaata, DB tak kam aata.

**Pooche: bot fake code (/xyz999) pe lakhon click, sab DB pe?** Code = counter ka number; itna number diya hi
nahi to App wahin 404, DB tak nahi. Deleted / expired ke liye Redis me "nahi hai" 1 min rakho.

---

*ab App hi ek hai, wo gir gaya to?*

## 2. Usi bheed me ek App machine giri, poori site band

Isliye kai App lagaye aur aage Load Balancer (LB). LB har request baari-baari kisi bhi App ko deta (round-robin).
App kuch yaad nahi rakhta (stateless), sab Redis / DB me, isliye koi bhi App koi bhi click le sakta.

Ye kyun chalta? Koi App giri to /health ka jawab nahi deti, LB use list se hata deta, agla click doosri App pe.

Ek badi machine kyun nahi? Kitni bhi badi ho, ek hi hai. Wo giri to poori site band.

User ka login App me hi yaad rakh lete to? User usi App se bandh jaata. Wo giri to logout, kyunki doosri App ko
kuch pata hi nahi. Isliye session bahar, Redis / DB me.

**Pooche: giri hui App theek ho gayi to?** /health phir haan bolta, LB use wapas list me le leta.

**Pooche: App giri nahi, bas dheemi hai?** Jiske kam connection khule hain, use bhejo (least-conn).

---

*ab kai App hain, sab code bana rahe, takkar?*

## 3. Maine aur Ravi ne link banaya, dono ko ek hi code mila

Isliye Counter har App ko alag range deta hai: App-1 ko 1 se 1000, App-2 ko 1001 se 2000. App apni range me se agla
number leta. Ek range kabhi do App ko nahi milti, to code kabhi same nahi.

Range kyun? Range ke andar App khud number deta, har link pe Counter tak jaana nahi. 1000 link pe ek call.

UUID kyun nahi? 36 akshar ka hai. Link chhota karna hi to kaam tha.

Lambe link ka hash le ke pehle 7 akshar? Do link ke pehle 7 akshar same aa sakte (takkar), har baar DB me dekhna
padta. Counter me takkar hoti hi nahi.

**Pooche: range khatam to?** Tabhi Counter (ZooKeeper / DB) se agli range.

**Pooche: App-1 400 pe crash ho gaya?** 401 se 1000 bekaar, jaan-boojh ke. 3.5 lakh crore me kuch hazaar se farak nahi.

**Pooche: Counter hi gir gaya?** App ki range chalti rehti, tab tak Counter ki copy uth jaati.

**Pooche: Redis INCR se number kyun nahi?** Chal jaata, par har link pe ek call. Range me 1000 link pe ek.

**Pooche: har number bachana ho to?** Har link pe Counter se poochho. Phir range ka fayda khatam.

---

*ab click gin-ne gaye to redirect dheema*

## 4. Maine click ginti chahi, redirect hi dheema ho gaya

Isliye redirect turant karte hain (302), aur "ek click hua" ki khabar Kafka me daal ke aage badh jaate hain.
Analytics service apni speed se Kafka se padh ke ginti karti.

Ye kyun chalta? Redirect kisi ka intezaar nahi karta. Ginti peeche, disk pe pakki. Ginti wala dheema ya band ho tab
bhi user ka link khulta, bas ginti thodi der se aati.

Click seedha DB me likhte to? Har redirect DB likhai ka intezaar karta. Wahi dheemapan.

Sirf Redis me INCR? Ginti ho jaati, par sirf ginti. Kaun, kab, kahan se click hua, wo nahi milta.

**Pooche: Analytics dheema hai to?** Khabar Kafka me padi rehti. Wo apni speed se padhta, kahan tak padha yaad rakhta (offset).

**Pooche: wahi click do baar aaya?** Har click ki apni id. Wahi id dobara aayi to ginti nahi.

**Pooche: koi khabar kharab nikli?** Alag line me daal do (DLQ), baaki ginti chalti rahe.

**Pooche: Kafka hi band?** Redirect phir bhi chalta. Bas kuch click ginti me nahi aate.

**Pooche: naya report chahiye?** Kafka me pade purane click dobara padh lo (replay).

**Pooche: SQS kyun nahi, Kafka kyun?** SQS padhte hi message hata deta. Kafka rakhta hai, naya report = dobara padho.

---

*ab data hi itna ho gaya*

## 5. DB team boli: 5 saal me 90 TB, ek machine me nahi

Isliye data kai machine me baant diya (shard), link ke code se. Code ring pe aage chalta hai, jo pehli machine mile
wo uski (consistent hashing). Har tukde ki 3 copy.

Code se hi kyun baanta? Click hamesha code ke saath aata hai, to har click seedha ek machine pe. Country se baantte to
code dekh ke pata nahi chalta kis country ka, saari machine poochni padti.

Ek badi machine? 90 TB ek machine me aata hi nahi.

Seedha code % machine? Machine judi to lagbhag saari keys hil jaati (3 se 4 = sab badle). Ring me sirf pados ki.

**Pooche: ek machine mari?** Baaki 2 copy se milta, link nahi gaye.

**Pooche: ek machine pe zyada bojh?** Har machine ring pe 100-200 jagah baithti, bojh barabar (virtual nodes).

**Pooche: Cassandra kyun, MySQL kyun nahi?** Join aur transaction chahiye hi nahi, bas code se link, aur 90 TB.
Wajah kaam ka dhaancha + size hai, "NoSQL powerful" nahi.

**Pooche: Cassandra me main copy kaunsi?** Koi nahi. Teeno barabar, koi bhi likh / padh sakti.

**Pooche: copy badha do to likhna tez?** Nahi, har copy pe likhna padta. Likhne ka bojh = shard, copy = bachav.

---

*ab har tukde ki 3 copy, par naya link 404?*

## 6. Maine naya link banaya, turant khola, 404 aaya

Isliye likhna 2 copy me, aur padhna bhi 2 copy se (quorum, 3 me se 2). A, B me likha, padha A + C se, to A me mil
gaya. Har jodi me ek taaza copy zaroor.

Ye kyun chalta? 2 + 2 = 4, copy sirf 3. To padhne wali jodi me likhne wali ek copy aani hi hai. Banane ke baad 404 nahi.

Teeno me likho? Ek copy dheemi ya giri to har likhai ruk jaati.

Ek se padho? Wahi copy mil sakti jahan abhi pahuncha nahi. Wahi 404.

**Pooche: likhte waqt C giri?** 2 me ho gaya to save. C uthi to padosi ke paas rakha note use mil jaata (hinted handoff).

**Pooche: padhte waqt A aur C me farak?** Wahin C ko theek kar do (read repair).

**Pooche: A aur B dono giri?** Sirf 1 bachi. Likhna fail, user ko error (ya 1 pe likho, risk ke saath).
Par jo "done" bola, wo kabhi khota nahi.

**Pooche: 2 se padhna dheema nahi?** Thoda. Isliye pehla ilaaj Redis, DB tak kam aata.

**Pooche: 5 copy ho to?** Likhne + padhne ki ginti copy se zyada ho (W + R > N): 3 + 3 > 5.

**Pooche: data bachane ko Kafka laga dein?** Nahi. DB ka apna log ye khud karta, extra dabba nahi.

---

*ab LB hi ek hai*

## 7. Shaam ko LB hi gir gaya, koi request andar nahi

Isliye do LB, aur aage Route 53. Route 53 har ~30 second LB se poochta rehta (health check). Jo 3 baar chup, wo list
se bahar, sab LB-2 pe (DNS failover).

Route 53 hi kyun? Phone sabse pehle Route 53 se hi poochta hai "kahan jaun?". Wahin se zinda LB ka pata mil jaata.

Ek hi LB rakhte? Wo gira to App kitne bhi hon, koi andar nahi aa sakta.

**Pooche: phone ne purana pata yaad rakha to?** ~60 second kuch request fail (DNS TTL). Isliye LB khud kai zone me
(ALB), aur har jagah 2+.

**Pooche: poora region gaya?** Route 53 doosre region pe bhejta. Naye link ke aakhri kuch second kho sakte.

---

*ab koi galat istemal kare to?*

## 8. Raat ko spammer ki script, har second 5,000 link

Isliye gateway pe hi ginti: har API key ki ek shared ginti (Redis me). Had paar hui to 429, request andar aayi hi nahi
(rate limit).

Ye kyun chalta? Rokna darwaze pe, sabki ek ginti se. Asli users ki speed bachi rehti.

Har App apni ginti rakhe? 10 App = had 10 guna, aur bheed App tak aa chuki hoti.

Sirf IP pe had? Bot naya IP le aata. Isliye ginti API key / user_id pe, jo bot badal na sake.

**Pooche: pehchaana hua hamla?** WAF darwaze pe hi rok deta.

**Pooche: koi phishing ka lamba link chhota kare?** Chhota karne se pehle check (virus / dhokhe ki site), taaki hamara
domain spam na bane.

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

---

ARCHETYPE F (infra/component) · CONCEPTS: [ID-gen](../../FOUNDATIONS/13_distributed_id_snowflake.md) · [caching](../../FOUNDATIONS/04_caching.md) · [sharding](../../FOUNDATIONS/06_database_sharding.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
