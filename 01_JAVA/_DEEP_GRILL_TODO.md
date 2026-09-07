# JAVA — DEEP-GRILL TODO (kal subah SABSE PEHLE)

> 7-Sep audit se nikla: notes recall-achhe + kai genuinely DEEP, par concurrency-primitives patle.
> JP round-2 = Java GRILL (decider round). Ye gaps bharne hain — deep, grill-ready.

## ★ METHOD (Arpan-LOCKED — aise hi karna)
```
1. Claude PEHLE TEACH kare — concept + analogy (store/box/visual jaisa), samjhaye.
2. Arpan SAMJHE -> "samjha?" gate.
3. PHIR file me likhwaye (Arpan bole, ya Claude likhe uske samajhne ke baad).
   -> silent "file update kar di" NAHI. samajh pehle, likhna baad me.
4. HashMap wala bhi dobara TEACH (grill me quiz kiya tha, deep padhaya nahi).
```

---

## ★ PRIORITY — top 8 deeper layer (ADD karne hain, order me)

```
1. volatile + JMM happens-before (NAYA note)
   -> visibility vs atomicity · count++ race · double-checked-locking singleton
2. Atomic / CAS / ABA / LongAdder (NAYA note)
   -> CAS spin · ABA problem + AtomicStampedReference · LongAdder striping
3. ThreadPoolExecutor internals (executor note EXTEND)
   -> 7 constructor params · core->queue->max ordering · rejection policies · IO-vs-CPU sizing
4. Streams hard-layer (streams note EXTEND)
   -> parallelStream common-ForkJoinPool pitfall · Spliterator · short-circuit · stateful/stateless
5. GC collectors + reference types (GC note EXTEND)
   -> Serial/Parallel/CMS/G1/ZGC · G1 regions · Soft/Weak/Phantom · tuning flags
6. HashMap resize + load-factor math (hashmap_internal EXTEND)
   -> 0.75 Poisson derive · lo/hi resize split · treeify needs table>=64 · untreeify 6
7. CompletableFuture thread-of-execution (future note EXTEND)
   -> thenApply vs thenApplyAsync · thenCompose vs thenApply · whenComplete/handle
8. final-field safe publication + effectively-final (final/immutable/lambda EXTEND)
   -> JMM guarantee jo immutable object ko thread-safe banata
```

## ★ MISSING ENTIRELY (naye note — biggest grill-risk)
```
- volatile (upar #1)
- Atomic/CAS (upar #2)
- JMM / happens-before (upar #1 ke saath)
- Lock toolkit: ReentrantLock / ReadWriteLock / Semaphore / CountDownLatch / CyclicBarrier
- ThreadLocal (finance request-scoped me common)
```

## ★ FACTUAL FIX (quick win)
```
- 04_MULTITHREADING/02_thread_lifecycle.md -> "RUNNING" state GALAT.
  Java me RUNNING state hota hi nahi. Sahi 6 = NEW / RUNNABLE / BLOCKED / WAITING / TIMED_WAITING / TERMINATED.
  (RUNNABLE hi on-CPU include karta.) -> theek karna.
```

## ★ PEHLE SE DEEP (inpe kaam nahi — chinta mat karna)
```
hashmap_vs_concurrenthashmap · fail_fast_vs_fail_safe · wait_vs_sleep ·
notify_deep_dive · checked_vs_unchecked · abstract_vs_interface
```

---
> KAL SUBAH: Claude ye file kholke #1 (volatile+JMM) se TEACH-first shuru kare. ek-ek topic, samajh -> phir likho.
