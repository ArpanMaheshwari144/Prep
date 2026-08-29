# Stream API — Methods Cheatsheet

> Poora toolkit ek jagah. Practice = 01_JAVA/09_WRITE_PRACTICE (Q1-Q16 + REDO).
> **CORE mantra:** stream = for + if. Sab methods 3 category: **BADLO → AGGREGATE → COLLECT**. Naam alag, kaam wahi 3.

---

## 1. INTERMEDIATE ops (Stream lautate, LAZY — gate lagate, chalte nahi jab tak terminal na aaye)

| method | kya karta | connect |
|--|--|--|
| `filter(pred)` | chhaanta (rakh / hata) | = `if` |
| `map(fn)` | har element BADLO (1 → 1) | transform |
| `mapToInt / mapToLong / mapToDouble` | Stream<Integer> → **IntStream** (aggregate ke liye) | boxed ka ulta |
| `boxed()` | IntStream → **Stream<Integer>** (primitive → object) | List<Integer> ke liye |
| `flatMap(fn)` | nested (list-of-list) → **flat** stream | 2 nested loop = 1 |
| `distinct()` | duplicates hatao | Set jaisa |
| `sorted() / sorted(Comparator)` | sort (natural / custom) | Comparator chaining |
| `limit(n) / skip(n)` | pehle-n lo / pehle-n chhodo | top-K / pagination |
| `peek(fn)` | har element DEKHO (debug, side-effect) | print-debug |

---

## 2. TERMINAL ops (result lautate, chain CHALATE — inke bina kuch nahi hota)

| method | kya | connect |
|--|--|--|
| `collect(Collectors…)` | List / Set / Map banao | sabse common |
| `forEach(fn)` | har element pe action (print/save) | loop-body |
| `count()` | kitne (long) | size |
| `sum() / max() / min() / average()` | IntStream pe aggregate | jod / bada / chhota |
| `reduce(identity, (acc,x)->…)` | custom fold (product etc) | `acc = acc OP x` |
| `anyMatch / allMatch / noneMatch(pred)` | **boolean** (koi / sab / koi-nahi match?) | early-exit |
| `findFirst() / findAny()` | pehla match → **Optional** | search |
| `toArray()` | array me | — |

---

## 3. COLLECTORS (`collect()` ke andar)

| collector | kya | connect |
|--|--|--|
| `toList() / toSet()` | list / set | — |
| `toMap(keyFn, valFn[, mergeFn])` | Map (dup-key → mergeFn) | ek key ek value |
| `groupingBy(keyFn[, downstream])` | key → buckets | HashMap<k, List> |
| `partitioningBy(pred)` | true / false 2 bucket (hamesha dono keys) | boolean split |
| `counting()` | bucket me count (Long) | freq |
| `summingInt(fn) / averagingDouble(fn)` | bucket me sum / avg | aggregate |
| `mapping(fn, downstream)` | bucket me transform + collect | value badlo |
| `joining(", ")` | strings ko JODO (CSV) | String.join |

**Nested downstream:** `groupingBy(k1, groupingBy(k2, mapping(fn, toList())))` = 2-level buckets.

---

## 4. Bonus — number range (for-loop ka stream roop)
- `IntStream.range(0, n)` → 0..n-1
- `IntStream.rangeClosed(1, n)` → 1..n

---

## 5. TYPE INFERENCE (yaad rakh)
- **Stream ke andar** (`map/filter`) → lambda param ka type **stream se aata** → likhna nahi (`e -> e.name`).
- **Comparator/collector jo ALAG + chained ban raha** → context nahi → type khud likho (`Comparator.comparing((Emp e) -> e.dept)`).
- Rule: context (stream) hai → na likho; standalone+chained → likho. (= C++ `auto`)

## 6. boxed vs mapToInt (confuse mat)
```
int[]  → Arrays.stream → IntStream  --.boxed()-->  Stream<Integer>   (List ke liye)
Stream<Integer>  --.mapToInt(x->x)-->  IntStream                      (sum/max ke liye)
```
IntStream = primitive (fast, .sum() milta) · Stream<Integer> = object (collect/List milta).

---

## Power Phrase

> **"Stream = for + if. 3 category: BADLO (filter/map/flatMap) → AGGREGATE (sum/reduce/count) → COLLECT (toList/groupingBy). Intermediate = lazy (gate), terminal = chalao. Fast/parallel bonus = parallelStream + lazy short-circuit; baaki readability."**

---

## Memory Hook

```
BADLO:     filter(if) · map(1→1) · flatMap(nested→flat) · mapToInt/boxed · distinct · sorted · limit/skip
AGGREGATE: count · sum/max/min · reduce · anyMatch/allMatch · findFirst
COLLECT:   toList/toSet · toMap · groupingBy(+counting/mapping/summingInt) · partitioningBy · joining

lazy (intermediate) → gate lagao;  terminal → belt chalao (warna kuch nahi hota)
```
