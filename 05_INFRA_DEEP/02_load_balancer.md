# 🚦 Load Balancer Deep

---

## 1️⃣ Problem Without LB

```
        USER 1, 2, 3, ... 10,000
              │   │   │       │
              ▼   ▼   ▼       ▼
            ┌──────────────────┐
            │  SINGLE SERVER   │ 💥 crashed
            └──────────────────┘
            
Problems:
   ❌ Single point of failure
   ❌ Capacity limit (1 server = 1000 req/sec)
   ❌ No scaling
   ❌ Maintenance = downtime
```

---

## 2️⃣ Restaurant Host Analogy 🍽️

```
Busy restaurant:
   100 customers arrive
   8 waiters available
   
   Without HOST:
      Customers randomly pick waiter
      → 1 waiter overloaded, others idle
      → Chaos

   With HOST 🧑‍💼:
      Knows each waiter's load
      Routes EACH customer to RIGHT waiter
      → Balanced load
   
   = HOST = Load Balancer
   = Waiters = Backend servers
```

---

## 3️⃣ Architecture With LB

```
   USERS (10,000)
        │
        ▼
   ┌──────────────────┐
   │   LOAD BALANCER  │ 🚦
   │  (routing brain) │
   └──────┬───────────┘
          │
   ┌──────┼──────┬──────┐
   ▼      ▼      ▼      ▼
   S1     S2     S3     S4
   2500   2500   2500   2500
   req    req    req    req
   
   = Load distributed evenly
   = S2 crashes → LB stops sending
   = Demand grows → add S5
```

---

# 🌟 L4 vs L7 — THE KEY DISTINCTION

## 📮 POSTAL OFFICE ANALOGY

```
Letter Mumbai → Delhi.
Postal office sort karta hai.
2 type ke postmen:
```

---

### 🔵 POSTMAN A — Sirf ENVELOPE dekhta (= L4)

```
Envelope par:
   FROM: Mumbai, Arpan
   TO:   Delhi, Ramesh

Postman A:
   "Address Delhi → Delhi bus mein daal do"
   
   ✋ Letter ANDAR nahi padhta
   ✋ Content kya hai — pata nahi
   
= FAST (just sticker check)
= SIMPLE (no content read)
```

```
L4 sees:
   • Source IP
   • Source port
   • Dest IP
   • Dest port (443, 80...)
   • Protocol (TCP/UDP)
   
   ❌ Doesn't open packet
   ❌ Doesn't read HTTP
```

---

### 🟢 POSTMAN B — Letter KHOL ke padhta (= L7)

```
Postman B:
   Envelope dekha
   Letter KHOLA + PADHA:
   
   "Subject: Wedding" → Wedding bag
   "Subject: Bank cheque" → Priority bag
   "Subject: Promo" → Slow track
   
   ✅ Content read karta → SMART decision
   ✅ Content ke basis pe alag route
   
= SLOWER (reading takes time)
= SMARTER (content-aware)
```

```
L7 sees:
   • All L4 stuff PLUS:
   • URL path (/api/cart)
   • Hostname (api.com)
   • HTTP method (GET/POST)
   • Headers (User-Agent)
   • Cookies (session)
   
   ✅ Reads INSIDE HTTP
   ✅ Routes on CONTENT
```

---

## 🎯 CONCRETE EXAMPLE — Amazon.com

```
USER REQUEST:
   "GET amazon.com/api/cart"  (HTTPS, port 443)
   
   Envelope (L4 sees):
      Source: User IP
      Dest: amazon.com:443
      Protocol: TCP
      
   Letter (L7 sees):
      Host: amazon.com
      Path: /api/cart
      Method: GET
      Cookie: session=user123
```

```
L4 LB decision:
   "TCP to port 443 → koi bhi server"
   ✋ Path-aware NAHI
   ✋ Bas connection forward
```

