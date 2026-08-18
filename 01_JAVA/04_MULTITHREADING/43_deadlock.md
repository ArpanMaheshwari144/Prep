# Deadlock — Circular Wait

> **V90 — Multithreading: Topic 43**

---

## STORY — 2 Threads Phans Gaye

→ **Thread 1** ke paas **Lock A** hai, **Lock B** chahiye
→ **Thread 2** ke paas **Lock B** hai, **Lock A** chahiye
→ Dono ek-doosre ka **wait** kar rahe — **koi release nahi karega**
→ **Both stuck. Yahi DEADLOCK hai.**

---

## Visualization

```
        ┌───────────┐         ┌───────────┐
        │ Thread A  │         │ Thread B  │
        │ holds: 1  │ ◄──┐    │ holds: 2  │
        │ waits: 2  │    │    │ waits: 1  │
        └─────┬─────┘    │    └─────┬─────┘
              │          │          │
              ▼          │          ▼
        ┌──────────┐     │    ┌──────────┐
        │ Lock 2   │     └────│ Lock 1   │
        │ (held by │          │ (held by │
        │  B)      │          │   A)     │
        └──────────┘          └──────────┘

        CIRCULAR WAIT — koi release nahi karega
```

**4 Conditions for deadlock (sab honi padti):**
1. **Mutual Exclusion** — lock exclusive
2. **Hold & Wait** — lock pakda + aur ka wait
3. **No Preemption** — lock force se nahi le sakte
4. **Circular Wait** — A→B→A wala chakkar

**Break ANY ONE = no deadlock.**

---

## Code — Deadlock Banana

```java
// DEADLOCK — alag order mein lock le rahe
// Thread 1:
synchronized(lockA) {
    synchronized(lockB) {     // B chahiye — T2 ke paas hai
        // ...
    }
}

// Thread 2:
synchronized(lockB) {
    synchronized(lockA) {     // A chahiye — T1 ke paas hai
        // ...
    }
}
// Dono wait — DEADLOCK
```

---

## Fix — Same Order Mein Lock Lo

```java
// FIX — dono threads SAME order mein
// Thread 1:
synchronized(lockA) {
    synchronized(lockB) { /* kaam */ }
}

// Thread 2 — same order (A pehle, B baad):
synchronized(lockA) {
    synchronized(lockB) { /* kaam */ }
}

// Ab circular wait IMPOSSIBLE — dono A ke liye compete karenge
// jo pehle lega wo B bhi le lega. NO DEADLOCK.
```

---

## TRAP 1 — Deadlock Silent Hai

> **Deadlock koi exception nahi throw karta — program silently HANG ho jaata.**
> **Diagnose karna mushkil — production mein detect karne ke liye thread dump dekhna padta.**

## TRAP 2 — Nested synchronized = Risk

> **Multiple `synchronized` blocks nested = deadlock risk.**
> **Avoid karo jaha tak ho sake.** Single lock se kaam chal jaata, multiple locks zaroori nahi.

---

## POWER PHRASE

> *"Deadlock occurs when two threads hold one lock each and wait for the other's lock, creating a circular wait. Prevention: always acquire locks in the same order across all threads."*

> **Yaad rakh:**
> 4 conditions: Mutual Exclusion, Hold & Wait, No Preemption, Circular Wait
> Break ANY = no deadlock
> Fix = same order lock acquire across threads
> Deadlock = silent hang, no exception
