# Garbage Collection

> **V90 — Core Java Extras: Topic 19**

---

## STORY — C++ vs Java

→ **C++** mein tune object banaya, kaam hua, `delete` karna **bhool gaya** — **memory leak**
→ Java mein ye tension nahi
→ **Koi reference nahi = GC eligible**
→ JVM background mein check karta rehta hai — unreachable objects **saaf kar deta**
→ `System.gc()` sirf **hint** hai, guarantee nahi
→ `finalize()` deprecated hai — **use mat karo**
→ C++: manually delete → bhool gaye → memory leak. **Java: GC khud karta hai**
→ Object ka koi reference nahi → GC eligible
→ `System.gc()` = sirf hint, guarantee nahi

---

## GC Eligible Kab?

```java
// Case 1 — reference null kar diya
Student s = new Student("Arpan");
s = null;                              // koi point nahi kar raha → GC le jaayega

// Case 2 — reference reassign kiya
Student s = new Student("Arpan");
s = new Student("Rahul");              // "Arpan" wala ORPHAN → GC

// Case 3 — method khatam hua
void method() {
    Student s = new Student("Arpan");
}                                      // method khatam → s scope se bahar → GC
```

---

## Visualization — Reachable vs Unreachable

```
              Garbage Collection — Reachable vs Unreachable

╔════════════════════════════════════════════════════════════╗
║ Reachable Objects — GC Safe                                ║
╚════════════════════════════════════════════════════════════╝

  GC Roots (stack, static refs, etc.)
       │
       ▼
  ┌─────────┐
  │   s     │  (Stack reference, alive)
  └────┬────┘
       │
       ▼
  ┌─────────────┐
  │  Student    │   ← reachable, GC ne touch nahi karega
  │ "Arpan"     │
  └─────────────┘


╔════════════════════════════════════════════════════════════╗
║ Case 1 — Reference null kar diya                           ║
╚════════════════════════════════════════════════════════════╝

  Student s = new Student("Arpan");
  s = null;                              ← reference disconnect

  GC Roots
       │
       ▼
  ┌─────────┐                  ┌─────────────┐
  │  s=null │     ──── ──── │  Student    │  ← orphan
  └─────────┘                  │ "Arpan"     │   GC eligible
                               └─────────────┘   ↓
                                              GC clean kar dega


╔════════════════════════════════════════════════════════════╗
║ Case 2 — Reference reassign                                ║
╚════════════════════════════════════════════════════════════╝

  Student s = new Student("Arpan");      ┌─────────────┐
  s = new Student("Rahul");               │ "Arpan"     │  ← orphan, GC
                                          └─────────────┘
  s ─────────────────────►┌─────────────┐
                          │ "Rahul"     │  ← reachable, alive
                          └─────────────┘


╔════════════════════════════════════════════════════════════╗
║ Case 3 — Method khatam = local var scope se bahar          ║
╚════════════════════════════════════════════════════════════╝

  void method() {
      Student s = new Student("Arpan");
  }   // ← method end, s ka scope khatam

  Stack frame removed → GC Roots se disconnect → orphan → GC clean


╔════════════════════════════════════════════════════════════╗
║ System.gc() — Sirf Hint                                    ║
╚════════════════════════════════════════════════════════════╝

  System.gc()  ─── (request) ───► JVM
                                   │
                                   ▼
                                "Theek hai, kabhi karunga..."
                                (no guarantee, JVM decide karta)
```

---

## MARK AND SWEEP — GC garbage kaise dhoondhta (algorithm)

```
   STACK = references/locals | HEAP = saare objects (new se bane)

   2 step:
   1. MARK  -> GC ROOTS (stack locals, static vars) se shuru -> jitne object tak pahunchа ja sake
              sabko MARK (reachable = zinda). roots -> A -> B -> C ... sab mark.
   2. SWEEP -> jo MARK nahi hue (kisi root se pahunch nahi) = garbage -> memory FREE.

   reachable = zinda | unreachable = garbage.
   (circular reference bhi handle: A<->B ek doosre ko point karein par root se koi na pahunche -> dono garbage.
    isi liye Java reference-COUNTING nahi, reachability use karta.)
```

---

## GENERATIONAL GC (young/old) — JP yahan deep jaata

```
   NEEV: "zyadatar object JALDI marte" (method ke temp object -> method khatam -> turant garbage).
         thode object LAMBa chalte (config/cache/connection).

   HEAP age-wise baanta:
   ┌──────────────────────┬──────────────────┐
   │  YOUNG (naye object)  │  OLD (bach gaye)  │
   └──────────────────────┴──────────────────┘
   - naya object -> YOUNG. Young ke kai GC bach jaaye -> OLD mein PROMOTE.

   MINOR GC -> sirf YOUNG saaf. CHHOTA+FAST+frequent (90% kachra yahin).
   MAJOR GC -> OLD/poora heap saaf. BADA+SLOW+kam-baar. "STOP-THE-WORLD" (app ek pal ruk jaati).

   KYUN smart: chote Young ko baar-baar scan (jahan zyada kachra) = fast.
               bada Old kam scan (yahan kam kachra) = mehnga kaam kam baar. -> performance behtar.
```

---

## MEMORY LEAK in JAVA (GC hai phir bhi leak kaise? — JP twist)

```
   GC sirf UNREACHABLE uthata. Leak tab: object ki zaroorat NAHI, par reference galti se ZINDA
   -> GC reachable samajh ke saaf NAHI karta -> memory bharti.

   Classic causes:
   1. static collection mein add karte raho, remove nahi (static = GC root -> sab zinda)
   2. cache/Map mein daala, kabhi nikaala nahi (reference zinda)
   3. listener/callback register kiya, unregister nahi
   Asli wajah: "logically zaroorat khatam, par reference physically zinda."

   FIX: kaam khatam -> reference HATAO (remove/null) | cache -> size-limit+LRU eviction ya WeakHashMap | listener -> unregister.
```

---

## TRAP

> **`System.gc()` sirf request hai — guarantee nahi. JVM decide karta hai kab chalega.**
> **Production code mein `System.gc()` kabhi mat likho.**
> **`finalize()` deprecated hai Java 9+ mein.**

---

## POWER PHRASE

> *"Garbage Collector automatically frees memory of objects with no live references — you cannot force GC, `System.gc()` is just a hint to the JVM."*
