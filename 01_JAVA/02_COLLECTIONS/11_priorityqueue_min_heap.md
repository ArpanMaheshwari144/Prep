# PriorityQueue: min-heap

## Tere DSA se (top k frequent, kth largest, Dijkstra = C++ priority_queue)

```
C++  priority_queue<int>      -> default MAX-heap (bada pehle)
Java PriorityQueue<Integer>   -> default MIN-heap (chhota pehle)   <- ulta!
Java max-heap: new PriorityQueue<>(Comparator.reverseOrder())
C++ top/pop/push  =  Java peek/poll/add
```

C++ ki aadat se Java likha to "sabse bada" ki jagah "sabse chhota" niklega.

## Kis dikkat se bana: top 5 of 1 lakh

Poora sort O(n log n), chahiye sirf 5. 5 ki min-heap: naya daalo, size 5 se upar to sabse chhota nikaalo. Aakhir me 5
sabse bade, O(n log k). Wahi tera kth largest.

## poll() sorted kyun deta (C++ top + pop jaisa)

Heap ka ek niyam: har parent apne bachchon se chhota -> root hamesha sabse chhota, baaki kram ki guarantee nahi.
`poll()`: root nikaalo -> aakhri ko upar rakho -> neeche khiskao jab tak niyam wapas -> phir upar bache hue me
sabse chhota. Har baar sabse chhota = baar-baar poll = sorted.

## Poora sorted kyun nahi rakhte

Sorted array me har naye pe sab khiskao = O(n). Heap me daalna O(log n) (ek raasta upar, 1 lakh me ~17 kadam).
Chahiye sirf "abhi sabse chhota kaun", poora kram faltu mehnat.
Ghar ka ped: har baap bachchon se bada -> dada sabse upar turant dikhte; do chacha me kaun bada, koi niyam nahi.

## Sorted kab NAHI aata (prod trap)

`poll()` ke bina poori queue dekho to andar ka array jaisa hai waisa:
`System.out.println(pq)` · `for (x : pq)` · `new ArrayList<>(pq)` · `stream()` · `toArray()` -> `[5, 10, 20, 30, 15]`.

```java
return new ArrayList<>(top5);   // "top 5" API -> screen pe ulta-seedha kram
```

Sorted chahiye = `poll()` ek-ek, ya list sort karo. C++ ka `priority_queue` loop chalane hi nahi deta (sirf top/pop),
isliye wahan ye kabhi dikha nahi; Java dekhne deta hai, wahi trap.

## Interview line

```
PriorityQueue in Java is a min-heap by default, unlike C++'s max-heap, so for top-K I keep a min-heap
of size K and drop the smallest, which is O(n log k); iterating the queue is not sorted, only
repeated poll() gives sorted order.
```
