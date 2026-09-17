# Lock Toolkit — ReadWriteLock · Semaphore · CountDownLatch · CyclicBarrier

> **NAV** — KYA: jab ek saada taala kaafi na ho. UP: [04_synchronized.md](04_synchronized.md) (taala + ReentrantLock) ·
> saath: [06_atomic_integer_cas.md](06_atomic_integer_cas.md) · [10_executor_service_thread_pool.md](10_executor_service_thread_pool.md) ·
> [11_future_vs_completablefuture.md](11_future_vs_completablefuture.md) · [12_producer_consumer.md](12_producer_consumer.md)

> **★ IS FILE KA MAQSAD:** ye TAALE nahi hain — ye **COORDINATION** hain.
> Taala poochta hai *"andar kaun jaayega?"* · Ye poochte hain *"kitne jaa sakte hain?"* aur *"kab tak ruko?"*
> (`ReentrantLock` taala hai — wo [04_synchronized.md](04_synchronized.md) me hai, kyunki wo `synchronized` ka upgrade hai.)

---

## ★★ PEHLE YE — kya taala chahiye BHI?

> Zyadatar baar jawab yahin mil jaata hai, aur apna lock likhna hi nahi padta.

```
ek counter badhana hai         ->  AtomicInteger       (taala mat lagao)
ek map share karna hai         ->  ConcurrentHashMap   (taala mat lagao)
queue se kaam pass karna hai   ->  BlockingQueue       (taala mat lagao)
value kabhi badalti hi nahi    ->  final / immutable   (taala mat lagao)
kai async kaam ka intezaar     ->  CompletableFuture.allOf(...)

ye sab ANDAR se lock sambhal lete hain.
Apna lock likhna AAKHRI raasta hai, pehla nahi.
```

---

## 1. `ReadWriteLock` — padhne wale ek doosre ko rokein hi kyun

```
synchronized me:
   T1 sirf PADH raha  ─┐
   T2 sirf PADH raha  ─┤  teeno LINE me khade, ek-ek karke andar
   T3 sirf PADH raha  ─┘  jabki koi kisi ka kuch bigaad hi nahi raha tha
```

> Padhne wale ek doosre ko **nuksaan pahuncha hi nahi sakte**. Nuksaan tabhi hota hai jab koi **likhta** hai.

```
READ  lock  ->  kitne bhi log EK SAATH        (shared)
WRITE lock  ->  sirf EK, aur tab koi padh bhi nahi sakta   (exclusive)

     padhne wale:  T1  T2  T3  T4      <- sab saath andar
     likhne wala:  W1                  <- akela, sabko bahar nikal ke
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

```
padhna ~95%, likhna ~5%   ->  BADA faayda. 95% traffic bina ruke chal raha.
                              (config, cache, reference data, pricing table)

padhna-likhna barabar     ->  FAAYDA NAHI, ULTA NUKSAAN.
                              RW lock ka apna hisaab-kitaab bhaari hai —
                              saada synchronized isse TEZ nikal jaayega.
```

### ★ Do edge

```
1. WRITER STARVATION
      readers aate hi ja rahe, lock kabhi khaali hi nahi hota
      -> writer khada hi reh gaya
      -> ilaaj: new ReentrantReadWriteLock(true)   (fair mode)
                writer aane ke baad naye readers line me lagenge

2. DOWNGRADE ho sakta hai, UPGRADE NAHI
      write -> read   ->  chalega (write pakde hue read lo, phir write chhodo)
      read  -> write  ->  ★ DEADLOCK. Tu read chhodne ka intezaar kar raha hai,
                          aur read tu hi pakde hai.
```

### HLD se joint

```
DB replication  ->  ek leader likhta, kai copies padhti
ReadWriteLock   ->  ek writer likhta, kai readers padhte

soch bilkul EK hai: padhne wale bahut, likhne wale kam
                    -> padhne walon ko ek doosre se mat roko
(dekh: 04_HLD/FOUNDATIONS/05_database_replication.md)
```

> **BOLNE WALI LINE:** *"Read-heavy shared state pe `ReadWriteLock` lagata hoon — readers aapas me
> nahi takraate, sirf writer sabko rokta hai. Keemat do hain: writes kam na hon to iska overhead
> `synchronized` se mehnga pad jaata hai, aur readers ki bheed me writer bhookha reh sakta hai —
> isliye fair mode."*

---

## 2. `Semaphore` — taala nahi, GINTI

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

```java
// haarna hai to
if (slots.tryAcquire()) {                        // turant — mila to true
    try { heavyPdfWork(r); } finally { slots.release(); }
} else {
    throw new BusyException("abhi 3 report ban rahi hain, thodi der me try karo");
}

slots.tryAcquire(2, TimeUnit.SECONDS);           // 2 second ruk ke haar
```

### Kis dikkat se paida hua

```
200 thread hain, aur ek BHAARI kaam hai —
   PDF banana / image resize / kisi purane system ko call karna
   jo system sirf 10 request ek saath jhel sakta hai

