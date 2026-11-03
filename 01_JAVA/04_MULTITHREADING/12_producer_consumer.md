# Producer-Consumer

## Kahani: Amazon ke order

Amazon pe ek second me 1000 order aate hain, par processing (DB + email + inventory) ek second me sirf 50 order ki ho paati hai. To beech me ek fixed size ka bench rakh do. Jab bench bhar jaaye, to order lene wala ruk jaata hai. Jab bench khaali ho, to process karne wala ruk jaata hai. File 08 aur 09 ka `wait/notify` isi kaam ke liye tha.

```
PRODUCER ──> [ ][ ][ ][ ][ ] (fixed jagah) ──> CONSUMER
   bench bhari -> producer ruko                bench khaali -> consumer ruko
```

## Tera practical

[ProducerConsumer](practical/ProducerConsumer.java) me synchronized + wait/notify hai, aur LIMIT 5 hai. Chalane pe 10 item produce hue, 10 consume hue, aur end me "Done" aaya. Kabhi 2 item ek saath bane, kabhi consumer peeche reh gaya, par buffer kabhi 5 se aage nahi gaya.

## Khaas baat

**1. Ise banane ke teen tareeke hain.**

Pehla tareeka `synchronized + wait/notifyAll` hai. Ye interview ka classic jawab hai, aur isme `while` aur `notifyAll` dono zaroori hain (file 08 aur 09).

Doosra tareeka `BlockingQueue` hai, aur production me yahi chalta hai. Isme `put()` bhari queue pe apne aap ruk jaata hai, aur `take()` khaali queue pe apne aap ruk jaata hai. Tujhe koi taala ya `wait` khud nahi likhna padta.

```java
BlockingQueue<Order> q = new ArrayBlockingQueue<>(1000);
q.put(order);          // producer: bhara ho to ruko
Order o = q.take();    // consumer: khaali ho to ruko
```

Teesra tareeka `ReentrantLock + Condition` hai. Isme do alag line (Condition) banti hain, `notFull` aur `notEmpty`. Isse sirf sahi wale thread jaagte hain, aur file 09 wali "galat thread jag gaya" wali dikkat khatam ho jaati hai.

Kaunsa kab chunna hai, ye tasveer me:

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

**2. Saada `Queue` sirf ek thread ke liye hai.** Jaise BFS me ek hi thread use karta hai. Agar kai thread ek queue share kar rahe hain, to `BlockingQueue` chahiye. Ye chala ke dekha tha: 2 thread ne 1 lakh-1 lakh item add kiye. LinkedList me 197868 item aaye (kuch kho gaye), aur BlockingQueue me poore 200000 aaye.

**3. Ye pattern har jagah hai.** `ExecutorService` ka bench wahi hai jo file 10 me dekha. Async logging bhi yahi hai, aur `@Async` bhi. Bade level pe Kafka bhi yahi hai: Producer, topic, aur Consumer group. Notification aur Kafka ka design yahi idea hai, bas kai server pe phaila hua.

## BlockingQueue vs Kafka: idea same hai, par 4 farak hain

**Kahan rehta hai:** BlockingQueue ek JVM ki RAM me rehti hai. Kafka ka data disk pe rehta hai, aur 3 broker pe uski copy hoti hai.

**App crash ho jaaye to:** BlockingQueue ka saara maal chala jaata hai. Kafka me maal bacha rehta hai.

**Padhne ke baad kya hota hai:** BlockingQueue me `take()` kiya to item queue se nikal gaya. Kafka me message wahin rehta hai, bas consumer ka offset aage badh jaata hai.

**Kaun padhta hai:** BlockingQueue me ek item sirf ek hi consumer ko milta hai. Kafka me har consumer group apni alag copy padhta hai.

Seedhi baat: ek JVM ke andar threads ke beech BlockingQueue lagao, aur services ya machines ke beech Kafka. Kafka me maal kaise bachta hai, wo [kafka_concepts hissa 5](../../04_HLD/SYSTEM_DESIGNS/12_message_queue_kafka/kafka_concepts.md) me hai.

## Prod wala trap

```java
q.offer(order);    // bhari queue pe false lautata -> check nahi kiya to order chup-chaap gaya
q.put(order);      // jagah banne tak rukta, order kabhi nahi khota
new LinkedBlockingQueue<>();   // bina size = koi had nahi = file 10 wala OOM, naya naam
```

