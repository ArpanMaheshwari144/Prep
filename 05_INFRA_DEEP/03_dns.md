# 🌐 DNS — Domain Name System

---

## 1️⃣ Problem Without DNS

```
Without DNS, har site ke liye IP yaad karna:

   Google     → 142.250.190.46
   Facebook   → 157.240.0.35
   Netflix    → 54.230.123.45
   YouTube    → 142.251.40.46
   
   ❌ IMPOSSIBLE to remember
   ❌ Server change ho jaaye IP = sab tut jaaye
```

---

## 2️⃣ Phonebook Analogy 📞

```
Old days:
   Phonebook kholo
   "Ramesh Mumbai" search karo
   → 9876543210 mil gaya
   → Dial karo

Internet:
   Browser kholo
   "google.com" type karo
   DNS phonebook check:
   → 142.250.190.46 mil gaya
   → Connect karo

= Domain name = Human friendly name
= IP address = Machine readable number
= DNS = Translator (name → number)
```

---

## 3️⃣ DNS Hierarchy

```
   ┌──────────────────────┐
   │   ROOT (.)           │  ← Top of hierarchy
   │   13 root servers     │     globally
   └──────────┬───────────┘
              │
   ┌──────────┴───────────┐
   │   TLD                 │  ← Top Level Domain
   │   .com .org .in .net  │     "Country / category"
   └──────────┬───────────┘
              │
   ┌──────────┴───────────┐
   │   AUTHORITATIVE       │  ← Domain owner's servers
   │   google.com NS server│     "Where this domain lives"
   └──────────┬───────────┘
              │
              ▼
       Actual IP: 142.250.190.46
```

```
Real example: www.google.com
   .              → Root server
   com            → TLD server (handles .com)
   google.com     → Authoritative server (Google's NS)
   www.google.com → Final answer (IP)
```

---

## 4️⃣ DNS Resolution Flow (8 STEPS)

```
User types "www.google.com" in browser

1. Browser cache check
   ❌ Not found

2. OS cache check
   ❌ Not found

3. Browser asks LOCAL RESOLVER (ISP DNS)
   "google.com ka IP do"

4. Resolver asks ROOT server
   Root: "I don't know exact IP,
          but ask .com TLD server at X"

5. Resolver asks TLD (.com) server
   TLD: "google.com ka NS is ns1.google.com at Y"

6. Resolver asks Google's NS (authoritative)
   NS: "www.google.com → 142.250.190.46"

7. Resolver returns IP to browser
   + CACHES it (TTL)

8. Browser connects to 142.250.190.46
   Page loads ✅
```

```
Visual:
   USER → BROWSER → OS → LOCAL RESOLVER
                              │
                              ▼
                            ROOT (.)
                              │
                              ▼
                          TLD (.com)
                              │
                              ▼
                       AUTHORITATIVE
                       (ns.google.com)
                              │
                              ▼
                       IP: 142.250.190.46
                              │
                              ▼
                          BROWSER ✅
```

---

## 5️⃣ DNS Record Types

```
┌─────────┬──────────────────────────────────────┐
│  Type   │  Purpose                              │
├─────────┼──────────────────────────────────────┤
│  A       │ Domain → IPv4 address                 │
│          │ google.com → 142.250.190.46           │
├─────────┼──────────────────────────────────────┤
│  AAAA   │ Domain → IPv6 address                 │
│          │ google.com → 2607:f8b0:4005:80a::200e │
├─────────┼──────────────────────────────────────┤
│  CNAME  │ Alias to another domain               │
│          │ www.example.com → example.com          │
│          │ (canonical name)                       │
├─────────┼──────────────────────────────────────┤
│  MX     │ Mail server                            │
│          │ example.com mail → mx.example.com      │
├─────────┼──────────────────────────────────────┤
│  TXT    │ Text data (SPF, verification)         │
│          │ "v=spf1 include:_spf.google.com"       │
├─────────┼──────────────────────────────────────┤
│  NS     │ Name server for domain                 │
│          │ example.com → ns1.example.com         │
├─────────┼──────────────────────────────────────┤
│  SOA    │ Start of Authority                     │
│          │ Zone metadata (admin email, refresh)  │
├─────────┼──────────────────────────────────────┤
│  PTR    │ Reverse lookup (IP → domain)          │
│          │ 142.250.190.46 → google.com           │
└─────────┴──────────────────────────────────────┘
```

---

## 6️⃣ TTL + Caching Levels

```
TTL (Time To Live) — kitne der cache valid

  CACHING LAYERS:
  
  ┌──────────────────────┐
  │  BROWSER cache       │  TTL: short (seconds)
  └──────────┬───────────┘
             ▼
  ┌──────────────────────┐
  │  OS cache (DNS)      │  TTL: longer (minutes)
  └──────────┬───────────┘
             ▼
  ┌──────────────────────┐
  │  LOCAL RESOLVER (ISP)│  TTL: hours
  └──────────┬───────────┘
             ▼
  ┌──────────────────────┐
  │  AUTHORITATIVE (NS)  │  Source of truth
  └──────────────────────┘
```

