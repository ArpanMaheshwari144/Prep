# ThreadLocal — Har Thread Ka Apna Locker

**Kahani:** server pe ek saath 3 request aayin: Arpan, Rahul, Sita. Teeno alag thread pe chal rahi hain. Ek shared `String currentUser` rakha, to sab ek hi whiteboard pe likh rahe hain. Arpan ki request padhne gayi aur use "Rahul" mila, kyunki beech me Rahul wale thread ne upar likh diya tha. Bank me iska matlab hai ki kisi aur ka account dikh gaya. `ThreadLocal` = naam ek, par har thread ka apna alag locker. Har thread ko sirf apni value dikhti hai.

```
ThreadLocal<String> currentUser
   Thread-A locker: "Arpan"   -> A ka get() = Arpan
   Thread-B locker: "Rahul"   -> B ka get() = Rahul
   Thread-C locker: "Sita"    -> C ka get() = Sita
```

**Tera practical:** [ThreadLocalDemo](practical/ThreadLocalDemo.java)
```
DEMO 1:  [T-A] Arpan · [T-B] Rahul · [T-C] Sita          <- har thread apni value
DEMO 2a: pool reuse, NO remove()   -> readOnly sees = Arpan   <- LEAK
DEMO 2b: pool reuse, WITH remove() -> readOnly sees = null    <- saaf
```

**Khaas baat:**
1. Do fayde: (a) ek variable, kai thread, koi takkar nahi. (b) userId / traceId har method me parameter bana ke 10 layer tak bhejna nahi padta, jahan chahiye `get()`.
2. Methods: `set()`, `get()`, `remove()`, aur shuru ki value ke liye `ThreadLocal.withInitial(() -> 0)`.
3. Sabse bada trap pool hai. Thread naya nahi banta, wahi thread dobara aata hai. `remove()` nahi kiya to Rahul ki request ko Arpan ki value mil jaati hai (DEMO 2a). Ilaaj:
   ```java
   try { currentUser.set(userId); /* kaam */ }
   finally { currentUser.remove(); }   // pool me wapas jaane se pehle locker saaf
   ```
4. Spring andar se yahi use karta hai: `SecurityContextHolder` (logged-in user), `@Transactional` ka transaction, logs ka MDC traceId. Tera JWT filter SecurityContext set karta hai, aur request khatam hone pe filter use saaf karta hai. Ye wahi `remove()` hai.

**Prod wala trap:** kaam ko `@Async` ya `CompletableFuture` pe bheja (file 10 aur 11), to wo doosre thread pe chala gaya. Doosre thread ka locker khaali hota hai.
```java
user.set("Arpan");
user.get();                                          // request thread: Arpan
CompletableFuture.supplyAsync(() -> user.get());     // async thread:   null
```
Async me `SecurityContextHolder.getContext()` khaali aata hai, aur log me traceId gayab ho jaata hai. Ilaaj: zaroori value pehle variable me nikaal ke async ko pass karo, ya Spring ka `TaskDecorator` lagao jo context copy kare.

```
Interview: "ThreadLocal gives each thread its own copy behind one reference - used for request context
like userId, traceId, SecurityContext without passing it through every method. With thread pools
I always remove() in finally, otherwise a reused thread leaks the previous request's value.
And it doesn't cross threads - in @Async or CompletableFuture I pass the value explicitly
or use a TaskDecorator."
```

---

## GEHRA HISSA (purani file se, interview me kaam ka)

## 1. PROBLEM pehle (kyun aaya)

Ek normal SHARED variable: `String currentUser;`

Server pe ek saath 3 request -> teeno ALAG THREAD pe:
```
Thread-A (req-1): currentUser = "Arpan"
Thread-B (req-2): currentUser = "Rahul"
Thread-C (req-3): currentUser = "Sita"
```
**Clash:** ek hi shared variable -> ek doosre ko OVERWRITE.
```
Thread-A ne "Arpan" likha, tab tak Thread-B ne "Rahul" likh diya ->
   Thread-A `currentUser` padhta -> "Rahul" -> GALAT (uska "Arpan" hona tha).
= finance me: ek user ka data doosre ko dikh gaya = disaster.
```

**Do ganda option:**
```
1. Har thread ke liye alag variable -> thread runtime pe bante, kitne banaoge -> nahi ho sakta.
2. userId ko HAR method me PARAMETER pass karo (controller->service->repo->util, 10 layer) -> ganda.
```

---

## 2. ThreadLocal = ilaaj (WHAT)

