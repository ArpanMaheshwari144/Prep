# AtomicInteger / CAS: Taale Ke Bina Race Fix

## Scene: parchi pe taala nahi, "abhi bhi 500 hai?"

File 04 me parchi pe taala lagaya tha: ek cashier likhta, baaki 9 line me intezaar karte. CAS taala lagata hi nahi. Har
cashier bas itna kehta hai (parchi 10000 se kat-kat ke neeche aa rahi, maan lo is pal 500 hai): "maine parchi pe 500 padha tha. Agar abhi bhi 500 likha hai to 499 likh do, warna kuch mat likho,
main dobara padhunga." Ye wahi cheez hai jo pehle dekh chuke: Google Docs ka version (tune 5 pe save kiya, beech me koi 7 kar
gaya, to tera save reject), banking ki `WHERE balance >= 500`, aur e-commerce ka Redis DECR.

Chala ke dekha (10 cashier, har ek 1000 baar, shuru 10000):

```
get() phir set():     bacha ghee: 1944   <- AtomicInteger liya, phir bhi 1944 kho gaye
decrementAndGet():    bacha ghee: 0
compareAndSet loop:   bacha ghee: 0
   us loop me kitni baar 'koi aur pehle likh gaya, dobara': 9311
```

Har baar chalane pe ye number alag aate hain (12-Nov dobara chalaya to 950 aur 7654), par pehli line kabhi 0 pakki nahi.

## Khaas baat

CAS CPU ki ek hi instruction hai, isliye "abhi bhi 500?" aur "499 likho" ke beech koi doosra thread ghus nahi sakta. Taala
nahi lagta, isliye counter ke liye ye `synchronized` se tez hai.

Upar aakhri line dekh: 9311 baar retry hua. Bahut saare thread ek hi counter pe toot padein to retry badh jaate hain. Tab
`LongAdder` lagao, jo ginti ko kai khaanon me baant deta hai, aur total chahiye tab sab jod deta hai. Ye e-commerce ke 10
counter jaisa hi hai.

ABA problem: value 5 se 6 hui aur phir wapas 5 ho gayi, to CAS ko lagta hai kuch badla hi nahi. Counter me isse koi farak
nahi padta. Par lock-free stack jaise structure me, jahan pointer dobara use hota hai, ye asli bug ban jaata hai. Iska ilaaj
`AtomicStampedReference` hai, jisme value ke saath ek version bhi check hota hai.

Ye sab ek hi parivar ke hain, aur sabka niyam ek hai: "match ho to likho, warna retry". AtomicInteger memory me,
`ConcurrentHashMap.putIfAbsent` map me, aur `@Version` DB me (`UPDATE ... WHERE version = ?`).

## Prod wala trap

Upar ki pehli line hi trap hai. AtomicInteger le liya to log maan lete hain ki code safe ho gaya. Par `g.set(g.get() - 1)`
do alag kaam hain (padho, phir likho), aur beech me koi aur ghus gaya, isliye 1944 dabbe kho gaye. Ek hi atomic kaam chahiye.

```java
count.set(count.get() + 1);    // GALAT: get aur set do alag kadam
count.incrementAndGet();       // SAHI: ek hi atomic kadam
```

Ye wahi "pehle check karo, phir likho" wali galti hai jo usercrud idempotency me dekhi thi: `containsKey` ke baad `put` likha
to 2 order bane (double charge), `putIfAbsent` lagaya to 1 hi order bana.

## Tera project

Usercrud [`IdempotencyController`](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/IdempotencyController.java)
me dono hain: `count.incrementAndGet()` CAS wala counter, aur `map.putIfAbsent(key, "PROCESSING")` jo ConcurrentHashMap me
key ko atomic tareeke se claim karta hai. 20 parallel request same key ke saath aayi, phir bhi order ek hi bana.
