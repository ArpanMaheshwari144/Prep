# ⚖️ Comparable vs Comparator — Visual Revision

---

## 1️⃣ Pehle Problem

```
Imagine students ka list:
   [Arpan (age 25), Suresh (age 30), Mukesh (age 20)]
   
Sort karna hai by age. Java ko kaise pata "age" se compare karein?
   
   Collections.sort(students);   // ??? kis basis pe?
```

---

## 2️⃣ Cricket Analogy

```
Team selection 2 ways:

   COMPARABLE = "Default rule team ke andar"
      Team Captain set karta: "We sort by jersey number"
      Class hi mein likha = natural order
      
   COMPARATOR = "External judge ka custom rule"
      Selector: "Sort by score"
      Auditor: "Sort by age"
      Coach: "Sort by stamina"
      
      Class touch nahi karte
      External rule banao
```

---

## 3️⃣ Comparable (Natural Order)

### Concept
```
Class mein KHUD compareTo() method likho.
Java automatically samjhega "iss class ka default order kya hai".
```

### Code
```java
class Student implements Comparable<Student> {
    String name;
    int age;
    
    @Override
    public int compareTo(Student other) {
        return this.age - other.age;   // sort by age
    }
}

// Usage:
List<Student> students = ...;
Collections.sort(students);    // uses compareTo
```

### compareTo Return Values
```
   this.age - other.age:
   
   NEGATIVE → this BEFORE other  (this < other)
   ZERO     → equal
   POSITIVE → this AFTER other   (this > other)
```

```
Example:
   this.age = 25, other.age = 30
   25 - 30 = -5 (negative) → this first
   
   this.age = 30, other.age = 25
   30 - 25 = +5 (positive) → this later
```

---

## 4️⃣ Comparator (External Rule)

### Concept
```
External class banao jo compare karna janta.
Class touch nahi.
Multiple rules possible.
```

### Code
```java
class AgeComparator implements Comparator<Student> {
    @Override
    public int compare(Student a, Student b) {
        return a.age - b.age;
    }
}

class NameComparator implements Comparator<Student> {
    @Override
    public int compare(Student a, Student b) {
        return a.name.compareTo(b.name);
    }
}

// Usage:
Collections.sort(students, new AgeComparator());
Collections.sort(students, new NameComparator());

// Or lambda (Java 8+):
Collections.sort(students, (a, b) -> a.age - b.age);
```

---

## 5️⃣ Visual Difference

```
   COMPARABLE                COMPARATOR
   ──────────                ───────────
   
   Inside class              Outside class
   ONE compareTo method      MANY rules possible
   "Natural order"           "Custom order"
   
   Modify class needed       Class touch nahi
```

---

## 6️⃣ Side-by-Side Code

### Comparable (inside)
```java
class Student implements Comparable<Student> {
    int age;
    
    public int compareTo(Student o) {     // INSIDE
        return this.age - o.age;
    }
}

Collections.sort(list);   // uses natural order
```

### Comparator (outside, multiple)
```java
class Student {  // no Comparable
    int age;
    double score;
    String name;
}

// 3 different comparators:
Comparator<Student> byAge = (a, b) -> a.age - b.age;
Comparator<Student> byScore = (a, b) -> 
        Double.compare(a.score, b.score);
Comparator<Student> byName = (a, b) -> 
        a.name.compareTo(b.name);

Collections.sort(list, byAge);     // sort by age
Collections.sort(list, byScore);   // sort by score
Collections.sort(list, byName);    // sort by name
```

---

## 7️⃣ Comparator Power — thenComparing

```java
// Multi-criteria sort:
// First by age, ties broken by name

Comparator<Student> combo = 
    Comparator.comparingInt((Student s) -> s.age)
              .thenComparing(s -> s.name);

Collections.sort(list, combo);
```

```
Example:
   [Arpan (25), Suresh (30), Mukesh (25)]
   
   Sort by age:
      Arpan (25), Mukesh (25), Suresh (30)
   
   Tie? Use thenComparing (name):
      Arpan (25), Mukesh (25), Suresh (30)
      ↑ Arpan < Mukesh alphabetically
```

---

## 8️⃣ Reverse Order

```java
// Reverse natural order:
Collections.sort(list, Comparator.reverseOrder());

// Reverse custom:
Comparator<Student> byAgeDesc = 
    Comparator.comparingInt((Student s) -> s.age).reversed();
```

---

## 9️⃣ When To Use Which?

```
COMPARABLE:
   ✅ Class has ONE natural order
   ✅ Numbers, Strings (built-in)
   ✅ Custom class with obvious order
      (e.g., Date, Money — sorted by amount)

COMPARATOR:
   ✅ Multiple sort criteria needed
   ✅ Class is third-party (can't modify)
   ✅ Sort same class differently in different contexts
   ✅ Ad-hoc sorting (one-time)
```

---

## 🔟 Real Java Built-ins

```java
// Numbers — already Comparable
List<Integer> nums = List.of(3, 1, 2);
Collections.sort(nums);   // [1, 2, 3]

// Strings — Comparable
List<String> names = List.of("Zebra", "Apple", "Mango");
Collections.sort(names);   // alphabetical

// Date, BigDecimal, LocalDate — all Comparable
```

---

## 1️⃣1️⃣ Comparable + Comparator Together

```java
class Student implements Comparable<Student> {
    int age;
    String name;
    
    public int compareTo(Student o) {
        return this.age - o.age;   // natural = by age
    }
}

// Default:
Collections.sort(list);            // sorted by age

// Override with comparator:
Collections.sort(list, byName);    // sorted by name
```

```
Natural order = compareTo (default)
External override = pass comparator
   Both can coexist
```

---

## 🎯 Memory Hooks

```
Comparable  = ⭐ class apne aap mein compare-able
              ONE natural order (compareTo)
              modify class

Comparator  = 👨‍⚖️ external judge
              MULTIPLE rules
              don't touch class
              lambda-friendly

compareTo return:
   negative → first
   zero     → equal
   positive → later
```

---

## 1️⃣2️⃣ Common Use Cases

```
✅ Sort employees by salary (Comparator)
✅ Sort products by price then by rating (chain)
✅ Sort dates chronologically (Comparable built-in)
✅ PriorityQueue with custom order (Comparator)
✅ TreeMap with custom key order (Comparator)
```

📚 [← Back to JAVA](../)
