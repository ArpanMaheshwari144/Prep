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

## ★ ANDAR KYA HAI — ThreadPoolExecutor (7 param + task-flow) (8-Sep)

> `Executors.newFixedThreadPool(5)` andar-andar bas ek `new ThreadPoolExecutor(...)` hai. Ye 7 knob pool ka poora behaviour decide karte.

**Restaurant analogy (7 param):**
```
corePoolSize        -> PERMANENT waiter (hamesha payroll pe, kaam ho ya na ho)
maximumPoolSize     -> MAX waiter (permanent + temp mila ke itne se zyada nahi)
keepAliveTime       -> temp-waiter ko kitni der khaali baithne do phir nikaal do
workQueue           -> WAITING BENCH (customer baithe, waiter free ho to serve)
threadFactory       -> waiter "banane" ka tareeka (naam/priority set karna)
rejectedExecutionHandler -> jab bench full + waiter max -> customer ko kya bolo
timeUnit            -> keepAliveTime ki unit (sec/ms)
```

### LIVE MOVIE — task ek-ek aate hain (core=2, bench=3, max=4)
```
T1 aaya  -> W1 free   -> W1 serve         Waiters:[W1:T1][W2: - ]   Bench:[ ][ ][ ]
T2 aaya  -> W2 free   -> W2 serve         Waiters:[W1:T1][W2:T2]   Bench:[ ][ ][ ]   (core BUSY)

T3 aaya  -> core full -> BENCH pe baith   Bench:[T3][  ][  ]
T4 aaya  -> BENCH                          Bench:[T3][T4][  ]
T5 aaya  -> BENCH                          Bench:[T3][T4][T5]   (bench FULL)

T6 aaya  -> bench full -> TEMP W3 hire    Waiters:[W1][W2][W3:T6]          (threads=3)
T7 aaya  -> bench full -> TEMP W4 hire    Waiters:[W1][W2][W3][W4:T7]      (threads=4 = MAX)

T8 aaya  -> bench FULL + waiters MAX -> jagah nahi -> REJECT (rejection-handler chalta)
```

**ORDER (counterintuitive — yaad rakh):** core bharo -> **phir bench(queue)** -> **phir temp-thread(max)** -> phir reject.

### ★ KYUN queue PEHLE, naya thread BAAD me? (asli samajh)
```
Bench pe bithana   -> bas list me ek reference daala. lagbhag FREE.
Naya thread banana -> OS se thread maango -> ~1 MB stack + context-switch. MEHNGA.
```
Isiliye pool ki soch: **"pehle SASTI jagah (queue) bharo, kaam thoda wait karega chalega; jab sasti jagah bhi khatam -> TAB majboori me MEHNGA kaam (naya thread)."**
Ek line: **thread banana = LAST RESORT. Jitna queue se chal jaaye chalao; thread tabhi jab queue bhi bhar jaaye.**

### ★ OOM TRAP — bench INFINITE (unbounded queue)
```
newFixedThreadPool ki queue = UNBOUNDED (LinkedBlockingQueue, no limit).
T3,T4,...T10000 -> sab bench pe baithte jaate (bench kabhi FULL nahi hoti)
   -> temp-thread KABHI hire nahi hote (bench full hui hi nahi)
   -> sirf core threads kaam karein, bench pe laakhon ka dher -> MEMORY phatt = OOM.
   -> aur maximumPoolSize bekaar pada raha (kabhi use hi nahi hua).
```
Isiliye production me apna `ThreadPoolExecutor` **bounded queue** ke saath banao — bench limit ho to jaldi pata chale + reject/backpressure milega, chup-chaap OOM nahi.

### Rejection policies (bench full + max ke baad kya)
| Policy | Kya karta |
|--------|-----------|
| **AbortPolicy** (default) | `RejectedExecutionException` phenk deta |
| **CallerRunsPolicy** | jo submit kar raha WAHI thread task chala de (natural slow-down/backpressure) |
| **DiscardPolicy** | task chup-chaap phenk de (koi error nahi) |
| **DiscardOldestPolicy** | queue ka sabse purana task hata ke naya daal de |

### Sizing (kitne thread)
```
CPU-bound kaam (calc/loop)  -> ~ cores jitne (zyada thread = sirf context-switch waste)
IO-bound kaam (DB/API wait) -> cores se ZYADA (thread waise bhi wait me, doosra kaam kar le)
```

---

## POWER PHRASE

> *"`ExecutorService` manages a pool of reusable threads — avoiding the cost of creating new threads for every task. Use `FixedThreadPool` for predictable load, `submit()` to get a `Future`, and always call `shutdown()` to release resources."*

> *"Internally it's a `ThreadPoolExecutor`: tasks fill the core threads first, then the queue, and only then spawn threads up to maximumPoolSize — because creating a thread is expensive, so the queue is used first. If the queue is unbounded (like `newFixedThreadPool`), the max size never kicks in and the queue can grow until OOM — so in production I use a bounded queue with a rejection policy."*

> **Yaad rakh:**
> `new Thread()` = expensive, manual lifecycle
> `ExecutorService` = pool, reuse, future, shutdown
> Production = FixedThreadPool (cached = OOM risk)
> shutdown() finally mein — leaked threads alive rehte
