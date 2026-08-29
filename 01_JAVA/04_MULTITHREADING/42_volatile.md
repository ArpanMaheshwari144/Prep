# volatile — Visibility Problem Fix

> **V90 — Multithreading: Topic 42**

---

## STORY — Background Thread Loop

→ Ek **background thread** tha jo **flag check karke** loop chal raha tha
→ Doosre thread ne **flag false kar diya** — lekin **pehla thread band nahi hua**
→ Ghante baad pata chala
→ **CPU caching ki wajah se** pehle thread ko **nayi value dikhi hi nahi**
→ Apni **purani cache se padh raha tha**
→ **`volatile` lagaya** — ab har thread **seedha main memory se padhta**, cache se nahi

---

## Visualization — CPU Cache Problem

```
                   MAIN MEMORY (RAM)
                   running = true
                          ↑
                          │
        ┌─────────────────┴─────────────────┐
        ▼                                   ▼
   Thread 1                             Thread 2
   L1 Cache: running = true             L1 Cache: running = true

  Thread 2 ne main memory mein false kiya:
   MAIN: running = false
   T1 cache: running = true  (purana!)
   T2 cache: running = false

  Thread 1 ko PATA HI NAHI CHALA — loop chalta raha!
```

**Fix — `volatile` lagao** → cache band, **seedha main memory** se padho.

---

## Code

### Without volatile — bug
```java
class Worker implements Runnable {
    private boolean running = true;          // cache risk

    public void stop() { running = false; }

    public void run() {
        while (running) { /* kaam */ }       // cache se padha — false kabhi nahi dikha
    }
}
```

### With volatile — fix
```java
class Worker implements Runnable {
    private volatile boolean running = true;  // cache band, main memory

    public void stop() { running = false; }

    public void run() {
        while (running) { /* kaam */ }        // har read main memory se
    }
}
```

---

## TRAP — `volatile` ≠ Atomic

> **`volatile` SIRF visibility fix karta. ATOMICITY nahi.**
> **`count++` jaisa compound operation `volatile` se SAFE NAHI hota.**

**Why?** `count++` actually **3 steps** hai:
1. **READ** `count` from memory
2. **ADD** 1
3. **WRITE** back to memory

`volatile` guarantee karta READ latest value milegi. Lekin **2 threads dono ne `count = 5` padha**, dono ne `6` likha → ek **increment LOST**.

```java
// GALAT — volatile compound operation safe nahi
volatile int count = 0;
count++;                         // race condition

// FIX 1 — synchronized
synchronized void inc() { count++; }

// FIX 2 — AtomicInteger
AtomicInteger count = new AtomicInteger(0);
count.incrementAndGet();         // hardware-level atomic
```

---

## synchronized vs volatile vs AtomicInteger

| | volatile | synchronized | AtomicInteger |
|--|---------|-------------|---------------|
| **Visibility** | | | |
| **Atomicity** | | | |
| **Lock?** | NO | YES | NO (CAS — hardware) |
| **Performance** | Fastest | Slowest | Fast (counters ke liye best) |
| **Use case** | Flag (read-mostly) | Critical sections | Counters/numbers |

---

## POWER PHRASE

> *"`volatile` ensures every thread reads the latest value from main memory instead of CPU cache — it solves visibility but NOT atomicity. For compound operations like increment, use `synchronized` or `AtomicInteger`."*

> **Yaad rakh:**
> volatile = visibility fix (cache se nahi, RAM se padho)
> volatile ≠ atomic (`count++` race condition rahega)
> Flag → volatile. Counter → AtomicInteger. Critical section → synchronized.

---

## ★ TRIO CONNECT — volatile / atomic / synchronized (ek jagah)

> 3 tools, badhta hua "kitna deta hai". aaj usercrud idempotency me atomic+synchronized dekhe; volatile flag ke liye.

```
volatile      -> sirf VISIBILITY (cache -> RAM)          | flag/boolean | Fastest | lock NAHI, atomic NAHI
AtomicInteger -> VISIBILITY + ATOMICITY (CAS)            | counter      | Fast    | lock NAHI (optimistic)
synchronized  -> VISIBILITY + ATOMICITY (lock)          | multi-line   | Slowest | LOCK (pessimistic)
```

