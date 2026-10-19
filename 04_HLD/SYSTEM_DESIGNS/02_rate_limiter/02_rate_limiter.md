# Rate Limiter

> Ek user / IP ek tay samay me kitni request bhej sakta hai, uski had. Had paar to 429 (Too Many Requests). Misaal: ek IP se ek minute me 5 login.
> Dil teen cheezein: har request ke saamne baitha hai, isliye ~1 ms. Ginti saari machines pe ek. Limiter khud gire to site na gire.
>
> Mock me sirf heading + Cue padho. Har card: kya toota -> kya kiya -> yahi kyun -> doosra kyun nahi. "Pooche" wali line = interviewer aage poochhe to.

---

## Poori tasveer (aakhir me design aisa banta hai)

![poora design](img/c9_2-dc36e0eb.svg)

---


## Shuru

Seedhi baat: ek banda had se zyada request bheje to baaki users ke liye server khaali nahi bachta. Niyam lagao "itne
se zyada nahi", yahi rate limiting. API pe ek bot ek second me 10,000 baar login try karta hai (password guess), to
niyam "ek IP, ek minute, 5".

Sabse simple: App ki memory me ek ginti. Har request pe ginti + 1. Had paar to 429.

Pehle poocho: limit kis pe, IP / user_id / API key? Har endpoint ki alag (login 5 / min, search 60 / min)? Plan ke
hisaab se (free / pro / enterprise)? Sabse zaroori: maqsad server bachana hai ya paisa / security? Server bachana hai
to thoda zyada nikal jaaye, chalta. Paisa / OTP / login hai to bilkul sahi ginti chahiye.

Misaalein: login 5 / min ek IP se, password reset 3 / ghanta ek email pe, public API 100 / min ek key pe, signup
10 / din ek IP se, search 60 / min ek user ka.

Had kaise lage? Token bucket. Bucket me max 10 token, har second 1 bharta, har request 1 leti. Khaali to 429. Redis me
sirf 2 number: kitne token, aur aakhri baar kab. Bharne ka koi job nahi, request aane pe hisaab lagta (lazy). Request
aayi, tokens 7 the, beeta time jodo, 1 ghatao, andar.

Numbers: jagah ka hisaab bekaar hai, har user ki ek ginti + expiry, bas. Asli number = kitni baar poocha jaata hai.
Ye har request ke aage hai, to poore system me sabse zyada chalne wala dabba (laakhon / second). Isliye ye jagah ka
nahi, speed ka sawaal hai. RAM me (Redis), DB nahi. Aur ginti badhana ek jhatke me (atomic) hona chahiye.

**Pooche: 10 request ek saath aayi to?** Bucket bhara tha, 10 nikal gayi (burst). Asli traffic aisa hi aata hai.

**Pooche: 11vi request?** Bucket khaali, 429 + Retry-After (agla token kab).

**Pooche: user ghanta bhar soya, phir aaya?** Lautne pe hisaab: beeta time x 1, par had 10 se upar nahi.

**Pooche: fixed window kyun nahi?** Sasta, par kinare pe double (59th + 61st second).

**Pooche: sliding log?** Har request ka time rakho, pichhle 60 sec gino. Sahi, par memory bahut.

**Pooche: sliding counter?** Is minute ki ginti + pichhle minute ka hissa (jaise 70%). 2 number, lagbhag sahi.

**Pooche: leaky bucket?** Request line me, fixed speed se nikalti. Smooth, par burst nahi nikalne deta.

**Pooche: token bucket hi kyun?** Burst chahiye, memory kam, Redis me 2 number. AWS / Stripe yahi lagate.

**Pooche: do request ek saath hisaab lagayein to?** Padhna + hisaab + likhna EK Lua me (HMGET -> HSET). Time ek hi
jagah se.

---

*ab tootega kahan? kai App hain, har App apni ginti gin raha*

## 1. Password bhool gaya, 9 baar login kiya, saari andar chali gayi

**Cue:** Redis me EK ginti, 60 sec umar · dabba: Redis (LB + App x2 pehle)

