# 🌐 CDN — Content Delivery Network

---

## 1️⃣ Problem (1 line)

```
   USER (India)                    SERVER (USA)
       📱                              🖥️
        │                                │
        │   "netflix.com chahiye"        │
        │ ─────────────────────────────►│
        │   (12,000 km away)              │
        │                                │
        │  Response: 800ms latency        │
        │ ◄─────────────────────────────│

   ❌ Latency BAHUT zyada
   ❌ Server overload (sab requests yahin)
   ❌ Bandwidth costs zyada
   ❌ Peak time crash (Netflix night)
```

---

## 2️⃣ Pizza Analogy 🍕

```
WITHOUT CDN:
   Pizza Hut headquarters (USA) — sirf 1 kitchen
   India se order → 30 ghante delivery 😢

WITH CDN:
   Pizza Hut har city mein BRANCH
   Apne city ke branch se order → 30 min 🚀

= Same content, closer to user
= Faster delivery
= Less load on HQ
```

---

## 3️⃣ Architecture

```
                    ORIGIN SERVER (USA)
                          🖥️
                          │
              ┌───────────┼───────────┐
              ▼           ▼           ▼
       Edge Server   Edge Server  Edge Server
       (Mumbai 🇮🇳)  (London 🇬🇧)  (Tokyo 🇯🇵)
            │             │             │
            ▼             ▼             ▼
        USER India     USER UK       USER Japan
        (latency 50ms) (40ms)        (60ms)
        
   = User → NEAREST edge server
   = Origin sirf jab edge ke paas nahi ho
```

---

## 4️⃣ Cache Hit vs Cache Miss

```
SCENARIO A: CACHE HIT ✅
   User → Mumbai Edge: "Logo.png chahiye"
   Mumbai Edge: "Mere paas hai!"
   → 20ms response 🚀
   
   = Origin ko bother nahi kiya
   = FAST
```

```
SCENARIO B: CACHE MISS ❌
   User → Mumbai Edge: "NewMovie.mp4 chahiye"
   Mumbai Edge: "Mere paas nahi hai"
   Mumbai Edge → Origin (USA): "Bhej do"
   Origin → Mumbai Edge → User
   → 800ms first time
   
   BUT: Mumbai Edge ne YE FILE CACHE KAR LIYA
   Agla user (Mumbai) request kare = 20ms hit
   
   = First user slow, baaki sab fast
```

---

## 5️⃣ Kya Cache Hota Hai?

```
✅ STATIC content (high cache):
   • Images (logo, banners, thumbnails)
   • CSS files
   • JavaScript files
   • Videos (Netflix shows)
   • PDFs, documents
   • Font files

❌ DYNAMIC content (NO cache or short TTL):
   • User-specific data (account page)
   • Real-time prices (stock ticker)
   • Search results (personalized)
   • Cart contents
   
= STATIC = perfect for CDN
= DYNAMIC = origin se hi
```

---

## 6️⃣ Cache Invalidation (TRICKY)

```
Problem:
   Netflix logo change kiya origin pe
   Lekin Mumbai Edge mein PURANA logo cached
   → User ko purana dikhega
   
   "There are only two hard things in CS:
    cache invalidation and naming things"
```

```
SOLUTION 1: TTL (Time To Live)
   Logo.png: TTL = 1 hour
   1 hour baad edge khud purge karega
   = Simple, but stale window exists

SOLUTION 2: VERSIONING
   logo-v1.png → logo-v2.png
   Naam change = naya file = naya cache entry
   = Clean, common technique

SOLUTION 3: ACTIVE PURGE
   CloudFront API call → "purge logo.png"
   All edges immediately delete cached copy
   = Instant, but API cost
   = Use for emergencies
```

---

## 7️⃣ Push vs Pull CDN

