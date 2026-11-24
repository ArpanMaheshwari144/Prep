# Microservices Communication — services aapas me kaise baat karti hain

> **NAV** — KYA: services aur client baat kaise karte (sync/async, server-push real-time, saga, gateway). lagta hai: [payment](../SYSTEM_DESIGNS/06_payment_system/06_payment_system.md) · [notification](../SYSTEM_DESIGNS/04_notification_system/04_notification_system.md)

---

## 1. Sync ya async

**Koi poochhe to aise bolo:**
```
Jahan jawab abhi chahiye aur user ruka hai (login, payment confirm), wahan sync: REST ya gRPC.
Jahan kaam peeche ho sakta hai (SMS, email, video banana) ya ek khabar kai service ko chahiye,
wahan async: Kafka jaisi line. Isse ek service dheemi ho to baaki nahi rukti.
```

**Scene (e-com se):** Ravi ka ghee ka order pay hua. "Paisa kata?" payment service se = sync, jawab bina order aage nahi. SMS = async, 10 sec der chalegi; order service khabar line me daal ke aage (Kafka stadium).

**Trap:** order service SMS ko bhi sync call kare aur SMS company dheemi ho, to Ravi ka "order ho gaya" bhi atka rahega (Kafka kadam 1: ek dheema to sab dheema). Jo kaam user ke rukne layak nahi, use sync mat banao.

**Checkout me dono saath:** pehle order bane (PENDING), phir stock roko, phir paisa kaato, ye sab sync kyunki Ravi screen pe ruka hai. "Order ho gaya" milte hi baaki sab (email, SMS, analytics, recommendation) ek "OrderPlaced" khabar se Kafka pe, koi bhi dheema ho Ravi ko farak nahi.

---

## 2. Sync ke andar: REST ya gRPC

**Koi poochhe to aise bolo:**
```
REST = JSON ki chitthi HTTP pe, har koi padh leta (browser, mobile, doosri company), isliye bahar ke liye.
gRPC = andar ki services ke beech chhoti packed chitthi (protobuf) aur pakka contract (.proto file), HTTP/2 pe
ek hi connection pe kai baat saath. Isliye andar tez aur chhota, par browser seedha gRPC nahi bolta.
```

**Scene:** Ravi ka phone order service se REST me baat karta (JSON, koi bhi padh le). Andar order service ko payment aur stock se baar-baar baat karni hai, wahan gRPC: chitthi chhoti, aur .proto me field ka naam badla to code compile hi nahi hoga, galti pehle hi pakdi jaati.

**Trap:** frontend ko seedha gRPC service se jod diya, browser bol hi nahi paaya. Bahar REST (ya gateway pe badlo), andar gRPC.

---

## 3. Server se phone tak khud bhejna (push)

**Koi poochhe to aise bolo:**
```
Phone baar-baar "naya kuch?" poochhe (polling) to crore request bekaar jaati, aur data phir bhi late.
Isliye taar ek baar khula rakho aur naya aaye to server khud bheje: dono taraf baat = WebSocket,
sirf server se = SSE. Bahut log hon to alag connection server, aur update sab tak pub/sub se.
```

**Scene (IPL score):** 50 lakh log score dekh rahe. Har 2 sec poochte to 25 lakh request har second, 99% baar jawab wahi purana. Taar khula rakha (score me user kuch bhejta nahi, isliye SSE bhi kaafi). Ek machine ~50k-100k taar pakadti hai, isliye alag connection server. Score badla to score service pub/sub pe daalta, har connection server apne logon ko bhej deta (Docs me bhi yahi tha: edit -> Redis pub/sub -> doosra server -> doosra user).

**50 lakh tak ek update, teen alag dikkat, teen alag ilaaj (mila mat dena):**
```
1. data BADA hai          -> sharding (yahan lagti hi nahi, score 50 byte ka hai)
2. user DOOR hai (US/UK)  -> connection server user ke paas, update ek baar har region me, wahan se local push
3. BAHUT JAGAH bhejna     -> fanout tree: score service -> 3 region hub -> 100 connection server -> 50 lakh
   + sirf jo badla wo bhejo { runs:156, wickets:3 } ~50 byte, poora scorecard ~5 KB nahi
```

