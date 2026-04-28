# ArrayList — Andar Se Samjho

> **V90 — Collections: Topic 25**

---

## 🟡 WHY — ArrayList Internals

→ ArrayList = **internally `Object[]` ARRAY**
→ **Index se access = O(1) FAST**
→ Add at end = **O(1) amortized**. Add at start/middle = **O(n) shift**
→ Full hone pe **1.5x grow + copy**
→ Use case: **random access = ArrayList**, frequent insert/remove middle = LinkedList

---

## 📖 STORY — Active Survey IDs

→ Manager ne bola — **active survey IDs maintain karo**
→ Array liya, size 100 diya. **101st ID aayi — crash**
→ Tab ArrayList — internally **dynamic array hai, khud grow karta hai**
→ Default size 10, full hua toh **1.5x naya array**. Index se access = **O(1) fast**
→ Beech mein insert/remove = **shift karna padta = O(n) slow**
→ **Read-heavy kaam ke liye best**
→ ArrayList andar se ek Array hi hai — bas **smart wrapper** hai uske upar

---

## 💻 Internal

```java
// Jab tu likhta: new ArrayList<>()
// Andar ye hota:
Object[] elementData = new Object[10];     // default size 10
size = 0;                                   // abhi koi element nahi
```

**WHY size 10?**
→ 10 elements ke baad 11th aaya → naya array banao **10 × 1.5 = 15**
→ Purana copy karo → 11th add karo
→ Costly operation isliye **agar pata ho: `new ArrayList<>(500)`** — resizing avoid

---

## 🧠 Visualization

```
                ArrayList Internal — Andar ka Sach

╔════════════════════════════════════════════════════════════╗
║ Initial — Default size 10                                  ║
╚════════════════════════════════════════════════════════════╝

ArrayList<Integer> list = new ArrayList<>();

Internal Object[] elementData (heap):
  ┌────┬────┬────┬────┬────┬────┬────┬────┬────┬────┐
  │null│null│null│null│null│null│null│null│null│null│   capacity = 10
  └────┴────┴────┴────┴────┴────┴────┴────┴────┴────┘
   0    1    2    3    4    5    6    7    8    9

   size = 0    (abhi koi element nahi)


╔════════════════════════════════════════════════════════════╗
║ list.add(5);  list.add(10);  list.add(20);                 ║
╚════════════════════════════════════════════════════════════╝

  ┌────┬────┬────┬────┬────┬────┬────┬────┬────┬────┐
  │ 5  │ 10 │ 20 │null│null│null│null│null│null│null│   capacity = 10
  └────┴────┴────┴────┴────┴────┴────┴────┴────┴────┘
   0    1    2    3    4    5    6    7    8    9

   size = 3

   list.get(2)  →  index direct access  →  20  →  O(1) ✅


╔════════════════════════════════════════════════════════════╗
║ Full hua — 11th element add karna hai                      ║
╚════════════════════════════════════════════════════════════╝

Step 1: Naya array banao (1.5x = 10 × 1.5 = 15)
  ┌────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┐
  │null│null│null│null│null│null│null│null│null│null│null│null│null│null│null│
  └────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴────┘
   0    1    2    3    4    5    6    7    8    9    10   11   12   13   14

Step 2: Purana copy karo
Step 3: 11th element add karo
        elementData = naya array (size 15)


╔════════════════════════════════════════════════════════════╗
║ Beech mein insert — O(n) shift                             ║
╚════════════════════════════════════════════════════════════╝

list.add(1, 99)  // index 1 pe daalo

Before:
  ┌────┬────┬────┬────┬────┬────┐
  │ 5  │ 10 │ 20 │ 30 │ 40 │null│
  └────┴────┴────┴────┴────┴────┘

Shift karna padta (saare elements 1 right):
  ┌────┬────┬────┬────┬────┬────┐
  │ 5  │ 99 │ 10 │ 20 │ 30 │ 40 │   ← 4 elements shift hue
  └────┴────┴────┴────┴────┴────┘
       ↑    →    →    →    →
       insert    shifted

  10000 elements? 10000 shifts = SLOW O(n)
```

---

## 📊 Time Complexity

| Operation | Time | Kyu? |
|-----------|------|------|
| `get(index)` | **O(1)** | Direct index access — instant |
| `add(end)` | **O(1)** | Last position pe daalo |
| `add(middle)` | **O(n)** | Shift karna padega |
| `remove(middle)` | **O(n)** | Shift karna padega |
| `search(value)` | **O(n)** | Ek-ek check |

---

## 🔴 ArrayList Kab Use?

> **✅ Zyada READ karna ho. ✅ End pe add/remove. ✅ Index se access.**
> **❌ Beech mein baar-baar insert/delete — slow.**

---

## 💬 POWER PHRASE

> *"ArrayList internally uses a dynamic array with default capacity 10, growing by 1.5x when full. `get()` is O(1) due to index-based access, but add/remove in the middle is O(n) due to shifting. Best for read-heavy operations."*
