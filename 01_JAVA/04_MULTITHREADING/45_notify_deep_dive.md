# notify() Deep Dive — Internally Kya Hota?

> **V90 — Multithreading: Topic 45 (deep extension of `wait()/notify()`)**

---

## `notify()` Internally Kya Karta?

### Pehle 2 Concepts Clear

**1. Lock object kaun hai?**

`synchronized void produce(...)` likhna = **`synchronized(this)`** likhne ke barabar
- `this` = Buffer ka instance
- Lock **Buffer object** pe hai

**2. Har object ke paas 2 cheez hoti hain:**
- **Lock (monitor)** — ek time pe ek hi thread le sakta
- **Wait Set** — internal list of threads jo `wait()` kar rahe hain is object pe

---

## Step-by-Step Flow

### Scene Set: 2 threads — Producer (P) + Consumer (C). Buffer queue full hai.

---

### Step 1: Producer enters `produce()`

```
P: produce() call
   → P ne `this` (Buffer) ka LOCK liya
   → State: RUNNING
   → Queue check: FULL — `wait()` call
```

### Step 2: `wait()` ka 3 kaam

Jab `wait()` call hota, **3 cheez ek saath hoti**:

1. **Lock RELEASE** — Buffer ka lock chhod deta
2. **WAITING state** mein chala jaata
3. Buffer ka **Wait Set** mein add ho jaata

```
Buffer's Wait Set:  [P]   ← Producer yahan baitha hai
P state:            WAITING
P holding lock?     NO (release kar diya)
```

---

### Step 3: Consumer enters `consume()`

```
C: consume() call
   → C ne `this` (Buffer) ka LOCK liya  (P ne chhod diya tha)
   → State: RUNNING
   → Queue se item nikaala (poll)
   → ab queue full nahi hai
```

---

### Step 4: Consumer calls `notify()`

**Yahan asli magic hai. `notify()` exactly ye karta:**

1. Buffer ke **Wait Set** mein dekho — koi thread hai?
2. Ek thread **pick karo** (ye P hai)
3. P ko **WAITING → BLOCKED** state mein move karo
4. **LEKIN — lock abhi BHI Consumer ke paas hai!**

```
Buffer's Wait Set:  []     ← P nikal gaya
P state:            BLOCKED  (lock ke liye compete kar raha)
C state:            RUNNING (lock abhi bhi C ke paas)
```

> **Important:** `notify()` ne sirf **"jaago aur lock ke liye line mein lago"** bola. **Lock immediately P ko nahi mila.**

---

### Step 5: Consumer exits `synchronized` block

```
C: } method end
   → Lock RELEASE
   → State: continue
```

---

### Step 6: Producer (BLOCKED) gets the lock

```
P: lock mil gaya
   → State: BLOCKED → RUNNABLE → RUNNING
   → wait() ke baad continue kar raha
   → while loop re-check: queue full hai abhi? → NAHI
   → loop exit, queue.add() chala, notify() chala...
```

---

## Visual Summary

```
WAITING SET                  RUNNING                   LOCK

┌──────────┐                                         ┌──────┐
│   P      │  ◄── wait()      C: lock =          │  C   │
│ WAITING  │                  C: poll()              └──────┘
└──────────┘                  C: notify()

                   ↓ notify() picks P

┌──────────┐                                         ┌──────┐
│  empty   │      P: BLOCKED                         │  C   │
└──────────┘      (wait set se nikla)                └──────┘

                   ↓ C exits sync (lock release)

                                                     ┌──────┐
                  P: BLOCKED → RUNNING               │  P   │
                  P: wait() ke baad chala            └──────┘
```

---

## Key Insights

| Confusion | Reality |
|-----------|---------|
| `notify()` = thread immediately wake up? | **NAHI** — sirf BLOCKED state mein move karta. Lock ke liye line mein khada |
| `notify()` ke baad notifier ka lock release? | **NAHI** — notifier sync block khatam hone tak lock pakda rahega |
| Thread `wait()` se kaha resume karega? | **`wait()` ke just baad** — wahin se while loop dobara check |
| `notify()` vs `notifyAll()`? | `notify()` = **ek** thread jagao. `notifyAll()` = **sab** wait set ke threads jagao |

---

## 1-Line Truth

> **`notify()` ka kaam:** *"Wait set se ek thread nikaalo, BLOCKED state mein daalo. Jab notifier sync block exit kare aur lock chhode, tab wo BLOCKED thread lock le ke continue karega."*

---

## `notifyAll()` Kab Better?

Producer-consumer mein agar **multiple producers + multiple consumers** hain — `notify()` se **galat thread jag sakta** (e.g., consumer ne `notify()` kiya consumer ko hi jag gaya — useless).

**Best practice:** `notifyAll()` use karo — sab jagao, jisko condition match karegi wo kaam karega.

---

## POWER PHRASE

> *"`notify()` removes one thread from the object's wait set and moves it to BLOCKED state — but the notifier still holds the lock until it exits the synchronized block. Only then can the woken thread reacquire the lock and resume from where `wait()` returned."*
