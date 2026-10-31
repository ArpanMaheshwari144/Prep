# Spring Proxy: JDK Dynamic vs CGLIB

## Kahani

`transfer()` me asli kaam sirf 2 line ka hai. Par transaction haath se sambhalo to har method me 8 line ki plumbing likhni padti hai:

```java
conn.setAutoCommit(false);
try { /* asli kaam */ conn.commit(); }
catch (Exception e) { conn.rollback(); throw e; }
finally { conn.close(); }
```

100 method ka matlab hai 100 baar wahi 8 line, aur kahin ek jagah `rollback` bhool gaye to bug. Spring kehta hai: method pe bas `@Transactional` likh do, ye plumbing main ek jagah se sab pe laga dunga.

## Tasveer

Asli bean ek bada aadmi hai, aur uske darwaze pe ek **secretary (proxy)** baithi hai.

```
Tu -> [SECRETARY: BEGIN] -> asli transfer() -> [SECRETARY: COMMIT ya ROLLBACK] -> Tu
```

Tu asli object ko kabhi call hi nahi karta. Tera har call pehle secretary ke paas jaata hai.

## 4 khaas baat

1. **Ye koi jaadu nahi hai, wahi code hai jo tu haath se likhta.** Spring use runtime pe bana ke chala deta hai. Asli fayda kam code nahi hai. Fayda ye hai ki plumbing ek jagah aur sahi likhi jaati hai (DRY), aur business logic alag dikhta hai (separation of concerns).
2. **Secretary do tarah ki hoti hai:**
   - **JDK dynamic proxy:** class koi interface implement karti ho to proxy wahi interface pehen leti hai, jaise wahi badge laga ke baithi ho.
   - **CGLIB:** interface na ho to Spring teri class ka nakli beta (subclass) bana deta hai.
   Spring Boot 2 se default CGLIB hai.
3. **Self-invocation trap:** class ke andar se `this.b()` call kiya to wo secretary ke paas se guzarta hi nahi. Isliye `b()` ka `@Transactional` lagta hi nahi. Ilaaj: `b()` ko alag bean me nikaalo.
4. **`final` / `private` method pe proxy nahi lagta**, kyunki CGLIB unhe override nahi kar sakta. Annotation chup-chaap kaam nahi karta, koi error bhi nahi aata. Aur `@Configuration` class bhi CGLIB proxy hoti hai, isliye uske andar `dependency()` ko do baar call karo to bhi ek hi singleton milta hai.

## Prod wala trap

```java
@Transactional
public void transfer() throws IOException {
    debit(); credit();
    throw new IOException("bank timeout");   // checked exception
}
```

Default me secretary sirf `RuntimeException` pe rollback karti hai. Checked exception pe wo **commit** kar deti hai, yaani debit ho gaya aur credit nahi hua. Ilaaj: `@Transactional(rollbackFor = Exception.class)`. Ye wahi banking wali kahani hai jahan aadha paisa kat jaata hai.

## Tera project

usercrud me `/internals` endpoint tune khud likha ([`SpringInternalsDemo.java`](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/demo/SpringInternalsDemo.java)). Usme `userService.getClass()` ne `UserService$$SpringCGLIB$$0` print kiya, yaani asli bean nahi, uski secretary mili. Aur `createWithSimulatedFailure()` ne `RuntimeException` phenka to secretary ne rollback kiya, aur DB me user nahi bacha. Ye tune live dekha tha.

## Interview line

```
@Transactional, @Async and @Cacheable work through proxies that wrap the bean and add logic
around the real call. JDK dynamic proxy needs an interface; CGLIB subclasses the class,
and Spring Boot uses CGLIB by default. Self-invocation bypasses the proxy, final methods
can't be proxied, and by default only unchecked exceptions trigger rollback.
```
