# TreeMap = Hamesha Sorted Order

> **V90 — Brain Ticklers: Topic 54**

---

## 📖 STORY

→ Insert kiya: Banana, Apple, Cherry — **alag-alag order mein**
→ TreeMap output: **Apple, Banana, Cherry** — alphabetically sorted, hamesha

```java
Map<String, Integer> map = new TreeMap<>();
map.put("Banana", 2);
map.put("Apple", 5);
map.put("Cherry", 1);

map.keySet();    // [Apple, Banana, Cherry] — SORTED!
```

---

## 🟡 WHY 3 Alag Maps?

| | Order | Use Case |
|--|------|---------|
| **HashMap** | Koi guarantee nahi | Fastest O(1) — speed chahiye |
| **LinkedHashMap** | Insertion order | LRU cache, order matter karta |
| **TreeMap** | Sorted (Red-Black Tree) | Leaderboard, dictionary, range queries |

---

## 🔴 TRAP

> **TreeMap null key NAHI leta** — sort karta hai, `null.compareTo()` = crash.
> **HashMap null key allowed.**
> **TreeMap = KEYS sort hoti, VALUES nahi.**

---

## 💬 POWER PHRASE

> *"TreeMap maintains keys in sorted order using a Red-Black Tree — `O(log n)` operations. Use it when you need natural ordering, range queries, or sorted iteration. HashMap if order doesn't matter, LinkedHashMap for insertion order."*
