# synchronized: Race Condition Se Bachao

**Kahani:** ek bank account me 1000 rupaye hain. Ravi 500 daalta hai aur ek EMI 200 kaatti hai, dono bilkul ek saath. Dono thread ne balance 1000 padha. Ravi wale thread ne 1500 likha, EMI wale ne 800 likha. Jo baad me likhta hai wahi bachta hai, to final balance 800 reh gaya, jabki hona 1300 chahiye tha. Ravi ke 500 gayab ho gaye.

```
Thread 1: padha 1000 ──> +500 ──> likha 1500
Thread 2: padha 1000 ──> -200 ──> likha  800   <- baad me likha, 1500 mit gaya
```

Dikkat ye hai ki padhna, jodna aur likhna teen alag kadam hain, aur inke beech me doosra thread ghus jaata hai. `synchronized` ka matlab hai ek taala: ek thread andar gaya, to baaki thread bahar line me khade rahenge.

**Khaas baat:**

1. `synchronized` do kaam karta hai. Pehla, ek waqt pe ek hi thread andar jaata hai (atomicity). Doosra, jo ek thread ne likha wo baaki threads ko dikhta hai (visibility). `volatile` sirf doosra kaam karta hai (file 05).

2. Poori method pe lock lagane se achha hai ki sirf zaroori line pe `synchronized(this) { ... }` lagao, taaki baaki thread kam der ruke.

3. `ReentrantLock` wahi taala hai, bas extra control ke saath:
   - `tryLock(2, SECONDS)` se lock na mile to thread haar maan leta hai, hamesha intezaar nahi karta.
   - Lock ka intezaar karte thread ko interrupt kiya ja sakta hai.
   - Iski keemat ye hai ki `unlock()` ab teri zimmedari hai. Use hamesha `finally` me likho, warna exception aate hi lock hamesha ke liye phas jaata hai.

   ```java
   lock.lock();
   try { count++; } finally { lock.unlock(); }
   ```

4. Race se bachne ki do soch hain:
   - `synchronized` me pehle taala lete ho, phir kaam karte ho. Ise pessimistic kehte hain.
   - CAS ya `@Version` me bina taale ke try karte ho, aur takkar ho jaaye to dobara karte ho. Ise optimistic kehte hain. DB me iska joda hai `SELECT ... FOR UPDATE` (pessimistic) vs `WHERE version = ?` (optimistic).

**Prod wala trap:** Spring ka service bean singleton hota hai, yaani poore app me uska ek hi object hota hai.

```java
@Service
class TransferService {
    public synchronized void transfer(...) { ... }   // poore app ka EK taala
}
```

Ravi ka transfer aur Priya ka transfer ek doosre se bilkul alag hain, phir bhi dono ek line me lagenge, kyunki taala ek hi object pe hai. Aur agar 2 server chala diye, to ye taala apne server ke bahar kuch nahi rokta, kyunki dono server pe alag-alag taala hoga. Isliye banking design me lock DB me lagaya tha (atomic UPDATE ya row lock), Java me nahi.

**Tera project:** usercrud [`RateLimitController.hit()`](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/RateLimitController.java) me `synchronized` lagaya hai. Ek server pe ye theek hai. Multi-instance pe Redis counter chahiye, wahi jo rate limiter design me kiya tha.

```
Interview: "synchronized lets only one thread into the critical section and also gives visibility.
I prefer block-level sync to keep the lock short. ReentrantLock when I need tryLock with
timeout or interruptible waits - but unlock must be in finally. In a Spring singleton or
multi-instance setup, a Java lock isn't enough - I push the lock to the DB or Redis."
```


---

## GEHRA HISSA (purani file se, interview me kaam ka)

## ★★ Jab `synchronized` kaafi na ho: `ReentrantLock`

`synchronized` ko aise samajh: ek darwaza hai, ek chaabi hai, aur iske upar tera koi control nahi hai.

```
        ┌──────────────┐
  T1 ───┤              │   T1 andar gaya, darwaza band
  T2 ───┤ synchronized │   T2, T3 bahar khade, ANANT samay tak
  T3 ───┤    BLOCK     │   na nikal sakte, na pooch sakte, na cancel kar sakte
        └──────────────┘
```

