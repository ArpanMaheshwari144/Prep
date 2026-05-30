# ExecutorService / Thread Pool

> **Multithreading: Topic 46 — production-critical (V90 ke bahar but important)**

---

## STORY — `new Thread()` Ki Problem

→ API ko **1000 requests** aaye. Tune **har request pe `new Thread()`** banaya
→ **JVM crash** — thread create karna **costly hai**, OS-level resources lagti hain
→ Memory leak. **Threads garbage collect nahi hote** kabhi-kabhi
→ Modern fix: **Thread Pool** — pehle se kuch threads bana lo, **reuse karo**
→ `ExecutorService` = thread pool ka manager

---

## WHY — ExecutorService?

→ **`new Thread()`** har baar — **expensive create + destroy**
→ Pool = **fixed threads** banake rakho, kaam aate hi assign
→ Kaam khatam → **thread reuse** ho jaata, naya nahi banta
→ **Throughput up, memory predictable**

---

## Visualization — Pool with Reusable Threads

```
              ExecutorService — Thread Pool Visualization

╔════════════════════════════════════════════════════════════╗
║ Without Pool — har request pe new Thread                   ║
╚════════════════════════════════════════════════════════════╝

  Request 1  →  new Thread()  →  task done  →  thread DESTROY
  Request 2  →  new Thread()  →  task done  →  thread DESTROY
  Request 3  →  new Thread()  →  task done  →  thread DESTROY
   ...
  1000 reqs  →  1000 threads create + destroy  →  JVM crash


╔════════════════════════════════════════════════════════════╗
║ With Pool — 5 threads, REUSE                               ║
╚════════════════════════════════════════════════════════════╝

  ExecutorService pool = Executors.newFixedThreadPool(5);

       Pool (heap)
       ┌─────────────────────────────────────────┐
       │  ┌────┐ ┌────┐ ┌────┐ ┌────┐ ┌────┐    │
       │  │ T1 │ │ T2 │ │ T3 │ │ T4 │ │ T5 │    │   ← 5 reusable threads
       │  └────┘ └────┘ └────┘ └────┘ └────┘    │
       └─────────────────────────────────────────┘
                          ▲
                          │ submit()
                          │
       Task Queue
       ┌─────────────────────────────────────────┐
       │  task1  task2  task3  ... task1000      │
       └─────────────────────────────────────────┘

  Task aaya → free thread mile → assign → done → thread WAPAS pool mein
                                                    (DESTROY nahi, REUSE)


╔════════════════════════════════════════════════════════════╗
║ Pool Types                                                 ║
╚════════════════════════════════════════════════════════════╝

  FixedThreadPool(N)     ┌─────────┐
                         │ N fixed │  ← predictable load (production)
                         └─────────┘

  CachedThreadPool       ┌─────────────────┐
                         │ unbounded grows │  ← OOM risk under load!
                         └─────────────────┘

  SingleThreadExecutor   ┌─────┐
                         │  1  │  ← FIFO sequential
                         └─────┘

  ScheduledThreadPool    ┌─────────┐
                         │ N + cron │  ← delayed/periodic tasks
                         └─────────┘


╔════════════════════════════════════════════════════════════╗
║ Lifecycle                                                  ║
╚════════════════════════════════════════════════════════════╝

  pool.submit(task)           ──►  task chala
  pool.shutdown()             ──►  naya kaam nahi loga, current finish karega
  pool.shutdownNow()          ──►  force kill — running tasks bhi rok dega
  pool.awaitTermination(...)  ──►  wait jab tak sab khatam
```

---

## Code — 4 Common Pools

```java
import java.util.concurrent.*;

// 1. Fixed Pool — fixed N threads
ExecutorService pool = Executors.newFixedThreadPool(5);     // 5 threads

// 2. Cached Pool — demand pe banao, idle 60s baad destroy
ExecutorService cached = Executors.newCachedThreadPool();

// 3. Single Thread — sirf 1 thread (sequential FIFO)
ExecutorService single = Executors.newSingleThreadExecutor();

// 4. Scheduled — delay/periodic execution
ScheduledExecutorService sched = Executors.newScheduledThreadPool(3);
```

### Submit Task

```java
ExecutorService pool = Executors.newFixedThreadPool(3);

// Runnable — return nahi
pool.submit(() -> System.out.println("Task chala by " + Thread.currentThread().getName()));

// Callable — return value
Future<Integer> future = pool.submit(() -> {
    Thread.sleep(1000);
    return 42;
});

System.out.println(future.get());      // 42 (block jab tak result na aaye)

pool.shutdown();                        // graceful shutdown — naya kaam nahi loga
// pool.shutdownNow();                  // force kill — running tasks bhi rok dega
```

---

## Pool Types Comparison

| Pool | Threads | Use case |
|------|---------|----------|
| **FixedThreadPool(N)** | N constant | Predictable load — 95% production |
| **CachedThreadPool** | Demand pe (unbounded) | Short, async tasks — danger of OOM under load |
| **SingleThreadExecutor** | 1 | Sequential FIFO — order important |
| **ScheduledThreadPool** | N | Cron jobs, delayed tasks |

---

## TRAP 1 — `Executors.newCachedThreadPool()` Production Mein Risky

> **Cached pool **unlimited** threads bana sakta** under heavy load → **OOM crash**.
> **Production mein FixedThreadPool ya custom `ThreadPoolExecutor` use karo.**

## TRAP 2 — `shutdown()` Bhulna

> **Shutdown nahi kiya = JVM exit nahi karega** — pool ke threads alive rehte.
> **HAMESHA `shutdown()` call karo** finally block mein.

```java
try {
    // tasks submit karo
} finally {
    pool.shutdown();
}
```

## TRAP 3 — `submit()` vs `execute()`

> **`execute(Runnable)`** = no return, exception **silently swallow** ho sakta hai
> **`submit(Runnable/Callable)`** = `Future` deta, exception `Future.get()` se milta

---

## POWER PHRASE

> *"`ExecutorService` manages a pool of reusable threads — avoiding the cost of creating new threads for every task. Use `FixedThreadPool` for predictable load, `submit()` to get a `Future`, and always call `shutdown()` to release resources."*

> **Yaad rakh:**
> `new Thread()` = expensive, manual lifecycle
> `ExecutorService` = pool, reuse, future, shutdown
> Production = FixedThreadPool (cached = OOM risk)
> shutdown() finally mein — leaked threads alive rehte
