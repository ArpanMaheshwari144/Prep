# wait/notify + Deadlock — Visual Revision

---

## 1 wait/notify (Thread Coordination)

```
PROBLEM:
   Thread A producing items
   Thread B consuming items

   B kaise wait kare jab tak A item produce na kare?
   = Need coordination mechanism
```

---

## 2 Restaurant Analogy

```
Chef (producer)               Waiter (consumer)
   │                              │
   │ "Order ready!"                │
   │ ───── notify ─────►          │
   │                              │ "Pick up order"
   │                              │
   │ (cooking next order)         │ "Order taken"
   │                              │ ───── wait ─────
   │                              │     for next order
```

```
   wait()   = "I'll wait till you notify me"
   notify() = "Item ready, wake one waiter"
```

---

## 3 wait/notify/notifyAll Methods

```java
synchronized (sharedObj) {

    while (queueEmpty()) {
        sharedObj.wait();           // release lock + sleep
    }

    // process item
    sharedObj.notify();             // wake ONE waiter
    sharedObj.notifyAll();          // wake ALL waiters
}
```

```
KEY RULES:
   MUST be inside synchronized block
   wait() releases the lock while sleeping
   notify() doesn't release lock — caller must exit synchronized
   Use while loop, not if (spurious wakeups possible)
```

---

## 4 Producer-Consumer Example

```java
class Buffer {
    private Queue<Integer> q = new LinkedList<>();
    private final int CAPACITY = 10;

    public synchronized void produce(int item)
            throws InterruptedException {
        while (q.size() == CAPACITY) {
            wait();          // buffer full, wait
        }
        q.add(item);
        notifyAll();         // wake consumers
    }

    public synchronized int consume()
            throws InterruptedException {
        while (q.isEmpty()) {
            wait();          // buffer empty, wait
        }
        int item = q.poll();
        notifyAll();         // wake producers
        return item;
    }
}
```

---

## 5 Visual — Producer-Consumer Flow

```
   PRODUCER                BUFFER              CONSUMER
   ────────                ──────              ────────

   produce(1)              [1]                 (waiting)
   notify  ─────────────────────────────────►
                                                consume → 1
                            []                  notify ──┐
                                                          │
                                              ◄──────────┘
   produce(2)              [2]
   produce(3)              [2,3]
                                                consume → 2
                            [3]                 consume → 3
                            []                  wait...

   = Coordination via wait/notify
```

---

## 6 wait() vs sleep() — Common Trap

```
┌──────────────────┬──────────────────┬──────────────────┐
│  Aspect          │  wait()           │  sleep()         │
├──────────────────┼──────────────────┼──────────────────┤
│ Releases lock?   │ YES            │ NO            │
│ Where called?    │ synchronized block│ Anywhere         │
│ Defined in?      │ Object class      │ Thread class     │
│ Wake up by?      │ notify/notifyAll  │ Timeout only     │
└──────────────────┴──────────────────┴──────────────────┘
```

```
wait()  → "wait for signal"  (releases lock)
sleep() → "wait for time"    (keeps lock)
```

---

## 7 Deadlock — The Problem

```
   Thread A holds Lock 1, wants Lock 2
   Thread B holds Lock 2, wants Lock 1

   Both stuck forever waiting!
   = DEADLOCK
```

### Visual
```
   Thread A                Thread B
   ────────                ────────
   Lock 1               Lock 2
   ↓                       ↓
   wants Lock 2            wants Lock 1
   (blocked)               (blocked)

   Neither releases → frozen forever
```

---

## 8 4 Conditions for Deadlock

```
1. MUTUAL EXCLUSION
   Resource can only be held by ONE thread at a time

2. HOLD AND WAIT
   Thread holds one resource, waits for another

3. NO PREEMPTION
   Resource can't be forcibly taken

4. CIRCULAR WAIT
   Thread A waits B, B waits C, C waits A (cycle)
```

```
   Need ALL 4 → deadlock
   Break any 1 → prevent deadlock
```

---

## 9 Code Example — Deadlock

```java
class Account {
    int balance;

    void transfer(Account other, int amt) {
        synchronized (this) {           // lock 1
            synchronized (other) {       // lock 2
                this.balance -= amt;
                other.balance += amt;
            }
        }
    }
}

// Thread A: a.transfer(b, 100)
//   Locks: a, then b
// Thread B: b.transfer(a, 50)
//   Locks: b, then a
//   = CIRCULAR WAIT = DEADLOCK
```

---

## Deadlock Prevention

### Strategy 1: Lock Ordering
```java
void transfer(Account other, int amt) {
    Account first = this.id < other.id ? this : other;
    Account second = this.id < other.id ? other : this;

    synchronized (first) {        // always lower ID first
        synchronized (second) {
            // transfer
        }
    }
}

// = All threads lock in SAME order
// = No circular wait
```

### Strategy 2: tryLock with Timeout
```java
ReentrantLock lock1 = new ReentrantLock();
ReentrantLock lock2 = new ReentrantLock();

if (lock1.tryLock(1, SECONDS)) {
    try {
        if (lock2.tryLock(1, SECONDS)) {
            try {
                // work
            } finally { lock2.unlock(); }
        }
    } finally { lock1.unlock(); }
}

// = If timeout, retry or fail
// = No infinite block
```

---

## 11 Detection

```java
// Java tool to detect deadlocks:
//   jstack <pid>   → shows stack traces + deadlock info

// Programmatic:
ThreadMXBean bean = ManagementFactory.getThreadMXBean();
long[] ids = bean.findDeadlockedThreads();
if (ids != null) {
    // deadlock detected
}
```

---

## 12 Liveness Issues

```
1. DEADLOCK
   All threads stuck forever (mutual block)

2. LIVELOCK
   Threads keep changing state but no progress
   (like 2 people in corridor sidestepping each other)

3. STARVATION
   Some thread never gets CPU / lock
   (low priority always preempted)
```

---

## Memory Hooks

```
wait()       = release lock + sleep till notified
notify()     = wake ONE waiter
notifyAll()  = wake ALL waiters

MUST be in synchronized block
Use WHILE loop (not if) for condition check

wait vs sleep:
   wait  = releases lock
   sleep = keeps lock

DEADLOCK = circular wait on locks
   4 conditions: mutex + hold-wait + no-preempt + circular wait

PREVENT:
   1. Lock ordering (consistent)
   2. tryLock with timeout
   3. Avoid nested locks if possible
```

[← Back to JAVA](../../)
