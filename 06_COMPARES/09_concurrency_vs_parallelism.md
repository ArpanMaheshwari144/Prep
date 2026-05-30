# Concurrency vs Parallelism

---

## 1-Line Analogy

```
Concurrency = 1 chef cooking 3 dishes (alternating)
Parallelism = 3 chefs cooking 3 dishes (truly same time)
```

---

## Rob Pike (Go creator) — KEY QUOTE

```
"Concurrency is dealing with lots of things at once.
 Parallelism is doing lots of things at once."

   = Concurrency = STRUCTURE (handle multiple tasks)
   = Parallelism = EXECUTION (actually run together)
```

---

## Side by Side

```
┌──────────────────┬──────────────────┬──────────────────┐
│  Feature         │  Concurrency      │  Parallelism      │
├──────────────────┼──────────────────┼──────────────────┤
│ Definition       │ Multiple tasks    │ Multiple tasks    │
│                  │ MAKING PROGRESS    │ RUNNING TOGETHER  │
├──────────────────┼──────────────────┼──────────────────┤
│ Time             │ Interleaved        │ Simultaneous      │
│                  │ (rapid switching)  │ (truly same time) │
├──────────────────┼──────────────────┼──────────────────┤
│ CPU cores        │ 1 core enough      │ Need MULTIPLE      │
│                  │                     │ cores              │
├──────────────────┼──────────────────┼──────────────────┤
│ About            │ STRUCTURE          │ EXECUTION          │
│                  │ (design pattern)   │ (hardware ability)│
├──────────────────┼──────────────────┼──────────────────┤
│ Synonyms         │ Multitasking       │ Multi-core         │
│                  │ Time-slicing       │ True parallel      │
├──────────────────┼──────────────────┼──────────────────┤
│ Example          │ Node.js single    │ Java Streams        │
│                  │ thread async       │ .parallelStream()  │
└──────────────────┴──────────────────┴──────────────────┘
```

---

## CONCURRENCY VISUAL

```
1 CHEF (1 CPU core):

Task A: ▓▓░░▓▓░░▓▓░░░░▓▓
Task B: ░░▓▓░░▓▓░░░░▓▓░░
Task C: ░░░░░░░░▓▓▓▓░░░░

Time: ──────────────────►

   Chef switches between dishes:
   A (chop) → B (stir) → A (chop) → C (boil) → B (stir)

   Sirf 1 task ek time pe actually running
   But MULTIPLE tasks PROGRESS kar rahe

   = Concurrent, not parallel
   = Time-slicing
```

---

## PARALLELISM VISUAL

```
3 CHEFS (3 CPU cores):

Chef 1 (Task A): ▓▓▓▓▓▓▓▓▓▓
Chef 2 (Task B): ▓▓▓▓▓▓▓▓▓▓
Chef 3 (Task C): ▓▓▓▓▓▓▓▓▓▓

Time: ──────────────────►

   All 3 dishes cooking SIMULTANEOUSLY
   = True parallel execution
   = Requires multiple cores
```

---

## SUBTLE — Concurrency can EXIST without Parallelism

```
SINGLE CORE CPU:

   Concurrent? YES (rapid switching simulates parallel)
   Parallel?   NO  (only 1 thing actually runs at a time)

   Examples:
   • Node.js (single-threaded event loop)
   • Original Java threads on single-core CPU
   • Goroutines on 1-core machine

MULTI-CORE CPU:

   Concurrent? YES (multiple tasks managed)
   Parallel?   YES (actually run together)

   Examples:
   • Java parallel streams
   • Goroutines on 8-core machine
   • Modern web servers (Tomcat thread pool on multi-core)
```

---

## KEY INSIGHT

```
"Parallelism IMPLIES concurrency,
 but concurrency does NOT imply parallelism."

Think of it as:
   PARALLELISM = subset of CONCURRENCY

   ┌──────────────────────────────┐
   │  CONCURRENCY                   │
   │  (handling multiple tasks)     │
   │                                  │
   │   ┌────────────────────────┐  │
   │   │  PARALLELISM            │ │
   │   │  (running together)     │ │
   │   │  needs multi-core         │ │
   │   └────────────────────────┘  │
   └──────────────────────────────┘
```

---

## JAVA EXAMPLES

```
CONCURRENT (non-parallel) — single thread:
   // Async on single thread
   CompletableFuture.supplyAsync(() -> task1())
       .thenApply(result -> task2(result))
       .thenApply(result -> task3(result));

   = Multiple tasks structured concurrently
   = But on 1 thread = no parallelism

PARALLEL — multiple threads:
   List<Integer> data = ...;
   data.parallelStream()
       .map(this::heavyComputation)
       .collect(toList());

   = JVM splits work across multiple cores
   = TRUE parallel execution

CONCURRENT + PARALLEL:
   ExecutorService pool = Executors.newFixedThreadPool(4);
   pool.submit(task1);
   pool.submit(task2);
   pool.submit(task3);

   = Concurrent (handling multiple tasks)
   = Parallel (runs on multiple cores)
```

---

## WHEN TO USE WHAT

```
CONCURRENCY USEFUL FOR:
   • I/O-bound work (waiting for DB, network)
   • Many slow tasks (don't need CPU)
   • Single-threaded event loops (Node.js model)
   • Async user interactions

PARALLELISM USEFUL FOR:
   • CPU-bound work (heavy computation)
   • Independent tasks (no shared state)
   • Data processing (map/reduce)
   • Image/video encoding
   • Scientific computing
```

---

## INTERVIEW LINE

```
"Concurrency is the structure of handling multiple tasks
 — interleaving their progress, can happen on a single core
 via time-slicing.

 Parallelism is actually executing multiple tasks at the
 same instant — requires multiple cores.

 Rob Pike: concurrency is dealing with many things at once,
 parallelism is doing many things at once.

 Node.js single-threaded model is concurrent but not
 parallel. Java parallel streams or Goroutines on multi-core
 are both concurrent and parallel.

 Choose concurrency for I/O-bound work, parallelism for
 CPU-bound computation."
```

---

## Memory Hook

```
Concurrency = 1 chef juggling 3 dishes (structure)
Parallelism = 3 chefs (execution)

Rob Pike:
   "Concurrency = dealing with many at once"
   "Parallelism = doing many at once"

Single core → only concurrency possible
Multi-core  → parallelism possible

I/O-bound → concurrency wins
CPU-bound → parallelism wins
```

[← HLD README](../README.md)
