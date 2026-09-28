# HashMap vs LinkedHashMap vs TreeMap

> **V90 — Collections: Topic 28**

---

## WHY — Alag-Alag Maps Kyu?

→ **HashMap** = `O(1)` lookup, **NO order**
→ **LinkedHashMap** = `O(1)` + **INSERTION order** maintain
→ **TreeMap** = `O(log n)`, **SORTED** by key (Red-Black tree)
→ Order chahiye → LinkedHashMap. Sorting chahiye → TreeMap. Speed chahiye → HashMap. **95% cases HashMap.**

---

## STORY — User Database

→ **userId se naam fetch** karna tha — HashMap = fastest, no order
→ Ek baar **LRU cache** banana tha — insertion order maintain karni thi → **LinkedHashMap**
→ Aur ek baar **sorted users by ID** chahiye the — keys auto-sorted → **TreeMap**
→ Teen alag problems, teen alag Maps. Andar se bilkul alag kaam karte

---

## Code — Teeno

### 1. HashMap — Sabse Fast, NO Order
```java
Map<Integer, String> map = new HashMap<>();
map.put(103, "Priya"); map.put(101, "Arpan"); map.put(102, "Rahul");

System.out.println(map);
// {102=Rahul, 101=Arpan, 103=Priya} — ORDER KUCH BHI HO SAKTA HAI
// get/put = O(1)
```

### 2. LinkedHashMap — INSERTION Order Maintain
```java
Map<Integer, String> map = new LinkedHashMap<>();
map.put(103, "Priya"); map.put(101, "Arpan"); map.put(102, "Rahul");

System.out.println(map);
// {103=Priya, 101=Arpan, 102=Rahul} — INSERTION ORDER SAME
// HashMap + doubly linked list extra
```

### 3. TreeMap — Sorted Order, Hamesha
```java
Map<Integer, String> map = new TreeMap<>();
map.put(103, "Priya"); map.put(101, "Arpan"); map.put(102, "Rahul");

System.out.println(map);
// {101=Arpan, 102=Rahul, 103=Priya} — KEY PE SORTED
// Red-Black Tree andar — get/put = O(log n)
// Range queries: firstKey(), lastKey(), subMap()
```

---

## Visualization — Andar Ka Structure

```
              3 Map Types — Andar Ka Structure

╔════════════════════════════════════════════════════════════╗
║ HashMap — Hash Table (No Order)                            ║
╚════════════════════════════════════════════════════════════╝

put(101, "Arpan"); put(103, "Priya"); put(102, "Rahul");

Buckets (hashCode se decide):
┌────┐
│ 0  │ → null
├────┤
│ 1  │ → null
├────┤
│ 2  │ → [102: Rahul]   ← hashCode(102) % size = 2
├────┤
│ 3  │ → [101: Arpan]   ← hashCode(101) % size = 3
├────┤
│ 4  │ → null
├────┤
│ 5  │ → [103: Priya]   ← hashCode(103) % size = 5
├────┤
│ 6  │ → null
└────┘

Iterate output:  {102=Rahul, 101=Arpan, 103=Priya}  ← bucket order, kuch bhi


╔════════════════════════════════════════════════════════════╗
║ LinkedHashMap — Hash Table + Doubly Linked List            ║
╚════════════════════════════════════════════════════════════╝

put(101, "Arpan"); put(103, "Priya"); put(102, "Rahul");

Buckets (HashMap jaisa):       Insertion-Order Linked List:
┌────┐
│ 2  │ → [102: Rahul]           HEAD → [101] ↔ [103] ↔ [102] ← TAIL
├────┤                                  Arpan   Priya  Rahul
│ 3  │ → [101: Arpan]
├────┤                          Iterate output:
│ 5  │ → [103: Priya]            {101=Arpan, 103=Priya, 102=Rahul}
└────┘                            (insertion order maintained )


╔════════════════════════════════════════════════════════════╗
║ TreeMap — Red-Black Tree (Sorted by Key)                   ║
╚════════════════════════════════════════════════════════════╝

put(101, "Arpan"); put(103, "Priya"); put(102, "Rahul");

       ┌──────────┐
       │ 102:Rahul│  (root, balanced)
       └──┬────┬──┘
          │    │
   ┌──────▼┐  ┌▼──────┐
   │101    │  │103    │
   │Arpan  │  │Priya  │
   └───────┘  └───────┘

Iterate output:  {101=Arpan, 102=Rahul, 103=Priya}  ← sorted by key
Operations:      O(log n)
```

---

## Teeno Ek Saath

| Feature | HashMap | LinkedHashMap | TreeMap |
|---------|---------|---------------|---------|
| **Order** | No order | Insertion order | Sorted by key |
| **Performance** | O(1) | O(1) | O(log n) |
| **Andar kya?** | Hash table | Hash + LinkedList | Red-Black Tree |
| **null key?** | Allowed (1 only) | Allowed (1 only) | **NOT allowed** |
| **Memory** | Less | Thoda zyada | Zyada |

---

## TRAP 1

> **TreeMap null key nahi leta — `NullPointerException`.**
> Kyu? Sort karta hai → `null.compareTo()` = crash. **HashMap null key allowed hai.**

**TreeMap ke 2 aur trap:**
```java
// Apni class key bani, Comparable nahi -> put pe ClassCastException
TreeMap<Person, Integer> m = new TreeMap<>();                // Person implements Comparable ? nahi -> crash
TreeMap<Person, Integer> ok = new TreeMap<>(Comparator.comparing(Person::getName));  // fix

// Sort KEY pe hota, VALUE pe nahi
map.put("Banana", 1); map.put("Apple", 99);   // iteration: Apple=99, Banana=1

// Ulta / apna order
new TreeMap<String, Integer>(Comparator.reverseOrder());
```

