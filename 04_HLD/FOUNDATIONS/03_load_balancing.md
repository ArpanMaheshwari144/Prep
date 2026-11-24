# Load Balancing

> **NAV** — KYA: LB (L4/L7, algos, health-check). lagta hai: [rate-limiter](../SYSTEM_DESIGNS/02_rate_limiter/02_rate_limiter.md) · [browser-journey](19_url_browser_journey.md)

> **HLD Topic 3 — Traffic distribution + scaling foundation**

---

## 1. LB kya hai aur kyun

**Koi poochhe to aise bolo:**
```
LB server ke aage ek darwaza hai. Saari request pehle isi pe aati hai, aur ye har request
kisi zinda aur khaali server ko de deta hai. Isse ek server ki had paar nahi hoti, ek server
gira to baaki chalte rehte, aur bheed badhe to peeche naya server jod do.
Par LB request baantta hai, ghatata nahi. Aur LB khud gira to sab band, isliye LB bhi ek nahi rakhte.
```

**Scene (Netflix):** raat 8 baje sab log app kholte hain. Peeche 3 server hain aur aage LB. LB har request baari-baari teeno ko deta hai. Server 2 gir gaya to LB usko bhejna band kar deta hai, aur Arpan ko pata bhi nahi chalta. Bheed badhi to 4th server jod diya, LB use bhi dene lagta hai. App ke code me kuch nahi badla.

**LB khud gira to:** saare server zinda hain par unke paas request pahunchane wala koi nahi bacha. Isliye LB do rakhte hain: ek chalta hai, doosra taiyaar baitha rehta hai, ya dono saath kaam karte hain. AWS ALB lete ho to ye kaam AWS khud karta hai, kyunki wo andar se kai machine pe chalta hai. Ye hamara faisla nahi hai.

**Trap:** 10,000 request aa rahi hain aur ek server sirf 2,000 jhel sakta hai. 4 server lagaye to LB ke baad bhi har server pe 2,500 hi padengi, aur sab doobenge. LB ka kaam baantna hai. Bojh ghatane wali cheezein alag hain: cache, rate limit, aur zyada server (autoscaling).

---

## 2. L4 vs L7 (LB request ko kitna padhta hai)

**Koi poochhe to aise bolo:**
```
L4 LB sirf lifafa dekhta hai: kis IP se aaya, kis port pe jaana hai. Andar kya likha hai,
wo nahi padhta, isliye bahut tez hai aur bahut zyada connection jhel leta hai (AWS NLB).
L7 LB chitthi khol ke padhta hai: URL, header, cookie. Isliye /search ko search service pe
aur /pay ko payment service pe bhej sakta hai (AWS ALB, Nginx). Aam web app me L7 hi lagta hai.
```

**Scene (e-com):** Ravi ka phone ek hi darwaze (`shop.com`) pe teen request bhejta hai: `/api/search?q=ghee`, `/api/cart`, `/api/pay`. L7 LB URL padhta hai aur har request apni service pe bhejta hai: search wali search service pe, cart wali cart service pe, pay wali payment service pe. Bahar se ek hi darwaza dikhta hai, andar teen alag ghar hain.

**L4 kahan lagta hai (PUBG):** game me phone har second dher saare chhote UDP packet bhejta hai. Unme URL hota hi nahi, aur yahan tez hona sabse zaroori hai. Isliye wahan L4 lagta hai, jo bina padhe packet aage badha deta hai.

**HTTPS ka taala kahan khulta hai:** L7 ko chitthi padhni hai, to taala (TLS) LB pe hi khulta hai, aur LB peeche server ko request deta hai. Iska fayda ye hai ki certificate ek jagah rehta hai aur server ka CPU bachta hai. L4 taala nahi kholta, wo encrypted packet waise ka waisa aage bhej deta hai.

**Trap:** L4 (NLB) laga ke socha ki `/pay` ko alag server pe bhejenge. Ye ho hi nahi sakta, kyunki L4 URL dekhta hi nahi. Rasta URL se tay karna hai to L7 chahiye.

---

## 3. LB kisko de, ye kaise chunta hai (algorithm)

**Koi poochhe to aise bolo:**
```
Round robin = baari-baari: 1, 2, 3, phir 1. Sabse seedha hai, par har request ko barabar ginta hai.
Least connections = jis server ke paas abhi sabse kam kaam chal raha hai, use do. Production me yahi zyada chalta hai.
Weighted = bade server ko zyada hissa, chhote ko kam.
IP hash = ek IP hamesha ek hi server pe jaaye. Ye tab chahiye jab session server ki memory me ho.
Stateless app (JWT / Redis) me iski zaroorat hi nahi padti.
```

