# PriorityQueue: min-heap

## Tere DSA se (top k frequent, kth largest, Dijkstra = C++ priority_queue)

```
C++  priority_queue<int>      -> default MAX-heap (bada pehle)
Java PriorityQueue<Integer>   -> default MIN-heap (chhota pehle)   <- ulta!
Java me max-heap chahiye:  new PriorityQueue<>(Comparator.reverseOrder())
C++ top / pop / push  =  Java peek / poll / add
```

Tu DSA me C++ ka priority_queue use karta hai, jo default me bada pehle deta hai. Java ka PriorityQueue ulta hai, wo
default me chhota pehle deta hai. Isliye agar C++ ki aadat se Java likha, to "sabse bada" ki jagah "sabse chhota"
niklega.

## Kis dikkat se bana: 1 lakh me se top 5

Maan lo 1 lakh numbers me se sabse bade 5 chahiye. Poora sort karoge to O(n log n) lagega, jabki chahiye sirf 5. Isliye
5 size ki min-heap rakho. Har naya number daalo, aur jaise hi size 5 se upar jaaye, sabse chhota nikaal do. Aakhir me
heap me 5 sabse bade bachte hain, aur kaam O(n log k) me ho jaata hai. Tera kth largest bhi yahi hai.

## poll() sorted kyun deta hai (C++ ke top + pop jaisa)

Heap ka ek hi niyam hai: har parent apne bachchon se chhota hota hai. Isliye root hamesha sabse chhota hota hai. Baaki
elements ke kram ki koi guarantee nahi hoti.

`poll()` aise chalta hai: pehle root nikaalta hai, phir aakhri element ko upar root pe rakhta hai, phir use neeche
khiskata hai jab tak niyam wapas na aa jaaye. Ab jo root pe hai, wo bache hue me sabse chhota hai. Matlab har `poll()` pe
sabse chhota hi nikalta hai, isliye baar-baar poll karoge to sorted kram milega.

## Poora sorted kyun nahi rakhte

Agar sorted array rakhte, to har naye element pe baaki sab khiskane padte, jo O(n) hai. Heap me daalna O(log n) hai,
kyunki bas ek raasta upar chadhna hota hai, aur 1 lakh elements me ye lagbhag 17 kadam hai. Hume chahiye sirf ye ki
"abhi sabse chhota kaun hai", isliye poora kram banaye rakhna faltu mehnat hai.

Isko ghar ke ped jaisa samjho: har baap apne bachchon se bada hai, isliye dada sabse upar turant dikh jaate hain. Par do
chacha me kaun bada hai, iska koi niyam nahi hai.

## Sorted kab NAHI aata (prod trap)

Agar tu `poll()` ke bina poori queue dekhega, to andar ka array jaisa hai waisa hi dikhega, sorted nahi. Ye in sab
tareeko pe hota hai: `System.out.println(pq)`, `for (x : pq)`, `new ArrayList<>(pq)`, `stream()` aur `toArray()`. Sab
kuch aisa dikhayenge: `[5, 10, 20, 30, 15]`.

```java
return new ArrayList<>(top5);   // "top 5" API -> screen pe ulta-seedha kram
```

Agar sorted chahiye, to ek-ek karke `poll()` karo, ya list bana ke use sort karo. C++ ka `priority_queue` tujhe us pe loop
chalane hi nahi deta, wahan sirf top aur pop milta hai, isliye wahan ye galti kabhi dikhi hi nahi. Java dekhne deta hai,
aur wahi trap hai.

## Interview line

```
PriorityQueue in Java is a min-heap by default, unlike C++'s max-heap, so for top-K I keep a min-heap
of size K and drop the smallest, which is O(n log k); iterating the queue is not sorted, only
repeated poll() gives sorted order.
```
