# synchronized — Race Condition Se Bachao

**Kahani:** ek bank account me 1000 rupaye hain. Ravi 500 daalta hai aur ek EMI 200 kaatti hai, dono bilkul ek saath. Dono thread ne balance 1000 padha. Ravi wale ne 1500 likha, EMI wale ne 800. Jo baad me likhta hai wahi bachta hai, to final 800 reh gaya, jabki hona 1300 chahiye tha. Ravi ke 500 gayab ho gaye.

```
Thread 1: padha 1000 ──> +500 ──> likha 1500
Thread 2: padha 1000 ──> -200 ──> likha  800   <- baad me likha, 1500 mit gaya
```
Dikkat ye hai ki padhna, jodna aur likhna teen alag kadam hain, aur beech me doosra thread ghus jaata hai. `synchronized` ka matlab hai ek taala: ek thread andar gaya to baaki bahar line me khade rahenge.

**Khaas baat:**
1. `synchronized` do kaam karta hai: ek waqt pe ek hi thread andar (atomicity), aur jo likha wo baaki threads ko dikhe (visibility). `volatile` sirf doosra kaam karta hai (file 05).
2. Poori method pe lock lagane se achha hai sirf zaroori line pe `synchronized(this) { ... }` lagao, taaki baaki thread kam der ruke.
3. `ReentrantLock` = wahi taala, par extra control ke saath:
   - `tryLock(2, SECONDS)`: lock na mile to haar maan lo, hamesha intezaar mat karo.
   - interrupt ho sakta hai.
   - Keemat: `unlock()` ab teri zimmedari hai. Hamesha `finally` me likho, warna exception aate hi lock hamesha ke liye phas jaata hai.
   ```java
   lock.lock();
   try { count++; } finally { lock.unlock(); }
   ```
4. Do soch hain:
   - `synchronized`: pehle taala lo (pessimistic).
   - CAS / `@Version`: bina taale ke try karo, takkar ho to dobara (optimistic). DB me iska joda hai `SELECT ... FOR UPDATE` vs `WHERE version = ?`.

**Prod wala trap:** Spring ka service bean singleton hota hai, yaani poore app me ek hi object.
```java
@Service
class TransferService {
    public synchronized void transfer(...) { ... }   // poore app ka EK taala
}
```
Ravi ka transfer aur Priya ka transfer ek doosre se bilkul alag hain, phir bhi ek line me lagenge, kyunki taala ek hi object pe hai. Aur 2 server chala diye to ye taala apne server ke bahar kuch nahi rokta, dono server pe alag-alag taala hoga. Isliye banking design me lock DB me lagaya tha (atomic UPDATE / row lock), Java me nahi.

**Tera project:** usercrud [`RateLimitController.hit()`](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/RateLimitController.java) me `synchronized` lagaya hai. Ek server pe ye theek hai. Multi-instance pe Redis counter chahiye, wahi jo rate limiter design me kiya tha.

```
Interview: "synchronized lets only one thread into the critical section and also gives visibility.
I prefer block-level sync to keep the lock short. ReentrantLock when I need tryLock with
timeout or interruptible waits - but unlock must be in finally. In a Spring singleton or
multi-instance setup, a Java lock isn't enough - I push the lock to the DB or Redis."
```