**Scene (e-com):** ek request "ghee dikhao" hai jo 10 ms me ho jaati hai. Doosri "pichhle saal ke saare order ki report" hai jo 5 second leti hai. Round robin dono ko "1 request" ginta hai. Jis server pe teen report aa gayi wo doob raha hai, par ginti me sab barabar dikhta hai. Least connections dekhta hai ki us server pe abhi 3 kaam atke hue hain, to nayi request doosre server ko de deta hai.

**IP hash ka jaal:** poora office ya college bahar ek hi IP (NAT) se nikalta hai. Hash same aayega, to 500 log ek hi server pe gir jaayenge. Isliye sticky chahiye to IP se nahi, cookie se karo. Usse bhi behtar ye ki session Redis me rakho, taaki sticky ki zaroorat hi na pade.

**Trap (chat, WebSocket):** LB **connection** baantta hai, har request alag se nahi. Chat me ek WebSocket ghanton khula rehta hai aur usi pe hazaaron message aate hain. Bheed badhi, tune 4th server joda, par purane log purane taar pe hi chipke rahe aur naya server khaali baitha raha. Ilaaj: connection ki umar fix kar do (kuch der baad client dobara jude, tab LB use naye server pe bhi bhej sakta hai). Aam HTTP me L7 LB har request alag baant deta hai.

---

## 4. Health check (LB ko kaise pata ki server zinda hai)

**Koi poochhe to aise bolo:**
```
LB har kuch second har server ko GET /health bhejta hai. Jawab nahi aaya ya der se aaya,
to wo ek fail gina jaata hai. Server ko mara tab maante hain jab lagaataar kuch baar fail ho
(maan lo 3 baar), taaki ek baar ki dheemi pe zinda server na nikal jaaye. Wapas tab lete hain
jab lagaataar kuch baar theek jawab de. Aur check aisa ho jo sach bole, sirf "process chal raha" nahi.
```

**Scene (Netflix):** server 2 pe ek pal ke liye GC ruka, aur ek check 2 second late aaya. Agar LB isi pe use mara maan le, to bina wajah ek theek server hat jaata hai. Agla check pass hua, to ginti wapas 0 ho gayi, aur server bacha raha. Server 3 sach me gira, to teeno check fail hue, aur LB ne use list se hata diya.

**Kitna tez pakde (trade-off):**
```
1 fail pe hata do, aur jaldi-jaldi check karo  -> mara server jaldi pakda, par dheema-zinda bhi hat sakta
5 fail tak ruko, aur der-der se check karo      -> galti kam, par mara server der se pakda, tab tak request usi pe
beech ka raasta: 3 lagaataar fail, aur har 5-10 sec check
```

**Do tareeke:** health check me LB khud jaake poochta hai (pull). Heartbeat me server khud batata rehta hai "main zinda hoon" (push), aur awaaz band hui to maan lo mar gaya.

**Trap (jhootha check):** `/health` sirf "OK" bol deta hai, kyunki process chal raha hai. Peeche DB ke connection khatam ho chuke hain. LB ko sab hara dikhta hai, wo request bhejta rehta hai, aur har request 500 ban ke lautti hai. Ilaaj: check me wo cheez dekho jo sach me kaam rokti hai, jaise "pool me connection bacha hai?". Par har check pe DB ko query mat maaro. 50 server, har 5 sec me ek query, to DB pe faltu bojh padega.

---

## 5. Jo cheez bachane ke liye lagayi, wahi system maar de

**Koi poochhe to aise bolo:**
```
LB ki har bachane wali cheez ka apna failure hai. Health check sabko mara maan le to koi request
hi nahi jaati, isliye panic mode. Ek server gira aur uska bojh baaki pe gira to wo bhi girte hain,
isliye pehle se jagah chhod ke chalo (N+1). Retry har layer pe jude to bojh 3-4 guna ho jaata hai,
isliye retry ki had + backoff + jitter. Naya server khaali dikhe to sab usi pe gir jaata hai, isliye slow start.
```

**1. Panic mode (Netflix):** DB gira, to saare server ka health check fail ho gaya, aur LB ne sabko mara maan liya. Ab continue watching wali request bhi nahi chal rahi, jo Redis se chal sakti thi. Aadhi kharabi poori kharabi ban gayi. Ilaaj: aadhe se zyada server ek saath mare dikhein to LB check ko nazarandaaz karke sabko request bhejta hai. Soch ye hai ki "sab mar gaye" ka matlab aksar ye hota hai ki check galat hai. (Envoy me ye setting isi naam se hai.)

