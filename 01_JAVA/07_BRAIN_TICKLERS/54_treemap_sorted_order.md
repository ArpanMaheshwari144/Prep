# TreeMap = Hamesha Sorted Order

> **V90 — Brain Ticklers: Topic 54**

---

## STORY — Library Catalog

> Imagine **library mein 3 system**:
>
> **HashMap** = *"Books pile up randomly"* — fast pickup, but order pata nahi
>
> **LinkedHashMap** = *"Books shelved jaise rakhi"* — insertion order maintained
>
> **TreeMap** = *"Library catalog A-Z"* — alphabetical hamesha, automatic sorting
>
> Tu jab bhi book "Z" daale ya "A" daale — TreeMap **automatically right place** pe rakh deta. **Self-organizing.**

---

## INSERTION VISUAL

```
INSERT ORDER:        TreeMap (auto-sort):       HashMap (random):
─────────────       ─────────────────────       ──────────────────

put("Banana")  →    A: -                        bucket[3]: Banana
                    B: Banana                   bucket[7]: -
                    C: -                        bucket[1]: -

put("Apple")   →    A: Apple                    bucket[3]: Banana
                    B: Banana                   bucket[7]: Apple ← random pos
                    C: -

put("Cherry")  →    A: Apple                    bucket[3]: Banana
                    B: Banana                   bucket[7]: Apple
                    C: Cherry                   bucket[1]: Cherry

Iteration:    Apple → Banana → Cherry        Banana → Apple → Cherry
              (sorted!)                       (random — HashMap)
```

---

## CODE EXAMPLE

```java
Map<String, Integer> map = new TreeMap<>();
map.put("Banana", 2);
map.put("Apple", 5);
map.put("Cherry", 1);

System.out.println(map.keySet());
// Output: [Apple, Banana, Cherry]   ← alphabetically sorted

// Iteration ka order — sorted bhi
for (Map.Entry<String, Integer> e : map.entrySet()) {
    System.out.println(e.getKey() + " = " + e.getValue());
}
// Apple = 5
// Banana = 2
// Cherry = 1
```

---

## INTERNAL — Red-Black Tree

TreeMap **balanced binary search tree** use karta — Red-Black tree.

```
              Banana
             /      \
         Apple    Cherry
                  /
              Cherry?
              
   Properties:
   • Left subtree < node < right subtree
   • Auto-balanced on insert/delete
   • Operations: O(log n) guaranteed
```

**Java implementation:** `java.util.TreeMap` uses Red-Black Tree internally.

---

## 3 Maps Comparison

| Property | `HashMap` | `LinkedHashMap` | `TreeMap` |
|---|---|---|---|
| **Order** | None (random) | Insertion order | **Sorted** (natural / Comparator) |
| **Internal** | Array + LinkedList/Tree | HashMap + Doubly-LinkedList | **Red-Black Tree** |
| **get/put complexity** | O(1) avg | O(1) avg | **O(log n)** |
| **Iteration order** | Random | Insertion | Sorted |
| **null key** | 1 allowed | 1 allowed | NPE (compare fails) |
| **null value** | Multiple | Multiple | Multiple |
| **Memory** | Lowest | Higher (linked list overhead) | Higher (tree overhead) |
| **Use case** | Fastest lookup | LRU cache, ordered iteration | Sorted range queries, leaderboard |

---

## USE CASE GUIDE

### Use `HashMap` when:
- Fastest get/put needed
- Order doesn't matter
- Memory matters

### Use `LinkedHashMap` when:
- Insertion order matters
- LRU cache (with `accessOrder=true`)
- Order-preserving iteration

### Use `TreeMap` when:
- Sorted iteration needed
- Range queries (`subMap`, `headMap`, `tailMap`)
- Leaderboard / ranked data
- Dictionary / glossary
- First/last/floor/ceiling key needed

```java
TreeMap<Integer, String> scores = new TreeMap<>();
scores.put(100, "Arpan");
scores.put(85, "Rahul");
scores.put(92, "Priya");

scores.firstKey();           // 85 (lowest)
scores.lastKey();            // 100 (highest)
scores.floorKey(90);         // 85 (largest <= 90)
scores.ceilingKey(90);       // 92 (smallest >= 90)
scores.subMap(85, 95);       // {85=Rahul, 92=Priya}
```

---

## Custom Comparator

Default = **natural ordering** (alphabetical for String, numeric for Integer).

