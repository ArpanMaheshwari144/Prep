# join(): Thread Ke Khatam Hone Ka Wait

## Scene: do ladke godown, bill tab jab dono laut aayein

Customer counter pe khada hai. Mausi ne do ladke godown bheje, ek ghee laane aur ek daal. Bill tabhi banna chahiye jab
dono laut aayein. Agar cashier (main thread) unka intezaar kiye bina bill bana de, to thaila khaali jaayega. Chala ke
dekha:

```
Bina join, bill:    []              <- ladke abhi godown me hi the
join ke baad bill:  [ghee, daal]    <- dono ke laut aane ka intezaar kiya
```

## Khaas baat

`join()` sirf cashier (main) ko rokta hai. Dono ladke phir bhi ek saath godown jaate hain, ek ke baad ek nahi. Isliye kul
time 500 ms laga (daal wale ka), 800 nahi.

`join()` se sirf timing theek hoti hai, jhagda (race) nahi. Agar dono ladke ek hi register me ek saath likhein, to galti ho
sakti hai, aur uska ilaaj `synchronized` ya atomic hai (file 04 aur 06). [RaceDemo](practical/RaceDemo.java) exact 2000000
isliye deta hai kyunki usme join bhi hai aur atomic bhi.

`join()` checked `InterruptedException` deta hai, isliye method me `throws` likhna padta hai.

## Prod wala trap

Maan le teesra ladka tel laane gaya aur godown me atak gaya. Agar `join()` bina time ke likha hai, to cashier hamesha
khada rahega, aur request kabhi lautegi hi nahi. Isliye join me time do:

```java
t.join(2000);                     // max 2 sec
if (t.isAlive()) t.interrupt();   // ab bhi atka hai to rukne ko bolo
```

```
join(2000) ke baad: 2000 ms, abhi bhi zinda? true
  tel (godown me atka) wala: rok diya gaya
```

Ye wahi baat hai jo PR drill me hamesha pakadi jaati hai: bahar ki call pe timeout lagao. Asli code me yahi kaam
`future.get(2, SECONDS)` se hota hai (file 11).
