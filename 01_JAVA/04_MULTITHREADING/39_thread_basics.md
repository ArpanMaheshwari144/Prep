# Thread Kya Hai + Banane Ke 2 Tarike

> **V90 — Multithreading: Topic 39**

---

## STORY — API Slow Tha

→ API ke 3 DB calls the — **user fetch, orders fetch, recommendations fetch**
→ Ek ke baad ek — **300ms + 300ms + 300ms = 900ms** response time
→ Manager ne bola **"too slow."** Teen threads banaye, **teeno parallel chale**
→ **300ms total**
→ Multithreading ke bina modern backend slow hai
→ Lekin **shared state pe careful** rehna padta — **race condition aur deadlock** ka darr hamesha rehta

---

## WHY — Thread Kya Hai?

→ Restaurant mein **ek waiter = ek kaam ek time pe**
→ **Teen waiters = teeno alag customers ek saath**
→ **Thread = alag-alag kaam parallel mein**

```
// Without Thread:
Task1 → Task2 → Task3   (ek ke baad ek — sequential)

// With Thread:
Task1 ┐
Task2 ├→ sab ek saath (parallel)
Task3 ┘
```

---

## Thread Banane Ke 2 Tarike

### Tarika 1 — `Thread` extend karo
```java
class MyThread extends Thread {
    public void run() {
        System.out.println("Thread chal raha hai!");
    }
}

MyThread t = new MyThread();
t.start();    // ← start() likho, run() NAHI!
```

### Tarika 2 — `Runnable` implement karo (PREFER karo)
```java
class MyTask implements Runnable {
    public void run() {
        System.out.println("Task chal raha hai!");
    }
}

Thread t = new Thread(new MyTask());
t.start();
```

---

## TRAP 1 — `t.run()` vs `t.start()`

> **`t.run()` likha toh NAYA thread NAHI banega** — normal method call hoga **same thread mein**.
> **HAMESHA `t.start()` likho** — JVM naya thread create karta.

```java
t.run();      // same thread — sequential, no parallel
t.start();    // naya thread — parallel
```

---

## TRAP 2 — Runnable Prefer Karo

> **`Thread` extend kiya toh koi aur class extend nahi kar sakte (Java single inheritance).**
> `Runnable` implement kiya toh class **free** rehti — kuch aur bhi extend kar sakte.

```java
class MyTask extends Thread { }              // ab kuch aur extend nahi
class MyTask extends BaseClass implements Runnable { }   // flexibility
```

---

## POWER PHRASE

> *"Thread can be created by extending Thread class or implementing Runnable. Runnable is preferred because Java doesn't support multiple inheritance — implementing Runnable keeps the class free to extend others."*

> **Yaad rakh:**
> Thread extend = inheritance use, lekin koi aur extend nahi
> Runnable implement = flexibility
> `t.start()` likho, `t.run()` NAHI
> Modern: `new Thread(() -> { ... }).start()` lambda se 1-line
