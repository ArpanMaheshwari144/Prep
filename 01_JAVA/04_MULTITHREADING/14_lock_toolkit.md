# Lock Toolkit — ReadWriteLock, Semaphore, CountDownLatch, CyclicBarrier

## Kahani: BMS ka theatre

Normal taala (`synchronized`) ka matlab hai ki andar sirf EK banda ja sakta hai. Par theatre me har jagah logon ko ek-ek karke bhejna bekaar hai. Isliye 4 alag auzaar hain, aur har ek theatre ki ek jagah se samajh aata hai.

**1. Seat map (ReadWriteLock).** Seat map dekhne wale 1000 log hain, aur wo sab ek saath dekh sakte hain, kyunki dekhne se kuch nahi bigadta. Ye `readLock` hai, jo shared hai. Par jab koi book karne aata hai, to map badlega. Tab use `writeLock` chahiye, jo akela milta hai, aur us waqt baaki sab ko rukna padta hai.

**2. Ticket printer (Semaphore(3)).** Printer ek saath sirf 3 log jhel sakta hai.

```
T1 T2 T3 andar      T4 T5 ruke hue      T1 nikla -> T4 andar
```

Lock kehta hai "EK, ya koi nahi". Semaphore kehta hai "ITNE, aur bas itne".

**3. Theatre khulna (CountDownLatch(3)).** AC, projector aur sound, teeno ready honge tabhi darwaza khulega. Manager bahar khade hoke wait karta hai (`await`), aur staff har kaam hone pe ginti ek ghata deta hai (`countDown`). Ek baar darwaza khul gaya to hamesha khula rehta hai, ye reset nahi hota.

**4. Teen dost (CyclicBarrier(3)).** Teeno dost gate pe milte hain, aur phir saath andar jaate hain. Agle show pe phir wahi hota hai, aur barrier apne aap reset ho jaata hai. Isiliye iska naam "cyclic" hai.

## Khaas baat

**1. ReadWriteLock tabhi faayda deta hai jab padhna zyada aur likhna kam ho.** Jaise seat map, config, ya cache. Agar padhna aur likhna barabar ho, to iska apna hisaab-kitaab bhaari padta hai, aur saada `synchronized` tez nikalta hai.

**2. Read lock pakad ke write lock maanga to DEADLOCK.** Kyunki tu read chhutne ka intezaar kar raha hai, aur wo read tu hi pakde baitha hai.

**3. Latch aur Barrier ka farak.** Latch me koi bahar wala thread wait karta hai, aur ye ek hi baar kaam aata hai. Barrier me wahi workers ek doosre ka wait karte hain, aur har round ke baad ye reset ho jaata hai.

**4. Ek party pahunchi hi nahi, to baaki hamesha ruke rahenge.** Isliye `await` ka timeout wala version lagao. Barrier me agar kisi ek ka timeout ho gaya, to baaki sab ko `BrokenBarrierException` milta hai.

## Prod wala trap: release tab bhi kar diya jab acquire mila hi nahi

```java
try {
    slots.tryAcquire();      // false aaya (printer full)...
    printTicket();
} finally {
    slots.release();         // ...phir bhi release -> Semaphore(3) chupke se 4 ka ho gaya
}
```

Yahan `tryAcquire()` ne `false` diya, matlab chaabi mili hi nahi, phir bhi `finally` me `release()` ho gaya. Semaphore ka koi maalik nahi hota, isliye koi bhi thread `release()` kar sakta hai, aur Java rokta nahi. Ilaaj: `release()` sirf tab karo jab `tryAcquire()` ne `true` diya ho.

## Tera project

`usercrud` ka RateLimitController "ek saath kitne" wali soch pe hai, jo semaphore wali soch hai. Par "ek second me kitne" wala hisaab token bucket hai, semaphore nahi.

## Interview line

