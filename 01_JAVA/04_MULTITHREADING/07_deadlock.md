# Deadlock — Gol Chakkar Ka Intezaar

**Kahani:** banking design le. Ravi Priya ko 100 bhejta hai, aur usi pal Priya bhi Ravi ko 50 bhejti hai. Pehla thread pehle Ravi ka account lock karta hai, phir Priya ka maangta hai. Doosra thread pehle Priya ka lock karta hai, phir Ravi ka maangta hai. Dono ek doosre ka intezaar karte rehte hain aur koi apna lock nahi chhodta. Na koi exception aata hai, na koi log, app bas chup ho jaata hai.

```
T1: holds Ravi  ──waits──> Priya
T2: holds Priya ──waits──> Ravi      <- gol chakkar, koi nahi chhodega
```

**Tera practical:** [DeadlockDemo](practical/DeadlockDemo.java) chalao to program hang hota hai. `jstack <pid>` se thread dump:
```
Found one Java-level deadlock:
"Thread-0": waiting to lock monitor ... which is held by "Thread-1"
"Thread-1": waiting to lock monitor ... which is held by "Thread-0"
   at DeadlockDemo.lambda$main$0(DeadlockDemo.java:29)
```

**Khaas baat:**
1. Deadlock ke liye 4 shart chahiye:
   - Lock ek waqt pe ek hi thread ke paas ho sakta hai.
   - Thread ek lock pakad ke doosre ka intezaar karta hai.
   - Kisi se lock zabardasti chheena nahi ja sakta.
   - Intezaar gol chakkar me hai.

   Inme se koi ek bhi tod do to deadlock nahi hoga.
2. Sabse aasaan ilaaj: saare thread lock hamesha ek hi kram me lein. Tab gol chakkar ban hi nahi sakta.
3. Deadlock exception nahi deta, app bas chupchaap hang ho jaata hai. Pakadne ka tareeka thread dump hai, jaise upar `jstack` ne seedha "Found one Java-level deadlock" bol diya. File 02 ki BLOCKED state yahi thi.
4. File 04 ka `tryLock(timeout)` bhi ilaaj hai. Lock time pe na mile to thread haar maan leta hai, matlab "pakad ke intezaar" wali shart toot jaati hai.

**Prod wala trap:** transfer me lock ka kram "pehle bhejne wala, phir lene wala" rakha, to ulti disha wale do transfer ek doosre ko phasa denge. Kram account ki id se tay karo, kaun bhej raha hai usse nahi:
```java
Account first  = from.id < to.id ? from : to;   // chhoti id pehle, hamesha
Account second = from.id < to.id ? to : from;
synchronized (first) { synchronized (second) { /* transfer */ } }
```
DB me bhi bilkul yahi hota hai. Do transaction ulte kram me row update karein to DB deadlock pakad ke ek transaction ko rollback kar deta hai (09_DATABASE/08 deadlock).

```
Interview: "Deadlock is a circular wait - each thread holds one lock and waits for the other's.
It needs 4 conditions; breaking any one prevents it. Simplest fix is a global lock order,
e.g. always lock the lower account id first in a transfer. tryLock with timeout also works.
It doesn't throw - it silently hangs, so I detect it with a thread dump (jstack)."
```


---

## GEHRA HISSA (purani file se, interview me kaam ka)

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
