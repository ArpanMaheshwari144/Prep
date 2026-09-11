# final keyword — 3 Jagah Use Hota Hai

> **V90 — Core Java Extras: Topic 12**

---

## STORY — PI ki Value

→ Tune `PI` ki value store kiya — `3.14159`
→ Koi aur developer ne **accidentally PI = 3** kar diya. **Bug.**
→ Tune **config values** define kiye → koi service ne override kar diya production mein
→ **`final` = lock laga do** → ye nahi badlega
→ **Variable pe, Method pe, Class pe** — teeno jagah same intent: ye change nahi hona chahiye
→ `final` variable → **reassign nahi**
→ `final` method → **override nahi**
→ `final` class → **extend nahi**
→ **Teeno ka ek hi matlab: change nahi hoga**

---

## Code

```java
final int x = 10;
x = 20;                              // compile error — value change nahi hogi

final class String { }
class MyString extends String { }    // compile error — extend nahi ho sakti

final void show() { }
// child override nahi kar sakta
```

---

## TRAP

> **`final` variable ka matlab sirf `reassign nahi` — object ke ANDAR ke fields change ho sakte hain!**

```java
final List<String> list = new ArrayList<>();
list.add("Arpan");                   // allowed — andar change kiya
list = new ArrayList<>();            // reference change nahi hoga
```

**WHY andar badal sakta? `final` = LABEL lock hai, DABBA nahi.**

→ Jaise ghar ka **address final** — ghar nahi badlega, lekin **andar furniture rearrange** kar sakte ho
→ `list = new ArrayList()` = ghar badalna = NAHI
→ `list.add()` = andar furniture = HAAN

---

## Visualization

```
                   final = LABEL lock, DABBA lock NAHI

STACK                        HEAP
┌──────────────┐             ┌─────────────────────────┐
│ list (final) ├────────────►│ ArrayList               │
│ LOCKED    │             │ ["Arpan"]               │  ← content modify OK
└──────────────┘             │ list.add("Rahul")    │
                             │ ["Arpan", "Rahul"]      │
                             └─────────────────────────┘

╔════════════════════════════════════════════════════════════╗
║ list = new ArrayList<>();   →  BLOCKED — reference lock ║
║ list.add("Rahul");          →  ALLOWED — content change ║
╚════════════════════════════════════════════════════════════╝

Ghar ka address final = ghar nahi badlega (reference)
                       Andar furniture rearrange ho sakta (content)
```

---

## ★ FINAL-FIELD SAFE PUBLICATION (deep, grill — 9-Sep)

> `final` ka ek CHHUPA JMM guarantee: multi-thread me object safely publish hota. [[05_volatile]] (JMM/happens-before), [[06_immutable_class]]

### Problem — "aadhe-bane object ka leak" (unsafe publication)
Object bante waqt fields set ho rahe; doosra thread us object ka REFERENCE dekhta.
JVM/CPU instructions REORDER kar sakta -> **reference PEHLE publish ho jaaye, fields likhne se pehle** ->
doosra thread reference paa ke andar dekhe -> fields abhi set hi nahi -> default (0/null) -> **aadha-bana object. BUG.**

**House analogy:**
```
object banana = ghar · fields = furniture · reference = ADDRESS.
UNSAFE: reordering se ADDRESS de diya furniture lagne se PEHLE -> koi andar jaaye -> khaali ghar. BUG.
```

### Fix — `final` fields ka FREEZE
Agar fields `final` hain -> JMM constructor ke END pe ek **FREEZE** lagata:
```
"jab tak SAARE final-furniture lag ke lock na ho, ADDRESS share nahi hoga."
-> jise bhi reference mile, wo GUARANTEED poora-bana (final-fields sahi) object dekhega. bina lock/volatile.
```
```java
// UNSAFE — field final NAHI
class Config { int port; Config(){ port=8080; } }
//  Thread B: shared.port  -> KABHI 0 dikh sakta (reordering)

// SAFE — field final
class Config { final int port; Config(){ port=8080; } }
//  Thread B: shared.port  -> HAMESHA 8080 (constructor-end freeze)
```
★ SHART: constructor ke andar `this` **escape na ho** (reference complete hone se pehle leak na kare), warna guarantee toot jaati.

### Isiliye IMMUTABLE = thread-safe
`String`/`Integer`/koi final-fields immutable class -> **bina synchronization multi-thread me share** kar sakte, kyunki safe-publication unhe theek deta. Yehi immutable ko thread-safe banata.

### ★ "Sab kuch immutable kyun nahi bana dete?" (Arpan-Q, honest)
Instinct sahi ("prefer immutable" = Java best-practice), par **sab immutable possible NAHI:**
```
1. Kuch state BADALNI HI padti (counter/balance/cache/cart) -> immutable = kuch-karta-hi-nahi.
2. Immutable "change" = HAR baar NAYA object (String jaisa) -> baar-baar change = mehnga (garbage). isliye StringBuilder.
3. bade object har-change-pe-copy = waste.
```
```
badalta-NAHI  -> immutable banao -> thread-safe MUFT (pehli pasand)
badalta-HAI (counter/balance/cache) -> AtomicInteger / ConcurrentHashMap / synchronized-lock
```

### ★ LIVE DEMO (khud chalaya, 9-Sep — [`04_MULTITHREADING/practical/SafePubDemo.java`](../../04_MULTITHREADING/practical/SafePubDemo.java))
```
Demo1 mutable Counter (plain int c++), 2 thread × 100000 inc  -> 192415  (200000 se KAM = lost updates, RACE dikha)
Demo2 AtomicInteger, wahi 2×100000                            -> 200000  exactly (atomic = safe)
Demo3 effectively-final: lambda ne x=5 capture kiya -> chala; `x=6` uncomment -> COMPILE ERROR
```
Sabak: mutable-shared akela safe nahi (192415 race live) -> Atomic/lock chahiye · final-safe-pub = iska ULTA side (immutable bana do to race aata hi nahi, muft safe).

## ★ EFFECTIVELY-FINAL (alag, par `final` se juda)
```
effectively-final = variable jise init ke baad DOBARA assign nahi kiya (bhale `final` keyword na ho).
Java 8+: lambda / anonymous-class sirf final YA effectively-final variables CAPTURE kar sakte.
   int x=5; Runnable r=()->print(x);   // x effectively-final -> OK
   x=6;                                 // ab effectively-final NAHI -> lambda me use = COMPILE ERROR
Kyun: lambda variable ki COPY pakadta; badalne do to "kaunsi value?" confusion -> Java freeze rakhta.
```

---

## POWER PHRASE

> *"`final` on a variable prevents reassignment. `final` on a class prevents inheritance. `final` on a method prevents overriding — String class is final which is part of why it is immutable."*

> *"`final` fields also give a JMM safe-publication guarantee: at the end of the constructor there's a freeze, so any thread that sees the object's reference is guaranteed to see the fully-initialized final fields — without synchronization. That's what makes immutable objects safe to share across threads. It only holds if `this` doesn't escape during construction. Mutable state that must change (counters, caches) can't be immutable, so it needs Atomic/concurrent/locks instead."*
