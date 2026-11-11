# Stream methods cheatsheet

Iski practice `01_JAVA/07_STREAM_API_WRITE` folder me hai (Q1 se Q17, aur REDO).

Padhne ka tareeka: ek line jodo, neeche dekho belt pe kya bacha. Upar se neeche padho.

---

## Udaharan 1: numbers (`int[]` se shuru)

**Sawaal:** `{1,2,3,4,5,6}` me se even ka square, List me.

**Line 1**
```java
Arrays.stream(nums)
```
```
[1, 2, 3, 4, 5, 6]
```
Saare number belt pe aa gaye.

**+ Line 2**
```java
Arrays.stream(nums)
      .filter(n -> n % 2 == 0)
```
```
[2, 4, 6]
```
1, 3, 5 gaye. "sirf even" = **filter**.

**+ Line 3**
```java
Arrays.stream(nums)
      .filter(n -> n % 2 == 0)
      .map(n -> n * n)
```
```
[4, 16, 36]
```
Har ek ka square ban gaya. "har ek ka X" = **map**.

**+ Line 4, bina boxed ke List maangi**
```java
      .map(n -> n * n)
      .collect(Collectors.toList());
```
```
error: method collect in interface IntStream cannot be applied
```

**+ boxed laga ke**
```java
      .map(n -> n * n)
      .boxed()
      .collect(Collectors.toList());
```
```
[4, 16, 36]
```
`int[]` se shuru kiya aur ant me List chahiye, to **boxed** lagta hai.

**Ant badal ke dekh:** List ki jagah sabka jod maanga.
```java
      .map(n -> n * n)
      .sum();
```
```
56
```
Yahan boxed nahi, collect nahi. Ant me kya chahiye, wahi aakhri line.

---

## Udaharan 2: words (List se shuru)

**Sawaal:** `[aam, kela, anaar, kela, seb]` me se jo 3 se lambe hain, unhe CAPITAL me karo.

**Line 1**
```java
words.stream()
```
```
[aam, kela, anaar, kela, seb]
```
List hai, to seedha `.stream()`. Arrays nahi lagta.

**+ Line 2**
```java
      .filter(s -> s.length() > 3)
```
```
[kela, anaar, kela]
```
aam aur seb gaye.

**+ Line 3**
```java
      .map(String::toUpperCase)
```
```
[KELA, ANAAR, KELA]
```
Har ek capital ho gaya.

**Ant badal ke dekh.** Upar ki teen line wahi, sirf aakhri line badli:
```
.collect(Collectors.toList())                -> [KELA, ANAAR, KELA]    list do
.count()                                     -> 3                      kitne
.collect(Collectors.joining(", "))           -> KELA, ANAAR, KELA      ek line me jodo
.collect(Collectors.toSet())                 -> [KELA, ANAAR]          dohra hatao
.collect(groupingBy(s -> s, counting()))     -> {ANAAR=1, KELA=2}      har ek kitni baar
```

**Boxed kahan gaya?** Words pehle se String object hain, kachche int nahi. Boxed sirf `int[]` se shuru karne pe.

**Ek aur ant:** sirf even ke square ka jod (List se aaye to `mapToInt`, kyunki `sum()` sirf int ki belt pe milta hai):
```java
nums.stream().filter(x -> x % 2 == 0).mapToInt(x -> x * x).sum();    // [1,2,3,4] -> 20
```

---

## Maang: "sabse bade 3"

`n = [5, 1, 9, 3, 7, 2, 9]`

```java
n.stream()                                 -> [5, 1, 9, 3, 7, 2, 9]
  .sorted(Comparator.reverseOrder())       -> [9, 9, 7, 5, 3, 2, 1]     bada pehle
  .limit(3)                                -> [9, 9, 7]                 pehle 3 rakho
  .collect(Collectors.toList())
```

## Maang: "doosra sabse bada" (dohra ek hi gina jaaye)

