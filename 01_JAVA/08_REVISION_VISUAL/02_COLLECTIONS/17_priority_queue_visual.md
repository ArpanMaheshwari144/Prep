# PriorityQueue (Min-Heap) — Visual Revision

---

## 1️⃣ Concept (Hospital ER Analogy)

```
Normal queue:
   First come, first served
   Heart attack? Wait apni baari ka
   Wrong for emergencies

Hospital ER (Priority Queue):
   Severity decides order, NOT arrival
   Heart attack → INSTANT priority
   Minor cut    → wait
   Emergency-friendly
```

```
PriorityQueue = ER mein triage system
   "Most important first"
   NOT FIFO like normal queue
```

---

## 2️⃣ Min-Heap vs Max-Heap

```
   MIN-HEAP                  MAX-HEAP
   ─────────                 ────────
   
   Smallest at TOP           Largest at TOP
   poll() = smallest         poll() = largest
   
   Java default = MIN-HEAP
```

---

## 3️⃣ Tree Structure (Behind The Scenes)

```
PriorityQueue internally = BINARY HEAP

   MIN-HEAP example:
   
            1            ← smallest at root
           / \
          3   5
         /\   /\
        4  8 7  9
   
   RULE:
      Parent <= both children
      = Min-heap property
```

```
Stored as ARRAY (compact):
   [1, 3, 5, 4, 8, 7, 9]
   
   Index magic:
      Parent of i  = (i-1) / 2
      Left child   = 2i + 1
      Right child  = 2i + 2
```

---

## 4️⃣ Basic Operations

### Insert (offer)
```java
PriorityQueue<Integer> pq = new PriorityQueue<>();
pq.offer(5);   // [5]
pq.offer(3);   // [3, 5]
pq.offer(7);   // [3, 5, 7]
pq.offer(1);   // [1, 3, 7, 5]  ← 1 bubbles up
```

```
INSERT flow:
   1. Add at last position
   2. "Bubble up" — compare with parent
   3. Smaller? Swap with parent
   4. Repeat until heap property restored
   
   = O(log n)
```

### Remove top (poll)
```java
pq.poll();   // returns 1 (smallest)
pq.poll();   // returns 3
pq.poll();   // returns 5
pq.poll();   // returns 7
```

```
POLL flow:
   1. Take root (smallest)
   2. Move last element to root
   3. "Bubble down" — compare with smaller child
   4. Larger than child? Swap
   5. Repeat until heap property restored
   
   = O(log n)
```

### Peek (look without removing)
```java
pq.peek();   // see top without removal — O(1)
```

---

## 5️⃣ Time Complexity

```
┌────────────────┬──────────────┐
│  Operation     │ Time          │
├────────────────┼──────────────┤
│ offer (insert) │ O(log n)      │
│ poll (remove)  │ O(log n)      │
│ peek           │ O(1)          │
│ contains       │ O(n)          │
└────────────────┴──────────────┘
```

---

## 6️⃣ Custom Priority (with Comparator)

### Max-Heap
```java
PriorityQueue<Integer> maxHeap = 
    new PriorityQueue<>(Comparator.reverseOrder());

maxHeap.offer(5);
maxHeap.offer(3);
maxHeap.offer(7);
maxHeap.poll();   // 7 (largest first)
```

### Object Priority
```java
class Patient {
    String name;
    int severity;   // 10 = critical, 1 = minor
}

// Max severity first (most critical first):
PriorityQueue<Patient> pq = new PriorityQueue<>(
    (a, b) -> b.severity - a.severity
);

pq.offer(new Patient("Mukesh", 3));
pq.offer(new Patient("Suresh", 9));
pq.offer(new Patient("Rakesh", 5));

pq.poll();   // Suresh (severity 9, most critical)
pq.poll();   // Rakesh (5)
pq.poll();   // Mukesh (3)
```

---

## 7️⃣ Real Use Cases

```
1. DIJKSTRA'S ALGORITHM
   Shortest path — process closest node first
   = Min-heap of (distance, node)

2. TOP-K PROBLEMS
   "Top 10 trending tweets"
   "Top K largest numbers"
   = Min-heap of size K

3. TASK SCHEDULER
   Tasks by priority
   = Priority queue

4. EVENT-DRIVEN SIMULATION
   Process earliest event first
   = Min-heap by timestamp

5. HUFFMAN CODING
   Build encoding tree by frequency
   = Min-heap of nodes by frequency
```

---

## 8️⃣ Top K Problem (Classic Pattern)

```java
// Top 3 largest numbers in stream

int k = 3;
PriorityQueue<Integer> minHeap = new PriorityQueue<>();

int[] nums = {1, 5, 3, 8, 2, 7, 4};

for (int n : nums) {
    minHeap.offer(n);
    if (minHeap.size() > k) {
        minHeap.poll();   // remove smallest, keep top K
    }
}

// minHeap now has top 3 largest: [5, 7, 8]
```

```
TRICK:
   Min-heap of size K
   When >K, pop smallest
   = Top K largest remain
   
   Time: O(n log k)
   Space: O(k)
```

---

## 9️⃣ Visual — Bubble Up (Insert)

```
   Insert 1 into:
   
            3
           / \
          5   7
         /\
        8 9
   
   Step 1: Add at last:
            3
           / \
          5   7
         /\   /
        8 9  1
   
   Step 2: Compare with parent (7), 1 < 7 → swap
            3
           / \
          5   1
         /\   /
        8 9  7
   
   Step 3: Compare with parent (3), 1 < 3 → swap
            1
           / \
          5   3
         /\   /
        8 9  7
   
   Done — heap property restored
```

---

## PriorityQueue vs TreeMap

```
┌──────────────────┬─────────────────┬─────────────────┐
│  Aspect          │ PriorityQueue   │ TreeMap         │
├──────────────────┼─────────────────┼─────────────────┤
│ Order operation  │ Peek smallest   │ Full sorted     │
│ Insert           │ O(log n)        │ O(log n)        │
│ Find smallest    │ O(1) peek       │ O(log n)        │
│ Remove arbitrary │ O(n)            │ O(log n)        │
│ Iteration order  │ NOT sorted!     │ Sorted          │
└──────────────────┴─────────────────┴─────────────────┘

KEY:
   PQ = "min/max access fast"
   TreeMap = "full sorted view"
```

---

## 1️⃣1️⃣ Common Trap

```java
PriorityQueue<Integer> pq = new PriorityQueue<>();
pq.offer(5);
pq.offer(3);
pq.offer(7);

// Iterating ≠ sorted order!
for (int n : pq) {
    System.out.print(n + " ");
}
// Output: 3 5 7  ← NOT guaranteed sorted!
// Only poll() gives sorted

// CORRECT way:
while (!pq.isEmpty()) {
    System.out.print(pq.poll() + " ");
}
// Output: 3 5 7  ← guaranteed sorted
```

---

## Memory Hooks

```
PriorityQueue = Hospital ER
   Severity-based, not arrival

Min-heap   = smallest at top (Java default)
Max-heap   = largest at top (use reverseOrder)

Binary heap as array:
   parent  = (i-1)/2
   left    = 2i+1
   right   = 2i+2

offer/poll = O(log n)
peek       = O(1)

USE CASES:
   • Dijkstra
   • Top-K problems
   • Task scheduling
   • Event simulation
```

[← Back to JAVA](../)