`offer()` bhari queue pe rukta nahi, bas `false` lauta deta hai. Agar tune wo `false` check nahi kiya, to order chup-chaap kho gaya. `put()` tab tak rukta hai jab tak jagah na ban jaaye, isliye order kabhi nahi khota. Aur bina size ke `LinkedBlockingQueue` banaya, to uski koi had nahi hai. Ye wahi file 10 wala OOM hai, bas naam naya hai.

## Interview line

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

### Flow ki tasveer

Producer bhari bench pe `wait()` karke lock chhod deta hai. Consumer item nikaalta hai aur `notifyAll()` se producer ko jagata hai. Ulta bhi waise hi hota hai.

```
PRODUCER thread                    CONSUMER thread
───────────────                    ───────────────
synchronized (this) {              synchronized (this) {
  while (FULL) wait() ──┐            while (EMPTY) wait() ──┐
                        │                                    │
                        │ Lock chhoda, ruk gaya              │ Lock chhoda
                        │                                    │
  items.add(item)       │            int item = remove(0)    │
  notifyAll() ──────────┴────→ jagata hai ──                 │
                                     ←──── jagata hai ───────┴
}                                  }
```

### Teen zaroori gotchas

#### 1. Wait ki shart ke liye `while` lagao, `if` nahi

```java
while (items.size() == CAPACITY) wait();   // defensive
if (items.size() == CAPACITY) wait();      // buggy
```

Kyun: spurious wakeup ki wajah se. JVM kuch rare halaat me `wait()` se bina `notify()` ke bhi return ho jaata hai. Agar tune `if` lagaya, to thread purani (stale) halat pe hi aage chal dega, aur race condition ho jaayegi.

#### 2. `notify()` nahi, `notifyAll()` lagao

```java
notifyAll();   // wake ALL waiting threads
notify();      // only wakes ONE — could be wrong type (producer when consumer needed)
```

Jab sirf ek hi condition variable ho (sab ek hi wait set me), to `notify()` galat type ke thread ko jaga sakta hai, jaise consumer chahiye tha aur producer jag gaya. Isliye hamesha `notifyAll()` lagao.

#### 3. `InterruptedException` checked exception hai

Ise method signature me declare karna padta hai. Production me catch karo to interrupt ka flag wapas set karo:

```java
catch (InterruptedException e) {
    Thread.currentThread().interrupt();
    throw new RuntimeException(e);
}
```

### BlockingQueue ke variants

- **`ArrayBlockingQueue`:** iski capacity fixed hoti hai aur andar array hota hai. Producer-Consumer ke classic case me yahi.
- **`LinkedBlockingQueue`:** isme capacity dena optional hai, aur andar linked nodes hote hain.
- **`PriorityBlockingQueue`:** isme item priority ke hisaab se nikalte hain.
- **`SynchronousQueue`:** iski capacity zero hai. Producer item seedha consumer ke haath me deta hai (direct handoff).
- **`DelayQueue`:** isme item ek der (delay) ke baad hi nikal sakta hai. Ye scheduling ke kaam aata hai.

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

### Ye `synchronized + wait/notify` se behtar kyun hai

- **Lock kaise lagta aur chhootta hai:** `synchronized` me lock method ya block pe lagta hai, aur JVM use khud chhod deta hai. `ReentrantLock` me tu khud `lock()` aur `unlock()` karta hai, aur `unlock()` ko `finally` me khud likhna padta hai.
- **Kitni wait condition:** `synchronized` me ek hi `wait()` hai, aur sab ek hi line me baithte hain. `ReentrantLock` me kai `Condition` bana sakte ho, jaise `notFull` aur `notEmpty`.
- **Sirf sahi wale ko jagana:** `synchronized` me `notifyAll()` sabko jaga deta hai. `ReentrantLock` me `notEmpty.signalAll()` sirf unhi threads ko jagata hai jinka kaam hai.
- **Try-lock aur timeout:** `synchronized` me ye nahi hai. `ReentrantLock` me `tryLock(timeout)` hai.
- **Interrupt:** `synchronized` me ye limited hai. `ReentrantLock` me `lockInterruptibly()` hai.
- **Speed:** dono lagbhag same hain, kyunki Java 6 ke baad `synchronized` optimize ho gaya. `ReentrantLock` ka faayda speed nahi, CONTROL hai.

`signalAll()` aur `notifyAll()` ka concept same hai, bas API alag hai. Alag Condition hone se sirf sahi wale thread jaagte hain, aur bekaar ke wakeup kam ho jaate hain.

