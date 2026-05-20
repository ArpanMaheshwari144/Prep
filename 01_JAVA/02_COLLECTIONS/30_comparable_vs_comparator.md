# Comparable vs Comparator

> **V90 — Collections: Topic 30**

---

## STORY — Sort Karne Ki Zarurat

→ Panelists ki list **age se sort** karni thi. `Collections.sort()` kiya — **error**
→ Panelist class mein `compareTo()` nahi tha
→ **Comparable** = class ke **andar** ek natural ordering define karo
→ Lekin kal **naam se sort** karna tha, **parso rating se** — ek hi `compareTo()` nahi chalega
→ **Comparator** = bahar se **alag-alag rules** banao. Class touch nahi karni
→ **Comparable** = "main age se sort hounga" (class ka apna rule)
→ **Comparator** = "isko salary se sort karo" (Manager ka rule, bahar se)

---

## Visualization

```
COMPARABLE                          COMPARATOR
(Natural Order)                     (Custom Order)

class Employee implements           class SalaryComparator implements
        Comparable                          Comparator

  compareTo(Employee o)               compare(Employee a, Employee b)
  return this.age - o.age             return a.salary - b.salary

Class ke ANDAR define                ALAG class mein define
java.lang.Comparable                 java.util.Comparator
EK default sorting                   MULTIPLE sorting options
Collections.sort(list)               list.sort(new SalaryComp())
```

---

## Code

### 1. Comparable — Class Ke Andar
```java
class Panelist implements Comparable<Panelist> {
    String name;
    int age;

    @Override
    public int compareTo(Panelist other) {
        return Integer.compare(this.age, other.age);   // best practice
    }
}

List<Panelist> list = new ArrayList<>();
Collections.sort(list);                                 // compareTo() khud use hoga
```

**`compareTo()` return:**
- Negative → `this` pehle
- Zero → equal
- Positive → `other` pehle

### 2. Comparator — Class Ke Bahar
```java
// Rule 1 — Age se sort
Comparator<Panelist> byAge = (a, b) -> Integer.compare(a.age, b.age);

// Rule 2 — Name se sort
Comparator<Panelist> byName = (a, b) -> a.name.compareTo(b.name);

// Use:
Collections.sort(list, byAge);
Collections.sort(list, byName);
```

---

## TRAP 1 — `this.age - other.age` ≠ `Integer.compare()`

```java
return this.age - other.age;        // OVERFLOW risk!
// age = Integer.MAX_VALUE, other = -1 → subtraction overflow

return Integer.compare(this.age, other.age);   // SAFE — best practice
```

> **HAMESHA `Integer.compare()` use karo. Interview mein yahi best practice.**

---

## Comparable vs Comparator

| Feature | Comparable | Comparator |
|---------|-----------|-----------|
| **Kahan?** | Class ke **andar** | Class ke **bahar** |
| **Method** | `compareTo(other)` | `compare(a, b)` |
| **Kitne rules?** | Sirf **EK** — default sorting | **MULTIPLE** |
| **Class change?** | Haan — implement karo | Nahi — untouched |
| **Sort kaise?** | `Collections.sort(list)` | `Collections.sort(list, rule)` |

---

## POWER PHRASE

> *"Comparable defines natural ordering within the class via `compareTo()`. Comparator defines external, flexible ordering via `compare()` — you can have multiple Comparators for the same class. Use `Integer.compare()` instead of subtraction to avoid overflow."*

> **Yaad rakh:**
> Comparable = class andar, ek rule, `compareTo()`
> Comparator = class bahar, multiple rules, `compare()`
> `Integer.compare()` = overflow safe, best practice

---

## Confusion Clear — `compare()` vs `compareTo()` Dono Kyu Aate?

**Pakdi gayi confusion (interview ka classic):**

```java
Comparator<Panelist> byAge  = (a, b) -> Integer.compare(a.age, b.age);
Comparator<Panelist> byName = (a, b) -> a.name.compareTo(b.name);
//                                              ↑
//                                  ye compareTo kahan se aaya?
```

**Asli baat:** `Comparator` interface ne sirf ek demand ki — tu `compare(a, b)` likh aur int return kar. **Beech mein kya tool use karta — tera choice.**

### Compare karne ke 3 tools:

| Tool | Kab use? |
|------|---------|
| **`Integer.compare(x, y)`** | int compare karna ho — **safest** (overflow nahi) |
| **`x.compareTo(y)`** | String, Integer, Double pe — kyunki Java ne ye sab pehle se **Comparable** banaye hain |
| **`x - y`** | AVOID — overflow risk |

### Code Breakdown:

```java
// byAge — int compare kar raha
Comparator<Panelist> byAge = (a, b) -> Integer.compare(a.age, b.age);
//                            ↑                  ↑
//              Comparator override      tool: Integer.compare (int safe)


// byName — String compare kar raha
Comparator<Panelist> byName = (a, b) -> a.name.compareTo(b.name);
//                            ↑                  ↑
//              Comparator override      tool: String.compareTo()
//                                       (String pehle se Comparable hai)
```

### Saaf Rule:

> **`Comparator` ka `compare(a, b)` = jo TU likh raha (override).**
> **Andar `compareTo()` ya `Integer.compare()` = compare karne ka TOOL.**
> Dono alag cheezein hain — ek interface method, doosra utility.

### Quick reference:

```java
// int field
(a, b) -> Integer.compare(a.x, b.x)

// String field
(a, b) -> a.name.compareTo(b.name)

// double field
(a, b) -> Double.compare(a.salary, b.salary)

// long field
(a, b) -> Long.compare(a.timestamp, b.timestamp)
```

**Mantra:** Java ne jo built-in `compareTo` diya, woh **utility** hai. `Comparator` ka `compare(a,b)` woh **interface ka method** hai jo tu override karta. Naam mile-jule sound karte par **alag jagah, alag kaam.**
