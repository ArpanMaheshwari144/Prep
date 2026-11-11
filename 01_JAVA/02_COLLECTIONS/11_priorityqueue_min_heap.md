# PriorityQueue: min-heap

## Scene: tere DSA se (top k frequent, kth largest, Dijkstra = C++ priority_queue)

```
C++  priority_queue<int>      -> default MAX-heap (bada pehle)
Java PriorityQueue<Integer>   -> default MIN-heap (chhota pehle)   <- ulta!
Java me max-heap chahiye:  new PriorityQueue<>(Comparator.reverseOrder())
C++ top / pop / push  =  Java peek / poll / add
```

```
Java default peek = 5   reverseOrder peek = 30
```

Daala 30, 5, 20, 10, 15. Java ne default me 5 (sabse chhota) diya. C++ ki aadat se Java likha to "sabse bada" ki jagah
"sabse chhota" niklega.

## Tere DSA me min-heap kahan tha, kahan nahi

- **Dijkstra = min-heap.** Har baar wo node chahiye jiski doori abhi SABSE KAM hai (Maps jaisa: jo jagah sabse paas,
  pehle use pakka karo, phir wahan se aage). Isliye top pe sabse chhoti doori. Tera C++ (sheet 13 graphs):

```cpp
priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;   // {dist,node}
```

  C++ me default max-heap hai, isliye `greater<...>` likh ke min-heap banana pada. Java me default hi min-heap hai,
  bas batao ki doori pe compare kare:

```java
PriorityQueue<int[]> pq = new PriorityQueue<>((a, b) -> Integer.compare(a[0], b[0]));   // {dist, node}
```

  Yahan bhi `a[0] - b[0]` nahi, `Integer.compare` (file 08 wala trap: badi doori pe minus wala tareeka had paar).

- **Simple topo sort (Course Schedule) = sirf QUEUE, heap nahi.** Koi bhi sahi kram chalega, to jo pehle ready hua wo
  pehle.
- **Topo ka bhai (modules: "ready me se sabse chhota number pehle") = min-heap.** Wahan khaas kram maanga hai, isliye
  queue -> min-heap, `front()` -> `top()`, baaki sab same. Sheet 13 me likha: "koi bhi valid order -> queue · sabse
  chhota / lexicographic -> MIN-HEAP".

## Kis dikkat se bana: 1 lakh me se top 5

Poora sort = O(n log n), jabki chahiye sirf 5. Isliye 5 size ki min-heap rakho: har naya number daalo, size 5 se upar
jaaye to sabse chhota nikaal do. Aakhir me 5 sabse bade bachte hain, kaam O(n log k). Tera kth largest bhi yahi.

```
1 lakh me top 5, println(pq)     : [999960, 999964, 999971, 999988, 999994]
new ArrayList<>(pq)              : [999960, 999964, 999971, 999988, 999994]
poll() baar-baar                 : [999960, 999964, 999971, 999988, 999994]
```

Teeno sorted dikhe, par sirf `poll()` wala pakka sorted hai. Upar ke do sanyog se aaye (neeche prod trap).

## poll() sorted kyun deta hai (C++ ke top + pop jaisa)

Heap ka ek hi niyam: har parent apne bachchon se chhota. Isliye root hamesha sabse chhota. Baaki elements ke kram ki koi
guarantee nahi.

`poll()`: root nikaalo, aakhri element ko root pe rakho, use neeche khiskao jab tak niyam wapas na aaye. Ab root pe bache
hue me sabse chhota. Har `poll()` pe sabse chhota nikalta hai, isliye baar-baar poll = sorted kram.

## Poora sorted kyun nahi rakhte

Sorted array rakhte to har naye element pe baaki sab khiskane padte, O(n). Heap me daalna O(log n), kyunki bas ek raasta
upar chadhna hai, 1 lakh me lagbhag 17 kadam. Chahiye sirf "abhi sabse chhota kaun", isliye poora kram faltu mehnat.

Ghar ke ped jaisa: max-heap (C++) me har baap apne bachchon se bada, to dada sabse upar turant dikhte hain. Java ke
min-heap me ulta, har baap bachchon se chhota. Dono me do chacha me kaun bada, iska koi niyam nahi.

## Prod trap: poll() ke bina dekha to sorted NAHI

```
println(min-heap 30,5,20,10,15)  : [5, 10, 20, 30, 15]
```

`poll()` ke bina poori queue dekhi to andar ka array jaisa hai waisa dikhta hai: 30 ke baad 15. Ye in sab pe hota hai:
`System.out.println(pq)`, `for (x : pq)`, `new ArrayList<>(pq)`, `stream()`, `toArray()`.

```java
return new ArrayList<>(top5);   // "top 5" API -> screen pe ulta-seedha kram
```

Upar wale 1 lakh ke run me `println(pq)` aur `new ArrayList<>(pq)` sorted aa gaye the, par wo SANYOG tha (HashMap wale
sorted int jaisa, file 06). Test me sorted dikha, prod me bikhra.

Sorted chahiye to ek-ek `poll()` karo, ya list bana ke sort karo. C++ ka `priority_queue` loop chalane hi nahi deta, sirf
top aur pop, isliye wahan ye galti kabhi dikhi nahi. Java dekhne deta hai, wahi trap.
