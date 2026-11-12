# Lock Toolkit: ReadWriteLock, Semaphore, CountDownLatch, CyclicBarrier

## Scene: BMS ka theatre, har jagah ek auzaar

Normal taala (`synchronized`) ka matlab hai andar sirf ek banda. Par theatre me har jagah ek-ek karke bhejna bekaar hai, isliye
4 auzaar hain:

- **Seat map = `ReadWriteLock`.** 1000 log ek saath seat map dekh sakte hain, kyunki dekhne se kuch nahi bigadta (`readLock`,
  shared). Jab koi book karta hai to map badalta hai, to use akele chaabi chahiye (`writeLock`), aur baaki sab rukte hain. Ye
  tabhi faayda deta hai jab padhne wale zyada aur likhne wale kam hon (seat map, config, cache).
- **Ticket printer = `Semaphore(3)`.** Printer ek saath 3 hi jhel sakta hai. Lock kehta hai "ek, ya koi nahi", Semaphore kehta
  hai "itne, bas itne". E-commerce ka "utne andar jitni jagah" yahi tha.
- **Theatre khulna = `CountDownLatch(3)`.** AC, projector aur sound teeno ready honge tabhi darwaza khulega. Manager `await`
  karta hai, staff `countDown`. Ek baar khula to khula, reset nahi hota.
- **Teen dost = `CyclicBarrier(3)`.** Teeno gate pe milte hain aur saath andar jaate hain, agle show pe phir yahi. Har round ke
  baad barrier apne aap reset ho jaata hai.

Chala ke dekha:

```
teen log printer pe, bacha: 0
chautha: printer full, chaabi nahi mili
galti se release ke baad bacha: 1    <- printer ab 4 jhel raha
AC / projector / sound ready -> teeno ready, darwaza khula
```

## Prod wala trap

```java
try {
    slots.tryAcquire();      // false aaya (printer full)...
    printTicket();
} finally {
    slots.release();         // ...phir bhi release -> Semaphore(3) chupke se 4 ka
}
```

Semaphore ka koi maalik nahi hota, koi bhi thread `release()` kar sakta hai aur Java nahi rokta. Chaabi mili hi nahi thi, phir
bhi lauta di, to printer pe ab 4 log. `release()` sirf tab karo jab `tryAcquire()` ne `true` diya ho.

Ek aur: read lock pakad ke write lock maanga to deadlock, kyunki tu read ke chhutne ka intezaar kar raha hai aur wo read tu hi
pakde baitha hai. Aur koi party pahunchi hi nahi to baaki hamesha ruke rahenge, isliye `await` ka timeout wala version lagao.

Taala likhne se pehle socho: kya taala chahiye bhi? Counter ke liye `AtomicInteger`, map ke liye `ConcurrentHashMap`, kaam pass
karne ke liye `BlockingQueue`, aur kai async kaam ke liye `CompletableFuture.allOf`. Apna lock aakhri raasta hai.

## Tera project

Usercrud ka RateLimitController "ek second me kitne" (1 sec me 5, fixed window, `synchronized`) hai, semaphore nahi.
Semaphore ki soch alag hai: "ek saath kitne andar" (printer pe 3). Rate (per second) = window / token bucket, concurrency
(ek saath) = semaphore.
