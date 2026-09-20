# Load Balancing

> **NAV** — KYA: LB (L4/L7, algos, health-check). UP: [MASTER](../00_MASTER_SHEET.md) · trade-off: [L4 vs L7](../TRADEOFFS.md) · lagta hai: [rate-limiter](../SYSTEM_DESIGNS/02_rate_limiter/02_rate_limiter.md) · [browser-journey](../SYSTEM_DESIGNS/05_url_browser_journey/05_url_browser_journey.md)

> **HLD Topic 3 — Traffic distribution + scaling foundation**

---

## STORY — Airport Security

> Imagine **airport pe security check**:
>
> **1 counter only:**
> 1000 log line — 3 ghante wait. Counter wala thak gaya, slow.
>
> **5 counters open:**
> Sab counter parallel — 30 min mein nikal jaate.
>
> But agar passengers seedha kisi bhi counter pe jaye → chaos. Koi empty, koi over-crowded.
>
> **Solution:** **Coordinator** — *"Aap counter 3 pe jaayein"*. **Traffic distribute** karta intelligently.
>
> **Coordinator = Load Balancer**
> - Counters = Servers
> - Passengers = Requests
> - Distribution logic = Algorithm

---

## Why Load Balancer?

### Without LB
```
   1M requests/sec
        │
        ▼
   ┌─────────┐
   │ Server  │  ← can't handle
   │      │     CPU 100%, OOM, crash
   └─────────┘
```

**3 fatal problems:**
1. **Throughput limit** — single server max ~10K-50K RPS
2. **Single Point of Failure** — server down = whole app down
3. **No scaling** — vertical (bigger CPU/RAM) limited + expensive

### With LB
```
   1M requests/sec
        │
        ▼
   ┌──────────────┐
   │ LOAD BALANCER│
   └──────┬───────┘
          │
   ┌──────┼───────┐
   ▼      ▼       ▼
 Srv-1  Srv-2  Srv-3
 (333K) (333K) (333K)
```

**Benefits:** Throughput multiplied, fault tolerance, horizontal scaling, zero-downtime deploys.

---

## LB Position in Architecture

```
   USER → DNS → CDN → LOAD BALANCER → App Servers → DB/Cache
                          │
                  ┌───────┴───────┐
                 App-1   App-2   App-3
```

**LB sits between client and app servers.** Sab traffic LB ke through.

---

## OSI Model Refresher

```
Layer 7 — APPLICATION   (HTTP, FTP)
Layer 6 — Presentation
Layer 5 — Session
Layer 4 — TRANSPORT     (TCP, UDP, ports)  ← important
Layer 3 — Network       (IP addresses)
Layer 2 — Data Link
Layer 1 — Physical
```

**LB context:** Layer 4 (transport) vs Layer 7 (HTTP).

---

## Layer 4 vs Layer 7 — KEY DISTINCTION

### Layer 4 LB (TCP-level)
**Sees:** IP address + port only.
```
Request: Source IP 192.168.1.5 → Port 443
LB:      "Forward to Server-2" (based on round-robin/least-conn)
         HTTP details NOT visible (URL/headers hidden)
```
**Speed:** Fast (less inspection). **Examples:** AWS NLB, HAProxy TCP mode.

### Layer 7 LB (HTTP-level)
**Sees:** Full HTTP request — URL, headers, body, cookies.
```
Request: GET /api/users/123
         Authorization: Bearer eyJ...
         User-Agent: Chrome

LB:      "URL has /api → backend pool"
         "URL has /static → CDN/static-pool"
         "Header X-User=admin → admin-pool"
```
**Speed:** Slower (deep inspect). **Examples:** AWS ALB, Nginx, Traefik.

---

### Postal Analogy

```
Layer 4 = Courier at sorting center
   • Sees address + pincode only
   • Fast, dumb routing

Layer 7 = Office receptionist
   • Asks "kya kaam mein aaye"
   • Opens content, smart routing
```

### Comparison

| | Layer 4 LB | Layer 7 LB |
|---|---|---|
| Sees | IP/port | Full HTTP |
| Speed | Faster | Slower |
| Routing | Round-robin only | URL-based, header-based, smart |
| SSL termination | | |
| Sticky sessions | Limited | Cookie-based |
| Use | TCP services, gaming, IoT | Web apps (90% case) |
| Examples | AWS NLB, HAProxy | AWS ALB, Nginx |

**Modern apps = 90% Layer 7.**

---

## LB Algorithms

### 1. **Round Robin** (simplest)
```
Req 1 → Srv-1, Req 2 → Srv-2, Req 3 → Srv-3, Req 4 → Srv-1...
```
**Pros:** Simple. **Cons:** Doesn't consider load.

### 2. **Least Connections** (production preferred)
```
LB tracks active connections per server.
New request → server with FEWEST active connections.
```
**Pros:** Better load distribution.

### 3. **Weighted Round Robin**
```
Server-1 (powerful):  weight 5
Server-2 (medium):    weight 2
Server-3 (weak):      weight 1
→ Server-1 gets 5/8 traffic
```
**Use:** Heterogeneous servers.

