# Producer-Consumer Pattern — Complete Interview Reference

> **Classic multithreading interview question.** Wait/notify ka real-world use case + modern BlockingQueue solution.

---

## WHY — Asli zaroorat

**Scenario:** Tu Amazon order processing system bana raha hai.

```
Order placement (frontend)  →  Order processing (DB + email + inventory)
       │                              │
       ▼                              ▼
   FAST (1000/sec)               SLOW (50/sec)
```

**Problem:** Producer (orders) consumer (processor) se zyada **fast** hai — orders pile up.

**Without sync:**
- Producer: order DB mein daal raha, RAM full ho gayi 
- Consumer: kab order ready? Pata nahi — busy-wait loop spin kar raha CPU 

**Solution:** **Shared buffer** + **coordination** between producer/consumer.

---

## Visual — The Pattern

```
   ┌─────────────┐                                ┌──────────────┐
   │  PRODUCER   │  ──→ [ buffer ] [ buffer ]──→  │  CONSUMER    │
   │  (Order)    │      [ buffer ] [ buffer ]     │  (Processor) │
   │             │      ↑ FIXED CAPACITY ↑        │              │
   └─────────────┘                                └──────────────┘
   
   Rules:
   • Producer adds to buffer  → if FULL, WAIT
   • Consumer takes from buffer → if EMPTY, WAIT
   • Both notify each other when state changes
```

---

## The Problem

Naive implementation:

```java
class Buffer {
    List<Integer> items = new ArrayList<>();
    int CAPACITY = 10;
    
    void produce(int item) {
        if (items.size() < CAPACITY) {
            items.add(item);
        }
        // FULL hua to silently drop 
    }
    
    int consume() {
        if (!items.isEmpty()) {
            return items.remove(0);
        }
        return -1;   // empty hua to sentinel 
    }
}
```

**3 fatal problems:**
1. **Not thread-safe** — race conditions on add/remove
2. **Silent drops** — producer FULL pe item lose karta
3. **Sentinel returns** — consumer EMPTY pe -1 return karta (anti-pattern)

**Real solution:** Producer/Consumer should **BLOCK** (wait) until conditions met.

---

## Approach 1: `synchronized` + `wait/notify` (CLASSIC)

```java
public class Buffer {
    private final List<Integer> items = new ArrayList<>();
    private final int CAPACITY = 10;

    public synchronized void produce(int item) throws InterruptedException {
        while (items.size() == CAPACITY) {   // while NOT if
            wait();                          // release lock + suspend
        }
        items.add(item);
        System.out.println("Produced: " + item);
        notifyAll();                          // wake consumers
    }

    public synchronized int consume() throws InterruptedException {
        while (items.isEmpty()) {
            wait();
        }
        int item = items.remove(0);
        System.out.println("Consumed: " + item);
        notifyAll();                          // wake producers
        return item;
    }
}
```

### Visual — Flow

```
PRODUCER thread                    CONSUMER thread
───────────────                    ───────────────
synchronized (this) {              synchronized (this) {
  while (FULL) wait() ──┐            while (EMPTY) wait() ──┐
                        │                                    │
                        │ Lock released, suspend             │ Lock released
                        │                                    │
  items.add(item)       │            int item = remove(0)    │
  notifyAll() ──────────┴────→ wakes ─────                   │
                                     ←──── wakes ────────────┴
}                                  }
```

### Critical gotchas

#### 1. **`while` NOT `if` for wait condition**
```java
while (items.size() == CAPACITY) wait();   // defensive
if (items.size() == CAPACITY) wait();      // buggy
```
**Why:** **Spurious wakeups** — JVM rare conditions mein `wait()` se return ho jata bina `notify()` ke. `if` use kiya toh stale state mein continue → race condition.

#### 2. **`notifyAll()` not `notify()`**
```java
notifyAll();   // wake ALL waiting threads
notify();      // only wakes ONE — could be wrong type (producer when consumer needed)
```
With single condition variable, **wrong thread wake** ho sakta. Always `notifyAll()`.

#### 3. **`InterruptedException`** — checked exception
Method signature mein declare karna padta. Production mein restore interrupt flag:
```java
catch (InterruptedException e) {
    Thread.currentThread().interrupt();
    throw new RuntimeException(e);
}
```