```
Interview: "Jahan resource limited ho (connection pool, ya downstream jo sirf N concurrent jhele)
wahan Semaphore, kyunki lock sab ek-ek karke chalata. Read-heavy state pe ReadWriteLock, taaki
readers aapas me na rukein. N kaam khatam hone ka intezaar = CountDownLatch, round-by-round sab
milein = CyclicBarrier. Har await ka timeout version, aur release / countDown hamesha finally me."
```

---

## GEHRA HISSA (purani file se, interview me kaam ka)

## Sabse pehle ye socho: kya taala chahiye bhi?

Zyadatar baar jawab yahin mil jaata hai, aur apna lock likhna hi nahi padta.

- Ek counter badhana hai, to `AtomicInteger` lo, taala mat lagao.
- Ek map share karna hai, to `ConcurrentHashMap` lo, taala mat lagao.
- Queue se kaam aage pass karna hai, to `BlockingQueue` lo, taala mat lagao.
- Value kabhi badalti hi nahi, to use `final` ya immutable bana do, taala mat lagao.
- Kai async kaam ka intezaar karna hai, to `CompletableFuture.allOf(...)` lo.

Ye sab ANDAR se lock khud sambhal lete hain. Apna lock likhna AAKHRI raasta hai, pehla nahi.

---

## 1. `ReadWriteLock`: padhne wale ek doosre ko rokein hi kyun?

```
synchronized me:
   T1 sirf PADH raha  ─┐
   T2 sirf PADH raha  ─┤  teeno LINE me khade, ek-ek karke andar
   T3 sirf PADH raha  ─┘  jabki koi kisi ka kuch bigaad hi nahi raha tha
```

Padhne wale ek doosre ko **koi nuksaan pahuncha hi nahi sakte**. Nuksaan tabhi hota hai jab koi **likhta** hai. To padhne walon ko line me khada karna bekaar hai.

READ lock kitne bhi log EK SAATH le sakte hain, ye shared hai. WRITE lock sirf EK banda le sakta hai, aur jab tak wo pakde hai, tab tak koi padh bhi nahi sakta. Ye exclusive hai.

```
     padhne wale:  T1  T2  T3  T4      <- sab saath andar
     likhne wala:  W1                  <- akela, sabko bahar nikaal ke
```

```java
import java.util.concurrent.locks.ReentrantReadWriteLock;

class Config {
    private final ReentrantReadWriteLock rw = new ReentrantReadWriteLock();
    private final Map<String, String> data = new HashMap<>();

    public String get(String key) {
        rw.readLock().lock();                 // 100 thread saath aa sakte
        try {
            return data.get(key);
        } finally {
            rw.readLock().unlock();
        }
    }

    public void put(String key, String val) {
        rw.writeLock().lock();                // akela — saare readers rukenge
        try {
            data.put(key, val);
        } finally {
            rw.writeLock().unlock();
        }
    }
}
```

### Kab faayda, kab NUKSAAN

Agar padhna ~95% hai aur likhna ~5%, to BADA faayda hai, kyunki 95% traffic bina ruke chalta hai. Ye config, cache, reference data, ya pricing table jaisi cheezon me hota hai.

Agar padhna aur likhna barabar hai, to koi FAAYDA NAHI, balki ULTA NUKSAAN hai. RW lock ka apna hisaab-kitaab bhaari hai, aur saada `synchronized` isse TEZ nikal jaayega.

### Do edge case

**1. Writer starvation.** Readers aate hi ja rahe hain, to lock kabhi khaali hi nahi hota, aur writer bechaara khada hi reh jaata hai. Ilaaj hai `new ReentrantReadWriteLock(true)`, yaani fair mode. Fair mode me writer ke aane ke baad jo naye readers aate hain, wo line me lagte hain.

**2. Downgrade ho sakta hai, upgrade NAHI.** Write se read pe jaana chalega: write pakde-pakde read le lo, phir write chhod do. Par read se write pe jaana DEADLOCK hai. Kyunki tu read chhutne ka intezaar kar raha hai, aur wo read tu hi pakde baitha hai.

### HLD se jod

