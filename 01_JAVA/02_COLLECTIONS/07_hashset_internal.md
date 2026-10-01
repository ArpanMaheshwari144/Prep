# HashSet — Andar Se Samjho

> **V90 — Collections: Topic 29**

---

## STORY — Duplicate Emails

→ 10,000 panelists — kuch ne **duplicate emails register** kiye
→ ArrayList mein add karo — **duplicate aa jaata**
→ **HashSet liya — duplicate automatically reject**
→ **Kyu?** Andar se HashSet **HashMap hi hai** — tera element key ban jaata hai, value ek dummy object
→ Same key dobara? **Naya entry nahi banta** — purani KEY wahi rehti, sirf dummy value (PRESENT -> PRESENT) likhi jaati, `add()` **false** lautata. **Isliye duplicate impossible**

---

## WHY — HashMap Reuse Kyu Kiya?

→ Duplicate check ke liye **`hashCode()` + `equals()` chahiye**
→ HashMap **already ye karta hai** keys ke liye
→ Naya data structure banane ki **zarurat hi nahi**
→ Clever reuse — `set.add("X")` = `map.put("X", PRESENT)`
→ Same key dobara? **KEY replace NAHI hoti** (pehla object hi rehta) — naya entry nahi → **duplicate rejected**, `add()` = false

---

## Internal — HashSet Source Code

```java
// HashSet andar se ye hai:
private HashMap<E, Object> map;
private static final Object PRESENT = new Object();   // dummy value

public boolean add(E e) {
    return map.put(e, PRESENT) == null;               // element = key, value = dummy
}
```

```java
Set<String> emails = new HashSet<>();
emails.add("arpan@gmail.com");
emails.add("rahul@gmail.com");
emails.add("arpan@gmail.com");                        // duplicate!

System.out.println(emails);
// [arpan@gmail.com, rahul@gmail.com] — duplicate gone
```

---

## Visualization

```
HashSet = HashMap Ki Keys (Dummy Value)

  set.add("arpan@gmail.com");
  set.add("rahul@gmail.com");
  set.add("arpan@gmail.com");    ← duplicate!

  Internally → HashMap mein store:

  ┌──────────────────┬────────────┐
  │  KEY             │  VALUE     │
  ├──────────────────┼────────────┤
  │ "arpan@gmail.com"│  PRESENT   │   ← dummy
  ├──────────────────┼────────────┤
  │ "rahul@gmail.com"│  PRESENT   │   ← dummy
  └──────────────────┴────────────┘

  3rd add → "arpan@gmail.com" pehle se hai (key match)
          → naya entry NAHI bana. Purani KEY wahi rahi, sirf value PRESENT->PRESENT
          → map.put ne purani value lautayi (null nahi) -> add() = FALSE
          → Duplicate avoid


╔════════════════════════════════════════════════════════════╗
║ Custom Class Trap — hashCode + equals BOTH                 ║
╚════════════════════════════════════════════════════════════╝

  set.add(new Panelist("arpan@gmail.com"));
  set.add(new Panelist("arpan@gmail.com"));    ← same email, alag object

  Bina hashCode() override:
  ┌────┐
  │ 5  │ → [obj1: arpan]    ← hashCode = 12345
  ├────┤
  │ 8  │ → [obj2: arpan]    ← hashCode = 67890   ← ALAG bucket!
  └────┘
  size = 2  (duplicate ghus gayi)


  Sahi — hashCode() + equals() DONO override:
  ┌────┐
  │ 5  │ → [obj1: arpan]    ← hashCode same
  └────┘                       equals true → obj2 ANDAR NAHI GAYA, obj1 hi raha
  size = 1

  ★ Chala ke dekha (1-Oct):  add(PEHLA) = true · add(DOOSRA, same email) = false · set = [PEHLA]
    -> set PEHLA object hi rakhta hai. Doosre ka "name" badla tha to wo badlaav set me NAHI aaya.
```

---

## Operations

| Method | Time | Kaise? |
|--------|------|--------|
| `add(e)` | O(1) | hashCode → bucket → store |
| `contains(e)` | O(1) | hashCode → bucket → check |
| `remove(e)` | O(1) | hashCode → bucket → delete |
| **Order** | No order | HashMap jaisa — koi guarantee nahi |

---

## TRAP — Custom Class: hashCode() + equals() DONO Override

```java
// GALAT — sirf class likhi, kuch override nahi
set.add(new Panelist("arpan@gmail.com"));
set.add(new Panelist("arpan@gmail.com"));
// set.size() = 2 — duplicate aaya! Alag objects, alag hashCode, alag bucket.

// FIX — DONO override karo
@Override
public int hashCode() { return email.hashCode(); }    // same email = same bucket

@Override
public boolean equals(Object o) {
    Panelist p = (Panelist) o;
    return this.email.equals(p.email);                 // same email = equal
}
// ab set.size() = 1
```

> **TRAP 1:** Sirf `equals()` override kiya, `hashCode()` nahi → alag hashCode → alag bucket → `equals()` kabhi call hi nahi hoga → duplicate aa jayega.
> **RULE:** equals() override → hashCode() **ZAROOR** override. Java ka contract hai.

> **TRAP 2:** HashSet order **guarantee nahi**.
> Order chahiye → **LinkedHashSet**.
> Sorted chahiye → **TreeSet**.

---

## POWER PHRASE

> *"HashSet internally uses a HashMap — each element becomes a key with a dummy value. Duplicates are prevented because put on an existing key keeps the original key and only overwrites the dummy value, so add returns false. For custom objects, both `hashCode()` and `equals()` must be overridden — without `hashCode()`, two equal objects may land in different buckets and both get stored."*

> **Yaad rakh:**
> HashSet = HashMap ki keys
> Duplicate nahi kyunki same key pe naya entry banta hi nahi (purana object rehta, add = false)
> Custom class mein `hashCode() + equals()` dono override