---

## Approach 2: `BlockingQueue` (PRODUCTION GOLD)

```java
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ArrayBlockingQueue;

BlockingQueue<Integer> queue = new ArrayBlockingQueue<>(10);

// Producer:
queue.put(item);            // blocks if FULL until space available

// Consumer:
int item = queue.take();    // blocks if EMPTY until item available
```

**That's it.** No `synchronized`, no `wait/notify`, no spurious wakeup handling. Library handles everything.

### BlockingQueue variants

| Implementation | Use case |
|---|---|
| **`ArrayBlockingQueue`** | Fixed capacity, array-backed (Producer-Consumer classic) |
| **`LinkedBlockingQueue`** | Optional capacity, linked nodes |
| **`PriorityBlockingQueue`** | Priority-based ordering |
| **`SynchronousQueue`** | Zero capacity — direct handoff |
| **`DelayQueue`** | Items only available after delay (scheduling) |

### Production example

```java
BlockingQueue<Order> orderQueue = new LinkedBlockingQueue<>(1000);

// Producer thread
new Thread(() -> {
    while (true) {
        Order order = receiveOrder();
        orderQueue.put(order);   // blocks if 1000 full
    }
}).start();

// Multiple consumer threads
for (int i = 0; i < 5; i++) {
    new Thread(() -> {
        while (true) {
            try {
                Order order = orderQueue.take();
                processOrder(order);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                break;
            }
        }
    }).start();
}
```

**Production mein 99% cases mein yeh use karte.** `wait/notify` boilerplate is gone.

---

## Approach 3: `ReentrantLock` + `Condition` (GRANULAR)

```java
import java.util.concurrent.locks.*;

public class Buffer {
    private final Lock lock = new ReentrantLock();
    private final Condition notFull  = lock.newCondition();
    private final Condition notEmpty = lock.newCondition();
    
    private final List<Integer> items = new ArrayList<>();
    private final int CAPACITY = 10;

    public void produce(int item) throws InterruptedException {
        lock.lock();
        try {
            while (items.size() == CAPACITY) {
                notFull.await();          // wait on specific condition
            }
            items.add(item);
            notEmpty.signalAll();         // wake ONLY consumers
        } finally {
            lock.unlock();
        }
    }

    public int consume() throws InterruptedException {
        lock.lock();
        try {
            while (items.isEmpty()) {
                notEmpty.await();
            }
            int item = items.remove(0);
            notFull.signalAll();          // wake ONLY producers
            return item;
        } finally {
            lock.unlock();
        }
    }
}
```

### Why this is better than `synchronized + wait/notify`

| Feature | `synchronized + wait/notify` | `ReentrantLock + Condition` |
|---|---|---|
| Lock granularity | Full method | Explicit lock/unlock |
| Multiple wait conditions | Single `wait()`, shared queue | Multiple `Condition`s — `notFull`, `notEmpty` |
| Selective wakeup | `notifyAll()` wakes everyone | `notEmpty.signalAll()` wakes only relevant threads |
| Try-lock / timeout | Not available | `tryLock(timeout)` |
| Interruptibility | Limited | `lockInterruptibly()` |
| Performance | Lower | Higher under contention |

**`signalAll()` vs `notifyAll()`** — same concept, different API. Granular control = better performance.

---

## Comparison Table — Which Approach?

| Approach | Boilerplate | Flexibility | Performance | When to use |
|---|---|---|---|---|
| **synchronized + wait/notify** | High | Low | Medium | Interview classic, simple cases |
| **BlockingQueue** | None | Low (preset) | High | **99% production** |
| **ReentrantLock + Condition** | High | Highest | Highest | Advanced — custom signaling |

**Production rule:** Default to **`BlockingQueue`**. Custom karna ho only — go to `ReentrantLock`.

---

## Real-world Use Cases

### 1. **Kafka / Message Queues**
Producer publishes messages, consumer subscribes — distributed producer-consumer at internet scale.