**2. Ek gira, sab gire (cascading):**
```
3 server, har ek 70% pe  ->  ek gira  ->  bache 2 pe 105%  ->  wo bhi gire  ->  sab band
```
Iska ilaaj LB me nahi hai, capacity me hai. Ek server girne pe bhi chalna hai, to aam din me har server ko itna khaali rakho ki uska hissa baaki jhel lein (3 server = har ek ~66% se neeche). Isko N+1 kehte hain. Saath me load shedding: girne se pehle kuch request seedha mana kar do.

**3. Retry ka toofan:** server dheema hua, request timeout hui. LB ne retry ki, phone ne bhi ki, upar wali service ne bhi ki. Ek request 3-4 ban gayi, theek us waqt jab system pehle se doob raha tha. Ilaaj wahi hai jo notification me padha tha: retry ki had (kul ka ~10%), backoff + jitter taaki sab ek saath dobara na maarein, aur retry sirf wahan jahan dobara chalana safe ho (idempotent).

**4. Slow start:** algorithm least connections hai. Naya server khada hua, uske paas 0 connection hain, to LB saari nayi request usi ko dene laga. Uska JVM garam nahi hai, cache khaali hai, aur wo pehle minute me hi gir gaya. Ilaaj: naye server ko dheere-dheere bojh do (maan lo 30 sec me 0 se poora). Iska ulta connection draining hai: server band karte waqt nayi request roko, aur purani poori hone do.

**Trap (metastable):** bheed hat gayi, phir bhi system theek nahi hua, kyunki retry ki line khud naya bojh ban chuki hai. Aise me traffic rok ke, purani line phenk ke, system ko zabardasti wapas khada karna padta hai.

---

## 6. LB ki apni had, aur bade system me LB ek parat

**Koi poochhe to aise bolo:**
```
LB bhi ek machine hai, to uski bhi had hai: network ka taar, TLS kholne ka CPU, aur kitne connection
yaad rakh sakta hai. Bade system me isliye ek LB nahi hota, ek parat hoti hai: DNS kai LB ke IP baant deta hai,
aage halka L4 LB hota hai jo bina padhe packet aage badhata hai, uske peeche L7 LB jo URL padh ke service chunta hai,
aur phir server. (AWS me NLB ke peeche ALB wala dhaancha yahi hai.)
```

**LB chaar jagah bharta hai:**
```
1. network taar  saari request aur saara jawab isi se guzarta. 10 Gbps ka card = 10 Gbps, chahe peeche 50 server ho
                 (video / file wale system me ye sabse pehle bharta)
2. TLS ka CPU    har nayi https connection pe taala kholne ka hisaab LB karta. Har request nayi connection khole
                 to LB ka CPU 100%, jabki peeche server khaali baithe hain
3. connection    LB har khule connection ka hisaab rakhta (kaun client, kaun server). Ye ginti limited hai
4. port          LB se peeche ek server tak ~65,000 port hi hote. Bahut zyada traffic pe sach me khatam ho jaate
```

**Ilaaj, har had ka alag:**
```
TLS ka CPU bhara          -> keep-alive (ek connection pe kai request), ya taala kholne ka kaam alag jagah
taar bhara                -> ek LB se nahi chalega, kai LB. DNS se hi 2-3 LB ke IP baanto,
                             ya anycast: ek hi IP, par alag sheher me alag machine jawab deti
connection / port bhara   -> aage halka L4, peeche kai L7. L4 HTTP padhta hi nahi, isliye bahut zyada jhelta
```

**Scene (WhatsApp ke crore taar):** crore phone ke taar sabse pehle DNS se pata lete hain ki kis IP pe jaana hai, aur paas wale region ka IP mil jaata hai. Wahan aage L4 LB baitha hai, jo sirf taar aage badhata hai, chitthi nahi padhta. Uske peeche connection server (gateway) hain, jo taar pakad ke rakhte hain. Bojh har parat pe bant gaya, kisi ek machine pe nahi aaya.

**Trap:** sirf DNS ko LB maan liya, ki DNS 3 IP de dega to bojh bant jaayega. Par phone aur ISP DNS ka jawab cache kar lete hain, to sab ek hi IP pakad ke baith sakte hain. Aur DNS ko ye pata hi nahi chalta ki koi server mara. Isliye DNS ke peeche bhi asli LB chahiye.

---

## (baad me saanche me) Sticky ka garam server

### 6. STICKY session ka garam server

Sticky on → ek user hamesha ek hi server pe. Kuch bhaari user ek hi server pe baith gaye →
wo 90% pe, baaki 30% pe. LB dekh raha hai par kuch kar nahi sakta — user ko hila nahi sakta.
Aur wo server gira to unke session hi ud gaye.

