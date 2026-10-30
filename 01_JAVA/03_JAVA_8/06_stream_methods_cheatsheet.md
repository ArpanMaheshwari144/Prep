# Stream methods cheatsheet

> Practice = 01_JAVA/07_STREAM_API_WRITE (Q1-Q17 + REDO).

## Kahani

HR bola: "har department ki total salary batao." Data ye hai:

```
(arpan, eng, 100)  (bob, sales, 200)  (cara, eng, 150)  (dev, sales, 50)
```

Loop se karo to `map[dept] += salary`. Stream me ye kaam ek line ka hai:

```java
emps.stream().collect(groupingBy(e -> e.dept, summingInt(e -> e.salary)));
// {eng=250, sales=250}
```

## Tasveer

`groupingBy` ka pehla argument batata hai ki **dabbe kis cheez pe banenge**. Doosra argument batata hai ki **har dabbe ke andar kya banega**. Slot ek hi hai, bas usme collector badalte raho:

```
groupingBy(dept)                          -> eng=[Emp, Emp]       poora object (default)
groupingBy(dept, counting())              -> eng=2                ginti
groupingBy(dept, mapping(name, toList())) -> eng=[arpan, cara]    sirf naam
groupingBy(dept, summingInt(salary))      -> eng=250              jod
```

## 4 khaas baat

1. **Saare methods 3 dabbon me aate hain, isliye kaam pata ho to naam mil jaata hai:**
   ```
   BADLO:     filter · map · flatMap · distinct · sorted · limit / skip
   AGGREGATE: count · sum / max · reduce · anyMatch · findFirst
   COLLECT:   toList · toMap · groupingBy · partitioningBy · joining
   ```
2. **Naam sirf dabbe ke andar nikalo (`mapping`), collect se pehle `.map(name)` mat lagao.** Pehle hi naam bana diya to stream me sirf String bachegi, dept kho jaayega. Phir group kis cheez pe karoge?
3. **`IntStream` aur `Stream<Integer>` alag cheezein hain.** `.sum()` sirf `IntStream` pe milta hai, wahan pahunchne ke liye `mapToInt` lagao. List banani ho to `.boxed()` se wapas `Stream<Integer>` banao.
4. **`reversed()` jahan lagaoge, wahin tak ka sab ulta hoga.** `thenComparing(comparingInt(salary).reversed())` me sirf salary ulti hoti hai. Chain ke end me `.reversed()` lagao to poori chain ulti ho jaati hai, dept bhi Z-A.

## Prod wala trap

```java
emps.stream().collect(toMap(e -> e.dept, e -> e.name));
// eng do baar aaya -> IllegalStateException: Duplicate key eng
```

Test data me har dept ek hi baar tha, isliye sab chal gaya. Prod me ek dept me doosra banda aate hi crash. Isliye `toMap` me teesra argument (merge) do, jaise `(a, b) -> a + ", " + b`. Ek key ki kai values chahiye to `toMap` ki jagah `groupingBy` lagao.

## Loop ya stream

Seedha kaam (filter, map, group, ginti) ho to stream chhoti padti hai. Logic bhaari ho (beech me `break`, kai if-else, checked exception, debug me breakpoint chahiye) to loop behtar hai. Dono sahi hain, jo padhne me saaf lage wahi likho.

## Tera hands-on

`07_STREAM_API_WRITE` me Q6 (`counting`), Q13 (`mapping`), Q14 (`summingInt`) aur Q16 (`reversed`) tune khud likhe hain. Upar ki tasveer wahi chaar sawaal hain.

## Interview line

```
groupingBy takes a key function and an optional downstream collector that decides
what each group holds: counting, mapping, summingInt and so on.
toMap throws on duplicate keys unless you pass a merge function.
mapToInt gives an IntStream for sum and max, boxed converts back to objects.
```
