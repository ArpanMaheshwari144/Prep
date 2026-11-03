# wait() vs sleep()

## Kahani: office ka ek hi bathroom

Office me ek hi bathroom hai, aur bahar 3 log line me khade hain.

- `sleep()` wala case: tu andar gaya, kunda lagaya aur 5 minute ke liye so gaya. Kunda laga hua hai, isliye bahar wale tere uthne tak khade hi rahenge.
- `wait()` wala case: tu andar gaya aur dekha ki paani hi nahi aa raha. Tu bahar aa gaya aur bola "tum log jao, paani aaye to mujhe bula lena". Kunda khul gaya, to baaki log apna kaam kar lete hain.

```
sleep() — lock PAKDE rakha                    wait() — lock CHHOD diya
A: synchronized ke andar sleep(2000)          A: synchronized ke andar wait()
B: same lock chahiye -> BLOCKED, 2 sec khada  B: turant andar gaya, kaam kiya, notify()
                                              A: jaaga, lock wapas liya, aage chala
```

## Khaas baat

**1. Lock ka farak.** `sleep()` lock pakde rehta hai. Ye `Thread` class ki static method hai, aur time khatam hote hi thread apne aap uth jaata hai. `wait()` lock chhod deta hai. Ye `Object` class ki method hai, aur thread tab uthta hai jab koi `notify()` kare. Uthne ke baad aage chalne se pehle wo lock dobara leta hai.

**2. `wait()` aur `notify()` sirf `synchronized` ke andar chalte hain.** Wajah simple hai: lock chhodne ke liye pehle lock tere paas hona chahiye. Agar tune inhe `synchronized` ke bahar likha, to `IllegalMonitorStateException` aata hai.

**3. `wait()` ko hamesha `while` me likho, `if` me nahi.** Kabhi-kabhi thread bina kisi ke jagaye bhi uth jaata hai, isko spurious wakeup bolte hain. Isliye uthne ke baad shart dobara check karna zaroori hai, aise: `while (queue.isEmpty()) wait();`

**4. Kab kya lagana hai.** Agar bas thodi der rukna hai, jaise polling me ya retry ke beech gap dena hai, to `sleep()` lagao. Agar ek thread ko doosre thread ke signal ka intezaar karna hai, jaise producer-consumer me (file 12), to `wait()` lagao.

## Prod wala trap: retry ke beech sleep, wo bhi lock ya transaction ke andar

```java
@Transactional
public void pay() {
    for (int i = 0; i < 3; i++) {
        if (psp.call()) break;
        Thread.sleep(2000);   // DB connection + row lock 6 sec tak pakde baitha
    }
}
```

Sleep lock nahi chhodta, aur yahan jo pakda hua hai wo DB connection aur row lock hai. Matlab har request 6 second tak connection pakad ke baithi hai. Thodi si bheed aayi, aur Hikari pool khaali ho gaya. Ye wahi baat hai jo PR drill me baar-baar aati hai: bahar ki call ko transaction ke bahar rakho.

Ek aur chhota trap: `catch (InterruptedException e) {}` ko khaali mat chhodo. Usme `Thread.currentThread().interrupt();` likho, warna thread ko rukne ka jo signal mila tha wo kho jaata hai.

## Interview line

```
Interview: "sleep() is a static Thread method - it pauses but keeps the lock. wait() is an Object method,
called inside synchronized - it releases the lock and resumes after notify, re-acquiring it.
sleep is for delays, wait is for thread coordination. Always wait in a while loop
because of spurious wakeups."
```


---

## GEHRA HISSA (purani file se, interview me kaam ka)

## Sabse bada farak: lock ke saath kya hota hai

- **Kis class ka hai:** `wait()` `Object` class ka method hai, aur ise lock wale object pe call karte hain. `sleep()` `Thread` class ka method hai, aur ye thread pe call hota hai.
- **Lock chhodta hai ya nahi:** `wait()` lock chhod deta hai. `sleep()` lock pakad ke rakhta hai.
- **Uthta kaise hai:** `wait()` wala thread `notify()` ya `notifyAll()` se, timeout khatam hone se, ya `interrupt()` aane se uthta hai. `sleep()` wala thread time khatam hote hi apne aap uth jaata hai, ya `interrupt()` aane pe `InterruptedException` ke saath uthta hai.
- **Kis kaam ke liye:** `wait()` threads ke beech baat-cheet ke liye hai (inter-thread communication, jaise producer-consumer). `sleep()` bas der lagane ke liye hai, jaise timer ya polling.
- **Kahan call kar sakte ho:** `wait()` ko `synchronized` block ke andar hi bulana MUST hai. `sleep()` kahin bhi bula sakte ho.

## Trap: synchronized ke andar sleep() lock nahi chhodta (bathroom wala code)

Neeche ke code me pehle method me thread so raha hai par lock pakde hue hai, isliye baaki sab bahar atke rahenge. Doosre method me `wait()` lock chhod deta hai, isliye baaki log andar aa sakte hain.

```java
public class Bathroom {

    // sleep — lock held, others blocked
    public synchronized void useBathroomSleep() throws InterruptedException {
        System.out.println("In bathroom, taking 5 sec nap");
        Thread.sleep(5000);                  // ← lock HELD during nap
        System.out.println("Done");
    }

    // wait — lock released, others can enter
    public synchronized void useBathroomWait() throws InterruptedException {
        System.out.println("Waiting for signal");
        wait();                              // ← lock RELEASED
        System.out.println("Got signal, proceeding");
    }
}
```

## Exception kaunsa aata hai, aur thread kahan se dobara chalta hai

- **Exception:** `Thread.sleep()` sirf `InterruptedException` deta hai. `Object.wait()` bhi `InterruptedException` deta hai, aur agar tune ise synchronized ke bahar bulaya to `IllegalMonitorStateException` bhi deta hai.
- **Dobara kahan se chalta hai:** `sleep()` wala thread sleep ka time poora hone ke baad usi line se aage chal deta hai. `wait()` wala thread pehle lock dobara leta hai (re-acquire), aur uske baad hi aage chalta hai.

## Teen chhote trap, aur yaad rakhne ka tareeka

**Trap 1:** log sochte hain ki `wait()` hamesha ke liye atak jaata hai. Aisa zaroori nahi, kyunki `wait(timeout)` wala overload bhi hai. `wait(5000)` ka matlab hai ki thread zyada se zyada 5 second rukega.

**Trap 2:** log sochte hain ki `sleep(0)` turant return ho jaata hai. Asal me `sleep(0)` scheduler ko ek hint deta hai, aur ye lagbhag `Thread.yield()` jaisa kaam karta hai (approximately).

**Trap 3:** `InterruptedException` ko ignore karna anti-pattern hai. Catch me `Thread.currentThread().interrupt()` likh ke interrupt ka flag wapas set karo.

**Yaad rakhne ke liye:** `sleep()` ek "selfish nap" hai. Ye `Thread` class ka hai aur lock pakde rehta hai. `wait()` ek "generous wait" hai. Ye `Object` class ka hai aur lock chhod deta hai.

## sleep() ka sahi use: API rate limiter

Yahan har API call ke baad 100ms ka gap rakhna hai, aur koi lock pakda hua nahi hai, isliye `sleep` bilkul theek hai.

```java
// API rate limiter — sleep
for (Request req : requests) {
    callApi(req);
    Thread.sleep(100);  // 100ms gap
}
```
