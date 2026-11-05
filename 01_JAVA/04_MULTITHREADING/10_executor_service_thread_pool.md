# ExecutorService / Thread Pool

## Scene: Diwali ke din cashier

Mausi agar har naye customer ke liye ek naya cashier rakhe aur bill khatam hote hi nikaal de, to ye mehnga bhi hai aur
pagalpan bhi. Bheed badhi to cashier itne ho jaayenge ki dukaan me jagah nahi bachegi. Java me isi ka naam hai
`OutOfMemoryError: unable to create native thread`. Isliye kuch pakke cashier rakho, ek line lagwao, aur zaroorat pade to kuch
temp cashier bulao.

Pool: `new ThreadPoolExecutor(2, 4, 30, SECONDS, new ArrayBlockingQueue<>(3))`, yaani 2 pakke cashier, line me 3 ki jagah, aur
max 4 cashier. Chala ke dekha:

```
customer 1 -> cashier=1, line=0
customer 2 -> cashier=2, line=0      <- pakke cashier bhar gaye
customer 3 -> cashier=2, line=1      <- naya cashier nahi aaya, pehle LINE bhari
customer 4 -> cashier=2, line=2
customer 5 -> cashier=2, line=3      <- line full
customer 6 -> cashier=3, line=3      <- ab temp cashier aaya
customer 7 -> cashier=4, line=3      <- max
customer 8 -> REJECT
```

Kram ulta lagta hai par sahi hai, kyunki line me khada karna lagbhag muft hai, jabki naya thread mehnga hai (har thread ka
~1 MB stack, aur OS ka kaam).

## Dikkat aur hal

`newFixedThreadPool` me line ki koi had nahi hoti. Line kabhi bharti hi nahi, isliye temp cashier kabhi aata hi nahi, laakhon
task ka dher memory kha jaata hai aur OOM ho jaata hai. `newCachedThreadPool` me ulta hai: cashier ki koi had nahi, to threads
badh-badh ke OOM. Isliye production me apna `ThreadPoolExecutor` khud banao, line ki had rakho, aur reject pe kya hoga wo tay
karo. `CallerRunsPolicy` me jo thread task bhej raha hai wahi khud kaam kar leta hai, aur bhejne wala apne aap dheema ho jaata
hai (backpressure). Ye e-commerce wala hi khel hai: utne andar jitni jagah.

Kitne thread: kaam CPU wala ho to jitne core utne, aur DB ya API ke intezaar wala ho to core se zyada. Aur `shutdown()`
`finally` me likho, warna pool ke thread zinda rehte hain aur JVM band nahi hota.

## Prod wala trap

```
submit() ke baad: (koi error nahi dikha)                       <- payment fail hua, log khaali
execute() ke baad: Exception ... RuntimeException: payment fail <- dikh gaya
```

`submit()` exception ko `Future` ke andar band kar deta hai. Agar `get()` kabhi nahi bulaya, to payment fail ho gaya aur log me
ek line bhi nahi aayi. Ye catch me exception nigalne jaisa hi hai, bas chhupa hua. Result nahi chahiye to `execute()` lagao, wo
stack trace print karta hai.

## Tera project

Usercrud ke [`DemoJobs`](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/DemoJobs.java) me
`@Async sendEmail` ke peeche yahi pool chalta hai, isiliye log me thread ka naam `task-1` aata hai, http wala thread nahi. Wahan
bhi line ki had rakhna zaroori hai.

## Interview line

```
A thread pool reuses a fixed set of threads instead of creating one per task.
ThreadPoolExecutor fills core threads first, then the queue, then extra threads up to max,
then rejects, because queueing is cheap and threads are expensive. newFixedThreadPool has an
unbounded queue, so in production I use a bounded queue with a policy like CallerRunsPolicy.
And submit() hides exceptions inside the Future unless someone calls get().
```