LB ne mere 9 login baant diye: App-1 ne 5 gine, App-2 ne 4. Kisi ki had paar nahi hui, 9 andar.

Isliye sab App ek hi Redis key ginte hain (rate:Arpan:login). Har request pe +1. Pehli request pe key ki umar 60 sec
(EXPIRE): minute khatam, key khud mit gayi, ginti 0. 6thi request pe ginti 5 se upar, 429.

Redis hi kyun? Had tabhi sahi jab ginti EK jagah ho, aur wo har request ke saamne RAM jitni tez ho. Key ki umar hi
window hai.

Ginti DB me kyun nahi? Har request pe disk + lock, dheema. Redis RAM se, tez.

Ek user hamesha ek hi App pe bhej do (sticky LB)? Wo App gira to ginti gayi. App badhe ya ghate to ginti phir bat gayi.

**Pooche: minute ke kinare 5 + 5 aayi to?** 2 sec me 10 andar. Ye fixed window ki kamzori hai, isliye asli me token
bucket.

---

*ab ginti ek jagah hai, par ek saath 20 aayi to?*

## 2. Bot ne mere account pe 20 login ek saath bheje, 6 ghus gaye

**Cue:** ATOMIC: INCR / LUA script · dabba: (Redis atomic)

App-1 aur App-2 dono ne ginti 4 padhi, dono ne 5 likha. Had 5 thi, andar 6.

Isliye dekhna + badalna ek hi kadam me, Redis ke andar (INCR / Lua). INCR me padhna + badhana ek command hai. Token
bucket me dekhna + ghatana ek Lua script me: tokens + aakhri waqt, bharna request pe (lazy).

Ye kyun chalta? Redis ek line me chalta hai, ek ke baad ek. Beech me koi request ghus hi nahi sakti.

Pehle padho, phir likho (GET, phir SET)? Do kadam hue to do request ek hi purana number padh ke dono andar. Wahi 6.

App me synchronized / lock? Wo sirf ek machine ke andar chalta hai. Do App me race wahi ki wahi.

**Pooche: INCR ke baad, EXPIRE se pehle crash ho gaya?** Key bina umar reh gayi, user hamesha block. Isliye INCR +
EXPIRE ek Lua me.

**Pooche: Lua dheema ho ya atak jaaye?** Poora Redis rukta, kyunki ek line me chalta hai. Isliye script chhota, koi
loop nahi.

**Pooche: Redis ek line me, dheema nahi?** RAM hai: ek machine ~lakh command / sec (andaza). Zyada chahiye to shard.

**Pooche: Lua me 2 key, aur Redis cluster hai?** {Arpan} hash tag lagao: rate:{Arpan}:min aur :day ek hi machine pe.

---

*ab ginti sahi hai, par mana wali request bhi andar tak aa rahi*

## 3. Bot ki 429 wali request bhi App tak ghoom ke aayi

**Cue:** LIMITER GATEWAY pe, sabse aage · dabba: API Gateway

Bot 10,000 / sec bhej raha, LB se hoke App tak aati, App ginti karke 429 deta. App bhar gaya.

Isliye ginti darwaze pe: limiter API Gateway pe, sabse aage. Gateway Redis se poochta, had ke andar to App. Had paar
to wahin 429, LB / App tak pahunchti hi nahi.

Gateway hi kyun? Jitna aage roko (gateway / edge), utna kam kaam bekaar jaata.

Ginti App me hi rakhte? Mana wali request bhi andar tak aati, App ka kaam usi me chala jaata.

**Pooche: 429 ke saath kya bhejte ho?** Kab aao + kitni bachi (Retry-After: 30, X-RateLimit-Remaining).

**Pooche: sab 30 sec baad ek saath wapas aaye?** Nayi laher. Isliye Retry-After alag-alag (28, 33) (jitter).

**Pooche: gateway hi gira?** 2+ machine, aage LB. Darwaza bhi akela nahi.

