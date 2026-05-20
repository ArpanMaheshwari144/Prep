# HashMap Internal — Visual Revision

---

## 1️⃣ Concept (Locker Analogy)

```
Imagine GYM mein 16 lockers (numbered 0-15):
   
   ┌────┬────┬────┬────┬────┐
   │  0 │  1 │  2 │  3 │ ...│  → 16 lockers
   └────┴────┴────┴────┴────┘
   
   Tu locker maange:
      "Mera naam Arpan hai, locker do"
      
   Manager:
      hash("Arpan") % 16 = (kuch index, maan le 5)
      "Locker 5 mein daal do"
```

```
HashMap = manager
Lockers = bucket array (default 16)
Naam     = key
Saamaan  = value
```

---

## 2️⃣ Internal Structure

```java
HashMap<String, String> map = new HashMap<>();
map.put("Arpan", "Java");
map.put("Suresh", "Python");
```

```
   Internal array (16 buckets):
   
   ┌───┬───┬───┬───┬───┬───────┬───┬─────────┐
   │ 0 │ 1 │ 2 │ 3 │ 4 │   5   │ 6 │   ...   │
   ├───┼───┼───┼───┼───┼───────┼───┼─────────┤
   │   │   │ S │   │   │   A   │   │         │
   └───┴───┴───┴───┴───┴───────┴───┴─────────┘
                  ▲          ▲
                  │          │
            hash(Suresh)  hash(Arpan)
            % 16 = 2      % 16 = 5
```

---

## 3️⃣ Put Operation Flow

```
   map.put("Arpan", "Java")
        │
        ▼
   1. hash("Arpan") → some int (let's say 1234567)
        │
        ▼
   2. 1234567 % 16 = 5  (index)
        │
        ▼
   3. Bucket 5 mein store
        Key   = "Arpan"
        Value = "Java"
```

---

## 4️⃣ Get Operation Flow

```
   map.get("Arpan")
        │
        ▼
   1. hash("Arpan") → 1234567
        │
        ▼
   2. 1234567 % 16 = 5
        │
        ▼
   3. Go to bucket 5
        │
        ▼
   4. Find key matching "Arpan"
        equals() check
        │
        ▼
   5. Return value: "Java"
```

```
Time complexity: O(1) average
   (single bucket access + equals check)
```

---

## 5️⃣ Collision Problem

```
PROBLEM:
   2 different keys → SAME bucket?
   
   hash("Arpan")  % 16 = 5
   hash("Mukesh") % 16 = 5    ← SAME bucket!
   
   = COLLISION
```

```
Why?
   16 buckets, but BILLIONS of possible strings
   Multiple keys can land same bucket
   = Pigeonhole principle
```

---

## 6️⃣ Solution — Chaining (LinkedList)

```
Bucket 5 mein LinkedList:
   
   Bucket 5:
   ┌─────────┐    ┌─────────┐    ┌─────────┐
   │ Arpan   │───▶│ Mukesh  │───▶│ Rakesh  │
   │ Java    │    │ Python  │    │ Go      │
   └─────────┘    └─────────┘    └─────────┘
   
   Same bucket → chain karte
```

```
GET flow with collision:
   "Mukesh" find karna?
        │
        ▼
   Bucket 5 → LinkedList traverse
        Arpan? no
        Mukesh? YES → return
        
   Time complexity: O(n) worst case (all collide)
                    O(1) average (good hash)
```

---

## 7️⃣ Java 8 Treeify (Performance Fix)

```
PROBLEM:
   Bucket mein 100+ items collide?
   LinkedList traversal = slow O(n)

SOLUTION (Java 8):
   Bucket mein 8+ items → CONVERT TO TREE (Red-Black)
   Tree traversal = O(log n)
```

```
   Bucket 5 (8+ collisions):
   
   BEFORE (LinkedList):
      A → M → R → S → T → V → W → X → Y
      Search Y = 9 comparisons
   
   AFTER (Tree):
              S
             / \
            M   V
           /\   /\
          A R  T W
                  \
                   Y
      Search Y = 3 comparisons (log2 8)
```

```
TREEIFY threshold = 8 (default)
UNTREEIFY = 6 (back to list if shrinks)
```

---

## 8️⃣ Load Factor + Rehashing

```
HashMap default:
   Capacity (buckets) = 16
   Load factor = 0.75
```

```
PROBLEM:
   Tu 100 items daal raha 16 buckets mein
   Sab bucket bhar gaye
   Collisions everywhere
   = Slow

SOLUTION: REHASHING
   Buckets used / capacity > 0.75?
      → Double capacity (16 → 32)
      → Re-distribute all existing items
```

---

## 9️⃣ Rehashing Visual

```
   Before rehash (16 buckets, 12 items, load = 12/16 = 0.75):
   ┌─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┐
   │A│B│C│D│ │E│F│G│ │H│I│J│K│L│ │ │
   └─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┘
   
   Threshold cross → REHASH
   
   After rehash (32 buckets):
   ┌─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┐
   │A│ │ │B│ │ │C│ │ │D│ │ │E│ │ │ │F│ │ │ │G│ │ │ │H│ │ │ │I│ │ │ │
   └─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┘
   = More spread, less collision
```

```
COST:
   Rehashing = expensive (recompute all hashes)
   But amortized O(1) put still
```

---

## hashCode() + equals() Contract

```
KEY RULES (Interview Classic):

1. Two equal objects → MUST have SAME hashCode
   a.equals(b) == true → a.hashCode() == b.hashCode()

2. Same hashCode → NOT necessarily equal
   (Collisions allowed)

3. If you OVERRIDE equals, MUST override hashCode
   Otherwise HashMap broken
```

```
PROOF:
   class Person {
       String name;
       
       @Override
       public boolean equals(Object o) { ... }   // override
       // forgot hashCode
   }
   
   Map<Person, String> map = new HashMap<>();
   map.put(new Person("Arpan"), "Java");
   map.get(new Person("Arpan"));   // returns NULL!
   
   Why? new Person().hashCode() = random
        → different buckets
        → not found
```

---

## 1️⃣1️⃣ Time Complexity Summary

```
┌────────────────┬──────────────┬─────────────┐
│  Operation     │ Average      │ Worst        │
├────────────────┼──────────────┼─────────────┤
│ put            │ O(1)         │ O(log n)     │
│                │              │ (after tree) │
│ get            │ O(1)         │ O(log n)     │
│ remove         │ O(1)         │ O(log n)     │
│ containsKey    │ O(1)         │ O(log n)     │
└────────────────┴──────────────┴─────────────┘
```

---

## 1️⃣2️⃣ HashMap vs ConcurrentHashMap (Bonus)

```
HashMap:
   Not thread-safe
   Concurrent writes = data corruption
   
ConcurrentHashMap:
   Thread-safe
   Internal segment/bucket-level locking
   Reads usually lock-free
   = Multi-thread use
```

---

## Memory Hooks

```
HashMap = Locker system
   16 default buckets
   key → hash → bucket index

Collision = same bucket, different keys
   Chain via LinkedList
   8+ items → convert to RED-BLACK TREE (Java 8)

Load factor = 0.75
   Cross threshold → REHASH (double capacity)

hashCode + equals contract:
   equals(a, b) = true → hashCode same
   Override one → must override other

Avg: O(1)  |  Worst: O(log n) after treeify
```

[← Back to JAVA](../)
