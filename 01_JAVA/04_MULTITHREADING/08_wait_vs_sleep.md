# wait() vs sleep()

**Kahani:** office me ek hi bathroom hai, aur 3 log bahar line me khade hain.
- `sleep()`: tu andar gaya, kunda lagaya aur 5 minute so gaya. Kunda laga hua hai, to bahar wale tere uthne tak khade rahenge.
- `wait()`: tu andar gaya, dekha ki paani nahi aa raha. Tu bahar aa gaya aur bola "tum log jao, paani aaye to mujhe bula lena". Kunda khul gaya, baaki log kaam kar lete hain.

```
sleep() — lock PAKDE rakha                 wait() — lock CHHODA
A: synchronized, sleep(2000)               A: synchronized, wait()
B: same lock -> BLOCKED, 2 sec khada       B: turant andar, kaam, notify()
                                           A: jaaga, lock wapas liya, aage chala
```

**Khaas baat:**
1. Lock ka farak:
   - `sleep()` lock pakde rehta hai. Ye `Thread` ki static method hai, time khatam hote hi khud uth jaata hai.
   - `wait()` lock chhod deta hai. Ye `Object` ki method hai, jab koi `notify()` kare tab uthta hai, aur aage chalne se pehle lock dobara leta hai.
2. `wait()` aur `notify()` sirf `synchronized` ke andar chalte hain, kyunki lock tere paas hoga tabhi to chhod paayega. Bahar likha to `IllegalMonitorStateException` aata hai.
3. `wait()` ko hamesha `while` me likho, `if` me nahi. Kabhi thread bina kisi ke jagaye bhi uth jaata hai (spurious wakeup), to uthne ke baad shart dobara check karni zaroori hai: `while (queue.isEmpty()) wait();`
4. Kab kya:
   - `sleep()`: bas thodi der rukna ho (polling, retry ke beech gap).
   - `wait()`: ek thread ko doosre ke signal ka intezaar ho (producer-consumer, file 12).

**Prod wala trap:** retry ke beech `sleep` lagaya, aur wo bhi lock ya transaction ke andar.
```java
@Transactional
public void pay() {
    for (int i = 0; i < 3; i++) {
        if (psp.call()) break;
        Thread.sleep(2000);   // DB connection + row lock 6 sec tak pakde baitha
    }
}
```
Sleep lock nahi chhodta, aur yahan pakda hua hai DB connection aur row lock. Thodi bheed aayi aur Hikari pool khaali. Ye wahi baat hai jo PR drill me baar-baar aati hai: bahar ki call transaction ke bahar rakho.

Ek aur chhota trap: `catch (InterruptedException e) {}` khaali mat chhodo. Usme `Thread.currentThread().interrupt();` likho, warna rukne ka signal kho jaata hai.

```
Interview: "sleep() is a static Thread method - it pauses but keeps the lock. wait() is an Object method,
called inside synchronized - it releases the lock and resumes after notify, re-acquiring it.
sleep is for delays, wait is for thread coordination. Always wait in a while loop
because of spurious wakeups."
```


---

## GEHRA HISSA (purani file se, interview me kaam ka)

## Sabse Bada Fark — Lock Behaviour

| | `wait()` | `sleep()` |
|--|---------|-----------|
| **Class** | `Object` (lock pe call) | `Thread` (thread pe call) |
| **Lock release?** | **YES** — chhod deta | **NO** — pakad ke rakhta |
| **Wake up kaise?** | `notify()` / `notifyAll()` / timeout / `interrupt()` | Time khatam hote hi automatic, ya `interrupt()` (InterruptedException) |
| **Use case** | **Inter-thread communication** (producer-consumer) | **Just delay** (timer, polling) |
| **Kahaan call?** | **`synchronized` block ke andar MUST** | Kahin bhi |
