# CDN — Content Delivery Network

> **NAV** — KYA: CDN (edge cache, invalidation, CDN kaise system girata). saath: [caching](04_caching.md) · lagta hai: [file-upload](../SYSTEM_DESIGNS/07_file_upload_validate_system/07_file_upload.md) · [twitter-feed](../SYSTEM_DESIGNS/03_twitter_feed/03_twitter_feed.md)

---

## 1. CDN kya hai aur kaise kaam karta hai

**Koi poochhe to aise bolo:**
```
CDN = duniya bhar me faili cache machines (edge), user ke paas. File pehli baar maangi gayi to edge ke paas
nahi hoti (miss): edge asli server (origin) se laata hai, rakh leta hai, aur user ko de deta hai. Us shehar ka
agla user wahi file edge se paata hai (hit), tez, aur origin ko pata bhi nahi chalta.
Isse teen fayde: user ke liye tez, origin pe bojh kam, aur door ka bandwidth bachta hai.
```

**Scene (Netflix poster):** origin America me hai. Arpan Bangalore PG se app kholta hai, aur "Dark" ka poster 12,000 km se aata hai, ~800 ms me. CDN laga diya: pehli baar Bangalore edge ke paas poster nahi tha, to wo America se laaya, rakh liya, aur Arpan ko de diya. Pehli baar dheema hi raha. Agle 10 lakh log Bangalore edge se ~20 ms me poster paate hain, aur America ko ye request dikhti hi nahi.

**Pull ya push:**
```
pull  edge tab laata hai jab koi maange. Pehla user der jhelta hai. Aam taur pe yahi (CloudFront default)
push  hum pehle se saari edges pe daal dete hain. Bada din pata ho tab: naye season ki release raat,
      sale ka din (Netflix notes ka "raat ko pehle se ISP ke dabbe me bhar do" yahi hai)
```

**Trap:** "origin gira to CDN sab sambhal lega". CDN sirf wahi de sakta hai jo uske paas pehle se rakha hai, aur utni der jitni TTL hai. Nayi cheez, login aur payment sab origin pe hi jaate hain. CDN backup server nahi hai, cache hai.

---

## 5 Kya Cache Hota Hai?

```
STATIC content (high cache):
   • Images (logo, banners, thumbnails)
   • CSS files
   • JavaScript files
   • Videos (Netflix shows)
   • PDFs, documents
   • Font files

DYNAMIC content (NO cache or short TTL):
   • User-specific data (account page)
   • Real-time prices (stock ticker)
   • Search results (personalized)
   • Cart contents

= STATIC = perfect for CDN
= DYNAMIC = origin se hi
```

---

## 6 Cache Invalidation (TRICKY)

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
   All edges se cached copy hatao
   = CloudFront pe seconds-minutes lagte (Fastly lagbhag turant), aur API cost
   = Use for emergencies
```

---

## 8 Real CDN Providers

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

## 9 Netflix Open Connect (case study)

```
Netflix didn't use someone else's CDN —
THEY BUILT THEIR OWN.

Open Connect:
   • Netflix servers PHYSICALLY in ISP data centers
   • Direct fiber to ISP
   • Same network = ZERO transit

   = Movie jaldi shuru, buffering kam
   = ISP saves bandwidth (no peering cost)
   = Netflix saves money

   Win-win architecture
```

---

## 11 Benefits Summary

```
┌──────────────────┬─────────────────────────┐
│  Benefit          │  How                     │
├──────────────────┼─────────────────────────┤
│ Low latency    │ Edge near user           │
│ Origin offload│ Edge serves majority      │
│ Cost savings  │ Less bandwidth from origin│
│ DDoS defense  │ Edges absorb attacks      │
│ Global scale  │ 100s of POPs worldwide    │
│ Uptime         │ Origin down = edge serves │
│                │ (sirf jo cached hai, TTL  │
│                │  ya stale-if-error tak)   │
└──────────────────┴─────────────────────────┘
```

---

## 12 When NOT to use CDN

```
Highly dynamic content (user-specific)
Frequently changing data (real-time stock prices)
Internal-only apps (no global users)
Very low traffic (cost > benefit)
```

---

## ★★ DEPTH-PASS (25-Sep) — CDN andar se + CDN KHUD kaise maarta hai

> Upar = CDN KYA hai. Ye section = andar ki mashine (cache key, headers, signed URL) aur
> wo 6 tarike jinse CDN khud system gira deta hai. Shakal wahi jo LB aur cache me thi.

### A. Cache KEY + headers — edge kaise tay karta "kya rakhu, kitni der"

```
CACHE KEY = edge kis cheez se pehchaanta "ye wahi file hai"
            default: URL (+ jo headers / query param tu chune)