### 2. **`ExecutorService` Internal**
```java
ExecutorService pool = Executors.newFixedThreadPool(10);
```
Internally uses `BlockingQueue` — submitted tasks queue mein, worker threads consume karte.

### 3. **Logging Frameworks**
Async logging — application logs queue mein, background thread file/network pe write karta. Application non-blocking.

### 4. **Order Processing Systems**
Frontend orders → Kafka topic → background workers process (DB, email, inventory).

### 5. **Spring `@Async`**
Tasks task executor mein submit hote — internal blocking queue.

---

## Interview Answer Templates

### Q: "Producer-Consumer pattern kya hai aur kaise implement karoge?"

**Answer:**
> "Producer-Consumer = pattern where one thread (producer) creates data, another (consumer) processes — both share a fixed-capacity buffer. Coordination chahiye: producer FULL pe wait kare, consumer EMPTY pe wait kare. Production mein **`BlockingQueue`** use karte hain — `put()` blocks if full, `take()` blocks if empty. Library spurious wakeup, `wait/notify` boilerplate sab handle karti."

### Q: "Why `while` not `if` in wait condition?"

**Answer:**
> "**Spurious wakeups** — JVM rare conditions mein `wait()` return ho sakta bina `notify()` ke (OS-level signal). Agar `if` use kiya toh stale state mein continue ho jayega — race condition. `while` hamesha re-check karta condition, defensive coding."

### Q: "`notify()` vs `notifyAll()`?"

**Answer:**
> "`notify()` ek waiting thread wakes up — but **kaunsa, control nahi hai.** Producer-consumer mein agar producer wakeup hua jab actually consumer chahiye tha → again wait → deadlock-like situation. **`notifyAll()`** safer — sab wake up, jo eligible hai aage badhta. Granular control chahiye toh `Condition` variables use karte (`notFull`, `notEmpty`)."

### Q: "Production mein konsa approach use karte ho?"

**Answer:**
> "**`BlockingQueue` 99% cases.** Library tested, performance optimized, no boilerplate. Custom signaling chahiye toh `ReentrantLock + Condition`. `synchronized + wait/notify` sirf interview/learning ke liye — production mein avoid."

---

## TRAP BOX

```
Trap 1: "if (FULL) wait()"
         Spurious wakeup possible — use while

Trap 2: "notify() instead of notifyAll()"
         Wrong thread wake possible — deadlock-like
         
Trap 3: "Buffer pe synchronized nahi laga"
         Race condition — modCount issues, data corruption
         
Trap 4: "ArrayList use kiya BlockingQueue ke bajaye"
         Reinventing wheel + bugs guaranteed
         
Trap 5: "InterruptedException swallow kar diya"
         Restore flag: Thread.currentThread().interrupt()
```

---

## MEMORY HOOKS

```
Producer-Consumer    =  "Fast factory, slow worker — buffer mein items"

while (condition)    =  "Defensive — spurious wakeup proof"
notifyAll()          =  "Sab wake — no wrong-thread issue"

BlockingQueue        =  "Production weapon — put/take blocks"
   put()             =  "Blocks if FULL"
   take()            =  "Blocks if EMPTY"
   offer()/poll()    =  "Non-blocking variants — return false/null"

ReentrantLock+Cond   =  "Granular — multiple wait queues"
   notFull.await()   =  "Wait until space"
   notEmpty.signal() =  "Wake one consumer"
```

---

## FINAL VISUAL — Choosing the right approach

```
                  ┌────────────────────────┐
                  │  Producer-Consumer ka  │
                  │  implementation chahiye│
                  └────────────┬───────────┘
                               │
                  ┌────────────┴────────────┐
                 │                          │
          Production code?          Interview/Learning?
                 │                          │
                 ▼                          ▼
        ┌──────────────────┐      ┌──────────────────────┐
        │  BlockingQueue   │      │ synchronized +       │
        │  (default)       │      │ wait/notify          │
        └──────────────────┘      │ (classic answer)     │
                 │                └──────────────────────┘
                 │
        Custom signaling needed?
                 │
                 ▼
        ┌──────────────────────┐
        │ ReentrantLock +      │
        │ Condition (notFull,  │
        │ notEmpty)            │
        └──────────────────────┘
```