DB replication me ek leader likhta hai aur kai copies padhti hain. ReadWriteLock me ek writer likhta hai aur kai readers padhte hain. Soch bilkul EK hi hai: padhne wale bahut hain aur likhne wale kam, to padhne walon ko ek doosre se mat roko. (Dekh: `04_HLD/FOUNDATIONS/05_database_replication.md`)

**Bolne wali line:** "Read-heavy shared state pe `ReadWriteLock` lagata hoon — readers aapas me nahi takraate, sirf writer sabko rokta hai. Keemat do hain: writes kam na hon to iska overhead `synchronized` se mehnga pad jaata hai, aur readers ki bheed me writer bhookha reh sakta hai — isliye fair mode."

---

## 2. `Semaphore`: ye taala nahi, GINTI hai

```
        SEMAPHORE(3)          <- 3 chaabi, 3 log ek saath andar

  T1 ──► [chaabi li]  andar
  T2 ──► [chaabi li]  andar
  T3 ──► [chaabi li]  andar
  T4 ──► rukega...            <- chaabi khatam
  T5 ──► rukega...

  T1 nikla, chaabi wapas rakhi  ->  T4 ko mili, T4 andar
```

```java
import java.util.concurrent.Semaphore;

class PdfService {
    private final Semaphore slots = new Semaphore(3);   // sirf 3 ek saath

    public void generate(Report r) throws InterruptedException {
        slots.acquire();              // chaabi lo — na ho to RUKO
        try {
            heavyPdfWork(r);          // yahan max 3 thread hi ho sakte
        } finally {
            slots.release();          // ★ chaabi WAPAS — finally me
        }
    }
}
```

Agar rukna nahi hai aur chaabi na mile to turant haar maan leni hai, to `tryAcquire()` lagao:

```java
// haarna hai to
if (slots.tryAcquire()) {                        // turant — mila to true
    try { heavyPdfWork(r); } finally { slots.release(); }
} else {
    throw new BusyException("abhi 3 report ban rahi hain, thodi der me try karo");
}

slots.tryAcquire(2, TimeUnit.SECONDS);           // 2 second ruk ke haar
```

### Ye kis dikkat se paida hua

Maan lo 200 thread hain, aur ek BHAARI kaam hai, jaise PDF banana, image resize karna, ya kisi purane system ko call karna jo ek saath sirf 10 request jhel sakta hai.

Agar `synchronized` lagaya, to sirf 1 thread chalega aur 199 line me khade rahenge. Ye BAHUT dheema hai. Agar kuch nahi lagaya, to 200 ek saath chale jaayenge, aur wo system gir jaayega ya memory phat jaayegi. `Semaphore(10)` lagaya, to theek 10 jaayenge, na kam na zyada.

**Nichod:** lock kehta hai "EK, ya koi nahi". Semaphore kehta hai "ITNE, aur bas itne".

### Teen baatein jo interview me pakadti hain

**1. Semaphore ka MAALIK NAHI HOTA.** `lock.unlock()` sirf wahi thread kar sakta hai jisne lock liya tha. Par `release()` KOI BHI thread kar sakta hai, chahe usne acquire kiya hi na ho. Ye bug bhi ban sakta hai: ek extra `release()` hua to permits BADH gaye, aur `Semaphore(3)` chupke se 4 ka ho gaya, aur tujhe kabhi pata bhi nahi chalega. Aur yahi cheez feature bhi hai: T1 acquire kare aur T2 release kare, to isse signalling ho jaati hai.

**2. Ye REENTRANT NAHI HAI.** `lock.lock(); lock.lock();` chalega, kyunki lock ka hold count 2 ho jaata hai. Par `sem.acquire(); sem.acquire();` me DO permit chale gaye. Agar `Semaphore(1)` pe ye kiya, to tu KHUD pe hi phans gaya, aur deadlock ho gaya.

**3. Kya `Semaphore(1)` ek mutex hai?** Kaam lagbhag wahi hai, PAR iska maalik nahi hota (upar wali baat). Isliye agar mutual exclusion chahiye, to `ReentrantLock` lo. Semaphore ka asli kaam GINTI hai, taala nahi.