### 4. **IP Hash** (sticky)
```
Hash(client IP) → always same server
```
**Use:** Session affinity needed (legacy stateful apps).
**Avoid in modern stateless (JWT) apps.**

### 5. **Least Response Time**
```
Track avg response time per server → route to fastest
```

---

## Health Checks (CRITICAL)

```
Every 30 sec:
   GET /health → Server-1: 200 healthy
   GET /health → Server-2: 503 mark unhealthy
   GET /health → Server-3: 200 healthy

Server-2 bypassed until recovers
```

**Without health checks**, LB sends traffic to dead servers → user errors.

### Slow/Late Server Handling — TIMEOUT + THRESHOLD (DEPTH)

**Concern:** server thoda LATE/slow respond kare (busy, blip) — kya LB turant DEAD maan le? **NO** — warna healthy-but-slow server galat se mar jaaye (**FALSE POSITIVE**).

**Fix = TIMEOUT + THRESHOLD:**
```
   TIMEOUT             — har check ki deadline (e.g. 2s).
                         Late beyond timeout = woh EK check failed.
   unhealthy_threshold — DEAD tabhi maane jab N CONSECUTIVE fails (e.g. 3).
                         Ek blip = SURVIVE (agla check pass → count reset).
   healthy_threshold   — wapas ZINDA tabhi jab N consecutive OK (e.g. 2)
                         (flaky server turant wapas na aaye).
   INTERVAL            — har N sec check (e.g. 5-10s).
```

```
   Server 1 baar late  → 1 fail count → agla pass → reset (zinda rehta)
   Server 3 baar fail   → sach mein down → DEAD mark (failover)
   = transient slowness tolerated; sustained failure = dead
```

**Tuning trade-off (interview-depth):**
```
   AGGRESSIVE (threshold 1, fast interval)
      → dead jaldi pakda, PAR false-positive risk (healthy-slow ko maar de)
   LENIENT (threshold 5, slow interval)
      → stable, kam false-positive, PAR dead detect slow
   = sweet spot: ~3 consecutive fails + 5-10s interval
```

---

## LB Redundancy — LB Khud SPOF (DEPTH)

**Problem:** saari traffic LB se guzarti. LB **KHUD** crash → poora system DOWN (servers zinda, par koi unhe traffic route nahi kar raha). LB ban gaya **Single Point of Failure**. (Ek hi guard, woh beemar = poora gate band.)

**Fix — LB bhi REDUNDANT (1 nahi, 2+):**
```
   ACTIVE-STANDBY  — ek ACTIVE (traffic handle), ek STANDBY (backup, idle)
                     active mar gaya → standby TAKE OVER
   ACTIVE-ACTIVE   — dono LB kaam karte (load share)
                     ek gira → doosra poora sambhaal le
   HEARTBEAT       — dono ek-doosre ko monitor (pulse)
                     active ka heartbeat band → standby detect → take over
   TRAFFIC SHIFT   — floating/virtual IP ya DNS → standby pe redirect
                     (clients ko pata bhi nahi chalta)
   MANAGED LB      — AWS ALB/NLB yeh internally handle (built-in redundant)
```

```
   = "guard ka backup guard" — ek gira, doosra turant sambhaal le
   = no downtime (LB ab SPOF nahi raha)
```

---

## ★★ LB KHUD BOJH BAN JAATA HAI — "LB laga diya, phir bhi dabaav aa gaya" (20-Sep deep-dive)

> Upar wala section LB ke MAR JAANE ki baat karta hai (SPOF → redundancy).
> Ye section usse aage ka hai: LB ZINDA hai, sab kuch "sahi" laga hai, aur phir bhi system baith gaya.

### Pehle ek line jo poori soch badalti hai

```
LB traffic ko BAANTTA hai. GHATATA nahi.
```

10,000 request aa rahi hain, 4 server hain → LB ke baad bhi 10,000 hi hain, bas 2,500-2,500.
Agar ek server 2,000 hi jhel sakta hai to LB kuch nahi bachayega — **wo LB ka kaam tha hi nahi.**
Dabaav GHATANE wali cheezein alag hain: cache · rate limit · autoscaling · load shedding.

### LB ki apni HAD — wo bhi ek machine hai, aur chaar jagah khatam hoti hai

```
1. NETWORK CARD      saari request AUR saara response isi ek taar se guzarta hai
                     10 Gbps card = 10 Gbps, chahe peeche 50 server ho
                     (video / file-download wale system me ye SABSE PEHLE bharta hai)

2. CPU — TLS         har NAYI https connection pe handshake ka hisaab LB karta hai
                     mehnga kaam hai; har request nayi connection khole to
                     LB ka CPU 100% ja sakta hai jabki peeche server KHAALI baithe hain

3. CONNECTION TABLE  LB har zinda connection ka hisaab rakhta hai (kaunsa client ↔ kaunsa server)
                     ye table memory + file-descriptor leti hai, ginti limited hai

4. PORT              LB peeche server se baat karne ko apne port kholta hai
                     ek machine ke paas ~65,000 port — bahut high traffic pe sach me khatam
```