**Ilaaj sticky ko theek karna NAHI hai — sticky ki ZAROORAT khatam karo.**
Session server ki memory se nikaal ke Redis ya token me daal do → har request kisi bhi server pe.

---

## ★★ IN CHHEO ME EK HI SHAKAL — CASCADING / METASTABLE FAILURE

```
har ek me, jo cheez BACHANE ke liye lagayi thi USI NE MAARA:

   health check       ->  bachane ko tha, usne SAB dead kar diya
   retry              ->  bachane ko tha, usne load 3x kar diya
   least-connections  ->  barabar baantne ko tha, usne NAYE server ko maar diya
   sticky             ->  session bachane ko tha, usne ek server GARAM kar diya
```

**CASCADING FAILURE** = ek cheez girne se agli girti hai, phir agli.

**METASTABLE FAILURE** (tedha roop) = load HAT jaane ke baad bhi system apne aap theek NAHI hota,
kyunki ab retry ka backlog KHUD hi naya load ban chuka hai. Aise me traffic band karke,
backlog PHENK ke, system ko zabardasti reset karna padta hai.

---

## ★ BOLNE KE LIYE — "X fail ho gaya to kya hoga" wale sawaal pe

```
1. "LB traffic baantta hai, ghatata nahi"           <- soch ki jad
2. "har bachane wali cheez ka apna failure mode hai" <- health check / retry / sticky
3. ilaaj hamesha DO jagah:  LB ki setting  +  CAPACITY ki planning (N+1)
4. "aadhi kharabi ko poori kharabi mat banne do"     <- panic mode / graceful degradation
```

> ★ IMAANDARI (20-Sep, jo likha wo ye hai):
> upar likha SAB technology ka behaviour hai — ye aise hi kaam karta hai, ispe khade raho.
> "interview me aisa poocha JAATA hai" — ye writeups se hai.
> Itna kaafi hai: senior-backend design round me "X fail hua to?" wale sawaal writeups me
> aksar milte hain, aur prod-support background ke saath ye sabse aasan zone hai.

---

## ★★ "LB LAGA HAI, PHIR BHI SAB EK SERVER PE" — ho sakta hai?

> Haan, ho sakta hai. Round robin ka code nahi bigadta, wo 1, 2, 3, 1, 2, 3 hi ghumata hai.
> Bigadta tab hai jab LB jo GIN raha hai (server / connection / IP) wo asli bojh nahi hota.

```
1. SIRF EK SERVER "ZINDA" DIKHA
   baaki health check me fail (sach me ya jhoothe) -> list me ek hi bacha
   -> round robin = 1, 1, 1, 1 ... sab usi pe
   ilaaj: upar wala PANIC MODE (50%+ dead dikhe to check ko nazarandaaz karo)

2. LB CONNECTION baant-ta hai, REQUEST nahi          <- sabse aam, sabse chhupa
   keep-alive / HTTP2 / gRPC / WebSocket = ek connection pe hazaaron request
   L4 LB ne connection ek baar server-1 ko diya -> us connection ki SAARI request server-1 pe
   scale out kiya? purane lambe connection purane server pe hi chipke rahe, naya khaali
   ilaaj: L7 LB (har REQUEST alag baante) ya connection ki umar limit
          (max connection age / max requests per connection -> client dobara jude, naye server pe bhi)

3. IP HASH + bahut log EK IP ke peeche
   poora office / college ek NAT IP se -> hash same -> sab ek server pe
   ilaaj: IP hash mat lo; stickiness chahiye to cookie / session-id se, ya sticky ki zaroorat hi hatao

4. DNS ne ek hi IP pakad liya
   DNS round-robin ne 3 IP diye, par client / ISP resolver ne pehla cache kar liya -> sab usi pe
   ilaaj: chhota TTL, aur DNS ke peeche bhi asli LB (DNS ko akela balancer mat maano)

5. CONFIG galti
   weight galat (server-1 = 100, baaki = 1) ya list me ek hi server
```

**Sab traffic ek pe nahi, par bojh phir bhi jhuka — request ki keemat alag:**
```
round robin har request ko BARABAR ginta hai
10ms wali aur 5 sec wali (report, bada query) = dono "1 request"
jis server pe bhaari wali padi wo doobta, ginti me sab barabar dikhta
=> production me least-connections / least-response-time (jo abhi ka bojh dekhte hain)
```

> **EK LINE:** *"LB barabar tabhi baant-ta hai jab jo wo gin raha hai wahi asli bojh ho.
> Connection gine aur request lambe connection pe aayein, IP gine aur sab ek NAT ke peeche hon,
> ya request ki keemat alag ho — wahan jhukaav aata hai."*

