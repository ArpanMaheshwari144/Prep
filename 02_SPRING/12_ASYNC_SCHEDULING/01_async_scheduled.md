# @Async + @Scheduled

## Kahani: signup ka welcome email

Ravi ne signup kiya, welcome email me 3 sec lagte. Ravi 3 sec button ghoomta kyun dekhe? Chhotu ko de do: "tu bhej, main
Ravi ko jawab de raha" = **@Async**. Raat 2 baje purane OTP saaf karne, koi button nahi dabata: alarm laga do jo khud
baje = **@Scheduled**.

## Tasveer (usercrud me live dekha)

```
GET /async-demo -> 3 sendEmail() call
   nio-8080-exec-1  (Tomcat)  -> turant "3 emails triggered" lauta diya
   task-1, task-2, task-3     -> teeno email EK SAATH, 3 sec baad teeno DONE  (9 sec ka kaam 3 me)
   scheduling-1               -> heartbeat har 3 sec, beech me bhi chalta raha
```

Teen alag pool: Tomcat ka · @Async ka (Boot ka applicationTaskExecutor, 8 thread, queue ki hadd nahi) · scheduler ka EK thread.
Chahiye: `@EnableAsync @EnableScheduling`, warna dono chupchaap kuch nahi karte.

## Dikkat -> kyun

Lamba kaam (email, report) request ke andar = user ruka. @Async = kaam doosre thread pe, request turant wapas.
Periodic kaam (cleanup, refresh) ka koi caller hi nahi = @Scheduled (`fixedRate` start-to-start, `fixedDelay`
end-to-start, `cron`).

## Trade-off (HLD se jod)

@Async kaam ko usi app ki MEMORY me ek line me daalta. Fayda: aasaan, ek annotation. Kaatta: app restart / crash =
line me pade email gaye, kisi ko pata nahi. Notification design me isiliye Kafka: khabar disk pe, worker gire to bhi
bachi. "Chhoot jaaye to chalega" = @Async · "khona nahi" = Kafka / outbox.

## Asli kaam wala trap

```java
@Scheduled(cron = "0 0 2 * * *")      // raat 2 baje
public void sendDailyReport() { ... }
```

App 3 server pe = teeno ka apna scheduler = report 3 baar, user ko 3 email. @Scheduled ko doosre server ka pata hi
nahi. Ilaaj: ShedLock (DB me lock, jo pehle le wahi chalaye) ya Quartz cluster. Wahi "ek hi ladka jaaye" soch.

Aur: `this.sendEmail()` = async nahi (proxy bypass, cache / @Transactional wala jaal) · void @Async me exception =
caller laut chuka, sirf log; result / error chahiye to `CompletableFuture` · scheduler ka ek thread: ek lamba job
baaki sab rok deta · production me apna `ThreadPoolTaskExecutor` (core / max / queue tay).

## Tera project (usercrud)

`DemoJobs` (alag bean): `@Async sendEmail`, `@Scheduled(fixedRate = 3000) heartbeat`. Controller `demoJobs.sendEmail()` bulata.

## Interview line

```
@Async runs a method on a separate thread pool so the request returns immediately, like sending an email
after signup; it goes through a proxy, so it must be called from another bean, and for real use I define
my own executor with a bounded queue. But it's in-memory, so if the app dies the pending work is lost;
anything that must not be lost goes through Kafka or an outbox. @Scheduled runs cron jobs, and with
multiple instances I add ShedLock so the job runs on only one node.
```