Dikhne me EK variable, par andar se **har thread ki apni PRIVATE copy**. Tu likhta ek hi jagah, har thread ko **sirf apni value**.
```
ThreadLocal<String> currentUser = new ThreadLocal<>();

Thread-A: currentUser.set("Arpan")  -> A ke locker me "Arpan"
Thread-B: currentUser.set("Rahul")  -> B ke locker me "Rahul"
Thread-A: currentUser.get() -> "Arpan"   (sirf apni)
Thread-B: currentUser.get() -> "Rahul"   (sirf apni)
-> clash KHATAM, ek doosre ko overwrite nahi karte.
```

**ANALOGY:** normal variable = ek COMMON whiteboard (sab likhte, mit-mita gadbad). ThreadLocal = naam ek, par har banda apne ALAG kamre me apna whiteboard dekhta -> koi kisi ka nahi mitata.

---

## 3. WHY (2 reason)
```
1. Kai thread "same variable" use karein bina CLASH (upar wala problem).
2. Per-request data (userId/traceId/logged-in-user) har method me PARAMETER pass kiye bina
   kahin bhi uthao -> "is thread = is request ke liye" -> jahan chahiye get().
```
Real: Spring **SecurityContextHolder** (logged-in user), **@Transactional** transaction-context, request-context — sab ThreadLocal pe.

---

## 4. Methods
```
threadLocal.set(value)   -> is thread ke liye value rakho
threadLocal.get()        -> is thread ki value lo (na ho to initialValue / null)
threadLocal.remove()     -> is thread ki value HATAO   <- CRITICAL (neeche trap)

// initial value:
ThreadLocal<Integer> tl = ThreadLocal.withInitial(() -> 0);
```

---

## 5. ★★ THE TRAP — thread-pool REUSE (interview #1, finance-disaster)

Thread hamesha naye nahi bante -> ek POOL, threads REUSE hote.
```
Req-1 -> Thread-A pe -> currentUser.set("Arpan") ... kaam done. PAR remove() NAHI kiya.
   Thread-A wapas POOL me -> uske locker me abhi bhi "Arpan" pada!
Baad me Req-2 (Rahul) -> pool ne wahi Thread-A diya (reuse) ->
   currentUser.get() -> "Arpan" (STALE!) -> Rahul ki request ko Arpan ka data = LEAK.
   (bank me = kisi aur ka account dikh gaya)
```

**ILAAJ — `finally` me `remove()`:**
```java
try {
    currentUser.set(loggedInUserId);   // request START
    // ... business logic, kahin bhi currentUser.get()
} finally {
    currentUser.remove();              // ★ request END -> locker SAAF (pool me bhejne se pehle)
}
```
> ONE LINE: ThreadLocal + thread-pool = **hamesha `finally` me `remove()`** (warna pichhli request ki value leak + memory-leak).

---

## 6. Bonus (grill me aata)
```
InheritableThreadLocal -> CHILD thread parent ki value INHERIT karta
   (normal ThreadLocal me child ko parent ki value NAHI dikhti).
Memory-leak: pooled-thread + no-remove() -> value held rehti -> leak. remove() = ilaaj.
```

---

## ★ CONNECT — usercrud / Spring Security
Spring ka `SecurityContextHolder.getContext().getAuthentication()` = ThreadLocal pe rakha logged-in user.
Har HTTP request (ek thread) apna security-context ThreadLocal me rakhta -> controller/service kahin bhi
`getAuthentication()` se current-user uthata bina param-passing. Filter chain request-end pe context CLEAR
karti (wahi remove() ka concept). Tera JWT-filter -> SecurityContext set -> downstream use = yahi pattern.
[[10_executor_service_thread_pool]] [[05_volatile]]

---

## ★ LIVE DEMO (chala ke dekha — [practical/ThreadLocalDemo.java](practical/ThreadLocalDemo.java))
```
DEMO 1 (3 threads):  T-A->Arpan · T-B->Rahul · T-C->Sita   = isolation (clash nahi)
DEMO 2a (no remove): readOnly -> "Arpan" (stale, pichhli req ki) = LEAK live
DEMO 2b (with remove): readOnly -> null   = clean
```
-> per-thread isolation + pool-reuse leak + remove()-fix, teeno output me confirm.

## POWER PHRASE
> "ThreadLocal = per-thread isolated variable — each thread has its own copy behind one reference. Used for
>  request-scoped context (userId, traceId, SecurityContext) without threading it through every method.
>  ★ Critical with thread pools: always remove() in finally, else a reused thread leaks the previous request's
>  value (data leak + memory leak). Spring's SecurityContextHolder and transaction context are ThreadLocal-based."