**Pooche: limit sirf gateway pe ya service me bhi?** Dono. Gateway pe sabke liye moti had, service ke andar apni
baarik had.

---

*ab sab kuch Redis pe tika hai, wo gira to?*

## 4. Dopahar Redis gira, limiter andha ho gaya

**Cue:** REPLICA + FAIL-OPEN / FAIL-CLOSED · dabba: Redis Cluster + R53 + ALB

Ek hi Redis tha, ginti wahin. Wo gira to kisi ko pata nahi kaun kitni bhej chuka.

Isliye Redis ki copy taiyaar (replica, Redis Cluster). Main mara to copy main ban jaati, Cluster khud karta. Aur poora
Cluster gaya to pehle se tay hai kya karna: feed / search khula (fail-open), login / OTP band (fail-closed).

Ye kyun chalta? Redis gira to kya karna, wo PEHLE se tay hai, endpoint ke hisaab se. Copy pe switch me thodi ginti
khoti hai, rate limit me chalta.

Sab fail-open kar do? Login khula reh gaya to koi bina had password guess karta rahega.

**Pooche: main mara to?** Copy main ban gayi. Aakhri thodi ginti khoi, rate limit me chalta.

**Pooche: Redis gira, pata kaise chalega?** Redis call pe chhota timeout (~5 ms). Nahi aaya to gira maano, ruko mat.

**Pooche: Redis gira nahi, bas CPU full hai, copy laga do?** Nahi. Har request pe ginti LIKHNI hai (INCR = likhna), aur
likhna sirf main pe. Main ab bhi bhara. Isliye keys tukdon me (shard): har user ki key alag Redis pe. Copy = bachav +
padhne ka bojh, shard = likhne ka bojh.

**Pooche: LB / gateway bhi akela?** ALB kai zone me, Route 53 zinda wale pe bhejta.

---

*ab ek region me sab theek, par user kai region se aaye to?*

## 5. Meri script 3 region se chali, 100 ki jagah 150 nikal gayi

**Cue:** HOME REGION me ginti · dabba: (ginti ghar wale Redis me)

India, US, Europe, har region ka apna Redis, apni ginti. Teeno me 50-50, jod 150. Had 100 tooti.

Isliye meri ginti mere ghar region me (home region). Mera ghar India hai, to Frankfurt ki request bhi wahin gini
jaati. 100 hi nikalti.

Ye kyun chalta? Ek user ki ek ginti, ek ghar me. Door jaane ka kharcha sirf zaroori endpoint pe (paisa / login), baaki
apni ginti + sync, ~1 sec ki dheel chalti.

Har region apni alag ginti? Wahi 150. Had har region me alag lagi, user ke liye nahi.

**Pooche: ghar door hai (100-200 ms)?** Sirf paisa / login ghar tak jaata. Baaki apni ginti + har second sync.

**Pooche: sync ke beech?** ~1 sec tak har region poori had de sakta. Halki had me chalta.

**Pooche: ghar region gira?** Route 53 doosra region, ginti 0 se. Ek minute dheela, chalta.

---

*ab had lag rahi, par wahi bot roz wapas aa jaata*

## 6. Wahi bot har 60 sec baad phir aa jaata, din bhar

**Cue:** KAFKA -> PATTERN -> WAF BAN · dabba: Kafka + Pattern Svc + WAF

429 sirf bolta "60 sec baad aao". Bot 60 sec baad phir aata, din bhar, roz. Kabhi nahi rukta.

Isliye har 429 Kafka me daal ke bhool jao. Pattern Svc ginta hai: 10 min me 500+ 429 to WAF ki block list me daal
deta, wo sabse aage, edge pe hi rok deta.

Ye kyun chalta? Limit maafi wali hai (429, phir aao). Ban sirf pakke gunde pe, pattern dekh ke, aur apne aap hatne
wala, kyunki galat ban = asli user bahar.

Turant ban kar do? Asli user bhi tez click karta hai, office ke 1000 log ek IP pe (NAT), sale me asli bheed. Sab bahar
ho jaate.

