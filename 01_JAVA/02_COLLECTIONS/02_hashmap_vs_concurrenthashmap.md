# HashMap vs ConcurrentHashMap

## Scene: tere usercrud ka idempotency map

```java
ConcurrentHashMap<String,String> processed;
processed.putIfAbsent(key, "PROCESSING");   // idempotency
```

Ek hi key ki do request ek saath aa sakti hain, aur dono do alag Tomcat thread pe chalti hain. Dono thread ek hi map ko
ek saath chhoo rahe hote hain. Isliye yahan plain HashMap nahi liya.

## Tera C1: plain HashMap, do thread, 1000-1000 alag key

```
expected 2000, mila 1953        (har run me alag: 1656, 1911, 1678, 1768 ...)
```

Koi exception nahi aaya, map chup-chaap galat ho gaya. HashMap bharne pe apni almari dugni karta hai. Usi beech doosra
thread entry daal de, to wo purani almari me gir ke kho jaati hai.

## ConcurrentHashMap kya alag karta hai

- Taala poore map pe nahi lagta, sirf us ek khaane (bucket) pe jisme likhna hai. Thread-1 khaana 5 me aur thread-2
  khaana 12 me likhe, to dono saath chal jaate hain.
- Khaana khaali ho to taala bhi nahi lagta, CAS se seedha value daal di jaati hai.
- Hashtable aur `synchronizedMap` poore map pe ek hi taala lagate hain, isliye ek waqt pe ek hi thread chalta hai, dheema.
- Null key ya null value daalo to NPE aata hai. Kyunki kai thread me `get` ka null dikhe to pata hi nahi chalta ki key
  hai hi nahi, ya key hai aur uski value null hai.

## Asli trap: tera C3 (map ConcurrentHashMap hi hai)

```java
int h = map.get("hits");  map.put("hits", h + 1);
```

```
expected 20000, mila 12915
```

`get` akela safe hai aur `put` akela safe hai, par dono ke beech ka gap safe nahi. Doosra thread usi gap me ghus ke likh
deta hai, aur ek thread ki ginti doosre ke upar likh di jaati hai.

## Hal: tera C4, dono kaam ek hi call me

```java
map.merge("hits", 1, Integer::sum);     // ya compute, ya value hi AtomicInteger
```

```
merge         = 20000
compute       = 20000
AtomicInteger = 20000
```

PR review me pakadne ka tareeka: ek hi key pe lagaataar do map-call dikhe, to shak karo.
- `merge` / `compute` ka lambda khaane ka taala pakad ke chalta hai.
- Isliye usme DB ya network call mat daalo, warna taala der tak pakda rahega.

## Trade-off

Ek hi thread wale code me ConcurrentHashMap thoda dheema hai, kyunki taale ka kharcha lagta hai.
- Method ke andar jo local map ho, wahan HashMap.
- Jo map kai thread me baanta ho, wahan ConcurrentHashMap.

## Tera hands-on

[C1 takkar](demo/concurrenthashmap/C1_Takkar.java) · [C2 concurrent](demo/concurrenthashmap/C2_Concurrent.java) ·
[C3 check-then-act](demo/concurrenthashmap/C3_CheckThenAct.java) · [C4 ilaaj](demo/concurrenthashmap/C4_Ilaaj.java)
