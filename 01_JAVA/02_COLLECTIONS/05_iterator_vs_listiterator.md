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

★★ **TRAP — upar wala GALAT example is list pe exception DETA HI NAHI:**
```
surveys = [S100, S101, S102, S103]     S102 = aakhri se PEHLA (index 2)

S100 -> S101 -> S102 mila -> list.remove -> size 4->3
cursor = 3, size = 3  ->  hasNext() = (cursor != size) = FALSE  ->  loop KHATAM
next() dobara bula hi nahi -> modCount check hua hi nahi -> NO exception
S103 kabhi dekha hi nahi gaya  ->  CHUPCHAAP BUG
```
Exception tab aata hai jab hatane ke baad `next()` dobara chale. **Chala ke dekha (1-Oct):**
```
remove S100 -> CME                       (pehla)
remove S101 -> CME
remove S102 -> NO exception, S103 CHHOOTA (aakhri se PEHLA — cursor == size)
remove S103 -> CME                       (aakhri: cursor 4, size 3 -> hasNext true -> next() -> CME)
```
Isliye "exception nahi aaya = code sahi" GALAT hai.

**Java 8+ ka seedha tareeka:** `surveys.removeIf(s -> s.equals("S102"));` — andar se iterator hi, loop likhne ki zarurat nahi.

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

  it.set(S99)     →  aakhri next()/previous() wala element replace
                     ★ yahan (add ke turant baad) set = IllegalStateException.
                       pehle next() ya previous() chalao, phir set.


╔════════════════════════════════════════════════════════════╗
║ ConcurrentModificationException — modCount mismatch        ║
╚════════════════════════════════════════════════════════════╝

4 add hue → modCount = 4  (har badlaav pe +1, ye size NAHI hai)
Iterator banaya → expectedModCount = 4 (copy)

  list.remove(...)  → modCount badha → 5
                       expectedModCount = 4 (purana)
                       agle next() pe MISMATCH → exception

  it.remove()       → modCount badha → 5
                       expectedModCount BHI → 5 (sync)
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
| `add(e)` | cursor ki jagah naya daalo (agla `next()` purana element hi dega) |
| `set(e)` | jo AAKHRI baar `next()`/`previous()` se mila, use badlo |

★ **Cursor element PE nahi, do element ke BEECH hota hai.** `remove()` aur `set()` "abhi wala" nahi, **aakhri lautaya hua** element chhedte hain. Isliye `next()` ke bina `remove()` ya `add()` ke turant baad `set()`/`remove()` = **IllegalStateException**.

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