### Asli duniya me kahan dikhta hai

**Concurrency limit:** "max 10 request EK SAATH chalein", ye ginti hai. Dhyan rakh, per-SECOND rate token bucket se aata hai, semaphore se nahi. Semaphore sirf "ek saath kitne" ginta hai, "ek second me kitne" nahi.

**DB connection pool:** "sirf 20 connection hain", ye bhi ginti hai.

**Bulkhead:** "payment service ko max 5 thread do, taaki wo gire to poori app na mare", ye bhi ginti hai.

Tere apne project me bhi, usercrud ke RateLimitController me, wahi soch chal rahi hai.

**Bolne wali line:** "Jahan resource limited ho — thread-pool, connection, ya koi downstream system jo sirf N concurrent jhel sakta hai — wahan `Semaphore`. Lock yahan galat auzaar hai: wo sab kuch ek-ek karke chalata, jabki mujhe N chahiye. Keemat ye ki semaphore ka maalik nahi hota — ek galat `release` permits badha deta hai — isliye `release` hamesha `finally` me."

---

## 3. `CountDownLatch`: ek taraf ka darwaza

```
       main thread          ┌─────────────┐
           │                │             │
        await()  ──────────►│   GATE      │   count = 3
           │                │   band hai  │
                            └─────────────┘
                                  ▲ ▲ ▲
                           W1 ────┘ │ │    countDown()  -> 2
                           W2 ──────┘ │    countDown()  -> 1
                           W3 ────────┘    countDown()  -> 0

       count 0 -> GATE khula -> main aage badha
       ★ aur ab wo HAMESHA khula rahega. Dobara band NAHI hota.
```

```java
CountDownLatch latch = new CountDownLatch(3);

for (Service s : List.of(userSvc, orderSvc, paymentSvc)) {
    pool.submit(() -> {
        try {
            s.warmUp();
        } finally {
            latch.countDown();     // ★ finally me — warna main HAMESHA atka rahega
        }
    });
}

latch.await();                      // teeno khatam hone tak ruko
// latch.await(10, TimeUnit.SECONDS);  // ya 10 sec ruk ke aage badh jao
System.out.println("app ready");
```

**Isme do alag kirdaar hain.** Intezaar karne wala main thread hai, jo `await` karta hai. Ginti ghatane wale workers hain, jo `countDown` karte hain. Ye alag-alag thread hote hain.

**Ye EK BAAR ka auzaar hai.** Count 0 ho gaya to iska kaam khatam, ye reset nahi hota. Dobara chahiye to naya latch banao.

---

## 4. `CyclicBarrier`: sab ek jagah milte hain

```
   W1 ──► await() ─┐
   W2 ──► await() ─┼──►  BARRIER (3 parties)   <- teeno pahunch gaye?
   W3 ──► await() ─┘                              haan -> teeno EK SAATH aage

   aur barrier khud ko RESET kar leta hai -> agle round me phir wahi kaam
```

```java
CyclicBarrier barrier = new CyclicBarrier(3, () -> {
    System.out.println("round khatam — ye LAST pahunchne wale ke thread pe chalta hai");
});

// har worker
for (int round = 0; round < 5; round++) {
    computeMyPart(round);
    barrier.await();      // baaki do ka intezaar
    // teeno yahan se EK SAATH aage — agla round
}
```

**Isme ek hi kirdaar hai.** Jo intezaar karte hain, WAHI barrier ko poora bhi karte hain, aur wo sab worker hain. Koi bahar khada tamashbeen nahi hota.

**Ye BAAR-BAAR chalta hai**, isliye naam "cyclic" hai. Har round ke baad ye apne aap reset ho jaata hai.

**Tu ek optional KAAM bhi de sakta hai** jo har round ke ant me chale, jaise upar wala lambda.

---

## LATCH vs BARRIER: saamne-saamne (ye dono aksar confuse hote hain)

**Kaun rukta hai:** latch me ek ya kai BAAHRI thread rukte hain. Barrier me khud WORKERS rukte hain.

