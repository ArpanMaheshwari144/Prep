# Future vs CompletableFuture

## Kahani: profile page ke teen call

File 01 wala API phir se le. Profile page ke liye teen call chahiye: user, orders aur recommendations, aur har call 300ms leti hai. File 10 me humein `Future` mila tha, par dikkat ye hai ki `future.get()` likhte hi main thread ruk jaata hai. Aur teen `get()` ko jodna bhi bhadda lagta hai.

`CompletableFuture` me tu teeno call ek saath bhej deta hai, aur bol deta hai "jab teeno aa jaayein to inhe jod dena". Ye bilkul waise hai jaise Swiggy pe teen dukaan se order kiya aur bola "sab aa jaaye to ek saath de dena".

```
ek ke baad ek:  user ──> orders ──> recs          ~900 ms
parallel:       user  ┐
                orders├── thenCombine ──> profile ~300 ms
                recs  ┘
```

## Khaas baat

**1. Asli farak.** `Future` me `get()` bulate hi thread ruk jaata hai, aur tu aage ka step uske saath jod nahi sakta. `CompletableFuture` me tu ek ke baad ek step jodta ja sakta hai (`thenApply`), do alag kaam ko jod sakta hai (`thenCombine`), aur sab kaam ka intezaar ek saath kar sakta hai (`allOf`).

**2. Teen jodiyan jo confuse karti hain.**

`thenApply` vs `thenCompose`: `thenApply` me tera function seedha ek value lautata hai. `thenCompose` me tera function khud ek CF (CompletableFuture) lautata hai. Agar aise function pe `thenApply` lagaya, to CF<CF<>> ban jaata hai, yaani CF ke andar CF. `thenCompose` use chapta (flatten) karke seedha CF<User> de deta hai. Ye bilkul Stream ke map vs flatMap jaisa hai.

`thenCombine` vs `thenCompose`: `thenCombine` me do alag kaam saath-saath chalte hain, aur dono khatam hone pe unka result joda jaata hai. `thenCompose` tab lagta hai jab B ko A ka result chahiye, isliye B, A ke khatam hone ke baad hi chalta hai.

`exceptionally` / `handle` / `whenComplete`: `exceptionally` sirf error aane pe chalta hai aur ek fallback value de deta hai. `handle` ko result aur error dono milte hain, aur wo ek naya value de sakta hai. `whenComplete` sirf log ya cleanup ke liye hai, wo value badal nahi sakta.

**3. `get()` vs `join()`.** `get()` checked exception deta hai, aur `join()` unchecked exception deta hai. Isliye lambda ke andar `join()` likhna aasaan padta hai, try-catch nahi lagana padta.

**4. Kaunsa thread chalayega.** `thenApply` usi thread pe chalta hai jisne pichhla step poora kiya tha. `thenApplyAsync` kaam ko pool ke kisi doosre thread pe bhej deta hai.

## Prod wala trap: do galtiyan jo aksar ek saath hoti hain

```java
CompletableFuture.supplyAsync(() -> fetchRecs());          // GALAT: commonPool + koi timeout nahi
CompletableFuture.supplyAsync(() -> fetchRecs(), myPool)   // SAHI: apna pool
    .orTimeout(500, MILLISECONDS)                          // atka to 500ms me chhodo
    .exceptionally(ex -> emptyRecs());                     // fallback, page phir bhi khule
```

**Pehli galti:** agar tune apna pool nahi diya, to ye `ForkJoinPool.commonPool()` pe chalta hai. Ye pool poore JVM me sab ke saath share hota hai, aur chhota hota hai. Agar usme DB ya API ka intezaar karne wale kaam daal diye, to baaki saare async kaam bhi atak jaate hain.

**Doosri galti:** agar timeout nahi lagaya, to ek atka hua recommendations call poore profile page ko latka deta hai. Timeout aur fallback lagane se page chal jaata hai, bas recommendations wala hissa khaali dikhta hai.

Ye wahi soch hai jo payment design me thi (timeout + fallback), aur wahi jo PR drill me "bahar ki call me timeout" wala point hai.

## Tera project

usercrud ka `@Async` method `CompletableFuture<X>` lauta sakta hai, aur caller usko `thenCombine` se doosre kaam ke saath jod sakta hai. Production me async service aksar isi tarah banti hain.

## Interview line

```
Interview: "Future is blocking - get() halts the caller and you can't chain. CompletableFuture is
non-blocking and composable: thenApply to transform, thenCompose for a dependent async call,
thenCombine or allOf to run independent calls in parallel, exceptionally or handle for errors.
In production I always pass my own executor instead of the common pool, and add orTimeout
with a fallback so one slow dependency can't hang the whole request."
```


---

## GEHRA HISSA (purani file se, interview me kaam ka)

## Future vs CompletableFuture: paanch farak

- **Blocking hai ya nahi:** `Future` (Java 5) blocking hai, kyunki `get()` thread ko rok deta hai. `CompletableFuture` (Java 8) blocking nahi hai, wo callbacks se chalta hai.
- **Steps jod sakte ho:** `Future` me nahi. `CompletableFuture` me haan, `thenApply` aur `thenCompose` se.
- **Kai kaam ko mila sakte ho:** `Future` me nahi. `CompletableFuture` me haan, `thenCombine` aur `allOf` se.
- **Exception kaise sambhalte ho:** `Future` me `get()` exception phenkta hai. `CompletableFuture` me `exceptionally()` aur `handle()` hain.
- **Haath se complete kar sakte ho:** `Future` me nahi. `CompletableFuture` me haan, `complete()` se.

## CompletableFuture: bina ruke, aur steps jod ke

### Async shuru karo, aur aage steps jodo

