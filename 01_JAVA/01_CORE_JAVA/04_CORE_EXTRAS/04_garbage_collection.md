# Garbage Collection

## Kahani

C++ me `new` se object banao to `delete` khud karna padta hai. Bhool gaye to memory leak. Java me ye kaam JVM ka safai wala (GC) karta hai. Uska ek hi sawaal hota hai: **"is object tak koi pahunch sakta hai?"**

```java
Patient p = new Patient("Arpan");
p = new Patient("Ravi");   // "Arpan" tak ab koi nahi pahunchta -> kachra
```

## Tasveer

Kaun zinda hai, kaun kachra:

```
GC ROOTS (stack ke variable, static field)
   |
   v
 [ p ] --> [ Ravi ]           pahunch hai = zinda (MARK)

           [ Arpan ]          koi teer nahi = kachra (SWEEP)
```

Heap ke do hisse:

```
[ YOUNG: Eden -> S0 <-> S1 ]  -->  [ OLD ]
  naye object, jaldi marte          kai baar bach gaye to yahan aate
  minor GC: chhota, tez, baar-baar  major GC: bada, dheema, kam baar
```

## 4 khaas baat

1. **GC ginti nahi karta, pahunch dekhta hai** (mark and sweep). Roots se chal ke jahan tak pahuncha, wo zinda hai. Baaki sab kachra hai. Isliye do object ek-doosre ko pakde baithe hon, par root se unka koi raasta na ho, to dono saaf ho jaate hain.
2. **Zyadatar object jaldi mar jaate hain, isliye heap umar ke hisaab se bata hota hai.** Chala ke dekha tha: 30 lakh chhote object banaye, `18M->1M`, pause 1ms se kam, 8 minor GC hue aur major ek bhi nahi. Saara kachra Young me hi mar gaya.
3. **GC hone ke baad bhi Java me leak hota hai.** Object ki zaroorat khatam ho gayi par reference zinda reh gaya, to GC use zinda hi maanega. Isi ka trap neeche hai.
4. **Collector ka faisla do cheezon ke beech hota hai: kitna kaam nikla (throughput) aur app kitni der ruki (pause).** Java 8 tak Parallel GC default tha: throughput zyada, par pause dikhta tha. Java 9 se G1 default hai: heap ko regions me baant ke sabse gande region ko pehle saaf karta hai, aur pause target diya ja sakta hai. ZGC bahut bade heap pe bhi pause 10ms se kam rakhta hai.

## Prod wala trap

```java
static Map<String, Session> cache = new HashMap<>();
cache.put(id, session);   // har login pe daala, kabhi remove nahi kiya
```

`static` field ek GC root hai. Uske andar ka sab kuch hamesha zinda rehta hai, isliye GC kuch utha hi nahi paata. Chala ke dekha tha: 31 MB jama hua aur `OutOfMemoryError: Java heap space`. Ilaaj: kaam khatam ho to remove karo, ya cache me size ki had + LRU rakho (wahi distributed cache wala LRU + TTL).

## Tera kaam

Prod me heap graph ka "aari" (sawtooth) pattern normal hai: memory badhti hai aur GC aate hi girti hai. Dikkat tab hai jab har GC ke baad wala neeche ka point bhi dheere-dheere upar chadhta jaaye. Wo leak ki pehchaan hai.

## Interview line

```
GC frees objects that are no longer reachable from GC roots like stack
variables and static fields, using mark and sweep, so circular references
are not a problem. The heap is generational: most objects die young, so
minor GCs on the young generation are frequent and cheap, and survivors are
promoted to the old generation. G1 is the default since Java 9. Java can
still leak memory if references stay alive, for example a static collection
that only grows.
```