**Kaun poora karta hai:** latch me workers `countDown` karke poora karte hain. Barrier me wahi workers `await` karke poora karte hain.

**Dobara use:** latch dobara use NAHI hota, ek hi baar ka hai. Barrier dobara use HOTA hai, apne aap reset ho jaata hai.

**Ginti:** latch me ginti sirf GHATTI hai. Barrier me ginti har round me phir se bhar jaati hai.

**Extra kaam:** latch me nahi hota. Barrier me barrier action de sakte ho.

**Tasveer:** latch ek rocket launch ka countdown hai: 3, 2, 1, 0, aur rocket chhoot gaya, dobara nahi. Barrier ek trek ka checkpoint hai: sab pahunche to saath chale, aur agle checkpoint pe phir wahi.

### Sabse bada gotcha: barrier TOOT jaata hai

Maan lo 3 parties chahiye, aur 2 already `await()` me khade hain.

**Case (a):** teesra `await()` tak pahuncha hi nahi, kyunki wo pehle hi CRASH ho gaya ya exception aa gaya. Tab barrier TOOTTA NAHI (`isBroken = false`), aur baaki dono HAMESHA ke liye WAITING me khade rehte hain. Ye latch jaisa hi anant intezaar hai.

**Case (b):** jo khade hain unme se kisi ka TIMEOUT ho gaya ya INTERRUPT aa gaya (jaise `await(500ms)` wala). Tab BARRIER TOOT JAATA HAI. Us thread ko `TimeoutException` milta hai, aur baaki sabko `BrokenBarrierException`. Poora barrier bekaar ho jaata hai, aur `reset()` se hi wapas theek hota hai.

4-Oct ko chala ke dekha tha: crash wale case me dono thread WAITING me rahe aur barrier toota nahi. `await(500ms)` wale case me W1 ko `TimeoutException` mila aur W2 ko `BrokenBarrierException`.

Isliye latch ho ya barrier, `await` ka TIMEOUT wala version zaroori hai, aur latch me `countDown` `finally` me. Timeout hi wo cheez hai jo "chup-chaap anant intezaar" ko ek shor machane wali exception me badal deti hai.

### Asli zindagi me kab kaunsa

**Latch:** app start hote waqt 3 service warm-up ho jaayein, tab traffic lo. Test me saare thread ek saath chhodne ho, to count 1 ka "start gate" bana lo. Kai API parallel call karo, aur sab ka jawab aane pe aage badho.

**Barrier:** round-by-round calculation me, jahan sab apna hissa karte hain, sab milte hain, phir agla round shuru hota hai. Jaise simulation, image ko tukdon me process karna, ya game ke turn.

**Aaj ki duniya me:** `CountDownLatch` ka kaam ab aksar `CompletableFuture.allOf(...)` se ho jaata hai ([11_future_vs_completablefuture.md](11_future_vs_completablefuture.md)). Par latch phir bhi interview me poocha jaata hai, aur legacy code me milta hai.

**Bolne wali line:** "`CountDownLatch` ek baar ka gate hai — koi baahar khada intezaar karta hai jab tak N kaam khatam na hon, aur uske baad wo dobara use nahi hota. `CyclicBarrier` me wahi thread ek doosre ka intezaar karte hain aur har round ke baad wo apne aap reset ho jaata hai. Dono me ek party na pahunche to baaki hamesha atke rehte — isliye `await` ka timeout version (aur latch me `countDown` `finally` me). Barrier me kisi ka timeout/interrupt ho to baaki sabko `BrokenBarrierException` milta hai."

---

## KAUNSA KAB: ek sawaal se tay ho jaata hai

Khud se poochho: **"Main kis cheez ko kaabu kar raha hoon?"**

