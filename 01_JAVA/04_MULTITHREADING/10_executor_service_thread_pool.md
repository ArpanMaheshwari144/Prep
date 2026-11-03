# ExecutorService / Thread Pool

## Kahani: restaurant ke waiter

Soch, restaurant me har naye customer ke liye ek naya waiter bharti karo, aur jaise hi uska khaana khatam ho, waiter ko nikaal do. Ye mehnga bhi hai aur bekaar bhi. Aur jab bheed aayegi, to waiter itne ho jaayenge ki restaurant me jagah hi nahi bachegi. Java me yahi `OutOfMemoryError: unable to create native thread` hai.

Thread pool ka matlab hai: kuch pakke waiter rakh lo. Customer bench pe baithte hain, aur jo waiter khaali hota hai wo agle customer ko serve karta hai. Waiter wahi rehte hain, bas unka kaam badalta rehta hai.

Ab ise ek movie ki tarah dekh. Pool hai `new ThreadPoolExecutor(2, 4, 30, SECONDS, new ArrayBlockingQueue<>(3))`, matlab 2 pakke waiter (core), bench pe 3 ki jagah, aur max 4 waiter:

```
T1 aaya -> threads=1, bench=0
T2 aaya -> threads=2, bench=0      <- pakke waiter bhar gaye
T3 aaya -> threads=2, bench=1      <- naya waiter nahi aaya, pehle BENCH bhari
T4 aaya -> threads=2, bench=2
T5 aaya -> threads=2, bench=3      <- bench bhar gayi
T6 aaya -> threads=3, bench=3      <- ab temp waiter aaya
T7 aaya -> threads=4, bench=3      <- max pe pahunch gaye
T8 aaya -> REJECT
```

## Khaas baat

**1. Kram ulta lagta hai, par sahi hai.** Pehle pakke waiter bharte hain, phir bench bharti hai, phir temp waiter aate hain, aur sab bhar jaaye to reject. Wajah ye hai ki bench pe kisi ko bithana lagbhag muft hai, jabki naya thread banana mehnga hai, kyunki usme ~1 MB stack lagta hai aur OS ko bhi kaam karna padta hai.

**2. `newFixedThreadPool` aur `newCachedThreadPool` dono ka khatra.** `newFixedThreadPool` ki bench ki koi had nahi hoti. Bench kabhi bharti hi nahi, isliye temp waiter kabhi aata hi nahi, aur laakhon task ka dher memory kha jaata hai, aur OOM ho jaata hai. `newCachedThreadPool` me ulta hai: wahan waiter ki koi had nahi, to threads ki ginti badh-badh ke OOM ho jaata hai. Isliye production me apna `ThreadPoolExecutor` khud banao, jisme bench ki had ho aur reject hone pe kya karna hai wo policy tay ho. `CallerRunsPolicy` me jo thread task bhej raha hai, wahi khud us task ko chala leta hai. Isse bhejne wala apne aap dheema ho jaata hai, aur isi ko backpressure kehte hain.

**3. Kitne thread rakhein.** Agar kaam CPU wala hai (calculation, loop), to jitne core hain utne thread rakho. Agar kaam DB ya API ke intezaar wala hai, to core se zyada thread rakho.

**4. `shutdown()` bhoolna mat.** Agar `shutdown()` bhool gaye, to pool ke thread zinda rehte hain aur JVM band hi nahi hota. Isliye ise `finally` me likho.

## Prod wala trap: `submit()` exception ko chhupa leta hai

```java
pool.submit(() -> { throw new RuntimeException("payment fail"); });
// get() kabhi nahi bulaya -> log me KUCH nahi aaya
```

`submit()` exception ko `Future` ke andar band kar deta hai. Agar tune `get()` kabhi nahi bulaya, to payment fail ho gaya aur log me ek line bhi nahi aayi. Ye catch me exception nigalne jaisa hi hai, jo tu PR drill me pakad leta hai, bas yahan wo chhupa hua hai. Agar tujhe result nahi chahiye, to `execute()` lagao, kyunki wo exception ko stack trace ke saath print karta hai.

## Tera project

usercrud ke [`DemoJobs`](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/DemoJobs.java) me `@Async sendEmail` ke peeche yahi pool chal raha hai. Isliye log me thread ka naam `task-1` aata hai, http wala thread nahi. Wahan bhi bench ki had rakhna zaroori hai.

