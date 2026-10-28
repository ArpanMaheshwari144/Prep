# Thread Kya Hai + Banane Ke 2 Tarike

**Kahani:** tera ek API 3 DB call karta hai: user, orders, recommendations. Har call 300ms ki hai. Ek ke baad ek chalein to 900ms lagte hain, aur manager bolta hai "slow hai". Teen thread banake teeno call ek saath bhejo, to 300ms me sab aa jaata hai.

```
Bina thread:  user ──> orders ──> recommendations     = 900ms

Thread ke saath:
              user ┐
            orders ├──> teeno ek saath                = 300ms
   recommendations ┘
```

**Khaas baat:**
1. Thread do tarah se banta hai: `extends Thread` ya `implements Runnable`.
2. Runnable lena behtar hai. Java me class sirf ek hi class extend kar sakti hai, to Thread extend kar diya to aur kuch extend nahi hoga. Runnable lene se class free rehti hai.
3. Aaj kal seedha lambda likhte hain: `new Thread(() -> { ... }).start()`.

**Prod wala trap:** `run()` aur `start()` ka farak.
```java
Thread t1 = new Thread(kaam, "worker-1");
t1.run();     // kaam chala: main      <- naya thread bana hi nahi
t2.start();   // kaam chala: worker-2  <- asli naya thread
```
`run()` likhne pe koi error nahi aata, code chal bhi jaata hai. Par sab kuch main thread pe ek ke baad ek hota hai, to API ab bhi 900ms leta hai. Ye bug code dekh ke nahi pakda jaata, sirf response time se pata chalta hai.

```
Interview: "Thread can be created by extending Thread or implementing Runnable. Runnable is preferred
because Java has single inheritance, so the class stays free to extend something else.
Always call start(), not run() - run() just executes on the current thread, no new thread."
```
