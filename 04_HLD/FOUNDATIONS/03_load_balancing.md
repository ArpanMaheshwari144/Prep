# Load Balancing

> **NAV** — KYA: LB (kyun, L4/L7, algorithm, health check, LB khud kaise maarta, LB ki parat, session, proxy / gateway). lagta hai: [rate-limiter](../SYSTEM_DESIGNS/02_rate_limiter/02_rate_limiter.md) · [browser-journey](19_url_browser_journey.md)

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

**Config ki galti:** weight galat (server 1 = 100, baaki = 1) ya list me ek hi server likha. LB wahi karega jo likha hai, aur sab ek pe gir jaayega.

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

## 7. Session kahan rakho, aur LB / reverse proxy / API gateway ka farak

**Koi poochhe to aise bolo:**
```
Session server ki memory me rakha to user ko hamesha usi server pe bhejna padega (sticky). Isse bojh tedha hota hai,
aur wo server gira to sabka login uda. Isliye session server se bahar nikaalo: Redis me rakho, ya JWT me
(token khud saboot hai, koi bhi server sign check kar le). Phir koi bhi request kisi bhi server pe ja sakti hai.

LB = kai jaise server me bojh baantna. Reverse proxy = server ke aage ka chehra (server chhupao, TLS kholo, cache).
API gateway = reverse proxy + API ka kaam (login check, rate limit, kaunsi service). Nginx teeno ban sakta hai,
isliye log confuse hote hain. Nginx ek tool hai, ye teeno kaam hain.
```

**Scene (sticky ka garam server):** sticky on hai, aur kuch bhaari user (badi company ke admin, jo din bhar report chalate) server 1 pe chipak gaye. Server 1 90% pe hai, baaki 30% pe. LB dekh raha hai par kuch kar nahi sakta, kyunki user ko hila diya to unka session gaya. Ilaaj sticky ko theek karna nahi hai. Sticky ki zaroorat hi khatam karo: session Redis me ya JWT me.

**Tera project:** usercrud aur todoapp me JWT hai. Login pe server token deta hai jisme user ki jaankari aur sign hota hai. Har request pe koi bhi server sirf sign check karta hai, Redis tak jaane ki bhi zaroorat nahi. Misaal: JWT = ID card pe office ka stamp, guard stamp dekh ke andar jaane deta hai. Session = visitor pass, guard har baar reception ko phone karta hai.

**Nginx vs ALB:**
```
Nginx   free software, khud install karo (EC2 pe, laptop pe, kahin bhi). Poora control, par chalana bhi khud ka
ALB     AWS ka bana hua LB. Tum sirf niyam dete ho (kaunsa URL kis group pe, health check), chalata AWS hai
```
LB koi scratch se nahi banata. Use karte hain aur config karte hain: listener (80 / 443), target group, routing ke niyam, health check, certificate.

**Trap:** "Nginx = reverse proxy" bol diya. Reverse proxy ek kaam hai, aur Nginx wo tool hai jo ye kaam (aur LB, web server, gateway bhi) kar sakta hai. Jaise translator ek kaam hai aur wo kaam karne wala insaan alag.

---

## 8. Thodi aur jaankari (naam aur tool)

**AWS ke LB:**
```
ALB   L7: URL / header se rasta, web app aur microservices
NLB   L4: TCP / UDP, bahut zyada traffic, game / video, static IP
CLB   purana, naye kaam me nahi
GWLB  firewall / jaanch wali machine ke aage traffic guzaarna. Geo routing iska kaam nahi, wo Route 53 (DNS)
```

**Nginx kyun bana (~2004, Igor Sysoev):** ek machine pe 10,000+ connection ek saath kaise sambhalein. Purana Apache har connection pe ek thread rakhta tha, to memory khatam. Nginx event loop pe chalta hai: ek worker hazaaron connection. Yahi soch WhatsApp ke connection server me bhi hai (Foundations 10, hissa 3). LB, reverse proxy, TLS, cache sab isi taakat se nikle kaam hain.

**Tera office (Konovo):** EC2 pe Nginx team ne install kiya, AWS ne nahi. AWS sirf machine deta hai, Nginx hamara software hai. Request ki logs isi se CloudWatch / New Relic me dikhti hain.

**Baaki tool:** HAProxy (tez, purana bharosemand), Traefik (container ke saath), K8s Ingress (K8s ke andar L7), Istio (har service ke saath chhota proxy).

---

## 9. HANDS-ON — Nginx Load Balancer LIVE (round-robin + failover, 22-Aug)

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