## Interview line

```
Interview: "ExecutorService reuses a pool of threads instead of creating one per task. Internally it's
ThreadPoolExecutor: core threads first, then the queue, then extra threads up to max, then
reject - because queueing is cheap and threads are expensive. newFixedThreadPool has an
unbounded queue, so in production I use a bounded queue with a rejection policy like
CallerRunsPolicy. And submit() hides exceptions inside the Future unless you call get()."
```


---

## GEHRA HISSA (purani file se, interview me kaam ka)

## Code: 4 common pools

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

### Task kaise submit karte hain

`Runnable` kuch return nahi karta. `Callable` value return karta hai, aur wo value `Future` se milti hai. `future.get()` tab tak rukta hai jab tak result na aa jaaye.

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

## Pool types ka farak

- **`FixedThreadPool(N)`:** isme N thread hamesha rehte hain. Ye tab theek hai jab load predictable ho. Par iski queue ki koi had nahi hoti, aur upar khaas baat 2 me dekha ki isse OOM ho sakta hai.
- **`CachedThreadPool`:** isme thread demand pe bante hain, aur unki koi had nahi hoti. Ye chhote async kaam ke liye hai, par bheed me isse OOM ka khatra hai.
- **`SingleThreadExecutor`:** isme sirf 1 thread hota hai, to kaam FIFO kram me ek ke baad ek chalte hain. Ye tab lagao jab order zaroori ho.
- **`ScheduledThreadPool`:** isme N thread hote hain, aur ye cron job ya der se chalne wale (delayed) kaam ke liye hai.

## TRAP 3: `submit()` vs `execute()`

`execute(Runnable)` kuch return nahi karta. Agar task me exception aaya, to wo thread ke UncaughtExceptionHandler tak jaata hai aur stack trace print ho jaata hai. Wo thread mar jaata hai, aur pool uski jagah naya thread bana leta hai.

`submit(Runnable/Callable)` ek `Future` deta hai. Agar exception aaya, to wo Future ke andar band ho jaata hai, aur `get()` bulane pe `ExecutionException` ke andar milta hai.

Asli trap ye hai: tune `submit()` kiya aur `get()` kabhi nahi bulaya. Tab exception chup-chaap gayab ho jaata hai, aur log me kuch nahi aata.

## Andar kya hai: ThreadPoolExecutor ke 7 param aur task ka flow (8-Sep)

`Executors.newFixedThreadPool(5)` andar se bas ek `new ThreadPoolExecutor(...)` hi hai. Uske 7 knob milke pool ka poora behaviour tay karte hain. Restaurant wali misaal se:

- **`corePoolSize`:** pakke waiter. Ye hamesha payroll pe rehte hain, kaam ho ya na ho.
- **`maximumPoolSize`:** max waiter. Pakke aur temp mila ke isse zyada waiter nahi ho sakte.
- **`keepAliveTime`:** temp waiter ko kitni der khaali baithne dena hai, uske baad use nikaal dete hain.
- **`workQueue`:** waiting bench. Customer yahan baithte hain, aur waiter free hote hi unhe serve karta hai.
- **`threadFactory`:** waiter "banane" ka tareeka, jaise thread ka naam ya priority set karna.
- **`rejectedExecutionHandler`:** jab bench bhi full ho aur waiter bhi max ho jaayein, tab customer ko kya bolna hai.
- **`timeUnit`:** `keepAliveTime` ki unit, jaise second ya millisecond.

### Rejection policies: bench full aur waiter max ke baad kya hota hai

- **`AbortPolicy`** (ye default hai): `RejectedExecutionException` phenk deta hai.
- **`CallerRunsPolicy`:** jo thread task submit kar raha hai, wahi thread us task ko khud chala deta hai. Isse bhejne wala apne aap dheema ho jaata hai, yahi backpressure hai.
- **`DiscardPolicy`:** task ko chup-chaap phenk deta hai, koi error nahi aata.
- **`DiscardOldestPolicy`:** queue ka sabse purana task hata deta hai aur naya task daal deta hai.

### Sizing: kitne thread rakhein

CPU-bound kaam (calculation, loop) me lagbhag utne thread rakho jitne core hain. Isse zyada thread rakhne se bas context-switch me waqt barbaad hota hai.