**DECISION (yaad rakh):**
- FLAG (ek likhe, baaki padhein) -> `volatile`
- COUNTER / single value -> `AtomicInteger` (CAS)
- MULTI-LINE critical block (2+ steps atomic) -> `synchronized`

**KYUN badhta:** `count++` = 3 step (read/add/write). volatile sirf READ-latest deta -> 3-step atomic nahi -> race rahega. Atomicity ke liye atomic (CAS) ya synchronized (lock).

> Project: `running` flag hota to volatile; `orderCounter` = AtomicInteger (CAS); poora 3-line block atomic chahiye hota to synchronized.
> optimistic (volatile/atomic/CHM/@Version) vs pessimistic (synchronized/SELECT-FOR-UPDATE) — wahi payment wali family.

---

## ★ HANDS-ON — usercrud VolatileDemoController (27-Aug, khud live chalaya)

> Maqsad tha: theory ko chal ke dekhna. Background thread ek `boolean running` flag pe loop kare; doosra thread
> `false` kare — kya loop rukega? Story wali "background thread band nahi hua" cheez LIVE reproduce karni thi.

### Code (do endpoint — start / stop)
```java
@RestController
public class VolatileDemoController {

    // pehle PLAIN rakha (volatile NAHI) — bug dekhne ko. baad me `volatile` lagaya.
    private boolean running = true;
    // private volatile boolean running = true;   // <- fix

    @GetMapping("/volatile/start")
    public String start() {
        running = true;
        new Thread(() -> {
            System.out.println("WORKER: loop shuru, running=" + running);
            long count = 0;
            while (running) {          // is flag ko baar-baar padhega (apni CPU-cache se)
                count++;
            }
            System.out.println("WORKER: loop RUKA! count=" + count);
        }).start();
        return "worker started";
    }

    @GetMapping("/volatile/stop")
    public String stop() {
        running = false;               // main-memory me false — worker ko dikhega?
        System.out.println("MAIN: running=false set kar diya");
        return "stop signal bheja";
    }
}
```

### Kya hua (honest — theek waisa nahi jaisa socha tha)
```
/start -> WORKER: loop shuru, running=true
/stop  -> MAIN: running=false set kar diya
          WORKER: loop RUKA! count=57390023988      <- ruk GAYA! (bina volatile ke bhi)
```
- **Bina volatile** ke bhi worker **ruk gaya** — 57 billion loop ke baad. Dobara chalaya -> count 77 billion (har baar ALAG).
- Ek shak: har 3-sec wala `@Scheduled heartbeat` (DemoJobs) apne lock/log se cache flush kara raha tha ->
  usko comment kiya -> **phir bhi ruk gaya** (count 57B).
- `volatile` laga ke chalaya -> **wahi** `RUKA! count=19B` -> is machine pe koi VISIBLE farak nahi.

### KYUN aisa? (online verify kiya — spec + JIT)
- Bina volatile, JIT ko `running` read ko loop ke **bahar hoist** karne ki **IJAAZAT hai, MAJBOORI nahi**.
  - hoist kiya -> `if(running) while(true){}` -> **HANG forever**.
  - hoist nahi kiya (meri machine pe) -> har baar RAM check -> **ruk jata**.
- Isiliye har run me count alag (57B/77B/19B) = behavior **nondeterministic + JIT/machine-dependent**.
- "Hang hota hai" JHOOTH nahi — bahut systems (server-JVM, purane setup) pe genuinely hang hota. Bas **GUARANTEED nahi.**
- Meri machine ki JIT ne is baar hoist nahi kiya -> isliye demo "flat" gaya (dono version ek jaise ruke).

### ASLI LESSON (yehi interview me bolna)
> "Without `volatile`, cross-thread visibility is not guaranteed — it's JIT/machine dependent. The same code may
> work on one JVM and hang on another. `volatile` forces every read/write through main memory, so the flag change
> is ALWAYS seen. The guarantee is the point — not that it always breaks without it."

- **"Kaam kar raha" dekh ke ye MAT samajhna ki code SAHI hai** — visibility bug chhupa reh sakta, machine badalte hi phoot sakta.
- Farak = **GUARANTEE** (invisible jab tak koi machine hang na kare), na ki visible-behavior-change.
- Isiliye flag pe hamesha `volatile` likhte — luck pe nahi chhodte.
