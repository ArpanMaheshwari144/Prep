# Multithreading Basics — Visual Revision

---

## 1️⃣ Concept (Restaurant Kitchen Analogy)

```
SINGLE-THREADED (1 chef):
   Order 1 → cut veg → cook → plate → serve
   Order 2 → wait... wait... cut veg → cook → plate
   Order 3 → wait wait wait...
   
   = Slow, sequential
```

```
MULTI-THREADED (5 chefs):
   Order 1 → Chef A handles
   Order 2 → Chef B handles  (parallel)
   Order 3 → Chef C handles  (parallel)
   
   = Fast, parallel
```

```
Process = whole restaurant
Thread  = each chef inside
```

---

## 2️⃣ Process vs Thread

```
   PROCESS                     THREAD
   ───────                     ──────
   
   Restaurant building         Each chef
   Heavy (own memory)          Light (shared memory)
   Isolated                    Shared with siblings
   Slow to create              Fast to create
   
   1 Java app = 1 process
   Multiple threads inside     = many workers
```

---

## 3️⃣ Why Multithreading?

```
1. PARALLEL WORK
   2-core CPU → 2 threads run truly parallel
   = Speed boost
   
2. ASYNC OPERATIONS
   Heavy task = background thread
   Main thread = UI/response stays responsive
   
3. RESOURCE UTILIZATION
   While 1 thread waits for DB
   Other thread can process
   = CPU not idle
```

---

## 4️⃣ Creating Threads — 3 Ways

### Way 1: Extend Thread
```java
class MyThread extends Thread {
    @Override
    public void run() {
        System.out.println("Running");
    }
}

MyThread t = new MyThread();
t.start();    // starts thread
```

### Way 2: Implement Runnable (preferred)
```java
class MyTask implements Runnable {
    @Override
    public void run() {
        System.out.println("Running");
    }
}

Thread t = new Thread(new MyTask());
t.start();
```

### Way 3: Lambda (modern)
```java
Thread t = new Thread(() -> System.out.println("Running"));
t.start();
```

```
Runnable preferred:
   Java single-inheritance — can still extend other class
   Decouples task from runner
   Lambda-friendly
```

---

## 5️⃣ Thread Lifecycle (5 States)

```
   NEW (just created, not started)
        │
        │  t.start()
        ▼
   RUNNABLE (eligible to run)
        │
        ▼
   RUNNING (actually executing)
        │
        ├─→ BLOCKED (waiting for lock)
        │
        ├─→ WAITING (Object.wait, join)
        │
        ├─→ TIMED_WAITING (sleep, wait(ms))
        │
        ▼
   TERMINATED (done)
```

### Visual
```
   NEW ────► RUNNABLE ◄────► RUNNING ────► TERMINATED
                │                ↑
                │                │
                └──→ BLOCKED ────┘
                └──→ WAITING ────┘
                └──→ TIMED_WAITING ──┘
```

---

## 6️⃣ start() vs run() — CRITICAL

```java
Thread t = new Thread(() -> System.out.println("Hi"));

t.run();      // runs in CURRENT thread (no new thread!)

t.start();    // creates NEW thread, executes run()
```

```
WHY?
   start() → JVM creates NEW thread, calls run()
   run()   → just a normal method call
   
   Calling run() directly = thread didn't actually start
```

---

## 7️⃣ Basic Operations

```java
Thread t = new Thread(() -> {
    try { Thread.sleep(1000); } catch (...) {}
    System.out.println("Done");
});

t.start();                  // start thread
t.join();                   // wait for it to finish
Thread.sleep(500);          // pause current thread
t.isAlive();                // is it still running?
t.getName();                // thread name
t.setPriority(Thread.MAX_PRIORITY);  // priority hint
```

---

## 8️⃣ Synchronized (Lock)

```
PROBLEM:
   2 threads → same data → race condition
```

```java
class Counter {
    int count = 0;
    
    void increment() {
        count++;        // NOT atomic — race!
    }
}

// 2 threads call increment() 1000 times each
// Expected: 2000
// Actual:   may be 1700-1900 (lost updates)
```

### Fix: Synchronized
```java
class Counter {
    int count = 0;
    
    synchronized void increment() {     // mutex lock
        count++;
    }
}

// Now thread-safe
// Result: always 2000
```

```
synchronized = only ONE thread can execute this at a time
   = mutex (mutual exclusion)
```

---

## 9️⃣ Visual — Race Condition

```
Thread A:                  Thread B:
   read count = 5             read count = 5
   add 1 → 6                  add 1 → 6
   write count = 6            write count = 6
   
   Expected: 5 → 6 → 7
   Actual:   5 → 6 (one increment lost!)
   = RACE
```

```
With synchronized:
   
   Thread A:                  Thread B (waits):
   acquire lock
   read 5 → write 6
   release lock
                              acquire lock
                              read 6 → write 7
                              release lock
   
   = Atomic, no race
```

---

## Synchronized — 3 Forms

```java
// 1. Synchronized method (locks the OBJECT 'this')
public synchronized void method() { }

// 2. Synchronized block (specific lock object)
synchronized (someObj) {
    // ...
}

// 3. Static synchronized (locks the CLASS)
public static synchronized void method() { }
```

---

## 1️⃣1️⃣ volatile (Visibility)

```
PROBLEM:
   Thread A writes to a variable
   Thread B reads it
   B might see CACHED old value (CPU cache)
   = Visibility issue
```

```java
class Flag {
    volatile boolean running = true;
    
    void stop() { running = false; }
    
    void run() {
        while (running) { ... }   // sees update IMMEDIATELY
    }
}
```

```
volatile = "always read/write from MAIN MEMORY"
   = Visibility guarantee
   = NOT atomicity (use synchronized for that)
```

---

## 1️⃣2️⃣ volatile vs synchronized

```
┌─────────────────┬────────────────────┬─────────────────┐
│  Aspect         │  volatile           │  synchronized   │
├─────────────────┼────────────────────┼─────────────────┤
│ Visibility      │                  │              │
│ Atomicity       │                  │              │
│ Use case        │ Flag, single var    │ Multiple steps  │
│ Performance     │ Fast (no lock)      │ Slower (lock)   │
└─────────────────┴────────────────────┴─────────────────┘
```

---

## Memory Hooks

```
Process = restaurant building
Thread  = each chef inside

Thread.start() → new thread + run()
Thread.run()   → just method call (no new thread)

Lifecycle:
   NEW → RUNNABLE → RUNNING → TERMINATED
   (sub: BLOCKED, WAITING, TIMED_WAITING)

synchronized = lock (atomicity + visibility)
volatile     =  visibility only (no atomicity)

Use cases:
   Flag boolean → volatile
   Multi-step update → synchronized
```

[← Back to JAVA](../../)