```
L7 LB decision:
   /api/cart → cart-service cluster ✅
   /api/payment → payment-service cluster ✅
   /api/search → search-service cluster ✅
   /images/* → CDN/static servers ✅
   /admin → admin cluster (restricted) ✅
   
   ✅ CONTENT samjha → SMART routing
   ✅ Microservices perfect
```

---

## 📊 SIDE-BY-SIDE COMPARE

```
┌──────────────────┬──────────────┬─────────────────┐
│  Feature         │  L4 (Envelope)│ L7 (Content)    │
├──────────────────┼──────────────┼─────────────────┤
│ What it sees     │ IP + Port     │ Full HTTP        │
│ Speed             │ ⚡ Fast       │ 🐢 Slower        │
│ Smart routing    │ ❌ No         │ ✅ Yes           │
│ Path-based route │ ❌ Cannot     │ ✅ Can           │
│ Hostname route   │ ❌ Cannot     │ ✅ Can           │
│ Cookie routing   │ ❌ Cannot     │ ✅ Can           │
│ SSL inspection   │ ❌ No         │ ✅ Yes (decrypt) │
│ Cost             │ Cheaper       │ Pricier          │
├──────────────────┼──────────────┼─────────────────┤
│ AWS product      │ NLB           │ ALB              │
│ Best for         │ TCP/UDP, gaming│ Web apps, REST  │
│                   │ Ultra throughput│ Microservices  │
└──────────────────┴──────────────┴─────────────────┘
```

---

## 🧠 1-LINE MEMORY

```
L4 = "Envelope reader"  
     → Address dekha, koi bhi server bhej do
     → Fast, dumb

L7 = "Letter content reader"
     → Subject padha, sahi department bhej do
     → Slow, smart
```

---

## 4️⃣ Load Balancing Algorithms

```
┌──────────────────────┬────────────────────────────────┐
│ Algorithm            │  How                            │
├──────────────────────┼────────────────────────────────┤
│ Round Robin          │ S1 → S2 → S3 → S1 → S2 ...     │
│                       │ Simple, equal capacity assumed │
├──────────────────────┼────────────────────────────────┤
│ Least Connections    │ Server with FEWEST active conns │
│                       │ Best for long-lived connections │
├──────────────────────┼────────────────────────────────┤
│ Weighted Round Robin │ Bigger server = more weight     │
│                       │ S1 (weight 3) gets 3x more     │
├──────────────────────┼────────────────────────────────┤
│ IP Hash              │ hash(source IP) → server         │
│                       │ Same IP always → same server   │
│                       │ Useful for session affinity    │
├──────────────────────┼────────────────────────────────┤
│ Least Response Time  │ Fastest responding server       │
│                       │ Auto-adapts to performance     │
└──────────────────────┴────────────────────────────────┘
```

---

## 5️⃣ Health Checks ❤️‍🩹

```
LB ko kaise pata server alive hai ya nahi?

PERIODIC HEALTH CHECKS:
   LB → /health endpoint hit har 30 sec
   
   Server response:
      200 OK   ✅ → healthy, keep sending traffic
      500/timeout ❌ → unhealthy, REMOVE from pool
   
   Healthy hua wapas (3 consecutive OK) → add back
```

```
   LB                         Server
    │                            │
    │ GET /health ─────────────►│
    │                            │
    │ ◄───────────────── 200 OK │ ✅ keep using
    
   ... after 5 minutes ...
   
    │ GET /health ─────────────►│
    │                            │
    │ ◄───────────── timeout (X) │ ❌ remove from pool
```

---

## 6️⃣ Sticky Sessions (Session Affinity)

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
   
   ✅ Simple
   ❌ S1 crashes = user session lost
   ❌ Uneven load

SOLUTION 2: SHARED SESSION STORE
   All servers share Redis for sessions
   Any server can handle any request
   
   ✅ Stateless servers
   ✅ S1 crashes = no problem
   ✅ Modern approach (preferred)