synchronized lagaya?   ->  sirf 1 chalega, 199 line me. BAHUT dheema.
kuch nahi lagaya?      ->  200 saath gaye. Wo system gira, ya memory phati.
Semaphore(10)          ->  theek 10. Na kam, na zyada.

★ NICHOD:  lock kehta hai "EK ya koi nahi".
           semaphore kehta hai "ITNE, aur bas itne".
```

### ★ Teen baatein jo interview me pakadti hain

```
1. SEMAPHORE ka MAALIK NAHI HOTA
     lock.unlock()  ->  sirf WAHI thread jisne liya tha
     release()      ->  KOI BHI thread, chahe usne acquire kiya hi na ho

     BUG     -> ek extra release() -> permits BADH gaye
                Semaphore(3) chupke se 4 ka ho gaya, aur kabhi pata nahi chalega
     FEATURE -> T1 acquire kare, T2 release kare = signalling

2. REENTRANT NAHI HAI
     lock.lock(); lock.lock();       ->  chalega (hold count 2)
     sem.acquire(); sem.acquire();   ->  DO permit gaye
     Semaphore(1) pe ye karo  ->  tu KHUD pe phans gaya. Deadlock.

3. Semaphore(1) == mutex?
     kaam lagbhag wahi, PAR maalik nahi hota (upar wali baat).
     mutual exclusion chahiye -> ReentrantLock.
     Semaphore ka asli kaam GINTI hai, taala nahi.
```

### Asli duniya me

```
rate limiter          ->  "ek user, ek second me 10 request"          = ginti
DB connection pool    ->  "sirf 20 connection hain"                   = ginti
BULKHEAD              ->  "payment service ko max 5 thread do, taaki
                           wo gire to poori app na mare"              = ginti

★ tere apne project me: usercrud / RateLimitController — wahi soch chal rahi hai
```

> **BOLNE WALI LINE:** *"Jahan resource limited ho — thread-pool, connection, ya koi downstream
> system jo sirf N concurrent jhel sakta hai — wahan `Semaphore`. Lock yahan galat auzaar hai:
> wo sab kuch ek-ek karke chalata, jabki mujhe N chahiye. Keemat ye ki semaphore ka maalik nahi
> hota — ek galat `release` permits badha deta hai — isliye `release` hamesha `finally` me."*

---

## 3. `CountDownLatch` — ek taraf ka darwaza

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

```
DO ALAG KIRDAAR:
   INTEZAAR karne wala  ->  main      (await)
   GINTI ghatane wale   ->  workers   (countDown)
   ye alag-alag thread hain

★ EK BAAR ka auzaar. count 0 hua to khatam — reset nahi hota.
  Dobara chahiye? Naya latch banao.
```

---

## 4. `CyclicBarrier` — sab ek jagah milte hain

```
   W1 ──► await() ─┐
   W2 ──► await() ─┼──►  BARRIER (3 parties)   <- teeno pahunche?
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

```
EK HI KIRDAAR:
   jo intezaar karte hain, WAHI barrier poora karte hain — sab worker.
   koi bahar khada tamashbeen nahi.

★ BAAR-BAAR chalta hai (isliye "cyclic"). Har round me apne aap reset.
★ Ek optional KAAM de sakte ho jo har round ke ant me chale (upar wala lambda).
```

---

## ★★ LATCH vs BARRIER — saamne-saamne (ye dono confuse hote hain)

```
                     CountDownLatch              CyclicBarrier
------------------------------------------------------------------------
kaun rukta           ek/kai BAAHRI thread        khud WORKERS
kaun poora karta     workers (countDown)         wahi workers (await)
dobara use           ★ NAHI — ek baar            ★ HAAN — apne aap reset
ginti                sirf GHATTI hai             har round me bharti hai
extra kaam           nahi                        barrier action de sakte ho

TASVEER              rocket launch countdown     trek ka checkpoint
                     3..2..1..0 -> chhoot gaya   sab pahunche -> saath chale
                     dobara nahi                 agla checkpoint phir wahi
```

### ★ Sabse bada gotcha — barrier TOOT jaata hai

```
3 parties chahiye. 2 pahunche, teesra CRASH kar gaya (ya interrupt/timeout)
   ->  BARRIER TOOT GAYA
   ->  baaki DONO ko BrokenBarrierException
   ->  poora barrier bekaar, sab gir gaye

latch me aisa nahi hota — wahan ek worker countDown na kare
   ->  main bas HAMESHA ke liye khada rahega
   ->  isliye await ka TIMEOUT version + countDown ka finally, DONO zaroori
```

### Kab kaunsa — asli zindagi me