```java
n.stream()                                 -> [5, 1, 9, 3, 7, 2, 9]
  .distinct()                              -> [5, 1, 9, 3, 7, 2]        dohra 9 gaya
  .sorted(Comparator.reverseOrder())       -> [9, 7, 5, 3, 2, 1]
  .skip(1)                                 -> [7, 5, 3, 2, 1]           sabse bada chhoda
  .findFirst()                             -> Optional[7]               dabbe me 7
  .orElse(-1)                              -> 7                         dabbe se nikala
```

- `findFirst()` seedha number nahi deta, `Optional` (dabba) deta hai, kyunki ho sakta hai kuch bacha hi na ho.
- `.orElse(-1)` = dabba khaali ho to -1 do. Khaali list pe chala ke dekha: `-1`.
- Salary wala sawaal ho to shuru me `.map(e -> e.salary)` lagao, baaki same.

## Maang: "kitne ALAG-ALAG"

```java
n.stream().distinct().count()              -> 6
```

## Maang: "har ek kitni baar" + "sabse zyada baar kaun"

`w = [a, b, a, c, a, b]`

```java
Map<String, Long> f = w.stream()
  .collect(groupingBy(x -> x, counting()))   -> {a=3, b=2, c=1}         har ek kitni baar
```

Ab sabse zyada wala nikaalna hai. Map pe seedha stream nahi chalta, pehle entries (key=value ki jodiyan) belt pe:

```java
f.entrySet().stream()                      -> [a=3, b=2, c=1]
  .max(Map.Entry.comparingByValue())       -> Optional[a=3]             value pe sabse bada
  .get().getKey()                          -> a                         dabbe se nikaal ke key
```

## Maang: "X ke hisaab se baanto" (groupingBy)

`emps = (arpan,eng,M,100) (bob,sales,M,200) (cara,eng,F,150) (dev,sales,M,50)`

Pehla khaana = kis cheez pe baanto. Doosra khaana = har dabbe me kya rakho. Bas doosra khaana badalte raho:

```java
groupingBy(Emp::dept)                                   -> {sales=[bob ka poora Emp, dev ka], eng=[arpan, cara ke poore Emp]}
groupingBy(Emp::dept, counting())                       -> {sales=2, eng=2}                 kitne
groupingBy(Emp::dept, mapping(Emp::name, toList()))     -> {sales=[bob, dev], eng=[arpan, cara]}   sirf naam
groupingBy(Emp::dept, summingInt(Emp::salary))          -> {sales=250, eng=250}             kitne ka
```

Pehle akshar pe: `groupingBy(w -> w.charAt(0))`.

**Do level (dept ke andar gender):** doosre khaane me phir se groupingBy:

```java
groupingBy(Emp::dept, groupingBy(Emp::gender, mapping(Emp::name, toList())))
                                                        -> {sales={M=[bob, dev]}, eng={F=[cara], M=[arpan]}}
```

**Data string me ho** (`"arpan,eng"`): pehle tod ke dekho kya milta hai:

```java
"arpan,eng".split(",")                     -> [arpan, eng]              [0] = naam, [1] = dept
groupingBy(s -> s.split(",")[1], mapping(s -> s.split(",")[0], toList()))
                                           -> {sales=[bob], eng=[arpan, cara]}
```

## Maang: "do hisse, haan / na" (partitioningBy)

```java
[1..6].stream().collect(partitioningBy(x -> x % 2 == 0))   -> {false=[1, 3, 5], true=[2, 4, 6]}
```

groupingBy jaisa, bas sirf do dabbe: true aur false.

## Maang: "word -> uski length" (toMap, har key ki EK value)

```java
[jp, java, arpan].stream().collect(toMap(w -> w, w -> w.length()))   -> {arpan=5, java=4, jp=2}
```

Duplicate key aayi to crash (neeche prod trap). Ek key ki kai values = groupingBy.

## Maang: "list ke andar list ko seedha karo" (flatMap)

```java
ll.stream()                                -> [[1, 2], [3, 4], [5]]     belt pe 3 list
  .flatMap(l -> l.stream())                -> [1, 2, 3, 4, 5]           har list khol ke number belt pe
```