Gateway seedha Pattern Svc ko call kare? Har request dheemi. Kafka me daal ke aage badho.

**Pooche: Kafka peeche chal raha?** Ban kuch minute late. Tab tak rate limit rok hi raha hai.

**Pooche: galat ban ho gaya (office IP)?** Ban "kuch der" ka: umar ~24 ghante, apne aap hate. Haath se bhi hata sakte.

---

*ab har user had me hai, phir bhi system bhar gaya to?*

## 7. Shaam ki sale, har koi limit me, phir bhi server gira

**Cue:** GLOBAL CAP + LOAD SHEDDING (503) · dabba: (gateway pe system ki had)

Laakhon user, har ek apni had ke andar. Par kul jod se server CPU 100%, sabke liye band.

Isliye gateway pe poore system ki bhi had (5,000 / sec). CPU 90% paar to kuch request 503. Kam zaroori pehle chhodo
(recommendation / analytics), login / payment bachao (load shedding).

Ye kyun chalta? User ki had aur system ki had alag cheez hain, dono chahiye. Sab user had me hon phir bhi system bhar
sakta hai.

Machine khud nahi badh jaati? Machine badhne me minute lagte, bheed second me aati. Pata ho to pehle badhao.

Line (queue) me rok lo? Sirf chhota (~2 sec). User chala gaya to uska kaam bekaar.

**Pooche: 429 ya 503?** User ki had = 429. System ki had = 503.

---

*ab sabse bura waqt: attack aur Redis ek saath gaya*

## 8. Raat ko attack, usi waqt poora Redis Cluster gira (copy bhi), aam API khuli

**Cue:** LOCAL COUNTER FALLBACK · dabba: (gateway ki memory)

Redis gaya to ginti kahin nahi. Fail-open wali API pe attacker ki sab request andar, server bhar gaye, site band.

Isliye har server apni memory me gine, had baant do (local fallback). Had 100, 4 server, to har ek 25.

Ye kyun chalta? Thodi galat had, bina had se behtar. Feed pe bhi har machine apni ginti rakhti, login ab bhi
fail-closed. User ko had thodi dheeli, attack khula nahi.

Redis gaya to sab andar aane do? Wahi hua: attacker ke liye site khuli, server bhar ke band.

**Pooche: ek user ki request alag-alag server pe gayi?** Motamoti had, exact nahi. Thodi galat had, bina had se
behtar.

**Pooche: server badh gaye?** Ginti config / service discovery se lo, had dobara baanto.

**Pooche: Redis wapas aaya?** Phir sabki ek ginti.

---

*ab had sahi jagah lagi, par kis naam pe?*

## 9. Office ki night shift: ek IP block, 1000 log atke

**Cue:** KEY = USER_ID (JWT), IP sirf login · dabba: (key badli)

Ginti IP pe thi. Office ka ek IP (NAT), 1000 log. Ek ki galti se IP block, 1000 atke.

Isliye login ke baad ginti user_id pe (key = user_id). JWT se user_id nikalta, 1000 log = 1000 alag ginti. Ek ki
galti, sirf uski ginti bhari, baaki 999 chalte.

Ye kyun chalta? Had us pehchaan pe jo ek insaan ki ho (user_id / username). IP sirf tab jab aur kuch na ho, kyunki ek
IP pe 1000 log hote hain aur ek botnet ke 1000 IP.

IP ki had badha do (5 se 500)? Office bach gaya, par botnet ko bhi utni hi chhoot.

**Pooche: login se pehle?** user_id hai hi nahi. IP pe dheeli had + WAF (botnet ke 1000 IP).

**Pooche: 5 galat password?** Username pe lock nahi, CAPTCHA / der. Warna attacker tujhe hi lock kar dega.

**Pooche: API key chori ho gayi?** Nuksaan seemit, had key pe hai. Key badlo / band karo.

---

## Poora design ek saans me

