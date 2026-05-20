# Iterator vs ListIterator

> **V90 — Collections: Topic 27**

---

## STORY — Loop Mein Remove

→ Loop mein **expired surveys remove** karne the. forEach mein `list.remove()` call kiya
→ **`ConcurrentModificationException`. Crash.**
→ Kyu? Loop chal raha hai aur tu saath mein **structure modify** kar raha hai. Java allow nahi karta
→ **Iterator se karo** — `it.remove()` safe hai kyunki Iterator internally `modCount` sync karta
→ **ListIterator** = Iterator ka **bada bhai** — aage bhi, peeche bhi, add bhi, set bhi

---

## Code — Loop Mein Safe Remove

### GALAT — list.remove() in forEach
```java
for (String s : surveys) {
    if (s.equals("S102")) {
        surveys.remove(s);                    // ConcurrentModificationException!
    }
}
```

### SAHI — Iterator.remove()
```java
Iterator<String> it = surveys.iterator();
while (it.hasNext()) {
    String s = it.next();
    if (s.equals("S102")) {
        it.remove();                          // SAFE — Iterator ka remove
    }
}
```

> **`it.remove()` = loop ke saath safe. `list.remove()` = exception.**

---

## Visualization — Pointer Position

```
              Iterator vs ListIterator — Pointer Visualization

╔════════════════════════════════════════════════════════════╗
║ Iterator — sirf FORWARD ja sakta                           ║
╚════════════════════════════════════════════════════════════╝

List: [S100, S101, S102, S103]

         ▼ (start: pointer 0 ke pehle)
       ┌────┬────┬────┬────┐
       │S100│S101│S102│S103│
       └────┴────┴────┴────┘

it.next()  →  pointer aage, S100 return
              ┌────┬────┬────┬────┐
              │S100│S101│S102│S103│
              └────┴────┴────┴────┘
                    ▲

it.next()  →  pointer aage, S101 return
              ┌────┬────┬────┬────┐
              │S100│S101│S102│S103│
              └────┴────┴────┴────┘
                         ▲

it.remove() →  abhi jo pointer pe hai (S101) hata do — SAFE
              ┌────┬────┬────┐
              │S100│S102│S103│
              └────┴────┴────┘

  peeche jaa nahi sakta — sirf forward


╔════════════════════════════════════════════════════════════╗
║ ListIterator — DONO direction                              ║
╚════════════════════════════════════════════════════════════╝

         pointer
         ▼
       ┌────┬────┬────┬────┐
       │S100│S101│S102│S103│
       └────┴────┴────┴────┘

  it.next()       →  S100 return  ▼
                                 ┌────┬────┬────┬────┐
                                 │S100│S101│S102│S103│
                                 └────┴────┴────┴────┘
                                       ▲

  it.previous()   →  peeche jao  ◄
                                 ┌────┬────┬────┬────┐
                                 │S100│S101│S102│S103│
                                 └────┴────┴────┴────┘
                                  ▲

  it.add(S105)    →  current position pe naya daalo
                                 ┌────┬────┬────┬────┬────┐
                                 │S105│S100│S101│S102│S103│
                                 └────┴────┴────┴────┴────┘
                                  ▲

  it.set(S99)     →  current element replace karo


╔════════════════════════════════════════════════════════════╗
║ ConcurrentModificationException — modCount mismatch        ║
╚════════════════════════════════════════════════════════════╝

Iterator banaya → expectedModCount = 3 (snapshot)

  list.remove(...)  → modCount badha → 4
                       expectedModCount = 3 (purana)
                       MISMATCH → exception

  it.remove()       → modCount badha → 4
                       expectedModCount BHI → 4 (sync)
                       SAFE 
```

---

## Iterator — 3 Methods

| Method | Kya karta? |
|--------|-----------|
| `hasNext()` | Aage kuch hai? → true/false |
| `next()` | Agla element lo + pointer aage badho |
| `remove()` | Abhi jo `next()` se liya, usse hata do |

> **Direction:** sirf **FORWARD** — ek hi taraf

---

## ListIterator — Iterator ka Bada Bhai

→ Iterator sirf forward ja sakta hai
→ **Suppose** S103 pe ho, wapas S102 pe jaana hai — Iterator se nahi hoga
→ **ListIterator** = dono direction: aage bhi, peeche bhi. Extra methods:

| Method | Kya karta? |
|--------|-----------|
| `hasPrevious()` | Peeche kuch hai? → true/false |
| `previous()` | Pichla element lo + peeche jao |
| `add(e)` | Current position pe naya element daalo |
| `set(e)` | Current element replace karo |

---

## Iterator vs ListIterator

| Feature | Iterator | ListIterator |
|---------|----------|--------------|
| **Direction** | Forward only | Forward + Backward |
| **add()** | Not available | Available |
| **set()** | Not available | Available |
| **Works with** | Any Collection | **Only List** |

---

## TRAP 1

> **`forEach` loop mein `list.remove()` = ConcurrentModificationException.**
> **`it.remove()` = SAFE. Ye classic interview question hai.**

## TRAP 2

> **`it.next()` bina `hasNext()` check kiye = `NoSuchElementException`.**
> **HAMESHA `while(it.hasNext())` pehle check karo.**

---

## Shortcut

> **Remove only → Iterator kaafi.**
> **Add/Set bhi chahiye → ListIterator must.**

---

## POWER PHRASE

> *"Iterator allows safe traversal and removal during iteration — calling `list.remove()` inside a loop throws ConcurrentModificationException, but `it.remove()` is safe. ListIterator extends Iterator with bidirectional traversal, `add()`, and `set()` — but only works with List."*
