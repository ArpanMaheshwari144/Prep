# JDK vs JRE vs JVM

> **V90 — Core Java Extras: Topic 10**

---

## WHY — JDK vs JRE vs JVM Alag Kyun?

→ **JVM** = bytecode execute kare (**WORA ka foundation**)
→ **JRE** = JVM + libraries (run karne ke liye enough)
→ **JDK** = JRE + compiler + tools (**develop karne ke liye**)
→ Nested hain: **JDK ⊃ JRE ⊃ JVM**
→ **Developer ko JDK** chahiye, **user ko sirf JRE**

---

## Visualization

```
┌──────────────────────────────────────────────────────────┐
│  JDK (Development Kit)  —  Developer ke liye             │
│                                                          │
│  ┌────────────────────────────────────────────────────┐  │
│  │  JRE (Runtime Environment)  —  User ke liye        │  │
│  │                                                    │  │
│  │  ┌──────────────────────────────────────────────┐  │  │
│  │  │  JVM (Virtual Machine)                       │  │  │
│  │  │  • Bytecode execute karta                    │  │  │
│  │  │  • Platform-specific                         │  │  │
│  │  │  • GC manage karta                           │  │  │
│  │  └──────────────────────────────────────────────┘  │  │
│  │                                                    │  │
│  │  + JRE extras: Class libraries (java.lang, util)   │  │
│  │                rt.jar (Java 8 tak; 9+ me modules)  │  │
│  └────────────────────────────────────────────────────┘  │
│                                                          │
│  + JDK extras: javac (compiler)                          │
│                jdb (debugger)                            │
│                jar (packager)                            │
│                javadoc                                   │
└──────────────────────────────────────────────────────────┘

         JDK  ⊃  JRE  ⊃  JVM
       (bada)  (medium)  (chota)
```

**Bada dabba** → chota dabba → aur chota dabba.

---

## STORY (V90 ka quick recap)

→ `HelloWorld.java` → compile → `HelloWorld.class` (bytecode) → **JVM run karta**
→ **JDK** = JRE + compiler + tools
→ **JRE** = JVM + libraries (sirf run karne ke liye)
→ **JVM** = bytecode ko machine pe chalata
→ Har OS pe alag JVM — isliye Java **"Write Once, Run Anywhere"**
→ Sirf run karna? **JRE kaafi**
→ Develop karna? **JDK chahiye**

---

## ★ TRAP — Platform independent kyun? (WORA visual)

```
Write Once, Run Anywhere

   .java
     ↓ javac
   .class (bytecode — UNIVERSAL)
     ↓
   Windows JVM    Linux JVM    Mac JVM
     ↓               ↓              ↓
   Windows OS     Linux OS      Mac OS

   Har OS ka APNA JVM · bytecode sab jagah SAME = platform independence
   (Java platform-independent hai, JVM platform-DEPENDENT hai)
```

## ★ TRAP — Java compiled hai ya interpreted? -> DONO

```
javac      .java -> .class (bytecode)          <- compile (ek baar)
JVM        bytecode ko pehle INTERPRET karta     <- shuru me line-by-line
JIT        jo code baar-baar chale (HOT), use NATIVE machine code me badal deta
           -> isliye Java app thodi der chalne ke baad TEZ ho jaati ("warm-up")
```

**BYTECODE kya dikhta** (`javap -c` se .class ke andar):
```
tera code:   return a + b;

bytecode:    iload_0    a uthao
             iload_1    b uthao
             iadd       jodo
             ireturn    lautao
```
CPU ise seedha nahi samajhta -> beech me JVM chahiye.

**JIT = TRANSLATOR ki misaal:**
```
tu Hindi bolta, saamne wala sirf Japanese samajhta, beech me translator

INTERPRETER = har baar sun ke translate
   "a+b jodo" 20 lakh baar bola -> 20 lakh baar translate -> DHEEMA

JIT = translator dekhta "ye line to baar-baar aa rahi" (HOT)
   ek baar Japanese me LIKH ke rakh leta (native code)
   ab sirf likha hua padhta -> TEZ
```
`java -version` me `mixed mode` = dono (shuru me interpreter, HOT code pe JIT).

**Chala ke dekha** (`add()` 20 lakh baar = 1 round; us = microsecond):
```
           JIT ON (normal)              JIT BAND (-Xint, sirf translator)
round 1    3120   translate ho raha     27201
round 2    2320   JIT likh raha         27653
round 3     475   likha hua padh raha   26899
round 4     465   tez                   26616
round 5     409   tez                   27728
round 6     421   tez                   26765   <- kabhi tez nahi hua
```
Round 3 se ~6x tez = WARM-UP. Aakhri round: JIT ke saath 421 vs bina 26765 = ~60x farak.
Kaam me: deploy ke turant baad pehli requests dheemi, phir app tez = JIT warm-up.

---

## TRAP

> **Server pe sirf run karna hai → JRE.**
> **Develop karna hai → JDK.**
> ★ Java 11+ me Oracle alag JRE download nahi deta — JDK hi lagate ya `jlink` se chhota runtime banate.
>   (Docker image me aksar "JRE" wali base image = wahi chhota runtime.)

---

## POWER PHRASE

> *"JVM executes bytecode. JRE is JVM plus libraries needed to run Java. JDK is JRE plus compiler and dev tools. JDK contains JRE which contains JVM."*
