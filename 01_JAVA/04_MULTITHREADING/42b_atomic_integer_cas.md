# AtomicInteger / CAS — Taale Ke Bina Race Fix

> **Multithreading: Topic 42b (volatile ke baad — counter ka fast + safe tareeka)**

---

## STORY — count++ ka race, bina taale

→ `count++` pe race condition aati (do thread ek saath → ek increment **gayab**)
→ Fix tha **synchronized** (taala) — par taala **slow** (ek thread kaam kare, baaki BLOCK ho ke wait karein)
→ **AtomicInteger** ek **fast** tareeka deta — **taala lagaye bina** safe increment
→ Andar **CAS (Compare-And-Swap)** use karta

---

## CAS kya hai — "shart wala likhna"

> **CAS bolta:** *"Agar value ABHI BHI woh hai jo maine padhi thi → nayi likh do.
> Agar kisi ne beech mein badal di → mat likho, RETRY karo."*

### Step-by-step (count = 5, +1 karna)

```
   1. padha current          ->  5
   2. nayi value soch        ->  6
   3. CAS: "agar abhi bhi 5 hai -> 6 likh do"
        - haan 5 hai  -> 6 ho gaya, SUCCESS ✓ -> done
        - koi aur thread beech mein 7 kar gaya -> "5 nahi hai" -> likha NAHI -> FAIL
   4. FAIL -> RETRY: dobara padho (7), soch (8), CAS "agar 7 -> 8" ... jab tak success
```

Ise **optimistic + retry (spin) loop** kehte — pehle try karo, koi beat kar gaya to dobara.

---

## Taala vs CAS — yehi asli farak

```
   synchronized -> taala lo, baaki thread BLOCK ho ke wait karein   (slow, blocking)
   CAS          -> taala NAHI. "try karo, koi beat kar gaya -> dobara try"
                   koi block nahi, koi wait nahi                     (FAST, lock-free)
```

CAS ek **single hardware (CPU) instruction** hai — CPU guarantee karta compare aur swap ke **beech koi thread ghus nahi sakta** (atomic).

---

## Analogy — Shared Doc / Version

→ Tu doc edit kar raha — tune **version 5** dekha, change banaya
→ Save pe bolta: *"sirf tab save kar jab version ABHI BHI 5 ho"*
→ Beech mein kisi ne save kiya (version 7) → tera save **reject** → tu dobara padhta + retry
→ Koi taala nahi — bas **"version match to save, warna retry"**

---

## Kab Kya Use Karein (yaad rakh)

| Situation | Tool | Kyun |
|-----------|------|------|
| **FLAG** (true/false, ek thread likhta) | `volatile` | sirf visibility chahiye |
| **COUNTER** (`count++`, numbers) | `AtomicInteger` (CAS) | fast, lock-free, atomic |
| **MULTI-LINE critical section** | `synchronized` | poora block atomic chahiye |

```java
// COUNTER ka sahi tareeka
AtomicInteger count = new AtomicInteger(0);
count.incrementAndGet();      // andar CAS -- atomic, taala nahi

// (volatile int count; count++  -> GALAT, race rahega -- volatile atomicity nahi deta)
```

---

## POWER PHRASE

> *"`AtomicInteger` uses CAS (Compare-And-Swap) — read the value, compute the new one, and write only if the value is still what we read; if another thread changed it, retry. It's a single atomic CPU instruction, lock-free, so it's faster than `synchronized` for counters."*

> **Yaad rakh:**
> CAS = "match to likho, warna retry" (optimistic, no lock)
> AtomicInteger = counter ka fast + safe tareeka (taala nahi)
> Flag → volatile · Counter → AtomicInteger · Critical section → synchronized

---

## ★ PROJECT CONNECT — usercrud Idempotency (27-Aug, khud kiya)

> Ye theory usercrud ke idempotency hands-on me LIVE chali. (poora = 04_HLD/SYSTEM_DESIGNS/17_payment_system.md)

```
IdempotencyController /pay:
  private AtomicInteger orderCounter;                  // asli order counter
  private ConcurrentHashMap<String,String> processed;  // Idempotency-Key -> result

  orderCounter.incrementAndGet();   // <- andar CAS chal raha (isiliye 20 concurrent me counter sahi, gayab nahi)
  processed.putIfAbsent(key, "PROCESSING");  // <- ConcurrentHashMap = BUCKET-level lock/CAS -> ATOMIC claim
```

**LIVE dekha (20 parallel same-key):**
- CAS/atomic version -> 1 hi order (safe). broken (containsKey+put, gap) -> 2 order = double-charge.
- `putIfAbsent` atomicity **ConcurrentHashMap se** aati (plain HashMap ka nahi) -- bilkul CAS wala "beech me koi ghus nahi sakta".

**Ek thread me 3 cousins (yaad rakh — sab "match to likho warna retry"):**
```
AtomicInteger CAS       -> memory me counter (compare value, swap)
ConcurrentHashMap       -> bucket-level lock/CAS (putIfAbsent atomic)
Optimistic Lock @Version -> DB pe (UPDATE ... WHERE version=? -> 0 rows -> retry)
```
Teeno = OPTIMISTIC (taala pehle nahi, clash pe retry). Synchronized/Pessimistic-lock = taala-pehle (blocking).
