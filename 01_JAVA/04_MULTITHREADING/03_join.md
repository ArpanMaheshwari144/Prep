# join() — Thread Ke Khatam Hone Ka Wait

**Kahani:** file 01 wala API yaad kar. Teen thread bheje: user, orders, recommendations. Agar `main` thread unka wait kiye bina seedha response bhej de, to user ko khaali ya adhoora data milega. `join()` ka matlab hai "teeno laut aayein, tabhi response bhejo". Jaise tu do doston ko kaam pe bheje aur meeting point pe dono ka intezaar kare, phir saath aage badhe.

Tera [RaceDemo](practical/RaceDemo.java) me join se pehle count print karo to adhoora aata hai (threads abhi chal rahe hote hain), join ke baad poora `2000000`.

```
main:  start(t1) start(t2) ──[t1.join WAIT]──[t2.join WAIT]── println
t1:              [═══════ chal raha ═══════] done
t2:              [═══════ chal raha ═══════] done    <- dono SAATH chal rahe
```

**Khaas baat:**
1. `join()` sirf `main` ko rokta hai. Threads phir bhi saath-saath hi chalte hain, ek ke baad ek nahi. Upar tasveer me dekh: t1 aur t2 dono ek saath chal rahe hain.
2. `join()` se sirf timing theek hoti hai, race theek nahi hoti. Count sahi aane ke liye do cheezein chahiye: join (sab khatam hone ke baad padho) aur atomic (beech me koi update gum na ho). RaceDemo me dono hain, isliye exact 2000000 aaya.
3. `join()` checked `InterruptedException` deta hai, isliye method me `throws` likhna padta hai.

**Prod wala trap:** `t.join()` bina time ke likha, aur thread kisi atke hue DB call pe ruk gaya. Ab `main` hamesha ke liye ruka rahega, request kabhi lautegi hi nahi. Isliye time de do:
```java
t.join(2000);                      // max 2 sec wait
if (t.isAlive()) t.interrupt();    // ab bhi atka hai to rukne ko bolo
```
Asli code me yahi kaam `future.get(2, SECONDS)` se hota hai (file 11). Ye wahi baat hai jo PR drill me baar-baar aati hai: "bahar ki call me timeout lagao".

```
Interview: "t.join() makes the calling thread wait until t finishes. Threads still run in parallel -
join only blocks the caller. It fixes timing, not race conditions - for that you need
synchronized or atomics. In production I use join(timeout) or future.get(timeout)
so a stuck thread can't hang the request forever."
```
