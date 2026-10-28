# ExecutorService / Thread Pool

**Kahani:** restaurant me har naye customer ke liye ek naya waiter bharti karo, aur khaana khatam hote hi use nikaal do. Ye mehnga bhi hai aur bekaar bhi. Bheed aayi to waiter itne ho jaayenge ki restaurant me jagah hi nahi bachegi (`OutOfMemoryError: unable to create native thread`). Thread pool ka matlab hai: kuch pakke waiter rakho, customer bench pe baithe, aur jo waiter khaali ho wo agle customer ko serve kare. Waiter wahi rehte hain, bas kaam badalta hai.

Movie: core = 2 pakke waiter, bench = 3, max = 4 waiter (`new ThreadPoolExecutor(2, 4, 30, SECONDS, new ArrayBlockingQueue<>(3))`):
```
T1 aaya -> threads=1, bench=0
T2 aaya -> threads=2, bench=0      <- pakke waiter bhar gaye
T3 aaya -> threads=2, bench=1      <- naya waiter nahi, BENCH pehle
T4 aaya -> threads=2, bench=2
T5 aaya -> threads=2, bench=3      <- bench bhar gaya
T6 aaya -> threads=3, bench=3      <- ab temp waiter
T7 aaya -> threads=4, bench=3      <- max
T8 aaya -> REJECT
```

**Khaas baat:**
1. Kram ulta lagta hai: pehle pakke waiter, phir bench, phir temp waiter, phir reject. Wajah ye hai ki bench pe kisi ko bithana lagbhag muft hai, jabki naya thread banana mehnga hai (~1 MB stack + OS ka kaam).
2. `newFixedThreadPool` ki bench ki koi had nahi hoti. Bench kabhi bharti hi nahi, to temp waiter kabhi aata hi nahi, aur laakhon task ka dher memory kha jaata hai, OOM. `newCachedThreadPool` me ulta hai: waiter ki koi had nahi, to threads ki ginti se OOM. Production me apna `ThreadPoolExecutor` banao jisme bench ki had ho aur reject ki policy tay ho. `CallerRunsPolicy` me jo task bhej raha hai wahi khud chala leta hai, isse bhejne wala apne aap dheema ho jaata hai (backpressure).
3. Kitne thread rakhein:
   - CPU wala kaam (calculation, loop): jitne core, utne thread.
   - DB / API ka intezaar wala kaam: core se zyada.
4. `shutdown()` bhool gaye to pool ke thread zinda rehte hain aur JVM band nahi hota. Isliye `finally` me likho.

**Prod wala trap:**
```java
pool.submit(() -> { throw new RuntimeException("payment fail"); });
// get() kabhi nahi bulaya -> log me KUCH nahi aaya
```
`submit()` exception ko `Future` ke andar band kar deta hai. Agar `get()` nahi bulaya, to payment fail ho gaya aur log me ek line bhi nahi aayi. Ye catch me exception nigalne jaisa hi hai, jo tu PR drill me pakad leta hai. Bas yahan wo chhupa hua hai. Result nahi chahiye to `execute()` lagao, wo exception ko stack trace ke saath print karta hai.

**Tera project:** usercrud [`DemoJobs`](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/DemoJobs.java) me `@Async sendEmail` ke peeche yahi pool hai. Log me thread ka naam `task-1` aata hai, http thread nahi. Wahan bhi bench ki had rakhna zaroori hai.

```
Interview: "ExecutorService reuses a pool of threads instead of creating one per task. Internally it's
ThreadPoolExecutor: core threads first, then the queue, then extra threads up to max, then
reject - because queueing is cheap and threads are expensive. newFixedThreadPool has an
unbounded queue, so in production I use a bounded queue with a rejection policy like
CallerRunsPolicy. And submit() hides exceptions inside the Future unless you call get()."
```


---

## GEHRA HISSA (purani file se, interview me kaam ka)

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
// pool.shutdownNow();                  // running ko interrupt (best effort) + pending list lautata
```

---

## Pool Types Comparison

| Pool | Threads | Use case |
|------|---------|----------|
| **FixedThreadPool(N)** | N constant | Predictable load (par queue unbounded — upar khaas baat 2, OOM) |
| **CachedThreadPool** | Demand pe (unbounded) | Short, async tasks — danger of OOM under load |
| **SingleThreadExecutor** | 1 | Sequential FIFO — order important |
| **ScheduledThreadPool** | N | Cron jobs, delayed tasks |

## TRAP 3 — `submit()` vs `execute()`

> **`execute(Runnable)`** = no return. Exception aaya -> thread ke UncaughtExceptionHandler tak jaata -> **stack trace print hota** (thread marta, pool naya bana leta)
> **`submit(Runnable/Callable)`** = `Future` deta. Exception **Future ke andar band** -> `get()` pe `ExecutionException` me milta
> **ASLI TRAP:** `submit()` kiya aur `get()` kabhi nahi bulaya -> exception **chup-chaap gayab**, log me kuch nahi

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
  hazaaron reqs  →  utne threads create + destroy  →  native-thread OOM / context-switch me CPU barbaad


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
  pool.shutdownNow()          ──►  running tasks ko interrupt() (best effort, ignore kare to chalta rahega) + queue ke pending tasks ki list lautata
  pool.awaitTermination(...)  ──►  wait jab tak sab khatam
```

### ★ KYUN queue PEHLE, naya thread BAAD me? (asli samajh)
```
Bench pe bithana   -> bas list me ek reference daala. lagbhag FREE.
Naya thread banana -> OS se thread maango -> ~1 MB stack + context-switch. MEHNGA.
```

### ★ OOM TRAP — bench INFINITE (unbounded queue)
```
newFixedThreadPool ki queue = UNBOUNDED (LinkedBlockingQueue, no limit).
T3,T4,...T10000 -> sab bench pe baithte jaate (bench kabhi FULL nahi hoti)
   -> temp-thread KABHI hire nahi hote (bench full hui hi nahi)
   -> sirf core threads kaam karein, bench pe laakhon ka dher -> MEMORY phatt = OOM.
   -> aur maximumPoolSize bekaar pada raha (kabhi use hi nahi hua).
```