```
LATCH    ->  app start: 3 service warm-up hon, tab traffic lo
             test: saare thread ek saath chhodo (count 1 ka "start gate")
             kai API parallel call karo, sab jawab aane pe aage badho

BARRIER  ->  round-by-round calculation: sab apna hissa karein, sab milein,
             phir agla round
             (simulation, image ko tukdo me process karna, game ke turn)
```

> **Aaj ki duniya me:** `CountDownLatch` ka kaam ab aksar `CompletableFuture.allOf(...)` se hota hai
> ([11_future_vs_completablefuture.md](11_future_vs_completablefuture.md)). Latch phir bhi poocha
> jaata hai, aur legacy code me milta hai.

> **BOLNE WALI LINE:** *"`CountDownLatch` ek baar ka gate hai — koi baahar khada intezaar karta hai
> jab tak N kaam khatam na hon, aur uske baad wo dobara use nahi hota. `CyclicBarrier` me wahi
> thread ek doosre ka intezaar karte hain aur har round ke baad wo apne aap reset ho jaata hai.
> Latch me ek worker `countDown` na kare to intezaar karne wala hamesha atka rehta — isliye
> `finally` aur timeout. Barrier me ek party gir jaaye to sabko `BrokenBarrierException` milta hai."*

---

## ★★ KAUNSA KAB — ek sawaal se tay

> **"Main kis cheez ko kaabu kar raha hoon?"**

```
EK BANDA andar jaaye                 ->  synchronized
                                          (haarna/interrupt chahiye -> ReentrantLock)

PADHNE wale bahut, LIKHNE wale kam   ->  ReadWriteLock

N BANDE andar jaa sakte hain         ->  Semaphore

"N kaam khatam hon TAB main chalu"   ->  CountDownLatch

"sab ek doosre ka intezaar karein,
 phir saath chalein — har round"     ->  CyclicBarrier
```

### Poora naksha (kaam + keemat)

```
AUZAAR            KAAM                          KEEMAT / KHATRA
--------------------------------------------------------------------------------
synchronized      ek andar, JVM sambhale        na haar sakte, na interrupt,
                                                na alag jagah unlock

ReentrantLock     wahi + tryLock + timeout      unlock TERI zimmedari -> finally
                  + interrupt + fairness        fair mode dheema hota hai

ReadWriteLock     readers saath, writer akela   writes kam na hon to ULTA mehnga
                                                writer starvation -> fair mode
                                                read->write upgrade = DEADLOCK

Semaphore         N ek saath (ginti)            maalik nahi hota — extra release()
                                                chupke se permits badha deta
                                                reentrant NAHI — khud pe phans sakte

CountDownLatch    "N khatam ho, gate khule"     EK BAAR ka — reset nahi
                                                countDown chhoota -> ANANT intezaar

CyclicBarrier     "sab mile, phir saath"        ek party giri -> SABKO
                  har round reset                BrokenBarrierException
```

### ★ Teen jode jo confuse hote hain

```
1. ReentrantLock  vs  Semaphore(1)
      dono "ek banda andar" karte hain
      FARAK: lock ka MAALIK hota hai (jisne liya wahi chhodega)
             semaphore ka nahi (koi bhi release kar sakta)
      -> mutual exclusion  ->  Lock
      -> ginti / signal    ->  Semaphore

2. CountDownLatch  vs  CyclicBarrier
      FARAK: latch me intezaar karne wala ALAG banda, aur EK BAAR ka
             barrier me wahi log intezaar karte hain, aur BAAR-BAAR chalta hai

3. ReadWriteLock  vs  synchronized
      FARAK: RW tabhi jeetta jab padhna ~90%+ ho
             barabar padhna-likhna ho to saada synchronized TEZ nikal jaata
```

---

## ★★ EK EDGE SABME COMMON HAI

```
har acquire ke saath ek release, aur wo HAMESHA finally me:

   lock.lock()      ->  finally { lock.unlock(); }
   sem.acquire()    ->  finally { sem.release(); }
   latch banaya     ->  finally { latch.countDown(); }

ek bhi chhoota  ->  koi thread HAMESHA ke liye khada reh jaayega,
                    aur error kahin dikhega bhi nahi.
                    Bas app dheere-dheere jam jaayegi — ye sabse mehnga bug hai,
                    kyunki ye chillata nahi.
```

> **AAKHRI BOLNE WALI LINE:** *"Pehle dekhta hoon ki lock chahiye bhi ya nahi — `AtomicInteger`,
> `ConcurrentHashMap`, `BlockingQueue` andar se ye kaam kar dete hain. Zaroorat pade to auzaar
> sawaal se chunta hoon: ek banda andar chahiye to lock, padhne wale bahut hain to `ReadWriteLock`,
> N ek saath chahiye to `Semaphore`, aur intezaar karna ho to — ek baar ka ho to `CountDownLatch`,
> baar-baar ka ho to `CyclicBarrier`. In sab ki ek hi common keemat hai: release ab meri
> zimmedari hai, isliye hamesha `finally` me."*
