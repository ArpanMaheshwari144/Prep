# Deadlock: Gol Chakkar Ka Intezaar

## Scene: chaabi ek ke paas, register doosre ke paas

Mart me do cashier hain. Pehle ke haath me galle ki chaabi hai, aur use register chahiye. Doosre ke haath me register hai, aur
use galle ki chaabi chahiye. Dono kehte hain "pehle tu de", aur koi apni cheez nahi chhodta. Dukaan khuli hai par billing ruk
gayi hai, na koi chillaya, na koi error aaya.

Banking me yahi hota hai: Ravi Priya ko 100 bhejta hai, aur usi pal Priya Ravi ko 50. Pehla thread Ravi ka account lock karke
Priya ka maangta hai, doosra Priya ka lock karke Ravi ka. Chala ke dekha (dono alag JVM me):

```
jo bhej raha pehle:  DEADLOCK, 2 thread phase, koi error nahi
id ke kram se lock:  dono transfer ho gaye
```

## Hal: sab ek hi kram me lock lein

Deadlock ke liye 4 shart ek saath chahiye: ek cheez ek hi ke paas, ek pakad ke doosri ka intezaar, koi zabardasti chheen nahi
sakta, aur intezaar gol chakkar me. Inme se koi ek bhi tod do to deadlock nahi banega. Sabse aasaan ye hai ki sab hamesha ek
hi kram me lock lein, jaise chhoti id wala account pehle, chahe bhej koi bhi raha ho. Tab gol chakkar ban hi nahi sakta.

"Bhejne wala pehle, lene wala baad me" sahi lagta hai, par ulti disha ke do transfer ek doosre ko phasa dete hain (upar ki
pehli line). Isliye kram kisi pakki cheez se tay karo:

```java
Account first  = from.id < to.id ? from : to;   // chhoti id pehle, hamesha
Account second = from.id < to.id ? to : from;
synchronized (first) { synchronized (second) { /* transfer */ } }
```

Doosra hal file 04 ka `tryLock(timeout)` hai: time pe lock na mile to thread chhod deta hai, aur "pakad ke intezaar" wali shart
toot jaati hai.

Deadlock exception nahi deta, app bas chup ho jaata hai. Isliye ise thread dump se pakadte hain, `jstack` seedha "Found one
Java-level deadlock" bolta hai ([DeadlockDemo](practical/DeadlockDemo.java)). DB me bhi yahi hota hai, par wahan DB khud
deadlock pakad ke ek transaction ko rollback kar deta hai (09_DATABASE/08).