### Teen cheezein jo `synchronized` me ho hi nahi sakti

**1. Wait karke haar nahi maan sakte.** Maan lo T2 khada hai aur 5 second baad bhi use chaabi nahi mili. "Chhodo, kuch aur karta hoon" bolne ka option `synchronized` me hai hi nahi. Isliye agar deadlock hua, to app hang ho jaayega aur pata bhi nahi chalega.

**2. Interrupt nahi kar sakte.** Maan lo app shutdown ho raha hai aur T2 lock ka intezaar kar raha hai. Tu `thread.interrupt()` bulayega, to kuch nahi hoga, T2 wahin khada rahega.

**3. Lock aur unlock alag jagah nahi kar sakte.** Lock lena aur chhodna usi `{ }` block ke andar hona padta hai. "Ek method me lock lo, doosre method me chhodo" wala kaam `synchronized` me ho hi nahi sakta.

### Wahi taala, par ab control tere haath me

```java
import java.util.concurrent.locks.ReentrantLock;

class Counter {
    private final ReentrantLock lock = new ReentrantLock();
    private int count = 0;

    public void increment() {
        lock.lock();              // ab ye ek CALL hai, block nahi
        try {
            count++;
        } finally {
            lock.unlock();        // ★ finally me HONA HI CHAHIYE
        }
    }
}
```

★★ **Sabse bada farak `finally` ka hai.** `synchronized` me agar exception aaye, to JVM khud lock chhod deta hai. `ReentrantLock` me lock tujhe khud chhodna hai. Agar tune `finally` nahi likha aur exception aa gaya, to lock hamesha ke liye phas jaayega, aur baaki saare thread wahin atke reh jaayenge. Matlab aazadi mili, par saath me zimmedari bhi aayi.

### Teen taakatein, code me

```java
// 1. HAAR sakta hai, intezaar hi nahi karega
if (lock.tryLock()) {                        // mila to true, warna TURANT false
    try { count++; } finally { lock.unlock(); }
} else {
    // kuch aur kar lo, ya user ko "abhi busy hai" bol do
}

// 1b. THODA rukega, phir haar maan lega
if (lock.tryLock(2, TimeUnit.SECONDS)) {
    try { count++; } finally { lock.unlock(); }
}

// 2. INTERRUPT ho sakta hai
lock.lockInterruptibly();     // intezaar me hai aur interrupt aaya to exception,
                              // thread bahar nikal aaya

// 3. FAIR taala, line lag jaati hai
ReentrantLock fair = new ReentrantLock(true);
// jo pehle aaya wo pehle jaayega.
// default false = koi line nahi, jiske haath lag gaya. TEZ hai,
// par ek badnaseeb thread baar-baar peeche reh sakta = STARVATION.
```

### "Reentrant" ka matlab (naam ka hissa hai, interview me poocha jaata hai)

```java
lock.lock();        // hold count = 1
   lock.lock();     // WAHI thread dobara -> count = 2, phansa NAHI
   lock.unlock();   // count = 1
lock.unlock();      // count = 0  -> ab doosre ko milega
```

Matlab jiske paas chaabi hai, wo usi kamre me dobara ghus sakta hai, khud se phasta nahi. `synchronized` bhi reentrant hai, wahan bhi yahi hota hai.

★ Dhyan rakhne wali baat: jitni baar lock kiya, utni hi baar unlock karna padega. Ek bhi unlock kam hua, to taala khula hi nahi, aur doosre thread ko kabhi nahi milega.

### Ek line me farak

`synchronized` saada taala hai. Ye sasta hai, surakshit hai, aur lock chhodna JVM sambhalta hai. 90% jagah yahi sahi hai.

`ReentrantLock` wahi taala hai, par saath me tryLock, timeout, interrupt aur fairness milte hain. Iski keemat ye hai ki unlock teri zimmedari hai (`finally` me), aur code lamba ho jaata hai.

