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
