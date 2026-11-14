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
