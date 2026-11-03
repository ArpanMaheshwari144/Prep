# @Async + @Scheduled

## Kahani: signup ka welcome email

Ravi ne signup kiya, aur welcome email bhejne me 3 sec lagte hain. Ravi 3 sec tak button ghoomta kyun dekhe? Ye kaam Chhotu ko de do: "tu email bhej, main
Ravi ko jawab de raha hoon". Yahi **@Async** hai. Ab doosra kaam: raat 2 baje purane OTP saaf karne hain, aur us waqt koi button nahi dabata. To ek alarm laga do jo khud
baje, yahi **@Scheduled** hai.

## Tasveer (usercrud me live dekha)

```
GET /async-demo -> 3 sendEmail() call
   nio-8080-exec-1  (Tomcat)  -> turant "3 emails triggered" lauta diya
   task-1, task-2, task-3     -> teeno email EK SAATH, 3 sec baad teeno DONE  (9 sec ka kaam 3 me)
   scheduling-1               -> heartbeat har 3 sec, beech me bhi chalta raha
```

`/async-demo` ne 3 baar `sendEmail()` bulaya. Tomcat ke thread ne turant "3 emails triggered" lauta diya. Teeno email task-1, task-2, task-3 pe ek saath chale aur 3 sec baad teeno DONE ho gaye, matlab 9 sec ka kaam 3 sec me hua. Aur scheduling-1 thread pe heartbeat har 3 sec chalta raha, beech me bhi nahi ruka.

Yahan teen alag pool hain. Pehla Tomcat ka hai. Doosra @Async ka hai, jo Boot ka applicationTaskExecutor hai, jisme 8 thread hain aur queue ki koi hadd nahi hai. Teesra scheduler ka hai, jisme sirf EK thread hai.
In dono ke liye `@EnableAsync @EnableScheduling` lagana zaroori hai, warna dono annotation chup-chaap kuch nahi karte.

## Dikkat aur ye kyun

Lamba kaam, jaise email ya report, request ke andar hi chalaya to user tab tak ruka rehta hai. @Async kaam ko doosre thread pe daal deta hai, isliye request turant wapas aa jaati hai.
Periodic kaam, jaise cleanup ya refresh, ko bulane wala koi caller hi nahi hota, isliye uske liye @Scheduled hai. Isme `fixedRate` ek run ke start se agle run ke start tak ka gap hai, `fixedDelay` ek run ke khatam hone se agle ke start tak ka gap hai, aur `cron` se time tay karte hain.

## Trade-off (HLD se jod)

@Async kaam ko usi app ki MEMORY me ek line (queue) me daal deta hai. Fayda ye hai ki ye aasaan hai, bas ek annotation. Kaat-ta ye hai ki app restart ho ya crash ho jaaye, to
line me pade email chale gaye, aur kisi ko pata bhi nahi chalta. Isiliye notification design me Kafka liya tha, kyunki wahan khabar disk pe rehti hai aur worker gire to bhi
bachi rehti hai. Seedha niyam: kaam chhoot jaaye to chalega, to @Async. Kaam khona nahi chahiye, to Kafka ya outbox.

## Asli kaam wala trap

```java
@Scheduled(cron = "0 0 2 * * *")      // raat 2 baje
public void sendDailyReport() { ... }
```

Maan lo app 3 server pe chal rahi hai. Teeno server ka apna scheduler hai, isliye report 3 baar banti hai aur user ko 3 email jaate hain. @Scheduled ko doosre server ka pata hi
nahi hota. Ilaaj hai ShedLock, jo DB me lock rakhta hai aur jo server pehle lock le leta hai wahi job chalata hai. Ya phir Quartz cluster. Ye wahi "ek hi ladka jaaye" wali soch hai.

Kuch aur baatein:
- `this.sendEmail()` se bulaya to wo async nahi chalega, kyunki call proxy ko bypass kar deti hai. Ye wahi cache aur @Transactional wala jaal hai.
- void @Async method me exception aaye to caller pehle hi laut chuka hota hai, isliye wo sirf log me dikhta hai. Result ya error chahiye to `CompletableFuture` lautao.
- Scheduler ka ek hi thread hai, isliye ek lamba job baaki saare jobs ko rok deta hai.
- Production me apna `ThreadPoolTaskExecutor` banao, jisme core, max aur queue ki size tay ho.

## Tera project (usercrud)

`DemoJobs` ek alag bean hai, jisme `@Async sendEmail` aur `@Scheduled(fixedRate = 3000) heartbeat` hain. Controller `demoJobs.sendEmail()` bulata hai, isliye call proxy se hoke jaati hai.

## Interview line

```
@Async runs a method on a separate thread pool so the request returns immediately, like sending an email
after signup; it goes through a proxy, so it must be called from another bean, and for real use I define
my own executor with a bounded queue. But it's in-memory, so if the app dies the pending work is lost;
anything that must not be lost goes through Kafka or an outbox. @Scheduled runs cron jobs, and with
multiple instances I add ShedLock so the job runs on only one node.
```
