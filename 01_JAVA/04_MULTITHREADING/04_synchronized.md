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