---

## Kaunsa approach kab: teeno ka hisaab

- **`synchronized + wait/notify`:** boilerplate zyada hai, flexibility kam hai, performance medium hai. Ye interview ka classic hai aur simple case ke liye theek hai.
- **`BlockingQueue`:** koi boilerplate nahi, flexibility kam hai (jo bana hai wahi milta hai), performance high hai. 99% production me yahi.
- **`ReentrantLock + Condition`:** boilerplate zyada hai, flexibility sabse zyada hai, performance high hai (kyunki bekaar wakeup kam hote hain). Ye advanced case ke liye hai, jab custom signaling chahiye.

**Production ka niyam:** default me **`BlockingQueue`** lo. Sirf tab `ReentrantLock` pe jao jab kuch custom karna ho.

## TRAP BOX: paanch galtiyan

**Trap 1: `if (FULL) wait()` likh diya.** Spurious wakeup ho sakta hai, isliye `while` lagao.

**Trap 2: `notifyAll()` ki jagah `notify()` lagaya.** Galat thread jag sakta hai, aur sab deadlock jaisa ruk jaata hai.

**Trap 3: buffer pe `synchronized` hi nahi lagaya.** Race condition aayegi, modCount wali dikkat aayegi, aur data kharab ho jaayega.

**Trap 4: `BlockingQueue` ki jagah `ArrayList` use kiya.** Ye bana-banaya pahiya dobara banana hai, aur bugs pakke aayenge.

**Trap 5: `InterruptedException` ko nigal gaye (swallow kar diya).** Flag wapas set karo: `Thread.currentThread().interrupt()`.

## Pattern ki tasveer

```
   ┌─────────────┐                                ┌──────────────┐
   │  PRODUCER   │  ──→ [ buffer ] [ buffer ]──→  │  CONSUMER    │
   │  (Order)    │      [ buffer ] [ buffer ]     │  (Processor) │
   │             │      ↑ FIXED CAPACITY ↑        │              │
   └─────────────┘                                └──────────────┘

   Niyam:
   • Producer buffer me daalta hai  → buffer FULL ho to WAIT
   • Consumer buffer se nikaalta hai → buffer EMPTY ho to WAIT
   • Halat badalte hi dono ek doosre ko notify karte hain
```

### Queue vs BlockingQueue (saada line vs chaukidaar wali line)

Saada `Queue` (`LinkedList` ya `ArrayDeque`) thread-safe NAHI hai. `BlockingQueue` (`ArrayBlockingQueue` ya `LinkedBlockingQueue`) thread-safe HAI, kyunki uske andar lock laga hai.

Khaali queue se nikaalne pe: saada Queue me `poll()` turant `null` lauta deta hai. BlockingQueue me `take()` tab tak RUKTA hai jab tak koi item na aa jaaye.

Bhari queue me daalne pe: saade Queue ki koi limit nahi, wo badhti hi jaati hai. BlockingQueue fixed size ki hai, aur `put()` tab tak RUKTA hai jab tak jagah na ban jaaye.

Kahan lagta hai: saada Queue ek thread ke kaam me (BFS, DSA). BlockingQueue kai thread ke kaam me (producer-consumer, thread pool).

### Production example

```java
BlockingQueue<Order> orderQueue = new LinkedBlockingQueue<>(1000);

// Producer thread
new Thread(() -> {
    while (true) {
        try {
            Order order = receiveOrder();
            orderQueue.put(order);   // blocks if 1000 full (InterruptedException checked hai)
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            break;
        }
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

## Yaad rakhne wali baatein (memory hooks)

Producer-Consumer ka matlab hai: factory tez hai, worker dheema hai, to beech me buffer me items rakhte hain.

`while (condition)` defensive hai, kyunki ye spurious wakeup se bacha leta hai.

`notifyAll()` sabko jaga deta hai, isliye galat thread wali dikkat nahi aati.

`BlockingQueue` production ka hathiyaar hai, kyunki `put()` aur `take()` khud ruk jaate hain. `put()` tab rukta hai jab queue FULL ho. `take()` tab rukta hai jab queue EMPTY ho. `offer()` aur `poll()` inke bina ruke wale roop hain, jo rukne ki jagah `false` ya `null` lauta dete hain.

`ReentrantLock + Condition` granular hai, kyunki isme kai alag wait line ban jaati hain. `notFull.await()` matlab jagah banne tak ruko. `notEmpty.signal()` matlab ek consumer ko jagao.