```
PULL CDN (most common):
   User requests file from edge
   Edge has it? Serve.
   Edge doesn't? Fetch from origin + cache.
   
   = Lazy loading
   = First user pays cost
   = AWS CloudFront default

PUSH CDN:
   YOU push content to all edges proactively
   Pre-warm cache
   
   = Eager loading
   = High initial bandwidth cost
   = Use for predictable high traffic
     (movie release day, product launch)
```

---

## 8️⃣ Real CDN Providers

```
┌────────────┬─────────────────────────────┐
│ Provider   │  Strength                    │
├────────────┼─────────────────────────────┤
│ CloudFront │ AWS native, easy integration │
│ Akamai     │ Largest network (oldest)     │
│ Cloudflare │ Free tier + DDoS protection  │
│ Fastly     │ Real-time purge, Instagram   │
│ Open Connect│ Netflix's OWN CDN          │
└────────────┴─────────────────────────────┘
```

---

## 9️⃣ Netflix Open Connect (case study)

```
Netflix didn't use someone else's CDN —
THEY BUILT THEIR OWN.

🎬 Open Connect:
   • Netflix servers PHYSICALLY in ISP data centers
   • Direct fiber to ISP
   • Same network = ZERO transit
   
   = Movie loads in 100ms
   = ISP saves bandwidth (no peering cost)
   = Netflix saves money
   
   Win-win architecture
```

---

## 🔟 Request Flow Visual

```
   USER (Mumbai)
      │
      │ "video.mp4"
      ▼
   ┌────────────────┐
   │  DNS resolve   │
   │  → Mumbai Edge │
   └────────┬───────┘
            │
            ▼
   ┌────────────────────────┐
   │  Mumbai Edge Server    │
   │                          │
   │  Check cache?            │
   │  ┌──────────────────┐    │
   │  │ HIT? → serve     │ ✅ │
   │  │ MISS? → next     │    │
   │  └──────────────────┘    │
   └────────┬───────────────┘
            │ cache miss
            ▼
   ┌────────────────────────┐
   │  Origin Server (USA)   │
   │  Returns file           │
   └────────┬───────────────┘
            │
            ▼
   ┌────────────────────────┐
   │  Mumbai Edge CACHES it │
   │  + forwards to user     │
   └────────────────────────┘
            │
            ▼
        USER (gets file)
```

---

## 1️⃣1️⃣ Benefits Summary

```
┌──────────────────┬─────────────────────────┐
│  Benefit          │  How                     │
├──────────────────┼─────────────────────────┤
│ ⚡ Low latency    │ Edge near user           │
│ 📉 Origin offload│ Edge serves majority      │
│ 💰 Cost savings  │ Less bandwidth from origin│
│ 🛡️ DDoS defense  │ Edges absorb attacks      │
│ 🌍 Global scale  │ 100s of POPs worldwide    │
│ 📈 Uptime         │ Origin down = edge serves │
└──────────────────┴─────────────────────────┘
```

---

## 1️⃣2️⃣ When NOT to use CDN

```
❌ Highly dynamic content (user-specific)
❌ Frequently changing data (real-time stock prices)
❌ Internal-only apps (no global users)
❌ Very low traffic (cost > benefit)
```

---

## 🎯 INTERVIEW LINE (memorize)

```
"CDN is geographically distributed caching.
 Content served from edge nearest to user —
 reduces latency, offloads origin, scales globally.
 
 Static content cached aggressively;
 dynamic content bypasses CDN or uses short TTL.
 
 Cache invalidation via TTL, versioning, or active purge.
 
 Examples: CloudFront, Akamai, Cloudflare;
 Netflix built Open Connect — placed servers
 directly inside ISP data centers."
```

---

## 🎯 Memory Hooks

```
CDN = Pizza chain (branches close to you)

Cache HIT  = "Mere paas hai" → 20ms
Cache MISS = "Origin se mangwa" → 800ms first time

3 invalidation:
   TTL    → expiry timer
   Version → naam change
   Purge  → emergency delete

Push vs Pull:
   PULL = lazy (first user pays)
   PUSH = eager (pre-warm)
```

📚 [← HLD README](../README.md)
