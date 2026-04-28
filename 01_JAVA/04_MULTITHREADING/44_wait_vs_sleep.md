# wait() vs sleep()

> **V90 — Multithreading: Topic 44**

---

## 🟡 WHY — Dono Thread Rokte Hain Lekin Alag Tareeke Se

→ Dono thread ko **rokne** ke liye hain, **lekin reason ALAG**
→ **`sleep()`** = main **thak gaya, thodi der rest karunga, lock NAHI chhodunga**
→ **`wait()`** = main **doosre ka wait kar raha hoon — tab tak lock CHHOD deta hoon**

---

## 🆚 Sabse Bada Fark — Lock Behaviour

| | `wait()` | `sleep()` |
|--|---------|-----------|
| **Class** | `Object` (lock pe call) | `Thread` (thread pe call) |
| **Lock release?** | ✅ **YES** — chhod deta | ❌ **NO** — pakad ke rakhta |
| **Wake up kaise?** | `notify()` / `notifyAll()` / timeout | Time khatam hote hi automatic |
| **Use case** | **Inter-thread communication** (producer-consumer) | **Just delay** (timer, polling) |
| **Kahaan call?** | **`synchronized` block ke andar MUST** | Kahin bhi |

---

## 💻 Code

### sleep() — thread thoda ruk
```java
Thread.sleep(2000);    // 2 second pause, lock NAHI chhodta
```

### wait() — lock chhod ke wait
```java
synchronized(this) {
    while (queue.isEmpty()) {
        wait();               // lock chhoda, doosra thread aa sakta
    }
    process(queue.poll());
}
```

### notify() — wait kar rahe ko jagao
```java
synchronized(this) {
    queue.add(item);
    notify();                 // ya notifyAll() — sab ko jagao
}
```

---

## 📖 Producer-Consumer Pattern (Real Use Case)

```java
class Buffer {
    Queue<Integer> queue = new LinkedList<>();
    int LIMIT = 5;

    synchronized void produce(int item) throws InterruptedException {
        while (queue.size() == LIMIT) {
            wait();                   // bhar gaya — consumer ka wait
        }
        queue.add(item);
        notify();                     // consumer ko jagao
    }

    synchronized int consume() throws InterruptedException {
        while (queue.isEmpty()) {
            wait();                   // khali — producer ka wait
        }
        int item = queue.poll();
        notify();                     // producer ko jagao
        return item;
    }
}
```

> **`notify()` exactly kya karta — deep flow ke liye file 45 padho.**

---

## 🔴 TRAP 1 — `wait()` Sirf `synchronized` Block Mein

> **`wait()` aur `notify()` ka use SIRF `synchronized` block ke andar.**
> **Bahar likhega → `IllegalMonitorStateException`.**

## 🔴 TRAP 2 — Naam Mix Mat Karna

> **`wait()` = `Object` class** (lock pe call hota)
> **`sleep()` = `Thread` class** (static method)
> **Interview mein specifically poochhte.**

## 🔴 TRAP 3 — `wait()` While Loop Mein

> **`wait()` ko `while` mein lapeto, `if` nahi** — "spurious wakeup" bug se bachao.

```java
// 🔴 GALAT
if (queue.isEmpty()) wait();         // spurious wakeup possible

// ✅ SAHI
while (queue.isEmpty()) wait();      // condition recheck after wakeup
```

---

## 💬 POWER PHRASE

> *"`wait()` is an Object class method called inside `synchronized` — it releases the lock and waits for `notify()`. `sleep()` is a Thread class static method that pauses the thread but holds the lock. `wait()` is for inter-thread communication; `sleep()` is just a delay."*

> **Yaad rakh:**
> wait() = lock RELEASE, Object class, synchronized only, notify() se jaago
> sleep() = lock HOLD, Thread class, kahin bhi, time khatam = jaago
> wait() — while loop mein wrap karo
