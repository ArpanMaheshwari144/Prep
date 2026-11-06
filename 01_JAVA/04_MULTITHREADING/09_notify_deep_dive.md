# notify() Deep Dive: Andar Kya Hota Hai

## Scene: shelf bhari, ladka baitha, cashier ne awaaz di

Shelf bhari hai, isliye ghee bharne wala ladka `wait()` karke baith gaya. Cashier ne ek dabba becha aur awaaz di "aa ja, jagah
ban gayi" (`notify()`). Par cashier abhi bhi shelf ke saamne khada hai. Ladka uth ke peeche khada ho jaata hai, aur andar tabhi
jaayega jab cashier hatega. Chala ke dekha:

```
ladka wait() ke baad:                 WAITING
cashier ne notify() kiya, abhi andar: BLOCKED      <- jaag gaya, par cashier ke hatne ka intezaar
  ladka: shelf mila, maal bhar raha                <- cashier hata, tab andar gaya
cashier bahar nikla, ladka:           TERMINATED
```

Har object ke paas do cheezein hoti hain: uska lock, aur uska wait set (wo threads jo is object pe `wait()` karke baithe hain).
`notify()` kisi ko seedha chala nahi deta. Wo bas ek thread ko wait set se nikaal ke BLOCKED me daal deta hai, aur wo tabhi
chalega jab notify karne wala `synchronized` se bahar nikle. Jaagne ke baad thread `wait()` ki agli line se chalta hai, aur tab
tak shelf phir bhar chuki ho sakti hai, isiliye `while` me shart dobara dekhni padti hai.

## Prod wala trap

Kai ladke aur kai cashier, aur sab `notify()` use kar rahe hain. Ek cashier ne awaaz di, par galti se doosra cashier jaag gaya,
ladka nahi. Shelf khaali hai to wo cashier phir so gaya, aur ladke ko kisi ne bulaya hi nahi. Ab sab so rahe hain: na koi
exception, na error, app bas ruk gaya, bilkul deadlock jaisa.

```java
notify();      // galat thread jag sakta hai, phir sab so jaate hain
notifyAll();   // sab jaago, jiski shart poori ho wahi kaam kare
```

Asli code me ye sab haath se likhte hi nahi, seedha `BlockingQueue` lete hain (file 12), wo andar se sambhal leta hai.
