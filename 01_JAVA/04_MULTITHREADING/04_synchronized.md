# synchronized — Race Condition Se Bachao

**Kahani:** ek bank account me 1000 rupaye hain. Ravi 500 daalta hai aur ek EMI 200 kaatti hai, dono bilkul ek saath. Dono thread ne balance 1000 padha. Ravi wale ne 1500 likha, EMI wale ne 800. Jo baad me likhta hai wahi bachta hai, to final 800 reh gaya, jabki hona 1300 chahiye tha. Ravi ke 500 gayab ho gaye.

```
Thread 1: padha 1000 ──> +500 ──> likha 1500
Thread 2: padha 1000 ──> -200 ──> likha  800   <- baad me likha, 1500 mit gaya
```
Dikkat ye hai ki padhna, jodna aur likhna teen alag kadam hain, aur beech me doosra thread ghus jaata hai. `synchronized` ka matlab hai ek taala: ek thread andar gaya to baaki bahar line me khade rahenge.

**Khaas baat:**
1. `synchronized` do kaam karta hai: ek waqt pe ek hi thread andar (atomicity), aur jo likha wo baaki threads ko dikhe (visibility). `volatile` sirf doosra kaam karta hai (file 05).
2. Poori method pe lock lagane se achha hai sirf zaroori line pe `synchronized(this) { ... }` lagao, taaki baaki thread kam der ruke.
3. `ReentrantLock` = wahi taala, par extra control ke saath:
   - `tryLock(2, SECONDS)`: lock na mile to haar maan lo, hamesha intezaar mat karo.
   - interrupt ho sakta hai.
   - Keemat: `unlock()` ab teri zimmedari hai. Hamesha `finally` me likho, warna exception aate hi lock hamesha ke liye phas jaata hai.
   ```java
   lock.lock();
   try { count++; } finally { lock.unlock(); }
   ```
4. Do soch hain:
   - `synchronized`: pehle taala lo (pessimistic).
   - CAS / `@Version`: bina taale ke try karo, takkar ho to dobara (optimistic). DB me iska joda hai `SELECT ... FOR UPDATE` vs `WHERE version = ?`.

**Prod wala trap:** Spring ka service bean singleton hota hai, yaani poore app me ek hi object.
```java
@Service
class TransferService {
    public synchronized void transfer(...) { ... }   // poore app ka EK taala
}
```
Ravi ka transfer aur Priya ka transfer ek doosre se bilkul alag hain, phir bhi ek line me lagenge, kyunki taala ek hi object pe hai. Aur 2 server chala diye to ye taala apne server ke bahar kuch nahi rokta, dono server pe alag-alag taala hoga. Isliye banking design me lock DB me lagaya tha (atomic UPDATE / row lock), Java me nahi.

**Tera project:** usercrud [`RateLimitController.hit()`](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/RateLimitController.java) me `synchronized` lagaya hai. Ek server pe ye theek hai. Multi-instance pe Redis counter chahiye, wahi jo rate limiter design me kiya tha.

```
Interview: "synchronized lets only one thread into the critical section and also gives visibility.
I prefer block-level sync to keep the lock short. ReentrantLock when I need tryLock with
timeout or interruptible waits - but unlock must be in finally. In a Spring singleton or
multi-instance setup, a Java lock isn't enough - I push the lock to the DB or Redis."
```


---

## GEHRA HISSA (purani file se, interview me kaam ka)

## ★★ JAB `synchronized` KAAFI NA HO — `ReentrantLock`

> `synchronized` = **ek darwaza, ek chaabi, aur koi control nahi.**

```
        ┌──────────────┐
  T1 ───┤              │   T1 andar gaya, darwaza band
  T2 ───┤ synchronized │   T2, T3 bahar khade — ANANT samay tak
  T3 ───┤    BLOCK     │   na nikal sakte, na pooch sakte, na cancel kar sakte
        └──────────────┘
```

### Teen cheezein jo `synchronized` me ho hi nahi sakti

```
1. WAIT karke HAAR nahi sakte
     T2 khada hai, 5 second baad bhi chaabi nahi mili.
     "chhodo, kuch aur karta hoon" -> ye option HAI HI NAHI.
     Deadlock hua to app hang, aur pata bhi nahi chalega.

2. INTERRUPT nahi kar sakte
     shutdown ho raha hai, T2 lock ka intezaar kar raha hai.
     thread.interrupt() -> kuch nahi hoga, wo wahin khada rahega.

3. lock aur unlock ALAG JAGAH nahi kar sakte
     shuru-khatam usi { } ke andar hona padega.
     "ek method me lo, doosre me chhodo" -> ho hi nahi sakta.
```

### Wahi taala, par ab uske HAATH hain

```java
import java.util.concurrent.locks.ReentrantLock;

class Counter {
    private final ReentrantLock lock = new ReentrantLock();
    private int count = 0;

    public void increment() {
        lock.lock();              // ab ye ek CALL hai, block nahi
        try {
            count++;
        } finally {
            lock.unlock();        // ★ finally me HONA HI CHAHIYE
        }
    }
}
```