SOLUTION 3: JWT (Stateless tokens)
   Token contains user info
   Any server validates and serves
   ✅ Truly stateless
   ✅ Tera UserCRUD use karta yeh
```

---

## 7️⃣ SSL Termination

```
SSL/TLS = encryption (HTTPS)
```

```
OPTION A: Termination at LB (common)
   ┌─────┐    HTTPS   ┌──────┐   HTTP   ┌────────┐
   │USER │ ─────────► │  LB  │ ───────► │SERVERS │
   └─────┘            └──────┘          └────────┘
   
   LB decrypts → forwards plain HTTP to backend
   ✅ Servers don't need SSL cert
   ✅ CPU offload from servers
   ✅ Cert management centralized
   ❌ Internal traffic unencrypted

OPTION B: Pass-through (end-to-end SSL)
   ┌─────┐    HTTPS   ┌──────┐   HTTPS  ┌────────┐
   │USER │ ─────────► │  LB  │ ───────► │SERVERS │
   └─────┘            └──────┘          └────────┘
   
   LB doesn't decrypt — forwards encrypted
   ✅ End-to-end encryption
   ❌ LB can't inspect (must be L4)
```

---

## 8️⃣ AWS Load Balancer Types

```
┌────────────┬───────────┬─────────────────────────┐
│  Type      │  Layer    │  Use Case                │
├────────────┼───────────┼─────────────────────────┤
│ ALB        │  L7        │ HTTP/HTTPS apps          │
│            │            │ Path-based routing       │
│            │            │ Microservices            │
├────────────┼───────────┼─────────────────────────┤
│ NLB        │  L4        │ TCP/UDP traffic          │
│            │            │ Gaming, video, IoT       │
│            │            │ Ultra-high throughput    │
├────────────┼───────────┼─────────────────────────┤
│ CLB        │ L4 + L7   │ LEGACY (don't use)       │
│ (Classic)  │            │ Old apps                 │
├────────────┼───────────┼─────────────────────────┤
│ GLB        │  L3        │ Geographic routing       │
│ (Global)   │            │ Multi-region failover    │
└────────────┴───────────┴─────────────────────────┘
```

---

## 9️⃣ Active-Active vs Active-Passive

```
ACTIVE-ACTIVE:
   ┌────┐  ┌────┐
   │ LB1│  │ LB2│  ← Both serving traffic
   └─┬──┘  └─┬──┘     (DNS load balances)
     │       │
     ▼       ▼
   servers servers
   
   ✅ Double capacity
   ✅ No idle resource
   ❌ More complex

ACTIVE-PASSIVE:
   ┌────┐  ┌────┐
   │ LB1│  │ LB2│  ← Only LB1 active
   └─┬──┘  └─X──┘     LB2 idle (standby)
     │       
     ▼       
   servers   
   
   LB1 crashes → LB2 takes over
   ✅ Simple
   ❌ Half capacity unused
```

---

## 🎤 INTERVIEW LINE

```
"Load balancer distributes traffic across servers.

 L4 routes on IP/port — fast, dumb (NLB).
 L7 routes on URL/host/header — smart, slower (ALB).

 Algorithms: round robin, least connections, IP hash.
 Health checks remove dead servers.
 Sticky sessions OR shared store OR JWT for state.

 SSL termination at LB for cert management + CPU offload.
 Active-active for capacity; active-passive for failover."
```

---

## 🎯 Memory Hooks

```
LB = Restaurant host (routes customers to waiters)

L4 = Envelope reader (fast, dumb)
L7 = Letter content reader (slow, smart)

5 algos: RR, Least-Conn, Weighted, IP-Hash, Least-Resp
Health: /health every 30s → remove dead
Sticky: cookie | Shared: Redis | JWT: stateless ✅
SSL: terminate at LB (common) | pass-through (L4 only)
AWS: ALB (L7) | NLB (L4)
```

📚 [← HLD README](../README.md)