**POWER PHRASE:** *"Default `synchronized` hi rakhta hoon, kyunki code kam hai aur lock release ki guarantee JVM deta hai. `ReentrantLock` tab laata hoon jab mujhe wait pe haar maanni ho, `tryLock` se, taaki thread anant intezaar me na phanse, ya lock ke intezaar ko interrupt karna ho. Iski keemat ye hai ki `unlock` ab meri zimmedari hai, isliye main use hamesha `finally` me likhta hoon."*

**AAGE:** `ReentrantLock + Condition` ka asli istemaal [12_producer_consumer.md](12_producer_consumer.md) (Approach 3) me hai. Baaki coordination tools (ReadWriteLock, Semaphore, CountDownLatch, CyclicBarrier) [14_lock_toolkit.md](14_lock_toolkit.md) me hain. Wo taale nahi hain, wo threads ka aapas me COORDINATION karte hain.

## ★ CONNECT: synchronized vs CAS (Pessimistic vs Optimistic)

usercrud idempotency me ye dono dekhe the. Ye asli farak yaad rakh, interview me follow-up me poocha jaata hai.

`synchronized` pessimistic hai: taala pehle lo, aur baaki thread block hoke wait karein. Ye safe hai par slow hai.

CAS ya atomic optimistic hai: koi taala nahi. Try karo, aur agar koi doosra beat kar gaya to retry karo. Ye fast hai aur lock-free hai.

```
synchronized -> PESSIMISTIC: taala PEHLE lo, baaki thread BLOCK ho ke wait   (safe, SLOW)
CAS/atomic   -> OPTIMISTIC:  taala NAHI. try karo, koi beat kare -> RETRY      (FAST, lock-free)
```

**Ye sab ek hi family hain.** Sabka kaam shared cheez ko bachana hai, bas tareeka alag hai:

- `synchronized` memory me taala pehle lagata hai. Ye pessimistic hai.
- `ConcurrentHashMap` bucket ke level pe lock ya CAS lagata hai, aur `putIfAbsent` atomic hota hai. Ye mostly optimistic hai.
- `AtomicInteger` CAS se compare-and-swap karta hai, bina lock ke. Counter ke liye hai, aur optimistic hai.
- Optimistic lock `@Version` DB me `UPDATE ... WHERE version = ?` chalata hai. Agar 0 rows update hui, to matlab kisi aur ne pehle badal diya, to retry karo. Ye optimistic hai.
- Pessimistic DB lock `SELECT ... FOR UPDATE` se pehle hi row lock kar leta hai. Ye pessimistic hai.

**Kab kya lagana hai:**

- Simple counter ya ek single value ho, to AtomicInteger (CAS) lagao. Lock ki zaroorat hi nahi hai.
- Multi-line critical block ho, yaani 2 ya zyada kadam ek saath atomic chahiye, to `synchronized` lagao.
- High contention ho, yaani takkar bahut hoti ho, to pessimistic lagao (pehle lock). Warna optimistic me retry pe retry hote rahenge (retry-storm).
- Low contention ho, yaani takkar kabhi-kabhi hoti ho, to optimistic (CAS ya `@Version`) lagao, wo fast hai.

Idempotency me `count.incrementAndGet()` CAS tha (counter ke liye), aur `putIfAbsent` bucket-atomic tha (map ke liye). Agar poori 3-4 line ek atomic block me chahiye hoti, to `synchronized` lagta.

## Race Condition Visualization

```
                MAIN MEMORY (RAM)
                     count = 1000
                    ↑          ↑
                    │          │
        ┌───────────┘          └───────────┐
        ▼                                  ▼
   Thread 1                            Thread 2
   padha: count = 1000                 padha: count = 1000     (dono ne SAME purana value padha)
   count++ = 1001                      count++ = 1001
   write                               write

   MAIN MEMORY = 1001  (galat, 1002 hona chahiye!)
   EK update LOST.
```

Asli jad ye hai ki padhna, +1 karna aur likhna teen alag kadam hain, aur beech me doosra thread ghus gaya. Cache na bhi ho, tab bhi ye race hoti.
