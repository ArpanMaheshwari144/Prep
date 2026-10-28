# Producer-Consumer

**Kahani:** Amazon pe ek second me 1000 order aate hain, par processing (DB + email + inventory) ek second me sirf 50 ho paati hai. Beech me ek fixed size ka bench rakh do. Bench bhar jaaye to order lene wala rukta hai, bench khaali ho to process karne wala rukta hai. File 08 aur 09 ka `wait/notify` isi ke liye tha.

```
PRODUCER ──> [ ][ ][ ][ ][ ] (fixed jagah) ──> CONSUMER
   bhara -> ruko                                  khaali -> ruko
```

**Tera practical:** [ProducerConsumer](practical/ProducerConsumer.java) (synchronized + wait/notify, LIMIT 5): 10 produce, 10 consume, "Done". Kabhi 2 item ek saath bane, kabhi consumer peeche, par buffer 5 se aage nahi gaya.

**Khaas baat:**
1. Teen tareeke hain:
   - `synchronized + wait/notifyAll`: interview ka classic. Isme `while` aur `notifyAll` zaroori hain (file 08 aur 09).
   - `BlockingQueue`: production me yahi. `put()` bhari queue pe rukta hai, `take()` khaali queue pe rukta hai. Koi taala ya `wait` khud nahi likhna padta.
     ```java
     BlockingQueue<Order> q = new ArrayBlockingQueue<>(1000);
     q.put(order);          // producer: bhara ho to ruko
     Order o = q.take();    // consumer: khaali ho to ruko
     ```
   - `ReentrantLock + Condition`: do alag bench-line, `notFull` aur `notEmpty`. Isse sirf sahi waale thread jaagte hain, file 09 wali galat-thread-jagne ki dikkat khatam.

   ```
                     ┌────────────────────────┐
                     │  Producer-Consumer ka  │
                     │  implementation chahiye│
                     └────────────┬───────────┘
                    ┌─────────────┴─────────────┐
             Production code?           Interview / Learning?
                    │                           │
                    ▼                           ▼
           ┌──────────────────┐       ┌──────────────────────┐
           │  BlockingQueue   │       │ synchronized +       │
           │  (default)       │       │ wait/notify          │
           └────────┬─────────┘       │ (classic answer)     │
                    │                 └──────────────────────┘
           Custom signaling chahiye?
                    ▼
           ┌──────────────────────┐
           │ ReentrantLock +      │
           │ Condition (notFull,  │
           │ notEmpty)            │
           └──────────────────────┘
   ```
2. Saada `Queue` ek thread ke liye hai (BFS jaisa). Kai thread share karein to `BlockingQueue`. Chala ke dekha tha: 2 thread x 1 lakh add, LinkedList pe 197868 aaye, BlockingQueue pe 200000.
3. Ye pattern har jagah hai:
   - `ExecutorService` ka bench file 10 wala hi hai.
   - Async logging.
   - `@Async`.
   - Bade level pe Kafka: Producer, topic, Consumer group. Notification aur Kafka design yahi hai, bas kai server pe phaila hua.

**BlockingQueue vs Kafka (idea same, 4 farak):**
```
                    BlockingQueue                Kafka
kahan rehta         ek JVM ki RAM                disk pe, 3 broker pe copy
app crash           saara maal gaya              maal bacha rehta
padhne ke baad      take() = nikal gaya          message rehta, sirf offset aage badhta
kaun padhe          ek item ek hi consumer       har consumer group apni copy padhe
```
Ek JVM ke andar threads ke beech BlockingQueue, services / machines ke beech Kafka. Maal kaise bachta = [kafka_concepts hissa 5](../../04_HLD/SYSTEM_DESIGNS/12_message_queue_kafka/kafka_concepts.md).

**Prod wala trap:**
```java
q.offer(order);    // bhari queue pe false lautata -> check nahi kiya to order chup-chaap gaya
q.put(order);      // jagah banne tak rukta, order kabhi nahi khota
new LinkedBlockingQueue<>();   // bina size = koi had nahi = file 10 wala OOM, naya naam
```

```
Interview: "Producer-consumer decouples a fast producer from a slow consumer with a bounded buffer -
producer waits when full, consumer waits when empty. Classic version is synchronized with
wait/notifyAll in a while loop. In production I use a bounded BlockingQueue - put and take
block for me - or ReentrantLock with notFull / notEmpty conditions for targeted wakeups.
At system scale the same idea is Kafka."
```


---

## GEHRA HISSA (purani file se, interview me kaam ka)

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

### BlockingQueue variants

| Implementation | Use case |
|---|---|
| **`ArrayBlockingQueue`** | Fixed capacity, array-backed (Producer-Consumer classic) |
| **`LinkedBlockingQueue`** | Optional capacity, linked nodes |
| **`PriorityBlockingQueue`** | Priority-based ordering |
| **`SynchronousQueue`** | Zero capacity — direct handoff |
| **`DelayQueue`** | Items only available after delay (scheduling) |

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
| Lock granularity | method ya block (JVM khud chhodta) | Explicit lock/unlock (finally me khud chhodo) |
| Multiple wait conditions | Single `wait()`, shared queue | Multiple `Condition`s — `notFull`, `notEmpty` |
| Selective wakeup | `notifyAll()` wakes everyone | `notEmpty.signalAll()` wakes only relevant threads |
| Try-lock / timeout | Not available | `tryLock(timeout)` |
| Interruptibility | Limited | `lockInterruptibly()` |
| Performance | Lagbhag same (Java 6+ me synchronized optimize ho gaya) | Lagbhag same — faayda speed nahi, CONTROL hai |

**`signalAll()` vs `notifyAll()`** — same concept, different API. Alag Condition = sirf sahi wale jaagte, bekaar wakeup kam.

---

## Comparison Table — Which Approach?

| Approach | Boilerplate | Flexibility | Performance | When to use |
|---|---|---|---|---|
| **synchronized + wait/notify** | High | Low | Medium | Interview classic, simple cases |
| **BlockingQueue** | None | Low (preset) | High | **99% production** |
| **ReentrantLock + Condition** | High | Highest | High (bekaar wakeup kam) | Advanced — custom signaling |

**Production rule:** Default to **`BlockingQueue`**. Custom karna ho only — go to `ReentrantLock`.

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