```java
CompletableFuture
    .supplyAsync(() -> fetchUser(101))         // async start
    .thenApply(user -> user.getName())          // transform
    .thenAccept(name -> System.out.println(name));   // consume — no return
```

### Do future ko milana

```java
CompletableFuture<User> userF  = CompletableFuture.supplyAsync(() -> fetchUser(101));
CompletableFuture<Order> orderF = CompletableFuture.supplyAsync(() -> fetchOrder(101));

userF.thenCombine(orderF, (user, order) -> user.getName() + " ordered " + order.getId())
     .thenAccept(System.out::println);
```

### Sabka intezaar ek saath

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

## TRAP 3: `Future.cancel()` sirf koshish karta hai (best effort)

Cancel karne ki koi guarantee nahi hai. `cancel(true)` bulane pe chalte hue task ko sirf ek interrupt bheja jaata hai, aur task ko khud interrupt check karna padta hai. `cancel(false)` me to chal raha task chalta hi rehta hai. Matlab task ek jhatke me nahi rukta.

Aur `CompletableFuture` me to `cancel(true)` chalte task ko interrupt bhi NAHI karta. Wo bas CF ko "cancelled" mark kar deta hai.

### 1. thenApply vs thenApplyAsync: kaunsa thread chalayega?

`thenApply(fn)` me `fn` usi thread pe chalta hai jisne pichhla stage complete kiya tha. Agar CF pehle se hi done ho, to `fn` us thread pe chalta hai jo `thenApply` bula raha hai. Ye halka hai, kyunki thread badalna nahi padta.

`thenApplyAsync(fn)` me `fn` ek alag thread pe chalta hai. Default me wo `ForkJoinPool.commonPool` hota hai, ya jo executor tu de. Ye tab lagao jab kaam bhaari ho, ya jab tu chahta hai ki jis thread ne pichhla stage poora kiya wo free rahe.

Yaad rakhne ke liye: `thenApply` matlab "jis bande ne kaam khatam kiya, wahi agla step bhi kar de". `Async` matlab "agla step kisi naye bande ko de do".

Grill me bolne wali line:

```
"Async variant shifts the continuation to a thread pool; non-Async runs it on whichever thread completed the previous stage."
```

## Visualization: steps kaise judte hain

```
              Future vs CompletableFuture — Chain Diagram

╔════════════════════════════════════════════════════════════╗
║ Future — Blocking (Java 5)                                 ║
╚════════════════════════════════════════════════════════════╝

   Main Thread                          Worker Thread
       │                                     │
       ├─── pool.submit(task) ──────────────►│ task start
       │                                     │   ...
       │  (doosra kaam...)                   │
       │                                     │
       │                                     │
       ├─── future.get() ───────────────────►│
       │                                     │
       │     BLOCKED                         │
       │     (wait kar raha)                 │
       │                                     │   ...
       │                                     │ task done
       │◄────── return 42 ───────────────────┤
       │                                     │
       │  result = 42                        │

   "Async ka faayda gaya — main thread ruka hua hai"


╔════════════════════════════════════════════════════════════╗
║ CompletableFuture — Non-Blocking + Chain (Java 8)          ║
╚════════════════════════════════════════════════════════════╝

  Main thread chalta rehta hai, worker thread task sambhalta hai

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
   │ thenAccept()     │  → name pe action (kuch return nahi)
   │ name → print     │
   └──────────────────┘

   Pipeline! Main thread block nahi hua, callbacks chain mein chale


╔════════════════════════════════════════════════════════════╗
║ Kai Futures ko milana — Parallel                           ║
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

   Ek ke baad ek:  900ms (300 + 300 + 300)
   Parallel:       ~300ms (sab saath)


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
                         (pakdo + sambhalo)
```

### 2. thenApply vs thenCompose: VALUE lautaya ya CF lautaya (flatMap wali baat)

`thenApply(fn)` me `fn` ek seedhi VALUE lautata hai, to result CF<Value> banta hai.

`thenCompose(fn)` tab lagta hai jab `fn` khud ek CompletableFuture lautata hai. Agar aise me tu `thenApply` laga deta, to CF<CF<Value>> ban jaata, yaani nested aur ganda. `thenCompose` use flatten karke seedha CF<Value> bana deta hai.

### 3. exceptionally vs handle vs whenComplete: error sambhalne ke 3 alag tareeke

`exceptionally(ex -> fallback)` sirf error aane pe chalta hai, aur ek fallback VALUE de ke recover kar leta hai. Success hone pe ye skip ho jaata hai.

`handle((res, ex) -> newVal)` ko result aur exception dono milte hain. Isse tu dono case ek hi jagah sambhal leta hai, aur ek naya value de sakta hai. Matlab ye recover bhi kar sakta hai aur value badal bhi sakta hai.

`whenComplete((res, ex) -> {...})` ko bhi dono milte hain, par ye value BADAL nahi sakta. Ye sirf side-effect ke liye hai, jaise log ya cleanup, aur value jaisi thi waisi aage chali jaati hai.

### 3b. Yahi teen, ek-ek line me yaad rakhne ke liye

`exceptionally` ko try-catch ka async roop samjho: sirf error pe recover karta hai.

`handle` ko result mile ya error, dono me naya value deta hai, to ye recover aur transform dono karta hai.

`whenComplete` bas jhaank ke dekhta hai (peek), log ya cleanup karta hai, aur value ko bina chhue aage bhej deta hai.

### thenCombine vs thenCompose: dependent ya parallel (jaldi wala)

`thenCompose` matlab ek ke BAAD doosra, kyunki B ko A ka result chahiye. Ye dependent chain hai.

`thenCombine` matlab do INDEPENDENT CF saath-saath chalte hain, aur dono done hone pe unhe joda jaata hai.
