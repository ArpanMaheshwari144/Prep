# Pattern 11: Queue

## Ye kya hai?

Bhai dekh — line mein khade log. Pehle aaya, pehle jayega. **First In First Out (FIFO).**

Stack mein upar se rakh upar se utha (LIFO). Queue mein peeche se daal, aage se nikaal (FIFO).

```
push(1) → [1]
push(2) → [1, 2]
push(3) → [1, 2, 3]
pop()   → 1 nikla. [2, 3]     ← PEHLE aaya pehle gaya
front() → 2
```

## Stack vs Queue:

```
Stack: LIFO — last in first out. Plates ka stack.
Queue: FIFO — first in first out. Line mein khade log.
```

## Kab use karte hain?

- **BFS (Breadth First Search)** — level by level traverse (Trees, Graphs)
- **Task scheduling** — pehle aaya pehle process
- **Sliding Window** mein kabhi kabhi deque use hota

## Hum already Queue use kar chuke hain!

**Level Order Traversal (Q48)** mein queue use kiya tha:
- Root push karo
- Queue se nikalo, children push karo
- Level by level traverse

## Dry Run — BFS (Level Order):

```
        1
       / \
      2   3

Queue: [1]

Step 1: 1 nikala. Children 2,3 daale. Queue: [2, 3]. Level 0: [1]
Step 2: size=2. 2 nikala, 3 nikala. Queue: []. Level 1: [2, 3]

Answer: [[1], [2, 3]] ✅
```

## C++ mein Queue:

```cpp
#include <queue>
queue<int> q;
q.push(5);       // peeche se daal
q.front();       // aage wala dekh
q.pop();         // aage se nikaal
q.empty();       // khaali hai?
q.size();        // kitne elements
```

---

## Questions Jo Kiye

| # | Question | Kahan kiya |
|---|----------|-----------|
| 48 | Level Order Traversal (BFS) | Trees folder mein — queue use kiya |

---

## Ek Line Mein Yaad Rakh

> Queue = **"Line mein khade. Pehle aaya pehle gaya. BFS ka base."**
