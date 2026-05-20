# PriorityQueue — Min-Heap by Default

> **V90 — Collections: Topic 33**

---

## STORY — Top 5 Highest Rated

→ Scene: **1000 survey responses** aaye — **top 5 highest-rated** dikhane hain
→ List sort karein? **Zyada slow** — pura sort kyu jab top 5 chahiye?
→ **PriorityQueue** seedha min/max efficiently deta — sorting ki zaroorat nahi

---

## WHY — PriorityQueue?

→ PriorityQueue **andar se min-heap** hai
→ `poll()` **hamesha sabse CHHOTA** element deta
→ Max-heap chahiye → **`Comparator.reverseOrder()`** pass karo
→ Normal Queue **FIFO**, ye **PRIORITY se nikalta**

---

## Code — Min-Heap (Default — Chhota Pehle)

```java
PriorityQueue<Integer> pq = new PriorityQueue<>();
pq.add(30);
pq.add(10);
pq.add(20);

pq.poll();    // 10  ← sabse chhota pehle 
pq.poll();    // 20
pq.poll();    // 30
```

## Max-Heap (Bada Pehle)

```java
PriorityQueue<Integer> maxPQ = new PriorityQueue<>(Comparator.reverseOrder());
maxPQ.add(30);
maxPQ.add(10);
maxPQ.add(20);

maxPQ.poll();    // 30  ← sabse bada pehle 
```

---

## 3 Important Methods

| Method | Kya karta? |
|--------|-----------|
| `pq.add(x)` | Element daalo |
| `pq.poll()` | **Sabse priority wala nikalo + REMOVE** ← most used |
| `pq.peek()` | **Sabse priority wala DEKHO** — remove mat karo |

**C++ Mapping (yaad rakhne ke liye):**
- C++ `top()` = Java `peek()` (sirf dekho)
- C++ `pop()` = Java `poll()` (nikalo + hatao)
- C++ `push()` = Java `add()`

---

## Konovo Real Use Case — Top-K Pattern

**Scenario:** 1 lakh survey responses, top 5 highest-rated chahiye.

```java
// Min-heap of size 5 — chote elements bahar throw karte ja
PriorityQueue<Integer> top5 = new PriorityQueue<>();   // min-heap

for (int rating : ratings) {
    top5.add(rating);
    if (top5.size() > 5) {
        top5.poll();    // chhota nikalo — bade rehte hain
    }
}
// top5 mein hamesha 5 sabse bade ratings
```

**Time:** O(n log k) instead of O(n log n) full sort.

---

## TRAP

> **PriorityQueue iterate karoge toh sorted order mein NAHI dega.**
> **Heap structure hai — sorted nahi.**
> **Sirf `poll()` se sorted milta.**

---

## POWER PHRASE

> *"PriorityQueue is a min-heap by default — `poll()` always returns the smallest element. Pass `Comparator.reverseOrder()` for max-heap. Best for top-K problems — gives O(n log k) instead of full sort O(n log n)."*

> **Yaad rakh:**
> Default = min-heap (chhota pehle)
> Max-heap = `reverseOrder()` pass
> Top-K problems ka best dost
> Iteration sorted nahi — sirf `poll()` deta sorted

---

## Deep — PQ Internally SORTED Nahi Hai (Important Doubt)

PQ andar se ek **binary heap** structure hai — sorted array NAHI. Heap mein **sirf ek rule**:

> **Parent hamesha child se chhota** (min-heap)
> **Siblings ke beech koi rule nahi.**

### Example — Add karo: 30, 10, 20, 5, 15

#### Tree Structure (Min-Heap):
```
              5             ← root (sabse chhota)
            /   \
          10     20
         /  \
        30   15
```

#### Internal Array Storage:
```
Index:    0    1    2    3    4
Array:  [ 5,  10,  20,  30,  15 ]
          ↑
     SORTED NAHI — 30 ke baad 15 hai!
```

> Array **sorted nahi hai**. 15 last mein, 30 beech mein. Bus heap property maintained (parent ≤ children).

### Iteration vs Poll — Output Compare

#### `iterate / print` — Heap Array As-Is (UNSORTED)
```java
PriorityQueue<Integer> pq = new PriorityQueue<>();
pq.add(30); pq.add(10); pq.add(20); pq.add(5); pq.add(15);

System.out.println(pq);
// Output: [5, 10, 20, 30, 15]   ← HEAP ORDER, NOT SORTED!

for (int x : pq) {
    System.out.print(x + " ");
}
// Output: 5 10 20 30 15   ← SAME unsorted order
```

#### `poll()` — Sorted Output (Ek-Ek Karke)
```java
while (!pq.isEmpty()) {
    System.out.print(pq.poll() + " ");
}
// Output: 5 10 15 20 30   ← SORTED 
```

`poll()` har baar:
1. Root nikalta (sabse chhota)
2. Heap **re-balance** karta — next sabse chhota root pe aata
3. Phir poll → next chhota → poll → ... → sorted output

### Time Complexity

| Operation | Time | Reason |
|-----------|------|--------|
| `add()` | O(log n) | Heap re-balance |
| `poll()` | O(log n) | Root nikalo + re-balance |
| `peek()` | O(1) | Sirf root dekho |
| **Sort full PQ** | **O(n log n)** | n polls = n × log(n) |
| Iteration | O(n) | Lekin **UNSORTED** |

### Bottom Line

| Cheez | State |
|-------|-------|
| **PQ ka internal array** | NOT sorted (heap property only) |
| **`peek()` / iterate / print** | Heap order — unsorted |
| **`poll()` ek-ek karke** | SORTED (chhota pehle) |
| **Saare elements poll karo** | Total O(n log n) — fully sorted output |

**Tldr:** PQ **on-demand** sorted deta — jab poll karega tab. Internally storage **lazy-sorted (heap)**, fully sorted NAHI.

### PQ Use Karte Kyu Phir?

Agar full sorting chahiye, `Collections.sort(list)` use karo — same O(n log n).

PQ ka asli use:
- **Top-K problems** — sirf top 5 chahiye, pura sort waste
- **Streaming data** — har naye element pe update, full re-sort nahi
- **Dijkstra, A*** algorithms — minimum repeatedly nikalna
