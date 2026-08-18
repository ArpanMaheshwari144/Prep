# final keyword — 3 Jagah Use Hota Hai

> **V90 — Core Java Extras: Topic 12**

---

## STORY — PI ki Value

→ Tune `PI` ki value store kiya — `3.14159`
→ Koi aur developer ne **accidentally PI = 3** kar diya. **Bug.**
→ Tune **config values** define kiye → koi service ne override kar diya production mein
→ **`final` = lock laga do** → ye nahi badlega
→ **Variable pe, Method pe, Class pe** — teeno jagah same intent: ye change nahi hona chahiye
→ `final` variable → **reassign nahi**
→ `final` method → **override nahi**
→ `final` class → **extend nahi**
→ **Teeno ka ek hi matlab: change nahi hoga**

---

## Code

```java
final int x = 10;
x = 20;                              // compile error — value change nahi hogi

final class String { }
class MyString extends String { }    // compile error — extend nahi ho sakti

final void show() { }
// child override nahi kar sakta
```

---

## TRAP

> **`final` variable ka matlab sirf `reassign nahi` — object ke ANDAR ke fields change ho sakte hain!**

```java
final List<String> list = new ArrayList<>();
list.add("Arpan");                   // allowed — andar change kiya
list = new ArrayList<>();            // reference change nahi hoga
```

**WHY andar badal sakta? `final` = LABEL lock hai, DABBA nahi.**

→ Jaise ghar ka **address final** — ghar nahi badlega, lekin **andar furniture rearrange** kar sakte ho
→ `list = new ArrayList()` = ghar badalna = NAHI
→ `list.add()` = andar furniture = HAAN

---

## Visualization

```
                   final = LABEL lock, DABBA lock NAHI

STACK                        HEAP
┌──────────────┐             ┌─────────────────────────┐
│ list (final) ├────────────►│ ArrayList               │
│ LOCKED    │             │ ["Arpan"]               │  ← content modify OK
└──────────────┘             │ list.add("Rahul")    │
                             │ ["Arpan", "Rahul"]      │
                             └─────────────────────────┘

╔════════════════════════════════════════════════════════════╗
║ list = new ArrayList<>();   →  BLOCKED — reference lock ║
║ list.add("Rahul");          →  ALLOWED — content change ║
╚════════════════════════════════════════════════════════════╝

Ghar ka address final = ghar nahi badlega (reference)
                       Andar furniture rearrange ho sakta (content)
```

---

## POWER PHRASE

> *"`final` on a variable prevents reassignment. `final` on a class prevents inheritance. `final` on a method prevents overriding — String class is final which is part of why it is immutable."*
