# 🌊 Streams API — Visual Revision

---

## 1️⃣ Concept (Factory Assembly Line)

```
Imagine factory ka conveyor belt:

   📦 → 🔍 Filter → 🔧 Transform → 📊 Pack
   ↑                                    ↓
   raw items                       finished
   
   Each item: filtered → modified → collected
```

```
Stream = data ka assembly line
   Collection (list) → filter → map → collect → result
```

---

## 2️⃣ Loop vs Stream — Visual

### OLD Loop Style
```java
List<Integer> nums = List.of(1, 2, 3, 4, 5);

// Filter evens, double them, sum
List<Integer> evens = new ArrayList<>();
for (int n : nums) {
    if (n % 2 == 0) {
        evens.add(n * 2);
    }
}
int sum = 0;
for (int n : evens) {
    sum += n;
}
// = 12 (2*2 + 4*2)
```

```
Imperative:
   "HOW to do" — manual loops, mutable state
```

### NEW Stream Style
```java
int sum = nums.stream()
              .filter(n -> n % 2 == 0)
              .mapToInt(n -> n * 2)
              .sum();
// = 12
```

```
Declarative:
   "WHAT to do" — clean pipeline
```

---

## 3️⃣ Stream Pipeline (3 Parts)

```
   SOURCE              INTERMEDIATE              TERMINAL
   ──────              ────────────              ────────
   
   list.stream()  →   .filter()           →    .collect()
                       .map()                   .reduce()
                       .sorted()                .forEach()
                       .distinct()              .count()
                       .limit()                 .sum()
   
   ↑                       ↑                       ↑
   Where data            Modify each            Final result
   comes from            (LAZY)                 (eager)
```

---

## 4️⃣ Intermediate Operations (Lazy)

```java
.filter(p -> ...)        // keep matching
.map(f -> ...)           // transform each
.flatMap(f -> ...)       // flatten nested
.sorted()                // sort
.distinct()              // remove dups
.limit(n)                // take first N
.skip(n)                 // skip first N
.peek(c -> ...)          // debug peek
```

```
LAZY:
   Yeh operations chalti NAHI till terminal called
   Pipeline build hota, execute baad mein
```

---

## 5️⃣ Terminal Operations (Eager)

```java
.collect(...)            // collect to List/Set/Map
.toList()                // Java 16+ shortcut
.reduce(...)             // aggregate
.forEach(c -> ...)       // iterate
.count()                 // total count
.sum() / .max() / .min() // numeric ops
.findFirst()             // first matching
.anyMatch(p -> ...)      // any?
.allMatch(p -> ...)      // all?
.noneMatch(p -> ...)     // none?
```

```
EAGER:
   Terminal called = pipeline execute
   Result returned
```

---

## 6️⃣ Common Examples

### Filter + Collect
```java
List<String> names = List.of("Arpan", "Suresh", "Mukesh");

// Names starting with 'S'
List<String> sNames = names.stream()
    .filter(n -> n.startsWith("S"))
    .toList();
// ["Suresh"]
```

### Map (Transform)
```java
List<String> upper = names.stream()
    .map(String::toUpperCase)
    .toList();
// ["ARPAN", "SURESH", "MUKESH"]
```

### Filter + Map + Collect
```java
List<String> result = names.stream()
    .filter(n -> n.length() > 5)
    .map(String::toUpperCase)
    .toList();
// ["SURESH", "MUKESH"] (both > 5 chars, uppercased)
```

---

## 7️⃣ Reduce (Aggregate)

```java
List<Integer> nums = List.of(1, 2, 3, 4, 5);

// Sum
int sum = nums.stream()
              .reduce(0, (a, b) -> a + b);
// = 15

// Max
int max = nums.stream()
              .reduce(Integer.MIN_VALUE, Integer::max);
// = 5

// Concatenate strings
String joined = List.of("a", "b", "c").stream()
                    .reduce("", String::concat);
// = "abc"
```

```
reduce = "accumulate into one value"
   identity + accumulator function
```

---

## 8️⃣ Collect with Collectors

```java
// To List
list.stream().collect(Collectors.toList());

// To Set
list.stream().collect(Collectors.toSet());

// To Map
list.stream()
    .collect(Collectors.toMap(
        Person::getId,    // key
        Person::getName   // value
    ));

// Joining strings
list.stream()
    .collect(Collectors.joining(", "));
// "Arpan, Suresh, Mukesh"

// Group by
Map<Integer, List<Person>> byAge = 
    people.stream()
          .collect(Collectors.groupingBy(Person::getAge));

// Count
long count = list.stream()
                 .filter(x -> x > 10)
                 .count();
```

---

## 9️⃣ Lazy Evaluation (Important!)

```java
List<Integer> nums = List.of(1, 2, 3, 4, 5);

Stream<Integer> pipeline = nums.stream()
    .filter(n -> {
        System.out.println("Filter: " + n);
        return n % 2 == 0;
    })
    .map(n -> {
        System.out.println("Map: " + n);
        return n * 10;
    });

// NOTHING printed yet — pipeline only built

pipeline.toList();    // NOW it runs
// Filter: 1
// Filter: 2
// Map: 2
// Filter: 3
// Filter: 4
// Map: 4
// Filter: 5
```

```
KEY:
   Intermediate ops = LAZY (deferred)
   Terminal op = TRIGGER (executes pipeline)
```

---

## 🔟 Parallel Streams (Speed Boost)

```java
// Sequential (default)
list.stream()
    .filter(...)
    .map(...)
    .toList();

// Parallel
list.parallelStream()
    .filter(...)
    .map(...)
    .toList();
```

```
parallelStream:
   Internally uses ForkJoinPool
   Splits work across CPU cores
   = Faster for LARGE data + CPU-intensive ops
   
WARNINGS:
   ❌ Don't use for small lists (overhead)
   ❌ Don't use with shared mutable state
   ❌ Don't assume order (use forEachOrdered)
```

---

## 1️⃣1️⃣ Common Trap

```java
// ❌ Can't reuse stream
Stream<Integer> s = list.stream();
s.toList();         // works
s.toList();         // ❌ IllegalStateException — stream consumed

// ✅ Create new stream each time:
list.stream().toList();   // 1st use
list.stream().toList();   // 2nd use (new stream)
```

---

## 1️⃣2️⃣ flatMap (Nested → Flat)

```java
List<List<Integer>> nested = List.of(
    List.of(1, 2),
    List.of(3, 4),
    List.of(5, 6)
);

// map returns Stream<List<Integer>>
// flatMap flattens to Stream<Integer>
List<Integer> flat = nested.stream()
    .flatMap(List::stream)
    .toList();
// [1, 2, 3, 4, 5, 6]
```

```
flatMap = "flatten nested streams into one"
```

---

## 🎯 Memory Hooks

```
Stream = 🏭 assembly line for collections

Pipeline:
   source → intermediate → intermediate → terminal
   
Intermediate (LAZY):
   filter, map, sorted, distinct, limit
   
Terminal (EAGER):
   collect, reduce, forEach, count, sum

Parallel = .parallelStream() (CPU split)

flatMap = nested → flat

Stream = one-time use (can't reuse)
```

📚 [← Back to JAVA](../)
