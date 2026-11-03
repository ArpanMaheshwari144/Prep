# Thread Kya Hai + Banane Ke 2 Tarike

**Kahani:** maan le tera ek API 3 DB call karta hai: user, orders aur recommendations. Har call 300ms ki hai. Agar ye ek ke baad ek chalein to kul 900ms lagte hain, aur manager bolta hai ki "slow hai". Agar tu teen thread banake teeno call ek saath bhej de, to 300ms me sab kuch aa jaata hai.

```
Bina thread:  user ──> orders ──> recommendations     = 900ms

Thread ke saath:
              user ┐
            orders ├──> teeno ek saath                = 300ms
   recommendations ┘
```

**Khaas baat:**

1. Thread do tarah se banta hai: ya to `extends Thread` karo, ya `implements Runnable` karo.

2. Runnable lena behtar hai. Java me ek class sirf ek hi class ko extend kar sakti hai, to agar tune Thread extend kar diya to wo class aur kuch extend nahi kar paayegi. Runnable lene se class free rehti hai.

3. Aaj kal log seedha lambda likh dete hain: `new Thread(() -> { ... }).start()`.

**Prod wala trap:** ye `run()` aur `start()` ka farak hai.

```java
Thread t1 = new Thread(kaam, "worker-1");
t1.run();     // kaam chala: main      <- naya thread bana hi nahi
Thread t2 = new Thread(kaam, "worker-2");
t2.start();   // kaam chala: worker-2  <- asli naya thread
```

`run()` likhne pe koi error nahi aata, aur code chal bhi jaata hai. Par sab kuch main thread pe hi ek ke baad ek hota hai, isliye API ab bhi 900ms leta hai. Ye bug code dekh ke nahi pakda jaata, sirf response time dekh ke pata chalta hai.

```
Interview: "Thread can be created by extending Thread or implementing Runnable. Runnable is preferred
because Java has single inheritance, so the class stays free to extend something else.
Always call start(), not run() - run() just executes on the current thread, no new thread."
```


---

## GEHRA HISSA (purani file se, interview me kaam ka)

## Thread Banane Ke 2 Tarike

### Tarika 1: `Thread` ko extend karo

Isme tu apni class ko seedha Thread ka bachcha bana deta hai aur `run()` me apna kaam likhta hai. Chalane ke liye `start()` bulana hai, `run()` nahi.

```java
class MyThread extends Thread {
    public void run() {
        System.out.println("Thread chal raha hai!");
    }
}

MyThread t = new MyThread();
t.start();    // ← start() likho, run() NAHI!
```

### Tarika 2: `Runnable` implement karo (yahi prefer karo)

Isme tera kaam ek alag task class me hota hai, aur wo task tu Thread ko de deta hai. Teri class kisi aur class ko extend karne ke liye free rehti hai.

```java
class MyTask implements Runnable {
    public void run() {
        System.out.println("Task chal raha hai!");
    }
}

Thread t = new Thread(new MyTask());
t.start();
```

## Thread kyun chahiye

Bina thread ke saare task ek ke baad ek chalte hain, yaani pehla khatam hoga tabhi doosra shuru hoga. Thread ke saath teeno task ek saath chal sakte hain.

```
// Without Thread:
Task1 → Task2 → Task3   (ek ke baad ek, sequential)

// With Thread:
Task1 ┐
Task2 ├→ sab ek saath (parallel)
Task3 ┘
```
