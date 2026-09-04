# join() — Thread Ke Khatam Hone Ka WAIT

> **V90 — Multithreading: Topic 40b (lifecycle ke baad — thread control)**

---

## STORY — Count Adhoora Aa Gaya

→ RaceDemo me `t1.start()`, `t2.start()` kiya — dono background me chalne lage
→ Bina wait ke `main()` method seedha `println(count)` pe pahunch gaya
→ Par threads **abhi chal rahe the** — count **0 / adhoora** print hua
→ `t1.join(); t2.join();` lagaya → `main()` **dono ke khatam hone ka wait** kiya
→ Phir print → **poora count** aaya
→ **`join()` = "jab tak thread khatam na ho, aage mat badho"**

---

## WHY — join() Kya Karta

> ★ NOTE (yaad rakh): yahan **"main"** = **`main()` method** (jo **"main thread"** pe chalta) — ye
>   angrezi word "main" hai, Hindi ka "मैं (I/me)" NAHI. har code kisi thread pe chalta; `main()` "main thread" pe.

→ `t.join()` = **calling thread (`main()` method wala = main thread) RUK jaata** jab tak thread `t` apna `run()` **poora na kar le**
→ Uske baad hi calling thread aage badhta

```
   Without join:
   main: start(t1), start(t2) --> println(count)   // threads abhi chal rahe -> ADHOORA

   With join:
   main: start(t1), start(t2) --> t1.join(), t2.join() [WAIT] --> println(count)  // POORA
```

---

## Analogy — Doston Ka Wait

→ Tu 2 doston ko kaam pe bheja (start)
→ `join` = **"main tabhi aage jaunga jab DONO ka kaam khatam ho"**
→ Meeting point pe unka wait karta, phir saath aage

---

## Code

```java
Thread t1 = new Thread(task);
Thread t2 = new Thread(task);

t1.start();     // dono PARALLEL chalu
t2.start();

t1.join();      // main RUKA -> t1 khatam hone tak
t2.join();      // main RUKA -> t2 khatam hone tak

System.out.println(count);   // ab dono done -> POORA result
```

---

## Variants

```java
t.join();        // jab tak t khatam na ho, WAIT (indefinite)
t.join(1000);    // max 1000ms wait, phir aage (chahe t khatam ho ya na)
```

---

## TRAP — join() throws InterruptedException

> **`join()` checked exception `InterruptedException` throw karta.**
> Isiliye use karne wali method me `throws InterruptedException` (ya try-catch) chahiye.

```java
public static void main(String[] args) throws InterruptedException {   // <- ye
    t1.join();
}
```

---

## TRAP 2 — join() SEQUENTIAL nahi karta

> **`t1.join(); t2.join();` threads ko ek-ke-baad-ek NAHI chalata.**
> t1 aur t2 **PARALLEL hi chalte** (saath-saath). join sirf **main ko rukata**, threads ko nahi.

```
main:  start(t1) start(t2) ──[t1.join WAIT]──[t2.join WAIT]── println
t1:              [═══════ running ═══════] done
t2:              [═══════ running ═══════] done   <- t1,t2 SAATH chal rahe
```

→ `t1.join()` pe main ruka, par **t2 tab bhi chal raha** tha saath me.
→ **Proof:** RaceDemo me race dikhi = dono ek saath chale (overlap). Sequential hote to race hoti hi nahi.

---

## TRAP 3 — join() akele CORRECT count nahi deta

> **join() = TIMING fix (dono done hone ka wait). RACE fix NAHI.**
> Correct count = `synchronized` ya `AtomicInteger (CAS)` se — wo increment ko **atomic** banate.

| | kaam | bina iske |
|--|------|-----------|
| **join** | dono khatam hone ka WAIT (timing) | main pehle padh leta -> **adhoora** |
| **synchronized/CAS** | increment ATOMIC (no lost update) | count **racy** (kam) -- chahe join ho |

→ **Exact result ke liye DONO chahiye:** join (done ke baad padho) + atomic (beech me gum na ho).

---

## POWER PHRASE

> *"`t.join()` makes the calling thread wait until thread t completes its execution. Without it, the main thread may proceed before the worker threads finish, giving incomplete results. join() only handles timing — thread-safety still needs synchronized or atomics."*

> **Yaad rakh:**
> `start()` = thread chala do (parallel)
> `join()`  = uske khatam hone ka WAIT karo
> `join(ms)` = max itna time wait
> throws `InterruptedException` (checked)