---

## ★ PURANI INFRA FILE SE AAYE HISSE (26-Sep merge — purani 05_INFRA_DEEP/02_load_balancer.md, ab git history me)

> Jo us file me tha aur yahan nahi tha, wahi laaya. Baaki (L4/L7, algo, health, active-active) upar pehle se hai.

### Sticky Sessions (Session Affinity)

```
PROBLEM:
   User logs in → server 1 stores session
   Next request → LB routes to server 2
   Server 2: "Who are you? Login again."
   = BAD UX
```

```
SOLUTION 1: STICKY SESSION
   LB cookie set: "this user → S1 always"
   Same user always lands on S1

   Simple
   S1 crashes = user session lost
   Uneven load

SOLUTION 2: SHARED SESSION STORE
   All servers share Redis for sessions
   Any server can handle any request

   Stateless servers
   S1 crashes = no problem
   Modern approach (preferred)

SOLUTION 3: JWT (Stateless tokens)
   Token contains user info
   Any server validates and serves
   Truly stateless
   Tera UserCRUD use karta yeh
```

**★ (drill se) — JWT kaise "saboot" hai + analogy:**
```
   login -> server token deta: user-info + SIGNATURE (secret key se bana)
   har request -> server sirf signature VERIFY kare (same secret) -> koi store / Redis lookup nahi
   saare server ke paas same secret -> koi bhi server verify kar le

   ANALOGY:
     JWT     = ID-card pe office ka STAMP -> guard sirf stamp dekhe, kahin bhi ghoomo
     SESSION = visitor pass -> guard har baar reception ko CALL kare (= Redis lookup)
```


### AWS Load Balancer Types

```
ALB   L7       HTTP/HTTPS · path-based routing · microservices
NLB   L4       TCP/UDP · bahut zyada throughput · gaming/video/IoT · static IP
CLB   L4+L7    LEGACY, naye kaam me mat lo
GWLB  L3       Gateway LB: firewall / inspection appliance ke aage (traffic unse guzaarna)
               ★ geo routing iska kaam NAHI — multi-region / geo = Route 53 (DNS) ya Global Accelerator
```

### ═══ HANDS-ON — Nginx Load Balancer LIVE (round-robin + failover, 22-Aug) ═══

> 2 backend (BACKEND-1, BACKEND-2) ke aage Nginx LB. Hit karo -> requests baari-baari dono pe (round-robin).
> Ek backend gira do -> saara traffic doosre pe (failover). Sirf config, koi program nahi.
> (files: ../../05_INFRA_DEEP/LB_DEMO/nginx-lb.conf)

### 0. Maqsad
```
Nginx ek REAL LB hai. 2 backend ke aage lagao -> load baant deta (round-robin) +
ek backend down ho to healthy pe route (failover). Scaling + resilience dono.
```

### 1. Setup — network + 2 backends
```
docker network create lbnet
docker run -d --name be1 --network lbnet hashicorp/http-echo -text="Hello from BACKEND-1" -listen=:5678
docker run -d --name be2 --network lbnet hashicorp/http-echo -text="Hello from BACKEND-2" -listen=:5678
   lbnet   = private network -> containers ek-doosre ko NAAM se dhoondhein (be1, be2)
   http-echo = tiny backend, bas apna text bolta
```

### 2. LB config (nginx-lb.conf) — ye 2 block hi asli LB
```
upstream backends {          # backends ka group
    server be1:5678;
    server be2:5678;         # default = ROUND ROBIN (aur: least_conn, ip_hash)
}
server {
    listen 80;
    location / { proxy_pass http://backends; }   # request ko group pe FORWARD -> nginx choose karta
}
```
```
docker run -d --name lb --network lbnet -p 8080:80 -v "<path>\nginx-lb.conf:/etc/nginx/nginx.conf:ro" nginx
```

### 3. Round-robin dekho
```
for /L %i in (1,1,8) do @curl -s http://localhost:8080
-> BACKEND-1, BACKEND-2, 1, 2, 1, 2, 1, 2   (baari-baari = load baant diya)
```

### 4. Failover dekho (ek backend gira do)
```
docker stop be1
for /L %i in (1,1,6) do @curl -s http://localhost:8080
-> SAB BACKEND-2   (nginx ne dead be1 skip kar diya -> service chalti rahi, no downtime)
docker start be1   (wapas -> phir round-robin)
```

