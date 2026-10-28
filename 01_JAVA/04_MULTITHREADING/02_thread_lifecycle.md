# Thread Lifecycle — 6 States (Thread.State)

**Kahani:** file 01 wala thread hi lo, jo orders laane jaata hai. `new Thread()` likha, to thread sirf bana hai, abhi chala nahi (NEW). `start()` hua to wo line me lag gaya, kabhi CPU milta hai kabhi nahi, Java dono ko ek hi naam deta hai: RUNNABLE. Beech me wo ruk sakta hai, aur rukne ki 3 wajah hoti hain. Kaam poora hua to TERMINATED, aur ye thread ab dobara nahi chalega.

```
NEW --start()--> RUNNABLE --run() poora--> TERMINATED
                    |
     ┌──────────────┼─────────────────┐
  BLOCKED        WAITING         TIMED_WAITING
  lock ke liye   wait() / join()  sleep(ms) / wait(ms)
  ruka           koi jagaye tab   time khatam to khud uthe
```

**Khaas baat:**
1. Java me kul 6 state hain. RUNNING naam ki alag state nahi hai, wo RUNNABLE ke andar hi aati hai.
2. Teen tarah ka rukna:
   - BLOCKED: `synchronized` ka lock chahiye, par wo kisi aur thread ke paas hai.
   - WAITING: bina time ka intezaar, koi jagaye tabhi uthega.
   - TIMED_WAITING: time wala intezaar, time khatam hote hi khud uth jaata hai.
3. Ek baar TERMINATED ho gaya to us thread pe dobara `start()` nahi chalta (`IllegalThreadStateException`). Naya Thread object banana padta hai.

**Prod wala trap:** maan le app hang ho gaya aur request ka jawab nahi aa raha. Tab thread dump nikaalte hain (`jstack <pid>`). Agar 200 thread BLOCKED dikhein aur sab ek hi lock pe ruke hon, to koi ek thread wo lock pakad ke baitha hai, ya phir deadlock hai (file 07). Agar sab thread WAITING me hon aur pool se connection maang rahe hon, to connection pool khatam ho gaya hai, tera HikariCP wala kissa. Matlab ye 6 state ratne ki cheez nahi hain, hang debug karte waqt thread dump me yahi padhte hain.

```
Interview: "A Java thread has 6 states: NEW, RUNNABLE, BLOCKED, WAITING, TIMED_WAITING, TERMINATED.
There is no separate RUNNING state - RUNNABLE covers both ready and on-CPU.
BLOCKED = waiting for a monitor lock, WAITING = no timeout, TIMED_WAITING = with timeout.
In production I read these states in a thread dump to debug hangs."
```
