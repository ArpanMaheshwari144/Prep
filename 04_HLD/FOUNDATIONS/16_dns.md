# DNS — Domain Name System

> **NAV** — KYA: DNS (resolution, records, TTL, Route 53, anycast). saath: [load-balancing](03_load_balancing.md) · lagta hai: [browser-journey](19_url_browser_journey.md)

---

## 1. Naam se IP kaise milta hai

**Koi poochhe to aise bolo:**
```
DNS = internet ki phonebook. Hum netflix.com likhte hain, machine ko IP chahiye. Phone pehle apne cache me
dekhta hai, phir ISP ke resolver se poochta hai. Resolver ke paas na ho to wo upar se neeche poochta hai:
root ("com walon se poocho"), phir .com ("netflix ka register ye hai"), phir Netflix ka apna DNS, jo IP batata hai.
Jawab har jagah TTL tak cache rehta hai, isliye agli baar ye poora safar nahi hota.
```

**Scene:** Arpan ne pehli baar netflix.com khola. Jio ka resolver root se poochta hai, phir .com se, phir Netflix ke DNS se, aur IP mil jaata hai. Ye pehli baar ~100 ms laga. TTL 5 minute hai, to agle 5 minute tak PG ke sab log Jio resolver ke cache se turant IP paate hain.

**Do record yaad rakho, baaki naam hain:**
```
A      naam -> IP          (netflix.com -> 54.x.x.x)
CNAME  naam -> doosra naam  (www.shop.com -> shop.cloudfront.net, CDN pe bhejne ke liye yahi lagta hai)
```

**Browser cache clear kiya to site dheere kyun:** browser site ki JS, CSS, photo, font apne paas rakhta hai. Clear kiya to pehli baar sab dobara net (CDN) se aata hai, aur kai baar DNS ka sawaal aur naya TLS connection bhi dobara. Ek baar khuli to cache phir bhar gaya, agli baar tez. (Cookies bhi saaf ki to login dobara, wo session mitna hai, dheema-tez nahi.)

**Trap:** IP badla, par log purane IP pe hi ja rahe hain, kyunki unke resolver ke paas purana jawab TTL tak pada hai. Isliye bade badlaav se ek din pehle TTL ghata ke 60 sec kar do. Badlaav ke baad sab jaldi naye IP pe aa jaate hain, aur kaam ho jaaye to TTL wapas badha do.

---

## 7 Route 53 Routing Policies (AWS)

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

## 8 DNS-Based Load Balancing

```
google.com → A record returns:
   142.250.190.46
   142.250.190.78
   142.250.190.110
   142.250.190.142

Client picks first (or rotates)
= Simple load balancing via DNS

Limitations:
   • Simple round-robin DNS me health check nahi (Route 53 jaisi managed DNS me hai)
   • TTL caching = dead server kept being tried
   • No advanced routing logic

When to use:
   • Geographic distribution
   • Multi-region failover
```

---

## Anycast (Advanced)

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

## INTERVIEW LINE

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

## Memory Hooks

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

