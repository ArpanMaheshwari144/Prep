# ExecutorService + Thread Pool — Visual Revision

---

## 1 Problem (Why Thread Pool?)

```
Without thread pool:
   Har task ke liye NEW thread

   Task 1 → new Thread()    (expensive)
   Task 2 → new Thread()    (expensive)
   Task 3 → new Thread()
   ...
   1000 tasks → 1000 threads
   = JVM crash (out of memory)
   = OS overhead massive
```

```
With thread pool:
   Pre-create N threads (workers)
   Tasks queue up
   Worker free? → grab task → execute

   = Reuse threads
   = Bounded resource usage
```

---

## 2 Office Analogy

```
WITHOUT POOL:
   Customer arrives → COMPANY HIRES new employee
   Employee handles → FIRED after task
   Next customer → HIRE again

   = Insane HR cost!
```

```
WITH POOL:
   Office mein 10 employees fixed
   Customer arrives → queue lagi
   Employee free? → grab customer → serve
   Done? → wait for next

   = Sustainable
```

```
Thread Pool = office with fixed employees
Workers     = pre-created threads
Tasks       = customers in queue
```

---

## 3 ExecutorService Basics

```java
// Create pool of 5 threads
ExecutorService executor = Executors.newFixedThreadPool(5);

// Submit tasks
executor.submit(() -> System.out.println("Task 1"));
executor.submit(() -> System.out.println("Task 2"));
executor.submit(() -> System.out.println("Task 3"));

// Shutdown
executor.shutdown();
```

```
Magic:
   5 threads share unlimited tasks
   Queue keeps tasks waiting
   Auto-distribute to free workers
```

---

## 4 Pool Types

```
┌──────────────────────────┬───────────────────────────┐
│  Type                    │  Behavior                  │
├──────────────────────────┼───────────────────────────┤
│ newFixedThreadPool(N)    │ N fixed threads            │
│                          │ Tasks queue if all busy    │
├──────────────────────────┼───────────────────────────┤
│ newCachedThreadPool()    │ Threads created as needed  │
│                          │ Idle threads die in 60s    │
│                          │ unlimited growth!        │
├──────────────────────────┼───────────────────────────┤
│ newSingleThreadExecutor()│ 1 thread, sequential       │
├──────────────────────────┼───────────────────────────┤
│ newScheduledThreadPool() │ Schedule tasks (cron-like) │
└──────────────────────────┴───────────────────────────┘
```

---

## 5 Visual — Fixed Pool

```
                  TASK QUEUE
   ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐
   │6│ │7│ │8│ │9│ │X│ │X│  ← waiting tasks
   └─┘ └─┘ └─┘ └─┘ └─┘ └─┘
            │
            ▼
   ┌──────────────────────────┐
   │   POOL (5 workers)       │
   │                            │
   │   T1 working on task 1     │
   │   T2 working on task 2     │
   │   T3 working on task 3     │
   │   T4 working on task 4     │
   │   T5 working on task 5     │
   └──────────────────────────┘
            │
            ▼
       Task done?
       Pull next from queue
```

---

## 6 submit vs execute

```java
// execute() — Runnable, no return
executor.execute(() -> doSomething());

// submit() — Runnable OR Callable
//   Returns Future<T> for tracking
Future<Integer> future = executor.submit(() -> {
    return computeSomething();   // Callable returns value
});

Integer result = future.get();   // blocks till done
```

```
┌─────────────┬─────────────┬──────────────┐
│  Method     │ Returns     │ Use          │
├─────────────┼─────────────┼──────────────┤
│ execute()   │ void        │ Fire & forget│
│ submit()    │ Future<T>   │ Need result  │
└─────────────┴─────────────┴──────────────┘
```

---

## 7 Future Basics

```java
Future<Integer> future = executor.submit(() -> {
    Thread.sleep(2000);
    return 42;
});

// Non-blocking check:
future.isDone();              // false (still running)

// Blocking get:
Integer result = future.get();    // wait + return 42

// Get with timeout:
Integer r = future.get(1, SECONDS);   // throws if not done

// Cancel:
future.cancel(true);
```

---

## 8 Real Example — Parallel Processing

```java
ExecutorService pool = Executors.newFixedThreadPool(5);
List<String> urls = List.of("url1", "url2", ..., "url10");

// Submit all
List<Future<String>> results = new ArrayList<>();
for (String url : urls) {
    results.add(pool.submit(() -> fetch(url)));
}

// Collect when done
for (Future<String> f : results) {
    System.out.println(f.get());
}

pool.shutdown();
```

```
10 URLs fetched in parallel
   With 5 threads → 2 round (5 + 5) vs 10 ek-ek karke = ~5x faster
```

---

## 9 Shutdown — IMPORTANT!

```java
// Graceful: finish ongoing, no new tasks
executor.shutdown();

// Wait for completion
executor.awaitTermination(10, SECONDS);

// Forceful: cancel ongoing
executor.shutdownNow();
```

```
WITHOUT shutdown:
   JVM doesn't exit
   Threads keep running indefinitely
   = Resource leak
```

---

## Best Practice Pattern

```java
ExecutorService pool = Executors.newFixedThreadPool(5);

try {
    // submit tasks
    for (Task t : tasks) {
        pool.submit(() -> t.run());
    }
} finally {
    pool.shutdown();
    try {
        if (!pool.awaitTermination(10, SECONDS)) {
            pool.shutdownNow();   // force shutdown
        }
    } catch (InterruptedException e) {
        pool.shutdownNow();
        Thread.currentThread().interrupt();
    }
}
```

---

## 11 Pool Size — How Many Threads?

```
CPU-BOUND tasks (computation):
   threads = number of CPU cores
   Reason: more = context switching overhead

I/O-BOUND tasks (network, DB):
   threads = CPU cores × 2 (or more)
   Reason: threads often blocked waiting

Spring Boot default (embedded Tomcat ke request threads):
   server.tomcat.threads.max = 200 (high — for I/O heavy)
```

---

## 12 When To Use Custom ThreadPoolExecutor?

```java
ThreadPoolExecutor pool = new ThreadPoolExecutor(
    5,                          // core threads
    10,                         // max threads
    60L, TimeUnit.SECONDS,      // keep-alive
    new ArrayBlockingQueue<>(100),  // queue size
    new ThreadPoolExecutor.AbortPolicy()  // rejection
);

// Custom control:
//   - core vs max threads
//   - queue size
//   - rejection policy when full
```

```
Rejection policies:
   AbortPolicy        → throw RejectedExecutionException
   CallerRunsPolicy   → caller thread runs it
   DiscardPolicy      → silently drop
   DiscardOldestPolicy → drop oldest in queue
```

---

## Memory Hooks

```
ExecutorService = office with fixed employees
Thread pool     = reuse threads (vs create new)

Types:
   Fixed     = N threads always
   Cached    = grow as needed (DANGEROUS — unlimited)
   Single    = 1 thread (sequential)
   Scheduled = cron-like timing

submit/execute:
   execute()  → void, fire-forget
   submit()   → Future, can get result

Future = handle to async work
   .get()      blocks
   .cancel()   stop

ALWAYS shutdown() in finally
```

[← Back to JAVA](../../)
