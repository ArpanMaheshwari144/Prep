# Future vs CompletableFuture

## Scene: profile page ke teen call, Swiggy jaisa

File 01 wala profile page: teen call chahiye (user, orders, recommendations), har ek 300 ms. File 10 ke `Future` me `get()`
bulate hi cashier khada ho jaata hai, aur teen `get()` jodna bhadda lagta hai. `CompletableFuture` Swiggy jaisa hai: teen
dukaan se order kiya aur bola "teeno aa jaayein to ek saath de dena". Chala ke dekha:

```
teeno saath:        user+orders+recs    305 ms
recs atka (5 sec):  user+recs-khaali    503 ms    <- 500 ms pe chhoda, page phir bhi khula
```

Kaam ke chaar shabd: `thenApply` result ko badalta hai. `thenCompose` tab lagta hai jab agle call ko pichhle ka result chahiye
(Stream ke `flatMap` jaisa, CF ke andar CF nahi banta). `thenCombine` / `allOf` alag-alag call saath chalake jodte hain. Aur
`exceptionally` error aane pe fallback value deta hai. `get()` checked exception deta hai aur `join()` unchecked, isliye lambda
ke andar `join()` aasaan hai.

## Prod wala trap

```java
CompletableFuture.supplyAsync(() -> fetchRecs());          // GALAT: commonPool + koi timeout nahi
CompletableFuture.supplyAsync(() -> fetchRecs(), myPool)   // SAHI: apna pool
    .orTimeout(500, MILLISECONDS)                          // atka to 500ms me chhodo
    .exceptionally(ex -> emptyRecs());                     // fallback, page phir bhi khule
```

Pehli galti: apna pool nahi diya, to kaam `ForkJoinPool.commonPool()` pe chalta hai, jo chhota hai aur poore JVM ka saanjha
hai. Usme DB ya API ke intezaar wale kaam daale, to baaki sab async kaam bhi atak jaate hain. Doosri galti: timeout nahi
lagaya, to ek atka hua recs call poora page latka deta hai. Upar ki doosri line dekh: timeout aur fallback ke saath page 500 ms
me khul gaya, bas recs khaali the. Ye wahi soch hai jo payment design me thi (timeout + fallback), aur PR drill ka "bahar ki
call pe timeout".

## Tera project

Usercrud ka `@Async` method `CompletableFuture<X>` lauta sakta hai, aur caller use `thenCombine` se doosre kaam ke saath jod
sakta hai.
