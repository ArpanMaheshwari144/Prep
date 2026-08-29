# fail-fast vs fail-safe — ConcurrentModificationException

> **V90 — Collections: Topic 32**

---

## WHY — Exception Throw Kyu Karta?

→ Tu list **padh raha** hai (iterator) aur koi beech mein **list badal raha** hai
→ Iterator ko **consistent view** chahiye
→ Agar data change ho gaya toh **galat result** ya `ArrayIndexOutOfBounds`
→ Safe rehne ke liye iterator `modCount` track karta hai — **badla?**
→ Turant roko — galat data dene se **acha exception** de do

---

## STORY

→ Scene: List **iterate** kar raha hai
→ Aur saath mein **`list.remove()` andar loop mein** — BOOM!
→ `ConcurrentModificationException`
→ ArrayList ka iterator ek **internal counter** rakhta — **`modCount`**
→ Jab bhi list **structurally modify** hoti (add ya remove dono), `modCount` badhta
→ Iterator start hote waqt `expectedModCount` set hota
→ Har step pe check: **`modCount` badla? EXCEPTION!**
→ Ye **fail-fast** hai — turant pakad lo, galat result mat do

---

## Code

### fail-fast — Galat Code
```java
List<String> list = new ArrayList<>();
list.add("Rahul");
list.add("Arpan");

for (String s : list) {
    list.remove(s);     // ConcurrentModificationException!
                        // modCount badh gaya — iterator ne pakad liya
}
```

### fail-fast — Sahi Fix (Iterator.remove())
```java
Iterator<String> it = list.iterator();
while (it.hasNext()) {
    String s = it.next();
    it.remove();        // Safe — modCount + expectedModCount dono sync
}
```

---

## TRAP — `list.remove()` ≠ `it.remove()`

> **`list.remove()`** = bahar se modify → modCount badha, expectedModCount nahi → **CME**
> **`it.remove()`** = dono ek saath update → safe

---

## fail-safe — CopyOnWriteArrayList

```java
CopyOnWriteArrayList<String> list = new CopyOnWriteArrayList<>();
list.add("Rahul");
list.add("Arpan");

for (String s : list) {
    list.remove(s);     // NO exception — copy pe iterate ho raha
}
```

→ **Tradeoff:** extra memory lagti hai (copy banane mein)
→ **Stale data:** loop mein add hua element us iteration mein nahi dikhega

---

## fail-fast vs fail-safe

| | fail-fast | fail-safe |
|--|----------|-----------|
| **Exception?** | Haan — CME | Nahi |
| **Collections** | ArrayList, HashMap | CopyOnWriteArrayList, ConcurrentHashMap |
| **Iterate over** | Original list | **Copy** of list |
| **Memory** | Less | Zyada (copy banata) |

---

## POWER PHRASE

> *"fail-fast iterators throw ConcurrentModificationException as soon as they detect a structural modification during iteration. fail-safe iterators work on a copy of the collection — no exception, but may iterate over stale data."*

> **Yaad rakh:**
> fail-fast = ArrayList, HashMap → CME on modify. `it.remove()` = safe.
> fail-safe = CopyOnWriteArrayList, ConcurrentHashMap → copy banata, no CME.
> `it.remove()` = safe. `list.remove()` inside loop = CME.

---

## Deep — Exception Aati Kyun? `modCount` ka Pura Khel

### Iterator ka Kaam — Photographer Wala

Iterator ek **photographer** hai. Tu bola "list ek-ek karke dikha" → photographer ne **snapshot** li.

Beech mein koi list modify kar de → snapshot **invalid** → galat result aayega.

Java ka solution: **"Snapshot toot gayi → exception, galat data mat do."**

### `modCount` — ArrayList ka Internal Counter

```java
public class ArrayList<E> {
    private Object[] elementData;
    private int size;
    protected transient int modCount = 0;    // ← ye internal counter
}
```

→ Jab bhi list **STRUCTURALLY modify** ho, ye **+1**
→ Structural modification = `add()`, `remove()`, `clear()`, `addAll()`
→ **`set()` ya `get()` modCount nahi badhata** — size change nahi hota

```java
List<String> list = new ArrayList<>();
list.add("Rahul");      // modCount: 0 → 1
list.add("Arpan");      // modCount: 1 → 2
list.remove("Rahul");   // modCount: 2 → 3
list.set(0, "Priya");   // modCount: 3  (set NAHI badhata)
```

### `expectedModCount` — Iterator ka Snapshot

Jab `list.iterator()` call karta hai, iterator **us waqt ka modCount** save kar leta:

```java
private class Itr implements Iterator<E> {
    int cursor;
    int expectedModCount = modCount;   // ← snapshot
}
```

`modCount = 5` jab iterator bana → `expectedModCount = 5` lock ho gaya.

### Har Step Pe Check (`checkForComodification`)

`it.next()` call hote hi internally:

```java
final void checkForComodification() {
    if (modCount != expectedModCount)
        throw new ConcurrentModificationException();
}
```

**Translation:**
- "Snapshot meri **5** thi"
- "Abhi list ka modCount **6** hai"
- "Beech mein kisi ne **mujhe bina bataye** modify kiya"
- "BOOM — exception"

### `it.remove()` Safe Kyu Hai?

**`list.remove(s)`** — list ka modCount badha, iterator ko **nahi bataya** → mismatch → CME

**`it.remove()`** — iterator **dono ek saath** update karta:

```java
public void remove() {
    // ... element hata
    modCount++;
    expectedModCount = modCount;    // ← apna bhi update
}
```

Dono sync → koi mismatch nahi → koi CME nahi.

### Visual Flow

```
list.add("Rahul")  → modCount = 1
list.add("Arpan")  → modCount = 2

Iterator banaya:
   expectedModCount = 2  (snapshot)

Loop:
  it.next()              → modCount(2) == expectedModCount(2)
  list.remove("Rahul")   → modCount = 3  (BAHAR SE BADLA)
  it.next()              → modCount(3) ≠ expectedModCount(2)  CME
```

Ab `it.remove()` use kiya:

```
  it.remove()   → modCount = 3, expectedModCount = 3   (DONO update)
  it.next()     → modCount(3) == expectedModCount(3)   pass
```

### Bottom Line

| Field | Kya hai |
|-------|---------|
| **`modCount`** | List ka apna counter (kitni baar modify hui) |
| **`expectedModCount`** | Iterator ka snapshot (jab bana tab kitni thi) |
| **Mismatch** | Iterator ko bina bataye list badli → CME |

**Production tip:** Multi-thread mein same mechanism — 2 threads ek hi list pe, ek modify kare → doosre ka iterator CME deta. Wahan **`CopyOnWriteArrayList`** ya **`ConcurrentHashMap`** use karo.
