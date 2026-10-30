# JDK vs JRE vs JVM

## Kahani

Tune todoapp banaya. Laptop pe code likha aur `javac` se compile kiya, isliye wahan **JDK** chahiye tha. Server pe sirf jar chalana hai, compile kuch nahi karna, isliye wahan **JRE** kaafi hai. Dono ke andar jo asli engine bytecode chalata hai, wo **JVM** hai.

## Tasveer

Ek dabbe ke andar doosra:

```
JDK  = JRE + javac, jdb, jar        (banane wale ke liye)
 JRE = JVM + libraries (java.util)  (chalane wale ke liye)
  JVM = bytecode chalata + GC        (engine)
```

Write Once, Run Anywhere:

```
.java --javac--> .class (bytecode, har jagah same)
                    |
       Windows JVM   Linux JVM   Mac JVM     <- har OS ka apna JVM
```

## 3 khaas baat

1. **Java platform-independent hai, par JVM nahi.** Bytecode har jagah ek hi hota hai, bas har OS ka JVM alag hota hai.
2. **Java compile bhi hoti hai aur interpret bhi.** `javac` code ko bytecode banata hai. JVM shuru me bytecode ko line-by-line translate karta hai. Jo code baar-baar chalta hai (hot code), use JIT ek baar native code me likh ke rakh leta hai. Chala ke dekha tha: round 1 me 3120 aaya, round 3 se ~450 aaya. JIT band karke (`-Xint`) chalaya to har round ~27000 hi raha.
3. **Java 11 se Oracle alag JRE nahi deta.** Ab ya to JDK lagate hain, ya `jlink` se chhota runtime banate hain. Docker ki `-jre` image wahi chhota runtime hai.

## Prod wala trap

Deploy ke turant baad ki pehli kuch requests dheemi aati hain, phir app tez ho jaati hai. Ye bug nahi hai, JIT ka warm-up hai. Isliye naye deploy ke pehle minute ki latency dekh ke ghabrana nahi chahiye, aur load test ke pehle round ke number ko asli number mat maano.

## Tera project

```
todoapp     FROM eclipse-temurin:21-jre   <- sirf chalana hai, jar pehle se bana hua
Insurance   FROM openjdk:8-jdk            <- poora JDK, server pe zaroorat se bada
```

todoapp ki Dockerfile me tune khud likha hai: "JRE chahiye sirf (JDK nahi, humne jar pehle build kiya)".

## Interview line

```
JVM runs bytecode, JRE is JVM plus the libraries needed to run, JDK is JRE plus compiler and tools.
Bytecode is the same everywhere and each OS has its own JVM, which makes Java portable.
The JVM interprets first and JIT compiles hot code to native, which is why apps warm up.
```
