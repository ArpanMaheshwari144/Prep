# AtomicInteger / CAS — Taale Ke Bina Race Fix

**Kahani:** file 04 me `count++` ki race ko `synchronized` ke taale se theek kiya tha. Par taala lagane me ek thread kaam karta hai aur baaki line me khade rehte hain. CAS taala lagata hi nahi. Wo bas bolta hai: "maine 5 padha tha; agar value abhi bhi 5 hai to 6 likh do, warna kuch mat likho aur dobara try karo." Ye wahi cheez hai jo Google Docs me hoti hai: version 5 dekh ke save kiya, beech me kisi aur ne version 7 save kar diya, to tera save reject hota hai aur tu dobara karta hai.

```
padha 5 -> socha 6 -> CAS("abhi bhi 5?")
                        haan -> 6 likha, ho gaya
                        nahi (koi 7 kar gaya) -> mat likho -> dobara padho 7 -> 8 ...
```

**Khaas baat:**
1. CAS CPU ki ek hi instruction hai, to compare aur likhne ke beech koi doosra thread ghus nahi sakta. Taala nahi lagta, isliye counter ke liye ye `synchronized` se tez hai.
2. ABA problem: value 5 se 6 hui aur phir wapas 5 ho gayi, to CAS ko lagta hai kuch badla hi nahi. Counter me isse koi farak nahi padta. Lock-free stack ya object pool me pointer dobara use hota hai, wahan ye asli bug ban jaata hai. Ilaaj `AtomicStampedReference` hai: value ke saath ek version number bhi check hota hai.
3. Bahut saare thread ek hi counter pe likhein to retry bahut badh jaate hain. Tab `LongAdder` lagao: wo counter ko kai khaanon me baant deta hai, har thread apne khaane me likhta hai, aur total chahiye to sab khaanon ka jod.
4. Ye teeno ek hi family ke hain, sabka niyam "match ho to likho, warna retry":
   - AtomicInteger: memory me rakha counter.
   - `ConcurrentHashMap.putIfAbsent`: map me ek key claim karna.
   - `@Version`: DB me `UPDATE ... WHERE version = ?`.

**Prod wala trap:** AtomicInteger le liya, matlab safe ho gaya? Nahi.
```java
count.set(count.get() + 1);    // GALAT: get aur set do alag kadam, beech me doosra ghus gaya
count.incrementAndGet();       // SAHI: ek hi atomic kadam
```
Ye wahi "pehle check, phir likho" wali galti hai jo usercrud idempotency me dekhi thi: `containsKey` phir `put` likha to 2 order ban gaye (double charge), `putIfAbsent` lagaya to 1 hi order bana.

**Tera project:** usercrud [`IdempotencyController`](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/IdempotencyController.java): `count.incrementAndGet()` (CAS counter) + `map.putIfAbsent(key, "PROCESSING")` (ConcurrentHashMap atomic claim). 20 parallel same-key request pe 1 hi order.

```
Interview: "AtomicInteger uses CAS: read, compute, and write only if the value is still what I read -
otherwise retry. It's a single CPU instruction, lock-free, so it's faster than synchronized
for counters. The trap is get() then set() - that's two steps and not atomic; use
incrementAndGet. Under heavy contention I'd use LongAdder. ABA matters for pointers, not counters."
```


---

## GEHRA HISSA (purani file se, interview me kaam ka)

## ★ ABA PROBLEM — CAS ka chhupa trap (8-Sep)
> CAS check karta "value == expected(5)?". Par value 5 -> 6 -> WAPAS 5 ho sakti. CAS ko 5 dikha -> "kuch nahi badla" -> success. Par badla tha aur wapas aa gaya.

**Counter pe HARMLESS** (5 = 5, farak nahi). **Reference/pointer jo REUSE ho + history matter kare** wahan asli bug.

Drawer-note analogy: tune "₹5 hai" note kiya -> koi ₹5 utha ke ALAG ₹5 rakh gaya -> tu "abhi bhi ₹5? haan" -> maan liya haath nahi laga. Par laga tha.

**ASLI USE-CASE — lock-free STACK (pointer reuse):**
```
Stack: head -> A -> B -> C
T1 pop-A plan: head=A, A.next=B -> CAS(head, exp=A, new=B) karne wala tha -> PAUSE.
T2: A pop -> B pop -> A wapas push (ab A.next=C, aur B free/reuse).
T1 wakes: CAS(head, exp=A, new=B) -> "head abhi bhi A? HAAN (wapas aaya)" -> SUCCESS
   -> par head=B set -> B to nikaal/free ho chuka -> STACK CORRUPT (dangling/lost node).
```
Aata kahan: **lock-free stack/queue (Treiber), object/memory POOLS (freed address reuse), "same pointer != same state"** structures.

**FIX = AtomicStampedReference** = value + VERSION-stamp; dono check.
```
A(v1) -> A(v3): value wapas A, par stamp 1 != 3 -> change PAKAD liya.  (₹5 note pe serial-number)
```
★ HONEST (interview me bhi): day-to-day Spring/backend me tu raw CAS-on-pointer khud NAHI likhta (ConcurrentHashMap/queues andar handle karte). ABA = mostly interview-depth / library-internals + lock-free-structure/pool likhne walon ka. Counter-level ignore, pointer/pool-level critical.

### Step-by-step (count = 5, +1 karna)
```
   1. padha current          ->  5
   2. nayi value soch        ->  6
   3. CAS: "agar abhi bhi 5 hai -> 6 likh do"
        - haan 5 hai  -> 6 ho gaya, SUCCESS ✓ -> done
        - koi aur thread beech mein 7 kar gaya -> "5 nahi hai" -> likha NAHI -> FAIL
   4. FAIL -> RETRY: dobara padho (7), soch (8), CAS "agar 7 -> 8" ... jab tak success
```

## Taala vs CAS — yehi asli farak
```
   synchronized -> taala lo, baaki thread BLOCK ho ke wait karein   (slow, blocking)
   CAS          -> taala NAHI. "try karo, koi beat kar gaya -> dobara try"
                   koi block nahi, koi wait nahi                     (FAST, lock-free)
```

## ★ LongAdder — high-contention counter (8-Sep)
```
AtomicLong -> ek counter, 100 log line -> dhakka-mukki (CAS retries) -> slow
LongAdder  -> 10 counter (CELLS) khol do -> log alag-alag pe (kam bheed) -> tez
              total chahiye -> sab cells ka SUM.
```

## Kab Kya Use Karein (yaad rakh)
| Situation | Tool | Kyun |
|-----------|------|------|
| **FLAG** (true/false, ek thread likhta) | `volatile` | sirf visibility chahiye |
| **COUNTER** (`count++`, numbers) | `AtomicInteger` (CAS) | fast, lock-free, atomic |
| **MULTI-LINE critical section** | `synchronized` | poora block atomic chahiye |
