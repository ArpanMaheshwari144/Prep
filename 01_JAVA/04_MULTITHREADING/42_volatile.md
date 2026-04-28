# volatile — Visibility Problem Fix

> **V90 — Multithreading: Topic 42**

---

## 📖 STORY — Background Thread Loop

→ Ek **background thread** tha jo **flag check karke** loop chal raha tha
→ Doosre thread ne **flag false kar diya** — lekin **pehla thread band nahi hua**
→ Ghante baad pata chala
→ **CPU caching ki wajah se** pehle thread ko **nayi value dikhi hi nahi**
→ Apni **purani cache se padh raha tha**
→ **`volatile` lagaya** — ab har thread **seedha main memory se padhta**, cache se nahi

---

## 🧠 Visualization — CPU Cache Problem

```
                   MAIN MEMORY (RAM)
                   running = true
                          ↑
                          │
        ┌─────────────────┴─────────────────┐
        ▼                                   ▼
   Thread 1                             Thread 2
   L1 Cache: running = true             L1 Cache: running = true

  Thread 2 ne main memory mein false kiya:
   MAIN: running = false ✅
   T1 cache: running = true ❌  (purana!)
   T2 cache: running = false

  Thread 1 ko PATA HI NAHI CHALA — loop chalta raha!
```

**Fix — `volatile` lagao** → cache band, **seedha main memory** se padho.

---

## 💻 Code

### ❌ Without volatile — bug
```java
class Worker implements Runnable {
    private boolean running = true;          // ⚠️ cache risk

    public void stop() { running = false; }

    public void run() {
        while (running) { /* kaam */ }       // 🔴 cache se padha — false kabhi nahi dikha
    }
}
```

### ✅ With volatile — fix
```java
class Worker implements Runnable {
    private volatile boolean running = true;  // ✅ cache band, main memory

    public void stop() { running = false; }

    public void run() {
        while (running) { /* kaam */ }        // ✅ har read main memory se
    }
}
```

---

## 🔴 TRAP — `volatile` ≠ Atomic

> **`volatile` SIRF visibility fix karta. ATOMICITY nahi.**
> **`count++` jaisa compound operation `volatile` se SAFE NAHI hota.**

**Why?** `count++` actually **3 steps** hai:
1. **READ** `count` from memory
2. **ADD** 1
3. **WRITE** back to memory

`volatile` guarantee karta READ latest value milegi. Lekin **2 threads dono ne `count = 5` padha**, dono ne `6` likha → ek **increment LOST**.

```java
// 🔴 GALAT — volatile compound operation safe nahi
volatile int count = 0;
count++;                         // race condition

// ✅ FIX 1 — synchronized
synchronized void inc() { count++; }

// ✅ FIX 2 — AtomicInteger
AtomicInteger count = new AtomicInteger(0);
count.incrementAndGet();         // hardware-level atomic
```

---

## 📊 synchronized vs volatile vs AtomicInteger

| | volatile | synchronized | AtomicInteger |
|--|---------|-------------|---------------|
| **Visibility** | ✅ | ✅ | ✅ |
| **Atomicity** | ❌ | ✅ | ✅ |
| **Lock?** | NO | YES | NO (CAS — hardware) |
| **Performance** | Fastest | Slowest | Fast (counters ke liye best) |
| **Use case** | Flag (read-mostly) | Critical sections | Counters/numbers |

---

## 💬 POWER PHRASE

> *"`volatile` ensures every thread reads the latest value from main memory instead of CPU cache — it solves visibility but NOT atomicity. For compound operations like increment, use `synchronized` or `AtomicInteger`."*

> **Yaad rakh:**
> volatile = visibility fix (cache se nahi, RAM se padho)
> volatile ≠ atomic (`count++` race condition rahega)
> Flag → volatile. Counter → AtomicInteger. Critical section → synchronized.
