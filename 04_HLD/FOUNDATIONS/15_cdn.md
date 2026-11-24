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

## 2. CDN pe kya rakhein, kya nahi

**Koi poochhe to aise bolo:**
```
Jo sabke liye ek jaisa hai (poster, JS, CSS, video, font), wo CDN pe lambi der tak. Jo ek user ka apna hai
(account page, cart, order) ya har second badalta hai (share ka daam), wo cache nahi hota, origin se aata hai.
Ye origin header se batata hai: public, max-age = CDN rakh le; private = sirf browser; no-store = koi nahi.
Paid / private file bhi CDN se chahiye to signed URL: file + kab tak valid + sign, galat ya purana link = 403.
```

**Signed URL (file upload kadam 6b se):** Ravi ki bank statement PDF CDN se tez aaye, par sirf Ravi ko mile. Server 10 minute ka signed link banata hai. CDN sign check karta hai, theek hai to file de deta hai. 10 minute baad wahi link 403 deta hai.

**Dynamic bhi CDN se guzar sakta hai:** cart cache nahi hoti, par request phir bhi paas wali edge se guzarti hai. TLS ka taala Bangalore me hi khul jaata hai, aur edge ka origin tak pehle se khula, garam taar hota hai. Isse raasta tez hota hai, bina kuch cache kiye.

**Trap (galat user ko galat data):** account page pe `private` lagana bhool gaye. CDN ne Arpan ka "My Account" rakh liya, aur Ravi ko Arpan ka page dikh gaya. Steam ke saath Christmas 2015 pe yahi hua tha: caching ki setting badli, aur logon ko doosron ke account page dikhe. Isliye user ke apne data pe hamesha `private` / `no-store` lagao, aur login wale jawab kabhi cache mat karo.

---

## 3. Purana maal CDN se kaise hatayein

**Koi poochhe to aise bolo:**
```
Teen raaste hain. TTL: itni der baad edge khud purana phenk de, simple hai par tab tak purana dikhega.
Versioned naam: app.3f9a.js, file badli to naam badla, to naya cache, purana kabhi galat dikhega hi nahi.
Isliye aise file pe TTL 1 saal, aur sirf index.html (jo naye naam batata hai) pe chhota TTL.
Purge: CDN ko bolo "ye file har edge se hatao", emergency ke liye, seconds se minute lagte hain.
```

**Scene (Netflix logo badla):** naya logo daala, par Bangalore edge ke paas purana rakha hai. TTL 1 ghanta hai to 1 ghante tak purana dikhega. Agar naam `logo-v2.png` kar diya to naya page naya naam maangega, aur edge ke paas wo hai hi nahi, to wo origin se naya le aayega. Galat logo pe legal ka notice aaya to purge karo.

**Trap (deploy ke baad page toota):** naya HTML aa gaya, par CDN pe purana JS pada hai, jisme naya function hai hi nahi, to page toot gaya. Ilaaj: versioned naam. Tab purge ki zaroorat lagbhag khatam ho jaati hai.

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

