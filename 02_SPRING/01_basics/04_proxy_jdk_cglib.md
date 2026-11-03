# Spring Proxy: JDK Dynamic vs CGLIB

## Kahani

`transfer()` me asli kaam sirf 2 line ka hai. Par agar transaction haath se sambhalo, to har method me ye 8 line ki plumbing likhni padti hai:

```java
conn.setAutoCommit(false);
try { /* asli kaam */ conn.commit(); }
catch (Exception e) { conn.rollback(); throw e; }
finally { conn.close(); }
```

100 method hain to 100 baar wahi 8 line, aur kisi ek jagah `rollback` likhna bhool gaye to bug. Spring kehta hai: tu method pe bas `@Transactional` likh de, ye plumbing main ek jagah likh ke sab methods pe laga dunga.

## Tasveer

Asli bean ek bada aadmi hai, aur uske darwaze pe ek **secretary (proxy)** baithi hai.

```
Tu -> [SECRETARY: BEGIN] -> asli transfer() -> [SECRETARY: COMMIT ya ROLLBACK] -> Tu
```

Tu asli object ko kabhi seedha call nahi karta. Tera har call pehle secretary ke paas jaata hai. Secretary pehle transaction BEGIN karti hai, phir asli `transfer()` chalati hai, aur aakhir me COMMIT ya ROLLBACK karke jawab tujhe lautati hai.

## 4 khaas baat

1. **Ye koi jaadu nahi hai, ye wahi code hai jo tu haath se likhta.** Spring use runtime pe bana ke chala deta hai. Asli fayda kam code likhna nahi hai. Fayda ye hai ki plumbing ek jagah aur sahi likhi jaati hai (DRY), aur business logic alag saaf dikhta hai (separation of concerns).
2. **Secretary do tarah ki hoti hai:**
   - **JDK dynamic proxy:** agar class koi interface implement karti hai, to proxy wahi interface pehen leti hai, jaise wahi badge laga ke baithi ho.
   - **CGLIB:** agar interface nahi hai, to Spring teri class ka ek nakli beta (subclass) bana deta hai.
   Spring Boot 2 se default CGLIB hai.
3. **Self-invocation trap:** agar class ke andar se hi `this.b()` call kiya, to wo call secretary ke paas se guzarti hi nahi. Isliye `b()` pe laga `@Transactional` lagta hi nahi. Ilaaj ye hai ki `b()` ko ek alag bean me nikaal do.
4. **`final` aur `private` method pe proxy nahi lagta**, kyunki CGLIB unhe override nahi kar sakta. Aise method pe annotation chup-chaap kaam nahi karta aur koi error bhi nahi aata. Ek aur baat: `@Configuration` class bhi CGLIB proxy hoti hai, isliye uske andar `dependency()` ko do baar call karo to bhi ek hi singleton milta hai.

## Prod wala trap

```java
@Transactional
public void transfer() throws IOException {
    debit(); credit();
    throw new IOException("bank timeout");   // checked exception
}
```

Default me secretary sirf `RuntimeException` aane pe rollback karti hai. Checked exception aaye to wo **commit** kar deti hai, matlab debit ho gaya aur credit nahi hua. Ilaaj hai `@Transactional(rollbackFor = Exception.class)`. Ye wahi banking wali kahani hai jahan aadha paisa kat jaata hai.

## Tera project

usercrud me `/internals` endpoint tune khud likha hai ([`SpringInternalsDemo.java`](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/demo/SpringInternalsDemo.java)). Usme `userService.getClass()` ne `UserService$$SpringCGLIB$$0` print kiya, matlab tujhe asli bean nahi, uski secretary mili thi. Aur jab `createWithSimulatedFailure()` ne `RuntimeException` phenka, to secretary ne rollback kar diya aur DB me user nahi bacha. Ye tune live dekha tha.

## Interview line

```
@Transactional, @Async and @Cacheable work through proxies that wrap the bean and add logic
around the real call. JDK dynamic proxy needs an interface; CGLIB subclasses the class,
and Spring Boot uses CGLIB by default. Self-invocation bypasses the proxy, final methods
can't be proxied, and by default only unchecked exceptions trigger rollback.
```