### 5. KYUN / mechanism
```
upstream = backend-pool. proxy_pass -> nginx har request pool me se ek server chunta.
   ROUND ROBIN (default) = baari-baari -> load barabar.
   FAILOVER = jo server respond na kare, nginx use skip karke healthy pe bhejta (passive health-check).
= horizontal scaling ka dil: load badhe -> aur backend upstream me jodo, LB baant dega.
   ek node marr jaaye -> baaki pe route -> HA (high availability).
```

### 6. ★★ GEMS
```
1. LB = load baantna (round-robin) + failover (dead node skip). Dono ek saath.
2. upstream + proxy_pass = poora nginx LB. algorithm: round-robin (default) / least_conn / ip_hash.
3. ip_hash / sticky session -> ek user hamesha SAME backend (session-affinity chahiye tab).
   warna stateless rakho (JWT/Redis) -> koi bhi backend serve kare.
4. L7 (ALB, HTTP-aware, path/host routing) vs L4 (NLB, TCP, fast). AWS: ALB=L7, NLB=L4.
5. LB khud SPOF na bane -> LB bhi 2 (active-passive / DNS).
```

### 7. Dobara kaise
```
docker start be1 be2 lb   (band ho to)
for /L %i in (1,1,8) do @curl -s http://localhost:8080   -> round-robin
docker stop be1  -> phir hit -> sab be2 (failover)
```


---

## Nginx Deep — User's Question

### **Nginx ≠ AWS** (open-source software, not AWS-specific!)

```
Nginx = Open-source software (free, install anywhere)
AWS   = Cloud platform (provides infrastructure)
```

Nginx run kar sakte: AWS EC2, Azure VM, GCP, laptop, on-premise.

**Kisne banaya:** Igor Sysoev (independent Russian engineer, ~2004). AWS ka product NAHI.

**"AWS ne Nginx banaya / apne andar install kiya?" → NAHI:**
```
   - AWS ne KHUD nahi banaya/install kiya
   - TUMHARE team (Konovo) ne AWS ke EC2 SERVER pe Nginx INSTALL kiya
   - Nginx AWS-server pe chal raha kyunki TUM ne daala (AWS ne bundle nahi)
```

**Relationship (key):**
```
   AWS   = INFRA provider (server/EC2 deta — khaali machine) [makaan]
   Nginx = SOFTWARE jo TU us machine pe install karta [tera saamaan]
   AWS ka APNA LB = ALB / NLB (AWS-made — Nginx ka managed alternative)
   = ALB = AWS-made;  Nginx = third-party, tu install karta
```

**Konovo connection:** EC2 pe Nginx chal raha (team ne install) → requests handle + log karta → logs CloudWatch mein. Isliye CloudWatch mein "Nginx" dikha (AWS ka nahi, tumhara install).

> **Line:** "Nginx = independent open-source software (AWS ne nahi banaya). Anywhere install hota. AWS pe chalta = TUM ne EC2 pe daala. AWS ka apna LB = ALB. AWS = infra; Nginx = software."

### Nginx — ASLI KAAM / Why it exists (CORE)

```
   Origin (~2004): "C10K problem" solve karne — ek server pe
   10,000+ SIMULTANEOUS connections kaise handle?

   Apache (purana): har connection = ek THREAD → 10K conn = 10K threads
                    → memory khatam, slow (scale nahi karta)
   Nginx ka jugaad: EVENT-DRIVEN / ASYNC → ek worker HAZAARON
                    connections, kam memory, fast

   ESSENCE = web traffic ka SUPER-EFFICIENT FRONT DOOR
   Analogy: Apache = har visitor ko alag receptionist (chaos);
            Nginx = EK super-fast receptionist jo hazaaron ek saath sambhaale

   = CORE: massive concurrent connections efficiently handle.
     Baaki roles (proxy/LB/SSL/cache/static) us core strength se NIKLE
     — core nahi roles. ("kaam: efficient gatekeeper; roles: bonus")
```

### Nginx — Multiple Roles

```
1. WEB SERVER         — HTML/CSS/JS serve karta
2. REVERSE PROXY      — Client → Nginx → Backend app
3. LOAD BALANCER   — Layer 4 + 7 both supported
4. API GATEWAY        — Routing, rate limiting, caching
5. SSL TERMINATION    — HTTPS handle
```

### Nginx Config Example

```nginx
upstream my_app {
    server app1.example.com;
    server app2.example.com;
    server app3.example.com;
}

server {
    listen 80;
    location / {
        proxy_pass http://my_app;   # forward to backend pool
    }
}
```

### Nginx vs AWS ALB

| | Nginx (self-hosted) | AWS ALB (managed) |
|---|---|---|
| Cost | Free | Per-request pricing |
| Maintenance | You handle | AWS handles |
| Control | Full | Limited |
| Cross-cloud | | AWS only |
| Best for | Custom config, on-prem | AWS-native apps |