**Trap:** WebSocket aur Kafka ko ek samajh liya. WebSocket = server aur user ke phone ka khula taar. Kafka = andar services ke beech ki line, phone tak nahi jaata. WhatsApp me dono: dono online = WebSocket se turant; net band tha = message DB me pada, online aaya to diya.

**Crore taar kaun sambhalta (WhatsApp / Slack / Teams):** taar phone kholta hai, server nahi, aur ek phone ka ek hi taar hota hai (saari chat usi pe). Khula taar jab chup hai to bas memory me kuch KB ki entry hai, CPU nahi; isliye ek machine lakhon pakad leti (WhatsApp blog 2012: ek machine pe 20 lakh+). Kam thread, bahut connection (event loop, Netty). Kaun kis machine pe = Redis me session map (chat notes). Net gaya to taar toota, entry hati; heartbeat band = mara maano. App background me ho to apna taar aksar band, phone ka ek hi taar Google / Apple ke paas (push notification), wahi app ko jagata. Ginti trillion nahi: duniya me ~8 arab log, WhatsApp ~3 arab mahine me.

Teeno raaste (WebSocket, SSE, long polling) = [Compares 12](../../06_COMPARES/12_websockets_vs_sse_vs_long_polling.md).

---

## 4. Async ke andar: Kafka ya RabbitMQ

Ek khabar kai service ko (email, SMS, analytics) aur dobara padhni pade = Kafka (stadium ka register). Ek kaam ek baar ek worker = RabbitMQ (daakghar). Poora = [07 message queues](07_message_queues.md) + [Compares 14](../../06_COMPARES/14_kafka_vs_rabbitmq.md).

---

## 5. Ek service dheemi ho to baaki ko bachao

**Koi poochhe to aise bolo:**
```
Ek service dheemi ho to baaki ko bachane ke teen kaam: timeout, taaki koi zyada der na latke;
retry with backoff, taaki girti service pe toot na padein; aur circuit breaker, taaki lagaataar fail
aaye to kuch der call hi band, phir thoda-thoda try karke wapas.
```

**Scene:** order service sync me payment ko call karti, payment atki, har call 30 sec latak ke fail; order service ke saare thread phanse, jo sirf order dekhne aaye unka bhi kaam ruka. Timeout = kuch second me "nahi hua". Backoff = 1, 2, 4 sec ruk ke (URL / cache ka jitter). Circuit breaker = ghar ka fuse: aakhri kuch call me fail had se upar gaye (jaise 10 me 5) to fuse uda, kuch der call band; phir kuch call bhej ke dekho, chali to wapas (News: site wapas aayi to thodi-thodi request). Fuse ke teen haal: CLOSED (sab call jaati), OPEN (sab band, turant "nahi"), HALF-OPEN (kuch call bhej ke jaanch). Backoff me thoda random (jitter) bhi jodo, warna sab client ek hi pal dobara maarte.

**Chauthi cheez, bulkhead:** har neeche wali service ke liye alag thread pool, jaise jahaz ke alag dibbe; payment ka pool bhar gaya to bhi stock wali call chalti rahe.

Spring me ye sab Resilience4j se: `@CircuitBreaker`, `@Retry`, `@TimeLimiter`, `@Bulkhead`.

**Trap:** retry bina backoff, sab 3-3 baar turant maarne lage, bojh teen guna, service poori giri. Retry hamesha ruk-ruk ke aur had ke saath.

---

## 6. Teen service se maal jodna (aggregator)

