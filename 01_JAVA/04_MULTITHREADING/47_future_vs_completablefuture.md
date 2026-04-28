# Future vs CompletableFuture

> **Multithreading: Topic 47 — Java 8 async (V90 ke bahar but production-critical)**

---

## 📖 STORY — Async Result Chahiye

→ `executor.submit(callable)` ne **`Future`** return kiya
→ `future.get()` call kiya — **block ho gaya** jab tak result na aaye. **Blocking call** — async ka faayda gone
→ Multiple async results ek saath chain karne mushkil
→ **Java 8 ne `CompletableFuture` diya** — non-blocking, chainable, callbacks support

---

## 🆚 Future vs CompletableFuture

| | `Future` (Java 5) | `CompletableFuture` (Java 8) |
|--|------------------|-------------------------------|
| **Blocking?** | YES (`get()` blocks) | NO — callbacks |
| **Chain operations?** | NO | YES (`thenApply`, `thenCompose`) |
| **Combine multiple?** | NO | YES (`thenCombine`, `allOf`) |
| **Exception handling?** | `get()` throws | `exceptionally()`, `handle()` |
| **Manually complete?** | NO | YES (`complete()`) |

---

## 🧠 Visualization — Chain Operations

```
              Future vs CompletableFuture — Chain Diagram

╔════════════════════════════════════════════════════════════╗
║ Future — Blocking (Java 5)                                 ║
╚════════════════════════════════════════════════════════════╝

   Main Thread                          Worker Thread
       │                                     │
       ├─── pool.submit(task) ──────────────►│ task start
       │                                     │   ...
       │  (other work...)                    │
       │                                     │
       │                                     │
       ├─── future.get() ───────────────────►│
       │                                     │
       │     🔴 BLOCKED                      │
       │     (wait kar raha)                 │
       │                                     │   ...
       │                                     │ task done
       │◄────── return 42 ───────────────────┤
       │                                     │
       │  result = 42                        │

   "Async ka faayda gone — main thread ruka hua"


╔════════════════════════════════════════════════════════════╗
║ CompletableFuture — Non-Blocking + Chain (Java 8)          ║
╚════════════════════════════════════════════════════════════╝

  Main Thread continues, worker handles task

   ┌──────────────────┐
   │ supplyAsync()    │  → Worker thread mein chala
   │ fetchUser(101)   │
   └────────┬─────────┘
            │
            ▼ result aaya
   ┌──────────────────┐
   │ thenApply()      │  → User → name (transform)
   │ user → user.name │
   └────────┬─────────┘
            │
            ▼ name aaya
   ┌──────────────────┐
   │ thenAccept()     │  → name pe action (no return)
   │ name → print     │
   └──────────────────┘

   Pipeline! Main thread block nahi hua, callbacks chain mein


╔════════════════════════════════════════════════════════════╗
║ Combine Multiple Futures — Parallel                        ║
╚════════════════════════════════════════════════════════════╝

   userF (300ms)         orderF (300ms)         settingsF (300ms)
       │                      │                       │
       │ parallel             │ parallel              │ parallel
       ▼                      ▼                       ▼
   ┌─────────┐            ┌─────────┐             ┌─────────┐
   │ User    │            │ Order   │             │Settings │
   └────┬────┘            └────┬────┘             └────┬────┘
        │                      │                       │
        └──────── thenCombine ─┴────── thenCombine ────┘
                              │
                              ▼
                       ┌─────────────┐
                       │   Profile   │
                       └─────────────┘

   Sequential time:  900ms (300 + 300 + 300)
   Parallel time:    ~300ms (sab saath)


╔════════════════════════════════════════════════════════════╗
║ Exception Handling                                         ║
╚════════════════════════════════════════════════════════════╝

   ┌──────────────────┐
   │ supplyAsync()    │
   │   riskyCall()    │
   └────────┬─────────┘
            │
       ✓ success ──────► thenApply()
            │
       ✗ exception ────► exceptionally(ex -> "fallback")
                                │
                                ▼
                         (catch + recover)
```

---

## 💻 Future — Basic (Blocking)

```java
ExecutorService pool = Executors.newFixedThreadPool(2);

Future<Integer> future = pool.submit(() -> {
    Thread.sleep(2000);
    return 42;
});

System.out.println("Doing other work...");
Integer result = future.get();        // 🔴 BLOCK — 2 sec wait
System.out.println(result);
```

---

## 💻 CompletableFuture — Non-Blocking + Chain

### Async + chain
```java
CompletableFuture
    .supplyAsync(() -> fetchUser(101))         // async start
    .thenApply(user -> user.getName())          // transform
    .thenAccept(name -> System.out.println(name));   // consume — no return
```

### Combine 2 futures
```java
CompletableFuture<User> userF  = CompletableFuture.supplyAsync(() -> fetchUser(101));
CompletableFuture<Order> orderF = CompletableFuture.supplyAsync(() -> fetchOrder(101));

userF.thenCombine(orderF, (user, order) -> user.getName() + " ordered " + order.getId())
     .thenAccept(System.out::println);
```

### Wait for ALL
```java
CompletableFuture<Void> all = CompletableFuture.allOf(future1, future2, future3);
all.join();           // wait for all
```

### Exception handling
```java
CompletableFuture
    .supplyAsync(() -> riskyCall())
    .exceptionally(ex -> {
        System.err.println("Error: " + ex);
        return "fallback";
    });
```

---

## 🎯 Real Use Case — Parallel API Calls

```java
// 3 alag DB calls, parallel
CompletableFuture<User>     u = CompletableFuture.supplyAsync(() -> fetchUser());
CompletableFuture<List>     o = CompletableFuture.supplyAsync(() -> fetchOrders());
CompletableFuture<Settings> s = CompletableFuture.supplyAsync(() -> fetchSettings());

// Sab parallel run hua, ab combine karo
Profile profile = u.thenCombine(o, (user, orders) -> new Profile(user, orders))
                   .thenCombine(s, (p, settings) -> p.with(settings))
                   .join();
```

**Sequential time:** 300ms × 3 = 900ms
**Parallel with CompletableFuture:** ~300ms

---

## 🔴 TRAP 1 — `get()` ke `join()` Difference

> **`get()`** throws checked exception (`InterruptedException`, `ExecutionException`)
> **`join()`** throws unchecked exception (cleaner with streams/lambdas)
> Modern code mein `join()` zyada use hota lambdas mein.

## 🔴 TRAP 2 — Default Executor

> **`supplyAsync(supplier)`** = ForkJoinPool.commonPool() use karta — shared
> Production mein **apna executor pass karo**: `supplyAsync(supplier, myExecutor)`

```java
CompletableFuture.supplyAsync(() -> task(), myExecutor);   // ✅ better isolation
```

## 🔴 TRAP 3 — `Future.cancel()` Best Effort

> **Cancellation guarantee nahi** — running task interrupt aata hai, but task usse handle karna padta. Abrupt stop nahi hota.

---

## 💬 POWER PHRASE

> *"`Future` is blocking — `get()` halts the calling thread until the result arrives. `CompletableFuture` is non-blocking and chainable — use `thenApply` to transform, `thenCombine` to merge multiple futures, and `exceptionally` for error handling. It's the standard for async programming in modern Java."*

> **Yaad rakh:**
> Future = blocking, no chain (Java 5)
> CompletableFuture = non-blocking, chainable, callbacks (Java 8)
> Parallel APIs → CompletableFuture
> Production: pass apna executor