### Network Tracking (Nginx logs)

```
192.168.1.5 - - [04/May/2026:10:23:45]
"GET /api/users/123 HTTP/1.1"
200 1234
"https://google.com"
"Mozilla/5.0..."
```

| Field | What |
|---|---|
| IP | Client IP |
| Timestamp | When |
| Method + URL | What requested |
| Status | Response code |
| Bytes | Response size |
| Referrer | Where user came from |
| User-Agent | Browser/device |

**Logs analyze via:** Filebeat → Elasticsearch → Kibana, OR CloudWatch, Datadog, **New Relic** (Konovo).

---

## Real-World LB Examples

```
AWS:
   ALB (Application LB)  — Layer 7, HTTP/HTTPS
   NLB (Network LB)       — Layer 4, TCP/UDP, ultra-low latency
   CLB (Classic LB)       — Legacy

Software:
   Nginx                  — Most popular
   HAProxy                — High performance
   Traefik                — Modern, container-friendly

Cloud-native:
   Kubernetes Ingress     — Layer 7 inside K8s
   Istio (Service Mesh)   — Sidecar-based LB
```

---

## Build vs Use vs Configure (real-world reality)

Koi LB **SCRATCH se nahi banata** — solved + complex problem, cloud/open-source ne pehle hi bana diya.

**USE karte (build nahi):**
```
   AWS pe    → AWS ALB / NLB
   Azure pe  → Azure Load Balancer
   GCP pe    → Cloud Load Balancing
   self-host → Nginx / HAProxy / Traefik (software, scratch nahi)
   (sirf hyperscalers — Google/Meta — massive scale ko custom banate)
```

**PAR CONFIGURE karte (managed LB bhi plug-and-play nahi):**
```
   - LB create (ALB/NLB)
   - Listeners (port 80/443)
   - Target groups (kaunse servers pe bheje)
   - Routing rules (L7: /api → group-A, /static → group-B)
   - Health check settings (path, INTERVAL, THRESHOLD)
   - SSL certificate (HTTPS)
   = AWS infra CHALATA/maintain karta; hum RULES + CONFIG dete
```

**Managed vs Self-host:**
```
   Managed (AWS ALB) → AWS maintain, tu sirf CONFIG (kam ops)
   Self-host (Nginx) → tu install + config + MAINTAIN (zyada control, zyada ops)
   Build from scratch → almost never
```

> **Line:** "LB build nahi karte (cloud ne bana diya) — USE + CONFIGURE karte ho.
> Health-check interval/threshold, algorithm, L7 routing rules = woh config knobs
> jo tu AWS ALB / Nginx mein set karega." (Interview mein "managed vs self-hosted LB"
> distinction puchte.)

---

## Load Balancer vs Reverse Proxy vs API Gateway

Teeno CONFUSE hote (Nginx teeno ban sakta — isiliye). Farak:

| | Load Balancer | Reverse Proxy | API Gateway |
|---|---|---|---|
| **Primary job** | identical servers mein load DISTRIBUTE | backends ke aage FRONT intermediary (forward, hide) | APIs/microservices ka SMART entry (route + manage) |
| **Focus** | scale, availability, health, failover | backend hide, SSL termination, cache, single-entry | auth, rate-limit, routing, transform, versioning, aggregation |
| **Layer** | L4 or L7 | L7 (mostly) | L7 (application/API) |
| **Used in** | any multi-server setup | front of web servers | microservices |
| **"Smartness"** | distribute | forward + features (CAN bhi LB) | full API management |

**SMARTNESS LADDER (key):**
```
   LB (distribute across identical servers)
     ⊂ Reverse Proxy (front intermediary; hide/SSL/cache; CAN also LB)
       ⊂ API Gateway (reverse proxy + API mgmt: auth/rate-limit/route/transform)

   = API Gateway = SPECIALIZED reverse proxy (API-focused)
   = Reverse Proxy load-balancing kar sakta
   = Nginx TEENO ban sakta (config se) — isiliye log confuse hote
```

**TOOL vs ROLE (confuse mat karna — Nginx ≠ reverse proxy):**
```
   ROLES (jobs/functions):  Load Balancer / Reverse Proxy / API Gateway
   TOOLS (software):        Nginx / HAProxy / Envoy / AWS-ALB

   → Nginx ek TOOL hai jo reverse-proxy ROLE play kar sakta
     (+ LB / web-server / API-gateway roles bhi) — Nginx ITSELF "reverse proxy" NAHI
   → ek TOOL (Nginx) → kayi ROLES; ek ROLE (reverse proxy) → kayi TOOLS kar sakte
   = "reverse proxy" = kaam; Nginx = banda jo woh kaam (+ aur) karta
   = Analogy: "reverse proxy" = job (translator); Nginx = person jo woh job kare. Person ≠ job.
```