Origin header se batata hai:
   Cache-Control: public, max-age=86400   ->  CDN + browser 1 din rakho
   Cache-Control: private                 ->  sirf browser, CDN NAHI (user ka apna data)
   Cache-Control: no-store                ->  koi nahi rakhe
```

### B. Versioned URL + TTL ki jodi (industry ka asli tareeka)

```
app.js  ->  app.3f9a.js     (content ka hash naam me — build tool khud karta)

versioned files (js / css / image)   ->  TTL 1 SAAL  (naam hi badlega, purana kabhi galat nahi)
index.html (jo naye naam batata)     ->  TTL chhota / no-cache
=> purge ki zaroorat lagbhag khatam. Purge = emergency ke liye.
```

### C. Private file — SIGNED URL

```
paid video / bank statement PDF bhi CDN se chahiye, par sirf us user ko
server ek URL banata: file + expiry (10 min) + signature
CDN signature check karta -> galat / expire -> 403
(S3 pre-signed URL jaisa, bas CDN level pe — CloudFront signed URL / signed cookie)
```

### D. Dynamic bhi CDN se GUZAR sakta hai (cache nahi, raasta tez)

```
upar likha "DYNAMIC = origin se hi" — CACHE ke liye sahi.
par dynamic request bhi CDN se guzar sakti hai, bina cache hue:
   TLS handshake paas wali edge pe (door ke origin se nahi)
   edge -> origin pehle se khula, garam connection
=> "dynamic site acceleration". Cloudflare / CloudFront dono karte hain.
```

### E. ★★ CHHE TARIKE JINSE CDN KHUD SYSTEM KO MAARTA HAI

```
1. GALAT USER KO GALAT DATA  (sabse khatarnak)
   personalized page pe Cache-Control bhoola -> CDN ne Arpan ka "My Account" rakh liya
   -> Rahul ko Arpan ka page dikha
   asli: Steam, Christmas 2015 — caching config change, logon ko doosron ke account page dikhe
   ilaaj: user-specific pe private / no-store HAMESHA. Auth wale response cache nahi.

2. PURGE / DEPLOY ke baad ORIGIN pe toofan
   "purge all" kiya ya sabka TTL ek saath khatam -> har edge MISS -> sab ek saath origin pe
   origin normal me ~5% dekhta tha (95% hit) -> achanak 100% -> gira
   ilaaj: ORIGIN SHIELD   = edges aur origin ke beech ek aur cache layer
                            (sau edge ki miss -> shield pe; origin ko sirf shield ki miss)
          REQUEST COLLAPSING = ek hi file ki 1000 miss -> origin pe EK request
          stale-while-revalidate = purana do, peeche se naya laao
   (ye cache notes wala STAMPEDE hi hai, duniya bhar ke scale pe)

3. GALAT CACHE KEY -> hit ratio gira
   ?utm_source=fb, ?utm_source=insta -> same image, alag key -> har ek MISS
   hit ratio 95% -> 40% = origin pe ~12 guna bojh, BINA traffic badhe
   ilaaj: key me faltu query param ignore, sirf wo rakho jo content badalta hai

4. ERROR CACHE ho gaya
   origin ne 30 sec 500 / 404 diya -> CDN ne wahi rakh liya TTL tak
   origin theek ho gaya, user ko 10 min tak error
   ilaaj: error response pe TTL bahut chhota ya zero

5. PURANA content, deploy ke baad page toota
   naya HTML + CDN pe purana JS -> function mile hi nahi
   ilaaj: upar wala versioned URL (B)

6. CDN KHUD gira
   provider down = poori site down (Fastly, June 2021 — bahut badi sites ek saath gayi)
   ilaaj: multi-CDN + DNS failover
   SACH: "CDN gira to origin sambhal lega" plan NAHI hai — origin 5% ke liye bana tha
```

### F. Ek shakal (wahi LB aur cache wali)

```
CDN origin ko BACHATA hai (95% traffic edge pe khatam)
=> jis pal CDN miss karne lage (purge, galat key, CDN down), origin ~20 guna bojh ke saamne
   jo usne kabhi dekha hi nahi. Bachane wali cheez hi girane wali.
```

> **BOLNE WALI LINE:** *"CDN origin ko bachata hai, isliye uski asli keemat tab dikhti hai jab wo
> miss karne lage. Main teen cheez dekhta hoon: user-specific response kabhi cache na ho (private /
> no-store), static files versioned naam + lambi TTL ke saath, aur origin ke aage shield + request
> collapsing taaki purge ya expiry pe saari edges ek saath origin pe na toot padein."*

---

## INTERVIEW LINE (memorize)

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

## Memory Hooks

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