Custom order chahiye? **Comparator** pass karo:

```java
// Reverse order
TreeMap<String, Integer> reverseMap = new TreeMap<>(Comparator.reverseOrder());
reverseMap.put("Apple", 1);
reverseMap.put("Banana", 2);
// Iteration: Banana → Apple

// Length-based ordering
TreeMap<String, Integer> byLength = new TreeMap<>(
    Comparator.comparingInt(String::length).thenComparing(Comparator.naturalOrder())
);
```

---

## INTERVIEW TRAPS

### Trap 1: null key in TreeMap
```java
TreeMap<String, Integer> map = new TreeMap<>();
map.put(null, 1);   // NullPointerException
                     // Why? compareTo() called → null.compareTo() → NPE
```

**HashMap allows null key, TreeMap NEVER.**

### Trap 2: Custom class as key without Comparable

```java
class Person {
    String name;
}

TreeMap<Person, Integer> map = new TreeMap<>();
map.put(new Person("Arpan"), 1);   // ClassCastException
                                    // Person doesn't implement Comparable
```

**Fix:** Implement Comparable OR pass Comparator to TreeMap constructor.

### Trap 3: VALUES sort hoti? NO, KEYS!

```java
TreeMap<String, Integer> map = new TreeMap<>();
map.put("Banana", 1);
map.put("Apple", 99);

// Iteration:
// Apple = 99   ← sorted by KEY (Apple), value 99 ke order pe nahi
// Banana = 1
```

---

## INTERVIEW TALKING POINT

**Q: "HashMap vs TreeMap — kab konsa?"**

> *"3 things alag:
>
> 1. **Order:** HashMap koi guarantee nahi, TreeMap **sorted by keys** (Red-Black Tree).
> 2. **Performance:** HashMap O(1), TreeMap O(log n) — sorted maintenance ka cost.
> 3. **null:** HashMap allows null key, TreeMap NEVER (compareTo crashes).
>
> HashMap default choice fastest lookup ke liye. TreeMap jab sorted iteration, range queries, ya leaderboard chahiye. LinkedHashMap insertion order ke liye between dono."*

**Q: "TreeMap implementation kya use karta?"**

> *"Red-Black Tree — self-balancing BST. Insert/delete pe automatic rotations balance maintain karte. O(log n) guaranteed for all operations. NavigableMap interface implement karta — `floorKey`, `ceilingKey`, `subMap` jaisi range queries."*

**Q: "null key kyu nahi allow?"**

> *"TreeMap sort karne ke liye `compareTo()` call karta keys pe. `null.compareTo(anything)` = NullPointerException. Toh insert time hi NPE throw kar deta. Custom Comparator bhi null handle nahi karta unless explicitly coded."*

---

## POWER PHRASE

> **"TreeMap keys ko sorted order mein maintain karta Red-Black Tree se — O(log n) operations. Use karo jab natural ordering, range queries, ya sorted iteration chahiye. Range queries ke liye `subMap`, `floorKey`, `ceilingKey` powerful methods. null key NAHI accept karta."**

---

## MEMORY HOOK

```
HashMap         → "Random pile" — fastest, no order
LinkedHashMap   → "Shelf order" — insertion preserved
TreeMap         → "Library catalog" — alphabetical/numeric sorted

Internal:
   HashMap         = Array + LinkedList (treeified at 8)
   LinkedHashMap   = HashMap + DoublyLinkedList
   TreeMap         = Red-Black Tree

null key:
   HashMap        
   LinkedHashMap  
   TreeMap        (NPE — compareTo fails)

Use case:
   "Fastest"      → HashMap
   "Order matter" → LinkedHashMap
   "Sorted/range" → TreeMap
```

---

## TRAP BOX

```
Trap 1: "TreeMap mein values sort hoti"
         KEYS sort hoti, values nahi
         Sort by value? entrySet stream + sorted use karo

Trap 2: "TreeMap mein null key daal sakte"
         NPE — compareTo fails
         HashMap null key support

Trap 3: "Custom class without Comparable"
         ClassCastException
         Implement Comparable OR pass Comparator

Trap 4: "TreeMap fast hai"
         O(log n) — slower than HashMap O(1)
         Trade-off: order vs speed

Trap 5: "TreeMap thread-safe"
         NOT thread-safe
         Use Collections.synchronizedSortedMap() or ConcurrentSkipListMap
```
