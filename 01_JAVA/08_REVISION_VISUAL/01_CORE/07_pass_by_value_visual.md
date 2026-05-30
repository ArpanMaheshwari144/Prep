# Pass by Value — Visual Revision (Interview Classic)

---

## 1 THE Statement

```
   ┌──────────────────────────────────┐
   │  JAVA IS ALWAYS PASS BY VALUE    │
   └──────────────────────────────────┘
```

**Interview mein BARBAAR poocha.**
**Bahut log galat samjhte. Tu sahi samjhega.**

---

## 2 Photocopy Analogy

```
Imagine tere paas ek IMPORTANT PAPER hai.
Tera dost paper mein change karna chahta.

Option A: Original paper de do
   → Dost ne change kar diya
   → TERA paper bhi changed
   = PASS BY REFERENCE

Option B: PHOTOCOPY de do
   → Dost ne photocopy mein change kiya
   → TERA original SAFE
   = PASS BY VALUE
```

```
   JAVA = ALWAYS Option B (photocopy)
```

---

## 3 Primitive Pass by Value (Easy Case)

```java
void changeNumber(int x) {
    x = 100;
    System.out.println("Inside: " + x);  // 100
}

int num = 5;
changeNumber(num);
System.out.println("Outside: " + num);   // STILL 5!
```

### Visual
```
   Outside:                Inside method:
   num = 5                 x = 5  (COPY of num)
                              ↓
                           x = 100  (modified copy)
                              ↓
                           method ends, x destroyed

   num is STILL 5
   Original UNCHANGED
```

```
PRIMITIVE PASS:
   Actual VALUE copied
   Method gets COPY
   Original safe
```

---

## 4 OBJECT — The Confusion Begins

```java
class Person {
    String name;
}

void changeName(Person p) {
    p.name = "Suresh";   // modifies object
}

Person arpan = new Person();
arpan.name = "Arpan";

changeName(arpan);
System.out.println(arpan.name);   // "Suresh"  ← changed!
```

```
WAIT — name DID change!
"Yeh to pass by reference lag raha?"

NAHI. Read carefully.
```

---

## 5 Reality — Reference VALUE Pass

```
Java mein object pass karte time —
   OBJECT NAHI bheja jata
   REFERENCE (address) bheja jata

   AND woh REFERENCE value-by-value copied hota
```

### Visual
```
   HEAP (where object lives):
   ┌──────────────────────────┐
   │ Person object             │
   │ name: "Arpan"             │
   └──────────────────────────┘
        ↑
        │ memory address: 0x1234
        │
   Outside:                Inside method:
   arpan = 0x1234          p = 0x1234  (COPY of address)
                              ↓
                           p.name = "Suresh"
                              ↓
                           Goes to 0x1234 → modifies object
                              ↓
                           Both arpan and p point to same object
                           Object IS modified
```

```
KEY INSIGHT:
   p ki COPY hai arpan ki reference value (0x1234)
   Dono SAME object pe point karte
   Method modify kar sakta object ke fields
   BUT cannot reassign arpan itself
```

---

## 6 Proof — Reassignment Test

```java
void reassign(Person p) {
    p = new Person();         // create NEW object
    p.name = "NewGuy";
}

Person arpan = new Person();
arpan.name = "Arpan";

reassign(arpan);
System.out.println(arpan.name);   // STILL "Arpan"!
```

### Why?
```
   Outside arpan reference: 0x1234 (original)

   Inside method:
      p = 0x1234 (copy of address)
      p = new Person() → p NOW points to 0x5678
      p.name = "NewGuy" → modifies 0x5678 (new object)

   Method ends, p destroyed

   arpan STILL points to 0x1234 (original)
   Original "Arpan" name unchanged
```

```
   = PROOF Java is pass by VALUE
   = Cannot REASSIGN original reference
   = Can only modify object via the copy of reference
```

---

## 7 THE Trap (Most People Get Wrong)

```
GALAT belief:
   "Primitives = pass by value"
   "Objects = pass by reference"

SAHI:
   "Java is ALWAYS pass by value"
   "For primitives: VALUE copied"
   "For objects: REFERENCE VALUE (address) copied"
```

```
Konfusion ki wajah:
   Object modify ho jata method ke andar
   Lage "pass by reference"

   Lekin reassignment test = always fails
   = pass by value confirmed
```

---

## 8 Visual Summary

```
PRIMITIVE:
   int num = 5
        │
   ┌────┘
   ▼
   ┌─────┐  pass to method
   │  5  │ ───────────────► COPY (value 5)
   └─────┘                   ↓
   num                       method modifies copy
                              num original UNCHANGED

OBJECT:
   Person arpan
        │
        │ reference: 0x1234
        ▼
   ┌──────────────┐
   │ Person obj   │ ← lives in heap
   │ name: "..."  │
   └──────────────┘
        ↑
        │
   pass to method
        │
        ▼
   p (copy of 0x1234)
        ↓
   p.name = "X" → modifies object via address
   p = new() → reassigns LOCAL copy only
                original arpan reference UNCHANGED
```

---

## 9 Interview Power Phrase

```
Q: "Is Java pass by value or pass by reference?"

A: "Java is ALWAYS pass by value.
    For primitives, the actual value is copied.
    For objects, the REFERENCE value (address) is copied.

    Both copies point to the same object on heap, so
    modifications to the object are visible.

    However, reassigning the parameter inside the method
    does NOT affect the original — proof of pass by value."
```

---

## Memory Hooks

```
JAVA = always PHOTOCOPY (pass by value)

Primitive: copy of VALUE          (5 → 5)
Object:    copy of ADDRESS         (0x1234 → 0x1234)

   Both copies → same heap object
   Modify field = visible outside
   Reassign reference = local only
```

[← Back to JAVA](../)
