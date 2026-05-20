# Process vs Thread

---

## 1-Line Analogy

```
Process = Building (separate building, own resources)
Thread  = Room in building (shares building resources)
```

```
RULE:
   Process = container
   Thread = process ke andar execution unit
   Threads ALWAYS belong to a process
   0-thread process exist nahi karta
```

---

## Side by Side

```
┌──────────────────┬──────────────────┬──────────────────┐
│  Feature         │  Process          │  Thread           │
├──────────────────┼──────────────────┼──────────────────┤
│ Memory           │ OWN address space │ SHARED with other │
│                  │ Isolated           │ threads of process│
├──────────────────┼──────────────────┼──────────────────┤
│ Creation cost    │ Expensive       │ Cheap          │
│                  │ (fork, copy memory)│ (just stack)      │
├──────────────────┼──────────────────┼──────────────────┤
│ Context switch   │ Slow            │ Fast            │
│                  │ Full state swap     │ Minimal state     │
├──────────────────┼──────────────────┼──────────────────┤
│ Communication    │ IPC required       │ Shared memory     │
│                  │ (pipes, sockets,    │ (direct access)   │
│                  │ shared memory)      │                    │
├──────────────────┼──────────────────┼──────────────────┤
│ Isolation        │ Strong          │ Weak (shared)  │
│                  │ One crash safe     │ One thread crash  │
│                  │                     │ = whole process    │
├──────────────────┼──────────────────┼──────────────────┤
│ Concurrency safe │ Natural             │ Need locks         │
│                  │                     │ (race conditions) │
├──────────────────┼──────────────────┼──────────────────┤
│ Number on system │ Hundreds            │ Thousands         │
└──────────────────┴──────────────────┴──────────────────┘
```

---

## BUILDING ANALOGY VISUAL

```
PROCESS = SEPARATE BUILDINGS

   ┌─────────────┐    ┌─────────────┐    ┌─────────────┐
   │  Chrome     │    │  VSCode     │    │  Spotify    │
   │  Process    │    │  Process    │    │  Process    │
   │             │    │             │    │             │
   │  Memory: 2GB│    │  Memory: 1GB│    │  Memory:500M│
   │             │    │             │    │             │
   │  Code +     │    │  Code +     │    │  Code +     │
   │  Heap +     │    │  Heap +     │    │  Heap +     │
   │  Stack +    │    │  Stack +    │    │  Stack +    │
   │  Files       │    │  Files       │    │  Files       │
   └─────────────┘    └─────────────┘    └─────────────┘
   
   = Isolated, no shared memory
   = Chrome crash → VSCode OK 
```

```
THREADS = ROOMS IN SAME BUILDING (Process)

   ┌─────────────────────────────────────┐
   │       CHROME PROCESS                │
   │                                       │
   │  ┌─────────┐ ┌─────────┐ ┌─────────┐│
   │  │ Thread 1│ │ Thread 2│ │ Thread 3││
   │  │ (UI)    │ │ (Audio) │ │(Network)││
   │  │         │ │         │ │         ││
   │  │ Stack   │ │ Stack   │ │ Stack   ││
   │  └─────────┘ └─────────┘ └─────────┘│
   │                                       │
   │     SHARED:                           │
   │     • Heap memory                     │
   │     • Code segment                    │
   │     • File descriptors                │
   │     • Process resources               │
   └─────────────────────────────────────┘
   
   = Threads share memory (FAST communication)
   = One thread crashes badly → WHOLE process dies
```

---

## CONCRETE EXAMPLE

```
Chrome browser:
   • Each TAB = separate PROCESS (since 2008)
   • Within a tab = multiple THREADS:
      - UI thread (render page)
      - Network thread (HTTP calls)
      - JavaScript thread (V8 engine)
      - GPU thread
      - Compositor thread
   
   Why?
   • One tab crashes → other tabs survive (process isolation)
   • UI smooth → JavaScript in separate thread (no block)
```

