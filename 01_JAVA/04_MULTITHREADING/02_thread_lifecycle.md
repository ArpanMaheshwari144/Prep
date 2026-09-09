# Thread Lifecycle — 6 States (Thread.State enum)

> **V90 — Multithreading: Topic 40**
> ★ FIX (9-Sep): pehle "RUNNING" state likha tha — **Java me RUNNING state hota hi NAHI.** Sahi = 6 states (`Thread.State` enum).

---

## WHY — Thread States Kyu?

→ **OS + JVM ko thread manage karna hai** — kab CPU de, kab wait karwaye
→ Java me `Thread.State` enum = **exactly 6 states** — `t.getState()` se milta

---

## ★ 6 STATES (yaad rakh — RUNNING NAHI hota)
```
NEW · RUNNABLE · BLOCKED · WAITING · TIMED_WAITING · TERMINATED
```
★ **"RUNNING" alag state NAHI** — wo RUNNABLE ke ANDAR hi hai. Java "ready-to-run" aur "actually-on-CPU" ko alag nahi ginta — **dono RUNNABLE**. (OS-level running/ready dono ek hi Java-state me.)

---

## Visualization

```
┌─────────┐ start() ┌────────────┐        ┌──────────────┐
│   NEW   │────────►│  RUNNABLE  │        │  TERMINATED  │
│(created)│         │(ready + on │ run()  │  (khatam)    │
└─────────┘         │  CPU dono) │───────►└──────────────┘
                    └─────┬──────┘
        ┌─────────────────┼──────────────────┐
        ▼                 ▼                  ▼
  ┌───────────┐    ┌────────────┐    ┌──────────────────┐
  │  BLOCKED  │    │  WAITING   │    │  TIMED_WAITING   │
  │(lock ke   │    │(wait()/    │    │(sleep(ms)/       │
  │ liye ruka)│    │ join() —   │    │ wait(ms)/        │
  │           │    │ no timeout)│    │ join(ms))        │
  └───────────┘    └────────────┘    └──────────────────┘
```

---

## State Table (6 — sahi)

| State | Kaise pahuncho | Matlab |
|-------|---------------|--------|
| **NEW** | `new Thread()` banaya | Bana, `start()` nahi kiya |
| **RUNNABLE** | `start()` call kiya | CPU ke liye ready YA CPU pe chal raha (dono isi me) |
| **BLOCKED** | `synchronized` lock ka wait | Monitor-lock chahiye, doosre ke paas hai |
| **WAITING** | `wait()` / `join()` / `park()` (bina timeout) | INDEFINITE wait — koi jagayega tabhi |
| **TIMED_WAITING** | `sleep(ms)` / `wait(ms)` / `join(ms)` | Timeout-wala wait — waqt-baad khud uthta |
| **TERMINATED** | `run()` complete | Khatam |

★ **BLOCKED vs WAITING vs TIMED_WAITING** (grill favourite):
```
BLOCKED       -> synchronized MONITOR-lock ka intezaar (koi aur held kiye hai)
WAITING       -> wait()/join() bina time -> jab tak notify/end na ho
TIMED_WAITING -> sleep(ms)/wait(ms)/join(ms) -> time khatam hote hi khud wapas
```

---

## Code

```java
Runnable task = new MyTask();
Thread t = new Thread(task);          // NEW
System.out.println(t.getState());     // NEW

t.start();                             // RUNNABLE (ready + on-CPU dono isi me)
// JVM/OS decide karta kab CPU
// run() complete → TERMINATED
```

---

## TRAP 1 — `start()` = RUNNABLE (RUNNING naam ki koi cheez nahi)

> **`start()` kiya = thread RUNNABLE me gaya. "RUNNING" alag state Java me hai HI NAHI** — on-CPU bhi RUNNABLE hi hai.

## TRAP 2 — Ek Baar TERMINATED, Dobara Nahi

> **TERMINATED thread `start()` dobara nahi → `IllegalThreadStateException`.** Naya `Thread` object banao.

```java
Thread t = new Thread(...);
t.start();
// ... thread khatam (TERMINATED)
t.start();    // IllegalThreadStateException
```

---

## POWER PHRASE

> *"A Java thread has exactly 6 states in `Thread.State`: NEW, RUNNABLE, BLOCKED, WAITING, TIMED_WAITING, TERMINATED. There is no separate RUNNING state — RUNNABLE covers both ready-to-run and actually-running-on-CPU. BLOCKED means waiting for a synchronized monitor lock, WAITING is an indefinite wait (wait()/join() with no timeout), and TIMED_WAITING is a timed wait (sleep(ms)/wait(ms))."*

> **Yaad rakh:**
> 6 states: NEW → RUNNABLE → (BLOCKED / WAITING / TIMED_WAITING) → TERMINATED
> RUNNING = NAHI hota (RUNNABLE ke andar)
> BLOCKED=lock · WAITING=no-timeout · TIMED_WAITING=timeout
> Terminated = dobara `start()` impossible