## TRAP 2

> **TreeMap = KEYS sort hoti, VALUES nahi.**
> ```java
> map.put("Rahul", 2); map.put("Arpan", 1);
> // Arpan pehle aayega (A < R)
> // Values (1, 2) se koi matlab nahi — sirf keys dictionary order mein.
> ```

---

## ★ TRAP — TreeMap ke baaki trap + NavigableMap methods

**Analogy — library ke 3 system:** HashMap = books random pile (fast pickup, order pata nahi) · LinkedHashMap = jaise rakhi waise shelf pe (insertion order) · TreeMap = library catalog A-Z — "Z" daalo ya "A", **automatically sahi jagah**. Self-organizing.

```java
Map<String, Integer> map = new TreeMap<>();
map.put("Banana", 2);
map.put("Apple", 5);
map.put("Cherry", 1);
System.out.println(map.keySet());   // [Apple, Banana, Cherry]  ← insert order Banana, Apple, Cherry tha
```

**Range / nearest queries (NavigableMap) — TreeMap ka asli faayda:**
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
Aur `headMap`, `tailMap` bhi — sab range queries.

**Custom order — length pe, phir alphabet:**
```java
TreeMap<String, Integer> byLength = new TreeMap<>(
    Comparator.comparingInt(String::length).thenComparing(Comparator.naturalOrder())
);
```

**Kab kaunsa:**
- **HashMap** → fastest get/put, order matter nahi, memory kam
- **LinkedHashMap** → insertion order, **LRU cache (`accessOrder=true` constructor)**, order-preserving iteration
- **TreeMap** → sorted iteration, range queries (`subMap/headMap/tailMap`), leaderboard / ranked data, dictionary, first/last/floor/ceiling key

```
Trap: "values sort karni hain"   → TreeMap KEYS sort karta. Value pe sort = entrySet().stream().sorted(...)
Trap: "TreeMap fast hai"         → O(log n), HashMap O(1) se slow. Trade-off: order vs speed
Trap: "TreeMap thread-safe"      → NAHI. Collections.synchronizedSortedMap() ya ConcurrentSkipListMap
Trap: null key + custom Comparator → Comparator bhi null handle nahi karta jab tak explicitly code na karo
```

**Q: "HashMap vs TreeMap — kab konsa?"**
> *"3 cheezein alag: (1) Order — HashMap koi guarantee nahi, TreeMap sorted by keys (Red-Black Tree). (2) Performance — HashMap O(1), TreeMap O(log n) — sorted maintain karne ka cost. (3) null — HashMap null key allow, TreeMap NEVER (compareTo crash). HashMap default choice; TreeMap jab sorted iteration, range queries, leaderboard; LinkedHashMap insertion order ke liye."*

**Q: "TreeMap implementation kya use karta?"**
> *"Red-Black Tree — self-balancing BST. Insert/delete pe automatic rotations balance rakhte → O(log n) guaranteed. NavigableMap implement karta — `floorKey`, `ceilingKey`, `subMap` jaisi range queries."*

> **"TreeMap keys ko sorted order mein maintain karta Red-Black Tree se — O(log n) operations. Use karo jab natural ordering, range queries, ya sorted iteration chahiye. Range queries ke liye `subMap`, `floorKey`, `ceilingKey` powerful methods. null key NAHI accept karta."**

---

## ★ TRAP — Null key / value rules, SAB maps ek jagah

**Common confusion:** "LinkedHashMap null key allow karta?" → **HAAN** — LinkedHashMap **HashMap ko extend** karta, same null rules.

| Map | null key? | null value? | Reason |
|-----|-----------|-------------|--------|
| **HashMap** | 1 allowed | multiple | hashCode special handle (`null` → bucket 0) |
| **LinkedHashMap** | 1 allowed | multiple | HashMap **extend** karta — same rules |
| **TreeMap** | NO | allowed (multiple) | sort = `null.compareTo()` = crash |
| **ConcurrentHashMap** | NO | NO | thread safety + ambiguity (poora = `02_hashmap_vs_concurrenthashmap.md`) |
| **Hashtable** (legacy) | NO | NO | thread-safe, null banned |

```java
LinkedHashMap<String, Integer> map = new LinkedHashMap<>();
map.put(null, 100);                    // kaam karta
map.put("Arpan", 200);
map.put(null, 999);                    // null key 1 hi hota — value REPLACE

System.out.println(map);
// {null=999, Arpan=200} — INSERTION ORDER + null key allowed
```

> **HashMap, LinkedHashMap → null key 1 allowed (LinkedHashMap = HashMap ka child).**
> **TreeMap → sort crash, no null key. Null value OK.**
> **ConcurrentHashMap / Hashtable → NO null AT ALL (key + value).**

> *"HashMap and LinkedHashMap allow one null key — LinkedHashMap extends HashMap so same rules apply. TreeMap rejects null keys because it cannot compare them. ConcurrentHashMap rejects all nulls to avoid the race condition between `get()` and `containsKey()`."*

---

## POWER PHRASES

> *"HashMap gives O(1) performance with no order guarantee. LinkedHashMap maintains insertion order using an additional linked list — same O(1) but slightly more memory. TreeMap keeps keys sorted using a Red-Black Tree — O(log n). TreeMap does not allow null keys since it needs to compare them."*

> **Yaad rakh:**
> HashMap = fast, no order
> LinkedHashMap = fast + insertion order
> TreeMap = sorted + slow
> **Keys ki dictionary, values se sorting nahi.**