```
Spring Boot app:
   • App runs as ONE PROCESS (JVM)
   • Inside JVM, many THREADS:
      - Main thread
      - Tomcat thread pool (200 by default)
      - Garbage collector threads
      - @Async pool threads
      - JPA connection pool threads
   
   Each HTTP request → handled by ONE thread from pool
```

---

## SHARED MEMORY = POWER + DANGER

```
THREADS share memory:

   FAST communication:
      Thread 1: counter = 5
      Thread 2: reads counter → 5 (instant)
   
   RACE CONDITIONS:
      Both threads do: counter++
      
      Thread 1 reads counter (5)
      Thread 2 reads counter (5)
      Thread 1 increments → 6
      Thread 2 increments → 6
      
      Expected: 7
      Actual: 6 (BUG!)
      
      FIX: synchronized / locks / atomic
```

```
PROCESSES don't share memory:

   NO RACE CONDITIONS
   Slow communication:
      Need IPC (sockets, pipes, shared memory file)
      Serialization overhead
```

---

## CONTEXT SWITCH COST

```
PROCESS SWITCH:
   1. Save full register state
   2. Save memory mappings
   3. Flush TLB (translation cache)
   4. Load new process's mappings
   5. Restore registers
   = ~1000+ ns
   = "EXPENSIVE"

THREAD SWITCH:
   1. Save register state
   2. Restore other thread's registers
   3. Same memory mappings (already there)
   = ~100 ns
   = "CHEAP"
   
= Threads can switch 10x faster
= Why thread pools are efficient
```

---

## WHEN TO USE WHAT

```
USE PROCESSES WHEN:
   • Strong isolation needed (security)
   • One crash shouldn't kill others
   • Different programs (Chrome + VSCode)
   • Multi-tenancy (containers, K8s pods)

   Examples:
   • Browser tabs (Chrome process per site)
   • Docker containers
   • K8s pods
   • Different microservices

USE THREADS WHEN:
   • Same program, parallel work
   • Need shared memory (fast)
   • Lightweight concurrency
   • CPU-bound + I/O-bound mix

   Examples:
   • Web server thread pool (handle requests)
   • Async tasks within app (@Async)
   • UI thread + worker threads
   • Multi-threaded algorithms
```

---

## JAVA SPECIFIC

```
Java JVM = ONE process
Inside JVM = many threads

   Thread t = new Thread(() -> doWork());
   t.start();
   
   ExecutorService pool = Executors.newFixedThreadPool(10);
   pool.submit(task);
   
Java 21 brought VIRTUAL THREADS (Project Loom):
   • Millions of threads possible
   • Light weight (managed by JVM)
   • Great for I/O-heavy work
   
Spring Boot 3.2+ supports virtual threads
   = Tomcat can scale to millions of concurrent users
```

---

## INTERVIEW LINE

```
"A process is an instance of a program with its own
 memory space, file descriptors, and resources. Processes
 are isolated — one crash doesn't affect others.

 A thread is a unit of execution within a process.
 Threads share the process's memory and resources.
 Cheap to create, fast context switch, but race conditions
 require synchronization.

 Use processes for isolation (browser tabs, containers,
 microservices). Use threads for parallel work in same
 program (web server pools, async tasks).

 Java JVM is a single process; modern Spring Boot uses
 virtual threads for massive concurrency without OS thread
 overhead."
```

---

## Memory Hook

```
Process = Building (own everything, isolated)
Thread  = Room (shares building stuff)

Process: Slow create, isolated, no race
Thread:  Fast create, shared, race conditions

Communication:
   Process → IPC (slow, safe)
   Thread → Shared memory (fast, needs locks)

Rule:
   Isolation matters → Process
   Speed matters → Thread (with sync)

Modern Java: Virtual threads (Project Loom)
```

[← HLD README](../README.md)
