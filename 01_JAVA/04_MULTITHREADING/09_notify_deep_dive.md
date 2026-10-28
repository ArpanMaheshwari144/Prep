# notify() Deep Dive — Andar Kya Hota

**Kahani:** wahi bathroom. Producer (P) andar gaya, dekha queue bhari hai, to `wait()` karke bahar baith gaya. Ab Consumer (C) andar gaya, ek item uthaya, aur P ko awaaz di (`notify()`). Par C abhi andar hi hai, darwaza band hai. P uth ke darwaze pe khada ho jaata hai. Andar wo tab hi jaa sakta hai jab C bahar aaye.

```
P: wait()  -> lock chhoda, WAIT SET me baitha        (WAITING)
C: lock liya -> poll() -> notify()                   P ab BLOCKED (darwaze pe khada)
C: } bahar nikla -> lock chhoda
P: lock liya -> wait() ke theek baad se aage chala   (RUNNABLE)
```

**Khaas baat:**
1. Har object ke paas do cheez hoti hain: ek lock, aur ek wait set (un threads ki list jo is object pe `wait()` kar rahe hain). `synchronized void produce()` likhne ka matlab hai `synchronized(this)`.
2. `notify()` thread ko seedha nahi chalata. Wo bas usse wait set se nikaal ke BLOCKED me daal deta hai, yaani lock ki line me. C jitni der andar rahe, P utni der BLOCKED hi rehta hai.
3. Jaagne ke baad P `wait()` ke theek agli line se chalta hai. Isliye `while` zaroori hai: lock milne tak shart badal chuki ho sakti hai, to dobara check karo.
4. `notify()` sirf ek thread uthata hai, `notifyAll()` sabko.

**Prod wala trap:** kai producer aur kai consumer hain, aur sab `notify()` use kar rahe hain. Ek consumer ne `notify()` kiya, aur galti se doosra consumer jag gaya. Queue khaali hai, to wo phir `wait()` karke baith gaya. Producer ko kisi ne jagaya hi nahi. Ab sab so rahe hain, koi exception nahi aata, app bas ruk jaata hai, bilkul deadlock jaisa.
```java
notify();      // galat thread jag sakta hai -> sab so jaate hain
notifyAll();   // sab jaago, jiski shart poori ho wo kaam kare
```
Asli code me ye sab haath se likhte hi nahi, `BlockingQueue` le lete hain (file 12).

```
Interview: "Every object has a monitor lock and a wait set. wait() releases the lock and joins the
wait set. notify() moves one waiting thread to BLOCKED - it still has to reacquire the lock,
which happens only after the notifier exits synchronized. With multiple producers and
consumers I use notifyAll, or better, a BlockingQueue."
```
