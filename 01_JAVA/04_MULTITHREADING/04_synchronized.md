# synchronized — Race Condition Se Bachao

> **V90 — Multithreading: Topic 41**

---

## STORY — Bank Account Race

→ **2 threads ek saath bank account update** kar rahe the
→ Thread 1 ne **balance = 1000** padha. Thread 2 ne bhi **1000** padha
→ Thread 1 ne **1500 likha**. Thread 2 ne **800 likha**
→ Thread 1 ka update **LOST**. Final balance **800 — galat!** (1300 hona chahiye tha)
→ Ye **race condition** hai
→ **`synchronized`** = ek time pe **sirf ek thread critical section** mein
→ Lock lega, kaam karega, release karega. **Tab doosra aayega**

---

## Race Condition Visualization

```
                MAIN MEMORY (RAM)
                     count = 1000
                    ↑          ↑
                    │          │
        ┌───────────┘          └───────────┐
        ▼                                  ▼
   Thread 1                            Thread 2
   L1 Cache: count = 1000              L1 Cache: count = 1000
   count++ = 1001                      count++ = 1001
   write to RAM                        write to RAM

   MAIN MEMORY = 1001  (galat — 1002 hona chahiye!)
   EK update LOST.
```

---

## Code — synchronized Fix

### Without sync — Race
```java
class BankAccount {
    private int balance = 1000;

    void deposit(int amount) {
        balance = balance + amount;    // race condition
    }
}
```

### Method-level synchronized
```java
class BankAccount {
    private int balance = 1000;

    synchronized void deposit(int amount) {
        balance = balance + amount;    // ek time pe sirf ek thread
    }

    synchronized void withdraw(int amount) {
        balance = balance - amount;
    }
}
```

### Block-level synchronized (BETTER — sirf critical section lock)
```java
void deposit(int amount) {
    synchronized(this) {               // sirf zaroori part lock
        balance += amount;
    }
}
```

---

## TRAP 1 — Method-Level vs Block-Level

> **Method-level `synchronized` = poori method lock = SLOW**
> **Block-level = sirf critical section lock = FASTER**

```java
// Slow — poori method
synchronized void deposit(int amount) { ... }

// Fast — sirf critical part
void deposit(int amount) {
    // koi non-critical kaam
    synchronized(this) {
        balance += amount;            // sirf ye lock
    }
}
```

---

## TRAP 2 — `synchronized` = Atomicity + Visibility

> **`synchronized` 2 cheez fix karta:**
> 1. **Atomicity** — ek time pe ek thread (race condition gone)
> 2. **Visibility** — main memory se padho/likho (CPU cache se nahi)
>
> **`volatile` SIRF visibility fix karta — atomicity nahi.**

---

## POWER PHRASE

> *"`synchronized` ensures only one thread executes the critical section at a time — it acquires a lock on the object, preventing race conditions. Use block-level sync for better performance."*

> **Yaad rakh:**
> `synchronized` = lock acquire/release
> Method-level = poori method lock (slow)
> Block-level = critical section lock (fast)
> `synchronized` = atomicity + visibility dono fix

---

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

---

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

> Idempotency me `orderCounter.incrementAndGet()` = CAS (counter), `putIfAbsent` = bucket-atomic (map).
> Agar poore 3-4 line ek atomic block chahiye hote to `synchronized` lagta.

---

## ★ PROJECT CONNECT — usercrud [RateLimitController](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/RateLimitController.java) (8-Sep)
```java
public synchronized ResponseEntity<String> hit() { ... }   // method-level lock
```
`synchronized` method = ek waqt ek THREAD hi `hit()` me ghus sakta -> rate-limit counter safe (do request ek saath aayein to gadbad nahi). Yahi tera note ka "poora method/block ek thread ke liye lock" LIVE. (single-instance me theek; multi-instance pe Redis-counter chahiye — distributed.)