```
User -> Route 53 (kaunsa LB zinda) -> WAF (ban list) -> ALB (kai data center)
-> API Gateway me limiter (sabse aage, jaldi mana · global cap + load shedding · Redis gire to local ginti)
-> Redis Cluster (token bucket ek Lua script me: tokens + last, key = user + endpoint, copy + shard)
   -> had ke andar? App : 429 + Retry-After
baar-baar wala -> Kafka -> Pattern Svc -> WAF ban
DB:  Redis = ginti ki ek hi sachchi jagah, 1 ms, key ki umar = window (umar khatam = reset, safai ka job nahi)
     user ka plan (free / pro) DB se, Redis me usi limit se milao
```

Bolne ke liye: limiter darwaze (API gateway) pe, mana hui request andar jaati hi nahi. Ginti Redis me, sab servers ki
ek jagah, ek kadam me (token bucket ki Lua script). Key = kaun user + kaunsa kaam, umar = window. Token bucket isliye
ki asli user jhatke me request bhejta. Had paar = 429 + kitni der baad aao. Redis ki copy. Redis poora band: har
server apni ginti, login / OTP band. Baar-baar wale hamlawar ko WAF se block, aur badi asli bheed ke liye poore system
ki had.

---

## 10 guna traffic

- **API Gateway**: har request pe limiter chalta, to RAM me + ek jhatke me (atomic). Gateway machine badhao.
- **Redis**: shard (user / region ke hisaab se) + copy (switch ke saath) + fail-open + local fallback.
- **Kai region**: har user ka home region.
- **Gunda**: Kafka -> Pattern service -> WAF ban.
- **Asli bheed**: global cap + load shedding + line (queue) + pehle se scale.

**Pooche: kahan se sab gir sakta?** Redis (cluster + switch; aam API fail-open, login / OTP fail-closed). LB (Route 53).

**Pooche: chal raha hai, kaise pata?** Limiter ka time (p99), kitne 429, Redis ki galtiyan, alert.

---

## Pooche to (kisi card me nahi baitha)

Jawab ke header: theek hai to aage bhejo + X-RateLimit-Limit: 100, X-RateLimit-Remaining: 47, X-RateLimit-Reset: kab.
Had paar to 429 + Retry-After: 30 + Remaining: 0. Sirf "na" mat bolo, "kitna bacha" aur "kab aao" bhi batao, warna
client turant phir maarta. Retry-After me thoda aage-peeche (28, 33), warna sab ek saath lautte hain.

Ulta sawaal, "tumhe kisi aur ne 429 diya, ab?": "I respect Retry-After, back off exponentially with jitter, and
throttle my own sends."

Redis key: `rate:<user>:<endpoint>` me ginti, umar = window (60 sec). Umar khatam = reset, safai ka job nahi. Jaise
rate:192.168.1.5:login = 4, rate:user_456:signup = 2, rate:apikey_xyz:search = 47.

Limit badalni ho? Niyam config me (DB / AppConfig), gateway har ~30 sec padhe. Naya code nahi.

Sliding window: niyam "pichhle 1 ghante me 5 se zyada nahi". 11:00 pe aaye, 10:00 ke baad 5 aa chuke, mana. 11:10 pe
aaye, ab 10:10 ke baad gino, Ramesh 10:05 wala bahar ho gaya, 4, andar. Window request aane pe khisakti hai, koi timer
/ job nahi.

Kaun kaisa: token = burst haan, memory kam. Leaky = burst nahi, smooth. Fixed = sabse sasta, kinare pe double. Sliding
log = sabse sahi, memory zyada.

Ginti vs bojh: 100 chhoti request (5 ms) = 0.5 second kaam. 100 bhaari report (30 second) = 50 MINUTE kaam. Limiter ke
liye dono "100". Isliye bhaari endpoint pe kadi limit, ya request ka wazan (report = 50 token, login = 1), ya "ek user
ki ek waqt me 2 hi bhaari query".

Plan: bina login 60 / ghanta, free 5,000, pro 10,000, enterprise apni. Plan DB se, Redis me usi limit se milao.

