# AtomicInteger / CAS: Taale Ke Bina Race Fix

**Kahani:** file 04 me `count++` ki race ko `synchronized` ke taale se theek kiya tha. Par taala lagane me ek thread kaam karta hai aur baaki line me khade rehte hain. CAS taala lagata hi nahi. Wo bas bolta hai: "maine 5 padha tha. Agar value abhi bhi 5 hai to 6 likh do, warna kuch mat likho aur dobara try karo."

Ye wahi cheez hai jo Google Docs me hoti hai. Tune version 5 dekh ke save kiya, par beech me kisi aur ne version 7 save kar diya, to tera save reject ho jaata hai aur tu dobara karta hai.

```
padha 5 -> socha 6 -> CAS("abhi bhi 5?")
                        haan -> 6 likha, ho gaya
                        nahi (koi 7 kar gaya) -> mat likho -> dobara padho 7 -> 8 ...
```

**Khaas baat:**

1. CAS CPU ki ek hi instruction hai, isliye compare karne aur likhne ke beech koi doosra thread ghus nahi sakta. Taala nahi lagta, isliye counter ke liye ye `synchronized` se tez hai.

2. ABA problem: value 5 se 6 hui aur phir wapas 5 ho gayi, to CAS ko lagta hai ki kuch badla hi nahi. Counter me isse koi farak nahi padta. Par lock-free stack ya object pool me pointer dobara use hota hai, aur wahan ye asli bug ban jaata hai. Iska ilaaj `AtomicStampedReference` hai, jisme value ke saath ek version number bhi check hota hai.

3. Agar bahut saare thread ek hi counter pe likhein, to retry bahut badh jaate hain. Tab `LongAdder` lagao. Wo counter ko kai khaanon me baant deta hai, har thread apne khaane me likhta hai, aur jab total chahiye to sab khaanon ko jod deta hai.

4. Ye teeno ek hi family ke hain, aur sabka niyam ek hai: "match ho to likho, warna retry karo".
   - AtomicInteger memory me rakha counter hai.
   - `ConcurrentHashMap.putIfAbsent` map me ek key ko claim karta hai.
   - `@Version` DB me `UPDATE ... WHERE version = ?` chalata hai.

**Prod wala trap:** AtomicInteger le liya, to kya code safe ho gaya? Nahi.

```java
count.set(count.get() + 1);    // GALAT: get aur set do alag kadam, beech me doosra ghus gaya
count.incrementAndGet();       // SAHI: ek hi atomic kadam
```

Ye wahi "pehle check karo, phir likho" wali galti hai jo usercrud idempotency me dekhi thi. Wahan `containsKey` ke baad `put` likha to 2 order ban gaye (double charge). `putIfAbsent` lagaya to 1 hi order bana.

**Tera project:** usercrud [`IdempotencyController`](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/IdempotencyController.java) me do cheezein hain. `count.incrementAndGet()` CAS wala counter hai. `map.putIfAbsent(key, "PROCESSING")` ConcurrentHashMap me atomic tareeke se key claim karta hai. Isse 20 parallel request same key ke saath aayin, tab bhi 1 hi order bana.

```
Interview: "AtomicInteger uses CAS: read, compute, and write only if the value is still what I read -
otherwise retry. It's a single CPU instruction, lock-free, so it's faster than synchronized
for counters. The trap is get() then set() - that's two steps and not atomic; use
incrementAndGet. Under heavy contention I'd use LongAdder. ABA matters for pointers, not counters."
```


---

## GEHRA HISSA (purani file se, interview me kaam ka)

## ★ ABA PROBLEM: CAS ka chhupa trap (8-Sep)

CAS sirf ye check karta hai ki "value abhi bhi expected (5) ke barabar hai?". Par value 5 se 6 hui aur phir wapas 5 ho sakti hai. CAS ko 5 dikha, to wo maan leta hai ki kuch nahi badla, aur success ho jaata hai. Jabki value badli thi, bas wapas aa gayi.

**Counter pe ye harmless hai,** kyunki 5 to 5 hi hai, koi farak nahi padta. **Asli bug wahan hai jahan reference ya pointer dobara use hota hai aur uski history matter karti hai.**

Drawer wale note ki misaal le. Tune note kiya ki drawer me ₹5 hain. Koi aaya, wo ₹5 utha le gaya aur ek alag ₹5 rakh gaya. Tune check kiya "abhi bhi ₹5? haan", aur maan liya ki kisi ne haath nahi lagaya. Par haath laga tha.

**Asli use-case: lock-free STACK, jahan pointer reuse hota hai.**

