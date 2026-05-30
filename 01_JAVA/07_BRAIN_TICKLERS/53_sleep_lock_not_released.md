# `sleep()` Mein Lock Release Nahi Hota

> **V90 — Brain Ticklers: Topic 53 (Multithreading Classic)**

---

## STORY — Bathroom Lock Analogy

> Imagine **office mein 1 bathroom** + **3 employees** wait kar rahe.
>
> **`sleep()` analogy:**
> Tu bathroom mein ja ke **darwaza lock kiya, neeche so gaya** — *"5 min nap lunga"*.
> **Lock nahi chhoda.** Baahar wale 3 wait karte rahenge tere uthne tak.
> *"Selfish rest"* — apne aap mein lock leke so raha.
>
> **`wait()` analogy:**
> Tu bathroom mein gaya, dekha kuch nahi mil raha — *"Tu pehle use kar, mai bahar wait karta"*.
> **Lock release kar diya.** Doosra andar ja ke kaam kare, phir tu use karega.
> *"Generous wait"* — lock chhoda taaki dusra kaam kar sake.

---

## VISUAL — Bathroom Lock Behavior

### `sleep()` — Lock Hold Karta

```
   Thread A (in synchronized method)
        │
        │ Took the LOCK
        │
        ▼
   ┌─────────────────────┐
   │ synchronized {      │
   │     Thread.sleep(5000);    │  ← 5 sec sleep, LOCK HELD
   │     // doing work    │
   │ }                   │
   └─────────────────────┘

   Meanwhile:
   Thread B → tries to enter same method → BLOCKED
                                            (lock still with A)
   Thread C → tries to enter same method → BLOCKED

   All wait for A's full 5 sec sleep to complete
```

### `wait()` — Lock Release Karta

```
   Thread A (in synchronized method)
        │
        │ Took the LOCK
        │
        ▼
   ┌─────────────────────┐
   │ synchronized {      │
   │     this.wait();    │  ← Lock RELEASED, A goes to wait queue
   │     // (after notify)│
   │ }                   │
   └─────────────────────┘

   Meanwhile:
   Thread B → tries to enter → ENTERS (lock free)
   Thread B does work, calls notify() → A wakes up
   Thread A → re-acquires lock → continues

   Cooperation between threads
```

---

## CODE EXAMPLE

```java
public class Bathroom {

    // sleep — lock held, others blocked
    public synchronized void useBathroomSleep() throws InterruptedException {
        System.out.println("In bathroom, taking 5 sec nap");
        Thread.sleep(5000);                  // ← lock HELD during nap
        System.out.println("Done");
    }

    // wait — lock released, others can enter
    public synchronized void useBathroomWait() throws InterruptedException {
        System.out.println("Waiting for signal");
        wait();                              // ← lock RELEASED
        System.out.println("Got signal, proceeding");
    }
}
```

---

## sleep() vs wait() — FULL COMPARE

| Property | `Thread.sleep()` | `Object.wait()` |
|---|---|---|
| **Class** | `Thread` (static method) | `Object` (instance method) |
| **Lock release?** | NO — holds lock | YES — releases lock |
| **Synchronized needed?** | Not required | MUST be in synchronized block |
| **Wakeup mechanism** | Time elapsed | `notify()` / `notifyAll()` / interrupt |
| **Use case** | Pure delay (rate limit, polling) | Inter-thread coordination |
| **Throws** | `InterruptedException` | `InterruptedException` + `IllegalMonitorStateException` if not synced |
| **Resume on** | Same code line after sleep duration | Re-acquires lock, then resumes |

---

## USE CASE GUIDE

### Use `sleep()` when:
- Pure delay needed (e.g., rate limiting between API calls)
- Polling with backoff
- Testing — simulate slow operations
- Animation timing in UI

```java
// API rate limiter
for (Request req : requests) {
    callApi(req);
    Thread.sleep(100);  // 100ms gap
}
```

### Use `wait()` when:
- Producer-Consumer pattern
- Conditional thread execution
- Signaling between threads

```java
// Wait until queue has data
synchronized (queue) {
    while (queue.isEmpty()) wait();
    return queue.remove(0);
}
```

---

## INTERVIEW TRAP — Calling wait() outside synchronized

```java
public void test() {
    wait();   // IllegalMonitorStateException!
              // wait() needs lock context
}

// Correct
public synchronized void test() throws InterruptedException {
    wait();   // OK — lock owner can wait
}

// Or with explicit object
public void test() throws InterruptedException {
    Object lock = new Object();
    synchronized (lock) {
        lock.wait();  // OK
    }
}
```

---

## INTERVIEW TALKING POINT

**Q: "`sleep()` aur `wait()` mein difference?"**

> *"Bathroom analogy se yaad karta hu. `sleep()` = bathroom mein lock leke so jana — selfish rest, lock nahi chhodta. Baahar wale wait karenge timer khatam hone tak. `wait()` = lock chhod ke bahar wait karna — generous, doosra thread andar ja sake. Class-wise: `sleep()` Thread ka static method, `wait()` Object ka instance method synchronized mein call hota. Resume mechanism alag — sleep timer-based, wait notify-based."*

**Q: "synchronized block mein sleep() use kar sakte?"**

> *"Haan, but wo entire block ke duration lock hold rakhega. Doosre threads wait karenge. Agar inter-thread communication chahiye toh `wait()` use karo — lock release ho jata, dusra thread kaam kar sake."*

**Q: "wait() ko synchronized ke bahar call kare?"**

> *"`IllegalMonitorStateException` aata. wait/notify lock ka monitor concept use karte — caller MUST own the lock. synchronized block/method mein hi valid call."*

---

## POWER PHRASE

> **"`sleep()` thread ko pause karta but lock hold rakhta — selfish rest. `wait()` lock release karta + thread block karta — generous wait, inter-thread coordination ke liye. Use `sleep()` for pure delays, `wait()` for thread cooperation."**

---

## MEMORY HOOK

```
sleep()    → "Selfish nap"     → Thread class   → Lock HELD
wait()     → "Generous wait"   → Object class   → Lock RELEASED

Bathroom analogy:
   sleep() = lock leke neeche so jaa (no one else can enter)
   wait()  = bahar nikal ke wait kar (others can enter)

Synchronized requirement:
   sleep() — anywhere
   wait()  — MUST be inside synchronized
```

---

## TRAP BOX

```
Trap 1: "sleep() mein lock release ho jata"
         NAHI — lock holds throughout sleep duration
         wait() releases lock

Trap 2: "wait() ko anywhere call kar sakte"
         IllegalMonitorStateException
         MUST inside synchronized block

Trap 3: "wait() infinite block"
         wait(timeout) overload available
         wait(5000) — max 5 sec wait

Trap 4: "sleep(0) = immediate return"
         Hint to scheduler — yield-like behavior
         sleep(0) ≈ Thread.yield() approximately

Trap 5: "InterruptedException ignore karu"
         Anti-pattern — restore interrupt flag
         Thread.currentThread().interrupt() in catch
```
