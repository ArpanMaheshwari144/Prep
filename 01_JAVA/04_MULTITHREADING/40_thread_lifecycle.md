# Thread Lifecycle — 5 States

> **V90 — Multithreading: Topic 40**

---

## WHY — Thread States Kyu?

→ **OS ko thread manage karna hai** — kab CPU de, kab wait karwaye
→ Thread ki **5 states** — har transition OS scheduler decide karta

---

## 5 States Visualization

```
                    ┌─────────────┐
                    │   WAITING   │
                    │  (paused)   │
                    └──────┬──────┘
                  notify()/│  ▲ wait()/sleep()
                  timeout  │  │
                           ▼  │
┌─────────┐ start() ┌──────────┐ scheduler picks ┌─────────┐ run()  ┌──────┐
│   NEW   │────────►│ RUNNABLE │────────────────►│ RUNNING │ done   │ DEAD │
│(created)│         │ (ready)  │                 │(CPU pe) │───────►│      │
└─────────┘         └──────────┘ yield()/time over└────────┘        └──────┘
```

**Flow:** NEW → start() → RUNNABLE → scheduler → RUNNING → done → DEAD

---

## State Table

| State | Kaise pahuncho | Matlab |
|-------|---------------|--------|
| **NEW** | `new Thread()` banaya | Bana but `start()` nahi kiya |
| **RUNNABLE** | `start()` call kiya | CPU ke liye ready |
| **RUNNING** | JVM ne CPU diya | Kaam chal raha (active) |
| **BLOCKED/WAITING** | `wait()`, `sleep()`, lock wait | Kisi resource ka wait |
| **TERMINATED** | `run()` complete | Khatam |

---

## Code

```java
Runnable task = new MyTask();
Thread t = new Thread(task);          // NEW

t.start();                             // RUNNABLE → RUNNING
// JVM decide karta kab CPU milega
// run() complete → TERMINATED
```

---

## TRAP 1 — `start()` Ke Baad Turant RUNNING Nahi

> **`start()` kiya = thread RUNNABLE state mein gaya, RUNNING NAHI.**
> **JVM/OS scheduler decide karta kab CPU milega.**

## TRAP 2 — Ek Baar TERMINATED, Dobara Nahi

> **TERMINATED thread `start()` dobara call nahi kar sakte → `IllegalThreadStateException`.**
> Naya `Thread` object banao.

```java
Thread t = new Thread(...);
t.start();
// ... thread khatam (TERMINATED)
t.start();    // IllegalThreadStateException
```

---

## POWER PHRASE

> *"A thread goes through 5 states: NEW when created, RUNNABLE after `start()`, RUNNING when the scheduler gives it CPU time, BLOCKED or WAITING when it needs a lock or waits, and TERMINATED when `run()` completes."*

> **Yaad rakh:**
> NEW → RUNNABLE → RUNNING → BLOCKED/WAITING → TERMINATED
> `start()` = RUNNABLE (not RUNNING)
> Terminated = dobara `start()` impossible
