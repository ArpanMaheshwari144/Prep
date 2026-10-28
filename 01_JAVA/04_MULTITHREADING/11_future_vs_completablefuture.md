# Future vs CompletableFuture

**Kahani:** file 01 wala API phir se: user, orders aur recommendations, teen call, har ek 300ms. File 10 me `Future` mila tha, par `future.get()` likhte hi main thread ruk jaata hai. Teen `get()` ko jodna bhi bhadda lagta hai. `CompletableFuture` me teeno call ek saath bhej do, aur bol do "teeno aa jaayein to jod dena". Ye wahi hai jaise Swiggy pe teen dukaan se order kiya aur bola "sab aa jaaye to ek saath de dena".

```
ek ke baad ek:  user ──> orders ──> recs          ~900 ms
parallel:       user  ┐
                orders├── thenCombine ──> profile ~300 ms
                recs  ┘
```

**Khaas baat:**
1. Fark:
   - `Future`: `get()` pe thread ruk jaata hai, aage ka step jod nahi sakte.
   - `CompletableFuture`: ek ke baad ek step jodte jao (`thenApply`), do alag kaam jodo (`thenCombine`), sabka intezaar ek saath karo (`allOf`).
2. Teen jodiyan jo confuse karti hain:
   - `thenApply` vs `thenCompose`:
     - `thenApply`: function seedha value lautata hai.
     - `thenCompose`: function khud ek CF lautata hai. `thenApply` lagaya to CF<CF<>> ban jaata, `thenCompose` use chapta (flatten) karke CF<User> deta hai. Stream ke map vs flatMap jaisa.
   - `thenCombine` vs `thenCompose`:
     - `thenCombine`: do alag kaam saath-saath chalte hain, phir jodte hain.
     - `thenCompose`: B ko A ka result chahiye, to A ke baad B chalta hai.
   - `exceptionally` / `handle` / `whenComplete`:
     - `exceptionally`: sirf error pe chalta hai aur fallback value deta hai.
     - `handle`: result aur error dono milte hain, aur naya value de sakta hai.
     - `whenComplete`: sirf log ya cleanup ke liye, value nahi badal sakta.
3. `get()` checked exception deta hai, `join()` unchecked. Lambda ke andar `join()` likhna aasaan hai.
4. `thenApply` usi thread pe chalta hai jisne pichhla step poora kiya. `thenApplyAsync` kaam ko pool ke kisi doosre thread pe bhej deta hai.

**Prod wala trap:** do galtiyan ek saath hoti hain.
```java
CompletableFuture.supplyAsync(() -> fetchRecs());          // GALAT: commonPool + koi timeout nahi
CompletableFuture.supplyAsync(() -> fetchRecs(), myPool)   // SAHI: apna pool
    .orTimeout(500, MILLISECONDS)                          // atka to 500ms me chhodo
    .exceptionally(ex -> emptyRecs());                     // fallback, page phir bhi khule
```
- Pehli galti: bina pool diye ye `ForkJoinPool.commonPool()` pe chalta hai, jo poore JVM me sab ke saath share hota hai aur chhota hota hai. Usme DB / API ka intezaar karne wale kaam daale to baaki sab async kaam bhi atak jaate hain.
- Doosri galti: bina timeout ke ek atka hua recommendations call poore profile page ko latka deta hai. Timeout + fallback se page chal jaata hai, bas recommendations khaali.

Ye wahi soch hai jo payment design me thi (timeout + fallback), aur jo PR drill me "bahar ki call me timeout" wala point hai.

**Tera project:** usercrud `@Async` method `CompletableFuture<X>` lauta sakta hai, aur caller usko `thenCombine` se jod sakta hai. Production me async service aksar isi tarah bante hain.

```
Interview: "Future is blocking - get() halts the caller and you can't chain. CompletableFuture is
non-blocking and composable: thenApply to transform, thenCompose for a dependent async call,
thenCombine or allOf to run independent calls in parallel, exceptionally or handle for errors.
In production I always pass my own executor instead of the common pool, and add orTimeout
with a fallback so one slow dependency can't hang the whole request."
```


---

## GEHRA HISSA (purani file se, interview me kaam ka)

## Future vs CompletableFuture

| | `Future` (Java 5) | `CompletableFuture` (Java 8) |
|--|------------------|-------------------------------|
| **Blocking?** | YES (`get()` blocks) | NO — callbacks |
| **Chain operations?** | NO | YES (`thenApply`, `thenCompose`) |
| **Combine multiple?** | NO | YES (`thenCombine`, `allOf`) |
| **Exception handling?** | `get()` throws | `exceptionally()`, `handle()` |
| **Manually complete?** | NO | YES (`complete()`) |

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

## TRAP 3 — `Future.cancel()` Best Effort

> **Cancellation guarantee nahi** — `cancel(true)` pe running task ko sirf interrupt bheja jaata (`cancel(false)` = chal raha task chalta rahega), task ko khud interrupt check karna padta. Abrupt stop nahi hota.
> (CompletableFuture me `cancel(true)` chalte task ko interrupt bhi NAHI karta — sirf CF ko "cancelled" mark karta.)

### 1. thenApply vs thenApplyAsync  (kaun-sa thread?)
```
thenApply(fn)       -> fn us SAME thread pe chalta jisne pichhla stage COMPLETE kiya
                       (ya agar CF already-done ho to CALLING thread pe). = lightweight, no thread-switch.
thenApplyAsync(fn)  -> fn ALAG thread pe (default ForkJoinPool.commonPool, ya jo executor do).
                       = jab kaam BHAARI ho, ya completing-thread ko free rakhna ho.
```
Anchor: `thenApply` = "jo bhi banda kaam khatam kare, wahi agla step bhi kar de" · `Async` = "naye banda ko de do".
GRILL-line: "Async variant shifts the continuation to a thread pool; non-Async runs it on whichever thread completed the previous stage."

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

### 2. thenApply vs thenCompose  (VALUE vs CF -> flatMap)
```
thenApply(fn)   : fn return karta ek VALUE      -> CF<Value>
thenCompose(fn) : fn return karta ek CompletableFuture -> agar thenApply lagate to CF<CF<Value>> (NESTED, ganda)
                  -> thenCompose FLATTEN kar deta -> CF<Value>.
```

### 3. exceptionally vs handle vs whenComplete  (error handling — 3 alag)
```
exceptionally(ex -> fallback)      -> SIRF error pe chale -> fallback VALUE de (recover). success pe skip.
handle((res, ex) -> newVal)        -> DONO (result AUR ex) milte -> ek jagah handle -> naya value DE sakta (recover+transform).
whenComplete((res, ex) -> {...})   -> DONO milte par value BADAL nahi sakta -> side-effect only (log/cleanup) -> value wahi aage.
```

### 3. exceptionally vs handle vs whenComplete  (error handling — 3 alag)
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
