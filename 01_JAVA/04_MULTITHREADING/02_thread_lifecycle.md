# Thread Lifecycle: 6 States (Thread.State)

## Scene: mart ke cashier ka din

Mart ke ek cashier ka poora din dekho, to wahi 6 haalat dikhti hain:

```
Naukri pe rakh liya, counter pe abhi baitha nahi       -> NEW
Counter pe hai, billing kar raha ya customer ka wait   -> RUNNABLE
Galle ki chaabi chahiye, par doosre cashier ke paas    -> BLOCKED
Mausi bolti hai "jab tak na bulaun, ruk" (koi time nahi) -> WAITING
"5 minute ki chai", time khatam to khud wapas         -> TIMED_WAITING
Shift khatam, ghar chala gaya                          -> TERMINATED
```

Chala ke dekha:

```
bana, start nahi:       NEW
sleep(500) me:          TIMED_WAITING
wait() bina time:       WAITING
galla kisi aur ke paas: BLOCKED
kaam khatam:            TERMINATED
dobara start():         IllegalThreadStateException
```

## Khaas baat

Java me kul 6 haalat hain. RUNNING naam ki koi alag haalat nahi hai. Billing ho rahi ho ya customer ka intezaar, dono
RUNNABLE hi kehlaate hain.

Rukna teen tarah ka hota hai. BLOCKED ka matlab hai ki `synchronized` ka lock doosre thread ke paas hai. WAITING ka matlab
hai bina time ka intezaar, koi jagayega tabhi uthega (`wait()`, `join()`). TIMED_WAITING ka matlab hai time wala intezaar,
time khatam hote hi thread khud uth jaata hai (`sleep(ms)`, `wait(ms)`, `join(ms)`).

Shift khatam ho gayi to wahi cashier dobara nahi bulaya ja sakta. TERMINATED thread pe `start()` karne se
`IllegalThreadStateException` aata hai, isliye dobara kaam chahiye to naya Thread banana padta hai.

## Prod wala trap

App hang ho gaya, request ka jawab nahi aa raha. Aise waqt thread dump nikaalte hain (`jstack <pid>`) aur yahi haalat
padhte hain. Agar 200 thread BLOCKED hain aur sab ek hi lock pe ruke hain, to koi ek thread chaabi leke baitha hai, ya
deadlock hai (file 07). Agar bahut saare thread connection pool pe ruke hain, to pool khatam ho gaya hai. Ye wahi tera
HikariCP wala kissa hai. HikariCP connection ka intezaar ek time-limit (`connectionTimeout`, default 30 sec) ke saath karta
hai, isliye dump me ye thread TIMED_WAITING dikhte hain.

Matlab ye 6 haalat ratne ki cheez nahi hain. Hang debug karte waqt thread dump me yahi padhi jaati hain.
