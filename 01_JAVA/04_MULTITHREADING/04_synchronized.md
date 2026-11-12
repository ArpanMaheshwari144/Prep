# synchronized: Race Condition Se Bachao

## Scene: ek parchi, 10 cashier

Godown ki parchi pe likha hai "ghee: 10000". 10 cashier ek saath bech rahe hain, aur har cashier 1000 baar "ek kaato" karta
hai. Aakhir me parchi pe 0 hona chahiye. Chala ke dekha:

```
Bina taala:     bacha ghee: 0     <- is baar kismat se sahi
Bina taala:     bacha ghee: 272   <- 272 dabbe bik gaye par kate nahi
Bina taala:     bacha ghee: 199
synchronized:   bacha ghee: 0     <- har baar sahi
```

Ye isliye hota hai kyunki `ghee--` ek kaam nahi, teen kaam hai: parchi padho, ek ghatao, aur likho. Parchi 10000 se kat-kat
ke neeche aa rahi hai. Maan lo kisi pal parchi pe 500 bacha tha: do cashier ne ek saath 500 padha aur dono ne 499 likha. Isse dabbe do bike, par kata ek. Pehli line dekh: kabhi-kabhi kismat
se sahi bhi aa jaata hai, isiliye ye bug testing me chhup jaata hai aur prod me pakda jaata hai.

`synchronized` parchi pe taala hai. Ek cashier ne parchi uthayi, to baaki line me khade rahenge.

## Khaas baat

`synchronized` do kaam karta hai. Pehla, ek waqt me ek hi thread andar jaata hai. Doosra, jo usne likha wo baaki sab threads
ko dikhta hai. `volatile` sirf doosra kaam karta hai (file 05).

Poori method pe taala mat lagao, sirf zaroori line pe `synchronized (this) { ... }` lagao. Taala jitna chhota hoga, line utni
jaldi chalegi.

`ReentrantLock` wahi taala hai, par control tere haath me hota hai. `tryLock(2, SECONDS)` ka matlab hai "2 second me lock na
mile to chhod do", hamesha intezaar nahi. Iski keemat ye hai ki `unlock()` tujhe khud `finally` me likhna padta hai. Agar
exception aaya aur finally nahi likha, to taala hamesha ke liye phas jaata hai.

```java
lock.lock();
try { count++; } finally { lock.unlock(); }
```

Race se bachne ki do soch hain. Pessimistic: pehle taala lo, phir kaam karo (`synchronized`, DB me `SELECT ... FOR UPDATE`).
Optimistic: bina taale ke koshish karo, aur takkar ho jaaye to dobara karo (CAS, DB me `@Version`).

## Prod wala trap

Spring ka `@Service` bean singleton hota hai, yaani poore app me uska ek hi object hota hai.

```java
@Service
class TransferService {
    public synchronized void transfer(...) { ... }   // poore app ka EK taala
}
```

Ravi aur Priya ke transfer ek doosre se bilkul alag hain, phir bhi dono ek hi line me lagenge, kyunki taala ek hi object pe
hai. Aur agar 2 server chala diye, to har server ka apna taala hoga, aur dono ek doosre ko rokenge hi nahi. Isiliye
e-commerce me ghee ki ginti ka taala Java me nahi, parchi pe lagaya tha (atomic UPDATE aur Redis `DECR`), aur banking me bhi
yahi kiya tha.

PR drill ka double payout (FD close, 12-Nov) bhi yahi race hai: "ACTIVE hai?" padha, phir paisa diya, phir CLOSED likha. Do click
aaye to dono ne ACTIVE padha. Java ka `synchronized` 2 server pe nahi bachata, isliye taala DB pe:
`UPDATE ... SET status='CLOSING' WHERE id=? AND status='ACTIVE'`, 1 row badli tabhi aage.

## Tera project

Usercrud ke [`RateLimitController.hit()`](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/RateLimitController.java)
me `public synchronized` lagaya hai. Ek server pe ye chalega. Do server hone pe Redis wala counter chahiye, jo rate limiter
design me banaya tha.
