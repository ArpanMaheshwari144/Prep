# Deadlock: Gol Chakkar Ka Intezaar

**Kahani:** banking design le. Ravi Priya ko 100 bhejta hai, aur usi pal Priya bhi Ravi ko 50 bhejti hai. Pehla thread pehle Ravi ka account lock karta hai, phir Priya ka maangta hai. Doosra thread pehle Priya ka account lock karta hai, phir Ravi ka maangta hai. Ab dono ek doosre ka intezaar karte rehte hain, aur koi bhi apna lock nahi chhodta. Na koi exception aata hai, na koi log, app bas chup ho jaata hai.

```
T1: holds Ravi  ──waits──> Priya
T2: holds Priya ──waits──> Ravi      <- gol chakkar, koi nahi chhodega
```

**Tera practical:** [DeadlockDemo](practical/DeadlockDemo.java) chalao to program hang ho jaata hai. Tab `jstack <pid>` se thread dump nikaalo, to ye dikhta hai:

```
Found one Java-level deadlock:
"Thread-0": waiting to lock monitor ... which is held by "Thread-1"
"Thread-1": waiting to lock monitor ... which is held by "Thread-0"
   at DeadlockDemo.lambda$main$0(DeadlockDemo.java:29)
```

**Khaas baat:**

1. Deadlock hone ke liye 4 shart ek saath chahiye:
   - Lock ek waqt pe ek hi thread ke paas ho sakta hai.
   - Thread ek lock pakad ke doosre lock ka intezaar karta hai.
   - Kisi thread se lock zabardasti chheena nahi ja sakta.
   - Intezaar gol chakkar me hai.

   Inme se koi ek bhi shart tod do, to deadlock nahi hoga.

2. Sabse aasaan ilaaj ye hai ki saare thread lock hamesha ek hi kram me lein. Tab gol chakkar ban hi nahi sakta.

3. Deadlock koi exception nahi deta, app bas chupchaap hang ho jaata hai. Ise pakadne ka tareeka thread dump hai, jaise upar `jstack` ne seedha "Found one Java-level deadlock" bol diya. File 02 ki BLOCKED state yahi thi.

4. File 04 ka `tryLock(timeout)` bhi ek ilaaj hai. Lock time pe na mile to thread haar maan leta hai, matlab "lock pakad ke intezaar karna" wali shart toot jaati hai.

**Prod wala trap:** agar transfer me lock ka kram "pehle bhejne wala, phir lene wala" rakha, to ulti disha wale do transfer ek doosre ko phasa denge, kyunki Ravi se Priya wale ke liye pehla account Ravi hai aur Priya se Ravi wale ke liye pehla account Priya. Isliye kram account ki id se tay karo, kaun bhej raha hai usse nahi:

```java
Account first  = from.id < to.id ? from : to;   // chhoti id pehle, hamesha
Account second = from.id < to.id ? to : from;
synchronized (first) { synchronized (second) { /* transfer */ } }
```

DB me bhi bilkul yahi hota hai. Agar do transaction ulte kram me row update karein, to DB deadlock pakad leta hai aur ek transaction ko rollback kar deta hai (09_DATABASE/08 deadlock).

```
Interview: "Deadlock is a circular wait - each thread holds one lock and waits for the other's.
It needs 4 conditions; breaking any one prevents it. Simplest fix is a global lock order,
e.g. always lock the lower account id first in a transfer. tryLock with timeout also works.
It doesn't throw - it silently hangs, so I detect it with a thread dump (jstack)."
```


---

## GEHRA HISSA (purani file se, interview me kaam ka)

## Code: deadlock kaise banta hai

Yahan dono thread alag order me lock le rahe hain. Thread 1 pehle A leta hai phir B maangta hai, aur Thread 2 pehle B leta hai phir A maangta hai. Dono ek doosre ka lock pakad ke baithe hain, isliye dono hamesha wait karte rahenge.

```java
// DEADLOCK: alag order mein lock le rahe
// Thread 1:
synchronized(lockA) {
    synchronized(lockB) {     // B chahiye, par T2 ke paas hai
        // ...
    }
}

// Thread 2:
synchronized(lockB) {
    synchronized(lockA) {     // A chahiye, par T1 ke paas hai
        // ...
    }
}
// Dono wait, DEADLOCK
```

---

## Fix: dono same order me lock lo

Ab dono thread pehle A lete hain aur phir B. Isliye dono A ke liye hi takrayenge. Jo A pehle le lega, wahi B bhi le lega, aur doosra bas A ka intezaar karega. Gol chakkar ban hi nahi sakta, isliye deadlock nahi hoga.

```java
// FIX: dono threads SAME order mein
// Thread 1:
synchronized(lockA) {
    synchronized(lockB) { /* kaam */ }
}

// Thread 2, same order (A pehle, B baad):
synchronized(lockA) {
    synchronized(lockB) { /* kaam */ }
}

// Ab circular wait IMPOSSIBLE, dono A ke liye compete karenge
// jo pehle lega wo B bhi le lega. NO DEADLOCK.
```

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

        CIRCULAR WAIT, koi release nahi karega
```
