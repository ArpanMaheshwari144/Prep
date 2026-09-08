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

## ★ GC COLLECTORS — safai-crew ka TAREEKA (8-Sep, deep-grill)

> Heap = bada hall (kachra). Collector = safai ka TAREEKA. Sabka ek trade-off:
> **throughput (kitna kaam ho) vs pause (app kitni der FREEZE = "stop-the-world").**

```
Serial GC    -> AKELA safai-wala. poora hall BAND karke akela jhaadu.
                chhota app / single-core. pause LAMBA.        (-XX:+UseSerialGC)

Parallel GC  -> KAI safai-wale ek saath, par hall abhi bhi BAND (stop-the-world).
                kai log = tez. THROUGHPUT max, par pause dikhta.
                Java 8 tak DEFAULT.                            (-XX:+UseParallelGC)

CMS          -> app-ke-CHALTE-CHALTE saaf (mostly concurrent), hall kam band.
                pause chhota. PAR fragmentation + Java 14 me HATA diya (dead).

G1 (Garbage  -> hall ko chhote REGIONS me baanta. jis region me SABSE ZYADA
   First)       kachra usko PEHLE saaf ("garbage-first").
                pause-target de sakte: -XX:MaxGCPauseMillis=10. Java 9+ DEFAULT.

ZGC/Shenandoah-> ultra-low pause (<1-10ms), BADE heap (TB) pe bhi. lagbhag poora
                concurrent. latency-critical (trading/low-latency) apps.
```

**Ek line:** purane (Serial/Parallel) = hall BAND karke saaf (pause zyada, throughput accha) · naye (G1/ZGC) = app-ke-chalte + region-wise (pause chhota, predictable). **G1 = aaj ka default sweet-spot.**

---

## ★ REFERENCE TYPES — object ko kitna KASke pakda (8-Sep, deep-grill)

> GC ka behaviour reference ki "grip" pe: **Strong = mutthi kaske · Soft = jeb me · Weak = haath dheela · Phantom = gir-chuka, sirf parchi.**

```
STRONG  -> normal `Student s = new Student()`. MUTTHI me kaske pakda.
           jab tak strong-ref zinda -> GC HAATH NAHI lagata. (99% code)

SOFT    -> "rakho jab tak memory hai; memory KAM padi -> hata do."
           GC tabhi uthata jab MEMORY-PRESSURE ho. -> perfect for CACHE.

WEAK    -> "dheela pakda — agli GC me agar sirf weak-ref bacha -> hata do."
           -> WeakHashMap ki keys (auto-remove jab key kahin aur use na ho).

PHANTOM -> object MAR chuka, bas "mar gaya" ki parchi (cleanup hook).
           post-mortem cleanup. rarely-directly-used.
```

**GC-aggressiveness order:** Strong (kabhi nahi) → Soft (memory-pressure pe) → Weak (agli GC) → Phantom (already gaya).

---

## TRAP

> **`System.gc()` sirf request hai — guarantee nahi. JVM decide karta hai kab chalega.**
> **Production code mein `System.gc()` kabhi mat likho.**
> **`finalize()` deprecated hai Java 9+ mein.**

---

## POWER PHRASE

> *"Garbage Collector automatically frees memory of objects with no live references — you cannot force GC, `System.gc()` is just a hint to the JVM."*
