# Garbage Collection — Visual Revision

---

## 1 Problem (C/C++ Pain)

```
C/C++ mein:
   Tu khud memory allocate kare (malloc)
   Tu khud free kare (free)

   Bhul gaye? = MEMORY LEAK
   Free kiya 2 baar? = CRASH
   Use kiya after free? = UNDEFINED BEHAVIOR
```

```
Java mein:
   Tu sirf "new" karke object banata
   Java AUTOMATICALLY clean up karta
   No manual free
   = GARBAGE COLLECTOR
```

---

## 2 Maid Analogy

```
Imagine tera ghar:
   Tu raat ko khaata hai
   Plates, kachra, dustbin bhar gaya

   WITHOUT MAID:
      Khud saaf karna padega
      Bhul gaye = bahut kachra

   WITH MAID (GC):
      Maid roz aati
      Kachra utha le jati
      Tu sirf use kar
```

---

## 3 Heap Visual

```
   ┌──────────────────────────────────────┐
   │                  HEAP                  │
   │   (jaha sab objects rehte)            │
   │                                        │
   │  ┌────────────────────────────────┐  │
   │  │  YOUNG GEN (new objects)        │  │
   │  │  ┌────────┬─────────┬────────┐ │  │
   │  │  │  Eden  │ Survivor│Survivor│ │  │
   │  │  │        │  S0     │  S1    │ │  │
   │  │  └────────┴─────────┴────────┘ │  │
   │  └────────────────────────────────┘  │
   │                                        │
   │  ┌────────────────────────────────┐  │
   │  │  OLD GEN (long-lived objects)   │  │
   │  └────────────────────────────────┘  │
   └──────────────────────────────────────┘
```

---

## 4 Object Lifecycle Flow

```
1. new Object()
        │
        ▼
2. EDEN (young gen — naya bachcha)
        │
        ▼
3. Eden full?  → Minor GC trigger
        │
        ├── unused objects → DELETED
        └── still used    → moved to SURVIVOR
                              │
                              ▼
4. Multiple GC cycles survive?
        │
        ▼
5. OLD GEN (promote — "yeh long-lived hai")
        │
        ▼
6. Old gen full? → Major GC (slower, full sweep)
```

---

## 5 Mark-Sweep Algorithm (Core)

```
Phase 1: MARK
   GC traverses all reachable objects (from roots)
   Marks them as "alive"

   ┌──────────────────────────┐
   │  Roots (stack, statics)   │
   │       │                    │
   │       ▼                    │
   │     obj1 (reachable)    │
   │       │                    │
   │       ▼                    │
   │     obj2 (reachable)    │
   │                            │
   │     obj3 (unreachable)  │  ← garbage
   │     obj4 (unreachable)  │  ← garbage
   └──────────────────────────┘

Phase 2: SWEEP
   Unmarked objects = garbage
   Memory free kar do

   ┌──────────────────────────┐
   │     obj1                │
   │     obj2                │
   │     (obj3, obj4 cleared)   │
   └──────────────────────────┘
```

---

## 6 Stop-The-World Pause

```
   App running normally
        │
        ▼
   GC needs to run
        │
        ▼
    ALL APP THREADS PAUSE
        │
        ▼
   GC scans + cleans heap
        │
        ▼
   ▶  ALL THREADS RESUME

   = Brief pause (ms to seconds)
   = Performance impact
```

```
Why pause needed:
   GC scanning karte time agar app threads chalti rahe
   → Objects change ho jaye scanning ke beech
   → Incorrect state
   → CRASH
```

---

## 7 Why Young + Old Split?

```
OBSERVATION:
   Most objects die YOUNG
   (created → used briefly → discarded)

   Few objects survive long
   (caches, singletons, etc.)
```

```
SMART DESIGN:
   Young gen mein frequent GC (fast, small)
   Old gen mein rare GC (slow, but full sweep)

   = Generational GC
   = Best of both
```

---

## 8 Minor vs Major GC

```
┌──────────────────┬─────────────────┬──────────────────┐
│  Type            │ Scope            │ Frequency         │
├──────────────────┼─────────────────┼──────────────────┤
│ Minor GC         │ Young gen only   │ Frequent (fast)   │
│ Major (Full) GC  │ Old gen + young  │ Rare (slow)       │
└──────────────────┴─────────────────┴──────────────────┘
```

---

## 9 GC Algorithms

```
1. SERIAL GC (single thread)
   Small apps, low memory

2. PARALLEL GC (multi-thread)
   High throughput
   Default in Java 8

3. CMS — Concurrent Mark Sweep
   Low pause time
   Deprecated in Java 14

4. G1 — Garbage First (Java 9+ default)
   Region-based heap
   Predictable pauses

5. ZGC / Shenandoah (modern)
   Ultra-low pause (< 10ms)
   Java 11+
```

---

## Common Interview Q&A

```
Q: Java mein memory leak hota hai?
A: Yes! Despite GC.
   Reasons:
   • Static collections jo grow karte
   • Listeners not unregistered
   • Inner classes holding outer references
   • Caching without eviction

Q: When does GC run?
A: JVM decides (algorithm-based).
   System.gc() = REQUEST (JVM may ignore)

Q: Stop-the-world good ya bad?
A: Bad for real-time systems.
   ZGC/Shenandoah minimize this.

Q: How to make object eligible for GC?
A: Remove all references:
   obj = null;
   list.remove(obj);
   obj going out of scope
```

---

## 11 Force GC (Don't Do This In Production)

```java
System.gc();      // REQUEST, JVM may ignore
Runtime.getRuntime().gc();   // same

// Reality:
//   Don't force GC manually
//   JVM is smarter than you
//   Forcing = often hurts performance
```

---

## 12 Memory Hooks

```
GC = Maid (auto-cleanup)

Young Gen   = bachpan (naya objects, mostly die fast)
Old Gen     = budhapa (long-lived survivors)

Mark        = mark reachable objects
Sweep       = delete unreachable

Stop-the-world = all threads pause during GC
Minor GC    = young gen only (fast, frequent)
Major GC    = full heap (slow, rare)

G1          = modern default (Java 9+)
ZGC         = ultra-low pause (Java 11+)
```

[← Back to JAVA](../)
