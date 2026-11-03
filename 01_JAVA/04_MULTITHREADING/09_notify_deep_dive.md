# notify() Deep Dive — Andar Kya Hota Hai

## Kahani: wahi bathroom

Producer (P) andar gaya aur dekha ki queue bhari hui hai. Isliye usne `wait()` kiya aur bahar jaake baith gaya. Ab Consumer (C) andar gaya, usne ek item uthaya, aur P ko awaaz di, yaani `notify()` kiya.

Par dhyan de: C abhi bhi andar hi hai, aur darwaza band hai. P uth ke darwaze pe aake khada ho jaata hai, par andar wo tabhi ja sakta hai jab C bahar aaye.

```
P: wait()  -> lock chhoda, WAIT SET me jaake baitha            (state: WAITING)
C: lock liya -> poll() kiya -> notify() kiya                   P ab BLOCKED (darwaze pe khada)
C: } se bahar nikla -> lock chhoda
P: lock liya -> wait() ke theek baad wali line se aage chala   (state: RUNNABLE)
```

## Khaas baat

**1. Har object ke paas do cheez hoti hain.** Ek uska lock, aur ek uska wait set. Wait set un threads ki list hai jo is object pe `wait()` karke baithe hain. Aur jab tu `synchronized void produce()` likhta hai, to uska matlab hota hai `synchronized(this)`, yaani lock isi object ka hai.

**2. `notify()` thread ko seedha chala nahi deta.** Wo bas us thread ko wait set se nikaal ke BLOCKED state me daal deta hai, matlab ab wo lock ki line me khada hai. Jitni der C andar rahega, utni der P BLOCKED hi rahega.

**3. Jaagne ke baad P `wait()` ke theek agli line se chalta hai.** Isliye `while` zaroori hai. Jab tak P ko lock milta hai, tab tak shart badal chuki ho sakti hai, isliye use dobara check karna padta hai.

**4. `notify()` sirf ek thread ko uthata hai, aur `notifyAll()` sabko uthata hai.**

## Prod wala trap: kai producer, kai consumer, aur sab `notify()` use kar rahe hain

Maan lo ek consumer ne `notify()` kiya, aur galti se doosra consumer jag gaya (producer nahi). Queue khaali hai, to wo consumer phir se `wait()` karke baith gaya. Producer ko kisi ne jagaya hi nahi. Ab sab so rahe hain. Koi exception nahi aata, app bas ruk jaata hai, bilkul deadlock jaisa.

```java
notify();      // galat thread jag sakta hai, phir sab so jaate hain
notifyAll();   // sab jaago, jiski shart poori ho wahi kaam kare
```

Asli code me ye sab haath se likhte hi nahi. Hum seedha `BlockingQueue` le lete hain (file 12), wo ye sab andar se sambhal leta hai.

## Interview line

```
Interview: "Every object has a monitor lock and a wait set. wait() releases the lock and joins the
wait set. notify() moves one waiting thread to BLOCKED - it still has to reacquire the lock,
which happens only after the notifier exits synchronized. With multiple producers and
consumers I use notifyAll, or better, a BlockingQueue."
```


---

## GEHRA HISSA (purani file se, interview me kaam ka)

## Key Insights: chaar baatein jo confuse karti hain

**Kya `notify()` se thread turant jaag ke chalne lagta hai?** Nahi. `notify()` use sirf BLOCKED state me bhejta hai, aur wahan wo lock ke liye line me khada rehta hai.

**Kya `notify()` karte hi notify karne wala lock chhod deta hai?** Nahi. Notify karne wala thread lock tab tak pakde rehta hai jab tak uska synchronized block khatam nahi hota.

**Thread `wait()` ke baad kahan se dobara chalta hai?** `wait()` ke theek baad se. Wahin se wo while loop ki shart dobara check karta hai.

**`notify()` aur `notifyAll()` me kya farak hai?** `notify()` wait set me se sirf ek thread ko jagata hai. `notifyAll()` wait set ke saare threads ko jagata hai.

### Step 2: `wait()` ke 3 kaam

Jab P `wait()` karta hai, to teen cheez hoti hain. P buffer ke wait set me chala jaata hai, uski state WAITING ho jaati hai, aur wo lock chhod deta hai.

```
Buffer ka Wait Set:      [P]   ← Producer yahan baitha hai
P ki state:              WAITING
P ke paas lock hai?      NAHI (chhod diya)
```

## Visual Summary

Upar se neeche padh: pehle P wait set me hai aur C ke paas lock hai. `notify()` P ko wait set se nikaal ke BLOCKED karta hai. Jab C synchronized se bahar nikalta hai, tab lock P ko milta hai aur P chalne lagta hai.

```
WAIT SET                     CHAL RAHA                 LOCK KISKE PAAS

┌──────────┐                                         ┌──────┐
│   P      │  ◄── wait()      C: lock liya          │  C   │
│ WAITING  │                  C: poll()              └──────┘
└──────────┘                  C: notify()

                   ↓ notify() ne P ko chuna

┌──────────┐                                         ┌──────┐
│  khaali  │      P: BLOCKED                         │  C   │
└──────────┘      (wait set se nikla)                └──────┘

                   ↓ C synchronized se bahar (lock chhoda)

                                                     ┌──────┐
                  P: BLOCKED → RUNNABLE              │  P   │
                  P: wait() ke baad se chala         └──────┘
```
