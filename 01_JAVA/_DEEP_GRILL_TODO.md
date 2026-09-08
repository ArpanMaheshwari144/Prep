# JAVA — DEEP-GRILL TODO (kal subah SABSE PEHLE)

> 7-Sep audit se nikla: notes recall-achhe + kai genuinely DEEP, par concurrency-primitives patle.
> JP round-2 = Java GRILL (decider round). Ye gaps bharne hain — deep, grill-ready.

## ★ REAL-PROJECT CONNECT — DONE (8-Sep)
```
Concurrency notes usercrud demos se jude: 05_volatile (VolatileDemoController running-flag),
04_synchronized (RateLimitController.hit), 02_hashmap-vs-CHM (Idempotency/KafkaConsumer CHM),
10_executor (@Async DemoJobs), 06_atomic (Idempotency AtomicInteger — pehle se).
Core Java (OOP/String/exceptions) = honest-general (no forced project-anchor).
```

## ★ METHOD (Arpan-LOCKED — aise hi karna)
```
1. Claude PEHLE TEACH kare — concept + analogy (store/box/visual jaisa), samjhaye.
2. Arpan SAMJHE -> "samjha?" gate.
3. PHIR file me likhwaye (Arpan bole, ya Claude likhe uske samajhne ke baad).
   -> silent "file update kar di" NAHI. samajh pehle, likhna baad me.
4. HashMap wala bhi dobara TEACH (grill me quiz kiya tha, deep padhaya nahi).
```

---

## ★ DONE (8-Sep) — hata diye list se, notes files me pakke
```
volatile+JMM · CAS/ABA/LongAdder · ThreadPoolExecutor · GC collectors+ref-types · HashMap resize-math
```

## ★ BACHA HUA — PRIORITY (ADD karne hain)

```
1. Streams hard-layer (streams note EXTEND)
   -> parallelStream common-ForkJoinPool pitfall · Spliterator · short-circuit · stateful/stateless
2. CompletableFuture thread-of-execution (future note EXTEND)
   -> thenApply vs thenApplyAsync · thenCompose vs thenApply · whenComplete/handle
3. final-field safe publication + effectively-final (final/immutable/lambda EXTEND)
   -> JMM guarantee jo immutable object ko thread-safe banata
```

## ★ MISSING ENTIRELY (naye note — biggest grill-risk)
```
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
