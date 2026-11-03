# ThreadLocal — Har Thread Ka Apna Locker

## Kahani: ek whiteboard, teen request

Server pe ek saath 3 request aayin: Arpan, Rahul aur Sita ki. Teeno alag-alag thread pe chal rahi hain. Agar tune ek shared `String currentUser` rakha, to matlab teeno ek hi whiteboard pe likh rahe hain. Arpan ki request ne value padhi to use "Rahul" mila, kyunki beech me Rahul wale thread ne uske upar likh diya tha. Bank me iska matlab hai ki kisi ko kisi aur ka account dikh gaya.

`ThreadLocal` ka matlab hai: naam ek hi hai, par har thread ka apna alag locker hai. Har thread ko sirf apni value dikhti hai.

```
ThreadLocal<String> currentUser
   Thread-A ka locker: "Arpan"   -> A ka get() = Arpan
   Thread-B ka locker: "Rahul"   -> B ka get() = Rahul
   Thread-C ka locker: "Sita"    -> C ka get() = Sita
```

## Tera practical

[ThreadLocalDemo](practical/ThreadLocalDemo.java) chala ke ye mila:

```
DEMO 1:  [T-A] Arpan, [T-B] Rahul, [T-C] Sita                  <- har thread ko apni value mili
DEMO 2a: pool reuse, remove() NAHI kiya -> readOnly ko dikha = Arpan   <- LEAK
DEMO 2b: pool reuse, remove() kiya      -> readOnly ko dikha = null    <- saaf
```

## Khaas baat

**1. Iske do fayde hain.** Pehla: ek hi variable ko kai thread use kar sakte hain, aur aapas me koi takkar nahi hoti. Doosra: userId ya traceId ko har method me parameter bana ke 10 layer tak bhejna nahi padta. Jahan chahiye, wahan `get()` kar lo.

**2. Methods.** `set()` se value rakhte ho, `get()` se value lete ho, aur `remove()` se value hatate ho. Shuru ki value chahiye to `ThreadLocal.withInitial(() -> 0)` likhte hain.

**3. Sabse bada trap thread pool hai.** Pool me thread naya nahi banta, wahi purana thread dobara aata hai. Agar tune `remove()` nahi kiya, to Rahul ki request ko Arpan ki value mil jaati hai, jaise DEMO 2a me hua. Ilaaj ye hai:

```java
try { currentUser.set(userId); /* kaam */ }
finally { currentUser.remove(); }   // pool me wapas jaane se pehle locker saaf
```

**4. Spring andar se yahi use karta hai.** `SecurityContextHolder` (logged-in user), `@Transactional` ka transaction, aur logs ka MDC traceId, ye sab ThreadLocal pe chalte hain. Tera JWT filter SecurityContext set karta hai, aur request khatam hone pe filter use saaf kar deta hai. Wo saaf karna wahi `remove()` hai.

## Prod wala trap: async me value nahi pahunchti

Jab tune kaam ko `@Async` ya `CompletableFuture` pe bheja (file 10 aur 11), to wo kaam doosre thread pe chala gaya. Aur doosre thread ka locker khaali hota hai.

```java
user.set("Arpan");
user.get();                                          // request thread: Arpan
CompletableFuture.supplyAsync(() -> user.get());     // async thread:   null
```

Isi wajah se async me `SecurityContextHolder.getContext()` khaali aata hai, aur log me traceId gayab ho jaata hai. Ilaaj do hain: zaroori value pehle ek variable me nikaal lo aur async ko pass karo, ya Spring ka `TaskDecorator` lagao jo context ko naye thread me copy kar de.

## Interview line

```
Interview: "ThreadLocal gives each thread its own copy behind one reference - used for request context
like userId, traceId, SecurityContext without passing it through every method. With thread pools
I always remove() in finally, otherwise a reused thread leaks the previous request's value.
And it doesn't cross threads - in @Async or CompletableFuture I pass the value explicitly
or use a TaskDecorator."
```

---

## GEHRA HISSA (purani file se, interview me kaam ka)

## 1. Pehle PROBLEM (ye aaya kyun)

Maan lo ek normal SHARED variable hai: `String currentUser;`

Server pe ek saath 3 request aati hain, aur teeno ALAG THREAD pe chalti hain:

```
Thread-A (req-1): currentUser = "Arpan"
Thread-B (req-2): currentUser = "Rahul"
Thread-C (req-3): currentUser = "Sita"
```

**Takkar kahan hai:** variable ek hi hai aur shared hai, to sab ek doosre ki value ke upar likh dete hain (OVERWRITE). Thread-A ne "Arpan" likha, par tab tak Thread-B ne "Rahul" likh diya. Ab Thread-A `currentUser` padhta hai to use "Rahul" milta hai, jo GALAT hai, kyunki uske liye "Arpan" hona chahiye tha. Finance me iska matlab hai ki ek user ka data doosre ko dikh gaya, jo disaster hai.

**Do gande raaste jo kaam nahi aate:**

Pehla raasta: har thread ke liye alag variable bana do. Par thread to runtime pe bante hain, to pehle se kitne variable banaoge? Ye ho hi nahi sakta.

Doosra raasta: userId ko HAR method me PARAMETER bana ke bhejo, controller se service, service se repo, repo se util, 10 layer tak. Ye ganda hai.

---

