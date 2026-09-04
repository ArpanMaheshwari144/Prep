# thenComparing() — Chained Sorting

> **V90 — Collections: Topic 31**

---

## STORY — Tie-Breaker

→ Employees ki list — **age se sort** karna hai
→ 2 panelists ki **age same** ho gayi
→ Manager bola: pehle **age**, tie ho toh **name** se
→ Comparator ke andar `if-else` lagana padega? **Nahi — `thenComparing()` hai!**
→ `comparing()` = pehla rule. `thenComparing()` = **tiebreaker**. Jitne chahiye utne lagao

---

## WHY — `thenComparing()` Kyu?

→ Ek `Comparator` sirf **EK rule** deta hai
→ Multiple rules chahiye → **chain karo**
→ `comparing()` = primary rule
→ `thenComparing()` = secondary, tertiary, jitne chahiye

---

## Code

### Pehle age, tie ho toh name
```java
list.sort(
    Comparator.comparing((Panelist p) -> p.age)
              .thenComparing(p -> p.name)
);
```

### Teen rules — age, then name, then salary
```java
list.sort(
    Comparator.comparing((Panelist p) -> p.age)
              .thenComparing(p -> p.name)
              .thenComparing(p -> p.salary)
);
```

---

## TRAP — `&&` Use Karna

> **`&&` operator `int` pe kaam nahi karta!**

```java
// GALAT
list.sort((a, b) -> Integer.compare(a.age, b.age) && a.name.compareTo(b.name));
// compile error — && Boolean expects, comparison returns int

// SAHI
list.sort(
    Comparator.comparing((Panelist p) -> p.age)
              .thenComparing(p -> p.name)
);
```

---

## Logic — `thenComparing` Kab Chalta?

```
comparing(age)      ─── Pehla compare
       │
       ▼
   Result == 0?
   ╱           ╲
 NO            YES
  │             │
  ▼             ▼
Use this     thenComparing(name)
result       (tiebreaker chala)
```

> **Jab tak previous comparison `0` (equal) na de, agla rule chalega hi nahi.**

---

## POWER PHRASE

> *"`Comparator.comparing()` sets the primary sort rule. `thenComparing()` adds a tiebreaker applied only when the previous comparison returns zero. Chain as many as needed."*

> **Yaad rakh:**
> `comparing()` = pehla rule
> `thenComparing()` = tiebreaker
> `&&` = compile error — chain karo
