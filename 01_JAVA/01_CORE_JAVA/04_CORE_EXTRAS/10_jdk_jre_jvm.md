# JDK vs JRE vs JVM

> **V90 — Core Java Extras: Topic 10**

---

## 🟡 WHY — JDK vs JRE vs JVM Alag Kyun?

→ **JVM** = bytecode execute kare (**WORA ka foundation**)
→ **JRE** = JVM + libraries (run karne ke liye enough)
→ **JDK** = JRE + compiler + tools (**develop karne ke liye**)
→ Nested hain: **JDK ⊃ JRE ⊃ JVM**
→ **Developer ko JDK** chahiye, **user ko sirf JRE**

---

## 🧠 Visualization

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
│  │                rt.jar                              │  │
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

## 📖 STORY (V90 ka quick recap)

→ `HelloWorld.java` → compile → `HelloWorld.class` (bytecode) → **JVM run karta**
→ **JDK** = JRE + compiler + tools
→ **JRE** = JVM + libraries (sirf run karne ke liye)
→ **JVM** = bytecode ko machine pe chalata
→ Har OS pe alag JVM — isliye Java **"Write Once, Run Anywhere"**
→ Sirf run karna? **JRE kaafi**
→ Develop karna? **JDK chahiye**

---

## 🔴 TRAP

> **Server pe sirf run karna hai → JRE.**
> **Develop karna hai → JDK.**

---

## 💬 POWER PHRASE

> *"JVM executes bytecode. JRE is JVM plus libraries needed to run Java. JDK is JRE plus compiler and dev tools. JDK contains JRE which contains JVM."*