```
TTL trade-off:
   Short TTL  → Fresh data, more DNS lookups
   Long TTL   → Less load, but slower propagation
   
   Common values:
      300 sec (5 min)   = quick updates
      3600 sec (1 hr)   = balance
      86400 sec (1 day) = stable, less load
```

---

## 7️⃣ Route 53 Routing Policies (AWS)

```
Route 53 = AWS DNS service
Different routing POLICIES based on need:
```

```
1. SIMPLE ROUTING
   google.com → single IP
   = Basic A record
   = No logic, just resolve
```

```
2. WEIGHTED ROUTING
   google.com → 
      Server A (70% traffic)
      Server B (30% traffic)
   
   = A/B testing
   = Canary deployments
```

```
3. LATENCY-BASED ROUTING
   User in India → Mumbai region (low latency)
   User in USA → Virginia region (low latency)
   
   = Multi-region apps
   = Auto-routes to nearest data center
```

```
4. GEOLOCATION ROUTING
   User from India → Hindi content server
   User from Japan → Japanese content server
   
   = Location-based content
   = Compliance (data residency)
```

```
5. FAILOVER ROUTING
   PRIMARY healthy → route to primary
   PRIMARY down → route to SECONDARY automatically
   
   = Disaster recovery
   = Active-passive setup
```

```
6. MULTI-VALUE ROUTING
   google.com → returns multiple IPs
   Client picks one (or rotates)
   
   = Simple DNS-based load balancing
```

```
7. GEOPROXIMITY ROUTING
   Bias traffic by geographic location
   "Send 60% of India traffic to Mumbai, 40% to Singapore"
   
   = Fine-grained geographic control
```

---

## 8️⃣ DNS-Based Load Balancing

```
google.com → A record returns:
   142.250.190.46
   142.250.190.78
   142.250.190.110
   142.250.190.142

Client picks first (or rotates)
= Simple load balancing via DNS

❌ Limitations:
   • No health check at DNS level
   • TTL caching = dead server kept being tried
   • No advanced routing logic
   
✅ When to use:
   • Geographic distribution
   • Multi-region failover
```

---

## 9️⃣ DNS Propagation

```
Naya domain register kiya / IP change kiya?
World mein sab DNS servers update hote hain
= "Propagation"

Time: 1 min to 48 hours
Depends on TTL of records

   ┌─────────────────────────────────────────┐
   │ Authoritative NS updated (instant)      │
   └────────────┬────────────────────────────┘
                │
                ▼
   ┌─────────────────────────────────────────┐
   │ ISP resolver: TTL expire wait           │
   └────────────┬────────────────────────────┘
                │
                ▼
   ┌─────────────────────────────────────────┐
   │ Browser/OS cache: TTL expire wait       │
   └────────────┬────────────────────────────┘
                │
                ▼
            Update visible to user
```

```
Tip: Before big change, REDUCE TTL to 60 sec
     a day before — faster propagation when needed
```

---

## 🔟 Anycast (Advanced)

```
Same IP advertised from MULTIPLE locations.
User's request routed to NEAREST one.

   142.250.190.46 advertised from:
      Mumbai server
      London server
      Tokyo server
   
   User in India → automatically routed to Mumbai
   User in UK → automatically routed to London
   
= How root DNS servers work (13 IPs, 100s of locations)
= Cloudflare, Google DNS use this
= Built-in geographic load balancing
```

---

## 🎤 INTERVIEW LINE

```
"DNS translates human-readable domains to IP addresses.
 Hierarchical system: Root → TLD → Authoritative.

 Records: A (IPv4), AAAA (IPv6), CNAME (alias),
 MX (mail), TXT (verification), NS (delegation).

 Caching at browser, OS, resolver levels with TTL.

 Route 53 routing policies — simple, weighted, latency,
 geolocation, failover, multi-value, geoproximity.

 DNS-based LB is simple but TTL-limited;
 Anycast for geographic auto-routing."
```

---

## 🎯 Memory Hooks

```
DNS = Internet phonebook (name → IP)

Hierarchy: ROOT → TLD (.com) → AUTHORITATIVE

8-step flow: Browser → OS → Resolver → Root → TLD → NS → IP

Records: A (IPv4), AAAA (IPv6), CNAME, MX, TXT, NS

Route 53 policies (7):
   Simple, Weighted, Latency, Geolocation,
   Failover, Multi-value, Geoproximity

Caching: Browser → OS → Resolver → NS (TTL-based)

Anycast = Same IP, many locations (auto-nearest)
```

📚 [← HLD README](../README.md)
