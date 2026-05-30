# Producer-Consumer Pattern — Visual Revision

---

## 1 Concept (Restaurant Analogy)

```
Imagine restaurant:

   CHEF (Producer)
        │
        ▼
   COUNTER (Buffer/Queue)
        │
        ▼
   WAITER (Consumer)
        │
        ▼
   Customer
```

```
PROBLEM:
   Chef speed ≠ Waiter speed
   Chef tez = counter full
   Waiter tez = counter empty

   Solution: COUNTER (buffer)
   = Decouples speeds
```

---

## 2 The Pattern

```
   PRODUCER  ──put──►  QUEUE (buffer)  ──take──►  CONSUMER

   • Producer adds items
   • Consumer removes items
   • Queue bounded (full = producer waits)
   • Queue empty = consumer waits
```

---

## 3 Two Implementations

### Old Way (wait/notify) — Covered Earlier
```java
class Buffer {
    Queue<Integer> q = new LinkedList<>();
    int CAPACITY = 10;

    synchronized void put(int item) throws InterruptedException {
        while (q.size() == CAPACITY) wait();
        q.add(item);
        notifyAll();
    }

    synchronized int take() throws InterruptedException {
        while (q.isEmpty()) wait();
        int item = q.poll();
        notifyAll();
        return item;
    }
}

// = Boilerplate, error-prone
```

### Modern Way (BlockingQueue) — RECOMMENDED
```java
BlockingQueue<Integer> q = new ArrayBlockingQueue<>(10);

// Producer:
q.put(item);     // blocks if full, auto-wait

// Consumer:
int item = q.take();   // blocks if empty, auto-wait

// = NO wait/notify boilerplate
// = Thread-safe by default
```

---

## 4 BlockingQueue Types

```
┌────────────────────────┬─────────────────────────┐
│  Type                  │  Behavior                │
├────────────────────────┼─────────────────────────┤
│ ArrayBlockingQueue     │ Fixed capacity, FIFO    │
│ LinkedBlockingQueue    │ Unbounded (or bounded)  │
│ PriorityBlockingQueue  │ Priority-based          │
│ SynchronousQueue       │ Hand-off (no storage)   │
│ DelayQueue             │ Items released by delay │
└────────────────────────┴─────────────────────────┘
```

---

## 5 Full Working Example

```java
public class ProducerConsumerDemo {

    static BlockingQueue<Integer> q =
        new ArrayBlockingQueue<>(5);

    public static void main(String[] args) {

        // Producer thread
        new Thread(() -> {
            for (int i = 1; i <= 10; i++) {
                try {
                    q.put(i);   // blocks if full
                    System.out.println("Produced: " + i);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        }).start();

        // Consumer thread
        new Thread(() -> {
            for (int i = 1; i <= 10; i++) {
                try {
                    int item = q.take();   // blocks if empty
                    System.out.println("Consumed: " + item);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        }).start();
    }
}
```

```
Output (interleaved):
   Produced: 1
   Produced: 2
   Consumed: 1
   Produced: 3
   Consumed: 2
   ...
```

---

## 6 Visual — Flow

```
   PRODUCER             QUEUE             CONSUMER
   ────────             ─────             ────────

   produce 1            [1]
                                          take → 1
                        []
   produce 2            [2]
   produce 3            [2,3]
   produce 4            [2,3,4]
                                          take → 2
                        [3,4]
   produce 5,6,7,8,9    [3,4,5,6,7] FULL
   produce 10           waits...
                                          take → 3
                        [4,5,6,7]
                        [4,5,6,7,10]

   = Auto sync via BlockingQueue
```

---

## 7 Multiple Producers + Consumers

```java
BlockingQueue<Task> q = new LinkedBlockingQueue<>();

// 3 producers
for (int i = 0; i < 3; i++) {
    new Thread(() -> {
        while (true) {
            q.put(generateTask());
        }
    }).start();
}

// 5 consumers
for (int i = 0; i < 5; i++) {
    new Thread(() -> {
        while (true) {
            Task t = q.take();
            t.execute();
        }
    }).start();
}
```

```
All threads coordinate via queue
   = Thread-safe automatically
```

---

## 8 Real Production Use Cases

```
1. EMAIL SERVICE
   Web request → put email task in queue
   Background workers → take → send email
   = Decouples request from email sending

2. LOG PROCESSING
   App threads → put log to queue
   Log writer thread → take → write to file
   = Doesn't block app threads

3. EVENT PROCESSING
   Producer: events from Kafka
   Consumer: process + save to DB

4. JOB SCHEDULER
   Producer: scheduler creates jobs
   Consumer: worker threads execute

5. ORDER PROCESSING
   E-commerce checkout:
      Producer: place order → queue
      Consumer: payment + shipping
```

---

## 9 Why Important?

```
Decoupling:
   Producer doesn't care about consumer speed
   Consumer doesn't care about producer rate
   Queue handles backpressure

Scaling:
   Add more producers OR more consumers
   Independently scalable

Resilience:
   Consumer crash? Queue keeps items
   Producer crash? Consumer continues with existing items
```

---

## BlockingQueue Methods

```
┌─────────────┬──────────────────┬─────────────────┐
│  Action     │ Method            │ Behavior        │
├─────────────┼──────────────────┼─────────────────┤
│ Insert      │ add(e)            │ throws if full  │
│             │ offer(e)          │ returns false   │
│             │ put(e)            │ BLOCKS          │
├─────────────┼──────────────────┼─────────────────┤
│ Remove      │ remove()          │ throws if empty │
│             │ poll()            │ returns null    │
│             │ take()            │ BLOCKS          │
└─────────────┴──────────────────┴─────────────────┘
```

```
USE:
   put + take = blocking (standard pattern)
   offer + poll = non-blocking + timeout option
```

---

## Memory Hooks

```
Producer-Consumer = chef + counter + waiter

OLD way: wait/notify + manual queue
   = Boilerplate, error-prone

NEW way: BlockingQueue
   put()  — blocks if full
   take() — blocks if empty
   = Auto thread-safe

Types:
   ArrayBlockingQueue   — fixed FIFO
   LinkedBlockingQueue  — unbounded
   PriorityBlockingQueue — priority

USE CASES:
   • Email queue
   • Log processing
   • Job scheduler
   • Event pipeline
```

[← Back to JAVA](../../)
