# join() — Thread Ke Khatam Hone Ka WAIT

> **V90 — Multithreading: Topic 40b (lifecycle ke baad — thread control)**

---

## STORY — Count Adhoora Aa Gaya

→ RaceDemo me `t1.start()`, `t2.start()` kiya — dono background me chalne lage
→ Bina wait ke `main` seedha `println(count)` pe pahunch gaya
→ Par threads **abhi chal rahe the** — count **0 / adhoora** print hua
→ `t1.join(); t2.join();` lagaya → main **dono ke khatam hone ka wait** kiya
→ Phir print → **poora count** aaya
→ **`join()` = "jab tak thread khatam na ho, aage mat badho"**

---

## WHY — join() Kya Karta

→ `t.join()` = **calling thread (jaise main) RUK jaata** jab tak thread `t` apna `run()` **poora na kar le**
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

## POWER PHRASE

> *"`t.join()` makes the calling thread wait until thread t completes its execution. Without it, the main thread may proceed before the worker threads finish, giving incomplete results."*

> **Yaad rakh:**
> `start()` = thread chala do (parallel)
> `join()`  = uske khatam hone ka WAIT karo
> `join(ms)` = max itna time wait
> throws `InterruptedException` (checked)
