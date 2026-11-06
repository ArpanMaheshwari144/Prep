# JDK vs JRE vs JVM

## Scene

Tune todoapp banaya. Laptop pe code likha aur `javac` se compile kiya, isliye wahan JDK chahiye tha. Server pe sirf jar chalana hai, compile kuch nahi karna, isliye wahan JRE kaafi hai. Dono ke andar jo engine asal me bytecode chalata hai, wo JVM hai.

```
JDK  = JRE + javac, jar, jdb        (banane wale ke liye)
 JRE = JVM + libraries (java.util)  (chalane wale ke liye)
  JVM = bytecode chalata + GC        (engine)

.java --javac--> .class (bytecode, har jagah same)
                    |
       Windows JVM   Linux JVM   Mac JVM     <- har OS ka apna JVM
```

Java platform-independent hai, par JVM nahi. Bytecode har jagah ek jaisa hota hai, sirf har OS ka JVM alag hota hai.

## Dikkat -> hal

JVM shuru me bytecode ko line-by-line translate karta hai, aur ye dheema hai. Jo code baar-baar chalta hai (hot code), use JIT ek baar native code me badal ke rakh leta hai, taaki agli baar translate na karna pade. Mart ka bill banane wala ek loop 5 baar chala ke dekha:

```
== normal ==              == -Xint (JIT band) ==
round 1: 3411 us          round 1: 23042 us
round 2: 2671 us          round 2: 24315 us
round 3: 2202 us          round 3: 22794 us
round 4: 1742 us          round 4: 23065 us
round 5: 1737 us          round 5: 21784 us
```

JIT ke saath har round tez hota gaya. JIT band kiya to har round utna hi dheema raha.

## Prod wala trap

Deploy ke turant baad pehli kuch requests dheemi aati hain, phir app tez ho jaati hai. Ye bug nahi hai, ye JIT ka warm-up hai (upar ka round 1). Isliye naye deploy ke pehle minute ki latency dekh ke ghabrana nahi, aur load test ke pehle round ke number ko asli number mat maano.

## Tera project

```
todoapp     FROM eclipse-temurin:21-jre   <- sirf chalana hai, jar pehle se bana hua
Insurance   FROM openjdk:8-jdk            <- poora JDK, server pe zaroorat se bada
```

todoapp ki Dockerfile me tune khud likha hai: "JRE chahiye sirf (JDK nahi, humne jar pehle build kiya)". Insurance me poori JDK wali image hai, jo server ke liye zaroorat se badi hai, kyunki wahan compile karne wale tools chahiye hi nahi.