### Ilaaj — har had ka alag

```
TLS ka CPU bhara       ->  keep-alive ON (ek connection pe kai request)
                           ya TLS ka kaam alag jagah karao (offload)

card / bandwidth bhara ->  ek LB se kaam nahi chalega, KAI LB chahiye
                           DNS se hi 2-3 LB ke IP baanto (DNS round-robin)
                           ya ANYCAST — ek hi IP, alag sheher me alag machine

connection / port bhara->  aage ek halka L4 LB, peeche kai L7 LB
                           (AWS ka NLB -> ALB wala dhaancha yahi hai)
                           L4 sirf packet aage badhata hai, HTTP padhta hi nahi -> bahut zyada jhelta
```

★ Isi wajah se bade system me LB **ek nahi hota — ek LAYER hota hai**:
```
DNS (kai IP)  ->  L4 LB  ->  L7 LB  ->  server
```

---

## ★★ CHHE TARIKE JINSE LB KHUD SYSTEM KO MAARTA HAI

> Har ek me LB ne "galat" kuch nahi kiya — usne apna kaam hi kiya.
> Yahi in sawaalon ka maza hai.

### 1. JHOOTHA health check

Server `/health` pe `200 OK` bhejta hai kyunki wo endpoint sirf itna kehta hai "main zinda hoon".
Peeche DB connection pool khatam ho chuka hai. LB ko sab HARA dikhta hai, traffic bhejta rehta hai,
har request 500 ban ke lautti hai.

```
shallow check   GET /health -> return "OK"                 <- sirf process zinda hai
deep check      GET /health -> DB ping + cache + downstream
```

Deep check sahi lagta hai par uski keemat hai — har 5 sec × har server × ek DB query.
50 server = DB pe faltu bojh. **Asli practice: check HALKA rakho par SACH bolne wala** —
"pool me connection bacha hai?" ye to memory me pada hai, DB tak jaane ki zaroorat hi nahi.

### 2. Health check ne hi POORA system maar diya — (PANIC MODE)

DB down hua → **saare** server ka deep health check fail → LB ne SABKO dead mark kar diya →
kisi ko traffic nahi bheja. Jo 20% request DB ke bina bhi chal sakti thi (cache se, static),
wo bhi nahi chali. **Aadhi kharabi ko LB ne POORI kharabi bana diya.**

```
ILAAJ = PANIC MODE
   agar ek had se zyada server unhealthy ho jaayein (maan le 50%),
   to LB health-check ko NAZARANDAAZ karke sabko traffic bhejne lagta hai

   soch: "sab dead hai" ka matlab aksar ye hota hai ki CHECK GALAT hai,
         na ki sach me sab mar gaye
   (Envoy me ye setting seedhe isi naam se hai)
```

### 3. Ek mara, baaki bhi mar gaye — (CASCADING)

```
3 server x 70%  ->  ek gira  ->  bache 2 pe 105%  ->  dono gire  ->  sab khatam
```

Ilaaj LB me hai hi nahi — **jagah chhod ke chalo**. Ek server ke marne pe bhi jeena hai to
normal haalat me har server `100/(N-1)` se KAM pe chalna chahiye (3 server = ~66% se neeche).
Isko **N+1 capacity** kehte hain. Saath me circuit breaker + load shedding — taaki server
marne ki jagah kuch request seedha MANA kar de.

### 4. RETRY ka toofan

Server slow hua → request timeout → LB ne retry ki → client ne bhi → upar wali service ne bhi.
Ek asli request 3-4 ban gayi — **theek us waqt jab system pehle se dooba hua tha.**

```
normal      100 req/s  ->  100 request
degraded    100 req/s  ->  300-400 request    (har layer apni retry jod rahi hai)
```

Teen cheezein chahiye:
```
RETRY BUDGET              kul ka ~10% se zyada retry nahi
BACKOFF + JITTER          sab ek saath dobara mat maaro, warna har 2 sec pe nayi LAHER
RETRY SIRF SAFE JAGAH     jahan dobara chalana theek ho (idempotent)
```

### 5. Naya server boot hua aur usi pe SAB gir gaya — (SLOW START)

Algorithm least-connections hai. Naya server khada hua → uske paas **0** connection → LB
**saari** nayi request usi ko bhejne laga. JVM garam nahi, JIT ne compile nahi kiya, local cache
khaali, pool khaali. Pehle hi minute me gir jaata hai, restart, phir wahi chakkar.

```
SLOW START            naye server ko DHEERE bojh do — maan le 30 sec me 0 se poora
CONNECTION DRAINING   band karte waqt: nayi request roko, purani poori hone do
                      (warna deploy ke waqt request BEECH me katti hai)
```

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
> "interview me aisa poocha JAATA hai" — ye writeups se hai, Claude ne na interview diya na liya.
> Itna kaafi hai: senior-backend design round me "X fail hua to?" wale sawaal writeups me
> aksar milte hain, aur 700-ticket wale background ke saath ye Arpan ka sabse aasan zone hai.

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
