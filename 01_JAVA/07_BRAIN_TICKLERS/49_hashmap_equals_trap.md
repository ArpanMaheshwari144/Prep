# HashMap mein `equals()` Override Nahi Kiya — Duplicate Ghus Gayi

> **V90 — Brain Ticklers: Topic 49**

---

## STORY

→ Custom class ko **HashMap key** banaya. `hashCode()` override kiya, **`equals()` nahi**
→ Same data ke 2 objects daale — **duplicate entry ban gayi!**
→ HashMap ne `hashCode` se same bucket mein dala, **phir `equals()` check kiya**
→ `Object.equals()` = **reference check** — alag objects, dono store ho gaye

```java
// hashCode() override kiya, equals() NAHI
class Employee { String name; int id; }    // sirf hashCode override

map.put(new Employee("Arpan", 101), "Dev");   // bucket 5
map.put(new Employee("Arpan", 101), "Dev");   // bucket 5 — equals() false (reference) — DONO STORE!

// FIX — hashCode + equals DONO override
// hashCode same → same bucket
// equals true → REPLACE (duplicate rejected)
```

---

## Visualization — Bucket Trap

```
            HashMap equals() Override Nahi Kiya — Trap

╔════════════════════════════════════════════════════════════╗
║ Setup — Custom Class with hashCode() but NO equals()       ║
╚════════════════════════════════════════════════════════════╝

  class Employee {
      String name;
      int id;
      // hashCode() override kiya, equals() NAHI
      @Override int hashCode() { return id; }
  }

  Employee e1 = new Employee("Arpan", 101);
  Employee e2 = new Employee("Arpan", 101);

  e1.hashCode()  →  101
  e2.hashCode()  →  101  (same!)

  e1 == e2       →  false (alag objects)
  e1.equals(e2)  →  false (default = reference check)


╔════════════════════════════════════════════════════════════╗
║ map.put(e1, "Dev"); map.put(e2, "Dev");                    ║
╚════════════════════════════════════════════════════════════╝

  Step 1: e1 ka hashCode = 101 → bucket index = 5
  Step 2: bucket 5 empty → store

  ┌────┐
  │ 0  │
  ├────┤
  │ 5  │ → [e1: "Dev"]   ← store ho gaya
  ├────┤
  │ 9  │
  └────┘


╔════════════════════════════════════════════════════════════╗
║ Step 3: e2 ka hashCode = 101 → bucket index = 5 (SAME)     ║
╚════════════════════════════════════════════════════════════╝

  Bucket 5 mein collision → e2 ko e1 se compare karna hai

  e1.equals(e2) check karta:
       │
       ▼
  default Object.equals() = REFERENCE check
       │
       ▼
  e1 ≠ e2 (alag objects)  →  EQUALS returns FALSE
       │
       ▼
  HashMap thinks "alag entries hain, dono store karo!"
       │
       ▼
  ┌────┐
  │ 5  │ → [e1: "Dev"] → [e2: "Dev"]   DONO store! Duplicate!
  └────┘

  map.size() = 2  (chahiye tha 1)


╔════════════════════════════════════════════════════════════╗
║ Fix — equals() bhi Override                                ║
╚════════════════════════════════════════════════════════════╝

  @Override
  public boolean equals(Object o) {
      Employee other = (Employee) o;
      return this.id == other.id;
  }

  Ab map.put(e2, "Dev") karte:

  hashCode same → bucket 5
  equals true → REPLACE, naya entry NAHI

  ┌────┐
  │ 5  │ → [e1: "Dev"]   ← e2 ne replace kiya
  └────┘

  map.size() = 1
```

---

## WHY DONO Chahiye?

→ **`hashCode`** = kaunsa bucket (address)
→ **`equals`** = same hai ya nahi (identity)
→ Sirf hashCode = same bucket but identity check fail → dono **alag maan liye** → duplicate store

> **Rule:** `equals()` override karo toh `hashCode()` **ZAROOR** override karo — Java ka contract.

---

## Deep — "Reference Check" Ka Matlab

### `Object.equals()` Default Source Code

`equals()` method **har class mein hota** kyunki har class `Object` ko extend karti.

**Object class ka default `equals()`:**

```java
// Java ne Object class mein literally ye likha
public boolean equals(Object obj) {
    return (this == obj);    // ← LITERALLY == hi return karta hai!
}
```

**Default `equals()` andar se kuch kaam nahi karta — sirf `==` return kar deta.**
Matlab default `equals()` aur `==` **SAME kaam** karte.

---

### Reference Check vs Content Check — Visualization

```java
Employee e1 = new Employee("Arpan", 101);
Employee e2 = new Employee("Arpan", 101);
```

```
STACK              HEAP
                   ┌────────────────────┐
e1  ──────────────►│ Employee Object 1  │   address: 0x100
                   │ name="Arpan"       │
                   │ id=101             │
                   └────────────────────┘

                   ┌────────────────────┐
e2  ──────────────►│ Employee Object 2  │   address: 0x200
                   │ name="Arpan"       │
                   │ id=101             │
                   └────────────────────┘

CONTENT same hai (Arpan, 101 dono mein)
LEKIN heap mein 2 ALAG objects hain (alag addresses)
```

---

### REFERENCE CHECK (== aur default equals())

```
  e1 == e2?
    ↓
  e1 ka address (0x100)  ==  e2 ka address (0x200)?
    ↓
  0x100 == 0x200?  →  FALSE

  "Tu jis object pe point kar raha, mei bhi USI pe point kar raha?"
  Answer: NAHI — alag-alag objects pe.
```

### CONTENT CHECK (overridden equals())

```
  e1.equals(e2)?  (assuming equals() override kiya hai)
    ↓
  e1.id == e2.id  AND  e1.name.equals(e2.name)?
    ↓
  101 == 101  AND  "Arpan".equals("Arpan")?  →  TRUE

  "Tera content aur mera content same hai?"
  Answer: HAAN — dono Arpan, 101.
```

---

### Asli Confusion

Log sochte: **"`equals()` matlab content check hota hi hai."** — **GALAT.**

Sahi: **"`equals()` content check TAB karta jab class ne override kiya. Default mein toh `==` jaisa hi hai."**

| Class | `equals()` override? | Behavior |
|-------|---------------------|----------|
| **String** | YES | Content check (chars compare) |
| **Integer / Wrapper** | YES | Value check |
| **ArrayList** | YES | Element-wise content check |
| **Tera custom Employee** | NO (default) | Reference check (== ke barabar) |

```java
// Custom class — no override
e1.equals(e2)  ≡  e1 == e2     // dono FALSE (reference check)

// String class — override hua hai
"Hello".equals("Hello")        // TRUE (content check)
```

---

### 1-Line Yaad

> **Reference check = memory address compare. SAME object pe point kar rahe? Content same ya alag — fark nahi padta.**
> **Default `Object.equals()` = `==` ke barabar = reference check.**

---

## POWER PHRASE

> *"For custom objects used as HashMap keys, both `hashCode()` and `equals()` must be overridden — without `equals()`, two objects in the same bucket are treated as distinct via reference check, causing duplicate entries."*
