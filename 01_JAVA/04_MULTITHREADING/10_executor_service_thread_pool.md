# ExecutorService / Thread Pool

**Kahani:** restaurant me har naye customer ke liye ek naya waiter bharti karo, aur khaana khatam hote hi use nikaal do. Ye mehnga bhi hai aur bekaar bhi. Bheed aayi to waiter itne ho jaayenge ki restaurant me jagah hi nahi bachegi (`OutOfMemoryError: unable to create native thread`). Thread pool ka matlab hai: kuch pakke waiter rakho, customer bench pe baithe, aur jo waiter khaali ho wo agle customer ko serve kare. Waiter wahi rehte hain, bas kaam badalta hai.

Movie: core = 2 pakke waiter, bench = 3, max = 4 waiter (`new ThreadPoolExecutor(2, 4, 30, SECONDS, new ArrayBlockingQueue<>(3))`):
```
T1 aaya -> threads=1, bench=0
T2 aaya -> threads=2, bench=0      <- pakke waiter bhar gaye
T3 aaya -> threads=2, bench=1      <- naya waiter nahi, BENCH pehle
T4 aaya -> threads=2, bench=2
T5 aaya -> threads=2, bench=3      <- bench bhar gaya
T6 aaya -> threads=3, bench=3      <- ab temp waiter
T7 aaya -> threads=4, bench=3      <- max
T8 aaya -> REJECT
```

**Khaas baat:**
1. Kram ulta lagta hai: pehle pakke waiter, phir bench, phir temp waiter, phir reject. Wajah ye hai ki bench pe kisi ko bithana lagbhag muft hai, jabki naya thread banana mehnga hai (~1 MB stack + OS ka kaam).
2. `newFixedThreadPool` ki bench ki koi had nahi hoti. Bench kabhi bharti hi nahi, to temp waiter kabhi aata hi nahi, aur laakhon task ka dher memory kha jaata hai, OOM. `newCachedThreadPool` me ulta hai: waiter ki koi had nahi, to threads ki ginti se OOM. Production me apna `ThreadPoolExecutor` banao jisme bench ki had ho aur reject ki policy tay ho. `CallerRunsPolicy` me jo task bhej raha hai wahi khud chala leta hai, isse bhejne wala apne aap dheema ho jaata hai (backpressure).
3. Kitne thread rakhein:
   - CPU wala kaam (calculation, loop): jitne core, utne thread.
   - DB / API ka intezaar wala kaam: core se zyada.
4. `shutdown()` bhool gaye to pool ke thread zinda rehte hain aur JVM band nahi hota. Isliye `finally` me likho.

**Prod wala trap:**
```java
pool.submit(() -> { throw new RuntimeException("payment fail"); });
// get() kabhi nahi bulaya -> log me KUCH nahi aaya
```
`submit()` exception ko `Future` ke andar band kar deta hai. Agar `get()` nahi bulaya, to payment fail ho gaya aur log me ek line bhi nahi aayi. Ye catch me exception nigalne jaisa hi hai, jo tu PR drill me pakad leta hai. Bas yahan wo chhupa hua hai. Result nahi chahiye to `execute()` lagao, wo exception ko stack trace ke saath print karta hai.

**Tera project:** usercrud [`DemoJobs`](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/DemoJobs.java) me `@Async sendEmail` ke peeche yahi pool hai. Log me thread ka naam `task-1` aata hai, http thread nahi. Wahan bhi bench ki had rakhna zaroori hai.

```
Interview: "ExecutorService reuses a pool of threads instead of creating one per task. Internally it's
ThreadPoolExecutor: core threads first, then the queue, then extra threads up to max, then
reject - because queueing is cheap and threads are expensive. newFixedThreadPool has an
unbounded queue, so in production I use a bounded queue with a rejection policy like
CallerRunsPolicy. And submit() hides exceptions inside the Future unless you call get()."
```