Ek hi shakal (poore HLD me ghoomti): LB me bachane wali cheez ne hi maara (health check / retry / sticky). Cache me tez
karne wali cheez ne raasta roka (dheema Redis, timeout nahi). Limiter me rokne wali cheez ne bheed roki hi nahi
(per-user vs poora system), aur attack me khud gayab (fail-open).

---

## HANDS-ON #1 — Nginx limiter LIVE (21-Aug, isi folder ki `nginx.conf`)

```
docker run -d --name rl -p 8080:80 -v "<path>\nginx.conf:/etc/nginx/nginx.conf:ro" nginx
   -d background · --name rl · -p laptop 8080 -> container 80 · -v apni conf andar MOUNT (ro) · nginx = image

nginx.conf ki 2 asli line:
   limit_req_zone $binary_remote_addr zone=mylimit:10m rate=1r/m;    <- har IP ka bucket, refill 1 / min
   limit_req zone=mylimit burst=5 nodelay;                           <- bucket SIZE 5, burst turant serve
   rate = kis speed se nikalti · burst = kitni ruk sakti
   + location / { ... root /usr/share/nginx/html; index index.html; }  <- content serve
   koi program NAHI likha — sirf ready tool (Nginx, rate limiter built-in) on kiya + config + hammer -> 503
   SACH: nginx docs isko LEAKY BUCKET kehte; nodelay se bartaav
         token bucket JAISA. 503 = limit_req_status default.

HAMMER:  for /L %i in (1,1,30) do @curl -s -o nul -w "%{http_code} " http://localhost:8080
         (1,1,30) = 30 baar · -s -o nul = chup, body phenk do · -w = sirf status code
DIKHA:   200 200 200 200 200 200 503 503 503 ...
LOG:     (Docker Desktop -> Containers -> rl -> Logs) "GET / HTTP/1.1" 503 197
         [error] limiting requests, excess: 5.774 by zone "mylimit", client: 172.17.0.1
         (excess = bucket se kitna upar · client = Docker gateway IP, sab ek IP = ek bucket)

GOTCHA:  1. 503 tabhi jab ARRIVAL > rate (rate=2r/s pe dheere loop -> sab 200, refill pakad leta)
         2. burst=N -> spike me N+1 pass (burst=5 -> 6, 100 ->
            101); textbook N, nginx N+1, timing se 21 / 22 bhi
         3. 1 IP = 1 bucket
         4. config badla -> docker restart rl
CHALAO:  docker start rl · curl http://localhost:8080 · docker logs rl · docker restart rl · docker stop rl
```

---

## HANDS-ON #2 — App ke andar fixed window (usercrud, 26-Aug)

```java
@RestController
public class RateLimitController {
    private int LIMIT = 5;
    private long windowStart = 0;
    private int count = 0;

    @GetMapping("/rate-demo")
    public synchronized ResponseEntity<String> hit() {          // synchronized: shared counter
        long now = System.currentTimeMillis();
        if (now - windowStart > 1000) { windowStart = now; count = 0; }   // window reset
        count++;
        if (count > LIMIT)
            return ResponseEntity.status(HttpStatus.TOO_MANY_REQUESTS).body("429 - limit paar");
        return ResponseEntity.ok("OK - request #" + count);
    }
}
// SecurityConfig: .requestMatchers("/rate-demo").permitAll()
// ResponseEntity KYUN: status-code (200/429) khud set karne ko
```
```
LIVE:  1..10 | % { curl.exe -s http://localhost:8080/rate-demo; "" }
       OK #1..#5 · 429 (6..10) · 1 sec baad window reset -> phir OK
EDGE:  fixed window boundary: 999ms pe 5 + 1001ms pe 5 = ~2ms me 10
PROD:  single node memory -> restart pe reset + har server apna count -> Redis INCR + EXPIRE
       per user / IP -> map<key, counter> · lib: Bucket4j (token bucket) / Redis + Lua
JAWAB:    usercrud me khud banaya: 5 request theek, chhathi pe 429.
          Asli system me token bucket (Bucket4j library) + sab servers ka saanjha Redis counter. Nginx
          bhi ye kaam sabse aage kar sakta (limit_req).
```

