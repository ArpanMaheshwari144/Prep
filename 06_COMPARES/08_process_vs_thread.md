# Process vs Thread — alag ghar ya ek ghar ke kamre

---

## Koi poochhe to aise bolo

**Process** ek chalta hua program hai jiski apni alag memory hai (Chrome, IntelliJ, tera Spring Boot app = ek JVM). Ek process
gire to doosra bacha rehta hai, kyunki memory alag hai. Par process banana mehenga, aur do process aapas me baat karein to pipe /
socket / network chahiye. **Thread** process ke andar kaam karne wala hai; ek process me kai thread, sab ek hi memory baant ke.
Banana sasta, baat turant (ek thread ka likha doosra seedha padh leta). Keemat: do thread ek hi cheez ek saath badlein to race
condition, isliye lock chahiye; aur ek thread poora process gira sakta hai.

---

## Kahani: alag ghar vs ek ghar ke kamre

```
Process = alag-alag ghar: har ghar ka apna fridge; ek ghar me aag lagi to padosi ka ghar bacha
Thread  = ek ghar ke kamre: fridge saajha, ek ne doodh nikala to sabko turant pata;
          par do log ek saath aakhri doodh uthayein to ladai (race), isliye fridge pe taala (lock)
```

---

## Tere project se

- **Spring Boot app** = ek process (JVM). Andar Tomcat ke thread (default 200), har request ek thread sambhalta; saath me GC ke
  thread aur `@Async` pool ke thread.
- **MT 04 race** = PR drill wala double payout: do thread ne ek hi balance padha, dono ne paisa bhej diya.
- **Microservices** = har service alag process, isliye baat REST / Kafka se (Compares 04).

---

## Dikkat se hal

`count++` do thread ne saath chalaya: dono ne 5 padha, dono ne 6 likha, ek ginti kho gayi. Hal: `AtomicInteger` / `synchronized`
/ DB me atomic update. Kai server ho to Redis lock ya DB ki row lock, kyunki tab thread ek process me nahi, alag machine pe hain.

---

## Trap

Ek thread me `OutOfMemoryError` jaisa error aaya to wo sirf us thread ko nahi, poore JVM ko gira sakta hai, kyunki heap saajha
hai; saari requests ek saath gayin. Isliye bhaari kaam (badi file, video) alag service / alag process me rakhte hain.
(Connection pool me DB connection hote hain, thread nahi.)
