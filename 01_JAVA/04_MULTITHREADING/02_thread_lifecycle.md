# Thread Lifecycle: 6 States (Thread.State)

**Kahani:** file 01 wala thread hi le le, jo orders laane jaata hai. Jab tune `new Thread()` likha, to thread sirf bana hai, abhi chala nahi hai. Ye NEW state hai.

Jab `start()` hua, to wo line me lag gaya. Kabhi use CPU milta hai, kabhi nahi milta, par Java dono haalat ko ek hi naam deta hai: RUNNABLE.

Beech me thread ruk bhi sakta hai, aur rukne ki 3 wajah hoti hain (neeche tasveer me). Jab kaam poora ho gaya to thread TERMINATED ho jaata hai, aur ye thread ab dobara nahi chalega.

```
NEW --start()--> RUNNABLE --run() poora--> TERMINATED
                    |
     ┌──────────────┼─────────────────┐
  BLOCKED        WAITING         TIMED_WAITING
  lock ke liye   wait() / join()  sleep(ms) / wait(ms)
  ruka           koi jagaye tab   time khatam to khud uthe
```

**Khaas baat:**

1. Java me kul 6 state hain. RUNNING naam ki koi alag state nahi hai, wo RUNNABLE ke andar hi aati hai.

2. Rukna teen tarah ka hota hai:
   - BLOCKED ka matlab hai ki thread ko `synchronized` ka lock chahiye, par wo lock abhi kisi aur thread ke paas hai.
   - WAITING ka matlab hai bina time ka intezaar. Koi doosra jagaye, tabhi ye uthega.
   - TIMED_WAITING ka matlab hai time wala intezaar. Time khatam hote hi thread khud uth jaata hai.

3. Ek baar thread TERMINATED ho gaya, to us thread pe dobara `start()` nahi chalta, `IllegalThreadStateException` aata hai. Dobara chalana hai to naya Thread object banana padta hai.

**Prod wala trap:** maan le app hang ho gaya aur request ka jawab hi nahi aa raha. Aise waqt thread dump nikaalte hain (`jstack <pid>`).

Agar dump me 200 thread BLOCKED dikhein aur sab ek hi lock pe ruke hon, to iska matlab hai ki koi ek thread wo lock pakad ke baitha hai, ya phir deadlock hai (file 07).

Agar saare thread WAITING me hon aur pool se connection maang rahe hon, to matlab connection pool khatam ho gaya hai. Ye wahi tera HikariCP wala kissa hai.

Matlab ye 6 state ratne ki cheez nahi hain. Hang debug karte waqt thread dump me yahi state padhte hain.

```
Interview: "A Java thread has 6 states: NEW, RUNNABLE, BLOCKED, WAITING, TIMED_WAITING, TERMINATED.
There is no separate RUNNING state - RUNNABLE covers both ready and on-CPU.
BLOCKED = waiting for a monitor lock, WAITING = no timeout, TIMED_WAITING = with timeout.
In production I read these states in a thread dump to debug hangs."
```


---

## GEHRA HISSA (purani file se, interview me kaam ka)

## Har state kaise aati hai aur uska matlab kya hai (6, sahi wali list)

- **NEW:** tune `new Thread()` se thread bana diya, par abhi `start()` nahi kiya. Thread bana hai, chala nahi.
- **RUNNABLE:** tune `start()` call kar diya. Ab thread ya to CPU milne ke liye ready baitha hai, ya CPU pe chal raha hai. Dono haalat isi ek state me aati hain.
- **BLOCKED:** thread `synchronized` ka lock maang raha hai. Use monitor lock chahiye, par wo doosre thread ke paas hai.
- **WAITING:** thread ne `wait()`, `join()` ya `park()` bina timeout ke bulaya. Ye bina had ka intezaar hai, koi jagayega tabhi uthega.
- **TIMED_WAITING:** thread ne `sleep(ms)`, `wait(ms)` ya `join(ms)` bulaya. Ye timeout wala intezaar hai, waqt poora hote hi thread khud uth jaata hai.
- **TERMINATED:** `run()` poora ho gaya. Thread khatam.

★ **BLOCKED vs WAITING vs TIMED_WAITING** (grill me ye sabse zyada poocha jaata hai):

BLOCKED me thread `synchronized` ke monitor lock ka intezaar kar raha hai, jo kisi aur ne pakad rakha hai. WAITING me thread ne `wait()` ya `join()` bina time ke bulaya hai, to wo tab tak ruka rahega jab tak notify na aaye ya jiska wait hai wo thread khatam na ho. TIMED_WAITING me `sleep(ms)`, `wait(ms)` ya `join(ms)` hai, to time khatam hote hi thread khud wapas aa jaata hai.

```
BLOCKED       -> synchronized MONITOR-lock ka intezaar (koi aur pakad ke baitha hai)
WAITING       -> wait()/join() bina time, jab tak notify/end na ho
TIMED_WAITING -> sleep(ms)/wait(ms)/join(ms), time khatam hote hi khud wapas
```

## Visualization

```
┌─────────┐ start() ┌────────────┐        ┌──────────────┐
│   NEW   │────────►│  RUNNABLE  │        │  TERMINATED  │
│(created)│         │(ready + on │ run()  │  (khatam)    │
└─────────┘         │  CPU dono) │───────►└──────────────┘
                    └─────┬──────┘
        ┌─────────────────┼──────────────────┐
        ▼                 ▼                  ▼
  ┌───────────┐    ┌────────────┐    ┌──────────────────┐
  │  BLOCKED  │    │  WAITING   │    │  TIMED_WAITING   │
  │(lock ke   │    │(wait()/    │    │(sleep(ms)/       │
  │ liye ruka)│    │ join() —   │    │ wait(ms)/        │
  │           │    │ no timeout)│    │ join(ms))        │
  └───────────┘    └────────────┘    └──────────────────┘
```
