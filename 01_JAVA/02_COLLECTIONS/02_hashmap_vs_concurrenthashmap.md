# HashMap vs ConcurrentHashMap

## Tere usercrud se shuru

```java
ConcurrentHashMap<String,String> processed;
processed.putIfAbsent(key, "PROCESSING");   // idempotency
```

Plain HashMap kyun nahi? Ek hi key ki do request ek saath aati hain, do alag Tomcat thread pe.

## Tera C1 run (asli output)

Do thread, alag-alag 1000 key -> size 2000 aana tha:

```
mila 1656 · 1911 · 1678 · 1768
```

Koi exception nahi, chup-chaap galat. HashMap almari dugni karta hai, us beech doosre thread ki entry purani almari me
gir ke kho jaati.

## ConcurrentHashMap kya alag karta

Taala poore map pe nahi, sirf us ek khaane (bucket) pe. Thread-1 khaana 5 me, thread-2 khaana 12 me = dono saath.
Khaali khaane me taala bhi nahi, CAS se seedha. Hashtable / `synchronizedMap` = poore map pe ek taala = dheema.
Null key / value: ConcurrentHashMap me NPE (`get` pe null aaya to "nahi hai" ya "null hai", kai thread me pata nahi chalta).

## Asli trap (tera C3)

```java
int h = map.get("hits");  map.put("hits", h + 1);   // 20000 ki jagah 12663
```

`get` safe, `put` safe, par beech ka gap safe nahi, doosra thread usi me ghusa. Ilaaj = ek call:

```java
map.merge("hits", 1, Integer::sum);           // ya compute, ya value hi AtomicInteger
```

PR me pakadne ka tareeka: ek hi key pe lagaataar do map-call = shak karo. `merge` / `compute` ka lambda bucket ka taala
pakad ke chalta, usme DB / network call mat daalo.

## Trade-off

Ek thread wale code me ConcurrentHashMap thoda dheema (taale ka kharcha). Method ke andar local map = HashMap; kai
thread me baanta map = ConcurrentHashMap.

## Tera hands-on

[C1 takkar](demo/concurrenthashmap/C1_Takkar.java) · [C2 concurrent](demo/concurrenthashmap/C2_Concurrent.java) ·
[C3 check-then-act](demo/concurrenthashmap/C3_CheckThenAct.java) · [C4 ilaaj](demo/concurrenthashmap/C4_Ilaaj.java)

## Interview line

```
In usercrud's idempotency check two requests with the same key can arrive on two Tomcat threads at
once, so the processed map is a ConcurrentHashMap and the claim is a single putIfAbsent; the map is
thread-safe per call, but a get followed by a put is still a race, which is why I use merge or
compute.
```
