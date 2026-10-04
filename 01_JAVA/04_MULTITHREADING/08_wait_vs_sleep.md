# wait() vs sleep()

> **V90 — Multithreading: Topic 44**

---

## WHY — Dono Thread Rokte Hain Lekin Alag Tareeke Se

→ Dono thread ko **rokne** ke liye hain, **lekin reason ALAG**
→ **`sleep()`** = main **thak gaya, thodi der rest karunga, lock NAHI chhodunga**
→ **`wait()`** = main **doosre ka wait kar raha hoon — tab tak lock CHHOD deta hoon**

---

## Sabse Bada Fark — Lock Behaviour

| | `wait()` | `sleep()` |
|--|---------|-----------|
| **Class** | `Object` (lock pe call) | `Thread` (thread pe call) |
| **Lock release?** | **YES** — chhod deta | **NO** — pakad ke rakhta |
| **Wake up kaise?** | `notify()` / `notifyAll()` / timeout / `interrupt()` | Time khatam hote hi automatic, ya `interrupt()` (InterruptedException) |
| **Use case** | **Inter-thread communication** (producer-consumer) | **Just delay** (timer, polling) |
| **Kahaan call?** | **`synchronized` block ke andar MUST** | Kahin bhi |

---

## Code

### sleep() — thread thoda ruk
```java
Thread.sleep(2000);    // 2 second pause, lock NAHI chhodta
```

### wait() — lock chhod ke wait
```java
synchronized(this) {
    while (queue.isEmpty()) {
        wait();               // lock chhoda, doosra thread aa sakta
    }
    process(queue.poll());
}
```

### notify() — wait kar rahe ko jagao
```java
synchronized(this) {
    queue.add(item);
    notify();                 // ya notifyAll() — sab ko jagao
}
```

---

## Producer-Consumer Pattern (Real Use Case)

```java
class Buffer {
    Queue<Integer> queue = new LinkedList<>();
    int LIMIT = 5;

    synchronized void produce(int item) throws InterruptedException {
        while (queue.size() == LIMIT) {
            wait();                   // bhar gaya — consumer ka wait
        }
        queue.add(item);
        notify();                     // consumer ko jagao
    }

    synchronized int consume() throws InterruptedException {
        while (queue.isEmpty()) {
            wait();                   // khali — producer ka wait
        }
        int item = queue.poll();
        notify();                     // producer ko jagao
        return item;
    }
}
```

> **`notify()` exactly kya karta — deep flow ke liye [09_notify_deep_dive.md](09_notify_deep_dive.md) padho.**

---

## TRAP 1 — `wait()` Sirf `synchronized` Block Mein

> **`wait()` aur `notify()` ka use SIRF `synchronized` block ke andar.**
> **Bahar likhega → `IllegalMonitorStateException`.**

## TRAP 2 — Naam Mix Mat Karna

> **`wait()` = `Object` class** (lock pe call hota)
> **`sleep()` = `Thread` class** (static method)
> **Interview mein specifically poochhte.**

## TRAP 3 — `wait()` While Loop Mein

> **`wait()` ko `while` mein lapeto, `if` nahi** — "spurious wakeup" bug se bachao.

```java
// GALAT
if (queue.isEmpty()) wait();         // spurious wakeup possible

// SAHI
while (queue.isEmpty()) wait();      // condition recheck after wakeup
```

---

## ★ TRAP — `sleep()` synchronized ke andar = LOCK RELEASE NAHI hota (Bathroom analogy)

**Analogy — office mein 1 bathroom, 3 log wait kar rahe:**
- **`sleep()`** = andar ja ke darwaza lock kiya, neeche so gaya "5 min nap". **Lock nahi chhoda** — bahar wale 3 tere uthne tak wait. *Selfish rest.*
- **`wait()`** = andar gaya, kaam nahi ban raha — "tu pehle use kar, main bahar wait karta". **Lock release** — doosra andar kaam kare. *Generous wait.*

```
sleep() — Lock HOLD                        wait() — Lock RELEASE
Thread A (synchronized, lock liya)         Thread A (synchronized, lock liya)
   Thread.sleep(5000);  ← LOCK HELD           this.wait();  ← lock RELEASED, A wait queue mein
Thread B → same method → BLOCKED           Thread B → ENTERS (lock free)
Thread C → same method → BLOCKED           B kaam karke notify() → A jaagta
Sab A ki poori 5 sec neend ka wait         A lock RE-ACQUIRE karta → phir aage chalta
```

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

**Table mein jo upar nahi:**

| Property | `Thread.sleep()` | `Object.wait()` |
|---|---|---|
| **Throws** | `InterruptedException` | `InterruptedException` + `IllegalMonitorStateException` if not synced |
| **Resume on** | Same line, sleep duration ke baad | Pehle lock RE-ACQUIRE, phir resume |

**Kab kya:**
- `sleep()` → pure delay: API calls ke beech rate limit, polling with backoff, testing me slow operation simulate, UI animation timing
- `wait()` → producer-consumer, conditional execution, threads ke beech signaling

```java
// API rate limiter — sleep
for (Request req : requests) {
    callApi(req);
    Thread.sleep(100);  // 100ms gap
}
```

**wait() synchronized ke bahar — code:**
```java
public void test() {
    wait();   // IllegalMonitorStateException! wait() needs lock context
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

**Q: "synchronized block mein sleep() use kar sakte?"**
> *"Haan, but wo poore duration lock hold rakhega. Doosre threads wait karenge. Inter-thread communication chahiye toh `wait()` use karo — lock release ho jata, dusra thread kaam kar sake."*

**Q: "wait() ko synchronized ke bahar call kare?"**
> *"`IllegalMonitorStateException` aata. wait/notify lock ka monitor concept use karte — caller MUST own the lock."*

```
Trap: "wait() infinite block"        → wait(timeout) overload hai — wait(5000) = max 5 sec
Trap: "sleep(0) = immediate return"  → scheduler ko hint, yield jaisa — sleep(0) ≈ Thread.yield() approximately
Trap: "InterruptedException ignore"  → anti-pattern — catch mein Thread.currentThread().interrupt() se flag restore karo

sleep()  → "Selfish nap"    → Thread class → Lock HELD
wait()   → "Generous wait"  → Object class → Lock RELEASED
```

> **"`sleep()` thread ko pause karta but lock hold rakhta — selfish rest. `wait()` lock release karta + thread block karta — generous wait, inter-thread coordination ke liye. Use `sleep()` for pure delays, `wait()` for thread cooperation."**

---

## POWER PHRASE

> *"`wait()` is an Object class method called inside `synchronized` — it releases the lock and waits for `notify()`. `sleep()` is a Thread class static method that pauses the thread but holds the lock. `wait()` is for inter-thread communication; `sleep()` is just a delay."*

> **Yaad rakh:**
> wait() = lock RELEASE, Object class, synchronized only, notify() se jaago
> sleep() = lock HOLD, Thread class, kahin bhi, time khatam = jaago
> wait() — while loop mein wrap karo
