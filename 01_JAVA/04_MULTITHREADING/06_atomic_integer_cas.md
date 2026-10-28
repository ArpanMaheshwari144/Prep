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
