# Map Variants — Visual Revision

---

## 1️⃣ 4 Variants Quick

```
   HashMap          → fast K-V, no order
   LinkedHashMap    → insertion order maintained
   TreeMap          → sorted keys (alphabetical/numeric)
   HashSet          → just keys, no values
```

---

## 2️⃣ Stationary Shop Analogy

```
Imagine 4 types of shelves:

   HashMap     = jumbled box
      Tu daala, jab nikalna hai = fast find
      Order? Random
   
   LinkedHashMap = stack with date sticker
      Jis order mein daala, same order remembered
   
   TreeMap     = alphabetically sorted shelf
      Auto-sorted by key name
   
   HashSet     = guest list (just names)
      Naam unique, value nahi store
```

---

## 3️⃣ Visual — Internal Differences

### HashMap
```
   Buckets array + chain/tree
   ┌──┬──┬──┬──┐
   │ 0│ 1│ 2│..│   no order
   └──┴──┴──┴──┘
   
   Order on iteration = unpredictable
```

### LinkedHashMap
```
   HashMap + doubly linked list of entries
   
   ┌──┬──┬──┬──┐
   │  │  │  │..│
   └──┴──┴──┴──┘
        +
   entry1 → entry2 → entry3 → entry4
   (insertion order tracked)
```

### TreeMap
```
   Red-Black tree (sorted by key)
   
              50
             /  \
           20    70
          /\     /\
         10 30  60 90
   
   In-order traversal = sorted output
```

### HashSet
```
   Internally uses HashMap!
   Each value stored as KEY (dummy value)
   
   Set<String> → HashMap<String, dummy>
   = Unique keys, no value
```

---

## 4️⃣ Iteration Order Comparison

```java
Map<String, Integer> map = ...;
map.put("Zebra", 1);
map.put("Apple", 2);
map.put("Mango", 3);

// HashMap output (any order):
//   Mango=3, Apple=2, Zebra=1   ← random

// LinkedHashMap output (insertion):
//   Zebra=1, Apple=2, Mango=3   ← inserted order

// TreeMap output (sorted):
//   Apple=2, Mango=3, Zebra=1   ← alphabetical
```

---

## 5️⃣ Time Complexity

```
┌─────────────────┬──────────┬─────────────────┬─────────┐
│  Map             │ get/put  │ Iteration order │ Sorted? │
├─────────────────┼──────────┼─────────────────┼─────────┤
│ HashMap          │ O(1)     │ Random           │ NO       │
│ LinkedHashMap    │ O(1)     │ Insertion order  │ NO       │
│ TreeMap          │ O(log n) │ Sorted by key    │ YES      │
└─────────────────┴──────────┴─────────────────┴─────────┘
```

---

## 6️⃣ When To Use Which?

```
HashMap:
   Default choice
   Need fast K-V lookup
   Order doesn't matter
   = 80% use cases

LinkedHashMap:
   Need insertion order remembered
   LRU cache implementation
   Iteration must follow add order

TreeMap:
   Need sorted keys
   Range queries (subMap, headMap, tailMap)
   "First key", "Last key" operations

HashSet:
   Just check existence (no value)
   Unique items
   Remove duplicates
```

---

## 7️⃣ LRU Cache via LinkedHashMap (Bonus)

```java
class LRUCache extends LinkedHashMap<Integer, Integer> {
    private int capacity;
    
    public LRUCache(int capacity) {
        super(capacity, 0.75f, true);  // accessOrder = true
        this.capacity = capacity;
    }
    
    @Override
    protected boolean removeEldestEntry(Map.Entry<Integer, Integer> e) {
        return size() > capacity;
    }
}

// Magic:
//   accessOrder = true → move accessed item to end
//   removeEldestEntry → auto-evict oldest when full
//   = LRU cache in ~10 lines!
```

---

## 8️⃣ HashSet Internals

```java
class HashSet<E> {
    HashMap<E, Object> map;
    static final Object PRESENT = new Object();
    
    boolean add(E e) {
        return map.put(e, PRESENT) == null;
    }
}
```

```
HashSet = HashMap with dummy values
   add(x)       → map.put(x, PRESENT)
   contains(x)  → map.containsKey(x)
   = HashSet basically HashMap with one fake value
```

---

## 9️⃣ TreeSet (Bonus)

```
Sorted Set (alphabetical/numeric order):
   TreeSet<String> set = new TreeSet<>();
   set.add("Zebra");
   set.add("Apple");
   set.add("Mango");
   
   Iteration: Apple, Mango, Zebra (sorted)
```

```
TreeSet internally uses TreeMap
   (Just like HashSet uses HashMap)
```

---

## Full Hierarchy

```
        Map (interface)
        /    |    \
       /     |     \
   HashMap   LinkedHashMap   TreeMap
                                │
                              SortedMap
                              NavigableMap

        Set (interface)
        /    |    \
       /     |     \
   HashSet  LinkedHashSet  TreeSet
   (uses    (uses          (uses
    HashMap) LinkedHashMap) TreeMap)
```

---

## Memory Hooks

```
HashMap       = jumbled box  (fast, no order)
LinkedHashMap = dated stack  (insertion order)
TreeMap       = alphabetical (sorted)
HashSet       = guest list  (just keys)

LRU cache     = LinkedHashMap with accessOrder=true
TreeSet       = sorted unique items
```

[← Back to JAVA](../)
