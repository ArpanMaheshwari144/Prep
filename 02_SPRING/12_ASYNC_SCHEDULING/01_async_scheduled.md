# @Async + @Scheduled — background threads + cron jobs (Spring)

> App me "alag thread pe kaam" (@Async) aur "khud-ba-khud timer pe kaam" (@Scheduled).
> Dono core Spring — interview me aate. usercrud pe live kiya (24-Aug): 3 email parallel + heartbeat har 3s.

---

## 1-LINE KYA
```
@Async     = method ko ALAG thread-pool pe chala do -> caller WAIT nahi karta (fire-and-forget).
             use: email bhejo, report banao, notification -> user response block na ho.
@Scheduled = method KHUD-BA-KHUD fire (timer/cron) -> koi trigger nahi.
             use: daily cleanup, cache-refresh, report-generation, health-ping.
```

---

## ═══ HANDS-ON (usercrud, 24-Aug) — STEP BY STEP ═══

### STEP 1 — enable (main class pe)
```java
@SpringBootApplication
@EnableAsync          // @Async ko on karta
@EnableScheduling     // @Scheduled ko on karta
public class UsercrudApplication { ... }
```
Ye 2 annotation na ho to @Async/@Scheduled IGNORE ho jaate (silently kuch nahi hota).

### STEP 2 — DemoJobs component (dono methods)
```java
@Component
public class DemoJobs {
    private static final Logger log = LoggerFactory.getLogger(DemoJobs.class);

    @Async                                             // -> alag thread
    public void sendEmail(String to) throws InterruptedException {
        log.info(">> sendEmail START to={} | THREAD = {}", to, Thread.currentThread().getName());
        Thread.sleep(3000);                            // maano email me 3 sec lage
        log.info(">> sendEmail DONE  to={} | THREAD = {}", to, Thread.currentThread().getName());
    }

    @Scheduled(fixedRate = 3000)                       // -> har 3 sec khud fire
    public void heartbeat() {
        log.info("## heartbeat @ {} | THREAD = {}", System.currentTimeMillis(), Thread.currentThread().getName());
    }
}
```

### STEP 3 — @Async trigger (controller) + security permit
```java
@RestController
public class AsyncDemoController {
    @Autowired private DemoJobs demoJobs;

    @GetMapping("/async-demo")
    public String trigger() throws InterruptedException {
        demoJobs.sendEmail("a@x.com");
        demoJobs.sendEmail("b@x.com");
        demoJobs.sendEmail("c@x.com");
        return "3 emails triggered - endpoint TURANT return (emails background me)";
    }
}
// SecurityConfig: .requestMatchers("/async-demo").permitAll()
```

### STEP 4 — run + observe (jo live dikha)
```
mvn spring-boot:run
-> heartbeat shuru se har 3s (scheduling-1 thread), koi trigger nahi:
   ## heartbeat @ ... | THREAD = scheduling-1     (06 -> 09 -> 12 -> 15 ...)

-> GET http://localhost:8080/async-demo  hit kiya:
   3 email EK saath (.084) START, par TEEN ALAG thread (PARALLEL):
     >> sendEmail START a@x.com | THREAD = task-1
     >> sendEmail START b@x.com | THREAD = task-2
     >> sendEmail START c@x.com | THREAD = task-3
   endpoint TURANT return hua (block nahi).
   beech me heartbeat CHALTA RAHA (14:47:21) -> main app roka nahi.
   3 sec baad teeno DONE (.098) ek saath.
```

---

## ★★ ASLI SAMAJH (jo logs ne saabit kiya)
```
1. @Scheduled = KHUD fire, scheduling-1 pool. fixedRate=3000 -> har 3s start-to-start.
2. @Async = teeno email PARALLEL (task-1/2/3), sequential nahi. 3x3=9s ka kaam 3s me + endpoint instant.
3. 3 ALAG THREAD-POOL ek saath dikhe:
     nio-8080-exec-*  = Tomcat request threads (HTTP)
     task-*           = @Async default pool (SimpleAsyncTaskExecutor / configurable)
     scheduling-1     = @Scheduled ka single-thread pool
4. Main/request thread BLOCK nahi hua -> background ne foreground roka nahi. yahi @Async ka point.
```

## ★ TRAP / GOTCHA (interview me poochte)
```
1. SELF-INVOCATION trap: @Async / @Scheduled PROXY se kaam karte. agar SAME class ke andar se
   this.sendEmail() call karo -> proxy bypass -> ASYNC NAHI hota (seedha same thread). isliye
   DemoJobs ko ALAG bean se call kiya (controller -> demoJobs.sendEmail).  [MOST-ASKED]
2. @EnableAsync/@EnableScheduling bhoole -> annotation SILENTLY ignore.
3. @Async return: void ya CompletableFuture<T> (result chahiye to Future). plain return-value bekaar.
4. @Scheduled options: fixedRate (start-to-start), fixedDelay (end-to-start), cron="0 0 * * * *".
5. default @Async pool unbounded ho sakta -> production me apna ThreadPoolTaskExecutor bean do (bounded).
```

## ★ CONNECT (resilience/concurrency family)
```
@Async (background)  +  @Retryable (retry on fail)  +  Circuit-Breaker (fail-fast)  = resilience toolkit.
@Scheduled = poor-man's cron (single-node). multi-node pe ShedLock / Quartz (ek hi node chale).
```

## INTERVIEW LINE
```
"@Async se method alag thread-pool pe chalta - fire-and-forget (email/report), caller block nahi hota;
 @EnableAsync + return void/CompletableFuture. @Scheduled se periodic jobs - fixedRate/fixedDelay/cron,
 @EnableScheduling. Gotcha: self-invocation pe async nahi hota (proxy bypass) - alag bean se call karo.
 Maine usercrud pe kiya - 3 email parallel task-threads pe, endpoint instant return; heartbeat har 3s."
```

### Dobara kaise
```
main-class: @EnableAsync @EnableScheduling -> method pe @Async / @Scheduled(fixedRate=..) -> ALAG bean se call.
run -> logs me alag thread-name (task-*/scheduling-1) + endpoint instant.
```

[← Spring README](../README.md)
