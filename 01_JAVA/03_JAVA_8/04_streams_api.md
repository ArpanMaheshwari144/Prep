# Streams API — Pipeline Banao, Loop Hatao

> **V90 — Java 8: Topic 37**

---

## STORY — Active Patients ki List

→ Manager ne bola — *"saare panelists mein se sirf **active wale** dhoondho, unke **naam nikalo, alphabetical sort karo, ek list bana do**."*
→ Purana style mein for loop, `if` condition, naya list, `Collections.sort()`
→ — **5 lines, 3 alag operations, loop mein sab ghusa hua**
→ Streams ne socha — **ye sab operations chain karo. Ek pipeline banao.**
→ Data ek side se jaaye, operations beech mein hon, **result doosri side se aaye**

---

## WHY — Streams Kya Solve Karta?

→ Loop mein **filter + transform + sort** sab mix ho jaate
→ Streams = **declarative pipeline** — jo karna chahiye wo bolo, kaise nahi
→ **`filter` → `map` → `sorted` → `collect`** — ek chain, readable

---

## Streams Ka Core Flow (Yaad Rakh!)

```
1. SOURCE        →  data lao (findAll, list, set, anything)
2. .stream()     →  pipeline mein daalo
3. INTERMEDIATE  →  filter, map, sorted, limit, distinct, skip...
                    (jitne chahiye, chain karo — LAZY)
4. TERMINAL      →  collect / forEach / count / sum / reduce
                    (yahin actual EXECUTE hota — TRIGGER)
```

**LAZY = bina terminal kuch nahi hota.** Terminal aate hi pura pipeline run.

### Real Example — Sab Operations Saath

```java
repository.findAll()                          // 1. SOURCE
    .stream()                                  // 2. pipeline
    .filter(a -> a.getBalance() > 10000)       // 3a. condition
    .sorted(Comparator.comparing(Account::getBalance).reversed())  // 3b. sort
    .limit(5)                                  // 3c. top 5
    .map(Account::getHolderName)               // 3d. transform
    .collect(Collectors.toList());             // 4. TERMINAL — list pakdo
```

**Kahani:** "saare accounts mein se balance > 10k wale chuno → balance descending sort → top 5 → unke naam → list bana"

```
   ┌─────────┐    ┌─────────────────────────────┐    ┌──────────┐
   │ SOURCE  │ → │  intermediate ops (LAZY)    │ → │ TERMINAL │
   │ (data)  │    │  filter • map • sorted      │    │ (TRIGGER)│
   │         │    │  limit • skip • distinct    │    │ collect  │
   │         │    │                              │    │ count    │
   └─────────┘    └─────────────────────────────┘    └──────────┘
```

---

## Pipeline Pattern

```
SOURCE              INTERMEDIATE (LAZY)            TERMINAL (TRIGGER)
                    ┌─────────────────────────┐
List<User>   ──►   │ .filter()  .map()        │ ──►  .collect()
[source]            │ .sorted()                │      toList()
                    └─────────────────────────┘

Source > filter > map > sorted (LAZY) > collect (TERMINAL = trigger)
```

**LAZY:** filter/map/sorted **kuch nahi karte** jab tak `collect()` na call ho. Terminal = trigger.

---

## Old vs New

```java
// Purana style — 5 lines
List<String> result = new ArrayList<>();
for (Panelist p : panelists) {
    if (p.isActive()) {
        result.add(p.getName());
    }
}
Collections.sort(result);

// Streams — ek pipeline
List<String> result = panelists.stream()
    .filter(p -> p.isActive())          // sirf active
    .map(p -> p.getName())              // naam nikalo
    .sorted()                            // alphabetical
    .collect(Collectors.toList());       // list banao
```

---

## Important Operations — PFCS Connection

| Operation | Kya leta | Kya karta |
|-----------|----------|-----------|
| `filter()` | **Predicate** (boolean) | Chhaan ke nikalo |
| `map()` | **Function** (transform) | Ek type se doosra |
| `sorted()` | Comparable / Comparator | Sort karo |
| `forEach()` | **Consumer** (void) | Har ek pe action |
| `collect()` | Collector | Final result pakdo |
| `reduce()` | BiFunction | Sab ko ek value mein milao |
| `distinct()` | — | Duplicates hatao |
| `limit(n)` / `skip(n)` | — | Pagination |
| `count()` | — | Kitne hain |

---

## Common Patterns

### filter + map + collect
```java
panelists.stream()
    .filter(p -> p.isActive())
    .map(p -> p.getName())
    .collect(Collectors.toList());
```

### sorted with custom Comparator
```java
panelists.stream()
    .sorted(Comparator.comparing(Panelist::getAge).reversed())
    .collect(Collectors.toList());
```

### reduce — sab milao
```java
List<Integer> nums = List.of(10, 20, 30);
int total = nums.stream()
    .reduce(0, (a, b) -> a + b);    // 0+10=10, 10+20=30, 30+30=60
```

### Top-5 active panelists
```java
panelists.stream()
    .filter(p -> p.isActive())
    .map(p -> p.getName())
    .sorted()
    .limit(5)
    .collect(Collectors.toList());
```

---

## Collectors — Result Format

```java
.collect(Collectors.toList())                       // List
.collect(Collectors.toSet())                        // Set (duplicates gone)
.collect(Collectors.joining(", "))                  // "Arpan, Rahul, Priya"
.collect(Collectors.toMap(p -> p.getId(), p -> p.getName()))   // Map
.collect(Collectors.groupingBy(p -> p.getCity()))   // Map<City, List<P>>
```

---

## TRAP 1 — Stream Ek Baar Use

> **Stream ek baar use ho sakta — dobara `.filter()` ya `.map()` call kiya toh `IllegalStateException`. Naya stream banao.**

```java
Stream<Panelist> s = panelists.stream();
s.filter(p -> p.isActive()).count();    // first use
s.map(p -> p.getName());                // IllegalStateException — already used
```

## TRAP 2 — Lazy Operations

> **Intermediate operations (filter, map, sorted) LAZY hain — jab tak terminal (`collect`, `forEach`, `count`) na aaye, kuch nahi hota.**

```java
panelists.stream().filter(p -> {
    System.out.println("filter called");
    return true;
});
// kuch print nahi hoga — koi terminal nahi
```

---

## POWER PHRASE

> *"Streams API provides a declarative pipeline for data processing — filter, map, sort, collect in a chain. Intermediate operations are lazy and only execute when a terminal operation like `collect()` or `forEach()` is called. Collectors determine the output format — `toList()`, `toSet()`, `toMap()`, `groupingBy()`, or `joining()`."*

> **Yaad rakh:**
> filter = chhaan, map = transform, sorted = sort, forEach = action,
> reduce = combine, distinct = unique, limit/skip = pagination, collect = result pakdo
> **Stream ek baar use, dobara nahi**
> **Intermediate = LAZY, Terminal = TRIGGER**