> ★★ **SABSE BADA EDGE — wo `finally`:**
> ```
> synchronized   ->  exception aaye to JVM KHUD lock chhod deta hai
> ReentrantLock  ->  TU chhodega. finally na likha + exception aaya
>                    ->  lock HAMESHA ke liye phansa
>                    ->  baaki saare thread wahin mar gaye
> = aazadi mili, par zimmedari bhi.
> ```

### Teen taakatein — code me

```java
// 1. HAAR sakta hai — intezaar hi nahi karega
if (lock.tryLock()) {                        // mila to true, warna TURANT false
    try { count++; } finally { lock.unlock(); }
} else {
    // kuch aur kar lo, ya user ko "abhi busy hai" bol do
}

// 1b. THODA rukega, phir haar maan lega
if (lock.tryLock(2, TimeUnit.SECONDS)) {
    try { count++; } finally { lock.unlock(); }
}

// 2. INTERRUPT ho sakta hai
lock.lockInterruptibly();     // intezaar me hai + interrupt aaya -> exception,
                              // thread bahar nikal aaya

// 3. FAIR taala — line lag jaati hai
ReentrantLock fair = new ReentrantLock(true);
// jo pehle aaya wo pehle jaayega.
// default false = koi line nahi, jiske haath lag gaya. TEZ hai,
// par ek badnaseeb thread baar-baar peeche reh sakta = STARVATION.
```

### "Reentrant" ka matlab (naam ka hissa — poocha jaata hai)

```java
lock.lock();        // hold count = 1
   lock.lock();     // WAHI thread dobara -> count = 2, phansa NAHI
   lock.unlock();   // count = 1
lock.unlock();      // count = 0  -> ab doosre ko milega
```

```
matlab: jiske paas chaabi hai, wo usi kamre me dobara ghus sakta hai.
        (synchronized bhi reentrant hai — wahan bhi yahi hota hai)
★ EDGE: jitni baar lock, UTNI baar unlock. Ek bhi kam = taala khula hi nahi.
```

### Ek line me farak

```
synchronized   =  saada taala. Sasta, surakshit, JVM sambhalta.  90% jagah YAHI sahi.
ReentrantLock  =  wahi taala + tryLock + timeout + interrupt + fairness
                  KEEMAT: unlock teri zimmedari (finally), aur code lamba
```

> **POWER PHRASE:** *"Default `synchronized` hi rakhta hoon — kam code, aur lock release JVM
> guarantee karta hai. `ReentrantLock` tab laata hoon jab mujhe **wait pe haarna** ho —
> `tryLock` se, taaki thread anant intezaar me na phanse — ya lock ko **interrupt** karna ho.
> Iski keemat ye hai ki `unlock` ab meri zimmedari hai, isliye hamesha `finally` me."*

> **AAGE:** `ReentrantLock + Condition` ka asli istemaal ->
> [12_producer_consumer.md](12_producer_consumer.md) (Approach 3).
> Baaki coordination tools (ReadWriteLock · Semaphore · CountDownLatch · CyclicBarrier) ->
> [14_lock_toolkit.md](14_lock_toolkit.md) — wo taale nahi, COORDINATION hain.

## ★ CONNECT — synchronized vs CAS (Pessimistic vs Optimistic)

> Aaj usercrud idempotency me dono dekhe. Ye asli farak yaad rakh (interview follow-up).

```
synchronized -> PESSIMISTIC: taala PEHLE lo, baaki thread BLOCK ho ke wait   (safe, SLOW)
CAS/atomic   -> OPTIMISTIC:  taala NAHI. try karo, koi beat kare -> RETRY      (FAST, lock-free)
```

**Ek family (sab "protect the shared thing", bas tareeka alag):**
```
synchronized          -> taala-pehle (in-memory block)          | pessimistic
ConcurrentHashMap /   -> bucket-level lock/CAS (putIfAbsent atomic) | mostly optimistic
  AtomicInteger CAS   -> compare-swap, no lock (counter)          | optimistic
Optimistic Lock @Version -> DB: UPDATE...WHERE version=? -> 0 rows -> retry | optimistic
Pessimistic DB lock   -> SELECT...FOR UPDATE (row lock pehle)     | pessimistic
```

**KAB KYA:**
- **Simple counter / single value** -> AtomicInteger (CAS) — lock ki zaroorat hi nahi.
- **Multi-line critical block** (2+ steps ek saath atomic chahiye) -> `synchronized`.
- **High-contention** (clash bahut) -> pessimistic (lock-pehle, warna retry-storm).
- **Low-contention** (clash rare) -> optimistic (CAS/@Version) — fast.

> Idempotency me `count.incrementAndGet()` = CAS (counter), `putIfAbsent` = bucket-atomic (map).
> Agar poore 3-4 line ek atomic block chahiye hote to `synchronized` lagta.
