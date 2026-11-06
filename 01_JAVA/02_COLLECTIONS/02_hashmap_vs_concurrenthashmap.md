# HashMap vs ConcurrentHashMap

## Tere usercrud se shuru

```java
ConcurrentHashMap<String,String> processed;
processed.putIfAbsent(key, "PROCESSING");   // idempotency
```

Yahan plain HashMap kyun nahi liya? Kyunki ek hi key ki do request ek saath aa sakti hain, aur dono do alag Tomcat thread
pe chalti hain. Dono thread ek hi map ko ek saath chhoo rahe hote hain.

## Tera C1 run (asli output)

Tune do thread chalaye, aur dono ne alag-alag 1000 key daali. To size 2000 aana chahiye tha. Par aaya ye:

```
mila 1656 · 1911 · 1678 · 1768
```

Koi exception nahi aaya, map chup-chaap galat ho gaya. Iska kaaran ye hai ki HashMap bharne pe apni almari dugni karta
hai. Usi beech agar doosra thread entry daal de, to wo entry purani almari me gir jaati hai aur kho jaati hai.

## ConcurrentHashMap kya alag karta hai

ConcurrentHashMap taala poore map pe nahi lagata, sirf us ek khaane (bucket) pe lagata hai jisme likhna hai. Maan lo
thread-1 khaana 5 me likh raha hai aur thread-2 khaana 12 me, to dono ek saath chal jaate hain, koi kisi ka intezaar
nahi karta. Aur agar khaana khaali hai, to taala bhi nahi lagta, CAS se seedha value daal di jaati hai.

Iske ulat Hashtable aur `synchronizedMap` poore map pe ek hi taala lagate hain, isliye ek waqt pe ek hi thread kaam kar
paata hai aur ye dheema hai.

Null key ya null value ConcurrentHashMap me daalo to NPE aata hai. Kyun? Kyunki agar `get` ne null diya, to kai thread
wali duniya me pata hi nahi chalta ki key "hai hi nahi" ya key hai aur uski value "null hai". Isliye null allow hi nahi.

## Asli trap (tera C3)

```java
int h = map.get("hits");  map.put("hits", h + 1);   // 20000 ki jagah 12663
```

Yahan `get` akela safe hai, aur `put` bhi akela safe hai. Par in dono ke beech jo gap hai, wo safe nahi hai. Doosra
thread usi gap me ghus ke apni value likh deta hai, aur ek thread ki ginti doosre ke upar likh di jaati hai. Ilaaj ye hai
ki dono kaam ek hi call me karo:

```java
map.merge("hits", 1, Integer::sum);           // ya compute, ya value hi AtomicInteger
```

PR review me isko pakadne ka tareeka: agar ek hi key pe lagaataar do map-call dikhe, to shak karo. Ek aur baat:
`merge` / `compute` ka lambda bucket ka taala pakad ke chalta hai, isliye usme DB ya network call mat daalo, warna taala
der tak pakda rahega.

## Trade-off

Agar code me ek hi thread hai, to ConcurrentHashMap thoda dheema padta hai, kyunki taale ka kharcha lagta hai. Isliye
method ke andar jo local map hai, wahan HashMap lo. Aur jo map kai thread me baanta hua hai, wahan ConcurrentHashMap lo.

## Tera hands-on

[C1 takkar](demo/concurrenthashmap/C1_Takkar.java) · [C2 concurrent](demo/concurrenthashmap/C2_Concurrent.java) ·
[C3 check-then-act](demo/concurrenthashmap/C3_CheckThenAct.java) · [C4 ilaaj](demo/concurrenthashmap/C4_Ilaaj.java)