---

## HANDS-ON #3 — Redis KHUD maara: kaise tootta

CODE: `04_HLD/HANDS_ON/01_rate_limiter_redis/RateLimiterDemo.java` · `04_HLD/HANDS_ON/02_incr_expire_crash/IncrExpireCrash.java`
(koi library nahi, socket se Redis, `java File.java`)

```
docker run -d --name demo-redis -p 6390:6379 redis:7 redis-server --save "" --appendonly no
   (RDB + AOF band -> sirf memory, restart = sab gaya, jaan-boojh ke)
docker stop / start demo-redis · docker exec demo-redis redis-cli TTL rl:user:1 · ... DEL rl:user:1

CODE KA DIL (user:1 ko 60 sec me 5):
   count = INCR KEY · if count == 1 -> EXPIRE KEY 60 · count <= 5 ALLOW else 429
   catch IOException -> FAIL_OPEN ? ALLOW : BLOCK
```
```
CASE A  chalta kaise  req 1..9:  count 1 TTL 60 ALLOW · 2/57 ALLOW · 3/56 ALLOW · 4/49 ALLOW · 5/48 ALLOW ·
                                 6/47 429 · 7/45 429 · 8/45 429 · 9/44 429     (window PEHLI request se)

CASE B  docker stop   "REDIS SE BAAT NAHI HUI: Connection refused" -> FAIL_OPEN = true -> ALLOW (2 baar)
                      fail-open = site chale, limit nahi · fail-closed = abuse nahi, site band
                      ("server ne haath khade kar diye, sab jao")

CASE C  docker start  GET rl:user:1 -> khaali · user (pehle count 7, BLOCK) -> count 1 ALLOW, 2 ALLOW
                      persistence band -> ginti gayab -> blocked ko 5 nayi mil gayi

CASE D  INCR ke baad, EXPIRE se pehle crash (USE_LUA = false)
                      RUN 1: count 1, CRASH · TTL -> -1
                      RUN 2: count 2..5 ALLOW, 6..7 429, har baar TTL
                             -1 -> key KABHI nahi mitegi, HAMESHA block
FIX D   USE_LUA = true   req 10..14: count 10 TTL 58 · 11/57 · 12/56 · 13/56 · 14/55 -> 1 min me free

TTL:  -1 = key hai, timer nahi · -2 = key hi nahi · 58 = 58 sec baaki
```
```
FIX:  B -> faisla pehle se: fail-open (aam API) / fail-closed (login, OTP) + replica
      C -> replica / AOF on. limiter me aksar chalne do (1 min ki ginti, paisa nahi). paisa hota to nahi.
      D -> Lua: INCR + EXPIRE ek command, Redis single-thread -> script beech me nahi rukti
           (EXPIRE NX se nahi bachta: wo bhi alag call hai, beech me crash wahi)
           pehle:  app --INCR--> Redis  [crash]  app --EXPIRE--> Redis
           Lua:    app --EVAL(INCR + EXPIRE)--> Redis
```
```lua
local c = redis.call('INCR', KEYS[1])
if c == 1 then redis.call('EXPIRE', KEYS[1], ARGV[1]) end
return c
```
```
JAWAB:    Docker Redis khud band karke teen cheez dekhi:
          1) Redis band: aam API chalti rahi, login / OTP ruk gaye.
          2) Redis restart: saari ginti zero ho gayi. Limiter me chalta hai, copy rakho to kam hota.
          3) Ginti +1 aur 'umar lagao' alag-alag bheje aur beech me crash hua: key bina umar ke reh
          gayi, user hamesha ke liye block. Isliye dono ek Lua script me ek saath.
```

ARCHETYPE F · CONCEPTS: [caching/Redis](../../FOUNDATIONS/04_caching.md) · [load-balancing](../../FOUNDATIONS/03_load_balancing.md) · [SPOF](../../FOUNDATIONS/11_reliability_spof_cloud.md) · [← MASTER SHEET](../../00_MASTER_SHEET.md)
