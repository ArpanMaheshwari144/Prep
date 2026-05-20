# Load Balancing

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

---

## Nginx Deep — User's Question

### **Nginx ≠ AWS** (open-source software, not AWS-specific!)

```
Nginx = Open-source software (free, install anywhere)
AWS   = Cloud platform (provides infrastructure)
```

Nginx run kar sakte: AWS EC2, Azure VM, GCP, laptop, on-premise.

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
```
