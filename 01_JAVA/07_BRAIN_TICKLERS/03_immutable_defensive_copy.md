# Immutable Class Toot Gayi — Defensive Copy Nahi Kiya

> **V90 — Brain Ticklers: Topic 50**

---

## STORY

→ `final` class banayi, fields `private final` — **immutable smajh ke khush**
→ Andar ek `List<String>` field thi
→ Caller ne **bahar wali list mein add kiya** — andar wali bhi badal gayi!
→ "Immutable" toot gayi — kyunki **List ka REFERENCE same** hai

```java
final class Student {
    private final List<String> courses;
    Student(List<String> c) { this.courses = c; }              // SAME reference!
    List<String> getCourses() { return courses; }              // SAME reference!
}

List<String> list = new ArrayList<>();
list.add("Java");
Student s = new Student(list);
list.add("Python");                                             // bahar se andar BADLA!
s.getCourses().size();    // 2 — Immutable TOOT GAYI!
```

---

## Step-by-Step — Memory Mein Kya Hota

### Step 1: List banayi aur "Java" add kiya

```java
List<String> list = new ArrayList<>();
list.add("Java");
```

```
STACK                    HEAP
                         ┌─────────────┐
list  ──────────────────►│  ["Java"]   │
                         └─────────────┘
```

`list` heap mein ArrayList object pe **point** kar raha.

---

### Step 2: `Student s = new Student(list)`

Constructor ke andar: `this.courses = c;`
**Critical: `this.courses` aur `c` (parameter) DONO SAME object pe point karte!**

```java
Student(List<String> c) {
    this.courses = c;        // SAME reference copy hua
}
```

```
STACK                    HEAP
                         ┌─────────────┐
list      ───────────────┤  ["Java"]   │
                         │  ◄──────────┤── courses
s  ──────► [Student]     │             │   (Student ka field)
            courses ─────┘             │
                         └─────────────┘
        DONO SAME object pe point!
```

**`final` ne sirf itna lock kiya — `courses` ka REFERENCE reassign nahi ho sakta.** Lekin **us reference se andar ka content modify ho sakta!**

---

### Step 3: `list.add("Python")`

`list` reference se ArrayList ke andar `"Python"` add kar diya:

```
STACK                    HEAP
                         ┌──────────────────────┐
list      ───────────────┤  ["Java", "Python"]  │
                         │  ◄───────────────────┤── courses
s  ──────► [Student]     │                      │
            courses ─────┘                      │
                         └──────────────────────┘

         "Python" add hua — DONO ko dikha!
         (kyunki dono SAME object dekh rahe hain)
```

---

### Step 4: `s.getCourses().size()` → `2`

Student ne **kabhi `add()` nahi call kiya**. Lekin uska andar wala list 2 elements ka ho gaya — kyunki **bahar wala caller** us same object ko modify kar raha tha.

---

## Asli Galti Kahan Hui?

Constructor mein:
```java
this.courses = c;     // caller ki list ka reference store kar liya
```

Caller ki list aur Student ki list **EK HI OBJECT** hai memory mein. Caller `list.add()` karega → Student's "immutable" state badal jayegi.

---

## Fix — Defensive Copy

```java
Student(List<String> c) {
    this.courses = new ArrayList<>(c);    // NAYI list, contents copy
}
List<String> getCourses() {
    return new ArrayList<>(courses);       // copy return karo
}
```

```
STACK                    HEAP
                         ┌─────────────┐
list  ──────────────────►│  ["Java"]   │   ← caller ki original
                         └─────────────┘

s ──► [Student]          ┌─────────────┐
       courses ─────────►│  ["Java"]   │   ← Student ki APNI copy
                         └─────────────┘

       Ab dono ALAG objects — caller modify kare, Student safe.
```

---

## Bottom Line

> **`final` = reference reassign nahi hota.**
> **`final` ≠ object ka content modify nahi hota.**

Mutable field (List, Set, Map) ka reference share kiya = "immutable" actually mutable hai bahar ke caller ke through.

**Defensive copy = naya object banao, reference share mat karo.**

---

## WHY String Field Mein Ye Problem Nahi?

→ String **khud immutable** hai — `s.add()` jaisi koi method nahi
→ Reference share ho bhi gaya, content modify hi nahi hota
→ Lekin **List/Set/Map mutable** hai — `add/remove` se content badal sakta
→ Isliye **mutable fields ke liye defensive copy** ZAROOR

---

## POWER PHRASE

> *"For immutable classes with mutable fields like List, you must defensive-copy in both the constructor and getter — otherwise the external reference can mutate the internal state. `final` only prevents reference reassignment, not content modification."*
