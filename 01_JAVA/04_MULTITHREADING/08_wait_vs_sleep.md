# wait() vs sleep()

## Scene: galle ki chaabi leke jhapki, ya chaabi rakh ke intezaar

Ek cashier galle ki chaabi leke baitha hai, aur doosre cashier ko bhi galla chahiye. `sleep()` me pehla cashier chaabi haath
me pakde-pakde 2 minute ki jhapki le leta hai, aur doosra khada intezaar karta rehta hai. `wait()` me pehle cashier ko chhutte
chahiye the, to wo chaabi rakh ke bolta hai "tum karo, chhutte aa jaayein to mujhe bula lena", aur doosra turant galla khol
leta hai. Chala ke dekha:

```
sleep(2000) galle pe baith ke:   doosre cashier ko galla mila: 2003 ms baad
wait() galla chhod ke:           doosre cashier ko galla mila:   52 ms baad
```

Bas yahi farak hai: `sleep` chaabi (lock) pakde rehta hai, aur `wait` chaabi chhod deta hai jab tak koi `notify()` na kare.
Isiliye `wait()` sirf `synchronized` ke andar chalta hai, kyunki chaabi chhodne ke liye pehle wo tere paas honi chahiye (warna
`IllegalMonitorStateException`). Aur `wait()` ko hamesha `while (shart) wait();` me likho, kyunki thread kabhi bina bulaye bhi
uth jaata hai (spurious wakeup), to uthne ke baad shart dobara dekhni padti hai.

Bas thodi der rukna ho (polling, retry ka gap) to `sleep()`. Ek thread ko doosre ke signal ka intezaar ho (producer-consumer,
file 12) to `wait()`.

## Prod wala trap

```java
@Transactional
public void pay() {
    for (int i = 0; i < 3; i++) {
        if (psp.call()) break;
        Thread.sleep(2000);   // DB connection + row lock 6 sec tak pakde baitha
    }
}
```

Ye wahi galle pe jhapki hai, bas yahan chaabi DB connection aur row lock hai. Har request 6 second tak connection pakde baithi
hai, aur thodi bheed aate hi Hikari pool khaali ho jaata hai. Isliye bahar ki call transaction ke bahar rakho, jaisa PR drill
me hota hai. Aur `catch (InterruptedException e) {}` khaali mat chhodo, usme `Thread.currentThread().interrupt();` likho, warna
rukne ka signal kho jaata hai.