Stack me `head -> A -> B -> C` hai. T1 ne A ko pop karne ka plan banaya: head A hai, A ka next B hai, to wo `CAS(head, exp=A, new=B)` karne wala tha, aur tabhi ruk gaya. Is beech T2 ne A pop kiya, B pop kiya, aur A ko wapas push kar diya. Ab A ka next C hai, aur B free ho chuka hai ya reuse ho raha hai. T1 jaagta hai aur CAS karta hai. Head abhi bhi A hai (kyunki wapas aaya), to CAS success ho jaata hai aur head B ban jaata hai. Par B to nikaal ke free kiya ja chuka tha, isliye stack corrupt ho gaya (dangling ya lost node).

```
Stack: head -> A -> B -> C
T1 pop-A plan: head=A, A.next=B -> CAS(head, exp=A, new=B) karne wala tha -> PAUSE.
T2: A pop -> B pop -> A wapas push (ab A.next=C, aur B free/reuse).
T1 wakes: CAS(head, exp=A, new=B) -> "head abhi bhi A? HAAN (wapas aaya)" -> SUCCESS
   -> par head=B set -> B to nikaal/free ho chuka -> STACK CORRUPT (dangling/lost node).
```

Ye dikkat kahan aati hai: lock-free stack ya queue (Treiber stack), object ya memory POOLS (jahan free hua address dobara use hota hai), aur aise structures jahan "same pointer ka matlab same state nahi hota".

**Ilaaj hai `AtomicStampedReference`.** Isme value ke saath ek VERSION stamp bhi hota hai, aur CAS dono ko check karta hai. Maan lo A ka stamp v1 tha aur ab v3 hai. Value wapas A hai, par stamp 1 aur 3 alag hain, to badlaav pakda gaya. Ye waise hi hai jaise ₹5 ke note pe serial number dekh lo.

```
A(v1) -> A(v3): value wapas A, par stamp 1 != 3 -> change PAKAD liya.  (₹5 note pe serial-number)
```

★ Imaandari wali baat, interview me bhi yahi bolna: roz ke Spring ya backend kaam me tu khud raw CAS pointer pe nahi likhta, kyunki ConcurrentHashMap aur queues ye andar hi sambhal lete hain. ABA zyadatar interview ki gehraai ka sawaal hai, ya library ke andar ka, ya unka jo lock-free structure ya pool khud likhte hain. Counter ke level pe ise ignore kar sakte ho, pointer ya pool ke level pe ye critical hai.

### Step-by-step (count = 5 hai, +1 karna hai)

```
   1. padha current          ->  5
   2. nayi value soch        ->  6
   3. CAS: "agar abhi bhi 5 hai -> 6 likh do"
        - haan 5 hai  -> 6 ho gaya, SUCCESS ✓ -> done
        - koi aur thread beech mein 7 kar gaya -> "5 nahi hai" -> likha NAHI -> FAIL
   4. FAIL -> RETRY: dobara padho (7), soch (8), CAS "agar 7 -> 8" ... jab tak success
```

## Taala vs CAS: yahi asli farak hai

`synchronized` me taala lete ho, aur baaki thread block hoke wait karte hain. Ye slow hai aur blocking hai.

CAS me koi taala nahi hai. Try karo, aur agar koi beat kar gaya to dobara try karo. Na koi block hota hai, na koi wait karta hai. Ye fast hai aur lock-free hai.

```
   synchronized -> taala lo, baaki thread BLOCK ho ke wait karein   (slow, blocking)
   CAS          -> taala NAHI. "try karo, koi beat kar gaya -> dobara try"
                   koi block nahi, koi wait nahi                     (FAST, lock-free)
```

## ★ LongAdder: high-contention counter (8-Sep)

AtomicLong ek hi counter hai. Agar 100 log ek hi line me khade hon, to dhakka-mukki hoti hai, yaani CAS baar-baar fail hoke retry karta hai, aur sab slow ho jaata hai.

LongAdder 10 counter (CELLS) khol deta hai. Log alag-alag counter pe jaate hain, to bheed kam hoti hai aur kaam tez hota hai. Jab total chahiye, to sab cells ka SUM le lo.

```
AtomicLong -> ek counter, 100 log line -> dhakka-mukki (CAS retries) -> slow
LongAdder  -> 10 counter (CELLS) khol do -> log alag-alag pe (kam bheed) -> tez
              total chahiye -> sab cells ka SUM.
```

## Kab kya use karein (yaad rakh)

- **FLAG** ho (true/false, jise ek thread likhta hai), to `volatile` lagao, kyunki yahan sirf visibility chahiye.
- **COUNTER** ho (`count++`, numbers), to `AtomicInteger` (CAS) lagao, kyunki wo fast hai, lock-free hai aur atomic hai.
- **MULTI-LINE critical section** ho, to `synchronized` lagao, kyunki wahan poora block ek saath atomic chahiye.
