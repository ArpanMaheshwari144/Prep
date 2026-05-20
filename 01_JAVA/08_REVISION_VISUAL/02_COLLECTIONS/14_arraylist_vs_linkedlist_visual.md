# ArrayList vs LinkedList — Visual Revision

---

## 1️⃣ Pehle Visualization

### ArrayList (Parking Spots)
```
   Imagine parking lot:
   
   Slot 0  Slot 1  Slot 2  Slot 3  Slot 4
   ┌──┐   ┌──┐    ┌──┐    ┌──┐    ┌──┐
   │A │   │B │    │C │    │D │    │E │
   └──┘   └──┘    └──┘    └──┘    └──┘
   
   • Numbered slots
   • Direct access: "Slot 3 mein kya hai?" → instant
   • Fixed-size physical layout
```

### LinkedList (Chain of People)
```
   Imagine people holding hands in a chain:
   
   ┌──┐    ┌──┐    ┌──┐    ┌──┐    ┌──┐
   │A │────│B │────│C │────│D │────│E │
   └──┘    └──┘    └──┘    └──┘    └──┘
   
   • Each person knows next (and previous)
   • To find "D" → start from A → B → C → D
   • No numbered slots
```

---

## 2️⃣ Internal Structure

### ArrayList — Dynamic Array
```java
class ArrayList {
    Object[] elementData;       // backed by array
    int size;
}
```

```
   Memory layout:
   ┌──┬──┬──┬──┬──┬──┬──┬──┬──┬──┐
   │ A│ B│ C│ D│ E│  │  │  │  │  │   capacity 10
   └──┴──┴──┴──┴──┴──┴──┴──┴──┴──┘
   size = 5
   
   Capacity full? → grow (1.5x by default)
```

### LinkedList — Doubly Linked Nodes
```java
class Node {
    Object data;
    Node prev;
    Node next;
}
```

```
   Each node = separate object in memory:
   
   ┌─────────┐    ┌─────────┐    ┌─────────┐
   │ data: A │    │ data: B │    │ data: C │
   │ prev: -1│←──→│ prev: A │←──→│ prev: B │
   │ next: B │    │ next: C │    │ next:-1 │
   └─────────┘    └─────────┘    └─────────┘
```

---

## 3️⃣ Access Time — get(index)

### ArrayList
```
   get(3) → direct array access
        │
        ▼
   array[3] → O(1)
   = INSTANT
```

### LinkedList
```
   get(3) → traverse from head
        │
        ▼
   head → next → next → next (4th node)
   = O(n) traversal
   = SLOW
```

```
┌──────────────┬─────────────┐
│ ArrayList    │  O(1)       │
│ LinkedList   │  O(n)       │
└──────────────┴─────────────┘
```

---

## 4️⃣ Insert at END

### ArrayList
```
   add("F") at end → place at next slot
   
   Before: [A, B, C, D, E, _, _]
   After:  [A, B, C, D, E, F, _]
   
   = O(1) amortized
   (occasional O(n) when resize needed)
```

### LinkedList
```
   add("F") at end → connect new node to tail
   
   ...E ←──→ F
   
   = O(1) (we have tail pointer)
```

```
┌──────────────┬─────────────┐
│ ArrayList    │  O(1) avg   │
│ LinkedList   │  O(1)       │
└──────────────┴─────────────┘
```

---

## 5️⃣ Insert at MIDDLE — BIG Difference

### ArrayList
```
   add(2, "X") → insert at index 2
   
   Before:  [A, B, C, D, E]
                     ↑
                     index 2
   
   Step 1: Shift right
   After:   [A, B, _, C, D, E]
                  ↑
                  insert here
   After:   [A, B, X, C, D, E]
   
   = O(n) — all subsequent elements shift!
```

### LinkedList
```
   add(2, "X") → insert at index 2
   
   Step 1: Traverse to index 2 (O(n))
   Step 2: Adjust pointers (O(1))
   
   ┌──┐    ┌──┐    ┌──┐    ┌──┐
   │A │────│B │────│C │────│D │
   └──┘    └──┘    └──┘    └──┘
   
   After insert:
   ┌──┐    ┌──┐    ┌──┐    ┌──┐    ┌──┐
   │A │────│B │────│X │────│C │────│D │
   └──┘    └──┘    └──┘    └──┘    └──┘
   
   = O(n) traversal + O(1) insertion
   
   BUT if you ALREADY have a node reference:
      Just adjust pointers = O(1)
```

---

## 6️⃣ Delete at MIDDLE

### ArrayList
```
   remove(2) → element at index 2
   
   Before: [A, B, C, D, E]
   After:  [A, B, D, E, _]   (shift left)
   
   = O(n) — shifting
```

### LinkedList
```
   Just adjust prev/next pointers
   = O(1) if you have node reference
   = O(n) if you only have index
```

---

## 7️⃣ Time Complexity Summary

```
┌─────────────────┬──────────────┬─────────────────┐
│  Operation      │  ArrayList   │  LinkedList     │
├─────────────────┼──────────────┼─────────────────┤
│ get(i)          │  O(1)        │  O(n)           │
│ add(end)        │  O(1) avg    │  O(1)           │
│ add(middle)     │  O(n)        │  O(n) traversal │
│ remove(i)       │  O(n)        │  O(n) traversal │
│ contains(x)     │  O(n)        │  O(n)           │
└─────────────────┴──────────────┴─────────────────┘
```

---

## 8️⃣ Memory Usage

```
ArrayList:
   • Contiguous array
   • Memory overhead: ~empty slots in capacity
   • CPU cache friendly
   = LESS memory per element

LinkedList:
   • Each node has data + prev + next references
   • 3x memory per element vs array
   • Nodes scattered in heap
   • CPU cache UNFRIENDLY
   = MORE memory per element
```

---

## 9️⃣ When To Use Which?

```
USE ARRAYLIST:
   Frequent random access (get by index)
   Mostly read-heavy
   Append-only (add at end)
   Memory matters
   Cache-friendly traversal
   
   = 90% of use cases

USE LINKEDLIST:
   Frequent insert/delete at known positions
   Implementing Queue (LinkedList implements Deque)
   No random access needed
   
   = Niche cases
```

---

## Real Production Reality

```
Joshua Bloch (Effective Java):
   "Almost always prefer ArrayList over LinkedList"

Reasons:
   • Cache locality (CPU loves arrays)
   • Better real-world performance
   • Less memory
   • Simpler

Use LinkedList ONLY when:
   • You need Deque/Queue operations
   • Frequent insert/remove at known node refs
```

---

## 1️⃣1️⃣ Vector vs ArrayList (Legacy)

```
Vector (old, Java 1.0):
   • Synchronized (thread-safe but slow)
   • Doubles capacity on grow

ArrayList (Java 1.2+):
   • Not synchronized
   • Grows by 50%

Modern equivalent of Vector:
   Collections.synchronizedList(arrayList)
   OR
   CopyOnWriteArrayList (read-heavy concurrent)
```

---

## Memory Hooks

```
ArrayList   = Parking lot (indexed slots, direct access)
LinkedList  = Chain of people (sequential, no index)

ArrayList:
   get  = O(1) 
   middle insert/delete = O(n) 

LinkedList:
   get  = O(n) 
   adjust pointers = O(1) (if node ref)

Default choice = ArrayList (90% cases)
```

[← Back to JAVA](../)
