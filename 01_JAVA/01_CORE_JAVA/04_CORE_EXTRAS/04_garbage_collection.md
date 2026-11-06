# Garbage Collection

## Scene

Mart ke counter pe har bill ke liye ek parchi object banta hai. Din bhar me 30 lakh parchiyan banti hain. C++ hota to har parchi ko `delete` khud karna padta, aur bhool jaate to memory leak hota. Java me ye kaam JVM ka safai wala (GC) karta hai, aur uska bas ek sawaal hota hai: "is object tak koi pahunch sakta hai?"

```
GC ROOTS (stack ke variable, static field)
   |
   v
 [ parchi #5 ]          abhi counter pe hai, teer pahunchta = zinda (MARK)
   [ parchi #1..4 ]     bill ho gaya, koi teer nahi = kachra (SWEEP)
```

Chala ke dekha (heap 32 MB, 30 lakh parchi):

```
GC(6) Pause Young ... 11M->1M(19M) 0.802ms
GC(7) Pause Young ... 11M->1M(19M) 0.690ms
GC(9) Pause Young ... 15M->1M(25M) 0.575ms
bill bane: 3000000
```

## Dikkat -> hal

Zyadatar object jaldi mar jaate hain, jaise ye parchiyan. Isliye heap umar ke hisaab se do hisson me bata hota hai. Naye object Young me bante hain, aur jo kai GC ke baad bhi bach jaate hain wo Old me chale jaate hain. Upar har baar `11M->1M` hua, matlab har GC me 10 MB kachra Young me hi saaf ho gaya, aur pause 1 ms se kam raha. Major GC ek baar bhi nahi hua.

GC ginti nahi karta, sirf pahunch dekhta hai, isliye do object ek-doosre ko pakde baithe hon par root se koi raasta na ho, to dono saaf ho jaate hain. Collector chunna ek trade-off hai: kitna kaam nikla (throughput) vs app kitni der ruki (pause). Java 9 se G1 default hai, jo upar log me dikh raha hai.

## Prod wala trap

GC hone ke baad bhi Java me leak hota hai.

```java
static Map<Integer, byte[]> purane = new HashMap<>();
purane.put(i, new byte[100_000]);   // har bill daala, kabhi remove nahi kiya
```

Chala ke dekha:

```
rakhe: 250
rakhe: 300
java.lang.OutOfMemoryError
```

`static` field ek GC root hai, isliye uske andar ka sab kuch hamesha zinda maana jaata hai aur GC kuch utha hi nahi paata. Ilaaj ye hai ki kaam khatam hote hi entry remove karo, ya cache me size ki had rakho aur LRU lagao, taaki purani entry apne aap nikal jaaye. Ye wahi LRU + TTL wala idea hai jo distributed cache me padha tha.

## Tera kaam

Prod ke heap graph me aari (sawtooth) jaisa pattern normal hai: memory badhti hai aur GC aate hi girti hai. Dikkat tab hai jab har GC ke baad ka neeche wala point bhi dheere-dheere upar chadhta jaaye. Yahi leak ki pehchaan hai.