## 2. ThreadLocal hi ilaaj hai (WHAT)

Dikhne me ye EK variable hai, par andar se **har thread ki apni PRIVATE copy** hoti hai. Tu ek hi jagah likhta hai, aur har thread ko **sirf apni value** milti hai.

```
ThreadLocal<String> currentUser = new ThreadLocal<>();

Thread-A: currentUser.set("Arpan")  -> A ke locker me "Arpan"
Thread-B: currentUser.set("Rahul")  -> B ke locker me "Rahul"
Thread-A: currentUser.get() -> "Arpan"   (sirf apni)
Thread-B: currentUser.get() -> "Rahul"   (sirf apni)
-> takkar KHATAM, koi kisi ki value overwrite nahi karta.
```

**Misaal:** normal variable ek COMMON whiteboard jaisa hai, jahan sab likhte hain aur ek doosre ka likha mita dete hain, to gadbad hoti hai. ThreadLocal me naam ek hi hai, par har banda apne ALAG kamre me apna whiteboard dekhta hai, isliye koi kisi ka likha nahi mitata.

---

## 3. WHY: do wajah

**Pehli wajah:** kai thread "same variable" use kar sakein, aur unki aapas me takkar na ho (upar wali problem).

**Doosri wajah:** per-request data (userId, traceId, logged-in user) ko har method me PARAMETER bana ke bheje bina kahin bhi utha sako. Kyunki "ye thread = ye request", to jahan bhi chahiye wahan `get()` kar lo.

Asli duniya me: Spring ka **SecurityContextHolder** (logged-in user), **@Transactional** ka transaction-context, aur request-context, ye sab ThreadLocal pe hi chalte hain.

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

## 5. THE TRAP: thread pool me thread REUSE hota hai (interview me #1, finance ka disaster)

Thread hamesha naye nahi bante. Ek POOL hota hai, aur usme threads dobara use (REUSE) hote hain. Ab kahani dekh:

Req-1 Thread-A pe aayi. Usne `currentUser.set("Arpan")` kiya aur kaam poora ho gaya. PAR `remove()` NAHI kiya. Thread-A wapas POOL me chala gaya, aur uske locker me abhi bhi "Arpan" pada hai.

Baad me Req-2 aayi, jo Rahul ki hai. Pool ne use wahi Thread-A de diya (reuse). Ab `currentUser.get()` ne "Arpan" lautaya, jo purani (STALE) value hai. Matlab Rahul ki request ko Arpan ka data mil gaya, yahi LEAK hai. Bank me iska matlab hai ki kisi aur ka account dikh gaya.

**ILAAJ: `finally` me `remove()`.**

```java
try {
    currentUser.set(loggedInUserId);   // request START
    // ... business logic, kahin bhi currentUser.get()
} finally {
    currentUser.remove();              // ★ request END -> locker SAAF (pool me bhejne se pehle)
}
```

**Ek line me:** ThreadLocal aur thread pool saath hon, to **hamesha `finally` me `remove()` karo**. Warna pichhli request ki value leak hogi, aur memory leak bhi hoga.

---

## 6. Bonus (grill me aata hai)

`InheritableThreadLocal` me CHILD thread apne parent ki value INHERIT kar leta hai. Normal ThreadLocal me child thread ko parent ki value NAHI dikhti.

Memory leak tab hota hai jab thread pool ka ho aur `remove()` na kiya ho, kyunki tab value pakdi hi rehti hai. Iska ilaaj bhi `remove()` hi hai.

---

## CONNECT: usercrud aur Spring Security

Spring ka `SecurityContextHolder.getContext().getAuthentication()` asal me ThreadLocal me rakha hua logged-in user hai.

Har HTTP request ek thread pe chalti hai, aur wo apna security-context ThreadLocal me rakhti hai. Isliye controller ya service kahin bhi `getAuthentication()` se current user utha leta hai, bina parameter pass kiye. Request khatam hone pe filter chain context ko CLEAR kar deti hai, jo wahi `remove()` wala concept hai. Tera JWT filter SecurityContext set karta hai, aur aage (downstream) ka code usi ko padhta hai. Yahi pattern hai.

[[10_executor_service_thread_pool]] [[05_volatile]]

---

## LIVE DEMO (chala ke dekha, [practical/ThreadLocalDemo.java](practical/ThreadLocalDemo.java))

**DEMO 1 (3 threads):** T-A ko Arpan mila, T-B ko Rahul, aur T-C ko Sita. Matlab har thread alag raha, koi takkar nahi hui (isolation).

**DEMO 2a (`remove()` nahi kiya):** readOnly ko "Arpan" dikha, jo pichhli request ki purani value thi. Ye LEAK live dikh gaya.

**DEMO 2b (`remove()` kiya):** readOnly ko `null` dikha, matlab locker saaf tha.

To teeno baat output me confirm ho gayi: har thread ka alag rehna, pool reuse se leak, aur `remove()` se fix.

## POWER PHRASE

```
"ThreadLocal = per-thread isolated variable — each thread has its own copy behind one reference. Used for
 request-scoped context (userId, traceId, SecurityContext) without threading it through every method.
 Critical with thread pools: always remove() in finally, else a reused thread leaks the previous request's
 value (data leak + memory leak). Spring's SecurityContextHolder and transaction context are ThreadLocal-based."
```