```
DESIGN: 3 service (A KYC, B credit report, C bank statement) se maal lao, jodo, save karo.
Loan officer customer 42 ki file kholta hai.

1  DIKKAT: pehle simple kiya: A ko call, phir B, phir C. Har ek 1 sec = file 3 sec me bani, dheema.
   ILAAJ:  teeno ko SAATH me call (parallel). Ab file 1 sec me, jitni sabse dheemi service.

2  DIKKAT: C (bank) atak gaya, 30 sec jawab nahi. Uska intezaar karte-karte saare thread phans gaye,
           ab A aur B ki call bhi nahi ho rahi. Ek ki wajah se sab baitha.
   ILAAJ:  har call pe timeout (2 sec). C baar-baar fail ho to circuit breaker: C ko call band,
           turant "nahi mila". C ke liye alag thread pool (bulkhead), wo bhare to bhi A, B chalte rahein.

3  DIKKAT: C nahi mila to poori file fail? Loan officer ke paas KYC aur credit report to hai.
   ILAAJ:  jo mila wo save, file PARTIAL ("bank statement baaki"). Background job baad me C laake bhar deta.
           (C zaroori ho to file FAILED, aur baad me dobara.)

4  DIKKAT: retry hua, ya do baar request aayi, to customer 42 ki do file ban gayi.
   ILAAJ:  save pe UNIQUE (customerId, requestId). Dobara aaya to wahi purani file milti, nayi nahi banti.

5  DIKKAT: aggregator beech me mar gaya. File kahin adhoori lataki hai, kisi ko pata nahi.
   ILAAJ:  job ka haal DB me (PENDING). Job PENDING me atka dikhe to sweeper use dobara chala deta.
           A, B, C se sirf padhna hai, to dobara call karna safe.

YAAD: saath me call · har service ka timeout + breaker + alag pool · PARTIAL save · UNIQUE · haal DB me.
```

---

## 7. Kai service, kai DB, ek kaam (saga)

**Koi poochhe to aise bolo:**
```
@Transactional sirf ek DB ka. Order, payment, stock alag DB me hon to saga: har service apna hissa commit
karti jaati, beech me fail hua to pichhle kadam ka ULTA kaam (paisa wapas, stock wapas). Ya to har service
khabar sun ke khud agla / ulta kare (choreography), ya ek boss sabko bataye (orchestration).
```

**Scene:** Ravi ka order bana, stock ruka, paisa katne pe fail. Order aur stock pehle hi commit ho chuke, rollback ho hi nahi sakta. Isliye "order cancel" + "stock wapas" ka ulta kaam chalao.

**Trap:** socha DB rollback sab theek kar dega. Rollback sirf ek DB ka bina-commit wala kaam ulta karta; saga me har kadam commit ho chuka, ulta kaam khud likhna padta.

---

## 8. Service kahan hai + bahar ka darwaza

**Koi poochhe to aise bolo:**
```
Service ke pate badalte rehte hain, isliye service discovery: har service register me apna pata likhwati
hai, baaki naam se poochte hain; K8s me service naam yahi kaam karta. Bahar se aane wala har request ek
darwaze, API gateway, se aata hai: wahan login, rate limit, phir sahi service tak.
```

**Scene:** payment ke 10 server, naya khula, ek gira aur naye pate pe aaya; order service ko kaise pata? Register me har service khulte hi pata likhwaye aur "zinda hoon" bolti rahe; order service naam se poochhe. K8s: pod ek service naam ke peeche, `http://payment-service:8080` bolo, K8s ka DNS use ek pakke ClusterIP pe le jaata, wahan se kisi zinda pod pe. Gateway = ek darwaza, login + rate limit (rate limiter ka gate wala guard), phir sahi service. CORS bhi gateway ek jagah sambhal leta.

**CORS kya:** frontend `myapp.com` se API `api.myapp.com` ko bulaya, browser ne roka kyunki origin alag. Server jawab me `Access-Control-Allow-Origin: https://myapp.com` bheje tab chalta. Prod me pakka origin likho, `*` nahi (cookie ke saath `*` chalta hi nahi). Ye browser ka niyam hai, server se server ya Postman me CORS hota hi nahi.

**Auzaar ke naam:** register = Eureka (Spring), Consul; K8s me service naam hi kaafi. Gateway = Spring Cloud Gateway, Kong, AWS API Gateway.

**Trap:** gateway ek hi machine pe aur wo gira = poora app band. Gateway ki bhi kai copy, aage LB.

---

## 9. Monolith ya microservices

Chhota app, kam log = monolith (simple, debug aasaan). Bada aur badhta = microservices (payment ke 10 instance, baaki 2; ek gira baaki chalu; alag deploy). Chhote app pe microservices = network call, bikhra debug, service-service ki consistency, sab faltu bojh. Monolith se shuru, bheed aaye tab todo. Poora = [Compares 04](../../06_COMPARES/04_monolith_vs_microservices.md).