- Agar EK BANDA andar jaaye, to `synchronized`. Agar haar maanni ho ya interrupt chahiye, to `ReentrantLock`.
- Agar PADHNE wale bahut hain aur LIKHNE wale kam, to `ReadWriteLock`.
- Agar N BANDE ek saath andar ja sakte hain, to `Semaphore`.
- Agar "N kaam khatam hon, TAB main chalu karun", to `CountDownLatch`.
- Agar "sab ek doosre ka intezaar karein, phir saath chalein, har round", to `CyclicBarrier`.

### Poora naksha: har auzaar ka kaam aur uski keemat

**`synchronized`:** ek banda andar, aur JVM sab sambhalta hai. Keemat ye ki na tu haar maan sakta hai, na interrupt kar sakta hai, na kisi alag jagah unlock kar sakta hai.

**`ReentrantLock`:** wahi kaam, plus `tryLock`, timeout, interrupt aur fairness. Keemat ye ki unlock karna TERI zimmedari hai, isliye `finally` me. Aur fair mode dheema hota hai.

**`ReadWriteLock`:** readers saath chalte hain, writer akela. Keemat ye ki agar writes kam na hon to ye ULTA mehnga padta hai. Writer starvation ho sakti hai, jiska ilaaj fair mode hai. Aur read se write ka upgrade DEADLOCK hai.

**`Semaphore`:** N ek saath (ginti). Keemat ye ki iska maalik nahi hota, to ek extra `release()` chupke se permits badha deta hai. Aur ye reentrant NAHI hai, to tu khud pe phans sakta hai.

**`CountDownLatch`:** "N khatam hon, tab gate khule". Keemat ye ki ye EK BAAR ka hai, reset nahi hota. Aur agar koi `countDown` chhoot gaya, to ANANT intezaar.

**`CyclicBarrier`:** "sab milein, phir saath chalein", aur har round reset. Keemat ye ki agar ek party pahunchi hi nahi to baaki ANANT ruke rehte hain. Aur timeout ya interrupt hua to SABKO `BrokenBarrierException` milta hai.

### Teen jode jo confuse hote hain

**1. `ReentrantLock` vs `Semaphore(1)`.** Dono "ek banda andar" wala kaam karte hain. Farak ye hai ki lock ka MAALIK hota hai, jisne liya wahi chhodega, aur semaphore ka maalik nahi hota, koi bhi release kar sakta hai. Isliye mutual exclusion ke liye Lock, aur ginti ya signal ke liye Semaphore.

**2. `CountDownLatch` vs `CyclicBarrier`.** Latch me intezaar karne wala ALAG banda hota hai, aur ye EK BAAR ka hai. Barrier me wahi log intezaar karte hain jo kaam kar rahe hain, aur ye BAAR-BAAR chalta hai.

**3. `ReadWriteLock` vs `synchronized`.** RW lock tabhi jeetta hai jab padhna ~90% se zyada ho. Agar padhna-likhna barabar ho, to saada `synchronized` TEZ nikal jaata hai.

---

## EK EDGE SABME COMMON HAI

Har acquire ke saath ek release hona chahiye, aur wo HAMESHA `finally` me:

```
   lock.lock()      ->  finally { lock.unlock(); }
   sem.acquire()    ->  finally { sem.release(); }
   latch banaya     ->  finally { latch.countDown(); }
```

Agar ek bhi chhoot gaya, to koi thread HAMESHA ke liye khada reh jaayega, aur error kahin dikhega bhi nahi. Bas app dheere-dheere jam jaayegi. Ye sabse mehnga bug hai, kyunki ye chillata nahi.

**Aakhri bolne wali line:** "Pehle dekhta hoon ki lock chahiye bhi ya nahi — `AtomicInteger`, `ConcurrentHashMap`, `BlockingQueue` andar se ye kaam kar dete hain. Zaroorat pade to auzaar sawaal se chunta hoon: ek banda andar chahiye to lock, padhne wale bahut hain to `ReadWriteLock`, N ek saath chahiye to `Semaphore`, aur intezaar karna ho to — ek baar ka ho to `CountDownLatch`, baar-baar ka ho to `CyclicBarrier`. In sab ki ek hi common keemat hai: release ab meri zimmedari hai, isliye hamesha `finally` me."
