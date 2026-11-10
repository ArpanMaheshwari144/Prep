# @Async + @Scheduled

## Ek line me (Arpan ki misaal)

- **@Async** = chhotu ko paise de diye "doodh ka packet le aa", aur main chhotu ke liye ruka nahi, ghar aa gaya. Chhotu laake de dega.
- **@Scheduled** = chhotu ko bol diya "roz raat 2 baje mera phone charging pe laga dena". Koi bulata nahi, ghadi bulati hai.
- Scheduler ke paas ek hi chhotu hai, aur wo ek waqt pe ek hi kaam karta hai. 2 baje wo kuch aur kar raha tha (lamba report), to us
  waqt mera phone charging pe nahi lagega. Kaam bhoolta nahi: haath khaali hote hi charger laga dega, bas der se. Neeche demo me
  heartbeat isi tarah 4 sec der se chala.

## Scene: signup ka welcome email

Ravi ne signup kiya, aur welcome email jaane me 3 sec lagte hain. Ravi 3 sec tak button ghoomta kyun dekhe? Email ka kaam chhotu ko de
do: "tu bhej, main Ravi ko jawab de raha hoon". Yahi `@Async` hai. Raat 2 baje purane OTP saaf karne hain, us waqt koi button nahi
dabata, to alarm laga do jo khud baje. Yahi `@Scheduled` hai. Dono ke liye `@EnableAsync @EnableScheduling` zaroori hai, warna dono
annotation chup-chaap kuch nahi karte.

## Chala ke dekha (plain Java pool, wahi kaam jo Spring andar karta)

```
    6 ms  http-exec-1  GET /async-demo aaya
   44 ms  http-exec-1  lauta diya: 3 emails triggered      <- Ravi ko turant jawab
 3056 ms  task-25      email 1 DONE
 3056 ms  task-26      email 2 DONE
 3056 ms  task-27      email 3 DONE                         <- 9 sec ka kaam 3 sec me
```

Request wala thread 44 ms me laut gaya, aur teeno email alag thread pe ek saath chale. Yahan teen alag pool hain: Tomcat ka, @Async ka
(Boot ka applicationTaskExecutor, 8 thread, queue ki koi hadd nahi), aur scheduler ka, jisme sirf EK thread hai. `@Scheduled` me
`fixedRate` = ek run ke start se agle start tak ka gap, `fixedDelay` = ek run khatam hone se agle start tak, `cron` = ghadi ka time.

## Chhoti dikkat: scheduler ka ek hi chhotu

```
 1015 ms  scheduling-1 heartbeat
 1516 ms  scheduling-1 report shuru (4 sec)
 5533 ms  scheduling-1 report khatam
 5533 ms  scheduling-1 heartbeat      <- 4 sec tak heartbeat chup, phir ek saath
 5534 ms  scheduling-1 heartbeat
```

Ek lamba job baaki sabko rok deta hai. Hal: scheduler ka pool badhao, ya lamba kaam `@Async` pe daal do. Production me apna
`ThreadPoolTaskExecutor` banao, jisme core, max aur queue ki size tay ho.

## HLD se jod

@Async kaam ko app ki MEMORY wali line me rakhta hai. App restart ya crash hua to line me pade email chale gaye, aur kisi ko pata bhi
nahi chalta. Isiliye Notification design me Kafka liya tha, wahan khabar disk pe rehti hai. Niyam: kaam chhoot jaaye to chalega, to
@Async; kaam khona nahi chahiye, to Kafka ya outbox.

## Tera project (usercrud)

`DemoJobs` ek alag bean hai, jisme `@Async sendEmail` aur `@Scheduled(fixedRate = 3000) heartbeat` hain. `/async-demo` pe tune
`task-1, task-2, task-3` ek saath dekhe aur heartbeat beech me bhi chalta raha. Controller `demoJobs.sendEmail()` bulata hai, isliye
call proxy se hoke jaati hai.

## Prod wala trap: 3 server, 3 report

```java
@Scheduled(cron = "0 0 2 * * *")      // raat 2 baje
public void sendDailyReport() { ... }
```

App 3 server pe chal rahi hai, aur teeno ka apna scheduler hai. Report 3 baar banti hai aur user ko 3 email jaate hain, kyunki
@Scheduled ko doosre server ka pata hi nahi. Ilaaj ShedLock hai: DB me lock rakho, jo server pehle lock le wahi job chalaye (ya
Quartz cluster). Ye wahi "ek hi ladka jaaye" wali soch hai.

Do baatein aur. `this.sendEmail()` se bulaya to wo async nahi chalega, kyunki proxy bypass hota hai (cache aur @Transactional wala
jaal). Aur void @Async method me exception aaye to caller pehle hi laut chuka hota hai, isliye wo sirf log me dikhta hai; result ya
error chahiye to `CompletableFuture` lautao.
