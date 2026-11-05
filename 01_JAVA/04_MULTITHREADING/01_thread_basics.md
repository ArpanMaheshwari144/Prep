# Thread Kya Hai + Banane Ke 2 Tarike

## Scene: mausi ka mart, ek counter ya teen

Mart me ek hi counter hai aur teen log line me khade hain. Har ek ki billing me 300 ms lagte hain, to teesre ko 900 ms baad
maal milta hai. Mausi ne 3 counter khol diye, to teeno ek saath 300 ms me nipat gaye. Ek counter = ek thread.

API me bhi yahi hota hai: ek API teen DB call karta hai (user, orders, recommendations), aur har call 300 ms ki hai. Chala ke
dekha:

```
Ek counter (bina thread):          Teen counter (start()):             Galti se run():
  user aaya    (thread: main)        user aaya   (thread: counter-1)     user aaya   (thread: main)
  orders aaya  (thread: main)        orders aaya (thread: counter-2)     orders aaya (thread: main)
  recomm aaya  (thread: main)        recomm aaya (thread: counter-3)     recomm aaya (thread: main)
  kul: 907 ms                        kul: 305 ms                          kul: 902 ms
```

## Khaas baat

Naya counter kholne ke do tareeke hain: `extends Thread`, ya `implements Runnable`. Aaj kal log seedha lambda likhte hain:
`new Thread(() -> kaam)`.

Runnable behtar hai, kyunki Java me ek class sirf ek hi class ko extend kar sakti hai. Agar tune Thread extend kar diya, to
wo class kisi aur ko extend nahi kar paayegi.

Counter kholne ka kaam `start()` karta hai. `run()` bas wahi kaam usi purane counter (main thread) pe chala deta hai.

## Prod wala trap

Upar ka teesra column dekh. `run()` likhne pe na error aata hai, na warning, aur code chal bhi jaata hai. Par sab kuch main
thread pe ek ke baad ek hota hai, isliye API ab bhi 900 ms leta hai. Ye bug code padh ke nahi pakda jaata. Ye sirf response
time ka graph dekh ke pakda jaata hai, jaise New Relic ya Grafana me.

```java
Thread t1 = new Thread(kaam, "counter-1");
t1.run();     // kaam chala: main       <- naya thread bana hi nahi
Thread t2 = new Thread(kaam, "counter-2");
t2.start();   // kaam chala: counter-2  <- asli naya thread
```

## Tera project

Todoapp ya usercrud jaise Spring Boot app me tujhe thread kabhi khud nahi banana pada. Iski wajah ye hai ki andar Tomcat har
aane wali request ko apne thread pool ka ek thread de deta hai, by default 200 tak. Yaani mart ke counter Tomcat pehle se
khol ke baitha hai.

## Interview line

```
A thread is one independent line of execution, like opening another billing counter.
If an API makes three 300 ms DB calls one after another it takes 900 ms; on three threads it is
about 300 ms. I prefer Runnable or a lambda over extending Thread because Java allows only one
superclass. The classic trap is calling run() instead of start(): it compiles and works, but
runs on the current thread, so you get no parallelism and no error.
```