**ANALOGIES:**
```
   LB            = toll-booth director ("counter 3 khaali, wahan jao") — identical counters
   Reverse Proxy = building RECEPTIONIST (sab visitor pehle usse milte; woh andar
                   bhejta; backend HIDDEN; ID/security bhi) (+ LB/SSL/cache)
   API Gateway   = receptionist + SECURITY + ROUTER + RULES for a MULTI-DEPARTMENT
                   office (microservices): "kaunsa dept? ID dikhao (auth)? aaj kitni
                   baar aaye (rate-limit)? form bharo (transform)? → us dept route"
```

> **Line:** LB = "kaunse server pe" (distribute). Reverse Proxy = "main front-face,
> backend hidden, forward" (+ LB/SSL/cache). API Gateway = "reverse proxy +
> auth/rate-limit/route/transform" (microservices ka smart darwaaza).
> = LB ⊂ Reverse Proxy ⊂ API Gateway (badhti smartness).

---

## Sticky Sessions vs Stateless

```
Sticky session (Layer 7 + IP Hash):
   User-A → always Server-1 (session stored on Server-1)
   Server-1 down → session lost
   Hard to scale, uneven load

Stateless (modern with JWT):
   User-A → ANY server (token has user info)
   Any server can handle
   Easy to scale
   Server crash = no session loss
```

**Modern recommendation:** Stateless (JWT) → no sticky sessions needed.

**UserCRUD project mein tu JWT use kiya — yahaan understand kyu STATELESS chahiye.**

---

## Interview Talking Points

**Q: "Load Balancer kya karta?"**

> *"Multiple servers ke beech traffic distribute karta. **3 benefits:** throughput multiply, fault tolerance, horizontal scaling. **2 types:** Layer 4 (TCP-level, fast, AWS NLB), Layer 7 (HTTP-level, smart routing, AWS ALB/Nginx). **Algorithms:** Round Robin, Least Connections (production preferred), IP Hash, Weighted. **Health checks** mandatory — dead servers bypass."*

**Q: "Nginx kya hai?"**

> *"Open-source software — multi-purpose: web server, reverse proxy, **LB**, API gateway, SSL termination. Layer 4 + 7 both. AWS ke saath confuse mat karo — Nginx anywhere chal sakta. AWS ALB managed alternative hai."*

**Q: "Layer 4 vs Layer 7 — kab use?"**

> *"Layer 4 = TCP-level, IP/port only sees, fast, gaming/IoT/raw TCP services. Layer 7 = HTTP-level, full request inspect, smart routing (URL/header-based), SSL termination, sticky sessions. Modern web apps 90% Layer 7."*

---

## Power Phrase

> **"Load Balancer = traffic coordinator. Layer 4 fast TCP-level, Layer 7 smart HTTP-level. Algorithms: Round Robin / Least Connections (prod). Nginx (self-host) vs AWS ALB (managed). Health checks mandatory. Modern apps stateless + Layer 7 = best combo."**

---

## Memory Hook

```
LB = "Airport coordinator"
   • Counters = Servers
   • Passengers = Requests
   • Coordinator = LB
   • Direction logic = Algorithm

Layer 4 (TCP):    "Sees IP/port only" — fast
Layer 7 (HTTP):   "Reads full request" — smart

Algorithms:
   Round Robin       → simple cycle
   Least Connections → production winner
   IP Hash           → sticky (avoid in stateless)
   Weighted          → uneven server specs

Real-world:
   AWS ALB    → Layer 7 managed
   AWS NLB    → Layer 4 managed
   Nginx      → both, self-hosted
   HAProxy    → high-perf

Stateless > Sticky (with JWT)
```

---

## Trap Box

```
Trap 1: "1 LB = single point of failure"
         LB can crash
         Multi-LB (active-passive) ya managed (AWS ALB)

Trap 2: "Sticky sessions for everything"
         Causes uneven load + session loss risk
         Stateless (JWT) preferred

Trap 3: "Round Robin best"
         Treats all equal — uneven load problems
         Least Connections better in production

Trap 4: "LB = Nginx = AWS"
         Different things
         Nginx software, AWS infrastructure, ALB AWS's LB service

Trap 5: "1 slow/late health-check response = server dead"
         NO — false positive (healthy-but-slow ko maar dega)
         TIMEOUT + unhealthy_threshold (N consecutive fails, e.g. 3)
         Ek blip survive; sustained failure = dead
```
