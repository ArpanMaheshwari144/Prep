# Jab ILAAJ hi BIMARI ban jaaye — cascading / metastable failure

> **Ye foundation baaki se alag hai.** Baaki files batati hain ki ek cheez **kaam kaise karti hai**.
> Ye file ek hi sawaal ki hai: *sab kuch sahi laga hua tha, sab chal raha tha — phir bhi system gir
> gaya. Kyun?*
>
> Ye poora section Arpan ke sawaal se bana (20-Sep): *"maan le LB lagaya taaki server pe dabaav na
> pade, phir bhi dabaav aa gaya — us case me kya? Redis lagaya aur lagte hi server down.
> Rate limiter lagaya par usne kaam nahi kiya. Queue spike absorb karne ki jagah traffic badha de
> to?"* — chaaron ka jawab chaar alag file me hai; ye page unhe ek shakal me jodta hai.

---

## ★★ EK HI SHAKAL — chaaron jagah wahi

```
LB me      "BACHANE wali cheez ne maara"
              health check ne SAB server dead mark kar diye
              retry ne load 3x kar diya
              least-connections ne NAYE server ko maar diya
              sticky ne ek server GARAM kar diya

cache me   "TEZ karne wali cheez ne RAASTA ROK diya"
              slow Redis + timeout nahi = thread pool bhara, DB khaali baitha
              aur jo load chhupaya tha wo ek din EK SAATH wapas aaya

limiter me "ROKNE wali cheez ne BHEED ko roka hi nahi"
              per-user limit abuse rokti hai, aggregate load nahi
              aur attack ke waqt fail-open se wo KHUD gayab ho gayi

queue me   "ABSORB karne wali cheez ne spike JAMA karke BADA karke chhoda"
              10 min ka backlog consumer ne poori raftaar se DB pe de maara
```

**Poora detail har apni file me — yahan sirf naksha:**

| kahan | section | file |
|---|---|---|
| Load balancer | CHHE TARIKE JINSE LB KHUD SYSTEM KO MAARTA HAI | [03_load_balancing.md](03_load_balancing.md) |
| Cache / Redis | "REDIS LAGATE HI SERVER DOWN HO GAYA" | [04_caching.md](04_caching.md) |
| Rate limiter | "LIMITER LAGA THA, PHIR BHI NAHI BACHA" | [../SYSTEM_DESIGNS/02_rate_limiter/02_rate_limiter.md](../SYSTEM_DESIGNS/02_rate_limiter/02_rate_limiter.md) |
| Queue / Kafka | "QUEUE NE SPIKE ABSORB NAHI KIYA" | [07_message_queues.md](07_message_queues.md) |

---

## DO NAAM — inhe alag-alag samjho

```
CASCADING FAILURE
   ek cheez girne se AGLI girti hai, phir agli
   3 server x 70% -> ek gira -> bache 2 pe 105% -> dono gire

METASTABLE FAILURE   (tedha roop)
   load HAT jaane ke BAAD bhi system apne aap theek NAHI hota
   kyunki ab retry ka backlog KHUD hi naya load ban chuka hai

   -> traffic band karo, backlog PHENKO, system ko ZABARDASTI reset karo
   -> "wait kar lete hain, theek ho jaayega" yahan kaam NAHI karta
```

---

## ★★ AATH ASOOL — jo chaaron kahaniyon se nikalte hain

```
1. HAR BACHANE WALI CHEEZ KA APNA FAILURE MODE HOTA HAI
      health check · retry · cache · queue · limiter — koi muft nahi
      "X laga diya" ke baad agla sawaal hamesha: "aur X khud kharab hua to?"

2. LOAD HATAYA NAHI JAATA, KHISKAYA JAATA HAI — dekho KAHAN gaya
      LB ne load baanta (ghataya nahi)
      cache ne load DB se hata ke Redis pe daala
      queue ne load ABHI se BAAD me khiskaya
      consumer autoscale ne load queue se DB pe daal diya

3. JO FIX LOAD KO CHHUPA DE, WO CAPACITY PLANNING KO JHOOTHA KAR DETA HAI
      DB ko cache ki aadat lag gayi -> uski capacity ab "cache wale" numbers pe tay hui
      -> cache gaya -> poora asli load ek jhatke me wapas -> DB ne wo kabhi dekha hi nahi

4. HAR BAAHRI CALL PE TIMEOUT + FALLBACK
      slow cheez down cheez se ZYADA khatarnak hai
      down = turant pata chalta; slow = thread pakad ke baith jaati aur sab ruk jaata

5. RETRY LOAD KO GUNA KARTA HAI — theek us waqt jab system doob raha ho
      retry budget (kul ka ~10%) + backoff + JITTER
      aur retry sirf wahan jahan dobara chalana safe ho

6. JAGAH CHHOD KE CHALO (N+1)
      ek server ke marne pe jeena hai -> normal me har server 100/(N-1) se NEECHE
      3 server = ~66% se neeche, warna pehla marte hi baaki bhi jaayenge

7. AADHI KHARABI KO POORI KHARABI MAT BANNE DO
      DB gaya -> jo cache se chal sakta hai wo CHALNE DO
      "sab unhealthy" ka matlab aksar "CHECK galat hai" hota hai -> panic mode
      graceful degradation > poora blackout

8. ALERT WAHAN LAGAO JAHAN CHEEZ CHHUPTI HAI
      error rate pe nahi — wo aakhir me badhta hai
      QUEUE DEPTH · oldest message ki UMAR · cache HIT RATIO ·
      connection POOL usage · p99 latency
```

---

## ★ INTERVIEW ME BOLNE KA DHAANCHA — "X fail ho gaya to?"

```
1. "pehle dekhta hoon X ne load HATAYA hai ya KHISKAYA hai — aur kahan khiskaya hai"
2. "X khud kab kharab hoga: mar ke, ya SLOW ho ke? dono ka ilaaj alag hai"
3. "X ke bina system kya kar sakta hai — poora band, ya kam kaam ke saath zinda?"
4. "aur jab X wapas aayega, tab ka spike kya hoga?"      <- ye chautha sawaal log bhool jaate
```

> ★ Chautha sawaal hi sabse kaam ka hai: recovery apne aap me ek spike hoti hai.
> Consumer wapas aaya, cache garam ho rahi hai, saare client ek saath dobara jud rahe hain —
> ye teeno recovery ke waqt hi maarte hain.

---

## ★ IMAANDARI (20-Sep, jaisa likha gaya)

```
FACT   upar likha sab technology ka BEHAVIOUR hai — ye aise hi kaam karta hai.
       Ispe khade raho.

GUESS  "interview me aisa poocha JAATA hai" — ye writeups se hai.
       Itna kaafi hai: senior-backend design round me "X fail hua to?" wale sawaal
       writeups me aksar milte hain, aur 700-ticket wale background ke saath
       ye Arpan ka sabse AASAN zone hai — ye cheezein usne asli me dekhi hain.
```

---

[← FOUNDATIONS](.) · [← Home README](../../README.md)