map hota to har list ek hi cheez rehti. flatMap har list ko khol deta hai.

## Maang: "sabka guna" (reduce)

```java
[1, 2, 3, 4].stream().reduce(1, (i, j) -> i * j)    -> 24
```

1 se shuru, har number se guna karte jao. Jod ke liye 0 se shuru aur `i + j` (ya seedha `sum()`).

## Maang: "pehle dept, usme salary bada pehle" (do chaabi se sort)

```java
emps.stream()
  .sorted(Comparator.comparing(Emp::dept))                     -> [arpan, cara, bob, dev]   sirf dept
  .sorted(Comparator.comparing(Emp::dept)
          .thenComparing(Comparator.comparingInt(Emp::salary).reversed()))
                                                               -> [cara, arpan, bob, dev]   eng me cara 150 upar
  .map(e -> e.name).collect(toList())
```

- `.reversed()` sirf salary wale pe lagao, poori chain pe nahi (Collections 09 wala trap).
- Lambda likho (`e -> e.dept`) to chain me type batana padta hai: `comparing((Emp e) -> e.dept)`. `Emp::dept` likho
  to nahi.

---

## Mart ka rack: groupingBy ka doosra khaana

Mart me 2 rack hain. Doodh wale rack pe ghee 550 aur paneer 300 hain, anaaj wale pe daal 140 aur aata 320. Mausi boli:
"har rack ka hisaab do."

`groupingBy` ka pehla argument batata hai ki dabbe kis cheez pe banenge. Doosra argument batata hai ki har dabbe ke andar
kya rakhna hai. Slot ek hi hai, bas usme collector badalte raho:

```java
groupingBy(Item::rack, counting())                    // kitne
groupingBy(Item::rack, mapping(Item::name, toList())) // kaun kaun
groupingBy(Item::rack, summingInt(Item::price))       // kitne ka
```

```
{anaaj=2, doodh=2}
{anaaj=[daal, aata], doodh=[ghee, paneer]}
{anaaj=460, doodh=850}
sum: 1310          <- mapToInt(Item::price).sum()
```

Do cheezein dhyaan me:
- Naam sirf dabbe ke andar nikalo (`mapping`). Collect se pehle `.map(name)` lagaya to stream me sirf String bachegi aur
  rack kho jaayega, phir group kis cheez pe karoge.
- `.sum()` sirf `IntStream` pe milta hai, isliye pehle `mapToInt` lagao.

---

## Saare method ek nazar me (naam bhoolo to docs / net pe dekh lo, normal hai)

```
BADLO (beech):  filter · map · flatMap · distinct · sorted · limit / skip
AGGREGATE (ant): count · sum / max · reduce · anyMatch · findFirst
COLLECT (ant):   toList · toMap · groupingBy · partitioningBy · joining
```

---

## Prod wala trap: toMap me duplicate key

```java
l.stream().collect(toMap(Item::rack, Item::name));
```

```
Duplicate key anaaj (attempted merging values daal and aata)
```

Test data me har rack pe ek hi dabba tha, isliye sab chal gaya. Prod me usi rack pe doosra dabba aate hi crash. Isliye
`toMap` me teesra argument (merge) do:

```
toMap(Item::rack, Item::name, (x, y) -> x + ", " + y)   ->   {anaaj=daal, aata, doodh=ghee, paneer}
```

Ek key ki kai values chahiye to `toMap` ki jagah `groupingBy`.

---

## Loop ya stream

Kaam seedha ho (filter, map, group, ginti) to stream chhoti aur saaf. Logic bhaari ho (beech me `break`, kai if-else,
checked exception, ya breakpoint lagana ho) to loop behtar. Jo padhne me saaf lage wahi likho.

## Tera hands-on

`07_STREAM_API_WRITE` me Q6 (`counting`), Q13 (`mapping`) aur Q14 (`summingInt`) tune khud likhe hain. Mart wale teen
collector wahi teen sawaal hain.
