# Future vs CompletableFuture

> **Multithreading: Topic 47 — Java 8 async (V90 ke bahar but production-critical)**

---

## STORY — Async Result Chahiye

→ `executor.submit(callable)` ne **`Future`** return kiya
→ `future.get()` call kiya — **block ho gaya** jab tak result na aaye. **Blocking call** — async ka faayda gone
→ Multiple async results ek saath chain karne mushkil
→ **Java 8 ne `CompletableFuture` diya** — non-blocking, chainable, callbacks support

---

## Future vs CompletableFuture

| | `Future` (Java 5) | `CompletableFuture` (Java 8) |
|--|------------------|-------------------------------|
| **Blocking?** | YES (`get()` blocks) | NO — callbacks |
| **Chain operations?** | NO | YES (`thenApply`, `thenCompose`) |
| **Combine multiple?** | NO | YES (`thenCombine`, `allOf`) |
| **Exception handling?** | `get()` throws | `exceptionally()`, `handle()` |
| **Manually complete?** | NO | YES (`complete()`) |

---

## Visualization — Chain Operations

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
       │     BLOCKED                      │
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
       success ──────► thenApply()
            │
       exception ────► exceptionally(ex -> "fallback")
                                │
                                ▼
                         (catch + recover)
```

---

## Future — Basic (Blocking)

```java
ExecutorService pool = Executors.newFixedThreadPool(2);

Future<Integer> future = pool.submit(() -> {
    Thread.sleep(2000);
    return 42;
});

System.out.println("Doing other work...");
Integer result = future.get();        // BLOCK — 2 sec wait
System.out.println(result);
```

---

## CompletableFuture — Non-Blocking + Chain

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

## Real Use Case — Parallel API Calls

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

## TRAP 1 — `get()` ke `join()` Difference

> **`get()`** throws checked exception (`InterruptedException`, `ExecutionException`)
> **`join()`** throws unchecked exception (cleaner with streams/lambdas)
> Modern code mein `join()` zyada use hota lambdas mein.

## TRAP 2 — Default Executor

> **`supplyAsync(supplier)`** = ForkJoinPool.commonPool() use karta — shared
> Production mein **apna executor pass karo**: `supplyAsync(supplier, myExecutor)`

```java
CompletableFuture.supplyAsync(() -> task(), myExecutor);   // better isolation
```

## TRAP 3 — `Future.cancel()` Best Effort

> **Cancellation guarantee nahi** — running task interrupt aata hai, but task usse handle karna padta. Abrupt stop nahi hota.

---

## POWER PHRASE

> *"`Future` is blocking — `get()` halts the calling thread until the result arrives. `CompletableFuture` is non-blocking and chainable — use `thenApply` to transform, `thenCombine` to merge multiple futures, and `exceptionally` for error handling. It's the standard for async programming in modern Java."*

> **Yaad rakh:**
> Future = blocking, no chain (Java 5)
> CompletableFuture = non-blocking, chainable, callbacks (Java 8)
> Parallel APIs → CompletableFuture
> Production: pass apna executor

---

## ★★ DEEP-GRILL LAYER — thread-of-execution (11-Sep) — interview yahi kuredta

> Upar "kya karta" hai. Ye 3 distinction = "KAUN-SE THREAD pe + kaunsa method kab" — real grill.

### 1. thenApply vs thenApplyAsync  (kaun-sa thread?)
```
thenApply(fn)       -> fn us SAME thread pe chalta jisne pichhla stage COMPLETE kiya
                       (ya agar CF already-done ho to CALLING thread pe). = lightweight, no thread-switch.
thenApplyAsync(fn)  -> fn ALAG thread pe (default ForkJoinPool.commonPool, ya jo executor do).
                       = jab kaam BHAARI ho, ya completing-thread ko free rakhna ho.
```
Anchor: `thenApply` = "jo bhi banda kaam khatam kare, wahi agla step bhi kar de" · `Async` = "naye banda ko de do".
GRILL-line: "Async variant shifts the continuation to a thread pool; non-Async runs it on whichever thread completed the previous stage."

### 2. thenApply vs thenCompose  (VALUE vs CF -> flatMap)
```
thenApply(fn)   : fn return karta ek VALUE      -> CF<Value>
thenCompose(fn) : fn return karta ek CompletableFuture -> agar thenApply lagate to CF<CF<Value>> (NESTED, ganda)
                  -> thenCompose FLATTEN kar deta -> CF<Value>.
```
= bilkul Stream ka **map vs flatMap**. fn khud async-call (CF) return kare -> thenCompose. warna thenApply.
```java
cf.thenApply(id -> id * 2)               // fn -> value
  .thenCompose(id -> fetchUserFromDb(id))// fn -> CF<User> (flatten, warna CF<CF<User>>)
```

### 3. exceptionally vs handle vs whenComplete  (error handling — 3 alag)
```
exceptionally(ex -> fallback)      -> SIRF error pe chale -> fallback VALUE de (recover). success pe skip.
handle((res, ex) -> newVal)        -> DONO (result AUR ex) milte -> ek jagah handle -> naya value DE sakta (recover+transform).
whenComplete((res, ex) -> {...})   -> DONO milte par value BADAL nahi sakta -> side-effect only (log/cleanup) -> value wahi aage.
```
```
exceptionally = try-catch ka async (error-only recover)
handle        = result-YA-error -> naya value (recover + transform dono)
whenComplete  = PEEK (log/cleanup) -> value untouched aage
```

### thenCombine vs thenCompose (dependent vs parallel — quick)
```
thenCompose = ek ke BAAD doosra (B ko A ka result chahiye = DEPENDENT chain)
thenCombine = do INDEPENDENT CF parallel chalein -> dono done -> jodo
```

### ★ POWER-PHRASE (deep)
> "Non-Async continuations run on the thread that completed the prior stage; Async variants offload to a pool.
>  thenApply maps a value, thenCompose flatMaps a CF (avoids CF<CF<>>), thenCombine joins two independent futures.
>  Error handling: exceptionally (error-only recover), handle (result-or-error → new value), whenComplete (peek, value unchanged)."

### ★ CONNECT — usercrud @Async
Tera usercrud `@Async` DemoJobs = Spring ka thread-off-load (naye thread pe method). CompletableFuture = usi async ka
CHAINABLE version — @Async method `CompletableFuture<X>` return kar sakta, phir caller thenApply/thenCombine se chain kare.
Spring `@Async` + `CompletableFuture` return = production async-service pattern. [[10_executor_service_thread_pool]]
