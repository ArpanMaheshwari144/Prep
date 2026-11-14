# Spring Proxy: JDK Dynamic vs CGLIB

## Scene

Asli bean ek bada aadmi hai, aur uske darwaze pe ek **secretary (proxy)** baithi hai. Tera har call pehle secretary se guzarta hai. Plain Java me khud secretary bana ke chalaya:

```java
class RealBank implements Bank {
    public void transfer() { print("asli transfer()"); this.audit(); }   // andar se call
    public void audit()    { print("asli audit()"); }
}
proxy.transfer();   // bahar se, secretary ke through
```

```
[SECRETARY] BEGIN  transfer
   asli transfer()
   asli audit()                 <- audit ke liye secretary NAHI aayi
[SECRETARY] COMMIT transfer
class = $Proxy0
```

`@Transactional` yahi secretary hai: BEGIN, asli kaam, phir COMMIT ya ROLLBACK. Wo 8 line ki plumbing (`setAutoCommit(false)`, commit, rollback, close) jo har method me haath se likhni padti, Spring ek jagah likh ke sab methods pe laga deta hai.

## Dikkat (self-invocation)

`transfer()` bahar se aaya, to secretary ne BEGIN / COMMIT kiya. Par andar se `this.audit()` seedha asli object pe chala, secretary ke paas se guzra hi nahi. Matlab `audit()` pe `@Transactional` laga ho to wo lagta hi nahi, aur koi error bhi nahi aata.

## Hal

`audit()` ko alag bean me nikaal do, taaki uska call bhi secretary ke through jaaye.

## Proxy kab banta hai, kab nahi

Asli Spring pe do service: ek me koi khaas annotation nahi, doosri me ek normal method aur ek `@Async` method.

```java
@Service class PlainService { void hello() }
@Service class MixService   { void simple();  @Async void bhejo() }
```

```
PlainService mila: PlainService                  <- asli object, koi proxy nahi
   hello() chala, thread = main
MixService mila:   MixService$$SpringCGLIB$$0    <- proxy mila
   simple() chala, thread = main                 <- proxy ne seedha aage bhej diya
   bhejo() chala, thread = SimpleAsyncTaskExecutor-1   <- proxy ne doosre thread pe chalaya
```

`@Transactional`, `@Async` khud kuch nahi karte, ye bas sticker hain. App start pe (bean ki zindagi ka step 5) Spring har bean dekhta hai: kisi method pe aisa sticker mila jo use samajh aata hai, to poore bean ke aage ek dhaancha (proxy) khada kar deta hai. Proxy me tera method nahi hota, uske andar sirf asli bean ka pata hota hai. Call aaye to proxy sticker padhta hai (`@Transactional` = BEGIN / COMMIT, `@Async` = doosra thread, sticker nahi = seedha aage), aur phir wahi asli bean ka method chalata hai. Tera method chalta hamesha hai, bas beech me dhaancha aa jaata hai.

- Proxy banta hai: `@Transactional`, `@Async`, `@Cacheable`, `@PreAuthorize`, `@Retryable`, apna `@Aspect` (jahan method ke aage-peeche kuch karna ho).
- Proxy nahi banta: `@Service`, `@Component`, `@Autowired`, `@Value`, `@GetMapping` (ye register me naam likhne, jodne, URL batane ke liye hain).
- Bean me ek bhi aisa sticker nahi, to proxy hi nahi; ek method pe bhi hai, to poora bean lapeta jaata hai, baaki methods proxy se seedha aage chale jaate hain.

## Spring ne yahi raasta kyun chuna

Java me "har method call se pehle kuch chalao" jaisa koi hook nahi hai: `orderService.place()` likha to JVM seedha us object ke method pe jaata hai. Beech me kuch ghusane ke do hi raaste hain: (1) method ka code (bytecode) badlo, ya (2) `orderService` jis object ki taraf ishara karta hai wo badal do. Spring ke paas doosra pehle se tha, kyunki object Spring hi banata aur deta hai (IoC + DI), to asli ki jagah dhaancha de dena lagbhag muft tha. Aur JDK me 2000 se `java.lang.reflect.Proxy` bana hua hai: na alag compiler, na build badlaav, plain jar daalo aur chalo. Spring 2004 me EJB ke bhaari server-setup ke khilaaf hi aaya tha (Rod Johnson ki kitaab "J2EE Development without EJB").

Pehla raasta bhi hai: AspectJ (compile ke waqt ya `-javaagent` se bytecode badalta), `@EnableTransactionManagement(mode = AdviceMode.ASPECTJ)` se on hota, isme andar wala `this.audit()` bhi pakda jaata. Lombok `@RequiredArgsConstructor` bhi isi pehle raaste jaisa hai (compile ke waqt constructor likh deta). Keemat proxy ki: ek extra object, har call me ek chhota kadam, aur upar wale trap (`this.`, `final`, `new`). Badle me 100 method me plumbing nahi likhni padti.

## Secretary ke 2 roop

- **JDK proxy:** class koi interface implement karti ho, to proxy wahi interface pehen leti hai (plain Spring me). Upar `$Proxy0` yahi hai.
- **CGLIB:** Spring teri class ka ek nakli beta (subclass) bana deta hai. Plain Spring me ye tab aata hai jab interface na ho; Spring Boot 2 se interface ho tab bhi default yahi hai. Isliye `final` / `private` method pe proxy nahi lagta, kyunki subclass unhe override hi nahi kar sakti.

Tera usercrud: `/internals` endpoint ([`SpringInternalsDemo.java`](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/demo/SpringInternalsDemo.java)) me `userService.getClass()` ne `UserService$$SpringCGLIB$$0` print kiya: tujhe asli bean nahi, uski secretary mili thi. `createWithSimulatedFailure()` ne `RuntimeException` phenka, to secretary ne rollback kiya aur DB me user nahi bacha. Ye tune live dekha tha.

## Trap (prod)

```java
@Transactional
public void transfer() throws IOException {
    debit(); credit();
    throw new IOException("bank timeout");   // checked exception
}
```

Default me secretary sirf `RuntimeException` pe rollback karti hai. Checked exception aaye to wo **commit** kar deti hai, yaani paisa kat gaya aur pahuncha nahi. Ilaaj hai `@Transactional(rollbackFor = Exception.class)`.