IO-bound kaam (DB ya API ka intezaar) me core se zyada thread rakho. Kyunki thread waise bhi intezaar me baitha hai, to us beech koi doosra thread kaam kar le.

## Visualization: pool jisme thread dobara use hote hain

```
              ExecutorService — Thread Pool Visualization

╔════════════════════════════════════════════════════════════╗
║ Bina Pool ke — har request pe new Thread                   ║
╚════════════════════════════════════════════════════════════╝

  Request 1  →  new Thread()  →  task done  →  thread DESTROY
  Request 2  →  new Thread()  →  task done  →  thread DESTROY
  Request 3  →  new Thread()  →  task done  →  thread DESTROY
   ...
  hazaaron reqs  →  utne threads create + destroy  →  native-thread OOM / context-switch me CPU barbaad


╔════════════════════════════════════════════════════════════╗
║ Pool ke saath — 5 threads, REUSE                           ║
╚════════════════════════════════════════════════════════════╝

  ExecutorService pool = Executors.newFixedThreadPool(5);

       Pool (heap)
       ┌─────────────────────────────────────────┐
       │  ┌────┐ ┌────┐ ┌────┐ ┌────┐ ┌────┐    │
       │  │ T1 │ │ T2 │ │ T3 │ │ T4 │ │ T5 │    │   ← 5 threads jo dobara use hote hain
       │  └────┘ └────┘ └────┘ └────┘ └────┘    │
       └─────────────────────────────────────────┘
                          ▲
                          │ submit()
                          │
       Task Queue
       ┌─────────────────────────────────────────┐
       │  task1  task2  task3  ... task1000      │
       └─────────────────────────────────────────┘

  Task aaya → free thread mila → use diya → kaam done → thread WAPAS pool mein
                                                         (DESTROY nahi, REUSE)


╔════════════════════════════════════════════════════════════╗
║ Pool Types                                                 ║
╚════════════════════════════════════════════════════════════╝

  FixedThreadPool(N)     ┌─────────┐
                         │ N fixed │  ← predictable load (production)
                         └─────────┘

  CachedThreadPool       ┌─────────────────┐
                         │ bina had badhta │  ← bheed me OOM ka khatra!
                         └─────────────────┘

  SingleThreadExecutor   ┌─────┐
                         │  1  │  ← FIFO, ek ke baad ek
                         └─────┘

  ScheduledThreadPool    ┌──────────┐
                         │ N + cron │  ← delayed / periodic tasks
                         └──────────┘


╔════════════════════════════════════════════════════════════╗
║ Lifecycle                                                  ║
╚════════════════════════════════════════════════════════════╝

  pool.submit(task)           ──►  task chala
  pool.shutdown()             ──►  naya kaam nahi lega, jo chal raha wo poora karega
  pool.shutdownNow()          ──►  chalte tasks ko interrupt() (best effort, task ignore kare to chalta rahega) + queue ke pending tasks ki list lautata
  pool.awaitTermination(...)  ──►  tab tak ruko jab tak sab khatam na ho
```

### Queue PEHLE kyun, aur naya thread BAAD me kyun? (asli samajh)

Bench pe kisi ko bithane ka matlab hai bas ek list me ek reference daal dena. Ye lagbhag FREE hai.

Naya thread banane ke liye OS se thread maangna padta hai, jisme ~1 MB stack lagta hai aur context-switch ka kharcha bhi aata hai. Ye MEHNGA hai. Isliye pool pehle bench bharta hai, aur naya thread tabhi banata hai jab bench bhar jaaye.

### OOM trap: jab bench ki koi had hi na ho (unbounded queue)

`newFixedThreadPool` ki queue UNBOUNDED hoti hai. Andar `LinkedBlockingQueue` hai, aur uski koi limit nahi.

To kya hota hai: T3, T4, aur T10000 tak sab bench pe baithte jaate hain, kyunki bench kabhi FULL hoti hi nahi. Bench full nahi hui, isliye temp thread kabhi bharti hi nahi hote. Sirf core threads kaam karte rehte hain, aur bench pe laakhon task ka dher lag jaata hai, jisse memory phat jaati hai aur OOM aata hai. Aur `maximumPoolSize` bekaar pada rehta hai, kyunki uski kabhi zaroorat hi nahi padi.
