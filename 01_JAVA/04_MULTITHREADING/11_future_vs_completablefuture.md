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
